#include "..\inc\swilib.h"
#include "..\lib.h"

const int LibID = 3;
extern void itoa(int n, char s[]);

TYPEExecStruct execparam;
TYPEParamStruct *param;

char *text; 

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

void CloseLib()
{
  zeromem(text, 128);
  mfree(text);
  SUBPROC((void *)ElfKiller);   
}

char *IntToStr(int i)
{
  zeromem(text, 128);
  itoa(i,text);
  return text;
}

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* записываем знак */
        n = -n;          /* делаем n положительным числом */
    i = 0;
    do {       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0';   /* берем следующую цифру */
    } while ((n /= 10) > 0);     /* удаляем */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    param->reverse(s);
} 

int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (int k=0; ; k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A) {ret=ret*10+attr[k]-0x30;} else { if ( attr[k]=='-') {neg=-1;} else {return(ret*neg);}}
  }
}

int main(char *exe, TYPEParamStruct *in)
{
  text = malloc(128);
  param = in;
  execparam.LibID = LibID;
  execparam.CloseLib = *CloseLib;
  execparam.itoa = *itoa;
  execparam.atoi = *atoi;
  execparam.IntToStr = *IntToStr;
  param->Struct = &execparam;
  
  return (0);
}
