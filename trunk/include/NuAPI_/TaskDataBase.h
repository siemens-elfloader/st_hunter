

#ifndef __TASK_DATA_BASE_H__
#define __TASK_DATA_BASE_H__

#include <NuAPI/ExtArray.h>

#ifdef __cplusplus
extern "C" {
#endif


struct db_value
{
    ExtTask *task;
    char want_die;
};



int task_db_push(ExtTask *task, int kill_on_db_exit);
int task_db_pop(ExtTask *task);
int task_db_is_exist(ExtTask *task);
int task_db_count();
ExtArray *task_db_array();
ExtArray *thread_db_array();

int task_db_kill_and_free();

int db_task_id(ExtTask *task);


#ifdef __cplusplus
}
#endif

#endif
