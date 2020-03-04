#include "..\inc\cfg_items.h"
__root const CFG_HDR cfghdr0={CFG_CBOX,"Long press",0,2};
__root const int LONG_P = 0;
__root const CFG_CBOX_ITEM cbox1[2]={"No","Yes"};

__root const CFG_HDR cfghdr1 = {CFG_INT, "Hotkey (dec)", 0, 255};
__root const int BTN = 0x15;

__root const CFG_HDR cfghdr2={CFG_CBOX,"Open first",0,2};
__root const int OPEN_FIRST = 1;
__root const CFG_CBOX_ITEM cfgcbox2[2]={"Info","Bookmarks"};
/*
__root const CFG_HDR cfghdr3 = {CFG_INT, "Количество закладок (max 10)", 0, 10};
__root const int number_book=4;
*/
__root const CFG_HDR cfghdr4= {CFG_INT, "Размер шрифта", 0, 255};
__root const int FONT=7;

__root const CFG_HDR cfghdr19={CFG_COORDINATES,"Idle position",0,0};
__root const unsigned int X1=0;
__root const unsigned int Y1=0;


__root const CFG_HDR cfghdr20={CFG_COORDINATES,"Idle position2",0,0};
__root const unsigned int X2=90;
__root const unsigned int Y2=90;

__root const CFG_HDR cfghdr6={CFG_STR_UTF8,"No icon",0,127};
__root const char NO_ICON[128]="0:\\zbin\\fastrun\\img\\no_icon.png";

__root const CFG_HDR cfghdr_m121={CFG_LEVEL,"Настройки цвета",1,0};

__root const CFG_HDR cfghdr9= {CFG_INT, "Цвет надписей закладок (dec)", 0, 255};
__root const int name_cl=4;

__root const CFG_HDR cfghdr5= {CFG_INT, "Цвет описаний закладок (dec)", 0, 255};
__root const int description_cl=7;

__root const CFG_HDR cfghdr12_0={CFG_COLOR,"Цвет шрифта информации",0,0};
__root const char black[]={0x00,0x00,0x00,0x64};


__root const CFG_HDR cfghdr12_1={CFG_COLOR,"Цвет обводки",0,0};
__root const char red[]={0xFF,0x00,0x00,0x64};


__root const CFG_HDR cfghdr_m120={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"1 Закладка",1,0};

