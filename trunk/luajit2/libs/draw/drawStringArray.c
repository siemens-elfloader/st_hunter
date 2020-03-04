
#include <swilib.h>



void __safe_ws2str(WSHDR *ws, const char *str, int len);
extern WSHDR *local_ws;






void ldrawStringArray(char **array, int array_size, int x, int y, int x2, int y2, int step, int font, int attr, char *pen, char *brush)
{
    int fontH = GetFontYSIZE(font);
    int ystep = y;
    for(int i=0; i<array_size; ++i)
    {
        __safe_ws2str(local_ws, array[i], 128);
        DrawScrollString(local_ws, x, ystep, x2, y2, 1, font, attr, pen, brush);
        ystep += fontH+step;
    }
}


