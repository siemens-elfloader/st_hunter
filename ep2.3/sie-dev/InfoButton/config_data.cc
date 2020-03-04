#include "../inc/cfg_items.h"
#include "../inc/swilib.h"

__root const CFG_HDR cfghdr1= {CFG_LEVEL, "Main settings", 1, 0};

    __root const CFG_HDR cfghdr1_2= {CFG_RECT, "Background position", 0, 0};
    __root const RECT CFG_POS_BACK = {80, 24, 239, 123};

/*__root const CFG_HDR cfghdr1_3 = {CFG_CHECKBOX, "Open animation", 0, 3};
__root const int cfg_anim_open = 1;

__root const CFG_HDR cfghdr1_4= {CFG_CBOX,"Open animation style",0, 4};
__root const int cfg_anim_open_style=0;
__root const CFG_CBOX_ITEM cfgcbox1_5[4]= {"Left", "Right", "Down", "Up"};

__root const CFG_HDR cfghdr1_6 = {CFG_CHECKBOX, "Close animation", 0, 3};
__root const int CFG_ANIM_CLOSED = 1;

__root const CFG_HDR cfghdr1_7= {CFG_CBOX,"Close animation style",0, 4};
__root const int CFG_ANIM_CLOSED_STYLE=1;
__root const CFG_CBOX_ITEM cfgcbox1_8[4]= {"Left", "Right", "Down", "Up"};

__root const CFG_HDR cfghdr1_9= {CFG_UINT,"Animation step",0,65535};
__root const int cfg_anim_step=12;

__root const CFG_HDR cfghdr1_10= {CFG_UINT,"Animation speed",0,65535};
__root const int cfg_anim_speed=1;*/

    __root const CFG_HDR cfghdr1_11= {CFG_UINT,"Auto update time (sec)",0,65535};
    __root const int CFG_AUTOUPDATE_TMR=1;

    __root const CFG_HDR cfghdr10_1= {CFG_LEVEL, "Colors", 1, 0};

        __root const CFG_HDR cfghdr10_2= {CFG_COLOR,"Background color",0,0};
        __root const char CFG_COL_BACK_BACK[4]= {0x00,0x00,0x00, 0x50};

        __root const CFG_HDR cfghdr10_3= {CFG_COLOR,"Background border color",0,0};
        __root const char CFG_COL_BACK_BORDER[4]= {0x40,0x40,0x40,0x64};

        __root const CFG_HDR cfghdr10_4= {CFG_COLOR,"Progressbar background",0,0};
        __root const char CFG_COL_PR_BG[4]= {0x00,0x00,0x00, 0x35};

        __root const CFG_HDR cfghdr10_5= {CFG_COLOR,"Progressbar border",0,0};
        __root const char CFG_COL_PR_BORDER[4]= {0xcc,0xcc,0xcc,0x5a};

        __root const CFG_HDR cfghdr10_6= {CFG_COLOR,"70%<Progressbar filled<100%",0,0};
        __root const char CFG_COL_PR_FILL3[4]= {0x94,0x94,0x94, 0xFF};

        __root const CFG_HDR cfghdr10_7= {CFG_COLOR,"30%<Progressbar filled<70% ",0,0};
        __root const char CFG_COL_PR_FILL2[4]= {0x94,0x94,0x94, 0xFF};

        __root const CFG_HDR cfghdr10_8= {CFG_COLOR,"0%<Progressbar filled<30%",0,0};
        __root const char CFG_COL_PR_FILL1[4]= {0xFF,0x00,0x00, 0xFF};

        __root const CFG_HDR cfghdr10_9= {CFG_COLOR,"Font",0,0};
        __root const char CFG_COL_FNT[4]= {0xff,0xff,0xff,0xff};

    __root const CFG_HDR cfghdr10_10= {CFG_LEVEL, "", 0, 0};


    __root const CFG_HDR cfghdr11= {CFG_LEVEL, "Keyboard", 1, 0};

        __root const CFG_HDR cfghdr11_1= {CFG_CBOX,"Activation style",0,2};
        __root const int CFG_ACTIVATION_STYLE=1;
        __root const CFG_CBOX_ITEM cfgcbox11_2[2]= {"Key down", "Long press"};

        __root const CFG_HDR cfghdr11_3= {CFG_KEYCODE, "Activation key", 0, 99};
        __root const int CFG_KEY_OPEN=ENTER_BUTTON;

        __root const CFG_HDR cfghdr11_4= {CFG_KEYCODE, "Close key", 0, 99};
        __root const int CFG_KEY_CLOSE=RIGHT_SOFT;

        __root const CFG_HDR cfghdr11_5= {CFG_KEYCODE, "About", 0, 99};
        __root const int cfg_key_about='*';

        __root const CFG_HDR cfghdr11_6= {CFG_KEYCODE, "Open config", 0, 99};
        __root const int cfg_key_openconfig='#';

    __root const CFG_HDR cfghdr11_7= {CFG_LEVEL, "", 0, 0};

