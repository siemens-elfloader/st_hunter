//  ŒÕ‘»√!!!
#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"


#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_LEVEL,"Global setting",1,0};

    __root const CFG_HDR cfghdr0_o={CFG_STR_UTF8,"Bookmark config",3,127};
    __root const char config[128]=DEFAULT_DISK":\\fastrun.fmnu";

#ifdef NEWSGOLD
    __root const CFG_HDR cfghdr_2_2={CFG_CHECKBOX,"Enable eMenu",0,2};
    __root const int ENA_EMENU = 1;
    
    __root const CFG_HDR cfghdr0_0={CFG_STR_UTF8,"Path to eMenu",3,127};
    __root const char eMenuPath[128]="4:\\zbin\\utilities\\emenu.elf";
#endif    
    
    __root const CFG_HDR cfghdr_q_2={CFG_CHECKBOX,"Use full set font",0,2};
    __root const int fullset = 1;
    
    __root const CFG_HDR cfghdr3={CFG_UINT,"Pixels between letters",0,65535};
    __root const int space=1;    
    
    __root const CFG_HDR cfghdr1_112={CFG_STR_UTF8,"Font path 1",0,127};
    __root const char FontPath[128]=DEFAULT_DISK":\\zbin\\img\\font2\\";
    
    __root const CFG_HDR cfghdr1112={CFG_STR_UTF8,"Font path 2",0,127};
    __root const char FontPath2[128]=DEFAULT_DISK":\\zbin\\img\\font2\\";
   
    __root const CFG_HDR cfghdr13={CFG_UINT,"Key change FastPanels",0,65535};
    __root const int KEY_CHANGE_PANEL=0x2A;// '*'
__root const CFG_HDR cfghdr2111={CFG_LEVEL,"",0,0};//Global setting


//--------------------------------------------------------------------------------------//


__root const CFG_HDR cfghdr2={CFG_LEVEL,"FastRun 1 panel setup",1,0};

