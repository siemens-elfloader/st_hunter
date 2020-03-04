#ifndef _LANG_H
#define _LANG_H

#include "lib.h"

extern int InitLgp(char *lang, void *lng);	// если ленг структура
extern int InitLgp_2(char *lang, void **array);	// если ленг массив
extern int CreateLgp(char *lang,  int build, int type, int count, int size, char *cright, void *lng);	// если структура
extern int CreateLgp_2(char *lang,  int build, int type, int count, int size, char *cright, void **array);	// если массив
extern int GetLgpInfo(char *lang, LANGH *head);	// получение информации о ленге

#endif