__root const CFG_HDR cfghdr1_12= {CFG_LEVEL, "", 0, 0};

//=====================================FreeRAM================================================================
__root const CFG_HDR cfghdr2= {CFG_LEVEL, "FreeRAM", 1, 0};

    __root const CFG_HDR cfghdr2_1 = {CFG_CHECKBOX, "Enable", 0, 3};
    __root const int CFG_RAM_ENA = 1;

    __root const CFG_HDR cfghdr2_2= {CFG_RECT, "Bar position", 0, 0};
    __root const RECT CFG_RAM_POS = {120, 28, 234, 39};

    __root const CFG_HDR cfghdr2_3 = {CFG_COORDINATES, "Text position" , 0, 0};
    __root const int CFG_RAM_X_TEXT = 85;
    __root const int CFG_RAM_Y_TEXT = 24;

    __root const CFG_HDR cfghdr2_4= {CFG_CBOX, "Show in", 0, 3};
    __root const int CFG_RAM_MEASURE=2;
    __root const CFG_CBOX_ITEM cfgcbox2_5[3]=
    {
        "B",
        "KB",
        "MB"
    };

    __root const CFG_HDR cfghdr2_6= {CFG_STR_UTF8, "Text", 0, 127};
    __root const char CFG_RAM_TEXT[128]="RAM:    %f MB";

    __root const CFG_HDR cfghdr2_7= {CFG_UINT,"Total ram(kb)",0,65535};
    __root const int CFG_RAM_TOTAL=9000;

    __root const CFG_HDR cfghdr2_8= {CFG_CBOX, "Font", 0, 8};
    __root const int CFG_RAM_FNT=0;
    __root const CFG_CBOX_ITEM cfgcbox2_9[8]=
    {
        "Small italic",
        "Small italic bold",
        "Small",
        "Small bold",
        "Medium",
        "Medium bold",
        "Large",//
        "Large bold"
    };

__root const CFG_HDR cfghdr2_11= {CFG_LEVEL, "", 0, 0};

//==================================FreeFlexSpace==================================================================
__root const CFG_HDR cfghdr3= {CFG_LEVEL, "FreeFlexSpace 0", 1, 0};

    __root const CFG_HDR cfghdr3_1 = {CFG_CHECKBOX, "Enable", 0, 3};
    __root const int CFG_FLEX0_ENA = 1;

    __root const CFG_HDR cfghdr3_3= {CFG_RECT, "Bar position", 0, 0};
    __root const RECT CFG_FLEX0_POS = {120, 44, 234, 55};

    __root const CFG_HDR cfghdr3_2 = {CFG_COORDINATES, "Text position" , 0, 0};
    __root const int CFG_FLEX0_X_TEXT = 85;
    __root const int CFG_FLEX0_Y_TEXT = 40;

    __root const CFG_HDR cfghdr3_4= {CFG_CBOX, "Show in", 0, 3};
    __root const int CFG_FLEX0_MEASURE=1;
    __root const CFG_CBOX_ITEM skincbox3_5[3]=
    {
        "B",
        "KB",
        "MB"
    };

    __root const CFG_HDR cfghdr3_6= {CFG_STR_UTF8, "Text", 0, 127};
    __root const char CFG_FLEX0_TEXT[128]="Disk 0:  %f KB";

    __root const CFG_HDR cfghdr3_7= {CFG_CBOX, "Font", 0, 8};
    __root const int CFG_FLEX0_FNT=0;
    __root const CFG_CBOX_ITEM skincbox3_8[8]=
    {
        "Small italic",
        "Small italic bold",
        "Small",
        "Small bold",
        "Medium",
        "Medium bold",
        "Large",
        "Large bold"
    };

__root const CFG_HDR cfghdr3_9= {CFG_LEVEL, "", 0, 0};

