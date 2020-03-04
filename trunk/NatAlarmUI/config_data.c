#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

/** Адаптация конфига под гсс
 * (с) Z.Vova
 */

// на случай если несколько конфигов
//__CFG_BEGIN(1)
//__CFG_BEGIN(2)
// и так далее

__CFG_BEGIN(0)

//Конфигурация

__root const CFG_HDR cfghdr0={CFG_STR_UTF8,"Путь к alarmclock.pd",0,127};
__root const char alarmclock[128]="1:\\system\\alarmclock.pd";

 __root const CFG_HDR cfghdr1 = {CFG_UINT, "CFG_FONT_SIZE", 0, 100};
 __root const unsigned int CFG_FONT_SIZE = 17;

__root const CFG_HDR cfghdr2 = {CFG_UINT, "CFG_FONT_TIME_SIZE", 0, 100};
 __root const unsigned int CFG_FONT_TIME_SIZE = 50;

  __root const CFG_HDR cfghdr4={CFG_UINT,"CFG_ALARM_ICON_NUM",0,6000};
__root const unsigned int  CFG_ALARM_ICON_NUM=0x003;

__root const CFG_HDR cfghdr5={CFG_CBOX,"CFG_DISABLE_ICONBAR", 0, 2};
__root const unsigned int CFG_DISABLE_ICONBAR = 1;
__root const CFG_CBOX_ITEM cfgcbox5_1[2]={"NO", "YES"};

__root const CFG_HDR cfghdr6={CFG_CBOX,"CFG_SHOW_CLOCK", 0, 2};
__root const unsigned int CFG_SHOW_CLOCK = 1;
__root const CFG_CBOX_ITEM cfgcbox6_1[2]={"NO", "YES"};

//----------------------------Настройки позиционирования----------------------------------
__root const CFG_HDR cfghdrm15={CFG_LEVEL,"Настройки позиционирования",1,0};
__root const CFG_HDR cfghdr15_1={CFG_COORDINATES,"ALARM_TIME_Coordinates",0,0};
__root const unsigned int ALARM_TIME_x=95;
__root const unsigned int ALARM_TIME_y=75;

__root const CFG_HDR cfghdr15_2={CFG_COORDINATES,"ALARM_ICON_Coordinates",0,0};
__root const unsigned int ALARM_ICON_x=6;
__root const unsigned int ALARM_ICON_y=52;


__root const CFG_HDR cfghdr15_5={CFG_COORDINATES,"ALARM_ON_Coordinates",0,0};
__root const unsigned int ALARM_ON_x=22;
__root const unsigned int ALARM_ON_y=155;


__root const CFG_HDR cfghdr15_6={CFG_COORDINATES,"REPEAT_ON_Coordinates",0,0};
__root const unsigned int REPEAT_ON_x=22;
__root const unsigned int REPEAT_ON_y=180;


__root const CFG_HDR cfghdr15_7={CFG_COORDINATES,"REPEAT_TIME_Coordinates",0,0};
__root const unsigned int REPEAT_TIME_x=24;
__root const unsigned int REPEAT_TIME_y=210;


__root const CFG_HDR cfghdr15_91={CFG_COORDINATES,"DAYS_Coordinates",0,0};
__root const unsigned int DAYS_x=22;
__root const unsigned int DAYS_y=257;

__root const CFG_HDR cfghdr15_92={CFG_UINT,"CFG_WIDTH_BETWEEN_DAYS",0,50};
__root const unsigned int CFG_WIDTH_BETWEEN_DAYS=28;


/*
__root const CFG_HDR cfghdr1={CFG_CBOX,"LOAD MODE", 0, 3};
__root const int CFG_LOAD_MODE = 2;
__root const CFG_CBOX_ITEM cfgcbox1_1[3]={"FT_LOAD_DEFAULT", "FT_LOAD_TARGET_MONO", "FT_LOAD_TARGET_LIGHT"};

__root const CFG_HDR cfghdr2={CFG_CBOX,"RENDER MODE", 0, 3};
__root const int CFG_RENDER_MODE = 1;
__root const CFG_CBOX_ITEM cfgcbox2_1[3]={"FT_RENDER_MODE_NORMAL", "FT_RENDER_MODE_LIGHT", "FT_RENDER_MODE_MONO"};

 __root const CFG_HDR cfghdr3 = {CFG_UINT, "RESOLUTION H/V", 0, 300};
 __root const unsigned int CFG_RESOLUTION = 96;
 */

__CFG_END(0)




/** Правая кнопка по config_data.c
  * Properties -> Advanced -> Use custom command to build this file
  * Ставим галочку
  * Вставляем туда $compiler $options -xc $includes -c $file -o $object -O0
*/
