#ifndef _7ZSLIB_H
#define _7ZSLIB_H

#include "inc/arch.h"


int IsIt7ZipFile(char *fname);
int OpenTab7Z(int tab, char * zipFileName);
int Fill7ZFiles(int tab, char * subdname);
void CloseTab7Z(int tab);
int Open7ZFile(ARCHINFO* arc, char * zipFileName);
void Close7Z(ARCHINFO* arc);
int ExtractFileByID7Z(ARCHINFO* pzi, int id, char * extractDir, int usePaths, int ip);
int ExtractFile7Z(ARCHINFO* pzi, char * fname, char * extractDir, int usePaths);
int ExtractDir7Z(ARCHINFO* pzi, char * dname, char * extractDir, int usePaths);

int _Un7Z(void);

#endif
