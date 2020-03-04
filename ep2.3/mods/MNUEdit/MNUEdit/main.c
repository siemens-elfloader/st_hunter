#include "..\inc\swilib.h"
#include "language.h"
#include "../inc/cfg_items.h"

CSM_DESC icsmd;

extern void InitConfig();
extern const char smenu_path[128];
int id_ed;
extern void CreateRootMenu();
extern void open_fm(void *ed_gui,int type);

int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;

//-----------------------------------------

const char per_t[]="%t";
const char per_s[]="%s";
const char empty_str[]="";

//-----------------------------------------

char *action[]={"","FILE","SHORTCUT","SUBMENU","FOLDER","KEYSEND","FILES","JAPP","CALL","LIB","ENTRY"};
char file[256];
DIR_ENTRY de;
WSHDR *ews;

//------------------------------------------

unsigned int char16to8(unsigned int c);
void win1251_2ws(WSHDR *ws, const char *s);
void ascii2ws(WSHDR *ws, const char *s);
void koi2ws(WSHDR *ws, const char *s);

unsigned short maincsm_name_body[140];

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

int DrawText(int i);
int create();
void SaveFile(char *fname);

GBSTMR mytmr;


int my_csm_id=0;

unsigned int err;

#pragma inline
void patch_header(HEADER_DESC* head)
{
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}
#pragma inline
void patch_input(INPUTDIA_DESC* inp)
{
  inp->rc.x=0;
  inp->rc.y=HeaderH()+1+YDISP;
  inp->rc.x2=ScreenW()-1;
  inp->rc.y2=ScreenH()-SoftkeyH()-1;
}

//---------------------------------------------------------------------//

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}


void mess(char *s)
{
  ShowMSG(1,(int)s);
}

void mess2(char *s)
{
  ShowMSG(2,(int)s);
}

void messd(int i)
{
  char s[10];
  ShowMSG(1,(int)s);
}

//---------------------------------------------------------------------//

typedef struct
{
  void *next;
  char *name;
  char *icon;
  char *type;
  char *path;
}MNU;
volatile MNU *mnutop;

EDITCONTROL ec;

//---------------------------------------------------------------------//

