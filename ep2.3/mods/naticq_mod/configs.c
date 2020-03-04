#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "configs.h"
#include "NatICQ.h"
#include "conf_loader.h"

//Теперь конфиги делаются отдельно!!!!как в экстраинфо! прикладываю два проекта 1stconfig && 2ndconfig
//должны находится в папке с исходником натаси в своих папках!чтобы конфиг можно было удоьно редактировать
//если изменили какой то из конфигов,то так же нужно изменить структуры в таком же порядке,
//как зменили конфиг с темиже данными!и поправить функцию UpdateConfig(),и потом скомпилить отдельно эльфы 1stconfig && 2ndconfig,
//чтобы получить разные конфиги...по другому пока не знаю как сделать...
//та и короче...теперь меняется тока конфиг мода...оригинальный конфиг не трогать!
//понятнее хз как объяснить....;)
// by kluchnik 31.03.08 


//--------------------------------------------------------------------//
//------------------------  ORIGINAL CONFIG  -------------------------//
//--------------------------------------------------------------------//

typedef struct 
{
  
  CFG_HDR cfghdr0;
  unsigned int UIN;
  
  CFG_HDR cfghdr1;
  char PASS[9];

  
//--- NETWORK SETTING ---//
  
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


//---  INTERFACE ---//

CFG_HDR cfghdr4;
//Status icon on mainscreen position
  CFG_HDR cfghdr4_1;
  unsigned int IDLEICON_X;
  unsigned int IDLEICON_Y;
  
//Popup chat
  CFG_HDR cfghdr4_2;
  int DEVELOP_IF;
  CFG_CBOX_ITEM cfgcbox4_2[3];
  
//Sorting
  CFG_HDR cfghdr4_3;
  int SORT_CLIST;
  CFG_CBOX_ITEM cfgcbox4_3[2];

//First letter when typing
  CFG_HDR cfghdr4_4;
  int FIRST_LETTER;
  CFG_CBOX_ITEM cfgcbox4_4[2];


  
//--- FONT AND COLOR ---//
  
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
  CFG_HDR cfghdr4_6;//FONT & COLOR  

  
//--- ILLUMINATION ---//
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



//---------------------//
//       HISTORY       //
//---------------------//

//History
CFG_HDR cfghdr6;

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
    CFG_CBOX_ITEM cfgcbox6_7[6];
  
CFG_HDR cfghdr7;//HISTORY


//---------------------//
//    NOTYFY POWER     //
//---------------------//

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

  
//--- SOUND SETUP ---//
  
  CFG_HDR cfghdr8_5;

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
  CFG_HDR cfghdr8_6;//SOUND SETUP
  
CFG_HDR cfghdr9;//NOTYFY POWER
}myConfig;




//----------------------------------------------//
//                  MOD CONFIG                  //
//----------------------------------------------//

