#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"

extern void InitConfig();
extern const char list_txt[128];
extern const int scan_disc;

#define CBOX_CHECKED 0xE10B
#define CBOX_UNCHECKED 0xE10C
static int icon_array[2];

const char per_t[]="%t";
const char per_s[]="%s";
const char per_d[]="%d";

const int minus11=-11;
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;

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

//======================
const char ICON1[128]="0:\\shell\\icons\\no_icon.png";//пути к иконкам
int S_ICONS[1]={
(int)ICON1
};
//======================
int java_num=0;//Количество папок в папке java
char empty_string[]="";
int TotalFolder=0;
int TotalEqual=0;

void FreeList();
void CreateList(void);
void DelFolder();
//========================================
//        Чтение файла
//========================================
/*****************
файл вида:       
name [number]
******************/ 

typedef struct
{
  void *next;
  char num[32];
  char name[32];
  char java[32];
  char tmp[32];
  char tmp2[32];
  int del;
  int TotalFolder;
}LISTT;

LISTT list[1000];


void ShowInt(int i)
{
 char *s=malloc(64);
 sprintf(s,per_d,i); 
 ShowMSG(1,(int)s);
}
/*
void process(char *str, int number)//Читаем конфиг и сохраняем пути
{ 
  int j=0; 
  int k=0;
  while (str[j]!='|') { list[number].num[k++]=str[j]; j++; }    
  j++;
  k=0;
  while (str[j]!='|') { list[number].name[k++]=str[j]; j++; }    
  

}*/
void process(char *str, int number)//Читаем конфиг и сохраняем пути
{ 
  int j=0; 
  int k=0;
  while (str[j]!='[') { list[number].name[k++]=str[j]; j++; }    
  j++;
  k=0;
  while (str[j]!=']') { list[number].num[k++]=str[j]; j++; }    
}

void LoadList()
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  int size; 
  int i,j=0; 
  char *str; 
  plhandle = fopen(list_txt, A_ReadOnly + A_TXT, P_READ, & err ); 
 if ( plhandle==-1 ) 
  { 
    ShowMSG(1,(int)"Can't open list.txt");
    fclose( plhandle, & err ); 
    return; 
  }
  mem=malloc(16384); 
  str=malloc(256); 
 if ((mem!=0)&&(str!=0)) 
  { 
    size=fread(plhandle,mem,16384,&err); 
    i=0; 
    while (i<size) //пока не конец файлa
    { 
      strcpy(str,empty_string); 
      j=0; 
      while (((*(mem+i)!='\r'))&&(i<size)) {*(str+j)=*(mem+i); j++;i++;}    //читаем строку из файла 
      //обрабатываем енту строчку 
      process(str,TotalFolder);//Сохраняем имена и пути
      i+=2; 
      TotalFolder++;
    }
  }
mfree(mem); 
mfree(str); 

fclose( plhandle, & err ); 

//ShowMSG(1,(int)"Read list.txt ok!");
/* char *s=malloc(64);
 sprintf(s,per_d,TotalFolder); 
 ShowMSG(1,(int)s);*/
}

//======================================================
//          Search folder 0:\\system\\java
//======================================================
int FindDir(char *dir)
{ 

  DIR_ENTRY de;
  unsigned int err;
  WSHDR *ws=AllocWS(64);
  char path[256];
  strcpy(path,dir); 
  char *ptr=path+strlen(path)+1;
  strcat(path,"\\*.*");
  if (FindFirstFile(&de,path,&err))
  { 
    do{
        strcpy(ptr,de.file_name);  
        if(isdir(path,&err))
        {
          strcpy(list[java_num].java,de.file_name);  
          java_num++;
        }
      
      }while(FindNextFile(&de,&err));
  }FindClose(&de,&err);

if(java_num==0)ShowMSG(1,(int)"No folder!");  
/* char *s=malloc(64);
 sprintf(s,per_s,list[1].java); 
 ShowMSG(1,(int)s);*/
//  FreeWS(ws);
 // mfree(ptr);
return 0;
}

int ChangeDelMode(int i)
{
  list[i].del=!list[i].del;
  RefreshGUI();
  return list[i].del;
}

void cmpp()
{
 int k=0;
 for(int i=0;i<java_num;i++)
  {
    for(int j=0;j<TotalFolder;j++)
     {
      int cmp=strcmp(list[j].num,list[i].java);
      if(cmp==0)
      {
        strcpy(list[k].tmp,list[j].name);
        strcpy(list[k].tmp2,list[j].num);
        k++;
        TotalEqual++;
      }
    }
  }
CreateList();
}
void DelFolder()
{ 

DIR_ENTRY de;
unsigned int err;
WSHDR *ws=AllocWS(64);
int ii=0;
for(int kk=0;kk<TotalFolder;kk++)
 {
  if(list[kk].del==1)
  {
  char *s=list[kk].tmp2;
  char path[256];
  if(scan_disc==1) sprintf(path,"2:\\System\\java\\");
  else sprintf(path,"0:\\System\\java\\");
  char path2[256];
  strcat(path,s);
  strcpy(path2,path);
  char *ptr=path+strlen(path)+1;
  strcat(path,"\\*.*");
  if (FindFirstFile(&de,path,&err))
  { 
    do{
        strcpy(ptr,de.file_name);  
        unlink(path,&err);
        ii++;
      }while(FindNextFile(&de,&err));
  }FindClose(&de,&err);
 rmdir(path2,&err);
// mfree(s);
// mfree(ptr);
  }
}
if(ii!=0) ShowMSG(1,(int)"Delete");
else ShowMSG(1,(int)"NOT DELETE!!!");
FreeWS(ws);
 
}
//======================================================