void about()
{ 
  char s[128];
  snprintf(s,128,"MNUEdit v0.5fix\n(c)kluchnik\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
}

//---------------------------------------------------------------------//

int GetFListN2()
{
  int i=0;
  MNU *fl=(MNU*)&mnutop;
  while((fl=fl->next)) i++;
  return (i);
}

MNU *Add(const char *name,const char *icon,const char *type,const char *path)
{
  MNU *pr;
  MNU *fl;
  MNU *fn=malloc(sizeof(MNU));
  fn->name=malloc(strlen(name)+1);
  fn->icon=malloc(strlen(icon)+1);
  fn->type=malloc(strlen(type)+1);
  fn->path=malloc(strlen(path)+1);
  strcpy(fn->name,name);
  strcpy(fn->icon,icon);
  strcpy(fn->type,type);
  strcpy(fn->path,path);
  
  fn->next=0;
  fl=(MNU *)&mnutop;
  if(fl)
  {
    pr=(MNU *)mnutop;
    pr=fl;
    fl=fl->next;    
    fn->next=fl;
    pr->next=fn;
  }
  else
    mnutop=fn;  
return fn;
}

void parse(char *str)//Разбираем строчку
{
  int j=0; 
  int k=0;
  char *name=malloc(64);
  char *icon=malloc(128);
  char *type=malloc(24);
  char *path=malloc(256);
  
  if(isnewSGold()==1 || isnewSGold()==2)//Если нсг или елка то делаем так, на сголд пикофф при таком способе...
  {
    while(*(str+k)!='|') { *(name+j)=*(str+k); j++; k++; } *(name+j++)=0; j=0; k++;
    while(*(str+k)!='|') { *(icon+j)=*(str+k); j++; k++; } *(icon+j++)=0; j=0; k++;
    while(*(str+k)!='|') { *(type+j)=*(str+k); j++; k++; } *(type+j++)=0; j=0; k++;
    while(*(str+k))      { *(path+j)=*(str+k); j++; k++; } *(path+j++)=0;
  }
  else//для сголд делаем так)
  {
    while (str[j]!='|') { name[k++]=str[j]; j++;} name[k++]=0; j++; k=0;
    while (str[j]!='|') { icon[k++]=str[j]; j++;} icon[k++]=0; j++; k=0;
    while (str[j]!='|') { type[k++]=str[j]; j++;} type[k++]=0; j++; k=0;
    while (str[j])      { path[k++]=str[j]; j++;} path[k++]=0; 
  }  
  Add(name,icon,type,path);
}

void LoadMNU(char *path)
{
  unsigned int err; 
  int f; 
  char *mem;
  int size; 
  int i; 
  int j=0;
  char *str; 
  f=fopen(path,A_ReadOnly/*+A_BIN*/,P_READ,&err); 
  if(f!=-1) 
  {
    mem=malloc(get_file_size(path)); 
    str=malloc(258); 
    if ((mem!=0)&&(str!=0)) 
     { 
      size=fread(f,mem,get_file_size(path),&err); 
      i=0; 
      while (i<size) //пока не конец файлa
      { 
        strcpy(str,""); 
        j=0; 
        while((i<size) && (j<255))//читаем данные по строкам
        {
          if(*(mem+i)==0x0D)// 0D
          {
            i++;
            if(*(mem+i)==0x0A)//0D0A 
            {
             i++;
             break;
            }
            break;
          }
          else
           if(*(mem+i)==0x0A)// 0A
            {
             i++;
             break;
            }
          
          *(str+j)=*(mem+i);
          j++;
          i++;
        }
        *(str+j)=0;
        parse(str);
      }
    }
  }
  else
  {
    ShowMSG(1,(int)"Can't open *.mnu");
  }
fclose(f,&err); 
mfree(mem); 
mfree(str); 
}

//---------------------------------------------------------------------//

MNU *FindByN(int n);

void del(int i)//альтернативное удаление:)
{
 MNU *mnu;
 mnu=FindByN(i);
 mnu->name[0]=';';
 RefreshGUI();
}

void AddItem()//Добавляем пункт меню
{
  Add("new","icons\\","FILE","4:\\");
  RefreshGUI();
  ShowMSG(1,(int)"New item add!");
}

//---------------------------------------------------------------------//

void bm_menu_iconhnd(void *data, int curitem, void *unk);
HEADER_DESC bm_menuhdr={0,0,131,21,NULL,(int)"MNUEditor",LGP_NULL};
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)LG_OPTIONS},
  {0x0001,0x0000,(int)LG_CLOSE},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

int item;
void bm_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
    Menu_SetItemCountDyn(data,GetFListN2());
  }
};

extern void ShowMainMenu(void);

int bm_menu_onkey2(void *data, GUI_MSG *msg)
{
 int i;
 i=GetCurMenuItem(data);
 int k=msg->gbsmsg->submess;
 
 if(k==LEFT_SOFT) ShowMainMenu(); 
 
 item=i;//Какой выбран итем
 
 if(k==ENTER_BUTTON)
 {
   DrawText(item);
 }
 
 if (msg->gbsmsg->msg==KEY_DOWN)
  {
    i=msg->gbsmsg->submess;
    if(i==GREEN_BUTTON)
    {    
      SaveFile(file);
    }
  }
 if (k=='*') about();    
 
return 0;
}

const ML_MENU_DESC bmmenu=//Bookmarks
{
  8,bm_menu_onkey2,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x11,//0x1,
  bm_menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0,   //n
  1  
};

MNU *FindByN(int n)
{
  MNU *fl;
  int i=0;
  int j=0;
  int k=GetFListN2();
  fl=(MNU *)mnutop;
  j=k-n-1;//Я видимо талпаеп,или сименс все таки сименс оч загадочный аппарат)) если написать так k-=n-1 то буит пикофф...
  if(j==-1) fl=fl->next;
  else
  {
    for(i=0;i<j;i++)
     {
      fl=fl->next;
     }
  }
  return fl;
}

void bm_menu_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  WSHDR *ws1;
  void *item=AllocMLMenuItem(data);
  MNU *fl;
  fl=FindByN(curitem);
  ws1=AllocWS(strlen(fl->path));
  if (fl->name)
  {
     ws=AllocMenuWS(data,strlen(fl->name)+4);
     wsprintf(ws,"%d.%t",curitem+1,fl->name);
     wsprintf(ws1,per_s,fl->path);
  }
  else
   {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);
   }  
  
  char *s;
  if(fl->icon[0]>='0' || fl->icon[0]<='9')
    SetMenuItemIconArray(data,item,(const int *)fl->icon);
  else
  {
   s=malloc(128);
   sprintf(s,"%s%s",smenu_path,fl->icon);
   SetMenuItemIconArray(data,item,(const int *)s);
  }
    
  SetMLMenuItemText(data, item, ws, ws1, curitem);
}

