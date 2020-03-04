#include "main.h"
char NO_ICON[256]="0:\\shell\\icons\\no_icon.png";;
void nn()
{
//descmenu[1].description="test";
//descmenu[1].icon="0:\\shell\\icons\\balans.png";
sprintf(descmenu[1].name,per_s,"TESSTTT");
//descmenu[1].shortcut="ELSE_DEVELP_MNU";
sprintf(descmenu[1].file,per_s,"ORGZ_CALENDERMV");
}
int S_ICONS[16]={
(int)descmenu[1].icon,
(int)descmenu[2].icon,
(int)descmenu[3].icon,
(int)descmenu[4].icon,
(int)descmenu[5].icon,
(int)descmenu[6].icon,
(int)descmenu[7].icon,
(int)descmenu[8].icon,
(int)descmenu[9].icon,
(int)descmenu[10].icon,
(int)descmenu[11].icon,
(int)descmenu[12].icon,
(int)descmenu[13].icon,
(int)descmenu[14].icon,
(int)descmenu[15].icon,
(int)NO_ICON
};


int number_book=0;//количечтво закладок

const int name_cl=7;//цвет имен закладок
const int description_cl=4;//цвет описаний закладок

volatile int contactlist_menu_id;
unsigned int *ErrorNumber;

static const char black1[]={0x00,0x00,0x00,0x30};

