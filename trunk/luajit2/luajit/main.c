
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "std_init.h"
#include <swilib.h>

#define luajit_c
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luajit.h"
#include "lj_lib.h"
#include "lj_arch.h"
#include "my_lua.h"
#include "curent_state.h"
#include <ep3/loader.h>
#include <debug/io.h>

void *lpoc = 0;
lua_State *curent_lstate;
extern void m_lua_abort(int status);
extern void luaL_myopenlibs (lua_State *L);
char __lua_temp[256];
extern void RunInLuaProc(void *func, void *ud1);

#include "luajit.c"


void luajit_init(lua_State *L)
{
    LUAJIT_VERSION_SYM();  /* linker-enforced version check */
    lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
    luaL_openlibs(L);  /* open libraries */
    luaL_myopenlibs(L);
    lua_gc(L, LUA_GCRESTART, -1);
}


void setting_struct(m_lua_State *l, char *elf, char *fname)
{
    l->is_lock = 0;
    l->atexitl = 0;
    memset(l, 0, sizeof(m_lua_State));
    l->keep_alive = 0;
    l->abort = m_lua_abort;

    size_t len = strlen(fname);

    l->script = malloc(len+1);
    memcpy(l->script, fname, len);
    l->script[len] = 0;

    char *si = elf+strlen(elf)-1;
    while(*si!='\\') si--;
    si++;
    l->plugin_dir = malloc(si-elf+56);
    memcpy(l->plugin_dir, elf, si-elf);
    l->plugin_dir[si-elf] = 0;
    strcat(l->plugin_dir, "lib\\");

    si = fname+len-1;
    while(*si!='\\') si--;
    si++;
    l->s_folder = malloc(si-fname+1);
    memcpy(l->s_folder, fname, si-fname);
    l->s_folder[si-fname] = 0;

    si = fname+len-1;
    while(*si!='\\') si--;
    l->s_name = malloc( ((fname+len) - si) + 1 );
    memcpy(l->s_name, fname, (fname+len) - si);
    l->s_name[(fname+len) - si] = 0;

}


void __subp(void *f, void *p)
{
    SUBPROC(f, p);
}

void init_lproc(m_lua_State *mls)
{
    getcwd(__lua_temp, 128);
    strcat(__lua_temp, "lib\\luaproc.so");
    lpoc = dlopen(__lua_temp, 0);
    void *proc = 0;
    if(lpoc)
    {
        proc = dlsym(lpoc, "RunInLuaProc");
    } else {
        proc = __subp;
    }

    mls->RunInLuaProc = (void (*)(void *, void *))proc;
}



void fini_lproc()
{
    if(lpoc)
        dlclose(lpoc);
}



int main(char * exe, char *name)
{
    int argc = 5;
    char *argv[10]= {exe, "-O3", "-jon", "-Ocallunroll=10", name,  0};
    if(name) argc++;
    int status;

    if(!name)
    {
        kill_elf();
        return -1;
    }

    getcwd(__lua_temp, 128);
    strcat(__lua_temp, "lua.bcfg");
    extern int InitConfig_byName(const char*);
    InitConfig_byName(__lua_temp);

    SetCpuClockTempHi(2);

    m_lua_State *mls = malloc(sizeof(m_lua_State));
    setting_struct(mls, exe, name);
    std_init(mls->s_folder);


    extern void *__ex;
    Elf32_Exec *ex = (Elf32_Exec *)&__ex;

    printf(" ELF: %X - %d\n", ex->body, ex->bin_size);
    fflush(stdout);

    struct Smain s;
    lua_State *L = lua_open();  /* create state */
    curent_lstate = L;
    if (L == NULL)
    {
        if(mls->s_folder) free(mls->s_folder);
        if(mls->s_name) free(mls->s_name);
        if(mls->script) free(mls->script);
        if(mls->plugin_dir) free(mls->plugin_dir);
        free(mls);
        l_message(argv[0], "cannot create state: not enough memory");
        kill_elf();
        return EXIT_FAILURE;
    }

    init_lproc(mls);
    atexit(fini_lproc);

    luaL_loadfile(L, name);
    luajit_init(L);

    lua_boxpointer(L, mls);
    lua_setglobal(L,"__MLS__");

    s.argc = argc;
    s.argv = argv;
    status = lua_cpcall(L, pmain, &s);
    if(status)
    {
         mls->abort(status);
         return 0;
    }

    if(mls->keep_alive)
       return 0;

    mls->abort(0xdead);
    return EXIT_SUCCESS;
}




