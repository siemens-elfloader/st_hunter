#include "..\inc\swilib.h"
//Исходники кривые бля)))))))нужно оптимизировать...
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

int my_csm_id=0;

extern const char description1[32];// Описание закладок
extern const char description2[32];
extern const char description3[32];
extern const char description4[32];
extern const char description5[32];
extern const char description6[32];
extern const char description7[32];
extern const char description8[32];
extern const char description9[32];
extern const char description10[32];
extern const char description11[32];/*
extern const char description12[32];
extern const char description13[32];
extern const char description14[32];
extern const char description15[32];
*/
/*
extern const char NAME1[32];//Подписи для инфы
extern const char NAME2[32];
extern const char NAME3[32];
extern const char NAME4[32];
extern const char NAME5[32];
extern const char NAME6[32];
extern const char NAME7[32];*/
//extern const char NAME8[32];
//extern const char NAME9[32];

extern const char ICON1[128];//пути к иконкам
extern const char ICON2[128];
extern const char ICON3[128];
extern const char ICON4[128];
extern const char ICON5[128];
extern const char ICON6[128];
extern const char ICON7[128];
extern const char ICON8[128];
extern const char ICON9[128];
extern const char ICON10[128];
/*extern const char ICON11[128];
extern const char ICON12[128];
extern const char ICON13[128];
extern const char ICON14[128];
extern const char ICON15[128];*/
extern const char NO_ICON[128];

int S_ICONS[11]={
(int)ICON1,
(int)ICON2,
(int)ICON3,
(int)ICON4,
(int)ICON5,
(int)ICON6,
(int)ICON7,
(int)ICON8,
(int)ICON9,
(int)ICON10,/*
(int)ICON11,
(int)ICON12,
(int)ICON13,
(int)ICON14,
(int)ICON15,*/
(int)NO_ICON
};

extern const char BM9FILE[128];//пути к файлам
extern const char BM1FILE[128];
extern const char BM2FILE[128];
extern const char BM3FILE[128];
extern const char BM4FILE[128];
extern const char BM5FILE[128];
extern const char BM6FILE[128];
extern const char BM7FILE[128];
extern const char BM8FILE[128];
extern const char BM9FILE[128];
extern const char BM10FILE[128];/*
extern const char BM11FILE[128];
extern const char BM12FILE[128];
extern const char BM13FILE[128];
extern const char BM14FILE[128];
extern const char BM15FILE[128];
*/
extern const char BMNAME1[32];//Название закладок
extern const char BMNAME2[32];
extern const char BMNAME3[32];
extern const char BMNAME4[32];
extern const char BMNAME5[32];
extern const char BMNAME6[32];
extern const char BMNAME7[32];
extern const char BMNAME8[32];
extern const char BMNAME9[32];
extern const char BMNAME10[32];/*
extern const char BMNAME11[32];
extern const char BMNAME12[32];
extern const char BMNAME13[32];
extern const char BMNAME14[32];
extern const char BMNAME15[32];
*/
extern const char shortcuts1[32];
extern const char shortcuts2[32];
extern const char shortcuts3[32];
extern const char shortcuts4[32];
extern const char shortcuts5[32];
extern const char shortcuts6[32];
extern const char shortcuts7[32];
extern const char shortcuts8[32];
extern const char shortcuts9[32];
extern const char shortcuts10[32];

extern const int FILE1;
extern const int FILE2;
extern const int FILE3;
extern const int FILE4;
extern const int FILE5;
extern const int FILE6;
extern const int FILE7;
extern const int FILE8;
extern const int FILE9;
extern const int FILE10;

extern const int LONG_P;
extern const int OPEN_FIRST;
int number_book=0;//количечтво закладок

extern const int name_cl;//цвет имен закладок
extern const int description_cl;//цвет описаний закладок

extern const int FONT;
extern const unsigned int X1;
extern const unsigned int X2;
extern const unsigned int Y1;
extern const unsigned int Y2;
extern void InitConfig();

volatile int contactlist_menu_id;
unsigned int *ErrorNumber;

const char percent_t[]="%t";
const char percent_2t[]="%t%t";


int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);
extern void kill_data(void *p, void (*func_p)(void *));

