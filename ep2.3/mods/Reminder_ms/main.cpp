#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"
#include "print.h"

#define IPC_TEXTINFO_NAME "Reminder" 
#define IPC_UPDATE_STAT 1 

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ my_ipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

void TimerProc(void)
{  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);//эта херня отправляет ипц хтаску
}


int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);

extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}



#define TMR_SECOND 216
GBSTMR mytmr;
extern void InitConfig();
int my_csm_id;
unsigned int *ErrorNumber;

const char percent_t[]="%t";

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;   

// ----------------------------------------------------------------------------
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

int get_string_width(WSHDR *ws, int font)
{
  int width=0;
  unsigned short *body=ws->wsbody;
  int len=body[0];
  while(len)
  {
    width+=GetSymbolWidth(body[len],font);
    len--;
  }
  return (width+1);//хз, без этого тупит
}

const char per_t[]="%t";


extern const unsigned int X;
extern const unsigned int Y;
extern const int font;
extern const char col[];
extern const int align;
extern const int cfgDay;
extern const int cfgMonth;

extern const char TEXT_BEFORE[];
extern const char TEXT_AFTER[];

void *canvasdata;

int day_count[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

int getDayCount()
{
  TDate date;
  int i = 0;
  int c = 0;
  GetDateTime(&date,0);
  
  if(date.month == cfgMonth)
      c += cfgDay - date.day;
  else
  {
    c = day_count[date.month] - date.day;
  
    for(i = date.month ; i < cfgMonth - 1 ; i++)
      c += day_count[i];
    c += cfgDay;
  }  
  return c-1;  
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
  void *icsm;
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"Reminder_ms config updated!");
      InitConfig();      
    }
  }
  
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
  }
//  icsm=FindCSMbyID(CSM_root()->idle_id);
  
 
/*  if(msg->msg == MSG_IPC)
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
            icsm=FindCSMbyID(CSM_root()->idle_id);
            if (icsm && checkGui(idlegui_id(icsm)))
            {
               WSHDR *ws = AllocWS(256);
                //Здесь рисуем
                canvasdata = BuildCanvas();
                wsprintf(ws,per_t,getDayCount());
                DrawCanvas(canvasdata, X, Y, X+Get_WS_width(ws, font), Y+GetFontYSIZE(font), 1);                 
                DrawString(ws,X,Y,X+Get_WS_width(ws,font)+4, Y+GetFontYSIZE(font), font,0,col,GetPaletteAdrByColorIndex(23)); 
             }              
          break;
        }
      }
    }
  }*/
  

  icsm=FindCSMbyID(CSM_root()->idle_id);
  if (icsm && checkGui(idlegui_id(icsm)))              
 // if (icsm)
  {
   // if (IsGuiOnTop(idlegui_id(icsm))) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {        
        WSHDR *ws = AllocWS(256);        
     //   canvasdata = BuildCanvas();
        wsprintf(ws,"%t%d%t",TEXT_BEFORE, getDayCount(), TEXT_AFTER);
        DrawCanvas(canvasdata, X, Y, X+Get_WS_width(ws, font), Y+GetFontYSIZE(font), 1);                 
        DrawString(ws,X,Y,X+Get_WS_width(ws,font)+4, Y+GetFontYSIZE(font), font,align,col,GetPaletteAdrByColorIndex(23)); 
        FreeWS(ws);
      }
    }
  }
   
 return(1);
}

static void maincsm_oncreate(CSM_RAM *data)
{ 
  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}

void maincsm_onclose(CSM_RAM *data)
{ 
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

static unsigned short maincsm_name_body[140];
const int minus11=-11;

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
};


static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Reminder");
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

  return (0);
}
