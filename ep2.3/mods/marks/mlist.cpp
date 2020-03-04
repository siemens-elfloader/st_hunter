#include "main.h"
#include "list.h"
#include "mlist.h"

extern CList *list;

List * List::Active = NULL;

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
  List::Active->gHook(data, cmd);
}

void list_itemhndl(void * data, int curitem, void * unk)
{
  List::Active->ItemHandler(data, curitem, unk);
}

int list_keyhook(void * data, GUI_MSG *msg)
{
  return List::Active->onKey(data, msg);
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

int List::onKey(void * data, GUI_MSG * msg)
{
//  if(DownloadHandler::Top)
  {
    if(msg->keys == 0x3D)
    {
       readonly=0; 
       DrawText(NULL,fl);
    }
    if (msg->keys == 0x18)
    {
      return 1;
    
    }
    if (msg->gbsmsg->msg == KEY_DOWN)
    {
      switch(msg->gbsmsg->submess)
      {
    
      }
    }
  }
  return GUI_RESULT_OK;
}

void List::gHook(void *data, int cmd)
{
  if (cmd == TI_CMD_FOCUS)
  {
    DisableIDLETMR();
    int n = list->Count();//DownloadHandler::Top->GetNumOfDownloads();
    Menu_SetItemCountDyn(data, n);
  }
}


void List::ItemHandler(void * data, int curitem, void * unk)
{
  WSHDR * ws1, * ws;
  void * item = AllocMLMenuItem(data);

  MNU *fl;
  
  fl = list->Items(curitem);//FindFLISTtByN(curitem);
  ws=AllocWS(strlen(fl->dat));  
  if(fl->name)
  {
    ws1=AllocMenuWS(data,strlen(fl->name)+4);
    CutWSTR(ws1,0);
    ascii2ws(ws1,fl->name);
    if(fl->is_first==1)
    {
      wsInsertChar(ws1,0x0007,1);
      wsInsertChar(ws1,0xE008,1);
     }
     wsprintf(ws,per_s,fl->dat);
  }
  else
   {
    ws=AllocMenuWS(data,10);
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

void List::Show()
{
  patch_header(&list_hdr);
  
  list_sk[0].lgp_id = (int)"Delete";
  list_sk[1].lgp_id = (int)"Exit";
  
  gui_id = CreateMultiLinesMenu(NULL, NULL, &list_desc, &list_hdr, NULL, list->Count());
}

List::List()
{
  Active = this;
  gui_id = NULL;
}

List::~List()
{
}
