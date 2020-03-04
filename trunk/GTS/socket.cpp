

#include <swilib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdarg.h>
#include "socket.h"


/* нада добвить стейты, видимо из за них приходили данные совсем с другой области */

//#define DEBUG 1
int sub_close(Socket *sock);
int sub_connect(Socket *sock);


void Socket :: log(const char *log, ...)
{
    va_list va;
    va_start(va, log);

    vsprintf(connect_log, log, va);
    va_end(va);
}

Socket :: Socket()
{
    header = new char[1024];
    zeromem(header, 1024);
    connect_state  = 0;
    _sock = sock = -1;
    port = 80;
    url = 0;
    DNR_ID = 0;
    s_buffer = 0;
    s_len = 0;
    r_buffer = 0;
    r_len = 0;
    sended = 0;
    received = 0;
    connect_log[0] =0;
}

Socket :: ~Socket()
{
    SUBPROC((void*)sub_close, this);
    delete header;
}


void Socket :: http_add_header(const char *head, ...)
{
    va_list args;
    va_start(args,head);
    vsprintf(this->header+strlen(header), head, args);
    va_end(args);
    strcat(this->header, "\r\n\0");
}

void Socket :: clean_header()
{
    zeromem(header, 1024);
}


void Socket :: set_url(const char *curl)
{
    int urllen = strlen(curl);
    if(this->url)
        delete this->url;

    this->url = new char[urllen+2];
    memcpy(this->url, curl, urllen);
    this->url[urllen]=0;
}


const char * Socket :: get_url()
{
    return (const char*)url;
}


void Socket::abort()
{
    close();
}


// ”станавливаем соединение
int Socket :: connect()
{
    isReceiving = false;
    isSending = false;
    zeromem(connect_log, 5);

        DNR_ID = 0;
        _sock = sock = -1;
        sended = 0;
        received = 0;
        connect_state = 0;

    SUBPROC((void*)sub_connect, this);
    return 0;
}


int sub_connect(Socket *sock)
{
    if(!sock) return -2;
    if(!sock->get_url()) return -1;

    sock->log("connect...");
    int ***p_res=NULL;
    SOCK_ADDR sa;
    int err;

    err = async_gethostbyname(sock->get_url(),&p_res,&sock->DNR_ID);


    if (err)
    {
        if ((err==0xC9)||(err==0xD6))
        {
            if (sock->DNR_ID)
            {
                sock->log("dnr не готов");
                sock->dnr_not_ready();
                return 1; //∆дем готовности DNR
            }
        }
        else
        {
            sock->end_socket_connect(sock->DNR_ERROR);
            return 2;
        }
    }

    if (p_res)
    {
        if (p_res[3])
        {
            sa.ip=p_res[3][0][0];
            sock->_sock = sock->sock = socket(1,1,0);

            if (sock->sock != -1)
            {
                sa.family=1;
                sa.port=htons(sock->port);
                if (connect(sock->sock,&sa,sizeof(sa))!=-1)
                {
                    sock->connect_state = 1;
                    sock->log("connect ok");
                }
                else
                {
                    int s=sock->sock;
                    sock->sock=-1;
                    closesocket(s);
                    sock->log("fail connect");
                }
            }
            else
            {
                //ShowMSG(1,(int)"Ќе осилили создани€ сокета");
                sock->end_socket_connect(sock->SOCKET_ERROR);
                return 3;
            }
        }
    }


    if( sock->connect_state == 1 )
    {
        sock->log("socket was successfully created");
        return 0;
    }
    return -1;
}


int sub_read(Socket *sock)
{
    if( !sock->connect_state || !sock->r_buffer || !sock->r_len) return 0;
    sock->log("reading...");

    if( (sock->r_len - sock->received) < 1 )
    {
        sock->close();
        return 0;
    }

    size_t rc = recv(sock->sock, sock->r_buffer + sock->received, sock->r_len - sock->received, 0);

    if(rc < 1)
    {
        sock->close();
    }

    sock->received += rc;

    sock->r_buffer[sock->received] = 0;
    return 0;
}


int Socket :: read(char *buf, int size)
{
    if(buf && size)
    {
        this->r_buffer = buf;
        this->r_len = size;
        this->received = 0;
        isReceiving = true;
        return 0;
    }

    SUBPROC((void*)sub_read, this);
    return 1;
}


int sub_write(Socket *sock)
{
    if( !sock->connect_state || !sock->s_buffer || !sock->s_len) return 0;

    sock->log("writing...");

    sock->sended += send(sock->sock, sock->s_buffer + sock->sended, sock->s_len-sock->sended, 0);

    if(sock->sended == sock->s_len) sock->connect_state = 2;

    if( (sock->s_len-sock->sended) < 1) return 0;

    return sock->sended;
}


int Socket :: write(char *buf, int size)
{
    if(buf && size)
    {
        s_buffer = buf;
        s_len = size;
        sended=0;
        isSending = true;
        return 0;
    }

    SUBPROC((void*)sub_write, this);
    return 1;
}


int Socket :: close()
{
    sock = 0;
    SUBPROC((void *)sub_close, this);
    //sub_close (this);
    return 0;
}


int sub_close(Socket *sock)
{
    if (sock->_sock != -1)
    {
        shutdown(sock->_sock, 2);
        closesocket(sock->_sock);
        sock->_sock = -1;
        //sock->log("sub_close");
    }
    return 0;
}


int Socket :: GBS_ConnectMessages(CSM_RAM *data, GBS_MSG *msg)
{
    if (msg->msg==MSG_HELPER_TRANSLATOR)
    {
        if ( ((int)msg->data1) == sock)
        {
            switch((int)msg->data0)
            {
            case ENIP_SOCK_CONNECTED:
                if (connect_state == 1)
                {
                    write(0, 0);
                } else
                {
                    log("Illegal message ENIP_SOCK_CONNECTED!");
                    end_socket_connect(SOCKET_STATE_ERROR);
                }
                break;
            case ENIP_SOCK_DATA_READ:
                if (connect_state == 2)
                {
                    read(0, 0);
                } else
                {
                    log("Illegal message ENIP_SOCK_DATA_READ!");
                    end_socket_connect(SOCKET_STATE_ERROR);
                }
                break;
            case ENIP_SOCK_REMOTE_CLOSED:
                end_socket_connect(REMOTE_CLOSED_SOCKET);
                close();
                break;

            case ENIP_SOCK_CLOSED:
                //if( connect_state != 3 )
                //{
                //    log("Illegal message ENIP_SOCK_CLOSED!");
                //    end_socket_connect(SOCKET_STATE_ERROR);
                //} else
                end_socket_connect(END_SOCKET_CONNECT);

                connect_state = 0;
                close();
                log("done");
                break;
            }
        }
    }
    return 0;
}

