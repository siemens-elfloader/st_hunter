
#ifndef __CORE_EVENT_H__
#define __CORE_EVENT_H__

#include <spl/async_print.h>

enum {

    CORE_EVENT_PROCESS = 0x01,
    TIMER_EVENT,
    CSM_EVENT,
    GUI_EVENT
};


typedef struct {
    int id;
    int type;
    void (*dispatcher)(void *);
}CoreEvent;

#ifdef __cplusplus
extern "C" {
#endif

void processEvents();
void initUsart();



#ifdef __cplusplus
}
#endif

#endif
