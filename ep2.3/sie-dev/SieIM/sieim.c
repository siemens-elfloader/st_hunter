#include "inc\swilib.h"
#include "sieim.h"

const char ELF_PATH[128]=DEFAULT_DISK":\\ZBin\\SieIM\\";

int check_id_type (char * id)
{
  if (strlen(id)>9)
    return 1;
  for (register int i=0; i < strlen(id); ++i)
  {
    if (*(id+i)<'0' || *(id+i)>'9')
      return 1;
  }
  return 0;
}

unsigned int id2uin(char *attr)
{
  unsigned int ret=0;
  for (int k=0; ; k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A)
      ret=ret*10+attr[k]-0x30;
    else
      return ret;
  }
}

void * return_id (char * id)
{
  if (check_id_type(id))
    return (char *) id;
  else
    return (unsigned int *) id2uin(id);
}

char *path_rd (char *subpath)
{
  char *path;
  int ln;
  path=malloc((ln=strlen(ELF_PATH))+strlen(subpath)+2);
  strcpy(path,ELF_PATH);
  strcat(path+ln,subpath);
  return path;
}

void exec(char *exename, void *fname)
{
  WSHDR *ws=AllocWS(256); 
  str_2ws(ws,exename,strlen(exename)+1); 
  ExecuteFile(ws,0,fname);
  FreeWS(ws);
}

