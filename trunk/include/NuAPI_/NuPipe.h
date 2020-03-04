
#ifndef __NU_PIPE_H__
#define __NU_PIPE_H__

#include <swilib.h>
#include <nu_swilib.h>
#include <NuAPI/ExtendedNuTask.h>


struct PipeData
{
    void *data;
    size_t size;
} __attribute__((aligned(4)));

typedef struct
{
    NU_QUEUE queue;
    void *queue_space;
    ExtTask *crt_in;
    struct PipeData pdata;
    char removed_from_task;
    char released, locked;
}NuPipe;



int NuPipe_create(NuPipe *pipe, int size, unsigned char suspend_type);
int NuPipe_release(NuPipe *pipe);

int NuPipe_push(NuPipe *pipe, void *buffer, size_t size, unsigned int suspend_type);
int NuPipe_recv(NuPipe *pipe, void *buffer, int size, size_t *event_size, unsigned int suspend_type);

#endif
