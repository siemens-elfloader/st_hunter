#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"
//--------------------------------------------------------------------//
//                           READ CONFIGS                             //
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//------------------------  ORIGINAL CONFIG  -------------------------//
//--------------------------------------------------------------------//

typedef struct 
{
  
  CFG_HDR cfghdr0;
  unsigned int UIN;
  
  CFG_HDR cfghdr1;
  char PASS[9];

//--------------- NETWORK SETTING -------------------//
  
CFG_HDR cfghdr2;
  CFG_HDR cfghdr2_1;
  char NATICQ_HOST[128];

  CFG_HDR cfghdr2_2;
  unsigned int NATICQ_PORT;

  CFG_HDR cfghdr2_3;
  unsigned int RECONNECT_TIME;
  
  CFG_HDR cfghdr2_4;
  char HIST_PATH[64];

  CFG_HDR cfghdr2_5;
  char SMILE_FILE[64];

  CFG_HDR cfghdr2_6;
  char SMILE_PATH[64];

  CFG_HDR cfghdr2_7;
  char ICON_PATH[64];

  CFG_HDR cfghdr2_8;
  char XSTATUSES_PATH[64];

  CFG_HDR cfghdr2_9;
  char TEMPLATES_PATH[64];
CFG_HDR cfghdr3;//NETWORK SETTING


//------------------  INTERFACE ------------------//

CFG_HDR cfghdr4;
//Status icon on mainscreen position
  CFG_HDR cfghdr4_1;
  unsigned int IDLEICON_X;
  unsigned int IDLEICON_Y;
  
//Popup chat
  CFG_HDR cfghdr4_2;
  int DEVELOP_IF;
  CFG_CBOX_ITEM cfgcbox4_2[4];
  
//Sorting
  CFG_HDR cfghdr4_3;
  int SORT_CLIST;
  CFG_CBOX_ITEM cfgcbox4_3[2];

//First letter when typing
  CFG_HDR cfghdr4_4;
  int FIRST_LETTER;
  CFG_CBOX_ITEM cfgcbox4_4[2];


//------------------ FONT AND COLOR ------------------//

  
 CFG_HDR cfghdr4_5;
    CFG_HDR cfghdr4_5_1;
    unsigned int I_COLOR;
  
    CFG_HDR cfghdr4_5_2;
    unsigned int TO_COLOR;
  
    CFG_HDR cfghdr4_5_3;
    unsigned int X_COLOR;
  
    CFG_HDR cfghdr4_5_4;
    unsigned int ED_FONT_SIZE;
  
    CFG_HDR cfghdr4_5_5;
    unsigned int ED_H_FONT_SIZE;
  
    CFG_HDR cfghdr4_5_6;
    unsigned int ED_X_FONT_SIZE;
  
    CFG_HDR cfghdr4_5_7;
    unsigned int O_I_COLOR;
  
    CFG_HDR cfghdr4_5_8;
    unsigned int O_TO_COLOR;
    
    CFG_HDR cfghdr4_5_9;
    unsigned int O_X_COLOR;
  
    CFG_HDR cfghdr4_5_10;
    unsigned int O_ED_FONT_SIZE;
  
    CFG_HDR cfghdr4_5_11;
    unsigned int O_ED_H_FONT_SIZE;
  
    CFG_HDR cfghdr4_5_12;
    unsigned int O_ED_X_FONT_SIZE;
  
    CFG_HDR cfghdr4_5_13;
    unsigned int ACK_COLOR;
  
    CFG_HDR cfghdr4_5_14;
    unsigned int UNACK_COLOR;
        
//Color for background in select smail
  CFG_HDR cfghdr19_1;
  unsigned int color_choose;
  
  CFG_HDR cfghdr4_6;//FONT & COLOR  

//------------------ ILLUMINATION ------------------//
//Illumination by BoBa 19.04.2007
  CFG_HDR cfghdr4_7;
    CFG_HDR cfghdr4_7_1;
    unsigned int ILL_DISP_RECV;
  
    CFG_HDR cfghdr4_7_2;
    unsigned int ILL_KEYS_RECV;
  
    CFG_HDR cfghdr4_7_3;
    unsigned int ILL_RECV_TMR;
  
    CFG_HDR cfghdr4_7_4;
    unsigned int ILL_RECV_FADE;
  
    CFG_HDR cfghdr4_7_5;
    unsigned int ILL_DISP_SEND;
  
    CFG_HDR cfghdr4_7_6;
    unsigned int ILL_KEYS_SEND;
  
    CFG_HDR cfghdr4_7_7;
    unsigned int ILL_SEND_TMR;
  
    CFG_HDR cfghdr4_7_8;
    unsigned int ILL_SEND_FADE;
  
    CFG_HDR cfghdr4_7_9;
    unsigned int ILL_OFF_FADE;
  CFG_HDR cfghdr4_8;  //ILLUMINATION
 
CFG_HDR cfghdr5;//INTERFACE


//------------------------------------------------------------------------------//
//                                  HISTORY
//------------------------------------------------------------------------------//

//History
CFG_HDR cfghdr6;

    CFG_HDR cfghdr6_8;
    int SHOW_ACTIVE;
    CFG_CBOX_ITEM cfgcbox6_8[2];
//Enable history logging
    CFG_HDR cfghdr6_1;
    int LOG_ALL;
    CFG_CBOX_ITEM cfgcbox6_1[2];

//History type
    CFG_HDR cfghdr6_2;
    int HISTORY_TYPE;
    CFG_CBOX_ITEM cfgcbox6_2[2];
  
//Enable status logging
    CFG_HDR cfghdr6_3;
    int LOG_STATCH;
    CFG_CBOX_ITEM cfgcbox6_3[2];

//Enable X-status logging
    CFG_HDR cfghdr6_4;
    int LOG_XTXT;
    CFG_CBOX_ITEM cfgcbox6_4[2];
  
//Auto request X-status if entering to chat
    CFG_HDR cfghdr6_5;
    int ENA_AUTO_XTXT;
    CFG_CBOX_ITEM cfgcbox6_5[2];

//Don't log X-status, if it same as prevision
    CFG_HDR cfghdr6_6;
    int NOT_LOG_SAME_XTXT;
    CFG_CBOX_ITEM cfgcbox6_6[2];

//Buffer for fill from history
    CFG_HDR cfghdr6_7;
    int HISTORY_BUFFER;
    CFG_CBOX_ITEM cfgcbox6_7[7];
  
CFG_HDR cfghdr7;//HISTORY

//--------------------------------------------------//
//                   NOTYFY POWER                   //
//--------------------------------------------------//

CFG_HDR cfghdr8;

  CFG_HDR cfghdr8_1;
  unsigned int sndVolume;

  CFG_HDR cfghdr8_2;
  unsigned int vibraPower;

  CFG_HDR cfghdr8_3;
  int VIBR_TYPE;
  CFG_CBOX_ITEM cfgcbox8_3[2];

  CFG_HDR cfghdr8_4;
  int VIBR_ON_CONNECT;
  CFG_CBOX_ITEM cfgcbox8_4[2];

//------------------ SOUND SETUP ------------------//
  CFG_HDR cfghdr8_5;
  
    CFG_HDR cfghdr181;
    unsigned int extended_sounds;
    CFG_CBOX_ITEM cfgcbox18_3_2[2];
    
    CFG_HDR cfghdr8_5_1;
    char sndStartup[64];
  
    CFG_HDR cfghdr8_5_2;
    char sndSrvMsg[64];
  
    CFG_HDR cfghdr8_5_3;
    char sndGlobal[64];
  
    CFG_HDR cfghdr8_5_4;
    char sndMsg[64];
  
    CFG_HDR cfghdr8_5_5;
    char sndMsgSent[64];
    
    CFG_HDR cfghdr15;
    char sndGlobal2[64];
  
    CFG_HDR cfghdr161;
    char sndMsg2[64];
    
    CFG_HDR cfghdr16r1;
    char pingsnd[64];
  CFG_HDR cfghdr8_6;//SOUND SETUP
  
CFG_HDR cfghdr9;//NOTYFY POWER
}myConfig;

