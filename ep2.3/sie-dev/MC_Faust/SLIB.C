#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\gui.h"
#include "..\inc\siemens_unicode.h"

const char _fullnull[] = "";
const char _null[] = " ";
const char _s[] = "%s";
const char _d[] = "%d";
const char _s_stars[] = "%s\\*.*";
const char _s_sbcfg[] = "%s\\%s.bcfg";
const char _s_s[] = "%s\\%s";
const char _s_s_d[] = "%s\\%s.%d";
const char _ss[] = "%s: %s";
const char _t[] = "%t";
const char _tc[] = "%t: ";
const char _d_t[] = "%d %t";
const char _s_i_s[] = "%s(%i).%s";

int isNumericStr(char* s)
{
 // Внимание! пустую строку тоже считаем числовой
 for (int i = 0; s[i]; i++)
  if (s[i] < '0' || s[i] > '9') return 0;

 return 1;
}

// Замена символов
char* strreplace(char* s, char cprev, char cnew)
{
 for (int i = 0; s[i]; s++)
  if (s[i] == cprev) s[i] = cnew;
 return s;
}

char* strtolower(char* src, char* dst, int sz)
{
 int sl = strlen(src)+1;
 int len = sz==-1?sl:sz;
 if (len>sl) len=sl;

 for(int ii=0; ii<len-1; ii++)
 {
  int ch = src[ii];

  if (ch>='A' && ch<='Z') ch=ch-'A'+'a';

  dst[ii]=ch;
 }
 dst[len-1]=0;
 return dst;
}

char* str2lower(char *st)
{
  unsigned int len = strlen(st);
  for(int i=0;i<len;i++)
  {
    char sym = *(st+i);
    if(sym<0x80)
    {
      if(st[i]>='A' && st[i]<='Z') st[i] += ('a' - 'A');
      continue;
    }
    if(sym<0xe0)
    {
      i++;
      if(st[i] >= 0x90 && st[i]<=0xAF) st[i]+= (0xB0 - 0x90);
    }
  }
  return st;
}

// Аналог strcmp, но без чувствительности к регистру
int stricmp(char* str1, char* str2)
{
 return strnicmp(str1, str2, 1000); // тысячи хватит на сравнение, надеюсь
}

int strnicmp(char* str1, char* str2, int count)
{
 for(int i=0; i<count; i++)
 {
  unsigned char c1 = str1[i];
  unsigned char c2 = str2[i];

  if (c1 >= 'A' && c1 <= 'Z') c1 = c1 - 'A' + 'a';
  if (c2 >= 'A' && c2 <= 'Z') c2 = c2 - 'A' + 'a';
  if (c1 < c2) return -1;
  if (c1 > c2) return +1;
  if (c1 == 0) return 0;
 }
 return 0;
}

/******************************************************************************/
short wcharlow2(short wc)
{
 if (wc>=0x0041 && wc<=0x005a) return wc=wc+(0x0061-0x0041); //A-Z
 if (wc>=0x0410 && wc<=0x042f) return wc=wc+(0x0430-0x0410); //А-Я
 if (wc>=0x0400 && wc<=0x040f) return wc=wc+(0x0450-0x0400);

 if (wc>=0x00c0 && wc<=0x00de && wc!=0x00d7) return wc=wc+(0x00e0-0x00c0);

 if (wc>=0x0100 && wc<=0x0136 && ~(wc & 1)) return wc=wc+1;
 if (wc>=0x0139 && wc<=0x0147 &&  (wc & 1)) return wc=wc+1;
 if (wc>=0x014a && wc<=0x0176 && ~(wc & 1)) return wc=wc+1;
 if (wc>=0x0179 && wc<=0x017d &&  (wc & 1)) return wc=wc+1;
 if (wc>=0x01e4 && wc<=0x01ea && ~(wc & 1)) return wc=wc+1;

 if (wc>=0x01fa && wc<=0x01fe && ~(wc & 1)) return wc=wc+1;

 if (wc>=0x0490 && wc<=0x04e8 && ~(wc & 1)) return wc=wc+1;

 if (wc==0x01a0 || wc==0x01af || wc==0x017d ||
  wc==0x01ee || wc==0x0218 || wc==0x021a || wc==0x0228) return wc=wc+1;

 if (wc==0x0178) return wc=0x00ff;
 if (wc==0x018f) return wc=0x0259;
 if (wc==0x01b7) return wc=0x0292;
 return wc;
}

