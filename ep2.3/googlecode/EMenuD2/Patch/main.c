#include "..\\..\\inc\\swilib.h"


extern void kill_data(void *p, void (*func_p)(void *));

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


#define ipc_my_name "EMenuD 2"
#define IPC_OPEN_GUI 2

IPC_REQ myipc=
{
  ipc_my_name,
  ipc_my_name,
  NULL
};



int main(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_OPEN_GUI,&myipc);
  SUBPROC((void *)ElfKiller);
  return 0;
}
