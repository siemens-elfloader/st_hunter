#include "mymenu.h"
#include "radio.h"

int scrollStep=5;
int scrollCount=5;

int scroolWait=0;

bool
RadioItem::
Draw (Image *buffer, ft_font *font, int x, int y, int x2, int y2, bool active, bool press){
    bool need_redraw=0;
    //int len=GetStringW (name_, font, UTF8);
    int fontH=GetFontH (font);
    char text_buf[16];
    freq2str (text_buf, freq_);

    if (active){
        DrawScrollString_ASCII (buffer, name_, font, x, y, x2, y2, 0, TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_ACT_MENU));
        DrawScrollString_ASCII (buffer, text_buf, font, x, y, x2-10, y2, 0, TEXT_ALIGNRIGHT, get_color_from_set(CLR_FONT_ACT_MENU));
    }
    else{
        DrawScrollString_ASCII (buffer, name_, font, x, y, x2, y2, 0, TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));
        DrawScrollString_ASCII (buffer, text_buf, font, x, y, x2-10, y2, 0, TEXT_ALIGNRIGHT, get_color_from_set(CLR_FONT_MENU));
    }

    return 0;
}
