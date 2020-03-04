#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "GPRS MAN.h"
#include "..\inc\csm.h"
#include "..\inc\str.h"
#include "main.h"

/*#pragma swi_number = 221 // Низкая частота проца
__swi __arm void SetCpuClockLow();

#pragma swi_number = 222 // Высокая частота проца
__swi __arm void SetCpuClockTempHi();*/


// -------------------------- Global Variables -------------------------------------

/*#pragma inline=forced

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
}*/
//-----------------------------------------------------------------------------------
/*
typedef struct  // объявляем структурку для IP-заголовка
{
  char Ttl;
  char Tos;
  char IPFlags;
  char OptSize;
  char Options;
}IPINFO;

typedef struct  // объявляем структурку для хранения ICMP пакета 
{
  long Source;
  long Status;
  long RTTime;
  short DataSize;
  short Reserved;
  unsigned pData; 
  IPINFO i_ipinfo;
}ICMPECHO;
*/
/////////////////////////////////////////////////////////////////////////////////

const char perc_t[]="%t";
const char perc_s[]="%s";

//////////////////////////     Переменные для сети     //////////////////////////

/*int sock=-1;  // номер сокета
int DNR_ID=0; // ХЗ
int DNR_TRIES=3;  // ХЗ*/

//////////////////////////  Переменные для состояние  //////////////////////////

//int is_gprs_online; // Состояние GPRS
//int CONECTED = 0; // Состояние подключения к хосту
volatile int VIBRA=0; // Состояние вибрации
/*int recv_ping = 1;  // Пинг принят
int send_ping = 0;  // Пинг отправлен*/
int profile;  // Номер профиля
int UpDateInfo = 0; // Состояние обновления информации

//////////////////  Переменные для временного хранения данных  //////////////////

//char LAST_HOST[64]; // Предыдущий хост
int session_all_times = 0;  // Общее время в сети
int session_times = 0;  // Время в сети
int session_max_times = 0;  // Максимальное время в сети
//int session_all_times = 0;  // Общее время в сети
int session_num = 0;  // Количество подключений к сети
int session_sr_times = 0;  // Среднее время в сети
int oldGPRSTraffic;  // Старый размер GPRS трафика
int h,m,s;  // Вермя сессии
int HookEnable = 0;

char *text;// = malloc(128);

//////////////////////////         Пути к файлам       //////////////////////////

char LOG[128] = DEFAULT_DISK":\\Zbin\\GPRS MAN.log"; // Путь к лог файлу
char Param[128] = ""; //  Параметр запуска эльфа

//////////////////////////            Таймеры          //////////////////////////

//GBSTMR reconnect_tmr; // Таймер переподключения
//GBSTMR AutoPing;  // Таймер авто пинга
//GBSTMR CheckPing;  // Таймер проверки пинга
GBSTMR timer;   // Таймер слежения за состоянием GPRS
GBSTMR VibraTimer;  // Таймер вибрации
GBSTMR UpdaTimer;  // Таймер обновления инфы.
//GBSTMR TimeHook;  // Таймер отсчета времени сессии

//////////////////////////            Другие          //////////////////////////


TDate date;
TTime time; 
/*TDate date_;
TTime time_; */

//////////////////////////     Текстовые переменные   //////////////////////////

const char ses_start[128] = "Начало сессии %02d.%02d.%04d %02d:%02d:%02d\r\n";
const char ses_end[128] = "Конец сессии %02d.%02d.%04d %02d:%02d:%02d\r\n";
const char ses_traf[128] = "Траффик сессии %s\r\n";
const char ses_traf_blur[128] = "Округленный траффик %s\r\n";
const char ses_money[128] = "Потрачено денег %s\r\n";
const char ses_time[128] = "Время сессии %02d:%02d:%02d\r\n";
const char file_open[128] = "Было открыто файлов: %d";
const char ipc_my_name[] = "iGPRS 0.4.7 (c)PICAS";

// Переменные для вывода на экран

typedef struct{
  int enabled;
  int isconnect;
  int islock;
  RECT rc;
  WSHDR *ws;
  char pen[4];
  unsigned short font;
  unsigned short type;
  WSHDR wsh;
  unsigned short wsbody[WS_MAXLEN+1];
  unsigned int draw_flag;
  char fr_cl[4];
} TInfo;

TInfo InfoData[6];

#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

////////////////////////////////////////////////////////////////////////////////
//                              Основные функции                              //
////////////////////////////////////////////////////////////////////////////////

void FreeText(void)
{
  zeromem(text, 128); // обнуляем, воизбежание некоторых ошибок...  
}


char *GetDateTimeNow(int Start)
{
	TDate idate; TTime itime; 

  FreeText();

	GetDateTime(&idate,&itime);
	if (Start)
		sprintf(text,ses_start,idate.day,idate.month,idate.year,itime.hour,itime.min,itime.sec); 
	else
		sprintf(text,ses_end,idate.day,idate.month,idate.year,itime.hour,itime.min,itime.sec);

	return text;
}

