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


 __root const CFG_HDR cfghdr0={CFG_STR_UTF8,"Адрес исполения", 3, 8};
 __root const char entry[9]="AXXXXXXX";

__CFG_END(0)


/** Правая кнопка по config_data.c
  * Properties -> Advanced -> Use custom command to build this file
  * Ставим галочку
  * Вставляем туда $compiler $options -xc $includes -c $file -o $object -O0
*/
