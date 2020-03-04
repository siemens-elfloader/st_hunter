#include "../inc/swilib.h"
#include "stopwatch.h"
#include "interface.h"
#include "notify.h"

TTime time;
RESULTS res;
int saved_sec;
static GBSTMR tmr;

void StopWatch(void)
{
	extern int IsIdleOnTop();
	extern unsigned int gui_id;
	extern const int CFG_ALERT_SECONDS;
	
	const int step=6;
	time.millisec+=step;
	if(time.millisec>=100)
	{
		time.millisec-=100;
		
		TTime cur_time;
		GetDateTime(NULL, &cur_time);
		int cur_sec=GetSecondsFromTime(&cur_time);
		
		GetTimeFromSeconds(&time, cur_sec-saved_sec);
		
		if((time.min*60+time.sec)%CFG_ALERT_SECONDS==0) Notify();
		
		if(IsGuiOnTop(gui_id)) DrawCurTime(0); //отрисовываем все врямя, кроме секунд
		
		if(IsIdleOnTop())  DrawTimeOnIdle();
	}
	if(IsGuiOnTop(gui_id)) DrawCurTime(1); //отрисовываем только секунды времени
	GBS_StartTimerProc(&tmr, SIX_SEC/100, StopWatch);
}

int IsTime(TTime *time)
{
	if(time->millisec!=0) return 1;
	for(int i=0; i<=2; i++)
		if(*(&(time->sec)-sizeof(char)*i)!=0) return 1;
	return 0;
}

void StopWatch_Start(void)
{
	TTime t;
	GetDateTime(NULL, &t);
	saved_sec=GetSecondsFromTime(&t);//время запуска в секундах
	
	if(IsTime(&time))//коррекция времени запуска
		saved_sec-=GetSecondsFromTime(&time);
	
	StopWatch();
}

unsigned int IsStart(void){ return (IsTimerProc(&tmr)) ? 1 : 0; }

void Results_Rem(void)
{
	res.data=realloc(res.data, sizeof(TTime)*(res.total+1));
	memcpy(&res.data[res.total++], &time, sizeof(TTime));
	
}

void Results_Free(void)
{
	mfree(res.data);
	res.data=NULL;
	res.total=0;
    InitTime(&time, 0, 0, 0, 0);
}

void StopWatch_Stop(void)
{
  if(IsStart()) GBS_DelTimer(&tmr);
}
