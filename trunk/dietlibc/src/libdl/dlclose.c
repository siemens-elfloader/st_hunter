
#include <swilib.h>
#include <stdlib.h>



int dlclose(void * handle)
{
    if(handle)
    {
        int d = (int )(*(int*)handle);
        int ret = _dlclose(d);
        free(handle);
        return ret;
    }
    return 0;
}
