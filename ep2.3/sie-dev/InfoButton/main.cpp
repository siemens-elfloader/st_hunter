#include "main.h"
#include "ipc.h"
#include "conf_loader.h"
#include "keyhook.h"

#define DCSM_NAME "InfoButtonD"
#define CSM_NAME "InfoButtonD"
#define MSG_PR_ABSENT "Progress bars are absent!\nClose..."
#define TMR_MIN 217

extern const int CFG_KEY_CLOSE, CFG_AUTOUPDATE_TMR;
extern const RECT CFG_POS_BACK;
extern const char CFG_COL_BACK_BACK[4], CFG_COL_BACK_BORDER[4];

const char *percent_d="%d";
int CSM_ID; //csm id...
int GUI_ID; //gui id...
GBSTMR tmr;

void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN, (void(*)(void*))mfree_adr());
}

int toupper(int c)
{
    if ((c>='a')&&(c<='z')) c+='A'-'a';
    return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
    int i;
    int c;
    while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
    return(i);
}

void AutoUpdate(void)
{
    if(IsGuiOnTop(GUI_ID))
    {
        DirectRedrawGUI();
        GBS_StartTimerProc(&tmr, TMR_MIN*CFG_AUTOUPDATE_TMR, AutoUpdate);
    }
    else GBS_DelTimer(&tmr);
}

/*******************************************************************************
 MY_GUI
*******************************************************************************/

//{ ******* MY_GUI: *******
//

void MY_GUI :: DrawIMG(IMGHDR *img, int x, int y, int x2, int y2)
{
    RECT rc;
    DRWOBJ drwobj;
    if(!x2 && !y2)
    {
        StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
        SetPropTo_Obj5(&drwobj, &rc, 0, img);
    }
    else
    {
        StoreXYXYtoRECT(&rc, x, y, x2, y2);
        SetProp2ImageOrCanvas(&drwobj, &rc, 0, img, x, y);
    }
    SetColor(&drwobj, NULL, NULL);
    DrawObject(&drwobj);
}

void MY_GUI::onRedraw()
{
    DrawIMG(img);

    bg->Draw(CFG_POS_BACK.x, CFG_POS_BACK.y, CFG_POS_BACK.x2, CFG_POS_BACK.y2, CFG_COL_BACK_BACK,
             CFG_COL_BACK_BORDER);
    for(int i=0; i<countbar; i++)
    {
        bar[i]->SetCurrent();
        bar[i]->Draw(ws);
    }
}

void MY_GUI::onCreate()
{
    ws=AllocWS(128);
    for(int i=0; i<MAX_BAR; i++)
    {
        bar[countbar]=new PBar(i);
        if(bar[countbar]->GetState())
            countbar++;
        else delete bar[countbar];
    }
    if(countbar) bg=new Rectangle;
    else ShowMSG(1, (int)MSG_PR_ABSENT);
    //scrot
    int w=ScreenW(), h=ScreenH();
    unsigned int bitsize=w*h*2;
    img=new IMGHDR;
    img->bitmap=(char*)malloc(bitsize);
    memcpy(img->bitmap, RamScreenBuffer(), bitsize);
    img->w=w; img->h=h;
    img->bpnum=0x08;
    GUI_ID=gui_id;
}

void MY_GUI::onFocus()
{
#ifdef ELKA
    DisableIconBar(1);
#endif
    isFocused = 1;
    if(CFG_AUTOUPDATE_TMR) AutoUpdate();
}

void MY_GUI::onUnFocus()
{
#ifdef ELKA
    DisableIconBar(0);
#endif
    isFocused = 0;
}

void MY_GUI::onClose()
{
    for(int i=0; i<countbar; i++)
        if(bar[i]) delete bar[i];
    FreeWS(ws);
    if(bg) delete bg;
    if(img)
    {
        delete img;
        delete img->bitmap;
    }
    if(IsTimerProc(&tmr)) GBS_DelTimer(&tmr);
    delete this;
}

int MY_GUI :: onKey(char key_code, int key_msg, short keys)
{
    if(key_code==CFG_KEY_CLOSE)
    {
        Close();
        return GUI_RESULT_CLOSE;
    }
    return GUI_RESULT_OK;
}

void MY_GUI::Redraw()
{
    if (isFocused)
        REDRAW();
}

MY_GUI::MY_GUI()
{
    countbar=0;
    bg=NULL;
}

MY_GUI::~MY_GUI()
{
}

//} ******* end of MY_GUI *******


//{ ******* MY_CSM: *******

void MY_CSM::onCreate()
{
    SetName(CSM_NAME);

    gui=new MY_GUI();
    gui->Create();
    CSM_ID=csm_id;
}

void MY_CSM::Close()
{
    CloseCSM(csm_id);
}


int MY_CSM::onMessage(GBS_MSG* msg)
{
    if (msg->msg==MSG_GUI_DESTROYED && (int)msg->data0==gui->gui_id)
    {
        data->state=-3;
        gui->gui_id = NULL;
        Close();
    }
    return CSM_MSG_RESULT_CONTINUE;
}

void MY_CSM::onClose()
{
    if(gui->gui_id)
        gui->Close();
    CSM_ID=0;
}


MY_CSM::MY_CSM()
{

}

MY_CSM::~MY_CSM()
{

}

void CreateOpenCSM(void)
{
    if(!CSM_ID)
        (new MY_CSM())->Create();
    else
        IPC_Send(IPC_MY_NAME, IPC_MY_NAME, (void*)&CSM_ID, NULL);
}
/*==================DaemonCSM========================*/

void MY_DAEMON_CSM::onCreate()
{
    SetName(DCSM_NAME);
    IPC_Send(IPC_MY_NAME, IPC_MY_NAME, (void*)&csm_id, NULL);   //отсылаем сообщение сами себе
    InitConfig();
    InitKeyhook();
    //gui=new MY_GUI();
    //gui->Create();
}

void MY_DAEMON_CSM::Close()
{
    CloseCSM(csm_id);
}


int MY_DAEMON_CSM::onMessage(GBS_MSG* msg)
{
    if(msg->msg == MSG_RECONFIGURE_REQ)
	{
		if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
			InitConfig();
	}
    if (msg->msg==MSG_IPC)
    {
        IPC_REQ *ipc;
        if((ipc=(IPC_REQ*)msg->data0))  //принимаем сообщение
        {
            if (strcmp_nocase(ipc->name_to, IPC_MY_NAME)==0)    //если оно наше
            {
                int ipc_data=*((int*)ipc->data);
                if(csm_id!=ipc_data)  //если присланный id не равен текущему
                {
                    CloseCSM(ipc_data); //закрываем присланный
                    SUBPROC((void*)CreateOpenCSM);  //создаем цсм или открываем уже окрытый
                }
            }
        }//
    }
    return CSM_MSG_RESULT_CONTINUE;
}

void MY_DAEMON_CSM::onClose()
{
    DestroyKeyhook();
    if(CSM_ID) CloseCSM(CSM_ID);
    if(successed_config_filename)delete successed_config_filename;
    SUBPROC((void*)Killer);
    //if(gui->gui_id)
    //gui->Close();
}


MY_DAEMON_CSM::MY_DAEMON_CSM()
{
}

MY_DAEMON_CSM::~MY_DAEMON_CSM()
{

}



// Start programm
int main(char *exename, void *params)
{
    (new MY_DAEMON_CSM())->Create();
    return 0;
}

