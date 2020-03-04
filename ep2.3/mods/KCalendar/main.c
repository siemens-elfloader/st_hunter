#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "config_data.h"

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

char config_name[64];
  
const char *color(int i)
{
  switch(i)
  {
    case 1: return (const char*)CLR_1;
    case 2: return (const char*)CLR_2;
    case 3: return (const char*)CLR_3;
    case 4: return (const char*)CLR_4;
    case 5: return (const char*)CLR_5;
    case 6: return (const char*)CLR_6;
    case 7: return (const char*)CLR_7;
    case 8: return (const char*)CLR_8;
    case 9: return (const char*)CLR_9;
    case 10: return (const char*)CLR_10;
    case 11: return (const char*)CLR_11;
    case 12: return (const char*)CLR_12;
    case 13: return (const char*)CLR_13;
    case 14: return (const char*)CLR_14;
    case 15: return (const char*)CLR_15;
    case 16: return (const char*)CLR_16;
    case 17: return (const char*)CLR_17;
    case 18: return (const char*)CLR_18;
    case 19: return (const char*)CLR_19;
    case 20: return (const char*)CLR_20;
    case 21: return (const char*)CLR_21;
    case 22: return (const char*)CLR_22;
    case 23: return (const char*)CLR_23;
    case 24: return (const char*)CLR_24;
    case 25: return (const char*)CLR_25;
    case 26: return (const char*)CLR_26;
    case 27: return (const char*)CLR_27;
    case 28: return (const char*)CLR_28;
    case 29: return (const char*)CLR_29;
    case 30: return (const char*)CLR_30;
    case 31: return (const char*)CLR_31;
    default : return GetPaletteAdrByColorIndex(25);//прозрачный
    //case '': return (const char*)CLR_;
  }  
}



const char _s[]="%s";

#pragma inline=forced
int toupper(int c)
{
  if((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}

int GetWidth(char *s, int font)
{
  WSHDR *ws=AllocWS(strlen(s));
  wsprintf(ws,"%t",s);
  unsigned short *body=ws->wsbody;
  int len=body[0],width;
  if(len>0) width=1; else width=0;
  while(len>0)
  {
    width+=GetSymbolWidth(body[len],font);
    len--; 
  }
  FreeWS(ws);
  return (width+1);
}


IMGHDR DoScreen(int ScrH,int HEADERSIZE)
{
  IMGHDR screen={0,0,8,""};
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen.w=ScrW;
  screen.h=ScrH;  
  screen.bitmap=malloc(ScrW*ScrH*2);
  ms+=(HEADERSIZE*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
  return screen;
}


int count_days[]={31,28,31,30,31,30,31,31,30,31,30,31};
//int X_POS = ;
//int Y_POS = ;
int space = 0;

void DrawCal()
{
  TDate date;
  GetDateTime(&date,0);
  date.day = 1;// как бы первый день месяца, чтоб узнать какой день недели был первого числа месяца
  int daypos = GetWeek(&date);//номер первой позиции
//  int space = GetWidth("25",FONT);

  int pos_x = daypos * space + X_POS; //первая позиция по Х
  int pos_y = 5 + Y_POS; // первая позиция по У
  WSHDR *ws = AllocWS(32);
  GetDateTime(&date,0);
  
  for(int i = 0 ; i < count_days[date.month] + 1 ; i++)
  {
    //дата
    wsprintf(ws, "%02d/%02d/%04d",date.day, date.month,date.year);
    DrawString(ws,X_POS+2,Y_POS+1,X_POS+Get_WS_width(ws,FONT)+5/*((GetWidth("25",FONT) + 3)*7)*/,Y_POS+GetFontYSIZE(FONT)+1,FONT_SMALL,TEXT_OUTLINE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
    
    //цветной квадратик
    DrawRectangle(pos_x+2,pos_y+GetFontYSIZE(FONT),pos_x+space,pos_y+GetFontYSIZE(FONT)*2,0,0/*CLR_BACK*/,color(i+1));
    //день месяца
    wsprintf(ws,"%d",i+1);
    DrawString(ws,pos_x,pos_y+GetFontYSIZE(FONT),pos_x+space,pos_y+GetFontYSIZE(FONT)*2+1,FONT_SMALL,TEXT_OUTLINE+TEXT_ALIGNRIGHT,((i+1)==date.day) ? CLR_CURDAY : GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
    
    pos_x += space;//прибавили шаг
    daypos++;//позиция дня недели
    if(daypos >= 7)//если дошли до новой недели, то
    {
       pos_y += GetFontYSIZE(FONT)+1;// У смещаем вниз
       pos_x = X_POS;// Х на стартовую позицию
       daypos = 0;
    }
  }  
  FreeWS(ws);
}

void DrwImage(IMGHDR *img, int x, int y)
{
  if(!img) return;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0/*pen,brush*/);
  DrawObject(&drwobj);
}

IMGHDR Shot;
int len_line;
void MM_OnRedraw(MAIN_GUI *data)
{  
  DrwImage(&Shot,0,YDISP);
  DrawRoundedFrame(X_POS,Y_POS,X_POS + space*7+5,Y_POS+GetFontYSIZE(FONT)*8,5,5,0,CLR_BORD,CLR_BACK);
  DrawLine(X_POS,Y_POS+GetFontYSIZE(FONT)+3,X_POS+len_line + 10,Y_POS+GetFontYSIZE(FONT)+3,0,CLR_BORD);
  DrawCal();
}


//------------------------------------------


void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  int HSIZE = YDISP*ScreenW();
  int ScrH = ScreenH()-YDISP;
    
  Shot = DoScreen(ScrH, HSIZE);
  len_line = GetWidth("12/08/2009",FONT);
  space = GetWidth("25",FONT);
#ifdef ELKA
  space += 3;
#else
  space++;
#endif
  
  data->gui.state=1;
}

void MM_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  mfree(Shot.bitmap);
  data->gui.state=0;
}

void MM_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  DisableIDLETMR();
}

void MM_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int MM_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
 // DirectRedrawGUI(); 
//  REDRAW();
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    if(k==RED_BUTTON) return 1;
  }   
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int MM_method8(void){return(0);}
int MM_method9(void){return(0);}

static const void * const gui_methods[11]={
  (void *)MM_OnRedraw,
  (void *)MM_onCreate,
  (void *)MM_onClose,
  (void *)MM_onFocus,
  (void *)MM_onUnfocus,
  (void *)MM_OnKey,
  0,
  (void *)kill_data,
  (void *)MM_method8,
  (void *)MM_method9,
  0
};


const RECT Canvas={0,0,0,0};
static void maincsm_oncreate(CSM_RAM *data)
{

  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  //patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
//  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  
}


//---------------------------------------------------------------------------//

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}


void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)ElfKiller);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
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
      ShowMSG(1,(int)"KCalendar config updated!");
      InitConfig(config_name);
    }
  }

  return(1);
}



const int minus11=-11;
unsigned short maincsm_name_body[140];

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

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"kCalendar - (c)kluchnik");
}


int main(char *exename, char *fname)
{ 
  zeromem(config_name,sizeof(config_name));
  char *ptr = strrchr(exename,'\\')+1;
  memcpy(config_name, (char*)(exename+(strlen(exename)-strlen(ptr))),strlen(ptr)-4);
  
  InitConfig(config_name);
  MAIN_CSM main_csm;
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();

  return 0;
}
