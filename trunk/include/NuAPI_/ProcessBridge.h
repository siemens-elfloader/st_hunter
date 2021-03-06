

#ifndef __PROCESS_BRIDGE_H__
#define __PROCESS_BRIDGE_H__

#include <swilib.h>
#include <nu_swilib.h>
#include <ep3/loader.h>

#define BridgeMOPI_ID 0x4409    // � �� ����� �����

#define MOPI_THREAD_PROC 0x1
#define NU_SYNCHRONIZED_PROC 0x13
#define NU_ASYNC_PROC 0x14


typedef struct
{
    NU_TASK *task;
    int sync;
    int loked;
    void *ret;
}ThreadInfo;

/* arguments manipulation code */

#define collect_args(args) \
            __free(args)



void * pack_args(int argc, ...);
uint32_t unpack_args(void *_data, void ***_args);
void *BridgeMessageSend(NU_TASK *task, void *func_ptr, int type, void *packed_args);


void _sync_ShowMSG(int type, int lang);
void _async_ShowMSG(int type, int lang);


int sync_open(const char * cFileName, unsigned int iFileFlags, unsigned int iFileMode, unsigned int *ErrorNumber);
int sync_read(int FileHandler, void *cBuffer, int iByteCount, unsigned int *ErrorNumber);
int sync_write(int FileHandler, void const * cBuffer, int iByteCount, unsigned int *ErrorNumber);
int sync_close(int FileHandler, unsigned int *ErrorNumber);
int sync_flush(int FileHandler, unsigned int *ErrorNumber);
int sync_truncate(int FileHandler, int length, unsigned int *errornumber);

int sync_unlink(const char *cFileName, unsigned int *errornumber);
int sync_lseek(int FileHandler, unsigned int offset, unsigned int origin, unsigned int *ErrorNumber, unsigned int *ErrorNumber2);

int sync_mkdir(const char * cDirectory, unsigned int *ErrorNumber);
int sync_rmdir(const char * cDirectory, unsigned int *ErrorNumber);
int sync_isdir(const char * cDirectory, unsigned int *ErrorNumber);

int sync_FindFirstFile(DIR_ENTRY *DIRENTRY, const char *mask, unsigned int *ErrorNumber);
int sync_FindNextFile(DIR_ENTRY *DIRENTRY, unsigned int *ErrorNumber);
int sync_FindClose(DIR_ENTRY *DIRENTRY, unsigned int *ErrorNumber);

int sync_GetFileStats(const char *cFileName, FSTATS * StatBuffer, unsigned int *errornumber);
int sync_GetFileAttrib(const char *cFileName, unsigned char *cAttribute, unsigned int *ErrorNumber);
int sync_SetFileAttrib(const char *cFileName, unsigned char cAttribute, unsigned int *ErrorNumber);

IMGHDR *sync_CreateIMGHDRFromPngFile(const char *file, int type);
int sync_setfilesize(int FileHandler, unsigned int iNewFileSize, unsigned int *ErrorNumber);
int sync_fmove(const char * SourceFileName, const char * DestFileName, unsigned int *ErrorNumber);

Elf32_Exec* sync_elfopen(const char* filename);
int sync_ExecuteFile(WSHDR *file, WSHDR *mime, void *d);

void sync_DirectRedrawGUI();
void sync_DrawRectangle(int x,int y,int x2,int y2,int flags,const char *pen,const char *brush);
#endif

