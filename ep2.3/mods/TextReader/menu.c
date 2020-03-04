#include "..\inc\swilib.h"
#include "main.h"
extern int codepage;
extern int id_ed;
extern int id_ed2;
extern char *file;
extern char exe_name[];
extern int curpos;
extern int atoi(char *attr);
extern void *about();

extern const char per_d[];
extern const char per_t[];
extern const char per_s[];
extern const char empty_str[];
const char per_2s[]="%s%s";

extern const char IMG_PATH[128];
extern const char HIST_PATH[128];
extern const char *successed_config_filename;

extern void ShowBookmarks(void);
extern int LoadBookmark();
extern void UpdateCSMname();
void ShowMenuLast(void);
int LoadHist();


extern void patch_header( HEADER_DESC* head);
extern void patch_input( INPUTDIA_DESC* inp);
//------------------------MENU------------------------//
extern int id_ed;
int last_id;
int MenuCode_id;
int item=8;//cколько пунктов меню
int code=5;//Codepage
int go_to_item=4;//cколько пунктов для Go to...
int last_item=0;//Сколько есть истории...
int menu_id;
char *name[]={
/*
  "Выбрать файл",
  "Новый файл",
  "Последние...",
  "Перейти к...",
  "Сохранить",
  "Настройки",
  "Об эльфе",
  "Выход"*/
  LG_SELECT_FILE,
  LG_NEW_FILE,
  LG_LAST,
  LG_GOTO,
  LG_SAVE,
  LG_SET,
  LG_ABOUT,
  LG_EXIT
};
char *name_codepage[]={
  "WIN1251",
  "UTF8",
  "KOI8",
  "ASCII",
  "WIN1250"
};
char *go_to_str[]={
  /*"В начало",
  "В конец",
  "На линию...",
  "Закладки..."*/
  LG_START,
  LG_END,
  LG_LINE,
  LG_BOOKMARK
};

 char ICON0[128];//пути к иконкам меню
 char ICON1[128];//"\\open.png";//пути к иконкам
 char ICON2[128];//"\\arrow.png";
 char ICON3[128];//"\\goto.png";
 char ICON4[128];//"\\save.png";
 char ICON5[128];//"\\options.png";
 char ICON6[128];//"\\about.png";
 char ICON7[128];//"\\quit.png";
 char NO_ICON[128];//"\\no_icon.png";

int S_ICONS[9]={
(int)ICON0,
(int)ICON1,
(int)ICON2,
(int)ICON3,
(int)ICON4,
(int)ICON5,
(int)ICON6,
(int)ICON7,
(int)NO_ICON
};

//---------------------Создаем список меню-----------------------//

void bm_menu_iconhnd(void *data, int curitem, void *unk);
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0001,0x0000,(int)LG_SELECT},
  {0x0004,0x0000,(int)LG_CLOSE},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

void bm_menu_ghook(void *data, int cmd){};
int bm_menu_onkey2(void *data, GUI_MSG *msg);
HEADER_DESC bm_menuhdr2={0,0,131,21,NULL,(int)LG_MENU,LGP_NULL};
const MENU_DESC bmmenu=
{
  8,bm_menu_onkey2,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x10,//0x11,
  bm_menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  7   //n
};
void bm_menu_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  char *s;
  void *item=AllocMenuItem(data);
  s=malloc(64);
  sprintf(s,per_s,name[curitem]);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+4);
      wsprintf(ws,per_t,s);
    }
    else 
    { 
      ws=AllocMenuWS(data,10);
      wsprintf(ws,per_t,"no name");
    }
  }
  else  
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей

  }
  // SetMenuItemIconArray(data,item,S_ICONS + curitem + 1);
   SetMenuItemText(data,item,ws,curitem);
}

void ShowMenuGoTo(void);

int bm_menu_onkey2(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
 if(k=='*')
   {
     about();
     return (-1);
   }
  if (k==ENTER_BUTTON || k==LEFT_SOFT)
  {
    switch(i)
      {
        case 0:  open_fm();    break;
        case 1:  new_file_ec();break;
        case 2: 
          LoadHist();
          ShowMenuLast();
          break;
        case 3:  ShowMenuGoTo(); break;
        case 4:  SaveFile(file); break;
        case 5:  start(successed_config_filename); break;
        case 6:  about(); return 0;
        case 7:  //exit
            GeneralFunc_flag1(MenuCode_id,1);
            GeneralFunc_flag1(last_id,1);
            GeneralFunc_flag1(id_ed,1);
            GeneralFunc_flag1(menu_id,1);
            break;
      }
    return(1);
  }
 if(k==RIGHT_SOFT || k==RED_BUTTON)
 {
   GeneralFunc_flag1(menu_id,1);
//   GeneralFunc_flag1(MenuCode_id,1);
  // GeneralFunc_flag1(last_id,1);
 }
  return(0);
}

