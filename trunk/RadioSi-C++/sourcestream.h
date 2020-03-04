
#ifndef __SOURCE_STREAM_H__
#define __SOURCE_STREAM_H__

#include <stdio.h>
#include <string.h>
#include <string>
#include <spl/coreevent.h>
#include <spl/process.h>
#include "GSProperty"
#include "Thread.h"
#include "ibstream.h"


class SourceStream;

typedef struct
{
    CoreEvent head;
    SourceStream *s;
    int t;
    int code;
    ibstream *ibs;
}NativeEvent __attribute__((aligned(4)));


class SourceStream
{
    class AsyncConnection : public Thread
    {
    public:
        explicit AsyncConnection(SourceStream *_s) {
            start();
            s = _s;
        }

        virtual ~AsyncConnection() {
            terminate();
        }


    protected:

        int openDirectSocket(const std::string & path, int port = -1, int sock = -1) {
            return s->connect(path, port, sock);
        }

        void run() override;

    private:
        SourceStream *s;
    };

public:

    SourceStream(const std::string & path = "");
    ~SourceStream();

    void fetchDirectConnection();
    int connect(const std::string & path, int port = -1, int sock = -1);

    inline ibstream *ibStream() {
        return fd;
    }

protected:

    void run();
    static void nativeEventDisplatcher(NativeEvent *e);
    void emitState(NativeEvent & e);
    int openConnection(const std::string & host, unsigned short port = 80);
    int openConnection(unsigned int ip, unsigned short port = 80);
    virtual void connectStatus(int code) {}
    virtual void headerFetched() {}
    virtual void icyMetaUpdated() {}

    inline ssize_t read(void *data, size_t size) {
        return ::read(socket_fd, data, size);
    }

    inline void destroyConnection() {
        if(connection)
            delete connection;

        connection = nullptr;
    }

public:
    std::string path;
    std::string header;
    std::string icy_chunk;
    int port;
    int icy_metaint;
    int block;
    //FILE *file;

private:
    AsyncConnection *connection;
    ibstream *fd;
    int socket_fd;


    static ssize_t read(void *fd, void *data, size_t size);
    enum {
        LOCAL_STREAM = 1,
        INET_STREAM
    };
};



#endif

