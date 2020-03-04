
#ifndef __CORE_CSM_H__
#define __CORE_CSM_H__

#include <swilib.h>

#define CoreCSM_GUI 1
#define CoreCSM_IDLE 2


int csmCreate(const char *name, int type,
              void (*onCreate)(CSM_RAM *),
              void (*onClose)(CSM_RAM *),
              void (*onMessage)(CSM_RAM *, GBS_MSG *));

int csmClose(int id);
int csmBindGUI(int id, int gui_id);

#endif
