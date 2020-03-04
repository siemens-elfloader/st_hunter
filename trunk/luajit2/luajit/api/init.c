
#define linit_c
#define LUA_LIB

#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"
#include "lj_lualib.h"



static const luaL_Reg mylualibs[] = {
  {LUA_SYSNAME, luaopen_sys},
  {0, 0}
};


LUALIB_API void luaL_myopenlibs (lua_State *L) {
  const luaL_Reg *lib = mylualibs;
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
}
