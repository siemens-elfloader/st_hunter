#ifndef _STOPWATCH_H_
  #define _STOPWATCH_H_

#define SIX_SEC 1300

typedef struct
{
    TTime *data;
    int total;
}RESULTS;

void StopWatch_Start(void);  //старт секундомера
void StopWatch_Stop(void);           //остановка таймера
unsigned int IsStart(void);          //проверка на активность
void Results_Rem(void);//сохранение результатов
void Results_Free(void);   //очистка результатов

extern TTime time;
extern RESULTS res;
#endif
