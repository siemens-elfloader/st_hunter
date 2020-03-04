
#include <swihelper.h>
#include <unistd.h>
#include <stddef.h>
#include <spl/memctl.h>


void *malloc(size_t size)
{
    return memoryAlloc(getpid(), size);
}
