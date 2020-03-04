#include "include.h"

#ifdef DEBUG
#include "dbglog.h"

char *text;

void WriteDebug(char * msg)
{
    char t[128];
    sprintf(t,"%s ---> %s\r\n",GetDateTimeNow(),msg);
    WriteStr(t,PATH);
}

void WriteDebug(int msg)
{
    char t[128];
    sprintf(t,"%s ---> %d\r\n",GetDateTimeNow(),msg);
    WriteStr(t,PATH);
}

void WriteStr(char * msg, char * fn)  // запись в файл
{
  int f;
  unsigned err;
  
  if ((f=fopen(fn,A_WriteOnly+A_BIN+A_Create+A_Append,P_WRITE,&err))!=-1)
  {
    fwrite(f,msg,strlen(msg),&err);
    fclose(f,&err);
  }   
 
}

char *GetDateTimeNow(void)
{
	TDate idate; TTime itime; 
        zeromem(text, 128);
	GetDateTime(&idate,&itime);
	sprintf(text,"%02d.%02d.%04d %02d:%02d:%02d",idate.day,idate.month,idate.year,itime.hour,itime.min,itime.sec); 

	return text;
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

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* ?????????? ???? */
        n = -n;          /* ?????? n ????????????? ?????? */
    i = 0;
    do {       /* ?????????? ????? ? ???????? ??????? */
        s[i++] = n % 10 + '0';   /* ????? ????????? ????? */
    } while ((n /= 10) > 0);     /* ??????? */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
} 

char *IntToStr(int i)
{
  zeromem(text, 128);
  itoa(i,text);
  return text;
}

void InitDBG()
{
  text = new char[128];
}

void DestrDBG()
{
  zeromem(text, 128);
  delete text;
}

#endif
