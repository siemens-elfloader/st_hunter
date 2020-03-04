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
#include "lang.h"
#include "rect_patcher.h"
#ifdef GUI_INTERFACE
  #include "externals.h"
#endif

//===============================================================================================
/*
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
}*/
//===============================================================================================

extern const char TEMPLATES_PATH[];
static const char XS_FILE_NAME[]="\\XStatuses.txt";


char *XStatusText;

extern int CurrentStatus;
extern int CurrentXStatus;
extern  int S_ICONS[];

#pragma inline
static void Change_Status(int status)
{
  CurrentStatus=status;
  set_my_status();
  SUBPROC((void*)WriteDefSettings);
}

#pragma inline
static void Change_XStatus(int xstatus)
{
  CurrentXStatus=xstatus;
  set_my_xstatus();
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

#define STATUSES_NUM 12

static char * menutexts[STATUSES_NUM]=
{
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};


#ifndef GUI_INTERFACE
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

void st_icons_set(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  extern const char percent_t[];
  ws=AllocMenuWS(data,strlen(menutexts[curitem]));
  wsprintf(ws,percent_t,menutexts[curitem]);
  SetMenuItemIconArray(data,item,S_ICONS+GetStatusInMenuByPos(curitem));
  SetMenuItemText(data, item, ws, curitem);
}

static HEADER_DESC st_menuhdr={0,0,0,0,NULL,NULL,LGP_NULL};

static const int st_menusoftkeys[]={0,1,2};

static SOFTKEY_DESC st_menu_sk[]=
{
  {0x0018,0x0000,NULL},
  {0x0001,0x0000,NULL},
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
  
  //Инициализация ленгпака
  st_menuhdr.lgp_id=(int)lgpData[LGP_ChgStatus];
  st_menu_sk[0].lgp_id=(int)lgpData[LGP_Select];
  st_menu_sk[1].lgp_id=(int)lgpData[LGP_Back];
  
  menutexts[0]=(char *)lgpData[LGP_StOnline];
  menutexts[1]=(char *)lgpData[LGP_StAway];
  menutexts[2]=(char *)lgpData[LGP_StNa];
  menutexts[3]=(char *)lgpData[LGP_StDnd];
  menutexts[4]=(char *)lgpData[LGP_StOccup];
  menutexts[5]=(char *)lgpData[LGP_StFfc];
  menutexts[6]=(char *)lgpData[LGP_StInvis];
  menutexts[7]=(char *)lgpData[LGP_StDepression];
  menutexts[8]=(char *)lgpData[LGP_StEvil];
  menutexts[9]=(char *)lgpData[LGP_StHome];
  menutexts[10]=(char *)lgpData[LGP_StLunch];
  menutexts[11]=(char *)lgpData[LGP_StWork];
  
  CreateMenu(0,0,&st_tmenu,&st_menuhdr,GetStatusIndexInMenu(CurrentStatus),STATUSES_NUM,0,0);
}


//------------------------------------------


#else //#ifndef GUI


//------------------------------------------


static GUIMENU stmenu;
extern void InitMMenu();
int XStatusChange_ID;


//------------------------------------------


void SC_DrawList()
{
  int font_y = FontH(MMFont)+1;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  
  LockSched();
  
  int i=0;
  int start_y=0;
  int start_x=stmenu.rc.x+2;
 
  RECT rc;
#ifdef ELKA
  stmenu.itemFromDisp = sdiv( FontH(MMFont) , stmenu.rc.y2-stmenu.rc.y );  
#else
  stmenu.itemFromDisp = sdiv( FontH(MMFont) , stmenu.rc.y2-stmenu.rc.y ) - 3;  
#endif
  
  if(stmenu.itemFromDisp > STATUSES_NUM) stmenu.itemFromDisp = STATUSES_NUM;
  
  if( font_y < GetIconByItem(GetStatusInMenuByPos(stmenu.curpos))->h )
    font_y = GetIconByItem(GetStatusInMenuByPos(stmenu.curpos))->h + 2;
  
  while(i<stmenu.itemFromDisp)
  {    
    start_y = (stmenu.rc.y + 2 + (i*font_y));
    StoreXYXYtoRECT(&rc,start_x,start_y,stmenu.rc.x2,start_y+font_y);            
    if(i+stmenu.startItem==stmenu.curpos)
      DrawRoundedFrame(rc.x,rc.y-1,rc.x2-5,rc.y2-2,0,0,0,MMCursorClr,MMCursorBGClr);
   
    DrwImage(GetIconByItem(GetStatusInMenuByPos(i+stmenu.startItem)),stmenu.rc.x+4,start_y,0);
    DrawStr(menutexts[i+stmenu.startItem],rc.x+5+GetIconByItem(GetStatusInMenuByPos(i+stmenu.startItem))->w,rc.y,rc.x2-5,rc.y2,0,MMFont,(char*)MMFontClr,1);
    i++;
    if(i+stmenu.startItem >= STATUSES_NUM) break;
  }
  UnlockSched();  
}


//------------------------------------------


// Управление курсором
void SC_MoveUp(int i)
{
  if(stmenu.curpos > stmenu.itemFromDisp + 2 ){ stmenu.curpos--; }
  else
    if(stmenu.curpos > 0){ stmenu.curpos--; if(stmenu.startItem>0) stmenu.startItem--; }
  else
  {
    stmenu.curpos=STATUSES_NUM-1;
    stmenu.startItem=STATUSES_NUM-stmenu.itemFromDisp;
  }
}


void SC_MoveDown(int i)
{  
  if(stmenu.curpos <stmenu.itemFromDisp - 5) stmenu.curpos++;
  else
    if(stmenu.curpos < STATUSES_NUM - 1 ){ if(stmenu.startItem < STATUSES_NUM-stmenu.itemFromDisp) stmenu.startItem++; stmenu.curpos++; }
  else   
  {
    stmenu.curpos=0;
    stmenu.startItem=0;
  }
}  


//------------------------------------------


IMGHDR SCScreen;

void SC_OnRedraw(MAIN_GUI *data)
{
  LockSched();
  DrwImage(&SCScreen,0,HeaderY+2,0);
  DrawRoundedFrame(stmenu.rc.x,stmenu.rc.y-2,stmenu.rc.x2,stmenu.rc.y2,0,0,0,MMBackgrBordClr,MMBackgrClr);//"\xF5\x22\x19\x64"
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  UnlockSched();
  SC_DrawList();
}

void SC_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif

  int HSIZE = (HeaderY+2)*ScreenW();
  int ScrH=ScreenH()-(HeaderY+2);
  SCScreen = DoScreen(ScrH,HSIZE);
  
  stmenu.rc=SRect;
//  stmenu.rc.y2=stmenu.rc.y+(FontH(MMFont)*(STATUSES_NUM+1));
  stmenu.curpos=GetStatusIndexInMenu(CurrentStatus);

  data->gui.state=1;
}

