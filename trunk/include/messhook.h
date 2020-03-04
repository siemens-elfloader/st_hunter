
#ifndef __MESS_HOOK_H__
#define __MESS_HOOK_H__


#include <swilib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*message_handler_t)(CSM_RAM *data, GBS_MSG *msg);

int createMessageHook(message_handler_t h);
int deleteMessageHook(int id);


#ifdef __cplusplus
}
#endif

#endif
