
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "task.h"
#include "corearray.h"
#include "corelist.h"
#include "resctl.h"

typedef struct
{
    int prio;
    int stack_size;
    void *stack;
    char is_stack_freeable;
}ProcessConf;

typedef struct
{
    CoreTask t;

    CoreArray ctors, dtors;
    char kill_mode, terminated;

    struct {
        void (*kik)(void *);
        void *d;
    } kostil;

    struct {
        CoreList list;
    } threads;

    ResCtlData *resData;
    int resAttached;
    CoreArray idUsersData;

    int start_wait_cond,
        exit_wait_cond;
    char *name;
    void *userdata;
    short ppid;

    int (*main)(int, char**);
    int argc;
    char **argv;
    int retcode;

}CoreProcess;


CoreProcess *coreProcessData(short pid);
int sendEvent(int pid, void *event, size_t size);
void initProcessConf(ProcessConf *conf);

int createConfigurableProcess(ProcessConf *conf, const char *name, int (*_main)(int, char**), int argc, char **argv, int run);
int createProcess(const char *name, int prio, int (*_main)(int, char**), int argc, char **argv, int run);
int resetProcess(int pid, int argc, char **argv);

int getpid(void);
int getppid(void);
int getParentPid(int pid);
const char *pidName();

void kill(int pid, int code);
void exit(int code);
void quit();

struct CoreListInode *addProcessThread(int pid, int tid);
int delProcessThread(int pid, struct CoreListInode *inode);

int fullSuspendProcess(int pid);
int fullResumeProcess(int pid);
int suspendProcess(int pid);
int resumeProcess(int pid);

int setProcessUserdata(int pid, void *d);

int addProcessCtors(int pid, void (*h)(void *, void *), void *data1, void *data2);
int addProcessDtors(int pid, void (*h)(void *, void *), void *data1, void *data2);
int setProcessDieAction(int pid, void (*h)(void *), void *d);

int eraseProcessCtor(int pid, int at);
int eraseProcessDtor(int pid, int at);

int waitForProcessStarted(int _pid);
int waitForProcessFinished(int _pid, int *retcode);

int attachProcessIdUserData(int _pid, void *userData);

ResCtlData *dataOfResCtl(int _pid, int id);
#endif
