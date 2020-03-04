/*
* Библиотека Socket (c++)
* Использование сименс функций
* (с) Danil_e71 2012
*/


#ifndef SOCKET_H
#define SOCKET_H


class Socket
{
private:
int connect_state;
char *buf;
char *host;
int port;
int DNR_ID;
int sock;
char recv_buffer[4048];
public:

//standart methods
Socket();
~Socket();
//dont use this methods
void end_socket();
void Send();
void Recv();
void Connect();
//main methods
void connect(char *host,int port);
void send(char *buf);
//other methods
void onmessage(int sock,int msg);//put this method into onmessage
//virtual methods
void (*on_recv)(char *data,int size);
void (*on_connect)();
void (*on_closed)();
};

#endif
