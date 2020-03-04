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
#include "manage_cl.h"
#include "rect_patcher.h"
#include "cl_work.h"
#include "print.h"
#include "configs.h"
#include "addon.h"
//==============================================================================
int MainMenu_ID;

extern int CurrentStatus;
extern int CurrentXStatus;
extern DYNPNGICONLIST *XStatusesImgList;
extern int *XStatusesIconArray;
extern  int S_ICONS[];
extern const SOFTKEYSTAB menu_skt;
extern int Is_INC_On;
extern int Is_SLI_On;
extern int Is_SMARTCPU_On;
extern int Is_ILU_On;
extern int Is_Double_Vibra_On;
int janis;
extern int Is_PING_On;
extern int Is_UPTIME_On;
extern int Is_Vibra_Enabled;
#ifdef NEWSGOLD
extern int Is_Id3tag_Enabled;
#endif
extern unsigned int Is_Sounds_Enabled; 
extern int Is_Show_Offline;
extern int Is_Show_Groups;


static void ChangeVibraMode(void)
{
  Is_Vibra_Enabled=!(Is_Vibra_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
#ifdef NEWSGOLD
static void ChangeId3tagMode(void)
{
  Is_Id3tag_Enabled=!(Is_Id3tag_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
  set_my_xstatus();
}
#endif

extern int msg_away_timer;
extern GBSTMR away_tmr;
extern void away_timer();
extern int main_d;
extern int away_m;
extern unsigned int AUTO_ENGADE;

extern void UpdateCLheader(void);

static void AutoAwayMSG(void)
{
 if(!away_m)
  {
   away_m=!away_m;
   GBS_StopTimer(&away_tmr);
   msg_away_timer=AUTO_ENGADE*12;
   away_timer();
   main_d=1;
  }
 else
  {
   msg_away_timer=0;
   away_m=!away_m;
  }
 UpdateCLheader();
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
  RecountMenu(NULL,1);
  RefreshGUI();
}

static void ChangeShowGroupsMode(void)
{
  void RecountMenu(CLIST *req,int needfocus);
  Is_Show_Groups=!(Is_Show_Groups);
  SUBPROC((void*)WriteDefSettings);
  ResortCL();
  RecountMenu(NULL,1);
  RefreshGUI();
}

//extern int get_file_size(char* fname);
extern char suc_config_name[64];
extern char suc_config_name_mod[64];
static void EditConfig(void)
{
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,  suc_config_name,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  GeneralFuncF1(1);
}

static void EditConfigMod(void)
{
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,suc_config_name_mod,128);
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

extern int uins;
char hmm1[64];

static void PingToServer(void)
{
  janis=1;
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
  char s[256];
  snprintf(s,255,LG_COPYRIGHT,__SVN_REVISION__);
  ShowMSG(2, (int)s);
}

static void SaveCL()
{
  extern void CreateCLFile(void);
  CreateCLFile();
}

static const HEADER_DESC menuhdr={0,0,0,0,NULL,(int)LG_MENU,LGP_NULL};

static const int mmenusoftkeys[]={0,1,2};

//Переходим к универсальности))
#ifdef NEWSGOLD
  #ifdef ELKA
      #define MENU_ITEMS 23//ELKA
  #else
      #define MENU_ITEMS 22//NEWSGOLD
  #endif
#else
  #define MENU_ITEMS 21//SGOLD
#endif
/*
static const char * const menutexts[MENU_ITEMS]=
{
  LG_MNUSTATUS,
  LG_MNUXSTATUS,
  LG_MANAGELIST,
#ifdef NEWSGOLD
  LG_ID3TAG,
#endif
  LG_AUTOANSWER,//"Автоответчик",
  LG_MNUVIBRA,
  LG_MNUSOUND,
//#ifdef NEWSGOLD
  LG_INCMSG,
//#endif
#ifdef ELKA
  LG_MNUSLION, 
#endif
  LG_MNUSMARTCPU,
  LG_REMINDERON,
  LG_MNUILUON,
  LG_MNUPINGON,
  LG_MNUUPTIMEON,
  LG_MNUSHOWOFF,
  LG_MNUSHOWGROUP,
  LG_MNUEDCFG,
  LG_MNUEDCFG_MOD,
  LG_MNUDISCONNECT,
  LG_MSGINFO,
  LG_SAVECL,
  LG_MNUPING,
  LG_MNUABOUT
};
*/
char *menutexts[MENU_ITEMS]=
{
  LG_MNUSTATUS,
  LG_MNUXSTATUS,
  LG_MANAGELIST,
  LG_AUTOANSWER,//"Автоответчик",
  LG_MNUVIBRA,
  LG_MNUSOUND,
  LG_INCMSG,
  LG_MNUSMARTCPU,
  LG_REMINDERON,
  LG_MNUILUON,
  LG_MNUPINGON,
  LG_MNUUPTIMEON,
  LG_MNUSHOWOFF,
  LG_MNUSHOWGROUP,
  LG_MNUEDCFG,
  LG_MNUEDCFG_MOD,
  LG_MNUDISCONNECT,
  LG_MSGINFO,
  LG_SAVECL,
  LG_MNUPING,
  LG_MNUABOUT,
#ifdef NEWSGOLD
  LG_ID3TAG,
#endif
#ifdef ELKA
  LG_MNUSLION
#endif
};

#ifdef ELKA
static void ChangeSLIMode(void)
{
  Is_SLI_On=!(Is_SLI_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
#endif
static void ChangeSMARTCPUMode(void)
{
  Is_SMARTCPU_On=!(Is_SMARTCPU_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
//#ifdef NEWSGOLD
static void ChangeINCMode(void)
{
  Is_INC_On=!(Is_INC_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
//#endif
static void ChangeILUMode(void)
{
  Is_ILU_On=!(Is_ILU_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

static void ChangePINGMode(void)
{
  Is_PING_On=!(Is_PING_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

static void ChangeUPTIMEMode(void)
{
  Is_UPTIME_On=!(Is_UPTIME_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

int main_count;
int main2_count;

static void MSG_Info(void)
{
  char *info=malloc(64);
  sprintf(info, "Отправлено: %d\n"
                "Принято: %d", main2_count, main_count);
  ShowMSG(1,(int)info);
  mfree(info);
}

static void ChangeDoubleVibra(void)
{
  Is_Double_Vibra_On=!(Is_Double_Vibra_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

extern void CreateSCMenu();
extern void CreateXSMenu();
static const void *menuprocs[MENU_ITEMS]=
{
  (void *)CreateSCMenu,//(void *)DispStatusChangeMenu,
  (void *)CreateXSMenu,//DispXStatusChangeMenu,
  (void *)CreateManageCLMenu,
  (void *)AutoAwayMSG,
  (void *)ChangeVibraMode,
  (void *)ChangeSoundMode,
  (void *)ChangeINCMode,
  (void *)ChangeSMARTCPUMode,
  (void *)ChangeDoubleVibra,
  (void *)ChangeILUMode,
  (void *)ChangePINGMode,
  (void *)ChangeUPTIMEMode,
  (void *)ChangeShowOfflineMode,
  (void *)ChangeShowGroupsMode,
  (void *)EditConfig,
  (void *)EditConfigMod,
  (void *)Disconnect, 
  (void *)MSG_Info,
  (void*)SaveCL,
  (void *)PingToServer,
  (void *)AboutDlg,
#ifdef NEWSGOLD
  (void *)ChangeId3tagMode,
#endif
#ifdef ELKA
  (void *)ChangeSLIMode
#endif
};


/*static const void *menuprocs[MENU_ITEMS]=
{
  (void *)DispStatusChangeMenu,
  (void *)DispXStatusChangeMenu,
  (void *)CreateManageCLMenu,
#ifdef NEWSGOLD
  (void *)ChangeId3tagMode,
#endif
  (void *)AutoAwayMSG,
  (void *)ChangeVibraMode,
  (void *)ChangeSoundMode,
//#ifdef NEWSGOLD
  (void *)ChangeINCMode,
//#endif
#ifdef ELKA
  (void *)ChangeSLIMode,
#endif
  (void *)ChangeSMARTCPUMode,
  (void *)ChangeDoubleVibra,
  (void *)ChangeILUMode,
  (void *)ChangePINGMode,
  (void *)ChangeUPTIMEMode,
  (void *)ChangeShowOfflineMode,
  (void *)ChangeShowGroupsMode,
  (void *)EditConfig,
  (void *)EditConfigMod,
  (void *)Disconnect, 
  (void *)MSG_Info,
  (void*)SaveCL,
  (void *)PingToServer,
  (void *)AboutDlg,
};
*/
static const SOFTKEY_DESC mmenu_sk[]=
{
  {0x0018,0x0000,(int)LG_SELECT},
  {0x0001,0x0000,(int)LG_BACK},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB mmenu_skt=
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
#ifdef NEWSGOLD
  case 3:
    SetMenuItemIconArray(data,item,icon_array+(Is_Id3tag_Enabled?0:1));
    break;
  case 4:
    SetMenuItemIconArray(data,item,icon_array+(away_m?0:1));
    break;
  case 5:
    SetMenuItemIconArray(data,item,icon_array+(Is_Vibra_Enabled?0:1));
    break;
  case 6:
    SetMenuItemIconArray(data,item,icon_array+(Is_Sounds_Enabled?0:1));
    break;
#ifdef ELKA
  case 7:
    SetMenuItemIconArray(data,item,icon_array+(Is_INC_On?0:1));
    break;  
  case 8:
    SetMenuItemIconArray(data,item,icon_array+(Is_SLI_On?0:1));
    break;
  case 9:
    SetMenuItemIconArray(data,item,icon_array+(Is_SMARTCPU_On?0:1));
    break;
  case 10:
    SetMenuItemIconArray(data,item,icon_array+(Is_Double_Vibra_On?0:1));
    break;
  case 11:
    SetMenuItemIconArray(data,item,icon_array+(Is_ILU_On?0:1));
    break;
  case 12:
    SetMenuItemIconArray(data,item,icon_array+(Is_PING_On?0:1));
    break;
  case 13:
    SetMenuItemIconArray(data,item,icon_array+(Is_UPTIME_On?0:1));
    break;  
  case 14:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Offline?0:1));
    break;
  case 15:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Groups?0:1));
    break;    
  case 16:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_SETTINGS);
    break;
  case 17:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_SETTINGS);
    break;
  case 18:
    SetMenuItemIconArray(data,item,S_ICONS+IS_OFFLINE);
    break;
  case 19:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;  
  case 20:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;  
  case 21:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_PING);
    break;
  case 22:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;
#else
  case 7:
    SetMenuItemIconArray(data,item,icon_array+(Is_INC_On?0:1));
    break;
  case 8:
    SetMenuItemIconArray(data,item,icon_array+(Is_SMARTCPU_On?0:1));
    break;
  case 9:
    SetMenuItemIconArray(data,item,icon_array+(Is_Double_Vibra_On?0:1));
    break;
  case 10:
    SetMenuItemIconArray(data,item,icon_array+(Is_ILU_On?0:1));
    break;
  case 11:
    SetMenuItemIconArray(data,item,icon_array+(Is_PING_On?0:1));
    break;
  case 12:
    SetMenuItemIconArray(data,item,icon_array+(Is_UPTIME_On?0:1));
    break;  
  case 13:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Offline?0:1));
    break;
  case 14:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Groups?0:1));
    break;    
  case 15:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_SETTINGS);
    break;
  case 16:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_SETTINGS);
    break;
  case 17:
    SetMenuItemIconArray(data,item,S_ICONS+IS_OFFLINE);
    break;
  case 18:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;
  case 19:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;  
  case 20:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_PING);
    break;
  case 21:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;
#endif
#else
  case 3:
    SetMenuItemIconArray(data,item,icon_array+(away_m?0:1));
    break;
  case 4:
    SetMenuItemIconArray(data,item,icon_array+(Is_Vibra_Enabled?0:1));
    break;
  case 5:
    SetMenuItemIconArray(data,item,icon_array+(Is_Sounds_Enabled?0:1));
    break;
  case 6:
    SetMenuItemIconArray(data,item,icon_array+(Is_INC_On?0:1));
    break;
  case 7:
    SetMenuItemIconArray(data,item,icon_array+(Is_SMARTCPU_On?0:1));
    break;
  case 8:
    SetMenuItemIconArray(data,item,icon_array+(Is_Double_Vibra_On?0:1));
    break;
  case 9:
    SetMenuItemIconArray(data,item,icon_array+(Is_ILU_On?0:1));
    break;
  case 10:
    SetMenuItemIconArray(data,item,icon_array+(Is_PING_On?0:1));
    break;
  case 11:
    SetMenuItemIconArray(data,item,icon_array+(Is_UPTIME_On?0:1));
    break;  
  case 12:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Offline?0:1));
    break;
  case 13:
    SetMenuItemIconArray(data,item,icon_array+(Is_Show_Groups?0:1));
    break;    
  case 14:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_SETTINGS);
    break;
  case 15:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_SETTINGS);
    break;
  case 16:
    SetMenuItemIconArray(data,item,S_ICONS+IS_OFFLINE);
    break;
  case 17:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;
  case 18:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;   
  case 19:
    SetMenuItemIconArray(data,item,S_ICONS+ICON_PING);
    break;
  case 20:
    SetMenuItemIconArray(data,item,S_ICONS+IS_UNKNOWN);
    break;
