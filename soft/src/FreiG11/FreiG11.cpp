/***************************************************************************
 *   Copyright (C) 2012 by Barracuda                                       *
 *   barracuda72@bk.ru                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "common.h"
#include "icqkid2.h"
#include "miif.h"
#include "naticq.h"

#define FREIG11_REVISION "3500"
#define FREIG11_STACKSIZE 0x10000
#define FREIG11_TIMEOUT 300

void ig11_mutex_create();
void ig11_mutex_destroy();
void ig11_mutex_lock();
void ig11_mutex_unlock();

HANDLER handle_connection(void *p);
void update_stats();
int recv_packet(int socket, TPKT *buf, int timeout);
int send_packet(int socket, TPKT *buf);
char *utf2win(const char *utf, char *win);
char *win2utf(const char *win, char *utf);

struct sockaddr_in serv;
struct sockaddr_in client;

struct conn_info
{
  unsigned int sock;
  unsigned int unk1;
  struct sockaddr_in addr;
};

int finished = 0;
char *path_to_stat = 0;
int clientsNum = 0;

char *forbid_mod = 0;
char *forbid_2058 = 0;

#ifdef WIN32
// Win-specific
WSADATA WSAData = {0};
typedef int socklen_t;
#else
// Linux-specific
void sigpipe_handler(int a)
{
  printf("Caught SIGPIPE, probably some socket fucked-up\n");
  /*return */signal(SIGPIPE, sigpipe_handler);
}

void sigint_handler(int a)
{
  printf("Caught SIGINT, exiting... Hit Ctrl-C once more to terminate immediately\n");
  finished = 1;
  /*return*/ signal(SIGINT, 0);
}

pthread_attr_t pth_attr;
pthread_t thread;
#endif

