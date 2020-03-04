
#ifndef __TASK_RESOURCE_CONTROL_H__
#define __TASK_RESOURCE_CONTROL_H__

#include <NuAPI/ExtendedNuTask.h>
#include <NuAPI/ExtQueue.h>


struct IOCtl_system_handle
{
    ExtTask *task;
    void *data;
};



typedef struct
{
    void (*sys_fini)(void *);

    struct IOCtl_system_handle *(*init)(ExtTask *);
    void (*fini)(struct IOCtl_system_handle *);

    void (*suspend)(struct IOCtl_system_handle *);
    void (*resume)(struct IOCtl_system_handle *);

    void (*statistic)(struct IOCtl_system_handle *, char *, size_t);
    void *userData;

}TaskResourceControl;


TaskResourceControl *trc_init(TaskResourceControl *trc, void (*sysfini_func)(TaskResourceControl *),
                              struct IOCtl_system_handle *(*init_trc)(ExtTask *), void (*fini_trc)(struct IOCtl_system_handle *),
                              void (*subsystem_statistic)(struct IOCtl_system_handle *trc_data, char *buf, size_t max),
                              int *system_id);

void trc_set_suspend_handle(TaskResourceControl *trc, void (*handle)(struct IOCtl_system_handle *));
void trc_set_resume_handle(TaskResourceControl *trc, void (*handle)(struct IOCtl_system_handle *));

int trc_subsystem_count();
ExtQueue *trc_get_subsystem_list();

int trc_register_subsystem(TaskResourceControl *trc);
TaskResourceControl *trc_get_subsystem_allocator(int id);

struct IOCtl_system_handle *trc_alloc_subsysteml(TaskResourceControl *trc, ExtTask *task);
struct IOCtl_system_handle *trc_alloc_subsystemi(int id, ExtTask *task);

void trc_finalize_subsysteml(TaskResourceControl *trc, struct IOCtl_system_handle *subsys_data);
int trc_finalize_subsystemi(int id, struct IOCtl_system_handle *subsys_data);

struct IOCtl_system_handle **trc_prepare_all_systems(ExtTask *task, int *count);
int trc_finalize_all_systems(struct IOCtl_system_handle **systems, int cnt);

void trc_subsystem_stat(int num, struct IOCtl_system_handle *trc_data, char *buf, size_t max);
#endif
