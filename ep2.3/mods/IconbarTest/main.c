#include "..\inc\swilib.h"
#include "readimg.h"

const int minus11=-11;

typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;

CSM_RAM *under_idle;

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
  return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{

}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)Killer);
}

static unsigned short maincsm_name_body[140];

#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);

IMGHDR *icon, *icon1;
static void addIconBar(short* num)
{
  int icon_n = 6000;
  IMGHDR *ic = GetPITaddr(icon_n);
  
//  if(ic->bitmap!=icon->bitmap)
  {
    ic->h = icon->h;
    ic->w = icon->w;
    ic->bpnum = icon->bpnum;
    
    mfree(ic->bitmap);
    
    int len = strlen(icon->bitmap);
    ic->bitmap = malloc(len);
    zeromem(ic->bitmap, len);  
    memcpy(ic->bitmap, icon->bitmap, len);
/*    ic->bitmap = malloc(icon->h*icon->w*icon->bpnum);
    zeromem(ic->bitmap, icon->h*icon->w*icon->bpnum);  
    memcpy(ic->bitmap, icon->bitmap, ic->h*ic->w*icon->bpnum);*/
//    ic->bitmap = icon->bitmap;
    
  }
  //map = malloc(ic->h*ic->w*icon->bpnum);
  
 
  AddIconToIconBar(icon_n,num);
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
  },
  {
    "IconBar"
  }
};

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NSD");
}

void SetIconBarHandler()
{
  MAINCSM.iconbar_handler.addr = (int)addIconBar;
}

int main()
{
  icon = CreateIMGHDRFromPngFile("4:\\8004.png",0);
  icon1 = CreateIMGHDRFromPngFile("4:\\8005.png",0);
  CSM_RAM *save_cmpc;
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();  
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,dummy,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
  SetIconBarHandler();
  return 0;
}
