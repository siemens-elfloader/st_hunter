#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "system.h"
//#include "config.h"

#define ipc_my_name "EMenuD 2"
#define IPC_CHECK_DOUBLERUN 1
#define IPC_OPEN_GUI 2
#define IPC_CLOSE 3

IPC_REQ gipc;
const int minus11=-11;
unsigned int DaemonGSM_ID=0;
CSM_RAM *under_idle;

int IDLE_KeyHook(int submsg, int msg)
{
  int mode=LongPress?LONG_PRESS:KEY_DOWN;
  if(IDLE_BUTTON && IsUnlocked() && (submsg == IDLE_BUTTON) && (msg==mode) && 
    IsGuiOnTop (((int *)FindCSMbyID(CSM_root()->idle_id))[DISPLACE_OF_IDLEGUI_ID/4]) )
  {
    CreateMMenu();
    return KEYHOOK_ENDQ;
  }
  return KEYHOOK_NEXT;
}

extern void kill_data(void *p, void (*func_p)(void *));

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

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ReadSettings();
    }
  }
  
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
          case IPC_CLOSE:
            if (ipc->name_from==gipc.name_from)
              CloseCSM((int)ipc->name_from);
            break;
          case IPC_CHECK_DOUBLERUN:
            if (ipc->name_from!=gipc.name_from)
            {
              LockSched();
              gipc.name_to=ipc_my_name;
              gipc.name_from=ipc->name_from;
              gipc.data=(void *)-1;
              GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CLOSE,&gipc);
              //GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_OPEN_GUI,&gipc);
              CreateMMenu();
              UnlockSched();
            }
            if (ipc->name_from==gipc.name_from)
              //ReadSettings();
	    break;
          case IPC_OPEN_GUI:
	    CreateMMenu();
	    break;
        }
      }
    }
  }
  
  return(1);
}

static void maincsm_oncreate(CSM_RAM *data)
{ 
  LockSched();
  gipc.name_to=ipc_my_name;
  gipc.name_from=(char *)DaemonGSM_ID;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
  UnlockSched();
  AddKeybMsgHook((void *)IDLE_KeyHook);
}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
  RemoveKeybMsgHook((void *)IDLE_KeyHook);
  if(MAINCSM_ID) CloseCSM(MAINCSM_ID);
  SUBPROC((void *)Killer);
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

static void UpdateDAEMONname(void){wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"EMenuD 2");}

__thumb unsigned int GetBLAddr(unsigned int adr)
{
  short _1_11=((short*)adr)[0];
  int _1=(_1_11&0x7FF)<<11|(_1_11&0x0400?0xFFC00000:0);
  short _2=((short*)adr)[1]&0x7FF;
  return(unsigned int)(adr+_1*2+_2*2+4+(_1_11>>12&1));
}

NATMENU_ITEM_DESC *Menu_Items;
ITC Items[12];

int m;

void Get_Table()
{
  unsigned int adr;
  #pragma swi_number=0x8061
  __swi __arm unsigned int ShowNativeMenu_adr();
  adr=GetBLAddr(GetBLAddr(ShowNativeMenu_adr()+1)+9)+23;
  Menu_Items=*(void**)((adr&0xFFFFFFFD)+*(char*)adr*4+4);
}

int main()
{
  CSM_RAM *save_cmpc;

  char dummy[sizeof(MAIN_CSM)];
  UpdateDAEMONname();
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  DaemonGSM_ID=CreateCSM(&MAINCSM.maincsm,dummy,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  
  Get_Table();
  
  ReadSettings();
  UnlockSched();
  Casher();
  return 0;
}
