//#include <vcl.h>
//#include "Unit1.h"
#include "../inc/swilib.h"
#include "List.h"

/*
  (c)kluchnik 04.03.2009
  v0.1
  Сделал на досуге, для общего развития, да и думаю будет полезно...

  Возможности:
    Добавление записей в конец списка
    Добавление элементов любую позицию списка
    Удаление элементов
    Изменение элементов
    Перемещение
*/

const char _d[]="%d";

int CList::add(LIST_STRUCT* item)
{
  if(!item) return (-1);
  if(!this->toplist) this->toplist = item;
  else
  {
    LIST_STRUCT *top = this->toplist;
    while(top->next) top = top->next;
    top->next = item;
  }
  return (1);
}

int CList::add(char *name)
{
  if(!name) return -1;
  LIST_STRUCT *l = new LIST_STRUCT();
  l->next = 0;
  l->name = name;
  this->add(l);
  return 0;
}


int CList::add(int d)
{
  LIST_STRUCT *l = new LIST_STRUCT();
  l->next = 0;
  l->name = (char*)malloc(32);
  sprintf(l->name,_d,d);
  this->add(l);
  return 0;
}

int CList::Insert(LIST_STRUCT* new_item,int pos)
{
  if(!this->toplist) return -1;
  if(pos > this->Count()) return -2;
  
  LIST_STRUCT* prev_item = NULL;
  LIST_STRUCT* cur_item = this->toplist;

  int i=0;
  while(/*cur_item!=NULL && */(i++ < pos))
  {
    prev_item = cur_item;
    cur_item = cur_item->next;
  }

  
  if(prev_item == NULL)
  {
    new_item->next = toplist;
    toplist = new_item;
  }
  else
  {
    prev_item->next = new_item;
    new_item->next = cur_item;
  }
  return 0;
}

int CList::Insert(char* new_item,int pos)
{
  LIST_STRUCT *insert = new LIST_STRUCT;
  insert->next=0;
  insert->name = new_item;
 return (this->Insert(insert,pos));
}

int CList::Delete(LIST_STRUCT *del_item,int pos)
{
  if(!this->toplist) return -1;
  if(pos > this->Count()) return -2;
  
  LIST_STRUCT* prev_item = NULL;
  LIST_STRUCT* tmp_item = NULL;
  LIST_STRUCT* cur_item = this->toplist;
  int i = 0;

   //если в качестве аргумента для поиска передана структура,то ищем по ней
  // и\или если в качестве позиции указано -1

  //удалить первый итем
  //strcmp грубовато...надо придумать что то другое
  if(((del_item || pos == -1) ? (!strcmp(del_item->name,cur_item->name)) : (pos == 0)))
  {
    tmp_item = toplist;
    toplist = toplist->next;
    delete tmp_item;
    //free(tmp_item);
    return 0;
  }
  else
  {
    prev_item = NULL;
    cur_item = cur_item;

    while(cur_item!=NULL)
    {
      if(((del_item || pos == -1) ? (!strcmp(del_item->name,cur_item->name))
                                  : (pos == i++))) break;
      prev_item = cur_item;
      cur_item = cur_item->next;
    }

    if(cur_item!=NULL)
    {
      tmp_item = cur_item;
      prev_item->next = cur_item->next;
      delete tmp_item;
      //free(tmp_item);
    }
  }
  return 0;
}


//              a,b,c,d
// Swap(0,2) -> c,b,a,d
int CList::Swap(int source,int desc)
{
  if(desc > this->Count()) return -1;

  LIST_STRUCT *s_item,*d_item;

  s_item = this->Items(source);
  d_item = this->Items(desc);

  LIST_STRUCT *t = new LIST_STRUCT();
  t->name = d_item->name;
  t->next = 0;

  LIST_STRUCT *tt = new LIST_STRUCT();
  tt->name = s_item->name;
  tt->next = 0;

  Change(t,source);
  Change(tt,desc);
  return 0;
}


void CList::Change(LIST_STRUCT *change,int item)
{
   if(!change) return;
   if(!this->toplist) return;
   if(item > this->Count()) return;

   LIST_STRUCT *prev = NULL;
   LIST_STRUCT *cur = toplist;
   LIST_STRUCT *tmp = NULL;

   int i = 0;
   while(i++ < item/* && cur!=NULL*/)
   {
     prev = cur;
     cur = cur->next;
   }

  if(prev == NULL)//первый элемент в списке 
  {
    Delete(0,0);
    change->next = toplist;
    toplist = change;
  }
  else
   if(cur!=NULL)
   {
     tmp = cur;
     change->next = cur->next;
     prev->next = change;
     delete tmp;
   }
}

void CList::Change(char *change,int item)
{
  if(!change) return;
  LIST_STRUCT *ch = new LIST_STRUCT();
  ch->next=0;
  ch->name = change;
  this->Change(ch,item);
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
  int i = 0;
  while(t)
  {
    if(i++ == curitem) return t;
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

