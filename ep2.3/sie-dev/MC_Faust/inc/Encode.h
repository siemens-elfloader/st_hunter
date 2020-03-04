#ifndef _ENCODE_H
#define _ENCODE_H

void dos2ws(WSHDR *ws, const char *s);
void dos2utf8(const char* s);
int  utf82win(char* utf8);
int utf82filename(char *str, char *dest);

#endif