__root const CFG_HDR cfghdr23={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE1 = 1;
__root const CFG_CBOX_ITEM cbox2[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr1_1={CFG_STR_UTF8,"Bookmark 1 file",3,127};
__root const char BM1FILE[128]="0:\\zbin\\mc\\mc.elf";

__root const CFG_HDR cfghdr1_5={CFG_STR_UTF8,"shortcuts 1",0,31};
__root const char shortcuts1[32]="";

__root const CFG_HDR cfghdr1_2={CFG_STR_WIN1251,"Bookmark 1 name",1,31};
__root const char BMNAME1[32]="mc";

__root const CFG_HDR cfghdr1_3={CFG_STR_UTF8,"1 icon",0,127};
__root const char ICON1[128]="0:\\zbin\\smenu\\data\\images\\icons\\explorer.png";

__root const CFG_HDR cfghdr1_4={CFG_STR_WIN1251,"1 description",1,31};
__root const char description1[32]="Файл. менеджер";

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"2 закладка",1,0};

__root const CFG_HDR cfghdr24={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE2 = 1;
__root const CFG_CBOX_ITEM cbox5[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr2_1={CFG_STR_UTF8,"Bookmark 2 file",3,127};
__root const char BM2FILE[128]="0:\\zbin\\siejc\\siejc.elf";

__root const CFG_HDR cfghdr2_5={CFG_STR_UTF8,"shortcuts 2",0,31};
__root const char shortcuts2[32]="";

__root const CFG_HDR cfghdr2_2={CFG_STR_WIN1251,"Bookmark 2 name",1,31};
__root const char BMNAME2[32]="SieJC";

__root const CFG_HDR cfghdr2_3={CFG_STR_UTF8,"2 icon",0,127};
__root const char ICON2[128]="0:\\zbin\\siejc\\img\\online.png";

__root const CFG_HDR cfghdr2_4={CFG_STR_WIN1251,"2 description",1,31};
__root const char description2[32]="Jabber клиент";

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m31={CFG_LEVEL,"3 закладка",1,0};

__root const CFG_HDR cfghdr25={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE3 = 1;
__root const CFG_CBOX_ITEM cbox6[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr3_2={CFG_STR_UTF8,"Bookmark 3 file",3,127};
__root const char BM3FILE[128]="0:\\zbin\\naticq\\naticq.elf";

__root const CFG_HDR cfghdr3_5={CFG_STR_UTF8,"shortcuts 3",0,31};
__root const char shortcuts3[32]="";

__root const CFG_HDR cfghdr3_3={CFG_STR_WIN1251,"Bookmark 3 name",1,31};
__root const char BMNAME3[32]="NatICQ";


__root const CFG_HDR cfghdr3_4={CFG_STR_UTF8,"3 icon",0,127};
__root const char ICON3[128]="0:\\zbin\\naticq\\img\\online.png";


__root const CFG_HDR cfghdr5_4={CFG_STR_WIN1251,"3 description",1,31};
__root const char description3[32]="ICQ клиент";

__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m41={CFG_LEVEL,"4 закладка",1,0};

__root const CFG_HDR cfghdr27={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE4 = 0;
__root const CFG_CBOX_ITEM cbox7[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr4_2={CFG_STR_UTF8,"Bookmark 4 file",3,127};
__root const char BM4FILE[128]="0:\\zbin\\mc\\mc.elf";

__root const CFG_HDR cfghdr4_5={CFG_STR_UTF8,"shortcuts 4",0,31};
__root const char shortcuts4[32]="CNCT_ADDRESSBOK";

__root const CFG_HDR cfghdr4_3={CFG_STR_WIN1251,"Bookmark 4 name",1,31};
__root const char BMNAME4[32]="Adress book";


__root const CFG_HDR cfghdr4_4={CFG_STR_UTF8,"4 icon",0,127};
__root const char ICON4[128]="";


__root const CFG_HDR cfghdr4_10={CFG_STR_WIN1251,"4 description",1,31};
__root const char description4[32]="Shortcut";

__root const CFG_HDR cfghdr_m40={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m51={CFG_LEVEL,"5 закладка",1,0};

__root const CFG_HDR cfghdr28={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE5 = 0;
__root const CFG_CBOX_ITEM cbox8[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr5_3={CFG_STR_UTF8,"Bookmark 5 file",3,127};
__root const char BM5FILE[128]="";

__root const CFG_HDR cfghdr5_8={CFG_STR_UTF8,"shortcuts 5",0,31};
__root const char shortcuts5[32]="FLSH_FLEXPLORER";

__root const CFG_HDR cfghdr5_6={CFG_STR_WIN1251,"Bookmark 5 name",1,31};
__root const char BMNAME5[32]="Card Explorer";


__root const CFG_HDR cfghdr5_7={CFG_STR_UTF8,"5 icon",0,127};
__root const char ICON5[128]="";


__root const CFG_HDR cfghdr5_5={CFG_STR_WIN1251,"5 description",1,31};
__root const char description5[32]="Shortcut";

__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m61={CFG_LEVEL,"6 закладка",1,0};

__root const CFG_HDR cfghdr29={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE6 = 1;
__root const CFG_CBOX_ITEM cbox9[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr6_1={CFG_STR_UTF8,"Bookmark 6 file",3,127};
__root const char BM6FILE[128]="";

__root const CFG_HDR cfghdr6_5={CFG_STR_UTF8,"shortcuts 6",0,31};
__root const char shortcuts6[32]="";

__root const CFG_HDR cfghdr6_2={CFG_STR_WIN1251,"Bookmark 6 name",1,31};
__root const char BMNAME6[32]="";

__root const CFG_HDR cfghdr6_7={CFG_STR_UTF8,"6 icon",0,127};
__root const char ICON6[128]="";


__root const CFG_HDR cfghdr6_4={CFG_STR_WIN1251,"6 description",1,31};
__root const char description6[32]="";

__root const CFG_HDR cfghdr_m60={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m71={CFG_LEVEL,"7 закладка",1,0};

__root const CFG_HDR cfghdr30={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE7 = 1;
__root const CFG_CBOX_ITEM cbox10[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr7_1={CFG_STR_UTF8,"Bookmark 7 file",3,127};
__root const char BM7FILE[128]="";

__root const CFG_HDR cfghdr7_5={CFG_STR_UTF8,"shortcuts 7",0,31};
__root const char shortcuts7[32]="";

__root const CFG_HDR cfghdr7_2={CFG_STR_WIN1251,"Bookmark 7 name",1,31};
__root const char BMNAME7[32]="";


__root const CFG_HDR cfghdr7_3={CFG_STR_UTF8,"7 icon",0,127};
__root const char ICON7[128]="";


__root const CFG_HDR cfghdr7_4={CFG_STR_WIN1251,"7 description",1,31};
__root const char description7[32]="";

__root const CFG_HDR cfghdr_m70={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m81={CFG_LEVEL,"8 закладка",1,0};

__root const CFG_HDR cfghdr31={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE8 = 1;
__root const CFG_CBOX_ITEM cbox11[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr8_1={CFG_STR_UTF8,"Bookmark 8 file",3,127};
__root const char BM8FILE[128]="";

__root const CFG_HDR cfghdr8_5={CFG_STR_UTF8,"shortcuts 8",0,31};
__root const char shortcuts8[32]="";

__root const CFG_HDR cfghdr8_2={CFG_STR_WIN1251,"Bookmark 8 name",1,31};
__root const char BMNAME8[32]="";


__root const CFG_HDR cfghdr8_3={CFG_STR_UTF8,"8 icon",0,127};
__root const char ICON8[128]="";


__root const CFG_HDR cfghdr8_4={CFG_STR_WIN1251,"8 description",1,31};
__root const char description8[32]="";

__root const CFG_HDR cfghdr_m80={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m91={CFG_LEVEL,"9 закладка",1,0};

__root const CFG_HDR cfghdr32={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE9 = 1;
__root const CFG_CBOX_ITEM cbox12[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr9_1={CFG_STR_UTF8,"Bookmark 9 file",3,127};
__root const char BM9FILE[128]="";

__root const CFG_HDR cfghdr9_5={CFG_STR_UTF8,"shortcuts 9",0,31};
__root const char shortcuts9[32]="";

__root const CFG_HDR cfghdr9_2={CFG_STR_WIN1251,"Bookmark 9 name",1,31};
__root const char BMNAME9[32]="";

__root const CFG_HDR cfghdr9_3={CFG_STR_UTF8,"9 icon",0,127};
__root const char ICON9[128]="";

__root const CFG_HDR cfghdr9_4={CFG_STR_WIN1251,"9 description",1,31};
__root const char description9[32]="";

__root const CFG_HDR cfghdr_m90={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m111={CFG_LEVEL,"10 закладка",1,0};

__root const CFG_HDR cfghdr33={CFG_CBOX,"Тип закладки",0,2};
__root const int FILE10 = 1;
__root const CFG_CBOX_ITEM cbox13[2]={"Shortcut","File"};

__root const CFG_HDR cfghdr11_1={CFG_STR_UTF8,"Bookmark 10 file",3,127};
__root const char BM10FILE[128]="";

__root const CFG_HDR cfghdr11_5={CFG_STR_UTF8,"shortcuts 10",0,31};
__root const char shortcuts10[32]="";

__root const CFG_HDR cfghdr11_2={CFG_STR_WIN1251,"Bookmark 10 name",1,31};
__root const char BMNAME10[32]="";

__root const CFG_HDR cfghdr11_3={CFG_STR_UTF8,"10 icon",0,127};
__root const char ICON10[128]="";

__root const CFG_HDR cfghdr11_4={CFG_STR_WIN1251,"10 description",1,31};
__root const char description10[32]="";

__root const CFG_HDR cfghdr_m110={CFG_LEVEL,"",0,0};


/*
__root const CFG_HDR cfghdr_m101={CFG_LEVEL,"Подписи для инфы",1,0};

__root const CFG_HDR cfghdr10_0={CFG_STR_WIN1251,"Пусто",1,31};
__root const char EMPTY[32]="Пусто:)";

__root const CFG_HDR cfghdr10_1={CFG_STR_WIN1251,"1 name",1,31};
__root const char NAME1[32]="Дата:";

__root const CFG_HDR cfghdr10_2={CFG_STR_WIN1251,"2 name",1,31};
__root const char NAME2[32]="Время:";

__root const CFG_HDR cfghdr10_3={CFG_STR_WIN1251,"3 name",1,31};
__root const char NAME3[32]="Заряд АКБ:";

__root const CFG_HDR cfghdr10_4={CFG_STR_WIN1251,"4 name",1,31};
__root const char NAME4[32]="Free disc 0:";

__root const CFG_HDR cfghdr10_5={CFG_STR_WIN1251,"5 name",1,31};
__root const char NAME5[32]="CPU Load:";

__root const CFG_HDR cfghdr10_6={CFG_STR_WIN1251,"6 name",1,31};
__root const char NAME6[32]="Уров.сети:";

__root const CFG_HDR cfghdr10_7={CFG_STR_WIN1251,"7 name",1,31};
__root const char NAME7[32]="FreeRam:";

__root const CFG_HDR cfghdr_m100={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr1_5={CFG_STR_UTF8,"Bookmark 5 file",3,127};
__root const char BM5FILE[128]="";

__root const CFG_HDR cfghdr1_6={CFG_STR_UTF8,"Bookmark 6 file",3,127};
__root const char BM6FILE[128]="";

__root const CFG_HDR cfghdr1_7={CFG_STR_UTF8,"Bookmark 7 file",3,127};
__root const char BM7FILE[128]="";

__root const CFG_HDR cfghdr1_8={CFG_STR_UTF8,"Bookmark 8 file",3,127};
__root const char BM8FILE[128]="";

__root const CFG_HDR cfghdr1_9={CFG_STR_UTF8,"Bookmark 9 file",3,127};
__root const char BM9FILE[128]="";

__root const CFG_HDR cfghdr1_10={CFG_STR_UTF8,"Bookmark 10 file",3,127};
__root const char BM10FILE[128]="";

__root const CFG_HDR cfghdr1_11={CFG_STR_UTF8,"Bookmark 11 file",3,127};
__root const char BM11FILE[128]="";

__root const CFG_HDR cfghdr1_12={CFG_STR_UTF8,"Bookmark 12 file",3,127};
__root const char BM12FILE[128]="";

__root const CFG_HDR cfghdr1_13={CFG_STR_UTF8,"Bookmark 13 file",3,127};
__root const char BM13FILE[128]="";

__root const CFG_HDR cfghdr1_14={CFG_STR_UTF8,"Bookmark 14 file",3,127};
__root const char BM14FILE[128]="";

__root const CFG_HDR cfghdr1_15={CFG_STR_UTF8,"Bookmark 15 file",3,127};
__root const char BM15FILE[128]="";

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"Имена закладок",1,0};




__root const CFG_HDR cfghdr2_4={CFG_STR_WIN1251,"Bookmark 4 name",1,31};
__root const char BMNAME4[32]="";

__root const CFG_HDR cfghdr2_5={CFG_STR_WIN1251,"Bookmark 5 name",1,31};
__root const char BMNAME5[32]="";

__root const CFG_HDR cfghdr2_6={CFG_STR_WIN1251,"Bookmark 6 name",1,31};
__root const char BMNAME6[32]="";

__root const CFG_HDR cfghdr2_7={CFG_STR_WIN1251,"Bookmark 7 name",1,31};
__root const char BMNAME7[32]="";

__root const CFG_HDR cfghdr2_8={CFG_STR_WIN1251,"Bookmark 8 name",1,31};
__root const char BMNAME8[32]="";

__root const CFG_HDR cfghdr2_9={CFG_STR_WIN1251,"Bookmark 9 name",1,31};
__root const char BMNAME9[32]="";

__root const CFG_HDR cfghdr2_10={CFG_STR_WIN1251,"Bookmark 10 name",1,31};
__root const char BMNAME10[32]="";

__root const CFG_HDR cfghdr2_11={CFG_STR_WIN1251,"Bookmark 11 name",1,31};
__root const char BMNAME11[32]="";

__root const CFG_HDR cfghdr2_12={CFG_STR_WIN1251,"Bookmark 12 name",1,31};
__root const char BMNAME12[32]="";

__root const CFG_HDR cfghdr2_13={CFG_STR_WIN1251,"Bookmark 13 name",1,31};
__root const char BMNAME13[32]="";

__root const CFG_HDR cfghdr2_14={CFG_STR_WIN1251,"Bookmark 14 name",1,31};
__root const char BMNAME14[32]="";

__root const CFG_HDR cfghdr2_15={CFG_STR_WIN1251,"Bookmark 15 name",1,31};
__root const char BMNAME15[32]="";

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m31={CFG_LEVEL,"Путь к иконкам",1,0};








__root const CFG_HDR cfghdr3_5={CFG_STR_UTF8,"4 icon",0,127};
__root const char ICON4[128]="";

__root const CFG_HDR cfghdr3_6={CFG_STR_UTF8,"5 icon",0,127};
__root const char ICON5[128]="";

__root const CFG_HDR cfghdr3_7={CFG_STR_UTF8,"6 icon",0,127};
__root const char ICON6[128]="";

__root const CFG_HDR cfghdr3_8={CFG_STR_UTF8,"7 icon",0,127};
__root const char ICON7[128]="";

__root const CFG_HDR cfghdr3_9={CFG_STR_UTF8,"8 icon",0,127};
__root const char ICON8[128]="";

__root const CFG_HDR cfghdr3_10={CFG_STR_UTF8,"9 icon",0,127};
__root const char ICON9[128]="";

__root const CFG_HDR cfghdr3_11={CFG_STR_UTF8,"10 icon",0,127};
__root const char ICON10[128]="";

__root const CFG_HDR cfghdr3_12={CFG_STR_UTF8,"11 icon",0,127};
__root const char ICON11[128]="";

__root const CFG_HDR cfghdr3_13={CFG_STR_UTF8,"12 icon",0,127};
__root const char ICON12[128]="";

__root const CFG_HDR cfghdr3_14={CFG_STR_UTF8,"13 icon",0,127};
__root const char ICON13[128]="";

__root const CFG_HDR cfghdr3_15={CFG_STR_UTF8,"14 icon",0,127};
__root const char ICON14[128]="";

__root const CFG_HDR cfghdr3_16={CFG_STR_UTF8,"15 icon",0,127};
__root const char ICON15[128]="";

__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};





__root const CFG_HDR cfghdr_m51={CFG_LEVEL,"Описание закладок",1,0};






__root const CFG_HDR cfghdr5_4={CFG_STR_WIN1251,"4 description",1,31};
__root const char description4[32]="";

__root const CFG_HDR cfghdr5_5={CFG_STR_WIN1251,"5 description",1,31};
__root const char description5[32]="";

__root const CFG_HDR cfghdr5_6={CFG_STR_WIN1251,"6 description",1,31};
__root const char description6[32]="";

__root const CFG_HDR cfghdr5_7={CFG_STR_WIN1251,"7 description",1,31};
__root const char description7[32]="";

__root const CFG_HDR cfghdr5_8={CFG_STR_WIN1251,"8 description",1,31};
__root const char description8[32]="";

__root const CFG_HDR cfghdr5_9={CFG_STR_WIN1251,"9 description",1,31};
__root const char description9[32]="";

__root const CFG_HDR cfghdr5_10={CFG_STR_WIN1251,"10 description",1,31};
__root const char description10[32]="";

__root const CFG_HDR cfghdr5_11={CFG_STR_WIN1251,"11 description",1,31};
__root const char description11[32]="";

__root const CFG_HDR cfghdr5_12={CFG_STR_WIN1251,"12 description",1,31};
__root const char description12[32]="";

__root const CFG_HDR cfghdr5_13={CFG_STR_WIN1251,"13 description",1,31};
__root const char description13[32]="";

__root const CFG_HDR cfghdr5_14={CFG_STR_WIN1251,"14 description",1,31};
__root const char description14[32]="";

__root const CFG_HDR cfghdr5_15={CFG_STR_WIN1251,"15 description",1,31};
__root const char description15[32]="";
*/


