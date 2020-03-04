
#include <swilib.h>
#include <nu_swilib.h>
#include <spl/coreevent.h>
#include <spl/timer.h>
#include <spl/process.h>
#include <spl/static_data_base.h>


typedef struct {
    CoreEvent head;
    void (*callback)(int, void *);
    void *userdata;
}TimerEvent __attribute__((aligned(4)));

typedef struct {
    NU_TIMER timer;
    void (*callback)(int, void *);
    int id;
    int pid;
    int sended;
    int dt_id;
    char last;
    void *userdata;
}TimerData;


#define MAX_TIMER_ID 128
declare_static_db(TimerData, timerData, MAX_TIMER_ID);


void timersInit()
{
    timerDataInit();
}


void timersFini()
{
    timerDataFini();
}



static void timer_displatcher(TimerEvent *event)
{
    if(_db[event->head.id].pid < 0)
        return;

    _db[event->head.id].sended --;
    if(_db[event->head.id].last) {
        freeTimerDataData(event->head.id);
        return;
    }

    event->callback(event->head.id, event->userdata);

}


static void timer_handle(unsigned long id) //?
{
    TimerEvent event;
    event.head.id = id;
    event.head.type = TIMER_EVENT;
    event.callback = _db[id].callback;
    event.head.dispatcher = (void (*)(void*))timer_displatcher;
    event.userdata = _db[id].userdata;

    _db[id].sended++;
    sendEvent(_db[id].pid, &event, sizeof event);
}


int timerStart(unsigned long time, void (*callback)(int, void *), void *userdata)
{
    TimerData *timer = newTimerDataData();
    if(!timer)
        return -1;

    int pid = getpid();
    enterProcessCriticalCode(pid);
    if(isProcessKilling(pid) == 1)
        return -1;

    timer->callback = callback;
    timer->pid = pid;
    timer->sended = 0;
    timer->last = 0;
    timer->userdata = userdata;


    int err = NU_Create_Timer((NU_TIMER*)timer, "ololo", timer_handle, timer->id, time, time, NU_ENABLE_TIMER);
    if(err != NU_SUCCESS) {
        printf("timerStart: err %d\n", err);
    } else
        timer->dt_id = addProcessDtors(timer->pid, (void (*)(void*, void*))timerStop, (void *)timer->id, 0);

    leaveProcessCriticalCode(pid);

    return err == NU_SUCCESS?
            timer->id : err;
}


int timerReset(int id, unsigned long time, void (*callback)(int, void *), void *userdata)
{
    TimerData *timer = checkTimerDataData(id);
    if(!timer)
        return -1;

    timer->callback = callback;
    timer->pid = getpid();
    timer->sended = 0;
    timer->last = 0;
    timer->userdata = userdata;

    return NU_Reset_Timer((NU_TIMER*)timer, timer_handle, time, time, NU_ENABLE_TIMER);
}


int timerPause(int id)
{
    TimerData *timer = checkTimerDataData(id);
    if(!timer)
        return -1;

    return NU_Control_Timer((NU_TIMER*)timer, NU_DISABLE_TIMER);
}


int timerResume(int id)
{
    TimerData *timer = checkTimerDataData(id);
    if(!timer)
        return -1;

    return NU_Control_Timer((NU_TIMER*)timer, NU_ENABLE_TIMER);
}


int timerStop(int id)
{
    TimerData *timer = checkTimerDataData(id);
    if(!timer)
        return -1;

    eraseProcessDtor(timer->pid, timer->dt_id);

    timerPause(id);
    NU_Control_Timer((NU_TIMER*)timer, NU_DISABLE_TIMER);
    int err = NU_Delete_Timer((NU_TIMER*)timer);

    if(timer->sended < 1)
        timer->pid = -1;
    else
        timer->last = 1;
    return err;
}


void *timerUserData(int id)
{
    TimerData *timer = checkTimerDataData(id);
    if(!timer)
        return 0;

    return timer->userdata;
}


