#include "..\inc\swilib.h"
#include "language.h"
#include "main.h"
#include "File.h"
#include "List.h"

CList *list;
MenuList *mlist;

CSM_DESC icsmd;
int id_ed;
int draw_id;

unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;

const char per_t[]="%t";
const char per_s[]="%s";
const char empty_str[]="";

char hdr_txt[64];
int readonly=0;

int total_char=0;
char dir[128];//="0:\\mark";

EDITCONTROL ec;

volatile int prev_itemcount;

//MNU *mnutop;
#define mnutop list->gettop();

GBSTMR mytmr;

int DrawText(int i,MNU *fl);

void about()
{
  char s[128];
  snprintf(s,128,"Marks v0.2 Cpp\n(c)kluchnik\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
}

void Message(char *s)
{
  ShowMSG(1,(int)s);
}

void Message(unsigned long i)
{
  char *s=(char*)malloc(64);
  sprintf(s,"%d",i);
  ShowMSG(1,(int)s);
  mfree(s);
}

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

//------------------------------------------//

char *fdt2s(unsigned int time, char *buf)//Разберем дату и время
{
  short y,M,d,h,m;//,s;
  y = (time >> 25) + 80;
  if (y>100) y -= 100;
  M = (time >> 21) & 0x0f;
  d = (time >> 16) & 0x1f;

  h = (time >> 11) & 0x1f;
  m = (time >> 5)  & 0x3f;

  sprintf(buf, "%.2d.%.2d.%.2d %.2d:%.2d", d,M,y,h,m);
  return buf;
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

//------------------------------------------//

void Add(const char *filename,const char *text,char *dt,int is_first)
{
  MNU *fn = (MNU*)malloc(sizeof(MNU));
  fn->filename = (char*)malloc(strlen(filename)+1);
  fn->name=(char*)malloc(32);
  fn->text=(char*)malloc(strlen(text)+1);
  fn->dat=(char*)malloc(strlen(dt));
  
  strcpy(fn->filename,filename);
  strncpy(fn->name,text,32);
  strcpy(fn->text,text);
  strcpy(fn->dat,dt);

  fn->is_first = is_first;   
  fn->next=0;
  
  list->add(fn);
}

void ws2str(WSHDR *ws,char *s)
{
  int i;
  for(i=0;i<ws->wsbody[0];i++)
   *(s+i)=char16to8(ws->wsbody[i+1]);
   *(s+i++)=0;
}

static const char *LoadTEXT(char *path)
{
  unsigned int err; 
  int f; 
  char *buf;
  if((f = fopen(path,A_ReadOnly+A_BIN,P_READ,&err ))!=-1)
  {
    buf=(char*)malloc(get_file_size(path)); 
    buf[fread(f,buf,get_file_size(path),&err)]=0;        
  }
  fclose(f,&err); 
 return buf;
}


void LoadMark()//Ищем все файлы истории и сохраняем имена,текст и дату создания
{
   Add("<Создать новую запись>","<Создать новую запись>","",1);//Добавляем в начало пункт
   DIR_ENTRY de;
   unsigned int err;
   char *s = (char*)malloc(32);
   char *path = (char*)malloc(256);
   if(!(isdir(dir,&err)))
   {
     mkdir(dir,&err);//Если директории нет,то создаем ее
     SetFileAttrib(dir,FA_HIDDEN,&err);
   }
   sprintf(path,per_s,dir);
   char *ptr = path+strlen(path)+1;
   strcat(path,"\\*.txt");
   if(FindFirstFile(&de,path,&err))
    {
     do
     {
       strcpy(ptr,de.file_name);  
       Add(de.file_name,LoadTEXT(path),fdt2s(de.create_date_time, s),0);
       prev_itemcount++;
     }
     while(FindNextFile(&de,&err));
    }
   FindClose(&de,&err);
   mfree(path);
   mfree(s);
}

//------------------- Т9 ------------------//

//Ключи для поиска по T9
static const char table_T9Key[257]=
"11111111111111111111111111111111"
"10001**0***0000*012345678900***0"
"0222333444555666777788899991*110"
"122233344455566677778889999111*1"
"11111111111111111111111111111111"
"11111111311111111111111131111111"
"22223333444455566677778888899999"
"22223333444455566677778888899999";


char T9Key[32];
char _sk_r[16];
char main_hdr_txt[64];

MNU *FindBCFGByNS(int *i,int si)
{
  MNU *t;
  t=(MNU *)mnutop;
  char *s;
  char *d;
  int c;
  while(t)
  {
   s=T9Key;
   d=t->name;
   while(c=*s++)
   {
    if(c!=table_T9Key[*d++]) goto L_NOT9;
   }
   if(t->is_first==si)
    {
      if(!(*i)) return(t);
      (*i)--;
    }
 L_NOT9:
    t=t->next;
  }
  return(t);
}

#define IS_ANY 0xFFFFFFFF

MNU *FindFLISTtByN(int n)
{
  MNU *fl;
  fl=FindBCFGByNS(&n,1); if ((!n)&&(fl)) return(fl);
  fl=FindBCFGByNS(&n,0); if ((!n)&&(fl)) return(fl);
  return fl;
}

void ClearT9Key(void)
{
  zeromem(T9Key,sizeof(T9Key));
}

int CountBCFG(void)
{
  int l=-1;
//  FindBCFGByNS(&l,IS_ANY);
  FindBCFGByNS(&l,1);
  FindBCFGByNS(&l,0);
  l=-1-l;
  return l;
}

//----------------------------------//

void UpdateHeader(void)
{
  if (strlen(T9Key))
  {
    strcpy(main_hdr_txt,"Ввод T9:");
    strcat(main_hdr_txt,T9Key);
    strcpy(_sk_r,"<C");
  }
  else
  {
    strcpy(main_hdr_txt,"Заметки");
    strcpy(_sk_r,"Выход");
  }
}

void AddT9Key(int chr)
{
  int l=strlen(T9Key);
  if(l<(sizeof(T9Key)-1))
  {
    T9Key[l]=chr;
  }
}

void BackSpaceT9(void)
{
  int l=strlen(T9Key);
  if(l)
  {
    l--;
    T9Key[l]=0;
  }
}

static void RecountMenuBCFG(void *data)
{
  int i;
  UpdateHeader();
  if (!id_ed) return; //Нечего считать
  i=CountBCFG();
  if(i!=prev_itemcount)
  {
    prev_itemcount=i;
    Menu_SetItemCountDyn(data,i);
  }
  SetCursorToMenuItem(data,0);
  if (IsGuiOnTop(id_ed)) RefreshGUI();
}

//------------------------Создаем список--------------------------//

void bm_menu_iconhnd(void *data, int curitem, void *unk);
HEADER_DESC bm_menuhdr={0,0,0,0,NULL,(int)main_hdr_txt,LGP_NULL};
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)"Удалить"},
  {0x0001,0x0000,(int)_sk_r},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

