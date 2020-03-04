#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"

#pragma segment="CONFIG_C"
int SaveConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  int result=-1;
  unsigned int wlen;
  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((f=fopen(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
  {
    wlen=fwrite(f,__segment_begin("CONFIG_C"),len,&ul);
    fclose(f,&ul);
    if (wlen==len) result=0;
  }
  return(result);
}

int LoadConfigData(const char *fname, void* cfg,unsigned int len)
{
  int f;
  unsigned int ul;
  int result=-1;
  if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    if (fread(f,cfg,len,&ul)==len)
    {
      fclose(f,&ul);
      result = 0;
    }
    else
    {
      fclose(f,&ul);
    }
  }
  return(result);
}


char *config_path=":\\zbin\\etc";
int InitConfig(void* cfgstruct, int len, char *name,int disk)
{
  int result;
  char config_name[128];
  sprintf(config_name, "%d%s\\%s.bcfg", disk, config_path,name);
  result = LoadConfigData(config_name,  cfgstruct, len);
  return result;
}
