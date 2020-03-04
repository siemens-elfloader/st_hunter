#ifndef _ITEMS_H_
  #define _ITEMS_H_

typedef struct
{
  void *next;
  char item[64];
  int pos;
}LIST;

typedef struct
{
  void *next;
  char Text[128];
  char Description[128];
  char IconSmall[128];
  char IconBig[128];
  LIST *IconAnim;
  char Run[128];
}ITEM;


void AddToItem(char *name,char *description,char *iconsmall,char *iconbig, char *run);
ITEM *GetItem(int curitem);
void FreeItemsList();
int TotalAnim(int curitem);
char *IconAnim(int curitem, int pic);
int TotalItems();

#endif