int create()
{  
  patch_header(&bm_menuhdr);
  return id_ed=CreateMultiLinesMenu(0,0,&bmmenu,&bm_menuhdr,0,GetFListN2());
}

//==============================================================================

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  csm->gui_id=create();
}

void Killer(void)
{
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)Killer);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      {
	csm->csm.state=-3;
      }
    }
  } 
  return(1);
}

unsigned short maincsm_name_body[140];

const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  maincsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};
WSHDR *ws_name;
void UpdateCSMname(void)
{ 
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"MNUEdit: %w",ws_name);
}

int maincsm_id;
int main(char *exename, char *fname)
{ 
  InitConfig();

  if (fname)
  {
    if (strlen(fname)<256)
    {
      strcpy(file,fname);
      char *p=strrchr(file,'\\');
      if (p)
      {
       ws_name=AllocWS(strlen(p+1));
       str_2ws(ws_name,p+1,strlen(p)+1);
      }
      else 
      {
        ws_name=AllocWS(32);
        wsprintf(ws_name,per_s,"error");
      }
    }
    LoadMNU(file);
  }
  else
  {
    ShowMSG(1,(int)"Nothing to do!");
    return 1;
  }

  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  LockSched();
  maincsm_id=CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched();
 return 0;
}

//--------------------------------юзер меню--------------------------------//

extern int DispShortcutsMenu(void *ed_gui);

void on_utf8ec(USR_MENU_ITEM *item)
{
  if (item->type==0)
  {
    switch(item->cur_item)
    {
    case 0:
      wsprintf(item->ws,per_t,"SelectFile");
      break;
    case 1:
      wsprintf(item->ws,per_t,"SelectShortcut");
    }
  }

  if (item->type==1)
  {
    switch(item->cur_item)
    {
    case 0: 
      CreateRootMenu();open_fm(item->user_pointer,1);
      break;
    case 1:
      DispShortcutsMenu(item->user_pointer);
      break;
    }
  }   
}

//--------------------------------Редактор--------------------------------//

HEADER_DESC disk_prop_hdr={0,0,131,21,NULL,0,LGP_NULL};

int empty_onkey(GUI *data, GUI_MSG *msg)
{ 
  int i=0;
  MNU *fl;
  WSHDR *ws;
  ws=AllocWS(256);    
  if (msg->keys==0xFFF)
  {  
    EDITCONTROL ec;
    fl=FindByN(item);                       
    ExtractEditControl(data,2,&ec);	    
    ws=ec.pWS;
    GeneralFuncF1(1);
                 
    for(i=0;i<ws->wsbody[0];i++)
      *(fl->name+i)=char16to8(ws->wsbody[i+1]);
      *(fl->name+i++)=0;//Обозначим конец строки,иначе кака :)
                  
    ExtractEditControl(data,4,&ec);	
    ws=ec.pWS;
    GeneralFuncF1(1);
    ws_2str(ws,fl->icon,wstrlen(ws));
                  
    ExtractEditControl(data,6,&ec);	
    ws=ec.pWS;
    GeneralFuncF1(1);
    ws_2str(ws,fl->type,wstrlen(ws));
                  
    ExtractEditControl(data,8,&ec);	
    ws=ec.pWS;
    GeneralFuncF1(1);
    ws_2str(ws,fl->path,wstrlen(ws));    
  }
  
  if(msg->gbsmsg->submess==ENTER_BUTTON)
   {
     EDIT_OpenOptionMenuWithUserItems(data,on_utf8ec,data,2);
     return -1;
   }
 return(0);
}

SOFTKEY_DESC sk[];
SOFTKEYSTAB skt={ sk,0 };

void empty_locret(void){}

