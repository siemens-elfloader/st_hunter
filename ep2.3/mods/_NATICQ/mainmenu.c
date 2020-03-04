#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "history.h"
#include "mainmenu.h"
#include "main.h"
#include "conf_loader.h"
#include "status_change.h"
#include "language.h"
#include "revision.h"
#include "revisio0.h"
#include "manage_cl.h"
#include "rect_patcher.h"
#include "cl_work.h"
#include "lang.h"

//#ifdef GUI_INTERFACE
  #include "externals.h"
//#endif

//==============================================================================
int MainMenu_ID;
int isAbout=0;

extern int CurrentStatus;
extern int CurrentXStatus;
extern DYNPNGICONLIST *XStatusesImgList;
extern int *XStatusesIconArray;
extern  int S_ICONS[];
extern const SOFTKEYSTAB menu_skt;

extern int Is_Vibra_Enabled;
extern unsigned int Is_Sounds_Enabled; 
extern int Is_Show_Offline;
extern int Is_Show_Groups;


static void ChangeVibraMode(void)
{
  Is_Vibra_Enabled=!(Is_Vibra_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
  
static void ChangeSoundMode(void)
{
  Is_Sounds_Enabled=!(Is_Sounds_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

static void ChangeShowOfflineMode(void)
{
  void RecountMenu(CLIST *req, int needfocus);
  Is_Show_Offline=!(Is_Show_Offline);
  SUBPROC((void*)WriteDefSettings);
  RecountMenu(NULL, 1);
  RefreshGUI();
}

static void ChangeShowGroupsMode(void)
{
  void RecountMenu(CLIST *req, int needfocus);
  Is_Show_Groups=!(Is_Show_Groups);
  SUBPROC((void*)WriteDefSettings);
  ResortCL();
  RecountMenu(NULL, 1);
  RefreshGUI();
}

static void EditConfig(void)
{
  extern const char *successed_config_filename;
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,successed_config_filename,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  GeneralFuncF1(1);
}

static void Disconnect(void)
{
  extern volatile int disautorecconect;
  disautorecconect=1;
  extern void end_socket(void);
  SUBPROC((void*)end_socket);
  GeneralFuncF1(1);
}

void PingToServer(void)
{
  TPKT *p;
  p=malloc(sizeof(PKT)+sizeof(TTime));
  GetDateTime(NULL,(TTime *)p->data);
  p->pkt.uin=0;
  p->pkt.type=T_ECHO;
  p->pkt.data_len=sizeof(TTime);
  SUBPROC((void *)SendAnswer,0,p);
}

static void AboutDlg()
{
#ifdef GUI_INTERFACE
  isAbout=1;
#else
  char s[256];
  snprintf(s,255,LG_COPYRIGHT,__SVN_REVISION__);
  ShowMSG(2, (int)s);
#endif
}

#ifndef GUI_INTERFACE
static HEADER_DESC menuhdr={0,0,0,0,NULL,NULL,LGP_NULL};

static const int mmenusoftkeys[]={0,1,2};

static char * menutexts[11]=
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
  NULL
};

/*MENUITEM_DESC menuitems[7]=
{
  {S_ICONS,    (int)LG_MNUSTATUS,  LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,       (int)LG_MNUADDCONT, LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {icon_array, (int)LG_MNUVIBRA,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {icon_array, (int)LG_MNUSOUND,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,       (int)LG_MNUEDCFG,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,       (int)LG_MNUPING ,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {S_ICONS,    (int)LG_MNUABOUT,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
};*/

static const void *menuprocs[11]=
{
  (void *)DispStatusChangeMenu,
  (void *)DispXStatusChangeMenu,
  (void *)CreateManageCLMenu,
  (void *)ChangeVibraMode,
  (void *)ChangeSoundMode,
  (void *)ChangeShowOfflineMode,
  (void *)ChangeShowGroupsMode,
  (void *)EditConfig,
  (void *)Disconnect,
  (void *)PingToServer,
  (void *)AboutDlg,
};

static SOFTKEY_DESC mmenu_sk[]=
{
  {0x0018,0x0000,NULL},
  {0x0001,0x0000,NULL},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static SOFTKEYSTAB mmenu_skt=
{
  mmenu_sk,0
};

void tmenu_ghook(void *data, int cmd)
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
  }
}

static int icon_array[2];

static void menuitemhandler(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  extern const char percent_t[];
  ws=AllocMenuWS(data,strlen(menutexts[curitem]));
  wsprintf(ws,percent_t,menutexts[curitem]);
  switch(curitem)
  {
  case 0:
    SetMenuItemIconArray(data,item,S_ICONS+CurrentStatus);
    break;
  case 1:
    SetMenuItemIconArray(data,item,XStatusesIconArray+CurrentXStatus);
    break;
  case 2:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_ADDCONTACT);
    break;
  case 3:
    SetMenuItemIconArray(data,item,icon_array+(Is_Vibra_Enabled?0:1));
    break;
  case 4:
    SetMenuItemIconArray(data,item,icon_array+(Is_Sounds_Enabled?0:1));
    break;
  case 5:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Offline?0:1));
    break;
  case 6:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Groups?0:1));
    break;    
  case 7:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_SETTINGS);
    break;
  case 8:
    SetMenuItemIconArray(data,item,S_ICONS+IS_OFFLINE);
    break;
  case 9:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_PING);
    break;
  case 10:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;
  }
  SetMenuItemText(data, item, ws, curitem);
}

