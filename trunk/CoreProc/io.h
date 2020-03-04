
#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include "corelist.h"

enum {
    FS_STREAM,
    SOCKET_STREAM,
    PIPE_STREAM
}stream_type;


typedef struct
{
    int id;
    int type;
    int fd;
    int mode;
    ssize_t (*read)(int fd, void *data, size_t size);
    ssize_t (*write)(int fd, const void *data, size_t size);
    off_t (*lseek)(int fd, off_t offset, int whence);
    int (*flush)(int fd);
    int (*close)(int fd);

    struct CoreListInode *resctl_inode;
    short pid;
} idStream;



int open_fd();
int close_fd(int fd);
idStream *getStreamData(int fd);


ssize_t read(int fd, void *buf, size_t len);
ssize_t write(int fd, const void *buf, size_t len);
off_t lseek(int fd, off_t offset, int whence);
int flush(int fd);
int close(int fd);


#endif