typedef struct
{
  CFG_HDR cfghdr0;
  CFG_HDR cfghdr31;//={CFG_LEVEL,"Оригинальные настройки",1,0};

//Popup chat
  CFG_HDR cfghdr4_2;//={CFG_CBOX,"Popup",0,2};
  int DEVELOP_IF_MOD;
  CFG_CBOX_ITEM cfgcbox15_1[2];//={"Use set orig.config", "After unlock"};
  
//history
  CFG_HDR cfghdr6_8;//"Show Active contact"
  int SHOW_ACTIVE;
  CFG_CBOX_ITEM cfgcbox6_8[2];//={"No","Yes"};

  CFG_HDR cfghdr6_10;//={CFG_CBOX,"Save history with name",0,2};
  int HISTORY_SAVE_TYPE;// = 1;
  CFG_CBOX_ITEM cfgcbox6_9[2];//={"No","Yes"};
    
//Buffer for fill from history
  CFG_HDR cfghdr6_7;//={CFG_CBOX,"History read buffer",0,2};
  int HISTORY_BUFFER_MOD;
  CFG_CBOX_ITEM cfgcbox6_7[2];//={"Use set orig.conf.","4 kbyte"};    
    

  CFG_HDR cfghdr181;//={CFG_CBOX,"Extended sounds",0,2};
  unsigned int extended_sounds;
  CFG_CBOX_ITEM cfgcbox18_3_2[2];//={"No","Yes"};
    
  CFG_HDR cfghdr15;//={CFG_STR_UTF8,"add online msg path",0,63};
  char sndGlobal2[64];
  
  CFG_HDR cfghdr161;//={CFG_STR_UTF8,"add new msg path",0,63};
  char sndMsg2[64];
    
  CFG_HDR cfghdr16r1;//={CFG_STR_UTF8,"ping sound",0,63};
  char pingsnd[64];
    
CFG_HDR cfghdr30;


//----------- НАСТРОЙКА КЛАВИШ -----------//


//View settings
  CFG_HDR cfghdr23_4;
  
//Action on # button in contact list
  CFG_HDR cfghdr78_4520;
  int ActionResh;
  CFG_CBOX_ITEM cfgcbox_668_z[2];
  

//Press vol up or vol down button in CL
  CFG_HDR cfghdr34_642;
  int isVolDownButton;
  CFG_CBOX_ITEM cfgcbox434_56[2];
  
  CFG_HDR cfghdr34_6342;
  int isVolUpButton;
  CFG_CBOX_ITEM cfgcbox4134_56[2];
  
  CFG_HDR cfghdr99_1;  
  
  //Long press right in chat
  CFG_HDR cfghdr4_67;
  int LongRightAction;
  CFG_CBOX_ITEM cfgcbox4_56[3];

//Long press right in chat
  CFG_HDR cfghdr34_64;
  int LongLeftAction ;
  CFG_CBOX_ITEM cfgcbox444_56[2];
  

  CFG_HDR cfghdr23_1;
  int LongVolUpAction;
  CFG_CBOX_ITEM cfgcbox23_2[2];

  CFG_HDR cfghdr23_3;
  int LongVolDownAction ;
  CFG_CBOX_ITEM cfgcbox23_4[2];

  CFG_HDR cfghdr99_3_0;
  int longAct_0;
  CFG_CBOX_ITEM cfgcbox99_4_0[14];

  CFG_HDR cfghdr99_3_1;
  int longAct_1;
  CFG_CBOX_ITEM cfgcbox99_4_1[14];
  
  CFG_HDR cfghdr99_3_2;
  int longAct_2;
  CFG_CBOX_ITEM cfgcbox99_4_2[14];
      
  CFG_HDR cfghdr99_3_3;
  int longAct_3;
  CFG_CBOX_ITEM cfgcbox99_4_3[14];
      
  CFG_HDR cfghdr99_3_4;
  int longAct_4;
  CFG_CBOX_ITEM cfgcbox99_4_4[14];
  
  CFG_HDR cfghdr99_3_5;
  int longAct_5;
  CFG_CBOX_ITEM cfgcbox99_4_5[14];
      
  CFG_HDR cfghdr99_3_6;
  int longAct_6;
  CFG_CBOX_ITEM cfgcbox99_4_6[14];
  
  CFG_HDR cfghdr99_3_7;
  int longAct_7;
  CFG_CBOX_ITEM cfgcbox99_4_7[14];
      
  CFG_HDR cfghdr99_3_8;
  int longAct_8;
  CFG_CBOX_ITEM cfgcbox99_4_8[14];
      
  CFG_HDR cfghdr99_3_9;
  int longAct_9;
  CFG_CBOX_ITEM cfgcbox99_4_9[14];
  
  CFG_HDR cfghdr99_3_10;
  int longAct_10;
  CFG_CBOX_ITEM cfgcbox99_4_10[14];
      
  CFG_HDR cfghdr99_3_11;
  int longAct_11;
  CFG_CBOX_ITEM cfgcbox99_4_11[14];
      
  CFG_HDR cfghdr99_3_RB;
  int longAct_RB;
  CFG_CBOX_ITEM cfgcbox99_4_RB[14];
  
  CFG_HDR cfghdr00_2;

CFG_HDR cfghdr223_5; //НАСТРОКА КЛАВИШ
	
	
//----------- STATUS SETTING -----------//
  

CFG_HDR cfghdr_45;
#ifdef NEWSGOLD
//Status icon on mainscreen position
    CFG_HDR cfghdr467_1;
    int ShowStatusIcon;
    CFG_CBOX_ITEM cfgcbox9_1[5];
#endif
#ifdef NEWSGOLD
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

    CFG_HDR cfghdr780_1;
    int ind_set_xstatus;
    CFG_CBOX_ITEM cfgcbox8_z2[2];
    
#ifdef NEWSGOLD
//Number first pic status in folder img for show in iconbar
    CFG_HDR cfghdr19_22;
    unsigned int NumStatusToIconBar;  

//Number first pic x-status in folder img for show in iconbar    
    CFG_HDR cfghdr19_242;
    unsigned int NumXStatusToIconBar;  

    CFG_HDR cfghdr_32_1;//={CFG_LEVEL,"Доп. статус в иконбар",1,0};
 
      CFG_HDR cfghdr32_2;//={CFG_UINT,"UIN",0,0xFFFFFFFF};
      unsigned int ADDITION_UIN;
      
      CFG_HDR cfghdr32_3;//={CFG_CBOX,"Показывать",0,2};
      int show_add_state;
      CFG_CBOX_ITEM cfgcbox32_4[2];

      CFG_HDR cfghdr32_5;//={CFG_CBOX,"Показывать хстатус",0,2};
      int show_add_xstate;
      CFG_CBOX_ITEM cfgcbox32_6[2];       

   CFG_HDR cfghdr_32_0;
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
   
   CFG_HDR cfghdrs19_1;
   unsigned int main_msg_timer;
   
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
      
CFG_HDR cfghdrss_45;//PNGFONT SETTING


//-------------------------------------------------------//
//                SMILES COLOR SETTING                   //
//-------------------------------------------------------//


CFG_HDR cfghdr41;//={CFG_LEVEL,"Цвета выбора смайлов",1,0};

 CFG_HDR cfghdr40_1;//={CFG_COLOR,"Цвет фона",0,0};
 char SmBGcolor[4];
  
 CFG_HDR cfghdr40_2;//={CFG_COLOR,"Цвет курсора",0,0};
 char SmSelectColor[4];
  
 CFG_HDR cfghdr40_3;//={CFG_COLOR,"Цвет текста стат.",0,0};
 char SmFontColor[4];

CFG_HDR cfghdr40;
  

//-------------------------------------------------------//
//                     IDLE SETTING                      //
//-------------------------------------------------------//

CFG_HDR cfghdr_m71;

    CFG_HDR cfghdr699;
    unsigned int onlineTime_x;
    unsigned int onlineTime_y;

    CFG_HDR cfghdr698;
    int Pause_Time;
    CFG_CBOX_ITEM cfgcbox85[2];


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
    
    CFG_HDR cfghdr71_3;// = {CFG_CBOX,"Показывать подключ.",0,2};
    int Is_Online_On;
    CFG_CBOX_ITEM cfgcbox71_2[2];
    
    CFG_HDR cfghd71_1;// = {CFG_COORDINATES,"Подключ.контакты",0,0};
    unsigned int online_x;
    unsigned int online_y;  

CFG_HDR cfghdr_m90;


//-------------------------------------------------------//
//                     AUTO STATUS                       //
//-------------------------------------------------------//


CFG_HDR cfghdr_m61;

    CFG_HDR cfghdr668;
    int Auto_Status;
    CFG_CBOX_ITEM cfgcbox89[2];
    
    
    CFG_HDR cfghdr_m61_1;// = {CFG_CBOX,"Вкл. недоступен",0,2};
    int IsOnNAStatus ;
    CFG_CBOX_ITEM cfgcbox_m61_2[2];//={"No","Yes"};

    CFG_HDR cfghdr61_8;//={CFG_UINT,"Время до недоступен(мин)",0,150};
    unsigned int NAStatusTime;    
    
    CFG_HDR cfghdr6678;// = {CFG_CBOX,"Включить автоматически при",0,2};
    int isAutoStatusOn;// = 0;
    CFG_CBOX_ITEM cfgcboxa1_153[13];//={"Выключен","В сети", "Отошел", "Недоступен", "Не беспокоить", "Занят", "Готов болтать", "Невидимый", "Депрессия", "Злой", "Дома", "Жру","На работе"};

    
    CFG_HDR cfghdr667;
    unsigned int AUTO_ENGADE;
    
    CFG_HDR cfghdr61_18;//={CFG_UINT,"Количество ответов",0,150};
    unsigned int automsgcount;//=3;
    
    CFG_HDR cfghdr_13;
    int time_zone;
    CFG_CBOX_ITEM cfgcbox_13[33];
    
    CFG_HDR cfghdrzz_8;
    char awayHelloMsg[24];
    
    CFG_HDR cfghdrzz_9;
    char awayMsg[256];
    
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



//-------------------------------------------------------//
//                   AUTO CHANGE STATUS                  //
//-------------------------------------------------------//


CFG_HDR cfghdra1;

    CFG_HDR cfghdra1_3;
    int status_1;
    CFG_CBOX_ITEM cfgcboxa1_13[13];

    CFG_HDR cfghdra1_2;
    TTime tm_status_1;     

    CFG_HDR cfghdra1_1;
    int status_2;
    CFG_CBOX_ITEM cfgcboxa1_14[13];

    CFG_HDR cfghdra1_4;
    TTime tm_status_2;

    CFG_HDR cfghdra1_5;
    int status_3;
    CFG_CBOX_ITEM cfgcboxa1_15[13];
    
    CFG_HDR cfghdra1_6;
    TTime tm_status_3;

    CFG_HDR cfghdra1_7;
    int status_4;
    CFG_CBOX_ITEM cfgcboxa1_16[13];
    
    CFG_HDR cfghdra1_8;
    TTime tm_status_4;

    CFG_HDR cfghdra1_9;
    int status_5 ;
    CFG_CBOX_ITEM cfgcboxa1_17[13];
    
    CFG_HDR cfghdra1_14;
    TTime tm_status_5;
    
CFG_HDR cfghdra0;

//-------------------------------------------------------//
//                    SCREEN SAVER                       //
//-------------------------------------------------------//
#ifdef NEWSGOLD

CFG_HDR cfghdr_91;//={CFG_LEVEL,"Заставка",1,0};

    CFG_HDR cfghdr9_1;// = {CFG_CBOX,"Включить заставку",0,2};
    int isScreenSaverEna;
    CFG_CBOX_ITEM cfgcbox9_2[2];
    
    CFG_HDR cfghdr9_3;//={CFG_UINT,"Время до срабатывания(12=1мин)",0,1000};
    unsigned int screenSaverTime;
CFG_HDR cfghdr_90;

#endif

//-------------------------------------------------------//
//                     POPUP MESSAGE                     //
//-------------------------------------------------------//

CFG_HDR cfghdr_51;//={CFG_LEVEL,"Всплывающие сообщ.",1,0};

   CFG_HDR cfghdr51_0;// = {CFG_CBOX,"Показывать",0,4};
   int PopupShow;
   CFG_CBOX_ITEM cfgcbox51_2[4];//={"Ничего","Вход.сообш.","Подкл.конт.","Все"};
    
   CFG_HDR cfghdr51_1;//={CFG_STR_UTF8,"Формат входящ.сооб.",0,8};
   char PopupMsgString[9];//="%s";
    
   CFG_HDR cfghdr51_2;//={CFG_STR_UTF8,"Формат подкл.контакта",0,8};
   char PopupOnlineString[9];//=">%s<";  
    
   CFG_HDR cfghdr51_3;//={CFG_UINT,"Шрифт всплыв.сообщ",0,1000};
   unsigned int PopupFont;
   
   CFG_HDR cfghdr51_6;//={CFG_COLOR,"Фон всплыв.сообщ.",0,0};
   char PopupBGCl[4];

   CFG_HDR cfghdr51_4;//={CFG_COLOR,"Цвет подкл.контакта",0,0};
   char PopupOnlineCl[4];
    
   CFG_HDR cfghdr51_5;//={CFG_COLOR,"Цвет входящ.сообщ.",0,0};
   char PopupMsgCl[4];
   
CFG_HDR cfghdr_50;//={CFG_LEVEL,"",0,0};



//-------------------------------------------------------//
//              SETTING CONTACTLIST HEADER               //
//-------------------------------------------------------//

CFG_HDR cfghdr_s1;//={CFG_LEVEL,"Настройка заголовка КЛ",1,0};

//Show contact header with...
    CFG_HDR cfghdrs1_3;// = {CFG_CBOX,"Показывать в заголовке КЛ",0,6};
    int TIME_OR_MSG;
    CFG_CBOX_ITEM cfgcboxs2[6];//={"Время","Сообщения","Контакты","Все","Пнг шрифт(все)","Ничего"};

    CFG_HDR cfghdrs1_s1;//={CFG_UINT,"Время подкл./отключ.(сек)",0,1000};
    unsigned int ON_OFF_TIME;
    
    
    CFG_HDR cfghdrs1_15;
    int ShowTraff;
    CFG_CBOX_ITEM cfgcboxs15[2];
    
    CFG_HDR cfghdrs1_11;//={CFG_CBOX,"Отображать кол-во контактов",0,2};
    int isShowCont;
    CFG_CBOX_ITEM cfgcboxs11[2];

    CFG_HDR cfghdrs1_12;//={CFG_CBOX,"Отображать кол-во непрочит.",0,2};
    int isShowUnread;
    CFG_CBOX_ITEM cfgcboxs12[2];

    CFG_HDR cfghdrs1_4;//={CFG_CBOX,"Отображать вибру [R] ",0,2};
    int isShowV ;
    CFG_CBOX_ITEM cfgcboxs3[2];
    
    CFG_HDR cfghdrs1_5;//={CFG_CBOX,"Отображать звук [S]",0,2};
    int isShowS ;
    CFG_CBOX_ITEM cfgcboxs5[2];
    
    CFG_HDR cfghdrs1_6;//={CFG_CBOX,"Отображать автостатус [A]",0,2};
    int isShowA ;
    CFG_CBOX_ITEM cfgcboxs6[2];
    
    CFG_HDR cfghdrs1_7;//={CFG_CBOX,"Отображать напоминалку [R]",0,2};
    int isShowR ;
    CFG_CBOX_ITEM cfgcboxs7[2];
    
    CFG_HDR cfghdrs1_8;//={CFG_CBOX,"Отображать статус/время [Tm/Xs]",0,2};
    int isShowTmXs ;
    CFG_CBOX_ITEM cfgcboxs8[2];
    
    CFG_HDR cfghdrs1_13;//={CFG_CBOX,"Отображать инвиз [I]",0,2};
    int isShowI;
    CFG_CBOX_ITEM cfgcboxs13[2];
    
    CFG_HDR cfghdrs1_9;//={CFG_CBOX,"Отображать заряд батареи",0,2};
    int isShowBat ;
    CFG_CBOX_ITEM cfgcboxs9[2];
    
    CFG_HDR cfghdrs1_10;//={CFG_CBOX,"Отображать время",0,2};
    int isShowTime;
    CFG_CBOX_ITEM cfgcboxs10[2];
    
CFG_HDR cfghdr_s0;



//-------------------------------------------------------//
//                      EXTRA CONFIG                     //
//-------------------------------------------------------//

CFG_HDR cfghdr_m51;

 
//Vibra count in vibra remaider
    CFG_HDR cfghdr65;
    unsigned int Vibra_cc;
#ifdef NEWSGOLD
//CSM for player
    CFG_HDR cfghdr66;
    char CsmAdr[9];  
    
//Text before name track
    CFG_HDR cfghdr67;
    char playing[13];
#endif
//Show command sending    
    CFG_HDR cfghdr780;
    int show_send_info;
    CFG_CBOX_ITEM cfgcbox8_z[2];
    
//Show changr status contact
    CFG_HDR cfghdr7_80;
    int isLogStatusChange;
    CFG_CBOX_ITEM cfgcbox8df_z[2];

//Show Loopback contact in CL
    CFG_HDR cfghdr71_30;
    int IsShowLoopback;
    CFG_CBOX_ITEM cfgcbox_3z[2];
    
    CFG_HDR cfghdr71_31;//={CFG_CBOX,"Открывать ссылки",0,2};
    int IsOpenBrowser;
    CFG_CBOX_ITEM cfgcbox_32[2];
    
    CFG_HDR cfghdr71_33;//={CFG_CBOX,"Открывать ссылки",0,2};
    int IsMultiBuffer;
    CFG_CBOX_ITEM cfgcbox_34[2];   

CFG_HDR cfghdr_m50;//level

//---------------------------------------//
//        SETTING LOADING SCREEN         //
//---------------------------------------//


CFG_HDR cfghdr_q1;//={CFG_LEVEL,"Экран заргузки",1,0};

  CFG_HDR cfghdr_s2;// = {CFG_RECT, "Прогрессбар", 0, 0};
  RECT PBRECT;// = {4,281,236,294};//LOadScreenRECT
   
  CFG_HDR cfghdr_s3;//={CFG_COLOR,"Цвет прогрессбара",0,0};
  char PBcolor[4];//={0x00,0x00,0x00,100};//ProgressBarColor

  CFG_HDR cfghdr_s7;//={CFG_COLOR,"Фон прогрессбара",0,0};
  char BGPBcolor[4];//={0xFF,0xFF,0xFF,100};//BackGroundProgressBarColor
  
  CFG_HDR cfghdr_s4;//={CFG_CBOX,"Менять софты",0,2};
  int IsInvertSoftKey;
  CFG_CBOX_ITEM cfgcbox_s5[2];//={"No","Yes"};  

  CFG_HDR cfghdr_s6;//={CFG_CBOX,"Показывать инфу",0,2};
  int IsShowInfo;
  CFG_CBOX_ITEM cfgcbox_s7[2];//={"No","Yes"};    

CFG_HDR cfghdr_q0;//={CFG_LEVEL,"",0,0};


//--------------------------------------//

CFG_HDR cfghdr11_10;
char myName[32];

CFG_HDR cfghdr11_9;
char ignor_list[256];

}myConfigMod;


