
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

#define GBSTMR_HANDLE "timer"

GBSTMR *push_timer(lua_State *L, GBSTMR *m)
{
    lua_boxpointer(L, m);
    luaL_getmetatable(L, GBSTMR_HANDLE);
    lua_setmetatable(L, -2);
    return m;
}


GBSTMR* check_timer(lua_State *L, int index)
{
    GBSTMR *im;
    if( !lua_isuserdata(L, index) ){
        luaL_error(L, "Given parameter omited");
        return 0;
    }
    im = (GBSTMR*)lua_unboxpointer(L, index);
    return im;
}



void __lua_timer_proc(GBSTMR *tmr)
{
    lua_State *L = (lua_State*)tmr->param5;
    if( mluaState(L)->is_aborting == 1 ) return;

    lua_rawgeti (L, LUA_REGISTRYINDEX, tmr->param4);

    GBS_DelTimer(tmr);

    int status;
    if( (status = docall(L, 0, 0)) )
    {
        printf("Call error(timer)\n");
        mluaState(L)->abort(status);
    }
}



static int timer_start (lua_State *L)
{
    int ms = luaL_checkint(L, 2);
    luaL_checktype(L, 1, LUA_TFUNCTION);

    GBSTMR *timer = malloc(sizeof(GBSTMR));

    /* ref it */
    lua_pushvalue(L, 1);
    timer->param4 = luaL_ref (L, LUA_REGISTRYINDEX);
    timer->param5 = (int)L;

    GBS_StartTimerProc(timer, ms, ( void (*) (GBSTMR *) )__lua_timer_proc);
    push_timer(L, timer);
    return 1;
}



static int timer_stop (lua_State *L)
{
    GBSTMR *timer = check_timer(L, 1);
    if(timer) {
        GBS_DelTimer( (GBSTMR*)timer );
        return 0;
    }

    fprintf(stderr, "Warning: given a ZERO value of timer struct\n");
    lua_pushnil(L);
    return 1;
}



static int timer_gc (lua_State *L)
{
    GBSTMR *timer = check_timer(L, 1);
    GBS_DelTimer( (GBSTMR*)timer );
    memset(timer, 0, sizeof(GBSTMR));
    free(timer);
    return 0;
}



static const luaL_Reg timer_lib[] =
{
    {"start", timer_start},
    {"stop", timer_stop},
    {NULL, NULL}
};



static const luaL_Reg timer_mlib[] =
{
    {"stop", timer_stop},
    {"__gc", timer_gc},
    {NULL, NULL}
};



int timer_register (lua_State *L)
{
    luaL_newmetatable(L, GBSTMR_HANDLE);  /* create metatable for file handles */
    lua_pushvalue(L, -1);  /* push metatable */
    lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
    luaL_register(L, NULL, timer_mlib);  /* file methods */

    return 1;
}


LUALIB_API int luaopen_timer (lua_State *L)
{
    timer_register(L);
    luaL_register(L, GBSTMR_HANDLE, timer_lib);
    return 1;
}


