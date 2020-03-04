
#include "myutils.h"

void ExtractFileName(char *fullname,char *output){
int i=0;
while(fullname[i]!=0)i++;
while(fullname[i]!='\\')i--;
i++;
strcpy(output,&fullname[i]);
}

void uppercase(char *str){
int i=0;
int slen;
slen=strlen(str);
while(i<slen){
if((str[i]>='a')&&(str[i]<='z'))str[i]=str[i]-('a'-'A');
i++;
}
}

void ExtractFileExt(char *fullname,char *output){
int i=0;
while(fullname[i]!=0)i++;
while(fullname[i]!='.'){
i--;
if(&fullname[i]<=fullname)break;
}
i++;
strcpy(output,&fullname[i]);
}


void ExtractFileDir(char *fullname,char *output){
int i=0;
while(fullname[i]!=0)i++;
while(fullname[i]!='\\')i--;
i++;
memcpy(output,fullname,i-1);
output[i-1]=0;
}

int fread32(int fh, char *buf, int len, unsigned int *err) // Читать из файла поблочно (c)Rst7
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

int fwrite32(int fh, char *buf, int len, unsigned int *err) // Писать в файл поблочно (c)Rst7
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
}
return(total);
}
