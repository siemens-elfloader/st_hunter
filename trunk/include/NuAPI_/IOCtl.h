
#ifndef __IOCTL_H__
#define __IOCTL_H__

#include <NuAPI/ExtArray.h>
#include <NuAPI/ExtendedNuTask.h>


enum{
    IO_CTL_FILE_STREAM,
    IO_CTL_PIPE
};


struct IOCtl_cell
{
    void *stream;
    int  flags;
    char type;
    char used;
    char *path;
};


void IOCtl_systems_init();
void IOCtl_push(ExtArray *q, int fd);
int IOCtl_pop(ExtArray *q, int fd);
int IOCtl_clean(struct IOCtl_system_handle *dat);
size_t IOCtl_opened_handler_count(ExtArray *q);

int IOCtl_sys_open(ExtTask *task, const char *path, int type, int flags, void *stream);
int IOCtl_sys_close(ExtTask *task, int handle);
int IOCtl_find_stream(ExtTask *task, int type, void *stream, struct IOCtl_cell **c);
struct IOCtl_cell * IOCtl_stream_info(ExtTask *task, int handle);

ssize_t ext_read(ExtTask *task, int fd, void *buf, size_t size);
ssize_t ext_write(ExtTask *task, int fd, const void *buf, size_t size);
int ext_close(ExtTask *task, int fd);


int IOCtl_open(const char * cFileName, unsigned int iFileFlags, unsigned int iFileMode, unsigned int *ErrorNumber);
int IOCtl_read(int handle, void *buffer, int size, unsigned int *err);
int IOCtl_write(int handle, const void *buffer, int size, unsigned int *err);
int IOCtl_lseek(int handle, unsigned int offset, unsigned int origin,
                unsigned int *err, unsigned int *err1);
int IOCtl_flush(int handle, unsigned int *err);
int IOCtl_truncate(int handle, int length, unsigned int *err);

int IOCtl_close(int fd, unsigned int *ErrorNumber);

#endif
