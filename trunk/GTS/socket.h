
#ifndef __SOCKET_H__
#define __SOCKET_H__



class Socket
{
public:
   Socket();
   ~Socket();

private:
   char *header;
   char *url;
   int (*idle_onMessage)(CSM_RAM*,GBS_MSG*);

public:

   // переменные для сокета
   int DNR_ID;
   int port;
   int sock, _sock;
   int connect_state;
   char *r_buffer, *s_buffer;
   int r_len, received, sended, s_len;
   char connect_log[1024];
   bool isReceiving, isSending;
   //bool BreakConnect;

   // функции хедера
   void http_add_header(const char *, ...);
   void clean_header();
   void set_url(const char *);
   void set_port(int p){port = p;}
   const char *get_url();
   char *get_header(){return header;}

   void abort();

   // Socket

   int getsocket_id(){return sock;}
   int getConnectState(){return connect_state;}
   int getReceivedSize(){return received;}
   int getSendedSize(){return sended;}

   // функции коннекта

   int connect();
   int read(char *, int);
   int write(char *, int);
   int close();

   int GBS_ConnectMessages(CSM_RAM *data, GBS_MSG *msg);

   virtual void log(const char *log, ...);

   virtual void dnr_not_ready() =0;
   virtual void end_socket_connect(int) =0;

   static const int REMOTE_CLOSED_SOCKET = 4;
   static const int END_SOCKET_CONNECT = 5;
   static const int DNR_ERROR = 7;
   static const int SOCKET_STATE_ERROR = 2;
   static const int SOCKET_ERROR = 8;
};



#endif
