#include "../inc/swilib.h"
#include "List.h"

const char _d[]="%d";

//CList *list;

int CList::add(LIST_STRUCT* item)
{
  if(!item) return (-1);
  if(!this->toplist)
  {
    this->toplist = item;
    this->toplist->index = 0;
  }
  else
  {
    LIST_STRUCT *top = this->toplist;
    while(top->next) top = top->next;
    item->index = top->index+1;
    top->next = item;
    item->prev = top;
  }
  return (1);
}
/*
int CList::add(char *name)
{
  if(!name) return -1;
  LIST_STRUCT *l = new LIST_STRUCT();
  l->next = 0;
  l->prev = 0;
  l->name = name;
  this->add(l);
  return 0;
}
*/
int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}


int CList::add(int numbmk,char *name,char *descript,char *icon)
{
   BOOKMARKS *bmk = new BOOKMARKS();
   bmk->next = 0;
   bmk->prev = 0;
   bmk->name = (char*)malloc(strlen(name)+5);
   bmk->descript = descript;
   bmk->desc_len = strlen(descript);
  
   bmk->icon=CreateIMGHDRFromPngFile(icon, 2);   
   bmk->icon_size = get_file_size(icon);
   sprintf(bmk->name,"%d. %s",numbmk,name);
   return (this->add(bmk));
}


int CList::add(int d)
{
  LIST_STRUCT *l = new LIST_STRUCT();
  l->next = 0;
  l->prev = 0;
  l->name = (char*)malloc(32);
  sprintf(l->name,_d,d);
  this->add(l);
  return 0;
}


void CList::FreeList()
{
  LIST_STRUCT *t = this->toplist;
  this->toplist = NULL;

  while(t/* && t->next*/)
  {
    LIST_STRUCT *t_prev = t;
    t = t->next;
    delete t_prev;
    //free(t_prev);
  }
}

CList::CList()
{
  this->toplist = 0;
  this->list = 0;
}

CList::~CList()
{
  this->FreeList();
}


LIST_STRUCT* CList::Items(int curitem)
{
  LIST_STRUCT* t = this->toplist;
//  int i = 0;
  while(t)
  {
    if(/*i++*/t->index == curitem) return t;
    t = (LIST_STRUCT*)t->next;
  }
  return 0;
}

int CList::Count()
{
  LIST_STRUCT* t = this->toplist;
  int i = 0;
  while(t)
  {
    t = (LIST_STRUCT*)t->next;
    i++;
  }
  return i;
}

