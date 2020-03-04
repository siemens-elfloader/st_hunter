#include "..\inc\cfg_items.h"
#include "swilib.h"

__root const CFG_HDR cfghdr0 = {CFG_UINT, "Время обновления(мин)", 0, 120};
__root const unsigned int refresh_time = 60;

__root const CFG_HDR cfghdr2={CFG_STR_UTF8,"Путь к картинкам",0,63};
__root const char logpath[64]="4:\\weather.txt";

__root const CFG_HDR cfghdr3={CFG_STR_UTF8,"ид города",0,63};
__root const char id_tower[64]="1956";
