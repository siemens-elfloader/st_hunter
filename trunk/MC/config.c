#include "inc/mc.h"
#include "inc/config.h"


const char mccf_config[]="mcconfig.cfg";
MC_CFG MCConfig;

char* GetCfgPath()
{
 if (pathbuf)
 {
  if (!CONFIG_MCCONFIG_PATH[0])
   strcpy((char*)CONFIG_MCCONFIG_PATH, mcpath);

  int pathLen = strlen((const char*)CONFIG_MCCONFIG_PATH);
  if (pathLen > 0)
  {
   if (((char*)CONFIG_MCCONFIG_PATH)[pathLen - 1] == '\\')
    ((char*)CONFIG_MCCONFIG_PATH)[pathLen - 1] = 0;
   sprintf(pathbuf, _s_s, CONFIG_MCCONFIG_PATH, mccf_config);
   return pathbuf;
  }
 }
 return NULL;
}

void InitCfg()
{
 zeromem(&MCConfig, sizeof(MCConfig));
 //if (CONFIG_SAVE_PATH)
 if (curtab >= MAX_TABS)
   MCConfig.curtab = back_tab < MAX_TABS ? back_tab : 0;
 else
   MCConfig.curtab = curtab;

 for(int ii=0; ii < MAX_TABS; ii++)
 {
   MCConfig.tabs[ii].sort = tabs[ii]->sort;
   if (CONFIG_SAVE_PATH)
   {
    int dr = tabs[ii]->CurDrv;
    MCConfig.tabs[ii].LastInd = tabs[ii]->iIndex[dr];
    strcpy(MCConfig.tabs[ii].LastPath, tabs[ii]->szDirs[dr]);
   }
 }
}

void SaveCfg()
{
 if (pathbuf)
 {
  int f;
  char* cfgPath = GetCfgPath();
  if (cfgPath)
  {
   if ((f=_open(cfgPath,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
   {
    MCCF_hdr hdr;
    hdr.sig=mccf_sig;
    hdr.ver=mccf_ver;
    hdr.size=mccf_size;
    hdr.offset=mccf_offset;

    InitCfg();

    _write(f,&hdr,sizeof(hdr),&err);
    _write(f,&MCConfig,sizeof(MCConfig),&err);
    _close(f,&err);
   }
  }
 }
}

int LoadCfg()
{
 int res = 0;

 if (pathbuf)
 {
  int f;
  char* cfgPath = GetCfgPath();
  if (cfgPath)
  {
   if ((f=_open(cfgPath,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
   {
    MCCF_hdr hdr;
    if (_read(f, &hdr, sizeof(hdr), &err)==sizeof(hdr) &&
     (hdr.sig == mccf_sig) && ((uint32_t)hdr.size >= mccf_size))
    {
     _lseek(f,hdr.offset,S_SET,&err,&err);
     res = _read(f, &MCConfig, sizeof(MCConfig), &err)==sizeof(MCConfig);
    }
    _close(f, &err);
   }
  }
 }
 if (!res)zeromem(&MCConfig, sizeof(MCConfig));
 return res;
}
