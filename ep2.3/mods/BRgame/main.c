#include "..\inc\swilib.h"
#include "rect_patcher.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"
#include "print.h"

#define TMR_SECOND 216
#define kadrCLK 4
//-----------------------------------------------------//
//Эт стремные переменные,но они нужны в любом проекте :)
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
//Объявляем переменные из конфига
extern const int font;
extern void InitConfig();
//----------------------------------//
int sm=2; // смещение в байтах от начала для обхода даных карты до начала номера первой картинки
int sizeSP=16; //размер ячейки
int sizeX=8;  // сколько ячеек выводить на экран по Х
int sizeY=9;  // сколько ячеек выводить на экран по Y
int scrX=0; //координаты экрана относительно начала карты
int scrY=0;
int cursorX=0; //координаты курсора в карте
int cursorY=0;
int Xscr=2;  // текущие координаты в экране 
              // точнее координаты с которых выводится карта 
GBSTMR timerredraw;

char pathF[]= 
{"4:\\zbin\\Black Raven\\img\\font\\"
};// путь к шрифту 
int countM2=3; // текущий пункт меню
int counCOLM=6; // максимум пунктов меню
int sizeF=15;  // размер точнее высота шрифта
int Yscr=2;
int i;
int y;
int x;
int p;
int DelayVAR;
int tpic=17;  // текущая картинка
int maxpic=34;// максимальный номер картинки для фона
int minpic=17;
int scrCURx;
int typeGUI;
int scrCURy;
int typeGUI=0;// номер текущего экрана:0-карта 1-меню 2-выбор картинки

void printmap(); // функция рисует карту
int npic (int x,int y); // функция возвращает номер картинки
void printpic (int x,int y,int numer); // рисует картинку
void printCUR (); // рисует курсор
void vverh ();
void vniz ();
void vpravo ();
void vlevo ();
void vibor ();
void printM1 ();
void drawSBy ();
void drawSBx ();
int REDRAW2 ();
void sss();

extern char map[3728]=// карта ландшафта
{
  0x14,0x14,// размер карты по умоланию 20 на 20
  
  54,0x01,0x01,23,18,18,21,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,32,27,27,27,27,
  0x01,0x01,0x01,0x01,24,20,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,32,27,27,27,
  0x01,0x01,0x01,0x01,24,18,25,25,25,25,25,25,19,0x01,0x01,0x01,0x01,33,27,27,
  0x01,0x01,0x01,0x01,23,18,18,18,18,18,18,18,20,0x01,0x01,0x01,0x01,33,27,27,
  54,0x01,0x01,0x01,0x01,23,18,18,18,22,18,18,20,0x01,0x01,0x01,0x01,33,27,27,
  0x01,0x01,0x01,0x01,0x01,0x01,24,18,21,0x01,23,22,21,0x01,0x01,0x01,0x01,33,27,27,
  0x01,0x01,0x01,0x01,0x01,0x01,23,21,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,33,27,27,
  19,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,17,19,0x01,0x01,33,27,27,
  18,25,19,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,23,21,0x01,0x01,33,27,27,
  18,18,21,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,32,27,27,
  18,21,0x01,0x01,0x01,17,25,25,25,25,20,0x01,0x01,0x01,0x01,0x01,0x01,0x01,33,27,
  20,0x01,0x01,0x01,0x01,24,18,18,22,22,21,0x01,0x01,0x01,0x01,0x01,0x01,26,27,30,
  20,0x01,0x01,0x01,0x01,23,22,21,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,26,27,30,0x01,
  18,19,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,26,27,30,0x01,0x01,
  18,20,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,26,27,31,31,31,30,0x01,0x01,0x01,
  18,20,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,26,27,30,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  18,18,19,0x01,0x01,0x01,0x01,0x01,0x01,26,27,29,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  18,18,18,19,0x01,0x01,0x01,0x01,26,27,27,29,0x01,0x01,0x01,0x01,0x01,0x01,0x01,17,
  18,18,18,18,25,19,0x01,26,27,27,27,29,0x01,0x01,0x01,0x01,0x01,17,25,18,
  18,18,18,18,18,20,26,27,27,27,27,30,0x01,0x01,0x01,0x01,17,18,18,18,
  
};

