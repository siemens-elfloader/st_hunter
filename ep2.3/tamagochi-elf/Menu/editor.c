//editor
#include "../inc/swilib.h"
#include "rect_patcher.h"

extern const char percent_t[];
const char perc_t[]="%t";
//#include "main.h"

// для чтения файла меню
typedef struct{
  char Text[32];
  char Description[128];
  char IconBig[128];
  char IconSmall[128];
  char Run[128];
} TMenuPoint;

TMenuPoint *itemsF;
int Item_num=0;
int cur_item=0;
int is_any_change=0;

int editor_csm_id;
int editor_menu_id;


const int MSG_RECONFIGURE_MENU=2608;

extern void ascii2ws(WSHDR* ws, const char* s);
extern int char16to8(int c);

int menusoftkeys[]={0,1,2};
SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Edit"},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};

char path_menu[128];


extern void open_select_file_gui(void *ed_gui);
void on_file_select(USR_MENU_ITEM *item)
{
  if (item->type==0)
  {
    switch(item->cur_item)
    {
    case 0:
      wsprintf(item->ws,percent_t,"Select File/Dir");
      break;
    }
  }
  if (item->type==1)
  {
    switch(item->cur_item)
    {
    case 0:
      open_select_file_gui(item->user_pointer);
      break;
    }
  }   
}


void new_or_editext_locret(void){}

int new_or_editext_onkey(GUI *data, GUI_MSG *msg)
{
  int efocus;
  efocus=EDIT_GetFocus(data);
  
  char *s, tmp[132];
  WSHDR *ws;
  
  TMenuPoint *cur_es=EDIT_GetUserPointer(data);

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    if (msg->gbsmsg->submess==ENTER_BUTTON)
    {
      if (efocus==6 || efocus==8 || efocus==10)
      {
        EDIT_OpenOptionMenuWithUserItems(data,on_file_select,data,1);
        return (-1);
      }
    }
  }      
  
  if (msg->keys==0xFFF)
  {
    if (!cur_es)
    {
      itemsF=realloc(itemsF,(Item_num+1)*sizeof(TMenuPoint));
      cur_es=&itemsF[Item_num];
      ++Item_num;
    }
    
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);   // Text
    ws=ec.pWS;
    s=tmp;
    for (int i=0; i<ws->wsbody[0]; i++) *s++=char16to8(ws->wsbody[i+1]);
    *s=0;
    
    if (cur_es)  
    {
          sprintf(cur_es->Text,tmp);
          is_any_change=1;
    }
      
    ExtractEditControl(data,4,&ec);   // Description
    ws=ec.pWS;
    s=tmp;
    for (int i=0; i<ws->wsbody[0]; i++) *s++=char16to8(ws->wsbody[i+1]);
    *s=0;
    if (cur_es)   
    {
          sprintf(cur_es->Description,tmp);
          is_any_change=1;
    }

    
    ExtractEditControl(data,6,&ec);   // SMALLICON
    ws=ec.pWS;
    s=tmp;
    for (int i=0; i<ws->wsbody[0]; i++) *s++=char16to8(ws->wsbody[i+1]);
    *s=0;
    if (cur_es)  
    {
          sprintf(cur_es->IconSmall,tmp);
          is_any_change=1;
    }
    
    ExtractEditControl(data,8,&ec);   // BIGIcon
    ws=ec.pWS;
    s=tmp;
    for (int i=0; i<ws->wsbody[0]; i++) *s++=char16to8(ws->wsbody[i+1]);
    *s=0;
    if (cur_es)  
    {
          sprintf(cur_es->IconBig,tmp);
          is_any_change=1;
    }
    
    ExtractEditControl(data,10,&ec);   // RUN
    ws=ec.pWS;
    s=tmp;
    for (int i=0; i<ws->wsbody[0]; i++) *s++=char16to8(ws->wsbody[i+1]);
    *s=0;
        if (cur_es)  
    {
          sprintf(cur_es->Run,tmp);
          is_any_change=1;
    }


    return (1);
      
  }

  return (0);
}

void new_or_editext_ghook(GUI *data, int cmd)
{
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Save!"};
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
  if (cmd==7)
  {
    SetSoftKey(data,&sk,SET_SOFT_KEY_N);
  }
}

HEADER_DESC new_or_editext_hdr={0,0,0,0,NULL,(int)"Изменить",LGP_NULL};

