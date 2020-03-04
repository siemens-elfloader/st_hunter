
#include <swilib.h>



void *dlsym(void *handle, const char *sym)
{
    if(!handle || !sym || !*sym) return 0;

    int d = (int )(*(int*)handle);
    return _dlsym(d, sym);
}
