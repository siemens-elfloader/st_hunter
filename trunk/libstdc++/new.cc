
#include <stdlib.h>
#include <swilib.h>


void operator delete(void *d) {
    free(d);
}


void operator delete[](void *d) {
    free(d);
}

void *operator new(size_t sz) {
    return malloc(sz);
}

void *operator new[](size_t sz) {
    return malloc(sz);
}
