#include "G:\inc\cfg_items.h"

__root const CFG_HDR cfghdr0={CFG_CBOX,"Show time",0,2};
__root const int cfg_time=1;
__root const CFG_CBOX_ITEM  cfgcbox0[2]={"No","Yes"};

__root const CFG_HDR cfghdr1={CFG_CBOX,"Show seconds",0,2};
__root const int cfg_sec=1;
__root const CFG_CBOX_ITEM  cfgcbox1[2]={"No","Yes"};

__root const CFG_HDR cfghdr2={CFG_UINT,"Time x",0,132};
__root const unsigned int cfg_tx=30;

__root const CFG_HDR cfghdr3={CFG_UINT,"Time y",0,176};
__root const unsigned int cfg_ty=82;

__root const CFG_HDR cfghdr4={CFG_UINT,"Time font",0,12};
__root const unsigned int cfg_tfont=1;

__root const CFG_HDR cfghdr5={CFG_UINT,"run after any key(Seconds*2)",1,10};
__root const int cfg_runtime=1;

__root const CFG_HDR cfghdr6={CFG_UINT,"Check screensaver each",1,100};
__root const int cfg_chksec=1;
