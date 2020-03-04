
#ifndef __MEMCTL_H__
#define __MEMCTL_H__







void *memoryAlloc(int _pid, size_t size);
void *memoryRealloc(int _pid, void *_ptr, size_t size);
int memoryFree(int _pid, void *ptr);




#endif
