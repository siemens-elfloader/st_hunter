
#include <swilib.h>
#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <my_lua.h>
#include "curent_state.h"

int l_cur_status;





void finalize(int status)
{
    m_lua_State *l = mluaState(curent_lstate);
    int *point = &l->is_lock;

    atexit_list *list = l->atexitl;
    while(list)
    {
        list->exit_func(curent_lstate, list->user_data);
        list = list->next;
    }

    fprintf(stderr, "Closing lua thread\n");

    lua_gc(curent_lstate, LUA_GCCOLLECT, 0);
    lua_close(curent_lstate);
    curent_lstate = 0;

    fprintf(stderr, "lock state: %d\n", *point);

    while(*point > 0)
        NU_Sleep(100);

    if(l->s_folder) free(l->s_folder);
    if(l->s_name) free(l->s_name);
    if(l->script) free(l->script);
    if(l->plugin_dir) free(l->plugin_dir);
    free(l);

    fprintf(stderr, "Done\n");
    kill_elf();
}


int double_call_protect = 0;
void m_lua_abort(int status)
{
    if(!curent_lstate || double_call_protect) return;
    double_call_protect = 1;
    l_cur_status = status;

    if(status != 0xdead)
    {
        report(curent_lstate, status);
        traceback(curent_lstate);
        fprintf(stderr, "break with errors\n");
    }

    fflush(stderr);
    fflush(stdout);

    m_lua_State *l = mluaState(curent_lstate);
    l->is_aborting = 1;
    fprintf(stderr, "exit status: %d\n", status);
    SUBPROC((void *)finalize, (void*)status);
}
