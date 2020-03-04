
#include <swilib.h>
#include <spl/process.h>
#include <spl/thread.h>
#include <spl/memctl.h>
#include <spl/signals.h>


void kill_hisr_chek(int _pid, int tid, int code, int hisr, int signum);


sighandler_t signal(int signum, sighandler_t action)
{
    CoreProcess *proc = coreProcessData(getpid());
    if(!proc || proc->t.id < 0)
        return 0;

    struct CoreListInode *inode;
    corelist_foreach(inode, proc->siglist.list.first)
    {
        struct siglist_data *d = inode->self;

        if(d->signum == signum) {
            return d->sighandler;
        }
    }

    struct siglist_data *data = memoryAlloc(proc->t.id, sizeof *data);
    data->signum = signum;
    data->sighandler = action;
    corelist_push_back(&proc->siglist.list, data);
    return data->sighandler;
}



int kill(int pid, int signum)
{
    int hisr = NU_Current_Task_Pointer() == 0? 1 : 0;

    switch(signum)
    {
        case SIGKILL:
            kill_hisr_chek(pid, gettid(), -1, hisr, signum);
            break;

        case SIGTERM:
            kill_hisr_chek(pid, gettid(), -1, hisr, signum);
            break;

        case SIGSTOP:
            if(pid == getpid())
                return -1;
            suspendProcessThreads(pid);
            suspendProcess(pid);
            break;

        case SIGCONT:
            resumeProcessThreads(pid);
            resumeProcess(pid);
            break;

        default:
            return -1;
    }

    return 0;
}