static int tmenu_keyhook(void *data, GUI_MSG *msg)
{
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    ((void (*)(void))(menuprocs[GetCurMenuItem(data)]))();
  }
  return(0);
}


static const MENU_DESC tmenu=
{
  8,tmenu_keyhook,tmenu_ghook,NULL,
  mmenusoftkeys,
  &mmenu_skt,
  0x11,//MENU_FLAG,
  menuitemhandler,
  NULL,//menuitems,
  NULL,//menuprocs,
  11
};


void ShowMainMenu()
{
  icon_array[0]=GetPicNByUnicodeSymbol(CBOX_CHECKED);
  icon_array[1]=GetPicNByUnicodeSymbol(CBOX_UNCHECKED);

  //Инициализация ленгпака
  menuhdr.lgp_id=(int)lgpData[LGP_Menu];
  
  mmenu_sk[0].lgp_id=(int)lgpData[LGP_Select];
  mmenu_sk[1].lgp_id=(int)lgpData[LGP_Back];
  
  menutexts[0]=(char *)lgpData[LGP_MnuStatus];
  menutexts[1]=(char *)lgpData[LGP_MnuXStatus];
  menutexts[2]=(char *)lgpData[LGP_ManageList];
  menutexts[3]=(char *)lgpData[LGP_MnuVibra];
  menutexts[4]=(char *)lgpData[LGP_MnuSound];
  menutexts[5]=(char *)lgpData[LGP_MnuShowOff];
  menutexts[6]=(char *)lgpData[LGP_MnuShowGroup];
  menutexts[7]=(char *)lgpData[LGP_MnuEdCfg];
  menutexts[8]=(char *)lgpData[LGP_MnuDisconnect];
  menutexts[9]=(char *)lgpData[LGP_MnuPing];
  menutexts[10]=(char *)lgpData[LGP_MnuAbout];
  
  *((int **)(&menuhdr.icon))=S_ICONS+IS_ONLINE;
  patch_header(&menuhdr);
  MainMenu_ID=CreateMenu(0,0,&tmenu,&menuhdr,0,11,0,0);
}

//------------------------------------------


#else//#define GUI


//------------------------------------------

#define DYNMENU

#ifndef DYNMENU

#ifdef ELKA
  #define MENU_ITEMS 16
#else
  #define MENU_ITEMS 15
#endif

static char * menutexts[MENU_ITEMS]=
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
  NULL,
  NULL,
  NULL,
  NULL
#ifdef ELKA
  ,
  NULL
#endif
};
#else
 int MENU_ITEMS = 0;
#endif
   
extern int IsActiveUp;


//------------------------------------------


void ChangeActiveUp(void)
{
  IsActiveUp=!(IsActiveUp);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}


//------------------------------------------


