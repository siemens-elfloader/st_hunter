#include "csgui.h"
#include "cfg_param.h"
#include "print.h"

static const char * const weekdays[7]=	//массив с именами дней недели
{
	"Понедельник",
	"Вторник",
	"Среда",
	"Четверг",
	"Пятница",
	"Суббота",
	"Воскресенье"
};

static const char * const monthes[12]=	//название месяцев
{
	"Января",
	"Февраля",
	"Марта",
	"Апреля",
	"Мая",
	"Июня",
	"Июля",
	"Августа",
	"Сентября",
	"Октября",
	"Ноября",
	"Декабря"
};

int isGUI;		//ну это гуи либо есть, либо его нет. флаг состояния
void *canvasdata;

void csGUI::redraw ()	//перерисуем-ка мы гуй
{
	DirectRedrawGUI();
}

void csGUI::onRedraw()	//рисовалка гуйни
{
	TTime time;
	TDate date;

	WSHDR *date_str=AllocWS(128);
	const char transparent[4]={0,0,0,0};


	if (BG_TYPE==1)	//если тип фона - картинка
	{
		DrawImg(0,0,(int)bg_img);	//то рисуем картинку
	}
	else	//если нет
	{
		DrawRectangle(0,0,ScreenW()-1,ScreenH()-1,0,bg_col,bg_col);	//то рисуем цветом
	}

	if (USE_CLOCK_BG)	//надо подложку?
	{
		DrawImg((ScreenW()-GetImgWidth((int)clock_bg_img))/2,clock_bg_y,(int)clock_bg_img);	//получите
	}

	GetDateTime(&date, &time);	//получим время/дату


	wsprintf(date_str,"%02d %t %d%t\n%t", date.day, monthes[date.month-1], date.year, "г", weekdays[GetWeek(&date)]);			//напечатали
	DrawString(date_str,0,date_y,ScreenW()-1,date_y+GetFontYSIZE(DATE_FONT_SIZE)*2,DATE_FONT_SIZE,2,date_font_col,transparent);	//и вывели ее

	if (!CLOCK_FONT_TYPE) //если системный
	{
		WSHDR *time_str=AllocWS(32);
		wsprintf(time_str,"%02d:%02d",time.hour,time.min);	//запишем
		DrawString(time_str,0,clock_y,ScreenW()-1,clock_y+GetFontYSIZE(CLOCK_FONT_SIZE),CLOCK_FONT_SIZE,2,clock_font_col,transparent); //вывели
		FreeWS(time_str);
	}
	else
	{
		PrintLibInit(1);	//инициируем либу пнг-шрифтов из одного шрифта
		FontPathInit((char *)clock_font_path,0);	//инициируем шрифт

		char *clockstr=new char [5];
		sprintf(clockstr,"%02d:%02d", time.hour, time.min);	//так и запишем

		PrintField(ScreenW()/2, clock_y, clockstr, 1, 1, clock_font_gap);	//нарисуем на экране
	}

	FreeWS(date_str);
}

void csGUI::onCreate()	//при создании гуйни
{
	isGUI=1;	//он есть!
}

void csGUI::onFocus()	//если в фокусе
{
#ifdef ELKA
	DisableIconBar(1);	//вырубим статусбар
#endif
}

void csGUI::onUnFocus()	//потеряли фокус
{
#ifdef ELKA
	DisableIconBar(0);	//опять включим статусбар
#endif
	Close();		//и закроем гуй
}

void csGUI::onClose()	//при закрытии
{
	isGUI=0;		//нет его
	SetIllumination(0,1,0,0);	//выключим свет
	extern GBSTMR timer;
	GBS_DelTimer(&timer);	//удалим таймер
	DrawScreenSaver();
	delete this;		//не забываем подчистить за собой
}

int  csGUI::onKey(char key_code, int key_msg, short keys)	//при нажатии клавиш
{
	if (key_msg==KEY_DOWN)	//если нажали клавишу. притом любуй (кроме кнопки хтаска)
	{
		Close();	//закроемся
		return GUI_RESULT_CLOSE;
	}
	return GUI_RESULT_OK;
}


csGUI::csGUI()
{}
csGUI::~csGUI()
{}
