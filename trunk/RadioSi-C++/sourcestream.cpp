
#include <swilib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <spl/async_print.h>
#include <spl/socket.h>
#include "Url.h"
#include "sourcestream.h"



void SourceStream::emitState(NativeEvent & e)
{
    e.head.id = 0;
    e.head.type = 0;
    e.head.dispatcher = (void (*)(void*))nativeEventDisplatcher;
    sendEvent(getpid(), &e, sizeof e);
}


void SourceStream::nativeEventDisplatcher(NativeEvent *e)
{
    switch(e->t)
    {
    case 1:
        e->s->connectStatus(e->code);
        break;

    case 2:
        e->s->icyMetaUpdated();
        break;

    case 3:
        e->s->headerFetched();
        break;
    }
}


SourceStream::SourceStream(const std::string & path) :
    port(-1),
    icy_metaint(0),
    block(0),
    connection(nullptr),
    socket_fd(-1)

{
    this->path = path;
    this->port = -1;

    fd = new ibstream(static_cast<void*>(this), this->read);
    //file = fopen("4:\\elf\\RadioSI\\log.txt", "w+");
}



SourceStream::~SourceStream()
{
    //fclose(file);
    if(connection)
        delete connection;
}


void SourceStream::AsyncConnection::run()
{
    openDirectSocket(s->path, s->port);
}


void SourceStream::fetchDirectConnection()
{
    if(!connection) {
        connection = new AsyncConnection(this);
    }
}


int SourceStream::openConnection(const std::string & host, unsigned short port)
{
    struct in_addr adr;
    adr.s_addr = gethostbyname(host.c_str());
    if(adr.s_addr == (unsigned int)-1) {
        printf("Can`t resolve ip\n");
        return -1;
    }

    printf("Connecting to %s(%s)\n", host.c_str(), inet_ntoa(adr));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("Can`t create socket\n");
        return -1;
    }

    printf("socket: %d\n", sock);

    struct sockaddr_in sa;

    sa.sin_family = AF_INET;
    sa.sin_addr = adr;
    sa.sin_port = htons(port);

    printf("Connecting...\n");
    if( ::connect(sock, (struct sockaddr*)&sa, sizeof sa) ){
        printf("Can`t connect\n");
        close(sock);
        return -1;
    }
    return sock;
}


int SourceStream::openConnection(unsigned int ip, unsigned short port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("Can`t create socket\n");
        return -1;
    }

    printf("socket: %d\n", sock);

    struct in_addr adr = {.s_addr = ip};
    struct sockaddr_in sa;

    sa.sin_family = AF_INET;
    sa.sin_addr = adr;
    sa.sin_port = htons(port);

    printf("Connecting...\n");
    if( ::connect(sock, (struct sockaddr*)&sa, sizeof sa) ){
        printf("Can`t connect\n");
        close(sock);
        return -1;
    }
    printf("Done.\n");

    return sock;
}


