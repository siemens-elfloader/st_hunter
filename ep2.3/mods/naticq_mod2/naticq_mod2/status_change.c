/*
  Интерфейс и функции смены своего статуса
*/
#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "main.h"
#include "status_change.h"
#include "strings.h"
#include "language.h"
#include "print.h"
#include "configs.h"
//===============================================================================================
// ELKA Compatibility
#pragma inline
static void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC *)head)->rc.x=0;
  ((HEADER_DESC *)head)->rc.y=YDISP;
  ((HEADER_DESC *)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC *)head)->rc.y2=HeaderH()-1+YDISP;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC *)inp)->rc.x = 0;
  ((INPUTDIA_DESC *)inp)->rc.y = HeaderH() + 1 + YDISP;
  ((INPUTDIA_DESC *)inp)->rc.x2=ScreenW() - 1;
  ((INPUTDIA_DESC *)inp)->rc.y2=ScreenH() - SoftkeyH() - 1;
}
//===============================================================================================                                                                                                                                                                                                                                                                                                                    

extern char TEMPLATES_PATH[];
static const char XS_FILE_NAME[]="\\XStatuses.txt";


char *XStatusText;

extern int CurrentStatus;
extern int CurrentXStatus;
extern  int S_ICONS[];
#ifdef NEWSGOLD
extern void SetIconBarHandler();
#endif

extern int isAutoStatusOn;    
extern int msg_away_timer;
extern void away_timer();
extern int away_m;
extern unsigned int AUTO_ENGADE;
   
#pragma inline
static void Change_Status(int status)
{
  CurrentStatus=status;
  set_my_status();
#ifdef NEWSGOLD
  SetIconBarHandler();
#endif

  if(isAutoStatusOn!=12 && isAutoStatusOn==status)
  {
    msg_away_timer=(AUTO_ENGADE*12);
    away_m=1;
    away_timer();
  }
  else if(status==IS_ONLINE || status==IS_FFC || status== IS_HOME)
  {
    msg_away_timer=0;
    away_m=0;
  }
  
  SUBPROC((void*)WriteDefSettings);
}

#pragma inline
static void Change_XStatus(int xstatus)
{
  CurrentXStatus=xstatus;
  set_my_xstatus();
#ifdef NEWSGOLD
  SetIconBarHandler();
#endif
  SUBPROC((void*)WriteDefSettings);
}

///////////////////////////////////////////////////////////////////////////////
#pragma inline
static unsigned int GetStatusIndexInMenu(unsigned int status)
{
  switch(status)
  {
  case IS_ONLINE: {return 0;}
  case IS_AWAY: {return 1;}
  case IS_NA: {return 2;}
  case IS_DND: {return 3;}
  case IS_OCCUPIED: {return 4;}
  case IS_FFC: {return 5;}
  case IS_INVISIBLE: {return 6;}
  case IS_DEPRESSION: return 7;
  case IS_EVIL: return 8;
  case IS_HOME: return 9;
  case IS_LUNCH: return 10;
  case IS_WORK: return 11;
  }
  return 0;
}

#pragma inline 
static unsigned int GetStatusInMenuByPos(int pos)
{
  switch(pos)
  {
  case 0:
    return IS_ONLINE;
  case 1:
    return IS_AWAY;
  case 2:
    return IS_NA;
  case 3:
    return IS_DND;
  case 4:
    return IS_OCCUPIED;
  case 5:
    return IS_FFC;
  case 6:
    return IS_INVISIBLE;
  case 7:
    return IS_DEPRESSION;
  case 8:
    return IS_EVIL;
  case 9:
    return IS_HOME;
  case 10:
    return IS_LUNCH;
  case 11:
    return IS_WORK;
  }
  return IS_UNKNOWN;
}

static int st_onkey(void *data, GUI_MSG *msg)
{
  int i;
  if (msg->keys==0x18 || msg->keys==0x3D)
  {
    i=GetCurMenuItem(data);
    Change_Status(GetStatusInMenuByPos(i));
    return (1);
  }
  return (0);
}
    
