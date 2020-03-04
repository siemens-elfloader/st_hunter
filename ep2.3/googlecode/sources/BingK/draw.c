#include "..\inc\swilib.h"
#include "conf_loader.h"

extern unsigned int pos_x;
extern unsigned int pos_y;
extern unsigned int font;
extern unsigned int txt_attr;
extern char word_color[4];
extern char word_frm_color[4];
extern char rnd_frm_color[4];
extern char frm_color[4];

void memcpy_n(char *s1, char *s2, int len)
{
	int i;
	for(i=0;i<len;i++)
		s1[i]=s2[i];
}

void draw(void)
{
	unsigned int x1=131;
	unsigned int y1=0x02;
	unsigned int xfont=FONT_SMALL;
	unsigned int xtxt_attr=TEXT_ALIGNMIDDLE+TEXT_OUTLINE;
	char xword_color[4]={0xFF, 0xFF, 0xFF, 100};
	char xword_frm_color[4]={0, 0, 0, 100};
	char xrnd_frm_color[4]={0xFF, 0xFF, 0xFF, 100};
	char xfrm_color[4]={0, 0, 0, 100};
	if(InitConfig()>0)
	{
		x1=pos_x;
		y1=pos_y;
		xfont=font;
		xtxt_attr=txt_attr;
		memcpy_n(xword_color,word_color,4);
		memcpy_n(xword_frm_color,word_frm_color,4);
		memcpy_n(xrnd_frm_color,rnd_frm_color,4);
		memcpy_n(xfrm_color,frm_color,4);
	}
	
	int traf=*GetGPRSTrafficPointer();
	WSHDR *ws=AllocWS(32);
	wsprintf(ws, "%dKB", traf/1024);
	int fh=GetFontYSIZE(font);
	int width=Get_WS_width(ws, font)+0x10;
	DrawRoundedFrame(x1, y1, x1+width+2, y1+fh+2, 2, 2, 0, xrnd_frm_color, xfrm_color);
	DrawString(ws, x1+1, y1+1, x1+width+1, y1+fh+1, xfont, xtxt_attr, xword_color, xword_frm_color);
	FreeWS(ws);
}
