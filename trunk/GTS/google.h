
#ifndef __GOOGLE_H__
#define __GOOGLE_H__

#include "socket.h"
#include <string>
using namespace std;

class Google :public Socket
{
public:
    Google();
    ~Google();

    void log(const char *msg, ...);

    virtual void dnr_not_ready();
    virtual void end_socket_connect(int);
    int translate(char *text, const string & thislang, const string & translatelang);
};


#endif


/*int bufsize = strlen(buf);
   char *s = strstr(buf, head);
   if(s < buf || s > buf+bufsize)
   {
      edit->SetTextToItem(4, "Ошибка");
      return;
   }

   s+=strlen(head);

   char *e=s, *translate_buff;

   while(*e++!='"'); e--;

   if(e-s<1)
   {
      edit->SetTextToItem(4, "Ошибка");
      return;
   }

   translate_buff = new char[e-s+1];

   memcpy(translate_buff, s, e-s);
   translate_buff[e-s]=0;
   */