void empty_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Save!"};
  
  if(cmd==0xA)
  {
    DisableIDLETMR();   // Отключаем таймер выхода по таймауту
  }
  
  if(cmd==0x03)     // onDestroy
  {
    FreeWS(ews);
  }
  
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);
  }

  MNU *fl;
  fl=FindByN(item);
  if (cmd==0x0D)
  {
    int i;
    //onCombo
    ews=AllocWS(256);
    if ((i=EDIT_GetItemNumInFocusedComboBox(gui)))
    {
      sprintf(action[0],per_s,fl->type);
      wsprintf(ews,per_t,action[i-1]);
    }
    else
    {
      ExtractEditControl(gui,EDIT_GetFocus(gui)-1,&ec);
      wstrcpy(ews,ec.pWS);
    }
    EDIT_SetTextToFocused(gui,ews);
  }
}


INPUTDIA_DESC edit_desc=//Редактирование
{
  1,
  empty_onkey,
  empty_ghook,
  (void *)empty_locret,
  0,
  &skt,
  {0,0,0,0},
  FONT_SMALL,
  100,
  101,
  0,
  0,
  0x40000000
};
 
//--------------------------------------------------------//

void SaveFile(char *fname)
{
  unsigned int err;
  int f;
  char *s;//=malloc(get_file_size(fname));
  char *p=malloc(strlen(fname)+10);
  strcpy(p,fname);
  strcat(p,".bkp");
  if(get_file_size(p))
    unlink(p,&err);
  fmove(fname,p,&err);
  MNU *fl;
  if((f=fopen(fname,A_Create+A_ReadWrite+A_BIN,P_WRITE,&err))!=-1)
   {
     for(int i=0;i<GetFListN2();i++)//TotalItem
      {
        fl=FindByN(i);             
        if(fl->name[0]==';'){ continue;}//Удаляем нах строку:)

        s=malloc(strlen(fl->name)+1);
        *(s)=0;
        snprintf(s,strlen(fl->name)+1,per_s,fl->name);
        fwrite(f,s,strlen(s),&err);
        fwrite(f,"|",1,&err);
        
        s=malloc(strlen(fl->icon)+1);
        *(s)=0;
        snprintf(s,strlen(fl->icon)+1,per_s,fl->icon);
        fwrite(f,s,strlen(s),&err);
        fwrite(f,"|",1,&err);
        
        s=malloc(strlen(fl->type)+1);
        *(s)=0;
        snprintf(s,strlen(fl->type)+1,per_s,fl->type);
        fwrite(f,s,strlen(s),&err);
        fwrite(f,"|",1,&err);
        
        s=malloc(strlen(fl->path)+1);
        *(s)=0;
        snprintf(s,strlen(fl->path)+1,per_s,fl->path);
        fwrite(f,s,strlen(s),&err);
        
        fwrite(f,"\r\n",2,&err);
      }
     ShowMSG(1,(int)LG_FILESAVE);
   }
  else ShowMSG(1,(int)LG_NOTSAVE);
  fclose(f,&err);
  mfree(s);
}

//--------------------------------------------------------//

int DrawText(int i)
{     
  EDITC_OPTIONS ec_options;
  PrepareEditControl(&ec);

  void *ma=malloc_adr();
  void *eq=AllocEQueue(ma, mfree_adr());
  WSHDR *ws=AllocWS(256);
  MNU *fl;
  fl=FindByN(i);

  wsprintf(ws,per_t,LG_NAME);//"Имя:\n"
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ws,256);
  AddEditControlToEditQend(eq,&ec,ma);

  ascii2ws(ws,fl->name);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ws,256);
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,1);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);
  

  wsprintf(ws,per_t,LG_ICON);//"\nИконка:\n"
  ConstructEditControl(&ec,ECT_HEADER,ECF_SET_CURSOR_END,ws,256);
  AddEditControlToEditQend(eq,&ec,ma);

  str_2ws(ws,fl->icon,strlen(fl->icon));
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_SET_CURSOR_END,ws,256);
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,1);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);

  
  wsprintf(ws,per_t,LG_ACTION);//"\nДействие:\n"
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ws,256);
  AddEditControlToEditQend(eq,&ec,ma);

  str_2ws(ws,fl->type,strlen(fl->type));
  ConstructComboBox(&ec,7,ECF_APPEND_EOL,ws,10,0,11,1);//0x40
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);
  
  
  wsprintf(ws,per_t,LG_PATH);//"Путь:\n"
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ws,256);
  AddEditControlToEditQend(eq,&ec,ma);

  str_2ws(ws,fl->path,strlen(fl->path));
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_SET_CURSOR_END,ws,256);
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,1);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);
  

  disk_prop_hdr.lgp_id = (int)LG_EDIT;        
  patch_header(&disk_prop_hdr);
  patch_input(&edit_desc);      

  FreeWS(ws);
  return CreateInputTextDialog(&edit_desc,&disk_prop_hdr,eq,1,0);
}