//==================================FreeFlexSpace 1==================================================================
__root const CFG_HDR cfghdr4= {CFG_LEVEL, "FreeFlexSpace 1", 1, 0};

    __root const CFG_HDR cfghdr4_1 = {CFG_CHECKBOX, "Enable", 0, 3};
    __root const int CFG_FLEX1_ENA = 0;

    __root const CFG_HDR cfghdr4_3= {CFG_RECT, "Bar position", 0, 0};
    __root const RECT CFG_FLEX1_POS = {120, 46, 234, 57};

    __root const CFG_HDR cfghdr4_2 = {CFG_COORDINATES, "Text position" , 0, 0};
    __root const int CFG_FLEX1_X_TEXT = 85;
    __root const int CFG_FLEX1_Y_TEXT = 42;

    __root const CFG_HDR cfghdr4_4= {CFG_CBOX, "Show in", 0, 3};
    __root const int CFG_FLEX1_MEASURE=1;
    __root const CFG_CBOX_ITEM skincbox4_5[3]=
    {
        "B",
        "KB",
        "MB"
    };

    __root const CFG_HDR cfghdr4_6= {CFG_STR_UTF8, "Text", 0, 127};
    __root const char CFG_FLEX1_TEXT[128]="Disk 1:  %f KB";

    __root const CFG_HDR cfghdr4_7= {CFG_CBOX, "Font", 0, 8};
    __root const int CFG_FLEX1_FNT=0;
    __root const CFG_CBOX_ITEM skincbox4_8[8]=
    {
        "Small italic",
        "Small italic bold",
        "Small",
        "Small bold",
        "Medium",
        "Medium bold",
        "Large",
        "Large bold"
    };

__root const CFG_HDR cfghdr4_9= {CFG_LEVEL, "", 0, 0};

//==================================FreeFlexSpace 2==================================================================
__root const CFG_HDR cfghdr5= {CFG_LEVEL, "FreeFlexSpace 2", 1, 0};

    __root const CFG_HDR cfghdr5_1 = {CFG_CHECKBOX, "Enable", 0, 3};
    __root const int CFG_FLEX2_ENA = 0;

    __root const CFG_HDR cfghdr5_3= {CFG_RECT, "Bar position", 0, 0};
    __root const RECT CFG_FLEX2_POS = {120, 46, 234, 57};

    __root const CFG_HDR cfghdr5_2 = {CFG_COORDINATES, "Text position" , 0, 0};
    __root const int CFG_FLEX2_X_TEXT = 85;
    __root const int CFG_FLEX2_Y_TEXT = 42;

    __root const CFG_HDR cfghdr5_4= {CFG_CBOX, "Show in", 0, 3};
    __root const int CFG_FLEX2_MEASURE=1;
    __root const CFG_CBOX_ITEM skincbox5_5[3]=
    {
        "B",
        "KB",
        "MB"
    };

    __root const CFG_HDR cfghdr5_6= {CFG_STR_UTF8, "Text", 0, 127};
    __root const char CFG_FLEX2_TEXT[128]="Disk 2:  %f KB";

    __root const CFG_HDR cfghdr5_7= {CFG_CBOX, "Font", 0, 8};
    __root const int CFG_FLEX2_FNT=0;
    __root const CFG_CBOX_ITEM skincbox5_8[8]=
    {
        "Small italic",
        "Small italic bold",
        "Small",
        "Small bold",
        "Medium",
        "Medium bold",
        "Large",
        "Large bold"
    };

__root const CFG_HDR cfghdr5_9= {CFG_LEVEL, "", 0, 0};

//==================================FreeFlexSpace 4==================================================================
__root const CFG_HDR cfghdr6= {CFG_LEVEL, "FreeFlexSpace 4", 1, 0};

    __root const CFG_HDR cfghdr6_1 = {CFG_CHECKBOX, "Enable", 0, 3};
    __root const int CFG_FLEX4_ENA = 1;

    __root const CFG_HDR cfghdr6_3= {CFG_RECT, "Bar position", 0, 0};
    __root const RECT CFG_FLEX4_POS = {120, 61, 234, 72};

    __root const CFG_HDR cfghdr6_2 = {CFG_COORDINATES, "Text position" , 0, 0};
    __root const int CFG_FLEX4_X_TEXT = 85;
    __root const int CFG_FLEX4_Y_TEXT = 57;

    __root const CFG_HDR cfghdr6_4= {CFG_CBOX, "Show in", 0, 3};
    __root const int CFG_FLEX4_MEASURE=2;
    __root const CFG_CBOX_ITEM skincbox6_5[3]=
    {
        "B",
        "KB",
        "MB"
    };

    __root const CFG_HDR cfghdr6_6= {CFG_STR_UTF8, "Text", 0, 127};
    __root const char CFG_FLEX4_TEXT[128]="Flash:   %.0f MB";

    __root const CFG_HDR cfghdr6_7= {CFG_CBOX, "Font", 0, 8};
    __root const int CFG_FLEX4_FNT=0;
    __root const CFG_CBOX_ITEM skincbox6_8[8]=
    {
        "Small italic",
        "Small italic bold",
        "Small",
        "Small bold",
        "Medium",
        "Medium bold",
        "Large",
        "Large bold"
    };

