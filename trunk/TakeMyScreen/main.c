#include <swilib.h>
#include <unistd.h>
#include <conf_loader.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <png.h>
#include <strings.h>
#include <de/drawing.h>
#include <de/pixtypes.h>

unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;
int my_csm_id=0;

typedef struct
{
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;



extern volatile const char save_to[128];
extern volatile const int key_hot;
extern volatile const unsigned int islong;
extern volatile const int format;
extern volatile const unsigned int quality;
extern volatile const int notification;
extern volatile const unsigned int is_idle_save;
extern char * successed_config_filename;

extern int save_png_to_file (image_t *bitmap, const char *path);
extern int save_jpeg_to_file(image_t *img, char* filename, int quality);

/*------------------------------------------------------------------------*/
/*------------------------------ TakeMyScreen ----------------------------*/
/*------------------------------------------------------------------------*/
image_t img;
char buf[256] = {0};
TDate td;
TTime tt;
static GBSTMR tmr;


void setToBufWay()
{
    GetDateTime(&td, &tt);
    sprintf(buf, "%s%02d%02d%d_%02d%02d%02d.%s", save_to, td.day, td.month, td.year,
                                  tt.hour, tt.min, tt.sec, format?"jpg":"png");
}


void notifi()
{
    switch(notification)
    {
        case 0:
            PlaySound(100, MMI_CEPID, 0, 0x34, 0);
            break;

        case 1:
            ShowMSG(1, (int)"It`s taked!");
            break;

        case 2:
        {
            void stop_vibro(GBSTMR *g)
            {
                SetVibration(0);
            }

            SetVibration(80);
            GBS_StartTimerProc(&tmr, 40, stop_vibro);
            break;
        }

        case 3:
        {
            void restore_light(GBSTMR *g)
            {
                #ifndef NEWSGOLD
                SetIllumination(0, 1, g->param4, 50);
                #else
                SetIllumination(1, 1, g->param4, 50);
                #endif
            }

            tmr.param4 = *(GetIlluminationDataTable()+3);

            #ifndef NEWSGOLD
            SetIllumination(0, 1, 0, 50);
            #else
            SetIllumination(1, 1, 0, 50);
            #endif

            GBS_StartTimerProc(&tmr, 100, restore_light);
            break;
        }
    }
}


void SavePngFile(image_t *_img)
{
    setToBufWay();
    save_png_to_file(_img, buf);
    free_image(_img);
    notifi();
}


void SaveJpegFile(image_t *_img)
{
    setToBufWay();
    save_jpeg_to_file(_img, buf, quality);
    free_image(_img);
    notifi();
}


int up = 1;
int keyhook(int key, int msg)
{
    if( msg == KEY_UP ) up = 1;
    if( msg == LONG_PRESS && islong && up )
    {
        key_p:
        if( key == key_hot )
        {
            up = 0;
            PlaySound(20, MMI_CEPID, 0, 0x30, 0);
            switch(format)
            {
                case 0:
                {
                    image_t *_img = image_fork(&img);
                    if(is_idle_save) SUBPROC(SavePngFile, _img);
                    else             SavePngFile(_img);
                    break;
                }

                case 1:
                {
                    image_t *_img = image_fork(&img);
                    if(is_idle_save) SUBPROC(SaveJpegFile, _img);
                    else             SaveJpegFile(_img);
                    break;
                }

                default:
                {
                    image_t *_img = image_fork(&img);
                    if(is_idle_save) SUBPROC(SavePngFile, _img);
                    else             SavePngFile(_img);
                    break;
                }
            }
        }
    }
    else if( msg == KEY_DOWN && !islong && up )
    {
        goto key_p;
    }


    return KEYHOOK_NEXT;
}


static void maincsm_oncreate(CSM_RAM *data)
{
    img.w = ScreenW();
    img.h = ScreenH();
    img.bpnum = 16;
    img.bitmap = RamScreenBuffer();


    AddKeybMsgHook((void*)keyhook);
}


void ElfKiller(void)
{
    RemoveKeybMsgHook((void*)keyhook);
    kill_elf();
}


static void maincsm_onclose(CSM_RAM *csm)
{
    ElfKiller();
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    MAIN_CSM *csm=(MAIN_CSM*)data;
    if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
    {
        csm->csm.state=-3;
    }

    if(msg->msg == MSG_RECONFIGURE_REQ)
    {
        if (strncasecmp(successed_config_filename, (char *)msg->data0, strlen(successed_config_filename) ) == 0 )
        {
            InitConfig();
        }
    }

    return(1);
}


static const struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
} MAINCSM =
{
    {
        maincsm_onmessage,
        maincsm_oncreate,
#ifdef NEWSGOLD
        0,
        0,
        0,
        0,
#endif
        maincsm_onclose,
        sizeof(MAIN_CSM),
        1,
        &minus11
    },
    {
        maincsm_name_body,
        NAMECSM_MAGIC1,
        NAMECSM_MAGIC2,
        0x0,
        139,
        0
    }
};


void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"TakeMyScreen");
}


void _unsetenv()
{
    unsetenv("TakeMyScreen");
}


int main(char *exename, char *fname)
{
    const char *env = getenv("TakeMyScreen");

    if(env) {
        ShowMSG(1, (int)"Уже запущен");
        kill_elf();
        return 0;
    }

    setenv("TakeMyScreen", exename, 1);
    atexit(_unsetenv);

    InitConfig();
    CSM_RAM *save_cmpc;
    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
    CSM_root()->csm_q->current_msg_processing_csm=(CSM_RAM*)CSM_root()->csm_q->csm.first;
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
    UnlockSched();
    return 0;
}


