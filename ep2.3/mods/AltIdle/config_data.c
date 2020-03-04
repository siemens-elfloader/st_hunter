#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"
#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif
__root const CFG_HDR cfghdr0={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Общие настройки",1,0};//
  
  __root const CFG_HDR cfghdr1={CFG_INT,"Размер шрифта",0,99};
  __root const int font=FONT_SMALL;

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"Настройка координат",1,0};//

  __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Время",0,0};
  __root const unsigned int Clock_x=2;
  __root const unsigned int Clock_y=305;
  
  __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Сеть графически",0,0};
  __root const unsigned int NetExt_x=2;
  __root const unsigned int NetExt_y=2;
  
  __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Сеть db",0,0};
  __root const unsigned int NetDb_x=2;
  __root const unsigned int MetDb_y=10;
  
  __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Аккум графически",0,0};
  __root const unsigned int BattStd_x=142;
  __root const unsigned int BattStd_y=2;
  
  __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Аккум %",0,0};
  __root const unsigned int BattCap_x=203;
  __root const unsigned int DattCap_y=10;
  
  __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Напряжение",0,0};
  __root const unsigned int BattVolt_x=196;
  __root const unsigned int BattVolt_y=26;
  
  __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Оператива",0,0};
  __root const unsigned int FreeRam_x=6;
  __root const unsigned int FreeRam_y=290;
  
__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};//

__root const CFG_HDR cfghdr_2_2={CFG_CHECKBOX,"Enable eMenu",0,2};
__root const int ENA_EMENU = 1;

__root const CFG_HDR cfghdr68={CFG_COORDINATES,"Coordinates panel",0,0};
__root const unsigned int x1=0;
__root const unsigned int y1=75;

__root const CFG_HDR cfghdr60={CFG_COLOR,"Panel border col",0,0};
__root const char cfgPanBorderCol[4]={255,255,255,100};

