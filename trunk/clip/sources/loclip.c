#include <stdlib.h>
#include <stdio.h>
#include <swilib.h>
#include "parametrs.h"

char *dfname="4:\\Zbin\\etc\\clipbuf.dat";
WSHDR ret,*mem;

WSHDR *LoadData(char *dfile){
	FILE *fh;
	WPARAM p;
if((fh=fopen(dfile,"rb"))==NULL)return 0;
if(fread(&p,sizeof(WPARAM),1,fh)!=1){fclose(fh);return 0;}

ret.maxlen=p.maxlen;
ret.unk1=p.unk1;
ret.wsbody=(unsigned short *)malloc(p.maxlen*sizeof(unsigned short));
ret.isbody_allocated=1;

if(fread(ret.wsbody,p.maxlen*sizeof(unsigned short),1,fh)!=1){fclose(fh);free(ret.wsbody);return 0;}
fclose(fh);
return &ret;
}


void ElfKiller(void)                              //так вот эльф мы выключаем:
{
kill_elf();
}

int main()
{

  if((mem=LoadData(dfname))==NULL){
  	dfname[0]='0';
  mem=LoadData(dfname);
  }
  if(mem){
  CopyTextToClipboard(mem,1,wstrlen(mem));
  free(mem->wsbody);
  }
 SUBPROC((void *)ElfKiller);
  return 0;
}