int wstrcmpi2(WSHDR* ws1, WSHDR* ws2)
{
 int len = ws1->wsbody[0];
 if (len<ws2->wsbody[0]) return -1;

 short wc1, wc2;
 int ii=1;
 for(int cc=1;cc<=len;cc++)
 {
  wc1 = wcharlow2(ws1->wsbody[cc]);
  wc2 = wcharlow2(ws2->wsbody[ii]);
  
  if(wc1==wc2)
  {
    if(ws2->wsbody[0]<ii)
      return 1;
    else
      ii++;
  }else
    ii=1;
  
 }
 return 0;
}

int stristr(char *s, char *find)
{
  WSHDR *ws=AllocWS(strlen(s)+1);
  WSHDR *ws_find=AllocWS(strlen(find)+1);
  
  str_2ws(ws, s, strlen(s));
  str_2ws(ws_find, find, strlen(find));
  
  if(wstrcmpi2(ws, ws_find)==1)
  {
    FreeWS(ws);
    FreeWS(ws_find);
    return 1;
  }
  
  FreeWS(ws);
  FreeWS(ws_find);
  return 0;
}

/******************************************************************************/
// Аналог strcmp, но без чувствительности к регистру и сравнивае с конца
int strircmp(char* str1, char* str2)
{
 return strnircmp(str1, str2, 1000); // тысячи хватит на сравнение, надеюсь
}

int strnircmp(char* str1, char* str2, int count)
{
 int sl1 = strlen(str1);
 int sl2 = strlen(str2);
 for(int i=1; i<=count; i++)
 {
  if (sl1-i < 0 || sl2-i < 0) return 1;

  unsigned char c1 = str1[sl1 - i];
  unsigned char c2 = str2[sl2 - i];

  if (c1 >= 'A' && c1 <= 'Z') c1 = c1 - 'A' + 'a';
  if (c2 >= 'A' && c2 <= 'Z') c2 = c2 - 'A' + 'a';
  if (c1 < c2) return -1;
  if (c1 > c2) return +1;
 }
 return 0;
}

short wcharlow(short wc)
{
 if (wc>=0x0041 && wc<=0x005a) return wc+(0x0061-0x0041); //A-Z
 if (wc>=0x0410 && wc<=0x042f) return wc+(0x0430-0x0410); //А-Я
 if (wc>=0x0400 && wc<=0x040f) return wc+(0x0450-0x0400);

 if (wc>=0x00c0 && wc<=0x00de && wc!=0x00d7) return wc+(0x00e0-0x00c0);

 if (wc>=0x0100 && wc<=0x0136 && ~(wc & 1)) return wc+1;
 if (wc>=0x0139 && wc<=0x0147 &&  (wc & 1)) return wc+1;
 if (wc>=0x014a && wc<=0x0176 && ~(wc & 1)) return wc+1;
 if (wc>=0x0179 && wc<=0x017d &&  (wc & 1)) return wc+1;
 if (wc>=0x01e4 && wc<=0x01ea && ~(wc & 1)) return wc+1;

 if (wc>=0x01fa && wc<=0x01fe && ~(wc & 1)) return wc+1;

 if (wc>=0x0490 && wc<=0x04e8 && ~(wc & 1)) return wc+1;

 if (wc==0x01a0 || wc==0x01af || wc==0x017d ||
  wc==0x01ee || wc==0x0218 || wc==0x021a || wc==0x0228) return wc+1;

 if (wc==0x0178) return 0x00ff;
 if (wc==0x018f) return 0x0259;
 if (wc==0x01b7) return 0x0292;
 return wc;
}

int wstrcmpi(WSHDR* ws1, WSHDR* ws2)
{
 int len = ws1->wsbody[0];
 if (len<ws2->wsbody[0]) return -1;
 else if (len>ws2->wsbody[0]) return +1;

 short wc1, wc2;
 for(int cc=1;cc<=len;cc++)
 {
  wc1 = wcharlow(ws1->wsbody[cc]);
  wc2 = wcharlow(ws2->wsbody[cc]);
  if (wc1<wc2) return -1;
  else if (wc1>wc2) return +1;
 }
 return 0;
}

char *sz2s(unsigned int size, char *buf)
{
 float s = size;
 int type;
 type=ind_byte;
 if (s >= 1024)
 {
  s /= 1024;
  type=ind_kbyte;
  if (s >= 1024)
  {
   s /= 1024;
   type=ind_mbyte;
   if (s >= 1024)
   {
    s /= 1024;
    type=ind_gbyte;
   }
  }
  sprintf(buf, "%.2f %s", s, muitxt(type));
 }
 else
  sprintf(buf, "%d %s", size, muitxt(type));
 return buf;
}

