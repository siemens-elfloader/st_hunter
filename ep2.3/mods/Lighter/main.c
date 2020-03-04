#include "..\inc\swilib.h"
//#include "..\inc\cfg_items.h"
//#include "conf_loader.h"
#define IPC_TEXTINFO_NAME "ElfStart"
#define IPC_UPDATE_STAT 1

#define IPC_XTASK_NAME "XTask"
#define IPC_XTASK_IDLE 1
#define IPC_XTASK_SHOW_CSM 2
#define IPC_XTASK_LOCK_SHOW 3
#define IPC_XTASK_UNLOCK_SHOW 4
const char ipc_my_name[32]=IPC_TEXTINFO_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
IPC_REQ gipc;

#define TMR_SECOND 216
GBSTMR mytmr;

const char percent_t[]="%t";
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);
extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

void TimerProc(void)
{
  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;   


//extern void InitConfig();

GBSTMR mytmr2;
//*(GetIlluminationDataTable ()+ 3)
int light;
int light1;
int prof;
void change()
{
  if( (prof!=GetProfile()))/* || 
      (light!=(int)*(GetIlluminationDataTable()+3)) ||
      (light1!=(int)*(GetIlluminationDataTable()+3+152)))*/
  {
   light=(int)*(GetIlluminationDataTable()+3);
   light1=(int)*(GetIlluminationDataTable()+3+152);
   prof=GetProfile();
  }
  
  SetIllumination(0,1,light,256);
  if(light1) SetIllumination(1,1,light1,256);
  GBS_StartTimerProc(&mytmr2,100,change);
}

// ----------------------------------------------------------------------------
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}
#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  void *icsm;
  /*if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
     InitConfig();
      ShowMSG(1,(int)"ElfStart config updated!");
    }
  }*/
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
        case IPC_UPDATE_STAT:
          GBS_StartTimerProc(&mytmr, 10*TMR_SECOND/10, TimerProc);
        }
      }
    }
  }
  icsm=FindCSMbyID(CSM_root()->idle_id);
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm))) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {
      }
    }
  }
  return(1);
}
static void maincsm_oncreate(CSM_RAM *data)
{
  //change((int)*(GetIlluminationDataTable()+3));
  prof=GetProfile();
  light=(int)*(GetIlluminationDataTable()+3);
  light1=(int)*(GetIlluminationDataTable()+3+152);
  change();
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

void maincsm_onclose(CSM_RAM *data)
{ 
  GBS_DelTimer(&mytmr); 
  GBS_DelTimer(&mytmr2);
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

static unsigned short maincsm_name_body[140];
const int minus11=-11;

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
  sizeof(CSM_RAM),
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Light");
}
// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
//  InitConfig();
  UpdateCSMname();
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
  
/*  char s[10];
  sprintf(s,"%d",(int)*(GetIlluminationDataTable ()+ 3));
  ShowMSG(1,(int)s);*/
 // ShowMSG(1,(int)*(GetIlluminationDataTable ()+ 3));
  return (0);
}
