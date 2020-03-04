#include <swilib.h>
#include <cfg_items.h>
#include <conf_loader.h>
#include <stdlib.h>
/*
    Переделанный загрузчик конфига под gcc.
    (c) Z.Vova
*/


char *successed_config_filename="";


//__attribute__((section("CONFIG_C")))
int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=0;
  void *cfg;

  extern CFG_HDR cfghdr0;
  extern const int __config_end0, __config_begin0;
  cfg=(void*)&cfghdr0;
  volatile unsigned int _segb = (volatile unsigned int)(&__config_begin0)+4;
  volatile unsigned int _sege = (volatile unsigned int)&__config_end0;


  unsigned int len = (_sege - _segb);
  if (!(buf=malloc(len))) return -1;
  if ((f=_open(fname,A_ReadOnly+A_BIN,0,&ul))!=-1)
  {
    if (_read(f,buf,len,&ul) == (int)len)
    {
      memcpy(cfg,buf,len);
      _close(f,&ul);
    }
    else
    {
      _close(f,&ul);
      goto L_SAVENEWCFG;
    }
  }
  else
  {
  L_SAVENEWCFG:
    if ((f=_open(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
    {
      if (_write(f,cfg,len,&ul)!= (int)len) result=-1;
      _close(f,&ul);
    }
    else
      result=-1;
  }
  mfree(buf);
  if (result>=0) successed_config_filename = (char*)fname;
  return(result);
}


int InitConfig_byName(const char *cfg)
{
  return LoadConfigData(cfg) > -1;
}
