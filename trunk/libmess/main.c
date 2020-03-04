
#include <swilib.h>
#include <messhook.h>


static CSM_DESC icsmd;
static CSM_RAM *old_icsm;
static message_handler_t base[128] = {0};
static int hcnt = 0;
static int (*old_message_handler)(CSM_RAM *data, GBS_MSG *msg);



static int newMessageHook() {

    for(int i=0; i<128; ++i)
        if(!base[i])
            return i;

    return -1;
}



int createMessageHook(message_handler_t h)
{
    int id = newMessageHook();
    if(id > -1) {
        base[id] = h;
        ++hcnt;
    }

    return id;
}


int deleteMessageHook(int id)
{
    if(id > -1 && id < 128) {
        base[id] = 0;
        --hcnt;
        return 0;
    }

    return -1;
}



static int message_handler(CSM_RAM *data, GBS_MSG *msg)
{
    int cnt = 0;
    for(int i = 0; i<128 && cnt < hcnt; ++i) {
        if(base[i]) {
            base[i](data, msg);
            cnt ++;
        }
    }

    return old_message_handler(data, msg);
}

/*
void patch_address32(void *_address, uint32_t data)
{
    char *address = (char*)&data;

    ApplyPatch((void*)((int)_address), address[0]);
    ApplyPatch((void*)((int)_address+1), address[1]);
    ApplyPatch((void*)((int)_address+2), address[2]);
    ApplyPatch((void*)((int)_address+3), address[3]);
}


void unpatch_address32(void *_address, uint32_t data)
{
    char *address = (char*)&data;

    UndoPatch((void*)((int)_address), address[0]);
    UndoPatch((void*)((int)_address+1), address[1]);
    UndoPatch((void*)((int)_address+2), address[2]);
    UndoPatch((void*)((int)_address+3), address[3]);
}*/

void _init()
{
    CSM_RAM *icsm = FindCSMbyID(CSM_root()->idle_id);
    memcpy(&icsmd, icsm->constr, sizeof(icsmd));
    old_icsm = (CSM_RAM *)icsm->constr;
    old_message_handler = icsmd.onMessage;
    icsmd.onMessage = message_handler;
    icsm->constr = &icsmd;

    /* avoid unloading library */
    int h = dlopen("libmess.so");
}


void _fini()
{
    CSM_RAM *icsm = FindCSMbyID(CSM_root()->idle_id);
    icsm->constr = (void *)old_icsm;
}