#endif
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
  MENU_ITEMS
};

void ShowMainMenu()
{
  icon_array[0]=GetPicNByUnicodeSymbol(CBOX_CHECKED);
  icon_array[1]=GetPicNByUnicodeSymbol(CBOX_UNCHECKED);
  *((int **)(&menuhdr.icon))=S_ICONS+IS_ONLINE;
  patch_header(&menuhdr);
  MainMenu_ID=CreateMenu(0,0,&tmenu,&menuhdr,0,MENU_ITEMS,0,0);
}



int startItem=0;

#ifdef ELKA
  int x2_cb=10;
#else
  int x2_cb=10;
#endif
extern char clrBlack[];
extern char clrWhite[];
extern char clrRed[];
extern char transparent[];
extern char blue[];
extern const int clfont;
extern IMGHDR *icons[];
char clrRed2[]={0xFF,0x00,0x00,0x15};
int mm_itemFromDisp;//кол-во на экране
int mm_totalItems;//общее количество итемов
int mm_curpos=0;//позичия курсора
int mm_curpage=1;//текущая страница


extern void DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush);
extern DYNPNGICONLIST *GetXSByItem(int curitem);
RECT mm_rc;//MainMenu Rect
#define mm_font 108

#define CLIST_Y1 (YDISP+2)

