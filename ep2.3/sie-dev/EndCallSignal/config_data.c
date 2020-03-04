#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"
//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr1={CFG_LEVEL,"Main settings",1,0};

__root const CFG_HDR cfghdr1_0={CFG_CHECKBOX,"Startup message",0,1};
__root const unsigned int ENA_HELLO_MSG = 1;

__root const CFG_HDR cfghdr1_alsdj={CFG_CHECKBOX,"Enable StartSignal",0,1};
__root const unsigned int ENA_SIGNAL_ST = 1;

__root const CFG_HDR cfghdr1_tpe={CFG_CBOX,"What's play at this",0,2};
__root const int PLAY_TYPE_ST = 0;
__root const CFG_CBOX_ITEM cfgcbox1_tpe[2]={"Sound","Melody"};

__root const CFG_HDR cfghdr1_asdj={CFG_CHECKBOX,"Enable EndSignal",0,1};
__root const unsigned int ENA_SIGNAL = 1;

__root const CFG_HDR cfghdr1_tpej={CFG_CBOX,"What's play at this",0,2};
__root const int PLAY_TYPE = 0;
__root const CFG_CBOX_ITEM cfgcbox1_kjl[2]={"Sound","Melody"};

__root const CFG_HDR cfghdr1_end={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr123451={CFG_LEVEL,"StartSignal settings",1,0};

__root const CFG_HDR cfghdr1234={CFG_LEVEL,"Sound settings",1,0};

__root const CFG_HDR cfghdr2_snmwr={CFG_UINT,"Sound's number",0,65535};
__root const unsigned int SND_NUM_ST=82;

__root const CFG_HDR cfghdr2_svwerkjb={CFG_CHECKBOX,"Enable vibration",0,1};
__root const unsigned int VIBRA_SND_ST = 0;

__root const CFG_HDR cfghdr2_svwerp={CFG_UINT,"Vibra's power",0,100};
__root const unsigned int VIBRA_SND_POWER_ST=70;

__root const CFG_HDR cfghdr2_svdert={CFG_UINT,"Vibra's duration (0.1 sec)",0,65535};
__root const unsigned int VIBRA_SND_DURATION_ST=10;

__root const CFG_HDR cfghdr2_endery={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdreter3={CFG_LEVEL,"Melody's settings",1,0};

__root const CFG_HDR cfghdr3_ertert1={CFG_STR_UTF8,"Melody's path",0,127};
__root const char MEL_PATH_ST[128]="0:\\Sounds\\startcall.wav";

#ifdef NEWSGOLD
__root const CFG_HDR cfghdr3_mvlert={CFG_UINT,"Melody's volume",0,14};
__root const unsigned int MEL_VOL_ST=8;
#else
__root const CFG_HDR cfghdr3_mvlert={CFG_UINT,"Melody's volume",0,4};
__root const unsigned int MEL_VOL_ST=3;
#endif

__root const CFG_HDR cfghdr3_2werd={CFG_CHECKBOX,"Enable vibration",0,1};
__root const unsigned int VIBRA_MEL_ST=0;

__root const CFG_HDR cfghdr3_svsdfsp={CFG_UINT,"Vibra's power",0,100};
__root const unsigned int VIBRA_MEL_POWER_ST=70;

__root const CFG_HDR cfghdr3_svdsdfsd={CFG_UINT,"Vibra's duration (0.1 sec)",0,65535};
__root const unsigned int VIBRA_MEL_DURATION_ST=10;

__root const CFG_HDR cfghdr3_endsf={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr1_endsdf={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr12891={CFG_LEVEL,"EndSignal settings",1,0};

__root const CFG_HDR cfghdr2={CFG_LEVEL,"Sound settings",1,0};

__root const CFG_HDR cfghdr2_snm={CFG_UINT,"Sound's number",0,65535};
__root const unsigned int SND_NUM=82;

__root const CFG_HDR cfghdr2_svb={CFG_CHECKBOX,"Enable vibration",0,1};
__root const unsigned int VIBRA_SND = 0;

__root const CFG_HDR cfghdr2_svp={CFG_UINT,"Vibra's power",0,100};
__root const unsigned int VIBRA_SND_POWER=70;

__root const CFG_HDR cfghdr2_svd={CFG_UINT,"Vibra's duration (0.1 sec)",0,65535};
__root const unsigned int VIBRA_SND_DURATION=10;

__root const CFG_HDR cfghdr2_end={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr3={CFG_LEVEL,"Melody's settings",1,0};

__root const CFG_HDR cfghdr3_1={CFG_STR_UTF8,"Melody's path",0,127};
__root const char MEL_PATH[128]="0:\\Sounds\\endcall.wav";

#ifdef NEWSGOLD
__root const CFG_HDR cfghdr3_mvl={CFG_UINT,"Melody's volume",0,14};
__root const unsigned int MEL_VOL=8;
#else
__root const CFG_HDR cfghdr3_mvl={CFG_UINT,"Melody's volume",0,4};
__root const unsigned int MEL_VOL=3;
#endif

__root const CFG_HDR cfghdr3_2={CFG_CHECKBOX,"Enable vibration",0,1};
__root const unsigned int VIBRA_MEL=0;

__root const CFG_HDR cfghdr3_svp={CFG_UINT,"Vibra's power",0,100};
__root const unsigned int VIBRA_MEL_POWER=70;

__root const CFG_HDR cfghdr3_svd={CFG_UINT,"Vibra's duration (0.1 sec)",0,65535};
__root const unsigned int VIBRA_MEL_DURATION=10;

__root const CFG_HDR cfghdr3_end={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr1_endku={CFG_LEVEL,"",0,0};
