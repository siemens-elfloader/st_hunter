#ifndef _LIB_H_
  #define _LIB_H_

#include "stdarg.h"
//#include "lang.h"

#define TMR_SECOND (1300/6)
#define LIBCOUNT 6

// ID статусов SieIM
////////////////////////////////////////////////////////////////////////////////
// Если протокол не поддерживает какой-нибудь статус, он должен быть не допущен к использованию, т.е.
// функция применения статуса должна вернуть 0 (т.е. код ошиби)
////////////////////////////////////////////////////////////////////////////////
// обычные статусы
#define OFFLINE 0 // Не в сети
#define INVISIBLE 1 // Скрытый
#define AWAY 2  // Отошел
#define NA 3  // Недоступен
#define OCCUPIED 4  // Небеспокоить
#define DND 5 // Занят
#define DEPRESSION 6  // Деперсия
#define EVIL 7  // Злой
#define HOME 8  // Дома
#define LUNCH 9 // Кушаю
#define WORK 10 // На работе 
#define ONLINE 11 // В сети
#define FFC 12  // Готов болтать
// приватные статусы
#define ALL_CAN_SEE 0  // виден для всех
#define NOBODY_CAN_SEE 1 // виден для всех, кроме списка невидящих
#define VISLIST_CAN_SEE 2  // виден только для "видящих"
#define INVISLIST_CANNOT_SEE 3 // невидем для всех
#define CONTACTLIST_CAN_SEE 4  // виден только для КЛ
// типы протокола
#define TYPE_ICQ 100  // если это плагин icq
#define TYPE_JABBER 101 // если это плагин jabber
#define TYPE_RSS 102  // если это плагин RSS
#define TYPE_MISC 500  // другой протокол
#define TYPE_NONE 501 // пустой плагин (без меню статусов и т.д. только КЛ)

////////////////////////////////////////////////////////////////////////////////
//                                 loadlib.c                                  //
////////////////////////////////////////////////////////////////////////////////

extern int LoadLib; // Загружены ли библиотеки?
extern void loadlib_str();  // Загрузка библиотеки str
extern void loadlib_date();  // Загрузка библиотеки date
extern void loadlib_dbg();  // Загрузка библиотеки dbg
extern void loadlib_type();  // Загрузка библиотеки type
extern void loadlib_gprs();  // Загрузка библиотеки gprs
extern void loadlib_mesg();  // Загрузка библиотеки mesg
extern int closelib();  // Закрытие библиотек
extern int loadlib();  // Загрузка библиотек
extern void set_libs (); //Установка путей для библиотек
////////////////////////////////////////////////////////////////////////////////
//                   переменные одинаковые для всех lib                       //
////////////////////////////////////////////////////////////////////////////////
extern void kill_data(void *p, void (*func_p)(void *));
extern const int LibID;

////////////////////////////////////////////////////////////////////////////////
//                          настройки для всех lib                            //
////////////////////////////////////////////////////////////////////////////////

extern int *Debug;
extern char *Path;
extern int *trafic_size;
extern int *trafic_gold;
extern int *trafic_silver;
extern int *trafic_blur;
extern char *text_valute;
extern int *LIMIT_TRAF_ACTIVE;
extern int *LIMIT_TRAF_KB;
extern int *LIMIT_TRAF_SEC;  
extern int *session_time_enable;
extern int *TypeMes;


////////////////////////////////////////////////////////////////////////////////
// Структура ленга
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	int build;	// версия языкового пакета
	int type;	// тип языка (рус, англ и т.д.)
	int count;	// количество записей
	int size;	// размер ленга, чтоб проверять на изменения и повреждения..
	char cright[64];	// Автор(ы) пакета/перевода
} LANGH;

/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
Структуры библиотеки STR  ID = 0
                                                                              */
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
   void (*WriteStr)(char * msg, char * fn);
   int  (*strcmp_nocase)(const char *s, const char *d);
   char *(*MakeGlobalString)(const char *first, int breakchar, const char *last);
   void (*reverse)(char s[]);
   void (*vsprintf2)(char *text, const char *msg, va_list param);
   int (*InitLgp)(char *lang, void *lng);	// если ленг структура
   int (*InitLgp_2)(char *lang, void **array);	// если ленг массив
   int (*CreateLgp)(char *lang,  int build, int type, int count, int size, char *cright, void *lng);	// если структура
   int (*CreateLgp_2)(char *lang,  int build, int type, int count, int size, char *cright, void **array);	// если массив
   int (*GetLgpInfo)(char *lang, LANGH *head);	// получение информации о ленге
   void (*CloseLib)();
   int LibID;
}STRExecStruct;

