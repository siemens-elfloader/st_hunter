#include "cfg_items.h"
#include "swilib.h"

#ifdef NEWSGOLD
 #define DEFAULT_DISK "4"
#else
 #define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_UINT,"Авто проверка, часов",0,24};
 __root const unsigned int CHECK_HOURS=1;
__root const CFG_HDR cfghdr1={CFG_UINT,"Авто проверка с",0,24};
 __root const unsigned int CHECK_HOURS_FROM=10;
__root const CFG_HDR cfghdr2={CFG_UINT,"Авто проверка по",0,24};
 __root const unsigned int CHECK_HOURS_TOO=22;
__root const CFG_HDR cfghdr3={CFG_CHECKBOX,"Сохонять USSD",0,0};
 __root const unsigned int ENA_USD = 0;
__root const CFG_HDR cfghdr4={CFG_CHECKBOX,"Писать лог",0,0};
 __root const unsigned int ENA_CASHTRACELOG = 1;
__root const CFG_HDR cfghdr5= {CFG_STR_UTF8,"Log file",0,127};
 __root const char cashLOG_FILE[128]=DEFAULT_DISK ":\\ZBin\\var\\Cash.log";
__root const CFG_HDR cfghdr6= {CFG_STR_UTF8,"Temp path",0,127};
 __root const char cashTEMP_PATH[128]=DEFAULT_DISK ":\\ZBin\\var\\";

__root const CFG_HDR cfghdr7={CFG_LEVEL,"Настройка pattern\'ов",1,0};

__root const CFG_HDR cfghdr8={CFG_LEVEL,"Первый",1,0};
__root const CFG_HDR cfghdr9={CFG_STR_WIN1251,"Number for Cash Request",0,31};
 __root const char CASHREQNUM[32]="*111#";
__root const CFG_HDR cfghdr10={CFG_STR_WIN1251,"Cash pattern 1",0,31};
 __root const char CashPat0[32]="ku: # g";
__root const CFG_HDR cfghdr11= {CFG_STR_UTF8,"PNG file",0,127};
 __root const char PIC_FILE0[128]=DEFAULT_DISK ":\\ZBin\\var\\1.png";
__root const CFG_HDR cfghdr12={CFG_COORDINATES,"Idle position",0,0};
 __root const unsigned int IDLE_X0=4;
 __root const unsigned int IDLE_Y0=100;
__root const CFG_HDR cfghdr13= {CFG_STR_WIN1251,"Format string",0,127};
  __root const char text0[128]="Счёт: %s%u.%02u";
__root const CFG_HDR cfghdr14={CFG_CHECKBOX,"Писать в файл",0,0};
 __root const unsigned int ENA_FILE0= 0;


__root const CFG_HDR cfghdr15={CFG_STR_WIN1251,"Cash pattern 2",0,31};
__root const char CashPat1[32]="ku: # g";
__root const CFG_HDR cfghdr16={CFG_CHECKBOX,"Выводить",0,0};
 __root const unsigned int ENA_CashPat1 = 0;
__root const CFG_HDR cfghdr17= {CFG_STR_UTF8,"PNG file",0,127};
 __root const char PIC_FILE1[128]=DEFAULT_DISK ":\\ZBin\\var\\1.png";
__root const CFG_HDR cfghdr18={CFG_COORDINATES,"Idle position",0,0};
 __root const unsigned int IDLE_X1=4;
 __root const unsigned int IDLE_Y1=120;
__root const CFG_HDR cfghdr19= {CFG_STR_WIN1251,"Format string",0,127};
  __root const char text1[128]="Счёт: %s%u.%02u";
__root const CFG_HDR cfghdr20={CFG_CHECKBOX,"Писать в файл",0,0};
 __root const unsigned int ENA_FILE1= 0;
 