int item=0;

void bm_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
//    Menu_SetItemCountDyn(data,CountBCFG());
  }
};

MNU *FindByN(int n)
{
  MNU *fl;
  int i=0;
  int j=0;
  int k = list->Count(); //GetFListN2();
  fl = list->gettop();  //(MNU *)mnutop;
  j=k-n-1;
  for(i=0; i<j && (fl) ;i++)
   {
     fl=fl->next;
   }
  return fl;
}

void DelItem(MNU *fl/*char *filename*/)
{
  if(!fl) return;
  //fl->filename
  char *s = (char*)malloc(256);
  unsigned int err;
  sprintf(s,"%s\\%s",dir,fl->filename);
  unlink(s,&err);
  
  //list->FreeList();
//  LoadMark();
  list->Delete(fl,-1);
//  Message("File deleted");
 // if(IsGuiOnTop(id_ed)) RefreshGUI();
  mfree(s);
}


int bm_menu_onkey2(void *data, GUI_MSG *msg)
{
 int i;
 i=GetCurMenuItem(data);
 int k=msg->gbsmsg->submess;
 MNU *fl;
 fl=FindFLISTtByN(i);
 if(msg->keys==0x3D)
 {
   if(fl)
   {
    if(strlen(T9Key))
    {
      ClearT9Key();
      RecountMenuBCFG(data);
    }
    readonly=0; 
    DrawText(NULL,fl);
   }
   return (-1);
 }
 /*
 if(k==LEFT_SOFT)  
 {
   delname=malloc(strlen(fl->filename));
   strcpy(delname,fl->filename);
   changefile=fl;
   ShowMainMenu();
 }*/
 if(k==LEFT_BUTTON) return 1;
 if(k=='*') about(); 
 if(k=='#')         
 {
   DelItem(fl);
 } 

 if (msg->gbsmsg->msg==KEY_DOWN)
  {
    int key=msg->gbsmsg->submess;
    if((key>='0')&&(key<='9'))
    {
      AddT9Key(key);
      RecountMenuBCFG(data);
      return(-1);
    }
  }
  
  if (msg->keys==1 || msg->keys==RED_BUTTON)
  {
    if (strlen(T9Key))
    {
      BackSpaceT9();
      RecountMenuBCFG(data);
      return(-1);
    }
  }
return 0;
}

const ML_MENU_DESC bmmenu=//Bookmarks
{
  8,bm_menu_onkey2,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x10,
  bm_menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0,   //n
  1  
};

void bm_menu_iconhnd(void *data, int curitem, void *unk)
{
  WSHDR *ws1;
  WSHDR *ws;
  void *item=AllocMLMenuItem(data);
  MNU *fl;
  
  fl=FindFLISTtByN(curitem);
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
}