void AutoAwayMSG(void)
{
 if(!away_m)
  {
   away_m=!away_m;
   GBS_StopTimer(&away_tmr);
   msg_away_timer = AUTO_ENGADE*12;
   away_timer();
  }
 else
  {
   msg_away_timer=0;
   away_m=!away_m;
   if(change_back_status)
    {
      CurrentStatus = status_rem;
      set_my_status();
      change_back_status=0;
    }
  }
 RefreshGUI();
}


//------------------------------------------


#ifdef ELKA
void ChangeSLIMode(void)
{
  Is_SLI_On=!(Is_SLI_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
#endif


//------------------------------------------


void ChangePOPUPMode(void)
{
  Is_POPUP_On=!(Is_POPUP_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}


//------------------------------------------


void ChangeDoubleVibra(void)
{
  Is_Double_Vibra_On=!(Is_Double_Vibra_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}


//------------------------------------------



extern void CreateSCMenu();
extern void CreateXSMenu();

#ifndef DYNMENU
static const void *menuprocs[MENU_ITEMS]=
{
  (void *)CreateSCMenu,
  (void *)CreateXSMenu,
  (void *)CreateManageCLMenu,
  (void *)AutoAwayMSG,
  (void *)ChangeVibraMode,
  (void *)ChangeSoundMode,
#ifdef ELKA
  (void *)ChangeSLIMode,
#endif
  (void *)ChangeDoubleVibra,
  (void *)ChangePOPUPMode,
  (void *)ChangeShowOfflineMode,
  (void *)ChangeShowGroupsMode,
  (void *)ChangeActiveUp,
  (void *)EditConfig,
  (void *)Disconnect, 
  (void *)PingToServer,
  (void *)AboutDlg
};
#endif

//------------------------------------------

static GUIMENU mainmenu;

//------------------------------------------

#ifdef DYNMENU

//переходим к динамическому меню....
typedef void (*func)();

typedef struct
{
  char *name;
  func run;
  IMGHDR *img;//картинка меню
  int h; //высота картинки
  int checkbox; //если чекбокс
  int flag;//флаг для чекбокса
  void *next;
}MAINMENU;

static MAINMENU *mmtop;

void AddMenuItem(char *name,func myfunc,IMGHDR *img,int checkbox,int flag)
{
  MAINMENU *m = malloc(sizeof(MAINMENU));
  //strncpy(m->name,name, 32);
  m-> name = name;
  m->img = img;
  m->h = img->h;  
  m->run = myfunc;
  m->checkbox = checkbox;
  m->flag = flag;
  m->next = 0;
  
  MENU_ITEMS++;//кол-во пунктов
  if(!mmtop)
  {
    mmtop = m;   
  }
  else
  {
    MAINMENU *mm = mmtop;
    while(mm->next) mm = mm->next;    
    mm->next = m;
  }
}

MAINMENU *GetItem(int item)
{
  MAINMENU *m = (MAINMENU*)mmtop;
  int i=0;
  while(m)
  {
    if(i == item) return m;
    i++;
    m = (MAINMENU*)m->next;
  }
  return 0;
}

void FreeList()
{  
  LockSched();
  MAINMENU *bmk=(MAINMENU *)mmtop;
  mmtop=0;
  UnlockSched();
  while(bmk)
  {
    MAINMENU *bmk_prev = bmk;
    bmk = bmk->next;
    if(bmk_prev) mfree(bmk_prev);
  }
}


IMGHDR *GetCheckBox(int i)
{
 return GetIconByItem(ICON_UNCHECK - i);
}


extern int CurrentPrivateStatus;
extern int CreateAddContactGrpDialog(CLIST *cl_sel);
extern int CreatePrivateStatusMenu(void);
extern int GetPrivaeIcon();

static void AddContactGrp()
{
  CreateAddContactGrpDialog(NULL);  
}


#define CHECKBOX 1
#define FUNC 0

void InitMMenu()
{
  FreeList();
  MENU_ITEMS = 0;
   
  AddMenuItem((char *)lgpData[LGP_MnuStatus],  CreateSCMenu,                  GetIconByItem(CurrentStatus), FUNC,0);
  AddMenuItem((char *)lgpData[LGP_MnuXStatus], CreateXSMenu,                  GetXIconByItem(CurrentXStatus), FUNC,0);
  AddMenuItem((char *)lgpData[LGP_PrivStatus], (func)CreatePrivateStatusMenu, GetIconByItem(GetPrivaeIcon()), FUNC,0);
  AddMenuItem((char *)lgpData[LGP_AddCont], AddContactGrp,                    GetIconByItem(ICON_ADDCONTACT), FUNC,0);
//  AddMenuItem((char *)lgpData[LGP_ManageList], (func)CreateManageCLMenu, GetIconByItem(ICON_ADDCONTACT), FUNC,0);
  
  //checkbox
  AddMenuItem((char *)lgpData[LGP_MnuAwayMsg],    AutoAwayMSG,           0, CHECKBOX, away_m);
  AddMenuItem((char *)lgpData[LGP_MnuVibra],      ChangeVibraMode,       0, CHECKBOX, Is_Vibra_Enabled);
  AddMenuItem((char *)lgpData[LGP_MnuSound],      ChangeSoundMode,       0, CHECKBOX, Is_Sounds_Enabled);
#ifdef ELKA
  AddMenuItem((char *)lgpData[LGP_MnuSliMode],    ChangeSLIMode,         0, CHECKBOX, Is_SLI_On);
#endif
  AddMenuItem((char *)lgpData[LGP_MnuReminder],   ChangeDoubleVibra,     0, CHECKBOX, Is_Double_Vibra_On);
  AddMenuItem((char *)lgpData[LGP_MnuPopup],      ChangePOPUPMode,       0, CHECKBOX, Is_POPUP_On);
  AddMenuItem((char *)lgpData[LGP_MnuShowOff],    ChangeShowOfflineMode, 0, CHECKBOX, Is_Show_Offline);
  AddMenuItem((char *)lgpData[LGP_MnuShowGroup],  ChangeShowGroupsMode,  0, CHECKBOX, Is_Show_Groups);
  AddMenuItem((char *)lgpData[  LGP_MnuActiveUp], ChangeActiveUp,        0, CHECKBOX, IsActiveUp);
  //
  
  AddMenuItem((char *)lgpData[LGP_MnuEdCfg],      EditConfig,   GetIconByItem(ICON_SETTINGS), FUNC, 0);
  AddMenuItem((char *)lgpData[LGP_MnuDisconnect], Disconnect,   GetIconByItem(IS_OFFLINE),    FUNC, 0);
  AddMenuItem((char *)lgpData[LGP_MnuPing],       PingToServer, GetIconByItem(ICON_PING),     FUNC, 0);
  AddMenuItem((char *)lgpData[LGP_MnuAbout],      AboutDlg,     GetIconByItem(IS_UNKNOWN),    FUNC, 0);
  
//  AddMenuItem(menutexts[i++], , 0, 1, );
}

#else //#ifdef NEWNEMU

int DrawIcon(int x,int y,int curitem, unsigned int h2)
{
  unsigned int h=0;
  x+=1;
  switch(curitem)
  {
    case 0:  h = DrwImage(GetIconByItem(CurrentStatus),x,y,RET_Y);       break;
    case 1:  h = DrwImage(GetXIconByItem(CurrentXStatus),x,y,RET_Y);     break;
    case 2:  h = DrwImage(GetIconByItem(ICON_ADDCONTACT),x,y,RET_Y);     break;
    case 3:  h = DrwImage(GetIconByItem(ICON_UNCHECK - away_m),x,y,RET_Y);   break;
    case 4:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Vibra_Enabled),x,y,RET_Y);   break;
    case 5:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Sounds_Enabled),x,y,RET_Y);  break;      
#ifdef ELKA
    case 6:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_SLI_On),x,y,RET_Y);          break;      
    case 7:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Double_Vibra_On),x,y,RET_Y); break;      
    case 8:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_POPUP_On),x,y,RET_Y);        break;      
    case 9:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Show_Offline),x,y,RET_Y);    break;
    case 10: h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Show_Groups),x,y,RET_Y);     break;
    case 11: h = DrwImage(GetIconByItem(ICON_UNCHECK - IsActiveUp),x,y,RET_Y);         break;
    case 12: h = DrwImage(GetIconByItem(ICON_SETTINGS),x,y,RET_Y); break;
    case 13: h = DrwImage(GetIconByItem(IS_OFFLINE),x,y,RET_Y);    break;
    case 14: h = DrwImage(GetIconByItem(IS_UNKNOWN),x,y,RET_Y);    break;
    case 15: h = DrwImage(GetIconByItem(ICON_PING),x,y,RET_Y);     break;