//----------------------------------------------------------------//
//------------------------  MOD CONFIG  --------------------------//
//----------------------------------------------------------------//

typedef struct
{
  CFG_HDR cfghdr0;
//----------- НАСТРОЙКА КЛАВИШ -----------//
//View settings
  CFG_HDR cfghdr23_4;
/*
  CFG_HDR cfghdr23_4_1;//={CFG_CBOX,"Показ. сообщ.при дл.нажатии",0,2};
  int isShowMsgLongPress;// = 1;
  CFG_CBOX_ITEM cfgcbox23_4_2[2];//={"Нет","Да"};
  */
//Long press right in chat
  CFG_HDR cfghdr4_67;
  int LongRightAction;
  CFG_CBOX_ITEM cfgcbox4_56[3];

//Long press right in chat
  CFG_HDR cfghdr34_64;
  int LongLeftAction ;
  CFG_CBOX_ITEM cfgcbox444_56[2];
  
//Action on # button in contact list
  CFG_HDR cfghdr78_4520;
  int ActionResh;
  CFG_CBOX_ITEM cfgcbox_668_z[2];
  
#ifdef ELKA
//Press vol up or vol down button in CL
  CFG_HDR cfghdr34_642;
  int isVolDownButton;
  CFG_CBOX_ITEM cfgcbox434_56[2];
  
  CFG_HDR cfghdr34_6342;
  int isVolUpButton;
  CFG_CBOX_ITEM cfgcbox4134_56[2];
#endif
CFG_HDR cfghdr223_5; //НАСТРОКА КЛАВИШ
	
	
//----------- STATUS SETTING -----------//
  

CFG_HDR cfghdr_45;
#ifdef ELKA
//Status icon on mainscreen position
    CFG_HDR cfghdr467_1;
    int ShowStatusIcon;
    CFG_CBOX_ITEM cfgcbox9_1[5];
#endif
#ifdef ELKA
//Show x-status on iconbar or mainscreen
    CFG_HDR cfghdrz_68;
    int show_xstatus;
    CFG_CBOX_ITEM cfgcbox_8z[4];
#else
    CFG_HDR cfghdrz_68;
    int show_xstatus;
    CFG_CBOX_ITEM cfgcbox_8z[2];
#endif
//Xstatus idle position
    CFG_HDR cfghdrz_1;
    unsigned int IDLEICON_XX;
    unsigned int IDLEICON_YX;

#ifdef ELKA
//Number first pic status in folder img for show in iconbar
    CFG_HDR cfghdr19_22;
    unsigned int NumStatusToIconBar;  

//Number first pic x-status in folder img for show in iconbar    
    CFG_HDR cfghdr19_242;
    unsigned int NumXStatusToIconBar;  
#endif    
CFG_HDR cfghdr_4;//STATUS SETTINGS
  
  
//----------- PNGFONT SETTING -----------//     
  
CFG_HDR cfghdr1_45;
   
//Nember png-font or system
   CFG_HDR cfghdr65z;
   unsigned int fonto;
    
//Path to png-font
   CFG_HDR cfghdr98;
   char fontpath[64];

//Coordinates for message on idle
   CFG_HDR cfghd83;
   unsigned int msg_d_x ;
   unsigned int msg_d_y ;   
   
  CFG_HDR cfghdr60_2;
  char fontColor[4];
  
  CFG_HDR cfghdr60_3;
  char bgrColor[4];

//Max show symbol on idle
  CFG_HDR cfghdr19a_1;
  unsigned int main_lenght;
      
   CFG_HDR cfghdr9z9;
   int use_cutter;
   CFG_CBOX_ITEM cfgcbox18a_4[2];

   CFG_HDR cfghdr65y;
   unsigned int spaco1;
      
   //__root const CFG_HDR cfghdr65u={CFG_UINT,"Main msg space",0,999999};
   //__root const unsigned int spaco2=0;
    
CFG_HDR cfghdrss_45;//PNGFONT SETTING
  

 //-----------------------------------------------------------------------------//
//                             ONLINE TIME
//-----------------------------------------------------------------------------//

CFG_HDR cfghdr_m71;

    CFG_HDR cfghdr699;
    unsigned int onlineTime_x;
    unsigned int onlineTime_y;

    CFG_HDR cfghdr698;
    int Pause_Time;
    CFG_CBOX_ITEM cfgcbox85[2];

CFG_HDR cfghdr_m70;


//-----------------------------------------------------------------------------//
//                              AUTO PING/UNREAD
//-----------------------------------------------------------------------------//


CFG_HDR cfghdr_m91;

    CFG_HDR cfghd66;
    unsigned int Ping_X ;
    unsigned int Ping_Y ;
    
    CFG_HDR cfghdr1z9a_1;
    unsigned int ping_time_config ;
    
    CFG_HDR cfghdr298;
    int Is_Unread_On;
    CFG_CBOX_ITEM cfgcbox86[2];
    
    CFG_HDR cfghd67;
    unsigned int unread_X;
    unsigned int unread_Y;

CFG_HDR cfghdr_m90;


//-----------------------------------------------------------------------------//
//                                AUTO STATUS                                  //
//-----------------------------------------------------------------------------//


CFG_HDR cfghdr_m61;

    CFG_HDR cfghdr668;
    int Auto_Status;
    CFG_CBOX_ITEM cfgcbox89[2];
    
    CFG_HDR cfghdr667;
    unsigned int AUTO_ENGADE;
    
    CFG_HDR cfghdr_13;
    int time_zone;
    CFG_CBOX_ITEM cfgcbox_13[33];
    
    CFG_HDR cfghdrzz_8;
    char awayHelloMsg[24];
    
    CFG_HDR cfghdrzz_9;
    char awayMsg[64];
    
    CFG_HDR cfghdr66_2;
    int Reset_timer;
    CFG_CBOX_ITEM cfgcbox90[2];
  
//Show away time on idle
    CFG_HDR cfghdr663;
    int showAwayIdle ;
    CFG_CBOX_ITEM cfgcbox82[2];
    
    CFG_HDR cfghd6_7;
    unsigned int auto_x;
    unsigned int auto_y;
  
CFG_HDR cfghdr_m60;


//------------------------------------------------------------------------------//
//                                EXTRA CONFIG                                  //
//------------------------------------------------------------------------------//


CFG_HDR cfghdr_m51;

//Show contact header with...
    CFG_HDR cfghdr467;
    int TIME_OR_MSG;
    CFG_CBOX_ITEM cfgcbox91[5];
  
//Vibra count in vibra remaider
    CFG_HDR cfghdr65;
    unsigned int Vibra_cc;
    
//CSM for player
    CFG_HDR cfghdr66;
    char CsmAdr[9];  //el=A087B454 //es=A068E244
    
//Text before name track
    CFG_HDR cfghdr67;
    char playing[13];
  
//Show command sending    
    CFG_HDR cfghdr780;
    int show_send_info;
    CFG_CBOX_ITEM cfgcbox8_z[2];

//Show time offline contact
    CFG_HDR cfghdr7_80;
    int isLogOffline;
    CFG_CBOX_ITEM cfgcbox8df_z[2];
    
//Show time onlain contact
    CFG_HDR cfghdr7_81;
    int isLogOnline;
    CFG_CBOX_ITEM cfgcbox9df_z[2];
   
//Show Loopback contact in CL
    CFG_HDR cfghdr71_30;
    int IsShowLoopback;
    CFG_CBOX_ITEM cfgcbox_3z[2];


#ifdef kluchnik
//from me!!!)))
    CFG_HDR cfghdr71280;
    int autobot_kel;
    CFG_CBOX_ITEM cfgcbox8a_z[2];
    
    CFG_HDR cfghdr781;
    unsigned int AUTOBOT_UIN;
#endif    
CFG_HDR cfghdr_m50;//level

CFG_HDR cfghdr11_9;//level
char ignor_list[256];
}myConfigMod;



