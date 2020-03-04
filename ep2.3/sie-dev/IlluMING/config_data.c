#include "..\inc\cfg_items.h"
#include "../inc/swilib.h"

__root const CFG_HDR cfghdr0={CFG_INT,"Level",0,100};
__root const int LIGHT_LVL=100;

__root const CFG_HDR cfghdr1={CFG_KEYCODE,"Activate key",0,100};
__root const unsigned int CALL_BTN=GREEN_BUTTON;


__root const CFG_HDR cfghdr2={CFG_CBOX,"Activate press type",0,2};
__root const unsigned int CALL_TYPE = 0;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"Short","Long"};