static DrawCheckBox(int x,int y,int flag)
{
  RECT rc;
  StoreXYXYtoRECT(&rc,x+2,y,x+x2_cb+2,y+x2_cb);
  DrawRectangle(rc.x,rc.y,rc.x2,rc.y2,0,clrWhite,transparent);
  if(flag)
  {
    DrawLine(rc.x+2,rc.y+5,rc.x2-7,rc.y2-2,0,clrWhite);
    DrawLine(rc.x2-7,rc.y2-2,rc.x+x2_cb,rc.y-2,0,clrWhite);
  }
}

void DrawIcon(int x,int y,int curitem)
{
 switch(curitem)
  {
    case 0: DrwImage(icons[CurrentStatus],x,y,0,0);  break;
    case 1:
    {
      DYNPNGICONLIST *xs=GetXSByItem(CurrentXStatus);
      DrwImage(xs->img,x,y,0,0);
    }
    break;
    case 2: DrwImage(icons[ICON_ADDCONTACT],x,y,0,0);       break;//DrawImg(x,y,S_ICONS[ICON_ADDCONTACT]);
    case 3: DrawCheckBox(x,y,away_m?1:0);                   break;
    case 4: DrawCheckBox(x,y,Is_Vibra_Enabled?1:0);         break;
    case 5: DrawCheckBox(x,y,Is_Sounds_Enabled?1:0);        break;
    case 6: DrawCheckBox(x,y,Is_INC_On?1:0);                break;
    case 7: DrawCheckBox(x,y,Is_SMARTCPU_On?1:0);           break;
    case 8: DrawCheckBox(x,y,Is_Double_Vibra_On?1:0);       break;
    case 9: DrawCheckBox(x,y,Is_ILU_On?1:0);                break;
    case 10: DrawCheckBox(x,y,Is_PING_On?1:0);              break;
    case 11: DrawCheckBox(x,y,Is_UPTIME_On?1:0);            break;  
    case 12: DrawCheckBox(x,y,Is_Show_Offline?1:0);         break;
    case 13: DrawCheckBox(x,y,Is_Show_Groups?1:0);          break;    
    case 14: DrwImage(icons[ICON_SETTINGS],x,y,0,0);       break;
    case 15: DrwImage(icons[ICON_SETTINGS],x,y,0,0);       break;
    case 16: DrwImage(icons[IS_OFFLINE],x,y,0,0);          break;
    case 17: DrwImage(icons[IS_UNKNOWN],x,y,0,0);          break;
    case 18: DrwImage(icons[IS_UNKNOWN],x,y,0,0);          break;   
    case 19: DrwImage(icons[ICON_PING],x,y,0,0);           break;
    case 20: DrwImage(icons[IS_UNKNOWN],x,y,0,0);          break;    
#ifdef NEWSGOLD
    case 21: DrawCheckBox(x,y,Is_Id3tag_Enabled?1:0);      break;
  #ifdef ELKA
    case 22: DrawCheckBox(x,y,Is_SLI_On?1:0);              break;
  #endif
#endif
  }
}