__root const CFG_HDR cfghdr129_9={CFG_LEVEL,"General setting",1,0};
    __root const CFG_HDR cfghdr_5_2={CFG_CHECKBOX,"Enable FastRun panel",0,2};
    __root const int FASTRUN_ENA = 1;
    
    __root const CFG_HDR cfghdr29_4={CFG_CBOX,"Show name",0,2};
    __root const int FASTRUN_NAME_1=0;
    __root const CFG_CBOX_ITEM cfgcbox21_23[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr2323_4={CFG_CBOX,"Start only idle",0,2};
    __root const int IDLE_1=0;
    __root const CFG_CBOX_ITEM cfgcbox23_23[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr1_s3={CFG_UINT,"Font",0,65535};
    __root const int font_1=1;
    /*
    __root const CFG_HDR cfghdr1112={CFG_STR_UTF8,"Font path 1",0,127};
    __root const char FontPath[128]=DEFAULT_DISK":\\zbin\\img\\font2\\";
    */
    __root const CFG_HDR cfghdr1_10={CFG_CBOX,"Alignment 1 panel",0,3};
    __root const int align_1=0;
    __root const CFG_CBOX_ITEM cfgcbox1[3]={"Left","Center","Right"};

__root const CFG_HDR cfghdr19_9s={CFG_LEVEL,"",0,0};//General setting


__root const CFG_HDR cfghdr1={CFG_LEVEL,"Setup keys",1,0};
    __root const CFG_HDR cfghdr2334={CFG_CBOX,"Enable key activation style",0,2};
    __root const int ENA_FR_LONGPRESS=1;
    __root const CFG_CBOX_ITEM cfgcbox23[2]={"Short press","Long press"};
    
    __root const CFG_HDR cfghdr5_3={CFG_UINT,"Enable key (DEC)",0,65535};
    __root const int FR_CALL_BUTTON=GREEN_BUTTON;
    
    __root const CFG_HDR cfghdr4={CFG_UINT,"Disable key (DEC)",0,65535};
    __root const int FR_EXIT_BUTTON=GREEN_BUTTON;
__root const CFG_HDR cfghdr5={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr6={CFG_LEVEL,"Setup position",1,0};
    __root const CFG_HDR cfghdr7={CFG_CBOX,"Position on screen",0,2};
    __root const int DRAW_MODE=0;
    __root const CFG_CBOX_ITEM cfgcbox1111[2]={"Horizontal","Vertical"};
    
    __root const CFG_HDR cfghdr68={CFG_COORDINATES,"Coordinates panel",0,0};
    __root const unsigned int x1_fr=0;
    __root const unsigned int y1_fr=75;
    
    __root const CFG_HDR cfghdr64_8={CFG_COORDINATES,"Coordinates name",0,0};
    __root const unsigned int name_fr1_x1=50;
    __root const unsigned int name_fr1_y1=150;
    
    __root const CFG_HDR cfghdr10={CFG_UINT,"Panel lenght",0,320};
    __root const unsigned int LEN=165;
    
    __root const CFG_HDR cfghdr6921={CFG_CBOX,"Pictures size",0,5};
    __root const int FR_PIC_SIZE=0;
    __root const CFG_CBOX_ITEM cfgcbox4433[5]={"16x16","24x24","32x32","48x48","64x64"};
__root const CFG_HDR cfghdr11={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr12={CFG_LEVEL,"Setup colors and font",1,0};
    __root const CFG_HDR cfghdr1110={CFG_UINT,"Text font",0,320};
    __root const unsigned int TEXT_FONT_1=8;
    
    __root const CFG_HDR cfghdr60={CFG_COLOR,"Panel border col",0,0};
    __root const char cfgPanBorderCol[4]={255,255,255,100};
    
    __root const CFG_HDR cfghdr61={CFG_COLOR,"Panel BG col",0,0};
    __root const char cfgPanBGCol[4]={255,255,255,37};
    
    __root const CFG_HDR cfghdr62={CFG_COLOR,"Bookmark border col",0,0};
    __root const char cfgBookBorderCol[4]={255,255,255,100};
    
    __root const CFG_HDR cfghdr63={CFG_COLOR,"Bookmark BG col",0,0};
    __root const char cfgBookBGCol[4]={255,255,255,37};
__root const CFG_HDR cfghdr100_3={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr66={CFG_LEVEL,"",0,0};


//---------------------------------FAST PANEL 2--------------------------------//


__root const CFG_HDR cfghdr2_2={CFG_LEVEL,"FastRun 2 panel setup",1,0};

__root const CFG_HDR cfghdr2_22={CFG_LEVEL,"General setting",1,0};
    __root const CFG_HDR cfghdr_q_22={CFG_CHECKBOX,"Enable FastRun panel",0,2};
    __root const int FASTRUN_ENA_2 = 0;
    
    __root const CFG_HDR cfghdr29_2={CFG_CBOX,"Show name",0,2};
    __root const int FASTRUN_NAME_2=0;
    __root const CFG_CBOX_ITEM cfgcbox21_22[2]={"No","Yes"};

    __root const CFG_HDR cfghdr211_4={CFG_CBOX,"Start only idle",0,2};
    __root const int IDLE_2=0;
    __root const CFG_CBOX_ITEM cfgcbox222_23[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr1_23={CFG_UINT,"Font",0,65535};
    __root const int font_2=1;
    /*
    __root const CFG_HDR cfghdr1_112={CFG_STR_UTF8,"Font path 2",0,127};
    __root const char FontPath2[128]=DEFAULT_DISK":\\zbin\\img\\font2\\"; */
    
    __root const CFG_HDR cfghdr11_0={CFG_CBOX,"Alignment 2 panel",0,3};
    __root const int align_2=0;
    __root const CFG_CBOX_ITEM cfgcbox12[3]={"Left","Center","Right"};

__root const CFG_HDR cfghdr2_ss2={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr1_2={CFG_LEVEL,"Setup keys",1,0};

    __root const CFG_HDR cfghdr2334_2={CFG_CBOX,"Enable key activation style",0,2};
    __root const int ENA_FR_LONGPRESS_2=1;
    __root const CFG_CBOX_ITEM cfgcbox23_2[2]={"Short press","Long press"};
    
    __root const CFG_HDR cfghdr3_2={CFG_UINT,"Enable key (DEC)",0,65535};
    __root const int FR_CALL_BUTTON_2=0;
    
    __root const CFG_HDR cfghdr4_2={CFG_UINT,"Disable key (DEC)",0,65535};
    __root const int FR_EXIT_BUTTON_2=0;
__root const CFG_HDR cfghdr5_2={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr6_2={CFG_LEVEL,"Setup position",1,0};

    __root const CFG_HDR cfghdr17={CFG_CBOX,"Position on screen",0,2};
    __root const int DRAW_MODE_2=0;
    __root const CFG_CBOX_ITEM cfgcbox22[2]={"Horizontal","Vertical"};
    
    __root const CFG_HDR cfghdr648={CFG_COORDINATES,"Coordinates panel",0,0};
    __root const unsigned int x1_fr_2=0;
    __root const unsigned int y1_fr_2=75;
    
    __root const CFG_HDR cfghdr64_68={CFG_COORDINATES,"Coordinates name",0,0};
    __root const unsigned int name_fr2_x1=50;
    __root const unsigned int name_fr2_y1=150;
    
    __root const CFG_HDR cfghdr10_2={CFG_UINT,"Panel lenght",0,320};
    __root const unsigned int LEN_2=165;
    
    __root const CFG_HDR cfghdr6921_2={CFG_CBOX,"Pictures size",0,5};
    __root const int FR_PIC_SIZE_2=0;
    __root const CFG_CBOX_ITEM cfgcbox4433_2[5]={"16x16","24x24","32x32","48x48","64x64"};

__root const CFG_HDR cfghdr11_2={CFG_LEVEL,"",0,0};
    
__root const CFG_HDR cfghdr12_2={CFG_LEVEL,"Setup colors and font",1,0};
    __root const CFG_HDR cfghdr60_2={CFG_COLOR,"Panel border col",0,0};
    __root const char cfgPanBorderCol_2[4]={0xFF,0x00,0x00,100};
    
    __root const CFG_HDR cfghdr61_2={CFG_COLOR,"Panel BG col",0,0};
    __root const char cfgPanBGCol_2[4]={0x00,0x00,255,37};
    
    __root const CFG_HDR cfghdr62_2={CFG_COLOR,"Bookmark b rder col",0,0};
    __root const char cfgBookBorderCol_2[4]={0x00,0x00,255,100};
    
    __root const CFG_HDR cfghdr63_2={CFG_COLOR,"Bookmark BG col",0,0};
    __root const char cfgBookBGCol_2[4]={255,255,255,37};
    __root const CFG_HDR cfghdr1_0={CFG_UINT,"Text font",0,320};
    __root const unsigned int TEXT_FONT_2=8;
__root const CFG_HDR cfghdr13_2={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr1588_2={CFG_LEVEL,"",0,0};

//---------------------------------FAST PANEL 3-------------------------------//

__root const CFG_HDR cfghdr31_2={CFG_LEVEL,"FastRun 3 panel setup",1,0};

__root const CFG_HDR cfghdr31_223={CFG_LEVEL,"General setting",1,0};
    __root const CFG_HDR cfghdr_3_22={CFG_CHECKBOX,"Enable FastRun panel",0,2};
    __root const int FASTRUN_ENA_3 = 0;
    
    __root const CFG_HDR cfghdr29_3={CFG_CBOX,"Show name",0,2};
    __root const int FASTRUN_NAME_3=0;
    __root const CFG_CBOX_ITEM cfgcbox23_22[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr231_4={CFG_CBOX,"Start only idle",0,2};
    __root const int IDLE_3=0;
    __root const CFG_CBOX_ITEM cfgcbox33_23[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr12223={CFG_UINT,"Font",0,65535};
    __root const int font_3=1;    
         /*
    __root const CFG_HDR cfghdr11_12={CFG_STR_UTF8,"Font path 3",0,127};
    __root const char FontPath3[128]=DEFAULT_DISK":\\zbin\\img\\font2\\";*/
    
    __root const CFG_HDR cfghdr110s={CFG_CBOX,"Alignment 3 panel",0,3};
    __root const int align_3=0;
    __root const CFG_CBOX_ITEM cfgcbox13[3]={"Left","Center","Right"};
__root const CFG_HDR cfghdr31_2_2={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr3_22={CFG_LEVEL,"Setup keys",1,0};
    __root const CFG_HDR cfghdr33_32={CFG_CBOX,"Enable key activation style",0,2};
    __root const int ENA_FR_LONGPRESS_3=1;
    __root const CFG_CBOX_ITEM cfgcbox233_2[2]={"Short press","Long press"};
    
    __root const CFG_HDR cfghdr3_3={CFG_UINT,"Enable key (DEC)",0,65535};
    __root const int FR_CALL_BUTTON_3=0;
    
    __root const CFG_HDR cfghdr4_3={CFG_UINT,"Disable key (DEC)",0,65535};
    __root const int FR_EXIT_BUTTON_3=0;
__root const CFG_HDR cfghdr50_3={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr6_3={CFG_LEVEL,"Setup position",1,0};
    __root const CFG_HDR cfghdr173={CFG_CBOX,"Position on screen",0,2};
    __root const int DRAW_MODE_3=0;
    __root const CFG_CBOX_ITEM cfgcbox232[2]={"Horizontal","Vertical"};
    
    __root const CFG_HDR cfghdr6438={CFG_COORDINATES,"Coordinates panel",0,0};
    __root const unsigned int x1_fr_3=0;
    __root const unsigned int y1_fr_3=75;
    
    __root const CFG_HDR cfghdr64_28={CFG_COORDINATES,"Coordinates name",0,0};
    __root const unsigned int name_fr3_x1=50;
    __root const unsigned int name_fr3_y1=150;
    
    __root const CFG_HDR cfghdr10_42={CFG_UINT,"Panel lenght",0,320};
    __root const unsigned int LEN_3=165;
    
    __root const CFG_HDR cfghdr623_2={CFG_CBOX,"Pictures size",0,5};
    __root const int FR_PIC_SIZE_3=0;
    __root const CFG_CBOX_ITEM cfgcbox33_2[5]={"16x16","24x24","32x32","48x48","64x64"};
__root const CFG_HDR cfghdr131_2={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr12_3={CFG_LEVEL,"Setup colors and font",1,0};
    __root const CFG_HDR cfghdr60_3={CFG_COLOR,"Panel border col",0,0};
    __root const char cfgPanBorderCol_3[4]={0xFF,0x00,0x00,100};
    
    __root const CFG_HDR cfghdr61_3={CFG_COLOR,"Panel BG col",0,0};
    __root const char cfgPanBGCol_3[4]={0x00,0x00,255,37};
    
    __root const CFG_HDR cfghdr62_3={CFG_COLOR,"Bookmark border col",0,0};
    __root const char cfgBookBorderCol_3[4]={0x00,0x00,255,100};
    
    __root const CFG_HDR cfghdr63_3={CFG_COLOR,"Bookmark BG col",0,0};
    __root const char cfgBookBGCol_3[4]={255,255,255,37};
    
    __root const CFG_HDR cfghdr11_00={CFG_UINT,"Text font",0,320};
    __root const unsigned int TEXT_FONT_3=8;
__root const CFG_HDR cfghdr13_3={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr1538_2={CFG_LEVEL,"",0,0};


//-----------------------------IDLE PANEL-----------------------------//


__root const CFG_HDR cfghdr67={CFG_LEVEL,"Idle panel setup",1,0};

__root const CFG_HDR cfghdr19_9={CFG_LEVEL,"General setting",1,0};

    __root const CFG_HDR cfghdr_c_2={CFG_CHECKBOX,"Display Idle panel",0,2};
    __root const int IDLE_ENA = 1;
    
    __root const CFG_HDR cfghdr692={CFG_CBOX,"Show Idle name",0,2};
    __root const int IDLE_NAME_MODE=0;
    __root const CFG_CBOX_ITEM cfgcbox4435[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr69_2={CFG_CBOX,"Show Idle panel is not active",0,2};
    __root const int SHOW_NOT_ATC_IDLE_P=1;
    __root const CFG_CBOX_ITEM cfgcbox44_35[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr66_2={CFG_CBOX,"Show Idle panel is kbdlock",0,2};
    __root const int SHOW_KDB_LOCK=0;
    __root const CFG_CBOX_ITEM cfgcbox4_5[2]={"No","Yes"};

    __root const CFG_HDR cfghdr1773={CFG_UINT,"Font",0,65535};
    __root const int font_idle=1;
/*
     __root const CFG_HDR cfghdr111_12={CFG_STR_UTF8,"Font path idle",0,127};
    __root const char FontPathIdle[128]=DEFAULT_DISK":\\zbin\\img\\font2\\";
    */
    __root const CFG_HDR cfghdr1130={CFG_CBOX,"Alignment idle panel",0,3};
    __root const int align_idle=0;
    __root const CFG_CBOX_ITEM cfgcbox14[3]={"Left","Center","Right"};
    
__root const CFG_HDR cfghdr1_99={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr199={CFG_LEVEL,"Setup keys",1,0};
    __root const CFG_HDR cfghdr2434={CFG_CBOX,"Enable key activation style",0,2};
    __root const int ENA_IDLE_LONGPRESS=1;
    __root const CFG_CBOX_ITEM cfgcbox443[2]={"Short press","Long press"};
    
    __root const CFG_HDR cfghdr389={CFG_UINT,"Enable key (DEC)",0,65535};
    __root const int IDLE_CALL_BUTTON=RIGHT_SOFT;
    
    __root const CFG_HDR cfghdr487={CFG_UINT,"Disable key (DEC)",0,65535};
    __root const int IDLE_EXIT_BUTTON=RIGHT_SOFT;
__root const CFG_HDR cfghdr576={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr671={CFG_LEVEL,"Setup position",1,0};
    __root const CFG_HDR cfghdr67_2={CFG_CBOX,"Idle position",0,2};
    __root const int IDLE_POSITION=1;
    __root const CFG_CBOX_ITEM cfgcbox5_5[2]={"Vertical","Horizontal"};
    
    __root const CFG_HDR cfghdr6_8={CFG_COORDINATES,"Coordinates panel",0,0};
#ifdef NEWSGOLD
    __root const unsigned int x1=0;
    __root const unsigned int y1=220;
#else
    __root const unsigned int x1=0;
    __root const unsigned int y1=50;
#endif
    
    __root const CFG_HDR cfghdr100={CFG_COORDINATES,"Coordinates name",0,0};
    __root const unsigned int name_idle_x1=70;
    __root const unsigned int name_idle_y1=50;
    
    __root const CFG_HDR cfghdr69={CFG_UINT,"Panel lenght",0,240};
#ifdef NEWSGOLD
    __root const unsigned int LEN_ID=319;
#else
    __root const unsigned int LEN_ID=131;
#endif
    
    __root const CFG_HDR cfghdr693={CFG_CBOX,"Pictures size",0,5};
#ifdef NEWSGOLD
    __root const int IDLE_PIC_SIZE=2;
#else
    __root const int IDLE_PIC_SIZE=0;
#endif
    __root const CFG_CBOX_ITEM cfgcbox4434[5]={"16x16","24x24","32x32","48x48","64x64"};
__root const CFG_HDR cfghdr8_12={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr672={CFG_LEVEL,"Setup colors",1,0};
    __root const CFG_HDR cfghdr134={CFG_COLOR,"Panel enabled BG col",0,0};
    __root const char cfgPanBGCol_ID_ena[4]={255,255,255,37};
    
    __root const CFG_HDR cfghdr1343={CFG_COLOR,"Panel disabled BG col",0,0};
    __root const char cfgPanBGCol_ID_dis[4]={255,255,255,16};
    
    __root const CFG_HDR cfghdr135={CFG_COLOR,"Bookmark border col",0,0};
    __root const char cfgBookBorderCol_ID[4]={255,255,255,100};
    
    __root const CFG_HDR cfghdr136={CFG_COLOR,"Bookmark BG col",0,0};
    __root const char cfgBookBGCol_ID[4]={255,255,255,20};
    
    __root const CFG_HDR cfghdr137={CFG_COLOR,"Font col",0,0};
    __root const char cfgFontCol_ID[4]={255,255,255,100};
__root const CFG_HDR cfghdr111={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr128={CFG_LEVEL,"Setup fonts",1,0};
    __root const CFG_HDR cfghdr157={CFG_UINT,"Font Size",0,100};
#ifdef NEWSGOLD
    __root const unsigned int TEXT_FONT=8;
#else
    __root const unsigned int TEXT_FONT=7;
#endif        
__root const CFG_HDR cfghdr138={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr1548_2={CFG_LEVEL,"",0,0};
