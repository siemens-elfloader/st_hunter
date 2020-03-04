#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"


//==============================================================
 //menu
  __root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Menu",1,0};

  __root const CFG_HDR cfghdr1_4 = {CFG_UINT, "Rows", 0, 30};
  __root const unsigned int _Rows = 4;

  __root const CFG_HDR cfghdr1_5 = {CFG_UINT, "Colunms", 0, 20};
  __root const unsigned int _Columns = 3;

  __root const CFG_HDR cfghdr1_3 = {CFG_UINT, "Start item", 0, 600};
  __root const unsigned int _Start = 4;

  __root const CFG_HDR cfghdr1_10 = {CFG_CHECKBOX, "Show cursor", 0, 2};
  __root const int _cursorShow = 1;
  
  __root const CFG_HDR cfghdr2_3={CFG_STR_UTF8,"Path cursor",0,63};
  __root const char _CURSOR_PATH[64]="4:\\ZBin\\Menu\\cursor.png";
  
  __root const CFG_HDR cfghdr1_6={CFG_RECT, "Position menu", 0, 0};
  __root const RECT _menuRect = {0, 48, 240, 320-32-16};
  
  __root const CFG_HDR cfghdr1_7={CFG_CBOX, "Style menu", 0, 3};
  __root const int _styleMenu = 0;
  __root const CFG_CBOX_ITEM cfgcbox0[3] = {"Matrix", "List","Circle"};

  ////////////////////////////////////
  __root const CFG_HDR cfghdr_f11={CFG_LEVEL,"Settings list",1,0};

    __root const CFG_HDR cfghdrfl_h0={CFG_UINT,"Name font",0,65535};
    __root const unsigned int _listNameFont=FONT_MEDIUM;

    __root const CFG_HDR cfghdrfl_h1={CFG_UINT,"Name style",0,65535};
    __root const unsigned int _listNameStyle=TEXT_ALIGNLEFT;
    
    __root const CFG_HDR cfghdrfl_h2={CFG_COLOR,"Name color",0,0};
    __root const char _listNameColor[4]={0xFF,0xFF,0xFF,0x64};
    
    __root const CFG_HDR cfghdrfl_8={CFG_CBOX, "Show description", 0, 3};
    __root const int _descListShow = 0;
    __root const CFG_CBOX_ITEM cfgcboxl0[3] = {"All", "Selected", "No"};
    
    __root const CFG_HDR cfghdrfl_h3={CFG_UINT,"Description font",0,65535};
    __root const unsigned int _listDescFont=FONT_SMALL;

    __root const CFG_HDR cfghdrfl_h4={CFG_UINT,"Description style",0,65535};
    __root const unsigned int _listDescStyle=TEXT_ALIGNLEFT;
    
    __root const CFG_HDR cfghdrfl_h5={CFG_COLOR,"Description color",0,0};
    __root const char _listDescColor[4]={0xFF,0xFF,0xFF,0x64};
  
  __root const CFG_HDR cfghdr_f10={CFG_LEVEL,"",0,0};
  
  //////////////////
  __root const CFG_HDR cfghdr_crc11={CFG_LEVEL,"Settings circle",1,0};

  __root const CFG_HDR cfghdrcrc_5 = {CFG_UINT, "Radius", 0, 240};
  __root const unsigned int _Radius = 100;
  
  __root const CFG_HDR cfghcrc_9={CFG_CBOX, "Position cursor", 0, 4};
  __root const int _position_cursor = 0;
  __root const CFG_CBOX_ITEM cfgcboxcrc_0[4] = {"Up", "Down", "Left", "Right"};
  
  __root const CFG_HDR cfghdrcrc_6 = {CFG_CHECKBOX, "Enable animation", 0, 2};
  __root const int _AnimationCirc = 1;
    
  __root const CFG_HDR cfghdrcrc_7 = {CFG_UINT, "Speed animation (1/100s)", 0, 6000};
  __root const unsigned int _SpeedAnimCirc = 2;

  __root const CFG_HDR cfghdrcrc_8 = {CFG_UINT, "Step animation", 0, 90};
  __root const unsigned int _GradAnimCirc = 5;
 
  __root const CFG_HDR cfghdr_crc10={CFG_LEVEL,"",0,0};

  //////////////////////////
  __root const CFG_HDR cfghdr_pi11={CFG_LEVEL,"Position icon",1,0};

    __root const CFG_HDR cfghdr1_8={CFG_CBOX, "Type", 0, 2};
    __root const int _position_type = 0;
    __root const CFG_CBOX_ITEM cfgcboxpi0[2] = {"Center", "Offset"};
    
    __root const CFG_HDR cfghdr1_9 = {CFG_UINT, "OffsetX", 0, 100};
    __root const unsigned int _OffsetX = 3;
  
    __root const CFG_HDR cfghdr1pi_10 = {CFG_UINT, "OffsetY", 0, 100};
    __root const unsigned int _OffsetY = 3;
  
  __root const CFG_HDR cfghdr_pi10={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

  
  //header
  __root const CFG_HDR cfghdr_h11={CFG_LEVEL,"Header",1,0};

    __root const CFG_HDR cfghdr0_h1 = {CFG_CHECKBOX, "Show", 0, 2};
    __root const int _headShow = 1;

    __root const CFG_HDR cfghdr0_h11 = {CFG_CHECKBOX, "Show icon", 0, 2};
    __root const int _headIconShow = 1;

    __root const CFG_HDR cfghdr0_h2={CFG_RECT, "Position Header", 0, 0};
    __root const RECT _headRect = {0, 24, 240, 24+24};
    
    __root const CFG_HDR cfghdr0_h3={CFG_UINT,"Font",0,65535};
    __root const unsigned int _headFont=FONT_MEDIUM;

    __root const CFG_HDR cfghdr0_h4={CFG_UINT,"Style",0,65535};
    __root const unsigned int _headStyle=TEXT_ALIGNLEFT;
    
    __root const CFG_HDR cfghdr0_h5={CFG_COLOR,"Color",0,0};
    __root const char _headColor[4]={0xFF,0xFF,0xFF,0x64};

  __root const CFG_HDR cfghdr_h10={CFG_LEVEL,"",0,0};

  //description
  __root const CFG_HDR cfghdr_k11={CFG_LEVEL,"Description",1,0};

    __root const CFG_HDR cfghdr0_1 = {CFG_CHECKBOX, "Show", 0, 2};
    __root const int _descShow = 1;

    __root const CFG_HDR cfghdr0_2={CFG_RECT, "Position description", 0, 0};
    __root const RECT _descRect = {0, 320-32-16, 240, 320-32};
    
    __root const CFG_HDR cfghdr0_3={CFG_UINT,"Font",0,65535};
    __root const unsigned int _descFont=FONT_SMALL;

    __root const CFG_HDR cfghdr0_4={CFG_UINT,"Style",0,65535};
    __root const unsigned int _descStyle=TEXT_ALIGNMIDDLE;
    
    __root const CFG_HDR cfghdr0_5={CFG_COLOR,"Color",0,0};
    __root const char _deskColor[4]={0xFF,0xFF,0xFF,0x64};

  __root const CFG_HDR cfghdr_k10={CFG_LEVEL,"",0,0};
  
    //description
  __root const CFG_HDR cfghdr_s11={CFG_LEVEL,"Scrolling",1,0};

    __root const CFG_HDR cfghdrs1_8={CFG_CBOX, "Show", 0, 3};
    __root const int _scrollShow = 0;
    __root const CFG_CBOX_ITEM cfgcboxs0[3] = {"Auto", "Alltime", "No"};
    
    __root const CFG_HDR cfghdrs0_5={CFG_COLOR,"Color",0,0};
    __root const char _scrollColor[4]={0xFF,0xFF,0xFF,0x64};

  __root const CFG_HDR cfghdr_s10={CFG_LEVEL,"",0,0};
  
