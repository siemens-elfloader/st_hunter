#include "..\inc\swilib.h"
//Исходники кривые бля)))))))нужно оптимизировать...

int my_csm_id=0;

char INFO1[32];// info
char INFO2[32];
char INFO3[32];
char INFO4[32];
char INFO5[32];
char INFO6[32];
char INFO7[32];
char INFO8[32];
//char BM9NAME[32];

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

extern const char NAME1[32];//Подписи для инфы
extern const char NAME2[32];
extern const char NAME3[32];
extern const char NAME4[32];
extern const char NAME5[32];
extern const char NAME6[32];
extern const char NAME7[32];
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
extern const char startmsg[64];//стартовое сообщение
extern const int OPEN_FIRST;//окрыть первым закладки или инфу
extern const int ENA_HELLO;//ативир. Hello message
extern const int LONG_P;
extern const char EMPTY[32];
extern const int number_book;//количечтво закладок

extern const int name_cl;//цвет имен закладок
extern const int description_cl;//цвет описаний закладок

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

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;   

//================================
//   Копируем инфу в переменные
//================================

void info()
{ WSHDR *ws=AllocWS(256); 
  TDate date;
  TTime time;
  GetDateTime(&date,&time);
  RAMNET *rn=RamNet();
  int dayy=0;
  char ss[32],ss1[32],ss3[32],ss4[32],ss5[32],ss6[32],ss7[32];
  char *days[8]={" ","Вс","Пн","Вт","Ср","Чт","Пт","Сб"},
       *mounth[13]={" ","Янв","Фев","Март","Апр","Май","Июн","Июл","Авг","Сент","Окт","Ноя","Дек"};
  dayy=date.day;
  while(dayy>7)
  {dayy=dayy-7;   
  }
  char *s=date.day<10?"0":"";
  sprintf(ss,"%s %s,%s%d %s",NAME1,days[dayy],s,date.day,mounth[date.month]);
  int len=strlen(ss);
    for(int j=0;j<len;j++)
    {INFO1[j]=ss[j];}

  char *t2=time.sec<10?"0":"";
  char *t=time.min<10?"0":"";
  sprintf(ss1,"%s %d:%s%d:%s%d",NAME2,time.hour,t,time.min,t2,time.sec);
  int len2=strlen(ss1);
    for(int j=0;j<len2;j++)
     {INFO2[j]=ss1[j];}
  sprintf(ss3,"%s %d %%",NAME3,*RamCap());
  int len3=strlen(ss3);
    for(int j=0;j<len3;j++)
     {INFO3[j]=ss3[j];}
  int f0 = GetFreeFlexSpace(0, ErrorNumber)/1024;
  sprintf(ss4,"%s %d kb",NAME4,f0);
  int len4=strlen(ss4);
    for(int j=0;j<len4;j++)
     {INFO4[j]=ss4[j];}
  sprintf(ss5,"%s %d %%",NAME5,GetCPULoad());
  int len5=strlen(ss5);
    for(int j=0;j<len5;j++)
     {INFO5[j]=ss5[j];}

  sprintf(ss6,"%s %d db",NAME6,rn->power);
  int len6=strlen(ss6);
    for(int j=0;j<len6;j++)
    {INFO6[j]=ss6[j];} 
    
    sprintf(ss7,"%s %d b",NAME7,GetFreeRamAvail()/1000);
  int len7=strlen(ss7);
    for(int j=0;j<len7;j++)
    {INFO7[j]=ss7[j];} 
    FreeWS(ws);
}

void *about()
{ char s[256];
snprintf(s,255,"Info Everywhere v0.1\n(c)kluchnik\nCompile:\n%s %s",__DATE__,__TIME__);
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



const char *infa(int bm)//вызов инфы
{
  switch(bm)
  {
  case 0: return INFO1;
  case 1: return INFO2;
  case 2: return INFO3;
  case 3: return INFO4;
  case 4: return INFO5;
  case 5: return INFO6;
  case 6: return INFO7;
  case 7: return "Об эльфе:)";
  }
  return(0);
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
  case 0: return BM1FILE;
  case 1: return BM2FILE;
  case 2: return BM3FILE;
  case 3: return BM4FILE;
  case 4: return BM5FILE;
  case 5: return BM6FILE;
  case 6: return BM7FILE;
  case 7: return BM8FILE;
  case 8: return BM9FILE;
  case 9: return BM10FILE;/*
  case 10: return BM11FILE;
  case 11: return BM12FILE;
  case 12: return BM13FILE;
  case 13: return BM14FILE;
  case 14: return BM15FILE;*/
  }
  return(0);
}
const char *bm_file(int bm)
{
  switch(bm)
  {
  case 0: return BM1FILE;
  case 1: return BM2FILE;
  case 2: return BM3FILE;
  case 3: return BM4FILE;
  case 4: return BM5FILE;
  case 5: return BM6FILE;
  case 6: return BM7FILE;
  case 7: return about();
  }
  return 0;
}

