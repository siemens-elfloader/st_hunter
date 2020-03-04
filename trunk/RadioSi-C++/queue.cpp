
#include "queue.h"


Queue::Queue(size_t _message_size, size_t _message_count)
{
    message_size = message_size;

    queue_size = message_size * _message_count;
    queue_memory = new char[queue_size];

    queue_id = createQueue("q", queue_memory, queue_size/4, NU_FIXED_SIZE, message_size, NU_FIFO);
}


Queue::~Queue()
{
    delete (char *)queue_memory;
    destroyQueue(queue_id);
}


int Queue::push(const void *data, size_t size, int suspend)
{
    return NU_Send_To_Queue(getQueueDataByID(queue_id), (void*)data, size, suspend);
}


int Queue::recv(void *data, size_t size, size_t *actual_size, int suspend)
{
    size_t act_size;

    if(!actual_size)
        actual_size = &act_size;

    return NU_Receive_From_Queue(getQueueDataByID(queue_id), data, size, (unsigned long*)actual_size, suspend);
}