int GetSessionTraffic(int blur)
{
	int GPRSTraffic=0; // обнуляем, иначе функция может вернуть какую-нибудь херню...
    
	RefreshGPRSTraffic();
	GPRSTraffic = *GetGPRSTrafficPointer() - oldGPRSTraffic;  // Получаем трафик за сессию
	if (blur && trafic_blur) // Округляем
	{
		if (GPRSTraffic % (trafic_blur * 1024))
			GPRSTraffic = ((GPRSTraffic / (trafic_blur * 1024))+1)*(trafic_blur * 1024);
	}
	return GPRSTraffic;
}

int GetAllTraffic(int blur)
{
	int GPRSTraffic=0;
    
	RefreshGPRSTraffic();
	GPRSTraffic = *GetGPRSTrafficPointer();  // Получаем весь трафик
	if (blur && trafic_blur)
	{
		if (GPRSTraffic % (trafic_blur * 1024))
			GPRSTraffic = ((GPRSTraffic / (trafic_blur * 1024))+1)*(trafic_blur * 1024);
	}
	return GPRSTraffic;
}

char *GetCharTraffic(int Traffic, int MB, int Text)
{
	float tr;
	
  FreeText();
  
	if (MB)
	{
		tr = (float)Traffic/(float)(1024*1024);
		if (!Text)
			sprintf(text,"%.2f",tr); 
		else
			sprintf(text,"%.2f %s",tr,text_mb); 
	}
	else
	{
		tr = (float)Traffic/(float)(1024);
		if (!Text)
			sprintf(text,"%.2f",tr); 
		else
			sprintf(text,"%.2f %s",tr,text_kb); 
	}
	return text;
}

char *GetCharMoney(int Traffic, int valute)
{
	int money;
	float r;
	
  FreeText();
  
	money = trafic_gold*100+trafic_silver;
	r = ((float)(Traffic / 1024) / (float)trafic_size)*(money / (float)100);
    
	if (valute)
	{
		sprintf(text,"%.2f %s",r,text_valute); 
	}
	else
	{
		sprintf(text,"%.2f",r); 
	}

	return text;
}

char *GetSessionTime(int Secunde, const char *Format)
{
            int h=0,m=0,s=0,temp=0;
            
  FreeText();
        
            temp = Secunde;
            h = temp / 3600; // вычисляем часы
            temp = temp - h*3600;
            m = temp / 60; // вычесляем минуты
            temp = temp - m*60;
            s = temp; // секунды...     

              sprintf(text,Format,h,m,s);
            
            return text;
}

char *GetTextFormat(const char *Mes, const char *Format)
{
            char Message[128];
            
            sprintf(Message,Format,Mes);
          
            return Message;
}

int GetSesInfo(int Now, int Max, int Sr)
{
  if (Now)
    return session_times;
  if (Max)
    return session_max_times;
  if (Sr)
    return session_sr_times;
  return 0;
}

int DoNotify(void)
{
        int OK = 1; // Переменная для проверки состояния, можно ли уведомлять?
        if (NOTIFY_EX)  // Проверяем время уведомления
        {
          TDate now_date; TTime now_time; 
          GetDateTime(&now_date,&now_time); 
          int _h,_h2,_h3,_m,_m2,_m3;
          _h = now_time.hour;
          _h2 = TimeFrom.hour;
          _h3 = TimeTo.hour;
          _m = now_time.min;
          _m2 = TimeFrom.min;
          _m3 = TimeTo.min;
            
          if (!_h) _h = 24;
          if (!_h2) _h2 = 24;
          if (!_h3) _h3 = 24;
          
          if (!_m) _m = 60;
          if (!_m2) _m2 = 60;
          if (!_m3) _m3 = 60;
            
          if ((_h >= _h2) && (_h <= _h3))// Если время совпадает с диапозоном
          { //   18  <> 60       18 <> 60      13 <> 22        13 <> 8
            if (((_m =! _m2) && (_h != _h2)) || ((_m != _m3) && (_h != _h3)))  // Если время и часы не совпадают с данными из конфига
                {              
                  OK = 1; // Уведомление включено
                }
                else
                {
                  OK = 0; // Уведомление выключено
                }
          }
          else
          {
            OK = 0; // Уведомление выключено
          }
        }
        return OK;
}

