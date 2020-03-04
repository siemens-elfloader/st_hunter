#include "..\inc\swilib.h"
#include "icq\ICQ.h"
#include "sieim.h"
#include "pluginlib.h"

ExecStruct     EStruct;
ParamStruct    PStruct;

void execelf(char *exename, void *fname)
{
  WSHDR *ws=AllocWS(256); 
  str_2ws(ws,exename,strlen(exename)+1); 
  ExecuteFile(ws,0,fname);
  FreeWS(ws);
}

void loadplugin()
{
  ICQPStruct.Struct=&ICQEStruct;
  execelf(path_rd("plugins\\ICQ.elf"), &ICQPStruct);
}