//-----------------------Edit chat-----------------------//


typedef struct
{
  unsigned short u;
  char dos;
  char win;
  char koi8;
} TUNICODE2CHAR;

const TUNICODE2CHAR unicode2char[]=
{
  // CAPITAL Cyrillic letters (base)
  0x410,0x80,0xC0,0xE1, // А
  0x411,0x81,0xC1,0xE2, // Б
  0x412,0x82,0xC2,0xF7, // В
  0x413,0x83,0xC3,0xE7, // Г
  0x414,0x84,0xC4,0xE4, // Д
  0x415,0x85,0xC5,0xE5, // Е
  0x416,0x86,0xC6,0xF6, // Ж
  0x417,0x87,0xC7,0xFA, // З
  0x418,0x88,0xC8,0xE9, // И
  0x419,0x89,0xC9,0xEA, // Й
  0x41A,0x8A,0xCA,0xEB, // К
  0x41B,0x8B,0xCB,0xEC, // Л
  0x41C,0x8C,0xCC,0xED, // М
  0x41D,0x8D,0xCD,0xEE, // Н
  0x41E,0x8E,0xCE,0xEF, // О
  0x41F,0x8F,0xCF,0xF0, // П
  0x420,0x90,0xD0,0xF2, // Р
  0x421,0x91,0xD1,0xF3, // С
  0x422,0x92,0xD2,0xF4, // Т
  0x423,0x93,0xD3,0xF5, // У
  0x424,0x94,0xD4,0xE6, // Ф
  0x425,0x95,0xD5,0xE8, // Х
  0x426,0x96,0xD6,0xE3, // Ц
  0x427,0x97,0xD7,0xFE, // Ч
  0x428,0x98,0xD8,0xFB, // Ш
  0x429,0x99,0xD9,0xFD, // Щ
  0x42A,0x9A,0xDA,0xFF, // Ъ
  0x42B,0x9B,0xDB,0xF9, // Ы
  0x42C,0x9C,0xDC,0xF8, // Ь
  0x42D,0x9D,0xDD,0xFC, // Э
  0x42E,0x9E,0xDE,0xE0, // Ю
  0x42F,0x9F,0xDF,0xF1, // Я
  // CAPITAL Cyrillic letters (additional)
  0x402,'_',0x80,'_', // _ .*.*
  0x403,'_',0x81,'_', // _ .*.*
  0x409,'_',0x8A,'_', // _ .*.*
  0x40A,'_',0x8C,'_', // _ .*.*
  0x40C,'_',0x8D,'_', // _ .*.*
  0x40B,'_',0x8E,'_', // _ .*.*
  0x40F,'_',0x8F,'_', // _ .*.*
  0x40E,0xF6,0xA1,'_', // Ў ...*
  0x408,0x4A,0xA3,0x4A, // _ .*.*
  0x409,0x83,0xA5,0xBD, // _ .*..
  0x401,0xF0,0xA8,0xB3, // Ё
  0x404,0xF2,0xAA,0xB4, // Є
  0x407,0xF4,0xAF,0xB7, // Ї
  0x406,0x49,0xB2,0xB6, // _ .*..
  0x405,0x53,0xBD,0x53, // _ .*.*
  // SMALL Cyrillic letters (base)
  0x430,0xA0,0xE0,0xC1, // а
  0x431,0xA1,0xE1,0xC2, // б
  0x432,0xA2,0xE2,0xD7, // в
  0x433,0xA3,0xE3,0xC7, // г
  0x434,0xA4,0xE4,0xC4, // д
  0x435,0xA5,0xE5,0xC5, // е
  0x436,0xA6,0xE6,0xD6, // ж
  0x437,0xA7,0xE7,0xDA, // з
  0x438,0xA8,0xE8,0xC9, // и
  0x439,0xA9,0xE9,0xCA, // й
  0x43A,0xAA,0xEA,0xCB, // к
  0x43B,0xAB,0xEB,0xCC, // л
  0x43C,0xAC,0xEC,0xCD, // м
  0x43D,0xAD,0xED,0xCE, // н
  0x43E,0xAE,0xEE,0xCF, // о
  0x43F,0xAF,0xEF,0xD0, // п
  0x440,0xE0,0xF0,0xD2, // р
  0x441,0xE1,0xF1,0xD3, // с
  0x442,0xE2,0xF2,0xD4, // т
  0x443,0xE3,0xF3,0xD5, // у
  0x444,0xE4,0xF4,0xC6, // ф
  0x445,0xE5,0xF5,0xC8, // х
  0x446,0xE6,0xF6,0xC3, // ц
  0x447,0xE7,0xF7,0xDE, // ч
  0x448,0xE8,0xF8,0xDB, // ш
  0x449,0xE9,0xF9,0xDD, // щ
  0x44A,0xEA,0xFA,0xDF, // ъ
  0x44B,0xEB,0xFB,0xD9, // ы
  0x44C,0xEC,0xFC,0xD8, // ь
  0x44D,0xED,0xFD,0xDC, // э
  0x44E,0xEE,0xFE,0xC0, // ю
  0x44F,0xEF,0xFF,0xD1, // я
  // SMALL Cyrillic letters (additional)
  0x452,'_',0x90,'_', // _ .*.*
  0x453,'_',0x83,'_', // _ .*.*
  0x459,'_',0x9A,'_', // _ .*.*
  0x45A,'_',0x9C,'_', // _ .*.*
  0x45C,'_',0x9D,'_', // _ .*.*
  0x45B,'_',0x9E,'_', // _ .*.*
  0x45F,'_',0x9F,'_', // _ .*.*
  0x45E,0xF7,0xA2,'_', // ў ...*
  0x458,0x6A,0xBC,0x6A, // _ .*.*
  0x491,0xA3,0xB4,0xAD, // _ .*..
  0x451,0xF1,0xB8,0xA3, // ё
  0x454,0xF3,0xBA,0xA4, // є
  0x457,0xF5,0xBF,0xA7, // ї
  0x456,0x69,0xB3,0xA6, // _ .*..
  0x455,0x73,0xBE,0x73, // _ .*.*
  0x0A0,'_',0xA0,0x20, // space .*..
  0x0A4,'_',0xA4,0xFD, // ¤   .*..
  0x0A6,'_',0xA6,'_', // ¦   .*.*
  0x0B0,0xF8,0xB0,0x9C, // °
  0x0B7,0xFA,0xB7,0x9E, // ·
  // 0x2022,,0x95,0x95, //    .*..
  // 0x2116,0xFC,0xB9,0x23, // №   ...*
  // 0x2219,,0xF9,0x9E, //    .*..
  // 0x221A,0xFB,,0x96, // v   ..*.
  // 0x25A0,0xFE,,0x94, // ¦
  0x0000,0,0,0
};

