#include "..\inc\swilib.h"
#include "local_ipc.h"

int my_csm_id;

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

// ----------------------------------------------------------------------------
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
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
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  }

static unsigned short maincsm_name_body[140];
const int minus11=-11;


#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);

int hmm;
int ggg;

void addIconBar(short* num)
{
  if(!IsUnlocked()) return;
  int i;
  if(IsGPRSConnected())
  {
    RefreshGPRSTraffic();
    int *traf = GetGPRSTrafficPointer();
    hmm=*traf-ggg;
    ggg=*traf;
  }

  if(hmm>=500)//везде
  {
    i=0;
  }
  else
  {
    if(hmm>=1)  i=2;//соединение
    else
    {
      if(IsGPRSConnected())  i=3;//ожидаем соединения
      else i=1;//нет коннекта
    }
  }
  AddIconToIconBar( 6690 + i, num);  
}

typedef struct
{
  char check_name[8];
  int addr;
}ICONBAR_H;

static struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
  ICONBAR_H iconbar_handler;
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
  },
  {
    "IconBar"
  }
};

void SetIconBarHandler()
{
  MAINCSM.iconbar_handler.addr = (int)addIconBar;
}

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"EdgeIconbar - (c)kluchnik");
}
// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  UpdateCSMname();
  SetIconBarHandler();
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
//  ShowMSG(1,(int)"Elfstart launch");
  return (0);
}
