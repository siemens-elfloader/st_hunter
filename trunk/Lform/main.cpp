#include <swilib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include "LForm.h"
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

LForm *lf;

/*------------------------------------------------------------------------*/
/*--------------------- Создание цсм процесса и гуя ----------------------*/
/*------------------------------------------------------------------------*/
int _item(LForm *lf, int key, void *ud);

int LeftItem(LForm *lf, int item, void *ud)
{
    lf->bindKey(KEY_UP, '5', _item, 0);
    lf->setHeaderText("Key is binded!");
    lf->redraw();
    return 0;
}


class test1
{
public:

    int func(LForm *lf, int item, void *ud)
    {
        lf->setHeaderText(lf->item(item)->uitem.datac());
        lf->redraw();
        return 0;
    }
};


int _item(LForm *lf, int key, void *ud)
{
    ShowMSG(1, (int)"Test");
    lf->unbindKey(KEY_UP, '5');
    return 0;
}


static void maincsm_oncreate(CSM_RAM *data)
{
    MAIN_CSM *csm = (MAIN_CSM*)data;
    lf = new LForm(LForm::MultiLine);

    test1 test;

    lf->append(0, 0, "Hello1", "o_O",  LeftItem, 0);
    lf->append(0, 0, "Hello2", "ппц", sigc::mem_fun(test, &test1::func), 0);
    lf->append(0, 0, "Hello3", "O_o", 0, 0);
    lf->append(0, 0, "Hello4", "Оыаы", 0, 0);
    lf->append("Hello5", "Ы?");

    lf->bindKey(KEY_UP, '5', _item, 0);

    lf->show();

    csm->gui_id = lf->gui_id();
}


void ElfKiller(void)
{
    lf->close(true);
    printf("FreeRAM: %d\n", GetFreeRamAvail());

    char tmp[128];
    sprintf(tmp, "%d", GetFreeRamAvail());
    ShowMSG(1, (int)tmp);

    kill_elf();
}

static void maincsm_onclose(CSM_RAM *csm)
{
    ElfKiller();
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


int __mfd_o, __mfd_e;
void __m_atexit()
{
    fflush(stdout);
    close(__mfd_o);
    setvbuf(stdout, 0, _IONBF, 0);
    __setup_stdout_fd(-1);
}


int main (char *exename, char *fname)
{
    /*char tmp[128];
    getcwd(tmp, 128);
    strcat(tmp, "stdout.txt");
    __setup_stdout_fd( (__mfd_o = open(tmp, O_CREAT | O_WRONLY | O_TRUNC)) );

    setvbuf(stdout, 0, _IOFBF, 128);*/

    //atexit(__m_atexit);
    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();

    return 0;
}