char *attr2s(int attr, char *buf)
{

 buf[0] = attr & FA_READONLY ? 'R' : '-';
 buf[1] = attr & FA_ARCHIVE ? 'A' : '-';
 buf[2] = attr & FA_HIDDEN ? 'H' : '-';
 buf[3] = attr & FA_SYSTEM ? 'S' : '-';
 buf[4] = 0;
 return buf;
}

char *fdt2s(unsigned int time, char *buf)
{
 short y,M,d,h,m;
 y = (time >> 25) + 80;
 if (y>100) y -= 100;
 M = (time >> 21) & 0x0f;
 d = (time >> 16) & 0x1f;

 h = (time >> 11) & 0x1f;
 m = (time >> 5)  & 0x3f;

 sprintf(buf, "%.2d.%.2d.%.2d %.2d:%.2d", d,M,y, h,m);
 return buf;
}

char *fd2s(unsigned int time, char *buf)
{
 short y,M,d;
 y = (time >> 25) + 80;
 if (y>100) y -= 100;
 M = (time >> 21) & 0x0f;
 d = (time >> 16) & 0x1f;

 sprintf(buf, "%.2d.%.2d.%.2d", d,M,y);
 return buf;
}

char *ft2s(unsigned int time, char *buf)
{
 short h,m;

 h = (time >> 11) & 0x1f;
 m = (time >> 5)  & 0x3f;

 sprintf(buf, "%.2d:%.2d", h,m);
 return buf;
}

int EnumIni(int local, char *ininame, INIPROC proc)
{
 int res=0;
 char *buf;
 char *cfg;
 int ch;
 int p=0;
 char name[MAX_INIBUF];
 char value[MAX_INIBUF];

 unsigned int size_cfg;

 char* fn = local ? MCFilePath(ininame) : ininame;
 int f;
 if (name && value && fn && (f = fopen(pathbuf, A_ReadOnly, 0, &err)) != -1)
 {
  size_cfg = lseek(f,0,S_END,&err,&err);
  lseek(f, 0, S_SET, &err, &err);
  cfg = buf = malloc(size_cfg + 1);
  if (cfg)
  {
   size_cfg = fread(f, buf, size_cfg, &err);
   buf[size_cfg] = 0;
   do
   {
    // Камент
    if (*buf == ';')
    {
     while( ((ch = *buf++) >= 32));
     if (!ch) break;
     else continue;
    }

    // Название
    p=0;
    while( (ch = *buf++) != '=')
    {
     if (ch < 32) break;
     if (p < MAX_INIBUF-1) name[p++] = ch;
    }
    if (ch < 32) continue;
    name[p]=0;

    // Значение
    p=0;
    while( (ch = *buf++) >= 32 )
    {
     if (p < MAX_INIBUF-1) value[p++] = ch;
    }
    value[p]=0;

    if (proc)
     proc(name, value);
   }
   while (ch);
  }
  mfree(cfg);
  fclose(f,&err);
  res = 1;
 }
 return res;
}

void nricp(char* src, char* dst)
{
 *(dst+0)=*(src+0);
 *(dst+1)=*(src+1);
 *(dst+2)=*(src+2);
 *(dst+3)=*(src+3);
}

int GetStrCount(WSHDR *prop,int width, int font)
{
 int sc=1;
 int strw=0;
 int perii=0;
 int font_opt=0;
 
 for(int ii=1;ii<prop->wsbody[0];ii++)
 {
  if(prop->wsbody[ii]==UTF16_FONT_SMALL) 
    font_opt=0;
  
  if(prop->wsbody[ii]==UTF16_FONT_SMALL_BOLD) 
    font_opt=1;
   
  if(prop->wsbody[ii]=='\n')
  {
    sc++;
    strw=0;
    perii=0;
    
    continue;
    
  }else
    strw+=GetSymbolWidth(prop->wsbody[ii], font+font_opt);
  
  if(prop->wsbody[ii]==' ')
    perii=ii;
  
  if(strw>width)
  {
    if(perii!=0)
    {
      prop->wsbody[perii]='\n';
      if(font_opt==1)
        wsInsertChar(prop,UTF16_FONT_SMALL_BOLD,perii+1); 
    }
    else
    {
      if(font_opt==1)wsInsertChar(prop,UTF16_FONT_SMALL_BOLD,ii); 
      wsInsertChar(prop,'\n',ii);
    }
    
    sc++;
    strw=0;
  }  
 }

 return sc;
}