#else
    case 6:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Double_Vibra_On),x,y,RET_Y); break;      
    case 7:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_POPUP_On),x,y,RET_Y);        break;      
    case 8:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Show_Offline),x,y,RET_Y);    break;
    case 9:  h = DrwImage(GetIconByItem(ICON_UNCHECK - Is_Show_Groups),x,y,RET_Y);     break;
    case 10: h = DrwImage(GetIconByItem(ICON_UNCHECK - IsActiveUp),x,y,RET_Y);         break;
    case 11: h = DrwImage(GetIconByItem(ICON_SETTINGS),x,y,RET_Y); break;
    case 12: h = DrwImage(GetIconByItem(IS_OFFLINE),x,y,RET_Y);    break;
    case 13: h = DrwImage(GetIconByItem(IS_UNKNOWN),x,y,RET_Y);    break;
    case 14: h = DrwImage(GetIconByItem(ICON_PING),x,y,RET_Y);     break;    
#endif
    default : h = 16; break;
  }
 
 if(h > GetIconByItem(IS_ONLINE)->h) h = GetIconByItem(IS_ONLINE)->h;
 if(h > 32)  h = 32;
 if(h2 > 32) h2 = 32;
 
 return ( (h < h2) ? (h2) : (h) );
}

#endif

//------------------------------------------


