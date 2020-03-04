#include "..\inc\swilib.h"
#include "language.h"
#include "main.h"


extern const char per_s[];
extern const char per_d[];
extern const char per_t[];
extern void about();
//extern int DrawText(int i);
int DrawText(int i,MNU *fl);
extern void DelItem(char *filename);
extern char *delname;
extern MNU *changefile;
extern int readonly;
extern int item;
extern int id_ed;
//extern const char *successed_config_filename;
#pragma inline
void patch_header2(HEADER_DESC* head)
{
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}
extern void patch_input( INPUTDIA_DESC* inp);

//--------------------------------------------------//

int start(const char *f)
{
  WSHDR *ws;
  ws=AllocWS(256);
  str_2ws(ws,f,256);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  return(1);
}

//------------------------MENU------------------------//

int menu_id;
char *name[]={
/*
  "Изменить",
  "Создать",
  "Настройки",
  "Об эльфе",
  "Выход"*/
  LG_CHANGE,
  LG_NEW_FILE,
  LG_DELETE,
  LG_SET,
  LG_ABOUT,
  LG_EXIT
};

//---------------------Создаем список меню-----------------------//

void menu_iconhnd(void *data, int curitem, void *unk);
const int menu_softkeys[]={0,1,2};

const SOFTKEY_DESC menusk2[]=
{
  {0x0001,0x0000,(int)LG_SELECT},
  {0x0004,0x0000,(int)LG_CLOSE},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menuskt2=
{
  menusk2,0
};

void menu_ghook(void *data, int cmd){};
int menu_onkey2(void *data, GUI_MSG *msg);
HEADER_DESC menuhdr2={0,0,131,21,NULL,(int)LG_MENU,LGP_NULL};
const MENU_DESC menu=
{
  8,menu_onkey2,menu_ghook,NULL,
  menu_softkeys,
  &menuskt2,
  0x10,//0x11,
  menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  7   //n
};
void menu_iconhnd(void *data, int curitem, void *unk)
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

//extern const char *successed_config_filename;
extern volatile MNU *mnutop;
int menu_onkey2(void *data, GUI_MSG *msg)
{
 int i;
 i=GetCurMenuItem(data);
 int k=msg->gbsmsg->submess;
 MNU *m=(MNU *)&mnutop;
 if(k=='*')
   {
     about();
   }
  if (k==ENTER_BUTTON || k==LEFT_SOFT)
  {
    switch(i)
      {
        case 0: readonly=0; DrawText(NULL,changefile); break;//Изменить
        case 1: DrawText(NULL,m); break;//Создать
        case 2: DelItem(delname);    break;
  //      case 3: start(successed_config_filename); break;//Setting
        case 3: about(); return 0;
        case 4:  //exit
            GeneralFunc_flag1(menu_id,1);
            GeneralFunc_flag1(id_ed,1);
            break;
      }
    return(1);
  }
 if(k==RIGHT_SOFT || k==RED_BUTTON)
 {
   GeneralFunc_flag1(menu_id,1);
 }
  return(0);
}

void ShowMainMenu(void)
{ 
 patch_header2(&menuhdr2);
 menu_id=CreateMenu(0,0,&menu,&menuhdr2,0,6,0,0);
}


