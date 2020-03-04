#include "..\inc\swilib.h"
#include "../inc/xtask_ipc.h"

extern const char ipc_my_name[];
//const char ipc_send_name[]="Xtask";
extern const char ipc_xtask_name[];

#define IPC_ICON 20

IPC_REQ tmr_gipc2;

void SendXtask(char *sended)
{
  tmr_gipc2.name_to=ipc_xtask_name;
  tmr_gipc2.name_from=ipc_my_name;
  tmr_gipc2.data=sended;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_ICON,&tmr_gipc2);
}


