#include "list.h"
#include "mlist.h"
#include "patch_rect.h"


StandartList * StandartList::Active = NULL;

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
  StandartList::Active->gHook(data, cmd);
}

void list_itemhndl(void * data, int curitem, void * unk)
{
  StandartList::Active->ItemHandler(data, curitem, unk);
}

int list_keyhook(void * data, GUI_MSG *msg)
{
  return StandartList::Active->onKey(data, msg);
}

HEADER_DESC list_hdr={0,0,0,0, NULL, (int)"bI", LGP_NULL};

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

int StandartList::onKey(void * data, GUI_MSG * msg)
{
    if(msg->keys == 0x3D)
    {       
      ShowMSG(1,(int)list->Items(GetCurMenuItem(data))->name);
    }
    
    if (msg->keys == 0x18)
    {
      return 1;    
    }
    if (msg->gbsmsg->msg == KEY_DOWN)
    {
      switch(msg->gbsmsg->submess)
      {
        case RED_BUTTON: return 1;
      }
    }
  return GUI_RESULT_OK;
}

void StandartList::gHook(void *data, int cmd)
{
  if (cmd == TI_CMD_FOCUS)
  {
    DisableIDLETMR();
    int n = list->Count();
    Menu_SetItemCountDyn(data, n);
  }
}


void StandartList::ItemHandler(void * data, int curitem, void * unk)
{
  WSHDR * ws1, * ws;
  void * item = AllocMLMenuItem(data);

  LIST_STRUCT *fl;  
  fl = list->Items(curitem);//FindFLISTtByN(curitem);
  ws = AllocWS(strlen("Test"));  
  
  if(fl->name)
  {
    ws1=AllocMenuWS(data,strlen(fl->name)+4);
    CutWSTR(ws1,0);
    //ascii2ws
    wsprintf(ws1,fl->name);
    wsprintf(ws,"Test");
  }
  else
   {
     ws=AllocMenuWS(data,10);
     wsprintf(ws,"error");
     wsInsertChar(ws,2,1);
   }   
  
    SetMLMenuItemText(data, item, ws1, ws, curitem);
}

void StandartList::Show()
{
  patch_header(&list_hdr);
  
  list_sk[0].lgp_id = (int)"Ok";
  list_sk[1].lgp_id = (int)"Exit";
  
  gui_id = CreateMultiLinesMenu(NULL, NULL, &list_desc, &list_hdr, NULL, list->Count());
}


void StandartList::Show(char *left, char *right)
{
  patch_header(&list_hdr);
  
  list_sk[0].lgp_id = (int)left;
  list_sk[1].lgp_id = (int)right;
  
  gui_id = CreateMultiLinesMenu(NULL, NULL, &list_desc, &list_hdr, NULL, list->Count());
}

StandartList::StandartList()
{
  Active = this;
  gui_id = NULL;
}

StandartList::~StandartList()
{
  delete list;
}
