
#ifndef __NU_GUI_H__
#define __NU_GUI_H__

#include <swilib.h>
#include <nu_swilib.h>
#include "ExtEvent.h"
#include "ExtendedNuTask.h"
#include "NuCSM.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUGUI_QUEUE_MAX_SIZE 128


typedef struct
{
    GUI gui;
    int id;
    char released, closed, removed_from_task;
    NER_data *ner;
    char queue_loked;
    void *userdata;
    ExtTask *crt_in;
}NuGUI_data;


int NuGUI_create(NuGUI_data *nu_gui, NuCSM_data *cdata, NER_data *ner, CSM_RAM *cram, RECT *canvas, void *userdata);
int NuGUI_close(NuGUI_data *nu_gui);
int NuGUI_release(NuGUI_data *nu_gui);
void NuGUI_wait_for_close(NuGUI_data *data);

#ifdef __cplusplus
}
#endif

#endif
