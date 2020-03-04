#include "..\inc\cfg_items.h"
#include "Tamagochi.h"
#include "language.h"
//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_CBOX,LG_GFGHELLO,0,2};
__root const int ENA_HELLO_MSG=1;
__root const CFG_CBOX_ITEM cfgcbox1[2]={LG_GFGNO,LG_GFGYES};

// -------------------------------------------------------------------------- //

__root const CFG_HDR cfghdr_m11={CFG_LEVEL,LG_GFGSETSCREEN,1,0};

  __root const CFG_HDR cfghdr1={CFG_UINT,LG_GFGUPDATE,0,600};
  __root const unsigned int REFRESH=20;

  __root const CFG_HDR cfghdr1_1 = {CFG_CBOX, LG_GFGVIEW, 0, 3};
  __root const int cfgShowIn = 2;
  __root const CFG_CBOX_ITEM cfgcbox1_0[3] = {LG_GFGLOCK, LG_GFGUNLOCK, LG_GFGBOTH};

  __root const CFG_HDR cfghdr1_2={CFG_COORDINATES,LG_GFGPOSITION,0,0};
  __root const unsigned int POS_X=120;
  __root const unsigned int POS_Y=160; 

  __root const CFG_HDR cfghdr1_3_1 = {CFG_CHECKBOX, "Enable Effects", 0, 2};
  __root const int Effects_Ena = 1;
  
  __root const CFG_HDR cfghdr1_3 = {CFG_UINT, LG_GFGMINSIZE, 0, 500};
  __root const unsigned int MINSIZE = 20;

  __root const CFG_HDR cfghdr1_4 = {CFG_UINT, LG_GFGMAXSIZE, 0, 500};
  __root const unsigned int MAXSIZE = 80;

  __root const CFG_HDR cfghdr1_5 = {CFG_UINT, LG_GFGOPACITY, 0, 100};
  __root const unsigned int OP = 90;

__root const CFG_HDR cfghdr_m10={CFG_LEVEL,"",0,0};

// -------------------------------------------------------------------------- //

__root const CFG_HDR cfghdr_m21={CFG_LEVEL,LG_GFGSETPATH,1,0};

  __root const CFG_HDR cfghdr2_1={CFG_STR_UTF8,LG_GFGPATHPET,0,63};
  __root const char PET_PATH[64]=DEFAULT_DISK ":\\ZBin\\Tamagochi\\Pet.cfg";

  __root const CFG_HDR cfghdr2_2={CFG_STR_UTF8,LG_GFGPATHROOM,0,63};
  __root const char ROOM_PATH[64]=DEFAULT_DISK ":\\ZBin\\Tamagochi\\Room\\";

  __root const CFG_HDR cfghdr2_3={CFG_STR_UTF8,LG_GFGPATHIMG,0,63};
  __root const char PIC_PATH[64]=DEFAULT_DISK ":\\ZBin\\Tamagochi\\img\\";

  __root const CFG_HDR cfghdr2_4={CFG_STR_UTF8,LG_GFGPATHSOUND,0,63};
  __root const char SOUND_PATH[64]=DEFAULT_DISK ":\\ZBin\\Tamagochi\\Sound\\";

  __root const CFG_HDR cfghdr3_2={CFG_STR_UTF8,LG_GFGPATHGAMELIST,0,63};
  __root const char GAMELIST_PATH[64]=DEFAULT_DISK ":\\ZBin\\Tamagochi\\Game.list";

  __root const CFG_HDR cfghdr3_3={CFG_STR_UTF8,LG_GFGPATHPLAYERLIST,0,63};
  __root const char PLAYERLIST_PATH[64]=DEFAULT_DISK ":\\ZBin\\Tamagochi\\Player.list";

  
__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m31={CFG_LEVEL,LG_GFGSETPET,1,0};

  __root const CFG_HDR cfghdr3_1 = {CFG_CBOX, LG_GFGSPEED, 0, 3};
  __root const int SpeedLife = 1;
  __root const CFG_CBOX_ITEM cfgcbox3_0[3] = {LG_GFGFAST, LG_GFGMEDIUM, LG_GFGSLOW};
  

  __root const CFG_HDR cfghdr3_1_2 = {CFG_CHECKBOX, LG_MALWARE, 0, 2};
  __root const int Malware_Ena = 1;
  	
          
   __root const CFG_HDR cfghdr_m311={CFG_LEVEL,"Scan Game",1,0};
  
      __root const CFG_HDR cfghdr3_11 = {CFG_CHECKBOX, "Enable", 0, 2};
      __root const int Scan_Game_Ena = 1;

      __root const CFG_HDR cfghdr3_12={CFG_STR_WIN1251,"UnderIdleCSM_DESC",0,8};
      __root const char UNDER_IDLE_CONSTR[9]="";

   __root const CFG_HDR cfghdr_m310={CFG_LEVEL,"",0,0};
   
   __root const CFG_HDR cfghdr_m321={CFG_LEVEL,"Nightly sleep",1,0};
  
      __root const CFG_HDR cfghdr3_31 = {CFG_CHECKBOX, "Enable", 0, 2};
      __root const int Night_Ena = 1;

      __root const CFG_HDR cfghdr3_32 = {CFG_UINT, "Begin", 0, 23};
      __root const unsigned int Night_begin = 22;

      __root const CFG_HDR cfghdr3_33 = {CFG_UINT, "End", 0, 23};
      __root const unsigned int Night_end = 7;

   __root const CFG_HDR cfghdr_m320={CFG_LEVEL,"",0,0};
   
   
