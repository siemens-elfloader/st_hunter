#ifndef _MAIN_H_
  #define _MAIN_H_


typedef struct {
  void *next;
  char *title;
  char *description;
}RSS_ITEM;

extern RSS_ITEM *rss_first;
extern RSS_ITEM *rss_last;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
}MAIN_GUI;

#endif
