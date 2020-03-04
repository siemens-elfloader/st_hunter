
#ifndef __CXX_WAITCONDITION_H__
#define __CXX_WAITCONDITION_H__

#include <spl/waitcondition.h>
#include "Mutex.h"


class WaitCondition
{
public:
    WaitCondition() :
        wid(-1)
    {
        wid = createWaitCond("wa");
    }

    ~WaitCondition() {
        wid = -1;
    }


    inline void wait() {
        waitCondition(wid);
    }

    inline void wakeOne() {
        wakeOneWaitCond(wid);
    }

    inline void wakeAll() {
        wakeAllWaitConds(wid);
    }

private:
    int wid;
};







#endif
