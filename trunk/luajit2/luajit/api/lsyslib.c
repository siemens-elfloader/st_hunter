
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <swilib.h>
#include <pnglist.h>

#define LUA_LIB

#include "lj_state.h"
#include "lua.h"
#include "lj_lualib.h"
#include "lauxlib.h"
#include "lualib.h"
#include <my_lua.h>
#include <luaconf.h>
#include "../curent_state.h"

#define TMRSNUM 512

extern int *lua_exiting;

typedef struct
{
    int param0;
    int param1;
    int param2;
    int param3;
    lua_State *L;
    int ref;
} lua_GBSTMR;


typedef struct
{
    lua_State *L;
    int type[24];
    char *data[24];
    int nums;
    char fname[128];
} lua_param;


lua_GBSTMR __timers[TMRSNUM];


void lua_stopAllTimers()
{
    for(int i=0; i<TMRSNUM; ++i)
    {
        if( IsTimerProc( (GBSTMR*)&__timers[i] ) )
        {
            GBS_DelTimer( (GBSTMR*)&__timers[i] );
        }
    }
}


static int sys_keep_alive (lua_State *L)
{
    mluaState(L)->keep_alive = 1;
    return 0;
}


static int sys_exit (lua_State *L)
{
    printf("sys_exit \n");
    mluaState(L)->abort(0xdead);
    return 0;
}


static int sys_ram_available (lua_State *L)
{
    lua_pushinteger(L, GetFreeRamAvail());
    return 1;
}



static int sys_isSgold (lua_State *L)
{
    lua_pushboolean(L, isnewSGold() == 0 );
    return 1;
}


static int sys_isNsg (lua_State *L)
{
    lua_pushboolean(L, isnewSGold() == 1 );
    return 1;
}


static int sys_isElka (lua_State *L)
{
    lua_pushboolean(L, isnewSGold() == 2 );
    return 1;
}


static int sys_cpu_usage (lua_State *L)
{
    lua_pushinteger(L, GetCPULoad() );
    return 1;
}


static int sys_pDir (lua_State *L)
{
    lua_pushstring(L, mluaState(L)->s_folder );
    return 1;
}


static int sys_clearPngCache (lua_State *L)
{
    ((void)L);
    PNGTOP_DESC *ptop=PNG_TOP();
    PNGLIST *pl;
    LockSched();
    pl=ptop->pltop;
    ptop->pltop=0;
    UnlockSched();
    while(pl)
    {
        PNGLIST *pl_prev;
        pl_prev=pl;
        pl=pl->next;
        mfree(pl_prev->pngname);
        if(pl_prev->img)
        {
            mfree(pl_prev->img->bitmap);
            mfree(pl_prev->img);
        }
        mfree(pl_prev);
    }
    return 0;
}



static int sys_reboot(lua_State *L)
{
    RebootPhone();
    mluaState(L)->abort(0xdead);
    return 0;
}


static int sys_setVibration(lua_State *L)
{
    int v = luaL_checkint(L, 1);

    lua_pushinteger(L, SetVibration(v));
    return 1;
}


static int sys_get_Phone_Info(lua_State *L)
{
    int i = luaL_checkint(L, 1);
    lua_pushstring(L, Get_Phone_Info(i));
    return 1;
}


static int sys_entry(lua_State *L)
{
    const char *s = luaL_checkstring(L, 1);

    unsigned int addr = strtoul(s, 0, 16);

    ((void (*)())addr)();
    return 0;
}


static int sys_getCap(lua_State *L)
{
    lua_pushinteger(L, (uint32_t)*RamCap());
    return 1;
}


static int sys_getNet(lua_State *L)
{
    lua_pushinteger(L, (uint32_t)RamNet()->power);
    return 1;
}


static int sys_isKbdLock(lua_State *L)
{
    lua_pushboolean(L, IsUnlocked()? 0 : 1 );
    return 1;
}


static int sys_isDeviceEnabled(lua_State *L)
{
    int dev = luaL_checkint(L, 1);
    lua_pushboolean(L, GetPeripheryState(dev, 1));
    return 1;
}