int SourceStream::connect(const std::string & path, int port, int sock)
{
    Url url(path);
    std::string refer = url.host();

    while(1)
    {
        if(url.lastError()) {
            ShowMSG(1, (int)"Invalid url");
            break;
        }

        if(url.host().empty())
            sock = openConnection(url.ip(), port != -1? port : url.port());
        else
            sock = openConnection(url.host(), port != -1? port : url.port());

        if(sock < 0)
            break;


        /* Установим жёсткий учёт чтения/записи данных,
         * при котором read/write по возможности будут возвращать запрошеное количество байт,
         * а не столько, сколько смог прочитать/записать сокет
         */

        int flags = 0;
        if( !ioctl(sock, SOCK_GET_FLAGS, &flags) ) {
            if( !(flags & SOCK_IO_STRONG_SZ_COMP) ) {
                ioctl(sock, SOCK_SET_FLAGS, flags | SOCK_IO_STRONG_SZ_COMP);
            }
        }

        char request[1024*2] = {0}, ansver[1024*1] = {0};
        int len = sprintf(request, "GET %s HTTP/1.0\r\n"
                     "Host: %s\r\n"
                     "User-Agent: Siemens HitFM player\r\n"
                     "Referer: %s\r\n"
                     "Icy-MetaData: 1\r\n"
                     "Connection: keep_alive\r\n\r\n", url.location().c_str(), url.host().c_str(), refer.c_str());



        printf("===========\n%s\nWrite\n", request);
        if( write(sock, request, len) < 1 ) {
            close(sock);
            break;
        }

        printf("Read\n");
        if( (len = ::read(sock, ansver, 512)) < 1 ) {
            ShowMSG(1, (int)"Empty ansver");
            close(sock);
            break;
        }
        printf("Readed\n");

        if(!memcmp(ansver+9, "302 Moved Temporarily", 21) || !memcmp(ansver+9, "301 Moved Permanently", 21))
        {
            printf(ansver);
            close(sock);
            sock = -1;

            printf(" ====> Moved Temporarily\n");
            char *loc = strstr(ansver, "Location: ");
            if(!loc || loc >= ansver+len) break;

            loc += 10;
            char *end = strstr(loc, "\r\n");
            if(!end)
                break;

            memcpy(request, loc, end-loc);
            request[end-loc] = 0;
            url = Url(request);
            continue;
        }

        if(!memcmp(ansver+9, "200 OK", 6))
        {
            printf(" ====> 200 OK\n");

            const char *metaint = strstr(ansver, "icy-metaint:");
            if(metaint)
            {
                metaint += 12;
                sscanf(metaint, "%d", &icy_metaint);
                printf("icy_metaint: %d\n", icy_metaint);
            }

            const char *ansver_end = strstr(ansver, "\r\n\r\n");
            if(ansver_end) {
                ansver_end += 4;

                header.assign(ansver, ansver_end-ansver);
                //headerFetched();
                NativeEvent e;
                e.s = this;
                e.t = 3;     // //headerFetched
                emitState(e);

                if(ansver_end < ansver+len){
                    char *data_end = ansver+len;
                    //printf("need unread: %d\n", ansver_end-data_end);
                    fd->unread(ansver_end, data_end-ansver_end);
                    block += data_end-ansver_end;
                }
            }

            socket_fd = sock;

            /*NativeEvent e;
            e.s = this;
            e.ibs = fd;
            e.code = 0;
            e.t = 1;     // //connectStatus(fd, 0);
            emitState(e);*/
            connectStatus(0);
            return sock;
        }
    }

    NativeEvent e;
    e.s = this;
    e.ibs = 0;
    e.code = -1;
    e.t = 1;     // //connectStatus(fd, 0);
    emitState(e);
    return -1;
}



ssize_t SourceStream::read(void *fd, void *_data, size_t size)
{
    SourceStream *_this = (SourceStream *)fd;
    char *data = (char *)_data;
    int readed = 0;

    if(_this->block + size > (size_t)_this->icy_metaint) {

        int meta_byte_at = _this->icy_metaint - _this->block;
        int need_read = meta_byte_at+1;

        char *tmp = new char[need_read + (256*16) + 2*1024];

        if( _this->read(tmp, need_read) != need_read ) {
            ShowMSG(1, (int)"1. Socket error");
            delete tmp;
            return -1;
        }

        int chunck_size = tmp[meta_byte_at] * 16;
        int chunck_data_size = 1 + chunck_size;
        int data_after_chunck = 0;
        int more_need = chunck_size;

        if(meta_byte_at < int(size))
            more_need += size - meta_byte_at;

        if(more_need > 0) {
            if( _this->read(tmp + need_read, more_need) != more_need ) {
                ShowMSG(1, (int)"2. Socket error");
                delete tmp;
                return -1;
            }
            need_read += more_need;
        }

        data_after_chunck = need_read - (meta_byte_at + chunck_data_size);

        const char *chunck_body = tmp+meta_byte_at+1;
        int chunck_body_size = chunck_size;
        if(!memcmp(chunck_body, "StreamTitle='", 13)) {
            chunck_body += 13;

            char *end = strstr(chunck_body, "';");
            chunck_body_size = end - chunck_body;
        }

        if(chunck_size > 0) {
            _this->icy_chunk.assign(chunck_body, chunck_body_size);
            _this->icyMetaUpdated();
        }

        memcpy(data, tmp, meta_byte_at);
        memcpy(data+meta_byte_at, tmp+meta_byte_at + chunck_data_size, data_after_chunck);

        readed = meta_byte_at + data_after_chunck;
        _this->block = data_after_chunck;


        /*fprintf(_this->file, "Chunk: %d\nByte at: %d\nSize: %d\nAfter: %d\nNeed: %d\nReaded: %d\nc: %d\n", (int)tmp[meta_byte_at], meta_byte_at, chunck_size,
                data_after_chunck, size, need_read - chunck_data_size, readed);

        fwrite(tmp, 1, need_read + 1*1024, _this->file);*/
        delete tmp;

    } else {

        int r = _this->read(data, size);

        if(r > 0) {
            readed += r;
        } else {
            ShowMSG(1, (int)"Socket problem");
            if(!readed)
                return -1;
        }

        _this->block += r;
    }

    return readed;
}


