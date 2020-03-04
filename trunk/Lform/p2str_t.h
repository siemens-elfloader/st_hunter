
#ifndef __P2STR_T_H__
#define __P2STR_T_H__

#include <string>
#include <inttypes.h>

#define isPhysicalAddres(x) ((uint32_t)x >= 0xA0000000 /*&& (uint32_t)x <= 0xA9000000 */)


class p2str_t
{
public:

    p2str_t() :
        ptr(0), _rptr(0), physical(false), len(0), _size(0)
    {
    }

    p2str_t(const char *_ptr) :
        ptr(0), _rptr(0), physical(false), len(0), _size(0)
    {
        set((void *)_ptr);
    }

    p2str_t(int _ptr) :
        ptr(0), _rptr(0), physical(false), len(0), _size(0)
    {
        set((void *)_ptr);
    }

    p2str_t(void *_ptr) :
        ptr(0), _rptr(0), physical(false), len(0), _size(0)
    {
        set(_ptr);
    }


    ~p2str_t();

    /* set data */
    void set(void *_ptr);

    /* set operator */
    int operator = (int _ptr) {
        set((void *)_ptr);
        return _ptr;
    }

    int operator = (unsigned int _ptr) {
        set((void *)_ptr);
        return _ptr;
    }

    const char *operator = (const char *_ptr) {
        set((void *)_ptr);
        return _ptr;
    }

    p2str_t & operator = (const p2str_t & p) {
        set((void *)p.datac());
        return *this;
    }

    /* non-cast return data */
    inline int datai() const {
        return (int)ptr;
    }


    inline const char *datac() const {
        return (const char *)ptr;
    }


    inline bool is_physical() {
        return physical;
    }

    void reserve(int reserv);
    void erase();
    size_t length();
    void noRealloc(bool is);

private:
    void *ptr, *_rptr;
    bool physical, nrealloc;
    size_t len, _size;
};






#endif
