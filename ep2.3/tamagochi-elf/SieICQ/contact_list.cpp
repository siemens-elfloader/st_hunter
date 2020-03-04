
#include "include.h"
#include "rect_patcher.h"
#include "contact_list.h"
#include "sieicqmain.h"
#include "langpack.h"


/*******************************************************************************
UsersList
*******************************************************************************/
UsersList::UsersList()
{
  Itemtop=NULL;
}

UsersList::~UsersList()
{
}

void UsersList::AddUser(char *name, unsigned int isgroup,unsigned int uin,unsigned int group)
{
 CLIST *bmk=(CLIST*)malloc(sizeof(CLIST));
 bmk->next=NULL;
 bmk->prev=NULL;
 sprintf(bmk->name,name);
 bmk->isgroup=isgroup;
 bmk->uin=uin;
 bmk->group=group;
 bmk->status=0;
 
 if(!Itemtop)
 {
   Itemtop=bmk; 
 }
 else
 {
   CLIST *Item=Itemtop;
   while(Item->next)
       Item=(CLIST*)Item->next;
   Item->next=bmk;   
 } 
}

CLIST *UsersList::GetItem(int curitem)
{
  CLIST *bmk;
  bmk=(CLIST*)Itemtop;
  int i=0;
  while(bmk)
  {
    if(i==curitem)
      return bmk;
    i++;
    if(bmk->next) bmk=(CLIST*)bmk->next;  
    else return 0;
  }
  return 0;
}
  
int UsersList::GetCount()
{
  if(!Itemtop) return 0;
  CLIST *bmk;
  bmk=Itemtop;
  int i=1; 
  while(bmk=(CLIST*)bmk->next) i++;
  return i;
}

void UsersList::FreeItemsList()
{  
  LockSched();
  CLIST *bmk=(CLIST *)Itemtop;
  Itemtop=NULL;
  UnlockSched();
  while(bmk)
  {
    CLIST *bmk_prev=bmk;
    bmk=(CLIST*)bmk->next;
    mfree(bmk_prev);
  }
}

/*******************************************************************************
ContactList
*******************************************************************************/

ContactList * ContactList::Active = NULL;

SOFTKEY_DESC ContactList_sk[]=
{
  {0x0018,0x0000,NULL},
  {0x0001,0x0000,NULL},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB ContactList_skt=
{
  ContactList_sk,0
};

void ContactList_ghook(void * data, int cmd)
{
  ContactList::Active->gHook(data, cmd);
}

void ContactList_itemhndl(void * data, int curitem, void * unk)
{
  ContactList::Active->ItemHandler(data, curitem, unk);
}

int ContactList_keyhook(void * data, GUI_MSG *msg)
{
  return ContactList::Active->onKey(data, msg);
}

HEADER_DESC ContactList_hdr={0,0,0,0, NULL, NULL, LGP_NULL};

const int ContactList_softkeys[]={0,1,2};

static const ML_MENU_DESC ContactList_desc=
{
  8, ContactList_keyhook, ContactList_ghook, NULL,
  ContactList_softkeys,
  &ContactList_skt,
  0x200 + 0x11,
  ContactList_itemhndl,
  NULL,   // Items
  NULL,   // Procs
  NULL,   // n
  1       // Добавочных строк  
};

int ContactList::onKey(void * data, GUI_MSG * msg)
{
  /*
  Download * dl;
  if(DownloadHandler::Top)
  {
    if(msg->keys == 0x3D)
    {
      if(dl = DownloadHandler::Top->GetDownloadbyN(GetCurMenuItem(data)))
      {
        LogWidget * log = new LogWidget(dl->log); // Открываем окно просмотра лога
        log->Create();
      }
    }
    if (msg->keys == 0x18)
    {
      dl = DownloadHandler::Top->GetDownloadbyN(GetCurMenuItem(data));
      ListOptions * opt = new ListOptions; // Открываем опции
      opt->Show(dl);
    }
    if (msg->gbsmsg->msg == KEY_DOWN)
    {
      switch(msg->gbsmsg->submess)
      {
      case GREEN_BUTTON:
        {
          URLInput * ui = new URLInput(); // Новая закачка
          ui->Show("http://");
        }
        break;
      case '#':
        if(dl = DownloadHandler::Top->GetDownloadbyN(GetCurMenuItem(data)))
        {
          DownloadHandler::Top->DeleteDownload(dl); // Удаляем выделенную закачку
          RefreshGUI();
        }
        break;
      case '5':
        if(dl = DownloadHandler::Top->GetDownloadbyN(GetCurMenuItem(data)))
        {
          if (dl->download_state == DOWNLOAD_COMPLETE) // Если закачка завершена, открываем файл
          {
            int len = strlen(dl->full_file_name);
            WSHDR * ws = AllocWS(len + 1);
            str_2ws(ws, dl->full_file_name, len);
            ExecuteFile(ws, 0, 0); 
            FreeWS(ws);
          }
        }
        break;
      }
    }
  }
  */
  return GUI_RESULT_OK;
}

void ContactList::gHook(void *data, int cmd)
{
  if (cmd == TI_CMD_FOCUS)
  {
    DisableIDLETMR();
    int n = list->GetCount();
    Menu_SetItemCountDyn(data, n);
  }
}

void ContactList::ItemHandler(void * data, int curitem, void * unk)
{
  
  WSHDR * ws1, * ws2, * ws3;
  void * item = AllocMLMenuItem(data);
  CLIST* dl = list->GetItem(curitem);
  if(dl && dl->name)
  {
    ws1 = AllocMenuWS(data, strlen(dl->name) + 1);
    str_2ws(ws1, dl->name, strlen(dl->name));
  }
  else
  {
    ws1 = AllocMenuWS(data, 32);
    ascii2ws(ws1, "No name");
  }
  
  ws2 = AllocMenuWS(data, 32);
  switch(dl->status)
  {
    
  case STATUS_ONLINE:
    SetMenuItemIconArray(data, item, &IconPack::Active->data[IMG_ONLINE]);
    break;
  /*  
  case DOWNLOAD_CONNECT:
    ascii2ws(ws2, LangPack::Active->data[LGP_Connecting]);
    SetMenuItemIconArray(data, item, &IconPack::Active->data[IMG_Start]);
    break;
  case DOWNLOAD_GET_INFO:
    ascii2ws(ws2, LangPack::Active->data[LGP_GettingInfo]);
    SetMenuItemIconArray(data, item, &IconPack::Active->data[IMG_GetInfo]);
    break;
    */
  default:
    SetMenuItemIconArray(data, item, &IconPack::Active->data[IMG_OFFLINE]);
    break;
  }
  char uin[64];
  sprintf(uin,percent_d,dl->uin);
  str_2ws(ws1, uin, strlen(uin));
  SetMLMenuItemText(data, item, ws1, ws2, curitem);
  
}

void ContactList::Show()
{
  patch_header(&ContactList_hdr, &IconPack::Active->data[IMG_UNKNOWN], (int)"SieICQ");
  
  ContactList_sk[0].lgp_id = (int)LangPack::Active->data[LGP_Options];
  ContactList_sk[1].lgp_id = (int)LangPack::Active->data[LGP_Close];
  
  gui_id = CreateMultiLinesMenu(NULL, NULL, &ContactList_desc, &ContactList_hdr, NULL, list->GetCount());
}

ContactList::ContactList()
{
  Active = this;
  gui_id = NULL;
  list = new UsersList;
  
//  list->AddUser("Eraser",0,0,0);
}

ContactList::~ContactList()
{
  delete list;
}
