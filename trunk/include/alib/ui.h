#ifndef __ALIB_UI_H__
#define __ALIB_UI_H__

#include <alib/img.h>

#define MAX_NAME_LEN 64
#define MAX_COLORS 20


#define CFG_PANEL_SIZE_UP       24
#define CFG_PANEL_SIZE_DOWN     24
#define CFG_PANEL_SIZE_HEAD     22

#define CFG_SCROLL_W    6
#define CFG_HEADER_X    10
#define CFG_HEADER_Y    0
#define CFG_SOFTKEY_X   8

#define wallp_path "0:\\Zbin\\theme\\wallpaper.jpg"
#define theme_path "0:\\Zbin\\theme\\theme.acs"

enum theme_enum{
  CLR_BACKGROUND,
  CLR_PANEL_BEG,
  CLR_PANEL_END,

  CLR_HEADER_BEG,
  CLR_HEADER_END,

  CLR_SCROLL_BG_BEG,
  CLR_SCROLL_BG_END,

  CLR_SCROLL_BEG,
  CLR_SCROLL_END,

  CLR_CURSOR_BEG,
  CLR_CURSOR_END,

  CLR_FONT_HEADER,
  CLR_FONT_MENU,
  CLR_FONT_ACT_MENU,
  CLR_FONT_SOFT,

  PANEL_UP_SIZE,
  PANEL_DOWN_SIZE,
  PANEL_HEADER_SIZE
};

extern "C"{
void init_color_set ();
color32_t get_color_from_set (int id);
void set_color_to_set (int id, color32_t val);
void set_color_rgb32_to_set (int id, color32_t val);

char *UI_GetWallpaper ();
char *UI_GetTheme ();
void LoadTheme (char const *path);

void DrawBG (Image *buffer);
void DrawRectBG (Image *buffer, MyRECT rc);
void DrawUpPanel (Image *buffer);
void DrawDownPanel (Image *buffer);
void DrawHeaderPanel (Image *buffer);

void DrawHeaderText (Image *buffer, ft_font *font, char *headstr, int align);
void DrawSoftsText (Image *buffer, ft_font *font, char *left, char *right);
void DrawIndex (Image *buffer, ft_font *font, int cur, int max);
void DrawScroll (Image *buffer, int cur, int show, int max);

Image *GetWallpaper ();
Image *GetImgBuffer ();

//T_16COL scrW x scrH
unsigned char *GetWallpaperBitmap ();
unsigned char *GetImgBufferBitmap ();

//key
void OnKeyPress (int mess, int key, int IsLong, int DreamKey, void (*f)());
void OnKeyPress2 (int mess, int key, int DreamKey, void (*f1)(), void (*f2)());

}

class UI{
    char header_name[MAX_NAME_LEN];
    char left_soft_name[MAX_NAME_LEN];
    char right_soft_name[MAX_NAME_LEN];

    bool focus_;

public:
    UI (){
        focus_=0;
    }

    UI (const UI &obj) {

	};

    ~UI (){}

    bool IsFocus (){ return focus_;}
    void OnFocus (){ focus_=1;}
    void UnFocus (){ focus_=0;}

    void DrawHeader (Image *buffer, ft_font *font);
    void DrawSofts (Image *buffer, ft_font *font);

    void SetHeader (const char *name);
    void SetSofts (const char *name1, const char *name2);
};

#endif
