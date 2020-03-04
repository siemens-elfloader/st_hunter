#ifndef _LIB2_H_
  #define _LIB2_H_

#include "stdarg.h"
#include "lib.h"

////////////////////////////////////////////////////////////////////////////////
//                            функции из всех lib                             //
//           ПЕРЕДВЫЗОВОМ ПРОВЕРЯТЬ ЗАГРУЖЕНЫ ЛИ БИБЛИОТЕКИ!!                 //
//                             типа if (LoadLib)                              //
////////////////////////////////////////////////////////////////////////////////
//  API Lib's
extern   void (*WriteStr)(char * msg, char * fn);
extern   int  (*strcmp_nocase2)(const char *s, const char *d);
extern   char *(*MakeGlobalString2)(const char *first, int breakchar, const char *last);
extern   void (*reverse)(char s[]);
extern   int (*InitLgp)(char *lang, void *lng);	// если ленг структура
extern   int (*InitLgp_2)(char *lang, void **array);	// если ленг массив
extern   int (*CreateLgp)(char *lang,  int build, int type, int count, int size, char *cright, void *lng);	// если структура
extern   int (*CreateLgp_2)(char *lang,  int build, int type, int count, int size, char *cright, void **array);	// если массив
extern   int (*GetLgpInfo)(char *lang, LANGH *head);	// получение информации о ленге */
extern   char *(*GetDateTimeNow)(void);
extern   void (*WriteDebug)(char * msg);
extern   void (*WriteDebugSf)(char * msg, ...);
extern   int  (*DumpAdr)(const void *source,int cnt);
extern   void (*WriteStr)(char * msg, char * fn); 
extern   void (*vsprintf2)(char *text, const char *msg, va_list param);
extern   char *(*IntToStr)(int i);
extern   void (*itoa)(int n, char s[]);
extern   int (*atoi)(char *attr);
extern   void (*RestartGPRS)(void);
extern   int (*GetSesInfo)(int Now, int Max, int Sr);
extern   int (*GetSessionTraffic)(int blur);
extern   int (*GetAllTraffic)(int blur);
extern   char *(*GetCharTraffic)(int Traffic, int MB, int Text);
extern   char *(*GetCharMoney)(int Traffic, int valute);
extern   char *(*GetSessionTime)(int Secunde, const char *Format);
extern   int (*CheckLimit)(void);
extern   void (*ShowMessage)(const char *Message);
// API Plugin Protocol
extern  int API_reconnect(void);  // Переподключение  +
extern  int API_connect(void);  // Подключение  +
extern  int API_disconnect(void); // Отключение +
extern  int API_ping(void); // Пинг +
extern  int API_set_normal_status(int Id, char *Msg, int Prioritet);  // Установить нормальный статус +
extern  int API_set_extended_status(int Id, char *Msg, char *Msg2); // Установить расширенный статус  +
extern  int API_set_private_status(int Id); // Установить приватный статус  +
extern  int API_SendMSG(char *id, char *msg); // Отправить сообщение  +
extern  int API_get_normal_status(char * id); // Получить нормальный статус
extern  int API_get_extended_status(char * id); // Получить расширенный статус
extern  int API_get_short_info(char * id);  // Получить короткую информацию +
extern  int API_get_extra_info(char * id);  // Получить дополнительную информацию -
extern  int API_addcontact2list(char * id, char * nickname, char * group);  // Добавить контакт в группу  +
//  API GUI
extern  void (*connected)(void); // - тут обработка изменения статуса
extern  int (*addcontact)(char * id, char * nikename); // - добавление контакта
extern  int (*movecontact)(char * id, char * group); // 	- перемещение в группу
extern  int (*removecontact)(char * id); //  - удаление контакта
extern  int (*changestatus)(char * id,int status); //  - изменение статуса контакта
extern  int (*recvmsg)(char * id,char * msg); //  - сообщение от контакта
extern  int (*srvask)(char * id); //  - сообщение дошло до сервера
extern  int (*clientask)(char * id); //  - сообщение дошло до контакта
extern  int (*changenormalstatus)(char * id,int status, char * text); //  - изменение обычного статуса контакта
extern  int (*changesextendedstatus)(char * id,int status, char *Msg, char *Msg2); //  - изменение дополнительного статуса контакта
extern  int (*privatestatus)(int status); //  - изменение приватного статуса

#endif
