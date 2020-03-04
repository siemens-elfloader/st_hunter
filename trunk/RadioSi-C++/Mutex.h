
#ifndef __CXX_MUTEX_H__
#define __CXX_MUTEX_H__

#include <spl/mutex.h>


class Mutex
{
public:
    Mutex() {
        createMutex(&mid);
    }

    ~Mutex() {
        destroyMutex(&mid);
    }


    inline int lock() {
        return lockMutex(&mid);
    }

    inline int unlock() {
        return unlockMutex(&mid);
    }

    inline int tryLock() {
        return tryLockMutex(&mid);
    }

private:
    CoreMutex mid;
};












#endif
