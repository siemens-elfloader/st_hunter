
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

#define __def_noinline_noret(id, ret, ...)\
    ((ret (*)())((__sys_switab_addres[id])))(__VA_ARGS__);

#define __def_noinline_noret_ram(id, ret)\
     (ret)(__sys_switab_addres[id-0x8000]);


int toaddress(lua_State *L)
{
    void *u = lua_touserdata(L, 1);
    lua_pushnumber(L, (unsigned long)u);
    return 1;
}


int swicall(lua_State *L)
{
    int swinum = luaL_checkint(L, 1);
    const char *t = luaL_checkstring(L, 2);

    void *datas[20];
    void *ret = 0;
    int i = 0, type;

    if(swinum >= 0x8000 ){
        goto ram_func;
    }

    for( i = 3; i < 17; ++i)
    {
        if( (type = lua_type(L, i)) == LUA_TNONE ) break;

        switch(type)
        {
        case  LUA_TNIL:
            //printf("LUA_TNIL\n");
            datas[i-3] = 0;
            break;

        case  LUA_TBOOLEAN:
            //printf("LUA_TBOOLEAN\n");
            datas[i-3] = (void*)lua_toboolean(L, i);
            printf("LUA_TBOOLEAN(%d) %X \n", i-3, datas[i-3]);
            break;

        case LUA_TNUMBER:
            //printf("LUA_TNUMBER\n");
            datas[i-3] = (void*)lua_tointeger(L, i);
            printf("LUA_TNUMBER(%d) %X \n", i-3, datas[i-3]);
            break;

        case  LUA_TSTRING:
            //printf("LUA_TSTRING\n");
            datas[i-3] = (void*)lua_tostring(L, i);
            printf("LUA_TSTRING(%d) %X \n", i-3, datas[i-3]);
            break;

        case LUA_TUSERDATA:
            //printf("LUA_TUSERDATA\n");
            datas[i-3] = (void*)lua_topointer(L, i);
            printf("LUA_TUSERDATA(%d) %X \n", i-3, datas[i-3]);
            break;

        case LUA_TCDATA:
            //printf("10 type oO\n");
            datas[i-3] = (void*)lua_topointer(L, i);
            printf("LUA_TCDATA(%d) %X \n", i-3, datas[i-3]);
            break;

        case LUA_TLIGHTUSERDATA:
            datas[i-3] = (void*)lua_touserdata(L, i);
            printf("LUA_TLIGHTUSERDATA(%d) %X \n", i-3, datas[i-3]);
            break;

        default:
            datas[i-3] = (void*)lua_topointer(L, i);
            printf("UNSUPPORTED(%d) %X \n", i-3, datas[i-3]);
            break;
        }
    }


    switch(i - 3)
    {
    case 0:
        ret = __def_noinline_noret(swinum, void *);
        break;

    case 1:
        ret = __def_noinline_noret(swinum, void *, datas[0]);
        break;

    case 2:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1]);
        break;

    case 3:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2]);
        break;

    case 4:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3]);
        break;

    case 5:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4]);
        break;

    case 6:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5]);
        break;

    case 7:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6]);
        break;

    case 8:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7]);
        break;

    case 9:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8]);
        break;

    case 10:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9]);
        break;

    case 11:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10]);
        break;

    case 12:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11]);
        break;

    case 13:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[12]);
        break;

    case 14:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[11], datas[13]);
        break;

    case 15:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[11], datas[13], datas[14]);
        break;

    case 16:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[11], datas[13], datas[14], datas[15]);
        break;
    }

    ram_func:
    if( swinum >= 0x8000 )
        ret = (void*)__def_noinline_noret_ram(swinum, void *);

    int rettype = (!strcmp(t, "none")? LUA_TNONE:-2 );
    if(rettype == -2)
        rettype = (!strcmp(t, "nil")? LUA_TNIL:-2 );

    if(rettype == -2)
        rettype = (!strcmp(t, "boolean")? LUA_TBOOLEAN:-2 );

    if(rettype == -2)
        rettype = (!strcmp(t, "number")? LUA_TNUMBER:-2 );

    if(rettype == -2)
        rettype = (!strcmp(t, "string")? LUA_TSTRING:-2 );

    if(rettype == -2)
        rettype = (!strcmp(t, "userdata")? LUA_TUSERDATA:-2 );

    if(rettype == -2)
        rettype = (!strcmp(t, "pointer")? LUA_TCDATA:-2 );


    if(rettype == -2)
        rettype = LUA_TNIL;

    switch(rettype)
    {
    case  LUA_TNIL:
        lua_pushnil(L);
        return 1;

    case  LUA_TBOOLEAN:
        lua_pushboolean(L, (int)ret);
        return 1;

    case LUA_TNUMBER:
        lua_pushinteger(L, (int)ret);
        return 1;

    case  LUA_TSTRING:
        lua_pushstring(L, (const char*)ret);
        return 1;

    case LUA_TUSERDATA:
        lua_pushlightuserdata(L, ret);
        return 1;

    case LUA_TCDATA:
        lua_pushlightuserdata(L, ret);
        return 1;
    }

    return 1;
}




static const luaL_Reg swi_lib[] =
{
    {"call", swicall},
    {"toaddress", toaddress},
    {NULL, NULL}
};



LUALIB_API int luaopen_swi (lua_State *L)
{
    luaL_register(L, "swi", swi_lib);
    return 1;
}



