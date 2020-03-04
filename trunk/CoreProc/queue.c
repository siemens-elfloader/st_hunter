
#include <swilib.h>
#include <spl/resctl.h>
#include <spl/corelist.h>
#include <spl/process.h>
#include <spl/queue.h>
#include <spl/static_data_base.h>



typedef struct  {
    int id;
    NU_QUEUE queue;
    int pid;
    int dt_id;
}idQueue;

#define MAX_QUEUE_ID 256
declare_static_db(idQueue, queue, MAX_QUEUE_ID)



int createQueue(const char *name, void *start_address, unsigned long queue_size,
                                  unsigned char message_type, unsigned long message_size,
                                  unsigned char suspend_type)
{
    int id;
    idQueue *q = newidQueueData();
    if(!q) {
        return -1;
    }

    id = q->id;
    q->pid = getpid();
    memset(&q->queue, 0, sizeof(q->queue));

    int status;
    if( (status = NU_Create_Queue(&q->queue, (CHAR*)name, start_address, queue_size, message_type, message_size, suspend_type)) != NU_SUCCESS ) {
        q->id = -1;
        return status;
    }

    q->dt_id = addProcessDtors(q->pid, (void*)destroyQueue, (void*)id, 0);
    return id;
}


int destroyQueue(int id)
{
    idQueue *q = checkidQueueData(id);
    if(!q)
        return -1;

    NU_Delete_Queue(&q->queue);
    eraseProcessDtor(q->pid, q->dt_id);
    freeidQueueData(id);
    return 0;
}


NU_QUEUE *getQueueDataByID(int id)
{
    idQueue *q = checkidQueueData(id);
    if(!q)
        return 0;

    return &q->queue;
}


int sendToQueue(int queue, void *message, unsigned long size, int suspend)
{
    idQueue *q = checkidQueueData(queue);
    if(!q)
        return -1;

    return NU_Send_To_Queue(&q->queue, message, size, suspend);
}


int receiveFromQueue(int queue, void *message, unsigned long size, unsigned long *actual_size, int suspend)
{
    idQueue *q = checkidQueueData(queue);
    if(!q)
        return -1;

    return NU_Receive_From_Queue(&q->queue, message, size, actual_size, suspend);
}







