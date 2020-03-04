
#ifndef __WAIT_COND_H__
#define __WAIT_COND_H__


int createWaitCond(const char *name);
int destroyWaitCond(int wid);
int waitCondition(int wid);
int wakeOneWaitCond(int wid);
int wakeAllWaitConds(int wid);
int resetWaitCondWaiters(int wid);

#endif