void OpenFile(int Start)
{
  int open = 0;
  char FilePath[128];
  
  if (reg_enable)// Если надо, то
  {
    if (Start)
    {
      if (reg_create_enable)  // Открываем файлы...
          {
            if (strlen(reg_create_file_1) > 0)
            {
              strcpy(FilePath,reg_create_file_1);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_2) > 0)
            {
              strcpy(FilePath,reg_create_file_2);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_3) > 0)
            {
              strcpy(FilePath,reg_create_file_3);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_4) > 0)
            {
              strcpy(FilePath,reg_create_file_4);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_5) > 0)
            {
              strcpy(FilePath,reg_create_file_5);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_6) > 0)
            {
              strcpy(FilePath,reg_create_file_6);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_7) > 0)
            {
              strcpy(FilePath,reg_create_file_7);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_8) > 0)
            {
              strcpy(FilePath,reg_create_file_8);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_9) > 0)
            {
              strcpy(FilePath,reg_create_file_9);
              execelf(FilePath,Param);
              open++;
            }
            if (strlen(reg_create_file_10) > 0)
            {
              strcpy(FilePath,reg_create_file_10);
              execelf(FilePath,Param);
              open++;
            }
          }
    }
    else
    {
      if (regulation_close_enable)  // Открываем файлы..
            {
              if (strlen(regulation_close_file_1) > 0)
              {
                strcpy(FilePath,regulation_close_file_1);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_2) > 0)
              {
                strcpy(FilePath,regulation_close_file_2);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_3) > 0)
              {
                strcpy(FilePath,regulation_close_file_3);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_4) > 0)
              {
                strcpy(FilePath,regulation_close_file_4);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_5) > 0)
              {
                strcpy(FilePath,regulation_close_file_5);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_6) > 0)
              {
                strcpy(FilePath,regulation_close_file_6);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_7) > 0)
              {
                strcpy(FilePath,regulation_close_file_7);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_8) > 0)
              {
                strcpy(FilePath,regulation_close_file_8);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_9) > 0)
              {
                strcpy(FilePath,regulation_close_file_9);
                execelf(FilePath,Param);
                open++;
              }
              if (strlen(regulation_close_file_10) > 0)
              {
                strcpy(FilePath,regulation_close_file_10);
                execelf(FilePath,Param);
                open++;
              }
            }
            
    }
   
  if (reg_file)
  {
    FreeText();
    sprintf(text, file_open,open);
    ShowMessage(text);
  }
    
  }
    
}

void DoFree(int Close)
{
      if (Close)
        GBS_DelTimer(&timer);
      GBS_DelTimer(&VibraTimer);
      free_ICONS();
      FreeText();
}

void CheckLimit(void)
{
  if ((LIMIT_TRAF_ACTIVE) && ( ((LIMIT_TRAF_KB) && (GetSessionTraffic(0) / 1024) > LIMIT_TRAF_KB) || ((LIMIT_TRAF_SEC) && (GetSesInfo(1,0,0) > LIMIT_TRAF_SEC)) ))
  {
    if (LIMIT_TRAF_OFF)
      GPRS_OnOff(0,1);
    if (LIMIT_TRAF_PLAY)
      PlaySound(1,0,0,LIMIT_TRAF_NUM,0);  // Звук..
    if (LIMIT_TRAF_VIBRA)
    {
      SetVibration(LIMIT_TRAF_POWER);        
      GBS_StartTimerProc(&VibraTimer, TMR_SECOND/3, StopVibra);
    }
    if (LIMIT_TRAF_MES)
      ShowMessage(LIMIT_TRAF_TEXT);
  }
}

void ShowMessage(const char *Message)
{
  ShowMSG(ShowMSG_Param,(int)Message);
}

GBSTMR tmr_illumination;

// SetIllumination(0-дисплей, 1 - клава. ХЗ.процент яркости,время подсветки)

void IlluminationOff()
{
  if (IsUnlocked())
    SetIllumination(0,1,0,TMR_SECOND/10*NOTIFY_SEND_NORM_FADE);
  else
    SetIllumination(0,1,0,TMR_SECOND/10*NOTIFY_SEND_LOCK_FADE);
  if (IsUnlocked())
    SetIllumination(1,1,0,TMR_SECOND/10*NOTIFY_SEND_NORM_FADE);
  else
    SetIllumination(1,1,0,TMR_SECOND/10*NOTIFY_SEND_LOCK_FADE);
}

void IlluminationOn()
{
  GBS_DelTimer(&tmr_illumination);
  // Дисплей
  if (IsUnlocked() && NOTIFY_SEND_NORM_DISP)
  {
    SetIllumination(0,1,NOTIFY_SEND_NORM_ILL,TMR_SECOND/10*NOTIFY_SEND_NORM_FADE);
  }
  if (!IsUnlocked() && NOTIFY_SEND_LOCK_DISP)
  {
    SetIllumination(0,1,NOTIFY_SEND_LOCK_ILL,TMR_SECOND/10*NOTIFY_SEND_LOCK_FADE);
  }
  // Клава
  if (IsUnlocked() && NOTIFY_SEND_NORM_KEY)
  {
    SetIllumination(1,1,NOTIFY_SEND_NORM_ILL,TMR_SECOND/10*NOTIFY_SEND_NORM_FADE);
  }
  if (!IsUnlocked() && NOTIFY_SEND_LOCK_KEY)
  {
    SetIllumination(1,1,NOTIFY_SEND_LOCK_ILL,TMR_SECOND/10*NOTIFY_SEND_LOCK_FADE);
  }

  if (IsUnlocked())
    GBS_StartTimerProc(&tmr_illumination,TMR_SECOND/10*NOTIFY_SEND_NORM_TIMEOUT,IlluminationOff);
  else
    GBS_StartTimerProc(&tmr_illumination,TMR_SECOND/10*NOTIFY_SEND_LOCK_TIMEOUT,IlluminationOff);
}

