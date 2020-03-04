#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"

//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

 __root const CFG_HDR cfghdr0={CFG_UINT,"Hotkey (dec)",0,99};
 __root const int _ACTIVE_KEY=26;
 
 __root const CFG_HDR cfghdr7_2={CFG_STR_WIN1251,"CSM name",0,31};
 __root const char _CSMText[32]="Главное меню";
 
 __root const CFG_HDR cfghdrm_m1 = {CFG_CHECKBOX, "Close menu after choice", 0, 2};
 __root const int _closeMenu = 0;


  //==============================================================
__root const CFG_HDR cfghdr_l21={CFG_LEVEL,"Animation",1,0};
 
  __root const CFG_HDR cfghdr10_1 = {CFG_CHECKBOX, "Enable", 0, 2};
  __root const int _Animation = 1;
    
  __root const CFG_HDR cfghdr10_2 = {CFG_UINT, "Speed 1/100s", 0, 6000};
  __root const unsigned int _SpeedAnim = 20;

__root const CFG_HDR _cfghdr_l20={CFG_LEVEL,"",0,0};
  
  //==============================================================
__root const CFG_HDR cfghdr_l11={CFG_LEVEL,"Paths",1,0};

  __root const CFG_HDR cfghdr2_2={CFG_STR_UTF8,"Path menu cfg",0,63};
  __root const char _MENU_PATH[64]=DEFAULT_DISK ":\\ZBin\\Menu\\Menu.cfg";
  
  __root const CFG_HDR cfghdr2_4={CFG_STR_UTF8,"Path img menu",0,63};
  __root const char _IMGMENU_PATH[64]=DEFAULT_DISK ":\\ZBin\\Menu\\img\\";

__root const CFG_HDR cfghdr_l10={CFG_LEVEL,"",0,0};
  //==============================================================
__root const CFG_HDR cfghdr_j11={CFG_LEVEL,"Softkey",1,0};

  __root const CFG_HDR cfghdr3_1={CFG_STR_WIN1251,"Text right soft ",0,31};
  __root const char _TextRight[32]="Назад";
  
  __root const CFG_HDR cfghdr3_2={CFG_STR_WIN1251,"Text left soft",0,31};
  __root const char _TextLeft[32]="Мое меню";

  __root const CFG_HDR cfghdr3_3={CFG_STR_WIN1251,"Run left soft",0,63};
  __root const char _RunLeft[64]="A06E835B"; //A06E835B MY_MENU

__root const CFG_HDR cfghdr_j10={CFG_LEVEL,"",0,0};
  //====================================================









