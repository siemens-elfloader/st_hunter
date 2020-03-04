#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

/** Адаптация конфига под гсс
 * (с) Z.Vova
 */

// на случай если несколько конфигов
//__CFG_BEGIN(1)
//__CFG_BEGIN(2)
// и так далее

__CFG_BEGIN(0)
/***********************************Background************************************/
__root const CFG_HDR cfghdr0={CFG_LEVEL, "Background", 1, 0};

    __root const CFG_HDR cfghdr0_1 = {CFG_RECT, "Position", 0, 239};
    __root const RECT CFG_POS_BG = {80,29,239,28};

    __root const CFG_HDR cfghdr0_2 = {CFG_CBOX, "Align", 0, 2};
    __root const unsigned int CFG_ALIGN_BG = 0;
    __root const CFG_CBOX_ITEM cfgcbox0_2[2] =
    {
        "Right",
        "Left",
    };

    __root const CFG_HDR cfghdr0_3= {CFG_UINT, "Level of smoothing angles" , 0, 320};
	__root const unsigned int CFG_ANGLES_BG = 3;

__root const CFG_HDR cfghdr0_4={CFG_LEVEL, "", 0, 0};

/***********************************Colors************************************/

__root const CFG_HDR cfghdr1={CFG_LEVEL, "Colors", 1, 0};

    __root const CFG_HDR cfghdr1_1 = {CFG_COLOR, "Frame color", 0, 0};
    __root const char CFG_COL_FRAME[4] = {0x00,0x00,0x00,0x4A};

    __root const CFG_HDR cfghdr1_2 = {CFG_COLOR, "Border frame color", 0, 0};
    __root const char CFG_COL_BORDER_FRAME[4] = {0x15,0x15,0x15,0x64};

    __root const CFG_HDR cfghdr1_3 = {CFG_COLOR, "Fonts color", 0, 0};
    __root const char CFG_COL_FNT[4] = {0xFF,0xFF,0xFF,0x64};

__root const CFG_HDR cfghdr1_4={CFG_LEVEL, "", 0, 0};

/***********************************Icon settings************************************/

    __root const CFG_HDR cfghdr2={CFG_LEVEL, "Icon settings", 1, 0};

    __root const CFG_HDR cfghdr2_1={CFG_STR_UTF8, "Path to Info icon", 0, 127};
    __root const char CFG_PATH_INFO[128]="";

    __root const CFG_HDR cfghdr2_2={CFG_STR_UTF8, "Path to Error icon", 0, 127};
    __root const char CFG_PATH_ERROR[128]="";

__root const CFG_HDR cfghdr2_3={CFG_LEVEL, "", 0, 0};


__root const CFG_HDR cfghdr3= {CFG_UINT, "Autoclose time(sec/10)" , 0, 320};
__root const unsigned int CFG_CLOSE_TIME = 8;


__CFG_END(0)
