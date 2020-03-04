#include "inc\mc.h"

unsigned int char8to16(int c)
{
  if (c < 128) return(c);
  
  if (c==0xF0) return 0x0401;
  if (c==0xF2) return 0x0404;
  if (c==0xF4) return 0x0404;
  if (c==0xF1) return 0x0451;
  if (c==0xF3) return 0x0454;
  if (c==0xF5) return 0x0457;
  if ((c>=0x80)&&(c<=0xAF)) return(c+0x390);
  if ((c>=0xE0)&&(c<=0xEF)) return(c+0x360);
  
  c&=0xFF;
  return(c);
}


unsigned int char16to8(unsigned int c, int type)
{
  if (c<32) return(NULL);
  if (c<128) return(c);

  if (c==0x0401) return 0xF0;
  if (c==0x0404) return 0xF2;
  if (c==0x0404) return 0xF4;
  if (c==0x0451) return 0xF1;
  if (c==0x0454) return 0xF3;
  if (c==0x0457) return 0xF5;
  if ((c>=0x0410)&&(c<=0x043F)) return(c-0x390);
  if ((c>=0x0440)&&(c<=0x044F)) return(c-0x360);
  
  c&=0xFF;
  if (c<32) return(NULL);
  return(c);
}

void RusTeg(WSHDR *ws1)
{
  //Патчим строку на предмет win1251 (c)Rst7
  int i=1;
  int c;
  while(i<=ws1->wsbody[0])
  {
    c=ws1->wsbody[i];
    if (c==0xA8) c=0x401;
    if (c==0xAA) c=0x404;
    if (c==0xAF) c=0x407;
    if (c==0xB8) c=0x451;
    if (c==0xBA) c=0x454;
    if (c==0xBF) c=0x457;
    if ((c>=0xC0)&&(c<0x100)) c+=0x350;
    ws1->wsbody[i++]=c;
  }
}

void Win1251(WSHDR *ws1)
{
  //Патчим строку на предмет UTF8
  int i=1;
  int c;
  while(i<=ws1->wsbody[0])
  {
    c=ws1->wsbody[i];
    if (c==0x401) c=0xA8;
    if (c==0x404) c=0xAA;
    if (c==0x407) c=0xAF;
    if (c==0x451) c=0xB8;
    if (c==0x454) c=0xBA;
    if (c==0x457) c=0xBF;
    if ((c>=0x410)&&(c<0x450)) c-=0x350;
    ws1->wsbody[i++]=c;
  }
}

void dos2ws(WSHDR* ws, const char* s)
{
	int c;
	CutWSTR(ws, 0);
	while((c = *s++))
		wsAppendChar(ws, char8to16(c));  
}  

void dos2utf8(const char* s)
{
	int len = strlen(s);
	WSHDR* ws = AllocWS(len);
	dos2ws(ws, s);
        ws_2str(ws, (char*)s, len*2);
	FreeWS(ws);
}

void dos2utf8_2(char* s, char *utf8)
{
	int len = strlen(s);
	WSHDR* ws = AllocWS(len);
	dos2ws(ws, s);
	ws_2str(ws, utf8, len*2);
	FreeWS(ws);
}

/////////////////////////////////////////

void ws2char8(WSHDR* ws, char *p, int maxlen, int type)
{
  int j=0;
  while(j<maxlen)
  {
    if (j>=ws->wsbody[0]) break;
    *p++=char16to8(ws->wsbody[j+1], type);
    j++;
  }
  *p=0;
}

void uni2dos(char* s, char *p, int maxlen)
{
  WSHDR* ws = AllocWS(maxlen);
  str_2ws(ws, s, maxlen);
  ws2char8(ws, p, maxlen, 1);
  FreeWS(ws);
}

//////////////////////////////////////////

int utf82win(char* utf8)
{
   str_2ws(wsbuf, utf8, MAX_PATH);
   Win1251(wsbuf);
   ws_2str(wsbuf, utf8, MAX_PATH);
   return 1;
}

int utf82filename(char *str, char *dest)
{
  int len = strlen(str)+32;
  WSHDR *ws = AllocWS(len+1);
  utf8_2ws(ws, str, len);
  ws_2str(ws, dest, len);
  FreeWS(ws);
  dest[len] = '\0';
  return len;
}