#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

//------------ origin setting ---------------//

int DEVELOP_IF_MOD;
int SHOW_ACTIVE;
int HISTORY_SAVE_TYPE;
int HISTORY_BUFFER_MOD;
unsigned int extended_sounds;
char sndGlobal2[64];
char sndMsg2[64];
char pingsnd[64];


//------------ SETTING BUTTON -------------//

int LongRightAction;
int LongLeftAction;
int ActionResh;
int LongVolUpAction;
int LongVolDownAction ;
int isVolUpButton;//If action vol up && vol down button on elka
int isVolDownButton;

int longAct_0;
int longAct_1;
int longAct_2;
int longAct_3;
int longAct_4;
int longAct_5;
int longAct_6;
int longAct_7;
int longAct_8;
int longAct_9;
int longAct_10;
int longAct_11;
int longAct_RB;
 

//------------ STATUS SETTING --------------//

#ifdef NEWSGOLD
int ShowStatusIcon;
#endif
int show_xstatus;
unsigned int IDLEICON_XX;
unsigned int IDLEICON_YX;
int ind_set_xstatus;
#ifdef NEWSGOLD
unsigned int NumStatusToIconBar;
unsigned int NumXStatusToIconBar;

unsigned int ADDITION_UIN;
int show_add_state;
int show_add_xstate;
#endif