//void canvasdata(void);

void *canvasdata;
int delay (int t);
void printPCUR ();
int npic2save ();
void printwin (int x,int y,int l,int h);
  


static void OnRedraw(MAIN_GUI *data)//эта функция нужна для перерисовки экрана,тут рисуем все что нам нужно
{
  
  printmap ();  // рисуем карту
  printCUR ();  // рисуем курсор
  drawSBy ();   //рисуем скролбар
  drawSBx ();
  if (typeGUI==2)
  {
  printPCUR ();
  }
  if (typeGUI==1)
   {
   printwin (4,10,120,150);
   printM1 ();
   }
}
// тут наши фунуции процедуры и т д///////////////////////////////////////////////////////////////////
void sss()
{
REDRAW();
GBS_StartTimerProc(&timerredraw,TMR_SECOND/kadrCLK,sss);
}
void printM1 ()// рисует меню
{
int countM=0;
int xm1=11;
int ym1=10;
int xm1m=xm1;
int ym1m=ym1;
 char *string=malloc(64);
  countM++;
  DrawRoundedFrame(xm1m-1,ym1m+countM2*sizeF-3,118,ym1m+countM2*sizeF+sizeF-4,0,0,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(15));
  sprintf (string, "ОТКРЫТЬ");
  PrintField(xm1m,ym1m+countM*sizeF,string,0,1,0); //вывод строки
 countM++;
 sprintf (string, "СОХРАНИТЬ");
  PrintField(xm1m,ym1m+countM*sizeF,string,0,1,0);
  countM++;
  sprintf (string, "Цель мисии");
  PrintField(xm1m,ym1m+countM*sizeF,string,0,1,0);
  countM++;
  sprintf (string, "Возможности уровня");
  PrintField(xm1m,ym1m+countM*sizeF,string,0,1,0);
  countM++;
  sprintf (string, "Золото");
  PrintField(xm1m,ym1m+countM*sizeF,string,0,1,0);
  countM++;
  sprintf (string, "Лес");
  PrintField(xm1m,ym1m+countM*sizeF,string,0,1,0);
  
  mfree(string);
  
  countM=1;
 }



int npic (int x,int y){// функция возвращает номер картинки на входе х и у кординаты
 i=1;
 p=1;
i=sm+x+y*map[0];
int p=map[i];
return p;
}

int npic2save (){// функция возвращает смещение от начала карты для текущей картинки
 i=1;
 y=cursorY;
 x=cursorX;
i=sm+x+y*map[0];
return i;
}


void printpic (int x,int y,int numer){//рисует картинку на входе координаты на экране и номер
char *path=malloc(128);
sprintf(path, "4:\\zbin\\Black Raven\\img\\map\\%d.png", numer);
DrawImg(x,y,(int)path);
mfree(path);
}
void printCUR (){// рисует курсор
 int scrCURx;
 int scrCURy;
 scrCURx=Xscr+(cursorX-scrX)*sizeSP;
 scrCURy=Yscr+(cursorY-scrY)*sizeSP;
DrawImg(scrCURx,scrCURy,(int)"4:\\zbin\\Black Raven\\img\\map\\0.png");
 
}

 void printPCUR ()//  рисует картинку текущую по месту курсора
{
 scrCURx=Xscr+(cursorX-scrX)*sizeSP;
 scrCURy=Yscr+(cursorY-scrY)*sizeSP;
 int numer2=tpic;
 char *path=malloc(128);
sprintf(path, "4:\\zbin\\Black Raven\\img\\map\\%d.png", numer2);
DrawImg(scrCURx,scrCURy,(int)path);
mfree(path);
}

void drawSBy ()// скролбар Х
{ int ySB; int lenSB;
lenSB=159*((sizeSP*100)/(sizeSP*map[1]))/100;
ySB=(lenSB)*cursorY;
  DrawLine(130,1,130,147,1,GetPaletteAdrByColorIndex(6));
  DrawRoundedFrame(129,ySB-3,131,ySB+lenSB-3,0,0,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(2));
}