void *about()
{ 
  char s[128];
  snprintf(s,255,"AltIdleCsm\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
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

const char *bmname(int bm)//возвращает имена для закладок
{
  switch(bm)
  {
  case 0: return descmenu[bm+1].name;
  case 1: return descmenu[bm+1].name;
  case 2: return descmenu[bm+1].name;
  case 3: return descmenu[bm+1].name;
  case 4: return descmenu[bm+1].name;
  case 5: return descmenu[bm+1].name;
  case 6: return descmenu[bm+1].name;
  case 7: return descmenu[bm+1].name;
  case 8: return descmenu[bm+1].name;
  case 9: return descmenu[bm+1].name;
  case 10: return descmenu[bm+1].name;
  case 11: return descmenu[bm+1].name;
  case 12: return descmenu[bm+1].name;
  case 13: return descmenu[bm+1].name;
  case 14: return descmenu[bm+1].name;
  }
  return(0);
}
const char *description(int bm)//Описание для закладок
{
  switch(bm)
  {
  case 0: return descmenu[1].description;
  case 1: return descmenu[2].description;
  case 2: return descmenu[3].description;
  case 3: return descmenu[4].description;
  case 4: return descmenu[5].description;
  case 5: return descmenu[6].description;
  case 6: return descmenu[7].description;
  case 7: return descmenu[8].description;
  case 8: return descmenu[9].description;
  case 9: return descmenu[10].description;
  case 10: return descmenu[11].description;
  case 11: return descmenu[12].description;
  case 12: return descmenu[13].description;
  case 13: return descmenu[14].description;
  case 14: return descmenu[15].description;
  }
  return(0);
}
const char *bmfile(int bm)//вызов файлов
{
  switch(bm)
  {
  case 0: if(strlen(descmenu[1].file)!=15)    return descmenu[1].file;    else return shortcut((char*)descmenu[bm+1].file);
  case 1: if(strlen(descmenu[2].file)!=15)    return descmenu[2].file;    else return shortcut((char*)descmenu[2].file);
  case 2: if(strlen(descmenu[3].file)!=15)    return descmenu[3].file;    else return shortcut((char*)descmenu[3].file);
  case 3: if(strlen(descmenu[bm+1].file)!=15) return descmenu[bm+1].file; else return shortcut((char*)descmenu[4].file);
  case 4: if(strlen(descmenu[bm+1].file)!=15) return descmenu[bm+1].file; else return shortcut((char*)descmenu[5].file);
  case 5: if(strlen(descmenu[bm+1].file)!=15) return descmenu[bm+1].file; else return shortcut((char*)descmenu[6].file);
  case 6: if(strlen(descmenu[bm+1].file)!=15) return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 7: if(strlen(descmenu[bm+1].file)!=15) return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 8: if(strlen(descmenu[bm+1].file)!=15) return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 9: if(strlen(descmenu[bm+1].file)!=15) return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 10: if(strlen(descmenu[bm+1].file)!=15)return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 11: if(strlen(descmenu[bm+1].file)!=15)return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 12: if(strlen(descmenu[bm+1].file)!=15)return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 13: if(strlen(descmenu[bm+1].file)!=15)return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  case 14: if(strlen(descmenu[bm+1].file)!=15)return descmenu[bm+1].file; else return shortcut((char*)descmenu[bm+1].file);
  }
  return(0);
}
//==========================
//      Создаем список
//==========================
void bm_menu_iconhnd(void *data, int curitem, void *unk);
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

void bm_menu_ghook(void *data, int cmd){}
int bm_menu_onkey2(void *data, GUI_MSG *msg);
const HEADER_DESC bm_menuhdr2={0,0,131,21,NULL,(int)"Bookmarks",LGP_NULL};

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


int RunBM2(int bm)
{
  const char *s=bmfile(bm);
  if (s)
  {
    if (strlen(s))
    {
      WSHDR *ws;
      ws=AllocWS(150);
      str_2ws(ws,s,128);
      ExecuteFile(ws,0,0);
      FreeWS(ws);
      return(1);
    }
  }
  return(0);
}

//==============================================
//  Вывод на экран названия закладок и иконоки
//==============================================
void bm_menu_iconhnd(void *data, int curitem, void *unk)
{ const char *s,*ss;
  WSHDR *ws;
  int len1=strlen(descmenu[1].icon);
  int len2=strlen(descmenu[2].icon);
  int len3=strlen(descmenu[3].icon);
  int len4=strlen(descmenu[4].icon);  
  int len5=strlen(descmenu[5].icon);
  int len6=strlen(descmenu[6].icon);
  int len7=strlen(descmenu[7].icon);
  int len8=strlen(descmenu[8].icon);
  int len9=strlen(descmenu[9].icon);
  int len10=strlen(descmenu[10].icon);
  int len11=strlen(descmenu[11].icon);
  int len12=strlen(descmenu[12].icon);
  int len13=strlen(descmenu[13].icon);
  int len14=strlen(descmenu[14].icon);
  int len15=strlen(descmenu[15].icon);

  WSHDR *ws1;
  void *item=AllocMLMenuItem(data);
  s=bmname(curitem); 
  if(curitem!=8) ss=description(curitem);
  ws1=AllocWS(128);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+128);
      wsprintf(ws,per_t,s);
      wsInsertChar(ws,name_cl,1);//Цвет надписей
      wsInsertChar(ws,0xE008,1);//
      
      wsprintf(ws1,per_t,ss);//Цвет описаний
      wsInsertChar(ws1,description_cl,1);//0x0002
      wsInsertChar(ws1,0xE008,0);
    }
    else
    { 
      ws=AllocMenuWS(data,10);
      wsprintf(ws,per_t,"Ниче нет...");
      wsInsertChar(ws,0xE008,1);//Цвет надписей
      wsInsertChar(ws,0xE008,1);//
      wsprintf(ws1,per_t,"Надо задать...");//Цвет описаний
      wsInsertChar(ws1,description_cl,1);//0x0002
      wsInsertChar(ws1,0xE008,0);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей

  }
 
  switch(curitem)//Вызов иконок
  { 
  case 0:
    if(len1!=0)//если не путь не равен 0 то рисуем иконку
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);//если пути до иконки нет то рисуем иконку NO_ICON
    break;
  case 1:
    if(len2 != 0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 2:
    if(len3!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 3:
    if(len4!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 4:
    if(len5!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 5:
    if(len6!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 6:
    if(len7!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;    
  case 7:
    if(len8!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 8:
    if(len9!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 9:
    if(len10!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 10:
    if(len11!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 11:
    if(len12!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 12:
    if(len13!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 13:
    if(len14!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
  case 14:
    if(len15!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 15);
    break;
   
  }
 SetMLMenuItemText(data, item, ws, ws1, curitem);
 //  SetMenuItemText(data,item,ws,curitem);
}

//==========================
//   Вызов самих менюшек
//==========================
int num_book()
{ int len1=strlen(descmenu[1].file);
  int len2=strlen(descmenu[2].file);
  int len3=strlen(descmenu[3].file);
  int len4=strlen(descmenu[4].file);  
  int len5=strlen(descmenu[5].file);
  int len6=strlen(descmenu[6].file);
  int len7=strlen(descmenu[7].file);
  int len8=strlen(descmenu[8].file);
  int len9=strlen(descmenu[9].file);
  int len10=strlen(descmenu[10].file);
   
  int len11=strlen(descmenu[1].shortcut);
  int len22=strlen(descmenu[2].shortcut);
  int len33=strlen(descmenu[3].shortcut);
  int len44=strlen(descmenu[4].shortcut);  
  int len55=strlen(descmenu[5].shortcut);
  int len66=strlen(descmenu[6].shortcut);
  int len77=strlen(descmenu[7].shortcut);
  int len88=strlen(descmenu[8].shortcut);
  int len99=strlen(descmenu[9].shortcut);
  int len12=strlen(descmenu[10].shortcut);
 
  if(len1!=0 || len11!=0) number_book=1;
  if(len2!=0 || len22!=0) number_book=2;
  if(len3!=0 || len33!=0) number_book=3;
  if(len4!=0 || len44!=0) number_book=4;
  if(len5!=0 || len55!=0) number_book=5;
  if(len6!=0 || len66!=0) number_book=6;
  if(len7!=0 || len77!=0) number_book=7;
  if(len8!=0 || len88!=0) number_book=8;
  if(len9!=0 || len99!=0) number_book=9;
  if(len10!=0 ||len12!=0) number_book=10;
  if(strlen(descmenu[11].file) || strlen(descmenu[11].shortcut)) number_book=11;
  if(strlen(descmenu[12].file) || strlen(descmenu[12].shortcut)) number_book=12;
  if(strlen(descmenu[13].file) || strlen(descmenu[13].shortcut)) number_book=13;
  if(strlen(descmenu[14].file) || strlen(descmenu[14].shortcut)) number_book=14;
  if(strlen(descmenu[15].file) || strlen(descmenu[15].shortcut)) number_book=15;
return number_book;
}

void ShowBMmenu1(void)
{ num_book();
  patch_header2(&bm_menuhdr2);
  CreateMultiLinesMenu(0,0,&bmmenu,&bm_menuhdr2,0,number_book);//number_book - кол-во закладок
}


//====================================
//         Обработка клавиш
//====================================

int bm_menu_onkey2(void *data, GUI_MSG *msg)//кейхук для закладок
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
 if(msg->keys==0x14)
   {about();
    return (-1);
   }
  if (k=='0') RunBM2(k-'1');
  if ((k>='1')&&(k<='9'))
   { if (RunBM2(k-'1')) return 1;
     return(-1);
   }
  if (msg->keys==0x3D)
  {
    if (RunBM2(i))
    {
      return(1);
    }
    return(-1);
  }
  return(0);
}

