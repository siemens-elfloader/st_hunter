#include <swilib.h>
#include <unistd.h>
#include <conf_loader.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "pxvid.h"


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

typedef struct
{
    GUI gui;
    WSHDR *ws;
} MAIN_GUI;

char avi_file[256] = "";
char filepath[256] = "4:\\mpeg4\\";
/*------------------------------------------------------------------------*/
/*---------------------îÎÎîîÎÎîîÎÎîîÎÎîîÎÎîîÎÎîîÎÎî ----------------------*/
/*------------------------------------------------------------------------*/

char clrWhite[]= {0xFF,0xFF,0xFF,0x64};
char clrBlack[]= {0x00,0x00,0x00,0x64};
IMGHDR screen = {132, 176, 8, 0};
int scr_w,scr_h;
TTime mtime;
int _unfocus = 0, pause = 0;
int fps = 0, last_time = 0, cfps = 0;
WSHDR *lws = 0;

int FORMAT = USE_PNM;
int BPP = 2;
void *dec_handle = NULL;
int XDIM = 0;
int YDIM = 0;
int CSP = XVID_CSP_RGB565;
const int display_buffer_bytes = 0;
#define BUFFER_SIZE (30*1024)
#define MIN_USEFUL_BYTES 1



unsigned char *mp4_buffer = NULL;
unsigned char *mp4_ptr    = NULL;
unsigned char *out_buffer = NULL;
int useful_bytes;
int chunk;
xvid_dec_stats_t xvid_dec_stats;
double totaldectime;
long totalsize;
int status;
int use_assembler = 1;
int debug_level = 0;
char filename[256];
FILE *in_file;
int filenr;
int i;
int is_decoding = 0;


int init_decoder()
{
    printf("xvid_decraw - raw mpeg4 bitstream decoder ");
    printf("written by Christoph Lampert\n\n");

    in_file = fopen(avi_file, "rb");
    if (in_file == NULL)
    {
        fprintf(stderr, "Error opening input file %s\n", avi_file);
        return(-1);
    }

    fseek(in_file, 1024, SEEK_SET);

    /* PNM/PGM format can't handle 16/32 bit data */
    if (BPP != 1 && BPP != 3 && FORMAT == USE_PNM)
    {
        FORMAT = USE_TGA;
    }
    if (BPP != 1 && FORMAT == USE_YUV)
    {
        FORMAT = USE_TGA;
    }

    /* Memory for encoded mp4 stream */
    mp4_buffer = (unsigned char *) malloc(BUFFER_SIZE);
    mp4_ptr = mp4_buffer;
    if (!mp4_buffer)
    {
        return -1;
    }

    status = dec_init(use_assembler, debug_level);
    if (status)
    {
        fprintf(stderr,
                "Decore INIT problem, return value %d\n", status);
        if (dec_handle)
        {
            status = dec_stop();
            if (status)
                fprintf(stderr, "decore RELEASE problem return value %d\n", status);
        }
        free(mp4_buffer);
        return -2;
    }

    is_decoding = 1;

    /* Fill the buffer */
	useful_bytes = (int) fread(mp4_buffer, 1, BUFFER_SIZE, in_file);

	totaldectime = 0;
	totalsize = 0;
	filenr = 0;
	mp4_ptr = mp4_buffer;
	chunk = 0;

    return 0;
}


int decode_frame()
{
    int used_bytes = 0;
    double dectime = 0;

    /*
     * If the buffer is half empty or there are no more bytes in it
     * then fill it.
     */
    if (mp4_ptr > mp4_buffer + BUFFER_SIZE/2)
    {
        int already_in_buffer = (int)(mp4_buffer + BUFFER_SIZE - mp4_ptr);

        /* Move data if needed */
        if (already_in_buffer > 0)
            memcpy(mp4_buffer, mp4_ptr, already_in_buffer);

        /* Update mp4_ptr */
        mp4_ptr = mp4_buffer;

        /* read new data */
        if(!feof(in_file))
        {
            useful_bytes += (int) fread(mp4_buffer + already_in_buffer,
                                        1, BUFFER_SIZE - already_in_buffer,
                                        in_file);
        }
    }


    /* This loop is needed to handle VOL/NVOP reading */
    do
    {
        /* Decode frame */
        used_bytes = dec_main(mp4_ptr, out_buffer, useful_bytes, &xvid_dec_stats);
        if(used_bytes <= 0)
        {
            return -4;
        }

        /* Resize image buffer if needed */
        if(xvid_dec_stats.type == XVID_TYPE_VOL)
        {

            /* Check if old buffer is smaller */
            if(XDIM*YDIM < xvid_dec_stats.data.vol.width*xvid_dec_stats.data.vol.height)
            {

                /* Copy new witdh and new height from the vol structure */
                XDIM = xvid_dec_stats.data.vol.width;
                YDIM = xvid_dec_stats.data.vol.height;

                /* Free old output buffer*/
                if(out_buffer) free(out_buffer);

                /* Allocate the new buffer */
                out_buffer = (unsigned char*)malloc(XDIM*YDIM*4);
                if(out_buffer == NULL)
                {
                    is_decoding = 0;
                    return -2;
                }

                //fprintf(stderr, "Resized frame buffer to %dx%d\n", XDIM, YDIM);
            }
        }

        /* Update buffer pointers */
        if(used_bytes > 0)
        {
            mp4_ptr += used_bytes;
            useful_bytes -= used_bytes;

            /* Total size */
            totalsize += used_bytes;
        }
    }
    while (xvid_dec_stats.type < 0 && useful_bytes > MIN_USEFUL_BYTES);

    /* Check if there is a negative number of useful bytes left in buffer
     * This means we went too far */
    if(useful_bytes < 0)
            return -1;

    /* Updated data - Count only usefull decode time */
    totaldectime += dectime;

    screen.bitmap = out_buffer;
    screen.w = XDIM;
    screen.h = YDIM;
    //printf("%dx%d\n", XDIM, YDIM);

    ++filenr;
    return 0;
}


