
#ifndef __ASYNC_PRINT_H__
#define __ASYNC_PRINT_H__

#include <stdlib.h>
#include <swilib.h>

#ifdef __cplusplus
extern "C" {
#endif


extern char _debug_data[4096];
void print(int, const char *);
void printLock();
void printUnLock();


#ifndef printf
#define printf(fmt, ...) { \
 printLock(); int __sz__ = sprintf(_debug_data, fmt, ##__VA_ARGS__); print(__sz__, _debug_data); printUnLock(); }
#endif

#ifndef printf_nlock
#define printf_nlock(fmt, ...) { \
int __sz__ = sprintf(_debug_data, fmt, ##__VA_ARGS__); print(__sz__, _debug_data); }
#endif



#ifdef __cplusplus
}
#endif


#endif

