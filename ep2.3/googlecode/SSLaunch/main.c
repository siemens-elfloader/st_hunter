//SSLaunch - Запускает эльф/мидлет в качестве СкринСейвера 
//Вообще версия для SGold, т.к. на остальные платформы уже есть подобный эльф
//(c)Dimadze
//(r)feanor

//Если не компилится (Нет слова "Linking"), Используйте "Clean"

#include "..\inc\swilib.h"
#include "..\inc\xtask_ipc.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"

#define ELF_NAME "SSLaunch"
#define ELF_VERSION "0.5"
#define ELF_AUTHOR "Dimadze"
#define ELF_RESPECT "feanor"
#define SGOLD 0


#define TIMER_SEC 216
#define MIN_UPTIME 1
#define MAX(a,b) (a)>(b)?(a):(b)

extern const int bcfg_hello_ena;
extern const unsigned int bcfg_timer;
extern const int bcfg_sec;
extern const char bcfg_ssaver_path[];
extern const char bcfg_ssaver_par[];
extern const int bcfg_illum_ena;
extern const int bcfg_lock_ena;

int sstimer=0;
int light=0;
int temp_t = 0, temp_d = 0;

char elfhello[64];
char elfcfg[64];


GBSTMR timer;



int IsIDLE(void)
{
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  if (IsGuiOnTop(((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])) return(1);
                                                           else return(0);
}

const int minus11=-11;

typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;

CSM_RAM *under_idle;

const char ipc_my_name[32]=ELF_NAME; 
const char ipc_xtask_name[]=IPC_XTASK_NAME; 
IPC_REQ gipc;

void Idle(void)
{
   gipc.name_to=ipc_xtask_name; 
   gipc.name_from=ipc_my_name; 
   gipc.data=0; 
   GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
}

extern void kill_data(void *p, void (*func_p)(void *));

void execelf(char const *exename, char const *fname) //fname-параметр (имя файла), передаваемый в эльф 
{
  WSHDR *ws=AllocWS(256); 
  str_2ws(ws,exename,strlen(exename)+1); 
  ExecuteFile(ws,0,(void*)fname); FreeWS(ws);
}

int checkbox2seconds(int checkbox)
{
 int res = 0;
 if (bcfg_sec==0) res=5;
 if (bcfg_sec==1) res=10;
 if (bcfg_sec==2) res=30;
 if (bcfg_sec==3) res=60;
 if (bcfg_sec==4) res=60*2;
 if (bcfg_sec==5) res=60*5;
 if (bcfg_sec==6) res=60*10;
 if (bcfg_sec==7) res=60*30;
 return res;
}


void SSLaunch()
{
 if (IsIDLE()==1)
 {
   TDate date; TTime time; 
   GetDateTime(&date,&time);
   if (sstimer == 0) {temp_t = time.hour*60*60 + time.min*60 + time.sec;temp_d = date.day;}
   sstimer = time.hour*60*60 + time.min*60 + time.sec - temp_t + 1;
   if (temp_d != date.day) sstimer = 0;
   if (sstimer>=checkbox2seconds(bcfg_sec))
    {
     short light = *( (char *) RamScreenBrightness());
     execelf(bcfg_ssaver_path,bcfg_ssaver_par);
     if (bcfg_illum_ena == 0) SetIllumination(0,1,light,0);
     if (bcfg_lock_ena  == 1 && IsUnlocked()==0) KbdUnlock();
     if (bcfg_lock_ena  == 0 && IsUnlocked()==1) KbdLock();
     sstimer=0;
    }
 }
 else  sstimer=0;
 GBS_StartTimerProc(&timer,bcfg_timer*216, SSLaunch);
}

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
      sprintf(elfcfg,"%s:\n%s\n%s",ELF_NAME,"конфиг","обновлён");
      ShowMSG(1,(int)elfcfg);
      InitConfig();
    }
  } 
  
 void *icsm=FindCSMbyID(CSM_root()->idle_id);

  return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{

}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  GBS_DelTimer(&timer);
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),ELF_NAME);
}


int main()
{
  if (isnewSGold()!=SGOLD) {ShowMSG(1,(int)"Данный эльф\nдля SGOLD!");return 1;}
  if (isnewSGold()==SGOLD)
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
  
  SSLaunch();
  
  sprintf(elfhello,"%s %s\n(c) %s\n(r) %s",ELF_NAME,ELF_VERSION,ELF_AUTHOR,ELF_RESPECT);
  if (bcfg_hello_ena==1) ShowMSG(1,(int)elfhello);
  return 0;
  }
}
