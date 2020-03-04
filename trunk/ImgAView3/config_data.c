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

//void SetFTsettings (int load, int render, int res, int h)

__CFG_BEGIN(0)

 __root const CFG_HDR cfghdr0 = {CFG_UINT, "CFG_SLIDE_TIMEOUT 1/10", 1, 100};
 __root const unsigned int CFG_SLIDE_TIMEOUT = 10;

 __root const CFG_HDR cfghdr1 = {CFG_UINT, "CFG_MOVE_STEP", 1, 100};
 __root const unsigned int CFG_MOVE_STEP = 10;

 __root const CFG_HDR cfghdr2={CFG_COLOR,"CFG_BG_COLOR",0,0};
__root const char CFG_BG_COLOR[4]={0x80,0x80,0x80,0x64};

 __root const CFG_HDR cfghdr2_1={CFG_COLOR,"CFG_BORDER_COLOR",0,0};
__root const char CFG_BORDER_COLOR[4]={0x80,0x80,0x80,0x32};

__root const CFG_HDR cfghdr3={CFG_CBOX,"CFG_SAVE_FORMAT", 0, 2};
__root const int CFG_SAVE_FORMAT = 0;
__root const CFG_CBOX_ITEM cfgcbox1_1[2]={"JPEG", "PNG"};
/*
__root const CFG_HDR cfghdr4={CFG_CBOX,"RENDER MODE", 0, 3};
__root const int CFG_RENDER_MODE = 1;
__root const CFG_CBOX_ITEM cfgcbox3_1[3]={"FT_RENDER_MODE_NORMAL", "FT_RENDER_MODE_LIGHT", "FT_RENDER_MODE_MONO"};

 __root const CFG_HDR cfghdr5 = {CFG_UINT, "RESOLUTION H/V", 0, 300};
 __root const unsigned int CFG_RESOLUTION = 96;
 */

__CFG_END(0)




/** Правая кнопка по config_data.c
  * Properties -> Advanced -> Use custom command to build this file
  * Ставим галочку
  * Вставляем туда $compiler $options -xc $includes -c $file -o $object -O0
*/
