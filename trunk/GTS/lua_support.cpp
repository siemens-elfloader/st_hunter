
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#ifdef __LUA_SUPPORT__

extern char dir[];
extern char temp[];
lua_State *L;

static const luaL_Reg gts_lualibs[] = {
  {"", luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_IOLIBNAME, luaopen_io},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  0, 0
};


void lua_load_libs (lua_State *L) {
  const luaL_Reg *lib = gts_lualibs;
  for (; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
}


int lua_init()
{
    int status;
    L = luaL_newstate();

    //lua_load_libs(L);
    luaL_openlibs(L);

    sprintf(temp, "%sparser.lua", dir);
    status = luaL_loadfile(L, temp);
    if (status) {
        /* If something went wrong, error message is at the top of */
        /* the stack */
        //sprintf(temp, "Error: %s", lua_tostring(L, -1));
        //ShowMSG(1, (int)temp);
        lua_close(L);
        return -1;
    }

    return 0;
}


int do_lua(const char *text, char *retbuf)
{
    int result;

    lua_pushstring(L, text);
    lua_setglobal(L, "google_text");

    result = lua_pcall(L, 0, 1, 0);
    if (result) {
        sprintf(temp, "Error: %s", lua_tostring(L, -1));
        ShowMSG(1, (int)temp);
        return -1;
    }

    const char *str = lua_tostring(L, -1);
    strcpy(retbuf, str);

    lua_pop(L, 1);

    return 0;
}



int lua_finish()
{
    lua_close(L);
}


#endif









