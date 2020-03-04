#include "\ARM\inc\cfg_items.h"

__root const CFG_HDR hdr0={CFG_STR_UTF8,"Save path",3,127};
__root const char SAVE_PATH[128]="0:\\Misc\\Shots\\";

__root const CFG_HDR hdr1 = {CFG_INT, "Hotkey (dec)", 0, 255};
__root const int SHOT_BTN = 0x15;

__root const CFG_HDR hdr2={CFG_CBOX,"Longpress",0,2};
__root const int LONG_P=0;
__root const CFG_CBOX_ITEM cbox2[2]={"No","Yes"};

__root const CFG_HDR hdr3={CFG_CBOX,"Enable if keyboard locked",0,2};
__root const int ENA_LOCK=0;
__root const CFG_CBOX_ITEM cbox3[2]={"No","Yes"};

__root const CFG_HDR hdr4={CFG_CBOX,"Enable shot message",0,2};
__root const int ENA_MSG=0;
__root const CFG_CBOX_ITEM cbox4[2]={"No","Yes"};

__root const CFG_HDR hdr5={CFG_STR_WIN1251,"Shot message",1,63};
__root const char SHOT_MSG[64]="Shot!";

