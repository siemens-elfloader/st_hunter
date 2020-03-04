#include "cfg_items.h"
#include "swilib.h"

#ifdef NEWSGOLD
 #define DEFAULT_DISK "4"
#else
 #define DEFAULT_DISK "0"
#endif

//Конфигурация

 __root const CFG_HDR cfghdr0={CFG_STR_WIN1251,"Number for Cash Request",0,31};
 __root const char CASHREQNUM[32]="*#";

 __root const CFG_HDR cfghdr000={CFG_UINT,"Auto cash check, hours",0,24};
 __root const unsigned int CHECK_HOURS=1;

 __root const CFG_HDR cfghdr01={CFG_CHECKBOX,"Enable Cash Trace",0,0};
 __root const int ENA_CASHTRACE = 1;

 __root const CFG_HDR cfghdr022={CFG_CHECKBOX,"Enable Op USSD Trace",0,0};
 __root const int ENA_AUTOCASHTRACE = 0;

 __root const CFG_HDR cfghdr08={CFG_CHECKBOX,"Enable Cash Trace logging",0,0};
 __root const int ENA_CASHTRACELOG = 1;

 __root const CFG_HDR cfghdr02={CFG_STR_WIN1251,"Cash pattern 0",0,31};
 __root const char CashPat0[32]="alans #hrn";

 __root const CFG_HDR cfghdr06 = {CFG_STR_UTF8,"Temp file path",0,127};
 __root const char cashTEMP_PATH[128]=DEFAULT_DISK ":\\ZBin\\var\\";

 __root const CFG_HDR cfghdr07 = {CFG_STR_UTF8,"Log file",0,127};
 __root const char cashLOG_FILE[128]=DEFAULT_DISK ":\\ZBin\\var\\Cash.log";

 __root const CFG_HDR cfghdr072 = {CFG_STR_UTF8,"Log format",0,127};
 __root const char cashLOG_Format[128]="%02d.%02d.%04d %02d:%02d %.0s %d %d %d %d";

__root const CFG_HDR cfghdr073 = {CFG_CBOX, "Show In", 0, 3};
__root const int cfgShowIn = 1;
__root const CFG_CBOX_ITEM cfgcbox1[3] = {"Lock", "Unlock", "Both"};

 __root const CFG_HDR cfghdr19={CFG_COORDINATES,"Idle position",0,0};
 __root const unsigned int IDLE_X=0;
 __root const unsigned int IDLE_Y=95;
 
  __root const CFG_HDR cfghdr062 = {CFG_STR_WIN1251,"Format string",0,127};
  __root const char text[128]="капустА: %s%u.%02u";


__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"Настройка шрифта",1,0};
  
  __root const CFG_HDR cfghdr2_8 = {CFG_UINT, "Шрифт(1-png/1xx-sys)", 0, 250};
  __root const unsigned int font = 100 + FONT_SMALL;
  
  __root const CFG_HDR cfghdr23 = {CFG_STR_UTF8, "Путь к png шрифту", 0, 127};
  __root const char fontpath[128] = "";
  
  __root const CFG_HDR cfghdr28={CFG_CBOX,"Использовать полный шрифт",0,2};
  __root const int fullfont=1;
  __root const CFG_CBOX_ITEM cfgcbox7[2]={"No","Yes"};
  
  __root const CFG_HDR cfghdr2_2 = {CFG_UINT, "Расстояние между буквами", 0, 10};
  __root const unsigned int space = 1;
    
  __root const CFG_HDR cfghdr1w073 = {CFG_CBOX, "Выравнивание", 0, 3};
  __root const int align = 1;
  __root const CFG_CBOX_ITEM cfgcbox111[3] = {"Left", "Center", "Right"};
  
    __root const CFG_HDR cfghdr60_2={CFG_COLOR,"Цвет сист. шрифта",0,0};
    __root const char fontColor[4]={0xFF,0xFF,0xFF,100};
    
    __root const CFG_HDR cfghdr60_3={CFG_COLOR,"Цвет фона сист.шрифта",0,0};
    __root const char bgrColor[4]={0x00,0x00,0x00,0x00};
    
    __root const CFG_HDR cfghdr60_4={CFG_CBOX,"Обводить шрифт",0,2};
    __root const int isOutline=0;
    __root const CFG_CBOX_ITEM cfgcbox7_1[2]={"No","Yes"};

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

