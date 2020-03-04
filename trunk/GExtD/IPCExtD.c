#include <stdio.h>
#include <stdlib.h>
#include <swilib.h>

#include "IPCExtD.h"

IPC_REQ IPC_free;

void SendIpc(int submess)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,submess,&IPC_free);
}

void SendIpcData(int submess, void *data)
{
  LockSched();
  IPCsend.data=data;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,submess,&IPCsend);
  UnlockSched();
}