#include "..\inc\swilib.h"
#include "bookmark.h"
/*
===============================
           ÊéÇ©
===============================
*/
int openBMKFile(char *filename, unsigned int *ul, int st) {
  char *ext = NULL;
  char *bmk = filename;
  while ((bmk = strrchr(bmk, '.'))) {
    if (bmk != ext) ext = bmk;
    else break;
  }
  if (!ext) ext = filename;
  
  bmk = malloc(ext - filename + 5);
  strncpy(bmk, filename, ext - filename);
  bmk[ext - filename] = '\0';
  strcat(bmk, ".bmk");
  unsigned int attrib;
  attrib = A_BIN;
  if (st) attrib += A_ReadWrite + A_Create + A_Truncate;
  else attrib += A_ReadOnly;
  return fopen(bmk, attrib, P_READ + P_WRITE, ul);
}

int* readAddrInt(char *filename, int *bmkfd, unsigned int *ul, int *len, int flag) {      //¶ÁÈ¡ÊéÇ©
	*len = 0;
	int* result = malloc(BOOK_NUM);
	if (*bmkfd > 0) fclose(*bmkfd, ul);
	*bmkfd = openBMKFile(filename, ul, 0);
	
	if (*bmkfd < 0 && flag == 1) {
		*bmkfd = openBMKFile(filename, ul, 1);
	}
	
	if (*bmkfd < 0) return result;
	char* buff = malloc(BOOK_NUM << 2);
  int bmk = 0, offset = 0, read = 0;
  while (lseek(*bmkfd, offset, S_SET, ul, ul) == offset) {
    if ((read = fread(*bmkfd, buff, BOOK_NUM, ul)) > 0) {
      for (int i = 0; i < read;) {
        bmk = (buff[i + 3] << 24) | buff[i + 2] << 16 | buff[i + 1] << 8 | buff[i];
        result[i >> 2] = bmk;
        i += 4;
      }
    }
    offset += (BOOK_NUM << 2);
    *len += (read >> 2);
  }
  mfree(buff);
  fclose(*bmkfd, ul);
  *bmkfd = -1;
  return result;
}

int writeAddrInt(char *filename, int *bmkfd, unsigned int *ul, int *bmk, int len, int flag) {
	if (flag == 2) return 0;
	if (*bmkfd <= 0) *bmkfd = openBMKFile(filename, ul, 1);
	if (*bmkfd <= 0) return 0;
	int bufflen = len << 2;
	char* buff = malloc(bufflen);
	int j = 0;
	for (int i = 0; i < bufflen;) {
		if (j < len) {
			buff[i++] = (char) (bmk[j] & 0xff);
			buff[i++] = (char) ((bmk[j] >> 8) & 0xff);
			buff[i++] = (char) ((bmk[j] >> 16) & 0xff);
			buff[i++] = (char) ((bmk[j++] >> 24) & 0xff);
		}
		else
			break;
	}
	lseek(*bmkfd, 0, S_SET, ul, ul);
	
  if (fwrite(*bmkfd, buff, bufflen, ul)) {
  	mfree(buff);
  	return 1;
  }
  mfree(buff);
  return 0;
}
/*
===============================
            end
===============================
*/