typedef struct
{
   STRExecStruct  *Struct;
}STRParamStruct;

////////////////////////////////////////////////////////////////////////////////
/*
Структуры библиотеки DATE ID = 1
                                                                              */
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
   char *(*GetDateTimeNow)(void);
   void (*CloseLib)();
   int LibID;
}DATEExecStruct;

typedef struct
{
   DATEExecStruct  *Struct;
}DATEParamStruct;

////////////////////////////////////////////////////////////////////////////////
/*
Структуры библиотеки DBG  ID = 2
                                                                              */
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   void (*WriteDebug)(char * msg);
   void (*WriteDebugSf)(char * msg, ...);
   int  (*DumpAdr)(const void *source,int cnt);
   void (*CloseLib)();
   int LibID;
}DBGExecStruct;

typedef struct
{
   int Debug;
   char  *Path;
   char *(*GetDateTimeNow)(void);
   void (*WriteStr)(char * msg, char * fn);  
   void (*vsprintf2)(char *text, const char *msg, va_list param);
   DBGExecStruct  *Struct;
}DBGParamStruct;

////////////////////////////////////////////////////////////////////////////////
/*
Структуры библиотеки TYPE ID = 3
                                                                              */
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
   char *(*IntToStr)(int i);
   void (*itoa)(int n, char s[]);
   int (*atoi)(char *attr);
   void (*CloseLib)();
   int LibID;
}TYPEExecStruct;

typedef struct
{
   void (*reverse)(char s[]);
   TYPEExecStruct *Struct;
}TYPEParamStruct;

////////////////////////////////////////////////////////////////////////////////
/*
Структуры библиотеки GPRS ID = 4
                                                                              */
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
   void (*RestartGPRS)(void);
   int (*GetSesInfo)(int Now, int Max, int Sr);
   int (*GetSessionTraffic)(int blur);
   int (*GetAllTraffic)(int blur);
   char *(*GetCharTraffic)(int Traffic, int MB, int Text);
   char *(*GetCharMoney)(int Traffic, int valute);
   char *(*GetSessionTime)(int Secunde, const char *Format);
   int (*CheckLimit)(void);
   void (*CloseLib)();
   int LibID;
}GPRSExecStruct;

typedef struct
{
   int trafic_size;
   int trafic_gold;
   int trafic_silver;
   int trafic_blur;
   char *text_valute;
   int LIMIT_TRAF_ACTIVE;
   int LIMIT_TRAF_KB;
   int LIMIT_TRAF_SEC;  
   int session_time_enable;
   GPRSExecStruct *Struct;
}GPRSParamStruct;

////////////////////////////////////////////////////////////////////////////////
/*
Структуры библиотеки MSG  ID = 5
                                                                              */
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
   void (*ShowMessage)(const char *Message);
   void (*CloseLib)();
   int LibID;
}MESGExecStruct;

typedef struct
{
   int TypeMes;
   MESGExecStruct *Struct;
}MESGParamStruct;

////////////////////////////////////////////////////////////////////////////////

typedef struct
{
   int (*loadlib)();
   int (*closelib)();
   int *LoadLib;
   STRExecStruct     *STREStruct ;
   DATEExecStruct    *DATEEStruct;
   DBGExecStruct     *DBGEStruct ;
   TYPEExecStruct    *TYPEEStruct;
   GPRSExecStruct    *GPRSEStruct;
   MESGExecStruct    *MESGEStruct;
}LMExecStruct;

