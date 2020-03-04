
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <NuAPI/Thread.h>
#include <NuAPI/Signals.h>
#include <NuAPI/ExtendedNuTask.h>
#include <stdio.h>
#include <swilib.h>


typedef void (*function)(void);
#define NUM_ATEXIT	32


enum {
    LIBC_TASK_STARTING = 1,
    LIBC_TASK_INITIALIZED,
    LIBC_TASK_DESTROYING,
    LIBC_TASK_EXITING
};


static inline void libc_collect_args(int argc, char **argv){
    --argc;
    if(argc < 0) return;
    do
    {
        if(argv[argc])
           _free(argv[argc]);
    }while(argc-- > 0);
    _free(argv);
}



typedef struct
{
    ExtTask task;
    int (*main)(int argc, char **argv);
    void (*init)(void *task);
    void (*fini)(void *task);
    void *ex;
    int argc;
    char **argv;
    char *dir;
    int process_return;

    FILE *stdin, *stdout, *stderr;

    function atexitlist[NUM_ATEXIT];
    int atexit_counter;

    struct  {
        NuSignalsSystem sig_system;
    } signals ;

    int state;
    char need_self_free;
    char can_exit;
}LibcTask;



typedef struct {
    LibcTask *task;
    int can_exit;
}TaskParameters;


LibcTask * libc_get_task();
char *generate_full_patch(LibcTask *task, const char *mypatch);
void shutdown_helper_systems(LibcTask *task);
void ext_exit(LibcTask *task, int code) __attribute__((noreturn));

LibcTask *___libc_start(LibcTask *task, int free_struct, int can_exit,
                        void *__ex, int (*main)(int argc, char **argv), void (*init)(void *), void (*fini)(void *),
                        const char *exe, char **params);

LibcTask * sys_exec(LibcTask *task, int can_exit, const char *filename, char **argv);
#endif