////////////////////////////////////////////////////////////////////////////////
//                              Работа с файлами                              //
////////////////////////////////////////////////////////////////////////////////

void WriteStr(char * msg, char * fn)  // запись в файл
{
  int f;
  unsigned err;
  
  if ((f=fopen(fn,A_WriteOnly+A_BIN+A_Create+A_Append,P_WRITE,&err))!=-1)
  {
    fwrite(f,msg,strlen(msg),&err);
    fclose(f,&err);
  }   
 
}

void WriteStat(int Start) // Записываем статистику
{
  char STATPATH[128];

  strcpy(STATPATH,STAT_FILEPATH);

if (Start)
{
  if (STAT_START_TIME)  // Начало
  {
    WriteStr("=====================\r\n",STATPATH);
    WriteStr(GetDateTimeNow(1),STATPATH);
  }
}
else
{
    if (STAT_ENABLE)  // Конец
    {
          
      if (START_END_TIME)
      {
        WriteStr(GetDateTimeNow(0),STATPATH);
      }        
          
      if (STAT_TRAF)  // Трафик
      {
        WriteStr(GetTextFormat(GetCharTraffic(GetSessionTraffic(0),0,1),ses_traf),STATPATH);
      }   
      
      if (STAT_BLUR_TRAF)
      {
        WriteStr(GetTextFormat(GetCharTraffic(GetSessionTraffic(1),0,1),ses_traf_blur),STATPATH);
      }         
          
      if (STAT_TIME)  //Время сессии
      {
        WriteStr(GetSessionTime(session_times,ses_time),STATPATH);
      }
          
      if (STAT_MONEY) //Стоимость
      {                 
        WriteStr(GetTextFormat(GetCharMoney(GetSessionTraffic(1),1),ses_money),STATPATH);
      }
    }
}
}


void execelf(char *exename, char *fname)  // открытия эльфа/файла
{ //fname-параметр (имя файла), передаваемый в эльф 
  WSHDR *ws=AllocWS(256); 
  str_2ws(ws,exename,strlen(exename)+1); 
  ExecuteFile(ws,0,fname); FreeWS(ws);
}
////////////////////////////////////////////////////////////////////////////////
//                             Вывод png шрифтом                              //
////////////////////////////////////////////////////////////////////////////////

int GetPngFontHeight(void)
{
  char text = 'a';
  int pic;
 
  char temp[128];
        sprintf(temp,"%s%d.png",PNG_FOLDER,(text+2048)); // В общем сама картинка
        pic = (int)temp;
        return GetImgHeight(pic);
}

int GetPngFontWidth(void)
{
  char text = 'a';
  int pic;
 
  char temp[128];
        sprintf(temp,"%s%d.png",PNG_FOLDER,(text+2048)); // В общем сама картинка
        pic = (int)temp;
        return GetImgWidth(pic);
}

int GetPngFontWidth2(WSHDR *ws)
{
  char text[128];
  ws_2str(ws,text,ws->maxlen);
  int length = strlen(text);  // Длина строки текста
  int X,pic;
  
  for (int i = 0; i <=length; i++)
  {
    {
        char temp[128];
        sprintf(temp,"%s%d.png",PNG_FOLDER,(text[i]+2048)); // В общем сама картинка
        pic = (int)temp;
      if ((!islower2(text[i]) && (!png_full)) || ((png_full)))
      {

      if (i != 0)
      {
        X = X + GetImgWidth(pic)+PNG_LEN;
      }
      else
      {
        X = 0;
      }
      
      }
    }
  } 
 return X;
}