INPUTDIA_DESC new_or_editext_desc =
{
  1,
  new_or_editext_onkey,
  new_or_editext_ghook,
  (void *)new_or_editext_locret,
  0,
  &menu_skt,
  {0,0,0,0},
  FONT_SMALL,
  100,
  101,
  0,
  //  0x00000001 - Выровнять по правому краю
  //  0x00000002 - Выровнять по центру
  //  0x00000004 - Инверсия знакомест
  //  0x00000008 - UnderLine
  //  0x00000020 - Не переносить слова
  //  0x00000200 - bold
  0,
  //  0x00000002 - ReadOnly
  //  0x00000004 - Не двигается курсор
  //  0x40000000 - Поменять местами софт-кнопки
  0x40000000
};
void CreateEditOrNewExt(void *menugui, int index)
{
  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  TMenuPoint *cur_es;
//  char *s;
//  int n;
//  int c;
  int ect_type;
  
  if (index!=-1) cur_es=&itemsF[index];
  else cur_es=NULL;
  
  eq=AllocEQueue(ma,mfree_adr());    // Text
  WSHDR *ews=AllocWS(128);
  wsprintf(ews,perc_t,"Name:");  
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);    //1
  
  CutWSTR(ews,0);
  ect_type=ECT_NORMAL_TEXT;
  if (cur_es)
  ascii2ws(ews,itemsF[index].Text);
  
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ect_type,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);   //2
  
  wsprintf(ews,perc_t,"Description:");      // Description
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   //3

  CutWSTR(ews,0);
  if (cur_es)
  ascii2ws(ews,itemsF[index].Description);
  
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);   //4
  
  wsprintf(ews,perc_t,"Small icon:");      // SMALL
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   //5
  
  CutWSTR(ews,0);
  if (cur_es)
  ascii2ws(ews,itemsF[index].IconSmall);
 
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);   //6
   
  wsprintf(ews,perc_t,"Big icon:");      // BIG
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   //7
 
  CutWSTR(ews,0);
  if (cur_es)
  ascii2ws(ews,itemsF[index].IconBig);
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);    //8
  
  wsprintf(ews,perc_t,"Run:");      // RUN
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);     //9
  
  CutWSTR(ews,0);
  if (cur_es)
  ascii2ws(ews,itemsF[index].Run);
  
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);    //10
  
  FreeWS(ews);
  patch_header(&new_or_editext_hdr);
  patch_input(&new_or_editext_desc);
  CreateInputTextDialog(&new_or_editext_desc,&new_or_editext_hdr,eq,1,cur_es);  
}
/////////////////////////////
///-----------------------
void SaveMenu(void)
{
 int f;
      unsigned int err;
      if ((f=fopen(path_menu,A_WriteOnly|A_BIN|A_Truncate|A_Create,P_WRITE,&err))!=-1)
      {
        for (int i=0; i<Item_num; i++)
        {
          fwrite(f,&itemsF[i],sizeof(TMenuPoint),&err);
        }
        fclose(f,&err);
      }
      is_any_change=2;
}
/////////////////////////
void RefreshEditor()
{
void *gui;
    gui=FindGUIbyId(editor_menu_id, NULL);
      if (gui)
      {
        Menu_SetItemCountDyn(gui, Item_num);
        RefreshGUI();
      }
}
//// меню редактора
static const HEADER_DESC smenuhdr={0,0,131,21,NULL,(int)"Menu",LGP_NULL};

static const int smenusoftkeys[]={0,1,2};

static const char * const smenutexts[6]=
{
  "Add",
  "Edit",
  "Delete",
  "Up",
  "Down",
  "Save menu"
};