int main(int argc, char *argv[])
{
  int mainSock = 0;

  // Если аргументы переданы верно
  if(argc > 1)
  {
    int port = atoi(argv[1]);
    // Если порт допустим
    if((port >= 2) && (port <= 65535))
    {
      printf("FreiG11 NatICQ server (rev %s)\n", FREIG11_REVISION);
#ifdef WIN32
      printf("Win32 build\n");
#else
      printf("Linux %s build\n", (sizeof(long) == 4 ? "32-bit" : "64-bit"));
      printf("Thread stack size: %u\n", FREIG11_STACKSIZE);
#endif

      int seed;
#ifdef WIN32
      seed = GetTickCount();
#else
      seed = time(0);
#endif

      srand(seed);
      
      //sub_807E310(); GetLocaleT() on Win or null on Linux
      /*v49 = &v47;
      sub_807E820((int)&v50, (int *)&unk_80874B8, (int *)&v47);
      v51 = &v48;
      sub_807E820((int)&v50, (int *)&unk_80874BC, (int *)&v48);*/
      
      // Если удалось что-то сделать со строками
      //if(somestr.lenght() && somestr2.lenght())
      if(1)
      {
        forbid_mod = (char *)malloc(/*somestr.lenght() + 1*/ 1024);
        forbid_2058 = (char *)malloc(/*somestr2.lenght() + 1*/ 1024);
        
        strcpy(forbid_mod, "mod_"/*somestr.c_str() + 1*/);
        strcpy(forbid_2058, "2058"/*somestr2.c_str() + 1*/);
      
        // Если передан путь к шаблону статистики
        if ( argc == 3 )
        {
          path_to_stat = (char *)malloc(strlen(argv[2]) + 1);
          strcpy(path_to_stat, argv[2]);
        }
        
#ifdef WIN32
        if ( WSAStartup(MAKEWORD(2, 0), &WSAData) )
        {
          printf("WSAStartup() failed with error: %i\n", WSAGetLastError());
          return -1;
        } else 
#else
        setvbuf((FILE *)stdout, 0, 2, 0);
#endif
        {
          // Создаем сокет
          mainSock = socket(AF_INET, SOCK_STREAM, 0);
          if(mainSock == -1)
          {
            printf("socket() failed with error: %i\n", GET_ERROR());
            return -1;
          } else {
            // Запрашиваем возможность использования занятого сокета
            int optval = 1;
            if ( setsockopt(mainSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, 4) == -1 )
            {
              printf("setsockopt(SO_REUSEADDR) failed with error: %i\n", GET_ERROR());
              return -1;
            } else {
              serv.sin_addr.s_addr = INADDR_ANY;
              serv.sin_family = AF_INET;
              serv.sin_port = htons(port);
              
              if ( bind(mainSock, (const struct sockaddr *)&serv, sizeof(struct sockaddr)) == -1 )
              {
                printf("bind() failed with error: %i\n", GET_ERROR());
                return -1;
              } else {
                if ( listen(mainSock, 5) == -1 )
                {
                  printf("listen() failed with error: %i\n", GET_ERROR());
                  return -1;
                } else {
                  clientsNum = 0;
                  socklen_t namelen = 16;
                  ig11_mutex_create();
#ifndef WIN32
                  signal(SIGINT, sigint_handler);
                  signal(SIGPIPE, sigpipe_handler);
                  if ( !finished ) {
#endif

                  while ( 1 )
                  {
                    socklen_t cl_len = sizeof(struct sockaddr);
                    int newSock = accept(mainSock, (struct sockaddr *)&client, &cl_len);
                    if ( newSock == -1 )
                    {
                      printf("Socket error. Exit! errno=%d\n", GET_ERROR());
                      //exit(1);
                      break;
                    }
                      
                    struct conn_info *addr = (struct conn_info *)malloc(sizeof(struct conn_info));
                    if ( !addr )
                    {
                      printf("Memory allocation error!\n");
                      break;
                    }
                    
                    addr -> unk1 = 0;
                    if ( !getpeername(newSock, (struct sockaddr *)&(addr -> addr), &namelen) )
                    {
                      int port = ntohs(addr->addr.sin_port);
                      char *host = inet_ntoa(addr->addr.sin_addr);
                      printf("Connect from %s:%i\n", host, port);
                    }
                    
                    addr->sock = newSock;
#ifndef WIN32
                    pthread_attr_init(&pth_attr);
                    pthread_attr_setstacksize(&pth_attr, FREIG11_STACKSIZE);
                    pthread_attr_setdetachstate(&pth_attr, 1);
                    
                    if (!pthread_create(&thread, &pth_attr, handle_connection, addr))
#else
                    if(_beginthreadex(0, 0, handle_connection, addr, 0, 0))
#endif
                    {
                      ig11_mutex_lock();
                      ++clientsNum;
                      update_stats();
                      ig11_mutex_unlock();
                    }
                    else
                    {
                      int err;
#ifdef WIN32
                      err = GetLastError();
#else
                      err = 1;
#endif
                      printf("Create thread failed with error: %i\n", err);
                    }
#ifndef WIN32
                    pthread_attr_destroy(&pth_attr);
                    if(finished)
                      break;
                  }
#endif
                  }

                  printf("Finished serving. Closing sockets and cleaning up...\n");
                  shutdown(mainSock, SHUT_RDWR);
                  char buf[16];
                  while ( recv(mainSock, buf, 16, 0) > 0 );
                  
                  ig11_mutex_destroy();
                  
                  if ( path_to_stat )
                    free(path_to_stat);
                    
                  free(forbid_2058);
                  free(forbid_mod);
                }
              }
            }
          }
        }
      } else {
        printf("Internal conversion fuckup\n");
        return 0;
      }
    } else {
      printf("Incorrect port number. Value must be in range 2-65535\n");
      return -1;
    }
    
  } else {
    printf("Usage: %s PORT [/path/to/statistic/output]\n", argv[0]);
    return -1;
  }
}

