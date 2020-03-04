#include "mymenu.h"

int scrollStep=5;
int scrollCount=5;

int scroolWait=0;

bool
MenuItem::
Draw (Image *buffer, ft_font *font, int x, int y, int x2, int y2, bool active, bool press){
    bool need_redraw=0;
    int len=GetStringW (name_, font, UTF8);

    if (active && (len>x2-x) && !press){
        DrawScrollString_UTF8 (buffer, name_, font, x, y, x2, y2, scrollSlide, TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_ACT_MENU));

        if ((scrollSlide==0 || (len-scrollSlide<=x2-x) )&& scroolWait<scrollCount) scroolWait++;
        else{
            scroolWait=0;
            if (len-scrollSlide>x2-x) scrollSlide+=scrollStep;
            else scrollSlide=0;
        }
        return 1;
    }
    else{
        if (active) DrawScrollString_UTF8 (buffer, name_, font, x, y, x2, y2, 0, TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_ACT_MENU));
        else DrawScrollString_UTF8 (buffer, name_, font, x, y, x2, y2, 0, TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));
    }
    return 0;
}
