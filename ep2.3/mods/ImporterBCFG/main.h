#ifndef _MAIN_H_
#define _MAIN_H_

void CreateLogScreen(void *data);
void Parser();


extern void OpenFile(char *fname,int flag);
extern long  strtol (const char *nptr,char **endptr,int base);
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
extern int SelectFile(int i);

#endif