//----------- PNG FONT SETTING -------------//

unsigned int fonto;
char fontpath[64];
unsigned int msg_d_x;
unsigned int msg_d_y;
unsigned int main_msg_timer;//Кол-во секунд сколько показывать сообщения на идле
char fontColor[];//Цвет для системного шрифта
char bgrColor[];
unsigned int main_lenght;
int use_cutter;
unsigned int spaco1;
unsigned int spaco2;
  

//------------ smiles color -----------------//

char SmBGcolor[4];
char SmSelectColor[4];
char SmFontColor[4];


//---------------- idle set -----------------//

//Online time on idle
unsigned int onlineTime_x;
unsigned int onlineTime_y;
int Pause_Time;  

unsigned int Ping_X;
unsigned int Ping_Y;
unsigned int ping_time_config;

int Is_Unread_On;
unsigned int unread_X;
unsigned int unread_Y;

int Is_Online_On;
unsigned int online_x;
unsigned int online_y;  

//---------------- AUTO STATUS ---------------//

int Auto_Status;//Включен автостатус или нет 
int IsOnNAStatus;
unsigned int NAStatusTime;    
int isAutoStatusOn;// = 0;
unsigned int AUTO_ENGADE;
unsigned int automsgcount;
int time_zone;
char awayHelloMsg[24];//hello msg
char awayMsg[256];//message
int showAwayIdle;  
unsigned int auto_x;
unsigned int auto_y;
int Reset_timer;  