const char wintranslation[128]=
{
  0x5F,0x5F,0x27,0x5F,0x22,0x3A,0xC5,0xD8,0x5F,0x25,0x5F,0x3C,0x5F,0x5F,0x5F,0x5F,
  0x5F,0x27,0x27,0x22,0x22,0x07,0x2D,0x2D,0x5F,0x54,0x5F,0x3E,0x5F,0x5F,0x5F,0x5F,
  0xFF,0xF6,0xF7,0x5F,0xFD,0x83,0xB3,0x15,0xF0,0x63,0xF2,0x3C,0xBF,0x2D,0x52,0xF4,
  0xF8,0x2B,'I' ,'i' ,0xA3,0xE7,0x14,0xFA,0xF1,0xFC,0xF3,0x3E,0x5F,0x5F,0x5F,0xF5,
  0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
  0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
  0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
  0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF
};
const unsigned short dos2unicode[128]=
{
  0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,
  0x0418,0x0419,0x041A,0x041B,0x041C,0x041D,0x041E,0x041F,
  0x0420,0x0421,0x0422,0x0423,0x0424,0x0425,0x0426,0x0427,
  0x0428,0x0429,0x042A,0x042B,0x042C,0x042D,0x042E,0x042F,
  0x0430,0x0431,0x0432,0x0433,0x0434,0x0435,0x0436,0x0437,
  0x0438,0x0439,0x043A,0x043B,0x043C,0x043D,0x043E,0x043F,
  0x002D,0x002D,0x002D,0x00A6,0x002B,0x00A6,0x00A6,0x00AC,
  0x00AC,0x00A6,0x00A6,0x00AC,0x002D,0x002D,0x002D,0x00AC,
  0x004C,0x002B,0x0054,0x002B,0x002D,0x002B,0x00A6,0x00A6,
  0x004C,0x0433,0x00A6,0x0054,0x00A6,0x003D,0x002B,0x00A6,
  0x00A6,0x0054,0x0054,0x004C,0x004C,0x002D,0x0433,0x002B,
  0x002B,0x002D,0x002D,0x002D,0x002D,0x00A6,0x00A6,0x002D,
  0x0440,0x0441,0x0442,0x0443,0x0444,0x0445,0x0446,0x0447,
  0x0448,0x0449,0x044A,0x044B,0x044C,0x044D,0x044E,0x044F,
  0x0401,0x0451,0x0404,0x0454,0x0407,0x0457,0x040E,0x045E,
  0x00B0,0x2022,0x00B7,0x0076,0x2116,0x00A4,0x00A6,0x00A0
};
const char koi8translation[128]=
{
  0x5F,0x5F,0x27,0x5F,0x22,0x3A,0xC5,0xD8,0x5F,0x25,0x5F,0x3C,0x5F,0x5F,0x5F,0x5F,
  0x5F,0x27,0x27,0x22,0x22,0x07,0x2D,0x2D,0x5F,0x54,0x5F,0x3E,0x5F,0x5F,0x5F,0x5F,
  0xFF,0xF6,0xF7,0xF1,0xF3,0x5F,'i' ,0xF5,0xF0,0x63,0xF2,0x3C,0xBF,0xA3,0x52,0xF4,
  0xF8,0x2B,0x5F,0xF0,0xF2,0xE7,'I' ,0xF4,0xF1,0xFC,0xF3,0x3E,0x5F,0x83,0x5F,0xF5,
  
  0xEE,0xA0,0xA1,0xE6,0xA4,0xA5,0xE4,0xA3,0xE5,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,
  0xAF,0xEF,0xE0,0xE1,0xE2,0xE3,0xA6,0xA2,0xEC,0xEB,0xA7,0xE8,0xED,0xE9,0xE7,0xEA,
  0x9E,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,
  0x8F,0x9F,0x90,0x91,0x92,0x93,0x86,0x82,0x9C,0x9B,0x87,0x98,0x9D,0x99,0x97,0x9A
};

