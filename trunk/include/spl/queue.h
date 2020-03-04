
#ifndef __COREQUEUE_H__
#define __COREQUEUE_H__

#include <nu_swilib.h>

#ifdef __cplusplus
extern "C" {
#endif


int createQueue(const char *name, void *start_address, unsigned long queue_size,
                                  unsigned char message_type, unsigned long message_size,
                                  unsigned char suspend_type);


int destroyQueue(int id);
NU_QUEUE *getQueueDataByID(int id);

int sendToQueue(int queue, void *message, unsigned long size, int suspend);
int receiveFromQueue(int queue, void *message, unsigned long size, unsigned long *actual_size, int suspend);


#ifdef __cplusplus
}
#endif

#endif