//------------- AUTO CHANGE STATUS ------------//

TTime tm_status_1;
unsigned int status_1;
TTime tm_status_2;
unsigned int status_2;
TTime tm_status_3;
unsigned int status_3;
TTime tm_status_4;
unsigned int status_4;
TTime tm_status_5;
unsigned int status_5;    
    

//---------------  SCREEN SAVER ---------------//

#ifdef NEWSGOLD
unsigned int screenSaverTime;
int isScreenSaverEna;
#endif

//------------ popup message -----------------//

int PopupShow;// = 3;
char PopupMsgString[9];//="%s";
char PopupOnlineString[9];//=">%s<";  
unsigned int PopupFont;//=FONT_SMALL;
char PopupBGCl[4];
char PopupOnlineCl[4];    
char PopupMsgCl[4];


//------------ CL HEADER SETTING -------------//

int TIME_OR_MSG;
unsigned int ON_OFF_TIME;
int ShowTraff;
int isShowCont;
int isShowUnread;
int isShowI;
int isShowV;
int isShowS;
int isShowR;
int isShowA;
int isShowTmXs;
int isShowTime;
int isShowBat;


//-------------  EXTRA CONFIG ---------------//

unsigned int Vibra_cc;
#ifdef NEWSGOLD
char playing[13];
char CsmAdr[9];
#endif
int show_send_info;  
int isLogStatusChange;
int IsShowLoopback;//Show Loopback contact in CL
int IsOpenBrowser;
int IsMultiBuffer;


