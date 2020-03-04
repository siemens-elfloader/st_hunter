#include "..\inc\swilib.h"
#include "rect_patcher.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"

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

//----------------------------------//

//простенький пример, перемещаем квадратик по экрану:)на центральную кнопку делаем его больше на 5 точек
int x=0,y=0;//координаты для квадратика
int col;//цвет надписи
int i=15;//Стокона квадратика:)

IMGHDR MMScreen;

static void OnRedraw(MAIN_GUI *data)//эта функция нужна для перерисовки экрана,тут рисуем все что нам нужно
{
  int scr_w=ScreenW();//Присваиваем переменным высоту и ширину экрана
  int scr_h=ScreenH();
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(1));//Рисуем фон
  DrawRectangle(x,y,x+i,y+i,1,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(1));//расуем сам квадратик
 
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))//Тут делаем что либо при создании гуи
{
#ifdef ELKA//Если елка,то отключаем иконбар
  DisableIconBar(1);
#endif
  data->ws1=AllocWS(256);//Выделяем память под строку
  data->gui.state=1;
  
  int HSIZE = (YDISP+2)*ScreenW();
  int ScrH  = ScreenH();
  MMScreen  = DoScreen(ScrH,HSIZE);
  
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))//Сдесь делаем что нить при закрытии гуи:)
{
  data->gui.state=0;
  FreeWS(data->ws1);//например, освобождаем память
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

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)//Ну это кей хук
{
  DirectRedrawGUI(); //перерисовываем экран при любом нажатии клавиши,мона также использовать REDRAW();
  if (msg->gbsmsg->msg==KEY_DOWN)//Если клавиша нажата
  {
    if(col<23)col++;else col=0;//Чисто для примера)) мигаем надписью внизу:)
    switch(msg->gbsmsg->submess)
    {
    case UP_BUTTON: case '2':y--;break;
    case DOWN_BUTTON: case '8':y++;break;
    case RIGHT_BUTTON: case '4':x++;break;
    case LEFT_BUTTON: case '6':x--;break;
    case ENTER_BUTTON: i+=5;break;
    case RIGHT_SOFT:case RED_BUTTON:
      return (1);
    }
  }
  if (msg->gbsmsg->msg==LONG_PRESS)//или удержана
  {
    if(col<23)col++;else col=0;//Чисто для примера)) мигаем надписью внизу:)
    switch(msg->gbsmsg->submess)
    {
    case UP_BUTTON: case '2':y-=5;break;
    case DOWN_BUTTON: case '8':y+=5;break;
    case RIGHT_BUTTON: case '4':x+=5;break;
    case LEFT_BUTTON: case '6':x-=5;break;
    case RIGHT_SOFT:case RED_BUTTON:
      return (1);
    }
  }
  return(0);
}

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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"example");
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