void DrawPngFontToIDLE(void *canvasdata,WSHDR *ws, int x, int y)
{
  char text[128];
  ws_2str(ws,text,ws->maxlen);

  int length = strlen(text);  // Длина строки текста
  int X,pic;  // положение картинки и указатель на саму картинку

  //////////////////////////////////////
  for (int i = 0; i <=length; i++)
  {
    {

      if ((!islower2(text[i]) && (!png_full)) || ((png_full)))
      {

      if (i != 0)
      {
        X = X + GetImgWidth(pic)+PNG_LEN;
      }
      else
      {
        X = x;
      } 
      // если а прописная и шрифт полный или а не прописная и шрит не полный
        char temp[128];
        sprintf(temp,"%s%d.png",PNG_FOLDER,(text[i]+2048)); // В общем сама картинка
        pic = (int)temp;
        if (canvas_on_idle && CLOSE)
          DrawCanvas(canvasdata, X, y, X + GetImgWidth(pic),y+GetImgHeight(pic), 1);  // Рисуем канву
        DrawImg(X,y,pic); // Картинку
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                     Другие                                 //
////////////////////////////////////////////////////////////////////////////////

int IsIDLE(void)
{
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  if (IsGuiOnTop(((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])) return(1);
                                                           else return(0);
}

////////////////////////////////////////////////////////////////////////////////
//                     Прослушивание входящих пакетов GPRS                    //
////////////////////////////////////////////////////////////////////////////////

void READ_DATA(int sock_hook)  
{
  char *msg_buf;
  char hook_path[128];
  
  msg_buf = malloc(interception_buffer);
  sprintf(hook_path,interception_FILEPATH,sock_hook); 
  recv(sock_hook,msg_buf,interception_buffer-1,0);

  WriteStr(msg_buf,hook_path);
  mfree(msg_buf);
}

////////////////////////////////////////////////////////////////////////////////
//                               Статистика GPRS                              //
////////////////////////////////////////////////////////////////////////////////

void FillInfoData(TInfo *Info,int x_start,int y_start, int font,const char *color,int draw_flag,const char *fr_cl)
{
  Info->rc.x=x_start;
  Info->rc.y=y_start;
  Info->rc.x2=x_start+Get_WS_width(Info->ws,font)+1;  //без +1 неправильно работает обводка
  Info->rc.y2=y_start+GetFontYSIZE(font);
  Info->font=font;
  memcpy(Info->pen,color,4);
  Info->draw_flag=draw_flag;
  memcpy(Info->fr_cl,fr_cl,4);
}

void InitInfoData(void)
{
  if (IDLE_SES_ENABLE)
  {

      InfoData[0].enabled=IDLE_SES_ENABLE;
      InfoData[0].isconnect=IDLE_SES_ACTIVE;
      InfoData[0].islock=IDLE_SES_LOCK;     
          
                        if (!png_on_idle)
			wsprintf(InfoData[0].ws, perc_t, GetSessionTime(session_times,IDLE_SES_MES));
                        else
			wsprintf(InfoData[0].ws, perc_s, GetSessionTime(session_times,IDLE_SES_MES));                          
			
      FillInfoData(&InfoData[0],IDLE_SES_X,IDLE_SES_Y,IDLE_SES_FONT,IDLE_SES_COLORS,
                   IDLE_SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,
                   IDLE_SES_FRINGING_COLORS);
  }
  else
    InfoData[0].enabled=0;    
  //////////      СТОИМОСТЬ СЕССИИ    /// Полет нормальный, но иногда очень сильно завышает стоимость, на 2 еденицы..

      if (IDLE_TRAF_SES_ENABLE)
      {
     
      InfoData[1].enabled=IDLE_TRAF_SES_ENABLE;
      InfoData[1].isconnect=IDLE_TRAF_SES_ACTIVE;
      InfoData[1].islock=IDLE_TRAF_SES_LOCK;     
		
			
			char Message[64] = "";
			sprintf(Message, IDLE_TRAF_SES_MES,GetCharMoney(GetSessionTraffic(1),1));

                        if (!png_on_idle)
			wsprintf(InfoData[1].ws, perc_t, Message);
                        else
			wsprintf(InfoData[1].ws, perc_s, Message);                          
			
      FillInfoData(&InfoData[1],IDLE_TRAF_SES_X,IDLE_TRAF_SES_Y,IDLE_TRAF_SES_FONT,IDLE_TRAF_SES_COLORS,
                   IDLE_TRAF_SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,
                   IDLE_TRAF_SES_FRINGING_COLORS);
      }
      else
      InfoData[1].enabled=0;
  //////////      ОБЩАЯ СТОИМОСТЬ
      if (IDLE_ALL_TRAF_ENABLE)
      {   
			
			char Message[64] = "";
                        sprintf(Message, IDLE_ALL_TRAF_MES,GetCharMoney(GetAllTraffic(1),1));
                        
                        if (!png_on_idle)
			wsprintf(InfoData[2].ws, perc_t, Message);
                        else
			wsprintf(InfoData[2].ws, perc_s, Message);                          
			
      InfoData[2].enabled=IDLE_ALL_TRAF_ENABLE;
      InfoData[2].isconnect=IDLE_ALL_TRAF_ACTIVE;
      InfoData[2].islock=IDLE_ALL_TRAF_LOCK;      

      FillInfoData(&InfoData[2],IDLE_ALL_TRAF_X,IDLE_ALL_TRAF_Y,IDLE_ALL_TRAF_FONT,IDLE_ALL_TRAF_COLORS,
                   IDLE_ALL_TRAF_FRINGING_ENABLE ? TEXT_OUTLINE : 0,
                   IDLE_ALL_TRAF_FRINGING_COLORS);
      }
      else
      InfoData[2].enabled=0;        
  //////////      ТРАФИК ЗА СЕССИЮ
      if (IDLE_TRAF_ENABLE)
      {
			
			char Message[64] = "";
			sprintf(Message, IDLE_TRAF_MES,GetCharTraffic(GetSessionTraffic(0),0,1));

                        if (!png_on_idle)
			wsprintf(InfoData[3].ws, perc_t, Message);
                        else
			wsprintf(InfoData[3].ws, perc_s, Message);                                                  
			
      InfoData[3].enabled=IDLE_TRAF_ENABLE;
      InfoData[3].isconnect=IDLE_TRAF_ACTIVE;
      InfoData[3].islock=IDLE_TRAF_LOCK;      

      FillInfoData(&InfoData[3],IDLE_TRAF_X,IDLE_TRAF_Y,IDLE_TRAF_FONT,IDLE_TRAF_COLORS,
                   IDLE_TRAF_FRINGING_ENABLE ? TEXT_OUTLINE : 0,
                   IDLE_TRAF_FRINGING_COLORS);      
      }
      else
      InfoData[3].enabled=0;        
  //////////      ОБЩИЙ ТРАФИК
      if (IDLE_TRAF_ALL_ENABLE)
      {
     
      char Message[64] = "";
			sprintf(Message, IDLE_TRAF_ALL_MES,GetCharTraffic(GetAllTraffic(0),1,1));

                        if (!png_on_idle)
			wsprintf(InfoData[4].ws, perc_t, Message);
                        else
			wsprintf(InfoData[4].ws, perc_s, Message);                          
			
      InfoData[4].enabled=IDLE_TRAF_ALL_ENABLE;
      InfoData[4].isconnect=IDLE_TRAF_ALL_ACTIVE;
      InfoData[4].islock=IDLE_TRAF_ALL_LOCK;      
      FillInfoData(&InfoData[4],IDLE_TRAF_ALL_X,IDLE_TRAF_ALL_Y,IDLE_TRAF_ALL_FONT,IDLE_TRAF_ALL_COLORS,
                   IDLE_TRAF_ALL_FRINGING_ENABLE ? TEXT_OUTLINE : 0,
                   IDLE_TRAF_ALL_FRINGING_COLORS);      
      }
      else
      InfoData[4].enabled=0;
        //////////      ВРЕМЯ
      if (IDLE_TIME_ENABLE)
      {
      TDate date; TTime time; 
      
      GetDateTime(&date,&time); 

      InfoData[5].enabled=IDLE_TIME_ENABLE;
      InfoData[5].isconnect=IDLE_TIME_ACTIVE;
      InfoData[5].islock=IDLE_TIME_LOCK;      
       

			char Message[64] = "";
			sprintf(Message, IDLE_TIME_MES,time.hour,time.min,time.sec);
                        if (!png_on_idle)
			wsprintf(InfoData[5].ws, perc_t, Message);
                        else
			wsprintf(InfoData[5].ws, perc_s, Message);                          
			
      FillInfoData(&InfoData[5],IDLE_TIME_X,IDLE_TIME_Y,IDLE_TIME_FONT,IDLE_TIME_COLORS,
                   IDLE_TIME_FRINGING_ENABLE ? TEXT_OUTLINE : 0,
                   IDLE_TIME_FRINGING_COLORS);    
      }
      else
      InfoData[5].enabled=0;
      
 UpDateInfo = 0;
}


////////////////////////////////////////////////////////////////////////////////
//                            Для прорисовки на IDLE                          //
////////////////////////////////////////////////////////////////////////////////

void DRAWIDLE(void *canvasdata,int i)
{
  if (png_on_idle)
  {
    DrawPngFontToIDLE(canvasdata,InfoData[i].ws,InfoData[i].rc.x, InfoData[i].rc.y);
  }
  else
  {
   if (canvas_on_idle)
                  DrawCanvas(canvasdata, InfoData[i].rc.x, InfoData[i].rc.y, InfoData[i].rc.x2, InfoData[i].rc.y2, 1);
                  DrawString(InfoData[i].ws, InfoData[i].rc.x, InfoData[i].rc.y, InfoData[i].rc.x2, InfoData[i].rc.y2,
                             InfoData[i].font,
                             InfoData[i].draw_flag,
                             InfoData[i].pen,
                             InfoData[i].draw_flag ? InfoData[i].fr_cl : GetPaletteAdrByColorIndex(23));  
  }
}

////////////////////////////////////////////////////////////////////////////////
//                                   Key HOOK                                 //
////////////////////////////////////////////////////////////////////////////////

int my_keyhook (int submsg, int msg)
{
  HookEnable = 1;
  int type;
  if (SEE_STAT_ENABLE)
  if ((IsUnlocked()||!SEE_STAT_LOCK) || ((SEE_STAT_ACTIVE && IsGPRSConnected()) || (!SEE_STAT_ACTIVE)))
  {

    //  Обработка клавиши показа статистики
  if (!SEE_MODE_KBD)
    type = KEY_DOWN;
  else
    if (SEE_MODE_KBD == 1)
      type = KEY_UP;
  else
    type = LONG_PRESS;
    if ((submsg==SEE_STAT_BTN) && (msg==type))     // Если клавиша для просмотра статистики нажата...
    { 
      ShowStat(0);
        return(2);
    }
  } 
   
return(0);
}


////////////////////////////////////////////////////////////////////////////////
//                        Уведомление о соединении GPRS                       //
////////////////////////////////////////////////////////////////////////////////


void StopVibra(void)  // Остановка вибры
{
  SetVibration(0);
}

void StartVibra(void) // Вибрация
{
    if (IsGPRSConnected())
    {
      SetVibration(Vibra_ON_Power);
    }
    else
    {
      SetVibration(Vibra_OFF_Power);        
    }
    GBS_StartTimerProc(&VibraTimer, TMR_SECOND/3, StopVibra);
}

void timer_proc(void){ // Функция выполняется по таймеру

  // Проверяем, изменился ли профиль?
    if (profile != GetProfile()+1)  // Если да, то обновляем настройки
    { 
      profile = GetProfile()+1;
      reconfig();
    }
           
if(ENABLE_MAN) // Если включен
{ 
  if (cpu_low)
  {
    if ((IsIDLE()) || (!CLOSE))
      SetCpuClockLow(2);
    else
      SetCpuClockTempHi(2);
  }
  CheckLimit();
  
  if (SEE_STAT_ENABLE)  // Если надо, обновляем данные в окне статистика
    if (!CLOSE)
      DirectRedrawGUI();
        
    if (IsGPRSConnected()) //Если GPRS соединение установленно, то
    {  
      
        //////////      ВРЕМЯ СЕССИИ
  if (session_time_enable)  // Если надо, то отсчитываем время
    {
      session_times++;  // За сессию
      session_all_times++; // Общее
      session_sr_times = session_all_times / session_num; // Вычесляем среднее время сессии
      if (session_times > session_max_times)  // Если надо, то запоминаем наибольшее время
        session_max_times = session_times;
    }
      
      if (VIBRA)// Если уже вибрировали
      {
        GBS_StartTimerProc(&timer,TMR_SECOND,timer_proc); // Стартуем таймер с частотой 1 раза в секунду
        if (NOTIFY_AUTO)
        if (!(session_times % NOTIFY_INT) && (NOTIFY_INT))
        {
          char Message[128]="";
          sprintf(Message,"%s %s",GetTextFormat(GetCharTraffic(GetSessionTraffic(0),0,1),ses_traf)
                                 ,GetTextFormat(GetCharMoney(GetSessionTraffic(1),1),ses_money));
          ShowMessage(Message);

        }
        return;
      }
      else  // Если еще не вибрировали
      { 
        
        session_num++; // Накручиваем счетчик подключений
        oldGPRSTraffic = *GetGPRSTrafficPointer(); // Запоминаем начальный трафик
        if (STAT_ENABLE)  // Записываем начало сессии
        {
          WriteStat(1);
        }
        
        OpenFile(1);  // Открываем файлы
        
          if ((NOTIFY_ON_VIBRA) && (DoNotify()) && (!IsCalling()))
          {
            SUBPROC((void *)StartVibra);  //Вибрируем
          }
          if ((NOTIFY_ON_SOUND) && (DoNotify()) && (!IsCalling()))
          {
            PlaySound(1,0,0,NUMBER_ON_SOUND,0);  // Играем стандартный звук
          }
          if ((NOTIFY_ON_MESSAGE) &&(DoNotify()) && (!IsCalling()))
          {           
            ShowMessage(MESSAGE_ON_TEXT);// Сообщение об активации соединения 
          }
        
        VIBRA = 1; // Присваеваем значение счетчику

      }
    }
  else
    {
      if (session_time_enable)  // Если включен отсчет
      {
          if (session_times)
          {
            if (session_time_show)  // если нужно показать время сессии
            {
              ShowMessage(GetSessionTime(session_times,session_time_message));              
            }
          }
          session_times = 0;  // обновляем время
      }
      
      if (VIBRA)
      {
        
        if (STAT_ENABLE)  // Записывае статистику
        {
          WriteStat(0);
        }

        oldGPRSTraffic = *GetGPRSTrafficPointer(); // Запоминаем начальный трафик
                
        OpenFile(0);
        
          if ((NOTIFY_OFF_VIBRA) && (DoNotify()) && (!IsCalling()))
          {
            SUBPROC((void *)StartVibra);  //Вибрируем
          }
          if ((NOTIFY_OFF_SOUND) && (DoNotify()) && (!IsCalling()))
          {
            PlaySound(1,0,0,NUMBER_OFF_SOUND,0);  // Играем стандартный звук
          }
          if ((NOTIFY_OFF_MESSAGE) &&(DoNotify()) && (!IsCalling()))
          {
            ShowMessage(MESSAGE_OFF_TEXT);// Сообщение об закрытии соединения            
          }
      }
      
      VIBRA = 0;  // Обнуляем счетчик вибрации
      
      oldGPRSTraffic = *GetGPRSTrafficPointer(); // Запоминаем начальный трафик
    }
  }
// Он выполняется однократно поэтому его нужно постоянно запускать 
  GBS_StartTimerProc(&timer,TMR_SECOND,timer_proc); // Стартуем таймер с частотой 1 раз в секунду     (Раз в 1 секунд)         
}


////////////////////////////////////////////////////////////////////////////////
/*                                  CSM MESSAGES                              */
////////////////////////////////////////////////////////////////////////////////

void reconfig()
{
      DoFree(0);
      InitConfig();
      setup_ICONS();
      if (NOTIFY_UPDATED)
      {
        ShowMessage("iGPRS, конфиг обновлен!");
      }    
}

void mesg(GBS_MSG* msg)
{
   
    if (msg->msg==MSG_IPC)
 {
  IPC_REQ *ipc;
  if ((ipc=(IPC_REQ*)msg->data0))
  {
    if (strcmp_nocase(ipc->name_to,my_name)==0)
    {
      switch (msg->submess)
      {
      case IPC_GET_PARAM:
	  if (!dbrun)
          {
            gipc.name_to=my_name;
            gipc.name_from=my_name;
            gipc.data=&execparam;
            GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SEND_PARAM,&gipc);
          }
	 break;
      case IPC_SEND_PARAM:
	  if (dbrun)
          {
             exparam = ipc->data;
             param->Struct = exparam;
             CSMClose();
          }
         
      }
    }
  }
 }
 
    void *icsm;
    int i;
  
if (ENABLE_MAN) // Если GPRM MAN включен
{  

// рисуем на IDLE
  icsm=FindCSMbyID(CSM_root()->idle_id);
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm))) //???? IdleGui ?? ????? ?????
    {
      GUI *igui=GetTopGUI();
      if (igui) //? ?? ??????????
      {
#ifdef ELKA
          void *canvasdata = BuildCanvas();
#else
        void *idata = GetDataOfItemByID(igui, 2);
        if (idata)
        {
          void *canvasdata = ((void **)idata)[DISPLACE_OF_IDLECANVAS / 4];
#endif  
          for (i=0;i<(sizeof(InfoData)/sizeof(TInfo));i++)
          {
            if (InfoData[i].enabled)
            {
              if (  (((InfoData[i].isconnect) && (IsGPRSConnected())) && (((InfoData[i].islock) && (IsUnlocked())) || (!InfoData[i].islock))) 
                 ||
                 ((!InfoData[i].isconnect) && (((InfoData[i].islock) && (IsUnlocked())) || (!InfoData[i].islock) )) )
                {
                  DRAWIDLE(canvasdata,i);
                }
            }
          }
            if (icon_on_idle)
            {
              int pic;
              int X,Y;
              char * icon=NULL;           
              
            X = icon_on_idle_X;
            Y = icon_on_idle_Y;
            
            if (IsGPRSConnected())
            {
              icon=MakeGlobalString(SEE_FOLDER,'\\',"net_on.png");
            } 
            else
            {
              icon=MakeGlobalString(SEE_FOLDER,'\\',"net_off.png");
            }

              pic = (int)icon;
              if (canvas_on_idle)
                DrawCanvas(canvasdata, X, Y, X + GetImgWidth(pic),Y+GetImgHeight(pic), 1);  // Рисуем канву
              DrawImg(X,Y,pic); // Картинку
              if(icon)
              {
                mfree(icon);
                icon=NULL;
              }
            }
#ifdef ELKA
#else          
        }
#endif
      }
    }
  }
  
  if (NOTIFY_SEND)
  {
    static int Traf;
    if (Traf != GetSessionTraffic(0))
    {
      Traf = GetSessionTraffic(0);
      IlluminationOn();
    }
  }
if (!UpDateInfo)
{
  GBS_StartTimerProc(&UpdaTimer,TMR_SECOND/10*UPDATE_TIME,InitInfoData); // Заполняем данные
  UpDateInfo = 1;
}
}
  return;
}

