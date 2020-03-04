#include "..\inc\cfg_items.h"
__root const CFG_HDR cfghdr0={CFG_CBOX,"Long press",0,2};
__root const int LONG_P = 0;
__root const CFG_CBOX_ITEM cbox1[2]={"No","Yes"};

__root const CFG_HDR cfghdr1 = {CFG_INT, "Hotkey (dec)", 0, 255};
__root const int BTN = 0x15;

__root const CFG_HDR cfghdr2={CFG_CBOX,"Open first",0,2};
__root const int OPEN_FIRST = 0;
__root const CFG_CBOX_ITEM cfgcbox2[2]={"Bookmarks","Info"};

__root const CFG_HDR cfghdr3 = {CFG_INT, "Количество закладок (max 10)", 0, 10};
__root const int number_book=4;

__root const CFG_HDR cfghdr4={CFG_CBOX,"Hello message",0,2};
__root const int ENA_HELLO = 0;
__root const CFG_CBOX_ITEM cfgcbox1[2]={"No","Yes"};

__root const CFG_HDR cfghdr5={CFG_STR_WIN1251,"Hello message",1,63};
__root const char startmsg[64]="HELLO WORLD:D";


__root const CFG_HDR cfghdr_m11={CFG_LEVEL,"Пути к файлам",1,0};

__root const CFG_HDR cfghdr1_1={CFG_STR_UTF8,"Bookmark 1 file",3,127};
__root const char BM1FILE[128]="0:\\zbin\\mc\\mc.elf";

__root const CFG_HDR cfghdr1_2={CFG_STR_UTF8,"Bookmark 2 file",3,127};
__root const char BM2FILE[128]="0:\\zbin\\siejc\\siejc.elf";

__root const CFG_HDR cfghdr1_3={CFG_STR_UTF8,"Bookmark 3 file",3,127};
__root const char BM3FILE[128]="0:\\zbin\\naticq\\naticq.elf";

__root const CFG_HDR cfghdr1_4={CFG_STR_UTF8,"Bookmark 4 file",3,127};
__root const char BM4FILE[128]="";

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
/*
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
*/
__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m21={CFG_LEVEL,"Имена закладок",1,0};

__root const CFG_HDR cfghdr2_1={CFG_STR_WIN1251,"Bookmark 1 name",1,31};
__root const char BMNAME1[32]="mc";

__root const CFG_HDR cfghdr2_2={CFG_STR_WIN1251,"Bookmark 2 name",1,31};
__root const char BMNAME2[32]="SieJC";

__root const CFG_HDR cfghdr2_3={CFG_STR_WIN1251,"Bookmark 3 name",1,31};
__root const char BMNAME3[32]="NatICQ";

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
/*
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
*/
__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m31={CFG_LEVEL,"Путь к иконкам",1,0};

__root const CFG_HDR cfghdr3_1={CFG_STR_UTF8,"No icon",0,127};
__root const char NO_ICON[128]="0:\\zbin\\fastrun\\img\\no_icon.png";

__root const CFG_HDR cfghdr3_2={CFG_STR_UTF8,"1 icon",0,127};
__root const char ICON1[128]="0:\\zbin\\smenu\\data\\images\\icons\\explorer.png";

__root const CFG_HDR cfghdr3_3={CFG_STR_UTF8,"2 icon",0,127};
__root const char ICON2[128]="0:\\zbin\\siejc\\img\\online.png";

__root const CFG_HDR cfghdr3_4={CFG_STR_UTF8,"3 icon",0,127};
__root const char ICON3[128]="0:\\zbin\\naticq\\img\\online.png";

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
/*
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
*/
__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};




__root const CFG_HDR cfghdr_m41={CFG_LEVEL,"Подписи для инфы",1,0};

__root const CFG_HDR cfghdr4_0={CFG_STR_WIN1251,"Пусто",1,31};
__root const char EMPTY[32]="Пусто:)";

__root const CFG_HDR cfghdr4_1={CFG_STR_WIN1251,"1 name",1,31};
__root const char NAME1[32]="Дата:";

__root const CFG_HDR cfghdr4_2={CFG_STR_WIN1251,"2 name",1,31};
__root const char NAME2[32]="Время:";

__root const CFG_HDR cfghdr4_3={CFG_STR_WIN1251,"3 name",1,31};
__root const char NAME3[32]="Заряд АКБ:";

__root const CFG_HDR cfghdr4_4={CFG_STR_WIN1251,"4 name",1,31};
__root const char NAME4[32]="Free disc 0:";

__root const CFG_HDR cfghdr_5={CFG_STR_WIN1251,"5 name",1,31};
__root const char NAME5[32]="CPU Load:";

__root const CFG_HDR cfghdr4_6={CFG_STR_WIN1251,"6 name",1,31};
__root const char NAME6[32]="Уров.сети:";

__root const CFG_HDR cfghdr4_7={CFG_STR_WIN1251,"7 name",1,31};
__root const char NAME7[32]="FreeRam:";

__root const CFG_HDR cfghdr_m40={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m51={CFG_LEVEL,"Описание закладок",1,0};

__root const CFG_HDR cfghdr5_1={CFG_STR_WIN1251,"1 description",1,31};
__root const char description1[32]="Файл. менеджер";

__root const CFG_HDR cfghdr5_2={CFG_STR_WIN1251,"2 description",1,31};
__root const char description2[32]="Jabber клиент";

__root const CFG_HDR cfghdr5_3={CFG_STR_WIN1251,"3 description",1,31};
__root const char description3[32]="ICQ клиент";

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
/*
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
__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_m61={CFG_LEVEL,"Цвета надписей",1,0};

__root const CFG_HDR cfghdr6_1= {CFG_INT, "Цвет надписей (dec)", 0, 255};
__root const int name_cl=4;

__root const CFG_HDR cfghdr6_2= {CFG_INT, "Цвет описаний (dec)", 0, 255};
__root const int description_cl=7;

__root const CFG_HDR cfghdr_m60={CFG_LEVEL,"",0,0};

