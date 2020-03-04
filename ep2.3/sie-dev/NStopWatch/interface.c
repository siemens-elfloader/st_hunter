#include "../inc/swilib.h"
#include "graphics.h"
#include "stopwatch.h"
#include "lang.h"

const char *d="%d";

WSHDR *ws_cur_time;

inline int get_ws_width(WSHDR *ws, int font){ return Get_WS_width(ws, font)+1; }

int SelectFont(int CFG_FONT)
{
	switch(CFG_FONT)
	{
		case 0: return FONT_SMALL;
		case 1: return FONT_SMALL_BOLD;
		case 2: return FONT_MEDIUM;
		case 3: return FONT_MEDIUM_BOLD;
		case 4: return FONT_LARGE;
		case 5: return FONT_LARGE_BOLD;
		default: return FONT_SMALL;
	}
}

void DrawIMG(IMGHDR *img, int x, int y, int x2, int y2)
{
	RECT rc;
	DRWOBJ drwobj;
	if(!x2 && !y2)
	{
		StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
		SetPropTo_Obj5(&drwobj, &rc, 0, img);
	}
	else
	{
		StoreXYXYtoRECT(&rc, x, y, x2, y2);
		SetProp2ImageOrCanvas(&drwobj, &rc, 0, img, x, y);
	}
	SetColor(&drwobj, NULL, NULL);
	DrawObject(&drwobj);
}

void DrawCurTime(const int mode) //текущее время mode=0-other, 1-sec, 2-all
{
	extern const int CFG_FONT_CUR_TIME, CFG_FONT_CUR_TIME_SEC, CFG_INDENT_Y_CUR_TIME_SEC;
	extern const unsigned int CFG_X_CUR_TIME, CFG_Y_CUR_TIME;
	extern const char CFG_COLOR_CUR_TIME[4];
	
	int x=0, x2=0, font=0;
	
	const int y2=CFG_Y_CUR_TIME+GetFontYSIZE(SelectFont(CFG_FONT_CUR_TIME));
	wsprintf(ws_cur_time, "%02d:%02d:%02d.", time.hour, time.min, time.sec);
	if(mode==0 || mode==2) 
	{
		font=SelectFont(CFG_FONT_CUR_TIME);
		x=CFG_X_CUR_TIME;
		x2=x+get_ws_width(ws_cur_time, font);
		if(mode!=2)//неполная перерисовка
			DrawIMG(IMG[IMG_BG], x, CFG_Y_CUR_TIME, x2, y2);
		DrawString(ws_cur_time, x, CFG_Y_CUR_TIME, x2, y2, font, TEXT_ALIGNLEFT,
			CFG_COLOR_CUR_TIME, GetPaletteAdrByColorIndex(23));
	}
	if(mode==1 || mode==2)
	{
		font=SelectFont(CFG_FONT_CUR_TIME_SEC);
		x=CFG_X_CUR_TIME+get_ws_width(ws_cur_time, SelectFont(CFG_FONT_CUR_TIME));
		wsprintf(ws_cur_time, "%02d", time.millisec);
		x2=x+get_ws_width(ws_cur_time, font);
		int y=y2-CFG_INDENT_Y_CUR_TIME_SEC;
		if(mode!=2)//неполная перерисовка
			DrawIMG(IMG[IMG_BG], x, y, x2, y2);
		DrawString(ws_cur_time, x, y, x2, y2, font, TEXT_ALIGNLEFT,
			CFG_COLOR_CUR_TIME, GetPaletteAdrByColorIndex(23));
	}
}