HANDLER handle_connection(void *arg)
{
  conn_info *info = (conn_info *)arg;
  int socket = info->sock;
  TPKT *p = (TPKT *)malloc(sizeof(PKT) + NAT_DATAMAX);
  int this_is_mod = 0;
  MyIcqInterface miif;
  int i = 0;
  unsigned short j = 0;
  int activ_time = 0;
  miif.nat_socket = socket;
  miif.setLoginHost("64.12.202.112", 5190);
  //miif.setProxy("192.168.1.1", 3128, "freeman", "15_11", ICQKid2::HTTP);
  char *encbuf = (char *)malloc(NAT_DATAMAX);
  char tmp[16];
  ICQKidShortUserInfo inf;
  ICQKid2::ICQKid2Message msg;

  // Сначала ждем или SETCLIENTID, или LOGIN
  while(miif.recv_packet(p, 60) > 0)
  {
    if ( p->pkt.type == NAT_SETCLIENT_ID )
    {
      /* Проверяем, а не мод ли это? */
      p->data[p->pkt.data_len] = 0;
      if ( strstr(p->data, forbid_mod) || !strcmp(p->data, forbid_2058) )
        this_is_mod = 1;
    }
    
    if( p->pkt.type == NAT_REQLOGIN )
    {
      p->data[p->pkt.data_len] = 0;
      sprintf(tmp, "%u", p->pkt.uin);
      miif.setUIN(tmp);
      if(p->pkt.data_len == 0)
      {
        // New-style auth
        miif.new_style_auth = true;
        miif.setPassword("none");
      } else {
        miif.setPassword(p->data);
      }
      printf("ICQ UIN is %u\n", p->pkt.uin);
      miif.setNetworkTimeout(4000);
      /*if(!miif.setXStatus(X_STATUS_NONE, "None", "None"))
      {
	    printf("Failed to set X-Status\n");
	  }*/
      if(miif.doConnect())
      {
       /* login success */
        p->pkt.data_len = 0;
        p->pkt.type = 4;
        miif.send_packet(p);

/*
        В оригинале - такой вот цирк        
        int i = miif.getMyPrivacyStatus();
        int j = 0;
        do
        {
          if ( i == byte_432650[j] )
            break;
          ++j;
        }
        while ( j < 5 );
        p->data[0] = j;
        p->pkt.data_len = 1;
        p->pkt.type = 31;
        send_packet(socket, p);
*/
        /* privacy status */
        int pstatus = miif.getMyPrivacyStatus();
        p->data[0] = pstatus;
        p->pkt.data_len = 1;
        p->pkt.type = NAT_LASTPRIVACY;
        miif.send_packet(p);
        
        /* send groups */
        for(i = 0; i < miif.getGroupsNum(); i++)
        {
          p->pkt.uin = miif.getGroupId(i);
          utf2win(miif.getGroupName(i).c_str(), p->data);
          p->pkt.data_len = strlen(p->data);
          p->pkt.type = NAT_GROUPID;
          miif.send_packet(p);
        }
		
        /* send contacts */
        int gid = -1;
        for(i = 0; i < miif.getContactsNum(); i++)
        {
          if(miif.getContactGid(i) != gid)
          {
            gid = miif.getContactGid(i);
            p->pkt.uin = gid;
            p->pkt.data_len = 0;
            p->pkt.type = NAT_GROUPFOLLOW;
            miif.send_packet(p);
          }
          p->pkt.uin = atoi(miif.getContactUin(i).c_str());
          utf2win(miif.getContactNick(i).c_str(), p->data);
          p->pkt.data_len = strlen(p->data);
          p->pkt.type = NAT_CLENTRY;
          miif.send_packet(p);
        }
        /* end of contact list */
        p->pkt.uin = 0;
        p->pkt.data_len = 0;
        p->pkt.type = NAT_CLENTRY;
        miif.send_packet(p);
        miif.setXStatus(X_STATUS_MEET, "None status", "None state");
        activ_time = TIME() + FREIG11_TIMEOUT;
        break;

      } else {
        sprintf(p->data, "Connect failed at %d%%", miif.getConnectPercentage());
        p->pkt.data_len = strlen(p->data);
        p->pkt.uin = 0;
        p->pkt.type = 6;
        miif.send_packet(p);
#ifdef WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
	  }
	  break;
    }
  }
  
  if(activ_time != 0)
  {
    while(1)
    {
	  int net_ret = miif.pollIncomingEvents(1);
	  if(net_ret != 1 && net_ret != TNETWORK_TIMEOUT)
	  {
	    break;
	  }
	  
	  int ret = miif.recv_packet(p, 0);
	  if(ret < 0)
	  {
	    miif.doDisconnect();
	    break;
	  }
	  
	  if(ret > 0)
	  {
            activ_time = TIME() + FREIG11_TIMEOUT;

            if(this_is_mod)
            {
              // Вставляем грабли владельцам модов
            }

	    // Actual code here
            if( p->pkt.type > 33)
            {
              // Unsupported packet code
              p->pkt.data_len = sprintf(p->data, "Unknown opcode %u", p->pkt.type);
              p->pkt.type = NAT_ERROR;
              miif.send_packet(p);
            } else {
              switch(p->pkt.type)
              {
                case NAT_PING:
		  // Ignore it
                  continue;

                case NAT_SENDMSG:
                  miif.natmsg_count = (miif.natmsg_count + 1) & 0x7FFF;
                  // Пытаемся послать сообщение в юникоде, если не выходит - посылаем как есть
                  p->data[p->pkt.data_len] = 0;
                  sprintf(tmp, "%u", p->pkt.uin);
                  win2utf((const char *)p->data, encbuf);
                  msg = ICQKid2::ICQKid2Message(tmp, encbuf, ICQKid2::ICQKid2Message::UTF8);
                  i = TIME();
                  msg.cookie = miif.natmsg_count;
                  msg.cookie2 = i;
                  if(!miif.sendMessage(msg))
                  {
                    msg = ICQKid2::ICQKid2Message(tmp, p->data, ICQKid2::ICQKid2Message::LOCAL8BIT, 1251);
                    msg.cookie = miif.natmsg_count;
                    msg.cookie2 = i;
                    miif.sendMessage(msg);
                  }
                  continue;

                case NAT_AUTHREQ:
                  p->data[p->pkt.data_len] = 0;
                  win2utf((const char *)p->data, encbuf);
                  sprintf(tmp, "%u", p->pkt.uin);
                  miif.authRequest(tmp, encbuf);
                  continue;

                case NAT_REQINFOSHORT:
                  sprintf(tmp, "%u", p->pkt.uin);
                  miif.getUserInfo((char *)tmp, inf);
                  char *gender;
                  switch(inf.Gender)
                  {
                    case INFO_GENDER_FEMALE:
                      gender = "Female";
                      break;
                    case INFO_GENDER_MALE:
                      gender = "Male";
                      break;
                    default:
                      gender = "Unknown";
                      break;
                  }
                  snprintf(
                               encbuf,
                               NAT_DATAMAX,
                               "Nick: %s\r\nFirstname: %s\r\nLastname: %s\r\nAge: %d\r\nGender: %s\r\nHomecity: %s\r\nNotes: %s\r\n",
                               inf.Nickname.c_str(),
                               inf.Firstname.c_str(),
                               inf.Lastname.c_str(),
                               inf.Age,
                               gender,
                               /*inf.Homecity*/"Unspecified",
                               /*inf.Notes*/"None");
                  utf2win((const char *)encbuf, p->data);
                  p->pkt.data_len =  strlen(p->data);
                  p->pkt.type = NAT_RECVMSG;
                  miif.send_packet(p);
                  continue;

                case NAT_ADDCONTACT:
                  continue;

                case NAT_AUTHGRANT:
                  p->data[p->pkt.data_len] = 0;
                  sprintf(tmp, "%u", p->pkt.uin);
                  win2utf((const char *)p->data, encbuf);
                  miif.authReply(tmp, encbuf, AUTH_ACCEPTED);
                  continue;

                case NAT_MY_STATUS_CH:
		  // TODO: implement properly!
                  if(p->data[0] < 13)
                    miif.setStatus(p->data[0]);
                  continue;

                case NAT_ECHO:
                  p->pkt.type = NAT_ECHORET;
                  miif.send_packet(p);
                  continue;

                case NAT_MY_XSTATUS_CH:
                  if(p->data[0] < 35)
                  {
                    miif.setXState(p->data[0]);
                  }
                  continue;

                case NAT_MSGACK:
                  for(i = 0; i < MAX_PEND; i++)
                  {
                    if(miif.pending_id[i].nat_id == p->pkt.uin)
                    {
                      sprintf(encbuf, "%u", miif.pending_id[i].uin);
                      miif.sendMsgAutoResponse(encbuf, miif.pending_id[i].icq_id, miif.pending_id[i].type);
                      miif.pending_id[i].uin = 0;
                      break;
                    }
                  }
                  continue;

                case NAT_XTEXT_REQ:
                  sprintf(tmp, "%u", p->pkt.uin);
                  // TODO: implement!
                  i = miif.findCLUIN(tmp);
                  if( i > 0)
                  {
					printf("XTEXT REQ for %d, text %s : %s\n", p->pkt.uin, 
					    miif.ContactListUins[i].xStatusTitle.c_str(),
					    miif.ContactListUins[i].xStatusDescription.c_str());
                    utf2win(miif.ContactListUins[i].xStatusTitle.c_str(), p->data+1);
                    j = strlen(p->data+1);
                    utf2win(miif.ContactListUins[i].xStatusDescription.c_str(), p->data+j+1);
                    p->data[0] = j;
                    j = strlen(p->data+1);
                    p->pkt.type = NAT_XTEXT_ACK;
                    p->pkt.data_len = j+1;
                    miif.send_packet(p);
                  }
                  continue;

                case NAT_XTEXT_SET:
                  i = strlen(p->data);
                  win2utf((const char *)p->data, encbuf);
                  win2utf((const char *)&(p->data[i+1]), &(encbuf[i+1]));
                  miif.setXText(encbuf, &(encbuf[i+1]));
                  continue;

                case NAT_ADDCONTACT_WITH_GRP:
                  // TODO: implement
                  continue;

                case NAT_SETPRIVACY:
		  // TODO: implement properly!
                  if(p->data[0] < 5)
                    miif.setMyPrivacyStatus(p->data[0]);
                  continue;

                case NAT_REMOVECONTACT:
                  p->data[p->pkt.data_len] = 0;
                  sprintf(tmp, "%u", p->pkt.uin);
                  miif.removeContact(tmp, &j);
                  if(j)
                  {
                    p->pkt.data_len = sprintf(p->data, "Error! Expected %d, got %d", 0, j);
                    p->pkt.type = NAT_ERROR;
                    miif.send_packet(p);
                  } else {
                    p->pkt.type = NAT_CONTACTREMOVED;
                    p->pkt.data_len = 0;
                    miif.send_packet(p);
                  }
                  continue;

                default:
                  p->pkt.data_len = sprintf(p->data, "Unknown opcode %u", p->pkt.type);
                  p->pkt.type = NAT_ERROR;
                  miif.send_packet(p);
                  continue;
              }
            }
          } else {
            int c_time;
        c_time = TIME();

        if(c_time > activ_time)
        {
		  miif.doDisconnect();
		  break;
		}
	  }
	}
  }
    
  int port = ntohs(info->addr.sin_port);
  char *host = inet_ntoa(info->addr.sin_addr);
  printf("Disconnect from %s:%i\n", host, port);
  int tId = 0;
#ifndef WIN32
  tId = pthread_self();
#endif
  printf("Thread %X (uin %u) received disconnect!\n", tId, p->pkt.uin);
  shutdown(socket, SHUT_RDWR);
  
#ifdef WIN32
  closesocket(socket);
#else
  close(socket);
#endif

  free(p);
  free(encbuf);
  free(arg);

  ig11_mutex_lock();
  --clientsNum;
  update_stats();
  ig11_mutex_unlock();
  return 0;
}

