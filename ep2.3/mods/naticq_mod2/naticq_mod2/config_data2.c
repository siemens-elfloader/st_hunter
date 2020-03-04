#include "..\inc\cfg_items.h"
#include "language.h"
#include "swilib.h"
//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

#ifdef BCFG
__root const CFG_HDR cfghdr0={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr31={CFG_LEVEL,"Оригинальные настройки",1,0};

//Popup chat
  __root const CFG_HDR cfghdr4_2={CFG_CBOX,"Popup",0,2};
  __root const int DEVELOP_IF_MOD = 1;
  __root const CFG_CBOX_ITEM cfgcbox15_1[2]={"Use set orig.config", "After unlock"};
  
//history
    __root const CFG_HDR cfghdr6_8={CFG_CBOX,"Show Active contact",0,2};
    __root const int SHOW_ACTIVE = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_8[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr6_10={CFG_CBOX,"Save history with name",0,2};
    __root const int HISTORY_SAVE_TYPE = 0;
    __root const CFG_CBOX_ITEM cfgcbox6_9[2]={"No","Yes"};

//Buffer for fill from history
    __root const CFG_HDR cfghdr6_7={CFG_CBOX,"History read buffer",0,2};
    __root const int HISTORY_BUFFER_MOD = 1;
    __root const CFG_CBOX_ITEM cfgcbox6_7[2]={"Use set orig.conf.","4 kbyte"};    
    

    __root const CFG_HDR cfghdr181={CFG_CBOX,"Extended sounds",0,2};
    __root const unsigned int extended_sounds = 0;
    __root const CFG_CBOX_ITEM cfgcbox18_3_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr15={CFG_STR_UTF8,"add online msg path",0,63};
    __root const char sndGlobal2[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\Uins_on\\";
  
    __root const CFG_HDR cfghdr161={CFG_STR_UTF8,"add new msg path",0,63};
    __root const char sndMsg2[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\Uins_msg\\";
    
    __root const CFG_HDR cfghdr16r1={CFG_STR_UTF8,"ping sound",0,63};
    __root const char pingsnd[64]=DEFAULT_DISK ":\\ZBin\\NatICQ\\Sounds\\sndPing.wav";
    
    
__root const CFG_HDR cfghdr30={CFG_LEVEL,"",0,0};

//-----------------------------------------------------------------------------//
//                                НАСТРОЙКА КЛАВИШ
//-----------------------------------------------------------------------------//

//View settings
__root const CFG_HDR cfghdr23_4={CFG_LEVEL,"Настройка кнопок",1,0};


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




      __root const CFG_HDR cfghdr99_3_0={CFG_CBOX,"Длинная 0 в КЛ",0,14};
      __root const int longAct_0 = 10;
      __root const CFG_CBOX_ITEM cfgcbox99_4_0[14]={LG_NOT_ACTION,//0
                                                    LG_SOUND,//1
                                                    LG_VIBRA,//2
                                                    LG_INVIS,//3
                                                    LG_VIBRA_REM,//4
                                                    LG_CLAER_ALL_CHAT,//5
                                                    LG_AUTOMSG,//6
                                                    LG_SHOW_XSTATE_TIME,//7
                                                    LG_CONFIG,//8
                                                    LG_CONFIG_MOD,//9
                                                    LG_PING,//10
                                                    LG_HIHE_LOCK,//11
                                                    LG_IDLE,//12
                                                    LG_USERMENU//13
                                                  };

      __root const CFG_HDR cfghdr99_3_1={CFG_CBOX,"Длинная 1 в КЛ",0,14};
      __root const int longAct_1 = 1;
      __root const CFG_CBOX_ITEM cfgcbox99_4_1[14]={LG_NOT_ACTION,
                                                  LG_SOUND,
                                                  LG_VIBRA,
                                                  LG_INVIS,
                                                  LG_VIBRA_REM,
                                                  LG_CLAER_ALL_CHAT,
                                                  LG_AUTOMSG,
                                                  LG_SHOW_XSTATE_TIME,
                                                  LG_CONFIG,
                                                  LG_CONFIG_MOD,
                                                  LG_PING,
                                                  LG_HIHE_LOCK,
                                                  LG_IDLE,
                                                  LG_USERMENU
                                                };

      __root const CFG_HDR cfghdr99_3_2={CFG_CBOX,"Длинная 2 в КЛ",0,14};
      __root const int longAct_2 = 2;
      __root const CFG_CBOX_ITEM cfgcbox99_4_2[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_3={CFG_CBOX,"Длинная 3 в КЛ",0,14};
      __root const int longAct_3 = 3;
      __root const CFG_CBOX_ITEM cfgcbox99_4_3[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_4={CFG_CBOX,"Длинная 4 в КЛ",0,14};
      __root const int longAct_4 = 4;
      __root const CFG_CBOX_ITEM cfgcbox99_4_4[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_5={CFG_CBOX,"Длинная 5 в КЛ",0,14};
      __root const int longAct_5 = 5;
      __root const CFG_CBOX_ITEM cfgcbox99_4_5[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_6={CFG_CBOX,"Длинная 6 в КЛ",0,14};
      __root const int longAct_6 = 6;
      __root const CFG_CBOX_ITEM cfgcbox99_4_6[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_7={CFG_CBOX,"Длинная 7 в КЛ",0,14};
      __root const int longAct_7 = 7;
      __root const CFG_CBOX_ITEM cfgcbox99_4_7[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_8={CFG_CBOX,"Длинная 8 в КЛ",0,14};
      __root const int longAct_8 = 8;
      __root const CFG_CBOX_ITEM cfgcbox99_4_8[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_9={CFG_CBOX,"Длинная 9 в КЛ",0,14};
      __root const int longAct_9 = 9;
      __root const CFG_CBOX_ITEM cfgcbox99_4_9[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_10={CFG_CBOX,"Длинная * в КЛ",0,14};
      __root const int longAct_10 = 11;
      __root const CFG_CBOX_ITEM cfgcbox99_4_10[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_11={CFG_CBOX,"Длинная # в КЛ",0,14};
      __root const int longAct_11 = 12;
      __root const CFG_CBOX_ITEM cfgcbox99_4_11[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };
      
      __root const CFG_HDR cfghdr99_3_RB={CFG_CBOX,"Длинная вправо в КЛ",0,14};
      __root const int longAct_RB = 13;
      __root const CFG_CBOX_ITEM cfgcbox99_4_RB[14]={LG_NOT_ACTION,
                                                    LG_SOUND,
                                                    LG_VIBRA,
                                                    LG_INVIS,
                                                    LG_VIBRA_REM,
                                                    LG_CLAER_ALL_CHAT,
                                                    LG_AUTOMSG,
                                                    LG_SHOW_XSTATE_TIME,
                                                    LG_CONFIG,
                                                    LG_CONFIG_MOD,
                                                    LG_PING,
                                                    LG_HIHE_LOCK,
                                                    LG_IDLE,
                                                    LG_USERMENU
                                                  };

      __root const CFG_HDR cfghdr99_2={CFG_LEVEL,"",0,0}; //LONG PRESS ACTION
      
__root const CFG_HDR cfghdr223_5={CFG_LEVEL,"",0,0}; //НАСТРОКА КЛАВИШ
	
	
//-----------------------------------------------------------------------------//
//------------------------------- STATUS SETTING ------------------------------//
//-----------------------------------------------------------------------------//
  
__root const CFG_HDR cfghdr_45={CFG_LEVEL,"Настройка статусов",1,0};

//Status icon on mainscreen position
#ifdef NEWSGOLD
  #ifdef kluchnik
      __root const CFG_HDR cfghdr467_1 = {CFG_CBOX,"Показывать иконку статуса",0,5};
      __root const int ShowStatusIcon = 1;
      __root const CFG_CBOX_ITEM cfgcbox9_1[5]={"На главном экране","В иконбаре","В иконбаре(кроме гэ)","Везде","Нигде"};
  #else
      __root const CFG_HDR cfghdr467_1 = {CFG_CBOX,"Показывать иконку статуса",0,5};
      __root const int ShowStatusIcon = 0;
      __root const CFG_CBOX_ITEM cfgcbox9_1[5]={"На главном экране","В иконбаре","В иконбаре(кроме гэ)","Везде","Нигде"};
  #endif
#endif
        
//Show x-status on iconbar or mainscreen
#ifdef kluchnik
    __root const CFG_HDR cfghdrz_68 = {CFG_CBOX,"Показ.хстатус иконку",0,4};
    __root const int show_xstatus = 3;
    __root const CFG_CBOX_ITEM cfgcbox_8z[4]={"Нигде","На гэ","В иконбаре","Везде"};
#else
  #ifdef NEWSGOLD
      __root const CFG_HDR cfghdrz_68 = {CFG_CBOX,"Показ.хстатус иконку",0,4};
      __root const int show_xstatus = 1;
      __root const CFG_CBOX_ITEM cfgcbox_8z[4]={"Нигде","На гэ","В иконбаре","Везде"};
  #else
      __root const CFG_HDR cfghdrz_68 = {CFG_CBOX,"Показ.хстатус иконку",0,2};
      __root const int show_xstatus = 1;
      __root const CFG_CBOX_ITEM cfgcbox_8z[2]={"Нигде","На гэ"};
  #endif
#endif    
    __root const CFG_HDR cfghdrz_1={CFG_COORDINATES,"Позиция хстатус на гэ",0,0};
    __root const unsigned int IDLEICON_XX=2;
    __root const unsigned int IDLEICON_YX=70;
    
    __root const CFG_HDR cfghdr780_1={CFG_CBOX,"Индив.настройки хстатуса",0,2};
    __root const int ind_set_xstatus = 0;
    __root const CFG_CBOX_ITEM cfgcbox8_z2[2]={"No","Yes"};
    
#ifdef NEWSGOLD
//Number first pic status in folder img for show in iconbar
    __root const CFG_HDR cfghdr19_22={CFG_UINT,"1я картика статуса в иконбаре",0,65535};
    __root const unsigned int NumStatusToIconBar = 8000;  

//Number first pic x-status in folder img for show in iconbar    
    __root const CFG_HDR cfghdr19_242={CFG_UINT,"1я картика хстатуса в иконбаре",0,65535};
    __root const unsigned int NumXStatusToIconBar = 8100;  

    __root const CFG_HDR cfghdr_32_1={CFG_LEVEL,"Доп. статус в иконбар",1,0};
 
       __root const CFG_HDR cfghdr32_2={CFG_UINT,"UIN",0,0xFFFFFFFF};
       __root const unsigned int ADDITION_UIN=0;
       
       __root const CFG_HDR cfghdr32_3={CFG_CBOX,"Показывать статус",0,2};
       __root const int show_add_state = 0;
       __root const CFG_CBOX_ITEM cfgcbox32_4[2]={"No","Yes"};       

       __root const CFG_HDR cfghdr32_5={CFG_CBOX,"Показывать хстатус",0,2};
       __root const int show_add_xstate = 0;
       __root const CFG_CBOX_ITEM cfgcbox32_6[2]={"No","Yes"};       

   __root const CFG_HDR cfghdr_32_0={CFG_LEVEL,"",0,0};//ADDITION STATUS TO ICONBAR
#endif
   
__root const CFG_HDR cfghdr_4={CFG_LEVEL,"",0,0};//STATUS SETTINGS
  
  
//-----------------------------------------------------------------------------//
//-------------------------------- PNGFONT SETTING ----------------------------//
//-----------------------------------------------------------------------------//
  
  
__root const CFG_HDR cfghdr1_45={CFG_LEVEL,"Настройка пнгшрифта",1,0};
   
//Nember png-font or system
   __root const CFG_HDR cfghdr65z={CFG_UINT,"Номер шрифта (1=png/1xx=system)",0,115};
   __root const unsigned int fonto=1;
    
//Path to png-font
#ifdef kluchnik
   __root const CFG_HDR cfghdr98={CFG_STR_UTF8,"Путь к шрифту",0,63};
   __root const char fontpath[64]="4:\\elf\\naticq\\font\\";
#else
   __root const CFG_HDR cfghdr98={CFG_STR_UTF8,"Путь к шрифту",0,63};
   __root const char fontpath[64]="";
#endif
//Coordinates for message on idle
   __root const CFG_HDR cfghd83 = {CFG_COORDINATES,"Позиция на гэ",0,0};
   __root const unsigned int msg_d_x = 0;
   __root const unsigned int msg_d_y = 65;   
   
   __root const CFG_HDR cfghdrs19_1={CFG_UINT,"Показывать секунд",0,1000};
   __root const unsigned int main_msg_timer = 300;
   
   __root const CFG_HDR cfghdr60_2={CFG_COLOR,"Цвет шрифта",0,0};
   __root const char fontColor[4]={0xFF,0xFF,0xFF,100};
  
  __root const CFG_HDR cfghdr60_3={CFG_COLOR,"Цвет фона",0,0};
  __root const char bgrColor[4]={0x00,0x00,0x00,100};

//Max show symbol on idle
   __root const CFG_HDR cfghdr19a_1={CFG_UINT,"Количество символов",0,50};
   __root const unsigned int main_lenght = 24;
      
   __root const CFG_HDR cfghdr9z9={CFG_CBOX,"Использовать .. в тектсе ",0,2};
   __root const int use_cutter = 1;
   __root const CFG_CBOX_ITEM cfgcbox18a_4[2]={"No","Yes"};

   __root const CFG_HDR cfghdr65y={CFG_UINT,"Расстояние между буквами",0,999999};
   __root const unsigned int spaco1=1;
      
   //__root const CFG_HDR cfghdr65u={CFG_UINT,"Main msg space",0,999999};
   //__root const unsigned int spaco2=0;
    
__root const CFG_HDR cfghdrss_45={CFG_LEVEL,"",0,0};//PNGFONT SETTING
  

//-----------------------------------------------------------------------------//
//                             SMILES COLOR SETTING                            //
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdr41={CFG_LEVEL,"Цвета выбора смайлов",1,0};


  __root const CFG_HDR cfghdr40_1={CFG_COLOR,"Цвет фона",0,0};
  __root const char SmBGcolor[4]={0x00,0x00,0x00,100};
  
  __root const CFG_HDR cfghdr40_2={CFG_COLOR,"Цвет курсора",0,0};
  __root const char SmSelectColor[4]={0xFF,0x00,0x00,100};
  
  __root const CFG_HDR cfghdr40_3={CFG_COLOR,"Цвет текста смайлов",0,0};
  __root const char SmFontColor[4]={0x00,0xFF,0x00,100};

__root const CFG_HDR cfghdr40={CFG_LEVEL,"",0,0};

//-----------------------------------------------------------------------------//
//                                 IDLE SETTING
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_m71={CFG_LEVEL,"Настройка ГЭ",1,0};

    __root const CFG_HDR cfghdr699 = {CFG_COORDINATES,"Позиция времени",0,0};
    __root const unsigned int onlineTime_x = 77;
    __root const unsigned int onlineTime_y = 98;

    __root const CFG_HDR cfghdr698 = {CFG_CBOX,"Пауза при дисконнекте",0,2};
    __root const int Pause_Time = 1;
    __root const CFG_CBOX_ITEM cfgcbox85[2]={"No","Yes"};

    __root const CFG_HDR cfghd66 = {CFG_COORDINATES,"Координаты автопинга",0,0};
    __root const unsigned int Ping_X = 2;
    __root const unsigned int Ping_Y = 262;
    
    __root const CFG_HDR cfghdr1z9a_1={CFG_UINT,"Звук пинга после(times)",0,50};
    __root const unsigned int ping_time_config = 29;
    
    __root const CFG_HDR cfghdr298 = {CFG_CBOX,"Показывать непрочитанные",0,2};
    __root const int Is_Unread_On = 0;
    __root const CFG_CBOX_ITEM cfgcbox86[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd67 = {CFG_COORDINATES,"Координаты непрочитанных",0,0};
    __root const unsigned int unread_X = 99;
    __root const unsigned int unread_Y = 65;
    
    __root const CFG_HDR cfghdr71_3 = {CFG_CBOX,"Показывать подключ.",0,2};
    __root const int Is_Online_On = 0;
    __root const CFG_CBOX_ITEM cfgcbox71_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghd71_1 = {CFG_COORDINATES,"Подключ.контакты",0,0};
    __root const unsigned int online_x = 99;
    __root const unsigned int online_y = 65;    
    

__root const CFG_HDR cfghdr_m90={CFG_LEVEL,"",0,0};



//-----------------------------------------------------------------------------//
//                                AUTO STATUS                                  //
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_m61={CFG_LEVEL,"Автостатус",1,0};

    __root const CFG_HDR cfghdr668 = {CFG_CBOX,"Включить",0,2};
    __root const int Auto_Status = 1;
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



//-----------------------------------------------------------------------------//
//                             AUTO CHANGE STATUS                              //
//-----------------------------------------------------------------------------//


__root const CFG_HDR cfghdra1={CFG_LEVEL,"Автосмена статусов",1,0};

    __root const CFG_HDR cfghdra1_3={CFG_CBOX,"1 cтатус",0,13};
    __root const int status_1 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_13[13]={"Выключен","В сети", "Отошел", "Недоступен", "Не беспокоить", "Занят", "Готов болтать", "Невидимый", "Депрессия", "Злой", "Дома", "Жру","На работе"};

    __root const CFG_HDR cfghdra1_2={CFG_TIME,"Время смены",0,0};
    __root const TTime tm_status_1={15,0};   
    

    __root const CFG_HDR cfghdra1_1={CFG_CBOX,"2 cтатус",0,13};
    __root const int status_2 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_14[13]={"Выключен","В сети", "Отошел", "Недоступен", "Не беспокоить", "Занят", "Готов болтать", "Невидимый", "Депрессия", "Злой", "Дома", "Жру","На работе"};

    __root const CFG_HDR cfghdra1_4={CFG_TIME,"Время смены",0,0};
    __root const TTime tm_status_2={15,0};


    __root const CFG_HDR cfghdra1_5={CFG_CBOX,"3 cтатус",0,13};
    __root const int status_3 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_15[13]={"Выключен","В сети", "Отошел", "Недоступен", "Не беспокоить", "Занят", "Готов болтать", "Невидимый", "Депрессия", "Злой", "Дома", "Жру","На работе"};
    
    __root const CFG_HDR cfghdra1_6={CFG_TIME,"Время смены",0,0};
    __root const TTime tm_status_3={15,0};


    __root const CFG_HDR cfghdra1_7={CFG_CBOX,"4 cтатус",0,13};
    __root const int status_4 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_16[13]={"Выключен","В сети", "Отошел", "Недоступен", "Не беспокоить", "Занят", "Готов болтать", "Невидимый", "Депрессия", "Злой", "Дома", "Жру","На работе"};
    
    __root const CFG_HDR cfghdra1_8={CFG_TIME,"Время смены",0,0};
    __root const TTime tm_status_4={15,0};


    __root const CFG_HDR cfghdra1_9={CFG_CBOX,"5 cтатус",0,13};
    __root const int status_5 = 0;
    __root const CFG_CBOX_ITEM cfgcboxa1_17[13]={"Выключен","В сети", "Отошел", "Недоступен", "Не беспокоить", "Занят", "Готов болтать", "Невидимый", "Депрессия", "Злой", "Дома", "Жру","На работе"};
    
    __root const CFG_HDR cfghdra1_14={CFG_TIME,"Время смены",0,0};
    __root const TTime tm_status_5={15,0};


__root const CFG_HDR cfghdra0={CFG_LEVEL,"",0,0};
#ifdef NEWSGOLD
//------------------------------------------------------------------------------//
//                                SCREEN SAVER                                  //
//------------------------------------------------------------------------------//

__root const CFG_HDR cfghdr_91={CFG_LEVEL,"Заставка",1,0};

    __root const CFG_HDR cfghdr9_1 = {CFG_CBOX,"Включить заставку",0,2};
    __root const int isScreenSaverEna = 0;
    __root const CFG_CBOX_ITEM cfgcbox9_2[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdr9_3={CFG_UINT,"Время до срабатывания(1мин=12)",0,1000};
    __root const unsigned int screenSaverTime=120;

__root const CFG_HDR cfghdr_90={CFG_LEVEL,"",0,0};
#endif

//------------------------------------------------------------------------------//
//                                POPUP MESSAGE                                 //
//------------------------------------------------------------------------------//

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

__root const CFG_HDR cfghdr_50={CFG_LEVEL,"",0,0};



//------------------------------------------------------------------------------//
//                         SETTING CONTACTLIST HEADER                           //
//------------------------------------------------------------------------------//

__root const CFG_HDR cfghdr_s1={CFG_LEVEL,"Настройка заголовка КЛ",1,0};

//Show contact header with...
#ifdef kluchnik
    __root const CFG_HDR cfghdrs1_1 = {CFG_CBOX,"Показывать в заголовке с КЛ",0,6};
    __root const int TIME_OR_MSG = 4;
    __root const CFG_CBOX_ITEM cfgcboxs1[6]={"Время","Сообщения","Контакты","Все","Пнг шрифт(все)","Стандартно"};
#else
    __root const CFG_HDR cfghdrs1_3 = {CFG_CBOX,"Показывать в заголовке КЛ",0,6};
    __root const int TIME_OR_MSG = 3;
    __root const CFG_CBOX_ITEM cfgcboxs2[6]={"Время","Сообщения","Контакты","Все","Пнг шрифт(все)","Стандартно"};
#endif
    
    __root const CFG_HDR cfghdrs1_s1={CFG_UINT,"Время подкл./отключ.(сек)",0,1000};
    __root const unsigned int ON_OFF_TIME=10;
    
    
    __root const CFG_HDR cfghdrs1_15={CFG_CBOX,"Отображать траффик",0,2};
    __root const int ShowTraff = 1;
    __root const CFG_CBOX_ITEM cfgcboxs15[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_11={CFG_CBOX,"Отображать кол-во контактов",0,2};
    __root const int isShowCont = 1;
    __root const CFG_CBOX_ITEM cfgcboxs11[2]={"No","Yes"};

    __root const CFG_HDR cfghdrs1_12={CFG_CBOX,"Отображать кол-во непрочит.",0,2};
    __root const int isShowUnread = 1;
    __root const CFG_CBOX_ITEM cfgcboxs12[2]={"No","Yes"};

    __root const CFG_HDR cfghdrs1_4={CFG_CBOX,"Отображать вибру [V] ",0,2};
    __root const int isShowV = 1;
    __root const CFG_CBOX_ITEM cfgcboxs3[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_5={CFG_CBOX,"Отображать звук [S]",0,2};
    __root const int isShowS = 1;
    __root const CFG_CBOX_ITEM cfgcboxs5[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_6={CFG_CBOX,"Отображать автостатус [A]",0,2};
    __root const int isShowA = 1;
    __root const CFG_CBOX_ITEM cfgcboxs6[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_7={CFG_CBOX,"Отображать напоминалку [R]",0,2};
    __root const int isShowR = 0;
    __root const CFG_CBOX_ITEM cfgcboxs7[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_8={CFG_CBOX,"Отображать статус/время [Tm/Xs]",0,2};
    __root const int isShowTmXs = 0;
    __root const CFG_CBOX_ITEM cfgcboxs8[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_13={CFG_CBOX,"Отображать инвиз [I]",0,2};
    __root const int isShowI = 1;
    __root const CFG_CBOX_ITEM cfgcboxs13[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_9={CFG_CBOX,"Отображать заряд батареи",0,2};
    __root const int isShowBat = 0;
    __root const CFG_CBOX_ITEM cfgcboxs9[2]={"No","Yes"};
    
    __root const CFG_HDR cfghdrs1_10={CFG_CBOX,"Отображать время",0,2};
    __root const int isShowTime = 0;
    __root const CFG_CBOX_ITEM cfgcboxs10[2]={"No","Yes"};    
    
__root const CFG_HDR cfghdr_s0={CFG_LEVEL,"",0,0};


//------------------------------------------------------------------------------//
//                                EXTRA CONFIG                                  //
//------------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_m51={CFG_LEVEL,"Экстра опции",1,0};

//Vibra count in vibra remaider
    __root const CFG_HDR cfghdr65={CFG_UINT,"Кол-во вибраций в реж.напомин.",0,10};
    __root const unsigned int Vibra_cc=2;

#ifdef NEWSGOLD
//CSM for player
    __root const CFG_HDR cfghdr66={CFG_STR_UTF8,"CSM плеера",0,8};
    __root const char CsmAdr[9]="A087B454";  //el=A087B454 //es=A068E244
    
//Text before name track
    __root const CFG_HDR cfghdr67={CFG_STR_WIN1251,"Текст для вывода песни",0,12};
    __root const char playing[13]="MediaPlayer:";
#endif  
//Show command sending    
    __root const CFG_HDR cfghdr780={CFG_CBOX,"Отображать запрос команд",0,2};
    __root const int show_send_info = 1;
    __root const CFG_CBOX_ITEM cfgcbox8_z[2]={"No","Yes"};

    //isLogStatusChange
    __root const CFG_HDR cfghdr7_80={CFG_CBOX,"Показ. время изм.стат.контакта",0,2};
    __root const int isLogStatusChange = 1;
    __root const CFG_CBOX_ITEM cfgcbox8df_z[2]={"No","Yes"};
    
//Show Loopback contact in CL
    __root const CFG_HDR cfghdr71_30={CFG_CBOX,"Показывать Loopback в КЛ",0,2};
    __root const int IsShowLoopback = 1;
    __root const CFG_CBOX_ITEM cfgcbox_3z[2]={"No","Yes"};   
    
     
    __root const CFG_HDR cfghdr71_31={CFG_CBOX,"Открывать ссылки",0,2};
    __root const int IsOpenBrowser = 0;
    __root const CFG_CBOX_ITEM cfgcbox_32[2]={"No","Yes"};   
    
    __root const CFG_HDR cfghdr71_33={CFG_CBOX,"Мульти буффер",0,2};
    __root const int IsMultiBuffer = 0;
    __root const CFG_CBOX_ITEM cfgcbox_34[2]={"No","Yes"};   

    
__root const CFG_HDR cfghdr_m50={CFG_LEVEL,"",0,0};





//------------------------------------------------------------------------------//
//                          SETTING LOADING SCREEN                              //
//------------------------------------------------------------------------------//


__root const CFG_HDR cfghdr_q1={CFG_LEVEL,"Экран заргузки",1,0};

   __root const CFG_HDR cfghdr_s2 = {CFG_RECT, "Прогрессбар", 0, 0};
#ifdef ELKA
   __root const RECT PBRECT = {4,280,231,287};//LOadScreenRECT
#else
   __root const RECT PBRECT = {4,150,126,157};//LOadScreenRECT
#endif
   
  __root const CFG_HDR cfghdr_s3={CFG_COLOR,"Цвет прогрессбара",0,0};
  __root const char PBcolor[4]={0xFF,0x00,0x00,0x20};//ProgressBarColor
  
  __root const CFG_HDR cfghdr_s7={CFG_COLOR,"Фон прогрессбара",0,0};
  __root const char BGPBcolor[4]={0xFF,0xFF,0xFF,0x00};//BackGroundProgressBarColor

  __root const CFG_HDR cfghdr_s4={CFG_CBOX,"Менять софты",0,2};
  __root const int IsInvertSoftKey = 0;
  __root const CFG_CBOX_ITEM cfgcbox_s5[2]={"No","Yes"};  

  __root const CFG_HDR cfghdr_s6={CFG_CBOX,"Показывать инфу",0,2};
  __root const int IsShowInfo = 1;
  __root const CFG_CBOX_ITEM cfgcbox_s7[2]={"No","Yes"};    

__root const CFG_HDR cfghdr_q0={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr11_10={CFG_STR_UTF8,"Мое имя",0,31};
__root const char myName[32]="I";

__root const CFG_HDR cfghdr11_9={CFG_STR_UTF8,"Небольшой игнор лист",0,255};
__root const char ignor_list[256]="";


#endif
