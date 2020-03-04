#include "../inc/cfg_items.h"
#include "../inc/swilib.h"

__root const CFG_HDR cfghdr0={CFG_STR_UTF8, "Skin folder", 0, 127};
__root const char CFG_PATH_SKIN[128]="";

__root const CFG_HDR cfghdr1 = {CFG_CHECKBOX, "Hide on startup", 0, 2};
__root const int CFG_STAY_AFTER_LAUNCH = 0;

__root const CFG_HDR cfghdr6={CFG_CBOX, "Illumination hold on", 0, 3};
__root const int CFG_KEEP_LIGHT=0;
__root const CFG_CBOX_ITEM cfgcbox6_1[3]=
{
  "Off",
  "As in mediaplayer",
  "Always",
};
 
__root const CFG_HDR cfghdr2_8={CFG_LEVEL, "", 0, 0};
//Messagers
__root const CFG_HDR cfghdr3={CFG_LEVEL, "Pop-up messages", 1, 0};

 __root const CFG_HDR cfghdr3_1 = {CFG_CHECKBOX, "No results for saving", 0, 2};
 __root const int CFG_MSG_NO_RESULTS = 1;

 __root const CFG_HDR cfghdr3_2 = {CFG_CHECKBOX, "Saving results pop-up message", 0, 2};
 __root const int CFG_MSG_SAVE_RESULTS = 0;
 
 __root const CFG_HDR cfghdr3_3 = {CFG_CHECKBOX, "Settings amendment message", 0, 2};
 __root const int CFG_MSG_REQ = 0;
 
 __root const CFG_HDR cfghdr3_4 = {CFG_CHECKBOX, "Exit confirmation", 0, 2};
 __root const int CFG_CFM_ON_EXIT = 1;
 
__root const CFG_HDR cfghdr3_5={CFG_LEVEL, "", 0, 0};
//Keyboard
__root const CFG_HDR cfghdr4={CFG_LEVEL, "Keyboard", 1, 0};

 __root const CFG_HDR cfghdr4_1={CFG_CBOX,"Right softkey action",0,2};
 __root const int CFG_ACTION_RIGHT_SOFT=0;
 __root const CFG_CBOX_ITEM cfgcbox4_2[2]={"Exit", "Stop and exit"};

 __root const CFG_HDR cfghdr4_3={CFG_KEYCODE, "Start and Split button", 0, 99};
 __root const int CFG_BUTTON_START_SPLIT=LEFT_SOFT;
 
 __root const CFG_HDR cfghdr4_4={CFG_KEYCODE, "Save results button", 0, 99};
 __root const int CFG_BUTTON_SAVE=GREEN_BUTTON;

 __root const CFG_HDR cfghdr4_5={CFG_KEYCODE, "Next page button", 0, 99};
 __root const int CFG_SWITCH_NEXT_PAGES=UP_BUTTON;
 
 __root const CFG_HDR cfghdr4_6={CFG_KEYCODE, "Previous page button", 0, 99};
 __root const int CFG_SWITCH_PREV_PAGES=DOWN_BUTTON;
 
 __root const CFG_HDR cfghdr4_7={CFG_KEYCODE, "Configuration button", 0, 99};
 __root const int CFG_BUTTON_OPEN_CFG='#';
 
 __root const CFG_HDR cfghdr4_8={CFG_KEYCODE, "About... button", 0, 99};
 __root const int CFG_BUTTON_SHOW_ABOUT='*';
 
__root const CFG_HDR cfghdr4_9={CFG_LEVEL, "", 0, 0};
//Notify
__root const CFG_HDR cfghdr5={CFG_LEVEL, "Notifications", 1, 0};

 __root const CFG_HDR cfghdr5_1={CFG_INT, "Vibration power (0 turns off)", 0, 100};
 __root const int CFG_VIBRA_POWER=10;

 __root const CFG_HDR cfghdr5_2={CFG_INT, "System sound number(0 turns off)", 0, 200};
 __root const int CFG_NUMBER_SYSTEM_SOUND=10;
 
 __root const CFG_HDR cfghdr5_4={CFG_INT, "Notify every N seconds", 0, 216000};
 __root const int CFG_ALERT_SECONDS=60;

__root const CFG_HDR cfghdr5_5={CFG_LEVEL, "", 0, 0};

