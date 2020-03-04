#include "..\..\inc\cfg_items.h"

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_CHECKBOX,"Instead of mainscreen",0,0};
__root const int CH_IDLE = 0;

//////////////////////////

__root const CFG_HDR cfghdr_00={CFG_LEVEL,"Paths",1,0};

	__root const CFG_HDR cfghdr0_0={CFG_STR_UTF8,"Background Picture",0,63};
	__root const char BG_IMG[64]=DEFAULT_DISK ":\\ZBin\\DeskTop\\img\\fon.png";

	__root const CFG_HDR cfghdr0_1={CFG_STR_UTF8,"Cursor Picture",0,63};
	__root const char CUR_IMG[64]=DEFAULT_DISK ":\\ZBin\\DeskTop\\img\\cur.png";

	__root const CFG_HDR cfghdr0_2={CFG_STR_UTF8,"root folder",0,63};
	__root const char DT_ROOT[64]=DEFAULT_DISK ":\\ZBin\\DeskTop";

	__root const CFG_HDR cfghdr0_3={CFG_STR_UTF8,"MC Path",0,63};
	__root const char MC_PATH[64]=DEFAULT_DISK ":\\ZBin\\mc\\mc.elf";

__root const CFG_HDR cfghdr_01={CFG_LEVEL,"",0,0};

//////////////////////////

__root const CFG_HDR cfghdr_10={CFG_LEVEL,"RightButton-Menu",1,0};

	__root const CFG_HDR cfghdr1_0={CFG_CBOX,"Cursor mode",0,2};
	__root const int MN_SEL_MOD=1;
	__root const CFG_CBOX_ITEM cfgcbox1[]={"Cursor","Without Cursor"};

	__root const CFG_HDR cfghdr1_1={CFG_COLOR,"BG Colour",0,0};
	__root const char MN_COL1[4]={0,0,0,50};

	__root const CFG_HDR cfghdr1_2={CFG_COLOR,"Border Colour",0,0};
	__root const char MN_COL0[4]={0xFF,0xFF,0xFF,50};

	__root const CFG_HDR cfghdr1_3={CFG_COLOR,"Selected BG Colour",0,0};
	__root const char S_MN_COL1[4]={200,200,200,50};

	__root const CFG_HDR cfghdr1_4={CFG_COLOR,"Selected Border Colour",0,0};
	__root const char S_MN_COL0[4]={0xFF,0xFF,0xFF,50};

	__root const CFG_HDR cfghdr1_5={CFG_COLOR,"Font Colour",0,0};
	__root const char MN_FONT_COL[4]={0xFF,0xFF,0xFF,100};

__root const CFG_HDR cfghdr_11={CFG_LEVEL,"",0,0};

//////////////////////////

__root const CFG_HDR cfghdr_20={CFG_LEVEL,"Cursor",1,0};

// 	__root const CFG_HDR cfghdr2_00={CFG_CHECKBOX,"Enable cursor?",0,0};
// 	__root const int CUR_ENABLE = 1;

	__root const CFG_HDR cfghdr2_0={CFG_CHECKBOX,"Cpu Control on Mooving",0,0};
	__root const int KEY_CPU_HI = 1;

	__root const CFG_HDR cfghdr2_1={CFG_CHECKBOX,"Jump on Screen end",0,0};
	__root const int CUR_JUMP = 1;

	__root const CFG_HDR cfghdr2_2={CFG_UINT,"Little Step Count",0,65535};
	__root const unsigned int L_STEP_COUNT=3;

	__root const CFG_HDR cfghdr2_3={CFG_UINT,"Cursor Big Step",0,65535};
	__root const unsigned int CUR_BIG_STEP=10;

	__root const CFG_HDR cfghdr2_4={CFG_UINT,"Cursor Timer",3,65535};
	__root const unsigned int CUR_TIMER=7;

__root const CFG_HDR cfghdr_21={CFG_LEVEL,"",0,0};


