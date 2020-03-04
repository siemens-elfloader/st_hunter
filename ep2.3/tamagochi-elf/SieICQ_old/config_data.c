#include "..\inc\cfg_items.h"
//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_UINT,"UIN",0,0xFFFFFFFF};
__root const unsigned int _UIN=0;

__root const CFG_HDR cfghdr1={CFG_STR_PASS,"Password",0,8};
__root const char _PASS[9]="";

//Network settings
__root const CFG_HDR cfghdr2={CFG_LEVEL,"Network settings",1,0};
__root const CFG_HDR cfghdr3={CFG_STR_WIN1251,"Host",0,127};
  __root const char SieICQ_HOST[128]="64.12.161.153;login.icq.com";

__root const CFG_HDR cfghdr4={CFG_UINT,"Default port",0,65535};
  __root const unsigned int SieICQ_PORT=5190;

__root const CFG_HDR cfghdr5={CFG_UINT,"Reconnect timeout",0,65535};
  __root const unsigned int RECONNECT_TIME=10;
  
__root const CFG_HDR cfghdr6={CFG_LEVEL,"",0,0};

//Paths
__root const CFG_HDR cfghdr7={CFG_LEVEL,"Setting paths",1,0};
__root const CFG_HDR cfghdr8={CFG_STR_UTF8,"History",0,63};
  __root const char HIST_PATH[64]=DEFAULT_DISK ":\\ZBin\\SieICQ\\history";

__root const CFG_HDR cfghdr9={CFG_STR_UTF8,"Templates path",0,63};
  __root const char TEMPLATES_PATH[64]=DEFAULT_DISK ":\\ZBin\\SieICQ\\templates";

__root const CFG_HDR cfghdr10={CFG_STR_UTF8,"Images .png path",0,63};
  __root const char ICON_PATH[64]=DEFAULT_DISK ":\\ZBin\\SieICQ\\img";

__root const CFG_HDR cfghdr11={CFG_STR_UTF8,"XStatus .png path",0,63};
  __root const char XSTATUSES_PATH[64]=DEFAULT_DISK ":\\ZBin\\SieICQ\\ximg";

__root const CFG_HDR cfghdr12={CFG_STR_UTF8,"Smiles File",0,63};
  __root const char SMILE_FILE[64]=DEFAULT_DISK ":\\ZBin\\SieICQ\\smiles.cfg";

__root const CFG_HDR cfghdr13={CFG_STR_UTF8,"Smiles .png path",0,63};
  __root const char SMILE_PATH[64]=DEFAULT_DISK ":\\ZBin\\SieICQ\\smiles";


__root const CFG_HDR cfghdr14={CFG_LEVEL,"",0,0};





