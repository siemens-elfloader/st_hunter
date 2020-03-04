#include <swilib.h>
#include <cfg_items.h>
#include "conf_loader.h"
#include "color.h"

char * successed_config_filename = NULL;

__CONFIG_EXTERN(0, cfghdr_CONNECT_LEVEL)
__CONFIG_EXTERN(1, cfgcolor1)
__CONFIG_EXTERN(2, cfghdr_IBI)


int SaveConfigData(const char * fname)   // result 0 - конфига не создан, 1 - cоздан
{
  int f;
  unsigned int ul;
  int result=0;
  unsigned int wlen;
  unsigned int len = __DATA_CFG_END(0) - __DATA_CFG_BEGIN(0);

  if ((f = _open(fname, A_WriteOnly+A_BIN+A_Create+A_Truncate, P_WRITE, &ul)) != -1)
  {
    wlen = _write(f, __DATA_CFG_BEGIN(0), len, &ul);
    _close(f, &ul);
    if (wlen == len) result=1;
  }
  return (result);
}


int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=0;
  void *cfg;
  unsigned int rlen, end;

  cfg=(void*)&cfghdr_CONNECT_LEVEL;

  unsigned int len = __DATA_CFG_END(0) - __DATA_CFG_BEGIN(0);

  if (!(buf=malloc(len))) return -1;
  if ((f=_open(fname, A_ReadOnly+A_BIN, P_READ, &ul))!=-1)
  {
    rlen=_read(f, buf, len, &ul);
    end=_lseek(f, 0, S_END, &ul, &ul);
    _close(f, &ul);
    if (rlen != end || rlen != len)  goto L_SAVENEWCFG;
    memcpy(cfg, buf, len);
    result=1;
  }
  else
  {
  L_SAVENEWCFG:
    result=SaveConfigData(fname);
  }
  mfree(buf);
  if (result>0)
  {
    if (successed_config_filename)
      mfree(successed_config_filename);
    successed_config_filename = malloc(strlen(fname) + 1);
    strcpy(successed_config_filename, fname);
  }
  return(result);
}

void InitConfig(char* fname)
{
#ifdef NEWSGOLD
  if(!fname)
#else
  if(LoadConfigData(fname)<=0)
#endif
  {
    if (LoadConfigData("4:\\ZBin\\etc\\SieJC.bcfg")<=0)
    {
      LoadConfigData("0:\\ZBin\\etc\\SieJC.bcfg");
    }
  }
#ifdef NEWSGOLD
  else LoadConfigData(fname);
#endif
}


#ifdef ICONBAR
int SaveIconSetData(const char * fname)   // result 0 - конфига не создан, 1 - cоздан
{
  int f;
  unsigned int ul;
  int result=0;
  unsigned int wlen;
  unsigned int len = __DATA_CFG_END(2) - __DATA_CFG_BEGIN(2);

  if ((f=_open(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
  {
    wlen=_write(f, __DATA_CFG_BEGIN(2), len, &ul);
    _close(f,&ul);
    if (wlen==len) result=1;
  }
  return(result);
}


int LoadIconSetData(const char *fname)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=0;
  void *cfg;
  unsigned int rlen, end;

  cfg=(void*)&cfghdr_IBI;
  unsigned int len = __DATA_CFG_END(2) - __DATA_CFG_BEGIN(2);

  if (!(buf=malloc(len))) return -1;
  if ((f=_open(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    rlen=_read(f,buf,len,&ul);
    end=_lseek(f,0,S_END,&ul,&ul);
    _close(f,&ul);
    if (rlen!=end || rlen!=len)  goto L_SAVENEWCFGIB;
    memcpy(cfg,buf,len);
    result=1;
  }
  else
  {
  L_SAVENEWCFGIB:
    result=SaveIconSetData(fname);
  }
  mfree(buf);
  return(result);
}

void InitIconSet()
{
  if (LoadIconSetData("4:\\ZBin\\etc\\SieJCIconSet.bcfg")<=0)
  {
    LoadIconSetData("0:\\ZBin\\etc\\SieJCIconSet.bcfg");
  }
}
#endif




int SaveColorSetData(const char * fname)   // result 0 - конфиг не создан, 1 - cоздан
{
  int f;
  unsigned int ul;
  int result=0;
  unsigned int wlen;
  unsigned int len = __DATA_CFG_END(1) - __DATA_CFG_BEGIN(1);

  if ((f=_open(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&ul))!=-1)
  {
    wlen=_write(f, __DATA_CFG_BEGIN(1), len, &ul);
    _close(f,&ul);
    if (wlen==len) result=1;
  }
  return(result);
}

int LoadColorSetData(const char *fname)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=-1;
  void *cfg;
  unsigned int rlen, end;

  cfg=(void*)&cfgcolor1;
  unsigned int len = __DATA_CFG_END(1) - __DATA_CFG_BEGIN(1);

  if (!(buf=malloc(len))) return result;
  if ((f=_open(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    rlen=_read(f,buf,len,&ul);
    end=_lseek(f,0,S_END,&ul,&ul);
    _close(f,&ul);
    if (rlen!=end || rlen!=len)
    {
      ShowMSG(1,(int)"Error color file.");
      result = 0;
    } else result=1;
    memcpy(cfg,buf,len);
  }
  mfree(buf);
  return(result);
}
// EOL, EOF

