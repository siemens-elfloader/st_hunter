#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"
#include "language.h"
//Конфигурация


#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_UINT,"UIN",0,0xFFFFFFFF};
__root const unsigned int UIN=0;

__root const CFG_HDR cfghdr1={CFG_STR_PASS,"Password",0,8};
__root const char PASS[9]="";

//Network settings
__root const CFG_HDR cfghdr2={CFG_LEVEL,"Hosts and paths",1,0};
  __root const CFG_HDR cfghdr2_1={CFG_STR_WIN1251,"Host",0,127};
  __root const char NATICQ_HOST[128]="cbsie.dyndns.info;himoza.zyns.com";

  __root const CFG_HDR cfghdr2_2={CFG_UINT,"Default port",0,65535};
  __root const unsigned int NATICQ_PORT=5050;

  __root const CFG_HDR cfghdr2_3={CFG_UINT,"Reconnect timeout",0,65535};
  __root const unsigned int RECONNECT_TIME=10;
  
  __root const CFG_HDR cfghdr2_4={CFG_STR_UTF8,"History path",0,63};
  __root const char HIST_PATH[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\history";

  __root const CFG_HDR cfghdr2_5={CFG_STR_UTF8,"Smiles File",0,63};
  __root const char SMILE_FILE[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\smiles.cfg";

  __root const CFG_HDR cfghdr2_6={CFG_STR_UTF8,"Smiles .png path",0,63};
  __root const char SMILE_PATH[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\smiles";

  __root const CFG_HDR cfghdr2_7={CFG_STR_UTF8,"Images .png path",0,63};
  __root const char ICON_PATH[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\img";

  __root const CFG_HDR cfghdr2_8={CFG_STR_UTF8,"XStatus .png path",0,63};
  __root const char XSTATUSES_PATH[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\ximg";

  __root const CFG_HDR cfghdr2_9={CFG_STR_UTF8,"Templates path",0,63};
  __root const char TEMPLATES_PATH[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\templates";
__root const CFG_HDR cfghdr3={CFG_LEVEL,"",0,0};

//View settings
__root const CFG_HDR cfghdr4={CFG_LEVEL,"Interface",1,0};
//Status icon on mainscreen position
  __root const CFG_HDR cfghdr4_1={CFG_COORDINATES,"Idle icon position",0,0};
  __root const unsigned int IDLEICON_X=0;
  __root const unsigned int IDLEICON_Y=50;

    
//Popup chat
  __root const CFG_HDR cfghdr4_2={CFG_CBOX,"Popup",0,3};
  __root const int DEVELOP_IF = 0;
  __root const CFG_CBOX_ITEM cfgcbox4_2[3]={"Allways", "If Kbd Unlock","Never"};
  
#ifndef GUI_INTERFACE
//Sorting
   __root const CFG_HDR cfghdr4_3={CFG_CBOX,"Sort CList",0,2};
   __root const int SORT_CLIST = 1;
   __root const CFG_CBOX_ITEM cfgcbox4_3[2]={"By Name","By Status"};
#endif
//First letter when typing
  __root const CFG_HDR cfghdr4_4={CFG_CBOX,"First letter",0,2};
  __root const int FIRST_LETTER = 0;
  __root const CFG_CBOX_ITEM cfgcbox4_4[2]={"Small","Big"}; 

  //First letter when typing
  __root const CFG_HDR cfghdr4_9={CFG_CBOX,"Enter button",0,2};
  __root const int IsBufferMenu = 1;
  __root const CFG_CBOX_ITEM cfgcbox4_9[2]={"Standart","Message buffer"}; 
    
  //Fonts and colors
  __root const CFG_HDR cfghdr4_5={CFG_LEVEL,"Font and colors",1,0};
  /*  __root const CFG_HDR cfghdr4_5_1={CFG_UINT,"My string color",0,1000};
    __root const unsigned int I_COLOR=3;
  
    __root const CFG_HDR cfghdr4_5_2={CFG_UINT,"Your string color",0,1000};
    __root const unsigned int TO_COLOR=2;
  
    __root const CFG_HDR cfghdr4_5_3={CFG_UINT,"XStatus color",0,1000};
    __root const unsigned int X_COLOR=2;
  
    __root const CFG_HDR cfghdr4_5_4={CFG_UINT,"Main font size",0,6};
    __root const unsigned int ED_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_5={CFG_UINT,"Header font size",0,6};
    __root const unsigned int ED_H_FONT_SIZE=2;
  
    __root const CFG_HDR cfghdr4_5_6={CFG_UINT,"XStatus font size",0,6};
    __root const unsigned int ED_X_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_7={CFG_UINT,"My old string color",0,1000};
    __root const unsigned int O_I_COLOR=15;
  
    __root const CFG_HDR cfghdr4_5_8={CFG_UINT,"Your old string color",0,1000};
    __root const unsigned int O_TO_COLOR=14;
    
    __root const CFG_HDR cfghdr4_5_9={CFG_UINT,"XStatus old color",0,1000};
    __root const unsigned int O_X_COLOR=14;
*/  
  
    __root const CFG_HDR cfghdr4_5_1={CFG_UINT,"My string color",0,1000};
    __root const unsigned int I_COLOR=9;
  
    __root const CFG_HDR cfghdr4_5_2={CFG_UINT,"Your string color",0,1000};
    __root const unsigned int TO_COLOR=7;
  
    __root const CFG_HDR cfghdr4_5_3={CFG_UINT,"XStatus color",0,1000};
    __root const unsigned int X_COLOR=7;
  
    __root const CFG_HDR cfghdr4_5_4={CFG_UINT,"Main font size",0,6};
    __root const unsigned int ED_FONT_SIZE=3;
  
    __root const CFG_HDR cfghdr4_5_5={CFG_UINT,"Header font size",0,6};
    __root const unsigned int ED_H_FONT_SIZE=3;
  
    __root const CFG_HDR cfghdr4_5_6={CFG_UINT,"XStatus font size",0,6};
    __root const unsigned int ED_X_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_7={CFG_UINT,"My old string color",0,1000};
    __root const unsigned int O_I_COLOR=18;
  
    __root const CFG_HDR cfghdr4_5_8={CFG_UINT,"Your old string color",0,1000};
    __root const unsigned int O_TO_COLOR=113;
    
    __root const CFG_HDR cfghdr4_5_9={CFG_UINT,"XStatus old color",0,1000};
    __root const unsigned int O_X_COLOR=22;
  

    __root const CFG_HDR cfghdr4_5_10={CFG_UINT,"Main old font size",0,6};
    __root const unsigned int O_ED_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_11={CFG_UINT,"Header old font size",0,6};
    __root const unsigned int O_ED_H_FONT_SIZE=2;
  
    __root const CFG_HDR cfghdr4_5_12={CFG_UINT,"XStatus old font size",0,6};
    __root const unsigned int O_ED_X_FONT_SIZE=1;
  
    __root const CFG_HDR cfghdr4_5_13={CFG_UINT,"Acked color",0,1000};
    __root const unsigned int ACK_COLOR=4;
  
    __root const CFG_HDR cfghdr4_5_14={CFG_UINT,"Unacked color",0,1000};
    __root const unsigned int UNACK_COLOR=20;
  
  __root const CFG_HDR cfghdr4_6={CFG_LEVEL,"",0,0};  

  //Illumination by BoBa 19.04.2007
  __root const CFG_HDR cfghdr4_7={CFG_LEVEL,"Illumination setup",1,0};
    __root const CFG_HDR cfghdr4_7_1={CFG_INT,"Display on Recv",0,100};
    __root const unsigned int ILL_DISP_RECV=10;
  
    __root const CFG_HDR cfghdr4_7_2={CFG_INT,"Keys on Recv",0,100};
    __root const unsigned int ILL_KEYS_RECV=10;
  
    __root const CFG_HDR cfghdr4_7_3={CFG_INT,"Timeout on Recv",0,60};
    __root const unsigned int ILL_RECV_TMR=10;
  
    __root const CFG_HDR cfghdr4_7_4={CFG_INT,"Fade on Recv",0,1000};
    __root const unsigned int ILL_RECV_FADE=256;
  
    __root const CFG_HDR cfghdr4_7_5={CFG_INT,"Display on Send",0,100};
    __root const unsigned int ILL_DISP_SEND=10;
  
    __root const CFG_HDR cfghdr4_7_6={CFG_INT,"Keys on Send",0,100};
    __root const unsigned int ILL_KEYS_SEND=0;
  
    __root const CFG_HDR cfghdr4_7_7={CFG_INT,"Timeout on Send",0,60};
    __root const unsigned int ILL_SEND_TMR=5;
  
    __root const CFG_HDR cfghdr4_7_8={CFG_INT,"Fade on Send",0,1000};
    __root const unsigned int ILL_SEND_FADE=256;
  
    __root const CFG_HDR cfghdr4_7_9={CFG_INT,"Fade on Off",0,1000};
    __root const unsigned int ILL_OFF_FADE=256;
    
    __root const CFG_HDR cfghdrI_9={CFG_INT,"Illum by online",0,1000};
    __root const unsigned int IllByONCantact = 5;  

  __root const CFG_HDR cfghdr4_8={CFG_LEVEL,"",0,0};  
 
__root const CFG_HDR cfghdr5={CFG_LEVEL,"",0,0};


#ifdef GUI_INTERFACE
//Contact list
__root const CFG_HDR cfghdrCL_1={CFG_LEVEL,"Contact List",1,0};
  
    //Sorting
     __root const CFG_HDR cfghdr4_3={CFG_CBOX,"Sort CList",0,2};
     __root const int SORT_CLIST = 1;
     __root const CFG_CBOX_ITEM cfgcbox4_3[2]={"By Name","By Status"};
   
     
     __root const CFG_HDR cfghdrI_2={CFG_CHECKBOX,"Two columns",0,2};
    #ifdef ELKA
     __root const int VIEW_MODE = 1;
    #else
     __root const int VIEW_MODE = 0;
    #endif
              
     __root const CFG_HDR cfghdrI_3={CFG_CHECKBOX,"Two lines",0,2};
     __root const int LINE_MODE = 0;
      
     __root const CFG_HDR cfghdrI_7={CFG_CHECKBOX,"Show softkey",0,2};
     __root const unsigned int SOFT_MODE = 1;

     __root const CFG_HDR cfghdrI_11={CFG_CHECKBOX,"Show statuses to iconbar",0,2};
     __root const unsigned int ShowStatusToIconBar = 1;               
    
     __root const CFG_HDR cfghdrI_8={CFG_CBOX,"Show process send",0,4};
     __root const unsigned int ShowProcessSend = 1;
     __root const CFG_CBOX_ITEM cfgcboxI_9[4]={"No","Number","Lines","Number+Lines"};  
      
     __root const CFG_HDR cfghdr6_8={CFG_CBOX,"Show active chat",0,2};
     __root const int SHOW_ACTIVE = 1;
     __root const CFG_CBOX_ITEM cfgcbox6_8[2]={"No","Yes"};  
      
     __root const CFG_HDR cfghdrI_10={CFG_CBOX,"Select active chat",0,2};
     __root const int SelBoldActiveChat = 1;
     __root const CFG_CBOX_ITEM cfgcboxI_11[2]={"No select","Bold font"};         
  
     __root const CFG_HDR cfghdrI_12={CFG_CHECKBOX,"Los status change time",0,2};
     __root const int isLogStatusChange = 1;
     
     __root const CFG_HDR cfghdrI_13={CFG_CHECKBOX,"Blink icon online contact",0,2};
     __root const int BlinkIconByOnline = 1;
     
     //CONTACT LIST HEADER
      __root const CFG_HDR cfghdr_s1={CFG_LEVEL,"CL Header",1,0};        
         
            __root const CFG_HDR cfghdrs1_s1={CFG_UINT,"Time online contact(sec)",0,1000};
            __root const unsigned int ON_OFF_TIME=10;            
          
            __root const CFG_HDR cfghdrHead_1={CFG_CHECKBOX,"Status",0,2};
            __root const int HEAD_STATUS = 1;
          
            __root const CFG_HDR cfghdrHead_2={CFG_CHECKBOX,"X-Status",0,2};
            __root const int HEAD_XSTATUS = 1;
          
            __root const CFG_HDR cfghdrHead_3={CFG_CHECKBOX,"Private status",0,2};
            __root const int HEAD_PRIVATE = 1;
           
            __root const CFG_HDR cfghdrs1_11={CFG_CHECKBOX,"Amount contact",0,2};
            __root const int isShowCont = 1;
          
            __root const CFG_HDR cfghdrs1_12={CFG_CHECKBOX,"Amount not read",0,2};
            __root const int isShowUnread = 1;
          
            __root const CFG_HDR cfghdrs1_4={CFG_CHECKBOX,"Show vibra [V] ",0,2};
            __root const int isShowV = 1;
            
            __root const CFG_HDR cfghdrs1_5={CFG_CHECKBOX,"Show sound [S]",0,2};
            __root const int isShowS = 1;
            
            __root const CFG_HDR cfghdrs1_9={CFG_CHECKBOX,"Отображать заряд батареи",0,2};
            __root const int isShowBat = 1;
            
            __root const CFG_HDR cfghdrs1_10={CFG_CHECKBOX,"Отображать автооветчик[A]",0,2};
            __root const int isShowA = 1;
            
            __root const CFG_HDR cfghdrs1_13={CFG_CHECKBOX,"Отображать [Vr]",0,2};
            __root const int isShowVr = 1;
          
        __root const CFG_HDR cfghdr_s0={CFG_LEVEL,"",0,0};///CL Header        
        
        
     __root const CFG_HDR cfghdrCl_1={CFG_LEVEL,"CL colors",1,0};   
     
         __root const CFG_HDR cfghdrrCl_2={CFG_COLOR,"Background CL",0,0};
         __root const char CLBackgrClr[4]={0x00,0x00,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_3={CFG_COLOR,"Softkeys",0,0};
         __root const char SoftBackgrClr[4]={0xFF,0x48,0x00,0x1B};   
         
         __root const CFG_HDR cfghdrrCl_4={CFG_COLOR,"Font softkeys",0,0};
         __root const char SoftFontClr[4]={0xFF,0xFF,0xFF,0x64};
         
         __root const CFG_HDR cfghdrrCl_5={CFG_COLOR,"Header",0,0};
         __root const char HeadBackgrClr[4]={0xFF,0x48,0x00,0x1B};   
         
         __root const CFG_HDR cfghdrrCl_6={CFG_COLOR,"Font header",0,0};
         __root const char HeadFontClr[4]={0xFF,0xFF,0xFF,0x64};
         
         __root const CFG_HDR cfghdrrCl_7={CFG_COLOR,"Scrollbar",0,0};
         __root const char ScrollBarClr[4]={0xFF,0x00,0x00,0x15};
         
         __root const CFG_HDR cfghdrrCl_8={CFG_COLOR,"Cursor",0,0};
         __root const char ClCursorClr[4]={0xFF,0x00,0x00,0x32};
         
         __root const CFG_HDR cfghdrrCl_9={CFG_COLOR,"Cursor  border",0,0};
         __root const char ClCursorBordClr[4]={0xFF,0xFF,0x00,0x60};
         
         
         __root const CFG_HDR cfghdrrCl_16={CFG_COLOR,"Group color",0,0};
         __root const char GroupClr[4]={0xFF,0x00,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_17={CFG_COLOR,"Second line font",0,0};
         __root const char CLDescriptClr[4]={0xFF,0x00,0x00,0x64};         
         
         __root const CFG_HDR cfghdrrCl_14={CFG_COLOR,"Color not send",0,0};
         __root const char UnAckedClr[4]={0xFF,0xFF,0x0,0x15};
         
         __root const CFG_HDR cfghdrrCl_15={CFG_COLOR,"Color sended",0,0};
         __root const char SendClr[4]={0x00,0xFF,0x00,0x15};
         
         __root const CFG_HDR cfghdrC2_1={CFG_LEVEL,"CL status colors",1,0};   
         
               __root const CFG_HDR cfghdrrCl_10={CFG_COLOR,"Active char",0,0};
               __root const char ActiveClr[4]={0xFF,0xFF,0x00,0x64};
               
               __root const CFG_HDR cfghdrrCl_11={CFG_COLOR,"Not active chat",0,0};
               __root const char NotActiveClr[4]={0x00,0xFF,0x00,0x64};         
               
               __root const CFG_HDR cfghdrrCl_12={CFG_COLOR,"Select contact",0,0};
               __root const char CLColorSelCont[4]={0xFF,0xFF,0x00,0x64}; 
               
               __root const CFG_HDR cfghdrsC2_1={CFG_CHECKBOX,"individual colors",0,2};
               __root const int isIndividualColor = 1;
               
               __root const CFG_HDR cfghdrrC2_2={CFG_COLOR,"Online",0,0};
               __root const char CLColorsOnline[4]={0x00,0xFF,0x00,0x64};         

               __root const CFG_HDR cfghdrrC2_3={CFG_COLOR,"Away",0,0};
               __root const char CLColorsAway[4]={0x00,0x00,0xFF,0x64};         

               __root const CFG_HDR cfghdrrC2_4={CFG_COLOR,"N/A",0,0};
               __root const char CLColorsNA[4]={0xFF,0x00,0x00,0x64};

               __root const CFG_HDR cfghdrrC2_5={CFG_COLOR,"Occupied",0,0};
               __root const char CLColorsOccupied[4]={0x00,0xBB,0x00,0x64};
               
               __root const CFG_HDR cfghdrrC2_6={CFG_COLOR,"DND",0,0};
               __root const char CLColorsDND[4]={0xBB,0x00,0x00,0x64};
               
               __root const CFG_HDR cfghdrrC2_7={CFG_COLOR,"Depression",0,0};
               __root const char CLColorsDepression[4]={0x00,0xF0,0xFF,0x64};
               
               __root const CFG_HDR cfghdrrC2_8={CFG_COLOR,"FFC",0,0};
               __root const char CLColorsFFC[4]={0x00,0xFF,0x00,0x64};
               
               __root const CFG_HDR cfghdrrC2_9={CFG_COLOR,"Offline",0,0};
               __root const char CLColorsOff[4]={0x7A,0x70,0x78,0x64};
         
         __root const CFG_HDR cfghdrC2_0={CFG_LEVEL,"",0,0};//Color in cl  
         
    __root const CFG_HDR cfghdrCl_0={CFG_LEVEL,"",0,0};//Color in cl  
    
    
   __root const CFG_HDR cfghdrF_1={CFG_LEVEL,"Font",1,0};//level     
          
        __root const CFG_HDR cfghdrF_3 = {CFG_UINT, "CL Font", 0, 15};  
        __root const unsigned int ClFont = FONT_SMALL;
        
        __root const CFG_HDR cfghdrF_5 = {CFG_UINT, "CL Bold Font", 0, 15};  
        __root const unsigned int ClBoldFont = FONT_SMALL_BOLD;
        
        __root const CFG_HDR cfghdrF_4 = {CFG_UINT, "Font header & sofkey", 0, 15};  
        __root const unsigned int HeadFont = FONT_SMALL;
    
    __root const CFG_HDR cfghdrF_0={CFG_LEVEL,"",0,0};//    
    
    //--------------------- НАСТРОЙКА КЛАВИШ ----------------------//


  //View settings
  __root const CFG_HDR cfghdr23_4={CFG_LEVEL,"Клавиши",1,0};    
  
    //Action on # button in contact list
      __root const CFG_HDR cfghdr78_4520={CFG_CBOX,"Действие на # в КЛ",0,2};
      __root const int ActionResh = 1;
      __root const CFG_CBOX_ITEM cfgcbox_668_z[2]={"Ничего","Очистить выделенный чат"};
      
  
  //Press vol up or vol down button in CL
    __root const CFG_HDR cfghdr34_642={CFG_CBOX,"Вкл. действие на громк.вниз",0,2};
    __root const int isVolDownButton = 1;
    __root const CFG_CBOX_ITEM cfgcbox434_56[2]={"Нет","Да"};
    
    __root const CFG_HDR cfghdr34_6342={CFG_CBOX,"Вкл.действие на громк.вверх",0,2};
    __root const int isVolUpButton = 1;
    __root const CFG_CBOX_ITEM cfgcbox4134_56[2]={"Нет","Да"};
    
  
  //{"Без действия","Звук","Вибра","Инвиз","Реж.напоминания","Очистить чаты","Автоответчик","Показ.хстат/время","Конфиг","Конфиг мода","Пинг","Заблокировать","На гэ","Меню юзера"}
    __root const CFG_HDR cfghdr99_1={CFG_LEVEL,"Длинное нажатие",1,0};
      
      //Long press right in chat
        __root const CFG_HDR cfghdr4_67={CFG_CBOX,"Длинное нажатие вправо в чате",0,3};
        __root const int LongRightAction = 0;
        __root const CFG_CBOX_ITEM cfgcbox4_56[3]={"Ничего","Следующий чат","Вставить смайл"};    
      
      //Long press right in chat
        __root const CFG_HDR cfghdr34_64={CFG_CBOX,"Длинное нажатие влево в чате",0,2};
        __root const int LongLeftAction = 0;
        __root const CFG_CBOX_ITEM cfgcbox444_56[2]={"Ничего","Предыдущий чат"};
        
      
      //Long press vol up button in chat
        __root const CFG_HDR cfghdr23_1={CFG_CBOX,"Длинное нажатие + в чате",0,2};
        __root const int LongVolUpAction = 1;
        __root const CFG_CBOX_ITEM cfgcbox23_2[2]={"Обычно","Вставка посл.сообщения"};
      
      //Long press vol down button in chat
        __root const CFG_HDR cfghdr23_3={CFG_CBOX,"Длинное нажатие - в чате",0,2};
        __root const int LongVolDownAction = 1;
        __root const CFG_CBOX_ITEM cfgcbox23_4[2]={"Обычно","Очистить текст"};  
  
  
  
        __root const CFG_HDR cfghdr99_3_0={CFG_CBOX,"Длинная 0 в КЛ",0,11};
        __root const int longAct_0 = 8;
        __root const CFG_CBOX_ITEM cfgcbox99_4_0[11]={LG_NOT_ACTION,//0
                                                      LG_SOUND,//1
                                                      LG_VIBRA,//2
                                                      LG_INVIS,//3
                                                      LG_VIBRA_REM,//4
                                                      LG_CLAER_ALL_CHAT,//5
                                                      LG_AUTOMSG,//6
                                                      LG_CONFIG,//7
                                                      LG_PING,//8
                                                      LG_HIHE_LOCK,//9
                                                      LG_IDLE,//10
                                                    };
  
        __root const CFG_HDR cfghdr99_3_1={CFG_CBOX,"Длинная 1 в КЛ",0,11};
        __root const int longAct_1 = 1;
        __root const CFG_CBOX_ITEM cfgcbox99_4_1[11]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_CONFIG,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE
                                                  };
  
        __root const CFG_HDR cfghdr99_3_2={CFG_CBOX,"Длинная 2 в КЛ",0,11};
        __root const int longAct_2 = 2;
        __root const CFG_CBOX_ITEM cfgcbox99_4_2[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,                                                       
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_3={CFG_CBOX,"Длинная 3 в КЛ",0,11};
        __root const int longAct_3 = 3;
        __root const CFG_CBOX_ITEM cfgcbox99_4_3[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE                                                        
                                                    };
        
        __root const CFG_HDR cfghdr99_3_4={CFG_CBOX,"Длинная 4 в КЛ",0,11};
        __root const int longAct_4 = 4;
        __root const CFG_CBOX_ITEM cfgcbox99_4_4[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_5={CFG_CBOX,"Длинная 5 в КЛ",0,11};
        __root const int longAct_5 = 5;
        __root const CFG_CBOX_ITEM cfgcbox99_4_5[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,                                                        
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_6={CFG_CBOX,"Длинная 6 в КЛ",0,11};
        __root const int longAct_6 = 6;
        __root const CFG_CBOX_ITEM cfgcbox99_4_6[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,                                                       
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_7={CFG_CBOX,"Длинная 7 в КЛ",0,11};
        __root const int longAct_7 = 7;
        __root const CFG_CBOX_ITEM cfgcbox99_4_7[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_8={CFG_CBOX,"Длинная 8 в КЛ",0,11};
        __root const int longAct_8 = 7;
        __root const CFG_CBOX_ITEM cfgcbox99_4_8[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_9={CFG_CBOX,"Длинная 9 в КЛ",0,11};
        __root const int longAct_9 = 0;
        __root const CFG_CBOX_ITEM cfgcbox99_4_9[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_10={CFG_CBOX,"Длинная # в КЛ",0,11};
        __root const int longAct_10 = 9;
        __root const CFG_CBOX_ITEM cfgcbox99_4_10[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };
        
        __root const CFG_HDR cfghdr99_3_11={CFG_CBOX,"Длинная * в КЛ",0,11};
        __root const int longAct_11 = 10;
        __root const CFG_CBOX_ITEM cfgcbox99_4_11[11]={LG_NOT_ACTION,
                                                      LG_SOUND,
                                                      LG_VIBRA,
                                                      LG_INVIS,
                                                      LG_VIBRA_REM,
                                                      LG_CLAER_ALL_CHAT,
                                                      LG_AUTOMSG,
                                                      LG_CONFIG,
                                                      LG_PING,
                                                      LG_HIHE_LOCK,
                                                      LG_IDLE
                                                    };     
  
        __root const CFG_HDR cfghdr99_2={CFG_LEVEL,"",0,0}; //LONG PRESS ACTION
        
  __root const CFG_HDR cfghdr223_5={CFG_LEVEL,"",0,0}; //НАСТРОКА КЛАВИШ
 
__root const CFG_HDR cfghdrCL_0={CFG_LEVEL,"",0,0};//Contact List

// Main menu
__root const CFG_HDR cfghdrM_1={CFG_LEVEL,"Main menu",1,0};
   
      __root const CFG_HDR cfghdrM_11 = {CFG_UINT, "Main Menu Font", 0, 15};  
      __root const unsigned int MMFont = FONT_SMALL;
      
      __root const CFG_HDR cfghdrsM_12={CFG_CHECKBOX,"Show xstatus second line",0,2};
      __root const int ShowXStatusSecLine = 1;
  
      __root const CFG_HDR cfghdrrM_11={CFG_COLOR,"Font",0,0};
      __root const char MMFontClr[4]={0xFF,0xFF,0xFF,0x64};
      
      __root const CFG_HDR cfghdrrM_4={CFG_COLOR,"Background",0,0};
      __root const char MMBackgrClr[4]={0x00,0x00,0x00,0x45};//"\x00\x00\x00\x45"
      
      __root const CFG_HDR cfghdrrM_5={CFG_COLOR,"Border",0,0};
      __root const char MMBackgrBordClr[4]={0xFF,0xFF,0xFF,0x64};
           
      __root const CFG_HDR cfghdrrM_2={CFG_COLOR,"Cursor",0,0};
      __root const char MMCursorBGClr[4]={0x00,0x00,0x00,0x00};
      
      __root const CFG_HDR cfghdrrM_3={CFG_COLOR,"Cursor border",0,0};
      __root const char MMCursorClr[4]={0xFF,0xFF,0xFF,0x64};       
      
      __root const CFG_HDR cfghdrM_9={CFG_CHECKBOX,"Auto rect",0,2};
      __root const unsigned int MainMenuAutoRect = 0;
     
      __root const CFG_HDR cfghdrM_10 = {CFG_UINT, "Main menu items", 0, 24};  
      __root const unsigned int MainMenuItems = 12;

      __root const CFG_HDR cfghdrM_6 = {CFG_RECT, "RECT Menu", 0, 0};
#ifdef ELKA
      __root const RECT MMRect = {0,103,170,299};
#else
      __root const RECT MMRect = {0,26,125,164};
#endif
      
      __root const CFG_HDR cfghdrM_7 = {CFG_RECT, "Rect statuses", 0, 0};
#ifdef ELKA
      __root const RECT SRect = {30,YDISP+2,190,232};
#else
      __root const RECT SRect = {5,5,128,150};
#endif
      
      __root const CFG_HDR cfghdrM_8 = {CFG_RECT, "Rect x-statuses", 0, 0};
#ifdef ELKA
      __root const RECT XSRect = {30,YDISP+2,228,271};   
#else
      __root const RECT XSRect = {5,5,126,158};   
#endif
   
__root const CFG_HDR cfghdrM_0={CFG_LEVEL,"",0,0};//  
  
#endif
  

__root const CFG_HDR cfghdr_m61={CFG_LEVEL,"Автостатус",1,0};

    __root const CFG_HDR cfghdr668 = {CFG_CBOX,"Включить",0,2};
    __root const int Auto_Status = 0;
    __root const CFG_CBOX_ITEM cfgcbox89[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr_m61_1 = {CFG_CBOX,"Вкл. недоступен",0,2};
    __root const int IsOnNAStatus = 1;
    __root const CFG_CBOX_ITEM cfgcbox_m61_2[2]={"No","Yes"};

    __root const CFG_HDR cfghdr61_8={CFG_UINT,"Время до недоступен(мин)",0,150};
    __root const unsigned int NAStatusTime=20;    
    
    __root const CFG_HDR cfghdr6678 = {CFG_CBOX,"Включить автоматически при",0,13};
    __root const int isAutoStatusOn = 12;
    __root const CFG_CBOX_ITEM cfgcboxa1_153[13]={"Выключен","В сети", "Отошел", "Недоступен", "Не беспокоить", "Занят", "Готов болтать", "Невидимый", "Депрессия", "Злой", "Дома", "Жру","На работе"};

    
    __root const CFG_HDR cfghdr667={CFG_UINT,"Время (мин)",0,500};
    __root const unsigned int AUTO_ENGADE=20;
    
    __root const CFG_HDR cfghdrA_1 = {CFG_CBOX,"Посылать текст",0,2};
    __root const int Send_Auto_Status = 0;
    __root const CFG_CBOX_ITEM cfgcboxA_2[2]={"No","Yes"};
    
    
    __root const CFG_HDR cfghdr61_18={CFG_UINT,"Количество ответов",0,150};
    __root const unsigned int automsgcount=3;
    
    __root const CFG_HDR cfghdr_13={CFG_CBOX,"GMT зона",0,33};
    __root const int time_zone = 16;
    __root const CFG_CBOX_ITEM cfgcbox_13[33]={"-12:00", "-11:00", "-10:00", "-09:00", "-08:00", "-07:00", "-06:00", "-05:00", "-04:00", "-03:30", "-03:00", "-02:00", "-01:00", "00:00", "+01:00", "+02:00", "+03:00", "+03:30", "+04:00", "+04:30", "+05:00", "+05:30", "+05:45", "+06:00", "+06:30", "+07:00", "+08:00", "+09:00", "+09:30", "+10:00", "+11:00", "+12:00", "+13:00"};
    
    __root const CFG_HDR cfghdrzz_8={CFG_STR_WIN1251,"Сообщение приветствия",0,23};
    __root const char awayHelloMsg[24]="Привет";
    
    __root const CFG_HDR cfghdrzz_9={CFG_STR_WIN1251,"Сообщение отсутствия",0,255};
    __root const char awayMsg[256]="я сейчас не могу ответить!";
    
    __root const CFG_HDR cfghdr66_2 = {CFG_CBOX,"Отключение автостатуса после",0,2};
    __root const int Reset_timer = 0;
    __root const CFG_CBOX_ITEM cfgcbox90[2]={"Отправки сообщения","Нажатия любой клавиши"};
  
//Show away time on idle
    __root const CFG_HDR cfghdr663 = {CFG_CBOX,"Показывать на гэ",0,2};
    __root const int showAwayIdle = 0;
    __root const CFG_CBOX_ITEM cfgcbox82[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd6_7 = {CFG_COORDINATES,"Позиция",0,0};
    __root const unsigned int auto_x = 0;
    __root const unsigned int auto_y = 252;
  
__root const CFG_HDR cfghdr_m60={CFG_LEVEL,"",0,0};


//------------------------------------------

__root const CFG_HDR cfghdr_51={CFG_LEVEL,"Всплывающие сообщ.",1,0};

    __root const CFG_HDR cfghdr51_0 = {CFG_CBOX,"Показывать",0,4};
    __root const int PopupShow = 3;
    __root const CFG_CBOX_ITEM cfgcbox51_2[4]={"Ничего","Вход.сообш.","Подкл.конт.","Все"};
    
    __root const CFG_HDR cfghdr51_1={CFG_STR_UTF8,"Формат входящ.сооб.",0,8};
    __root const char PopupMsgString[9]="%s";
    
    __root const CFG_HDR cfghdr51_2={CFG_STR_UTF8,"Формат подкл.контакта",0,8};
    __root const char PopupOnlineString[9]=">%s<";  
    
    __root const CFG_HDR cfghdr51_3={CFG_UINT,"Шрифт всплыв.сообщ",0,1000};
    __root const unsigned int PopupFont=FONT_SMALL;
    
    __root const CFG_HDR cfghdr51_6={CFG_COLOR,"Фон всплыв.сообщ.",0,0};
    __root const char PopupBGCl[4]={0x00,0x00,0x00,100};
    
    __root const CFG_HDR cfghdr51_4={CFG_COLOR,"Цвет подкл.контакта",0,0};
    __root const char PopupOnlineCl[4]={0x00,0xFF,0x00,100};
    
    __root const CFG_HDR cfghdr51_5={CFG_COLOR,"Цвет входящ.сообщ.",0,0};
    __root const char PopupMsgCl[4]={0xFF,0xFF,0xFF,100};
    
    
    __root const CFG_HDR cfghdr51_9={CFG_CHECKBOX,"Показывать отключенных",0,2};
    __root const int isPopupOff = 1;
    
    __root const CFG_HDR cfghdr51_7={CFG_COLOR,"Фон отключившихся",0,0};
    __root const char PopupOffBGCl[4]={0x00,0x00,0x00,100};
    
    __root const CFG_HDR cfghdr51_8={CFG_COLOR,"Цвет отключившихся",0,0};
    __root const char PopupOffCl[4]={0xFF,0x00,0x00,100};

__root const CFG_HDR cfghdr_50={CFG_LEVEL,"",0,0};

//------------------------------------------




//History
__root const CFG_HDR cfghdr6={CFG_LEVEL,"History",1,0};
//Enable history logging
  __root const CFG_HDR cfghdr6_1={CFG_CBOX,"Enable logs",0,2};
  __root const int LOG_ALL = 1;
  __root const CFG_CBOX_ITEM cfgcbox6_1[2]={"No","Yes"};

//History type
  __root const CFG_HDR cfghdr6_2={CFG_CBOX,"History for",0,2};
  __root const int HISTORY_TYPE = 1;
  __root const CFG_CBOX_ITEM cfgcbox6_2[2]={"All","Everyone"};

//Enable status logging
  __root const CFG_HDR cfghdr6_3={CFG_CBOX,"Log status changes",0,2};
  __root const int LOG_STATCH = 0;
  __root const CFG_CBOX_ITEM cfgcbox6_3[2]={"No","Yes"};

//Enable X-status logging
  __root const CFG_HDR cfghdr6_4={CFG_CBOX,"Log X-Text",0,2};
  __root const int LOG_XTXT = 1;
  __root const CFG_CBOX_ITEM cfgcbox6_4[2]={"No","Yes"};
  
//Auto request X-status if entering to chat
  __root const CFG_HDR cfghdr6_5={CFG_CBOX,"Auto Request XText",0,2};
  __root const int ENA_AUTO_XTXT = 1;
  __root const CFG_CBOX_ITEM cfgcbox6_5[2]={"No","Yes"};

//Don't log X-status, if it same as prevision
  __root const CFG_HDR cfghdr6_6={CFG_CBOX,"Not log same XText",0,2};
  __root const int NOT_LOG_SAME_XTXT = 1;
  __root const CFG_CBOX_ITEM cfgcbox6_6[2]={"No","Yes"};

//Buffer for fill from history
  __root const CFG_HDR cfghdr6_7={CFG_CBOX,"History read buffer",0,7};
  __root const int HISTORY_BUFFER = 5;
  __root const CFG_CBOX_ITEM cfgcbox6_7[7]={"Disabled","128 bytes","256 bytes","512 bytes","1 kbyte","2 kbyte","4 kbyte"};
  
__root const CFG_HDR cfghdr7={CFG_LEVEL,"",0,0};


//Notify
__root const CFG_HDR cfghdr8={CFG_LEVEL,"Notify power",1,0};

  __root const CFG_HDR cfghdr8_1={CFG_UINT,"Sound Volume",0,6};
  __root const unsigned int sndVolume=3;

  __root const CFG_HDR cfghdr8_2={CFG_UINT,"Vibra power",0,100};
  __root const unsigned int vibraPower=30;

  __root const CFG_HDR cfghdr8_3={CFG_CBOX,"Vibration type",0,2};
  __root const int VIBR_TYPE = 0;
  __root const CFG_CBOX_ITEM cfgcbox8_3[2]={"Single","Double"};

  __root const CFG_HDR cfghdr8_4={CFG_CBOX,"Vibra on connect",0,2};
  __root const int VIBR_ON_CONNECT = 1;
  __root const CFG_CBOX_ITEM cfgcbox8_4[2]={"No","Yes"};

  __root const CFG_HDR cfghdr8_5={CFG_LEVEL,"Sounds setup",1,0};
    __root const CFG_HDR cfghdr8_5_1={CFG_STR_UTF8,"snd Startup",0,63};
    __root const char sndStartup[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndStartup.wav";
  
    __root const CFG_HDR cfghdr8_5_2={CFG_STR_UTF8,"snd SrvMsg",0,63};
    __root const char sndSrvMsg[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndSrvMsg.amr";
  
    __root const CFG_HDR cfghdr8_5_3={CFG_STR_UTF8,"snd Global",0,63};
    __root const char sndGlobal[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndGlobal.amr";
  
    __root const CFG_HDR cfghdr8_5_4={CFG_STR_UTF8,"snd Msg",0,63};
    __root const char sndMsg[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndMsg.amr";
  
    __root const CFG_HDR cfghdr8_5_5={CFG_STR_UTF8,"snd MsgSent",0,63};
    __root const char sndMsgSent[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndMsgSent.amr";
    
    __root const CFG_HDR cfghdr8_5_6={CFG_STR_UTF8,"Пинг",0,63};
    __root const char pingsnd[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndPing.amr";    
    
    __root const CFG_HDR cfghdr8_5_7={CFG_STR_UTF8,"Обрыв связи",0,63};
    __root const char sndLostConn[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\SndLostConn.amr";  
    
  __root const CFG_HDR cfghdr8_6={CFG_LEVEL,"",0,0};
  

__root const CFG_HDR cfghdr9={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr11_10={CFG_STR_WIN1251,"Мое имя",0,31};
#ifdef kluchnik
__root const char I_str[32]="pashka";
#else
__root const char I_str[32]="I";
#endif

__root const CFG_HDR cfghdr11_11={CFG_UINT,"Доп UIN в иконбар",0,0xFFFFFFFF};
__root const unsigned int ADDITION_UIN=0;
  
