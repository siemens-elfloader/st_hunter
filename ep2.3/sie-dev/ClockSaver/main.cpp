#include "main.h"
#include "keyhook.h"
#include "conf_loader.h"
#include "cfg_param.h"
#include "csgui.h"

void Killer(void) {
    extern void *ELF_BEGIN;
    kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

#pragma inline=forced

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


csGUI *gui;		//êëàññ ãóéíè
extern int isGUI;	//ıòîò, êàê åãî
int tmr_sec=0;	//ñåêóíäû
GBSTMR timer;

void timer_proc ()	//ïğîöåññ òàéìåğà
{
	++tmr_sec;
	if (tmr_sec == 10)
	{
		gui->Close();
		SetIllumination(0,1,0,0);
		tmr_sec=0;
	}
	GBS_StartTimerProc(&timer,TMR_SECOND,timer_proc);
}

//{ ******* MY_CSM: *******

void MY_CSM::onCreate()	//ïğè ñîçäàíèè öñìà äåìîíà
{
	SetName("ClockSaver");	//ïğåäñòàâèìñÿ
	isGUI=0;		//ãóÿ íåòó
	SetHook();		//ñòàğòàíóëè êåéõóê
}

void MY_CSM::Close()	//Íó ıòî íå èíòåğåñíî
{
	CloseCSM(csm_id);	//âûøëè íàõ
}

int MY_CSM::onMessage(GBS_MSG* msg)	//ıòî òîæå ôèãíÿ
{
  if(msg->msg == MSG_RECONFIGURE_REQ)	//ñîîáùåíèå î ğåêîíôèãå
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      InitConfig();
    }
  }
  return CSM_MSG_RESULT_CONTINUE;
}

void MY_CSM::onClose()	//ïğè çàêğûòèè äåìîíà
{
	if (isGUI==1) gui->Close();	//óáèè ãóé, åñëè çàïóùåí
	DestroyHook();		//ñíÿëè êåéõóê
	SUBPROC((void *)Killer);
}


MY_CSM::MY_CSM()
{

}

MY_CSM::~MY_CSM()
{

}

//} ******* end of MY_CSM *******


// Start programm
int main(char *exename, void *params)	//ÏÎÅÕÀËÈ!!!111
{
	InitConfig();
	(new MY_CSM())->Create();

    return 0;
}