void DrawMMScrollBar(int cur,int total)
{
  int y=mainmenu.rc.y2-mainmenu.rc.y-2;
  int pos=y*cur/total;
  int y2=y/total;
  
  DrawLine(mainmenu.rc.x2-2, mainmenu.rc.y, mainmenu.rc.x2-2, mainmenu.rc.y2,1, clrWhite);
  DrawRoundedFrame(mainmenu.rc.x2-3, mainmenu.rc.y+pos+1, mainmenu.rc.x2-1, mainmenu.rc.y+pos+y2, 0,0,0,clrWhite,clrWhite);
}


//------------------------------------------


static void MM_DrawList()
{
  unsigned int i=0;
  unsigned int start_y=0;
  unsigned int start_x=0;
  unsigned int pic_h=0; 

  RECT rc;
  int scr_h = ScreenH()-1;
  int scr_w = ScreenW()-1;
  
  unsigned int font_y = 0;
  mainmenu.img = GetIconByItem(IS_ONLINE);
  mainmenu.FontH = FontH(MMFont);
  
  int h = (mainmenu.img->h < mainmenu.FontH) ?  (mainmenu.FontH) : (mainmenu.img->h); 
#ifdef ELKA
  if(h > 32) h = 32;
#else
  if(h > 24) h = 24;
#endif
  if(strlen(mainmenu.img->bitmap))
  {
    font_y = h + 2;
    start_x = mainmenu.img->w + 4;
  }
  else
  {
    font_y = h;//mainmenu.FontH;//FontH(MMFont);
    start_x  = 18;
  }

#ifdef ELKA
  mainmenu.itemFromDisp = sdiv( mainmenu.FontH/*FontH(MMFont)*/ , mainmenu.rc.y2-mainmenu.rc.y );
  int _y=-1;
#else
  mainmenu.itemFromDisp = sdiv( FontH(MMFont) , mainmenu.rc.y2-mainmenu.rc.y ) - 2;
  int _y=1;
#endif

  if( mainmenu.itemFromDisp > MENU_ITEMS ) mainmenu.itemFromDisp = MENU_ITEMS;
  
  //LockSched();  
#ifdef DYNMENU  
  MAINMENU *m;
#endif
  while(i < mainmenu.itemFromDisp)
  {
    start_y = (mainmenu.rc.y + 2 + (i*font_y));
    StoreXYXYtoRECT(&rc,start_x,start_y,mainmenu.rc.x2-5,start_y+font_y);
#ifdef DYNMENU  
    m = GetItem(mainmenu.startItem + i);
#else
    pic_h = DrawIcon(mainmenu.rc.x+1, start_y, mainmenu.startItem + i, pic_h);//рисуем иконку и возвращаем ее высоту
#endif

#ifdef DYNMENU  
    if(m)
//#endif
    {
//#ifdef DYNMENU  
      pic_h  = DrwImage((m->checkbox==0) ? (m->img) : GetCheckBox(m->flag), mainmenu.rc.x+2, start_y, RET_Y);
#endif
      if(pic_h == 0) pic_h = 24;   
      if(mainmenu.startItem+i == mainmenu.curpos)
      {
        DrawRoundedFrame(rc.x,rc.y-1,rc.x2,start_y + pic_h,0,0,0,MMCursorClr,MMCursorBGClr);
        DrawMMScrollBar(mainmenu.startItem+i,MENU_ITEMS);
      }      
#ifndef DYNMENU  
      DrawStr(menutexts[mainmenu.startItem+i],rc.x+3, rc.y+_y,rc.x2-rc.x-7,rc.y2,0,MMFont,(char*)MMFontClr,1);   
#else
      DrawStr(m->name,rc.x+3, rc.y+_y,rc.x2-rc.x-7,rc.y2,0,MMFont,(char*)MMFontClr,1);
    }    
#endif
    i++;
    if(mainmenu.startItem + i >= MENU_ITEMS) break;
  }

//  UnlockSched();  
}


