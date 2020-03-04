#include "..\inc\cfg_items.h"

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0 = {CFG_CBOX,"charset (may need reload file)", 0, 5};
__root const int charset = 0;
__root const CFG_CBOX_ITEM cfgcbox1[5] = {"default", "ansi", "unicode le", "unicode be", "utf8"};

__root const CFG_HDR cfghdr_m10 = {CFG_LEVEL, "Font", 1, 0};
__root const CFG_HDR cfghdr10 = {CFG_INT, "Content font", 0, 12};
__root const int font = 8;
__root const CFG_HDR cfghdr11 = {CFG_INT, "row space pixes", 0, 50};
__root const int space = 2;
__root const CFG_HDR cfghdr12 = {CFG_INT, "Statusbar font", 0, 12};
__root const int statfont = 11;
__root const CFG_HDR cfghdr_m11 = {CFG_LEVEL, "", 0, 0};

__root const CFG_HDR cfghdr_m20 = {CFG_LEVEL, "Color", 1, 0};
__root const CFG_HDR cfghdr20 = {CFG_COLOR, "font color", 0, 0};
__root const char fontcolor[4] = {0xFF, 0x0, 0x0, 0x64};
__root const CFG_HDR cfghdr21 = {CFG_COLOR, "background color", 0, 0};
__root const char bgcolor[4] = {0x00, 0x0, 0x0, 0x64};
__root const CFG_HDR cfghdr22 = {CFG_COLOR, "statusbar font color", 0, 0};
__root const char statfontcolor[4] = {0xC9, 0x97, 0xD7, 0x64};
__root const CFG_HDR cfghdr23 = {CFG_COLOR, "statusbar background color", 0, 0};
__root const char statbgcolor[4] = {0x00, 0x0, 0x0, 0x64};
__root const CFG_HDR cfghdr_m21 = {CFG_LEVEL, "", 0, 0};

__root const CFG_HDR cfghdr_m30 = {CFG_LEVEL, "Control", 1, 0};
__root const CFG_HDR cfghdr30 = {CFG_CHECKBOX, "Show status bar", 0, 2};
__root const int status = 1;
__root const CFG_HDR cfghdr31 = {CFG_CBOX, "Create bookmark", 0, 3};
__root const int acbm = 0;
__root const CFG_CBOX_ITEM cfgcbox2[3] = {"default", "always", "never"};
__root const CFG_HDR cfghdr_m31 = {CFG_LEVEL, "", 0, 0};

__root const CFG_HDR cfghdr_m40 = {CFG_LEVEL, "Map file path", 1, 0};
__root const CFG_HDR cfghdr40 = {CFG_STR_UTF8, "gb2uc.map", 0, 127};
__root const char MAP0[128] = DEFAULT_DISK ":\\ZBin\\SieTxtView\\gb2uc.map";
__root const CFG_HDR cfghdr41 = {CFG_STR_UTF8, "uc2gb.map", 0, 127};
__root const char MAP1[128] = DEFAULT_DISK ":\\ZBin\\SieTxtView\\uc2gb.map";
__root const CFG_HDR cfghdr_m41 = {CFG_LEVEL, "", 0, 0};

__root const CFG_HDR cfghdr60 = {CFG_STR_UTF8,"Last opened file", 0, 127};
__root const char LAST[128] = "";