int create()
{
  int i;
  i=CountBCFG();
  prev_itemcount=i;
  UpdateHeader();
  patch_header(&bm_menuhdr);
  return id_ed=CreateMultiLinesMenu(0,0,&bmmenu,&bm_menuhdr,0,i);
}

char *GetDate()
{
  TDate date;
  TTime time;
  GetDateTime(&date,&time);
  char *s = (char*)malloc(64);
  sprintf(s,"%02d-%02d-%02d_%02d:%02d",date.day,date.month,date.year-2000,time.hour,time.min);
  return s;
}

//--------------------------------Редактор--------------------------------//

char *cmp;
MNU *FindFileByName(char *name)
{
  MNU *fl;
  for(int k=0;k<list->Count()/*GetFListN2()*/;k++)//Ищем имя файла
  {
   fl=FindFLISTtByN(k);
   if(strcmp(cmp,fl->filename)==0) break;
   }
  return fl;
}

HEADER_DESC disk_prop_hdr={0,0,0,0,NULL,(int)hdr_txt,LGP_NULL};
int empty_onkey(GUI *data, GUI_MSG *msg)
{
  int k=msg->gbsmsg->submess;
  MNU *fl;
  MNU *nw;
  unsigned long num=0x0;
  unsigned int err;
  char *ss;
  WSHDR *ws;
  int last_name=0;

  if(msg->keys==0xFFF && readonly==1)//Если нажали "Сохранить" и у нас режим редактирования
  {
    EDITCONTROL ec;
    fl=FindFileByName(cmp);//Ищем запись по имени файла
    ExtractEditControl(data,1,&ec);	
    ws=ec.pWS;
    void *data=FindGUIbyId(id_ed,NULL);
    if(fl->is_first)//Если нажали "Создать заметку"
    {
      for(int t=0;t<list->Count();t++)//Ищем последнюю запись чтобы узнать последнее имя,на всяк случай...
      {
        nw = FindFLISTtByN(t);
        num = strtoul(nw->filename,0,16);
        if(num > last_name)
          last_name = num;
      }
      num=last_name;
      num+=0x01;
      sprintf(nw->filename,"%02X.txt",num);
      nw->text = (char*)malloc(ws->wsbody[0]);
      ws2str(ws,nw->text);
      SaveMark(nw->filename,nw->text);
      
      readonly=0;//Сохраняемся и открываем созданную запись.
      Add(nw->filename,nw->text,(char*)empty_str,0);
      GeneralFuncF1(1);
    }
    else
    {
     ss=(char*)malloc(256);
     sprintf(ss,"%s\\%s",dir,fl->filename);
     unlink(ss,&err);
     fl->text=(char*)malloc(ws->wsbody[0]);
     ws2str(ws,fl->text); 
     SaveMark(fl->filename,fl->text);
     strncpy(fl->name,fl->text,32);
     
     readonly=0;//Сохраняемся и переходим в режим реадонли:)
//     RecountMenuBCFG(data);
     GeneralFuncF1(1);
     DrawText(NULL,fl);
    }
  }

 if(msg->gbsmsg->msg==KEY_DOWN &&( ( (k>='0' && k<='9')||(k==ENTER_BUTTON) ) && (readonly==0)))
 {
   fl=FindFileByName(cmp);
   readonly=1;
   GeneralFunc_flag1(draw_id,1);
   DrawText(NULL,fl);
 }
 
 if(msg->gbsmsg->msg==KEY_DOWN  || msg->gbsmsg->msg==LONG_PRESS )
 {
   if((k==UP_BUTTON && readonly==0) && item!=0)
    {
      GeneralFunc_flag1(draw_id,1);
      int y;
      int num;
      y=strtoul(cmp,0,16);
      if(y>0x01) y-=0x01;
      else 
      {
       for(int t=0;t</*GetFListN2()*/list->Count();t++)
        {
          //Ищем последнюю запись чтобы узнать последнее имя.
          nw=FindFLISTtByN(t);
          num=strtoul(nw->filename,0,16);
          if(num>y) 
            y=num;
        }
      }
      for(int k=0;k<list->Count();k++)//Ищем имя файла
      {       
        fl=FindFLISTtByN(k);       
        if(y==strtoul(fl->filename,0,16)) break;
      }

      DrawText(NULL,fl); 
    }
   
   if((k==DOWN_BUTTON && readonly==0) && item!=0)
    {
      GeneralFunc_flag1(draw_id,1);
      int y;
      int num;
      int first,last=0;
      y=strtoul(cmp,0,16);
      for(int t=0;t<list->Count();t++)
        {
          //Ищем последнюю запись чтобы узнать последнее имя.
          nw=FindFLISTtByN(t);
          num=strtoul(nw->filename,0,16);
          if(num>last) 
            last=num;
        }
      nw=FindFLISTtByN(1);//имя файла первой записи
      first=strtoul(nw->filename,0,16);
      if(y<last) y+=0x01;
      else y=first;
      
      for(int k=0;k<list->Count();k++)//Ищем имя файла
      {       
       fl=FindFLISTtByN(k);       
       if(y==strtoul(fl->filename,0,16)) break;
      }

      DrawText(NULL,fl); 
    }
   
   if(k==GREEN_BUTTON)
   {
      int  y=strtoul(cmp,0,16);
      for(int k=0;k<list->Count();k++)//Ищем имя файла
      {       
       fl=FindFLISTtByN(k);       
       if(y==strtoul(fl->filename,0,16)) break;
      }
      SaveMark(fl->filename,fl->text);
   }
 }
return(0);
}

