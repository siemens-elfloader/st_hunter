
#ifndef __HANDLE_KILLER_H__
#define __HANDLE_KILLER_H__

#include "ExtendedNuTask.h"

typedef struct
{
    void (*func_ptr)(void *, void *, void *);
    void *args[3];
}func_scheduler_event __attribute__((aligned(4)));


void func_schedule(void *proc, void *arg1, void *arg2, void *arg3);
ExtTask *get_func_scheduler_task();

void func_schedule_lock();
void func_schedule_unlock();

void safe_kill_task_dump(ExtTask *must_die, int free_struct, void *dump, size_t size);
void safe_kill_task(ExtTask *must_die, int free_struct);

#endif