void SC_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if(SCScreen.bitmap) mfree(SCScreen.bitmap);
  InitMMenu();
  data->gui.state=0;
}

void SC_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  data->gui.state=2;
  DisableIDLETMR();
}

void SC_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int SC_OnKey(MAIN_GUI *data, GUI_MSG *msg)
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
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON: case LEFT_SOFT:case RIGHT_BUTTON:
          Change_Status(GetStatusInMenuByPos(stmenu.curpos));
          DrawHeader();
          return (1);
    }

   if(k>='0' && k<='9')
    {
      if(k=='0') stmenu.curpos=9;
      else stmenu.curpos=k-'1';
      Change_Status(GetStatusInMenuByPos(stmenu.curpos));
      return 1;
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
  
  //Инициализация ленгпака  
  menutexts[0]=(char *)lgpData[LGP_StOnline];
  menutexts[1]=(char *)lgpData[LGP_StAway];
  menutexts[2]=(char *)lgpData[LGP_StNa];
  menutexts[3]=(char *)lgpData[LGP_StDnd];
  menutexts[4]=(char *)lgpData[LGP_StOccup];
  menutexts[5]=(char *)lgpData[LGP_StFfc];
  menutexts[6]=(char *)lgpData[LGP_StInvis];
  menutexts[7]=(char *)lgpData[LGP_StDepression];
  menutexts[8]=(char *)lgpData[LGP_StEvil];
  menutexts[9]=(char *)lgpData[LGP_StHome];
  menutexts[10]=(char *)lgpData[LGP_StLunch];
  menutexts[11]=(char *)lgpData[LGP_StWork];
  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&SC_Canvas);
  main_gui->gui.methods=(void *)gui_methods_SC;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();  
  CreateGUI(main_gui);   
}

