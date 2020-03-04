#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"

__root const CFG_HDR cfghdr0={CFG_COLOR,"Main BG Color",0,0};
__root const char main_bg_color[4]={0,0,0,100};

__root const CFG_HDR cfghdr1={CFG_UINT,"Main Text Font",0,65535};
__root const unsigned int main_font=FONT_SMALL;

__root const CFG_HDR cfghdr2={CFG_COLOR,"Main Text Color",0,0};
__root const char main_text_color[4]={255,255,255,100};

__root const CFG_HDR cfghdr3={CFG_UINT,"Soft Key Font",0,65535};
__root const unsigned int sk_font=FONT_MEDIUM;

__root const CFG_HDR cfghdr4={CFG_COLOR,"Soft Key Color",0,0};
__root const char sk_color[4]={255,255,255,100};

__root const CFG_HDR cfghdr5={CFG_UINT,"SystemInfo Refresh Time 0.1s",0,600};
__root const unsigned int sys_re_t=5;

__root const CFG_HDR cfghdr6={CFG_UTF8_STRING,"String for Font Test",0,255};
__root const char test_string[256]="Test String 123";

__root const CFG_HDR cfghdr7={CFG_COLOR,"Text Attribute Brush Color",0,0};
__root const char t_a_b_color[4]={255,0,0,100};

__root const CFG_HDR cfghdr8={CFG_CBOX, "Default Vibra Status", 0, 2};
__root const int default_vibra_sta = 0;
__root const CFG_CBOX_ITEM cfgcbox0[2] = {"OFF", "ON"};

__root const CFG_HDR cfghdr9 = {CFG_CBOX, "Default Light In", 0, 4};
__root const int default_light_sta = 3;
__root const CFG_CBOX_ITEM cfgcbox1[4] = {"All","Display","Keybord","Do Nothing"};

