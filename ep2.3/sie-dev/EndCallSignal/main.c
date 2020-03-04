#include "..\inc\swilib.h"
#include "conf_loader.h"

const int minus11=-11;
GBSTMR VibraTimer;
//GBSTMR timer;
//int flagcall;
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
CSM_RAM *icsm;

void StopVibra()
{
  SetVibration(0);
}

void PlayMelody(const char *file, int sndVolume) 
{ 
  if (!IsCalling()) 
  { 
    FSTATS fstats; 
    unsigned int err; 
    if (GetFileStats(file,&fstats,&err)!=-1) 
    { 
      PLAYFILE_OPT _sfo1; 
      WSHDR* sndPath=AllocWS(128); 
      WSHDR* sndFName=AllocWS(128); 
      char s[128]; 
      const char *p=strrchr(file,'\\')+1; 
      str_2ws(sndFName,p,128); 
      strncpy(s,file,p-file); 
      s[p-file]='\0'; 
      str_2ws(sndPath,s,128); 

      zeromem(&_sfo1,sizeof(PLAYFILE_OPT)); 
      _sfo1.repeat_num=1; 
      _sfo1.time_between_play=0; 
      _sfo1.play_first=0; 
      _sfo1.volume=sndVolume; 
#ifdef NEWSGOLD 
      _sfo1.unk6=1; 
      _sfo1.unk7=1; 
      _sfo1.unk9=2; 
      PlayFile(0x10, sndPath, sndFName, GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1); 
#else 
#ifdef X75 
      _sfo1.unk4=0x80000000; 
      _sfo1.unk5=1; 
      PlayFile(0xC, sndPath, sndFName, 0,GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1); 
#else 
      _sfo1.unk5=1; 
      PlayFile(0xC, sndPath, sndFName, GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1); 
#endif 
#endif 
      FreeWS(sndPath); 
      FreeWS(sndFName); 
    }; 
  }; 
};

void PlayStart(void)
{
  extern const int PLAY_TYPE_ST;
  if (PLAY_TYPE_ST==0) 
  {
    extern const unsigned int SND_NUM_ST,VIBRA_SND_ST;
    PlaySound(1,0,0,SND_NUM_ST,0);
    if (VIBRA_SND_ST) 
    {
      extern const unsigned int VIBRA_SND_POWER_ST,VIBRA_SND_DURATION_ST;
      SetVibration(VIBRA_SND_POWER_ST);        
      GBS_StartTimerProc(&VibraTimer,21*VIBRA_SND_DURATION_ST, StopVibra); // 0.1 sec*XXX
    };
  }
  if (PLAY_TYPE_ST==1) 
  {
    extern const unsigned int MEL_VOL_ST,VIBRA_MEL_ST;
    int mel_vol;
    mel_vol=MEL_VOL_ST;
    extern const char MEL_PATH_ST[128];
    PlayMelody(MEL_PATH_ST,mel_vol);
    if (VIBRA_MEL_ST) 
    {
      extern const unsigned int VIBRA_MEL_POWER_ST,VIBRA_MEL_DURATION_ST;
      SetVibration(VIBRA_MEL_POWER_ST);        
      GBS_StartTimerProc(&VibraTimer,21*VIBRA_MEL_DURATION_ST, StopVibra); // 0.1 sec*XXX
    };
  };
};

void PlayEnd()
{
  extern const int PLAY_TYPE;
  if (PLAY_TYPE==0) 
  {
    extern const unsigned int SND_NUM,VIBRA_SND;
    PlaySound(1,0,0,SND_NUM,0);
    if (VIBRA_SND) 
    {
      extern const unsigned int VIBRA_SND_POWER,VIBRA_SND_DURATION;
      SetVibration(VIBRA_SND_POWER);        
      GBS_StartTimerProc(&VibraTimer,21*VIBRA_SND_DURATION, StopVibra); // 0.1 sec*XXX
    };
  }
  if (PLAY_TYPE==1) 
  {
    extern const unsigned int MEL_VOL,VIBRA_MEL;
    int mel_vol;
    mel_vol=MEL_VOL;
    extern const char MEL_PATH[128];
    PlayMelody(MEL_PATH,mel_vol);
    if (VIBRA_MEL) 
    {
      extern const unsigned int VIBRA_MEL_POWER,VIBRA_MEL_DURATION;
      SetVibration(VIBRA_MEL_POWER);        
      GBS_StartTimerProc(&VibraTimer,21*VIBRA_MEL_DURATION, StopVibra); // 0.1 sec*XXX
    };
  };
};

    

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
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"EndCallSignal.bcfg reloaded!");
      InitConfig();
    }
  }
  return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{
  
}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  GBS_DelTimer(&VibraTimer);
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
  ((CSM_DESC*)icsm->constr)->onMessage=old_icsm_onMessage;
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"%t","EndCallSignal");
}

int MyIDLECSM_onMessage(CSM_RAM* data,GBS_MSG* msg)
{
  int csm_result=old_icsm_onMessage(data,msg);
  
  if((msg->msg==MSG_STATE_OF_CALL)&&(msg->submess==1)&&((int)msg->data0==2))
  {
    PlayStart();
  };
  
  if(msg->msg == MSG_END_CALL) 
  {
    PlayEnd();
  };
  return(csm_result);
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
  icsm=FindCSMbyID(CSM_root()->idle_id); 
  old_icsm_onMessage=((CSM_DESC*)icsm->constr)->onMessage; 
  ((CSM_DESC*)icsm->constr)->onMessage=MyIDLECSM_onMessage;

  extern const unsigned int ENA_HELLO_MSG;
  if (ENA_HELLO_MSG) ShowMSG(1,(int)"EndCallSignal \n(c) kostett\n(r) 2all");
}
