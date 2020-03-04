
#include <swilib.h>
#include <nucleus/nucleus.h>
#include "ndefs.h"
#include "thread.h"


STATUS  TCCE_Create_Task (NU_TASK *task, CHAR *name,
                                   VOID (*task_entry)(UNSIGNED, VOID *), UNSIGNED argc,
                                   VOID *argv, VOID *stack_address, UNSIGNED stack_size,
                                   OPTION priority, UNSIGNED time_slice,
                                   OPTION preempt, OPTION auto_start)
{
    __def_adr(Create_Task, STATUS, task, name, task_entry,
                          argc, argv, stack_address,
                          stack_size, priority, time_slice,
                          preempt, auto_start)
}


STATUS TCCE_Delete_Task(NU_TASK *task)
{
    __def_adr(Delete_Task, STATUS, task)
}


STATUS TCCE_Reset_Task(NU_TASK *task, UNSIGNED argc, VOID *argv)
{
    __def_adr(Reset_Task, STATUS, task, argc, argv)
}


STATUS TCCE_Terminate_Task(NU_TASK *task)
{
    __def_adr(Terminate_Task, STATUS, task)
}


STATUS TCCE_Resume_Task(NU_TASK *task)
{
    __def_adr(Resume_Task, STATUS, task)
}


STATUS TCCE_Suspend_Task(NU_TASK *task)
{
    __def_adr(Suspend_Task, STATUS, task)
}


VOID TCCE_Relinquish(VOID)
{
    __defn_adr(Relinquish)
}


NU_TASK *TCCE_Current_Task_Pointer(VOID)
{
    __def_adr(Current_Task_Pointer, NU_TASK *)
}


VOID TCCE_Task_Sleep(UNSIGNED ticks)
{
    __defn_adr(Task_Sleep, ticks)
}


OPTION TCCE_Change_Priority(NU_TASK *task, OPTION new_priority)
{
    __def_adr(Change_Priority, OPTION, task, new_priority)
}


OPTION TCCE_Change_Preemption(OPTION preempt)
{
    __def_adr(Change_Preemption, OPTION, preempt)
}


UNSIGNED TCCE_Change_Time_Slice(NU_TASK *task, UNSIGNED time_slice)
{
    __def_adr(Change_Time_Slice, UNSIGNED, task, time_slice)
}


UNSIGNED TCCE_Check_Stack(VOID)
{
    __def_adr(Check_Stack, UNSIGNED)
}


UNSIGNED TCCE_Established_Tasks(VOID)
{
    __def_adr(Established_Tasks, UNSIGNED)
}


UNSIGNED TCCE_Task_Pointers(NU_TASK **pointer_list, UNSIGNED maximum_pointers)
{
    __def_adr(Task_Pointers, UNSIGNED, pointer_list, maximum_pointers)
}


STATUS TCCE_Task_Information(NU_TASK *task, CHAR *name,
                        DATA_ELEMENT *status, UNSIGNED *scheduled_count,
                        OPTION *priority, OPTION *preempt,
                        UNSIGNED *time_slice, VOID **stack_base,
                        UNSIGNED *stack_size, UNSIGNED *minimum_stack)
{
    __def_adr(Task_Information, STATUS, task, name, status, scheduled_count,
                                priority, preempt, time_slice, stack_base,
                                stack_size, minimum_stack)
}




