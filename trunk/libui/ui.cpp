#include <alib\ui.h>
#include <alib\font.h>

#define CFG_PANEL_SIZE_UP 24
#define CFG_PANEL_SIZE_DOWN 24
#define CFG_PANEL_SIZE_HEAD 22

color32_t color_set[MAX_COLORS];

color32_t get_color_from_set (int id){
    return color_set[id];
}

void set_color_to_set (int id, color32_t val){
    color_set[id]=val;
}

//rgba -> argb
void set_color_rgb32_to_set (int id, color32_t val){
    uint8_t r=(GetA(val));
    uint8_t g=(GetR(val));
    uint8_t b=(GetG(val));
    uint8_t a=(GetB(val));

    color_set[id]=rgb_rgb32 (r, g, b, a);
}

void init_color_set (){
    color_set[CLR_BACKGROUND]=rgb_rgb32 (90, 100, 110,255);

    color_set[CLR_PANEL_BEG]=rgb_rgb32 (100, 115, 130,180);
    color_set[CLR_PANEL_END]=rgb_rgb32 (40, 50, 60,180);

    color_set[CLR_HEADER_BEG]=rgb_rgb32 (120, 130, 140,180);
    color_set[CLR_HEADER_END]=rgb_rgb32 (40, 50, 60,180);

    color_set[CLR_SCROLL_BG_BEG]=rgb_rgb32 (120, 130, 140,180);
    color_set[CLR_SCROLL_BG_END]=rgb_rgb32 (200, 210, 220,180);

    color_set[CLR_SCROLL_BEG]=rgb_rgb32 (30, 40, 50,255);
    color_set[CLR_SCROLL_END]=rgb_rgb32 (80, 90, 100,255);

    color_set[CLR_CURSOR_BEG]=rgb_rgb32 (120, 130, 140,180);
    color_set[CLR_CURSOR_END]=rgb_rgb32 (40, 50, 60,180);

    color_set[CLR_FONT_HEADER]=0xFFFFFFFF;
    color_set[CLR_FONT_MENU]=0xFFFFFFFF;
    color_set[CLR_FONT_ACT_MENU]=0xFFFF00FF;
    color_set[CLR_FONT_SOFT]=0xFFFFFFFF;

    color_set[PANEL_UP_SIZE]=CFG_PANEL_SIZE_UP;
    color_set[PANEL_DOWN_SIZE]=CFG_PANEL_SIZE_DOWN;
    color_set[PANEL_HEADER_SIZE]=CFG_PANEL_SIZE_HEAD;


    LoadTheme (UI_GetTheme());
}


char *UI_GetWallpaper (){
    return wallp_path;
}

char *UI_GetTheme (){
    return theme_path;
}

Image img_bg;
bool img_bg_load=0;

Image img_buffer;
bool img_buffer_load=0;

void DrawBG (Image *buffer){
    Image *bg=GetWallpaper ();
    if (bg){
        if (bg->GetW()!=0){
            buffer->DrawLayer (bg, 0, 0);
            return;
        }
    }
    buffer->DrawFillRect (0, 0, buffer->GetW(), buffer->GetH(), color_set[CLR_BACKGROUND]);
}

void DrawRectBG (Image *buffer, MyRECT rc){
    Image *bg=GetWallpaper ();
    if (bg){
        if (bg->GetW()!=0){
            buffer->DrawLayerRect (bg, 0, 0, rc);
            return;
        }
    }
    buffer->DrawFillRect (rc.x, rc.y, rc.x2, rc.y2, color_set[CLR_BACKGROUND]);
}

void DrawUpPanel (Image *buffer){
    buffer->DrawGradient (0, 0, buffer->GetW(), color_set[PANEL_UP_SIZE], color_set[CLR_PANEL_BEG], color_set[CLR_PANEL_END], 0);
}
void DrawDownPanel (Image *buffer){
    buffer->DrawGradient (0, buffer->GetH()-color_set[PANEL_DOWN_SIZE], buffer->GetW(), buffer->GetH(),  color_set[CLR_PANEL_BEG], color_set[CLR_PANEL_END], 0);
}


void DrawHeaderText (Image *buffer, ft_font *font, char *headstr, int align){
    int FontH=GetFontH (font);
    DrawString_UTF8 (buffer, headstr, font, CFG_HEADER_X, color_set[PANEL_UP_SIZE]+CFG_HEADER_Y+((color_set[PANEL_HEADER_SIZE]-FontH)/2), buffer->GetW(), color_set[PANEL_UP_SIZE]+CFG_HEADER_Y+((color_set[PANEL_HEADER_SIZE]-FontH)/2)+FontH, align, color_set[CLR_FONT_HEADER]);
}

void DrawHeaderPanel (Image *buffer){
    buffer->DrawGradient (0, color_set[PANEL_UP_SIZE]+CFG_HEADER_Y, buffer->GetW(), color_set[PANEL_UP_SIZE]+CFG_HEADER_Y+color_set[PANEL_HEADER_SIZE], color_set[CLR_HEADER_BEG], color_set[CLR_HEADER_END], 0);
}

void DrawSoftsText (Image *buffer, ft_font *font, char *left, char *right){
    int FontH=GetFontH (font);
    int y=buffer->GetH()-color_set[PANEL_DOWN_SIZE]+((color_set[PANEL_DOWN_SIZE]-FontH)/2);
    DrawString_UTF8 (buffer, left, font, CFG_SOFTKEY_X, y, (buffer->GetW())>>1, y+FontH, TEXT_ALIGNLEFT, color_set[CLR_FONT_SOFT]);
    DrawString_UTF8 (buffer, right, font, (buffer->GetW())>>1, y, buffer->GetW()-CFG_SOFTKEY_X, y+FontH, TEXT_ALIGNRIGHT, color_set[CLR_FONT_SOFT]);
}