void  drawSBx ()// скролбар У
{
DrawLine(1,147,130,147,1,GetPaletteAdrByColorIndex(6));
int xSB; int lenSB;
lenSB=146*((sizeSP*100)/(sizeSP*map[0]))/100;
xSB=lenSB*cursorX;
DrawRoundedFrame(xSB-3,146,xSB+lenSB-3,148,0,0,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(2));
}
void printwin (int x,int y,int l,int h)  // рисует окно
{
  DrawRoundedFrame(x,y,x+l,y+h,5,5,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(1));
  DrawRoundedFrame(x+2,y+2,x+l-2,y+h-2,5,5,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(1));
  DrawRoundedFrame(x+3,y+3,x+l-3,y+h-3,5,5,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(1));
 
}
void printmap ()// рисует карту
{
  int scr_w=ScreenW();//Присваиваем переменным высоту и ширину экрана
  int scr_h=ScreenH();
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));//Рисуем фон
  DrawRoundedFrame(0,0,122,152,0,0,0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));//Рисуем рамку
  x=Xscr;   //координаты начала вывода карты в экране
  y=Yscr;
  int i;
  int n;
  int o; 
  for (o=0;o<sizeY;o++)
  {
  for (i=0;i<sizeX;i++)
    {
   n=npic(scrX+i,scrY+o);
   printpic (x+i*sizeSP,y+sizeSP*o,n);
    }
  }
}
 
 
// обработка клавиши вверх
void vverh ()
{
  
  if(cursorY-scrY==0)
  {
  if (scrY>0)
  {
    scrY--;
  }
  }
if(cursorY!=0)
{
  cursorY--;
}  
}


// обработка клавиши вниз
  void  vniz ()
{
      
  if(cursorY-scrY==sizeY-1)
  {
  if (scrY<map[1]-sizeY)
   {
    scrY++;
   }
  }
 if(cursorY<map[1]-1)
    {
  cursorY++;
    }  
}
  void  vniz2 ()
  {
///////////////////////////////////////////////// 
  }
void vpravo2 ()
{
if (tpic<maxpic) tpic++;
printPCUR ();
}
// обработка клавиши вправо
  void vpravo ()
    {
      
  if(cursorX-scrX==sizeX-1)
  {
    if (scrX<map[0]-sizeX)
  {
    scrX++;
  }
  }
if(cursorX<map[0]-1)
{
  cursorX++;
}  
}

// обработка клавиши влево
  void vlevo ()
 {
  
  if(cursorX-scrX==0)
  {
   if (scrX>0)
  {
    scrX--;
  }
  }
if(cursorX!=0)
{
  cursorX--;
}  
}

void vlevo2 ()
{

if (tpic>minpic) tpic--;

printPCUR ();

}

// обработка клавиши выбор

  void vibor ()
  {
   typeGUI=2;
  }

void vibor2 ()
{
  int i;
 i=npic2save ();
   map[i]=tpic;
   REDRAW();
   typeGUI=0;
}
void vibor3 ()
{
 typeGUI=0;
}

// обработка левого софта
  void menukl (){
    typeGUI=1;
        }

void vverh3 ()
{
  if (countM2==1)
  {
  countM2=counCOLM;
  
  }
  else 
  {
    countM2--;
    
  }
  }

void vniz3 ()
{
  if (countM2==counCOLM)
  {
    countM2=1;
   
  }
  else  
  {
    countM2++;
   
  }
}
//-----------------------------------------------------------------------------------------------------

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))//Тут делаем что либо при создании гуи
{
#ifdef ELKA//Если елка,то отключаем иконбар
  DisableIconBar(1);
#endif
  data->ws1=AllocWS(256);//Выделяем память под строку
  data->gui.state=1;
   PrintLibInit (1);
   FontPathInit(pathF,0);
   sss();
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))//Сдесь делаем что нить при закрытии гуи:)
{
  data->gui.state=0;
  PrintLibFree();
  FreeWS(data->ws1);//например, освобождаем память
  GBS_DelTimer(&timerredraw);
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))//Если гуи на верху то опять же делаем что нужно
{
  data->gui.state=2;
  DisableIDLETMR();//отключаем таймер,иначе через 2 минуты эльф закроется
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))//а если гуи не вверху,то можно закрыть эльф например:)
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

