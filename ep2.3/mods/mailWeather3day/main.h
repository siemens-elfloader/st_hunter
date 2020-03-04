#include "..\inc\swilib.h"

const int minus11=-11;

typedef struct
{
  CSM_RAM csm;
}MAIN_CSM;

extern void kill_data(void *p, void (*func_p)(void *));

typedef struct
{
  char town[32];
  char date[32];
  char daystate[16];//утро,день,вечер,ночь
  char temp[32];
  char speed[32];
  char davl[32];
  char vlazn[32];
  char picname[32];
  char prognoze[64];
}WEATHER;

