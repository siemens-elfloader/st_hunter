
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

#define DIRHANDLE "dir"


DIR_ENTRY *push_dir(lua_State *L, DIR_ENTRY *m)
{
    lua_boxpointer(L, m);
    luaL_getmetatable(L, DIRHANDLE);
    lua_setmetatable(L, -2);
    return m;
}


DIR_ENTRY* check_dir(lua_State *L, int index)
{
    DIR_ENTRY *im;
    if( !lua_isuserdata(L, index) ){
        luaL_error(L, "Given parameter omited");
        return 0;
    }
    im = (DIR_ENTRY*)lua_unboxpointer(L, index);
    return im;
}



int dir_findFirstFile(lua_State *L)
{
    const char *s = luaL_checkstring(L, 1);

    DIR_ENTRY * entr = (DIR_ENTRY*) malloc( (sizeof(DIR_ENTRY)) );

    int ret = FindFirstFile(entr, s, 0);
    if(!ret)
    {
        lua_pushnil(L);
        lua_pushnil(L);
        mfree(entr);
        return 2;
    }


    push_dir(L, entr);
    lua_pushinteger(L, (int)ret);
    return 2;
}



int dir_findNextFile(lua_State *L)
{
    DIR_ENTRY *dir = check_dir(L, 1);
    lua_pushinteger(L, FindNextFile(dir, 0));
    return 1;
}


static int dir_findClose (lua_State *L)
{
    DIR_ENTRY *dir = check_dir(L, 1);
    int ret = FindClose(dir, 0);
    lua_pushinteger(L, ret);
    return 1;
}


static int dir_name (lua_State *L)
{
    DIR_ENTRY* id = check_dir(L, 1);

    if(id)
    {
        char *s;
        int isdir = 0;
        if(id->file_attr & FA_DIRECTORY) {
            s = id->file_name;
            isdir = 1;
        }
        else
            s = id->file_name;

        lua_pushstring(L, s);
        lua_pushboolean(L, isdir);
        return 2;
    }
    else
    lua_pushnil(L);
    return 1;
}


static int dir_dirName (lua_State *L)
{
   DIR_ENTRY *dir = check_dir(L, 1);

    if(dir)
    {
        lua_pushstring(L, dir->folder_name);
        return 1;
    }
    else
        lua_pushnil(L);
    return 1;
}


static int dir_size (lua_State *L)
{
    DIR_ENTRY* id = check_dir(L, 1);

    if(id)
    {
        lua_pushnumber (L, id->file_size );
        return 1;
    }
    else
        lua_pushnil(L);
    return 1;
}


static int dir_attr (lua_State *L)
{
    DIR_ENTRY* id = check_dir(L, 1);

    if(id)
    {
        lua_pushinteger (L, id->file_attr );
        return 1;
    }
    else
        lua_pushnil(L);
    return 1;
}



static int dir_isDir (lua_State *L)
{
    DIR_ENTRY* id = check_dir(L, 1);
    if(id)
    {
        lua_pushboolean (L, id->file_attr & FA_DIRECTORY );
        return 1;
    }
    else
        lua_pushnil(L);
    return 1;
}



static int dir_gc (lua_State *L)
{
    DIR_ENTRY *dir = check_dir(L, 1);
    FindClose(dir, 0);
    free(dir);
    return 0;
}



static const luaL_Reg dir_lib[] =
{
    {"findFirstFile", dir_findFirstFile},
    {"findNextFile", dir_findNextFile},
    {"findClose", dir_findClose},
    {"name", dir_name},
    {"dirName", dir_dirName},
    {"size", dir_size},
    {"attr", dir_attr},
    {"isDir", dir_isDir},
    {NULL, NULL}
};


static const luaL_Reg dir_mlib[] =
{
    {"findNextFile", dir_findNextFile},
    {"findClose", dir_findClose},
    {"name", dir_name},
    {"dirName", dir_dirName},
    {"size", dir_size},
    {"attr", dir_attr},
    {"isDir", dir_isDir},
    {"__gc", dir_gc},
    {NULL, NULL}
};



int csm_register (lua_State *L)
{
    luaL_newmetatable(L, DIRHANDLE);  /* create metatable for file handles */
    lua_pushvalue(L, -1);  /* push metatable */
    lua_setfield(L, -2, "__index");  /* metatable.__index = metatable */
    luaL_register(L, NULL, dir_mlib);  /* file methods */

    return 1;
}

LUALIB_API int luaopen_dir (lua_State *L)
{
    csm_register(L);
    luaL_register(L, DIRHANDLE, dir_lib);
    return 1;
}



