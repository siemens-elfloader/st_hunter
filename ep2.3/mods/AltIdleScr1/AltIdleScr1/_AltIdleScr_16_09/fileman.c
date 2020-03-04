char _percent_t[]="%t";

#pragma inline
void patch_header(HEADER_DESC* head)
{
	head->rc.x=0;
	head->rc.y=YDISP;
	head->rc.x2=ScreenW()-1;
	head->rc.y2=HeaderH()+YDISP-1;
}
#pragma inline
void patch_input(INPUTDIA_DESC* inp)
{
	inp->rc.x=0;
	inp->rc.y=HeaderH()+1+YDISP;
	inp->rc.x2=ScreenW()-1;
	inp->rc.y2=ScreenH()-SoftkeyH()-1;
}

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}
int size;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  int is_rect_needed;
  int is_first_set;
  int x_pos;
  int y_pos;
  int x2_pos;
  int y2_pos;
  void *rect_or_xy;
  int cstep;
}RECT_GUI;

typedef struct
{
  GUI gui;
  WSHDR*ws1; 
  int r;
  int g;
  int b;
  int a;
  char* color;
  int current_column;
  char testcolor[4];
  int cstep;
}MAIN_GUI_2;


typedef struct
{
  void *next;
  char *fullname;
  char *name;
  int is_folder;
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
    FLIST *fl_prev;
    fl_prev=fl;
    fl=fl->next;
    mfree(fl_prev->fullname);
    mfree(fl_prev->name);
    mfree(fl_prev);
  }
}

#define IS_FOLDER 1
#define IS_FILE 0

const char back[]="..";             
int GetFListN()
{
  int i=0;
  FLIST *fl=(FLIST*)&fltop;
  while((fl=fl->next)) i++;
  return (i);
}

