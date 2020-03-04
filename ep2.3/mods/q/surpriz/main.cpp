#include "..\inc\swilib.h"
//#include "../inc/cfg_items.h"
//#include "conf_loader.h"

//-------------------------------------//

#define IPC_TEXTINFO_NAME "surpriz" 
#define IPC_UPDATE_STAT 1 

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ my_ipc={//эт структура ипц,в будущем отправим ее таску:)
  ipc_my_name,
  ipc_my_name,
  NULL
};

void TimerProc(void)
{  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);//эта херня отправляет ипц хтаску
}

//-------------------------------------//
//в принципе это тоже нужно для демона
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);

extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

//-------------------------------------//



#define TMR_SECOND 216
GBSTMR mytmr;
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

const char per_s[]="%s";
void *canvasdata;
char *s,*s1,*s2,*s3;
int X1=2,Y1=100;
int col=0;
GBSTMR tmr;
int str;
WSHDR *ws1;

int DrawChar(char *s,int str)
{
  if(col<23)col++; else col=0;
  wsprintf(ws1,per_t,s);
  DrawCanvas(canvasdata, X1+str+2, Y1, X1+str+Get_WS_width(ws1, FONT_MEDIUM), Y1+GetFontYSIZE(FONT_MEDIUM), 1);                 
  DrawString(ws1,X1+str+2,Y1,X1+str+Get_WS_width(ws1, FONT_MEDIUM)+4, Y1+GetFontYSIZE(FONT_MEDIUM), FONT_MEDIUM,0,GetPaletteAdrByColorIndex(col),GetPaletteAdrByColorIndex(23));                         
  return get_string_width(ws1,FONT_MEDIUM);
}
int w;
void idle()
{
  void *icsm;
  w=ScreenW();
  icsm=FindCSMbyID(CSM_root()->idle_id);  
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm))) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {

        canvasdata = BuildCanvas();
        str=DrawChar ("Д",2);
        str+=DrawChar("ю",str);        
        str+=DrawChar("х",str);
        str+=DrawChar("а",str);
        str+=10;
        str+=DrawChar("с",str);
        str+=10;
        str+=DrawChar("д",str);
        str+=DrawChar("н",str);
        str+=DrawChar("ю",str);
        str+=DrawChar("х",str);
        str+=DrawChar("о",str);
        str+=DrawChar("й",str);
        str+=DrawChar("!",str);
        str+=DrawChar("!",str);
        str+=DrawChar("!",str);
  
//        DrawRoundedFrame( X1-2,Y1-2,X1+str+2, Y1 + 2 + GetFontYSIZE(FONT_MEDIUM),0, 0, 0, GetPaletteAdrByColorIndex(col), GetPaletteAdrByColorIndex(23));        
        wsprintf(ws1,per_t,"От Олега с Пашкой!!!:)");
        DrawScrollString(ws1,X1+2,Y1+GetFontYSIZE(FONT_MEDIUM)+10,w,Y1+GetFontYSIZE(FONT_MEDIUM)+10+GetFontYSIZE(FONT_SMALL),1,FONT_SMALL,0,GetPaletteAdrByColorIndex(col),GetPaletteAdrByColorIndex(23));
      }
    }
  }
}

void check()
{
 idle();
 GBS_StartTimerProc(&tmr, 10*TMR_SECOND/10,check);
}
int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  if (msg->msg==MSG_IPC)//это проверка если через TimerProc() или при старте эльфа отправили запрос из эльфа,то выполняем команду
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
        case IPC_UPDATE_STAT://в нашем случае запускаем таймер для того точбы отправить структуру таску
          GBS_StartTimerProc(&mytmr, 10*TMR_SECOND/10, TimerProc);
        }
      }
    }
  }

 return(1);
}

static void maincsm_oncreate(CSM_RAM *data)
{ 
  ws1=AllocWS(256);
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);//При открытии эльфа отсылаем ипц чтобы таск понял что эльф запущен
}

void maincsm_onclose(CSM_RAM *data)
{ 
  GBS_DelTimer(&tmr);
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"surpriz");
}
// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
//-------------------------------------//
  CSMROOT *csmr;//А это самое интересное:) тут замуты с цсм для того чтобы он определялся как демон,А не просто процесс
  CSM_RAM *save_cmpc;//и голову ломать не стоит,прост копируешь отсюда и усе...
  CSM_RAM main_csm;
  UpdateCSMname();
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
//-------------------------------------//
  check();

  return (0);
}