void DrawMMScrollBar(int cur,int total)
{
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  int y=mm_rc.y2-mm_rc.y-2;//scr_h-CLIST_Y1-FontH(clfont)-2;
  int pos=y*cur/total;
  int y2=y/total;
 // int i=0;
  
  DrawLine(mm_rc.x2-2,mm_rc.y,mm_rc.x2-2,mm_rc.y2,1,clrWhite);
  DrawRoundedFrame(mm_rc.x2-3,mm_rc.y+pos+1,mm_rc.x2-1,mm_rc.y+pos+y2,0,0,0,clrWhite,clrWhite);
  /*for(i=0;i<y2;i+=1)
  {
    DrawRoundedFrame(mm_rc.x2-4,mm_rc.y+pos+1,mm_rc.x2-1,mm_rc.y+pos+i,0,0,0,clrWhite,clrWhite);
  }*/
}


static void MM_DrawList()
{
  int font_y = FontH(mm_font)+1;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  mm_itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(mm_font)-2)-1;
  
  LockSched();
  
  int i=0;
  int start_y=0;
  int start_x=x2_cb+8;
 
  char *s=malloc(128);
  RECT rc;

  while(i<mm_itemFromDisp)
  {
   sprintf(s,"%s",menutexts[startItem+i]);
        
   start_y = (mm_rc.y + 2 + (i/* - (mm_curpage-1)*mm_itemFromDisp*/)*font_y);    //определяем начальное смещение по у      
   StoreXYXYtoRECT(&rc,start_x,start_y,mm_rc.x2-2,start_y+font_y);            
   SetFontColor(clrWhite,transparent,0);           
   if(startItem+i==mm_curpos)//Если наш итем то выделяем
    {
     DrawRoundedFrame(rc.x,rc.y-1,rc.x2-5,rc.y2-1,0,0,0,clrWhite,transparent);//cursor        
     DrawMMScrollBar(startItem+i,MENU_ITEMS);
    } 
   DrawIcon(1,start_y,startItem+i);
   PrintText(rc.x+3,rc.y-1,s,0,mm_font,1,0,0,0,rc.x2-5);//item name
   i++;
   if(startItem+i >= MENU_ITEMS) break;
  }
  mfree(s);
  UnlockSched();  
}