int name_cl=7;//цвет имен закладок

//volatile int contactlist_menu_id;
unsigned int *ErrorNumber;


void *about()
{ 
  char s[128];
  snprintf(s,255,"JavaFolder v0.12b fix\n(c)kluchnik\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
  mfree(s);
  return 0;
}

#pragma inline
void patch_header2(const HEADER_DESC* headc)
{
  HEADER_DESC *head=(HEADER_DESC *)headc;
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}

//==========================
//      Создаем список
//==========================
void bm_menu_iconhnd(void *data, int curitem, void *unk);
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)"Выделить"},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

void bm_menu_ghook(void *data, int cmd)

{
 if(cmd==0x03)     // onDestroy
  {
    FreeWS(ews);
    ws=NULL
   }
}
int bm_menu_onkey2(void *data, GUI_MSG *msg);
HEADER_DESC bm_menuhdr2={0,0,131,21,NULL,0,LGP_NULL};

const MENU_DESC bmmenu=
{
  8,bm_menu_onkey2,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x11,//0x11,
  bm_menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};


//==============================================
//  Вывод на экран названия закладок и иконоки
//==============================================
void bm_menu_iconhnd(void *data, int curitem, void *unk)
{
  
  char *s;
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  s=malloc(1024);
  sprintf(s,per_s,list[curitem].tmp);
  
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,1024);
      wsprintf(ws,per_s,s);
 //     wsInsertChar(ws,name_cl,1);//Цвет надписей
   //   wsInsertChar(ws,0xE008,1);//
    }/*
    else
    { 
      ws=AllocMenuWS(data,10);
      wsprintf(ws,per_t,"Ниче нет...");
      wsInsertChar(ws,0xE008,1);//Цвет надписей
      wsInsertChar(ws,0xE008,1);//

    }*/
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей

  }
  icon_array[0]=GetPicNByUnicodeSymbol(CBOX_CHECKED);
  icon_array[1]=GetPicNByUnicodeSymbol(CBOX_UNCHECKED);
  SetMenuItemIconArray(data,item,icon_array+(list[curitem].del?0:1));
  SetMenuItemText(data, item, ws, curitem);
 
  mfree(s);
  FreeWS(ws);
//  mfree(item);
}

//==========================
//   Вызов самих менюшек
//==========================

const char per_sd[]="%s %d";
void CreateList(void)
{ 
  
  char *ss=malloc(64);
  sprintf(ss,per_sd,"JavaFolder:",TotalEqual);//num_str
  bm_menuhdr2.lgp_id = (int)ss;
  patch_header2(&bm_menuhdr2);
//  CreateMultiLinesMenu(0,0,&bmmenu,&bm_menuhdr2,0,TotalFolder);//number_book - кол-во закладок
  CreateMenu(0,0,&bmmenu,&bm_menuhdr2,0,TotalEqual,0,0);
}


//====================================
//         Обработка клавиш
//====================================

int bm_menu_onkey2(void *data, GUI_MSG *msg)//кейхук для закладок
{
  int i;
  i=GetCurMenuItem(data);
//  int k=msg->gbsmsg->submess;
 if(msg->keys==0x14)
   {
     about();
     return (-1);
   }


 if(msg->keys==0x15)
  {
   DelFolder();
   RefreshGUI();
  }
 if(msg->keys==0x04 || (msg->keys==0x3D))
 {
   ChangeDelMode(i);
   return -1;
 }

 return(0);
}
//==========================
//     Реконфигурация
//==========================
// ----------------------------------------------------------------------------
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}
#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  void *icsm;
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
   // extern const char *successed_config_filename;
   // if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    //{
      ShowMSG(1,(int)"JavaFolder config updated!");
      InitConfig();
    //}
  }
 
  icsm=FindCSMbyID(CSM_root()->idle_id);
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm))) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {
      }
    }
  }
  return(1);

}
static void maincsm_oncreate(CSM_RAM *data)
{
 

}

void FreeList()
{
  LISTT *lt;  
  LISTT *n;  
  /*for(int i=0;i<TotalFolder;i++)
   {
      mfree(list[i].name);
      mfree(list[i].num);
      mfree(list[i].java);
      mfree(list[i].tmp);
      mfree(list[i].tmp2);
      
    }*/
  //while(lt)
  for(int i=0;i<TotalFolder;i++)  
    {
      n=lt->next;
      mfree(lt);
      lt=n;
    }
}
void maincsm_onclose(CSM_RAM *data)
{ 
  SUBPROC((void *)FreeList);
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;

}
static unsigned short maincsm_name_body[140];


static const struct
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
  sizeof(CSM_RAM),
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


// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
  InitConfig();
  LockSched();
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  UnlockSched();
  LoadList();
 if(scan_disc==1)FindDir("2:\\System\\java");
 else FindDir("0:\\System\\java");
  cmpp();
  return (0);
}