///////////////////////////////////////////////////////////////////////////////

#define STATUSES_NUM 12

char *menutexts2[STATUSES_NUM]=
{
  LG_STONLINE,
  LG_STAWAY,
  LG_STNA,
  LG_STDND,
  LG_STOCCUP,
  LG_STFFC,
  LG_STINVIS,
  LG_STDEPRESSION,
  LG_STEVIL,
  LG_STHOME,
  LG_STLUNCH,
  LG_STWORK
};

void st_icons_set(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  extern const char percent_t[];
  ws=AllocMenuWS(data,strlen(menutexts2[curitem]));
  wsprintf(ws,percent_t,menutexts2[curitem]);
  SetMenuItemIconArray(data,item,S_ICONS+GetStatusInMenuByPos(curitem));
  SetMenuItemText(data, item, ws, curitem);
}

static const HEADER_DESC st_menuhdr={0,0,0,0,NULL,(int)LG_CHGSTATUS,LGP_NULL};

static const int st_menusoftkeys[]={0,1,2};

static const SOFTKEY_DESC st_menu_sk[]=
{
  {0x0018,0x0000,(int)LG_SELECT},
  {0x0001,0x0000,(int)LG_BACK},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB st_menu_skt=
{
  st_menu_sk,0
};

static void stmenu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

static const MENU_DESC st_tmenu=
{
  8,st_onkey,stmenu_ghook,NULL,
  st_menusoftkeys,
  &st_menu_skt,
  0x11,//MENU_FLAG,
  st_icons_set,
  NULL,//st_menuitems,
  NULL,
  STATUSES_NUM
};


void DispStatusChangeMenu()
{
  *((int **)(&st_menuhdr.icon))=S_ICONS+CurrentStatus;
  patch_header(&st_menuhdr);
  CreateMenu(0,0,&st_tmenu,&st_menuhdr,GetStatusIndexInMenu(CurrentStatus),STATUSES_NUM,0,0);
}



//int startItem=0;

extern char clrBlack[];
extern char clrWhite[];
extern char clrRed[];
extern char transparent[];
extern char blue[];
extern const int clfont;
extern IMGHDR *icons[];
//char clrRed2[]={0xFF,0x00,0x00,0x15};
int _itemFromDisp;//кол-во на экране
int _totalItems;//общее количество итемов
int _curpos=0;//позичия курсора
int _curpage=1;//текущая страница


extern void DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush);
RECT _rc;//
#define _font 108

static void SC_DrawList()
{
  int font_y = FontH(_font)+1;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
//  _itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(_font)-2)-1;
  
  LockSched();
  
  int i=0;
  int start_y=0;
  int start_x=_rc.x+2;
 
  char *s=malloc(128);
  RECT rc;
  while(i<STATUSES_NUM)
  {
   sprintf(s,"%s",menutexts2[i]);
   
   start_y = (_rc.y + 2 + (i - (_curpage-1)*_itemFromDisp)*font_y);    //определяем начальное смещение по у      
   StoreXYXYtoRECT(&rc,start_x,start_y,_rc.x2,start_y+font_y);            
   SetFontColor(clrWhite,transparent,0);           
   if(i==_curpos)//Если наш итем то выделяем
   {
     DrawRoundedFrame(rc.x,rc.y-1,rc.x2-5,rc.y2-2,0,0,0,clrWhite,transparent);//cursor        
   }
   DrwImage(icons[GetStatusInMenuByPos(i)],_rc.x+4,start_y,0,0);
   PrintText(rc.x+5+icons[GetStatusInMenuByPos(i)]->w,rc.y-1,s,0,_font,1,0,0,0,rc.x2-5);//item name
   i++;
   if(i >= STATUSES_NUM) break;
  }
  mfree(s);
  UnlockSched();  
}

