#include "../inc/cfg_items.h"
#include "../inc/swilib.h"

__root const CFG_HDR cfghdr1={CFG_STR_UTF8,"Skin folder",0,127};
__root const char CFG_PATH_SKIN[128]="";

__root const CFG_HDR cfghdr2={CFG_LEVEL, "Keyboard", 1, 0};

    __root const CFG_HDR cfghdr2_1={CFG_KEYCODE, "Activation key 1", 0, 99};
    __root const int cfg_key_activation1=VOL_UP_BUTTON;
    
    __root const CFG_HDR cfghdr2_2={CFG_KEYCODE, "Activation key 2", 0, 99};
    __root const int cfg_key_activation2=VOL_DOWN_BUTTON;
    
	__root const CFG_HDR cfghdr2_3={CFG_KEYCODE, "About...", 0, 99};
	__root const int cfg_key_about='*';
	
	__root const CFG_HDR cfghdr2_4={CFG_KEYCODE, "Open skin", 0, 99};
	__root const int cfg_key_open_skin='#';
	
__root const CFG_HDR cfghdr2_5={CFG_LEVEL, "", 0, 0};

__root const CFG_HDR cfghdr1_1={CFG_LEVEL, "", 0, 0};
