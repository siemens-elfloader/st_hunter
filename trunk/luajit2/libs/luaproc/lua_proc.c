

#include <swilib.h>


#define ID 0x4021

void lua_proc_func(void)
{
    GBS_MSG msg;
    if (GBS_RecActDstMessage(&msg))
    {
        if (msg.msg==1)
        {
            if (msg.data0)
            {
                void (*func)(void *) = (void (*)(void *))msg.data0;
                func(msg.data1);
            }
        }
    }
}

__attribute_constructor
void fini_lua_proc()
{
    CreateGBSproc(ID, "lua proc", lua_proc_func, 90, 0);
}


__attribute_destructor
void init_lua_proc()
{
    KillGBSproc(ID);
}


void RunInLuaProc(void *func, void *ud1)
{
    GBS_SendMessage(ID, 1, 0, func, (void*)ud1);
}

