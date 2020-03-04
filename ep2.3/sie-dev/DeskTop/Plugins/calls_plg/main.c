#include "..\..\..\inc\swilib.h"
#include "..\..\plglib.h"
#include "..\..\plglib.c"


extern void kill_data(void *p, void (*func_p)(void *));
static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


struct{
	char num[64];
	char pos;
}NMS; // нужно для ввода номера

int f_h; // высота мелкого шрифта
WSHDR *ews;
DT *dt; // указатель на основную структуру

DT_ICON *MI; // указатель на структуру нашеё иконки

const char call_txt[]="Вызов";






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

// DrwImg(IMGHDR *img, int x, int y, char *pen, char *brush)
// {
  // RECT rc;
  // DRWOBJ drwobj;
  // StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  // SetPropTo_Obj5(&drwobj,&rc,0,img);
  // SetColor(&drwobj,pen,brush);
  // DrawObject(&drwobj);
// }


extern PGUI_methods numgui;
 //Redraw
static void gOnRedraw() // перерисовка окна
{
	
	DrawRectangle(numgui.p.x,numgui.p.y,numgui.p.x2,numgui.p.y2,0,black,black); // фон окна
	
	// рисуем набираемый номер
	wsprintf(ews, perc_t, NMS.num); // вводимый номер
	DrawString(ews,numgui.p.x, numgui.p.y, numgui.p.x2,numgui.p.y2, FONT_MEDIUM,TEXT_ALIGNMIDDLE,white,NULL); // вывод номера на экран
	
	// рисуем кнопку:
	wsprintf(ews, perc_t, call_txt); // текст кнопки вызова
	DrawRectangle(numgui.p.x+3, numgui.p.y2-f_h-5, numgui.p.x+5+Get_WS_width(ews, FONT_SMALL), numgui.p.y2-3, 0,white,black); // рамка кнопки
	DrawString(ews, numgui.p.x+3, numgui.p.y2-f_h-5, numgui.p.x+5+Get_WS_width(ews, FONT_SMALL), numgui.p.y2-3, FONT_SMALL,TEXT_ALIGNMIDDLE,white,NULL); // вывод текста кнопки
}

 //Create
static void gOnCreate() // при создании окна
{
	f_h=GetFontYSIZE(FONT_SMALL); // задаем высоту мелкого шрифта
	ews=AllocWS(256); // надо
	zeromem(&NMS,sizeof(NMS)); // надо
	StoreXYXYtoRECT(&numgui.p, 10, ScreenH()/4, ScreenW()-10, ScreenH()/2); // задаём начальное положение окна
}

 //Close
static void gOnClose() // при закрытии окна
{
	FreeWS(ews);
}


/////////////////////////
///////////кнопка////////
/////////////////////////

int key_call(GUI_MSG *msg) // нажатие на кнопку вызова
{
  if(NMS.pos>1) // если введено больше одного символа в номере
  {
	MakeVoiceCall(NMS.num,0x10,0x2FFF); // звоним
	return 1; // и закрываем окно
  }
  return 0; // иначе оставляем окно
}

PG_BUTTON btn_call= // кнопка вызова
{
  0,0, //void *prev,*next; // не трогать
  0, //int n; // не трогать
  {0,0,0,0}, //RECT r; // не трогать
  0, //char *inf; // не трогать
  key_call //int (*Enter)(); // выполняется по нажатию на кнопку
};


/////////////////////////
/////////////////////////
/////////////////////////


 //Focus
static void gOnFocus() // тут можно "кнопки" добавлять в окно
{
  RECT tp;
  wsprintf(ews, perc_t, call_txt);
  StoreXYWHtoRECT(&tp, 3, numgui.w->h-f_h-5, 5+Get_WS_width(ews, FONT_SMALL), numgui.w->h-3); // задаём площадь для кнопки
  dt->pf->add_btn(&numgui, &btn_call, &tp); // добавляем кнопку в список
}

 //Unfocus
static void gOnUnfocus() // пока не применяется
{
}

 //OnKey