void DrawButtons(int cur)
{
	extern const unsigned int CFG_X_BUTTON_LEFT, CFG_Y_BUTTON_LEFT, CFG_X_BUTTON_RIGHT, CFG_Y_BUTTON_RIGHT;
	if(!IsStart())
	{
		switch(cur)
		{
			case 0:
				DrawIMG(IMG[IMG_START_PRESSED], CFG_X_BUTTON_LEFT, CFG_Y_BUTTON_LEFT, 0, 0);
				DrawIMG( IMG[IMG_RESET_UNPRESSED], CFG_X_BUTTON_RIGHT, CFG_Y_BUTTON_RIGHT, 0, 0);
			break;
			case 1:
				DrawIMG(IMG[IMG_START_UNPRESSED], CFG_X_BUTTON_LEFT, CFG_Y_BUTTON_LEFT, 0, 0);
				DrawIMG(IMG[IMG_RESET_PRESSED], CFG_X_BUTTON_RIGHT, CFG_Y_BUTTON_RIGHT, 0, 0);
			break;
		}
	}
	else
	{
		switch(cur)
		{
			case 0:
				DrawIMG(IMG[IMG_SPLIT_PRESSED], CFG_X_BUTTON_LEFT, CFG_Y_BUTTON_LEFT, 0, 0);
				DrawIMG(IMG[IMG_STOP_UNPRESSED], CFG_X_BUTTON_RIGHT, CFG_Y_BUTTON_RIGHT, 0, 0);
			break;
			case 1:
				DrawIMG(IMG[IMG_SPLIT_UNPRESSED], CFG_X_BUTTON_LEFT, CFG_Y_BUTTON_LEFT, 0, 0);
				DrawIMG(IMG[IMG_STOP_PRESSED], CFG_X_BUTTON_RIGHT, CFG_Y_BUTTON_RIGHT, 0, 0);
			break;
		}
	}
}

void DrawSplitTimes(const int type_redraw, const int cur_list)
{
	extern const unsigned int CFG_X_SPLIT_TIMES, CFG_Y_SPLIT_TIMES, CFG_X_NUM_RES, CFG_Y_NUM_RES;
	extern const unsigned int CFG_INDENT_X_SPLIT_TIMES, CFG_INDENT_Y_SPLIT_TIMES, CFG_INDENT_X_RES_NUM, CFG_INDENT_Y_RES_NUM ;
	extern const int CFG_FONT_SPLIT_TIMES, CFG_FONT_RES_NUM;
	extern const char CFG_COLOR_SPLIT_TIMES[];
	
	int x1, y1;
	int axes_x, axes_y;
	int cur_num;
	int font=SelectFont(CFG_FONT_SPLIT_TIMES);
	WSHDR *ws=AllocWS(16);
	unsigned int min, max;
	if(type_redraw==1)
	{
		if(res.total)
		{
			unsigned int x_bg, y_bg;
			x_bg=(CFG_X_SPLIT_TIMES<CFG_X_NUM_RES) ? CFG_X_SPLIT_TIMES : CFG_X_NUM_RES;
			y_bg=(CFG_Y_SPLIT_TIMES<CFG_Y_NUM_RES) ? CFG_Y_SPLIT_TIMES : CFG_Y_NUM_RES;
			DrawIMG(IMG[IMG_BG], x_bg, y_bg, ScreenW(), ScreenH()); //ScreenW(), ScreenH() - костылек, да
			//DrawRectangle(x_bg, y_bg, ScreenW(), ScreenH(), 0, GetPaletteAdrByColorIndex(13),
			//	GetPaletteAdrByColorIndex(4));
		}
		for(int i=0; i<10; i++)//номера результатов
		{
			axes_x=(i<5) ? 0 : CFG_INDENT_X_RES_NUM;
			axes_y=(i<5) ? CFG_INDENT_Y_RES_NUM*i : CFG_INDENT_Y_RES_NUM*(i-5);
			x1=CFG_X_NUM_RES+axes_x;
			y1=CFG_Y_NUM_RES+axes_y;
			wsprintf(ws, d, cur_list*10+i+1);
			DrawString(ws, x1, y1, x1+get_ws_width(ws, 10), y1+GetFontYSIZE(10), 10,
				TEXT_ALIGNLEFT, CFG_COLOR_SPLIT_TIMES, GetPaletteAdrByColorIndex(23));
		}
		min=0;
		if((cur_list+1)*10<=res.total) max=10;
		else max=res.total%10;
	}
	else
	{
		min=(res.total-cur_list*10)-1;
		max=min+1;
	}
	for(int i=min; i<max; i++)
	{
		cur_num=cur_list*10+i;
		axes_x=(i<5) ? 0 : CFG_INDENT_X_SPLIT_TIMES;
		axes_y=(i<5) ? CFG_INDENT_Y_SPLIT_TIMES*i : CFG_INDENT_Y_SPLIT_TIMES*(i-5);
		x1=CFG_X_SPLIT_TIMES+axes_x;
		y1=CFG_Y_SPLIT_TIMES+axes_y;
		wsprintf(ws, "%02d:%02d:%02d.%02d", res.data[cur_num].hour, res.data[cur_num].min, res.data[cur_num].sec,
						res.data[cur_num].millisec);
		DrawString(ws, x1, y1, x1+get_ws_width(ws, font), y1+GetFontYSIZE(font), font, TEXT_ALIGNLEFT,
			CFG_COLOR_SPLIT_TIMES, GetPaletteAdrByColorIndex(23));
	}
	FreeWS(ws);
}

