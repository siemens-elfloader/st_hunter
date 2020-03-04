
#include <spl/messhook.h>
#include <messhook.h>
#include <spl/static_data_base.h>
#include <spl/process.h>
#include <spl/coreevent.h>


static void messageHook(CSM_RAM *ram, GBS_MSG *msg);
static int mess_h = -1;
static int cnt = 0;
static int dl_mess_h = -1;


typedef struct{
    int id;
    int pid;
    int dt_id;
    void *userdata;
    message_hook_t hook;
}MessageHookData;

#define MAX_MHOOK_ID 128
declare_static_db(MessageHookData, messageHookData, MAX_MHOOK_ID);


static int crtStub(message_handler_t h) {
    return -1;
}

static int delStub(int i) {
    return -1;
}


int (*p_createMessageHook)(message_handler_t h) = crtStub;
int (*p_deleteMessageHook)(int id) = delStub;



void messageHookInit()
{
    dl_mess_h = dlopen("libmess.so");
    if(dl_mess_h > -1) {
        p_createMessageHook = dlsym(dl_mess_h, "createMessageHook");
        p_deleteMessageHook = dlsym(dl_mess_h, "deleteMessageHook");

        if(!p_createMessageHook || !p_deleteMessageHook) {
            ShowMSG(1, (int)"Failed to load libmess");
            p_createMessageHook = crtStub;
            p_deleteMessageHook = delStub;
        }

    } else {
        ShowMSG(1, (int)"libmess not found");
        p_createMessageHook = crtStub;
        p_deleteMessageHook = delStub;
    }

    mess_h = p_createMessageHook(messageHook);
    messageHookDataInit();
}


void messageHookFini()
{
    p_deleteMessageHook(mess_h);
    messageHookDataFini();


    dlclose(dl_mess_h);
    p_createMessageHook = crtStub;
    p_deleteMessageHook = delStub;
}


static void messageHook(CSM_RAM *ram, GBS_MSG *msg)
{
    typedef struct
    {
        CoreEvent head;
        MessageHookData *hdata;
        CSM_RAM ram;
        GBS_MSG msg;
    }Event;

    void disp(void *e) {
        Event *ev = (Event*)e;
        ev->hdata->hook(ev->hdata->id, &ev->ram, &ev->msg);
    }

    Event event;

    event.head.dispatcher = disp;

    memcpy(&event.ram, ram, sizeof(*ram));
    memcpy(&event.msg, msg, sizeof(*msg));

    int c = 0;
    for(int i=0; i<MAX_MHOOK_ID && c < cnt; ++i)
    {
        if(_db[i].id > -1) {
            event.hdata = &_db[i];
            sendEvent(_db[i].pid, &event, sizeof event);
            c++;
        }
    }
}


int regMessageHook(message_hook_t h, void *userdata)
{
    MessageHookData *m = newMessageHookDataData();
    if(!m)
        return -1;

    m->pid = getpid();
    m->hook = h;
    m->userdata = userdata;
    cnt++;

    m->dt_id = addProcessDtors(m->pid, (void (*)(void*, void*))unregMessageHook, (void*)m->id, 0);
    return m->id;
}


int unregMessageHook(int id)
{
    MessageHookData *m = checkMessageHookDataData(id);
    if(!m)
        return -1;

    eraseProcessDtor(m->pid, m->dt_id);

    freeMessageHookDataData(id);
    cnt--;
    return 0;
}


void *messageHookUserData(int id)
{
    MessageHookData *m = checkMessageHookDataData(id);
    if(!m)
        return 0;

    return m->userdata;
}

