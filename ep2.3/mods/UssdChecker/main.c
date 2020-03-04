#include "..\inc\swilib.h"
//#include "conf_loader.h"


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




int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
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
              SetFontColor("\xFF\xFF\xFF\x64","\x00\x00\x00\x00",1);
              PrintField(2,150,"test",0,108,1);              
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
  
  return(1);
}


void CallCenter_Destructor(void)
{
  GBS_DelTimer(&hours_tmr);
  SaveCash();
  EndUSSDtimer();
//  FreeWS(dbg_ws);
}


static void maincsm_oncreate(CSM_RAM *data)
{
}

void maincsm_onclose(CSM_RAM *data)
{ 
  CallCenter_Destructor();
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

//-------------------- ICON IN ICONBAR --------------------//

//kluchnik 26.03.08
#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);


void addIconBar(short* num)
{
  if(!IsUnlocked()) return;
  AddIconToIconBar( 6700, num);  
}
  
static unsigned short maincsm_name_body[140];
const int minus11=-11;

typedef struct
{
  char check_name[8];
  int addr;
}ICONBAR_H;
static const struct
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
  }
  ,
  {
    "IconBar"
  }
};


static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"UssdChecker");
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
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
  
  StartHoursTimer();
  
/*  char s[10];
  sprintf(s,"%d",(int)*(GetIlluminationDataTable ()+ 3));
  ShowMSG(1,(int)s);*/
 // ShowMSG(1,(int)*(GetIlluminationDataTable ()+ 3));
  return (0);
}
