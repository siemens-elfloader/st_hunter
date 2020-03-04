#include "..\inc\cfg_items.h"
#include "swilib.h"

__root const CFG_HDR cfghdr0={CFG_STR_UTF8,"Путь",0,127};
__root const char path[128]="4:\\ZBin\\AltMyMenu2\\img";

__root const CFG_HDR cfghdr_q1={CFG_LEVEL,"Интерфейс",1,0};

    __root const CFG_HDR cfghdr0_1={CFG_UINT,"Размер иконок",0,99};
    __root const int iconSize=20;    
    
    __root const CFG_HDR cfghdr_q1_1={CFG_CBOX,"Два столбца",0,2};
    __root const int VIEW_MODE=0;
    __root const CFG_CBOX_ITEM cfgcbox_q1_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr_q1_3={CFG_CBOX,"Две линии",0,2};
    __root const int LINE_MODE=1;
    __root const CFG_CBOX_ITEM cfgcbox_q1_4[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr_2_={CFG_CHECKBOX,"Градиент",0,2};
    __root const int isGradient = 1;
          

    __root const CFG_HDR cfghdrr60_2={CFG_COLOR,"Цвет курсора",0,0};
    __root const char CursorClr[4]={0xFF,0x00,0x00,0x15};
          
    __root const CFG_HDR cfghdrr61_2={CFG_COLOR,"Цвет обводки курсора",0,0};
    __root const char CursorBGClr[4]={0x00,0x00,0x00,0x48};
          
          
    __root const CFG_HDR cfghdr_e1={CFG_LEVEL,"Пнг щрифт",1,0};//level       
          
          __root const CFG_HDR cfghdr23 = {CFG_STR_UTF8, "Путь к png шрифту", 0, 127};
          __root const char fontpath[128] = "";
        
          __root const CFG_HDR cfghdr28={CFG_CBOX,"Использовать полный шрифт",0,2};
          __root const int fullfont=1;
          __root const CFG_CBOX_ITEM cfgcbox7[2]={"No","Yes"};        
          
          __root const CFG_HDR cfghdr2_2 = {CFG_UINT, "Расстояние между буквами", 0, 10};
          __root const unsigned int space = 1;          
          
     __root const CFG_HDR cfghdr_e0={CFG_LEVEL,"",0,0};// 
     
     
     __root const CFG_HDR cfghdr_y1={CFG_LEVEL,"Системный щрифт",1,0};//level     
     
          __root const CFG_HDR cfghdr_2_2={CFG_CHECKBOX,"Использовать сист. шрифт",0,2};
          __root const int isSystemFont = 1;
          
          __root const CFG_HDR cfghdr2_8 = {CFG_UINT, "Шрифт", 0, 15};  
          __root const unsigned int pngFont = FONT_SMALL;  
    
          __root const CFG_HDR cfghdr60_2={CFG_COLOR,"Цвет шрифта",0,0};
          __root const char fntSelect[4]={0xFF,0xFF,0xFF,100};
            
          __root const CFG_HDR cfghdr60_3={CFG_COLOR,"Цвет обводки",0,0};
          __root const char bordSelect[4]={0x00,0x00,0x00,0x00};
            
          __root const CFG_HDR cfghdr60_4={CFG_CBOX,"Обводить шрифт",0,2};
          __root const int isOutline=0;
          __root const CFG_CBOX_ITEM cfgcbox7_1[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr_y0={CFG_LEVEL,"",0,0};//     

__root const CFG_HDR cfghdr_q0={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_s1={CFG_LEVEL,"Настройка вызова",1,0};

      __root const CFG_HDR cfghdrs1_0={CFG_UINT,"Hotkey (dec)",0,99};
      __root const int ACTIVE_KEY_1=11;
      
      __root const CFG_HDR cfghdr123={CFG_CBOX,"Key mode",0,3};
      __root const int ACTIVE_KEY_STYLE=1;
      __root const CFG_CBOX_ITEM cfgcbox23[3]={"Short Press","Long Press","*+#"};
      
      __root const CFG_HDR cfghdr1_233={CFG_CBOX,"Only idle",0,2};
      __root const int IDLE_MODE=0;
      __root const CFG_CBOX_ITEM cfgcbox2443[2]={"No","Yes"};
      
      __root const CFG_HDR cfghdr22={CFG_CBOX,"Enable if keyboard locked",0,2};
      __root const int ENA_LOCK=1;
      __root const CFG_CBOX_ITEM cfgcbox22[2]={"No","Yes"};

__root const CFG_HDR cfghdr_s0={CFG_LEVEL,"",0,0};



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


__root const CFG_HDR cfghdr2_32={CFG_STR_WIN1251,"Bookmark 2 name",1,31};
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



__root const CFG_HDR cfghdr_t1_1={CFG_LEVEL,"16 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_1={CFG_STR_UTF8,"Bookmark 16  file",3,127};
    __root const char BM16FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_1={CFG_STR_WIN1251,"Bookmark 16 name",1,31};
    __root const char BMNAME16[32]="";
    
    __root const CFG_HDR cfghdr_t4_1={CFG_STR_UTF8,"16 icon",0,127};
    __root const char ICON16[128]="";
    
    __root const CFG_HDR cfghdr_t5_1={CFG_STR_WIN1251,"16 description",1,31};
    __root const char description16[32]="";

__root const CFG_HDR cfghdr_t0_1={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr_t1_2={CFG_LEVEL,"17 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_2={CFG_STR_UTF8,"Bookmark 17 file",3,127};
    __root const char BM17FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_2={CFG_STR_WIN1251,"Bookmark 17 name",1,31};
    __root const char BMNAME17[32]="";
    
    __root const CFG_HDR cfghdr_t4_2={CFG_STR_UTF8,"17 icon",0,127};
    __root const char ICON17[128]="";
    
    __root const CFG_HDR cfghdr_t5_2={CFG_STR_WIN1251,"17 description",1,31};
    __root const char description17[32]="";

__root const CFG_HDR cfghdr_t0_2={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_3={CFG_LEVEL,"18 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_3={CFG_STR_UTF8,"Bookmark 18 file",3,127};
    __root const char BM18FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_3={CFG_STR_WIN1251,"Bookmark 18 name",1,31};
    __root const char BMNAME18[32]="";
    
    __root const CFG_HDR cfghdr_t4_3={CFG_STR_UTF8,"18 icon",0,127};
    __root const char ICON18[128]="";
    
    __root const CFG_HDR cfghdr_t5_3={CFG_STR_WIN1251,"18 description",1,31};
    __root const char description18[32]="";

__root const CFG_HDR cfghdr_t0_3={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_4={CFG_LEVEL,"19 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_4={CFG_STR_UTF8,"Bookmark 19 file",3,127};
    __root const char BM19FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_4={CFG_STR_WIN1251,"Bookmark 19  name",1,31};
    __root const char BMNAME19[32]="";
    
    __root const CFG_HDR cfghdr_t4_4={CFG_STR_UTF8,"19 icon",0,127};
    __root const char ICON19[128]="";
    
    __root const CFG_HDR cfghdr_t5_4={CFG_STR_WIN1251,"19 description",1,31};
    __root const char description19[32]="";

__root const CFG_HDR cfghdr_t0_4={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_5={CFG_LEVEL,"20 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_5={CFG_STR_UTF8,"Bookmark 20 file",3,127};
    __root const char BM20FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_5={CFG_STR_WIN1251,"Bookmark 20 name",1,31};
    __root const char BMNAME20[32]="";
    
    __root const CFG_HDR cfghdr_t4_5={CFG_STR_UTF8,"20 icon",0,127};
    __root const char ICON20[128]="";
    
    __root const CFG_HDR cfghdr_t5_5={CFG_STR_WIN1251,"20 description",1,31};
    __root const char description20[32]="";

__root const CFG_HDR cfghdr_t0_5={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_6={CFG_LEVEL,"21 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_6={CFG_STR_UTF8,"Bookmark 21 file",3,127};
    __root const char BM21FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_6={CFG_STR_WIN1251,"Bookmark 21 name",1,31};
    __root const char BMNAME21[32]="";
    
    __root const CFG_HDR cfghdr_t4_6={CFG_STR_UTF8,"21 icon",0,127};
    __root const char ICON21[128]="";
    
    __root const CFG_HDR cfghdr_t5_6={CFG_STR_WIN1251,"21 description",1,31};
    __root const char description21[32]="";

__root const CFG_HDR cfghdr_t0_6={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_7={CFG_LEVEL,"22 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_7={CFG_STR_UTF8,"Bookmark 22 file",3,127};
    __root const char BM22FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_7={CFG_STR_WIN1251,"Bookmark 22 name",1,31};
    __root const char BMNAME22[32]="";
    
    __root const CFG_HDR cfghdr_t4_7={CFG_STR_UTF8,"22 icon",0,127};
    __root const char ICON22[128]="";
    
    __root const CFG_HDR cfghdr_t5_7={CFG_STR_WIN1251,"22 description",1,31};
    __root const char description22[32]="";

__root const CFG_HDR cfghdr_t0_7={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_8={CFG_LEVEL,"23 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_8={CFG_STR_UTF8,"Bookmark 23 file",3,127};
    __root const char BM23FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_8={CFG_STR_WIN1251,"Bookmark 23 name",1,31};
    __root const char BMNAME23[32]="";
    
    __root const CFG_HDR cfghdr_t4_8={CFG_STR_UTF8,"23 icon",0,127};
    __root const char ICON23[128]="";
    
    __root const CFG_HDR cfghdr_t5_8={CFG_STR_WIN1251,"23 description",1,31};
    __root const char description23[32]="";

__root const CFG_HDR cfghdr_t0_8={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_9={CFG_LEVEL,"24 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_9={CFG_STR_UTF8,"Bookmark 24 file",3,127};
    __root const char BM24FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_9={CFG_STR_WIN1251,"Bookmark 24 name",1,31};
    __root const char BMNAME24[32]="";
    
    __root const CFG_HDR cfghdr_t4_9={CFG_STR_UTF8,"24 icon",0,127};
    __root const char ICON24[128]="";
    
    __root const CFG_HDR cfghdr_t5_9={CFG_STR_WIN1251,"24 description",1,31};
    __root const char description24[32]="";

__root const CFG_HDR cfghdr_t0_9={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_10={CFG_LEVEL,"25 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_10={CFG_STR_UTF8,"Bookmark 25 file",3,127};
    __root const char BM25FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_10={CFG_STR_WIN1251,"Bookmark 25 name",1,31};
    __root const char BMNAME25[32]="";
    
    __root const CFG_HDR cfghdr_t4_10={CFG_STR_UTF8,"25 icon",0,127};
    __root const char ICON25[128]="";
    
    __root const CFG_HDR cfghdr_t5_10={CFG_STR_WIN1251,"25 description",1,31};
    __root const char description25[32]="";

__root const CFG_HDR cfghdr_t0_10={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_11={CFG_LEVEL,"26 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_11={CFG_STR_UTF8,"Bookmark 26 file",3,127};
    __root const char BM26FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_11={CFG_STR_WIN1251,"Bookmark 26 name",1,31};
    __root const char BMNAME26[32]="";
    
    __root const CFG_HDR cfghdr_t4_11={CFG_STR_UTF8,"26 icon",0,127};
    __root const char ICON26[128]="";
    
    __root const CFG_HDR cfghdr_t5_11={CFG_STR_WIN1251,"26 description",1,31};
    __root const char description26[32]="";

__root const CFG_HDR cfghdr_t0_11={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_12={CFG_LEVEL,"27 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_12={CFG_STR_UTF8,"Bookmark 27 file",3,127};
    __root const char BM27FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_12={CFG_STR_WIN1251,"Bookmark 27 name",1,31};
    __root const char BMNAME27[32]="";
    
    __root const CFG_HDR cfghdr_t4_12={CFG_STR_UTF8,"27 icon",0,127};
    __root const char ICON27[128]="";
    
    __root const CFG_HDR cfghdr_t5_12={CFG_STR_WIN1251,"27 description",1,31};
    __root const char description27[32]="";

__root const CFG_HDR cfghdr_t0_12={CFG_LEVEL,"",0,0};



__root const CFG_HDR cfghdr_t1_13={CFG_LEVEL,"28 закладка",1,0};

    __root const CFG_HDR cfghdr_t2_13={CFG_STR_UTF8,"Bookmark 28 file",3,127};
    __root const char BM28FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_13={CFG_STR_WIN1251,"Bookmark 28 name",1,31};
    __root const char BMNAME28[32]="";
    
    __root const CFG_HDR cfghdr_t4_13={CFG_STR_UTF8,"28 icon",0,127};
    __root const char ICON28[128]="";
    
    __root const CFG_HDR cfghdr_t5_13={CFG_STR_WIN1251,"28 description",1,31};
    __root const char description28[32]="";

__root const CFG_HDR cfghdr_t0_13={CFG_LEVEL,"",0,0};




/*
__root const CFG_HDR cfghdr_t1_={CFG_LEVEL," закладка",1,0};

    __root const CFG_HDR cfghdr_t2_={CFG_STR_UTF8,"Bookmark  file",3,127};
    __root const char BM FILE[128]="";
    
    __root const CFG_HDR cfghdr_t3_={CFG_STR_WIN1251,"Bookmark  name",1,31};
    __root const char BMNAME[32]="";
    
    __root const CFG_HDR cfghdr_t4_={CFG_STR_UTF8," icon",0,127};
    __root const char ICON[128]="";
    
    __root const CFG_HDR cfghdr_t5_={CFG_STR_WIN1251," description",1,31};
    __root const char description[32]="";

__root const CFG_HDR cfghdr_t0_={CFG_LEVEL,"",0,0};
*/
