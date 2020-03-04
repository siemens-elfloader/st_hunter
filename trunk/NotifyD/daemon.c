#include <swilib.h>
#include "gui.h"
#include "notify.h"
#include "swihook.h"
#include "conf_loader.h"

//#include <stdio.h>
//#include <unistd.h>

#define DAEMON_CSM_NAME "NotifyD"

const int minus11=-11;
int swi_addr_msg;
int swi_addr_msgerror;

typedef struct
{
    CSM_RAM csm;
} MAIN_CSM;

inline int toupper(int c)
{
    if ((c>='a')&&(c<='z')) c+='A'-'a';
    return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
    int i;
    int c;
    while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
    return(i);
}

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
    if(msg->msg == MSG_RECONFIGURE_REQ)
    {
        extern const char *successed_config_filename;
        if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
        {
            InitConfig();
            ShowMSG(1,(int)"NotifyD config updated!");
        }
    }
    if (msg->msg==MSG_IPC)
    {
        IPC_REQ *ipc=((IPC_REQ*)msg->data0);

        if((ipc=(IPC_REQ*)msg->data0))
        {
            if (strcmp_nocase(ipc->name_to, IPC_NAME_TO)==0)
            {
                SUBPROC((void*)CreateMyGUI, (char*) ipc->data, msg->submess);
            }
        }
    }
    return (1);
}

static void maincsm_oncreate(CSM_RAM *data)
{

}

static void maincsm_onclose(CSM_RAM *csm)
{
    DestroySWIHook(swi_addr_msg, SHOWMSG_SWI_ID);
    DestroySWIHook(swi_addr_msgerror, MSGBOXERROR_SWI_ID);
    kill_elf();
}

static unsigned short maincsm_name_body[140];

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

static void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name), DAEMON_CSM_NAME);
}


void CreateDaemon()
{
    InitConfig();
    SetSWIHook(&swi_addr_msg, SHOWMSG_SWI_ID, (void*)&ShowMsgOk);
    SetSWIHook(&swi_addr_msgerror, MSGBOXERROR_SWI_ID, (void*)&ShowMsgError);
    CSM_RAM *save_cmpc;
    char dummy[sizeof(MAIN_CSM)];
    UpdateCSMname();
    LockSched();
    save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
    CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
    CreateCSM(&MAINCSM.maincsm,dummy,0);
    CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
    UnlockSched();
}

