/*
  Типы данных, общие для мудуля main
*/
////////////////////////////////////////////////////////////////////////////////

/*int session_times;  // Время в сети
int session_max_times;  // Максимальное время в сети
//int session_all_times = 0;  // Общее время в сети
int session_num;  // Количество подключений к сети
int session_sr_times;  // Среднее время в сети*/

#pragma swi_number = 50 // Проигрование мелодии из прошивки
__swi __arm void PlaySound(long param1, long param2, long param3, long tone, long param5);
#pragma swi_number = 545 // SetCpuClockLow
__swi __arm void SetCpuClockLow();
#pragma swi_number = 546 // SetCpuClockTempHi
__swi __arm void SetCpuClockTempHi();
void execelf(char *exename, char *fname);

extern char *GetDateTimeNow(int Start);
extern int GetSessionTraffic(int blur);
extern int GetAllTraffic(int blur);
extern char *GetCharTraffic(int Traffic, int MB, int text);
extern char *GetCharMoney(int Traffic, int valute);
extern char *GetSessionTime(int Secunde, const char *Format);
extern char *GetTextFormat(const char *Mes, const char *Format);
extern int DoNotify(void);
extern void DoFree(int Close);
extern void OpenFile(int Start);
extern int GetSesInfo(int Now, int Max, int Sr);
extern void CheckLimit(void);
extern void ShowMessage(const char *Message);
extern void IlluminationOn();
extern void IlluminationOff();

extern void reconfig();

extern int GetPngFontHeight(void);
extern int GetPngFontWidth2(WSHDR *ws);
extern int GetPngFontWidth(void);
extern void DrawPngFontToIDLE(void *canvasdata,WSHDR *ws, int x, int y);

int my_keyhook(int submsg, int msg);
// для работы с файлами
void WriteStr(char * msg, char * fn);
void WriteStat(int Start); // Записываем статистику
// для вешанья на таймер
void timer_proc(void);
// обычные функции
void StartVibra(void);
void StopVibra(void);
