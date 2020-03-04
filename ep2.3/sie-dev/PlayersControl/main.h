#include "C:\ARM\inc\swilib.h"
#include "C:\ARM\inc\mplayer.h"
#include "rect_patcher.h"
#include "ivoplay_ipc.h"
#include "musicaplayer_ipc.h"
#include "splayer_ipc.h"


#define STANDART 0
#define IVOPLAY 1
#define MAPLAYER 2
#define SPLAYER 3



//------Config-----------
extern const unsigned int PlayPauseKey_1;
extern const unsigned int PlayPauseKey_1Type;
extern const unsigned int PlayPauseKey_2;
extern const unsigned int PlayPauseKey_2Type;
extern const unsigned int StartKey1_1;
extern const unsigned int StartKey1_1Type;
extern const unsigned int StartKey1_2;
extern const unsigned int StartKey1_2Type;
extern const unsigned int StartKey2_1;
extern const unsigned int StartKey2_1Type;
extern const unsigned int StartKey2_2;
extern const unsigned int StartKey2_2Type;
extern const unsigned int StartKey3_1;
extern const unsigned int StartKey3_1Type;
extern const unsigned int StartKey3_2;
extern const unsigned int StartKey3_2Type;
extern const unsigned int StartKey4_1;
extern const unsigned int StartKey4_1Type;
extern const unsigned int StartKey4_2;
extern const unsigned int StartKey4_2Type;
extern const unsigned int CloseKey_1;
extern const unsigned int CloseKey_1Type;
extern const unsigned int CloseKey_2;
extern const unsigned int CloseKey_2Type;
extern const unsigned int StopKey_1;
extern const unsigned int StopKey_1Type;
extern const unsigned int StopKey_2;
extern const unsigned int StopKey_2Type;
extern const unsigned int NextKey_1;
extern const unsigned int NextKey_1Type;
extern const unsigned int NextKey_2;
extern const unsigned int NextKey_2Type;
extern const unsigned int PrevKey_1;
extern const unsigned int PrevKey_1Type;
extern const unsigned int PrevKey_2;
extern const unsigned int PrevKey_2Type;
extern const unsigned int VolUPKey_1;
extern const unsigned int VolUPKey_1Type;
extern const unsigned int VolUPKey_2;
extern const unsigned int VolUPKey_2Type;
extern const unsigned int VolDOWNKey_1;
extern const unsigned int VolDOWNKey_1Type;
extern const unsigned int VolDOWNKey_2;
extern const unsigned int VolDOWNKey_2Type;
extern const unsigned int MuteKey_1;
extern const unsigned int MuteKey_1Type;
extern const unsigned int MuteKey_2;
extern const unsigned int MuteKey_2Type;
extern const unsigned int LockKey_1;
extern const unsigned int LockKey_1Type;
extern const unsigned int LockKey_2;
extern const unsigned int LockKey_2Type;
extern const unsigned int TimerOnKey;
extern const unsigned int TimerOnKeyType;
extern const unsigned int TimerOffKey;
extern const unsigned int TimerOffKeyType;
extern const int TimerHour,TimerMin;
extern const char PATH_STDICON[256];
extern const char PATH_IVOICON[256];
extern const char PATH_MAICON[256];
extern const char PATH_SPICON[256];
extern const char PATH_SPLAYER[256];
extern const char PATH_IVO[256];
extern const char PATH_MAPLAYER[256];
extern const  int player0;
extern const  int player1;
extern const  int player2;
extern const  int player3;
extern const unsigned int MainType;
extern const char *successed_config_filename;
extern const unsigned int HeadsetCType;
extern const unsigned int HeadsetCD;
extern const unsigned int HeadsetDD;
extern const char SHORCUT1[256];
extern const unsigned int HeadsetDType;
extern const char SHORCUT2[256];
extern const RECT TAGSPOS;
extern const char COLOR[4];
extern const int FONT;
extern const int TAGSType;
extern const unsigned int HeadseTimeOut;
extern const int OnOn;
extern const int TimerVibr;
extern const  int ena_lock;
//---------end-of-config--------------------
int IsMaplayerOn=0,
IsIvoOn=0,
IsSPlayerOn=0,
old=0;
int MAINCSM_ID;
//-----------------------
IPC_REQ ipc_send;
IPC_REQ *ipc_recv,gipc;
unsigned int* addr; 
int timer=0,
timer_speed=532,
key_timer=108,
acc=0,
sec,min,hour;
char st[256];
char Tag[256];
int doublekey[15];
GBSTMR Acctmr,
close_timer,
KeyTmr;
char IvoTag[128];
const char perc_ss[]="%s %s";
#ifdef BAR
void IconBar(void);
#endif
int ShowInfo=0;
int ShowTimer;
//-----------------------
extern void kill_data(void *p, void (*func_p)(void *));
const int minus11=-11;
static unsigned short maincsm_name_body[140];
typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;
//------Defines for key types--------------
#define KEY_ONE_CLICK 0
#define KEY_DOUBLE_CLICK 1
#define KEY_LONG_PRESS 2
#define KEY_TWO 3
#define KEY_NOT_USE 4
#define DOUBLE_NULL 0
#define DOUBLE_FIRST 1
#define DOUBLE_SECOND 2
#define TYPE_ONLY_LOCK 0
#define TYPE_ONLY_UNLOCK 1
#define TYPE_BOTH 2
//------end of defines---------------------
char Name[256];
WSHDR *ws;
GBSTMR RedrawTimer;