__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m41={CFG_LEVEL,LG_GFGSETALARM,1,0};

  __root const CFG_HDR cfghdr4_1 = {CFG_CHECKBOX, LG_GFGSNDENABLE, 0, 2};
  __root const int sndEnable = 1;

  __root const CFG_HDR cfghdr4_11={CFG_UINT,LG_GFGVOLUME,0,6};
  __root const unsigned int sndVolume=3;

  __root const CFG_HDR cfghdr4_2 = {CFG_CHECKBOX, LG_GFGVBRENABLE, 0, 2};
  __root const int Is_Vibra_Enabled = 1;
  
  __root const CFG_HDR cfghdr_m4_21={CFG_LEVEL,LG_GFGVBRSETUP,1,0};

        __root const CFG_HDR cfghdr4_21={CFG_UINT,LG_GFGVBRPOWER,0,100};
        __root const unsigned int vibraPower=100;
      
        __root const CFG_HDR cfghdr4_22={CFG_CBOX,LG_GFGVBRTYPE,0,2};
        __root const int VIBR_TYPE = 0;
        __root const CFG_CBOX_ITEM cfgcbox18_2[2]={LG_GFGVBRTYPE1,LG_GFGVBRTYPE2};

  __root const CFG_HDR cfghdr_m4_20={CFG_LEVEL,"",0,0};


  __root const CFG_HDR cfghdr4_3 = {CFG_CHECKBOX, LG_GFGBLINKENABLE, 0, 2};
  __root const int lgtEnable = 1;

  __root const CFG_HDR cfghdr_m4_31={CFG_LEVEL,LG_GFGBLINKSETUP,1,0};

        __root const CFG_HDR cfghdr4_1_1 = {CFG_CBOX, LG_GFGBLINKLOCKED, 0, 2};
        __root const int cfgLockOnly = 1;
        __root const CFG_CBOX_ITEM cfgcbox41[2] = {LG_GFGNO,LG_GFGYES};
    
        __root const CFG_HDR cfghdr4_1_2 = {CFG_UINT, LG_GFGBLINKPERIOD, 0, 100};
        __root const unsigned int cfgPeriod = 2;
        
        __root const CFG_HDR cfghdr4_1_21 = {CFG_UINT, LG_GFGBLINKCOUNT, 0, 20};
        __root const unsigned int cfgMaxEv = 5;
        
        __root const CFG_HDR cfghdr4_1_3 = {CFG_UINT, LG_GFGBLINKBRIGHT, 0, 100};
        __root const unsigned int cfgBright = 50;
        
        __root const CFG_HDR cfghdr4_1_4 = {CFG_CBOX, LG_GFGBLINKKBRD, 0, 2};
        __root const int cfgKbd = 1;
        __root const CFG_CBOX_ITEM cfgcbox42[2] = {LG_GFGNO,LG_GFGYES};
        
        __root const CFG_HDR cfghdr4_1_5 = {CFG_CBOX, LG_GFGBLINKDISPL, 0, 2};
        __root const int cfgDispl = 1;
        __root const CFG_CBOX_ITEM cfgcbox43[2] = {LG_GFGNO,LG_GFGYES};
        
        #ifndef NEWSGOLD
        __root const CFG_HDR cfghdr4_1_6 = {CFG_CBOX, LG_GFGBLINKDYN, 0, 2};
        __root const int cfgDyn = 1;
        __root const CFG_CBOX_ITEM cfgcbox44[2] = {LG_GFGNO,LG_GFGYES};
        #else
        __root const CFG_HDR cfghdr4_1_7 = {CFG_CBOX, LG_GFGBLINKLIGHT, 0, 2};
        __root const int cfgLighter = 1;
        __root const CFG_CBOX_ITEM cfgcbox45[2] = {LG_GFGNO,LG_GFGYES};
        #endif
        
        #ifdef ELKA
        __root const CFG_HDR cfghdr4_1_8={CFG_CBOX,"SLI Notify",0,2};
        __root const int cfgSLI = 1;
        __root const CFG_CBOX_ITEM cfgcbox46[2]={LG_GFGNO,LG_GFGYES};
        #endif

  __root const CFG_HDR cfghdr_m4_30={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr_m40={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m51={CFG_LEVEL,LG_GFGSETKEY,1,0};
#ifndef NEWSGOLD
__root const CFG_HDR cfghdr23={CFG_CBOX,LG_GFGMNUENA,0,4};
__root const int ACTIVE_KEY_STYLE=0;
__root const CFG_CBOX_ITEM cfgcbox23[4]={"Short press","Long press","* + #","Enter Button"};

__root const CFG_HDR cfghdr24={CFG_UINT,LG_GFGKEY,0,99};
__root const int ACTIVE_KEY=99;
#else
__root const CFG_HDR cfghdr23={CFG_CBOX,LG_GFGMNUENA,0,2};
__root const int ACTIVE_KEY_STYLE=1;
__root const CFG_CBOX_ITEM cfgcbox23[2]={"Short press","Long press"};

#ifdef ELKA
__root const CFG_HDR cfghdr24={CFG_UINT,LG_GFGKEY,0,99};
__root const int ACTIVE_KEY=13;
#else
__root const CFG_HDR cfghdr24={CFG_UINT,LG_GFGKEY,0,99};
__root const int ACTIVE_KEY=0x11;
#endif
#endif
__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};