//-----------------------------------------------------------------------------------//
//                            ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ                                  //
//-----------------------------------------------------------------------------------//
unsigned int color_choose;
unsigned int Vibra_cc;
unsigned int Ping_X;
unsigned int Ping_Y;
unsigned int unread_X;
unsigned int unread_Y;
int show_xstatus;
char fontpath[64];
int ShowStatusIcon;

//For online/offline log
int isLogOffline;
int isLogOnline;

char CsmAdr[9];
int SORT_CLIST;
#ifdef ELKA
//If action vol up && vol down button on elka
int isVolUpButton;
int isVolDownButton;
#endif
  
//Autoaway message
char awayHelloMsg[24];//hello msg
char awayMsg[64];//message
  
int IsShowLoopback;//Show Loopback contact in CL
//For png font
unsigned int fonto;
unsigned int spaco1;
unsigned int spaco2;
//int spaco2=spaco1;
char fontColor[];//Цвет для системного шрифта
char bgrColor[];
  
unsigned int AUTO_ENGADE;
int Is_Unread_On;
int Pause_Time;  
unsigned int auto_x;
unsigned int auto_y;
int showAwayIdle;
  
//Online time on idle
unsigned int onlineTime_x;
unsigned int onlineTime_y;
  
int Auto_Status;//Включен автостатус или нет
int time_zone;
int TIME_OR_MSG;

