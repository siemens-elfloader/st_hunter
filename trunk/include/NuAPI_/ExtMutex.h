
#ifndef __EXT_MUTEX_H__
#define __EXT_MUTEX_H__

//#include <swilib.h>
#include <nu_swilib.h>


typedef struct
{
    NU_SEMAPHORE sema;
    int loked;
}ExtMutex;



int ExtMutex_init(ExtMutex *mutex);
int ExtMutex_release(ExtMutex *mutex);
int ExtMutex_lock(ExtMutex *mutex);
int ExtMutex_trylock(ExtMutex *mutex);
int ExtMutex_unlock(ExtMutex *mutex);



#endif
