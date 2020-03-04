#include "swilib.h"
#include "conf_loader.h"
#include "ussd_process.h"
#include "main.h"
#include "print.h"
//#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])



extern int CurrentCASH[MAX_CASH_SIZE];
//extern int MaxCASH[MAX_CASH_SIZE];

extern const int ENA_CASHTRACE;
extern const unsigned int CHECK_HOURS;
extern const int cfgShowIn;

extern const unsigned int IDLE_X;
extern const unsigned int IDLE_Y;

extern const char fontColor[];
extern const char bgrColor[];
extern const int isOutline;
extern const unsigned int x;
extern const unsigned int y;
extern const char text[];
extern const int fullfont;
extern const int align;
extern const unsigned int space;
extern const char fontpath[];
extern const unsigned int font;
void *canvasdata;

#define TMR_SECOND (1300/6)

char cur_imsi[IMSI_DATA_BYTE_LEN];
//static volatile int is_pos_changed=0;

static GBSTMR hours_tmr;

static CSM_DESC icsmd;

static int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
static void (*old_icsm_onClose)(CSM_RAM*);

#pragma inline=forced
static int toupper(int c)
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

const char white[4]={0xFF,0xFF,0xFF,0x64};
const char transparent[4]={0,0,0,0};
static void DrawMyProgress(int x, int y, int n)
{
//  int start_y, end_y, font_size,scr_w,cur, max;
  int cur;
  //WSHDR *ws=AllocWS(32);
  char *s=malloc(32);
  //scr_w=ScreenW();
  cur=CurrentCASH[n];
  //max=MaxCASH[n];
  //font_size=GetFontYSIZE(FONT_SMALL)+1;
  //start_y=y+n*font_size;
//  end_y=y+(n+1)*font_size-1;

  sprintf(s,text,((cur<0)?"-":""), ((cur<0)?(0-cur):cur)/100, ((cur<0)?(0-cur):cur)%100);
  PrintField(x,y,s,align,1,space);    
//  DrawCanvas(BuildCanvas(), x, start_y, scr_w-x-1, end_y, 1);
  //wsprintf(ws, "%s%u.%02u/%u.%02u", ((cur<0)?"-":""), ((cur<0)?(0-cur):cur)/100, ((cur<0)?(0-cur):cur)%100, max/100, max%100);
//  DrawString(ws, x+2, start_y+1, scr_w-x-3, end_y-1, FONT_SMALL, TEXT_ALIGNMIDDLE, white, transparent);
//  FreeWS(ws);
  mfree(s);
}

static void HoursTimerProc(void)
{
  SendCashReq();
}

void StartHoursTimer(void)
{
  if (CHECK_HOURS)
  {
    TTime t; int sc;
    GetDateTime(0, &t);
    sc = 3600*CHECK_HOURS - 60*t.min - t.sec;
    GBS_StartTimerProc(&hours_tmr, sc*1300/6, HoursTimerProc);
  }
}

static volatile int is_voice_connected=0;



#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
int checkGui(int id)
{
 int retval = 0; 
 if (IsGuiOnTop(id)) 
 {
    GUI* igui = GetTopGUI();
    if (igui)
    {
#ifdef ELKA
      canvasdata = BuildCanvas();
      retval = 1;
#else
      void *idata = GetDataOfItemByID(igui, 2);
      if (idata)
      {
        canvasdata = ((void **)idata)[DISPLACE_OF_IDLECANVAS / 4];
        retval = 1;
      }
#endif
    }  
 }
 return retval;
}


static int MyIDLECSM_onMessage(CSM_RAM* data,GBS_MSG* msg)
{
#define edialgui_id (((int *)data)[DISPLACE_OF_EDGUI_ID/4])
  int csm_result;
  {
    char *imsi=RAM_IMSI();
    if (memcmp(imsi,cur_imsi,IMSI_DATA_BYTE_LEN))
    {
      SaveCash();
      memcpy(cur_imsi,imsi,IMSI_DATA_BYTE_LEN);
      InitConfig();
      LoadCash();
      StartHoursTimer();

      WSHDR *ws;
      if(strlen(fontpath)==0 && font<100)
      {
        LockSched();
        ShowMSG(1,(int)"Please set font!");
        ws=AllocWS(150);
        extern const char *successed_config_filename;
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
        UnlockSched();
        return 0;
      }
      
      PrintLibInit(1);
      FontPathInit((char *)fontpath, fullfont);
      SetCanvasFlag(0);
      SetFontColor((char*)fontColor,(char*)bgrColor,isOutline);
    }
  }
  
  if (msg->msg==MSG_USSD_RX || msg->msg==MSG_AUTOUSSD_RX)
  {
    if (ProcessUSSD(data,(GBS_USSD_MSG *)msg)) return 0; //Обработанно
  }
  if (msg->msg==MSG_END_CALL)
  {
    if (ENA_CASHTRACE)
    {
      if (is_voice_connected)
      {
	GBS_DelTimer(&hours_tmr);
	SendCashReq();
      }
    }
    is_voice_connected=0;
  }
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==edialgui_id)
    {
    }
  }
  if(msg->msg == MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"Casher config updated!");
      InitConfig();
      LoadCash();
      StartHoursTimer();
      
      PrintLibFree();
      PrintLibInit(1);
      FontPathInit((char *)fontpath, fullfont);
      SetFontColor((char*)fontColor,(char*)bgrColor,isOutline);
      SetCanvasFlag(0);
    }
  }

   csm_result=old_icsm_onMessage(data,msg); //Вызываем старый обработчик событий

  if(msg->msg == MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,"IdleUpd")==0)
      {
        switch (msg->submess)
        {
        case 0:
          {
            canvasdata = BuildCanvas();
            void *icsm=FindCSMbyID(CSM_root()->idle_id);
            if (icsm && checkGui(idlegui_id(icsm)))
            {                
              if (cfgShowIn != 1 - IsUnlocked())
              {          
                extern int CASH_SIZE;
                DrawMyProgress(IDLE_X, IDLE_Y, 0);
              }
            }
          }
        }
      }
    }
  }
 
  return(csm_result);
}

void CallCenter_Destructor(void)
{
  GBS_DelTimer(&hours_tmr);
  SaveCash();
  EndUSSDtimer();
//  FreeWS(dbg_ws);
}

static void MyIDLECSM_onClose(CSM_RAM *data)
{
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  CallCenter_Destructor();
  PrintLibFree();
  seqkill(data,old_icsm_onClose,&ELF_BEGIN,SEQKILLER_ADR());
}

int main(void)
{
//  InitConfig();//  Загружаем только когда изменится imsi с 0 на любую другую
  //LoadCash();
  
  LockSched();
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  memcpy(&icsmd,icsm->constr,sizeof(icsmd));
  old_icsm_onMessage=icsmd.onMessage;
  old_icsm_onClose=icsmd.onClose;
  icsmd.onClose=MyIDLECSM_onClose;
  icsmd.onMessage=MyIDLECSM_onMessage;
  icsm->constr=&icsmd;
  UnlockSched();  


  return 0;
}
