/*
* Библиотека Socket (c++)
* Использование сименс функций
* (с) Danil_e71 2012
*/


#include <swilib.h>
#include <sys/socket.h>
#include "socket.h"
Socket::Socket()
{
    connect_state=0;
    DNR_ID=0;
}

Socket::~Socket()
{

}


void cconnect(Socket *s)
{
 s->Connect();
}

void Socket::connect(char *host,int port)
{
this->host=host;
this->port=port;
SUBPROC((void *)cconnect, this);
}

void Socket::Connect()
{
    if(connect_state==1)
    {
        on_connect();
    }
    if(connect_state==0)
    {
    SOCK_ADDR sa;
    int err=0;
    int ***p_res=NULL;
    sock=-1;
    unsigned int ip=0;

    sock = _socket(1,1,0);
    if (sock!=-1)
    {
        sa.family=AF_UNIX;
        sa.port=htons(port);
        ip=str2ip(host);
        if (ip!=0xFFFFFFFF)
        {
          sa.ip=ip;
          goto CONECT_HOST;
        }
        err=async_gethostbyname(host,&p_res,&DNR_ID);
       if (err){return;}

        if (p_res)
        {
          if (p_res[3])sa.ip=p_res[3][0][0];
        }
 CONECT_HOST:
        if(_connect(sock,&sa,sizeof(sa))!=-1)
        {

            connect_state=1;
        }
        else
        {
            closesocket(sock);
            sock=-1;
        }
    }
    }
}

void Socket::Send()
{
_send(sock,buf,strlen(buf),0);
}


void Socket::Recv()
{
    recv_buffer[0]=0;
    int j=_recv(sock,&recv_buffer,2024,0);
    on_recv(recv_buffer,j);
}

void ssend(Socket *s)
{
  s->Send();
}
void srecv(Socket *s)
{
  s->Recv();
}

void Socket::send(char *buf)
{

this->buf=buf;
connect_state=2;
SUBPROC((void *)ssend, this);
}

void Socket::end_socket()
{
    if (sock>=0)
    {
        closesocket(sock);
        connect_state=0;
        on_closed();
    }
}
void send_socket(Socket *s)
{
  s->end_socket();
}


void Socket::onmessage(int sock,int msg)
{
     if(sock==this->sock)
            {

            switch(msg)
            {
            case ENIP_SOCK_CONNECTED:if(connect_state==1){on_connect();}break;
            case ENIP_SOCK_DATA_READ:if(connect_state==2)SUBPROC((void *)srecv, this);break;
            case ENIP_SOCK_REMOTE_CLOSED:SUBPROC((void *)send_socket, this);break;
            }
}

}


