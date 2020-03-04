#include "..\inc\cfg_items.h"
#include "language.h"
#include "..\inc\swilib.h"
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
  __root const char NATICQ_HOST[128]="cbsie.dyndns.info; freegw.alltime.ru; ig.boba.su; himoza.zyns.com";

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
  
#ifdef ELKA  
  __root const CFG_HDR cfghdr2_10={CFG_STR_UTF8,"Small images .png path",0,63};
  __root const char ICON_PATH2[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\img2";
#endif

  __root const CFG_HDR cfghdr2_8={CFG_STR_UTF8,"XStatus .png path",0,63};
  __root const char XSTATUSES_PATH[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\ximg";

  __root const CFG_HDR cfghdr2_9={CFG_STR_UTF8,"Templates path",0,63};
  __root const char TEMPLATES_PATH[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\templates";
  
__root const CFG_HDR cfghdr3={CFG_LEVEL,"",0,0};


//View settings
__root const CFG_HDR cfghdr4={CFG_LEVEL,"Interface",1,0};
  
//Popup chat
  __root const CFG_HDR cfghdr4_2={CFG_CBOX,"Popup",0,4};
  __root const int DEVELOP_IF = 0;
  __root const CFG_CBOX_ITEM cfgcbox4_2[4]={"Allways", "If Kbd Unlock","Never","After unlock"};



  __root const CFG_HDR cfghdrCL_1={CFG_LEVEL,"Контакт лист",1,0};
  
    //Sorting
     __root const CFG_HDR cfghdr4_3={CFG_CBOX,"Сортировка",0,2};
     __root const int SORT_CLIST = 0;
     __root const CFG_CBOX_ITEM cfgcbox4_3[2]={"By Name","By Status"};
   
     
     __root const CFG_HDR cfghdrI_2={CFG_CHECKBOX,"Два столбца",0,2};
    #ifdef ELKA
     __root const int VIEW_MODE = 1;
    #else
     __root const int VIEW_MODE = 0;
    #endif
              
     __root const CFG_HDR cfghdrI_3={CFG_CHECKBOX,"Две линии",0,2};
     __root const int LINE_MODE = 0;
      
     __root const CFG_HDR cfghdrI_7={CFG_CBOX,"Показывать",0,3};
     __root const unsigned int SOFT_MODE = 1;
     __root const CFG_CBOX_ITEM cfgcboxI_8[3]={"Ничего","Софты","Инфу"};
      
     __root const CFG_HDR cfghdrI_5={CFG_CHECKBOX,"Статусы в иконбаре",0,2};
     __root const unsigned int ShowStatusToIconBar = 1;
      
     __root const CFG_HDR cfghdrI_6={CFG_CHECKBOX,"Пинг в иконбаре",0,2};
     __root const unsigned int PingToIconBar = 0;

    
     __root const CFG_HDR cfghdrI_8={CFG_CBOX,"Показывать отправку",0,4};
     __root const unsigned int ShowProcessSend = 1;
     __root const CFG_CBOX_ITEM cfgcboxI_9[4]={"Ничего","Цифрами","Линией","Цифры+Цвет"};  
      
     __root const CFG_HDR cfghdr6_8={CFG_CBOX,"Показывать активные чаты",0,2};
     __root const int SHOW_ACTIVE = 1;
     __root const CFG_CBOX_ITEM cfgcbox6_8[2]={"No","Yes"};  
      
     __root const CFG_HDR cfghdrI_10={CFG_CBOX,"Выделять активные чаты",0,2};
     __root const int SelBoldActiveChat = 1;
     __root const CFG_CBOX_ITEM cfgcboxI_11[2]={"Обычным","Жирным"};  
        
     __root const CFG_HDR cfghdrI_9={CFG_CHECKBOX,"Подсветка при подключении конт.",0,2};
     __root const unsigned int IllByONCantact = 1;  
  
     
     //CONTACT LIST HEADER
      __root const CFG_HDR cfghdr_s1={CFG_LEVEL,"Заголовок КЛ",1,0};        
         
            __root const CFG_HDR cfghdrs1_s1={CFG_UINT,"Время подкл./отключ.(сек)",0,1000};
            __root const unsigned int ON_OFF_TIME=10;  
          
          
            __root const CFG_HDR cfghdrHead_1={CFG_CHECKBOX,"Статус",0,2};
            __root const int HEAD_STATUS = 1;
          
            __root const CFG_HDR cfghdrHead_2={CFG_CHECKBOX,"Х-Статус",0,2};
            __root const int HEAD_XSTATUS = 1;
          
            __root const CFG_HDR cfghdrHead_3={CFG_CHECKBOX,"Приват.статус",0,2};
            __root const int HEAD_PRIVATE = 1;
          
            __root const CFG_HDR cfghdrHead_4={CFG_CBOX,"Пинг",0,3};
            __root const int HEAD_PING = 1;
            __root const CFG_CBOX_ITEM cfgcboxs15[3]={"Картинка","Текст","Ничего"};
            
            __root const CFG_HDR cfghdrs1_15={CFG_CHECKBOX,"Отображать траффик",0,2};
            __root const int ShowTraff = 1;
            
            __root const CFG_HDR cfghdrs1_11={CFG_CHECKBOX,"Отображать кол-во контактов",0,2};
            __root const int isShowCont = 1;
          
            __root const CFG_HDR cfghdrs1_12={CFG_CHECKBOX,"Отображать кол-во непрочит.",0,2};
            __root const int isShowUnread = 1;
          
            __root const CFG_HDR cfghdrs1_4={CFG_CHECKBOX,"Отображать вибру [V] ",0,2};
            __root const int isShowV = 1;
            
            __root const CFG_HDR cfghdrs1_5={CFG_CHECKBOX,"Отображать звук [S]",0,2};
            __root const int isShowS = 1;
            
            __root const CFG_HDR cfghdrs1_6={CFG_CHECKBOX,"Отображать автостатус [A]",0,2};
            __root const int isShowA = 1;
            
            __root const CFG_HDR cfghdrs1_7={CFG_CHECKBOX,"Отображать напоминалку [Vr]",0,2};
            __root const int isShowR = 1;            
            
            __root const CFG_HDR cfghdrs1_9={CFG_CHECKBOX,"Отображать заряд батареи",0,2};
            __root const int isShowBat = 0;
          
        __root const CFG_HDR cfghdr_s0={CFG_LEVEL,"",0,0};///CL Header        
        
        
     __root const CFG_HDR cfghdrCl_1={CFG_LEVEL,"Цвета",1,0};   
     
         __root const CFG_HDR cfghdrrCl_2={CFG_COLOR,"Цвет фона КЛ",0,0};
         __root const char CLBackgrClr[4]={0x00,0x00,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_3={CFG_COLOR,"Цвет софтов",0,0};
         __root const char SoftBackgrClr[4]={0xFF,0x48,0x00,0x1B};   
         
         __root const CFG_HDR cfghdrrCl_4={CFG_COLOR,"Цвет надписей софтов",0,0};
         __root const char SoftFontClr[4]={0xFF,0xFF,0xFF,0x64};
         
         __root const CFG_HDR cfghdrrCl_5={CFG_COLOR,"Цвет заголовка",0,0};
         __root const char HeadBackgrClr[4]={0xFF,0x48,0x00,0x1B};   
         
         __root const CFG_HDR cfghdrrCl_6={CFG_COLOR,"Цвет надписей заголовка",0,0};
         __root const char HeadFontClr[4]={0xFF,0xFF,0xFF,0x64};
         
         __root const CFG_HDR cfghdrrCl_7={CFG_COLOR,"Цвет скроллбара",0,0};
         __root const char ScrollBarClr[4]={0xFF,0x00,0x00,0x15};
         
         __root const CFG_HDR cfghdrrCl_8={CFG_COLOR,"Цвет курсора",0,0};
         __root const char ClCursorClr[4]={0xFF,0x00,0x00,0x32};
         
         __root const CFG_HDR cfghdrrCl_9={CFG_COLOR,"Цвет рамки курсора",0,0};
         __root const char ClCursorBordClr[4]={0x00,0x00,0x00,0x60};
         
         __root const CFG_HDR cfghdrrCl_10={CFG_COLOR,"Цвет активных",0,0};
         __root const char ActiveClr[4]={0xFF,0xFF,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_11={CFG_COLOR,"Цвет неактивных",0,0};
         __root const char NotActiveClr[4]={0x00,0xFF,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_16={CFG_COLOR,"Цвет групп",0,0};
         __root const char GroupClr[4]={0xFF,0x00,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_17={CFG_COLOR,"Цвет подписей",0,0};
         __root const char CLDescricpClr[4]={0xFF,0x00,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_12={CFG_COLOR,"Фон доп.инфо",0,0};
         __root const char AddInfoClr[4]={0xFF,0x00,0x00,0x64};
         
         __root const CFG_HDR cfghdrrCl_13={CFG_COLOR,"Шрифт доп.инфо",0,0};
         __root const char FontInfoClr[4]={0xFF,0xFF,0xFF,0x64};
         
         __root const CFG_HDR cfghdrrCl_14={CFG_COLOR,"Цвет не доставленных",0,0};
         __root const char UnAckedClr[4]={0xFF,0xFF,0x0,0x15};
         
         __root const CFG_HDR cfghdrrCl_15={CFG_COLOR,"Цвет отправленных",0,0};
         __root const char SendClr[4]={0x00,0xFF,0x00,0x15};
     
    __root const CFG_HDR cfghdrCl_0={CFG_LEVEL,"",0,0};//Color in cl

  __root const CFG_HDR cfghdrCL_0={CFG_LEVEL,"",0,0};  //Contact list
  
  
  // Main menu
   __root const CFG_HDR cfghdrM_1={CFG_LEVEL,"Главное меню",1,0};
  
      __root const CFG_HDR cfghdrrM_4={CFG_COLOR,"Цвет фона",0,0};
      __root const char MMBackgrClr[4]={0x00,0x00,0x00,0x45};//"\x00\x00\x00\x45"
      
      __root const CFG_HDR cfghdrrM_5={CFG_COLOR,"Цвет обводки",0,0};
      __root const char MMBackgrBordClr[4]={0xFF,0xFF,0xFF,0x64};
           
      __root const CFG_HDR cfghdrrM_2={CFG_COLOR,"Цвет курсора",0,0};
      __root const char MMCursorBGClr[4]={0x00,0x00,0x00,0x00};
      
      __root const CFG_HDR cfghdrrM_3={CFG_COLOR,"Цвет обводки курсора",0,0};
      __root const char MMCursorClr[4]={0xFF,0xFF,0xFF,0x64}; 
      

      __root const CFG_HDR cfghdrM_6 = {CFG_RECT, "Меню", 0, 0};
#ifdef ELKA
      __root const RECT MMRect = {0,60,179,303};
#else
      __root const RECT MMRect = {0,26,125,164};
#endif
      
      __root const CFG_HDR cfghdrM_7 = {CFG_RECT, "Статусы", 0, 0};
      __root const RECT SRect = {30,YDISP+2,190,260};
      
      __root const CFG_HDR cfghdrM_8 = {CFG_RECT, "Х-Статусы", 0, 0};
      __root const RECT XSRect = {30,YDISP+2,231,300};   
      
       __root const CFG_HDR cfghdrM_9={CFG_CHECKBOX,"Показывать цифры",0,2};
       __root const unsigned int ShowNumbMenu = 1;    
   
  __root const CFG_HDR cfghdrM_0={CFG_LEVEL,"",0,0};//
  
  
  //--------------------------------
  
  
  __root const CFG_HDR cfghdr_e1={CFG_LEVEL,"Пнг шрифт",1,0};//level       
          
          __root const CFG_HDR cfghdr23 = {CFG_STR_UTF8, "Путь к 1 png шрифту", 0, 127};
          __root const char fontpath2[128] = "";
          
          __root const CFG_HDR cfghdr24 = {CFG_STR_UTF8, "Путь к 2 png шрифту", 0, 127};
          __root const char fontpath3[128] = "";        
          
          __root const CFG_HDR cfghdre1_2 = {CFG_STR_UTF8, "png шрифт заголовка", 0, 127};
          __root const char fontpath4[128] = "";
          
          __root const CFG_HDR cfghdr_e1_3={CFG_CHECKBOX,"Пнг шрифт заголовка",0,2};
          __root const int PNG_HEADER = 1;
        
          __root const CFG_HDR cfghdr_e1_4={CFG_CHECKBOX,"Пнг шрифт софтов",0,2};
          __root const int PNG_SOFTKEY = 1;
    
    __root const CFG_HDR cfghdr_e0={CFG_LEVEL,"",0,0};// 
     
     
    //-----------------------------------
    
    
    __root const CFG_HDR cfghdrF_1={CFG_LEVEL,"Системный шрифт",1,0};//level     
     
          __root const CFG_HDR cfghdrF_2_2={CFG_CHECKBOX,"Использовать сист. шрифт",0,2};
          __root const int isSystemFont = 1;
          
          __root const CFG_HDR cfghdrF_3 = {CFG_UINT, "Шрифт", 0, 15};  
          __root const unsigned int pngFont = FONT_SMALL;
          
          __root const CFG_HDR cfghdrF_4 = {CFG_UINT, "Шрифт заголовка", 0, 15};  
          __root const unsigned int HeadFont = FONT_SMALL;
    
    __root const CFG_HDR cfghdrF_5={CFG_LEVEL,"",0,0};//     
  
//-----------------------------------------//

  
  __root const CFG_HDR cfghdrC1={CFG_LEVEL,"Chat",1,0};
    //First letter when typing
      __root const CFG_HDR cfghdr4_4={CFG_CBOX,"First letter",0,2};
      __root const int FIRST_LETTER = 0;
      __root const CFG_CBOX_ITEM cfgcbox4_4[2]={"Small","Big"};
      
      
      __root const CFG_HDR cfghdr7_80={CFG_CHECKBOX,"Show time change stat.",0,2};
      __root const int isLogStatusChange = 1;
       
      __root const CFG_HDR cfghdr71_31={CFG_CHECKBOX,"Open URL",0,2};
      __root const int IsOpenBrowser = 0;
    
      __root const CFG_HDR cfghdr71_33={CFG_CHECKBOX,"MultBuffer",0,2};
      __root const int IsMultiBuffer = 1;
    
    
      //Fonts and colors
      __root const CFG_HDR cfghdr4_5={CFG_LEVEL,"Font and colors",1,0};
      
        __root const CFG_HDR cfghdr4_5_1={CFG_UINT,"My string color",0,1000};
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
        
      __root const CFG_HDR cfghdr4_6={CFG_LEVEL,"",0,0};  //Fonts and colors
      
  __root const CFG_HDR cfghdrC0={CFG_LEVEL,"",0,0};//Chat

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
  
  __root const CFG_HDR cfghdr4_8={CFG_LEVEL,"",0,0};  //illumination
 
  
  
      //********* НАСТРОЙКА КЛАВИШ ************//

    //View settings
    __root const CFG_HDR cfghdr23_4={CFG_LEVEL,"Клавиши",1,0};    
    
      //Action on # button in contact list
        __root const CFG_HDR cfghdr78_4520={CFG_CBOX,"Действие на # в КЛ",0,2};
        __root const int ActionResh = 0;
        __root const CFG_CBOX_ITEM cfgcbox_668_z[2]={"Очистить выделенный чат","След.акт.чат"};
        
    
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
    
    
    
    
__root const CFG_HDR cfghdr5={CFG_LEVEL,"",0,0};//Interface

//History
__root const CFG_HDR cfghdr6={CFG_LEVEL,"History",1,0};
//Enable history logging
  __root const CFG_HDR cfghdr6_1={CFG_CBOX,"Enable logs",0,2};
  __root const int LOG_ALL = 1;
  __root const CFG_CBOX_ITEM cfgcbox6_1[2]={"No","Yes"};

//History type
  __root const CFG_HDR cfghdr6_2={CFG_CBOX,"History for",0,2};
  __root const int HISTORY_TYPE = 0;
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
  __root const int HISTORY_BUFFER = 3;
  __root const CFG_CBOX_ITEM cfgcbox6_7[7]={"Disabled","128 bytes","256 bytes","512 bytes","1 kbyte","2 kbyte","4 byte"};
  
__root const CFG_HDR cfghdr7={CFG_LEVEL,"",0,0};


//Notify
__root const CFG_HDR cfghdr8={CFG_LEVEL,"Notify power",1,0};

  __root const CFG_HDR cfghdr8_1={CFG_UINT,"Sound Volume",0,6};
  __root const unsigned int sndVolume=3;

  __root const CFG_HDR cfghdr8_2={CFG_UINT,"Vibra power",0,100};
  __root const unsigned int vibraPower=100;

  __root const CFG_HDR cfghdr8_3={CFG_CBOX,"Vibration type",0,2};
  __root const int VIBR_TYPE = 0;
  __root const CFG_CBOX_ITEM cfgcbox8_3[2]={"Single","Double"};

  __root const CFG_HDR cfghdr8_4={CFG_CBOX,"Vibra on connect",0,2};
  __root const int VIBR_ON_CONNECT = 0;
  __root const CFG_CBOX_ITEM cfgcbox8_4[2]={"No","Yes"};

  __root const CFG_HDR cfghdr8_5={CFG_LEVEL,"Sounds setup",1,0};
    
  __root const CFG_HDR cfghdr8_5_1={CFG_STR_UTF8,"snd Startup",0,63};
    __root const char sndStartup[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndStartup.wav";
  
    __root const CFG_HDR cfghdr8_5_2={CFG_STR_UTF8,"snd SrvMsg",0,63};
    __root const char sndSrvMsg[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndSrvMsg.wav";
  
    __root const CFG_HDR cfghdr8_5_3={CFG_STR_UTF8,"snd Global",0,63};
    __root const char sndGlobal[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndGlobal.wav";
  
    __root const CFG_HDR cfghdr8_5_4={CFG_STR_UTF8,"snd Msg",0,63};
    __root const char sndMsg[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndMsg.wav";
  
    __root const CFG_HDR cfghdr8_5_5={CFG_STR_UTF8,"snd MsgSent",0,63};
    __root const char sndMsgSent[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndMsgSent.wav";
    
    __root const CFG_HDR cfghdr8_5_6={CFG_STR_UTF8,"Пинг",0,63};
    __root const char pingsnd[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndPing.amr";    
    
    __root const CFG_HDR cfghdr8_5_7={CFG_STR_UTF8,"Обрыв связи",0,63};
    __root const char sndLostConn[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\SndLostConn2.amr";  
    
  __root const CFG_HDR cfghdr8_6={CFG_LEVEL,"",0,0};
  

__root const CFG_HDR cfghdr9={CFG_LEVEL,"",0,0};



//Status setting
__root const CFG_HDR cfghdrS_1={CFG_LEVEL,"Статусы",1,0};

//Status icon on mainscreen position
#ifdef NEWSGOLD
      __root const CFG_HDR cfghdrS_2 = {CFG_CBOX,"Показывать иконку статуса",0,5};
      __root const int ShowStatusIcon = 1;
      __root const CFG_CBOX_ITEM cfgcboxS_3[5]={"На главном экране","В иконбаре","В иконбаре(кроме гэ)","Везде","Нигде"};
#endif
        
//Show x-status on iconbar or mainscreen
#ifdef NEWSGOLD
      __root const CFG_HDR cfghdrS_4 = {CFG_CBOX,"Показ.хстатус иконку",0,4};
      __root const int show_xstatus = 2;
      __root const CFG_CBOX_ITEM cfgcboS_5[4]={"Нигде","На гэ","В иконбаре","Везде"};
#else
      __root const CFG_HDR cfghdrS_6 = {CFG_CBOX,"Показ.хстатус иконку",0,2};
      __root const int show_xstatus = 1;
      __root const CFG_CBOX_ITEM cfgcboxS_7[2]={"Нигде","На гэ"};
#endif

  //Status icon on mainscreen position
    __root const CFG_HDR cfghdr4_1={CFG_COORDINATES,"Позиция статусa на гэ",0,0};
    __root const unsigned int IDLEICON_X=0;
    __root const unsigned int IDLEICON_Y=50;
  
    __root const CFG_HDR cfghdrS_8={CFG_COORDINATES,"Позиция хстатус на гэ",0,0};
    __root const unsigned int IDLEICON_XX=2;
    __root const unsigned int IDLEICON_YX=70;    
   
#ifdef NEWSGOLD
    __root const CFG_HDR cfghdrS_9={CFG_CBOX,"Мигать при сообщении",0,2};
    __root const int blink_mess = 0;
    __root const CFG_CBOX_ITEM cfgcboxS_10[2]={"No","Yes"};
    
//Number first pic status in folder img for show in iconbar
    __root const CFG_HDR cfghdrS_11={CFG_UINT,"1я картика статуса в иконбаре",0,65535};
    __root const unsigned int NumStatusToIconBar = 8000;  

//Number first pic x-status in folder img for show in iconbar    
    __root const CFG_HDR cfghdrS_13={CFG_UINT,"1я картика хстатуса в иконбаре",0,65535};
    __root const unsigned int NumXStatusToIconBar = 8100;  

    __root const CFG_HDR cfghdrS_14={CFG_LEVEL,"Доп. статус в иконбар",1,0};
 
       __root const CFG_HDR cfghdrS_15={CFG_UINT,"UIN",0,0xFFFFFFFF};
       __root const unsigned int ADDITION_UIN=0;
    
       __root const CFG_HDR cfghdrS_16={CFG_CHECKBOX,"Показывать при активном",0,2};
       __root const int ShowAddStatusToIconBar = 0;
       
       __root const CFG_HDR cfghdrS_17={CFG_CHECKBOX,"Показывать статус",0,2};
       __root const int show_add_state = 0;

       __root const CFG_HDR cfghdrS_18={CFG_CHECKBOX,"Показывать хстатус",0,2};
       __root const int show_add_xstate = 0;

   __root const CFG_HDR cfghdrS_19={CFG_LEVEL,"",0,0};//ADDITION STATUS TO ICONBAR
#endif
   
__root const CFG_HDR cfghdrS_0={CFG_LEVEL,"",0,0};//STATUS SETTINGS



__root const CFG_HDR cfghdr41={CFG_LEVEL,"Cмайлы",1,0};


  __root const CFG_HDR cfghdrSm_5={CFG_CBOX,"Стандартные клавиши",0,2};
  __root const int UseStandartSmileSel = 0;
  __root const CFG_CBOX_ITEM cfgcboxSm_6[2]={"No","Yes"};  
       
  __root const CFG_HDR cfghdr40_1={CFG_COLOR,"Цвет фона",0,0};
  __root const char SmBGcolor[4]={0xFF,0xFF,0xFF,100};
  
  __root const CFG_HDR cfghdr40_2={CFG_COLOR,"Цвет курсора",0,0};
  __root const char SmSelectColor[4]={0xFF,0x00,0x00,0x2A};
  
  __root const CFG_HDR cfghdr40_3={CFG_COLOR,"Цвет текста смайлов",0,0};
  __root const char SmFontColor[4]={0x00,0x00,0x00,100};

__root const CFG_HDR cfghdr40={CFG_LEVEL,"",0,0};



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



__root const CFG_HDR cfghdr_q1={CFG_LEVEL,"Экран заргузки",1,0};

   __root const CFG_HDR cfghdr_s2 = {CFG_RECT, "Прогрессбар", 0, 0};
#ifdef ELKA
   __root const RECT PBRECT = {1,280,238,287};//LOadScreenRECT
#else
   __root const RECT PBRECT = {4,150,126,157};//LOadScreenRECT
#endif
   
  __root const CFG_HDR cfghdr_s3={CFG_COLOR,"Цвет прогрессбара",0,0};
  __root const char PBcolor[4]={0xFF,0x00,0x00,0x20};//ProgressBarColor
  
  __root const CFG_HDR cfghdr_s7={CFG_COLOR,"Фон прогрессбара",0,0};
  __root const char BGPBcolor[4]={0xFF,0xFF,0xFF,0x00};//BackGroundProgressBarColor

  __root const CFG_HDR cfghdr_s4={CFG_CBOX,"Менять софты",0,2};
  __root const int IsInvertSoftKey = 1;
  __root const CFG_CBOX_ITEM cfgcbox_s5[2]={"No","Yes"};  

  __root const CFG_HDR cfghdr_s6={CFG_CBOX,"Показывать инфу",0,2};
  __root const int IsShowInfo = 1;
  __root const CFG_CBOX_ITEM cfgcbox_s7[2]={"No","Yes"};    

__root const CFG_HDR cfghdr_q0={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr11_10={CFG_STR_WIN1251,"Мое имя",0,31};
__root const char I_str[32]="I";
