
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



int clock_time(lua_State *L)
{
    TTime tm;
    GetDateTime(0, &tm);

    luaL_newmetatable(L, "time");

    lua_pushinteger(L, tm.sec);
    lua_setfield(L, -2, "sec");

    lua_pushinteger(L, tm.min);
    lua_setfield(L, -2, "min");

    lua_pushinteger(L, tm.hour);
    lua_setfield(L, -2, "hour");

    return 1;
}


int clock_date(lua_State *L)
{
    TDate td;
    GetDateTime(&td, 0);

    luaL_newmetatable(L, "time");

    lua_pushinteger(L, td.day);
    lua_setfield(L, -2, "day");

    lua_pushinteger(L, td.month);
    lua_setfield(L, -2, "month");

    lua_pushinteger(L, td.year);
    lua_setfield(L, -2, "year");

    return 1;
}



static const luaL_Reg clock_lib[] =
{
    {"time", clock_time},
    {"date", clock_date},
    {NULL, NULL}
};



LUALIB_API int luaopen_clock (lua_State *L)
{
    luaL_register(L, "clock", clock_lib);
    return 1;
}



