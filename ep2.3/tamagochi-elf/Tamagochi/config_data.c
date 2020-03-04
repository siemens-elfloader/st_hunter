#include "..\inc\cfg_items.h"
#include "Tamagochi.h"
#include "Language.h"
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

__root const CFG_HDR cfghdr1_3 = {CFG_UINT, LG_GFGMINSIZE, 0, 500};
__root const unsigned int MINSIZE = 20;

__root const CFG_HDR cfghdr1_4 = {CFG_UINT, LG_GFGMAXSIZE, 0, 500};
__root const unsigned int MAXSIZE = 80;

__root const CFG_HDR cfghdr1_5 = {CFG_UINT, LG_GFGOPACITY, 0, 100};
__root const unsigned int OP = 80;

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

__root const CFG_HDR cfghdr_m20={CFG_LEVEL,"",0,0};

// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m31={CFG_LEVEL,LG_GFGSETPET,1,0};

__root const CFG_HDR cfghdr3_1 = {CFG_CBOX, LG_GFGSPEED, 0, 3};
__root const int SpeedLife = 1;
__root const CFG_CBOX_ITEM cfgcbox3_0[3] = {LG_GFGFAST, LG_GFGMEDIUM, LG_GFGSLOW};

__root const CFG_HDR cfghdr_m30={CFG_LEVEL,"",0,0};
// -------------------------------------------------------------------------- //

// -------------------------------------------------------------------------- //
__root const CFG_HDR cfghdr_m41={CFG_LEVEL,LG_GFGSETALARM,1,0};
__root const CFG_HDR cfghdr18={CFG_UINT,LG_GFGVOLUME,0,6};
__root const unsigned int sndVolume=3;

//__root const CFG_HDR cfghdr18_1={CFG_UINT,"Vibra power",0,100};
//__root const unsigned int vibraPower=100;

//__root const CFG_HDR cfghdr18_2={CFG_CBOX,"Vibration type",0,2};
//__root const int VIBR_TYPE = 0;
//__root const CFG_CBOX_ITEM cfgcbox18_2[2]={"Single","Double"};

//__root const CFG_HDR cfghdr18_3={CFG_CBOX,"Vibra on connect",0,2};
//__root const int VIBR_ON_CONNECT = 0;
//__root const CFG_CBOX_ITEM cfgcbox18_3[2]={"No","Yes"};

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
__root const int ACTIVE_KEY_STYLE=0;
__root const CFG_CBOX_ITEM cfgcbox23[2]={"Short press","Long press"};

#ifdef ELKA
__root const CFG_HDR cfghdr24={CFG_UINT,LG_GFGKEY,0,99};
__root const int ACTIVE_KEY=0x11;
#else
__root const CFG_HDR cfghdr24={CFG_UINT,LG_GFGKEY,0,99};
__root const int ACTIVE_KEY=0x11;
#endif
#endif

__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};

