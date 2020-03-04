#include "..\inc\cfg_items.h"

__root const CFG_HDR cfghdr0={CFG_CBOX,"Enable hello message",0,2};
__root const int ENA_HELLO=1;
__root const CFG_CBOX_ITEM cfgcbox0[2]={"No","Yes"};

__root const CFG_HDR cfghdr10={CFG_CBOX,"Show time",0,2};
__root const int cfg_time=1;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"No","Yes"};

__root const CFG_HDR cfghdr1={CFG_CBOX,"Show seconds",0,2};
__root const int cfg_sec=1;
__root const CFG_CBOX_ITEM  cfgcbox2[2]={"No","Yes"};

__root const CFG_HDR cfghdr2={CFG_COORDINATES,"Time coordinate",0,320};
__root const unsigned int cfg_tx=30;
__root const unsigned int cfg_ty=82;

__root const CFG_HDR cfghdr33={CFG_UINT,"Time font",0,12};
__root const unsigned int cfg_tfont=1;

__root const CFG_HDR cfghdr6={CFG_UINT,"Speed update",0,250};
__root const unsigned int speed=50;
