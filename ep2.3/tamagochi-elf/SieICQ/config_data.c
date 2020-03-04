//  ŒÕ‘»√!!!
#include "..\inc\cfg_items.h"
#include "include.h"

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#define MAX_VOLUME 6
#else
#define DEFAULT_DISK "0"
#define MAX_VOLUME 4
#endif

/* Paths setup */
__root const CFG_HDR cfghdr0 = {CFG_LEVEL, "Paths setup", 1, 0};

__root const CFG_HDR cfghdr0_1 = {CFG_STR_UTF8, "SieICQ folder", 3, 127};
__root const char CFG_SIEICQ_FOLDER[128] = DEFAULT_DISK ":\\ZBin\\SieICQ\\";


__root const CFG_HDR cfghdr0_3 = {CFG_LEVEL, "", 0, 0};


__root const CFG_HDR cfghdr_net2={CFG_LEVEL,"Connection",1,0};
  __root const CFG_HDR cfghdrnet0={CFG_UINT,"UIN",0,0xFFFFFFFF};
  __root const unsigned int UIN=0;
  
  __root const CFG_HDR cfghdrnet1={CFG_STR_PASS,"Password",0,8};
  __root const char PASS[9]="";

//Network settings
  __root const CFG_HDR cfghdrnet2_1={CFG_STR_WIN1251,"Host",0,127};
  __root const char SieICQ_HOST[128]="64.12.161.153";

  __root const CFG_HDR cfghdrnet2_2={CFG_UINT,"Port",0,65535};
  __root const unsigned int SieICQ_PORT=5190;

  __root const CFG_HDR cfghdrnet2_3={CFG_UINT,"Reconnect timeout",0,65535};
  __root const unsigned int RECONNECT_TIME=10;
  
__root const CFG_HDR cfghdr_net21 = {CFG_LEVEL, "", 0, 0};

/* Notify setup */
__root const CFG_HDR cfghdr1 = {CFG_LEVEL, "Notify setup", 1, 0};

__root const CFG_HDR cfghdr1_1 = {CFG_UINT, "Sound volume", 0, MAX_VOLUME};
__root const unsigned int CFG_SOUNDS_VOLUME = MAX_VOLUME;

__root const CFG_HDR cfghdr1_2 = {CFG_UINT, "Vibra power", 0, 100};
__root const unsigned int CFG_VIBRA_POWER = 10;

__root const CFG_HDR cfghdr1_3 = {CFG_LEVEL, "", 0, 0};

/* Logs setup */
__root const CFG_HDR cfghdr2 = {CFG_LEVEL, "Logs setup", 1, 0};

__root const CFG_HDR cfghdr2_1 = {CFG_CHECKBOX, "Save logs to files", 0, 3};
__root const int CFG_LOG_TO_FILE = 1;

__root const CFG_HDR cfghdr2_2 = {CFG_CHECKBOX, "Log date/time", 0, 3};
__root const int CFG_LOG_DATE_TIME = 1;

__root const CFG_HDR cfghdr2_3 = {CFG_LEVEL, "", 0, 0};

/* IDLE icon setup */
__root const CFG_HDR cfghdr3 = {CFG_LEVEL, "IDLE icon setup", 1, 0};

__root const CFG_HDR cfghdr3_1 = {CFG_CHECKBOX, "Show icon on IDLE", 0, 3};
__root const int CFG_SHOW_IDLE_ICON = 1;

__root const CFG_HDR cfghdr3_2 = {CFG_COORDINATES, "IDLE icon position", 0, 0};
__root const unsigned int CFG_IDLE_ICON_X = 0;
__root const unsigned int CFG_IDLE_ICON_Y = 35;

__root const CFG_HDR cfghdr3_3 = {CFG_LEVEL, "", 0, 0};





