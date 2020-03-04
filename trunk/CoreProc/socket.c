
#include <swilib.h>
#include <spl/task.h>
#include <spl/mutex.h>
#include <spl/process.h>
#include <spl/waitcondition.h>
#include <spl/corearray.h>
#include <fcntl.h>
#include <spl/socket.h>
#include <spl/io.h>
#include <messhook.h>


static void onMessageHook(CSM_RAM *ram, GBS_MSG *msg);
static int message_h = -1;
CoreArray sockets_mess_handless;

extern int (*p_createMessageHook)(message_handler_t h);
extern int (*p_deleteMessageHook)(int id);


struct HandleQueue
{
    void (*handler)(int sid, CSM_RAM *, GBS_MSG *);
    int sid;
};



void replace_idle_message_func()
{
    corearray_init(&sockets_mess_handless, 0);
    message_h = p_createMessageHook(onMessageHook);
}


void restore_idle_message_func()
{
    corearray_release(&sockets_mess_handless);
    p_deleteMessageHook(message_h);
}


void onMessageHook(CSM_RAM *ram, GBS_MSG *msg)
{
    if (msg->msg != MSG_HELPER_TRANSLATOR )
        return;

    CoreArray *arr = &sockets_mess_handless;
    struct HandleQueue *h = 0;
    int i = 0;

    corearray_foreach(struct HandleQueue *, h, arr, i)
    {
        if(streamBySocket(h->sid) == (int)msg->data1)
            h->handler(h->sid, ram, msg);
    }
}


static int findEmpty()
{
    CoreArray *arr = &sockets_mess_handless;
    void *h = 0;
    int i = 0;
    corearray_foreach(struct HandleQueue *, h, arr, i)
    {
        if(!h)
            return i;
    }

    return corearray_push_back(arr, 0);
}


static int registerSocketMessageHandler(int sid, void (*handler)(int sid, CSM_RAM *, GBS_MSG *))
{
    struct HandleQueue *h = malloc(sizeof *h);
    h->handler = handler;
    h->sid = sid;

    int id = findEmpty();
    corearray_store_cell(&sockets_mess_handless, id, h);
    return id;
}


static int removeSocketMessageHandler(int hid)
{
    struct HandleQueue *h = corearray_cell(&sockets_mess_handless, hid);
    if(h)
        free(h);
    return corearray_store_cell(&sockets_mess_handless, hid, 0);
}




#define MAX_SOCKETS 256

typedef struct
{
    short id;
    int fd;
    int message_handler;
    int flags;
    SocketState state;

    int connectedWid, readWid;
    NU_SEMAPHORE read_sema, write_sema;
} CoreSocket;


static CoreMutex mutex;
static CoreSocket sockets[MAX_SOCKETS];


void socketsInit()
{
    replace_idle_message_func();
    createMutex(&mutex);
    for(int i=0; i<MAX_SOCKETS; ++i)
        sockets[i].id = -1;
}



void socketsFini()
{
    destroyMutex(&mutex);
    restore_idle_message_func();
}


static short emptySocket()
{
    for(int i=0; i<MAX_SOCKETS; ++i)
        if(sockets[i].id == -1)
            return i;

    return -1;
}


static CoreSocket *getSocketData(int sid)
{
    if(sid < 0 || sid > MAX_SOCKETS-1)
        return 0;

    return &sockets[sid];
}



static CoreSocket *newSocket()
{
    lockMutex(&mutex);

    short _sid;
    CoreSocket *sock = getSocketData((_sid = emptySocket()));
    if(sock) {
        sock->id = _sid;
    }

    unlockMutex(&mutex);
    return sock;
}


static int freeSocket(int _sid)
{
    CoreSocket *sock = getSocketData(_sid);
    if(!sock || sock->id < 0)
        return -1;

    sock->id = -1;
    return 0;
}


/******************************************************************************/
/**************************** socket implementation ***************************/
/******************************************************************************/
typedef struct {
    NU_SEMAPHORE wait;
    const char *host;
    int id;
    int ip;
    int err;
    int attempts;
    GBSTMR dnr_wait;

} dnrImpl;

static void gethostbyname_impl(dnrImpl *dnr);
static void socketMessageHandler(int sid, CSM_RAM *ram, GBS_MSG *msg);

int streamBySocket(int sid)
{
    CoreSocket *sock = getSocketData(sid);
    if(!sock || sock->id < 0)
        return -1;

    return sock->fd;
}


int gethostbyname(const char *host)
{
    dnrImpl dnr = {.host = host, .err = -1, .attempts = 4};
    int pid = getpid();

    if(isProcessKilling(pid) == 1)
        return -1;

    enterProcessCriticalCode(pid);

    NU_Create_Semaphore(&dnr.wait, (CHAR*)"host", 0, NU_PRIORITY);
    SUBPROC(gethostbyname_impl, &dnr);
    NU_Obtain_Semaphore(&dnr.wait, NU_SUSPEND);
    NU_Delete_Semaphore(&dnr.wait);

    leaveProcessCriticalCode(pid);
    return dnr.ip;
}



