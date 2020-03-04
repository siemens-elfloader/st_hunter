#ifndef _ZSLIB_H
#define _ZSLIB_H

#define CASESENSITIVITY (0)
#define MAX_PATH_INZIP  (256)
#define WRITEBUFFERSIZE (0x4000)
#define LOCALHEADERMAGIC (0x04034b50)

// Common functions
void FreeZipInfo(ARCHINFO* pzi);
int ReadZipInfo(ARCHINFO* pzi);
int OpenZip(ARCHINFO* pzi, char* zipFileName);
void CloseZip(ARCHINFO* pzi);
int ExtractCurrentFile(ARCHINFO* pzi, int ind, char* extractDir, int usePaths, int ip);
int ExtractFile(ARCHINFO* pzi, char* fname, char* extractDir, int usePaths);
int ExtractFileByID(ARCHINFO* pzi, int id, char* extractDir, int usePaths, int ip);
int ExtractDir(ARCHINFO* pzi, char* dname, char* extractDir, int usePaths);

// Tab functions

int IsZipOpened(int tab);
int IsInZip();

int OpenTabZip(int tab, char* zipFileName);
void CloseTabZip(int tab);
int FillZipFiles(int tab, char* subdname);

// Buffer functions
void ZipBufferExtractBegin();
int ZipBufferExtract(FN_ITM* pi, char* extractDir);
void ZipBufferExtractEnd();

int EnumZipFiles(ARCHINFO* pzi, char* subdname, ENUM_SEL_PROC enumproc, unsigned int param);
int IsItZipFile(char* fname);
int _UnZip(void);

#endif
