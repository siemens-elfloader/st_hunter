#ifndef __FONT_H__
#define __FONT_H__

#include <alib/img.h>

//Fonts
#define UTF16 0
#define UTF8 1
#define ASCII 2


#define TEXT_ALIGNLEFT 1
#define TEXT_ALIGNMIDDLE 2
#define TEXT_ALIGNRIGHT 4

#define TEXT_ROTATE 8192

int GetFontH (ft_font *ftf);

//UTF16
int FT_DrawLetter (Image *img, ft_font *ftf, int num, int x, int y, MyRECT *rc, color32_t clr, bool rot=0);
int FT_DrawScrollString (Image *img, wchar_t *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int align, color32_t clr);

int FT_GetLetterW (ft_font *ftf, int num);
int FT_GetStringW (wchar_t *str, ft_font *font);

//With charmap
int DrawScrollString (Image *img, char *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int align, color32_t clr, int charmap);
int DrawString (Image *img, char *str, ft_font *font, int x1, int y1, int x2, int y2, int align, color32_t clr, int charmap);

int GetStringW (char *str, ft_font *font, int charmap);

void DecodeString (wchar_t *wstr, char *str, int charmap);

#define DrawString_UTF8(...) DrawString(__VA_ARGS__, UTF8)
#define DrawString_UTF16(...) DrawString(__VA_ARGS__, UTF16)
#define DrawString_ASCII(...) DrawString(__VA_ARGS__, ASCII)

#define DrawScrollString_UTF8(...) DrawScrollString(__VA_ARGS__, UTF8)
#define DrawScrollString_UTF16(...) DrawScrollString(__VA_ARGS__, UTF16)
#define DrawScrollString_ASCII(...) DrawScrollString(__VA_ARGS__, ASCII)

#endif
