#include "../inc/swilib.h"

void IPC_Send(const char *name_to, const char *name_from, void *data,const int submess)
{
    static IPC_REQ ipc;
    ipc.name_to=name_to;
    ipc.name_from=name_from;
    ipc.data=data;
    GBS_SendMessage(MMI_CEPID, MSG_IPC, submess, &ipc);
}