void ShowMainMenu(void)
{ /*
 snprintf(ICON1,127,per_2s,IMG_PATH,"\\open.png");
 sprintf(ICON2,per_2s,IMG_PATH,"\\arrow.png");
 sprintf(ICON3,per_2s,IMG_PATH,"\\goto.png");
 sprintf(ICON4,per_2s,IMG_PATH,"\\save.png");
 sprintf(ICON5,per_2s,IMG_PATH,"\\options.png");
 sprintf(ICON6,per_2s,IMG_PATH,"\\about.png");
 sprintf(ICON7,per_2s,IMG_PATH,"\\quit.png");
 sprintf(NO_ICON,per_2s,IMG_PATH,"\\no_icon.png");*/
 
 patch_header(&bm_menuhdr2);
 menu_id=CreateMenu(0,0,&bmmenu,&bm_menuhdr2,0,item,0,0);
}

//----------------------------Выбор кодировки----------------------------//

void codepage_iconhnd(void *data, int curitem, void *unk);
const int menusoftkeys_codepage[]={0,1,2};

const SOFTKEY_DESC menu_sk_codepage[]=
{
  {0x0001,0x0000,(int)LG_SELECT},
  {0x0004,0x0000,(int)LG_CLOSE},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt_codepage=
{
  menu_sk_codepage,0
};

void codepage_ghook(void *data, int cmd){};
int codepage_onkey(void *data, GUI_MSG *msg);
HEADER_DESC codepage_menuhdr={0,0,131,21,NULL,(int)LG_CODEPAGE,LGP_NULL};

const MENU_DESC codepage_menu=
{
  8,codepage_onkey,codepage_ghook,NULL,
  menusoftkeys_codepage,
  &menu_skt_codepage,
  0x10,//0x11,
  codepage_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  5   //n
};

void codepage_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  char *s;
  void *item=AllocMenuItem(data);
  s=malloc(32);
  sprintf(s,per_s,name_codepage[curitem]);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+4);
      wsprintf(ws,per_s,s);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей

  }
  SetMenuItemText(data,item,ws,curitem);
}

int ShowMenuCode()
{ 
  patch_header(&codepage_menuhdr);
  return MenuCode_id=CreateMenu(0,0,&codepage_menu,&codepage_menuhdr,0,code,0,0);
}

int codepage_onkey(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
  
  if (k==ENTER_BUTTON || k==LEFT_SOFT)
  {
    switch(i)
     {
        case 0: codepage=0; break;//win1251      
        case 1: codepage=1; break;//utf8
        case 2: codepage=2; break;//koi8
        case 3: codepage=3; break;//ascii
        case 4: codepage=4; break;//win1250
     }    
   GeneralFunc_flag1(id_ed,1); 
   DrawText(file,codepage);
   return(0);
  }
  
 if(k==RIGHT_SOFT || k==RED_BUTTON) GeneralFunc_flag1(MenuCode_id,1); 
return(0);
}

//--------------------------------menu GO TO...-------------------------------//

void go_to_iconhnd(void *data, int curitem, void *unk);
void go_to_ghook(void *data, int cmd){};
int go_to_onkey2(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
  
  if (k==ENTER_BUTTON || k==LEFT_SOFT)
  {
    switch(i)
      {
         case 0:
            curpos=1;
            GeneralFunc_flag1(id_ed,1);//close
            DrawText(file,codepage);//open
            break;//В начало
         case 1:
            curpos=0;
            GeneralFunc_flag1(id_ed,1);
            DrawText(file,codepage);
            break;//В конец
         case 2:
            GeneralFunc_flag1(id_ed,1);
            pos();
            break;//Позиция
         case 3:
            ShowBookmarks();
            break;
      }
    return(1);
  }
  return(0);
}
HEADER_DESC go_to_menuhdr2={0,0,131,21,NULL,(int)LG_GOTO,LGP_NULL};
const MENU_DESC go_to_menu=
{
  8,go_to_onkey2,go_to_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x10,
  go_to_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

void go_to_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  char *s;
  void *item=AllocMenuItem(data);
  s=malloc(32);
  sprintf(s,per_s,go_to_str[curitem]);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+4);
      wsprintf(ws,per_t,s);
    }
    else 
    {
      ws=AllocMenuWS(data,10);
      wsprintf(ws,"no name!");
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей
  }
  SetMenuItemText(data,item,ws,curitem);
  mfree(s);
}

