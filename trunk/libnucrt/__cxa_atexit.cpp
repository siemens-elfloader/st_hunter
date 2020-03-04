
#include <swilib.h>

typedef struct
{
    void (*func)(void*);
    void *object;
    void *next;
}__cxa_atexit_struct;

extern char *_curent_dir;
__cxa_atexit_struct *__s_exit = 0, *__ex_start = 0;
int __cxa_is_killing = 0;
void *__dso_handle = 0;


extern void h_kill_elf(__cxa_atexit_struct *__ex_start, int *__cxa_is_killing, void *);
/*
extern "C"
void __crt_run_finiarray()
{
    h_kill_elf(__ex_start, &__cxa_is_killing, 0);
}

*/

void kill_elf()
{
    extern void *__ex;
    volatile void *ex = &__ex;
    h_kill_elf(__ex_start, &__cxa_is_killing, (void*)ex);
}
