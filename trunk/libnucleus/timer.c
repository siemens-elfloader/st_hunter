
#include <swilib.h>
#include <nucleus/nucleus.h>
#include "ndefs.h"
#include "timer.h"


STATUS TCCE_Create_Timer(NU_TIMER *timer, CHAR *name,
                        VOID (*expiration_routine)(UNSIGNED), UNSIGNED id,
                        UNSIGNED initial_time, UNSIGNED reschedule_time,
                        OPTION enable)
{
    __def_adr(Create_Timer, STATUS, timer, name, expiration_routine, id,
                                    initial_time, reschedule_time, enable)
}


STATUS TCCE_Delete_Timer(NU_TIMER *timer)
{
      __def_adr(Delete_Timer, STATUS, timer)
}


STATUS TCCE_Reset_Timer(NU_TIMER *timer,
                        VOID (*expiration_routine)(UNSIGNED),
                        UNSIGNED initial_time, UNSIGNED reschedule_timer,
                        OPTION enable)
{
    __def_adr(Reset_Timer, STATUS, timer, expiration_routine, initial_time,
                                    reschedule_timer, enable)
}


STATUS TCCE_Control_Timer(NU_TIMER *timer, OPTION enable)
{
    __def_adr(Control_Timer, STATUS, timer, enable)
}


UNSIGNED TCCE_Established_Timers(VOID)
{
    __def_adr(Established_Timers, UNSIGNED)
}

#ifndef E71V45
VOID TCCE_Set_Clock(UNSIGNED new_value)
{
    __defn_adr(Set_Clock, new_value)
}
#endif
