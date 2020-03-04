#include "../inc/swilib.h"
#include "../inc/cfg_items.h"

__root const CFG_HDR skinhdr1={CFG_LEVEL, "Coordinates", 1, 0};

	__root const CFG_HDR skinhdr1_1 = {CFG_COORDINATES, "Current time" , 0, 0};
	__root const unsigned int CFG_X_CUR_TIME = 80;
	__root const unsigned int CFG_Y_CUR_TIME = 50;
	
	__root const CFG_HDR skinhdr1_2= {CFG_UINT, "Milliseconds offset (Y axes)" , 0, 320};
	__root const int CFG_INDENT_Y_CUR_TIME_SEC = 26;

	__root const CFG_HDR skinhdr1_3= {CFG_COORDINATES, "Split timing" , 0, 0};
	__root const unsigned int CFG_X_SPLIT_TIMES = 25;
	__root const unsigned int CFG_Y_SPLIT_TIMES = 198;
	
	__root const CFG_HDR skinhdr1_4= {CFG_COORDINATES, "Number results" , 0, 0};
	__root const unsigned int CFG_X_NUM_RES = 4;
	__root const unsigned int CFG_Y_NUM_RES = 198;
	
	__root const CFG_HDR skinhdr1_5= {CFG_UINT, "X axes distance between fields" , 0, 320};
	__root const unsigned int CFG_INDENT_X_SPLIT_TIMES = 116;
	
	__root const CFG_HDR skinhdr1_6= {CFG_UINT, "Y axes distance between fields" , 0, 320};
	__root const unsigned int CFG_INDENT_Y_SPLIT_TIMES = 22;
	
	__root const CFG_HDR skinhdr1_7= {CFG_UINT, "Number result offset (X axes)" , 0, 320};
	__root const unsigned int CFG_INDENT_X_RES_NUM = 116;
 
	__root const CFG_HDR skinhdr1_8= {CFG_UINT, "Number results offset (Y axes)" , 0, 320};
	__root const unsigned int CFG_INDENT_Y_RES_NUM = 22;
	
	__root const CFG_HDR skinhdr1_9 = {CFG_COORDINATES, "Left button position" , 0, 0};
	__root const unsigned int CFG_X_BUTTON_LEFT = 3;
	__root const unsigned int CFG_Y_BUTTON_LEFT = 117;
	
	__root const CFG_HDR skinhdr1_10 = {CFG_COORDINATES, "Right button position" , 0, 0};
	__root const unsigned int CFG_X_BUTTON_RIGHT = 126;
	__root const unsigned int CFG_Y_BUTTON_RIGHT = 117;
		
__root const CFG_HDR skinhdr1_11={CFG_LEVEL, "", 0, 0};

__root const CFG_HDR skinhdr2={CFG_LEVEL, "Colors", 1, 0};//

	__root const CFG_HDR skinhdr2_1 = {CFG_COLOR, "Current time", 0, 0};
	__root const char CFG_COLOR_CUR_TIME[4] = {0x00,0x00,0x00,0x64};
	
	__root const CFG_HDR skinhdr2_2 = {CFG_COLOR, "Split timing", 0, 0};
	__root const char CFG_COLOR_SPLIT_TIMES[4] = {0x00,0x00,0x00,0x64};

__root const CFG_HDR skinhdr2_3={CFG_LEVEL, "", 0, 0};

__root const CFG_HDR skinhdr3_1={CFG_LEVEL, "Fonts", 1, 0};//

	__root const CFG_HDR skinhdr3_2={CFG_CBOX, "Current time", 0, 6};
	__root const int CFG_FONT_CUR_TIME=4;
	__root const CFG_CBOX_ITEM skincbox3_3[6]=
	{
		"Small",
		"Small bold",
		"Medium",
		"Medium bold",
		"Large",
		"Large bold"
	};
	
	__root const CFG_HDR skinhdr3_4={CFG_CBOX, "Current time milliseconds", 0, 6};
	__root const int CFG_FONT_CUR_TIME_SEC=3;
	__root const CFG_CBOX_ITEM skincbox3_5[6]=
	{
		"Small",
		"Small bold",
		"Medium",
		"Medium bold",
		"Large",
		"Large bold"
	};
	
	__root const CFG_HDR skinhdr3_6={CFG_CBOX, "Split timing", 0, 6};
	__root const int CFG_FONT_SPLIT_TIMES=2;
	__root const CFG_CBOX_ITEM skincbox3_7[6]=
	{
		"Small",
		"Small bold",
		"Medium",
		"Medium bold",
		"Large",
		"Large bold"
	};
	
	__root const CFG_HDR skinhdr3_8={CFG_CBOX, "Result number", 0, 6};
	__root const int CFG_FONT_RES_NUM=0;
	__root const CFG_CBOX_ITEM skincbox3_9[6]=
	{
		"Small",
		"Small bold",
		"Medium",
		"Medium bold",
		"Large",
		"Large bold"
	};
	
__root const CFG_HDR skinhdr3_10={CFG_LEVEL, "", 0, 0};

__root const CFG_HDR skinhdr4={CFG_LEVEL, "Data on Idle", 1, 0};

	__root const CFG_HDR skinhdr4_1={CFG_CHECKBOX, "Show current time", 0, 3};
	 __root const int CFG_SHOW_CUR_TIME_IDLE = 0;
	 
	 __root const CFG_HDR skinhdr4_2={CFG_CHECKBOX, "Show icons", 0, 3};
	 __root const int CFG_SHOW_ICONS_IDLE = 0;
	 
	 __root const CFG_HDR skinhdr4_3 = {CFG_COORDINATES, "Current time position" , 0, 0};
	 __root const unsigned int CFG_X_CUR_TIME_IDLE = 126;
	 __root const unsigned int CFG_Y_CUR_TIME_IDLE = 117;
	 
	 __root const CFG_HDR skinhdr4_4 = {CFG_COORDINATES, "Icons position" , 0, 0};
	 __root const unsigned int CFG_X_ICONS_IDLE = 126;
	 __root const unsigned int CFG_Y_ICONS_IDLE = 117;
	 
	 __root const CFG_HDR skinhdr4_5 = {CFG_COLOR, "Current time color", 0, 0};
	 __root const char CFG_COLOR_CUR_TIME_IDLE[4] = {0xFF,0xFF,0xFF,0x64};
	 
	 __root const CFG_HDR skinhdr4_6={CFG_CBOX, "Font", 0, 6};
	 __root const int CFG_FONT_CUR_TIME_IDLE=0;
	 __root const CFG_CBOX_ITEM skincbox4_7[6]=
	 {
		 "Small",
		 "Small bold",
		 "Medium",
		 "Medium bold",
		 "Large",
		 "Large bold"
	};
 
