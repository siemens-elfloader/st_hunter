#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"

#pragma segment="CONFIG_DATA_ID"
#pragma segment="CONFIG_DATA_I"
int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  int result=0;
  void *cfg;
  void *cfg_init;
  unsigned int rlen, end;

  cfg=(void *)__segment_begin("CONFIG_DATA_I");
  cfg_init=(void *)__segment_begin("CONFIG_DATA_ID");
  
  unsigned int len=(char *)__segment_end("CONFIG_DATA_ID")-(char *)__segment_begin("CONFIG_DATA_ID");
  if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    rlen=fread(f,cfg,len,&ul);
    end=lseek(f,0,S_END,&ul,&ul);
    fclose(f,&ul);
    if (rlen!=end || rlen!=len)  goto L_SAVENEWCFG;
  }
  else
  {
  L_SAVENEWCFG:
    memcpy(cfg,cfg_init,len);
    if ((f=fopen(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
    {
      if (fwrite(f,cfg,len,&ul)!=len) result=-1;
      fclose(f,&ul);
    }
    else
      result=-1;
  }
  return(result);
}

int InitConfig(void)
{
  if (LoadConfigData("4:\\ZBin\\etc\\TIIGPRSLogo.bcfg")<0)
  {
    if (LoadConfigData("0:\\ZBin\\etc\\TIIGPRSLogo.bcfg")<0)
    	return -1;
  }
  return 1;
}
