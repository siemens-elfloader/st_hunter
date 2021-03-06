#ifndef _LIST_H_
#define _LIST_H_

/*
typedef struct _LIST_STRUCT
  {
    char *name;
    _LIST_STRUCT *next;
  }LIST_STRUCT,S_LIST;

*/
typedef struct _LIST_STRUCT
  {
    _LIST_STRUCT *next;
    _LIST_STRUCT *prev;
    int index;
    char *name;
    char *descript;
    int desc_len;
    IMGHDR *icon;
    int icon_size;
  }LIST_STRUCT,BOOKMARKS;


class CList
{
private:
  LIST_STRUCT *toplist, *list;
public:
  int  add(LIST_STRUCT*);
//  int  add(char *name);
  int add(int numbmk,char *name,char *descript,char *icon);
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

#endif

