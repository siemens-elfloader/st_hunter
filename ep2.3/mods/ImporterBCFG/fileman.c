#include "../inc/swilib.h"
#include "main.h"
#include "rect_patcher.h"

typedef struct
{
  void *next;
  int is_folder;
  char *fullname;  
  char *name;  
}FLIST;

typedef struct
{
  void *gui;
  int type;
}FVIEW;

volatile FLIST *fltop;

void Free_FLIST(void)
{
  LockSched();
  FLIST *fl=(FLIST *)fltop;
  fltop=0;
  UnlockSched();
  while(fl)
  {
    FLIST *fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev);
  }
}

enum TYPES {IS_BACK, IS_FOLDER, IS_FILE}; 
const char back[]="..";             


int strcmp_nocase(const char *s, const char *d)
{
  int cs;
  int ds;
  do
  {
    cs=*s++;
    if (cs&0x40) cs&=0xDF;
    ds=*d++;
    if (ds&0x40) ds&=0xDF;
    cs-=ds;
    if (cs) break;
  }
  while(ds);
  return(cs);
}

FLIST *AddToFList(const char* full_name, const char *name, int is_folder)
{
  int l_fname;
  FLIST *fl;
  FLIST *fn=malloc(sizeof(FLIST)+(l_fname=strlen(full_name))+strlen(name)+2);
  fn->fullname=(char *)fn+sizeof(FLIST);
  fn->name=(char *)fn+sizeof(FLIST)+l_fname+1;
  strcpy(fn->fullname,full_name);
  strcpy(fn->name,name);
  fn->is_folder=is_folder;
  fn->next=0;
  fl=(FLIST *)fltop;
  if (fl)
  {
    FLIST *pr;
    pr=(FLIST *)&fltop;
    while(strcmp_nocase(fl->name,fn->name)<0)
    {
      pr=fl;
      fl=fl->next;
      if (!fl) break;
    }
    fn->next=fl;
    pr->next=fn;
  }
  else
  {
    fltop=fn;
  }
  return (fn);
}