#endif //GUI



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
#ifdef GUI_INTERFACE
      void InitXText();
      InitXText();
#endif
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

HEADER_DESC edit_xstatus_hdr={0,0,0,0,NULL,NULL,LGP_NULL};

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
  ascii2ws(ews, (char*)lgpData[LGP_Comment] );   // Коментарий
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

#ifndef GUI_INTERFACE
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

static SOFTKEY_DESC xst_menu_sk[]=
{
  {0x0018,0x0000,NULL},
  {0x0001,0x0000,NULL},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB xst_menu_skt=
{
  xst_menu_sk,0
};

static HEADER_DESC xst_menuhdr={0,0,0,0,NULL,NULL,LGP_NULL};

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
  
  //Инициализация ленгпака
  xst_menuhdr.lgp_id=(int)lgpData[LGP_ChgXStatus];
  edit_xstatus_hdr.lgp_id=(int)lgpData[LGP_MnuXStatus];
  xst_menu_sk[0].lgp_id=(int)lgpData[LGP_Text];
  xst_menu_sk[1].lgp_id=(int)lgpData[LGP_Back];
 
  CreateMultiLinesMenu(0,0,&xst_menu,&xst_menuhdr,CurrentXStatus,total_xstatuses);
}


//------------------------------------------


#else


//------------------------------------------

static GUIMENU xstmenu;

//------------------------------------------


void DrawXScrollBar(int cur,int total)
{
  int scr_h = ScreenH()-1;
  int scr_w = ScreenW()-1;
  int y = xstmenu.rc.y2 - xstmenu.rc.y;
  int pos = y*cur/total;
  int y2 = y/total;
  
  DrawLine(xstmenu.rc.x2-2,xstmenu.rc.y,xstmenu.rc.x2-2,xstmenu.rc.y2,1,clrWhite);
  DrawRoundedFrame(xstmenu.rc.x2-3,xstmenu.rc.y+pos+1,xstmenu.rc.x2-1,xstmenu.rc.y+pos+y2,0,0,0,clrWhite,clrWhite);
}


//------------------------------------------


extern const char _s[];

typedef struct
{
 void *next;
 char *xshort;
 char *xlong;
}XSTTEXT;

volatile XSTTEXT *xtexttop;

void FreeXSTList()
{  
  LockSched();
  XSTTEXT *bmk=(XSTTEXT *)xtexttop;
  xtexttop=0;
  UnlockSched();
  while(bmk)
  {
    XSTTEXT *bmk_prev=bmk;
    bmk=bmk->next;
    mfree(bmk_prev);
  }
}

extern int GetWidth(char *s, int font);