void DrawIndex (Image *buffer, ft_font *font, int cur, int max){
    int FontH=GetFontH (font);
    char str[16];
    sprintf (str, "%d/%d", cur+1, max);
    DrawString_UTF8 (buffer, str, font, CFG_HEADER_X, color_set[PANEL_UP_SIZE]+CFG_HEADER_Y+(color_set[PANEL_HEADER_SIZE]-FontH)/2, buffer->GetW()-CFG_HEADER_X, color_set[PANEL_UP_SIZE]+CFG_HEADER_Y+(color_set[PANEL_HEADER_SIZE]-FontH)/2+FontH, TEXT_ALIGNRIGHT, color_set[CLR_FONT_HEADER]);
}

void DrawScroll (Image *buffer, int cur, int show, int max){
    int w=buffer->GetW()-1;
    int scroll_beg=(color_set[PANEL_UP_SIZE]+CFG_HEADER_Y+color_set[PANEL_HEADER_SIZE]);
    int scroll_end=buffer->GetH()-color_set[PANEL_DOWN_SIZE]-1;
    int scroll_w=CFG_SCROLL_W;

    buffer->DrawGradient (w-scroll_w, scroll_beg, w-(scroll_w>>1), scroll_end, color_set[CLR_SCROLL_BG_BEG], color_set[CLR_SCROLL_BG_END], 1);
    buffer->DrawGradient (w-(scroll_w>>1), scroll_beg, w, scroll_end, color_set[CLR_SCROLL_BG_END], color_set[CLR_SCROLL_BG_BEG], 1);

    int len=(scroll_end-scroll_beg)*show/max;
    int beg=(scroll_end-scroll_beg-len)*cur/(max-1);

    buffer->DrawGradient (w-scroll_w, scroll_beg+beg, w-(scroll_w>>1), scroll_beg+beg+len, color_set[CLR_SCROLL_BEG], color_set[CLR_SCROLL_END], 1);
    buffer->DrawGradient (w-(scroll_w>>1), scroll_beg+beg, w, scroll_beg+beg+len, color_set[CLR_SCROLL_END], color_set[CLR_SCROLL_BEG], 1);
}

void
UI::DrawHeader (Image *buffer, ft_font *font){
    DrawHeaderText (buffer, font, header_name, TEXT_ALIGNLEFT);
}

void
UI::DrawSofts (Image *buffer, ft_font *font){
    DrawSoftsText (buffer, font, left_soft_name, right_soft_name);
}

void
UI::SetHeader (const char *name){
    strcpy (header_name, name);
}

void
UI::SetSofts (const char *name1, const char *name2){
    strcpy (left_soft_name, name1);
    strcpy (right_soft_name, name2);
}

#define MAX_EXT 8
char *GetFileExt (char const *fname){
  char *s1, *s2;
  s1=strrchr(fname,'.');
  if (s1){
    int i=0;
    s2=++s1;
    while(*s2++) i++;
    if (i>MAX_EXT) s1=0;
  }
  return (s1);
}

#define PNG_EXT "PNG"
#define JPG_EXT "JPG"
#define JPEG_EXT "JPEG"

#define UNK_T   0
#define PNG_T   1
#define JPG_T   2
#define JPEG_T  3

#define ERR_UNSUPPORTED_IMAGE_FORMAT -10

int GetImageType (char const *fname){
    char *ext=GetFileExt (fname);
    if (!strcmpi(PNG_EXT, ext)) return PNG_T;
    else{
        if (!strcmpi(JPG_EXT, ext)) return JPG_T;
        else if (!strcmpi(JPEG_EXT, ext)) return JPEG_T;
    }

    return UNK_T;
}

int LoadWallpaper (char const *fname){
    int err=0;
    int type=GetImageType (fname);

    switch (type){
        case PNG_T:
        err=img_bg.CreateFromPNG (fname);
        break;

        case JPG_T:
        case JPEG_T:
        err=img_bg.CreateFromJPEG (fname);
        break;

        default: return ERR_UNSUPPORTED_IMAGE_FORMAT;
    }

    if (err) return -1;
    img_bg_load=1;

    return 0;
}
Image *GetWallpaper (){
    if (!img_bg_load){
        char *wallpaper=UI_GetWallpaper ();
        if (LoadWallpaper (wallpaper)) return 0;
    }
    return &img_bg;
}

unsigned char *GetWallpaperBitmap (){
    if (!img_bg_load){
        char *wallpaper=UI_GetWallpaper ();
        if (LoadWallpaper (wallpaper)) return 0;
    }
    unsigned char *bitmap=*img_bg.GetBitmap ();
    return bitmap;
}

int CreateImgBuffer (){
    int scrH=ScreenH ();
    int scrW=ScreenW ();
    if (!img_buffer.Create (scrW, scrH, T_16COL)) img_buffer_load=1;
    else return -1;

    return 0;
}

Image *GetImgBuffer (){
    if (!img_buffer_load){
        if (CreateImgBuffer ()) return 0;
    }
    return &img_buffer;
}

unsigned char *GetImgBufferBitmap (){
    if (!img_buffer_load){
        if (CreateImgBuffer ()) return 0;
    }
    unsigned char *bitmap=*img_buffer.GetBitmap ();
    return bitmap;
}
