#include "cfg_items.h"
#include "swilib.h"
//Конфигурация

__root const CFG_HDR cfghdr0={CFG_UINT,"Refresh period, 1 sec",0,600};
__root const unsigned int cfgRefresh= 60;

__root const CFG_HDR cfghdr1_1={CFG_COLOR,"Border Color",0,0};
__root const char cfgBookBorderCol[4]={255,255,255,60};

__root const CFG_HDR cfghdr1_2={CFG_COLOR,"Background Color",0,0};
__root const char cfgBookBGCol[4]={255,255,255,60};
