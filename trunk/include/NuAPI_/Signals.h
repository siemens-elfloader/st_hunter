
#ifndef __SIGNALS_H__
#define __SIGNALS_H__


#include <NuAPI/ExtQueue.h>


enum
{
    NU_SIGNAL_EXIT = 1,
    NU_SIGNAL_ABORT,
    NU_SIGNAL_TERMINATE,
    NU_SIGNAL_EXCEPTION,
};


struct NuSignal
{
    int type;
    int pid;
    void *userdata;
} __attribute__((aligned(4)));


typedef struct
{
    NU_QUEUE queue;
    void *space;
    void (*signal_listener)();

}NuSignalsSystem;


int NuSignalsSystem_init(NuSignalsSystem *signals);
int NuSignalsSystem_release(NuSignalsSystem *signals);
int NuSignalsSystem_send(NuSignalsSystem *signals, int signal, void *userdata);
int NuSignalsWait(NuSignalsSystem *signals, struct NuSignal *sig);


#endif