int is_have_video_frame()
{
    return (useful_bytes>MIN_USEFUL_BYTES || !feof(in_file));
}


int finish_decode()
{
    if (dec_handle)
    {
        status = dec_stop();
        if (status)
            fprintf(stderr, "decore RELEASE problem return value %d\n", status);
    }


    if(out_buffer)
        free(out_buffer);
    if(mp4_buffer)
        free(mp4_buffer);

    if(in_file)
        fclose(in_file);
    return 0;
}



void DrwImg(int x, int y, IMGHDR *img)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0);
  DrawObject(&drwobj);
}


void DrawBackGround()
{
    DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0, clrBlack, clrBlack);
}


GBSTMR tmr;
int lst = 0, rdw_bkgr = 0;
void OnRedraw(MAIN_GUI *data)
{
    if(rdw_bkgr)
    {
        DrawBackGround();
        rdw_bkgr = 0;
    }

    if(is_decoding && !lst && is_have_video_frame())
    {
        if(!pause)
            lst = decode_frame();
        is_decoding == lst ==0;
    }
    else
    {
        CloseCSM(MAINCSM_ID);
        return;
    }

    if(screen.bitmap)
        DrwImg(0, 0, &screen);

    wsprintf(lws, "%d", fps);
    DrawString(lws, 60, 0, scr_w-1, scr_h-1, FONT_SMALL, 32, GetPaletteAdrByColorIndex(3), 0);

    GetDateTime(0, &mtime);
    if(last_time != mtime.sec)
    {
        SetIllumination(0,1, 100, 50);
        fps = cfps;
        cfps = 0;
        last_time = mtime.sec;
    }

    ++cfps;
    if(is_decoding && !pause)
        GBS_StartTimerProc(&tmr, 2, (void(*)(GBSTMR*))OnRedraw);
}


static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
    data->ws = lws = AllocWS(128);
    data->gui.state=1;
    if(init_decoder())
        ShowMSG(1, (int)"Error init");
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    GBS_DelTimer(&tmr);
    is_decoding = 0;
    finish_decode();
    data->gui.state=0;
    FreeWS( data->ws );
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
    pause = 0;
    _unfocus = 0;
    rdw_bkgr = 1;
    data->gui.state=2;
    DisableIDLETMR();
#ifdef ELKA
    DisableIconBar(1);
#endif
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    if (data->gui.state!=2) return;
    data->gui.state=1;
    //GBS_DelTimer(&tmr);
    //_unfocus = 1;
    pause = 1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
    if ((msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS))
    {
        switch(msg->gbsmsg->submess)
        {
            case RIGHT_SOFT:
                return (1);

            case '5':
                pause = pause?0:1;
                rdw_bkgr = 1;
                REDRAW();
                break;
        }
    }
    SetIllumination(0,1, 100, 50);
    //REDRAW();
    return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int method8(void)
{
    return(0);
}
int method9(void)
{
    return(0);
}

const void * const gui_methods[11]=
{
    (void *)OnRedraw,
    (void *)onCreate,
    (void *)onClose,
    (void *)onFocus,
    (void *)onUnfocus,
    (void *)OnKey,
    0,
    (void *)kill_data,
    (void *)method8,
    (void *)method9,
    0
};


const RECT Canvas= {0,0,0,0};
static void maincsm_oncreate(CSM_RAM *data)
{
    scr_w=ScreenW()-1;
    scr_h=ScreenH()-1;
    MAIN_CSM*csm=(MAIN_CSM*)data;
    MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(RECT *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    csm->csm.state=0;
    csm->csm.unk1=0;
    my_csm_id=csm->gui_id=CreateGUI(main_gui);
}

void ElfKiller(void)
{
    kill_elf();
}

static void maincsm_onclose(CSM_RAM *csm)
{
    SUBPROC((void *)ElfKiller);
    fclose(stdout);
    fclose(stderr);

}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    MAIN_CSM *csm=(MAIN_CSM*)data;
    if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
    {
        csm->csm.state=-3;
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
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NatAlarm");
}



int __mfd_o, __mfd_e;
void __m_atexit()
{
    fflush(stdout);
    close(__mfd_o);

    fflush(stderr);
    close(__mfd_e);
}


int main(char *exename, char *fname)
{
    if(!fname){
        ShowMSG(1, (int)"Âûáåðèòå ôàéë!");
        kill_elf();
        return 0;
    }

    strcpy(avi_file, fname);

    SetCpuClockTempHi(2);

    //char tmp[128];
    //getcwd(tmp, 128);
    //strcat(tmp, "stdout.txt");
    //__setup_stdout_fd( (__mfd_o = open(tmp, O_CREAT | O_WRONLY | O_TRUNC)) );

    //getcwd(tmp, 128);
    //strcat(tmp, "stderr.txt");
    //__setup_stderr_fd( (__mfd_e = open(tmp, O_CREAT | O_WRONLY | O_TRUNC)) );

    //setvbuf(stdout, 0, _IOFBF, 128);
    //setvbuf(stderr, 0, _IOFBF, 128);

    atexit(__m_atexit);

    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();
    return 0;
}