void DrawLoadingScreen(void)
{
	int y=
#ifdef ELKA
	24;
#else
	0;
#endif
	int x2=ScreenW()-1;
	int y2=ScreenH()-1;
	DrawRectangle(0, y, x2, y2, 0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(1));
	WSHDR *ws=AllocWS(32);
	wsprintf(ws, "%t", LGP[LGP_LOADING_SCREEN]);
	DrawString(ws, 0, (y2-GetFontYSIZE(FONT_MEDIUM)-y)/2, x2, ScreenH()-2, FONT_MEDIUM,
		TEXT_ALIGNMIDDLE, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(1));
	FreeWS(ws);
}

void DrawBuildCanvas(const int x,const int y,const int x2,const int y2)
{
	void *canvas=BuildCanvas();
	DrawCanvas(canvas, x, y, x2, y2, 1);
}

void DrawTimeOnIdle(void)
{
	extern const int CFG_SHOW_CUR_TIME_IDLE;
	extern const unsigned int CFG_X_CUR_TIME_IDLE, CFG_Y_CUR_TIME_IDLE;
	extern const char CFG_COLOR_CUR_TIME_IDLE[4];
	extern const int CFG_FONT_CUR_TIME_IDLE;
	
	if(CFG_SHOW_CUR_TIME_IDLE)
	{
		WSHDR *ws=AllocWS(12);
		wsprintf(ws, "%02d:%02d:%02d", time.hour, time.min, time.sec);
		int font=SelectFont(CFG_FONT_CUR_TIME_IDLE);
		int x2_time=CFG_X_CUR_TIME_IDLE+get_ws_width(ws, font);
		int y2_time=CFG_Y_CUR_TIME_IDLE+GetFontYSIZE(font);
		DrawBuildCanvas(CFG_X_CUR_TIME_IDLE, CFG_Y_CUR_TIME_IDLE, x2_time, y2_time);
		DrawString(ws, CFG_X_CUR_TIME_IDLE, CFG_Y_CUR_TIME_IDLE, x2_time, y2_time, font, TEXT_ALIGNLEFT,
			CFG_COLOR_CUR_TIME_IDLE, GetPaletteAdrByColorIndex(23));
		FreeWS(ws);
	}
}

inline int GetImgSize(const char *fname,const int type)
{
	enum {height, width};
	return (type==height) ? GetImgHeight((int)fname) : GetImgWidth((int)fname);
}

void DrawPicOnIdle(void)
{
	extern const int CFG_SHOW_ICONS_IDLE;
	extern const unsigned int CFG_X_ICONS_IDLE, CFG_Y_ICONS_IDLE;
	extern const char CFG_PATH_SKIN[128];
	
	if(CFG_SHOW_ICONS_IDLE)
	{
		char *fname_icon=malloc(strlen(CFG_PATH_SKIN)+20);
		if(IsStart())
			sprintf(fname_icon, "%sImg\\idle_start.png", CFG_PATH_SKIN);
		else
			sprintf(fname_icon, "%sImg\\idle_stop.png", CFG_PATH_SKIN);
		int width=GetImgSize(fname_icon, 1);
		int height=GetImgSize(fname_icon, 0);
		DrawBuildCanvas(CFG_X_ICONS_IDLE, CFG_Y_ICONS_IDLE,
			CFG_X_ICONS_IDLE+width, CFG_Y_ICONS_IDLE+height);
		DrawImg(CFG_X_ICONS_IDLE, CFG_Y_ICONS_IDLE, (int)fname_icon);
		mfree(fname_icon);
	}
}