void ShowMenuGoTo(void)
{ 
   patch_header(&go_to_menuhdr2);
   CreateMenu(0,0,&go_to_menu,&go_to_menuhdr2,0,go_to_item,0,0);
}


//-----------------------------menu LAST OPEN...------------------------------//

//бляяяя хз че я написал тут,сам путаюсь....а писал трезвый то....
//не стоит в дипрессии писать наверн все таки....

typedef struct
{
 char name[64];
}LAST;

LAST last_op[];
extern char *file_name;

int RunHST(char *file)//Запуск в новом окне так сказать:)
{
  if (file)
  {
    file_name=GetName(file,1);//Понятно из названия...получаем имя файла...:)
    if (strlen(file))
    {
      WSHDR *ws;
      ws=AllocWS(150);
      str_2ws(ws,exe_name,128);
      ExecuteFile(ws,0,file);
      FreeWS(ws);
      return(1);
    }
  }
  return(0);
}

char *ff;//путь к файлу из истории, тобишь какой нам нужно запустить из пункта "Последние..."

void LoadHST(int curitem)//читаем путь к файлу из файла истории...о как замутил бля...
{
  unsigned int err; 
  int f; 
  char *mem;
  ff=malloc(64);
  char path[256];//="0:\\zbin\\TextReader\\history\\"
  sprintf(path,per_s,HIST_PATH);
  strcat(path,last_op[curitem].name);
  f = fopen(path, A_ReadOnly + A_BIN, P_READ, & err ); 
  if ( f!=-1 ) 
   {
     mem=malloc(256); 
     int size=fread(f,mem,256,&err); 
     int i=0;
     while(i<size)//читаем строку из файла 
     {
       if(*(mem+i)==' ') break;
       *(ff+i)=*(mem+i);
       i++;
     } 
     *(ff+i)=0;
   }
  else curpos=1;
 fclose(f,&err); 
mfree(mem); 
}

int LoadHist()//Ищем все файлы истории и сохраняем имена
{
   DIR_ENTRY de;
   unsigned int err;
   char *path=malloc(256);//="0:\\zbin\\TextReader\\history\\";
   sprintf(path,per_s,HIST_PATH);
//   ShowMSG(1,(int)path);
   char *ptr=path+strlen(path)+1;
   strcat(path,"*.hst");
   if (FindFirstFile(&de,path,&err))
      { 
       do{
          strcpy(ptr,de.file_name);  
          strcpy(last_op[last_item].name,de.file_name);
          last_item++;
         }while(FindNextFile(&de,&err));
      }FindClose(&de,&err);
   mfree(path);
return last_item;
}

char *ss;

void last_iconhnd(void *data, int curitem, void *unk);
void last_ghook(void *data, int cmd){};

int last_onkey2(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
  unsigned int err;
  
  if(k==GREEN_BUTTON) //Открыть в новом окне
  {
    GeneralFunc_flag1(last_id,1);
    LoadHST(i);
    RunHST(ff);   
  }
  
  if(k==ENTER_BUTTON) //Открыть в текущем окне, без сохранения старого!!!
  {
    LoadHST(i);
    GeneralFunc_flag1(id_ed,1);
    GeneralFunc_flag1(last_id,1);
    strcpy(file,ff);
    ShowMenuCode();
//    DrawText(ff,0);
  }
  
  if(k=='#')//Показываем путь к файлу
  {    
    LoadHST(i);
    ss=malloc(128);
    sprintf(ss,"%s%s",LG_PATHFILE,ff);
    ShowMSG(2,(int)ss);
  }
  char *s;
  
  if(k=='*')//Удалить файл истории
  {    
    s=malloc(128);
    ss=malloc(128);
    sprintf(ss,per_s,HIST_PATH);
    strcat(ss,last_op[i].name);
    unlink(ss,&err);
    sprintf(s,LG_DELFILE,ss);
    ShowMSG(2,(int)s);
  }  
  if(k==RIGHT_SOFT) GeneralFunc_flag1(last_id,1);
  return(0);
}

HEADER_DESC last_menuhdr2={0,0,131,21,NULL,(int)LG_LAST,LGP_NULL};

const MENU_DESC last_menu=
{
  8,last_onkey2,last_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x10,//0x11,
  last_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

void last_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  char *s;
  void *item=AllocMenuItem(data);
  s=malloc(128);
  LoadHST(curitem);
  sprintf(s,per_s,ff);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+4);
      wsprintf(ws,"%d.%s",curitem+1,s);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей

  }
   SetMenuItemText(data,item,ws,curitem);
   mfree(s);
}

void ShowMenuLast(void)
{ 
   patch_header(&last_menuhdr2);
   last_id=CreateMenu(0,0,&last_menu,&last_menuhdr2,0,last_item,0,0);
}

