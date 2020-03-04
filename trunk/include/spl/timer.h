

#ifndef __TIMER_H__
#define __TIMER_H__

#include "coreevent.h"

#ifdef __cplusplus
extern "C" {
#endif



int timerStart(unsigned long time, void (*callback)(int, void *), void *userdata);
int timerReset(int id, unsigned long time, void (*callback)(int, void *), void *userdata);
int timerPause(int timer);
int timerResume(int timer);
int timerStop(int timer);
void *timerUserData(int id);

#ifdef __cplusplus
}
#endif

#endif
