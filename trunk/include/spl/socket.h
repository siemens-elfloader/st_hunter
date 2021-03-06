
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <arpa/inet.h>

enum {
    ER_CS_DNR = -1,
    ER_CS_DNR_ATTEMPTS_LIMIT = -2,
    ER_CS_CRT_SOCK = -3,
    ER_CS_CONNECT = -4,
    ER_CS_SEND = -5,
    ER_CS_RECV = -6,
    ER_CS_STATE = -7,
    ER_CS_REMOTE_CLOSED = -8,
    ER_CS_CLOSED = -9
};


typedef enum
{
    SS_NONE = 0,
    SS_SOCKET_CREATED = (1 << 1),
    SS_CONNECTING = (1 << 2),
    SS_CONNECTED = (1 << 3),
    SS_DISCONNECTED = (1 << 4),
    SS_CAN_READ = (1 << 5),
    SS_CAN_WRITE = (1 << 6)
}SocketState;


/* ioctl socket control */
enum {
    SOCK_IO_STRONG_SZ_COMP = (1 << 1)
}SocketFlags;


/* ioctl interface */
enum {
    SOCK_SET_FLAGS = (1 << 1),
    SOCK_GET_FLAGS = (1 << 2),
}SocketIOCtl;



#ifdef __cplusplus
extern "C" {
#endif


int streamBySocket(int sid);

int gethostbyname(const char *name);

#ifdef __USE_SPL__
int socket(int af, int type, int protocol);
int connect(int s, struct sockaddr *name, int namelen);
#endif

int _swrite(int fd, const void *data, size_t size, int flag);
int _sread(int fd, void *data, size_t size, int flag);
int _sclose(int sock);


#ifdef __cplusplus
}
#endif

#endif
