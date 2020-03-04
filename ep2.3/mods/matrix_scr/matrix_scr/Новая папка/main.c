#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"
#include "random.h"
#include "local_ipc.h"

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ my_ipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

#define TMR_SECOND 216
GBSTMR mytmr;

int scr_w;
int scr_h;
void TimerProc(void)
{  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;


void timer(void);
void Check(void);

extern const int cfg_time;
extern const int cfg_sec;
extern const int cfg_runtime;
extern const int cfg_chksec;
extern const unsigned int cfg_tx;
extern const unsigned int cfg_ty;
extern const unsigned int cfg_tfont;
extern const int unsigned speed;//50
extern const int ENA_HELLO;
GBSTMR matrix,check;

int speed_sl=262*2,count_on=0;
int rnd=5,y=-10,rx,i,n=1;
int yy[177];
int xx[177],spd[177];
char OBLACK[]={0,0,0,8};
char OOBLACK[]={0,0,0,100};
char WHITE[]={255,255,255,100};
char GREEN[]={30,240,30,100};
char RED[]={255,0,0,100};
char *buf;
int bufsize=256;
void window(void)
{
  DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0,OBLACK,OBLACK);
}

void line(int xpos,int y)
{
  DrawRoundedFrame(xpos,y,xpos,y+5,0,0,0,GREEN,GREEN);
  DrawRoundedFrame(xpos,y+5,xpos,y+7,0,0,0,WHITE,WHITE);
}

void time(void)
{
  WSHDR *ws=AllocWS(256);
  TDate date; TTime time; 
  GetDateTime(&date,&time); 
  if(cfg_sec!=1)
  {
    wsprintf(ws,"%02d:%02d",time.hour,time.min);
    DrawString(ws,cfg_tx,cfg_ty,cfg_tx+Get_WS_width(ws,cfg_tfont),cfg_ty+GetFontYSIZE(cfg_tfont),cfg_tfont,2,WHITE,OOBLACK);
  }
  else
  {
    wsprintf(ws,"%02d:%02d:%02d",time.hour,time.min,time.sec);
    DrawString(ws,cfg_tx,cfg_ty,cfg_tx+Get_WS_width(ws,cfg_tfont),cfg_ty+GetFontYSIZE(cfg_tfont),cfg_tfont,2,WHITE,OOBLACK);
  }
  FreeWS(ws);
}
int on=0;

void timer(void)
{
  void *canvasdata;  
  canvasdata=BuildCanvas();
  if (!IsUnlocked())
  {
  DisableIconBar(1);
    //DrawCanvas( canvasdata,0,0,scr_w,scr_h,1);
  DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0,OBLACK,OBLACK);
  //  window();
  if(cfg_time==1) time();
  for(i=0;i<=65;i++)
  {
    line(xx[i],yy[i]+=spd[i]);
    if(yy[i]>scr_h) 
    {      
      yy[i]-=320;
      spd[i]=random()/50;
      if (spd[i]>10) spd[i]=10;
      if (spd[i]<=0) spd[i]=1;
    } 
   }
  GBS_StartTimerProc(&matrix,262/speed,timer);
  }
GBS_StartTimerProc(&matrix,262/speed,timer);
}

void maincsm_oncreate(CSM_RAM *data)
{
   GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}
void ElfKiller(void)
{ 
  extern void *ELF_BEGIN;
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

// Вызывается при закрытии главного CSM. Тут и вызывается киллер
void maincsm_onclose(CSM_RAM *csm)
{
  GBS_DelTimer(&matrix);
  SUBPROC((void *)ElfKiller);
}
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
// Обработчик событий главного CSM
int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
   if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  } 
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      InitConfig();
      ShowMSG(1,(int)"Matrix config updated!");
    }
  } 
  return(1); 
}

const int minus11=-11;
unsigned short maincsm_name_body[140];
// Инициализация структуры MAINCSM
const struct
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"the matrix");
}
void main(void)
{
  scr_w=ScreenW();
  scr_h=ScreenH();
  LockSched();
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  InitConfig();
  UpdateCSMname();
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
 
  for(i=0;i<=65;i++)
  {
    spd[i]=3;
    xx[i]=i*4+1;
    yy[i]=random()+100;
  }
  InitConfig();
  if(ENA_HELLO)ShowMSG(1,(int)"The Matrix has You...");
  timer();
  UnlockSched();
}