static int sys_isMediaPlaying(lua_State *L)
{
    lua_pushboolean(L, (uint8_t)*RamMediaIsPlaying());
    return 1;
}


static int sys_GetMissedCallsCount(lua_State *L)
{
    lua_pushinteger(L, GetMissedCallsCount());
    return 1;
}


static int sys_cpu_clock(lua_State *L)
{
    lua_pushinteger(L, GetCPUClock());
    return 1;
}


static int sys_isCalling(lua_State *L)
{
    lua_pushboolean(L, IsCalling());
    return 1;
}


static int sys_SetVibration(lua_State *L)
{
    size_t power = luaL_checkint(L, 1);
    lua_pushinteger(L, SetVibration(power));
    return 1;
}


static int sys_kbdLock(lua_State *L)
{
    ((void)L);
    KbdLock();
    return 0;
}


static int sys_kbdUnLock(lua_State *L)
{
    ((void)L);
    KbdUnlock();
    return 0;
}


char get_net_id(char *buf)
{
    char *x = (char*)Get_CC_NC();
    char *y = x+1;
    char *z = x+2;
    char cc_2 = *x;
    char cc_1 = *y;
    // Немного жёсткого секса с BCD...
    if(cc_1>=0xF0)
    {
        cc_1 = cc_1 && 0x0F>>4;
    }
    cc_2 = (cc_2<<4) + (cc_2>>4);
    char nc = *z;
    nc = (nc>>4) + (nc<<4);
    if((nc == 0xFF)&&(cc_2==0xFF))
    {
        return 0;
    }
    // ... и золотой ключик у нас в кармане )
    sprintf(buf, "%X%X-%02X", cc_2, cc_1, nc);
    return 1;
}


static int sys_get_cc_nc(lua_State *L)
{
    char buf[26];

    get_net_id(buf);
    lua_pushstring(L, buf);
    return 1;
}



static int sys_exec(lua_State *L)
{
    char *argv[128] = {0};
    int argc = 0;
    size_t len;
    const char *programm = luaL_checklstring(L, 1, &len);

    while(lua_type(L, argc+2) != LUA_TNONE)
    {
        argv[argc++] = (char*)luaL_checkstring(L, argc+2);
    }

    argv[argc] = 0;

    WSHDR *ws = AllocWS(len + 1);
    str_2ws(ws, programm, len);

    ExecuteFile(ws, 0, (void*)argv);
    FreeWS(ws);
    return 0;
}


int sys_libDir(lua_State *L)
{
    lua_pushstring(L, LUA_LDIR);
    return 1;
}



static const luaL_Reg syslib[] =
{
    {"keep_alive", sys_keep_alive},
    {"exit", sys_exit},
    {"ram_available", sys_ram_available},
    {"isSgold", sys_isSgold},
    {"isNsg", sys_isNsg},
    {"isElka", sys_isElka},
    {"cpu_usage", sys_cpu_usage},
    {"pDir", sys_pDir},
    {"clearPngCache", sys_clearPngCache},
    {"reboot", sys_reboot},
    {"setVibration", sys_setVibration},
    {"phoneInfo", sys_get_Phone_Info},
    {"entry", sys_entry},
    {"getCap", sys_getCap},
    {"getNet", sys_getNet},
    {"isKbdLock", sys_isKbdLock},
    {"isDeviceEnabled", sys_isDeviceEnabled},
    {"isMediaPlaying", sys_isMediaPlaying},
    {"getMissedCallsCount", sys_GetMissedCallsCount},
    {"cpu_clock", sys_cpu_clock},
    {"isCalling", sys_isCalling},
    {"vibration", sys_SetVibration},
    {"kbdLock", sys_kbdLock},
    {"kbdUnLock", sys_kbdUnLock},
    {"get_cc_nc", sys_get_cc_nc},
    {"exec", sys_exec},
    {"libDir", sys_libDir},

    {NULL, NULL}
};


LUALIB_API int luaopen_sys (lua_State *L)
{
    luaL_register(L, LUA_SYSNAME, syslib);
    return 1;
}