//////////////////////////////////////////////////////////////////////////

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)//Ну это кей хук
{
  
  if (msg->gbsmsg->msg==KEY_DOWN)//Если клавиша нажата
  {
     if (typeGUI==0)  // карта
         switch(msg->gbsmsg->submess)
      { 
    case UP_BUTTON: case '2':vverh ();break;
    case DOWN_BUTTON: case '8':vniz ();break;
    case RIGHT_BUTTON: case '6':vpravo ();break;
    case LEFT_BUTTON: case '4':vlevo ();break;
    case ENTER_BUTTON:vibor (); break;
    case LEFT_SOFT:menukl ();break;
    case RIGHT_SOFT:case RED_BUTTON:
      return (1); 
      }
     if (typeGUI==1)  //  меню
           switch(msg->gbsmsg->submess)
    {
    case UP_BUTTON: case '2':vverh3 ();break;
    case DOWN_BUTTON: case '8':vniz3 ();break;
    case ENTER_BUTTON:vibor3 ();break;
    case LEFT_SOFT:REDRAW ();break;
    case RIGHT_SOFT:typeGUI=2; REDRAW ();break;
    case RED_BUTTON:
      return (1);
    }
     if (typeGUI==2)  // замена текущей картинки в карте
     switch(msg->gbsmsg->submess)
     {
    case RIGHT_BUTTON: case '6':vpravo2 ();break;
    case LEFT_BUTTON: case '4':vlevo2 ();break;
    case ENTER_BUTTON:vibor2 ();break;
    case LEFT_SOFT:menukl ();break;
    case RIGHT_SOFT:case RED_BUTTON:
    return (1);
    }
    
  }
  
  
  
  if (msg->gbsmsg->msg==LONG_PRESS)//или удержана
  {
    
    switch(msg->gbsmsg->submess)
    {
     if (typeGUI==2)
      {
    case UP_BUTTON: case '2':vverh ();break;
    case DOWN_BUTTON: case '8':vniz ();break;
    case RIGHT_BUTTON: case '6':vpravo ();REDRAW();vpravo ();break;
    case LEFT_BUTTON: case '4':vlevo ();REDRAW();vlevo ();break;
    case RIGHT_SOFT:case RED_BUTTON:
      return (1);
      }  
     }
  }
  return(0);
}

/////////////////////////////////////////////////////////////////////////
extern void kill_data(void *p, void (*func_p)(void *));//Эта функция убивает эльф и освобождает память

int method8(void){return(0);}
int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)OnRedraw,
  (void *)onCreate,
  (void *)onClose,
  (void *)onFocus,
  (void *)onUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data,
  (void *)method8,
  (void *)method9,
  0
};

static void maincsm_oncreate(CSM_RAM *data)//Вызывается при открытии эльфа,делаем что нужно
{
  //тут хитрые махинации для получения ид гуя чтобы мона было с ним манипулировать
  static const RECT Canvas={0,0,0,0};
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
}

void ElfKiller(void)//освобождаем все занятое место эльфом в оперативе
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)//функция закрытия эльфа,тут освобождаем памчть если нужно и т.п.
{
  SUBPROC((void *)ElfKiller);
}


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

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)//тут вообщем протекают все сообщения поступаемые из эльфа так сказать:)
{
  //сообщение о реконфигурации например,ипц и т.д. и т.п.
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  if (msg->msg==MSG_RECONFIGURE_REQ)//Елси была реконфигурация,то обновляем настройки эльфа
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"example config updated!");
      InitConfig();
    }
  }

  return(1);
}


static const struct//это структура цсм,разбираться не надо:)
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

void UpdateCSMname(void)//ну собственно имя эльфа передаваемое хтаску
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"BRgame");
}


int main(char *exename, char *fname)//exename-путь запускаемого эльфа, fname параметр передаваемый эльфу
{
  MAIN_CSM main_csm;
  InitConfig();//инициируем конфиг,тобишь читаем настройки из него
  LockSched();
  UpdateCSMname();//обновляем имя для хтаска
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);//создаем цсм
  UnlockSched();
  return 0;
}
