
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

#include "subproc.c"



static const luaL_Reg core_lib[] =
{
    {"subproc", core_subproc},
    {NULL, NULL}
};



LUALIB_API int luaopen_core(lua_State *L)
{
    luaL_register(L, "core", core_lib);
    return 1;
}



