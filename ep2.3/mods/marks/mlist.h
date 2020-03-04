#ifndef _LIST_H_
#define _LIST_H_


class List
{
public:
  List();
  ~List();
  
  void Show();
  int onKey(void * data, GUI_MSG * msg);
  void gHook(void * data, int cmd);
  void ItemHandler(void * data, int curitem, void * unk);
  int gui_id;
  
  static List * Active;
};

#endif

