
#ifndef __MEMCTL_H__
#define __MEMCTL_H__

#include <NuAPI/ExtendedNuTask.h>
#include <NuAPI/ExtList.h>


int MemCtl_push(ExtList *q, void *address, size_t size);
int MemCtl_pop(ExtList *q, void *address);
int MemCtl_replace(ExtList *q, void *from, void *to, int size);
int MemCtl_clean(ExtList *q);
size_t MemCtl_memory_used(ExtList *q);


void *MemCtl_heap_malloc(ExtTask *, size_t sz);
int MemCtl_heap_free(ExtTask *, void *ptr);
void *MemCtl_heap_calloc(ExtTask *, size_t count, size_t size);
void *MemCtl_heap_realloc(ExtTask *, void *ptr, size_t size);


#define MemCtl_malloc(sz)       MemCtl_heap_malloc(CurrentProcessTask(), sz)
#define MemCtl_free(ptr)        MemCtl_heap_free(CurrentProcessTask(), ptr)
#define MemCtl_calloc(cnt, sz)  MemCtl_heap_calloc(CurrentProcessTask(), cnt, sz)
#define MemCtl_realloc(ptr, sz) MemCtl_heap_realloc(CurrentProcessTask(), ptr, sz)

#endif