void ElfKiller(void)
{
  extern void *ELF_BEGIN;

  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

 
extern const char red[];
//static const char white[]={0xFF,0xFF,0xFF,0x20};
extern const char black[];
static const char black1[]={0x00,0x00,0x00,0x30};
//static const char proz[]={0xFF,0xFF,0xFF,0x00};
//static const char green[]={0x00,0xFF,0x00,0x20};


void show()
{ int font_size=GetFontYSIZE(FONT);
 // int y=(font_size*7)+7;
  DrawRoundedFrame(X1,Y1,X2,Y2,5,5,100,black,black1);
  WSHDR *ws=AllocWS(256);
  RAMNET *rn=RamNet();
  char* s;
  TDate date;
  TTime time; 
  int f0 = GetFreeFlexSpace(0, ErrorNumber)/1024;
  GetDateTime(&date,&time);  
  int dayy=0;
  char *days[8]={" ","Вс","Пн","Вт","Ср","Чт","Пт","Сб"},
       *mounth[13]={" ","Янв","Фев","Март","Апр","Май","Июн","Июл","Авг","Сент","Окт","Ноя","Дек"};
  dayy=date.day;
  while(dayy>7)
    dayy=dayy-7;

  char *s1=date.day<10?"0":"";
  s=(time.min<10)?":0":":";

  wsprintf(ws,"%d%s%d\n%s, %s%d %s\n%t %d %%\n%t %d db\nFreeRam %d kb\nFree 0: %d kb\nCPU Load %d %%",
           time.hour,s,time.min,days[dayy],s1,date.day,mounth[date.month],"Заряд АКБ",*RamCap(),"Сеть",rn->power,
           GetFreeRamAvail()/1000,f0,GetCPULoad());
  DrawString(ws,X1+2,Y1+2,X2,Y2,FONT,50,black,red); 
  mfree(ws);

}
void *about()
{ char s[256];
snprintf(s,255,"Info Everywhere v0.2\n(c)kluchnik\n(r)за идеи и тесты\nEverGrey(NSG),IntrO(Sgold)\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
  return 0;
}

#pragma inline
void patch_header(const HEADER_DESC* headc)
{
  HEADER_DESC *head=(HEADER_DESC *)headc;
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}

char *shortcut(char cuts[32])
{int (*p) (void);
 p=(int(*)(void))GetFunctionPointer(cuts);
 if(p!=NULL)
  (*p)();
 else
   ShowMSG(1,(int)"NULL pointer function!");
return 0;
}

const char *bmname(int bm)//возвращает имена для закладок
{
  switch(bm)
  {
  case 0: return BMNAME1;
  case 1: return BMNAME2;
  case 2: return BMNAME3;
  case 3: return BMNAME4;
  case 4: return BMNAME5;
  case 5: return BMNAME6;
  case 6: return BMNAME7;
  case 7: return BMNAME8;
  case 8: return BMNAME9;
  case 9: return BMNAME10;/*
  case 10: return BMNAME11;
  case 11: return BMNAME12;
  case 12: return BMNAME13;
  case 13: return BMNAME14;
  case 14: return BMNAME15;*/
  }
  return(0);
}
const char *description(int bm)//Описание для закладок
{
  switch(bm)
  {
  case 0: return description1;
  case 1: return description2;
  case 2: return description3;
  case 3: return description4;
  case 4: return description5;
  case 5: return description6;
  case 6: return description7;
  case 7: return description8;
  case 8: return description9;
  case 9: return description10;/*
  case 10: return description11;
  case 11: return description12;
  case 12: return description13;
  case 13: return description14;
  case 14: return description15;*/
  }
  return(0);
}
const char *bmfile(int bm)//вызов файлов
{
  switch(bm)
  {
  case 0: if(FILE1) return BM1FILE;  else return shortcut((char*)shortcuts1);
  case 1: if(FILE2) return BM2FILE;  else return shortcut((char*)shortcuts2);
  case 2: if(FILE3) return BM3FILE;  else return shortcut((char*)shortcuts3);
  case 3: if(FILE4) return BM4FILE;  else return shortcut((char*)shortcuts4);
  case 4: if(FILE5) return BM5FILE;  else return shortcut((char*)shortcuts5);
  case 5: if(FILE6) return BM6FILE;  else return shortcut((char*)shortcuts6);
  case 6: if(FILE7) return BM7FILE;  else return shortcut((char*)shortcuts7);
  case 7: if(FILE8) return BM8FILE;  else return shortcut((char*)shortcuts8);
  case 8: if(FILE9) return BM9FILE;  else return shortcut((char*)shortcuts9);
  case 9: if(FILE10) return BM10FILE; else return shortcut((char*)shortcuts10);
  /*
  case 10: return BM11FILE;
  case 11: return BM12FILE;
  case 12: return BM13FILE;
  case 13: return BM14FILE;
  case 14: return BM15FILE;*/
  }
  return(0);
}
//==========================
//      Создаем список
//==========================
void bm_menu_iconhnd(void *data, int curitem, void *unk);
const HEADER_DESC bm_menuhdr={0,0,131,21,NULL,(int)"Information",LGP_NULL};
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
  int len1=strlen(ICON1);
  int len2=strlen(ICON2);
  int len3=strlen(ICON3);
  int len4=strlen(ICON4);  
  int len5=strlen(ICON5);
  int len6=strlen(ICON6);
  int len7=strlen(ICON7);
  int len8=strlen(ICON8);
  int len9=strlen(ICON9);
  int len10=strlen(ICON10);/*
  int len11=strlen(ICON11);
  int len12=strlen(ICON12);
  int len13=strlen(ICON13);
  int len14=strlen(ICON14);
  int len15=strlen(ICON15);*/

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
      wsprintf(ws,percent_t,s);
      wsInsertChar(ws,name_cl,1);//Цвет надписей
      wsInsertChar(ws,0xE008,1);//
  
      wsprintf(ws1,percent_t,ss);//Цвет описаний
      wsInsertChar(ws1,description_cl,1);//0x0002
      wsInsertChar(ws1,0xE008,1);
    }
    else
    {
      ws=AllocMenuWS(data,10);
      wsprintf(ws,percent_t,"Ниче нет...");
      wsprintf(ws1,percent_t,"Надо задать...");//Цвет описаний
      wsInsertChar(ws1,description_cl,1);//0x0002
      wsInsertChar(ws1,0xE008,1);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
  }
 
  switch(curitem)//Вызов иконок
  { 
  case 0:
    if(len1!=0)//если не путь не равен 0 то рисуем иконку
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);//если пути до иконки нет то рисуем иконку NO_ICON
    break;
  case 1:
    if(len2 != 0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
  case 2:
    if(len3!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
  case 3:
    if(len4!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
  case 4:
    if(len5!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
  case 5:
    if(len6!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
  case 6:
    if(len7!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;    
  case 7:
    if(len8!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
  case 8:
    if(len9!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
  case 9:
    if(len10!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 10);
    break;
/*  case 10:
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
   */
  }
 //SetMLMenuItemText(data, item, ws, ws1, curitem);
   SetMenuItemText(data,item,ws,curitem);
}

//==========================
//   Вызов самих менюшек
//==========================
int num_book()
{ int len1=strlen(BM1FILE);
  int len2=strlen(BM2FILE);
  int len3=strlen(BM3FILE);
  int len4=strlen(BM4FILE);  
  int len5=strlen(BM5FILE);
  int len6=strlen(BM6FILE);
  int len7=strlen(BM7FILE);
  int len8=strlen(BM8FILE);
  int len9=strlen(BM9FILE);
  int len10=strlen(BM10FILE);
 
  int len11=strlen(shortcuts1);
  int len22=strlen(shortcuts2);
  int len33=strlen(shortcuts3);
  int len44=strlen(shortcuts4);  
  int len55=strlen(shortcuts5);
  int len66=strlen(shortcuts6);
  int len77=strlen(shortcuts7);
  int len88=strlen(shortcuts8);
  int len99=strlen(shortcuts9);
  int len12=strlen(shortcuts10);
 
  if(len1!=0 || (!(FILE1) && len11!=0)) number_book=1;
  if(len2!=0 || (!(FILE2) && len22!=0)) number_book=2;
  if(len3!=0 || (!(FILE3) && len33!=0)) number_book=3;
  if(len4!=0 || (!(FILE4) && len44!=0)) number_book=4;
  if(len5!=0 || (!(FILE5) && len55!=0)) number_book=5;
  if(len6!=0 || (!(FILE6) && len66!=0)) number_book=6;
  if(len7!=0 || (!(FILE7) && len77!=0)) number_book=7;
  if(len8!=0 || (!(FILE8) && len88!=0)) number_book=8;
  if(len9!=0 || (!(FILE9) && len99!=0)) number_book=9;
  if(len10!=0 || (!(FILE10 && len12!=0)) number_book=10;
return number_book;
}

void ShowBMmenu1(void)
{ num_book();
  patch_header(&bm_menuhdr);
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
  if (msg->keys==0x15)// '#'
  {
      WSHDR *ws;
      ws=AllocWS(150);
      str_2ws(ws,"0:\\Zbin\\utilities\\CfgEdit.elf",128);
      ExecuteFile(ws,0,"0:\\Zbin\\etc\\info.bcfg");
      FreeWS(ws);
    return(-1);
  }
//if(!OPEN_FIRST)
  if(msg->keys==0x04)
   {show();
    return (-1);
   }
 if(msg->keys==0x14)
   {about();
    return (-1);
   }
 
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

unsigned int *ErrorNumber;

typedef struct
{
  GUI gui;
  int show_csm;
}DUMMY_GUI;

int mode;
extern const int BTN;

//====================================
//        Вызов эльфа
//====================================

int my_keyhook(int submsg, int msg)
{
  int scrH=ScreenH();
  int scrW=ScreenW();
  if(IsUnlocked() && submsg==BTN)
  {
    if(mode==1)
    {
      if(msg==KEY_UP)
      {
        GBS_SendMessage(MMI_CEPID,KEY_DOWN,BTN);
        return(0);
      }
      if(msg==KEY_DOWN)
      {
        mode=0;
        GBS_SendMessage(MMI_CEPID,KEY_UP,BTN);
        return(0);
      }
    }

    if(msg==KEY_UP)
    { if(mode==2)
      {
        mode=0;
        return(2);
      }
    }
    else if(msg==(LONG_P?LONG_PRESS:KEY_DOWN) )//длинное нажатие или нет
    { if(OPEN_FIRST) ShowBMmenu1();//Вызов закладок
      else show();
    }

   
   if(!IsUnlocked()) return(2);

    if(LONG_P)
    {
      if(msg==LONG_PRESS)
      {
        mode=2;
        return(2);
      }
      if(msg==KEY_DOWN)
      {
        mode=1;
        return(2);
      }
    }
    else  return(2);
  } 
  return(0);

}

//==========================
//     11 методов:)
//==========================
void OnRedraw(MAIN_GUI *data) // OnRedraw
{
}

void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int)) //Create
{
  // Тут можно создать переменные
  data->gui.state=1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *)) //Close
{
  // Тут можно освободить выделяемую память

  data->gui.state=0;
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))//Focus
{
  data->gui.state=2;
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *)) //Unfocus
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
  CloseCSM(MAINCSM_ID);

}



int OnKey(MAIN_GUI *data, GUI_MSG *msg) //OnKey
{
  int i = 0;
  if ( msg->gbsmsg->msg == KEY_DOWN ) 
 // if(msg->gbsmsg->submess==RED_BUTTON) return 1;
 // else return 0;
   // case RIGHT_SOFT: show();REDRAW();
    
     
  if ( msg->gbsmsg->msg == LONG_PRESS ) 
  {
   }
  return(i);
}

#ifdef NEWSGOLD
void onDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  mfree_adr(data);
}
#else
extern void kill_data(void *p, void (*func_p)(void *));

void method7(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  kill_data(data,mfree_adr);
  mfree_adr(data);
}
#endif

int method8(void){return(0);}

int method9(void){return(0);}

// Массив с методами, чтобы дать ОС информацию, какие когда вызывать
const void * const gui_methods[11]={
  (void *)OnRedraw,	//Redraw
  (void *)onCreate,	//Create
  (void *)onClose,	//Close
  (void *)onFocus,	//Focus
  (void *)onUnfocus,	//Unfocus
  (void *)OnKey,	//OnKey
  0,
  (void *)kill_data,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};

// Канвас для основного GUI
const RECT Canvas={0,0,129,129};

// Вызывается при создании главного CSM. В данном примере
// создаётся GUI, его ID записывается в MAINGUI_ID
// на всякий случай - вдруг понадобится ;)
void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  MAINGUI_ID=csm->gui_id;
}
//==========================
//     Реконфигурация
//==========================
volatile int callhide_mode=0;

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}
int MyIDLECSM_onMessage(CSM_RAM* data,GBS_MSG* msg)
{
  int csm_result;
 
  csm_result = old_icsm_onMessage(data, msg); //Вызываем старый обработчик событий
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {   //ShowMSG(1,(int)"Config updated!");
      InitConfig();
   }
  

        
  return csm_result;  
}
void MyIDLECSM_onClose(CSM_RAM *data)
{ 
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;

  RemoveKeybMsgHook((void *)my_keyhook);
}

CSM_DESC icsmd;
int main()
{ InitConfig();
/*
  if (!AddKeybMsgHook_end((void *)keyhook))
  {
    ShowMSG(1,(int)"Невозможно зарегистрировать обработчик!");
    SUBPROC((void *)ElfKiller);
  }*/ LockSched();
     CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
      memcpy(&icsmd,icsm->constr,sizeof(icsmd));
      old_icsm_onClose=icsmd.onClose;
      old_icsm_onMessage=icsmd.onMessage;
      icsmd.onClose=MyIDLECSM_onClose;
      icsmd.onMessage=MyIDLECSM_onMessage;
      icsm->constr=&icsmd;
      AddKeybMsgHook((void *)my_keyhook);
  UnlockSched();
  return 0;
}
