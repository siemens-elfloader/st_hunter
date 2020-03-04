#include "multivariatebuffer.h"
#include <stdio.h>



MultivariateBuffer::MultivariateBuffer(int bufs_cnt, size_t bufs_sz)
{
    currentFlushBuffer = currentReadBuffer = 0;
    BuffresCount = bufs_cnt;
    BufferSize = bufs_sz;

    for(int i=0; i<bufs_cnt; ++i) {
        buffers.push_back(new MegaBuffer(BufferSize));
    }
}


MultivariateBuffer::~MultivariateBuffer()
{
    for(int i=0; i<BuffresCount; ++i) {
        delete buffers[i];
    }
}


ssize_t MultivariateBuffer::write(const void *data, size_t size)
{
    MegaBuffer *b =  buffers.at(currentFlushBuffer);
    return b->write(data, size);
}


ssize_t MultivariateBuffer::read(void *data, size_t size)
{
    MegaBuffer *b =  buffers.at(currentReadBuffer);
    return b->read(data, size);
}


int MultivariateBuffer::availableFlushData()
{
    return buffers.at(currentFlushBuffer)->availableData();
}


int MultivariateBuffer::availableFlushSpace()
{
    return buffers.at(currentFlushBuffer)->availableSpace();
}


int MultivariateBuffer::availableReadData()
{
    return buffers.at(currentReadBuffer)->availableData();
}


int MultivariateBuffer::availableReadSpace()
{
    return buffers.at(currentReadBuffer)->availableSpace();
}

