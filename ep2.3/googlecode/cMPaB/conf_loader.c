#include "c:\arm\inc\swilib.h"
#include "c:\arm\inc\cfg_items.h"
#include "conf_loader.h"
#include "lang.h"

const char *successed_config_filename="";

#pragma segment="CONFIG_C"
int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=0;
  void *cfg;

  extern const CFG_HDR cfghdr0; //first var in CONFIG
  cfg=(void*)&cfghdr0;

  unsigned int len=(int)__segment_end("CONFIG_C")-(int)__segment_begin("CONFIG_C");

  if (!(buf=malloc(len))) return -1;
  if ((f=fopen(fname,A_ReadOnly+A_BIN,0,&ul))!=-1)
  {
    if (fread(f,buf,len,&ul)==len)
    {
      memcpy(cfg,buf,len);
      fclose(f,&ul);
    }
    else
    {
      fclose(f,&ul);
      goto L_SAVENEWCFG;
    }
  }
  else
  {
  L_SAVENEWCFG:
    if ((f=fopen(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
    {
      if (fwrite(f,cfg,len,&ul)!=len) result=-1;
      fclose(f,&ul);
    }
    else
      result=-1;
  }
  mfree(buf);
  if (result>=0) successed_config_filename=fname;
  return(result);
}

void InitConfig()
{
#ifdef LGP_RUS  //If the language is Russian, then save the configuration to its name
  if (LoadConfigData("4:\\ZBin\\etc\\cMPaB_rus.bcfg")<0)
  {
    LoadConfigData("0:\\ZBin\\etc\\cMPaB_rus.bcfg");
  }
#endif
#ifdef LGP_ENG  //If language is English, then save the configuration to its name
  if (LoadConfigData("4:\\ZBin\\etc\\cMPaB_eng.bcfg")<0)
  {
    LoadConfigData("0:\\ZBin\\etc\\cMPaB_eng.bcfg");
  }  
#endif  
}
