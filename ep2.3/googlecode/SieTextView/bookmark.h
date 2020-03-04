#ifndef _BOOKMARK_H_
#define _BOOKMARK_H_

#define BOOK_NUM 100
int openBMKFile(char *filename, unsigned int *ul, int st);
int* readAddrInt(char *filename, int *bmkfd, unsigned int *ul, int *len, int flag);
int writeAddrInt(char *filename, int* bmkfd, unsigned int *ul, int *bmk, int len, int flag);
#endif