//------------------------------------------


// Управление курсором
void MM_MoveUp(int i)
{
  if(mainmenu.curpos > mainmenu.itemFromDisp + 2 ){ mainmenu.curpos--; }
  else
    if(mainmenu.curpos > 0){ mainmenu.curpos--; if(mainmenu.startItem>0) mainmenu.startItem--; }
  else
  {
    mainmenu.curpos = MENU_ITEMS-1;
    mainmenu.startItem = MENU_ITEMS-mainmenu.itemFromDisp;
  }
}


void MM_MoveDown(int i)
{
  if(mainmenu.curpos < mainmenu.itemFromDisp - 5) mainmenu.curpos++;
  else
    if(mainmenu.curpos < MENU_ITEMS - 1 ){if(mainmenu.startItem < MENU_ITEMS-mainmenu.itemFromDisp) mainmenu.startItem++; mainmenu.curpos++; }
  else   
  {
    mainmenu.curpos = 0;
    mainmenu.startItem = 0;
  }
}


//------------------------------------------


void DrawAbout()
{ 
  int y=0,y2=0;
  int x=5;

  y = (ScreenH()/2) - (FontH(MMFont)*7);
#ifdef ELKA
  y2 = (ScreenH()/2) + (FontH(MMFont)*7);
#else
  y2 = ScreenH()-1;
#endif
  
  DrawRoundedFrame(x,y,ScreenW()-x,y2,0,0,0,"\xFF\xFF\xFF\x45","\x00\x00\x00\x45");

  char *s=malloc(256);  
  sprintf(s,LG_COPYRIGHT,2615,ELF_REVISION,__DATE__,__TIME__);
  DrawStr(s,0,y+FontH(MMFont),ScreenW()-x,y2,TEXT_ALIGNMIDDLE,MMFont,clrRed,0);
  mfree(s);
}


//------------------------------------------


IMGHDR MMScreen;
void MM_OnRedraw(MAIN_GUI *data)
{
 // LockSched();
  if(MMScreen.bitmap) DrwImage(&MMScreen,0,HeaderY+2,RET_Y);
  DrawRoundedFrame(mainmenu.rc.x,mainmenu.rc.y-2,mainmenu.rc.x2,mainmenu.rc.y2+2,0,0,0,MMBackgrBordClr,MMBackgrClr);
 // UnlockSched();
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  MM_DrawList();
  if(isAbout) DrawAbout();
}


//------------------------------------------


void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  
  int HSIZE = (HeaderY+2)*ScreenW();
  int ScrH = ScreenH()-(HeaderY+2);
  MMScreen = DoScreen(ScrH,HSIZE);
  