__root const CFG_HDR cfghdr6_9= {CFG_LEVEL, "", 0, 0};

//=========================================Capacity=============================================================
__root const CFG_HDR cfghdr7= {CFG_LEVEL, "Capacity", 1, 0};

    __root const CFG_HDR cfghdr7_1 = {CFG_CHECKBOX, "Enable", 0, 3};
    __root const int CFG_CAP_ENA = 1;

    __root const CFG_HDR cfghdr7_3= {CFG_RECT, "Bar position", 0, 0};
    __root const RECT CFG_CAP_POS = {120, 77, 234, 88};

    __root const CFG_HDR cfghdr7_2 = {CFG_COORDINATES, "Text position" , 0, 0};
    __root const int CFG_CAP_X_TEXT = 85;
    __root const int CFG_CAP_Y_TEXT = 73;

    __root const CFG_HDR cfghdr7_6= {CFG_STR_UTF8, "Text", 0, 127};
    __root const char CFG_CAP_TEXT[128]="CAP:     %.0f";

    __root const CFG_HDR cfghdr7_7= {CFG_CBOX, "Font", 0, 8};
    __root const int CFG_CAP_FNT=0;
    __root const CFG_CBOX_ITEM skincbox7_8[8]=
    {
        "Small italic",
        "Small italic bold",
        "Small",
        "Small bold",
        "Medium",
        "Medium bold",
        "Large",
        "Large bold"
    };

__root const CFG_HDR cfghdr7_9= {CFG_LEVEL, "", 0, 0};

//==================================CPU=======================================================================

__root const CFG_HDR cfghdr8= {CFG_LEVEL, "CPU load", 1, 0};

    __root const CFG_HDR cfghdr8_1 = {CFG_CHECKBOX, "Enable", 0, 3};
    __root const int CFG_CPU_ENA = 1;

    __root const CFG_HDR cfghdr8_3= {CFG_RECT, "Bar position", 0, 0};
    __root const RECT CFG_CPU_POS = {120, 93, 234, 104};

    __root const CFG_HDR cfghdr8_2 = {CFG_COORDINATES, "Text position" , 0, 0};
    __root const int CFG_CPU_X_TEXT = 85;
    __root const int CFG_CPU_Y_TEXT = 89;


    __root const CFG_HDR cfghdr8_6= {CFG_STR_UTF8, "Text", 0, 127};
    __root const char CFG_CPU_TEXT[128]="CPU:     %.0f";

    __root const CFG_HDR cfghdr8_7= {CFG_CBOX, "Font", 0, 8};
    __root const int CFG_CPU_FNT=0;
    __root const CFG_CBOX_ITEM skincbox8_8[8]=
    {
        "Small italic",
        "Small italic bold",
        "Small",
        "Small bold",
        "Medium",
        "Medium bold",
        "Large",
        "Large bold"
    };

    __root const CFG_HDR cfghdr8_9= {CFG_COLOR,"70%<Progressbar filled<100%",0,0};
    __root const char CFG_CPU_COL_FILL3[4]= {0xff,0x00,0x00, 0xFF};

    __root const CFG_HDR cfghdr8_10= {CFG_COLOR,"30%<Progressbar filled<70% ",0,0};
    __root const char CFG_CPU_COL_FILL2[4]= {0x94,0x94,0x94, 0xFF};

    __root const CFG_HDR cfghdr8_11= {CFG_COLOR,"0%<Progressbar filled<30%",0,0};
    __root const char CFG_CPU_COL_FILL1[4]= {0x94,0x94,0x94, 0xFF};

__root const CFG_HDR cfghdr8_12= {CFG_LEVEL, "", 0, 0};
