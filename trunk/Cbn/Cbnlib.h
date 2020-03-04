//Release 2.10.2006

#ifndef _CBNLIB_H
#define _CBNLIB_H

/////////////mylib.h////////


//#ifdef NEWSGOLD
//#else
int fread32(int fh, void *buf, int len, unsigned int *err);
int fwrite32(int fh, void *buf, int len, unsigned int *err);
//#endif

//#ifdef GRAPH
char *loadgraph(char *filename); // Загрузить графику (распаковываем и перекодируем) #define HIGHCOLOR учесть
//#endif





int iszero(void *mem, int dsize);

int savefile(char *fname, char *buf, int size);
char *loadfile(char *filename); // Загрузить файл, распаковать, длина в loadfilesize
//int fgets(char *buf,int size,int fh);
//char toupper(char c);
char *toup(char *str);			// К верхнему регистру
char *todown(char *str);		// К нижнему регистру
int  isalth (unsigned char sym);	// Алфавитный символ ?
int  isval (char sym);		// Символ является числом ?
int strchrpos(const char *s, int c);	// Позиция символа в строке (1-N, 0=нет)
int emptystr(char *str);		// Пустая ли строка
char  *alltrim (char *str);		// Удалить пробелы по бокам строки
int   strcmpsize (const char *s1,const char *s2,int size); // Сравнить строки длины size
int getint(void *ptr); // Получить int
short getshort(void *ptr); // Получить short
void putint(void *ptr, int data); // Записать int
void putshort(void *ptr, short data); // Записать short
void bitblt(void *screen, void *bmp, int x, int y, int sizex, int sizey, int x0, int y0,  int colormask, int flags);
char *loadgraph(char *filename); // Загрузить графику (распаковываем и перекодируем)


#endif
