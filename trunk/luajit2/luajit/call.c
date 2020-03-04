

#include <lua.h>
#include <stdio.h>
#include <my_lua.h>



void l_message(const char *pname, const char *msg)
{
    if (pname) fprintf(stderr, "%s: ", pname);
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);
}


int report(lua_State *L, int status)
{
    m_lua_State *l = mluaState(L);

    if (status && !lua_isnil(L, -1))
    {
        const char *msg = lua_tostring(L, -1);
        if (msg == NULL) msg = "(error object is not a string)";
        if(l)
            l_message(l->script?l->script:"lua", msg);
        else
            l_message("lua", msg);
        lua_pop(L, 1);
    }
    return status;
}


int traceback(lua_State *L)
{
    if (!lua_isstring(L, 1))  /* 'message' not a string? */
        return 1;  /* keep it intact */
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    if (!lua_istable(L, -1))
    {
        lua_pop(L, 1);
        return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 2);
        return 1;
    }
    lua_pushvalue(L, 1);  /* pass error message */
    lua_pushinteger(L, 2);  /* skip this function and traceback */
    lua_call(L, 2, 1);  /* call debug.traceback */
    return 1;
}


int docall(lua_State *L, int narg, int clear)
{
    int status;
    int base = lua_gettop(L) - narg;
    lua_pushcfunction(L, traceback);
    lua_insert(L, base);
    status = lua_pcall(L, narg, clear, base);
    lua_remove(L, base);

    if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
    return status;
}
