#include "..\inc\cfg_items.h"
#include "swilib.h"

__root const CFG_HDR cfghdr0={CFG_STR_UTF8,"Путь",0,127};
__root const char path[128]="4:\\ZBin\\AltMMenu2\\img";

__root const CFG_HDR cfghdr0_1={CFG_UINT,"Размер иконок",0,99};
__root const int iconSize=20;

__root const CFG_HDR cfghdr_s1={CFG_LEVEL,"Настройка вызова",1,0};

__root const CFG_HDR cfghdrs1_0={CFG_UINT,"Hotkey (dec)",0,99};
__root const int ACTIVE_KEY_1=51;

__root const CFG_HDR cfghdr123={CFG_CBOX,"Key mode",0,3};
__root const int ACTIVE_KEY_STYLE=0;
__root const CFG_CBOX_ITEM cfgcbox23[3]={"Short Press","Long Press","*+#"};

__root const CFG_HDR cfghdr1_233={CFG_CBOX,"Only idle",0,2};
__root const int IDLE_MODE=0;
__root const CFG_CBOX_ITEM cfgcbox2443[2]={"No","Yes"};

__root const CFG_HDR cfghdr22={CFG_CBOX,"Enable if keyboard locked",0,2};
__root const int ENA_LOCK=1;
__root const CFG_CBOX_ITEM cfgcbox22[2]={"No","Yes"};

__root const CFG_HDR cfghdr_s0={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m121={CFG_LEVEL,"Настройки цвета",1,0};

__root const CFG_HDR cfghdr134_1={CFG_COLOR,"Цвет фона выделенного",0,0};
__root const char clrSelect[4]={0xFF,0x00,0x00,0x20};

__root const CFG_HDR cfghdr134_2={CFG_COLOR,"Цвет обводки выделенного",0,0};
__root const char bordSelect[4]={0x00,0x00,0x00,0x60};

__root const CFG_HDR cfghdr134_3={CFG_COLOR,"Цвет шрифта выделенного",0,0};
__root const char fntSelect[4]={0xFF,0xFF,0x00,0x60};


__root const CFG_HDR cfghdr134_4={CFG_COLOR,"Цвет фона не выделенных",0,0};
__root const char clrNotSelect[4]={0x00,0x00,0x00,0x00};

__root const CFG_HDR cfghdr134_5={CFG_COLOR,"Цвет обводки не выделенных",0,0};
__root const char bordNotSelect[4]={0x00,0x00,0x00,0x00};

__root const CFG_HDR cfghdr134_6={CFG_COLOR,"Цвет шрифта не выделенных",0,0};
__root const char fntNotSelect[4]={0xFF,0xFF,0xFF,0x60};

__root const CFG_HDR cfghdr0111={CFG_UINT,"Шрифт",0,99};
__root const int font=FONT_SMALL;

__root const CFG_HDR cfghdr_m120={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"1 Закладка",1,0};


__root const CFG_HDR cfghdr1_1={CFG_STR_UTF8,"Bookmark 1 file",3,127};
__root const char BM1FILE[128]="4:\\zbin\\mc\\mc.elf";

__root const CFG_HDR cfghdr1_2={CFG_STR_WIN1251,"Bookmark 1 name",1,31};
__root const char BMNAME1[32]="mc";

__root const CFG_HDR cfghdr1_3={CFG_STR_UTF8,"1 icon",0,127};
__root const char ICON1[128]="";

__root const CFG_HDR cfghdr1_4={CFG_STR_WIN1251,"1 description",1,31};
__root const char description1[32]="Файл. менеджер";

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"2 закладка",1,0};

__root const CFG_HDR cfghdr2_1={CFG_STR_UTF8,"Bookmark 2 file",3,127};
__root const char BM2FILE[128]="4:\\zbin\\siejc\\siejc.elf";


__root const CFG_HDR cfghdr2_2={CFG_STR_WIN1251,"Bookmark 2 name",1,31};
__root const char BMNAME2[32]="SieJC";

__root const CFG_HDR cfghdr2_3={CFG_STR_UTF8,"2 icon",0,127};
__root const char ICON2[128]="4:\\zbin\\siejc\\img\\online.png";

__root const CFG_HDR cfghdr2_4={CFG_STR_WIN1251,"2 description",1,31};
__root const char description2[32]="Jabber клиент";

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m31={CFG_LEVEL,"3 закладка",1,0};


__root const CFG_HDR cfghdr3_2={CFG_STR_UTF8,"Bookmark 3 file",3,127};
__root const char BM3FILE[128]="4:\\zbin\\naticq\\naticq.elf";