//-------------  LOAD SCREEN SET ------------//

RECT PBRECT;
char PBcolor[4];
char BGPBcolor[4];
int IsInvertSoftKey;
int IsShowInfo;



char ignor_list[256];
//char myName[256];
char I_str[32];

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
int SORT_CLIST;


int HISTORY_TYPE;
unsigned int vibraPower;

unsigned int IDLEICON_X;
unsigned int IDLEICON_Y;
  
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

char sndStartup[64];
char sndSrvMsg[64];
char sndGlobal[64];
char sndGlobal2[64];
char sndMsg[64];
char sndMsgSent[64];
unsigned int sndVolume;
  
char SMILE_FILE[64];
char TEMPLATES_PATH[64];
char ICON_PATH[64];
char SMILE_PATH[64];
char XSTATUSES_PATH[64];
char HIST_PATH[64];
char NATICQ_HOST[128];
unsigned int NATICQ_PORT;
unsigned int RECONNECT_TIME;



//--------------------------------------------------------//
//                      INIT CONFIGS                      //
//--------------------------------------------------------//

char per_s[]="%s";

char suc_config_name[64];
char suc_config_name_mod[64];
extern int maincsm_id;
int UpdateConfig()
{ 
  int myConfigSize = sizeof(myConfig);
  myConfig* myConf = (myConfig*)malloc(myConfigSize);
  int newCfgFile;
  int myConfigModSize = sizeof(myConfigMod);
  myConfigMod* myConfMod = (myConfigMod*)malloc(myConfigModSize);
  int newCfgModFile;// = InitConfig(myConfMod, myConfigModSize, "NATICQMod",disk);
  
#ifdef kluchnik
  newCfgFile= InitConfig(myConf, myConfigSize, "NATICQ_2747093",4);
  newCfgModFile = InitConfig(myConfMod, myConfigModSize, "NATICQMod_2747093",4);  
  sprintf(suc_config_name,"4:\\zbin\\etc\\NATICQ_2747093.bcfg");      
  sprintf(suc_config_name_mod,"4:\\zbin\\etc\\NATICQMod_2747093.bcfg");      
#else
  if((newCfgFile= InitConfig(myConf, myConfigSize, "NATICQRun",4))!=-1)
  { 
    sprintf(suc_config_name,"4:\\zbin\\etc\\NATICQRun.bcfg"); 
  }
  else
  {
     newCfgFile= InitConfig(myConf, myConfigSize, "NATICQRun",0);
     sprintf(suc_config_name,"0:\\zbin\\etc\\NATICQRun.bcfg");
  }
  
  if((newCfgModFile = InitConfig(myConfMod, myConfigModSize, "NATICQMod2",4))!=-1)
  {
    sprintf(suc_config_name_mod,"4:\\zbin\\etc\\NATICQMod2.bcfg");      
  }
  else
  {
     newCfgModFile = InitConfig(myConfMod, myConfigModSize, "NATICQMod2",0);
     sprintf(suc_config_name_mod,"0:\\zbin\\etc\\NATICQMod2.bcfg");      
  }
#endif

if(get_file_size(suc_config_name_mod)!=sizeof(myConfigMod) || get_file_size(suc_config_name)!=sizeof(myConfig))
{
   LockSched();
   ShowMSG(2,(int)"Config mod not correct!!!");
   UnlockSched();
 //  void ElfKiller(void);
   //SUBPROC((void *)ElfKiller);
   CloseCSM(maincsm_id);
}
  
if(newCfgFile!=-1 && newCfgModFile!=-1)//Если конфиги есть
{  
//-------------------- MOD CONFIG ---------------------//
  
//-------------- setting interface ---------------//
  
  TIME_OR_MSG=myConfMod->TIME_OR_MSG;  
  IsShowLoopback=myConfMod->IsShowLoopback;//Show Loopback contact in CL  
  SHOW_ACTIVE=myConfMod->SHOW_ACTIVE;
  HISTORY_SAVE_TYPE=myConfMod->HISTORY_SAVE_TYPE;
  IsOpenBrowser=myConfMod->IsOpenBrowser; 
  IsMultiBuffer=myConfMod->IsMultiBuffer;
  
  
//-------------- log status change ---------------//
  
  isLogStatusChange=myConfMod->isLogStatusChange;
  
  
//-------------- setting png font ----------------//
  
  sprintf(fontpath,per_s,myConfMod->fontpath);
  fonto=myConfMod->fonto;
  spaco1=myConfMod->spaco1;
  spaco2=myConfMod->spaco1;//так нада
  
  
//---------- Цвет для системного шрифта ----------//
  
  for(int i=0;i<4;i++)
  {
    fontColor[i]=myConfMod->fontColor[i];
    bgrColor[i]=myConfMod->bgrColor[i];
  }
  
  Is_Unread_On=myConfMod->Is_Unread_On;
  
  
//------------- automessage setting --------------//
  
  AUTO_ENGADE=myConfMod->AUTO_ENGADE;//time to away
  isAutoStatusOn=myConfMod->isAutoStatusOn;
  automsgcount=myConfMod->automsgcount;
  Auto_Status=myConfMod->Auto_Status;//Включен автостатус или нет
  time_zone=myConfMod->time_zone;
  auto_x=myConfMod->auto_x;
  auto_y=myConfMod->auto_y;
  showAwayIdle=myConfMod->showAwayIdle;
  sprintf(awayHelloMsg,per_s,myConfMod->awayHelloMsg);//hello msg
  snprintf(awayMsg,256,per_s,myConfMod->awayMsg);//message
  Reset_timer=myConfMod->Reset_timer;//reset away timer on send message or key down
  IsOnNAStatus=myConfMod->IsOnNAStatus;
  NAStatusTime=myConfMod->NAStatusTime;    
  
  
//------------- AUTO CHANGE STATUS ------------//

  tm_status_1=myConfMod->tm_status_1;
  status_1=myConfMod->status_1;
  
  tm_status_2=myConfMod->tm_status_2;
  status_2=myConfMod->status_2;
  
  tm_status_3=myConfMod->tm_status_3;
  status_3=myConfMod->status_3;
  
  tm_status_4=myConfMod->tm_status_4;
  status_4=myConfMod->status_4;
  
  tm_status_5=myConfMod->tm_status_5;
  status_5=myConfMod->status_5;    


//------------- Online time on idle --------------//
  
  onlineTime_x=myConfMod->onlineTime_x;
  onlineTime_y=myConfMod->onlineTime_y;
  Pause_Time=myConfMod->Pause_Time;

  
//----------------- setting ping -----------------//
  
  Ping_X=myConfMod->Ping_X;
  Ping_Y=myConfMod->Ping_Y;
  sprintf(pingsnd,per_s,myConfMod->pingsnd);
  ping_time_config=myConfMod->ping_time_config;
  
//---------- show online contact on idle --------//
  
  Is_Online_On=myConfMod->Is_Online_On;
  online_x=myConfMod->online_x;
  online_y=myConfMod->online_y;  

  
//------- setting last 5 message on idle ---------//
  
  main_msg_timer=myConfMod->main_msg_timer;
  msg_d_x=myConfMod->msg_d_x;
  msg_d_y=myConfMod->msg_d_y;
  main_lenght=myConfMod->main_lenght;
  use_cutter=myConfMod->use_cutter;
  
  
//----------------- player setting ---------------//

#ifdef NEWSGOLD
  sprintf(CsmAdr,per_s,myConfMod->CsmAdr);//csm mediaplayer
  sprintf(playing,per_s,myConfMod->playing);//text before name song
#endif
  
//----------- number status to iconbar -----------//
  
#ifdef NEWSGOLD
  NumStatusToIconBar=myConfMod->NumStatusToIconBar;
  NumXStatusToIconBar=myConfMod->NumXStatusToIconBar;
  ShowStatusIcon=myConfMod->ShowStatusIcon;
  
  ADDITION_UIN=myConfMod->ADDITION_UIN;
  show_add_state=myConfMod->show_add_state;
  show_add_xstate=myConfMod->show_add_xstate;
#endif
  
  
//--------------- setting xstatus ----------------//
  
  show_xstatus=myConfMod->show_xstatus;
  IDLEICON_XX=myConfMod->IDLEICON_XX;
  IDLEICON_YX=myConfMod->IDLEICON_YX;
  ind_set_xstatus=myConfMod->ind_set_xstatus;;
  
//------------------ setting keys ----------------//
  
  LongRightAction=myConfMod->LongRightAction;
  LongLeftAction=myConfMod->LongLeftAction;  
  ActionResh=myConfMod->ActionResh;//действие на # в контакт листе
    
//-  If action vol up && vol down button on elka -//
  
  LongVolUpAction=myConfMod->LongVolUpAction;
  LongVolDownAction=myConfMod->LongVolDownAction;
  isVolUpButton=myConfMod->isVolUpButton;
  isVolDownButton=myConfMod->isVolDownButton;

//---------- LONG PRESS ACTION ON CL -----------//

  longAct_0=myConfMod->longAct_0;
  longAct_1=myConfMod->longAct_1;
  longAct_2=myConfMod->longAct_2;
  longAct_3=myConfMod->longAct_3;
  longAct_4=myConfMod->longAct_4;
  longAct_5=myConfMod->longAct_5;
  longAct_6=myConfMod->longAct_6;
  longAct_7=myConfMod->longAct_7;
  longAct_8=myConfMod->longAct_8;
  longAct_9=myConfMod->longAct_9;
  longAct_10=myConfMod->longAct_10;
  longAct_11=myConfMod->longAct_11;
  longAct_RB=myConfMod->longAct_RB;
  
//---------------- other setting -----------------//
  
  sprintf(ignor_list,per_s,myConfMod->ignor_list);
  sprintf(I_str,per_s,myConfMod->myName);
  show_send_info=myConfMod->show_send_info;
  
  Vibra_cc=myConfMod->Vibra_cc;//vibra remainder
  
  unread_X=myConfMod->unread_X;
  unread_Y=myConfMod->unread_Y;
  
  
//------------- Screen Saver setting -------------//
  
#ifdef NEWSGOLD
  screenSaverTime=myConfMod->screenSaverTime;
  isScreenSaverEna=myConfMod->isScreenSaverEna;
#endif
  
//------------ smiles && popup color -------------//

  for(int i=0;i<4;i++)
  {
    SmBGcolor[i]=myConfMod->SmBGcolor[i];
    SmSelectColor[i]=myConfMod->SmSelectColor[i];
    SmFontColor[i]=myConfMod->SmFontColor[i];
    
    PopupBGCl[i]=myConfMod->PopupBGCl[i];;
    PopupOnlineCl[i]=myConfMod->PopupOnlineCl[i];    
    PopupMsgCl[i]=myConfMod->PopupMsgCl[i];
    
    
    PBcolor[i]=myConfMod->PBcolor[i];
    BGPBcolor[i]=myConfMod->BGPBcolor[i];
  }

  
//----------------- popup message ----------------//

  PopupShow=myConfMod->PopupShow;
  sprintf(PopupMsgString,per_s,myConfMod->PopupMsgString);
  sprintf(PopupOnlineString,per_s,myConfMod->PopupOnlineString);
  PopupFont=myConfMod->PopupFont;
  
  
  
//--------------- CL HEADER SETTING --------------//

  isShowCont=myConfMod->isShowCont;
  ShowTraff=myConfMod->ShowTraff;
  isShowUnread=myConfMod->isShowUnread;
  isShowI=myConfMod->isShowI;
  isShowV=myConfMod->isShowV;
  isShowS=myConfMod->isShowS;
  isShowR=myConfMod->isShowR;
  isShowA=myConfMod->isShowA;
  isShowTmXs=myConfMod->isShowTmXs;
  isShowTime=myConfMod->isShowTime;
  isShowBat=myConfMod->isShowBat;
  ON_OFF_TIME=myConfMod->ON_OFF_TIME;
//-------------  LOAD SCREEN SET ------------//

  PBRECT=myConfMod->PBRECT;
  IsInvertSoftKey=myConfMod->IsInvertSoftKey;
  IsShowInfo=myConfMod->IsShowInfo;


//------------------ ORIGINAL CONFIG --------------------//
  
  UIN=myConf->UIN;
  sprintf(PASS,per_s,myConf->PASS);
  
  SORT_CLIST=myConf->SORT_CLIST;
  LOG_ALL=myConf->LOG_ALL;
  LOG_STATCH=myConf->LOG_STATCH;
  VIBR_TYPE=myConf->VIBR_TYPE;
  VIBR_ON_CONNECT=myConf->VIBR_ON_CONNECT;

  DEVELOP_IF_MOD=myConfMod->DEVELOP_IF_MOD;
  if(!DEVELOP_IF_MOD)
    DEVELOP_IF=myConf->DEVELOP_IF;
  else
    DEVELOP_IF=3;
  FIRST_LETTER=myConf->FIRST_LETTER;    

  HISTORY_TYPE=myConf->HISTORY_TYPE;
  
 
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
  HISTORY_BUFFER_MOD=myConfMod->HISTORY_BUFFER_MOD;  
  if(!HISTORY_BUFFER_MOD)
     HISTORY_BUFFER=myConf->HISTORY_BUFFER;  
  else
    HISTORY_BUFFER=5;
  
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
  sprintf(sndGlobal2,per_s,myConfMod->sndGlobal2);
  sprintf(sndMsg,per_s,myConf->sndMsg);
  sprintf(sndMsg2,per_s,myConfMod->sndMsg2);
  extended_sounds=myConfMod->extended_sounds;
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
}
else
{
  LockSched();
  ShowMSG(1,(int)"Can't open configs!");
  UnlockSched();
  CloseCSM(maincsm_id);
}
return 0;
}



