#ifndef _CONTACT_LIST_H_
#define _CONTACT_LIST_H_

#include "include.h"
#include "gui.h"


typedef struct
{
  void *next;
  void *prev;
  unsigned int isgroup;
  unsigned int uin;
  unsigned int group;
  unsigned int status;
  char name[64];
  
}CLIST;

class UsersList
{
public:
  UsersList();
  ~UsersList();
  void AddUser(char *name, unsigned int isgroup, unsigned int uin, unsigned int group);
  
  int GetCount();
  CLIST * GetItem(int curitem);
private:
  CLIST *Itemtop;
  void FreeItemsList();
};




class ContactList
{
public:
  ContactList();
  ~ContactList();
  
  UsersList *list;
  
  void Show();
  int onKey(void * data, GUI_MSG * msg);
  void gHook(void * data, int cmd);
  void ItemHandler(void * data, int curitem, void * unk);
  int gui_id;
  
  static ContactList * Active;
private:

};

#endif

