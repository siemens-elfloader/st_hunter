
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <swilib.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <luajit.h>
#include <lj_lib.h>
#include <lj_arch.h>
#include <my_lua.h>
#include <stdarg.h>

WSHDR *local_ws = 0;
MUTEX *mutex;

__attribute__((constructor))
void __init()
{

    mutex = malloc(sizeof(MUTEX));
    MutexCreate(mutex);
    local_ws = AllocWS(2048);
}


__attribute__((destructor))
void __fini()
{
    MutexDestroy(mutex);
    free(mutex);
    FreeWS(local_ws);
}


void __safe_ws2str(WSHDR *ws, const char *str, int len)
{
    MutexLock(mutex);
    str_2ws(ws, str, len);
    MutexUnlock(mutex);
}


void ldrawLine(int x, int y, int x2, int y2, int type, const char *pen)
{
    DrawLine(x, y, x2, y2, type, pen);
}


void ldrawRectangle(int x, int y, int x2, int y2, int flags, const char *pen, const char *brush)
{
    DrawRectangle(x, y, x2, y2, flags, pen, brush);
}


void ldrawRoundedFrame(int x1, int y1, int x2, int y2, int x_round, int y_round, int flags, const char *pen, const char *brush)
{
    DrawRoundedFrame(x1, y1, x2, y2, x_round, y_round, flags, pen, brush);
}


void ldrawArc(int x1, int y1, int x2, int y2, int a1, int a2, int flag, char *pen, char *brush)
{
    drawArc(x1, y1, x2, y2, a1, a2, flag, pen, brush);
}



void ldrawImg(unsigned int x, unsigned int y, unsigned int picture)
{
    DrawImg(x, y, picture);
}



void ldrawCanvas(void *data, int x1, int y1, int x2, int y2, int flag_one)
{
    DrawCanvas(data, x1, y1, x2, y2, flag_one);
}



void ldrawScrollString(const char *text, int len, int x1, int y1, int x2, int y2, int xdisp, int font,
                       int attr, const char *pen, const char *brush)
{
    __safe_ws2str(local_ws, text, len);
    DrawScrollString(local_ws, x1, y1, x2, y2, xdisp, font, attr, pen, brush);
}



void ldrawString(const char *text, int len, int x1, int y1, int x2, int y2,
                int font, int attr, const char *pen, const char *brush)
{
    __safe_ws2str(local_ws, text, len);
    DrawString(local_ws, x1, y1, x2, y2, font, attr, pen, brush);
}


void lsetPixel(int x,int y, const char *color)
{
    SetPixel(x, y, color);
}


void ldirectRedrawGUI (void)
{
    DirectRedrawGUI();
}


void lredraw()
{
    REDRAW();
}


int lstrW(const char *str, int strl, int font)
{
    __safe_ws2str(local_ws, str, strl);
    return Get_WS_width(local_ws, font);
}


int lgetFontH(int font)
{
    return GetFontYSIZE(font);
}


void *lbuildCanvas()
{
    return BuildCanvas();
}







