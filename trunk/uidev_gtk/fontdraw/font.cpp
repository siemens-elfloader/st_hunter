#include <alib/font.h>
#include <alib/io.h>
#include <alib/freetype.h>

int GetFontH (ft_font *ftf){

    //return ftf->fti->h*1.1;

    return ftf->fti->height;
}

int FT_GetLetterW (ft_font *ftf, int num){
    if (!ftf)  return 0;

    fte_symbol *fte_s = fte_get_symbol(ftf->fti, num);

    if (fte_s == 0) return -1;

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


int GetStringW (char *str, ft_font *font, int charmap){
	if (!font || !str) return 0;

	int len=strlen (str);
	wchar_t *wstr=new wchar_t [len+1];

	DecodeString (wstr, str, charmap);

	int w=FT_GetStringW (wstr, font);

	delete wstr; wstr=NULL;

	return w;
}
int FT_DrawLetter (Image *img, ft_font *ftf, int num, int x, int y, MyRECT *rc, color32_t clr, bool rot){
    if (!ftf )  return -1;

    fte_symbol *fte_s = fte_get_symbol(ftf->fti, num);

    if(fte_s == 0) return -2;
    if(!fte_s->bitmap) return fte_s->xadvance;

    int w=img->GetW();
	int h=img->GetH();

    int nx, ny;

    if (!rot){
        nx=x+fte_s->left;
        ny=y-fte_s->top+ftf->fti->ascender;
    }else{
        ny=y+fte_s->left;
        nx=w-(x-fte_s->top+fte_s->width+ftf->fti->height)-1;
        rc->x=w-(rc->x)-1;
        rc->x2=w-(rc->x2+fte_s->width)-1;
    }

    uint8_t a = (GetA(clr));

    switch (rot){
        case 0:{
            for (int j=0; j<fte_s->height; j++){
                for (int i=0; i<fte_s->width; i++){
                    if (nx+i>=0 && nx+i<w && ny+j>=0 && ny+j<h){
                        if (nx+i>=rc->x && nx+i<rc->x2 && ny+j>=rc->y && ny+j<rc->y2){
                            a=fte_s->bitmap[i + fte_s->width*j];
                            color32_t c=rgb_rgb32 ((GetR(clr)), (GetG(clr)), (GetB(clr)), a);
                            img->SetColor (nx+i, ny+j, c);
                        }
                    }
                }
            }
        }break;

        case 1:{
            for (int j=0; j<fte_s->height; j++){
                for (int i=0; i<fte_s->width; i++){
                    if (nx+i>=0 && nx+i<w && ny+j>=0 && ny+j<h){
                        if ((nx+fte_s->width-j-1)<=rc->x && (nx+fte_s->width-j-1)>rc->x2 && ny+i>=rc->y && ny+i<rc->y2){
                            a=fte_s->bitmap[i + fte_s->width*j];
                            color32_t c=rgb_rgb32 ((GetR(clr)), (GetG(clr)), (GetB(clr)), a);
                            img->SetColor (nx+fte_s->width-j-1, ny+i, c);
                        }
                    }
                }
            }
        }break;
    }

	return fte_s->xadvance;
}


int FT_DrawScrollString (Image *img, wchar_t *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int align, color32_t clr){
	if (!font || !str) return -1;
	bool rot=0;
    int w=0;
	int xy_cur=x1-slide;

	if (align&TEXT_ALIGNMIDDLE || align&TEXT_ALIGNRIGHT) w=FT_GetStringW (str, font);

    if (align&TEXT_ALIGNMIDDLE){ xy_cur+=((x2-x1)-w)/2;}
    if (align&TEXT_ALIGNRIGHT){ xy_cur+=((x2-x1)-w);}
    if (align&TEXT_ROTATE) rot=1;

	for (int i=0; str[i]!='\0'; i++){
		int cur=str[i];
        MyRECT rc;
        if (!rot){
            rc.x=x1;
            rc.x2=x2;
            rc.y=y1;
            rc.y2=y2;
            xy_cur+=FT_DrawLetter (img, font, cur, xy_cur, y1, &rc, clr);
        }
        else{
            rc.x=y1;
            rc.x2=y2;
            rc.y=x1;
            rc.y2=x2;
            xy_cur+=FT_DrawLetter (img, font, cur, y1, xy_cur, &rc, clr, 1);
        }
	}

	return 0;
}

int DrawScrollString (Image *img, char *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int align, color32_t clr, int charmap){
	if (!font || !str) return -1;

    int len=strlen (str);
	wchar_t *wstr=new wchar_t [len+1];

    DecodeString (wstr, str, charmap);

    FT_DrawScrollString (img, wstr, font, x1, y1, x2, y2, slide, align, clr);

    delete wstr; wstr=NULL;

    return 0;
}

#define MAX_CHARACTERS 512
wchar_t buf_str[MAX_CHARACTERS];
int DrawString (Image *img, char *str, ft_font *font, int x1, int y1, int x2, int y2, int align, color32_t clr, int charmap){
	if (!font || !str) return -1;

    int len=strlen (str);
	wchar_t *wstr=new wchar_t [len+1];

    DecodeString (wstr, str, charmap);

    int ycur=0;
    int s=0;
	for (int i=0; wstr[i]!='\0'; i++){
        if (wstr[i]=='\n'){
            buf_str[s]='\0';
            FT_DrawScrollString (img, buf_str, font, x1, y1+ycur, x2, y2+ycur, 0, align, clr);
            s=0; buf_str[s]='\0';
            ycur+=GetFontH (font);
            i++;
        }
        buf_str[s]=wstr[i]; s++;

        if (s>=(MAX_CHARACTERS-1)) break;
	}
	buf_str[s]='\0';
    FT_DrawScrollString (img, buf_str, font, x1, y1+ycur, x2, y2+ycur, 0, align, clr);

    delete wstr; wstr=NULL;

    return 0;
}


