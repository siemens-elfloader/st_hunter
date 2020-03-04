#ifndef _SMILES_LOADER_
#define _SMILES_LOADER_

#include "../naticq.h"

typedef struct
{
  void *next;
  int index;
  IMGHDR *img;
}ICONS;

typedef struct
{
  int index;
  int w;
  int h;
}POS;

#endif
