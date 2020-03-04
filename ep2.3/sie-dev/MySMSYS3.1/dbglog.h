#ifndef _DBGLOG_H_
#define _DBGLOG_H_

#ifdef DEBUG

#define PATH "0:\\MySMSYS3.1.log"

extern void WriteDebug(char * msg);
extern void WriteDebug(int msg);
extern void WriteStr(char * msg, char * fn);
extern char *GetDateTimeNow(void);
extern char *IntToStr(int i);
extern void InitDBG();
extern void DestrDBG();
#endif

#endif