// Управление курсором
void SC_MoveUp(int i)
{ 
  //if(_curpos > _itemFromDisp){ _curpos--; }
//  else
    if(_curpos > 0){ _curpos--; /*if(startItem>0) startItem--; */}
  else
  {
    _curpos=STATUSES_NUM-1;
    //startItem=STATUSES_NUM-_itemFromDisp;
  }
}


void SC_MoveDown(int i)
{
  //if(_curpos <_itemFromDisp - 5) _curpos++;
  //else
    if(_curpos < STATUSES_NUM - 1 ){/*if(startItem < STATUSES_NUM-mm_itemFromDisp) startItem++;*/ _curpos++; }
  else   
  {
    _curpos=0;
    //startItem=0;
  }
}  

extern IMGHDR screen;
extern void DrawSoftKeys(char *right,char *left);
extern void DoScreen();
IMGHDR *mmscreen;
extern void DrawContactList();
static void SC_OnRedraw(MAIN_GUI *data)
{
//  ClearScreen();
  LockSched();
 // DrawContactList();
//  DrwImage(&screen,0,0,0,0);
//  DrwImage(mmscreen,0,0,0,0);
  DrawRoundedFrame(_rc.x,_rc.y-2,_rc.x2,_rc.y2,0,0,0,clrWhite,clrBlack);//"\xF5\x22\x19\x64"
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  UnlockSched();
  SC_DrawList();
}

static void SC_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  //DoScreen();
  _rc.x=30;
  _rc.y=YDISP+2;
  _rc.x2=ScreenW()-53;
  _rc.y2=YDISP+16+(FontH(_font)*STATUSES_NUM);
  _curpos=GetStatusIndexInMenu(CurrentStatus);
  SetCanvasFlag(1);
  data->gui.state=1;
}

static void SC_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  //if(mmscreen) mfree(mmscreen->bitmap);
  data->gui.state=0;
}

static void SC_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  data->gui.state=2;
  DisableIDLETMR();
}

static void SC_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int SC_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   SC_MoveUp(0);   break;
      case DOWN_BUTTON: SC_MoveDown(0); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: return (1);
     // case ENTER_BUTTON: case LEFT_SOFT: ((void (*)(void))(menuprocs[mm_curpos]))(); REDRAW(); break;
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON: case LEFT_SOFT:case RIGHT_BUTTON:
          Change_Status(GetStatusInMenuByPos(_curpos));
          return (1);
    }
  }   

  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int SC_method8(void){return(0);}
int SC_method9(void){return(0);}

const void * const gui_methods_SC[11]={
  (void *)SC_OnRedraw,
  (void *)SC_onCreate,
  (void *)SC_onClose,
  (void *)SC_onFocus,
  (void *)SC_onUnfocus,
  (void *)SC_OnKey,
  0,
  (void *)kill_data,
  (void *)SC_method8,
  (void *)SC_method9,
  0
};

const RECT SC_Canvas={0,0,0,0};

void CreateSCMenu()
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&SC_Canvas);
  main_gui->gui.methods=(void *)gui_methods_SC;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();  
  CreateGUI(main_gui);   
}


//--------------------------------
// XStatuses
//--------------------------------
extern DYNPNGICONLIST *XStatusesImgList;
extern int *XStatusesIconArray;
extern volatile int total_xstatuses;

