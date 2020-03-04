#include "swilib.h"
#include "conf_loader.h"
#include "config_data.h"
#include "ussd_process.h"
#include "main.h"
#include "print.h"
//#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])

//extern int MaxCASH[MAX_CASH_SIZE];
extern int CurrentCASH[MAX_CASH_SIZE];
void *canvasdata;
#define TMR_SECOND (1300/6)
char cur_imsi[IMSI_DATA_BYTE_LEN];
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
extern int CASH_num;
int is_check;
static void Write_CASH(int nusd, char *text)
{
  unsigned int ul;
  char std[300];
  char stdF[128];
  if (!text) return;
  sprintf(stdF, "%sCASH_%d.txt",cashTEMP_PATH, nusd);
     int f=fopen(stdF,A_ReadWrite+A_Create+A_BIN+A_Truncate,P_WRITE,&ul);
    if (f!=-1)
    {
     ul = sprintf(std, "%s",text);
     fwrite(f,std,ul,&ul);
     fclose(f,&ul);
    }
  //mfree(text);
}
static void DrawMyProgress(char* pic, char* text, int x, int y, int n, int inFile)
{
  int cur;
  char *s=malloc(100);
  int xx=GetImgWidth((int)pic)+ 2;
  cur=CurrentCASH[n];
  //max=MaxCASH[CASH_num][n];
  sprintf(s,text,((cur<0)?"-":""), ((cur<0)?(0-cur):cur)/100, ((cur<0)?(0-cur):cur)%100);
  if(!inFile)
   {
    if(xx) PrintPic( x,y,(int) pic);
    if(is_check)PrintField(x+xx, y, "< check >", align, font, space);
    else
     PrintField(x+xx, y, s, align, font, space);
   }
  else
   Write_CASH(n, s);
    mfree(s);
   }
  int IsCHec(int HOUR)
{
 int from   =   CHECK_HOURS_FROM;
 int to     =   CHECK_HOURS_TOO;
 while(from!=to)
 {
  if(from == HOUR)return 1;
  if(from+1 == 24)from=0; else from++;
 }
return 0;
}
int hhh;
static void HoursTimerProc(void)
{
  if(IsCHec(hhh))
   {
    is_check=1;
    SendCashReq();
   }
}


void StartHoursTimer(void)
{
  if (CHECK_HOURS)
  {
    TTime t;
    int sc;
    GetDateTime(0, &t);
    hhh = t.hour;
    sc  = 3600*CHECK_HOURS - 60*t.min - t.sec;
    GBS_StartTimerProc(&hours_tmr, sc*1300/6, HoursTimerProc);    
  }
}


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
int IsIdle(void)
{
  void *icsm=FindCSMbyID(CSM_root()->idle_id);
  if (!IsScreenSaver()
      &&IsUnlocked())
    return(1);
  else
  return(0);
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
      is_check=1;
      SendCashReq();
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
   if (ProcessUSSD(data,(GBS_USSD_MSG *)msg))
   {
    if(CASH_num==1)CASH_num=2;
    if(CASH_num==0)
    {
     if(ENA_CashPat2 || ENA_CashPat3 || ENA_CashPat4)
     {
      CASH_num=1;
      SendCashReq();
     }else
      CASH_num =2;
    }
    is_check=0;
    return 0; //Обработанно
   }
  }
  if (msg->msg==MSG_END_CALL)
  {
   GBS_DelTimer(&hours_tmr);
   is_check=1;
   SendCashReq();
  }
  if(msg->msg == MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
     InitConfig();
     LoadCash();
     is_check=1;
     SendCashReq();
     StartHoursTimer();

     PrintLibFree();
     PrintLibInit(1);
     FontPathInit((char *)fontpath, fullfont);
     SetFontColor((char*)fontColor,(char*)bgrColor,isOutline);
     SetCanvasFlag(0);
    }
  }
  csm_result=old_icsm_onMessage(data,msg); //Вызываем старый обработчик событий
   if(IsIdle())
   {
    void *icsm=FindCSMbyID(CSM_root()->idle_id);
    if (icsm && checkGui(idlegui_id(icsm)))
    {
      DrawMyProgress((char*)PIC_FILE0,(char*)text0,IDLE_X0,IDLE_Y0,0,ENA_FILE0);
      if(ENA_CashPat1)
       DrawMyProgress((char*)PIC_FILE1,(char*)text1,IDLE_X1,IDLE_Y1,1,ENA_FILE1);
      if(ENA_CashPat2)
       DrawMyProgress((char*)PIC_FILE2,(char*)text2,IDLE_X2,IDLE_Y2,2,ENA_FILE2);
      if(ENA_CashPat3)
       DrawMyProgress((char*)PIC_FILE3,(char*)text3,IDLE_X3,IDLE_Y3,3,ENA_FILE3);
      if(ENA_CashPat4)
       DrawMyProgress((char*)PIC_FILE4,(char*)text4,IDLE_X4,IDLE_Y4,4,ENA_FILE4);
    }
   }
  return(csm_result);
}

void CallCenter_Destructor(void)
{
  GBS_DelTimer(&hours_tmr);
  DUSSDtimer();
  //SaveCash();
  //EndUSSDtimer();
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


