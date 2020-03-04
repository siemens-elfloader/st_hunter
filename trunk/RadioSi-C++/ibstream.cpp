#include "ibstream.h"


ibstream::ibstream(void *fd, ssize_t (*read)(void *, void *, size_t)) :
    buffer(25*1024)
{
    _read = read;
    this->fd = fd;
}


ibstream::~ibstream()
{

}


int64_t ibstream::unread(const void *data, uint64_t size)
{
    if(!size)
        return size;

    if((size_t)buffer.availableSpace() < size) {
        buffer.resize( buffer.size() + ( size - buffer.availableSpace() ) + 128 );
    }

    return buffer.write(data, size);;
}


uint64_t ibstream::readBuffer(void *data, uint64_t size)
{
    if(buffer.isEmpty())
        return 0;
    else
        return buffer.read(data, size);
}


int64_t ibstream::read(void *_data, uint64_t size)
{
    char *data = (char *)_data;
    uint64_t r = readBuffer(data, size);

    if(r < size) {
        r += this->_read(fd, data + r, size-r);
    }

    return r;
}
