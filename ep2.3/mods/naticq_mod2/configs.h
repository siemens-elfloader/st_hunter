#ifndef _CONFIG_H_
#define _CONFIG_H_




//-------------------------------------------------------//
//                ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ                  //
//-------------------------------------------------------//


//------------ origin setting ---------------//

extern const int DEVELOP_IF_MOD;
extern const int SHOW_ACTIVE;
extern const int HISTORY_SAVE_TYPE;
extern const int HISTORY_BUFFER_MOD;
extern const unsigned int extended_sounds;
extern const unsigned int ext_type_sounds;
extern const char sndGlobal2[64];
extern const char sndMsg2[64];
extern const char pingsnd[64];
#ifdef ELKA
extern const char ICON_PATH2[64];
#endif
extern const char sndLostConn[64];
//-------------- INTERFACE ----------------//

extern const int LINE_MODE;
extern const int VIEW_MODE;
extern const unsigned int SOFT_MODE;
extern const unsigned int MAXLOGMSG;
extern const unsigned int ShowStatusToIconBar;    
extern const unsigned int PingToIconBar;  
extern const unsigned int ShowProcessSend;
extern const int SelBoldActiveChat;
extern const unsigned int IllByONCantact;

extern const char CLBackgrClr[4];

extern const char SoftBackgrClr[4];   
extern const char SoftFontClr[4];

extern const char HeadBackgrClr[4];   
extern const char HeadFontClr[4];
extern const char ScrollBarClr[4];

extern const char ClCursorClr[4];
extern const char ClCursorBordClr[4];
extern const char ActiveClr[4];
extern const char NotActiveClr[4];
extern const char GroupClr[4];
extern const char CLDescricpClr[4];

extern const char MMBackgrClr[4];
extern const char MMBackgrBordClr[4];
extern const char MMCursorBGClr[4];
extern const char MMCursorClr[4]; 
extern const RECT MMRect;
extern const RECT SRect;
extern const RECT XSRect;
extern const unsigned int ShowNumbMenu;

extern const char UnAckedClr[4];
extern const char SendClr[4];
   
   
extern const int isPopupOff;
extern const char AddInfoClr[4];
extern const char FontInfoClr[4];
//--------------------
  
extern const char fontpath2[128];
extern const char fontpath3[128];        
extern const char fontpath4[128];
extern int PNG_HEADER;
extern int PNG_SOFTKEY;        
extern const int isSystemFont;
extern const unsigned int pngFont;  
extern const unsigned int HeadFont;  

//------------ SETTING BUTTON -------------//

extern const int LongRightAction;
extern const int LongLeftAction;
extern const int ActionResh;
extern const int LongVolUpAction;
extern const int LongVolDownAction ;
extern const int isVolUpButton;//If action vol up && vol down button on elka
extern const int isVolDownButton;

extern const int longAct_0;
extern const int longAct_1;
extern const int longAct_2;
extern const int longAct_3;
extern const int longAct_4;
extern const int longAct_5;
extern const int longAct_6;
extern const int longAct_7;
extern const int longAct_8;
extern const int longAct_9;
extern const int longAct_10;//*
extern const int longAct_11;//#
extern const int longAct_RB;


//------------ STATUS SETTING --------------//

#ifdef NEWSGOLD
extern const int ShowStatusIcon;
#endif
extern const int show_xstatus;
extern const unsigned int IDLEICON_XX;
extern const unsigned int IDLEICON_YX;
extern const int ind_set_xstatus;
#ifdef NEWSGOLD
extern const int blink_mess;
extern const unsigned int NumStatusToIconBar;
extern const unsigned int NumXStatusToIconBar;
extern const unsigned int ADDITION_UIN;
extern const int ShowAddStatusToIconBar;
extern const int show_add_state;
extern const int show_add_xstate;
#endif


//----------- PNG FONT SETTING -------------//

extern const unsigned int fonto;
extern const char fontpath[64];
extern const unsigned int spaco1;
extern const unsigned int spaco2;
  

//------------ smiles color -----------------//
extern const int UseStandartSmileSel;
extern const char SmBGcolor[4];
extern const char SmSelectColor[4];
extern const char SmFontColor[4];

//-------------- online time ----------------//

//Online time on idle
extern const int Is_UPTIME_On;
extern const unsigned int onlineTime_x;
extern const unsigned int onlineTime_y;
extern const int Pause_Time;  


//-------------  AUTO PING/UNREAD -----------// 
extern const int isPingIdle; 
extern const unsigned int Ping_X;
extern const unsigned int Ping_Y;
extern const unsigned int ping_time_config;

extern const int Is_Unread_On;
extern const unsigned int unread_X;
extern const unsigned int unread_Y;

extern const int Is_Online_On;
extern const int Is_Online_On_Align;
extern const unsigned int online_x;
extern const unsigned int online_y;  

extern const int Is_SMARTCPU_On ;
extern const unsigned int msg_d_x;
extern const unsigned int msg_d_y;
extern const unsigned int main_msg_timer;//Кол-во секунд сколько показывать сообщения на идле
extern const char fontColor[];//Цвет для системного шрифта
extern const char bgrColor[];
extern const unsigned int main_lenght;
extern const int use_cutter;


//---------------- AUTO STATUS ---------------//

extern const int Auto_Status;//Включен автостатус или нет 
extern const int IsOnNAStatus;
extern const unsigned int NAStatusTime;   
extern const int isAutoStatusOn;// = 0;
extern const unsigned int AUTO_ENGADE;
extern const unsigned int automsgcount;
extern const int time_zone;
extern const char awayHelloMsg[24];//hello msg
extern char awayMsg[256];//message
extern const int showAwayIdle;  
extern const unsigned int auto_x;
extern const unsigned int auto_y;
extern const int Reset_timer;  


