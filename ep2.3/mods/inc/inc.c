#include "..\..\inc\swilib.h"
#include "inc.h"

char ipc_cout_name[]=IPC_COUT_NAME;

IPC_REQ *FFipc;

void COUTs(char string[])
{
  FFipc=malloc(sizeof(IPC_REQ));
  FFipc->name_to=ipc_cout_name;
  FFipc->name_from=ipc_cout_name;
  FFipc->data=malloc(strlen(string)+1);
  memcpy(((char *)(FFipc->data)),string,strlen(string)+1);
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_COUT,FFipc);
}

void COUTi(int number)
{
  char tmp[10];
  sprintf(tmp,"%u",number);
  COUTs(tmp);
}