SOFTKEY_DESC edit_x_sk[]=
{
  {0x0018,0x0000,(int)"Save!"},
  {0x0001,0x0000,(int)""},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB edit_x_skt=
{
  edit_x_sk,0
};

void edit_xstatus_locret(void){}

void ReplaceStr(char **str, int start_prev, int prev_len, char *next, int next_len)
{
  char *d, *buf;
  char *s=*str;
  if (s)
  {
    d=buf=malloc(strlen(s)+next_len-prev_len+1);
    if (start_prev)
    {
      memcpy(buf, s, start_prev);
      buf+=start_prev;
    }
    if (next_len)
    {
      memcpy(buf,next,next_len);
      buf+=next_len;
    }
    strcpy(buf, s+start_prev+prev_len);
    mfree(s);
    *str=d;
  }
}

int edit_xstatus_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws;
  char *s, *d, tmp[132];
  int cur_x_st;
  int len, l;
  int is_any_change=0;
  if (msg->keys==0xFFF)
  {
    cur_x_st=(int)EDIT_GetUserPointer(data);
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);   // Комментарий
    ws=ec.pWS;
    
    s=GetXStatusStr(cur_x_st*3,&len);
    d=tmp;

    if (s)
    {
      l=ws->wsbody[0];
      for (int i=0; i<l; i++) 
      {
        *d++=char16to8(ws->wsbody[i+1]);
      }
      *d=0;
      if (len!=l || strncmp(s,tmp,len))
      {
        ReplaceStr(&XStatusText,s-XStatusText,len,tmp,l);
        is_any_change=1;
      }
    }

    ExtractEditControl(data,4,&ec);   // Короткий икс статус
    ws=ec.pWS;
    
    s=GetXStatusStr(cur_x_st*3+1,&len);
    d=tmp;

    if (s)
    {
      l=ws->wsbody[0];
      for (int i=0; i<l; i++) 
      {
        *d++=char16to8(ws->wsbody[i+1]);
      }
      *d=0;
      if (len!=l || strncmp(s,tmp,len))
      {
        ReplaceStr(&XStatusText,s-XStatusText,len,tmp,l);
        is_any_change=1;
      }
    }
    
    ExtractEditControl(data,6,&ec);   // Длинный икс статус
    ws=ec.pWS;
    
    s=GetXStatusStr(cur_x_st*3+2,&len);
    d=tmp;

    if (s)
    {
      l=ws->wsbody[0];
      for (int i=0; i<l; i++) 
      {
        *d++=char16to8(ws->wsbody[i+1]);
      }
      *d=0;
      if (len!=l || strncmp(s,tmp,len))
      {
        ReplaceStr(&XStatusText,s-XStatusText,len,tmp,l);
        is_any_change=1;
      }
    }
    
    if (is_any_change)
    {
      SaveXStatusText();
    }
    return (1);
  }
  return (0);
}

void edit_xstatus_ghook(GUI *data, int cmd)
{
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Save!"};
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
  if (cmd==7)
  {
    SetSoftKey(data,&sk,SET_SOFT_KEY_N);
  }
}

HEADER_DESC edit_xstatus_hdr={0,0,0,0,NULL,(int)"X-Статус",LGP_NULL};

INPUTDIA_DESC edit_xstatus_desc =
{
  1,
  edit_xstatus_onkey,
  edit_xstatus_ghook,
  (void *)edit_xstatus_locret,
  0,
  &edit_x_skt,
  {0,0,0,0},
  FONT_SMALL,
  100,
  101,
  0,
  //  0x00000001 - Выровнять по правому краю
  //  0x00000002 - Выровнять по центру
  //  0x00000004 - Инверсия знакомест
  //  0x00000008 - UnderLine
  //  0x00000020 - Не переносить слова
  //  0x00000200 - bold
  0,
  //  0x00000002 - ReadOnly
  //  0x00000004 - Не двигается курсор
  //  0x40000000 - Поменять местами софт-кнопки
  0x40000000
};


static void EditXStatus(int xstatus)
{
  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  char *s;
  int len;
  int c;
   
  eq=AllocEQueue(ma,mfree_adr());    
  WSHDR *ews=AllocWS(128);
  ascii2ws(ews,"Комментарий:");   // Коментарий
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);    //1
  
  CutWSTR(ews,0);
  s=GetXStatusStr(xstatus*3, &len);
  if (s)
  {
    while((c=*s++) && len)
    {
      wsAppendChar(ews,char8to16(c));
      len--;
    }
  }
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,20);
  AddEditControlToEditQend(eq,&ec,ma);   //2
  
  ascii2ws(ews,"Short X:");      // Короткий Икс Статус
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   //3
  
  CutWSTR(ews,0);
  s=GetXStatusStr(xstatus*3+1, &len);
  if (s)
  {
    while((c=*s++) && len)
    {
      wsAppendChar(ews,char8to16(c));
      len--;
    }
  }
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);   //4
  
  ascii2ws(ews,"Large X:");     // Длинный Икс Статус
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   //5
  
  CutWSTR(ews,0);
  s=GetXStatusStr(xstatus*3+2, &len);
  if (s)
  {
    while((c=*s++) && len)
    {
      wsAppendChar(ews,char8to16(c));
      len--;
    }
  }
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);   //6
  
  FreeWS(ews);
  patch_header(&edit_xstatus_hdr);
  patch_input(&edit_xstatus_desc);
  CreateInputTextDialog(&edit_xstatus_desc,&edit_xstatus_hdr,eq,1,(void *)xstatus);  
}