void InitXText()
{
  FreeXSTList();
  char *s;
  int l;
  for(int i=0;i<total_xstatuses;i++)
  {
     XSTTEXT *bmk=malloc(sizeof(XSTTEXT));
     bmk->next=0;
     bmk->xlong=malloc(128);
     bmk->xshort=malloc(128);
     
     if(i!=0)
     {
      if(s=GetXStatusStr(i*3+1,&l))
        snprintf(bmk->xshort,128,_s,s);    
      else  
        snprintf(bmk->xshort,128,"Short XS %d",i);

      if(s=GetXStatusStr(i*3+2,&l))
      {
        /*
        xstmenu.img = GetXIconByItem(1);
        int len = strlen(s);
        //отрезаем строку для вывода
        while( GetWidth(s,MMFont) > (xstmenu.img->w - (XSRect.x2-XSRect.x-5)) )
        {
          s[len--] = 0;
        }*/
        
        snprintf(bmk->xlong,128,_s,s);
      }      
      else
       sprintf(bmk->xlong,"Large XS %d",(i));    
     }
     
     if(!xtexttop)
     {
       snprintf(bmk->xshort,128,"Без xстатуса");
       bmk->xlong="";
       xtexttop=bmk; 
     }
     else
     {
       XSTTEXT *bm=(XSTTEXT *)xtexttop;
       while(bm->next)
           bm=bm->next;
       bm->next=bmk;   
     } 
  }
}

XSTTEXT *GetXSTByItem(int curitem)
{
  XSTTEXT *bmk;
  bmk=(XSTTEXT *)xtexttop;
  int i=0;
  while(bmk)
  {
    if(i==curitem) return bmk;
    i++;
    bmk=bmk->next;  
  }
  return 0;
}

void XS_DrawList()
{
  int font_y = FontH(MMFont)+1;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
#ifdef ELKA
  xstmenu.itemFromDisp = sdiv(font_y,(xstmenu.rc.y2-xstmenu.rc.y));  
  if(ShowXStatusSecLine) xstmenu.itemFromDisp--;
#else
  xstmenu.itemFromDisp = sdiv(font_y,(xstmenu.rc.y2-xstmenu.rc.y))-3;  
#endif
  
  if( GetXIconByItem(1)->h > font_y ) font_y = GetIconByItem(1)->h + 2;
  
  LockSched();  
  int i=0;
  int start_y=0;
  //char *xshort=malloc(128);
 // char *s;
//  char *xlong=malloc(128);
//  int l;
  XSTTEXT *xst;
   
  RECT rc;  
  int x=0;
  while(i<xstmenu.itemFromDisp)
  {
     xst=GetXSTByItem(xstmenu.startItem+i);
     if(!xst) break;
      /*if(s=GetXStatusStr((xstmenu.startItem+i)*3+1,&l))
       snprintf(xshort,128,_s,s);    
      else  
        snprintf(xshort,128,"Short XS %d",xstmenu.startItem+i);
     
      if(ShowXStatusSecLine)
      {
         if(s=GetXStatusStr((xstmenu.startItem+i)*3+2,&l))
          snprintf(xlong,128,_s,s);
         else
         sprintf(xlong,"Large XS %d",(xstmenu.startItem+i));    
      }*/
//     if(i+xstmenu.startItem==0) snprintf(xshort,128,"Без xстатуса");
   
     
     //если ниже курсора и показывать вторую линию
     if(i+xstmenu.startItem > xstmenu.curpos && ShowXStatusSecLine) start_y = font_y+3;
     else start_y=0;
    
     start_y += (xstmenu.rc.y + 2 + (i*font_y));
     patch_rect(&rc,xstmenu.rc.x+3,start_y,xstmenu.rc.x2-2,start_y+font_y);  
  
     xstmenu.img = GetXIconByItem(i+xstmenu.startItem);
    // if(GetXIconByItem(i+xstmenu.startItem))
      // x=GetXIconByItem(i+xstmenu.startItem)->w;
     if(xstmenu.img) x = xstmenu.img->w;
     
     if(x>64) x=64;
     if( i + xstmenu.startItem == xstmenu.curpos)
      {
        DrawRoundedFrame(rc.x+1,rc.y-1,rc.x2-5,rc.y2-1 + (ShowXStatusSecLine?font_y:0),0,0,0,MMCursorClr,MMCursorBGClr); 
        if(ShowXStatusSecLine)
          DrawStr(xst->xlong, rc.x+x+6, rc.y+font_y, rc.x2-x, rc.y2+font_y, 0,MMFont,(char*)MMFontClr,1);  
        DrawXScrollBar(xstmenu.startItem+i,total_xstatuses);
      }
  
     if(xstmenu.img)
        DrwImage(xstmenu.img,rc.x+4,start_y,0);
  
     DrawStr(xst->xshort,rc.x+x+6,rc.y-1,rc.x2-(x+6),rc.y2,0,MMFont,(char*)MMFontClr,0);
     
     i++;
     if(xstmenu.startItem + i >= total_xstatuses) break;
  }
  UnlockSched();  
 // mfree(xshort);
 // mfree(xlong);
}


