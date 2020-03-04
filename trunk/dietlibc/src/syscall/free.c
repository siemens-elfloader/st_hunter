
#include <swihelper.h>
#include <stddef.h>
#include <spl/memctl.h>
#include <unistd.h>


void free(void * data)
{
    memoryFree(getpid(), data);
}
