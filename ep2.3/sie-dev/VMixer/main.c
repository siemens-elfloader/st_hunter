#include "../inc/swilib.h"
#include "conf_loader.h"
#include "keyhook.h"
#include "gui.h"
#include "ipc.h"

const int minus11=-11;

typedef struct
{
	CSM_RAM csm;
}MAIN_CSM;

#pragma inline=forced

int toupper(int c)
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

static int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
	if(msg->msg == MSG_RECONFIGURE_REQ) 
	{
		if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
			InitConfig();
	}
	if (msg->msg==MSG_IPC)
	{
		IPC_REQ *ipc;
		if((ipc=(IPC_REQ*)msg->data0))
		{
			if (strcmp_nocase(ipc->name_to, IPC_MY_NAME)==0)
			{
				int ipc_data=*((int*)ipc->data);
				if(data->id!=ipc_data)//присланный id не равен текущему
				{
					LockSched();
					CloseCSM(ipc_data);
					UnlockSched();
					SUBPROC((void*)CreateOpenGUI);
				}
			}
		}
	}
	return 1;
}

static void maincsm_oncreate(CSM_RAM *data)
{
	IPC_Send(IPC_MY_NAME, IPC_MY_NAME, &data->id, 0);
}

void ElfKiller(void)
{
	extern void *ELF_BEGIN;
	extern void kill_data(void *p, void (*func_p)(void *));
	kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
	RemoveKeyHook();
	if(successed_config_filename) mfree(successed_config_filename);
	SUBPROC((void*)CloseGUI);
	SUBPROC((void*)ElfKiller);
}

static unsigned short maincsm_name_body[140];

static const struct
{
	CSM_DESC maincsm;
	WSHDR maincsm_name;
}MAINCSM =
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
	139
}
};

static void UpdateCSMname(void)
{
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"VMixerD");
}

int main()
{
	CSM_RAM *save_cmpc;
	char dummy[sizeof(MAIN_CSM)];
	UpdateCSMname();  
	InitConfig();
	LockSched();
	save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
	CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
	CreateCSM(&MAINCSM.maincsm,dummy,0);
	CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
	AddKeyHook();
	UnlockSched();
	return 0;
}