unsigned int ping_time_config;
char ignor_list[256];
  
unsigned int msg_d_x;
unsigned int msg_d_y;
unsigned int main_lenght;
int use_cutter;
  
char playing[13];
int Reset_timer;  
unsigned int NumStatusToIconBar;
unsigned int NumXStatusToIconBar;

int LongRightAction;
int LongLeftAction;
  
#ifdef kluchnik
unsigned int AUTOBOT_UIN;
int autobot_kel;
#endif

//------------ ORIGINAL SETTING -------------//  

unsigned int UIN;
char PASS[9];

int VIBR_ON_CONNECT;
int SHOW_ACTIVE;
int VIBR_TYPE;
int DEVELOP_IF;

int LOG_ALL;
int LOG_STATCH;
int FIRST_LETTER;

int show_send_info;  
int ActionResh;

int HISTORY_TYPE;
unsigned int vibraPower;

unsigned int IDLEICON_X;
unsigned int IDLEICON_Y;
  
unsigned int IDLEICON_XX;
unsigned int IDLEICON_YX;
  
unsigned int I_COLOR;
unsigned int TO_COLOR;
unsigned int X_COLOR;
unsigned int O_I_COLOR;
unsigned int O_TO_COLOR;
unsigned int O_X_COLOR;
  
unsigned int ED_FONT_SIZE;
unsigned int ED_H_FONT_SIZE;
unsigned int ED_X_FONT_SIZE;
unsigned int O_ED_FONT_SIZE;
unsigned int O_ED_H_FONT_SIZE;
unsigned int O_ED_X_FONT_SIZE;
  
