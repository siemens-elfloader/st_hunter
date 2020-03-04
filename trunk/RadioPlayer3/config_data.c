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

__root const CFG_HDR cfghdr0={CFG_CBOX,"CFG_DISABLE_ICONBAR", 0, 2};
__root const unsigned int CFG_DISABLE_ICONBAR = 1;
__root const CFG_CBOX_ITEM cfgcbox5_1[2]={"NO", "YES"};

 __root const CFG_HDR cfghdr2 = {CFG_UINT, "CFG_FONT_SIZE", 0, 100};
 __root const unsigned int CFG_FONT_SIZE = 16;

__root const CFG_HDR cfghdr15_1={CFG_COORDINATES,"CFG_ICON",0,0};
__root const unsigned int  CFG_ICON_X=0;
__root const unsigned int  CFG_ICON_Y=92;

__root const CFG_HDR cfghdr15_11={CFG_COORDINATES,"CFG_PLAY_ICON",0,0};
__root const unsigned int  CFG_PLAY_ICON_X=10;
__root const unsigned int  CFG_PLAY_ICON_Y=230;

__root const CFG_HDR cfghdr15_2={CFG_COORDINATES,"CFG_SNAME",0,0};
__root const unsigned int  CFG_SNAME_X=0;
__root const unsigned int  CFG_SNAME_Y=60;

__root const CFG_HDR cfghdr15_3={CFG_COORDINATES,"CFG_FREQ",0,0};
__root const unsigned int  CFG_FREQ_X=0;
__root const unsigned int  CFG_FREQ_Y=153;

__root const CFG_HDR cfghdr15_4={CFG_COORDINATES,"CFG_VOL",0,0};
__root const unsigned int  CFG_VOL_X=0;
__root const unsigned int  CFG_VOL_Y=245;

__root const CFG_HDR cfghdr15_5={CFG_COORDINATES,"CFG_LEVEL",0,0};
__root const unsigned int  CFG_LEVEL_X=0;
__root const unsigned int  CFG_LEVEL_Y=0;

__root const CFG_HDR cfghdr15_6={CFG_COORDINATES,"CFG_STATUS",0,0};
__root const unsigned int  CFG_STATUS_X=0;
__root const unsigned int  CFG_STATUS_Y=130;



__CFG_END(0)




/** Правая кнопка по config_data.c
  * Properties -> Advanced -> Use custom command to build this file
  * Ставим галочку
  * Вставляем туда $compiler $options -xc $includes -c $file -o $object -O0
*/