extern int ind_set_xstatus;
extern int Is_Vibra_Enabled;
extern int Is_Id3tag_Enabled;
extern unsigned int Is_Sounds_Enabled;
extern int Is_Show_Offline;
extern int Is_Show_Groups;    
#ifdef ELKA
extern int Is_SLI_On;//мигаем светодиодом
#endif    
extern int Is_SMARTCPU_On;//Вывод сообщений на гэ
extern int Is_INC_On;//Popup message
extern int Is_ILU_On;//Режим подсветки
extern int Is_Double_Vibra_On;//Режим напоминания
extern DEF_SETTINGS2 def_set[];
extern int Is_PING_On;
extern int Is_UPTIME_On;
extern int Show_XTX;

    
static int xst_onkey(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  if (msg->keys==0x18)
  {
    EditXStatus(i);
    return(0);
  }
  if (msg->keys==0x3D)
  {
  
    Change_XStatus(i);
if(ind_set_xstatus)
{
    Is_Vibra_Enabled=def_set[i].vibra_status;
#ifdef NEWSGOLD
    Is_Id3tag_Enabled=def_set[i].Id3tag_status;
#endif
    Is_Sounds_Enabled=def_set[i].sound_status;
#ifdef ELKA
    Is_SLI_On=def_set[i].sli_state;
#endif
    Is_SMARTCPU_On=def_set[i].smartcpu_state;
    Is_INC_On=def_set[i].inc_state;
    Is_ILU_On=def_set[i].ilu_state;
    Is_PING_On=def_set[i].ping_state;
    Is_UPTIME_On=def_set[i].uptime_state;
    Is_Show_Offline=def_set[i].off_contacts;
    Is_Show_Groups=def_set[i].show_groups;
    Is_Double_Vibra_On=def_set[i].remind_state;
    Show_XTX=def_set[i].isXTX;//Показывать хстатус или время он лайн 
}
    return(1);
  }
  return (0);
}

static void xst_ghook(void *data, int cmd)
{
  PNGTOP_DESC *pltop=PNG_TOP();
  if (cmd==9)
  {
    pltop->dyn_pltop=NULL;
  }
  if (cmd==0x0A)
  {
    pltop->dyn_pltop=XStatusesImgList;
    DisableIDLETMR();
    //Загрузка текста при фокусе
    LoadXStatusText();
  }
}

static void xst_itemh(void *data, int curitem, void *unk)
{
  void *item=AllocMLMenuItem(data);
  WSHDR ws1loc, *ws1, *ws2;
  unsigned short num[128];
  WSHDR ws3loc, *ws3, *ws4;
  unsigned short num3[128];
  char ss[128];
  char *s;
  int l;
  extern const char percent_t[];
  
  ws1=CreateLocalWS(&ws1loc,num,128);
  ws3=CreateLocalWS(&ws3loc,num3,128);
  
  if (s=GetXStatusStr(curitem*3+1,&l))
  {
    memset(ss,0,128);
    if (l>127) l=127;
    strncpy(ss,s,l);
    wsprintf(ws1,percent_t,ss);
  }
  else
  {
    wsprintf(ws1,"Short XS %d",curitem);
  }
  if (s=GetXStatusStr(curitem*3+2,&l))
  {
    memset(ss,0,128);
    if (l>127) l=127;
    strncpy(ss,s,l);
    wsprintf(ws3,percent_t,ss);
  }
  else
  {
    wsprintf(ws3,"Large XS %d",curitem);
  }
  ws2=AllocMenuWS(data,ws1->wsbody[0]);

  wstrcpy(ws2,ws1);
  ws4=AllocMenuWS(data,ws3->wsbody[0]);
  wstrcpy(ws4,ws3);
  SetMenuItemIconArray(data, item, XStatusesIconArray+curitem);
  SetMLMenuItemText(data, item, ws2, ws4, curitem);
}

