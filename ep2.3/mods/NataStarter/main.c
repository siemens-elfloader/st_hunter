#include "..\inc\swilib.h"
#include "print.h"

const char percent_t[]="%t";
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


extern const int sleepsec;
extern const char FILE[];
extern void InitConfig();

int maincsm_id;
GBSTMR check_tmr;

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

void sleep(int sec)
{
  TTime t,t2;
  GetDateTime(0,&t);
  while(sec)
  {
   GetDateTime(0,&t2);
   if(t2.sec!=t.sec)
    {
       sec--;
       t=t2;
    }	
  }  
  

  if(get_file_size((char*)FILE))
  {
    WSHDR *ws;
    ws=AllocWS(256);
    wsprintf(ws,FILE);
    ExecuteFile(ws,0,0);
    FreeWS(ws); 
  }
  else
  {
    LockSched();
    ShowMSG(2,(int)"”кажи файл сцука!!!");
    UnlockSched();
    LockSched();
    ShowMSG(2,(int)"”кажи файл сцука!!!");
    UnlockSched();
  }
  CloseCSM(maincsm_id);
}


void Check()
{
 if(IsGPRSConnected())
 {
   SUBPROC((void*)sleep,sleepsec);
   GBS_DelTimer(&check_tmr);
 }
 else
  GBS_StartTimerProc(&check_tmr, 216, Check);
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
  return(1);
}
static void maincsm_oncreate(CSM_RAM *data)
{
}

void maincsm_onclose(CSM_RAM *data)
{    
  GBS_DelTimer(&check_tmr); 
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NataStarter");
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
  maincsm_id = CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
  
  Check();
  return (0);
}
