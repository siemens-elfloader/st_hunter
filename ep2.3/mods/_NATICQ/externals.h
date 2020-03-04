#ifndef _EXTERNALS_H_
  #define _EXTERNALS_H_

#ifdef GUI_INTERFACE
//-------------- INTERFACE ----------------//

extern const int LINE_MODE;
extern const int VIEW_MODE;
extern const unsigned int SOFT_MODE;


extern const int BlinkIconByOnline;
   
extern const unsigned int ShowProcessSend;
extern const int SelBoldActiveChat;
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
extern const char CLDescriptClr[4];

extern const char UnAckedClr[4];
extern const char SendClr[4];
   
   
extern const char AddInfoClr[4];
extern const char FontInfoClr[4];

extern const unsigned int ClFont;
extern const unsigned int ClBoldFont;
extern const unsigned int HeadFont;
extern const unsigned int MMFont;
//------------ CL HEADER SETTING -------------//

extern const unsigned int ON_OFF_TIME;
extern const int HEAD_STATUS;
extern const int HEAD_XSTATUS;
extern const int HEAD_PRIVATE;
extern const int HEAD_PING;
extern const int isShowCont;
extern const int isShowUnread;
extern const int isShowI;
extern const int isShowV;
extern const int isShowS;
extern const int isShowBat;
extern const int isShowA;
extern const int isShowVr;


extern const int ShowXStatusSecLine;
extern const char MMFontClr[4];
extern const char MMBackgrClr[4];
extern const char MMBackgrBordClr[4];
extern const char MMCursorBGClr[4];
extern const char MMCursorClr[4]; 
extern const unsigned int MainMenuAutoRect;     
extern const unsigned int MainMenuItems;
extern const RECT MMRect;
extern const RECT SRect;
extern const RECT XSRect;


extern void DrawSoftKeys(char *right,char *left);
extern void DrawContactList();
extern void DrawHeader();
extern IMGHDR DoScreen(int ScrH,int HSIZE);
extern unsigned int HeaderY;


//colors
//extern const char ActiveClr[4]={0xFF,0xFF,0x00,0x64};
//extern const char NotActiveClr[4]={0x00,0xFF,0x00,0x64};         
extern const char CLColorSelCont[];
extern const int isIndividualColor;
extern const char CLColorsOnline[];         
extern const char CLColorsAway[];         
extern const char CLColorsNA[];
extern const char CLColorsOccupied[];
extern const char CLColorsDND[];
extern const char CLColorsDepression[];
extern const char CLColorsFFC[];
extern const char CLColorsOff[];
#endif


extern const int isLogStatusChange;

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


//---------------- AUTO STATUS ---------------//

extern const int Auto_Status;//Включен автостатус или нет 
extern const int IsOnNAStatus;
extern const unsigned int NAStatusTime;   
extern const int isAutoStatusOn;// = 0;
extern const unsigned int AUTO_ENGADE;
extern const unsigned int automsgcount;
extern const int time_zone;
extern const char awayHelloMsg[24];//hello msg
extern const char awayMsg[256];//message
extern const int showAwayIdle;  
extern const unsigned int auto_x;
extern const unsigned int auto_y;
extern const int Reset_timer;  



//------------ popup message -----------------//


extern const int PopupShow;
extern const char PopupMsgString[9];
extern const char PopupOnlineString[9];
extern const unsigned int PopupFont;
extern const char PopupBGCl[4];
extern const char PopupOnlineCl[4];    
extern const char PopupMsgCl[4];

extern const char PopupOffBGCl[4];
extern const char PopupOffCl[4];
extern const int isPopupOff;
//------------------------------------------


extern const char pingsnd[64];
extern const char sndLostConn[64];    

extern char clrWhite[];
extern char clrRed[];
extern char transparent[];
extern char blue[];

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


#define RET_X 1
#define RET_Y 0
extern int DrwImage(IMGHDR *img, int x, int y,int rettype);

extern IMGHDR *GetIconByItem(int curitem);
extern IMGHDR *GetXIconByItem(int curitem);

extern int FontH(int font);
extern void DrawStr(char *str, int x, int y, int x2,int y2, int align, int font, char *color,int needcut);

extern int get_file_size(char* fname);
#endif