void create()
{
  int i;
  for (i=0;i<(sizeof(InfoData)/sizeof(TInfo));i++)
  InfoData[i].ws=CreateLocalWS(&InfoData[i].wsh,InfoData[i].wsbody,WS_MAXLEN+1);
}

void close()
{   
  DoFree(1);

  //////////////////////////////////////////////////////////////////////////////
  if (IsGPRSConnected())
    WriteStat(0);
  
      if (gprs_end)
      if (IsGPRSEnabled())
        GPRS_OnOff(0,1); 
}

void d2run()
{
   CSMClose();
}


// ==============================================================================================

int main(char *exename, char *fname)
{
  InitConfig();
  
  if (ENABLE_NSD)
  {
    csminit(1,ipc_my_name,successed_config_filename,mesg,create,close,d2run,reconfig);
  }
  
  text = malloc(128);
  
  profile = GetProfile()+1;

  if (ENABLE_MAN)
  {
      if (START_SOUND)
      {
        PlaySound(1,0,0,NUMBER_SOUNG,0);  // Звук при запуске
      }
    timer_proc();  //  Запускаем таймер проверки соединения
    if (gprs_start)
      if (!IsGPRSEnabled())
        GPRS_OnOff(1,0);  
    if (START_MES)
    {
      ShowMessage("iGPRS работает ^_^");
    }
  }
  
  return (0);
}
