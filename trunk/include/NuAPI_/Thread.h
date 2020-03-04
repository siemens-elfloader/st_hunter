
#ifndef __THREAD_H__
#define __THREAD_H__

#include "ExtendedNuTask.h"


struct Thread
{
    ExtTask task;
};


ExtTask *thread_parent_task(struct Thread *task);

int thread_create7v(struct Thread *thread, void *(*func_ptr)(int, void *),
                       void *stack, uint32_t stack_size, int argc, void *argv, int prio);
int thread_create5v(struct Thread *thread, void *(*func_ptr)(int, void *), int argc, void *argv, int prio);

int thread_release(struct Thread *thread);


int thread_suspend(struct Thread *thread);
int thread_resume(struct Thread *thread);
int thread_is_suspended(struct Thread *thread);

int thread_is_finished(struct Thread *thread);
void *thread_return_value(struct Thread *thread);

void thread_set_preinit_func(struct Thread *thread, void (*f)());
void thread_set_finished_func(struct Thread *thread, void (*f)());

int thread_wait_for_state(struct Thread *thread, int state, int timeout);
#endif
