#include "cfg_items.h"
#include "swilib.h"
//Конфигурация

__root const CFG_HDR cfghdr0={CFG_UINT,"Default selected menu item",0,254};
__root const unsigned int cfgCurrent= 0;

__root const CFG_HDR cfghdr1={CFG_UINT,"Header font",0,10};
__root const unsigned int cfgHeaderFontSize = 2;

__root const CFG_HDR cfghdr2={CFG_COLOR,"Header Text Color",0,0};
__root const char cfgHeaderColor[4]={0,0,0,100};

__root const CFG_HDR cfghdr3={CFG_COLOR,"Background Color",0,0};
__root const char cfgHeaderBrush[4]={0,0,0,0};

__root const CFG_HDR cfghdr4={CFG_UINT,"Header X pos",0,500};
__root const unsigned int cfgHeaderX = 2;

__root const CFG_HDR cfghdr5={CFG_UINT,"Header Y pos",0,500};
__root const unsigned int cfgHeaderY = 2;

__root const CFG_HDR cfghdr6={CFG_COLOR,"Selected item color",0,0};
__root const char cfgClr[4]={255,255,0,40};

__root const CFG_HDR cfghdr7={CFG_COLOR,"Four icons background color",0,0};
__root const char cfgFourColor[4]={0,0,0,60};

__root const CFG_HDR cfghdr8={CFG_INT,"Icons in line",0,10};
__root const int cfgLineCount = 4;

__root const CFG_HDR cfghdr9={CFG_INT,"Menu top coordinate",-500,500};
__root const int cfgTopY = 12;

__root const CFG_HDR cfghdr10={CFG_INT,"Menu bottom coordinate",-500,500};
__root const int cfgBottomY = 120;


