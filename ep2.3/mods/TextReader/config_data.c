#include "..\inc\cfg_items.h"

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif
__root const CFG_HDR cfghdr0={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Общие настройки",1,0};  
  __root const CFG_HDR cfghdr_10={CFG_CBOX,"Использовать цвет и шрифт скина",0,2};
  __root const int scin_mode=0;
  __root const CFG_CBOX_ITEM cfgcbox0[2]={"Нет","Да"};  
  
  __root const CFG_HDR cfghdr1={CFG_INT,"Размер шрифта",0,99};
  __root const int size_font=1;

  __root const CFG_HDR cfghdr2={CFG_INT,"Цвет шрифта",0,99};
  __root const int pen_font=0;

  __root const CFG_HDR cfghdr3={CFG_INT,"Цвет обводки(прозрачн.32)",0,256};
  __root const int brush_font=32;

  __root const CFG_HDR cfghdr4={CFG_CBOX,"Подсветка",0,2};
  __root const int illum_mode=0;
  __root const CFG_CBOX_ITEM cfgcbox2[2]={"Нет","Да"};

  __root const CFG_HDR cfghdr7={CFG_CBOX,"Показывать заголовок",0,2};
  __root const int screen_mode=0;
  __root const CFG_CBOX_ITEM cfgcbox3[2]={"Нет","Да"};
  

  __root const CFG_HDR cfghdr15={CFG_CBOX,"Делать бэкап",0,2};
  __root const int backup=0;
  __root const CFG_CBOX_ITEM cfgcbox6[2]={"No","Yes"};

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"Пути",1,0};

  __root const CFG_HDR cfghdr12={CFG_STR_UTF8,"Путь к истории",0,127};
  __root const char HIST_PATH[128]=DEFAULT_DISK ":\\ZBin\\TextReader\\history\\";

  __root const CFG_HDR cfghdr23={CFG_STR_UTF8,"Путь к закладкам",0,127};
  __root const char BMK_PATH[128]=DEFAULT_DISK ":\\ZBin\\TextReader\\Bookmarks\\";
/*
  __root const CFG_HDR cfghdr24={CFG_STR_UTF8,"Image path",0,127};
  __root const char IMG_PATH[128]=DEFAULT_DISK ":\\ZBin\\img\\TextReader";
*/
__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};


