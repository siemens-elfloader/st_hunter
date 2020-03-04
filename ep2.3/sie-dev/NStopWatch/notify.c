#include "../inc/swilib.h"

static GBSTMR tmr;
extern const int CFG_VIBRA_POWER;

void Vibra_Start(void)
{
	void Vibra_Stop(void);
	SetVibration(CFG_VIBRA_POWER);
	GBS_StartTimerProc(&tmr, 100, Vibra_Stop);
}

void Vibra_Stop(void)
{
	SetVibration(0);
	GBS_DelTimer(&tmr);
}

void Notify(void)
{
	extern const int CFG_NUMBER_SYSTEM_SOUND;
	if(!IsCalling())
	{
		if(CFG_NUMBER_SYSTEM_SOUND)
			PlaySound(1, 0, 0, CFG_NUMBER_SYSTEM_SOUND, 0);
		if(CFG_VIBRA_POWER)
			Vibra_Start();
	}
}