#ifdef DYNMENU  
  InitMMenu();//инизиализируем меню
#endif
  
  mainmenu.rc = MMRect;
    
  if(MainMenuAutoRect)//чтобы не подгонять координаты вручную
  {
    mainmenu.rc.y2 = mainmenu.rc.y + (FontH(MMFont)*MainMenuItems);
 
    if( mainmenu.rc.y2 > (ScreenH() - FontH(MMFont)) )
      mainmenu.rc.y2 = (ScreenH() - FontH(MMFont)) - 2;
  }
  
  isAbout=0;
  data->gui.state=1;
}

void MM_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if(MMScreen.bitmap)  mfree(MMScreen.bitmap);
#ifdef DYNMENU
  FreeList();
#endif
  data->gui.state=0;
}

void MM_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  DrawHeader();
  
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
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   MM_MoveUp(0);   break;
      case DOWN_BUTTON: MM_MoveDown(0); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: if(isAbout) isAbout=0; else return (1);
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {
      case ENTER_BUTTON: case LEFT_SOFT: case RIGHT_BUTTON:
        if(isAbout)
          isAbout=0;
        else
        {
#ifndef DYNMENU  
          ((void (*)(void))(menuprocs[mainmenu.curpos]))();        
#else
          
          MAINMENU *m = GetItem(mainmenu.curpos);
          (m->run)();
          if(m->checkbox) m->flag = !m->flag; //Если чекбокм то меняем флаг
#endif
        }
        REDRAW();
        break;
    }
    
    if( (k>='1' && k<= '9') && !isAbout )
    {
      mainmenu.curpos=(k-'1'); 
      if(k=='0') mainmenu.curpos=9;
#ifndef DYNMENU
      ((void (*)(void))(menuprocs[mainmenu.curpos]))();
#else
      
      MAINMENU *m = GetItem(mainmenu.curpos);
      (m->run)();
      if(m->checkbox) m->flag = !m->flag; //Если чекбокм то меняем флаг
#endif
      REDRAW();
    }
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int MM_method8(void){return(0);}
int MM_method9(void){return(0);}

static const void * const gui_methods_2[11]={
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


const RECT MM_Canvas={0,0,0,0};

void CreateMainMenu()
{  
  //Инициализация ленгпака 
  //menuhdr.lgp_id=(int)lgpData[LGP_Menu];
  //mmenu_sk[0].lgp_id=(int)lgpData[LGP_Select];
  //mmenu_sk[1].lgp_id=(int)lgpData[LGP_Back];
#ifndef DYNMENU  
  int i=0;
  menutexts[i++]=(char *)lgpData[LGP_MnuStatus];
  menutexts[i++]=(char *)lgpData[LGP_MnuXStatus];
  menutexts[i++]=(char *)lgpData[LGP_ManageList];
  menutexts[i++]=(char *)lgpData[LGP_MnuAwayMsg];//
  menutexts[i++]=(char *)lgpData[LGP_MnuVibra];
  menutexts[i++]=(char *)lgpData[LGP_MnuSound];
#ifdef ELKA
   menutexts[i++]=(char *)lgpData[LGP_MnuSliMode];//
#endif
   menutexts[i++]=(char *)lgpData[LGP_MnuReminder];//
   menutexts[i++]=(char *)lgpData[LGP_MnuPopup];//
  menutexts[i++]=(char *)lgpData[LGP_MnuShowOff];
  menutexts[i++]=(char *)lgpData[LGP_MnuShowGroup];
  menutexts[i++]=(char *)lgpData[  LGP_MnuActiveUp];
  menutexts[i++]=(char *)lgpData[LGP_MnuEdCfg];
  menutexts[i++]=(char *)lgpData[LGP_MnuDisconnect];
  menutexts[i++]=(char *)lgpData[LGP_MnuPing];
  menutexts[i++]=(char *)lgpData[LGP_MnuAbout];
#endif
  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&MM_Canvas);
  main_gui->gui.methods=(void *)gui_methods_2;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  MainMenu_ID=CreateGUI(main_gui);   
}

#endif //GUI
