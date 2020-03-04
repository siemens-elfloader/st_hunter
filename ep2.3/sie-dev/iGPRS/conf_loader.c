#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"
#include "GPRS MAN.h"
#include "main.h"

const char *successed_config_filename="";
const char _Path[64] = "%d:\\ZBin\\etc\\iGPRS 0.4.7 - %d profile.bcfg";

#pragma segment="CONFIG_C"
int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=0;
  void *cfg;
  unsigned int rlen, end;

  cfg=(char *)__segment_begin("CONFIG_C");
  

  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");
  

  if (!(buf=malloc(len))) return -1;
  if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    rlen=fread(f,buf,len,&ul);
    end=lseek(f,0,S_END,&ul,&ul);
    fclose(f,&ul);
    if (rlen!=end || rlen!=len)  goto L_SAVENEWCFG;
    memcpy(cfg,buf,len);
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
  char ConfigPath[64];
  sprintf(ConfigPath,_Path,4,GetProfile()+1);   
  if (LoadConfigData(ConfigPath)<0)
  {
    sprintf(ConfigPath,_Path,2,GetProfile()+1);   
    if (LoadConfigData(ConfigPath)<0)
    {
      sprintf(ConfigPath,_Path,1,GetProfile()+1);
      if (LoadConfigData(ConfigPath)<0)
      {      
        sprintf(ConfigPath,_Path,0,GetProfile()+1);
        if (LoadConfigData(ConfigPath)<0)
        {
          ShowMessage("Не могу создать bcfg T_T");    
        };
      }
    }
  }
  
  if (SEE_STAT_ENABLE && !HookEnable)
    AddKeybMsgHook((void *)my_keyhook); // Активируем KeyHook
  else
  if (!SEE_STAT_ENABLE && HookEnable)    
  {
    RemoveKeybMsgHook((void *)my_keyhook);
    HookEnable = 0;
  }
}



