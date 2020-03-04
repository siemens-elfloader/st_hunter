#include "cfg_items.h"
#include "swilib.h"

__root const CFG_HDR cfghdr0 = {CFG_CBOX, "Показывать когда", 0, 3};
__root const unsigned int cfgShowIn = 2;
__root const CFG_CBOX_ITEM cfgcbox1[3] = {"Блокировано", "Разблокировано", "Всегда"};

//__root const CFG_HDR cfghdr1 = {CFG_UINT, "X Position", 0, 132};
//__root const unsigned int cfgX = 10;

__root const CFG_HDR cfghdr2 = {CFG_UINT, "Позиция по Y", 0, 175};
__root const unsigned int cfgY = 120;

////////////
__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Ярлык 1",1,0};

__root const CFG_HDR cfghdr11_1={CFG_CBOX,"Тип",0,3};
__root const int type1=1;
__root const CFG_CBOX_ITEM cfgcbox4[3]={"файл","шорткат","адрес"};


__root const CFG_HDR cfghdr2_1={CFG_STR_UTF8,"Иконка 1.1",0,127};
__root const char spic1[128]="0:\\Zbin\\img\\4MotorIcons\\s_profiles.png";
__root const CFG_HDR cfghdr2_2={CFG_STR_UTF8,"Иконка 1.2",0,127};
__root const char bpic1[128]="0:\\Zbin\\img\\4MotorIcons\\b_profiles.png";

__root const CFG_HDR cfghdr2_3={CFG_STR_UTF8,"Файл 1/шорткат/aдрес",0,127};
__root const char file1[128]="PROF_PROFILEMNU";

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

/////////////////

__root const CFG_HDR cfghdr_m13={CFG_LEVEL,"Ярлык 2",1,0};

__root const CFG_HDR cfghdr12_1={CFG_CBOX,"Тип",0,3};
__root const int type2=1;
__root const CFG_CBOX_ITEM cfgcbox5[3]={"файл","шорткат","адрес"};


__root const CFG_HDR cfghdr3_1={CFG_STR_UTF8,"Иконка 2.1",0,127};
__root const char spic2[128]="0:\\Zbin\\img\\4MotorIcons\\s_sms.png";
__root const CFG_HDR cfghdr3_2={CFG_STR_UTF8,"Иконка 2.2",0,127};
__root const char bpic2[128]="0:\\Zbin\\img\\4MotorIcons\\b_sms.png";


__root const CFG_HDR cfghdr3_3={CFG_STR_UTF8,"Файл 2/шорткат/aдрес",0,127};
__root const char file2[128]="MESG_CREATE_NEW";

__root const CFG_HDR cfghdr_m12={CFG_LEVEL,"",0,0};

///////////////////

__root const CFG_HDR cfghdr_m16={CFG_LEVEL,"Ярлык 3",1,0};

__root const CFG_HDR cfghdr13_1={CFG_CBOX,"Тип",0,3};
__root const int type3=1;
__root const CFG_CBOX_ITEM cfgcbox6[3]={"файл","шорткат","адрес"};

__root const CFG_HDR cfghdr4_1={CFG_STR_UTF8,"Иконка 3.1",0,127};
__root const char spic3[128]="0:\\Zbin\\img\\4MotorIcons\\s_settings.png";
__root const CFG_HDR cfghdr4_2={CFG_STR_UTF8,"Иконка 3.2",0,127};
__root const char bpic3[128]="0:\\Zbin\\img\\4MotorIcons\\b_settings.png";

__root const CFG_HDR cfghdr4_3={CFG_STR_UTF8,"Файл 3/шорткат/aдрес",0,127};
__root const char file3[128]="STUP_SETPUPMENU";

__root const CFG_HDR cfghdr_m15={CFG_LEVEL,"",0,0};
////////////////
__root const CFG_HDR cfghdr_m18={CFG_LEVEL,"Ярлык 4",1,0};

__root const CFG_HDR cfghdr14_1={CFG_CBOX,"Тип",0,3};
__root const int type4=1;
__root const CFG_CBOX_ITEM cfgcbox7[3]={"файл","шорткат","адрес"};

__root const CFG_HDR cfghdr5_1={CFG_STR_UTF8,"Иконка 4",0,127};
__root const char spic4[128]="0:\\Zbin\\img\\4MotorIcons\\s_calls.png";
__root const CFG_HDR cfghdr5_2={CFG_STR_UTF8,"Иконка 4",0,127};
__root const char bpic4[128]="0:\\Zbin\\img\\4MotorIcons\\b_calls.png";

__root const CFG_HDR cfghdr5_3={CFG_STR_UTF8,"Файл 4/шорткат/aдрес",0,127};
__root const char file4[128]="CNCT_ADDRESSBOK";

__root const CFG_HDR cfghdr_m17={CFG_LEVEL,"",0,0};

/////
  /*
  0-Left_SOFT
  1-Right_soft
  2-up
  3-down
  4-left
  5-right
  6-vol up
  7-vol down
  */
