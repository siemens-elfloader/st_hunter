
#ifndef __EXT_QUEUE_H__
#define __EXT_QUEUE_H__


#include "ExtMutex.h"

void printout(const char *fmt, ...);

//#define edq_self(dq) ( ((ExtDataQueue*)dq->prev)->next )
#define edq_self(dq) ( dq->self )

typedef struct
{
    void *prev;
    void *next;
    void *self;
}ExtDataQueue;

typedef struct
{
    ExtDataQueue *first;
    ExtDataQueue *last;
    ExtMutex mutex;
    char inited, locked, mutex_used;
}ExtQueue;

void extqueue_init(ExtQueue *q, char use_mutex);

#define extqueue_iterate(qu)   \
        for(; qu; qu = qu->next)



void extqueue_release(ExtQueue *q);

void extqueue_push(ExtQueue *q, void *data);
int extqueue_replace(ExtQueue *q, void *from, void *to);
void extqueue_data_pop(ExtQueue *q, ExtDataQueue *qd);
int extqueue_pop(ExtQueue *q, void *data);

int extqueue_lock(ExtQueue *q, char lock);

#endif