void update_stats()
{
  if ( path_to_stat )
  {
    FILE *f = fopen("stat_template.txt", "rb");
    if ( f )
    {
      fseek(f, 0, SEEK_END);
      int size = ftell(f);
      fseek(f, 0, SEEK_SET);
      char *buf = (char *)malloc(size + 1);
      memset(buf, 0, size + 1);
      fread(buf, size, 1, f);
      fclose(f);
      
      size = size + 17;
      char *buf2 = (char *)malloc(size);
      memset(buf2, 0, size);
      
      sprintf(buf2, buf, clientsNum);
      size = strlen(buf2);
      f = fopen(path_to_stat, "wb");
      fwrite(buf2, size, 1, f);
      fclose(f);
      free(buf);
      free(buf2);
    }
    else
    {
      printf("Cannot open template, exiting...\n");
    }
  }
}

/*
  Encoding-decoding
*/

char *win2utf_b(const char *win, char *utf);
char *utf2win_b(const char *utf, char *win);
char *ucs2win_b(unsigned short *ucs, char *win, int len);

char *utf2win(const char *utf, char *win)
{
  size_t i = strlen(utf) + 1;
  size_t f = i;
#ifdef WIN32
  char *tmp = (char *)malloc(i*2);
  MultiByteToWideChar(CP_UTF8, 0, utf, -1, (wchar_t *)tmp, i);
  WideCharToMultiByte(1251, 0, (wchar_t *)tmp, -1, win, i, 0, 0);
  free(tmp);
#else
  size_t j = i;
  char *p1, *p2;
  p1 = (char *)utf;
  p2 = win;
  // TO, FROM
  iconv_t k = iconv_open("WINDOWS-1251", "UTF-8");
  iconv(k, &p1, &i, &p2, &j);
  win[f-j] = 0;
  iconv_close(k);
#endif
  return win;
}