// Управление курсором
void MM_MoveUp(int i)
{
  if(mm_curpos > mm_itemFromDisp + 2 ){ mm_curpos--; }
  else
    if(mm_curpos > 0){ mm_curpos--; if(startItem>0) startItem--; }
  else
  {
    mm_curpos=MENU_ITEMS-1;
    startItem=MENU_ITEMS-mm_itemFromDisp;
  }
}


void MM_MoveDown(int i)
{
  if(mm_curpos <mm_itemFromDisp - 5) mm_curpos++;
  else
    if(mm_curpos < MENU_ITEMS - 1 ){if(startItem < MENU_ITEMS-mm_itemFromDisp) startItem++; mm_curpos++; }
  else   
  {
    mm_curpos=0;
    startItem=0;
  }
}  

extern IMGHDR screen;
extern void DrawSoftKeys(char *right,char *left);
extern void DrawContactList();
extern void DrawHeader();
static void MM_OnRedraw(MAIN_GUI *data)
{
//  ClearScreen();
  LockSched();
 // DrwImage(&screen,0,0,0,0);
 // DrawHeader();
//  DrawContactList();
  DrawRoundedFrame(mm_rc.x,mm_rc.y-2,mm_rc.x2,mm_rc.y2,0,0,0,"\x00\x00\x00\x45",clrBlack);
  //for(int i=mm_rc.y2-mm_rc.y;i>0;i-=15)
  //   DrawRoundedFrame(mm_rc.x,mm_rc.y,mm_rc.x2,mm_rc.y2-i,0,0,0,clrRed2,clrRed2);    
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  MM_DrawList();
}