int GetFoldersLevel(char *name)
{
  int i=0;
  char *s=name;
  while(*s++)
  {
    if (*s=='\\' &&*(s+1)!=0) i++;    
  }
  return (i);
}

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
  FLIST *fl;
  FLIST *pr;
  FLIST *fn=malloc(sizeof(FLIST));
  fn->fullname=malloc(strlen(full_name)+1);
  strcpy(fn->fullname,full_name);
  
  fn->name=malloc(strlen(name)+1);
  strcpy(fn->name,name);
  
  fn->is_folder=is_folder;
  fn->next=0;
  fl=(FLIST *)fltop;
  if (fl)
  {
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

void FindFiles(char *str, int type)  // type == 0 SelectFolder, type == 1 SelectFile
{
  DIR_ENTRY de;
  unsigned int err;
  int i, c;
  char name[256]; 
  char fname[128];

  Free_FLIST();
  strcpy(name,str);
  strcat(name,"*");
  
  i=GetFoldersLevel(str);
  if (i==0)
  {
    AddToFList("ROOT",back,IS_FOLDER);
  }
  else
  {
    char *s=str;
    char *d=fname;
    for (int k=0; k!=i && *s; )
    {
      c=*s++;
      *d++=c;
      if (c=='\\')  k++;
    }
    *d=0;
    AddToFList(fname,back,IS_FOLDER);
  }

  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      strcpy(name,de.folder_name);
      strcat(name,"\\");
      strcat(name,de.file_name);
      if (de.file_attr&FA_DIRECTORY)
      {
        strcpy(fname,"\\");
        strcat(fname,de.file_name);
        strcat(name,"\\");
        AddToFList(name,fname,IS_FOLDER);
      }
      else
      {
        if (type!=0)
        {
          AddToFList(name,de.file_name,IS_FILE);
        }
      }
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
}



FLIST *FindFLISTtByNS(int *i, int si)
{
  FLIST *fl;
  fl=(FLIST *)fltop;
  while(fl)
  {
    if (fl->is_folder==si)
    {
      if (!(*i)) return (fl);
      (*i)--;
    }    
    fl=fl->next;
  }
  return fl;
}
  
FLIST *FindFLISTtByN(int n)
{
  FLIST *fl;
  fl=FindFLISTtByNS(&n,IS_FOLDER); if ((!n)&&(fl)) return(fl);
  fl=FindFLISTtByNS(&n,IS_FILE); if ((!n)&&(fl)) return(fl);
  return fl;
}
int start(char name[128]);


void SavePath(void *ed_gui, FLIST *fl)
{
/*  EDITCONTROL ec;
  EDIT_ExtractFocusedControl(ed_gui,&ec);
  WSHDR *ws=AllocWS(ec.maxlen);
  str_2ws(ws,fl->fullname,ec.maxlen);
  EDIT_SetTextToFocused(ed_gui,ws);
  FreeWS(ws);*/
  start(fl->fullname);//Стартуем выбранный файл))))))
}
  
//==============================================================================
/*int options_softkeys[]={0,1,2};//Нах не надо пока :D Но работает)))))))))
SOFTKEY_DESC options_menu_sk[]=
{
  {0x0018,0x0000,(int)""},
  {0x0001,0x0000,(int)"Select"},
  {0x003D,0x0000,(int)"+"}
};

SOFTKEYSTAB options_menu_skt=
{
  options_menu_sk,0
};

HEADER_DESC options_menuhdr={0,0,0,0,NULL,(int)"Опции",LGP_NULL};

MENUITEM_DESC options_menu_ITEMS[7]=
{
  {NULL,(int)"Sign",                       LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Generate jad",               LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Remove signature",           LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Copy Certificate to Config", LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Remove record from .sbp",    LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Store record to .sbp",       LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Back",                       LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
};

void *options_menu_HNDLS[7]=
{
  (void *)sign_jad,
  (void *)generate_jad,
  (void *)remove_sign_from_jad,
  (void *)copy_certificate,
  (void *)remove_record_sbp,
  (void *)store_record_sbp,
  (void *)options_back,
};

MENU_DESC options_menu_STRUCT=
{
  0,(void *)NULL,(void*)NULL,NULL,
  options_softkeys,
  &options_menu_skt,
  0x200+0x10,
  NULL,
  options_menu_ITEMS,   //Items
  NULL,   //Procs
  7 //n
};

#pragma inline
void patch_header_small(HEADER_DESC* head)
{
  head->rc.x=3;
  head->rc.y=YDISP+0x18;
  head->rc.x2=ScreenW()-3-1;
  head->rc.y2=YDISP+0x18+0x13;
}

int CreateOptionsMenu()
{
  patch_header_small((HEADER_DESC*)(&options_menuhdr));
  return (CreateMenu(1,0,&options_menu_STRUCT,&options_menuhdr,0,7,0,0)); 
}
*/
//==============================================================================
char header[];
int filelist_menu_onkey(void *data, GUI_MSG *msg)
{
  FVIEW *fview=MenuGetUserPointer(data);
  FLIST *fl;
  int i;
  i=GetCurMenuItem(data);
  fl=FindFLISTtByN(i);
  
  if (msg->keys==0x3D)
  {
    if (fl) 
    {
      if (fl->is_folder==IS_FOLDER)
      {
        int len;
        if (strcmp(fl->fullname,"ROOT"))
        {
          strncpy(header,fl->fullname,127);
          len=strlen(fl->fullname);
          header[len>127?127:len]=0;
          FindFiles(fl->fullname,1);
        }
        else
        {
          void CreateRootMenu();
          CreateRootMenu();
        }         
        Menu_SetItemCountDyn(data,GetFListN());
        SetCursorToMenuItem(data, 0);
        RefreshGUI();
      }
      else
      {
        SavePath(fview->gui,fl);        
        return (1);
      }
    }
    //    GeneralFunc_F1(1);
    return(-1);
  }
  if (msg->keys==0x18)
  {
    if (fl)
    {
      if (strcmp(fl->name,back))
      {
        SavePath(fview->gui,fl);
        return (1);
      }      
    }   
    return (-1);
  }
  /*if (msg->gbsmsg->msg==KEY_DOWN)
      {
        if(msg->gbsmsg->submess==0x2A)
          {
            CreateOptionsMenu();
            return(-1);
          }
      }
  */
 
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
}

void filelist_menu_iconhndl(void *data, int curitem, void *user_pointer)
{
  FLIST *fl;
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  int len;
  fl=FindFLISTtByN(curitem);
  if (fl)
  {
    len=strlen(fl->name);
    ws=AllocMenuWS(data,len+4);
    
    if (fl->is_folder)
    {
      str_2ws(ws,fl->name,len);
      wsInsertChar(ws,0x0007,1);
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
  {0x0001,0x0000,(int)"Close"},
  {0x0018,0x0000,(int)"Select"},
  {0x003D,0x0000,(int)"+"}
};

SOFTKEYSTAB fmenu_skt=
{
  fmenu_sk,1
};
char header[128];
HEADER_DESC filelist_HDR={0,0,0,0,NULL,(int)header,LGP_NULL};

MENU_DESC filelist_STRUCT=
{
  8,filelist_menu_onkey,filelist_menu_ghook,NULL,
  fmenusoftkeys,
  &fmenu_skt,
  0x10,
  filelist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0 
};

void CreateRootMenu()
{
  Free_FLIST();
  AddToFList("0:\\","0:\\",IS_FOLDER);
  AddToFList("1:\\","1:\\",IS_FOLDER);
  AddToFList("2:\\","2:\\",IS_FOLDER);
  AddToFList("4:\\","4:\\",IS_FOLDER);
  strcpy(header,"Root");
}

void open_fm()
{
  FVIEW *fview;
  char *s;
  WSHDR *ws=AllocWS(256);
  fview=malloc(sizeof(FVIEW));
  if (ws->wsbody[0]==0)
  {
    CreateRootMenu();
  }
  else
  {
    ws_2str(ws,header,127);
    s=strrchr(header, '\\');
    if (s) *(s+1)=0;
    int len=strlen(header);
    header[len>127?127:len]=0;
    FindFiles(header,1);
  }    
  patch_header(&filelist_HDR);
  CreateMenu(0,0,&filelist_STRUCT,&filelist_HDR,0,GetFListN(),fview,0);
}