__root const CFG_HDR cfghdr3_3={CFG_STR_WIN1251,"Bookmark 3 name",1,31};
__root const char BMNAME3[32]="NatICQ";


__root const CFG_HDR cfghdr3_4={CFG_STR_UTF8,"3 icon",0,127};
__root const char ICON3[128]="4:\\zbin\\naticq\\img\\online.png";


__root const CFG_HDR cfghdr5_4={CFG_STR_WIN1251,"3 description",1,31};
__root const char description3[32]="ICQ клиент";

__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m41={CFG_LEVEL,"4 закладка",1,0};


__root const CFG_HDR cfghdr4_2={CFG_STR_UTF8,"Bookmark 4 file",3,127};
__root const char BM4FILE[128]="";

__root const CFG_HDR cfghdr4_3={CFG_STR_WIN1251,"Bookmark 4 name",1,31};
__root const char BMNAME4[32]="";


__root const CFG_HDR cfghdr4_4={CFG_STR_UTF8,"4 icon",0,127};
__root const char ICON4[128]="";


__root const CFG_HDR cfghdr4_10={CFG_STR_WIN1251,"4 description",1,31};
__root const char description4[32]="";

__root const CFG_HDR cfghdr_m40={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m51={CFG_LEVEL,"5 закладка",1,0};


__root const CFG_HDR cfghdr5_3={CFG_STR_UTF8,"Bookmark 5 file",3,127};
__root const char BM5FILE[128]="";

__root const CFG_HDR cfghdr5_6={CFG_STR_WIN1251,"Bookmark 5 name",1,31};
__root const char BMNAME5[32]="";


__root const CFG_HDR cfghdr5_7={CFG_STR_UTF8,"5 icon",0,127};
__root const char ICON5[128]="";


__root const CFG_HDR cfghdr5_5={CFG_STR_WIN1251,"5 description",1,31};
__root const char description5[32]="";

__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m61={CFG_LEVEL,"6 закладка",1,0};

__root const CFG_HDR cfghdr6_1={CFG_STR_UTF8,"Bookmark 6 file",3,127};
__root const char BM6FILE[128]="";


__root const CFG_HDR cfghdr6_2={CFG_STR_WIN1251,"Bookmark 6 name",1,31};
__root const char BMNAME6[32]="";

__root const CFG_HDR cfghdr6_7={CFG_STR_UTF8,"6 icon",0,127};
__root const char ICON6[128]="";


__root const CFG_HDR cfghdr6_4={CFG_STR_WIN1251,"6 description",1,31};
__root const char description6[32]="";

__root const CFG_HDR cfghdr_m60={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m71={CFG_LEVEL,"7 закладка",1,0};


__root const CFG_HDR cfghdr7_1={CFG_STR_UTF8,"Bookmark 7 file",3,127};
__root const char BM7FILE[128]="";


__root const CFG_HDR cfghdr7_2={CFG_STR_WIN1251,"Bookmark 7 name",1,31};
__root const char BMNAME7[32]="";


__root const CFG_HDR cfghdr7_3={CFG_STR_UTF8,"7 icon",0,127};
__root const char ICON7[128]="";


__root const CFG_HDR cfghdr7_4={CFG_STR_WIN1251,"7 description",1,31};
__root const char description7[32]="";

__root const CFG_HDR cfghdr_m70={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m81={CFG_LEVEL,"8 закладка",1,0};


__root const CFG_HDR cfghdr8_1={CFG_STR_UTF8,"Bookmark 8 file",3,127};
__root const char BM8FILE[128]="";


__root const CFG_HDR cfghdr8_2={CFG_STR_WIN1251,"Bookmark 8 name",1,31};
__root const char BMNAME8[32]="";


__root const CFG_HDR cfghdr8_3={CFG_STR_UTF8,"8 icon",0,127};
__root const char ICON8[128]="";


__root const CFG_HDR cfghdr8_4={CFG_STR_WIN1251,"8 description",1,31};
__root const char description8[32]="";

__root const CFG_HDR cfghdr_m80={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m91={CFG_LEVEL,"9 закладка",1,0};


__root const CFG_HDR cfghdr9_1={CFG_STR_UTF8,"Bookmark 9 file",3,127};
__root const char BM9FILE[128]="";


__root const CFG_HDR cfghdr9_2={CFG_STR_WIN1251,"Bookmark 9 name",1,31};
__root const char BMNAME9[32]="";

__root const CFG_HDR cfghdr9_3={CFG_STR_UTF8,"9 icon",0,127};
__root const char ICON9[128]="";

__root const CFG_HDR cfghdr9_4={CFG_STR_WIN1251,"9 description",1,31};
__root const char description9[32]="";

__root const CFG_HDR cfghdr_m90={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m111={CFG_LEVEL,"10 закладка",1,0};


__root const CFG_HDR cfghdr11_1={CFG_STR_UTF8,"Bookmark 10 file",3,127};
__root const char BM10FILE[128]="";

__root const CFG_HDR cfghdr11_2={CFG_STR_WIN1251,"Bookmark 10 name",1,31};
__root const char BMNAME10[32]="";

__root const CFG_HDR cfghdr11_3={CFG_STR_UTF8,"10 icon",0,127};
__root const char ICON10[128]="";

__root const CFG_HDR cfghdr11_4={CFG_STR_WIN1251,"10 description",1,31};
__root const char description10[32]="";

__root const CFG_HDR cfghdr_m110={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m181={CFG_LEVEL,"11 закладка",1,0};


__root const CFG_HDR cfghdr18_1={CFG_STR_UTF8,"Bookmark 11 file",3,127};
__root const char BM11FILE[128]="";


__root const CFG_HDR cfghdr12_2={CFG_STR_WIN1251,"Bookmark 11 name",1,31};
__root const char BMNAME11[32]="";

__root const CFG_HDR cfghdr12_3={CFG_STR_UTF8,"11 icon",0,127};
__root const char ICON11[128]="";

__root const CFG_HDR cfghdr12_4={CFG_STR_WIN1251,"11 description",1,31};
__root const char description11[32]="";

__root const CFG_HDR cfghdr_m172={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_m131={CFG_LEVEL,"12 закладка",1,0};


__root const CFG_HDR cfghdr13_1={CFG_STR_UTF8,"Bookmark 12 file",3,127};
__root const char BM12FILE[128]="";

__root const CFG_HDR cfghdr13_2={CFG_STR_WIN1251,"Bookmark 12 name",1,31};
__root const char BMNAME12[32]="";

__root const CFG_HDR cfghdr13_3={CFG_STR_UTF8,"12 icon",0,127};
__root const char ICON12[128]="";

__root const CFG_HDR cfghdr13_4={CFG_STR_WIN1251,"12 description",1,31};
__root const char description12[32]="";

__root const CFG_HDR cfghdr_m122={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m141={CFG_LEVEL,"13 закладка",1,0};

__root const CFG_HDR cfghdr14_1={CFG_STR_UTF8,"Bookmark 13 file",3,127};
__root const char BM13FILE[128]="";


__root const CFG_HDR cfghdr14_2={CFG_STR_WIN1251,"Bookmark 13 name",1,31};
__root const char BMNAME13[32]="";

__root const CFG_HDR cfghdr14_3={CFG_STR_UTF8,"13 icon",0,127};
__root const char ICON13[128]="";

__root const CFG_HDR cfghdr14_4={CFG_STR_WIN1251,"13 description",1,31};
__root const char description13[32]="";

__root const CFG_HDR cfghdr_m132={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_m151={CFG_LEVEL,"14 закладка",1,0};


__root const CFG_HDR cfghdr15_1={CFG_STR_UTF8,"Bookmark 14 file",3,127};
__root const char BM14FILE[128]="";

__root const CFG_HDR cfghdr15_2={CFG_STR_WIN1251,"Bookmark 14 name",1,31};
__root const char BMNAME14[32]="";

__root const CFG_HDR cfghdr15_3={CFG_STR_UTF8,"14 icon",0,127};
__root const char ICON14[128]="";

__root const CFG_HDR cfghdr15_4={CFG_STR_WIN1251,"14 description",1,31};
__root const char description14[32]="";

__root const CFG_HDR cfghdr_m142={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m161={CFG_LEVEL,"15 закладка",1,0};

__root const CFG_HDR cfghdr16_1={CFG_STR_UTF8,"Bookmark 15 file",3,127};
__root const char BM15FILE[128]="";

__root const CFG_HDR cfghdr16_2={CFG_STR_WIN1251,"Bookmark 15 name",1,31};
__root const char BMNAME15[32]="";

__root const CFG_HDR cfghdr16_3={CFG_STR_UTF8,"15 icon",0,127};
__root const char ICON15[128]="";

__root const CFG_HDR cfghdr16_4={CFG_STR_WIN1251,"15 description",1,31};
__root const char description15[32]="";

__root const CFG_HDR cfghdr_m152={CFG_LEVEL,"",0,0};

