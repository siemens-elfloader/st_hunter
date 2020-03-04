#ifndef _LIST_H_
#define _LIST_H_

typedef struct _LIST_STRUCT
  {
     char *filename;
     char *name;
     char *text;
     char *dat;
     int is_first;
    _LIST_STRUCT *next;
  }LIST_STRUCT,S_LIST, MNU;


class CList
{
private:
  LIST_STRUCT *toplist, *list;
public:
  int  add(LIST_STRUCT*);
  int  add(char *name);
  int  add(int d);
  int  Insert(LIST_STRUCT *new_item,int pos);
  int  Insert(char *new_item,int pos);
  int  Delete(LIST_STRUCT *del_item,int pos);
  void Change(LIST_STRUCT *change,int item);
  void Change(char *change,int item);
  int  Swap(int source,int desc);
  LIST_STRUCT* Items(int curitem);
  LIST_STRUCT* gettop() {return toplist;};
  int Count();
  void FreeList();

  CList();
  ~CList();
};


class MenuList
{
public:
  MenuList();
  ~MenuList();
  
  void Show();
  int onKey(void * data, GUI_MSG * msg);
  void gHook(void * data, int cmd);
  void ItemHandler(void * data, int curitem, void * unk);
  int gui_id;
  
  static MenuList * Active;
};


#endif