//==========================
//      Создаем список
//==========================
void bm_menu_iconhndl(void *data, int curitem, void *unk);
void bm_menu_iconhnd(void *data, int curitem, void *unk);
const HEADER_DESC bm_menuhdr={0,0,131,21,NULL,(int)"Extra Info v0.1",LGP_NULL};
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};
const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

void bm_menu_ghook(void *data, int cmd){}
int bm_menu_onkey(void *data, GUI_MSG *msg);
int bm_menu_onkey2(void *data, GUI_MSG *msg);
const HEADER_DESC bm_menuhdr2={0,0,131,21,NULL,(int)"Bookmarks v0.1",LGP_NULL};

const MENU_DESC bm_menu=//Информация
{
  8,bm_menu_onkey,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt,
  0x10,//0x11,
  bm_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};
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

//==========================
//      Запуск закладок
//==========================
int RunBM(int bm)
{
  const char *s=bm_file(bm);
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

//===============================================
// Вывод на экран подписей для инфы и самой инфы
//===============================================

void bm_menu_iconhndl(void *data, int curitem, void *unk)
{
  const char *s;
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  s=infa(curitem);
  
  if (s)
  {
    if (strlen(s))
    {
      ws=AllocMenuWS(data,strlen(s));
      wsprintf(ws,percent_t,s);
    }
    else
    {
      ws=AllocMenuWS(data,10);
      wsprintf(ws,percent_t,EMPTY);//"Пусто:)");
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
  }
  SetMenuItemText(data,item,ws,curitem);
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
      ws=AllocMenuWS(data,strlen(s)+strlen(ss));
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
      wsprintf(ws,percent_t,EMPTY);//"Пусто:)");
      wsprintf(ws1,percent_t,"Надо задать:)");//Цвет описаний
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
 SetMLMenuItemText(data, item, ws, ws1, curitem);
  // SetMenuItemText(data,item,ws,curitem);
}

//==========================
//   Вызов самих менюшек
//==========================
void ShowBMmenu(void)
{
  patch_header(&bm_menuhdr);
  CreateMenu(0,0,&bm_menu,&bm_menuhdr,0,8,0,0);
}

void ShowBMmenu1(void)
{
  patch_header(&bm_menuhdr);
  CreateMultiLinesMenu(0,0,&bmmenu,&bm_menuhdr2,0,number_book);//number_book - кол-во закладок
}


//====================================
//         Обработка клавиш
//====================================
int bm_menu_onkey(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
  if (msg->keys==0x15)// '#'
  {
      WSHDR *ws;
      ws=AllocWS(150);
      str_2ws(ws,DEFAULT_DISK":\\Zbin\\utilities\\CfgEdit.elf",128);
      ExecuteFile(ws,0,DEFAULT_DISK":\\Zbin\\etc\\info.bcfg");
      FreeWS(ws);
    return(-1);
  }
  if(OPEN_FIRST)
  if(msg->keys==RIGHT_SOFT)
   {ShowBMmenu1();
    return (-1);
   }
   if ((k>='1')&&(k<='9'))
      { if (RunBM(k-'1')) return 1;
	return(-1);
      }
  if (msg->keys==0x3D)
  {
    if (RunBM(i))
    {
      return(1);
    }
    return(-1);
  }
  return(0);
}

int bm_menu_onkey2(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
  if (msg->keys==0x15)// '#'
  {
      WSHDR *ws;
      ws=AllocWS(150);
      str_2ws(ws,DEFAULT_DISK":\\Zbin\\utilities\\CfgEdit.elf",128);
      ExecuteFile(ws,0,DEFAULT_DISK":\\Zbin\\etc\\info.bcfg");
      FreeWS(ws);
    return(-1);
  }
if(!OPEN_FIRST)
  if(msg->keys==RIGHT_SOFT)
   {info();ShowBMmenu();
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

int keyhook(int submsg, int msg)
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
    {info();
    if(OPEN_FIRST) ShowBMmenu();
    else ShowBMmenu1();
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
  {   ShowMSG(1,(int)"Extra Info config updated!");
      InitConfig();
   }
  
  return csm_result;  
}
void MyIDLECSM_onClose(CSM_RAM *data)
{ 
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;

  RemoveKeybMsgHook((void *)keyhook);
}

CSM_DESC icsmd;
int main()
{ InitConfig();

  if (!AddKeybMsgHook_end((void *)keyhook))
  {
    ShowMSG(1,(int)"Невозможно зарегистрировать обработчик!");
    SUBPROC((void *)ElfKiller);
  }
  if(ENA_HELLO) ShowMSG(1,(int)startmsg);
  {   CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
      memcpy(&icsmd,icsm->constr,sizeof(icsmd));
      old_icsm_onClose=icsmd.onClose;
      old_icsm_onMessage=icsmd.onMessage;
      icsmd.onClose=MyIDLECSM_onClose;
      icsmd.onMessage=MyIDLECSM_onMessage;
      icsm->constr=&icsmd;
  }
}