typedef struct
{
   int Debug; // Включена ли отладка
   char *Path;  // Путь к файлу лога
   int trafic_size; // Размер траффика принятый за единицу
   int trafic_gold; // Цена траффика (рублей)
   int trafic_silver; //  Цена траффика (копеек)
   int trafic_blur; // Округлять траффик до ...
   char *text_valute; // Наименование валюты
   int LIMIT_TRAF_ACTIVE; // Включено ли ограничение по траффику
   int LIMIT_TRAF_KB; // Ограничение траффика в кб
   int LIMIT_TRAF_SEC;   // Ограничение траффика в сек
   int session_time_enable; // Считать ли время сессии
   int TypeMes; // Тип сообщений
   LMExecStruct *Struct;
}LMParamStruct;

////////////////////////////////////////////////////////////////////////////////

typedef struct
{
  int type; // тип плагин
  int status[20]; // список поддерживаемых статусов. статус поддеживается, пишем 1, нет - 0, если дальше парсить нет смысла = -i
  int privat[5];  // список поддерживаемых приватных статусов...
  int countx; // количество дополнительных статусов
  int (*reconnect)(void);  // Переподключение
  int (*connect)(void);  // Подключение
  int (*disconnect)(void); // Отключение
  int (*ping)(void); // Пинг
  int (*set_normal_status)(int Id, char *Msg, int Prioritet);  // Установить нормальный статус
  int (*set_extended_status)(int Id, char *Msg, char *Msg2); // Установить расширенный статус
  int (*set_private_status)(int Id); // Установить приватный статус
  int (*SendMSG)(char *id, char *msg); // Отправить сообщение
  int (*get_normal_status)(char * id); // Получить нормальный статус
  int (*get_extended_status)(char * id); // Получить расширенный статус
  int (*get_short_info)(char * id);  // Получить короткую информацию
  int (*get_extra_info)(char * id);  // Получить дополнительную информацию
  int (*addcontact2list)(char * id, char * nickname, char * group);  // Добавить контакт в группу
} ProtocolEStruct;

typedef struct
{
  char ID[128]; // Идентификатор пользователя
  char PASS[128]; // Пароль пользователя
  char ClientName[30];   // Имя клиента
  unsigned int ClienID; // Идентификатор клиента
  unsigned int AutoGPRS;  // Автоперезапуск жпрс (мб нах!?)
  STRExecStruct     *STREStruct ; //
  DATEExecStruct    *DATEEStruct; //
  DBGExecStruct     *DBGEStruct ; //  <===  СТРУКТУРЫ С ФУНКЦИЯМИ
  TYPEExecStruct    *TYPEEStruct; //  <===  ИЗ ЛИБ
  GPRSExecStruct    *GPRSEStruct; //
  MESGExecStruct    *MESGEStruct; //
  void (*connected)(void); // - тут обработка изменения статуса
  int (*addcontact)(char * id, char * nikename); // - добавление контакта
  int (*movecontact)(char * id, char * group); // 	- перемещение в группу
  int (*removecontact)(char * id); //  - удаление контакта
  int (*changestatus)(char * id,int status); //  - изменение статуса контакта
  int (*recvmsg)(char * id,char * msg); //  - сообщение от контакта
  int (*srvask)(char * id); //  - сообщение дошло до сервера
  int (*clientask)(char * id); //  - сообщение дошло до контакта
  int (*changenormalstatus)(char * id,int status, char * text); //  - изменение обычного статуса контакта
  int (*changesextendedstatus)(char * id,int status, char *Msg, char *Msg2); //  - изменение дополнительного статуса контакта
  int (*privatestatus)(int status); //  - изменение приватного статуса
  ProtocolEStruct *Struct;  // Указатель на структуру
} ProtocolPStruct;



////////////////////////////////////////////////////////////////////////////////

extern STRExecStruct     STREStruct ;
extern STRParamStruct    STRPStruct ;
extern DATEExecStruct    DATEEStruct;
extern DATEParamStruct   DATEPStruct;
extern DBGExecStruct     DBGEStruct ;
extern DBGParamStruct    DBGPStruct ;
extern TYPEExecStruct    TYPEEStruct;
extern TYPEParamStruct   TYPEPStruct;
extern GPRSExecStruct    GPRSEStruct;
extern GPRSParamStruct   GPRSPStruct;
extern MESGExecStruct    MESGEStruct;
extern MESGParamStruct   MESGPStruct;

#endif