char *ucs2win(const char *ucs, char *win, int len)
{
  size_t i = len;//strlen(ucs) + 1;
  size_t f = i;
#ifdef WIN32
  ucs2win_b((unsigned short *)ucs, win, len);
#else
  size_t j = i;
  char *p1, *p2;
  p1 = (char *)ucs;
  p2 = win;
  // TO, FROM
  iconv_t k = iconv_open("WINDOWS-1251", "UCS-2BE");
  int pos = iconv(k, &p1, &i, &p2, &j);
  win[f-j] = 0;
  iconv_close(k);
#endif
  return win;
}

char *win2utf(const char *win, char *utf)
{
  size_t i = strlen(win) + 1;
#ifdef WIN32
  char *tmp = (char *)malloc(i*2);
  WideCharToMultiByte(1251, 0, (wchar_t *)win, -1, tmp, i, 0, 0);
  MultiByteToWideChar(CP_UTF8, 0, tmp, -1, (wchar_t *)utf, i);
  free(tmp);
#else
  size_t j = i*2;
  char *p1, *p2;
  p1 = (char *)win;
  p2 = utf;
  // TO, FROM
  iconv_t k = iconv_open("UTF-8", "WINDOWS-1251");
  iconv(k, &p1, &i, &p2, &j);
  iconv_close(k);
#endif
  return utf;
}

