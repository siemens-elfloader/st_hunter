#include "c:\arm\inc\cfg_items.h"
#include "lang.h"

// ÓÌÙË„Û‡ˆËˇ
//==========================================================Õ¿—“–Œ… » - SETTINGS
__root const CFG_HDR cfghdr0   = {CFG_LEVEL, LGP_CONFIG_SETTINGS, 1,0};
//---------------------------------------------------œ–»¬≈“—“¬»≈ - HELLO MESSAGE
__root const CFG_HDR cfghdr0_0   = {CFG_CHECKBOX,LGP_CONFIG_HELLO_MSG, 0,0};
__root const int ENA_HELLO_MSG=1;
//----------------------------------------------–≈ ŒÕ‘»√”–¿÷»ﬂ - MSG_RECONFIGURE
__root const CFG_HDR cfghdr0_1   = {CFG_CHECKBOX,LGP_CONFIG_RECO_MSG, 0,0};
__root const int ENA_RECO_MSG=1;
//------------------------------------------------------«¿ –€“‹ ¡¿«” - KILL BASE
__root const CFG_HDR cfghdr0_2   = {CFG_CHECKBOX,LGP_CONFIG_KILL_BASE, 0,0};
__root const int ENA_KILL_BASE=1;
//------------------------------------------Csm ¡‡Á˚ ÏÂ‰Ë‡Ù‡ÈÎÓ‚ - MediaBase CSM
__root const CFG_HDR cfghdr0_3= {CFG_STR_WIN1251, LGP_CONFIG_MB_CSM,1,8};
__root const char CSMBIBLADR[9]="A068EDC0";
//-----------------------------------------------------------------“¿ “€ - TAKTS
__root const CFG_HDR cfghdr0_4={CFG_UINT,LGP_CONFIG_TAKTS,150,210};
__root const int TAKTS=200;
__root const CFG_HDR cfghdr0_00={CFG_LEVEL,"",0,0};
//==============================================================================

//==================================================================¬–≈Ãﬂ - TIME
__root const CFG_HDR cfghdr1   = {CFG_LEVEL, LGP_CONFIG_TIME_SET, 1,0};
//-------------------------------------------------------------¬ Àﬁ◊»“‹ - ENABLE
__root const CFG_HDR cfghdr1_1 = {CFG_CHECKBOX, LGP_CONFIG_ENABLE, 0,0};
__root const int ENA_CHECK_TIME=1;
//-------------------------------------------------------------------◊¿—€ - HOUR
__root const CFG_HDR cfghdr1_2 = {CFG_UINT, LGP_CONFIG_TIME_HOUR,0,24};
__root const int T_CLOCK_1=23;
//--------------------------------------------------------------Ã»Õ”“€ - MINUTES
__root const CFG_HDR cfghdr1_3={CFG_UINT, LGP_CONFIG_TIME_MINUTES,0,60};
__root const int T_MIN_1=0;
__root const CFG_HDR cfghdr1_00={CFG_LEVEL,"",0,0};
//==============================================================================



//===========================================================œ–Œ÷≈Õ“€ - PECRENTS
__root const CFG_HDR cfghdr2   = {CFG_LEVEL, LGP_CONFIG_PERCENTS, 1,0};
//-------------------------------------------------------------¬ Àﬁ◊»“‹ - ENABLE
__root const CFG_HDR cfghdr2_1 = {CFG_CHECKBOX, LGP_CONFIG_ENABLE, 0,0};
__root const int ENA_CHECK_PERC=1;
//-----------------------------------------------------------œ–Œ÷≈Õ“€ - PECRENTS
__root const CFG_HDR cfghdr2_3={CFG_UINT,LGP_CONFIG_PERCENTS,0,100};
__root const int PERCENT=20;
__root const CFG_HDR cfghdr2_00={CFG_LEVEL,"",0,0};
//==============================================================================


//============================================================ACTIONS - ƒ≈…—“¬»ﬂ
__root const CFG_HDR cfghdr3   = {CFG_LEVEL, LGP_CONFIG_ACTIONS, 1,0};

__root const CFG_HDR cfghdr3_0={CFG_CHECKBOX, LGP_CONFIG_CLOSE_CMPAB ,0,0};
__root const int ENA_KILL_CMPAB=0;

//------------------------------------------------------MEDIAPLAYER - Ã≈ƒ»¿œÀ≈≈–
__root const CFG_HDR cfghdr3_1={CFG_CBOX, LGP_CONFIG_ACT_MP, 1,4};
__root const int MODE_ACT_MP=0;
__root const CFG_CBOX_ITEM cfgcbox3_1_1[4]={ LGP_CONFIG_PLAYER_KILL  , 
                                             LGP_CONFIG_PLAYER_STOP  , 
                                             LGP_CONFIG_PLAYER_PAUSE ,
                                             LGP_CONFIG_NONE         };
//-----------------------------------------------------------¬€ Àﬁ◊»“‹ - SHUTDOWN
__root const CFG_HDR cfghdr3_2={CFG_CBOX, LGP_CONFIG_ACT_PHONE, 1,4};
__root const int MODE_FONE=0;
__root const CFG_CBOX_ITEM cfgcbox3_2_1[4]={ LGP_CONFIG_SHUTDOWN , 
                                             LGP_CONFIG_REBOOT   , 
                                             LGP_CONFIG_LOCK     ,
                                             LGP_CONFIG_NONE    }; 
//--------------------------------------------------------—¬≈–Õ”“‹ ¬—® - CURTAIL
__root const CFG_HDR cfghdr3_4={CFG_CHECKBOX, LGP_CONFIG_CURTAIL ,0,0};
__root const int ENA_SWAP=1;
//-------------------------------------------------------------œ–ŒŸ¿Õ»≈ - BY_MSG
__root const CFG_HDR cfghdr3_5={CFG_CHECKBOX, LGP_CONFIG_BY_MSG,0,0};
__root const int ENA_BY_MSG=0;
//--------------------------------------------------“≈ —“ œ–ŒŸ¿Õ»ﬂ - BY_MSG_TEXT
__root const CFG_HDR cfghdr3_6={CFG_STR_WIN1251, LGP_CONFIG_BY_MSG_TEXT,0,127};
__root const char BY_TEXT[128]="GOOD NIGHT!";
__root const CFG_HDR cfghdr3_00={CFG_LEVEL,"",0,0};
//==============================================================================
