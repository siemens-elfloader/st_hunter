/***************************************************************************
 *   Copyright (C) 2007 by Alexander S. Salieff                            *
 *   salieff@mail.ru                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
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

#include <string.h>
#include "socks4proxy.h"
#include "tnetwork.h"

#ifndef _WIN32
 #include <netdb.h>
 #define CLOSE_SOCK(arg) close(arg); arg=-1;
#else
 #define CLOSE_SOCK(arg) closesocket(arg); arg=-1;
#endif

#define NET_TIMEOUT 30000

#include <vector>

// ----------------=========ooooOOOOOOOOOoooo=========----------------
SOCKS4Proxy::SOCKS4Proxy(void)
{
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
SOCKS4Proxy::SOCKS4Proxy(string ahost, int aport, string auid)
{
 setHost(ahost);
 setPort(aport);
 setUser(auid);
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
SOCKS4Proxy::~SOCKS4Proxy()
{
}
  
// ----------------=========ooooOOOOOOOOOoooo=========----------------
void SOCKS4Proxy::setHost(string ahost)
{
 proxy_host=ahost;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void SOCKS4Proxy::setPort(int aport)
{
 proxy_port=aport;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void SOCKS4Proxy::setUser(string auid)
{
 proxy_user=auid;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
int SOCKS4Proxy::connectTo(string ahost, int aport)
{
 dst_host = ahost;
 dst_port = aport;
 
 if (!priv_connectToProxy()) return -1;
 if (!priv_sendConnect()) { CLOSE_SOCK(sock); return -1; }
 if (!priv_waitConnectOK()) { CLOSE_SOCK(sock); return -1; }
 
 return sock;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS4Proxy::priv_connectToProxy(void)
{
 struct hostent * he;
 sockaddr_in proxy_addr;
 
 if ((he=gethostbyname(proxy_host.c_str()))==NULL) return false;
 if (he->h_addr==NULL) return false;

 memset(&proxy_addr, 0, sizeof(proxy_addr));
 memcpy(&(proxy_addr.sin_addr.s_addr), he->h_addr, sizeof(proxy_addr.sin_addr.s_addr));
 proxy_addr.sin_port=htons(proxy_port);
 proxy_addr.sin_family=PF_INET;

 if ((sock=socket(PF_INET, SOCK_STREAM, 0))==-1) return false;
 if (t_connect(sock, (const struct sockaddr *)&proxy_addr, (socklen_t)sizeof(proxy_addr), NET_TIMEOUT)!=0)
  {
  CLOSE_SOCK(sock);
  return false;
  }
 return true;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS4Proxy::priv_sendConnect(void)
{
 struct hostent * he;
 if ((he=gethostbyname(dst_host.c_str()))==NULL) return false;
 if (he->h_addr==NULL) return false;
 
 unsigned short int tmp_port = htons(dst_port);

 vector<unsigned char> con_arr(8);
 con_arr[0]=0x04;
 con_arr[1]=0x01;
 memcpy(&con_arr[2], &tmp_port, 2);
 memcpy(&con_arr[4], he->h_addr, 4);
 con_arr.insert(con_arr.end(), proxy_user.begin(), proxy_user.end());
 con_arr.push_back(0);
 
 if (t_send(sock, &con_arr[0], con_arr.size(), MSG_NOSIGNAL, NET_TIMEOUT)!=con_arr.size())
  return false;  

 return true;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS4Proxy::priv_waitConnectOK(void)
{
 unsigned char con_arr[8];
 if (t_recv(sock, con_arr, 8, MSG_NOSIGNAL, NET_TIMEOUT, true)!=8)
  return false;
 
 if (con_arr[0]!=0 || con_arr[1]!=90) return false;
 
 return true;
}
