
#ifndef __NU_IFACE_LIB_H__
#define __NU_IFACE_LIB_H__

#include <swilib.h>

IMGHDR *NuCreateIMGHDRFromPng(const char *file, int type);
void NuIMGHDRClear(IMGHDR *);

WSHDR *NuAllocWS(size_t size);
void NuFreeWS(WSHDR *ws);

int dir_findfirst(DIR_ENTRY *d, char *mask, unsigned int *ErrorNumber);
int dir_findnext(DIR_ENTRY *d, unsigned int *ErrorNumber);
int dir_findclose(DIR_ENTRY *d, unsigned int *ErrorNumber);


#endif
