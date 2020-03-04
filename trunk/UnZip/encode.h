#ifndef _ENCODE_H
#define _ENCODE_H

unsigned int char8to16(int c);
unsigned int char16to8(unsigned int c, int type);

void RusTag(WSHDR *ws1);
void Win1251(WSHDR *ws1);

void dos2ws(WSHDR* ws, const char* s);
void dos2utf8(const char* s);
void dos2utf8_2(char* s, char *utf8);

void ws2char8(WSHDR* ws, char *p, int maxlen, int type);
void uni2dos(char* s, char *p, int maxlen);

int utf82filename(char *str, char *dest);


#endif