#if 0
char *win2utf(const char *win, char *utf) 
{
  static const int table[128] = {
	0x82D0,0x83D0,0x9A80E2,0x93D1,0x9E80E2,0xA680E2,0xA080E2,0xA180E2,
	0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,
	0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,
	0,0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,
	0xA0C2,0x8ED0,0x9ED1,0x88D0,0xA4C2,0x90D2,0xA6C2,0xA7C2,
	0x81D0,0xA9C2,0x84D0,0xABC2,0xACC2,0xADC2,0xAEC2,0x87D0,
	0xB0C2,0xB1C2,0x86D0,0x96D1,0x91D2,0xB5C2,0xB6C2,0xB7C2,
	0x91D1,0x9684E2,0x94D1,0xBBC2,0x98D1,0x85D0,0x95D1,0x97D1,
	0x90D0,0x91D0,0x92D0,0x93D0,0x94D0,0x95D0,0x96D0,0x97D0,
	0x98D0,0x99D0,0x9AD0,0x9BD0,0x9CD0,0x9DD0,0x9ED0,0x9FD0,
	0xA0D0,0xA1D0,0xA2D0,0xA3D0,0xA4D0,0xA5D0,0xA6D0,0xA7D0,
	0xA8D0,0xA9D0,0xAAD0,0xABD0,0xACD0,0xADD0,0xAED0,0xAFD0,
	0xB0D0,0xB1D0,0xB2D0,0xB3D0,0xB4D0,0xB5D0,0xB6D0,0xB7D0,
	0xB8D0,0xB9D0,0xBAD0,0xBBD0,0xBCD0,0xBDD0,0xBED0,0xBFD0,
	0x80D1,0x81D1,0x82D1,0x83D1,0x84D1,0x85D1,0x86D1,0x87D1,
	0x88D1,0x89D1,0x8AD1,0x8BD1,0x8CD1,0x8DD1,0x8ED1,0x8FD1
  }; 
  char *out = utf;

  while (*win)
  {
    if (*win & 0x80) 
	  {
	    int v = table[(int)(0x7f & *win++)];
      if (!v)
        continue;
      *out++ = (char)v;
      *out++ = (char)(v >> 8);
      if (v >>= 16)    
        *out++ = (char)v;
    } else *out++ = *win++;
  }
  *out = 0;
  return utf;
}
#endif

char *win2utf_b(const char *win, char *utf)
{
  char *out = utf;
  while(*win)
  {
    if((*win >= 192) && (*win <= 239))
    {
      *utf++ = 208;
      *utf++ = *win - 48;
    } else if(*win > 239)
    {
      *utf++ = 209;
      *utf++ = *win - 112;
    } else if(*win == 184)
    {
      *utf++ = 209;
      *utf++ = 209;
    } else if(*win == 168)
    {
      *utf++ = 208;
      *utf++ = 129;
    } else {
      *utf++ = *win;
    }
    *win++;
  }
  *utf = 0;
  return out;
}

char *utf2win_b(const char *utf, char *win)
{
  char *out = win;
  int newb;
  int new_c1;
  int new_c2;

  while(*utf)
  {
    if(*utf < 0x80) *win++ = *utf++;

    if((*utf >> 5) == 6)
    {
      new_c1 = (*utf>>2)&5;
      new_c2 = (((*utf++&3)<<6) | (*utf++&0x3F));
      newb = ((new_c1)<<8) + (new_c2);
      if(newb == 1025)
      {
        *win++ = 168;
      } else if(newb == 1105)
      {
        *win++ = 184;
      } else {
        *win++ = newb - 848;
      }
    }
  }
  *win = 0;
  return out;
}

/*
 * Mutex functions
*/
#ifdef WIN32
CRITICAL_SECTION ig11_mutex;
#else
pthread_mutex_t ig11_mutex;
#endif

void ig11_mutex_create()
{
#ifdef WIN32
  InitializeCriticalSection(&ig11_mutex);
#else
  pthread_mutex_init(&ig11_mutex, 0);
#endif
}

void ig11_mutex_destroy()
{
#ifdef WIN32
  DeleteCriticalSection(&ig11_mutex);
#else
  pthread_mutex_destroy(&ig11_mutex);
#endif
}

void ig11_mutex_lock()
{
#ifdef WIN32
  LeaveCriticalSection(&ig11_mutex);
#else
  pthread_mutex_lock(&ig11_mutex);
#endif
}

void ig11_mutex_unlock()
{
#ifdef WIN32
  EnterCriticalSection(&ig11_mutex);
#else
  pthread_mutex_unlock((pthread_mutex_t *)&ig11_mutex);
#endif
}

