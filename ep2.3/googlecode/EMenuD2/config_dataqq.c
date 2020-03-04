#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"
#include "lang.h"
//Configuration
/***************************Setup menu activation***************************/
__root const CFG_HDR cfghdr0={CFG_LEVEL,"Setup menu activation",1,0};

__root const CFG_HDR cfghdr1={CFG_UINT,"Activation button,0,99};
      __root const int IDLE_BUTTON=26;

__root const CFG_HDR cfghdr2={CFG_CHECKBOX,"Long press",0,2};
      __root const int LongPress=0;

__root const CFG_HDR cfghdr3={CFG_LEVEL,"",0,0};
/******************************************************************************/

///////////////////////////////////§¯§Ñ§ã§ä§â§à§Û§Ü§Ñ §á§å§ß§Ü§ä§à§Ó///////////////////////////
__root const CFG_HDR cfghdr4={CFG_LEVEL,LG_MENU_ITENS,1,0};

// Item "Contacts"
__root const CFG_HDR cfghdr5={CFG_LEVEL,LG_ITEM1,1,0};
  
__root const CFG_HDR cfghdr6={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_1[15]="";
      
__root const CFG_HDR cfghdr7={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_1[128]="";
      
__root const CFG_HDR cfghdr8={CFG_INT,LG_CADRI,0,12};
      __root const int anim_1=12;
      
__root const CFG_HDR cfghdr9={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc1=0;
      
__root const CFG_HDR cfghdr10={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p1=0;
      
__root const CFG_HDR cfghdr11={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Æóðíàë"
__root const CFG_HDR cfghdr12={CFG_LEVEL,LG_ITEM2,1,0};
  
__root const CFG_HDR cfghdr13={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_2[15]="";
      
__root const CFG_HDR cfghdr14={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_2[128]="";
      
__root const CFG_HDR cfghdr15={CFG_INT,LG_CADRI,0,12};
      __root const int anim_2=8;
      
__root const CFG_HDR cfghdr16={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc2=0;
      
__root const CFG_HDR cfghdr17={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p2=0;
      
__root const CFG_HDR cfghdr18={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Áðàóçå?
__root const CFG_HDR cfghdr19={CFG_LEVEL,LG_ITEM3,1,0};
  
__root const CFG_HDR cfghdr20={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_3[15]="";
      
__root const CFG_HDR cfghdr21={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_3[128]="";
      
__root const CFG_HDR cfghdr22={CFG_INT,LG_CADRI,0,12};
      __root const int anim_3=8;
      
__root const CFG_HDR cfghdr23={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc3=0;
      
__root const CFG_HDR cfghdr24={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p3=0;
      
__root const CFG_HDR cfghdr25={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Êàìåðà"
__root const CFG_HDR cfghdr26={CFG_LEVEL,LG_ITEM4,1,0};
  
__root const CFG_HDR cfghdr27={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_4[15]="";
      
__root const CFG_HDR cfghdr28={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_4[128]="";
      
__root const CFG_HDR cfghdr29={CFG_INT,LG_CADRI,0,12};
      __root const int anim_4=11;
      
__root const CFG_HDR cfghdr30={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc4=0;
      
__root const CFG_HDR cfghdr31={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p4=0;
      
__root const CFG_HDR cfghdr32={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Ñîîáùåíèÿ"
__root const CFG_HDR cfghdr33={CFG_LEVEL,LG_ITEM5,1,0};

__root const CFG_HDR cfghdr34={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_5[15]="";
      
__root const CFG_HDR cfghdr35={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_5[128]="";
      
__root const CFG_HDR cfghdr36={CFG_INT,LG_CADRI,0,12};
      __root const int anim_5=12;
      
__root const CFG_HDR cfghdr37={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc5=0;
      
__root const CFG_HDR cfghdr38={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p5=0;
      
__root const CFG_HDR cfghdr39={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Îðãàíàéçåð"
__root const CFG_HDR cfghdr40={CFG_LEVEL,LG_ITEM6,1,0};

__root const CFG_HDR cfghdr41={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_6[15]="";
      
__root const CFG_HDR cfghdr42={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_6[128]="";
      
__root const CFG_HDR cfghdr43={CFG_INT,LG_CADRI,0,12};
      __root const int anim_6=9;
      
__root const CFG_HDR cfghdr44={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc6=0;
      
__root const CFG_HDR cfghdr45={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p6=0;
      
__root const CFG_HDR cfghdr46={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Èãðû"
__root const CFG_HDR cfghdr47={CFG_LEVEL,LG_ITEM7,1,0};

__root const CFG_HDR cfghdr48={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_7[15]="";
      
__root const CFG_HDR cfghdr49={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_7[128]="";
      
__root const CFG_HDR cfghdr50={CFG_INT,LG_CADRI,0,12};
      __root const int anim_7=12;
      
__root const CFG_HDR cfghdr51={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc7=0;
      
__root const CFG_HDR cfghdr52={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p7=0;
      
__root const CFG_HDR cfghdr53={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Ìåäèàïëååð"
__root const CFG_HDR cfghdr54={CFG_LEVEL,LG_ITEM8,1,0};

__root const CFG_HDR cfghdr55={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_8[15]="";
      
__root const CFG_HDR cfghdr56={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_8[128]="";
      
__root const CFG_HDR cfghdr57={CFG_INT,LG_CADRI,0,12};
      __root const int anim_8=10;
      
__root const CFG_HDR cfghdr58={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc8=0;
      
__root const CFG_HDR cfghdr59={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p8=0;
      
__root const CFG_HDR cfghdr60={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Áóäèëüíè?
__root const CFG_HDR cfghdr61={CFG_LEVEL,LG_ITEM9,1,0};

__root const CFG_HDR cfghdr62={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_9[15]="";
      
__root const CFG_HDR cfghdr63={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_9[128]="";
      
__root const CFG_HDR cfghdr64={CFG_INT,LG_CADRI,0,12};
      __root const int anim_9=11;
      
__root const CFG_HDR cfghdr65={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc9=0;
      
__root const CFG_HDR cfghdr66={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p9=0;
      
__root const CFG_HDR cfghdr67={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Ïðî÷åå"
__root const CFG_HDR cfghdr68={CFG_LEVEL,LG_ITEM10,1,0};

__root const CFG_HDR cfghdr69={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_10[15]="";
      
__root const CFG_HDR cfghdr70={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_10[128]="";
      
__root const CFG_HDR cfghdr71={CFG_INT,LG_CADRI,0,12};
      __root const int anim_10=12;
      
__root const CFG_HDR cfghdr72={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc10=0;
      
__root const CFG_HDR cfghdr73={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p10=0;
      
__root const CFG_HDR cfghdr74={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Ìî?ôàéë?
__root const CFG_HDR cfghdr75={CFG_LEVEL,LG_ITEM11,1,0};

__root const CFG_HDR cfghdr76={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_11[15]="";
      
__root const CFG_HDR cfghdr77={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_11[128]="";
      
__root const CFG_HDR cfghdr78={CFG_INT,LG_CADRI,0,12};
      __root const int anim_11=9;
      
__root const CFG_HDR cfghdr79={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc11=0;
      
__root const CFG_HDR cfghdr80={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p11=0;
      
__root const CFG_HDR cfghdr81={CFG_LEVEL,"",0,0};
  
// Ïóíê?"Íàñòðîéê?
__root const CFG_HDR cfghdr82={CFG_LEVEL,LG_ITEM12,1,0};

__root const CFG_HDR cfghdr83={CFG_STR_WIN1251,LG_ITEM_NAME,0,14};
      __root const char name_12[15]="";
      
__root const CFG_HDR cfghdr84={CFG_STR_UTF8,LG_TYPE,0,127};
      __root const char open_12[128]="";
      
__root const CFG_HDR cfghdr85={CFG_INT,LG_CADRI,0,12};
      __root const int anim_12=11;
      
__root const CFG_HDR cfghdr86={CFG_CHECKBOX,LG_CLOSE_MENU,0,2};
      __root const int pc12=0;
      
__root const CFG_HDR cfghdr87={CFG_CHECKBOX,LG_ITEM_PASS,0,2};
      __root const int p12=0;
      
__root const CFG_HDR cfghdr88={CFG_LEVEL,"",0,0};

// Menu Options
__root const CFG_HDR cfghdr89={CFG_LEVEL,"Menu Options",1,0};

__root const CFG_HDR cfghdr90={CFG_STR_WIN1251,"Name of item 1",1,31};
__root const char BM1NAME[32]="";
__root const CFG_HDR cfghdr91={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM1FILE[128]="";

__root const CFG_HDR cfghdr92={CFG_STR_WIN1251,"Name of item 2",1,31};
__root const char BM2NAME[32]="";
__root const CFG_HDR cfghdr93={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM2FILE[128]="";

__root const CFG_HDR cfghdr94={CFG_STR_WIN1251,"Name of item 3",1,31};
__root const char BM3NAME[32]="";
__root const CFG_HDR cfghdr95={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM3FILE[128]="";

__root const CFG_HDR cfghdr96={CFG_STR_WIN1251,"Name of item 4",1,31};
__root const char BM4NAME[32]="";
__root const CFG_HDR cfghdr97={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM4FILE[128]="";

__root const CFG_HDR cfghdr98={CFG_STR_WIN1251,"Name of item 5",1,31};
__root const char BM5NAME[32]="";
__root const CFG_HDR cfghdr99={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM5FILE[128]="";

__root const CFG_HDR cfghdr100={CFG_STR_WIN1251,"Name of item 6",1,31};
__root const char BM6NAME[32]="";
__root const CFG_HDR cfghdr101={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM6FILE[128]="";

__root const CFG_HDR cfghdr102={CFG_STR_WIN1251,"Name of item 7",1,31};
__root const char BM7NAME[32]="";
__root const CFG_HDR cfghdr103={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM7FILE[128]="";

__root const CFG_HDR cfghdr104={CFG_STR_WIN1251,"Name of item 8",1,31};
__root const char BM8NAME[32]="";
__root const CFG_HDR cfghdr105={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM8FILE[128]="";

__root const CFG_HDR cfghdr106={CFG_STR_WIN1251,"Name of item 9",1,31};
__root const char BM9NAME[32]="";
__root const CFG_HDR cfghdr107={CFG_STR_UTF8,LG_TYPE,3,127};
__root const char BM9FILE[128]="";

__root const CFG_HDR cfghdr108={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr109={CFG_LEVEL,"",0,0};

/******************************************************************************/

//////////////////////////////// Set points/////////////////////////////
__root const CFG_HDR cfghdr110={CFG_LEVEL,"Setting GUI",1,0};

__root const CFG_HDR cfghdr111={CFG_LEVEL,"Set points",1,0};
__root const CFG_HDR cfghdr112={CFG_CBOX,"Menu style",0,2};
    __root const int Menu_style=0;
__root const CFG_CBOX_ITEM cfgcbox113[2]={"Standard","QMenu"};

__root const CFG_HDR cfghdr114={CFG_CHECKBOX,LG_POS_SAVE,0,2};
    __root const int possave=0;
    
__root const CFG_HDR cfghdr115={CFG_LEVEL,"Standard",1,0};

__root const CFG_HDR cfghdr116={CFG_UINT,LG_PODNYAT,0,100};
    __root const int podn = 0;

__root const CFG_HDR cfghdr117={CFG_UINT,LG_OTSTUP_X,0,100};
    __root const int smesh_x = 0;

__root const CFG_HDR cfghdr118={CFG_UINT,LG_OTSTUP_Y,0,100};
    __root const int smesh_y = 0;
    
__root const CFG_HDR cfghdr119={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr120={CFG_LEVEL,"QMenu",1,0};

__root const CFG_HDR cfghdr121= {CFG_COLOR, "Cursor Color", 0, 0};
__root const char color1[4]={0xff,0xff,0xff,100};

__root const CFG_HDR cfghdr122= {CFG_COLOR, "Cursor Background Color", 0, 0};
__root const char color2[4]={0,0,0,0};

__root const CFG_HDR cfghdr123={CFG_LEVEL,"",0,0};
__root const CFG_HDR cfghdr124={CFG_LEVEL,"",0,0};

//§¯§Ñ§ã§ä§â§à§Û§Ü§Ñ §à§Ü§ß§Ñ '§£§Ó§à§Õ §á§Ñ§â§à§Ý§ñ'
__root const CFG_HDR cfghdr125={CFG_LEVEL,"Setting\"Enter password\"",1,0};

__root const CFG_HDR cfghdr126={CFG_COORDINATES,"Background location",0,0};
    __root const unsigned int xpos=0;
    __root const unsigned int ypos=55;

__root const CFG_HDR cfghdr127={CFG_COLOR,"Color label",0,0};
    __root const char fontENcol[4]={255,255,255,100};
    
__root const CFG_HDR cfghdr128={CFG_RECT,"Location", 0, 239};
    __root const RECT pos1 = {27,90,216,140};
    
__root const CFG_HDR cfghdr129={CFG_COLOR,"Color \"***\"",0,0};
    __root const char fontPAcol[4]={255,255,255,100};
    
__root const CFG_HDR cfghdr130={CFG_RECT,"Location", 0, 239};
    __root const RECT pos2 = {38,128,157,143};
    
__root const CFG_HDR cfghdr131={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr132={CFG_LEVEL,"",0,0};

////////////////////////////////////////////////////////////////////////////////
/******************************§¯§Ñ§ã§ä§â§à§Û§Ü§Ú §á§Ñ§â§à§Ý§ñ********************************/
__root const CFG_HDR cfghdr133={CFG_LEVEL,LG_PASS_OF_MENU,1,0};

__root const CFG_HDR cfghdr134={CFG_CBOX,"When asking for password",0,3};
    __root const int prun=0;
__root const CFG_CBOX_ITEM cfgcbox135[3]={"Separate paragraph","Opening","Never"};
    
__root const CFG_HDR cfghdr136={CFG_CHECKBOX,LG_AVTO_PROV,0,2};
    __root const int prov=1;
    
__root const CFG_HDR cfghdr137={CFG_STR_PASS,LG_PASS,0,9};
    __root const char passw[9]="";
    
__root const CFG_HDR cfghdr138={CFG_LEVEL,"",0,0};
/************************§¯§Ñ§ã§ä§â§à§Û§Ü§Ñ §Ñ§ß§Ú§Þ§Ñ§è§Ú§Ú §á§å§ß§Ü§ä§à§Ó §Þ§Ö§ß§ð***********************/
/////////////////////////// §¯§Ñ§ã§ä§â§à§Û§Ü§Ñ §Ñ§ß§Ú§Þ§Ñ§è§Ú§Ú /////////////////////////////////
__root const CFG_HDR cfghdr139={CFG_LEVEL,LG_ANI_ITEM,1,0};

__root const CFG_HDR cfghdr140={CFG_CHECKBOX,LG_ON_ANI,0,2};
    __root const int anim_on=0;

__root const CFG_HDR cfghdr141={CFG_CHECKBOX,LG_ANI_INVERT,0,2};
    __root const int anim_invert=0;
    
__root const CFG_HDR cfghdr142={CFG_CHECKBOX,LG_ANI,0,2};
    __root const int anip=0;
    
__root const CFG_HDR cfghdr143={CFG_UINT,"Number of personnel items",0,12};
    __root const int glob_ani=4;

__root const CFG_HDR cfghdr144={CFG_UINT,LG_SPEED_ANI,0,216};
    __root const int TIME=4;
    
__root const CFG_HDR cfghdr145={CFG_INT,LG_SEL_KADR,0,12};
    __root const int select_nom=0;
    
__root const CFG_HDR cfghdr146={CFG_LEVEL,"",0,0};
////////////////////////////////////////////////////////////////////////////////

__root const CFG_HDR cfghdr147={CFG_CHECKBOX,"loquat?",0,2};
      __root const int Cash=0;
