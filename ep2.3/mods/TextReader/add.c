#include "..\inc\swilib.h"
//#include "main.h"
//------------------------------
int start(const char *f)
{
  WSHDR *ws;
  ws=AllocWS(256);
  str_2ws(ws,f,256);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  return(1);
}
//===============================
int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (int k=0;;k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A) {ret=ret*10+attr[k]-0x30;} 
    else 
       {
         if ( attr[k]=='-') {neg=-1;} 
         else {return(ret*neg);}
       }
  }
  
}

//=======================================================
void *about()
{ 
  char s[128];
  snprintf(s,255,"TextReader/Editor v0.1.5fix\n(c)kluchnik\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
  return 0;
}

//========================================================

int fread32(int fh, char *buf, int len, unsigned int *err)
{
  int clen;
  int rlen; 
  int total=0;
  while(len)
  {
    if (len>16384) clen=16384; else clen=len;
    total+=(rlen=fread(fh, buf, clen, err));
    if (rlen!=clen) break;
    buf+=rlen;
    len-=clen;
  }
return(total);
}

int fwrite32(int fh, char *buf, int len, unsigned int *err)
{
  int clen; 
  int rlen; 
  int total=0;
  while(len)
  {
    if (len>16384) clen=16384; else clen=len;
    total+=(rlen=fwrite(fh, buf, clen, err));
    if (rlen!=clen) break;
    buf+=rlen;
    len-=clen;
//progressbar();//add
  }
return(total);
}

//==========================================================================

char *GetName(char *file,int type)//Вытаскиваем имя из пути к файлу, type==0-без расширения,1 - с расширением
{
  char *p=strrchr(file,'\\');
  if(type) return p+1;
  char *name;
  if (p)
   {
     name=strchr(p,'.');
   }
 return name;
}
