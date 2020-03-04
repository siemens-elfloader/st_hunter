
#include <swihelper.h>
#include <stddef.h>
#include <spl/memctl.h>
#include <unistd.h>


void *calloc(size_t nelem, size_t elsize)
{
    return memoryAlloc(getpid(), nelem * elsize);
}

