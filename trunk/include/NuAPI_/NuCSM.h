
#ifndef __NUCSM_H__
#define __NUCSM_H__

#include <swilib.h>
#include <nu_swilib.h>
#include "ExtEvent.h"
#include "ExtendedNuTask.h"


#ifdef __cplusplus
extern "C" {
#endif


#define NUCSM_SUCCESS NU_SUCCESS

#define NUCSM_EVENT_SIZE 1

#define NUCSM_GUIMODE 0x02

#define NUCSM_QUEUE_MAX_SIZE 26



typedef struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
    unsigned short csm_name[128];
    int minus11;
    int  id, gui_id;
    char closed;
    char queue_loked;
    char released, removed_from_task;
    NER_data *ner;
    ExtTask *crt_in;
    void *userdata;
}NuCSM_data;




int NuCSM_create(NuCSM_data *data, NER_data *ner, int type, const char *name, void *userdata);
int NuCSM_close(NuCSM_data *data);
void NuCSM_release(NuCSM_data *data);
void NuCSM_wait_for_close(NuCSM_data *data);


#ifdef __cplusplus
}
#endif

#endif
