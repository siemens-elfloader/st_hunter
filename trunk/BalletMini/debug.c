
/*
 * (c) Z.Vova
 * ’ук на malloc free mfree calloc realloc с выводом в stderr подробной инфы о аллокаци€х,ака валгринд :D
*/


#include <swilib.h>
#include <sys/cdefs.h>
#include <stdio.h>
#include <stdlib.h>


__inl
void *_malloc(size_t size)
__def( 20, void *, size)

__inl
void _free(void * param1)
__defn( 21, param1)

__inl
void _mfree(void * param1)
__defn( 21, param1)

__inl
void *_calloc(size_t nelem, size_t elsize)
__def( 146, void *, nelem, elsize)

__inl
void *_realloc(void *ptr, size_t size)
__def( 186, void *, ptr, size)



typedef struct
{
    void *address;
    size_t size;
    char file[256];
    size_t line;
    void *prev, *next;
} val_debug;


val_debug *__ldb_base = 0, *__ldb_base_start = 0;

val_debug *getNextStruct()
{
    if(!__ldb_base)
    {
        __ldb_base = _malloc(sizeof(val_debug));
        memset(__ldb_base, 0, sizeof(val_debug));
        __ldb_base_start = __ldb_base;
        return __ldb_base;
    }
    else
    {
        val_debug *tmp = __ldb_base;
        __ldb_base->next = _malloc(sizeof(val_debug));
        __ldb_base = __ldb_base->next;
        __ldb_base->prev = tmp;
        __ldb_base->next = 0;
        return __ldb_base;
    }
}


val_debug *findInValBase(void *address)
{
    val_debug *tmp = __ldb_base_start;

    while(tmp)
    {
        if(tmp->address == address) return tmp;
        tmp = tmp->next;
    }
    return 0;
}


int rmFromValBase(val_debug *v)
{
    if(!v) return -1;
    if(__ldb_base_start == v)
    {
        __ldb_base_start = v->next;
        if(__ldb_base_start) __ldb_base_start->prev = 0;
        _free(v);
    }
    else
    {
        if(v->prev) ((val_debug*)v->prev)->next = v->next;
        if(v->next) ((val_debug*)v->next)->prev = v->prev;
        _free(v);
    }
    return 1;
}


void clearValBase()
{
    val_debug *tmp = __ldb_base_start;

    while(tmp)
    {
        fprintf(stderr, "exit: WARNING[%s:%d] it is maybe %d bytes leak!\n", tmp->file, tmp->line, tmp->size);
        rmFromValBase(tmp);
        tmp = __ldb_base_start;
    }
}


void *val_malloc(size_t sz, const char *file, int line)
{
    void *data = _malloc(sz);

    val_debug *db = getNextStruct();
    fprintf(stderr, "malloc:\n"
            "   Address: 0x%X\n"
            "   Size: %d\n"
            "   File: %s\n"
            "   Line: %d\n\n",
            data, sz, file, line);

    db->address = data;
    db->size = sz;
    db->line = line;
    strcpy(db->file, file);
    return data;
}


void *val_calloc(size_t nmemb, size_t size, const char *file, int line)
{
    void *data = _calloc(nmemb, size);

    size_t sz = nmemb * size;
    val_debug *db = getNextStruct();
    fprintf(stderr, "calloc(%d, %d):\n"
            "   Address: 0x%X\n"
            "   Size: %d\n"
            "   File: %s\n"
            "   Line: %d\n\n",
            nmemb, size, data, sz, file, line);

    db->address = data;
    db->size = sz;
    db->line = line;
    strcpy(db->file, file);
    return data;
}


void *val_realloc(void *adr, size_t newsz, const char *file, int line)
{
    void *data = _realloc(adr, newsz);
    val_debug *db;

    if(adr)
    {
        db = findInValBase(adr);
        if(!db)
        {
nuldb:
            fprintf(stderr, "realloc: WARNING[%s:%d] realloc address 0x%X not found in base\n",
                    file, line, adr);
            db = getNextStruct();
            db->line = 0;
            db->size = 0;
            db->address = 0;
            db->file[0] = 0x20;
            db->file[1] = 0;
        }
    }
    else
    {
        goto nuldb;
    }

    fprintf(stderr, "realloc:\n"
            "   Old alloc:"
            "       Address: 0x%X\n"
            "       Size: %d\n"
            "       File: %s\n"
            "       Line: %d\n"
            "   Realloc to:"
            "       Address: 0x%X\n"
            "       Size: %d\n"
            "       File: %s\n"
            "       Line: %d\n\n",
            adr, db->size, db->file, db->line,
            data, newsz, file, line);

    db->address = data;
    db->size = newsz;
    db->line = line;
    strcpy(db->file, file);
    return data;
}


void val_free(void *data, const char *file, int line)
{
    val_debug *db = 0;
    if(data)
    {
        db = findInValBase(data);
        if(!db)
        {
            fprintf(stderr, "free: WARNING[%s:%d] free address 0x%X not found in base\n",
                    file, line, data);
        }
        else
        {
            rmFromValBase(db);
        }
    }
    else
    {
        fprintf(stderr, "free: WARNING[%s:%d] delete address is zero!\n", file, line);
        return;
    }

    fprintf(stderr, "free:\n"
            "   Address: 0x%X\n"
            "   File: %s\n"
            "   Line: %d\n\n",
            data, file, line);
    _free(data);
}


