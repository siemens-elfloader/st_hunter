
#include <swihelper.h>
#include <stddef.h>
#include <spl/memctl.h>
#include <unistd.h>



void *realloc(void *ptr, size_t size)
{
    return memoryRealloc(getpid(), ptr, size);
}
