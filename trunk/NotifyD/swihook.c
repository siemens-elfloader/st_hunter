#include <swilib.h>

int GetSWIBlibExist()
{
    if ((int *)(*((int *)(int*)LIB_TOP()))) return 1;
    else return 0;
}

void *GetSWI(int swi_num)
{
    int *lib = (int *)(*((int *)(int*)LIB_TOP()));
    return (void *)lib[swi_num];
}

void SetSWI(int swi_num, void *swi_addr)
{
    LockSched();
    int *lib = (int *)(*((int *)(int*)LIB_TOP()));
    lib[swi_num] = (int)swi_addr;
    UnlockSched();
}

void SetSWIHook(int *swi_addr, int swi_id, void *func)
{
//Check RAM-SWILIB
    if (GetSWIBlibExist())
    {
//Save OLD SWI Address
        *swi_addr = (int)GetSWI(swi_id);
//Set SWI
        SetSWI(swi_id, func);
        //return 1;
    }
    /*else
    {
        MsgBoxError(1, (int)"swi.blib not loaded!");
        return 0;
    }*/
}

void DestroySWIHook(int swi_addr, int swi_id)
{
    SetSWI(swi_id, (void*)swi_addr);
}