// Stealed from strings.c
typedef struct
{
  unsigned short u;
  char dos;
  char win;
  char koi8;
} TUNICODE2CHAR;

const TUNICODE2CHAR unicode2char[]=
{
  // CAPITAL Cyrillic letters (base)
  0x410,0x80,0xC0,0xE1, // А
  0x411,0x81,0xC1,0xE2, // Б
  0x412,0x82,0xC2,0xF7, // В
  0x413,0x83,0xC3,0xE7, // Г
  0x414,0x84,0xC4,0xE4, // Д
  0x415,0x85,0xC5,0xE5, // Е
  0x416,0x86,0xC6,0xF6, // Ж
  0x417,0x87,0xC7,0xFA, // З
  0x418,0x88,0xC8,0xE9, // И
  0x419,0x89,0xC9,0xEA, // Й
  0x41A,0x8A,0xCA,0xEB, // К
  0x41B,0x8B,0xCB,0xEC, // Л
  0x41C,0x8C,0xCC,0xED, // М
  0x41D,0x8D,0xCD,0xEE, // Н
  0x41E,0x8E,0xCE,0xEF, // О
  0x41F,0x8F,0xCF,0xF0, // П
  0x420,0x90,0xD0,0xF2, // Р
  0x421,0x91,0xD1,0xF3, // С
  0x422,0x92,0xD2,0xF4, // Т
  0x423,0x93,0xD3,0xF5, // У
  0x424,0x94,0xD4,0xE6, // Ф
  0x425,0x95,0xD5,0xE8, // Х
  0x426,0x96,0xD6,0xE3, // Ц
  0x427,0x97,0xD7,0xFE, // Ч
  0x428,0x98,0xD8,0xFB, // Ш
  0x429,0x99,0xD9,0xFD, // Щ
  0x42A,0x9A,0xDA,0xFF, // Ъ
  0x42B,0x9B,0xDB,0xF9, // Ы
  0x42C,0x9C,0xDC,0xF8, // Ь
  0x42D,0x9D,0xDD,0xFC, // Э
  0x42E,0x9E,0xDE,0xE0, // Ю
  0x42F,0x9F,0xDF,0xF1, // Я
  // CAPITAL Cyrillic letters (additional)
  0x402,'_',0x80,'_', // _ .*.*
  0x403,'_',0x81,'_', // _ .*.*
  0x409,'_',0x8A,'_', // _ .*.*
  0x40A,'_',0x8C,'_', // _ .*.*
  0x40C,'_',0x8D,'_', // _ .*.*
  0x40B,'_',0x8E,'_', // _ .*.*
  0x40F,'_',0x8F,'_', // _ .*.*
  0x40E,0xF6,0xA1,'_', // Ў ...*
  0x408,0x4A,0xA3,0x4A, // _ .*.*
  0x409,0x83,0xA5,0xBD, // _ .*..
  0x401,0xF0,0xA8,0xB3, // Ё
  0x404,0xF2,0xAA,0xB4, // Є
  0x407,0xF4,0xAF,0xB7, // Ї
  0x406,0x49,0xB2,0xB6, // _ .*..
  0x405,0x53,0xBD,0x53, // _ .*.*
  // SMALL Cyrillic letters (base)
  0x430,0xA0,0xE0,0xC1, // а
  0x431,0xA1,0xE1,0xC2, // б
  0x432,0xA2,0xE2,0xD7, // в
  0x433,0xA3,0xE3,0xC7, // г
  0x434,0xA4,0xE4,0xC4, // д
  0x435,0xA5,0xE5,0xC5, // е
  0x436,0xA6,0xE6,0xD6, // ж
  0x437,0xA7,0xE7,0xDA, // з
  0x438,0xA8,0xE8,0xC9, // и
  0x439,0xA9,0xE9,0xCA, // й
  0x43A,0xAA,0xEA,0xCB, // к
  0x43B,0xAB,0xEB,0xCC, // л
  0x43C,0xAC,0xEC,0xCD, // м
  0x43D,0xAD,0xED,0xCE, // н
  0x43E,0xAE,0xEE,0xCF, // о
  0x43F,0xAF,0xEF,0xD0, // п
  0x440,0xE0,0xF0,0xD2, // р
  0x441,0xE1,0xF1,0xD3, // с
  0x442,0xE2,0xF2,0xD4, // т
  0x443,0xE3,0xF3,0xD5, // у
  0x444,0xE4,0xF4,0xC6, // ф
  0x445,0xE5,0xF5,0xC8, // х
  0x446,0xE6,0xF6,0xC3, // ц
  0x447,0xE7,0xF7,0xDE, // ч
  0x448,0xE8,0xF8,0xDB, // ш
  0x449,0xE9,0xF9,0xDD, // щ
  0x44A,0xEA,0xFA,0xDF, // ъ
  0x44B,0xEB,0xFB,0xD9, // ы
  0x44C,0xEC,0xFC,0xD8, // ь
  0x44D,0xED,0xFD,0xDC, // э
  0x44E,0xEE,0xFE,0xC0, // ю
  0x44F,0xEF,0xFF,0xD1, // я
  // SMALL Cyrillic letters (additional)
  0x452,'_',0x90,'_', // _ .*.*
  0x453,'_',0x83,'_', // _ .*.*
  0x459,'_',0x9A,'_', // _ .*.*
  0x45A,'_',0x9C,'_', // _ .*.*
  0x45C,'_',0x9D,'_', // _ .*.*
  0x45B,'_',0x9E,'_', // _ .*.*
  0x45F,'_',0x9F,'_', // _ .*.*
  0x45E,0xF7,0xA2,'_', // ў ...*
  0x458,0x6A,0xBC,0x6A, // _ .*.*
  0x491,0xA3,0xB4,0xAD, // _ .*..
  0x451,0xF1,0xB8,0xA3, // ё
  0x454,0xF3,0xBA,0xA4, // є
  0x457,0xF5,0xBF,0xA7, // ї
  0x456,0x69,0xB3,0xA6, // _ .*..
  0x455,0x73,0xBE,0x73, // _ .*.*
  0x0A0,'_',0xA0,0x20, // space .*..
  0x0A4,'_',0xA4,0xFD, // ¤   .*..
  0x0A6,'_',0xA6,'_', // ¦   .*.*
  0x0B0,0xF8,0xB0,0x9C, // °
  0x0B7,0xFA,0xB7,0x9E, // ·
  // 0x2022,,0x95,0x95, //    .*..
  // 0x2116,0xFC,0xB9,0x23, // №   ...*
  // 0x2219,,0xF9,0x9E, //    .*..
  // 0x221A,0xFB,,0x96, // v   ..*.
  // 0x25A0,0xFE,,0x94, // ¦
  0x0000,0,0,0
};

