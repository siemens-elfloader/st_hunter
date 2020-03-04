
#ifndef __CORE_MUTEX_H__
#define __CORE_MUTEX_H__

#include <swilib.h>
#include <nu_swilib.h>


typedef struct
{
    NU_SEMAPHORE sema;
    int locks;
} CoreMutex;



int createMutex(CoreMutex *mutex);
int destroyMutex(CoreMutex *mutex);
int lockMutex(CoreMutex *mutex);
int tryLockMutex(CoreMutex *mutex);
int unlockMutex(CoreMutex *mutex);


#endif