__root const CFG_HDR cfghdr21={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr22={CFG_LEVEL,"Второй",1,0};

__root const CFG_HDR cfghdr23={CFG_STR_WIN1251,"Number for Cash Request",0,31};
 __root const char CASHREQNUM1[32]="*119#";

__root const CFG_HDR cfghdr24={CFG_CHECKBOX,"Выводить",0,0};
 __root const unsigned int ENA_CashPat2 = 0;
__root const CFG_HDR cfghdr25={CFG_STR_WIN1251,"Cash pattern 1",0,31};
 __root const char CashPat2[32]="y: # g";
__root const CFG_HDR cfghdr26= {CFG_STR_UTF8,"PNG file",0,127};
 __root const char PIC_FILE2[128]=DEFAULT_DISK ":\\ZBin\\var\\2.png";
__root const CFG_HDR cfghdr27={CFG_COORDINATES,"Idle position",0,0};
 __root const unsigned int IDLE_X2=4;
 __root const unsigned int IDLE_Y2=140;
__root const CFG_HDR cfghdr28= {CFG_STR_WIN1251,"Format string",0,127};
  __root const char text2[128]="Бонус: %s%u.%02u";
__root const CFG_HDR cfghdr29={CFG_CHECKBOX,"Писать в файл",0,0};
 __root const unsigned int ENA_FILE2= 0;
 
__root const CFG_HDR cfghdr30={CFG_CHECKBOX,"Выводить",0,0};
 __root const unsigned int ENA_CashPat3 = 0;
__root const CFG_HDR cfghdr31={CFG_STR_WIN1251,"Cash pattern 2",0,31};
__root const char CashPat3[32]="n; #  S";
__root const CFG_HDR cfghdr32= {CFG_STR_UTF8,"PNG file",0,127};
 __root const char PIC_FILE3[128]=DEFAULT_DISK ":\\ZBin\\var\\2.png";
__root const CFG_HDR cfghdr33={CFG_COORDINATES,"Idle position",0,0};
 __root const unsigned int IDLE_X3=4;
 __root const unsigned int IDLE_Y3=160;
__root const CFG_HDR cfghdr34= {CFG_STR_WIN1251,"Format string",0,127};
  __root const char text3[128]="SMS: %s%u.%02u";
__root const CFG_HDR cfghdr35={CFG_CHECKBOX,"Писать в файл",0,0};
 __root const unsigned int ENA_FILE3= 0;

__root const CFG_HDR cfghdr36={CFG_CHECKBOX,"Выводить",0,0};
 __root const unsigned int ENA_CashPat4 = 0;
__root const CFG_HDR cfghdr37={CFG_STR_WIN1251,"Cash pattern 2",0,31};
__root const char CashPat4[32]="S; #  M";
__root const CFG_HDR cfghdr38= {CFG_STR_UTF8,"PNG file",0,127};
 __root const char PIC_FILE4[128]=DEFAULT_DISK ":\\ZBin\\var\\2.png";
__root const CFG_HDR cfghdr39={CFG_COORDINATES,"Idle position",0,0};
 __root const unsigned int IDLE_X4=4;
 __root const unsigned int IDLE_Y4=180;
__root const CFG_HDR cfghdr40= {CFG_STR_WIN1251,"Format string",0,127};
  __root const char text4[128]="MMS: %s%u.%02u";
__root const CFG_HDR cfghdr41={CFG_CHECKBOX,"Писать в файл",0,0};
 __root const unsigned int ENA_FILE4= 0;

__root const CFG_HDR cfghdr42={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr43={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr44={CFG_LEVEL,"Настройка шрифта",1,0};
__root const CFG_HDR cfghdr45= {CFG_UINT, "Шрифт(1-png/1xx-sys)", 0, 250};
  __root const unsigned int font = 110;
__root const CFG_HDR cfghdr46= {CFG_STR_UTF8, "Путь к png шрифту", 0, 127};
  __root const char fontpath[128] = "";
__root const CFG_HDR cfghdr47={CFG_CHECKBOX,"Использовать полный шрифт",0,2};
  __root const unsigned int fullfont=1;
__root const CFG_HDR cfghdr48= {CFG_UINT, "Расстояние между буквами", 0, 10};
  __root const unsigned int space = 1;
__root const CFG_HDR cfghdr49= {CFG_CBOX, "Выравнивание", 0, 3};
  __root const unsigned int align = 0;
__root const CFG_CBOX_ITEM cfgcbox50[3] = {"Left", "Center", "Right"};
__root const CFG_HDR cfghdr51={CFG_COLOR,"Цвет сист. шрифта",0,0};
  __root const char fontColor[4]={0xFF,0xFF,0xFF,100};
__root const CFG_HDR cfghdr52={CFG_COLOR,"Цвет фона сист.шрифта",0,0};
  __root const char bgrColor[4]={0x00,0x00,0x00,0x00};
__root const CFG_HDR cfghdr53={CFG_CHECKBOX,"Обводить шрифт",0,2};
  __root const unsigned int isOutline=1;
__root const CFG_HDR cfghdr54={CFG_LEVEL,"",0,0};