static const SOFTKEY_DESC xst_menu_sk[]=
{
  {0x0018,0x0000,(int)LG_TEXT},
  {0x0001,0x0000,(int)LG_BACK},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB xst_menu_skt=
{
  xst_menu_sk,0
};

static const HEADER_DESC xst_menuhdr={0,0,0,0,NULL,(int)LG_CHGXSTATUS,LGP_NULL};

static const ML_MENU_DESC xst_menu=
{
  8,xst_onkey,xst_ghook,NULL,
  st_menusoftkeys,
  &xst_menu_skt,
  0x11,
  xst_itemh,
  NULL,
  NULL,
  0,
  1
};

void DispXStatusChangeMenu(void)
{
  patch_header(&xst_menuhdr);
  *((int **)(&xst_menuhdr.icon))=XStatusesIconArray+CurrentXStatus;
  CreateMultiLinesMenu(0,0,&xst_menu,&xst_menuhdr,CurrentXStatus,total_xstatuses);
}

void LoadXStatusText(void)
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int fsize;
  char *p, *s;
  int c;
  FreeXStatusText();
  strcpy(fn,TEMPLATES_PATH);
  strcat(fn,XS_FILE_NAME);
  if (GetFileStats(fn,&stat,&ul)==-1) return;
  if ((fsize=stat.size)<=0) return;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return;
  p=XStatusText=malloc(fsize+2);
  s=p+1;
  s[fread(f,s,fsize,&ul)]=0;
  fclose(f,&ul);
  while((c=*s++)!=0)
  {
    if (c==10)
    {
      c=13;
      if (*s==13) s++;   // пропускаем
    }
    else if (c==13)
    {
      if (*s==10) s++;  // пропускаем
    }
    *p++=c;
  }
  *p=0;
}

void SaveXStatusText(void)
{
  int f;
  char fn[256];
  unsigned int ul;
  if (!XStatusText) return;
  strcpy(fn,TEMPLATES_PATH);
  strcat(fn,XS_FILE_NAME);
  if ((f=fopen(fn,A_ReadWrite+A_Create+A_Truncate+A_BIN,P_READ+P_WRITE,&ul))==-1) return;
  fwrite(f,XStatusText,strlen(XStatusText),&ul);
  fclose(f,&ul);
}

void FreeXStatusText(void)
{
  mfree(XStatusText);
  XStatusText=NULL;
}

char *GetXStatusStr(int n, int *len)
{
  int l=0;
  int c;
  char *p=XStatusText;
  char *pp;
  if (len) *len=0;
  if (!p) return p;
  while(n)
  {
    while((c=*p++)>=32);
    if (!c) return NULL;
    n--;
  }
  pp=p;
  while((*pp++)>=32) l++;
  if (len) *len=l;
  return p;
}



int xstartItem=0;
int xs_itemFromDisp;//кол-во на экране
//int xs_totalItems;//общее количество итемов
int xs_curpos=0;//позичия курсора
//int xs_curpage=1;//текущая страница


RECT xs_rc;//MainMenu Rect
#define xs_font 108

#define CLIST_Y1 (YDISP+2)

