#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"

char *successed_config_filename;

#pragma segment="CONFIG_C"
int SaveConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  int result=0;
  unsigned int wlen;
  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((f=fopen(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
  {
    wlen=fwrite(f,__segment_begin("CONFIG_C"),len,&ul);
    fclose(f,&ul);
    if (wlen==len) result=1;
  }
  return(result);
}


#pragma segment="CONFIG_C"
int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=0;
  unsigned int rlen, end;


  unsigned int len=(char *)__segment_end("CONFIG_C")-(char *)__segment_begin("CONFIG_C");

  if ((buf=malloc(len)))
  {
    if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
    {
      rlen=fread(f,buf,len,&ul);
      end=lseek(f,0,S_END,&ul,&ul);
      fclose(f,&ul);
      if (rlen!=end || rlen!=len)  goto L_SAVENEWCFG;
      memcpy(__segment_begin("CONFIG_C"),buf,len);
      result=2;
    }
    else
    {
    L_SAVENEWCFG:
      result=SaveConfigData(fname);
    }
    mfree(buf);
  }
  if (result>0)
  {
	  if(!successed_config_filename) successed_config_filename=malloc(strlen(fname)+1);
	  strcpy(successed_config_filename, fname);
  }
  return(result);
}

void InitConfig(void) 
{
	if(!LoadConfigData("4:\\ZBin\\etc\\NStopWatch.bcfg"))
		LoadConfigData("0:\\ZBin\\etc\\NStopWatch.bcfg");
}
//==========================================================================================

char *successed_config_skin_filename=NULL;

#pragma segment="CONFIG_SKIN_C"
int SaveConfigSkin(const char * fname)   // result 0 - конфиг не создан, 1 - cоздан
{
  int f;
  unsigned int ul;
  int result=0;
  unsigned int wlen;
  unsigned int len=(char *)__segment_end("CONFIG_SKIN_C")-(char *)__segment_begin("CONFIG_SKIN_C");

  if ((f=fopen(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
  {
    wlen=fwrite(f,__segment_begin("CONFIG_SKIN_C"),len,&ul);
    fclose(f,&ul);
    if (wlen==len) result=1;
  }
  return(result);
}

//#pragma segment="CONFIG_SKIN_C"
int LoadConfigSkin(const char *fname)
{
    int f;
    unsigned int ul;
    char *buf;
    int result=-1;
    void *cfg;
    unsigned int rlen, end;

    extern const CFG_HDR skinhdr1;
    cfg=(void*)&skinhdr1;
    unsigned int len=(int)__segment_end("CONFIG_SKIN_C")-(int)__segment_begin("CONFIG_SKIN_C");

    if (!(buf=malloc(len))) return result;
    if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
    {
        rlen=fread(f,buf,len,&ul);
        end=lseek(f,0,S_END,&ul,&ul);
        fclose(f,&ul);
        if (rlen!=end || rlen!=len) goto SAVECFG;
        memcpy(cfg, buf, len);
        result =1;
    }
    else
    {
        SAVECFG:
        result=SaveConfigSkin(fname);
    }
    mfree(buf);
    if(result>0)
    {
        if(!successed_config_skin_filename) successed_config_skin_filename=malloc(strlen(fname)+1);
        strcpy(successed_config_skin_filename, fname);
    }
    return(result);
}

void InitConfigSkin(void)
{
    extern const char CFG_PATH_SKIN[128];
    if(!CFG_PATH_SKIN) return;
      
    char *path=malloc(strlen(CFG_PATH_SKIN)+10);
    sprintf(path, "%s%s", CFG_PATH_SKIN, "skin.bcfg");
    if(LoadConfigSkin(path)<=0) SaveConfigSkin(path);
    mfree(path);
}
