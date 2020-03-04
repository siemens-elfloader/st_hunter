#ifndef BSTREAM_H
#define BSTREAM_H

#include <algorithm>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "multivariatebuffer.h"


class ibstream
{
public:
    ibstream(void *fd, ssize_t (*read)(void *, void *, size_t));
    ~ibstream();

    uint64_t readBuffer(void *data, uint64_t size);
    int64_t read(void *data, uint64_t);
    int64_t unread(const void *datam, uint64_t);


private:
    void *fd;

    MegaBuffer buffer;

    ssize_t (*_read)(void *, void *, size_t);
};

#endif // BSTREAM_H