static int gOnKey(GUI_MSG *msg) // обработка клавиш при активном окне
{
	int m=msg->gbsmsg->msg;
	int sm=msg->gbsmsg->submess;
	switch(m)
	{
		case LONG_PRESS:
			switch(sm)
			{
				case '0':
					if(NMS.pos==1) NMS.num[0]='+';
					return -1;
				case RIGHT_SOFT:
					if(NMS.pos)
						if(strlen(NMS.num)) zeromem(&NMS,sizeof(NMS));
					return -1;
			}
			break;
		case KEY_DOWN:
			if(((sm>='0')&&(sm<='9')) || (sm=='*') || (sm=='#'))
			{
				NMS.num[NMS.pos++]=sm;
				return -1;
			}
			switch(sm)
			{
				case RIGHT_SOFT:
					if(NMS.pos && strlen(NMS.num)) NMS.num[--NMS.pos]=0;
					else return 1;
					return -1;
				case RED_BUTTON:
					return 1;
// 				case ENTER_BUTTON: // энтер работать не будет
// 					if(dt->actpg->n==4)
// 					{
// 						if(NMS.pos>1)
// 						{
// 							MakeVoiceCall(NMS.num,0x10,0x2FFF);
// 							return 1;
// 						}
// 						return 2;
// 					}
// 					return 0;
				case LEFT_SOFT:
					return 2;
				case GREEN_BUTTON:
						if(NMS.pos>1)
						{
							MakeVoiceCall(NMS.num,0x10,0x2FFF);
							return 1;
						}
					return 2;
			}
	//DirectRedrawGUI();
			break;
	}
  //curkey(msg);
	//DirectRedrawGUI();
	return 0;
}

PGUI_methods numgui={ // структура окна
	0,0,
	"Number", // имя
	0, // state (PG_st_norm-обычное окно, PG_st_max-во весь экран)
	1+2, // type (0-without cursor, 1-with cursor, +2-with window, +4-disactive desctop) пока криво, лучше юзать только 0 или 3
	0, // пока лишняя, не тогать
	0,
	{10,30,100,120},{0,0,0,0}, // координаты
	gOnRedraw, // функция перерисовки
	gOnCreate, // выполняется при создании окна
	gOnClose, // выполняется при закрытии окна
	gOnFocus, // тут кнопки можно добавлять 
	gOnUnfocus, // пока не фурычит 
	gOnKey, // обработка клавиш 
	NULL, // не трогать 
	NULL, // не трогать 
	{0,0,0,0} // не трогать 
// 	void *next;
};












// void onMessage(GBS_MSG* msg)
// {
// }


int onEnter() // нажатие на иконку
{
	dt->pf->Cr_PGUI(&numgui); // создаём окно
	return 0;
}

void onClose() // при закрытии десктопа
{
    SUBPROC((void *)Killer);
}

void Refresh() // обновление (тут не нужно)
{
}

void onInf() // задание текста всплывающей подсказки
{
	zeromem(MI->inf, 256);
	sprintf(MI->inf, "%s", MI->nm); // берём имя иконки
}

int main(char *exename, char *fname)
{
	if((int)fname>=0xa0000000) // fname действительно указатель
	{
	  // не трогать ///////////////////////////
	  PLG_P *TP=(PLG_P*)fname; // не трогать
	  MI=TP->ic; // наша иконка
	  dt=TP->dt; // основная структура
	  MI->d=1; // не трогаем
	  /////////////////////////////////////////
	  
	  //MI->onDraw=onDraw; // задание функции рисования иконки (здесь не нужна, используется стандартная)
	  MI->onEnter=onEnter; // задание функции по нажатию на иконку
	  MI->onInf=onInf; // функция задания текста всплывающей подсказки
	  MI->dyn=0; // не динамическая (рисуется картинка, заданная в настройках иконки) если 1, необходимо задать onDraw
	  MI->sel=0; // не робит пока
	  MI->onClose=onClose; // задание функции при закрытии десктопа
	  MI->Refresh=NULL;//Refresh; // обновление иконки
// 	  MI->onMessage=onMessage; // не работает
	  dt->ef->fill_icons(3); // полностью перерисовываемся
	}
	else
	  SUBPROC((void *)Killer); // закрываемся если fname точно не указатель
  return 0;
}
