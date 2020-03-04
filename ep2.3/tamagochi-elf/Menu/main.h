#ifndef _MAIN_H_
  #define _MAIN_H_
#include "..\inc\swilib.h"
int strncmpNoCase(const char *s1,const char *s2,unsigned int n);
int strcmp_nocase(const char *s1,const char *s2);

void EditorWString(WSHDR * const fnamews,WSHDR * wsFName,unsigned short begin,unsigned short after,short position,short position2,unsigned short byposition,unsigned int wchar,unsigned int w2char);

void EditorString(const char * fname,char * FName,unsigned short begin,unsigned short after,short position,short position2,unsigned short byposition,unsigned int wchar,unsigned int w2char);

#endif
