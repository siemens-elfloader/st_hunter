#include "..\inc\swilib.h"
#include "conf_loader.h"

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
/////////////////////////////////////////////////////////////////////
int old_light, old_light_dur;
int pressed=0;
GBSTMR timer;
int tmr_sec=0;
#define TMR_SECOND (1300/6)

extern const int LIGHT_LVL;
extern const unsigned int CALL_BTN;

int GetPresTypeCode()
{
  extern const unsigned int CALL_TYPE;
  if (CALL_TYPE)
  {
    return LONG_PRESS;
  }
  else
  {
    return KEY_DOWN;
  }
}

#pragma swi_number=0x80D7
__swi __arm char *RamScreenBrightness();

#pragma swi_number=0x2DC
__swi __arm void SaveMaxIllumination(int level);
#pragma swi_number=0x2DD
__swi __arm void SetIlluminationoffTimeout(int time_sec);
#pragma swi_number=0x2DE
__swi __arm void IllumTimeRequest(int TimeMode, int Counter);
#pragma swi_number=0x2DF
__swi __arm void IllumTimeRelease(int TimeMode, int Counter);

void setIlluminationHook(int level)
{
  if(level==1)
  {
    SetIllumination(0,1,LIGHT_LVL,0);
    SaveMaxIllumination(LIGHT_LVL);
    IllumTimeRequest(4,3);
  }
  else if (level==2)
  {
    SetIllumination(0,1,0,0);
    SaveMaxIllumination(old_light);
    IllumTimeRelease(4,3);
  }
  else if (level==0)
  {
    SetIllumination(0,1,old_light,0);
    SaveMaxIllumination(old_light);
    IllumTimeRelease(4,3);
  }
}

void timer_proc()
{
  ++tmr_sec;
  if (tmr_sec==old_light_dur)
  {
    setIlluminationHook(2);
    pressed=0;
    tmr_sec=0;
  }
  GBS_StartTimerProc(&timer,TMR_SECOND,timer_proc);
}

int hook_proc(int key, int m)
{
  if (m==GetPresTypeCode())
    if (key==CALL_BTN)
      if (!pressed)
      {
        LockSched();
        setIlluminationHook(1);
        pressed=1;
        timer_proc();
        UnlockSched();
      }
      else
      {
        LockSched();
        setIlluminationHook(0);
        pressed=0;
        GBS_DelTimer(&timer);
        tmr_sec=0;
        UnlockSched();
      }
  if (pressed)
  {
    tmr_sec=0;
    timer_proc();
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////
int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      InitConfig();
      RemoveKeybMsgHook((void *)hook_proc);
      AddKeybMsgHook((void *)hook_proc);
    }
  }
  return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{
  LockSched();
  old_light=*RamScreenBrightness();
  old_light_dur=*(RamScreenBrightness()+3);
  AddKeybMsgHook((void *)hook_proc);
  UnlockSched();
}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
  RemoveKeybMsgHook((void *)hook_proc);
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

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"IlluMING");
}

int main()
{
  InitConfig();
  CSM_RAM *save_cmpc;
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();  
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,dummy,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched(); 
  return 0;
}
