#ifndef _ARCH_H
#define _ARCH_H

#include "../7Z/7zHeader.h"
#include "../7Z/7zCrc.h"
#include "../7Z/7zIn.h"
#include "../7Z/7zExtract.h"

#define MAX_PATH_INZIP  (256)

#define NO_ARCH 0
#define ZIP_ARCH 1
#define _7Z_ARCH 2

typedef struct 
{
  unzFile uf;
  unz_global_info gi;
  unz_file_info** pfi;
}zipinfo;

typedef struct 
{
  ISzInStream InStream;
  int File;
} CFileInStream;
    
typedef struct 
{
  CFileInStream archiveStream;
  ISzAlloc allocImp;
  ISzAlloc allocTempImp;
  CArchiveDatabaseEx db;
} _7zinfo;
    
typedef struct
{
  int type;
  char szZipPath[MAX_PATH];
  char szCurDir[MAX_PATH];
  char ** pszNames;
  int total_names;
  char* password;
  union 
  {
    zipinfo zip;
    _7zinfo _7z;
  }info;
} ARCHINFO;

typedef struct
{
  char destname[MAX_PATH];
  char password[32];
  int level;
}ZPARAMS;

extern ARCHINFO zi;

void FreeArchInfo(ARCHINFO* pzi);
int IsArchiveOpened(int tab);
int IsInArchive();

void ZeroArchiveBuffer();
void ArchiveBufferExtractBegin();
int ArchiveBufferExtract(int type, FN_ITM* pi, char * extractDir);
void ArchiveBufferExtractEnd();

int IsItArh(char* fname);

#endif
