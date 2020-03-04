
#ifndef __EXT_ARRAY_H__
#define __EXT_ARRAY_H__

#include <swilib.h>
#include <string.h>
#include <stdlib.h>
#include <NuAPI/ExtMutex.h>



typedef struct
{
    void **array;
    size_t size,
           cached;

    void *empty_fill;

    ExtMutex mutex;
    void *(*malloc)(size_t);
    void (*free)(void *);
    void *(*realloc)(void *, size_t);
}ExtArray;

#define extarray_foreach(type, value, _array, iterator)           \
    if((size_t)iterator < (_array)->size)                           \
        for( value = (type)((_array)->array[iterator]);               \
             (size_t)iterator < (_array)->size;                     \
             ++iterator, value = (type)((_array)->array[iterator]))

void extarray_init(ExtArray *array, void *empty_cell_fill,
                   void *(*malloc)(size_t),
                   void (*free)(void *),
                   void *(*realloc)(void *, size_t)
                   );
void extarray_release(ExtArray *array);
int extarray_reserve(ExtArray *array, size_t reserve);
int extarray_push_back(ExtArray *array, void *val);
int extarray_pop_back(ExtArray *array);
int extarray_store_cell(ExtArray *array, size_t cell, void *val);
int extarray_adjust_by_size(ExtArray *array);
int extarray_clear(ExtArray *array, void *clear);

int extarray_lock(ExtArray *array);
int extarray_unlock(ExtArray *array);


static inline void *
extarray_cell(ExtArray *array, unsigned int cell)
{
    if(cell >= array->size) return 0;

    return array->array[cell];
}


#endif /* __EXT_ARRAY_H__ */