//------------------------------------------


// Управление курсором
void XS_MoveUp()
{
  if(xstmenu.curpos > xstmenu.itemFromDisp + 2 )
  {
    xstmenu.curpos--; 
    if(xstmenu.startItem > 0 && xstmenu.curpos > 5 && xstmenu.curpos < total_xstatuses-5)
      xstmenu.startItem--; 
  }
  else
    if(xstmenu.curpos > 0)
    {
      xstmenu.curpos--;
      if(xstmenu.startItem > 0 && xstmenu.curpos > 5)
        xstmenu.startItem--; 
      else
        if(xstmenu.curpos < 5)
            xstmenu.startItem=0; 
    }
  else
  {
    xstmenu.curpos=total_xstatuses-1;
    xstmenu.startItem=total_xstatuses-xstmenu.itemFromDisp;
  }
}


void XS_MoveDown()
{
  if(xstmenu.curpos < xstmenu.itemFromDisp - 5) xstmenu.curpos++;
  else
    if(xstmenu.curpos < total_xstatuses - 1)
    {
      if(xstmenu.startItem < total_xstatuses-xstmenu.itemFromDisp)
        xstmenu.startItem++;
      xstmenu.curpos++; 
    }
  else   
  {
    xstmenu.curpos=0;
    xstmenu.startItem=0;
  }
}


//------------------------------------------


void XS_OnRedraw(MAIN_GUI *data)
{
  DrwImage(&SCScreen,0,HeaderY+2,0);
  DrawRoundedFrame(xstmenu.rc.x,xstmenu.rc.y-2,xstmenu.rc.x2,xstmenu.rc.y2,0,0,0,MMBackgrBordClr,MMBackgrClr);
  XS_DrawList();
}

void XS_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
 
  int HSIZE = (HeaderY+2)*ScreenW();
  int ScrH=ScreenH()-(HeaderY+2);
  SCScreen = DoScreen(ScrH,HSIZE);  
  
  InitXText();
  
  xstmenu.rc=XSRect;
  
  xstmenu.curpos = CurrentXStatus;
  if(xstmenu.curpos > 5 && xstmenu.curpos < total_xstatuses-xstmenu.itemFromDisp)
    xstmenu.startItem = xstmenu.curpos-5;
  else
    if(xstmenu.curpos > total_xstatuses-xstmenu.itemFromDisp)
      xstmenu.startItem = total_xstatuses-xstmenu.itemFromDisp;
  else
    if(xstmenu.curpos < 5)
      xstmenu.startItem=0; 
  
  data->gui.state=1;
}

void XS_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if(SCScreen.bitmap) mfree(SCScreen.bitmap);
  FreeXSTList();
  InitMMenu();
  data->gui.state=0;
}

void XS_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  data->gui.state=2;
  DisableIDLETMR();
}

void XS_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int XS_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
//  DirectRedrawGUI(); 
  DirectRedrawGUI_ID(XStatusChange_ID);
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   XS_MoveUp();   break;
      case DOWN_BUTTON: XS_MoveDown(); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: return (1);
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON:
          Change_XStatus(xstmenu.curpos);
          DrawHeader();
          return(1);  
      case LEFT_SOFT: case RIGHT_BUTTON:
          EditXStatus(xstmenu.curpos);
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
  edit_xstatus_hdr.lgp_id=(int)lgpData[LGP_MnuXStatus];  
  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&XS_Canvas);
  main_gui->gui.methods=(void *)gui_methods_XS;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  XStatusChange_ID = CreateGUI(main_gui);   
}

#endif

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
