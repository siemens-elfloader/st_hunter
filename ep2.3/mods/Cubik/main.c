#include "..\inc\swilib.h"
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

void TimerProc(void)
{
  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
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
  void *icsm;

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
  }/*
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
  }*/
  return(1);

}
static void maincsm_oncreate(CSM_RAM *data)
{ 
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

void maincsm_onclose(CSM_RAM *data)
{ 
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  }
/*
void do_logwrite(char *text)
{
  int hFile;
  unsigned int io_error = 0;
  hFile = fopen("4:\\icub.txt",A_ReadWrite + A_Create + A_Append,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
  {
    fwrite(hFile, text, strlen(text), &io_error);
    fwrite(hFile, "\r\n", 2, &io_error);
    fclose(hFile, &io_error);
  }
  else
    ShowMSG(1,(int)"Error!");
//  mfree(text);
}
*/
const unsigned char rndtab[256] ={
  1,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
 74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
 95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
 52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   1,
175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
 25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
 94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
 80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
 24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
 28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
 71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
 17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
120, 163, 236, 249
};


unsigned int Random(int min,int max)
{
  static const int DMonth[]={0,31,59,90,120,151,181,212,243,273,304,334,365};
  unsigned int iday;
  TTime tt;
  TDate dd;
  GetDateTime(&dd,&tt);
  dd.year=dd.year%100;
  iday=DMonth[tt.sec%12];
  iday = (tt.sec+60*(tt.min+60*(tt.hour+24* iday)));
  iday*=rndtab[iday%256];
  iday=iday%max;
  
  if(iday<min)
  {
    int i=0;
    while(iday<min)
    {
      iday=rndtab[iday%256]%(max-min);
//      i++;  
    }    
  }
  return iday;
}


static unsigned short maincsm_name_body[140];
const int minus11=-11;


#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);


void addIconBar(short* num)
{
  if(!IsUnlocked()) return;
  //char *s=malloc(sizeof(unsigned long));
  unsigned int pic=Random(13,30);
  //sprintf(s,"%u",pic);
  //do_logwrite(s);
//  mfree(s);
  AddIconToIconBar( 6680 + pic, num);  
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"IconbarCubik - (c)kluchnik");
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