static void gethostbyname_impl(dnrImpl *dnr)
{
    void wait_dnr_impl(GBSTMR *tmr)
    {
        SUBPROC(gethostbyname_impl, (void*)tmr->param4);
    }

    int ***p_res = NULL;
    int err;

    err = async_gethostbyname(dnr->host, &p_res, &dnr->id);

    if (err)
    {
        if ((err==0xC9)||(err==0xD6))
        {
            if (dnr->id)
            {
                if(--dnr->attempts > 4) {
                    dnr->err = -1;
                    dnr->ip = -1;
                    NU_Release_Semaphore(&dnr->wait);
                    return;
                }

                dnr->dnr_wait.param4 = (int)dnr;
                GBS_StartTimerProc(&dnr->dnr_wait, 2*216, wait_dnr_impl);
                return;
            }
        }
        else
        {
            dnr->err = -2;
            dnr->ip = -1;
            NU_Release_Semaphore(&dnr->wait);
            return;
        }
    }

    if(p_res && p_res[3])
    {
        dnr->err = 0;
        dnr->ip = p_res[3][0][0];
        NU_Release_Semaphore(&dnr->wait);
        return;
    }

    dnr->err = -3;
    dnr->ip = -1;
    NU_Release_Semaphore(&dnr->wait);
}




static ssize_t __read(int fd, void *_data, size_t size)
{
    idStream *s = getStreamData(getpid(), fd);
    if(!s)
        return -1;

    if(size < 1 || !_data)
        return -1;

    CoreSocket *_sock = getSocketData(s->fd);
    char *data = (char *)_data;
    size_t total = 0;

    if(_sock->flags & SOCK_IO_STRONG_SZ_COMP) {
        while(total < (int64_t)size) {
            int64_t r = _sread(s->fd, data + total, size - total, 0);
            if(r < 0) {
                if(total)
                    break;
                else
                    return -1;
            }

            total += r;
        }
    } else {
        total = _sread(s->fd, _data, size, 0);
    }

    return total;
}


static ssize_t __write(int fd, const void *_data, size_t size)
{
    idStream *s = getStreamData(getpid(), fd);
    if(!s) {
        return -1;
    }

    if(size < 1 || !_data)
        return -1;

    CoreSocket *_sock = getSocketData(s->fd);
    const char *data = (char *)_data;
    size_t total = 0;

    if(_sock->flags & SOCK_IO_STRONG_SZ_COMP) {
        while(total < (int64_t)size) {
            int64_t w = _swrite(s->fd, data + total, size - total, 0);
            if(w < 0) {
                if(total)
                    break;
                else
                    return -1;
            }

            total += w;
        }
    } else {
        total = _swrite(s->fd, _data, size, 0);
    }

    return total;
    //return _swrite(s->fd, data, size, 0);
}


static int __close(int fd)
{
    idStream *s = getStreamData(getpid(), fd);
    if(!s) {
        return -1;
    }

    return _sclose(s->fd);
}


static int __flush(int fd)
{
    idStream *s = getStreamData(getpid(), fd);
    if(!s) {
        return -1;
    }

    // errno = ENOENT
    return -1;
}


static off_t __lseek(int fd, off_t offset, int whence)
{
    UNUSED(offset);
    UNUSED(whence);

    idStream *s = getStreamData(getpid(), fd);
    if(!s) {
        return -1;
    }

    // errno = ENOENT
    return -1;
}


static int __ioctl(int fd, int command, va_list va)
{
    idStream *s = getStreamData(getpid(), fd);
    if(!s)
        return -1;

    CoreSocket *_sock = getSocketData(s->fd);

    switch(command)
    {
        case SOCK_SET_FLAGS:
        {
            int f = va_arg(va, int);
            _sock->flags = f;
            break;
        }

        case SOCK_GET_FLAGS:
        {
            int *f = va_arg(va, int *);
            *f = _sock->flags;
            break;
        }

        default:
            return -1;
    }

    return 0;
}