static const SOFTKEY_DESC smenu_sk[]=
{
  {0x0018,0x0000,(int)"Ok"},
  {0x0001,0x0000,(int)"Back"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB smenu_skt=
{
  smenu_sk,0
};

void smenu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

static int smenu_keyhook(void *data, GUI_MSG *msg)
{
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    switch (GetCurMenuItem(data))
    {
    case 0://add  
    CreateEditOrNewExt(data, -1);
    RefreshEditor();
    return(1);
    case 1://edit  
    CreateEditOrNewExt(data, cur_item);
    RefreshEditor();
    return(1);
    case 2://delete  
    for (int i=cur_item;i<Item_num-1;i++)
    {
      itemsF[i]=itemsF[i+1];
    }
    --Item_num;
    is_any_change=1;
    itemsF=realloc(itemsF,(Item_num)*sizeof(TMenuPoint));
    RefreshEditor();
    return(1);
    case 3://up  
    if (cur_item>0)
    {
      TMenuPoint tmp;
      tmp=itemsF[cur_item];
      itemsF[cur_item]=itemsF[cur_item-1];
      itemsF[cur_item-1]=tmp;
      is_any_change=1;
      RefreshEditor();
    }
    return(1);
    case 4://down  
    if (cur_item<Item_num-1)
    {
      TMenuPoint tmp;
      tmp=itemsF[cur_item+1];
      itemsF[cur_item+1]=itemsF[cur_item];
      itemsF[cur_item]=tmp;
      is_any_change=1;
      RefreshEditor();
    }
    return(1);
    case 5://save  
      SaveMenu();
    return(1);
    }
  }
  return(0);
}

static void smenuitemhandler(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
    ws=AllocMenuWS(data,strlen(smenutexts[curitem]));
    wsprintf(ws,perc_t,smenutexts[curitem]);
    SetMenuItemText(data, item, ws, curitem);
}

static const MENU_DESC smenu=
{
  6,smenu_keyhook,smenu_ghook,NULL,
  smenusoftkeys,
  &smenu_skt,
  0x11,//MENU_FLAG,
  smenuitemhandler,
  NULL,//menuitems,
  NULL,//menuprocs,
  0  //count items
};

///////////////////////////////

int main_menu_onkey(void *data, GUI_MSG *msg)
{
  cur_item=GetCurMenuItem(data);
  
  switch(msg->keys)
  {
  case 0x18:
    CreateEditOrNewExt(data, cur_item);
//  CreateEditOrNewExt(item->user_pointer, -1);
    return (-1);
    
  case 0x3D:
    patch_header(&smenuhdr);
    CreateMenu(0,0,&smenu,&smenuhdr,0,6,0,0);
    return (-1);
  }
  return(0);
}


void main_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}



void main_menu_iconhndl(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  {
    ws=AllocMenuWS(data,20);
    ascii2ws(ws,itemsF[curitem].Text);
  }
  SetMenuItemText(data,item,ws,curitem);
}


HEADER_DESC main_menuhdr={0,0,0,0,NULL,(int)"Editor",LGP_NULL};

MENU_DESC main_menustruct=
{
  8,main_menu_onkey,main_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt,
  0x10,
  main_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};



typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;



int LoadItem()
{
 
  FSTATS stat;
  char *fn;
  int f;
  unsigned int ul;
  int i;
  int fsize;
  int c;
  

  fn=(char *)path_menu;
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;

  c=fsize/sizeof(TMenuPoint);

  LockSched();
  
  itemsF=malloc(c*sizeof(TMenuPoint));
  for (i=0;i<c;i++)
  {
   fread(f,&itemsF[i],sizeof(TMenuPoint),&ul);
  }
  fclose(f,&ul);
  UnlockSched();
  return c;
}

void editorcsm_oncreate(CSM_RAM *data)
{
   MAIN_CSM *csm=(MAIN_CSM*)data;
  
   Item_num=LoadItem();
   
    patch_header(&main_menuhdr);
    csm->gui_id=editor_menu_id=CreateMenu(0,0,&main_menustruct,&main_menuhdr,0,Item_num,0,0);
}

void editorcsm_onclose(CSM_RAM *csm)
{
  mfree(itemsF);
  editor_csm_id=0;
  if (is_any_change==2)
  GBS_SendMessage(MMI_CEPID,MSG_RECONFIGURE_MENU,0,0);
}

int editorcsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
  }
  return(1);
}

extern const int minus11;
unsigned short maincsm_name_editor[140];

const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}EditorCSM =
{
  {
  editorcsm_onmessage,
  editorcsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
   editorcsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    maincsm_name_editor,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};

static void UpdateCSMEditor(void)
{
  WSHDR *ws=AllocWS(256);
  str_2ws(ws,path_menu,126);
  wsprintf((WSHDR *)(&EditorCSM.maincsm_name),"Menu editor: %w",ws);
  FreeWS(ws);
}

void ShowEditor()
{
  MAIN_CSM main_csm;
  UpdateCSMEditor();
  LockSched();
  if (editor_csm_id)
    CloseCSM(editor_csm_id);
  editor_csm_id=CreateCSM(&EditorCSM.maincsm,&main_csm,2);
  UnlockSched();
}
