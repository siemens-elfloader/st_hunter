#include "..\inc\cfg_items.h"

__root const CFG_HDR hdr0={CFG_STR_UTF8,"Save path",3,127};
__root const char SAVE_PATH[128]="0:\\Misc\\Shots\\";

__root const CFG_HDR hdr2={CFG_CBOX,"Format",0,2};
__root const int FORMAT=0;
__root const CFG_CBOX_ITEM cbox2[2]={"bmp","jpg"};

__root const CFG_HDR hdr3 = {CFG_INT, "JPEG quality", 0, 100};
__root const int JPG_QUAL = 75;

/*__root const CFG_HDR hdr4={CFG_CHECKBOX,"Progressive JPEG",0,1};
__root const int JPG_PR=0;*/

__root const CFG_HDR hdr5 = {CFG_INT, "Hotkey (dec)", 0, 255};
__root const int SHOT_BTN = 0x15;

__root const CFG_HDR hdr6={CFG_CHECKBOX,"Longpress",0,1};
__root const int LONG_P=0;

__root const CFG_HDR hdr7={CFG_CHECKBOX,"Enable if keyboard locked",0,1};
__root const int ENA_LOCK=0;

__root const CFG_HDR hdr8={CFG_CHECKBOX,"Enable shot message",0,1};
__root const int ENA_MSG=0;

__root const CFG_HDR hdr9={CFG_STR_WIN1251,"Shot message",1,63};
__root const char SHOT_MSG[64]="Shot!";