const unsigned short win2unicode[128]=
{
  0x0402,0x0403,0x201A,0x0453,0x201E,0x2026,0x2020,0x2021,
  0x20AC,0x2030,0x0409,0x2039,0x040A,0x040C,0x040B,0x040F,
  0x0452,0x2018,0x2019,0x201C,0x201D,0x2022,0x2013,0x2014,
  0x0020,0x2122,0x0459,0x203A,0x045A,0x045C,0x045B,0x045F,
  0x00A0,0x040E,0x045E,0x0408,0x00A4,0x0490,0x00A6,0x00A7,
  0x0401,0x00A9,0x0404,0x00AB,0x00AC,0x00AD,0x00AE,0x0407,
  0x00B0,0x00B1,0x0406,0x0456,0x0491,0x00B5,0x00B6,0x00B7,
  0x0451,0x2116,0x0454,0x00BB,0x0458,0x0405,0x0455,0x0457,
  0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,
  0x0418,0x0419,0x041A,0x041B,0x041C,0x041D,0x041E,0x041F,
  0x0420,0x0421,0x0422,0x0423,0x0424,0x0425,0x0426,0x0427,
  0x0428,0x0429,0x042A,0x042B,0x042C,0x042D,0x042E,0x042F,
  0x0430,0x0431,0x0432,0x0433,0x0434,0x0435,0x0436,0x0437,
  0x0438,0x0439,0x043A,0x043B,0x043C,0x043D,0x043E,0x043F,
  0x0440,0x0441,0x0442,0x0443,0x0444,0x0445,0x0446,0x0447,
  0x0448,0x0449,0x044A,0x044B,0x044C,0x044D,0x044E,0x044F
};

unsigned int char8to16(int c)
{
  if (c>=128)
  {
    return(win2unicode[c-128]);
  }
  return(c);
}

char char16to8(unsigned int c)
{
  const TUNICODE2CHAR *p=unicode2char;
  unsigned int i;
  if (c<128) return(c);
  while((i=p->u))
  {
    if (c==i)
    {
      return(p->win);
    }
    p++;
  }
  //c&=0xFF;
  //if (c<32) return(' ');
  //return(c);
  return 'X';
}

char *ucs2win_b(unsigned short *ucs, char *win, int len)
{
  int i;
  for(i = 0; i < (len >> 2); i++)
  {
    win[i] = char16to8(ucs[i*2]);
  }
  win[i] = 0;
  return win;
}