const SOFTKEY_DESC sk[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)_sk_r},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB skt={ sk,0 };

void empty_locret(void){}

void empty_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Сохранить"};
  
  if(cmd==0xA)
  {
    DisableIDLETMR();   // Отключаем таймер выхода по таймауту
  }
  
  if(cmd==0x03)
  {
    void *data=FindGUIbyId(id_ed,NULL);
    RecountMenuBCFG(data);
  }
  
  if(cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);
  }
};


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
 
//------------------------------------//

static void SaveMark(char *name,char *text)//Сохраняем в каталог
{
  unsigned int err;
  int f;
  char *p=(char*)malloc(strlen(dir)+64);
  sprintf(p,"%s\\%s",dir,name);
  SetFileAttrib(p,0x0000,&err);
  if((f=fopen(p,A_Create+A_ReadWrite+A_BIN+A_Truncate,P_WRITE,&err))!=-1)
   {
    fwrite(f,text,strlen(text)+1,&err);
    ShowMSG(1,(int)LG_FILESAVE);
   }
  else ShowMSG(1,(int)LG_NOTSAVE);
  SetFileAttrib(p,FA_READONLY,&err);
  fclose(f,&err);
}

//------------------------------------//

int DrawText(int i,MNU *fn)//собственно эдитконтроль
{
  ClearT9Key();
  EDITC_OPTIONS ec_options;
  PrepareEditControl(&ec);

  void *ma=malloc_adr();
  void *eq=AllocEQueue(ma, mfree_adr());
  WSHDR *ws=AllocWS(strlen(fn->text));
  MNU *fl;
  for(item=0 ; item<list->Count() ; item++)//Ищем имя файла
    {
      fl=FindFLISTtByN(item);
      if( (strtoul(fn->filename,0,16)) == (strtoul(fl->filename,0,16)) ) break;
    }
//  fl=FindFLISTtByN(i);
 // ShowMSG(1,(int)fl->filename);
  cmp=(char*)malloc(strlen(fl->filename));
  strcpy(cmp,fl->filename);
  
  if(fl->is_first==1) sprintf(hdr_txt,"%s","Новая запись");
  else
    if(!readonly) sprintf(hdr_txt,"%d. %s",item,fl->dat);//Если открываем для просмотра в режим реадонли,то выводим в заголовке дату
  else sprintf(hdr_txt,"%d. %s", item,"Редактирование");//Иначе просто слово Заметки:)

  if(fl->is_first!=1) ascii2ws(ws,fl->text);//Если открываем запись,то выводим текст
  else readonly=1;//Если создаем,то сразу открываем редактирование

  ConstructEditControl(&ec,readonly?ECT_NORMAL_TEXT:ECT_READ_ONLY,0x40,ws,16384);//16 кило думаю хватит :)
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,1);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);

  patch_header(&disk_prop_hdr);
  patch_input(&edit_desc);      

  FreeWS(ws);
  return draw_id=CreateInputTextDialog(&edit_desc,&disk_prop_hdr,eq,1,0);
}

//-------------------------------------------------------------------------------------//

void maincsm_oncreate(CSM_RAM *data)
{
  list = new CList;
  mlist = new MenuList;
  LoadMark();
  mlist->Show();
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
  delete mlist;
  delete list;  
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


void UpdateCSMname(void)
{ 
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Marks");
}

int maincsm_id;
int main(char *exename, char *fname)
{ 
  strcpy(dir,exename);
  for(int i=strlen(dir);dir[i]!='\\';i--) dir[i]=0;
  strcat(dir,"mark");

//  LoadMark();

  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  LockSched();
  maincsm_id=CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched();
 return 0;
}



//-----------------------Edit chat---------------------------//


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
    total_char++;
    wsAppendChar(ws,win2unicode(c));
  }
}

void koi2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    total_char++;
    wsAppendChar(ws,char8to16(c,2));
  }
}

void win1251_2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    total_char++;
    wsAppendChar(ws,char16to8(c));
  }
}


