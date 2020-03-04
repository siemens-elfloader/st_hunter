#ifndef MULTIVARIATEBUFFER_H
#define MULTIVARIATEBUFFER_H

#include <swilib.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include "Mutex.h"


struct MegaBuffer{

public:
    MegaBuffer() :
        data(0), seek(0),
        _size(0), offset(0)
    {}

    MegaBuffer(size_t _size) :
        seek(0),
        _size(_size),
        offset(0)
    {
        data = new char[_size+1];
    }

    ~MegaBuffer() {
        if(data)
            delete data;

        data = 0;
        seek = 0;
        _size = 0;
        offset = 0;
    }

    MegaBuffer & operator =(const MegaBuffer & b) {
        if(data)
            delete data;

        data = new char[b._size+1];
        memcpy(data, b.data, b._size);
        _size = b._size;
        seek = b.seek;
        offset = b.offset;
        return *this;
    }

    inline void resize(int nsize) {
        data = (char*)::realloc(data, nsize);
        _size = nsize;
    }

    inline int availableSpace() {
        return _size - seek;
    }


    inline int availableData() {
        return seek - offset;
    }


    inline int size() {
        return _size;
    }

    ssize_t write(const void *_data, size_t bsize) {

       mutex.lock();
       int can_write = std::min(int64_t(availableSpace()), int64_t(bsize));
       if(can_write > 0) {
           memcpy(data+seek, _data, can_write);
           seek += can_write;
       }
       mutex.unlock();

       return can_write;
    }

    ssize_t read(void *_data, size_t bsize) {

        mutex.lock();
        int can_read = std::min(int32_t(availableData()), int32_t(bsize));
        if(can_read > 0 && can_read < size()) {
            memcpy(_data, data+offset, can_read);
            offset += can_read;
            //if(offset == seek) {
            //    offset = 0;
            //    seek = 0;
            //}
        }
        mutex.unlock();

        return can_read;
    }

    inline int position() {
        return offset;
    }

    inline bool isEmpty() {
        return (seek - offset) == 0;
    }

    inline void reset() {
        seek = offset = 0;
    }

    inline bool isFull() {
        return availableData() == size();
    }

    char *data;
    uint32_t seek;
    uint32_t _size;
    uint32_t offset;
    Mutex mutex;
};


class MultivariateBuffer
{
public:
    MultivariateBuffer(int bufs_cnt, size_t bufs_sz);
    ~MultivariateBuffer();

    int availableFlushData();
    int availableFlushSpace();
    int availableReadData();
    int availableReadSpace();
    ssize_t write(const void *, size_t size);
    ssize_t read(void *data, size_t size);

    void nextFlushBuffer() {
        currentFlushBuffer++;
        if(currentFlushBuffer >= BuffresCount)
            currentFlushBuffer = 0;
    }

    void nextReadBuffer() {
        currentReadBuffer++;
        if(currentReadBuffer >= BuffresCount)
            currentReadBuffer = 0;
    }


    inline int flushBufferNumber() {
        return currentFlushBuffer;
    }

    inline int readBufferNumber() {
        return currentReadBuffer;
    }

    inline int buffersCount() {
        return BuffresCount;
    }

    inline int size() {
        return BufferSize;
    }

    inline int position() {
        return buffers[currentReadBuffer]->position();
    }

    inline bool isReadBufferEmpty() {
        return buffers[currentReadBuffer]->isEmpty();
    }

    inline void resetFlushBuffer() {
        return buffers[currentFlushBuffer]->reset();
    }

    inline bool isFlushBufferFull() {
        return buffers[currentFlushBuffer]->isFull();
    }

    inline bool isReadBufferFull() {
        return buffers[currentReadBuffer]->isFull();
    }


    int currentFlushBuffer, currentReadBuffer;

private:
    int BuffresCount;
    int BufferSize;

    std::vector<MegaBuffer*> buffers;
};

#endif // MULTIVARIATEBUFFER_H
