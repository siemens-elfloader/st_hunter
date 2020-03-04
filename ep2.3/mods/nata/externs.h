#ifndef _CONFIG_H_
  #define _CONFIG_H_


extern const int SHOW_ACTIVE;
extern const char pingsnd[64];
#ifdef ELKA
extern const char ICON_PATH2[64];
#endif
extern const char sndLostConn[64];

//-------------- INTERFACE ----------------//

extern const int LINE_MODE;
extern const int VIEW_MODE;
extern const unsigned int SOFT_MODE;
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

//------------ smiles color -----------------//
extern const int UseStandartSmileSel;
extern const char SmBGcolor[4];
extern const char SmSelectColor[4];
extern const char SmFontColor[4];

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
extern const int isShowBat;


extern const int isLogStatusChange;
extern const int IsOpenBrowser;
extern const int IsMultiBuffer;


//-------------  LOAD SCREEN SET ------------//

extern const RECT PBRECT;
extern const char PBcolor[4];
extern const char BGPBcolor[4];
extern const int IsInvertSoftKey;
extern const int IsShowInfo;

//------------------- INFA -----------------//

extern const char BGInfo[4];
extern const char FontClrInfo[4];


extern const char I_str[32];


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


extern IMGHDR *XStatusImg[];
extern IMGHDR *icons[];

//main menu
extern int CurrentStatus;
extern int CurrentXStatus;
extern DYNPNGICONLIST *XStatusesImgList;
extern int *XStatusesIconArray;
extern  int S_ICONS[];
extern const SOFTKEYSTAB menu_skt;

extern int Is_Vibra_Enabled;
extern unsigned int Is_Sounds_Enabled; 
extern int Is_Show_Offline;
extern int Is_Show_Groups;
extern int IsActiveUp;
extern int Is_POPUP_On;
extern int Is_SLI_On;
extern int Is_Double_Vibra_On;
extern int Is_ILU_On;


extern int msg_away_timer;
extern void away_timer();
extern int away_m;//состояние автоответчика
extern int change_back_status;
extern int status_rem;
extern GBSTMR away_tmr;

extern const char TEMPLATES_PATH[64];

extern IMGHDR DoScreen(int ScrH,int HSIZE);

extern char clrBlack[];
extern char clrWhite[];
extern char clrRed[];
extern char transparent[];
extern char blue[];
extern int clfont;

extern void DrawSoftKeys(char *right,char *left);
extern void DrawContactList();
extern int hy;


#endif

