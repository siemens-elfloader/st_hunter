#include <swilib.h>
#include <strings.h>
#include <conf_loader.h>
#include "radio.h"
#include <alib/io_ui.h>
#include <xtask_ipc.h>

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

#define ELF_NAME "RadioPlayer"

extern char * successed_config_filename;


/*------------------------------------------------------------------------*/
/*--------------------- Создание цсм процесса и гуя ----------------------*/
/*------------------------------------------------------------------------*/

char clrWhite[]= {0xFF,0xFF,0xFF,0x64};
char clrBlack[]= {0x00,0x00,0x00,0x64};

int scr_w,scr_h;


extern const unsigned int CFG_DISABLE_ICONBAR;
static void OnRedraw(MAIN_GUI *data)
{
#ifdef ELKA
    if (CFG_DISABLE_ICONBAR) DisableIconBar(1);
#endif
    //DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0,clrBlack,clrWhite);
    DrawWindow ();

    //wsprintf(data->ws, "%t\nHello World","Привет мир");

    //DrawString(data->ws,0,28,scr_w-1,scr_h-1,FONT_SMALL,2+32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
}


static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
    //data->ws = AllocWS(128);
    UI_OnCreate();

    data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    data->gui.state=0;

    OnCloseWin ();
    //FreeWS( data->ws );

}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
    data->gui.state=2;

    UI_OnFocus ();
    DisableIDLETMR();

}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{

#ifdef ELKA
    if (CFG_DISABLE_ICONBAR) DisableIconBar(0);
#endif


    UI_UnFocus ();

    if (data->gui.state!=2) return;
    data->gui.state=1;
}

void DoConfig (){
    WSHDR *ws=AllocWS (128);
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws, NULL, NULL);
    FreeWS(ws);
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
    /*if ((msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS))
    {
        switch(msg->gbsmsg->submess)
        {
            case '#':  DoConfig (); break;

            case RIGHT_SOFT:
            return (1);
        }
    }*/

    int mess=msg->gbsmsg->msg;
    int key=msg->gbsmsg->submess;

    return MyUIOnKey (mess, key);

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
    MAIN_GUI *main_gui = new MAIN_GUI [sizeof(MAIN_GUI)];
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(RECT *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    csm->csm.state=0;
    csm->csm.unk1=0;
    my_csm_id=csm->gui_id=CreateGUI(main_gui);
}

static void maincsm_onclose(CSM_RAM *csm)
{
    kill_elf();
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
            IsFontCfgUpdate=1;
            ShowMSG (1, (int)"Config updated!");

        }
    }

    return(1);
}


const struct
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
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name), ELF_NAME);
}

/*
#include <dlfcn.h>

int LibWork (){
    void *dl = dlopen("libtest.so", 0);
    if(!dl){
        ShowMSG(1, (int)"Can`t load library!");
        return -1;
    }

    void (*test)() = (void (*)())dlsym(dl, "test");
    if(!test) goto err;

    goto success;

err:
    dlclose(dl);
    return -1;

success:;

    test ();

    dlclose(dl);

    return 0;

}
*/
char dir[256];

char *GetCurDir (){
    return dir;
}
int main (char *exename, char *fname)
{
    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();

    InitConfig();

    GetFileDir (exename, dir);

    //SetCurFile (fname);

    //LibWork ();

    return 0;
}

IPC_REQ msg;
void SendIPC (char const *from, char const *to, int ipc, void* data){

  msg.name_to=to;
  msg.name_from=from;
  msg.data=data;
  GBS_SendMessage (MMI_CEPID, MSG_IPC, ipc, &msg);
}

void SwitchToIdle(){
    SendIPC (ELF_NAME, IPC_XTASK_NAME, IPC_XTASK_IDLE, 0);
}
