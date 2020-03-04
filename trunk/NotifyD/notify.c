#include <swilib.h>
#include "notify.h"

#define FLAG_ERROR 0
#define FLAG_OK 1

void Msg(int flag, unsigned int lgp_id)
{
    static IPC_REQ ipc;
    ipc.name_to=IPC_NAME_TO;
    ipc.name_from=IPC_NAME_FROM;
    if(lgp_id>=0xA0000000)  //если указатель
    {
        int len=strlen((char*)lgp_id)+1;
        ipc.data=malloc(len);
        memcpy(ipc.data, (char*)lgp_id, len);
    }
    else
    {
        MsgBoxError(1, (int)"LGP_ID not supported");
        return;
    }
    GBS_SendMessage(MMI_CEPID, MSG_IPC, flag, &ipc);
}


/**flag 0 - error, flag 1 -ok**/

inline void ShowMsgError(int flag, unsigned int lgp_id)
{
    Msg(FLAG_ERROR, lgp_id);
}

inline void ShowMsgOk(int flag, unsigned int lgp_id)
{
    Msg(FLAG_OK, lgp_id);
}
