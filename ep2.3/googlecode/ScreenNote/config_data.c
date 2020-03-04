#include "..\inc\cfg_items.h"


__root const CFG_HDR cfghdr0={CFG_STR_UTF8,"根目录",3,127};
__root const char root_dir[128]="0:\\Elf tool\\Snote";

// -------------------------------------------------------------------------- //

__root const CFG_HDR cfghdr_m1={CFG_LEVEL,"按星期显示",1,0};

__root const CFG_HDR cfghdr1_1={CFG_CBOX,"按星期显示",0,2};
__root const unsigned int cfgweek=1;
__root const CFG_CBOX_ITEM cfgcbox1_1[2]={"no","yes"};

/*__root const CFG_HDR cfghdr1_2={CFG_CBOX,"显示文件",0,3};
__root const unsigned int cfgcheck=2;
__root const CFG_CBOX_ITEM cfgcbox1[3]={"0-23.tmo","0.tmo","0-23.tmo 和 0.tmo"};*/

__root const CFG_HDR cfghdr1_3={CFG_COORDINATES,"左上角坐标",0,0};
__root const unsigned int cfgx=10;
__root const unsigned int cfgy=30;

__root const CFG_HDR cfghdr1_4={CFG_COORDINATES,"右下角坐标",0,0};
__root const unsigned int cfgx2=122;
__root const unsigned int cfgy2=45;

__root const CFG_HDR cfghdr1_5={CFG_UINT,"字体颜色",0,24};
__root const int unsigned cfgcolor=7;

__root const CFG_HDR cfghdr1_6={CFG_UINT,"字体描边颜色",0,24};
__root const int unsigned cfgoutlcolor=1;

__root const CFG_HDR cfghdr1_7={CFG_UINT,"字号",0,15};
__root const unsigned int cfgfont=8;

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

// -------------------------------------------------------------------------- //

__root const CFG_HDR cfghdr_m2={CFG_LEVEL,"按日期显示",1,0};

__root const CFG_HDR cfghdr2_1={CFG_CBOX,"按日期显示",0,2};
__root const int unsigned cfgdat=1;
__root const CFG_CBOX_ITEM cfgcbox2_1[2]={"no","yes"};

__root const CFG_HDR cfghdr2_2={CFG_COORDINATES,"左上角坐标",0,0};
__root const unsigned int cfgxd=10;
__root const unsigned int cfgyd=120;

__root const CFG_HDR cfghdr2_3={CFG_COORDINATES,"右下角坐标",0,0};
__root const unsigned int cfgxd2=122;
__root const unsigned int cfgyd2=135;

__root const CFG_HDR cfghdr2_4={CFG_UINT,"字体颜色",0,24};
__root const int unsigned cfgdcolor=2;

__root const CFG_HDR cfghdr2_5={CFG_UINT,"字体描边颜色",0,24};
__root const int unsigned cfgdoutlcolor=1;

__root const CFG_HDR cfghdr2_6={CFG_UINT,"字号",0,15};
__root const int unsigned cfgdfont=8;

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

// -------------------------------------------------------------------------- //

__root const CFG_HDR cfghdr1={CFG_UINT,"显示时间从(0-23)",0,23};
__root const unsigned int cfgtime1=6;

__root const CFG_HDR cfghdr2={CFG_UINT,"到(0-23)",0,23};
__root const unsigned int cfgtime2=2;

__root const CFG_HDR cfghdr4={CFG_UINT,"更新频率(sec)",0,300};
__root const int unsigned cfgupdate_time=300;
