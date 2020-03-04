
#ifndef __NU_TIMER_H__
#define __NU_TIMER_H__

#include <swilib.h>
#include <nu_swilib.h>
#include "ExtEvent.h"
#include "ExtendedNuTask.h"
#include "NuCSM.h"


typedef struct
{
    NU_TIMER timer;
    NER_data *ner;
    ExtTask *crt_in;
    int id;
    void *user_data;
    char released, deleted_from_task;
}NuTimer;


int NuTimer_create(NuTimer *timer, NER_data *ner, int ticks, int start, void *user_data);
int NuTimer_release(NuTimer *timer);

int NuTimer_enable(NuTimer *timer, int enable);


#endif