#pragma inline
unsigned int char16to8(unsigned int c)
{
  const TUNICODE2CHAR *p=unicode2char;
  unsigned int i;
  if (c<128) return(c);
  while((i=p->u))
  {
    if (c==i)
    {
      return(p->win);
    }
    p++;
  }
  c&=0xFF;
  if (c<32) return(' ');
  return(c);
}

unsigned int char8to16(int c, int type)
{
  if (c>=128)
  {
    switch(type)
    {
    case 1:
      //Win->Dos
      c=wintranslation[c-128];
      break;
    case 2:
      //Koi8->Dos
      c=koi8translation[c-128];
      break;
    }
    if (c<128) return(c);
    return(dos2unicode[c-128]);
  }
  return(c);
}

/*
void ascii2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    wsAppendChar(ws,char8to16(c,1));
  }
}*/
unsigned int win2unicode(int letter)
{
  if (letter<168) goto L_ENG;
  if (letter==168) goto L_BIG_YO;
  letter-=168;
  if (letter<24) goto L_UKR;
  if (letter>87) goto L_ENG;
//L_RUS:
  letter-=8;
  goto L_ADD_400;
L_UKR:
  switch(letter)
  {
  case 0x02:
    letter=0x04;
    break;
  case 10:
    letter=6;
    break;
  case 11:
    letter=0x56;
    break;
  case 16:
    letter=0x51;
    break;
  case 18:
    letter=0x54;
    break;
  case 23:
    letter=0x57;
    break;
  default:
    goto L_ENG;
  }
  goto L_ADD_400;
L_BIG_YO:
  letter=0x01;
L_ADD_400:
  return (letter+0x400);
  
L_ENG: return (letter);
}


void ascii2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    wsAppendChar(ws,win2unicode(c));
  }
}

void koi2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    wsAppendChar(ws,char8to16(c,2));
  }
}
void win1251_2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    wsAppendChar(ws,char16to8(c));
  }
}


