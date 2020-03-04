
#ifndef __PIPE_H__
#define __PIPE_H__

#include <swilib.h>
#include <nu_swilib.h>
#include <NuAPI/ExtMutex.h>
#include <NuAPI/NuPipe.h>

#define IsPipeCanRead(pipe) ( pipe->flags & A_ReadOnly || pipe->flags & A_ReadWrite)
#define IsPipeCanWrite(pipe) ( pipe->flags & A_WriteOnly || pipe->flags & A_ReadWrite)

enum
{
    PIPE_NOT_READABLE = -128,
    PIPE_NOT_WRITEABLE = -129,
};


typedef struct
{
    NuPipe pipe;
    int flags;
}IO_pipe;


int io_pipe_create(IO_pipe *pipe, int flags);
int io_pipe_push(IO_pipe *pipe, const void *d, int size);
int io_pipe_flush(IO_pipe *pipe);
int io_pipe_recv(IO_pipe *pipe, void *d, int size, uint32_t *real_size, int suspend);
int io_pipe_close(IO_pipe *pipe);

int io_pipe_recv_ncheck(IO_pipe *pipe, void *d, int size, uint32_t *real_size, int suspend);
int io_pipe_push_ncheck(IO_pipe *pipe, const void *d, int size);

#endif
