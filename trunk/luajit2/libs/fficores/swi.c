
#include <swilib.h>
#include <stdarg.h>


#define __def_noinline_noret(id, ret, ...)\
    ((ret (*)())((__sys_switab_addres[id])))(__VA_ARGS__);

#define __def_noinline_noret_ram(id, ret)\
     (ret)(__sys_switab_addres[id-0x8000]);


void *swi(int swinum, int argnum, void *f, ...)
{
    void *datas[20];
    void *ret = 0;

    if(swinum >= 0x8000 ){
        goto ram_func;
    }

    va_list va;
    va_start(va, f);
    datas[0] = f;
    printf("args: %d: %X\n", 0, datas[0]);

    for(int i = 1; i<argnum; ++i)
    {
        datas[i] = va_arg(va, void *);
        printf("args: %d: %X\n", i, datas[i]);
    }

    switch(argnum)
    {
    case 0:
        ret = __def_noinline_noret(swinum, void *);
        break;

    case 1:
        ret = __def_noinline_noret(swinum, void *, datas[0]);
        break;

    case 2:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1]);
        break;

    case 3:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2]);
        break;

    case 4:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3]);
        break;

    case 5:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4]);
        break;

    case 6:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5]);
        break;

    case 7:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6]);
        break;

    case 8:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7]);
        break;

    case 9:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8]);
        break;

    case 10:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9]);
        break;

    case 11:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10]);
        break;

    case 12:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11]);
        break;

    case 13:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[12]);
        break;

    case 14:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[11], datas[13]);
        break;

    case 15:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[11], datas[13], datas[14]);
        break;

    case 16:
        ret = __def_noinline_noret(swinum, void *, datas[0], datas[1], datas[2], datas[3], datas[4],
                                   datas[5], datas[6], datas[7], datas[8], datas[9], datas[10], datas[11],
                                   datas[11], datas[13], datas[14], datas[15]);
        break;
    }

    ram_func:
    if( swinum >= 0x8000 )
        ret = (void*)__def_noinline_noret_ram(swinum, void *);

    return ret;
}
