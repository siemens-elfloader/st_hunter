//#include <vcl.h>
//#include "Unit1.h"
#include "../inc/swilib.h"
#include "main.h"
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

int CList::Delete(LIST_STRUCT *del_item,int pos = -1)
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


//==============================================================
//                           menu
//==============================================================


extern CList *list;

MenuList * MenuList::Active = NULL;

SOFTKEY_DESC list_sk[]=
{
  {0x0018,0x0000,NULL},
  {0x0001,0x0000,NULL},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB list_skt=
{
  list_sk,0
};

void list_ghook(void * data, int cmd)
{
  MenuList::Active->gHook(data, cmd);
}

void list_itemhndl(void * data, int curitem, void * unk)
{
  MenuList::Active->ItemHandler(data, curitem, unk);
}

int list_keyhook(void * data, GUI_MSG *msg)
{
  return MenuList::Active->onKey(data, msg);
}

HEADER_DESC list_hdr={0,0,0,0, NULL, NULL, LGP_NULL};

const int list_softkeys[]={0,1,2};

static const ML_MENU_DESC list_desc=
{
  8, list_keyhook, list_ghook, NULL,
  list_softkeys,
  &list_skt,
  0x200 + 0x11,
  list_itemhndl,
  NULL,   // Items
  NULL,   // Procs
  NULL,   // n
  1       // Добавочных строк  
};

int MenuList::onKey(void * data, GUI_MSG * msg)
{
  {
    int i;
    i=GetCurMenuItem(data);
    if(msg->keys == 0x3D)
    {
    }
    
    if (msg->keys == 0x18)
    {
      return 1;    
    }
    
    if (msg->gbsmsg->msg == KEY_DOWN)
    {
      switch(msg->gbsmsg->submess)
      {
        case '#':
          {
            ///list->Delete(NULL,i);
            extern void DelItem(MNU *);
            DelItem(list->Items(i));
            Menu_SetItemCountDyn(data,list->Count());
            SetCursorToMenuItem(data,0);
            RefreshGUI();            
            break;
          }
      }
    }
  }
  return 0;
}

void MenuList::gHook(void *data, int cmd)
{
 /* if (cmd == TI_CMD_FOCUS)
  {
    DisableIDLETMR();
    int n = list->Count();//DownloadHandler::Top->GetNumOfDownloads();
    Menu_SetItemCountDyn(data, n);
  }*/
}


void MenuList::ItemHandler(void * data, int curitem, void * unk)
{
  WSHDR * ws1, * ws;
  void * item = AllocMLMenuItem(data);

//  ws1=AllocMenuWS(data,strlen("error")+4);
  MNU *fl;
  
   //for(int i=0 ; i<qlist->Count(); i++)
//     ShowMSG(1,(int)qlist->Items(curitem-1)->name); 
  fl = list->Items(curitem);//FindFLISTtByN(curitem);
//  ShowMSG(1,(int)fl->name);
  ws = AllocWS(64);  
  if(fl && fl->name)
  {
    ws1=AllocMenuWS(data,64);
    CutWSTR(ws1,0);
    ///ascii2ws(ws1,fl->name);
    wsprintf(ws1,"%s", fl->name);
    wsprintf(ws,fl->dat);
  }
  else
   {
     ws = AllocMenuWS(data,10);
     wsprintf(ws,"error");
     wsInsertChar(ws,2,1);
   } 
  
    SetMLMenuItemText(data, item, ws1, ws, curitem);
/*  WSHDR * ws1, * ws2, * ws3;
  void * item = AllocMLMenuItem(data);
  //Download * dl = DownloadHandler::Top->GetDownloadbyN(curitem);
  if(dl && dl->file_name)
  {
    ws1 = AllocMenuWS(data, strlen(dl->file_name) + 1);
    str_2ws(ws1, dl->file_name, strlen(dl->file_name));
  }
  else
  {
    ws1 = AllocMenuWS(data, 32);
    ascii2ws(ws1, "No name");
  }
  ws2 = AllocMenuWS(data, 32);

  SetMLMenuItemText(data, item, ws1, ws2, curitem);*/
}


void MenuList::Show()
{  
  patch_header(&list_hdr);
  
  list_sk[0].lgp_id = (int)"Delete";
  list_sk[1].lgp_id = (int)"Exit";
  
  gui_id = CreateMultiLinesMenu(NULL, NULL, &list_desc, &list_hdr, NULL, list->Count());
}

MenuList::MenuList()
{
  Active = this;
  gui_id = NULL;
}

MenuList::~MenuList()
{
}