unsigned int ACK_COLOR;
unsigned int UNACK_COLOR;
  
int ENA_AUTO_XTXT;
int NOT_LOG_SAME_XTXT;
int LOG_XTXT;
int HISTORY_BUFFER;  
  
//Illumination by BoBa 19.04.2007
///////////
unsigned int ILL_DISP_RECV;
unsigned int ILL_KEYS_RECV;
unsigned int ILL_RECV_TMR;
unsigned int ILL_DISP_SEND;
unsigned int ILL_KEYS_SEND;
unsigned int ILL_SEND_TMR;
unsigned int ILL_SEND_FADE;
unsigned int ILL_RECV_FADE;
unsigned int ILL_OFF_FADE;

//=============================Проигрывание звука=======================

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

char sndStartup[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndStartup.wav";
char sndSrvMsg[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndSrvMsg.wav";
char sndGlobal[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndGlobal.wav";
char sndGlobal2[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\Uins_on\\";
char sndMsg[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndMsg.wav";
char sndMsg2[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\Uins_msg\\";
char sndMsgSent[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndMsgSent.wav";
char pingsnd[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndPing.wav";
unsigned int extended_sounds;
unsigned int sndVolume;
  
//===============================================================
  
//int VIBR_TYPE;
//unsigned int vibraPower;
     
char SMILE_FILE[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\smiles.cfg";
char TEMPLATES_PATH[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\templates";
char ICON_PATH[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\img";
char SMILE_PATH[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\smiles";
char XSTATUSES_PATH[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\ximg";
char HIST_PATH[64];//=DEFAULT_DISK ":\\ZBin\\NatICQ\\history";
char NATICQ_HOST[128];//="cbsie.dyndns.info; freegw.alltime.ru";
unsigned int NATICQ_PORT;//=5050;
unsigned int RECONNECT_TIME; //2058

//-----------------------------------------------------------------------------------//
// --------------------------------  INIT CONFIGS -----------------------------------//
//-----------------------------------------------------------------------------------//

extern int janis;
char per_s[]="%s";
void UpdateConfig()
{
 
  int myConfigSize = sizeof(myConfig);
  myConfig* myConf = (myConfig*)malloc(myConfigSize);
  int newCfgFile = InitConfig(myConf, myConfigSize, "NATICQ",4);
    
  int myConfigModSize = sizeof(myConfigMod);
  myConfigMod* myConfMod = (myConfigMod*)malloc(myConfigModSize);
  int newCfgModFile = InitConfig(myConfMod, myConfigModSize, "NATICQMod",4);

//---------------------------
//         MOD CONFIG
//---------------------------
if(newCfgFile!=-1 && newCfgModFile!=-1)//Если конфиги есть
{
  Vibra_cc=myConfMod->Vibra_cc;
  Ping_X=myConfMod->Ping_X;
  Ping_Y=myConfMod->Ping_Y;
  unread_X=myConfMod->unread_X;
  unread_Y=myConfMod->unread_Y;
  show_xstatus=myConfMod->show_xstatus;
  sprintf(fontpath,per_s,myConfMod->fontpath);
//  ShowStatusIcon=myConfMod->ShowStatusIcon;
  AUTO_ENGADE=myConfMod->AUTO_ENGADE;
  
//For online/offline log
  isLogOffline=myConfMod->isLogOffline;
  isLogOnline=myConfMod->isLogOnline;
  
#ifdef ELKA
//If action vol up && vol down button on elka
  isVolUpButton=myConfMod->isVolUpButton;
  isVolDownButton=myConfMod->isVolDownButton;
#endif
  sprintf(CsmAdr,per_s,myConfMod->CsmAdr);
//Autoaway message
  sprintf(awayHelloMsg,per_s,myConfMod->awayHelloMsg);//hello msg
  sprintf(awayMsg,per_s,myConfMod->awayMsg);//message
  
  IsShowLoopback=myConfMod->IsShowLoopback;//Show Loopback contact in CL
//  isShowMsgLongPress=myConfMod->isShowMsgLongPress;
//For png font
  fonto=myConfMod->fonto;
  spaco1=myConfMod->spaco1;
  spaco2=myConfMod->spaco1;//так нада
  //Цвет для системного шрифта
  for(int i=0;i<4;i++)
  {
    fontColor[i]=myConfMod->fontColor[i];
    bgrColor[i]=myConfMod->bgrColor[i];
  }
  
  AUTO_ENGADE=myConfMod->AUTO_ENGADE;
  Is_Unread_On=myConfMod->Is_Unread_On;
  Pause_Time=myConfMod->Pause_Time;
  
  auto_x=myConfMod->auto_x;
  auto_y=myConfMod->auto_y;
  showAwayIdle=myConfMod->showAwayIdle;
  
//Online time on idle
  onlineTime_x=myConfMod->onlineTime_x;
  onlineTime_y=myConfMod->onlineTime_y;
  
  Auto_Status=myConfMod->Auto_Status;//Включен автостатус или нет
  time_zone=myConfMod->time_zone;
  
  TIME_OR_MSG=myConfMod->TIME_OR_MSG;
  
  sprintf(pingsnd,per_s,myConf->pingsnd);
  ping_time_config=myConfMod->ping_time_config;
  sprintf(ignor_list,per_s,myConfMod->ignor_list);
  
  msg_d_x=myConfMod->msg_d_x;
  msg_d_y=myConfMod->msg_d_y;
  main_lenght=myConfMod->main_lenght;
  use_cutter=myConfMod->use_cutter;
  
  sprintf(playing,per_s,myConfMod->playing);
  Reset_timer=myConfMod->Reset_timer;
#ifdef ELKA
  NumStatusToIconBar=myConfMod->NumStatusToIconBar;
  NumXStatusToIconBar=myConfMod->NumXStatusToIconBar;
  ShowStatusIcon=myConfMod->ShowStatusIcon;
#endif
  LongRightAction=myConfMod->LongRightAction;
  LongLeftAction=myConfMod->LongLeftAction;
  
//Позиция хстатус на гэ
  IDLEICON_XX=myConfMod->IDLEICON_XX;
  IDLEICON_YX=myConfMod->IDLEICON_YX;
  
  show_send_info=myConfMod->show_send_info;
  ActionResh=myConfMod->ActionResh;//действие на # в контакт листе

#ifdef kluchnik
  AUTOBOT_UIN=myConfMod->AUTOBOT_UIN;
  autobot_kel=myConfMod->autobot_kel;
#endif
 
  
//--------------------------------
//        ORIGINAL CONFIG
//--------------------------------
  UIN=myConf->UIN;
  sprintf(PASS,per_s,myConf->PASS);
  
  SORT_CLIST=myConf->SORT_CLIST;
  LOG_ALL=myConf->LOG_ALL;
  LOG_STATCH=myConf->LOG_STATCH;
  SHOW_ACTIVE=myConf->SHOW_ACTIVE;
  VIBR_TYPE=myConf->VIBR_TYPE;
  VIBR_ON_CONNECT=myConf->VIBR_ON_CONNECT;
  DEVELOP_IF=myConf->DEVELOP_IF;
  FIRST_LETTER=myConf->FIRST_LETTER;
    
  
  HISTORY_TYPE=myConf->HISTORY_TYPE;
  
  color_choose=myConf->color_choose;
  
  sprintf(HIST_PATH,per_s,myConf->HIST_PATH);
  sprintf(NATICQ_HOST,per_s,myConf->NATICQ_HOST);
  NATICQ_PORT=myConf->NATICQ_PORT;
  RECONNECT_TIME=myConf->RECONNECT_TIME; //2058
  
  vibraPower=myConf->vibraPower;  
  
  IDLEICON_X=myConf->IDLEICON_X;
  IDLEICON_Y=myConf->IDLEICON_Y;
  
  I_COLOR=myConf->I_COLOR;
  TO_COLOR=myConf->TO_COLOR;
  X_COLOR=myConf->X_COLOR;
  O_I_COLOR=myConf->O_I_COLOR;
  O_TO_COLOR=myConf->O_TO_COLOR;
  O_X_COLOR=myConf->O_X_COLOR;
  
  ED_FONT_SIZE=myConf->ED_FONT_SIZE;
  ED_H_FONT_SIZE=myConf->ED_H_FONT_SIZE;
  ED_X_FONT_SIZE=myConf->ED_X_FONT_SIZE;
  O_ED_FONT_SIZE=myConf->O_ED_FONT_SIZE;
  O_ED_H_FONT_SIZE=myConf->O_ED_H_FONT_SIZE;
  O_ED_X_FONT_SIZE=myConf->O_ED_X_FONT_SIZE;
  
  ACK_COLOR=myConf->ACK_COLOR;
  UNACK_COLOR=myConf->UNACK_COLOR;
  
  ENA_AUTO_XTXT=myConf->ENA_AUTO_XTXT;
  NOT_LOG_SAME_XTXT=myConf->NOT_LOG_SAME_XTXT;
  LOG_XTXT=myConf->LOG_XTXT;
  
  HISTORY_BUFFER=myConf->HISTORY_BUFFER;  
  
//Illumination by BoBa 19.04.2007
///////////
  ILL_DISP_RECV=myConf->ILL_DISP_RECV;
  ILL_KEYS_RECV=myConf->ILL_KEYS_RECV;
  ILL_DISP_SEND=myConf->ILL_DISP_SEND;
  ILL_KEYS_SEND=myConf->ILL_KEYS_SEND;
  ILL_SEND_TMR=myConf->ILL_SEND_TMR;
  ILL_SEND_FADE=myConf->ILL_SEND_FADE;
  ILL_RECV_TMR=myConf->ILL_RECV_TMR;
  ILL_RECV_FADE=myConf->ILL_RECV_FADE;
  ILL_OFF_FADE=myConf->ILL_OFF_FADE;
  
//=============================Проигрывание звука=======================
  
  sprintf(sndStartup,per_s,myConf->sndStartup);
  sprintf(sndSrvMsg,per_s,myConf->sndSrvMsg);
  sprintf(sndGlobal,per_s,myConf->sndGlobal);
  sprintf(sndGlobal2,per_s,myConf->sndGlobal2);
  sprintf(sndMsg,per_s,myConf->sndMsg);
  sprintf(sndMsg2,per_s,myConf->sndMsg2);
  extended_sounds=myConf->extended_sounds;
  sprintf(sndMsgSent,per_s,myConf->sndMsgSent);
  sndVolume=myConf->sndVolume; 

//-----------------------------------------
  
  vibraPower=myConf->vibraPower;
     
  sprintf(TEMPLATES_PATH,per_s,myConf->TEMPLATES_PATH);
  sprintf(ICON_PATH,per_s,myConf->ICON_PATH);
  sprintf(SMILE_FILE,per_s,myConf->SMILE_FILE);
  sprintf(SMILE_PATH,per_s,myConf->SMILE_PATH);
  sprintf(XSTATUSES_PATH,per_s,myConf->XSTATUSES_PATH);
  
  
//-----------------------------------------
  
  mfree(myConf);
  mfree(myConfMod);
  ShowMSG(1,(int)"ok");
}
else
  {
     LockSched();
     ShowMSG(1,(int)"Can't open configs!");
     UnlockSched();
  }
}
int main()
{
  UpdateConfig();
  return 0;
}
