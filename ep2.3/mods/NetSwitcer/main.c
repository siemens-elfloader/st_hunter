#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"
#include "local_ipc.h"

//IPC constants for XTask interface
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
extern const char EntryOff[8];
extern const char EntryOn[8];

extern void InitConfig();
int my_csm_id;
unsigned int *ErrorNumber;

const char percent_t[]="%t";
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);
extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}


typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;   

void Hide()
{
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
}

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
	int i;
	int c;
	while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
	return(i);
}


#define FUNC_PT_START	0xA085DEA4
#define FUNC_PT_END	0xA0865BA3
#define FUNC_PT_PSIZE	0x80
#define FUNC_PT_OFFSET	0x10
#define MAX_FUNC	0xFA


void run_address(unsigned int address) 
{
  void (*runaddr)(void);
  runaddr=(void (*)())address;
  if (runaddr) 
  {
    runaddr();
  }
}


int chr2num(char chr)
{
	if(chr>='0'&&chr<='9')
		return (chr-('0'-0));
	if(chr>='A'&&chr<='Z')
		//return (chr-0x37);
		return (chr-('A'-0xA));
	if(chr>='a'&&chr<='z')
		//return (chr-0x57);
		return (chr-('a'-0xA));
	return 0;
}

int start(const char s[128])
{
  if((s)&&strlen(s))
   {    
    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       ((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))
      /* Если в строке нет символов '\\' и '.' и первые два символа А0,
         то запускаем энтрипоинт */
        {/*
          int entry;
          sscanf(s,"%08X",&entry);
          SUBPROC((void*)entry);*/
           int i;
          unsigned int addr=0;
          char *p=(char*)s;
          for(i=0;i<strlen(s);i++)
            {
               addr=chr2num(p[i])+addr*0x10;
            }
          run_address(addr);
          return 1;
        }
    }
    
  return 0;
}

// ----------------------------------------------------------------------------
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

#define IPC_CHECK_DOUBLERUN 2
#define IPC_SHOW_MSG 3
int maincsm_id;
int off=0;
void CheckDoubleRun(void)
{
  int csm_id;
  if ((csm_id=(int)(gipc.data))!=-1)
  {
    gipc.name_to=ipc_xtask_name;
    gipc.name_from=ipc_my_name;
    gipc.data=(void *)csm_id;
//    GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SHOW_MSG,&gipc);  
    LockSched();
    CloseCSM(maincsm_id);
    ShowMSG(1,(int)"NetOff уже запущен!");
    UnlockSched();
  }
  else
    {
      if(strlen((char*)EntryOff)!=0)
      {
         if(start(EntryOff)==1) off=1;
      }
      else
         ShowMSG(1,(int)"Введи энтрик оффнет!!!");
    }   
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
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      InitConfig();
      ShowMSG(1,(int)"NetOff config updated!");
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
         case IPC_CHECK_DOUBLERUN:
	    //Если приняли свое собственное сообщение, значит запускаем чекер
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)CheckDoubleRun);
            else ipc->data=(void *)maincsm_id;
	    break; 
       case IPC_SHOW_MSG:          break;           
        }
      }
    }
  } 
  return(1);
}
         
static void maincsm_oncreate(CSM_RAM *data)
{ 
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
  
//  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

void maincsm_onclose(CSM_RAM *data)
{ 
  if(strlen((char*)EntryOn)!=0 && off==1) start(EntryOn);
     
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NetOff - (c)kluchnik");
}
// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  InitConfig();
  UpdateCSMname();
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  maincsm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
//  ShowMSG(1,(int)"Elfstart launch");
  return (0);
}
