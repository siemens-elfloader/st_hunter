#include "..\inc\cfg_items.h"
//Конфигурация

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

__root const CFG_HDR cfghdr0={CFG_UINT,"UIN",0,0xFFFFFFFF};
__root const unsigned int UIN=0;


__root const CFG_HDR cfghdr1={CFG_UINT,"TEST",0,0xFFFFFFFF};
__root const unsigned int TEST=0;