int socket(int af, int type, int protocol)
{
    typedef struct
    {
        NU_SEMAPHORE wait;
        int af;
        int type;
        int protocol;
        int ret;
    } SocketImpl;

    SocketImpl impl = {.af = (af == AF_INET? AF_UNIX : af), .type = type, .protocol = protocol, .ret = 0};
    int pid = getpid();
    if(isProcessKilling(pid) == 1)
        return -1;

    void socket_i(SocketImpl *i)
    {
        i->ret = _socket(i->af, i->type, i->protocol);
        NU_Release_Semaphore(&i->wait);
    }

    enterProcessCriticalCode(pid);

    NU_Create_Semaphore(&impl.wait, (CHAR*)"sock", 0, NU_PRIORITY);
    SUBPROC(socket_i, &impl);
    NU_Obtain_Semaphore(&impl.wait, NU_SUSPEND);
    NU_Delete_Semaphore(&impl.wait);

    leaveProcessCriticalCode(pid);

    if(impl.ret > -1) {
        enterProcessCriticalCode(pid);
        int iofd = open_fd();
        idStream *s = getStreamData(getpid(), iofd);
        if(!s) {
            goto error;
        }

        CoreSocket *sock = newSocket();
        if( NU_Create_Semaphore(&sock->read_sema, "rd", 0, NU_FIFO) != NU_SUCCESS ) {
            goto error;
        }

        if( NU_Create_Semaphore(&sock->write_sema, "wr", 0, NU_FIFO) != NU_SUCCESS ) {
            NU_Delete_Semaphore(&sock->read_sema);
            goto error;
        }

        sock->fd = impl.ret;
        sock->message_handler = registerSocketMessageHandler(sock->id, socketMessageHandler);
        sock->state = SS_SOCKET_CREATED;
        sock->connectedWid = createWaitCond("conn_wait");
        sock->readWid = createWaitCond("read_wait");
        sock->flags = SOCK_IO_STRONG_SZ_COMP;

        s->fd = sock->id;
        s->mode = O_RDWR;
        s->type = SOCKET_STREAM;
        s->read = __read;
        s->write = __write;
        s->close = __close;
        s->flush = __flush;
        s->lseek = __lseek;
        s->ioctl = __ioctl;
        leaveProcessCriticalCode(pid);
        return iofd;

        error:

        _sclose(impl.ret);
        leaveProcessCriticalCode(pid);
        return -1;
    }

    return impl.ret;
}



int connect(int sock, struct sockaddr *name, int namelen)
{
    typedef struct
    {
        NU_SEMAPHORE wait;
        int s;
        struct sockaddr *name;
        int namelen;
        int err;
    } ConnectImpl;


    idStream *s = getStreamData(getpid(), sock);
    if(!s || s->id < 0)
        return -1;

    CoreSocket *_sock = getSocketData(s->fd);
    if(!_sock || _sock->fd < 0)
        return -1;

    if( !(_sock->state & SS_SOCKET_CREATED) )
        return ER_CS_STATE;

    int pid = getpid();
    if(isProcessKilling(pid) == 1)
        return -1;

    _sock->state = SS_CONNECTING;
    // костыль для AF_INET
    if(name->sa_family == 2)
        name->sa_family = AF_UNIX;

    ConnectImpl impl = {.s = _sock->fd, .name = name, .namelen = namelen, .err = 0};

    void connect_i(ConnectImpl *i)
    {
        SOCK_ADDR *sa = (SOCK_ADDR *)i->name;
        sa->unk1 = 0;
        sa->unk2 = 0;

        i->err = _connect(i->s, sa, i->namelen);
        NU_Release_Semaphore(&i->wait);
    }


    enterProcessCriticalCode(pid);

    NU_Create_Semaphore(&impl.wait, (CHAR*)"sock", 0, NU_PRIORITY);
    SUBPROC(connect_i, &impl);
    NU_Obtain_Semaphore(&impl.wait, NU_SUSPEND);
    NU_Delete_Semaphore(&impl.wait);

    if(impl.err)
        _sock->state = SS_NONE;

    leaveProcessCriticalCode(pid);
    return impl.err;
}



int _sclose(int fd)
{
    CoreSocket *sock = getSocketData(fd);
    if(!sock || sock->id < 0)
        return -1;

    typedef struct
    {
        NU_SEMAPHORE wait;
        int sock;
        int err;
    } SocketImpl;

    SocketImpl impl = {.sock = sock->fd, .err = 0};
    int pid = getpid();

    void socket_i(SocketImpl *i)
    {
        i->err = closesocket(i->sock);
        NU_Release_Semaphore(&i->wait);
    }

    //printf("socket close\n");

    enterProcessCriticalCode(pid);
    //printf("entered critical code\n");

    NU_Create_Semaphore(&impl.wait, (CHAR*)"sock", 0, NU_PRIORITY);
    SUBPROC(socket_i, &impl);
    NU_Obtain_Semaphore(&impl.wait, NU_SUSPEND);
    NU_Delete_Semaphore(&impl.wait);

    destroyWaitCond(sock->connectedWid);
    destroyWaitCond(sock->readWid);

    NU_Delete_Semaphore(&sock->read_sema);
    NU_Delete_Semaphore(&sock->write_sema);
    removeSocketMessageHandler(sock->id);
    freeSocket(sock->id);

    leaveProcessCriticalCode(pid);
    //printf("socket closed\n");
    return impl.err;
}




