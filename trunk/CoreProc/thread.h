
#ifndef __THREAD_H__
#define __THREAD_H__

#include "task.h"


int createThread(int prio, int (*handle)(void *), void *data, int run);

int suspendThread(int tid);
int resumeThread(int tid);
int destroyThread(int tid);

int tidByTask(NU_TASK *task);
int gettid();
int getptid();


int waitForThreadStarted(int _tid);
int waitForThreadFinished(int _tid, int *retcode);

#endif
