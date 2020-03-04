
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

GBSTMR test;


void stop_timer(unsigned int tmr)
{
    GBS_DelTimer((GBSTMR *)tmr);
}


unsigned int start_timer(void (*call)(), int ticks)
{
    GBS_StartTimerProc(&test, ticks, (void (*)(GBSTMR *))call);
    return (unsigned int)&test;
}



unsigned char getCurrentSeconds()
{
    TTime t;
    GetDateTime(0, &t);
    return t.sec;
}
