#ifndef _STANDART_LIST_H_
#define _STANDART_LIST_H_

#include "../inc/swilib.h"
#include "List.h"

#define GUI_RESULT_REDRAW (-1)
#define GUI_RESULT_CLOSE  ( 1)
#define GUI_RESULT_OK     ( 0)

class StandartList
{
public:
  StandartList();
  ~StandartList();
  
  void Show();
  void Show(char *left, char *right);
  int onKey(void * data, GUI_MSG * msg);
  void gHook(void * data, int cmd);
  void ItemHandler(void * data, int curitem, void * unk);
  int gui_id;
  
  static StandartList * Active;
  
  CList *list;
};

#endif

