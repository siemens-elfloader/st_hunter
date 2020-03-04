
#include <iostream>
#include <string>
#include <sigc++/sigc++.h>
#include <swilib.h>
#include <nu_swilib.h>
//#include <stdio.h>


unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;
int my_csm_id=0;

typedef struct
{
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;

typedef struct
{
    GUI gui;
    WSHDR *ws;
} MAIN_GUI;


/*------------------------------------------------------------------------*/
/*--------------------- —оздание цсм процесса и гу€ ----------------------*/
/*------------------------------------------------------------------------*/

class test1
{
public:

    ~test1()
    {
    }

    void func()
    {
        signal.emit();
    }

    sigc::signal<void> signal;

};


class test2
{
    public:

    void func()
    {
        ShowMSG(1, (int)"Hello");
    }
};



static void maincsm_oncreate(CSM_RAM *data)
{

    test1 test;
    test2 _test;

    test.signal.connect( sigc::mem_fun(&_test, &test2::func) );

    test.func();

    CloseCSM(my_csm_id);
}

void ElfKiller(void)
{
    kill_elf();
}

static void maincsm_onclose(CSM_RAM *csm)
{

    SUBPROC((void *)ElfKiller);
}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    MAIN_CSM *csm=(MAIN_CSM*)data;
    if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
    {
        csm->csm.state=-3;
    }
    return(1);
}


const struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
} MAINCSM =
{
    {
        maincsm_onmessage,
        maincsm_oncreate,
#ifdef NEWSGOLD
        0,
        0,
        0,
        0,
#endif
        maincsm_onclose,
        sizeof(MAIN_CSM),
        1,
        &minus11
    },
    {
        maincsm_name_body,
        NAMECSM_MAGIC1,
        NAMECSM_MAGIC2,
        0x0,
        139,
        0
    }
};



void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NatAlarm");
}


int main (char *exename, char *fname)
{
    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    my_csm_id = CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();

    return 0;
}