int FindFiles(char *str/*, int type*/)  // type == 0 SelectFolder, type == 1 SelectFile
{
  DIR_ENTRY de;
  unsigned int err;
  char *rev,*s,*d;
  int i, c, n=0;
  char path[256];
  char name[128];
  
 // int type=1;
  strcpy(path,str);
  Free_FLIST();
  s=path;
  d=name;
  rev=0;
  while((c=*s++))
  {
    *d++=c;
    if (c=='\\' && *s!='\0') rev=d;
  }
  if(rev==0)
    AddToFList("ROOT",back,IS_BACK);
  else
  {
    *rev=0;
    AddToFList(name,back,IS_BACK);
  }
  n++;
  
  
  i=strlen(path);
  path[i++]='*';
  path[i]='\0';
  if (FindFirstFile(&de,path,&err))
  {
    do
    {
      i=strlen(de.folder_name);
      strcpy(path,de.folder_name);
      path[i++]='\\';
      strcpy(path+i,de.file_name);
      if (de.file_attr&FA_DIRECTORY)
      {
        i=strlen(path);
        path[i++]='\\';
        path[i]=0;
        name[0]='\\';
        strcpy(name+1,de.file_name);
        AddToFList(path,name,IS_FOLDER);
        n++;
      }
      else
      {
       if(strstr(de.file_name,".bcfg") != 0)
        {
          AddToFList(path,de.file_name,IS_FILE);
          n++;
        }
      }
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
  return n;
}


//Ключи для поиска по T9
static const char table_T9Key[256]=
"11111111111111111111111111111111"
"10001**0***0000*012345678900***0"
"0222333444555666777788899991*110"
"122233344455566677778889999111*1"
"11111111111111111111111111111111"
"11111111311111111111111131111111"
"22223333444455566677778888899999"
"22223333444455566677778888899999";

int id_flist;
int prev_itemcount;
char _hdr_text[128];
char _sk_r[16];
char header[128];
char FListT9Key[32];

void UpdateFListHeader(void)
{
  if (strlen(FListT9Key))
  {
    strcpy(_hdr_text,"Input T9:");
    strcat(_hdr_text,FListT9Key);
    strcpy(_sk_r,"<C");
  }
  else
  {
    strcpy(_hdr_text,header);
    strcpy(_sk_r,"Close");
  }
}

void AddFListT9Key(int chr)
{
  int l=strlen(FListT9Key);
  if (l<(sizeof(FListT9Key)-1))
  {
    FListT9Key[l]=chr;
  }
}

void BackFListSpaceT9(void)
{
  int l=strlen(FListT9Key);
  if (l)
  {
    l--;
    FListT9Key[l]=0;
  }
}

void ClearFListT9Key(void)
{
  zeromem(FListT9Key,sizeof(FListT9Key));
}

FLIST *FindFLISTtByNS(int *i, int si)
{
  FLIST *fl;
  fl=(FLIST *)fltop;
  char *s;
  char *d;
  int c;
  while(fl)
  {
    s=FListT9Key;
    if(si==IS_FOLDER) d=fl->name+1;
    else d=fl->name;
    
    while(c=*s++)
     {
      if(c!=table_T9Key[*d++]) goto L_NOT9;
     }
    if (fl->is_folder==si)
    {      
      if (!(*i)) return (fl);
      (*i)--;
    }    
  L_NOT9:
    fl=fl->next;
  }
  return fl;
}

int CountFList(void)
{
  int l=-1;
  FindFLISTtByNS(&l,IS_BACK);
  FindFLISTtByNS(&l,IS_FOLDER);
  FindFLISTtByNS(&l,IS_FILE);
  l=-1-l;
  return l;
}

void RecountFList()
{
  int i;
  void *data;
  UpdateFListHeader();
  if (!id_flist) return; //Нечего считать
  data=FindGUIbyId(id_flist,NULL);

  i=CountFList();
  if (i!=prev_itemcount)
  {
    prev_itemcount=i;
    Menu_SetItemCountDyn(data,i);
  }
  SetCursorToMenuItem(data,0);
  if (IsGuiOnTop(id_flist)) RefreshGUI();
}

FLIST *FindFLISTtByN(int n)
{
  FLIST *fl;
  fl=FindFLISTtByNS(&n,IS_BACK); if ((!n)&&(fl)) return(fl);
  fl=FindFLISTtByNS(&n,IS_FOLDER); if ((!n)&&(fl)) return(fl);
  fl=FindFLISTtByNS(&n,IS_FILE); if ((!n)&&(fl)) return(fl);
  return fl;
}


void SavePath(void *ed_gui, FLIST *fl,FVIEW *f)
{
  OpenFile(fl->fullname,f->type);
}

int filelist_menu_onkey(void *data, GUI_MSG *msg)
{
  FVIEW *fview=MenuGetUserPointer(data);
  FLIST *fl;
  int i, n;
  i=GetCurMenuItem(data);
  fl=FindFLISTtByN(i);
  
  if (msg->keys==0x3D)
  {
    ClearFListT9Key();
    if (fl) 
    {
      if (fl->is_folder==IS_FOLDER || fl->is_folder==IS_BACK)
      {
        int len;
        if (strcmp(fl->fullname,"ROOT"))
        {
          strncpy(header,fl->fullname,sizeof(header)-1);
          len=strlen(fl->fullname);
          header[len>sizeof(header)-1?sizeof(header)-1:len]=0;
          n=FindFiles(fl->fullname/*,fview->type*/);
        }
        else
        {
          int CreateRootMenu();
          n=CreateRootMenu();
        }
        UpdateFListHeader();
        Menu_SetItemCountDyn(data,n);
        SetCursorToMenuItem(data, 0);
        RefreshGUI();
      }
      else
      {
       // SavePath(fview->gui,fl,fview);        
         OpenFile(fl->fullname,fview->type);
        // CloseCSM(id_flist);
          GeneralFuncF1(1);
        //GeneralFunc_flag1(id_flist,1);
        return (1);
      }
    }
    if (strlen(FListT9Key))
      {
	ClearFListT9Key();
	RecountFList();
      }
    //    GeneralFunc_F1(1);
    return(-1);
  }
  if (msg->keys==0x18)
  {
    if (fl)
    {
      if (fl->is_folder!=IS_BACK)
      {
        SavePath(fview->gui,fl,fview);
        return (1);
      }      
    }   
    return (-1);
  }

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    int key=msg->gbsmsg->submess;
    if (((key>='0')&&(key<='9'))||(key=='#')||(key=='*'))
    {
      AddFListT9Key(key);
      RecountFList();
      return(-1);
    }
  }
  
  if (msg->keys==1)
  {
    if (strlen(FListT9Key))
    {
      BackFListSpaceT9();
      RecountFList();
      return(-1);
    }
  }
  return (0);
}

void filelist_menu_ghook(void *data, int cmd)
{
  FVIEW *fview=MenuGetUserPointer(data);
  if (cmd==3)
  {
    Free_FLIST();
    mfree(fview);    
  }
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

void filelist_menu_iconhndl(void *data, int curitem, void *user_pointer)
{
  FLIST *fl;
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  int len;
  UpdateFListHeader();
  fl=FindFLISTtByN(curitem);
  if (fl)
  {
    len=strlen(fl->name);
    ws=AllocMenuWS(data,len+4);
    if (fl->is_folder==IS_BACK || fl->is_folder==IS_FOLDER)
    {
      str_2ws(ws,fl->name,len);
      wsInsertChar(ws,0x0002,1);
      wsInsertChar(ws,0xE008,1);
    }
    else
    {
      str_2ws(ws,fl->name,len);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws, "Ошибка");
  }
  SetMenuItemText(data, item, ws, curitem);
}

int fmenusoftkeys[]={0,1,2};
SOFTKEY_DESC fmenu_sk[]=
{
  {0x0018,0x0000,(int)"Select"},
  {0x0001,0x0000,(int)_sk_r},
  {0x003D,0x0000,(int)"+"}
};

SOFTKEYSTAB fmenu_skt=
{
  fmenu_sk,0
};

HEADER_DESC filelist_HDR={0,0,0,0,NULL,(int)_hdr_text,LGP_NULL};

MENU_DESC filelist_STRUCT=
{
  8,filelist_menu_onkey,filelist_menu_ghook,NULL,
  fmenusoftkeys,
  &fmenu_skt,
  0x10,
  filelist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

int CreateRootMenu()
{
  char path[32];
  unsigned int err;
  int n=0;
  Free_FLIST();
  for (int i=0; i!=5; i++)
  {
    path[0]=i+'0';
    path[1]=':';
    path[2]='\\';
    path[3]=0;
    if (isdir(path,&err))
    {
      AddToFList(path,path,IS_FOLDER);
      n++;
    }
  }
  strcpy(header,"Root");
  return (n);
}


int SelectFile(int type)
{
  FVIEW *fview;
  
  int n=0;
  fview=malloc(sizeof(FVIEW));
  fview->type=type;

  n=CreateRootMenu();
  UpdateFListHeader();
  patch_header(&filelist_HDR);
  return id_flist=CreateMenu(0,0,&filelist_STRUCT,&filelist_HDR,0,n,fview,0);
}
