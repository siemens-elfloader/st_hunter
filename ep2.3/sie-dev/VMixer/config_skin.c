#include "../inc/cfg_items.h"
#include "../inc/swilib.h"

__root const CFG_HDR skinhdr1={CFG_LEVEL, "Positions", 1, 0};

	__root const CFG_HDR skinhdr1_1 = {CFG_COORDINATES, "Background" , 0, 0};
	__root const int CFG_X_BG = 10;
	__root const int CFG_Y_BG = 70;
	
	__root const CFG_HDR skinhdr1_2 = {CFG_COORDINATES, "System volume slider" , 0, 0};
	__root const int CFG_X_SLIDER_SYSTEM = 62;
	__root const int CFG_Y_SLIDER_SYSTEM = 150;
	
	__root const CFG_HDR skinhdr1_3 = {CFG_COORDINATES, "Alarm volume slider" , 0, 0};
	__root const int CFG_X_SLIDER_ALARM = 119;
	__root const int CFG_Y_SLIDER_ALARM = 150;
	
	__root const CFG_HDR skinhdr1_4 = {CFG_COORDINATES, "Call volume slider" , 0, 0};
	__root const int CFG_X_SLIDER_CALL = 176;
	__root const int CFG_Y_SLIDER_CALL = 150;
	
	__root const CFG_HDR skinhdr1_5 = {CFG_COORDINATES, "System volume" , 0, 0};
	__root const int CFG_X_VOL_SYSTEM = 61;
	__root const int CFG_Y_VOL_SYSTEM= 180;
	
	__root const CFG_HDR skinhdr1_6 = {CFG_COORDINATES, "Alarm volume" , 0, 0};
	__root const int CFG_X_VOL_ALARM = 119;
	__root const int CFG_Y_VOL_ALARM = 180;
	
	__root const CFG_HDR skinhdr1_7 = {CFG_COORDINATES, "Call volume" , 0, 0};
	__root const int CFG_X_VOL_CALL = 176;
	__root const int CFG_Y_VOL_CALL = 180;    
	
	__root const CFG_HDR skinhdr1_8= {CFG_UINT, "Slider's step" , 0, 320};
	__root const int CFG_STEP_SLIDERS = 12;

	__root const CFG_HDR skinhdr1_9 = {CFG_COORDINATES, "String 1" , 0, 0};
	__root const int CFG_X_TEXT1 = 44;
	__root const int CFG_Y_TEXT1= 83;
	
	__root const CFG_HDR skinhdr1_10 = {CFG_COORDINATES, "String 2" , 0, 0};
	__root const int CFG_X_TEXT2 = 105;
	__root const int CFG_Y_TEXT2 = 83;
	
	__root const CFG_HDR skinhdr1_11 = {CFG_COORDINATES, "String 3" , 0, 0};
	__root const int CFG_X_TEXT3 = 167;
	__root const int CFG_Y_TEXT3 = 183;
	
__root const CFG_HDR skinhdr1_12={CFG_LEVEL, "", 0, 0};

//==============================================================================

__root const CFG_HDR skinhdr2={CFG_LEVEL, "Text", 1, 0};

	__root const CFG_HDR skinhdr2_1={CFG_STR_WIN1251,"String 1",0,127};
	__root const char CFG_TEXT1[128]="System";

	__root const CFG_HDR skinhdr2_2={CFG_STR_WIN1251,"String 2",0,127};
	__root const char CFG_TEXT2[128]="Alarm";

	__root const CFG_HDR skinhdr2_3={CFG_STR_WIN1251,"String 3",0,127};
	__root const char CFG_TEXT3[128]="Call";

__root const CFG_HDR skinhdr2_4={CFG_LEVEL, "", 0, 0};

//===============================================================================

__root const CFG_HDR skinhdr3={CFG_LEVEL, "Color", 1, 0};

	__root const CFG_HDR skinhdr3_1 = {CFG_COLOR, "Text", 0, 0};
	__root const char CFG_COLOR_TEXT[4] = {0x00,0xff,0xff,0x64};

	__root const CFG_HDR skinhdr3_2 = {CFG_COLOR, "Volume", 0, 0};
	__root const char CFG_COLOR_VOLUME[4] = {0x00,0xff,0xff,0x64};
	
	__root const CFG_HDR skinhdr3_3 = {CFG_COLOR, "Background", 0, 0};
	__root const char cfg_col_bg[4] = {0x00,0x00,0x00,0x30};
	
__root const CFG_HDR skinhdr3_4={CFG_LEVEL, "", 0, 0};

//=================================================================================

__root const CFG_HDR skinhdr4={CFG_LEVEL, "Fonts", 1, 0};

	__root const CFG_HDR skinhdr4_1={CFG_CBOX, "Text", 0, 8};
	__root const int CFG_FONT_TEXT=0;
	__root const CFG_CBOX_ITEM cfgbox4_2[8]=
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

	__root const CFG_HDR skinhdr4_3={CFG_CBOX, "Volume", 0, 8};
	__root const int CFG_FONT_VOLUME=0;
	__root const CFG_CBOX_ITEM cfgbox4_4[8]=
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
	
__root const CFG_HDR skinhdr4_5={CFG_LEVEL, "", 0, 0};