int _swrite(int fd, const void *data, size_t size, int flag)
{
    CoreSocket *sock = getSocketData(fd);
    if(!sock || sock->id < 0)
        return -1;

    if( !(sock->state & SS_CAN_READ) && sock->state & SS_CONNECTING )
        waitCondition(sock->connectedWid);

    if( !(sock->state & SS_CAN_WRITE) )
        return -2;

    typedef struct
    {
        NU_SEMAPHORE *wait;
        int sock;
        const void *data;
        size_t size;
        int flag;
        int ret;
    }WriteData;

    WriteData d = {.sock = sock->fd, .data = data, .size = size, .flag = flag, .ret = 0, .wait = &sock->write_sema};
    int pid = getpid();
    if(isProcessKilling(pid) == 1)
        return -1;

    void __write(WriteData *d)
    {
        d->ret = send(d->sock, d->data, d->size, d->flag);
        NU_Release_Semaphore(d->wait);
    }

    // лочим убийство процесса на этом этапе,
    // так как это может привести к неожиданному исходу
    //printf("socket write ...\n");
    enterProcessCriticalCode(pid);

    //printf("socket write entered critical code.\n");
    SUBPROC(__write, &d);

    NU_Obtain_Semaphore(d.wait, NU_SUSPEND);

    leaveProcessCriticalCode(pid);
    //printf("socket write complete\n");
    return d.ret;
}


int _sread(int fd, void *data, size_t size, int flag)
{
    CoreSocket *sock = getSocketData(fd);
    if(!sock || sock->id < 0)
        return -1;

    //printf("_sread(%d, %X, %d, %d)\n", fd, data, size, flag);
    int pid = getpid();

    //printf("getpid: %d\n", pid);

    if(isProcessKilling(pid) == 1)
        return -1;

    if( !(sock->state & SS_CONNECTED) ) {
        return -1;
    }

    //printf("Wait cond(%d) ...\n", sock->readWid);
    waitCondition(sock->readWid);
    //printf("Wait cond done\n");

    if( !(sock->state & SS_CAN_READ) ) {
        return -2;
    }


    typedef struct
    {
        NU_SEMAPHORE *wait;
        int sock;
        void *data;
        size_t size;
        int flag;
        int ret;
    }WriteData;

    WriteData dat = {.sock = sock->fd, .data = data, .size = size, .flag = flag, .ret = 0, .wait = &sock->read_sema};


    //printf("socket read ...\n");
    void __read(WriteData *d) {
        d->ret = recv(d->sock, d->data, d->size, d->flag);
        NU_Release_Semaphore(d->wait);
    }

    enterProcessCriticalCode(pid);
    //printf("socket read entered critical code.\n");

    SUBPROC(__read, &dat);
    NU_Obtain_Semaphore(dat.wait, NU_SUSPEND);

    if(dat.ret < 1) {
        //char d[55];
        //sprintf(d, "sock ret: %d\n", dat.ret);
        //ShowMSG(1, (int)d);
    }
    //if(dat.ret > 0)
    //resetWaitConditions(sock->readWid);

    leaveProcessCriticalCode(pid);
    //printf("socket read complete\n");
    return dat.ret;
}



static void socketMessageHandler(int sid, CSM_RAM *ram, GBS_MSG *msg)
{
    UNUSED(ram);

    CoreSocket *sock = getSocketData(sid);
    if(!sock || sock->id < 0) {
        printf("%s: unhandled socket %d\n", __FUNCTION__, sid);
        return;
    }

    switch((int)msg->data0)
    {
        case ENIP_SOCK_CONNECTED:
            //printf("ENIP_SOCK_CONNECTED %d\n", sid);
            sock->state = SS_CONNECTED | SS_CAN_WRITE;
            wakeOneWaitCond(sock->connectedWid);
            break;


        case ENIP_SOCK_DATA_READ:
            //printf("ENIP_SOCK_DATA_READ %d\n", sid);
            if( !(sock->state & SS_CAN_READ) && sock->state & SS_CONNECTED )
                sock->state |= SS_CAN_READ;

            wakeOneWaitCond(sock->readWid);
            break;


        case ENIP_SOCK_REMOTE_CLOSED:
            //printf("ENIP_SOCK_REMOTE_CLOSED %d\n", sid);
            sock->state = SS_DISCONNECTED;
            wakeAllWaitConds(sock->readWid);
            wakeAllWaitConds(sock->connectedWid);
            break;


        case ENIP_SOCK_CLOSED:
            //printf("ENIP_SOCK_CLOSED %d\n", sid);
            sock->state = SS_DISCONNECTED;
            wakeAllWaitConds(sock->readWid);
            wakeAllWaitConds(sock->connectedWid);
            break;
    }
}

























