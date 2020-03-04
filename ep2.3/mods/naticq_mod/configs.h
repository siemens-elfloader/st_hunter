#ifndef _CONFIG_H_
#define _CONFIG_H_




//-------------------------------------------------------//
//                ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ                  //
//-------------------------------------------------------//


//------------ origin setting ---------------//

extern int DEVELOP_IF_MOD;
extern int SHOW_ACTIVE;
extern int HISTORY_SAVE_TYPE;
extern int HISTORY_BUFFER_MOD;
extern unsigned int extended_sounds;
extern char sndGlobal2[64];
extern char sndMsg2[64];
extern char pingsnd[64];


//------------ SETTING BUTTON -------------//

extern int LongRightAction;
extern int LongLeftAction;
extern int ActionResh;
extern int LongVolUpAction;
extern int LongVolDownAction ;
extern int isVolUpButton;//If action vol up && vol down button on elka
extern int isVolDownButton;

extern int longAct_0;
extern int longAct_1;
extern int longAct_2;
extern int longAct_3;
extern int longAct_4;
extern int longAct_5;
extern int longAct_6;
extern int longAct_7;
extern int longAct_8;
extern int longAct_9;
extern int longAct_10;//*
extern int longAct_11;//#
extern int longAct_RB;


//------------ STATUS SETTING --------------//

#ifdef NEWSGOLD
extern int ShowStatusIcon;
#endif
extern int show_xstatus;
extern unsigned int IDLEICON_XX;
extern unsigned int IDLEICON_YX;
extern int ind_set_xstatus;
#ifdef NEWSGOLD
extern unsigned int NumStatusToIconBar;
extern unsigned int NumXStatusToIconBar;
extern unsigned int ADDITION_UIN;
extern int show_add_state;
extern int show_add_xstate;
#endif


//----------- PNG FONT SETTING -------------//

extern unsigned int fonto;
extern char fontpath[64];
extern unsigned int msg_d_x;
extern unsigned int msg_d_y;
extern unsigned int main_msg_timer;//Кол-во секунд сколько показывать сообщения на идле
extern char fontColor[];//Цвет для системного шрифта
extern char bgrColor[];
extern unsigned int main_lenght;
extern int use_cutter;
extern unsigned int spaco1;
extern unsigned int spaco2;
  

//------------ smiles color -----------------//

extern char SmBGcolor[4];
extern char SmSelectColor[4];
extern char SmFontColor[4];


//-------------- online time ----------------//

//Online time on idle
extern unsigned int onlineTime_x;
extern unsigned int onlineTime_y;
extern int Pause_Time;  


//-------------  AUTO PING/UNREAD -----------// 

extern unsigned int Ping_X;
extern unsigned int Ping_Y;
extern unsigned int ping_time_config;

extern int Is_Unread_On;
extern unsigned int unread_X;
extern unsigned int unread_Y;

extern int Is_Online_On;
extern unsigned int online_x;
extern unsigned int online_y;  


//---------------- AUTO STATUS ---------------//

extern int Auto_Status;//Включен автостатус или нет 
extern int IsOnNAStatus;
extern unsigned int NAStatusTime;   
extern int isAutoStatusOn;// = 0;
extern unsigned int AUTO_ENGADE;
extern unsigned int automsgcount;
extern int time_zone;
extern char awayHelloMsg[24];//hello msg
extern char awayMsg[256];//message
extern int showAwayIdle;  
extern unsigned int auto_x;
extern unsigned int auto_y;
extern int Reset_timer;  


//------------- AUTO CHANGE STATUS ------------//

extern TTime tm_status_1;
extern unsigned int status_1;
extern TTime tm_status_2;
extern unsigned int status_2;
extern TTime tm_status_3;
extern unsigned int status_3;
extern TTime tm_status_4;
extern unsigned int status_4;
extern TTime tm_status_5;
extern unsigned int status_5;    
    

//---------------  SCREEN SAVER ---------------//

#ifdef NEWSGOLD
extern unsigned int screenSaverTime;
extern int isScreenSaverEna;
#endif

//------------ popup message -----------------//

extern int PopupShow;// = 3;
extern char PopupMsgString[9];//="%s";
extern char PopupOnlineString[9];//=">%s<";  
extern unsigned int PopupFont;//=FONT_SMALL;
extern char PopupBGCl[4];
extern char PopupOnlineCl[4];    
extern char PopupMsgCl[4];


//------------ CL HEADER SETTING -------------//

extern int TIME_OR_MSG;
extern unsigned int ON_OFF_TIME;
extern int ShowTraff;
extern int isShowCont;
extern int isShowUnread;
extern int isShowI;
extern int isShowV;
extern int isShowS;
extern int isShowR;
extern int isShowA;
extern int isShowTmXs;
extern int isShowTime;
extern int isShowBat;


//-------------  EXTRA CONFIG ---------------//

extern unsigned int Vibra_cc;
#ifdef NEWSGOLD
extern char playing[13];
extern char CsmAdr[9];
#endif
extern int show_send_info;  
extern int isLogStatusChange;
extern int IsShowLoopback;//Show Loopback contact in CL
extern int IsOpenBrowser;
extern int IsMultiBuffer;


//-------------  LOAD SCREEN SET ------------//

extern RECT PBRECT;
extern char PBcolor[4];
extern char BGPBcolor[4];
extern int IsInvertSoftKey;
extern int IsShowInfo;


extern char ignor_list[256];
extern char I_str[32];


//------------ ORIGINAL SETTING -------------//  

extern unsigned int UIN;
extern char PASS[9];

extern int VIBR_ON_CONNECT;
extern int SHOW_ACTIVE;
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
extern char sndGlobal2[64];
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
extern char suc_config_name_mod[64];

extern int get_file_size(char* fname);
extern int UpdateConfig();


typedef struct
{
  unsigned int uin;
  char pass[9];
}ADDUIN;


#endif

