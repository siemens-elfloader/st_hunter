
#include "thread.h"


Thread::Thread(int _stack_size, int _prio) :
    thread_id(-1),
    stack_size(_stack_size),
    prio(_prio)
{
    initTaskConf(&conf);

    conf.prio = prio;
    conf.stack_size = stack_size;
}


Thread::~Thread()
{
    destroyThread(thread_id);
}


int Thread::thread_handle(void *th)
{
    Thread *_this = static_cast<Thread*>(th);
    _this->run();
    return 0;
}


int Thread::start()
{
    return (thread_id = createConfigurableThread(&conf, thread_handle, (void *)this, 1));
}


int Thread::terminate()
{
    return destroyThread(thread_id);
}