void DrawXScrollBar(int cur,int total)
{
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  int y=xs_rc.y2-xs_rc.y;//scr_h-CLIST_Y1-FontH(clfont)-2;
  int pos=y*cur/total;
  int y2=y/total;
 // int i=0;
  
  DrawLine(xs_rc.x2-2,xs_rc.y,xs_rc.x2-2,xs_rc.y2,1,clrWhite);
  DrawRoundedFrame(xs_rc.x2-3,xs_rc.y+pos+1,xs_rc.x2-1,xs_rc.y+pos+y2,0,0,0,clrWhite,clrWhite);
}

extern const char percent_s[];
/*
typedef struct
{
 char *xshort;
 char *xlong;
}XSTTEXT;

XSTTEXT xtexttop;
*/
DYNPNGICONLIST *GetXSByItem(int curitem)
{
  PNGTOP_DESC *pltop=PNG_TOP();
  pltop->dyn_pltop=XStatusesImgList;
  int i=total_xstatuses-1;
  while(pltop->dyn_pltop && i>0)
  {
    if(i==curitem)
      return pltop->dyn_pltop;
    i--;
    pltop->dyn_pltop=pltop->dyn_pltop->next;  
  }
  return 0;
}

static void XS_DrawList()
{
  int font_y = FontH(xs_font)+2;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  xs_itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(xs_font)-2)-1;  
  int l;    
  
  LockSched();  
  int i=0;
  int start_y=0;
  char *xshort=malloc(128);
  char *s;
  char *xlong=malloc(128);
  
  RECT rc;
  DYNPNGICONLIST *xs;
  
  while(i<xs_itemFromDisp)
  {  
    if(s=GetXStatusStr((xstartItem+i)*3+1,&l))
    {
     snprintf(xshort,128,percent_s,s);
    }
    else  
    {
      snprintf(xshort,128,"Short XS %d",xstartItem+i);
    }
    
    if (s=GetXStatusStr((xstartItem+i)*3+2,&l))
    {    
     snprintf(xlong,128,percent_s,s);
    }
    else
    {
      sprintf(xlong,"Large XS %d",(xstartItem+i));
    }
  
   if(i+xstartItem==0) snprintf(xshort,128,"Без статуса");

   start_y=0;

   if(i+xstartItem > xs_curpos) start_y+=font_y+3;
   
   start_y += (xs_rc.y + 2 + (i/* - (xs_curpage-1)*xs_itemFromDisp*/)*font_y);    //определяем начальное смещение по у         
   StoreXYXYtoRECT(&rc,xs_rc.x,start_y,xs_rc.x2-2,start_y+font_y);
   
   SetFontColor(clrWhite,transparent,0);           
   if(i+xstartItem==xs_curpos)//Если наш итем то выделяем
    {
     DrawRoundedFrame(rc.x+1,rc.y-1,rc.x2-5,rc.y2+font_y-1,0,0,0,clrWhite,transparent);//cursor        
     PrintText(rc.x+18,rc.y+font_y,xlong,0,xs_font,1,350,0,0,xs_rc.x2-xs_rc.x-22);//description under item name
     DrawXScrollBar(xstartItem+i,total_xstatuses);
    }
//   DrawImg(xs_rc.x+4,start_y,NumXStatusToIconBar+xstartItem+i);
   xs=GetXSByItem(i+xstartItem);
   if(xs)
   {
     DrwImage(xs->img,xs_rc.x+4,start_y,0,0);
     rc.x+=xs->img->w;
   }
   PrintText(rc.x+6,rc.y-1,xshort,0,xs_font,1,350,0,0,xs_rc.x2-xs_rc.x-10);//item name
   i++;
   if(xstartItem+i >= total_xstatuses) break;
  }
//  mfree(s);
  UnlockSched();  
  mfree(xshort);
  mfree(xlong);
}

// Управление курсором
void XS_MoveUp(int i)
{
  if(xs_curpos > xs_itemFromDisp + 2 ){ xs_curpos--; }
  else
    if(xs_curpos > 0){ xs_curpos--; if(xstartItem>0) xstartItem--; }
  else
  {
    xs_curpos=total_xstatuses-1;
    xstartItem=total_xstatuses-xs_itemFromDisp;
  }
}


