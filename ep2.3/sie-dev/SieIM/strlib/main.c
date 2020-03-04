#include "..\inc\swilib.h"
#include "..\lib.h"
#include "..\lang.h"
#include "stdio.h"
#include "stdarg.h"

const int LibID = 0;

STRExecStruct execparam;
STRParamStruct *param;

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

void CloseLib()
{
  SUBPROC((void *)ElfKiller);
}

void WriteStr(char * msg, char * fn)  // Запись в файл
{
  int f;
  unsigned err;
  
  if ((f=fopen(fn,A_WriteOnly+A_BIN+A_Create+A_Append,P_WRITE,&err))!=-1)
  {
    fwrite(f,msg,strlen(msg),&err);
    fclose(f,&err);
  }   
 
}

int strcmp_nocase(const char *s, const char *d) // Сравнивание строчек без учета регистра
{
  int cs;
  int ds;
  do
  {
    cs=*s++;
    if (cs&0x40) cs&=0xDF;
    ds=*d++;
    if (ds&0x40) ds&=0xDF;
    cs-=ds;
    if (cs) break;
  }
  while(ds);
  return(cs);
}

char *MakeGlobalString(const char *first, int breakchar, const char *last) //хз, короче нужно
{
  int l;
  char *p;
  p=malloc((l=strlen(first))+strlen(last)+2);
  strcpy(p,first);
  if (breakchar)
    p[l++]=breakchar;
  strcpy(p+l,last);
  return p;
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void vsprintf2(char *text, const char *msg, va_list param)
{
  vsprintf(text,msg,param);
}

int main(char *exe, STRParamStruct *in)
{
  param = in;
  execparam.LibID = LibID;
  execparam.CloseLib = *CloseLib;
  execparam.vsprintf2 = *vsprintf2;
  execparam.reverse = *reverse;
  execparam.MakeGlobalString = *MakeGlobalString;
  execparam.strcmp_nocase = *strcmp_nocase;
  execparam.InitLgp = *InitLgp;
  execparam.InitLgp_2 = *InitLgp_2;
  execparam.CreateLgp = *CreateLgp;
  execparam.CreateLgp_2 = *CreateLgp_2;
  execparam.GetLgpInfo = *GetLgpInfo;
  execparam.WriteStr = *WriteStr;
  param->Struct = &execparam;
  return (0);
}
