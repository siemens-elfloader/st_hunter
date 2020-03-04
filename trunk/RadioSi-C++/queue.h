
#ifndef __CXX_QUEUE_H__
#define __CXX_QUEUE_H__

#include <spl/queue.h>
#include <string.h>


class Queue
{
public:
    Queue(size_t message_size, size_t message_count);
    ~Queue();


    int push(const void *data, size_t size, int suspend = NU_SUSPEND);
    int recv(void *data, size_t size, size_t *actual_size, int suspend = NU_SUSPEND);


private:
    int queue_id;
    void *queue_memory;
    size_t queue_size;
    size_t message_size;
};




#endif