//===================================================================

/*
  uins.txt
  uin;pass
  123456;qwe123
*/

#define max_uin 10
ADDUIN addonuin[max_uin];

char *temp_chars; //Собственно файл
char **temp_lines; //Массив указателей на строки

void FreeUINS(void)
{
  if (temp_lines) mfree(temp_lines);
  if (temp_chars) mfree(temp_chars);
  temp_lines=NULL;
  temp_chars=NULL;
}

int LoadUIN()
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  FreeUINS();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\uins.txt");
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;
  p=temp_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	temp_lines=realloc(temp_lines,(i+1)*sizeof(char *));
	temp_lines[i++]=pp;
      }
      pp=NULL;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==NULL) pp=p;
    }
    p++;
    if(i>max_uin) break;
  }
  return i;
}


extern const char percent_s[];
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

int ParseUINS()
{
  char *pass;
  char uin[9];
  int i=LoadUIN();
  int j=0;
  if(i==0) return 0;
  
  addonuin[0].uin=UIN;//В первую ячейку уин из конфига
  snprintf(addonuin[0].pass,9,percent_s,PASS);
  
  while(j<i)
  {
   pass=strrchr(temp_lines[j],';')+1; 
   snprintf(addonuin[j+1].pass,9,percent_s,pass);
  // ShowMSG(2,(int)pass);
   int k;
   for(k=0; k<9 && temp_lines[j][k]!=';';k++)
      uin[k]=temp_lines[j][k];
   uin[k]=0;
   addonuin[j+1].uin=strtoul(uin,0,10);
//   ShowMSG(2,(int)uin);   
   j++;
  }
return j;
}
