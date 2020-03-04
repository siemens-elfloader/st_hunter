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
   msg_away_timer=AUTO_ENGADE;
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
static const void *menuprocs[MENU_ITEMS]=
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
