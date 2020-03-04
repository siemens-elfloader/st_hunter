
#ifndef __EXT_EVENT_H__
#define __EXT_EVENT_H__

#include <swilib.h>
#include <nu_swilib.h>
#include "ExtendedNuTask.h"
#include "ExtMutex.h"
#include <NuAPI/PlatformDependensTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NuCSM_EventCreate = 1,
    NuCSM_EventClose,
    NuCSM_EventMessage,

    NuGUI_EventRedraw,
    NuGUI_EventCreate,
    NuGUI_EventClose,
    NuGUI_EventFocus,
    NuGUI_EventUnfocus,
    NuGUI_EventKey,
    NuGUI_EventGHook,
    NuGUI_EventUsrProc,
    NuTimer_EventTimeout,
    NuSignal_Event
}EventMessageType;



typedef struct
{
    int id;
    int type;
    CSM_RAM cram;
    GBS_MSG msg;
    GUI_MSG gmsg;
    GUI gui;
    int cmd;
    void *ud;
    void *userdata;
}ExtEvent __attribute__((aligned(4)));


typedef struct
{
    NU_QUEUE queue;
    void *queue_space;
    char removed_from_task, released, locked;
    ExtTask *crt_in;
}NER_data;


int NuEventReceiver_create(NER_data *data);
int NuEventReceiver_release(NER_data *data);
int NuEventReceive(NER_data *data, ExtEvent *event);
int NER_push(NER_data *data, ExtEvent *event, int suspend);

#ifdef __cplusplus
}
#endif

#endif
