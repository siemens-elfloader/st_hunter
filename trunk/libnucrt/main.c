#include <swilib.h>
#include <ep3/loader.h>
#include <process.h>

//#define LITE

extern int main(int , char **);
extern void __hcrt_run_initarray(void *_ex);
extern void *__ex;




static void __exec_finiarray()
{
    void stub_array() {}
    Elf32_Exec *ex = (Elf32_Exec *)&__ex;

    if(!ex->dyn[DT_FINI_ARRAY]) return;
    size_t sz = ex->dyn[DT_FINI_ARRAYSZ] / sizeof (void*);
    void ** arr = (void**)(ex->body + ex->dyn[DT_FINI_ARRAY] - ex->v_addr);

    for(size_t i=0; i < sz; ++i) {
        ( (void (*)())arr[i])();
        arr[i] = stub_array;
    }
}


static int hmain(int argc, char **argv)
{
    __hcrt_run_initarray((void*)&__ex);
    int ret = main(argc, argv);
    __exec_finiarray();

    return ret;
}


int _start(char *exe, char *fname)
{
    char **argv = 0;
    int argc = 1;

    if(fname)
        argc++;

    argv = malloc(argc * sizeof(void*));
    argv[0] = strdup(exe);

    if(fname)
        argv[1] = strdup(fname);

    int pid = createProcess(strrchr(exe, '\\')+1, 90, hmain, argc, argv, 0);

    if(pid > -1) {
        setProcessDieAction(pid, (void (*)(void *))kill_elf, 0); // kill_elf hack(или по нашему - костыль)
        resumeProcess(pid);
    } else {
        ShowMSG(1, (int)"Can`t create process");
        kill_elf();
    }

    return 0;
}