void XS_MoveDown(int i)
{
  if(xs_curpos < xs_itemFromDisp - 5) xs_curpos++;
  else
    if(xs_curpos < total_xstatuses - 1 )
    {
      if(xstartItem < total_xstatuses-xs_itemFromDisp)
        xstartItem++;
      xs_curpos++; 
    }
  else   
  {
    xs_curpos=0;
    xstartItem=0;
  }
}

static void XS_OnRedraw(MAIN_GUI *data)
{
  LockSched();
  //DrwImage(&screen,0,0,0,0);//"\x00\x00\x00\x45"
 // DrawRoundedFrame(0,xs_rc.y-2,ScreenW()-1,xs_rc.y2,0,0,0,"\xFF\xFF\xFF\x15","\xFF\xFF\xFF\x15");
  DrawRoundedFrame(xs_rc.x,xs_rc.y-2,xs_rc.x2,xs_rc.y2,0,0,0,clrWhite,clrBlack);
  DrawSoftKeys(LG_TEXT,LG_CLOSE);
  XS_DrawList();
}

static void XS_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
//  DoScreen();
  xs_rc.x=30;
  xs_rc.y=YDISP+2;
  xs_rc.x2=ScreenW()-10;
  xs_rc.y2=ScreenH()-FontH(clfont)-3;
 // xs_curpos=CurrentXStatus;
  //int i=sdiv(xs_itemFromDisp,total_xstatuses-5);
//  xstartItem=sdiv(i,CurrentXStaus);
  SetCanvasFlag(1);
  data->gui.state=1;
}

static void XS_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  //mfree(screen.bitmap);
//  zeromem(screen.bitmap,(screen.w*screen.h*2));
  data->gui.state=0;
}

static void XS_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  data->gui.state=2;
  DisableIDLETMR();
}

static void XS_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int XS_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   XS_MoveUp(0);   break;
      case DOWN_BUTTON: XS_MoveDown(0); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: return (1);
     // case ENTER_BUTTON: case LEFT_SOFT: ((void (*)(void))(menuprocs[mm_curpos]))(); REDRAW(); break;
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON:
          Change_XStatus(xs_curpos);
          if(ind_set_xstatus)
          {
              int i=xs_curpos;
              Is_Vibra_Enabled=def_set[i].vibra_status;
          #ifdef NEWSGOLD
              Is_Id3tag_Enabled=def_set[i].Id3tag_status;
          #endif
              Is_Sounds_Enabled=def_set[i].sound_status;
          #ifdef ELKA
              Is_SLI_On=def_set[i].sli_state;
          #endif
              Is_SMARTCPU_On=def_set[i].smartcpu_state;
              Is_INC_On=def_set[i].inc_state;
              Is_ILU_On=def_set[i].ilu_state;
              Is_PING_On=def_set[i].ping_state;
              Is_UPTIME_On=def_set[i].uptime_state;
              Is_Show_Offline=def_set[i].off_contacts;
              Is_Show_Groups=def_set[i].show_groups;
              Is_Double_Vibra_On=def_set[i].remind_state;
              Show_XTX=def_set[i].isXTX;//Показывать хстатус или время он лайн 
          }
          return(1);  
      case LEFT_SOFT: case RIGHT_BUTTON:
          EditXStatus(xs_curpos);
          REDRAW();
          break;
    }
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int XS_method8(void){return(0);}
int XS_method9(void){return(0);}

const void * const gui_methods_XS[11]={
  (void *)XS_OnRedraw,
  (void *)XS_onCreate,
  (void *)XS_onClose,
  (void *)XS_onFocus,
  (void *)XS_onUnfocus,
  (void *)XS_OnKey,
  0,
  (void *)kill_data,
  (void *)XS_method8,
  (void *)XS_method9,
  0
};

const RECT XS_Canvas={0,0,0,0};

void CreateXSMenu()
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&XS_Canvas);
  main_gui->gui.methods=(void *)gui_methods_XS;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  CreateGUI(main_gui);   
}