//------------- AUTO CHANGE STATUS ------------//

extern const TTime tm_status_1;
extern const int status_1;
extern const TTime tm_status_2;
extern const int status_2;
extern const TTime tm_status_3;
extern const int status_3;
extern const TTime tm_status_4;
extern const int status_4;
extern const TTime tm_status_5;
extern const int status_5;    
    

//---------------  SCREEN SAVER ---------------//

#ifdef NEWSGOLD
extern const unsigned int screenSaverTime;
extern const int isScreenSaverEna;
#endif

//------------ popup message -----------------//

extern const int PopupShow;// = 3;
extern const char PopupMsgString[9];//="%s";
extern const char PopupOnlineString[9];//=">%s<";  
extern const unsigned int PopupFont;//=FONT_SMALL;
extern const char PopupBGCl[4];
extern const char PopupOnlineCl[4];    
extern const char PopupMsgCl[4];

extern const char PopupOffBGCl[4];
extern const char PopupOffCl[4];

//------------ CL HEADER SETTING -------------//

//extern int TIME_OR_MSG;
extern const unsigned int ON_OFF_TIME;
extern const int HEAD_STATUS;
extern const int HEAD_XSTATUS;
extern const int HEAD_PRIVATE;
extern const int HEAD_PING;
extern const int ShowTraff;
extern const int isShowCont;
extern const int isShowUnread;
extern const int isShowI;
extern const int isShowV;
extern const int isShowS;
extern const int isShowR;
extern const int isShowA;
//extern int isShowTmXs;
//extern int isShowTime;
extern const int isShowBat;


//-------------  EXTRA CONFIG ---------------//

extern const unsigned int Vibra_cc;
#ifdef NEWSGOLD
extern const char playing[32];
extern const char CsmAdr[9];
extern const int Is_ID3_ANY_STATUS;
#endif
extern const int show_send_info;  
extern const int isLogStatusChange;
extern const int IsShowLoopback;//Show Loopback contact in CL
extern const int IsOpenBrowser;
extern const int IsMultiBuffer;


//-------------  LOAD SCREEN SET ------------//

extern const RECT PBRECT;
extern const char PBcolor[4];
extern const char BGPBcolor[4];
extern const int IsInvertSoftKey;
extern const int IsShowInfo;

//------------------- LOG -------------------//

extern const int IsLogEna;
extern const unsigned int _fontlog;
extern const char BGLog[4];
extern const char FontClrLog[4];

//------------------- INFA -----------------//

extern const char BGInfo[4];
extern const char FontClrInfo[4];


extern const char ignor_list[256];
extern const char I_str[32];


//------------ ORIGINAL SETTING -------------//  

extern unsigned int UIN;
extern char PASS[9];

extern int VIBR_ON_CONNECT;
//extern int SHOW_ACTIVE;
extern int VIBR_TYPE;
extern int DEVELOP_IF;

extern int LOG_ALL;
extern int LOG_STATCH;
extern int FIRST_LETTER;
extern int SORT_CLIST;


extern int HISTORY_TYPE;
extern unsigned int vibraPower;

extern unsigned int IDLEICON_X;
extern unsigned int IDLEICON_Y;
  
extern unsigned int I_COLOR;
extern unsigned int TO_COLOR;
extern unsigned int X_COLOR;
extern unsigned int O_I_COLOR;
extern unsigned int O_TO_COLOR;
extern unsigned int O_X_COLOR;
  
extern unsigned int ED_FONT_SIZE;
extern unsigned int ED_H_FONT_SIZE;
extern unsigned int ED_X_FONT_SIZE;
extern unsigned int O_ED_FONT_SIZE;
extern unsigned int O_ED_H_FONT_SIZE;
extern unsigned int O_ED_X_FONT_SIZE;
  
extern unsigned int ACK_COLOR;
extern unsigned int UNACK_COLOR;
  
extern int ENA_AUTO_XTXT;
extern int NOT_LOG_SAME_XTXT;
extern int LOG_XTXT;
extern int HISTORY_BUFFER;  
  
//Illumination by BoBa 19.04.2007
///////////
extern unsigned int ILL_DISP_RECV;
extern unsigned int ILL_KEYS_RECV;
extern unsigned int ILL_RECV_TMR;
extern unsigned int ILL_DISP_SEND;
extern unsigned int ILL_KEYS_SEND;
extern unsigned int ILL_SEND_TMR;
extern unsigned int ILL_SEND_FADE;
extern unsigned int ILL_RECV_FADE;
extern unsigned int ILL_OFF_FADE;

extern char sndStartup[64];
extern char sndSrvMsg[64];
extern char sndGlobal[64];
//extern char sndGlobal2[64];
extern char sndMsg[64];
extern char sndMsgSent[64];
extern unsigned int sndVolume;
  
extern char SMILE_FILE[64];
extern char TEMPLATES_PATH[64];
extern char ICON_PATH[64];
extern char SMILE_PATH[64];
extern char XSTATUSES_PATH[64];
extern char HIST_PATH[64];
extern char NATICQ_HOST[128];
extern unsigned int NATICQ_PORT;
extern unsigned int RECONNECT_TIME;


extern int janis;

extern char suc_config_name[64];
//extern char suc_config_name_mod[64];

extern int get_file_size(char* fname);
extern int UpdateConfig();

extern IMGHDR *XStatusImg[];
typedef struct
{
  unsigned int uin;
  char pass[9];
}ADDUIN;


#endif

