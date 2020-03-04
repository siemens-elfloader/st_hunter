
#ifndef __SPL_MESS_HOOK_H__
#define __SPL_MESS_HOOK_H__

#include <swilib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*message_hook_t)(int id, CSM_RAM *cram, GBS_MSG *msg);



int regMessageHook(message_hook_t h, void *userdata);
int unregMessageHook(int id);
void *messageHookUserData(int id);

#ifdef __cplusplus
}
#endif

#endif
