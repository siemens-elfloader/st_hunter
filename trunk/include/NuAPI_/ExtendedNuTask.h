
#ifndef __EXTENDED_NU_TASK_H__
#define __EXTENDED_NU_TASK_H__


#ifdef __cplusplus
extern "C" {
#endif



#include <NuAPI/ExtList.h>
#include <NuAPI/Signals.h>

#define DEFAULT_TASK_PRIORITY 190
#define DEFAULT_STACK_SIZE 0x3000

#define TASK_NULL_STATE 0
#define TASK_WORKING    1
#define TASK_FINISHED   2

struct Thread;
struct IOCtl_system_handle;
void printout(const char *, ...);

typedef struct ExtTask
{
    NU_TASK task;                   // Таск нуклеуса
    uint8_t istask;                 // Таск/тред
    void *first_parent;             // Самый первый родитель(для тредов)
    struct ExtTask *parent;         // ExtTask. Кто создал таск
    ExtList *childs;                // ExtTask. Что создано этим таском
    char *task_name;

    struct {
        char *base;                 // Стек
        uint32_t size;              // Размер стека
        char own_allocation;        // Мы сами аллочили
    }stack;

    struct {
        struct IOCtl_system_handle **list;                // task resource control
        int count;
    }trc_list;

    ExtList *memctl_queue;          // Контроль аллокаций памяти
    ExtList *finalizators;          // Функции-финализаторы. Вызываются при удалении таска

    ExtList *suspend_handles,
            *resume_handles;

    void (*on_exit)(void *);
    void *userdata;
    void *libc_process_ponter;      // Used by LibcTask
    void *(*task_proc)(int argc, void *argv);
    void *t_return;
    void (*task_runing)();
    void (*task_finishing)();
    char status;
    int reserved[2];
}ExtTask;


#define IS_TASK(tsk) (((ExtTask *)tsk)->istask)

int exttask_create_base(ExtTask *task, void (*do_on_exit)(), const char *name, void *stack, size_t stack_size,
                        char no_free_stack, char kill_by_db, void *(*func_ptr)(int, void *),
                        int argc, void *argv, int prio, int stay, char add_in_parent_task);

int exttask_create7v(ExtTask *task, void (*do_on_exit)(), const char *name,
                   void *(*func_ptr)(int, void *), int argc, void *argv, int prio);

int exttask_create8v(ExtTask *task, void (*do_on_exit)(), const char *name,
                   void *(*func_ptr)(int, void *), int argc, void *argv, int prio, int stay);

int exttask_create9v(ExtTask *task, void (*do_on_exit)(), const char *name, void *stack, uint32_t stack_size,
                   void *(*func_ptr)(int, void *), int argc, void *argv, int prio);

int exttask_create9vs(ExtTask *task, void (*do_on_exit)(), const char *name,
                     void *(*func_ptr)(int, void *), int argc, void *argv, int prio,
                     int start, char add_to_parent);

int exttask_release(ExtTask *task);

/* возвратит только таск, даже если функция вызовется в треде - будет возвращён родительский таск */
ExtTask *exttask_current();

int exttask_suspend(ExtTask *task);
int exttask_resume(ExtTask *task);
int exttask_is_suspended(ExtTask *task);

int exttask_finalizator_add(ExtTask *task, void *func_ptr, void *arg);
int exttask_finalizator_del(ExtTask *task, void *func_ptr, void *arg);
void exttask_finalizators_run(ExtTask *task);
void exttask_release_childs(ExtTask *task);
struct IOCtl_system_handle *exttask_get_trc_by_id(ExtTask *task, int id);


char exttask_is_finished(ExtTask *task);
void *exttask_return_value(ExtTask *task);

/* может возвратить тред или таск */
ExtTask *CurrentProcessTask();

void exttask_set_preinit_func(ExtTask *task, void (*f)());
void exttask_set_finished_func(ExtTask *task, void (*f)());
void exttask_shutdown_systems(ExtTask *task);

void exttask_push_handle(ExtTask *task, ExtList *list, void (*handle)(void *), void *userdata);
void exttask_pop_handle(ExtTask *task, ExtList *list, void (*handle)(void *), void *userdata);
void exttask_notify_handles(ExtTask *task, ExtList *list);
void exttask_notify_handles_clean(ExtTask *task, ExtList *list);


void exttask_push_suspend_handle(ExtTask *task, void (*handle)(void *), void *userdata);
void exttask_pop_suspend_handle(ExtTask *task, void (*handle)(void *), void *userdata);
void exttask_notify_suspend_handles(ExtTask *task);

void exttask_push_resume_handle(ExtTask *task, void (*handle)(void *), void *userdata);
void exttask_pop_resume_handle(ExtTask *task, void (*handle)(void *), void *userdata);
void exttask_notify_resume_handles(ExtTask *task);

int exttask_suspend_child_threads(ExtTask *task);
int exttask_resume_child_threads(ExtTask *task);

int exttask_get_pid(ExtTask *task);
int exttask_wait_for_state(ExtTask *task, int state, int timeout);

#ifdef __cplusplus
}
#endif

#endif
