
#include <spl/process.h>
#include <spl/thread.h>
#include <spl/coreevent.h>
#include <spl/waitcondition.h>



int _tid[130] = {-1};
int csz = 129;
int ololo = 0;



int thread_main(void *d)
{
    printf("thread_main: pid: %d | tid: %d\n", getpid(), gettid());
    ololo++;
    return 0;
}



int main()
{
    initUsart();
    printf(" [+] main: pid: %d\n", getpid());

    for(int i=0; i<csz; ++i) {
        _tid[i] = createThread(94, thread_main, 0, 1);
    }

    for(int i=0; i<csz; ++i)
        waitForThreadFinished(_tid[i], 0);

    printf("All threads finished\n");
    printf("Ram %d\n", GetFreeRamAvail());


    char d[24];
    sprintf(d, "%d threads done", ololo);
    ShowMSG(1, (int)d);

    NU_Sleep(216*3);
    return 0;
}





