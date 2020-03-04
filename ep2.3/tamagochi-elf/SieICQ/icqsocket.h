#ifndef _ICQSOCKET_H
#define _ICQSOCKET_H

//#include "include.h"
#include "mem.h"
#include "socket.h"
#include "dns.h"
#include "log.h"

class SocketAbstract;

class SocketDNR: public DNR
{
public:
  virtual void onResolve(DNR_RESULT_MSG result_msg, int value);
  void Bind(SocketAbstract * sock);
  SocketDNR();
private:
  SocketAbstract * Callback;
};

class SocketAbstract: public Socket
{
public:
  virtual void onCreate(); // Создание сокета
  virtual void onDataRead(); // Получение данных
  virtual void onConnected(); // Соединение
  virtual void onClose(); // Закрытие сокета
  virtual void onRemoteClose(); // Удаленное закрытие
  virtual void onError(SOCK_ERROR err); // Обработчик ошибок
  
 
//  virtual void onSocketConnect() = 0; // Вызывается при соединении
//  virtual int onSocketHeaders() = 0; // Обработка заголовка HTTP
//  virtual void onSocketData(char * data, int size) = 0; // Обработка остальных данных (сохранение в файл)
//  virtual void onSocketStopped() = 0; // Остановка
//  virtual void onSocketFinish() = 0; // Окончание закачки
  
  void onResolve(DNR_RESULT_MSG result_msg, int value);
  
  void Start(int ip, short port); // Соединение по IP
  void Start(const char * host, short port); // Соединение по имени хоста
  
  void doConnect();
  
  SocketAbstract();
  ~SocketAbstract();
  
  Buffer * req_buf; // Буфер запроса
  
private:
  int ip; // IP
  short port; // Порт
  Buffer * recv_buf; // Буфер ответа
  SocketDNR * Resolver; // Резолвер
};





#endif 

