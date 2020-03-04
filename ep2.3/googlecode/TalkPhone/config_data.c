#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"

#ifdef ELKA
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_CBOX,"打开TalkPhone",0,2};
__root const int on_off=1;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"否","是"};

__root const CFG_HDR cfghdr1={CFG_CBOX,"没有锁键盘时是否报时",0,2};
__root const int keypad_lock=0;
__root const CFG_CBOX_ITEM cfgcbox2[2]={"否","是"};

__root const CFG_HDR cfghdr2={CFG_UINT,"指定锁键盘时报时热键",0,65535};
__root const unsigned int CALL_BUTTON=0;

//---------------------------------------------------------------------------------------

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"时间设定",1,0};

__root const CFG_HDR cfghdr1_1={CFG_TIME,"开始时间",0,0};
__root const TTime TimeFrom={8,0};

__root const CFG_HDR cfghdr1_2={CFG_TIME,"结束时间",0,0};
__root const TTime TimeTo={23,0};

__root const CFG_HDR cfghdr1_3={CFG_UINT,"报时分钟 1 =",0,59};
__root const unsigned int begin_minute1=0;

__root const CFG_HDR cfghdr1_4={CFG_UINT,"报时分钟 2 =",0,59};
__root const unsigned int begin_minute2=30;

__root const CFG_HDR cfghdr1_5={CFG_UINT,"报时分钟 3 =",0,59};
__root const unsigned int begin_minute3=0;

__root const CFG_HDR cfghdr1_6={CFG_TIME,"报时排除时间开始",0,0};
__root const TTime TimeExFrom={0,0};

__root const CFG_HDR cfghdr1_7={CFG_TIME,"报时排除时间结束",0,0};
__root const TTime TimeExTo={0,0};

__root const CFG_HDR cfghdr1_8={CFG_UINT,"此情景下不自动报时",1,8};
__root const unsigned int ExProfile=2;

__root const CFG_HDR cfghdr1_9={CFG_CBOX,"听歌时不自动报时",0,2};
__root const int music_cut=1;
__root const CFG_CBOX_ITEM cfgcbox3[2]={"否","是"};


__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

//---------------------------------------------------------------------------------------

__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"音效设定",1,0};

__root const CFG_HDR cfghdr2_1={CFG_UINT,"音量",0,6};
__root const unsigned int volume=6;

__root const CFG_HDR cfghdr2_2={CFG_STR_UTF8,"报时*.WAV文件路径",0,127};
__root const char folder_path[128]=DEFAULT_DISK ":\\ZBin\\TalkPhone\\";

__root const CFG_HDR cfghdr2_3={CFG_CBOX,"每个小时播放",0,2};
__root const int PLAY_PARAM=1;
__root const CFG_CBOX_ITEM cfgcbox7[2]={"否","全部"};

__root const CFG_HDR cfghdr2_4={CFG_CBOX,"热键报时",0,2};
__root const int PLAY_PARAM_BTN_CALL=1;
__root const CFG_CBOX_ITEM cfgcbox4[2]={"否","是"};

__root const CFG_HDR cfghdr2_5={CFG_STR_UTF8,"报时前缀文件",0,127};
__root const char kurant_path[128]=DEFAULT_DISK ":\\ZBin\\TalkPhone\\x.wav";

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

//---------------------------------------------------------------------------------------

__root const CFG_HDR cfghdr_m31={CFG_LEVEL,"图标设定",1,0};

__root const CFG_HDR cfghdr3_1={CFG_CBOX,"显示图标",0,2};
__root const int show_icon=0;
__root const CFG_CBOX_ITEM cfgcbox5[2]={"否","是"};

__root const CFG_HDR cfghdr3_2={CFG_COORDINATES,"图标位置",0,0};
__root const unsigned int cfgX=0;
__root const unsigned int cfgY=30;

__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};

//---------------------------------------------------------------------------------------

__root const CFG_HDR cfghdr_m41={CFG_LEVEL,"振动设置",1,0};

__root const CFG_HDR cfghdr4_1={CFG_CBOX,"开启振动",0,2};
__root const int Is_Vibra_Enabled=0;
__root const CFG_CBOX_ITEM cfgcbox6[2]={"否","是"};

__root const CFG_HDR cfghdr4_2 = {CFG_UINT, "振动次数", 0, 10};
__root const unsigned int vibra_count = 1;

__root const CFG_HDR cfghdr4_3 = {CFG_UINT, "振动强度", 0, 100};
__root const unsigned int vibra_power = 50;

__root const CFG_HDR cfghdr_m40={CFG_LEVEL,"",0,0};