static void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
 // DoScreen();
  mm_rc.x=0;
  mm_rc.y=YDISP+20;
  mm_rc.x2=ScreenW()-50;
  mm_rc.y2=ScreenH()-FontH(clfont)-3;
  SetCanvasFlag(1);
  data->gui.state=1;
}

static void MM_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  //mfree(screen.bitmap);
//  zeromem(screen.bitmap,(screen.w*screen.h*2));
  data->gui.state=0;
}

static void MM_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  SetCanvasFlag(1);
  data->gui.state=2;
  DisableIDLETMR();
}

static void MM_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int MM_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   MM_MoveUp(0);   break;
      case DOWN_BUTTON: MM_MoveDown(0); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: return (1);
     // case ENTER_BUTTON: case LEFT_SOFT: ((void (*)(void))(menuprocs[mm_curpos]))(); REDRAW(); break;
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON: case LEFT_SOFT: case RIGHT_BUTTON:
          ((void (*)(void))(menuprocs[mm_curpos]))();
          REDRAW();
          break;
    }
  }   

  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int MM_method8(void){return(0);}
int MM_method9(void){return(0);}

const void * const gui_methods_2[11]={
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
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&MM_Canvas);
  main_gui->gui.methods=(void *)gui_methods_2;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  MainMenu_ID=CreateGUI(main_gui);   
}
