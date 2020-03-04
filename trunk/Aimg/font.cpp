#include <alib/img.h>
#include <alib/io.h>
#include <math.h>
#include <alib/freetype.h>

int GetFontH (ft_font *ftf){
    return ftf->fti->h*1.1;
}

int FT_GetLetterW (ft_font *ftf, int num){
    if (!ftf )  return 0;

    fte_symbol *fte_s = fte_get_symbol(ftf->fti, num);

    if(fte_s == 0) return -1;

    return fte_s->xadvance;
}


int FT_GetStringW (wchar_t *str, ft_font *font){
	if (!font || !str) return 0;

	int str_w=0;

	for (int i=0; str[i]!='\0'; i++){
		int cur=str[i];
        int w=FT_GetLetterW (font, cur);
        if (w>0) str_w+=w;
	}

	return str_w;
}


int GetStringW (char *str, ft_font *font){
	if (!font || !str) return 0;

	int len=strlen (str);
	wchar_t *wstr=new wchar_t [len+1];

	DecodeString (wstr, str);

	int w=FT_GetStringW (wstr, font);

	delete wstr; wstr=NULL;

	return w;
}

int
AIMG::
FT_DrawLetter (ft_font *ftf, int num, int x, int y, MyRECT rc, color clr){
    if (!ftf )  return -1;

    fte_symbol *fte_s = fte_get_symbol(ftf->fti, num);

    if(fte_s == 0) return -2;
    if(!fte_s->bitmap) return fte_s->xadvance;

	int nx=x+fte_s->left;
	int ny=y-fte_s->top+ftf->fti->h*0.8; //ftf->fti->h FontH

	for (int j=0; j<fte_s->height; j++){
		for (int i=0; i<fte_s->width; i++){

			if (nx+i>=0 && nx+i<w_ && ny+j>=0 && ny+j<h_){
			    if (nx+i>=rc.x && nx+i<rc.x2 && ny+j>=rc.y && ny+j<rc.y2){
                    clr.A=fte_s->bitmap[i + fte_s->width*j];
                    SetColor (nx+i, ny+j, clr);
                }
			}
		}
	}

	return fte_s->xadvance;
}


int
AIMG::
FT_DrawScrollString (wchar_t *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int TEXT_ALIGN, color clr){
	if (!font || !str) return -1;
    int w=0;
	int xy_cur=x1-slide;

	if (TEXT_ALIGN&TEXT_ALIGNMIDDLE || TEXT_ALIGN&TEXT_ALIGNRIGHT) w=FT_GetStringW (str, font);

    if (TEXT_ALIGN&TEXT_ALIGNMIDDLE){ xy_cur+=((x2-x1)-w)/2;}
    if (TEXT_ALIGN&TEXT_ALIGNRIGHT){ xy_cur+=((x2-x1)-w);}

	for (int i=0; str[i]!='\0'; i++){
		int cur=str[i];
        MyRECT rc;
        rc.x=x1;
        rc.x2=x2;
        rc.y=y1;
        rc.y2=y2;
        xy_cur+=FT_DrawLetter (font, cur, xy_cur, y1, rc, clr);
	}

	return 0;
}

int
AIMG::
DrawScrollString (char *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int TEXT_ALIGN, color clr){
	if (!font || !str) return -1;

    int len=strlen (str);
	wchar_t *wstr=new wchar_t [len+1];

    DecodeString (wstr, str);

    FT_DrawScrollString (wstr, font, x1, y1, x2, y2, slide, TEXT_ALIGN, clr);

    delete wstr; wstr=NULL;

    return 0;
}

wchar_t buf_str[512];
int
AIMG::
DrawString (char *str, ft_font *font, int x1, int y1, int x2, int y2, int TEXT_ALIGN, color clr){
	if (!font || !str) return -1;

    int len=strlen (str);
	wchar_t *wstr=new wchar_t [len+1];

    DecodeString (wstr, str);

    int ycur=0;
    int s=0;
	for (int i=0; wstr[i]!='\0'; i++){
        if (wstr[i]=='\n'){
            buf_str[s]='\0';
            FT_DrawScrollString (buf_str, font, x1, y1+ycur, x2, y2+ycur, 0, TEXT_ALIGN, clr);
            s=0; buf_str[s]='\0';
            ycur+=GetFontH (font);
            i++;
        }
        buf_str[s]=wstr[i]; s++;
	}
	buf_str[s]='\0';
    FT_DrawScrollString (buf_str, font, x1, y1+ycur, x2, y2+ycur, 0, TEXT_ALIGN, clr);

    delete wstr; wstr=NULL;

    return 0;
}

