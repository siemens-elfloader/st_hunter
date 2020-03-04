
#ifndef __LUA_SUPPORT__H__
#define __LUA_SUPPORT__H__


int lua_init();
int do_lua(const char *text, char *retbuf);
int lua_finish();


extern int is_use_lua;

#endif
