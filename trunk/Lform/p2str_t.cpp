
#include <sys/param.h>
#include "p2str_t.h"


p2str_t::~p2str_t()
{
    erase();
}


void p2str_t::set(void *_ptr)
{
    char *data = (char *)ptr;

    if(!nrealloc)
        erase();
    /* если дата нулевая и у нас есть ресерв */
    else if(!data && _rptr) {
        data = (char *)_rptr;
        _rptr = 0;
    }

    //printf("set: %X. ", _ptr);

    if(isPhysicalAddres(_ptr))
    {
        if(nrealloc) {
            size_t l = strlen((char*)_ptr);
            if(!data)
                data = new char[ (_size = ((len = l)+2)) ];

            int m = MIN(l, _size);
            memcpy(data, (char *)_ptr, m);
            data[m] = 0;
            physical = true;

        }
        else {
            data = new char[ (_size = ((len = strlen((char*)_ptr))+2)) ];
            memcpy(data, (char *)_ptr, len);
            data[len] = 0;
            physical = true;
        }
    }
    else
    {
        /* если вдруг нужен резерв а нам тут подсунули номер */
        if(nrealloc)
            _rptr = ptr;

        data = (char *)_ptr;
        physical = false;
    }

    ptr = (void *)data;
}


void p2str_t::erase()
{
        if(physical && ptr) {
            delete (char *)ptr;
        }
        ptr = 0;
        _rptr = 0;
        physical = false;
        _size = 0;
        len = 0;
}



void p2str_t::reserve(int reserv)
{
    if(reserv < 1) return;

    erase();

    ptr = (void*)new char[reserv];
    *((char*)ptr) = 0;
    physical = true;
    len = 0;
    _size = reserv;
}



size_t p2str_t::length()
{
    if(physical)
        return len;
    return 0;
}


void p2str_t::noRealloc(bool is)
{
    nrealloc = is;
}




