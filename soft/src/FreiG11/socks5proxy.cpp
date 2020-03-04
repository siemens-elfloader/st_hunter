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
#include "socks5proxy.h"
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
SOCKS5Proxy::SOCKS5Proxy(void)
{
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
SOCKS5Proxy::SOCKS5Proxy(string ahost, int aport, string auid, string apass)
{
 setHost(ahost);
 setPort(aport);
 setUser(auid);
 setPassword(apass);
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
SOCKS5Proxy::~SOCKS5Proxy()
{
}
  
// ----------------=========ooooOOOOOOOOOoooo=========----------------
void SOCKS5Proxy::setHost(string ahost)
{
 proxy_host=ahost;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void SOCKS5Proxy::setPort(int aport)
{
 proxy_port=aport;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void SOCKS5Proxy::setUser(string auid)
{
 proxy_user=auid;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void SOCKS5Proxy::setPassword(string apass)
{
 proxy_password=apass;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
int SOCKS5Proxy::connectTo(string ahost, int aport)
{
 dst_host = ahost;
 dst_port = aport;
 
 if (!priv_connectToProxy()) return -1;
 if (!priv_sendHello()) { CLOSE_SOCK(sock); return -1; }
 int auth_type=priv_waitHello();
 if (auth_type!=0x00 && auth_type!=0x02) { CLOSE_SOCK(sock); return -1; }
 if (auth_type==0x02)
  {
  if (!priv_sendAuth()) { CLOSE_SOCK(sock); return -1; }
  if (!priv_waitAuthOK()) { CLOSE_SOCK(sock); return -1; }
  }
 if (!priv_sendConnect()) { CLOSE_SOCK(sock); return -1; }
 if (!priv_waitConnectOK()) { CLOSE_SOCK(sock); return -1; }
 
 return sock;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS5Proxy::priv_connectToProxy(void)
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
bool SOCKS5Proxy::priv_sendHello(void)
{
 unsigned char hello_arr[] = { 0x05, 0x02, 0x00, 0x02 }; // ver.5, methods_count=2, no_auth, rfc1929 auth
 if (t_send(sock, hello_arr, sizeof(hello_arr), MSG_NOSIGNAL, NET_TIMEOUT)!=sizeof(hello_arr))
  return false;  

 return true;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
int SOCKS5Proxy::priv_waitHello(void)
{
 unsigned char hello_arr[2];
 if (t_recv(sock, hello_arr, 2, MSG_NOSIGNAL, NET_TIMEOUT, true)!=2)
  return -1;

 return hello_arr[1];
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS5Proxy::priv_sendAuth(void)
{
 vector<char> auth_arr(2);
 auth_arr[0]=1;
 auth_arr[1]=proxy_user.length();
 auth_arr.insert(auth_arr.end(), proxy_user.begin(), proxy_user.end());
 char pass_len = proxy_password.length();
 auth_arr.push_back(pass_len);
 auth_arr.insert(auth_arr.end(), proxy_password.begin(), proxy_password.end());
 if (t_send(sock, &auth_arr[0], auth_arr.size(), MSG_NOSIGNAL, NET_TIMEOUT)!=auth_arr.size())
  return false;  

 return true;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS5Proxy::priv_waitAuthOK(void)
{
 unsigned char auth_arr[2];
 if (t_recv(sock, auth_arr, 2, MSG_NOSIGNAL, NET_TIMEOUT, true)!=2)
  return -1;

 return (auth_arr[1]==0);
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS5Proxy::priv_sendConnect(void)
{
 struct hostent * he;
 if ((he=gethostbyname(dst_host.c_str()))==NULL) return false;
 if (he->h_addr==NULL) return false;
 
 unsigned char con_arr[10] = { 0x05, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff }; // ver.5, cmd=connect, reserv, addr_type=ipv4, 4 byte adr, 2 byte port
 memcpy(con_arr+4, he->h_addr, 4);
 unsigned short int tmp_port = htons(dst_port);
 memcpy(con_arr+8, &tmp_port, 2);
 
 if (t_send(sock, con_arr, sizeof(con_arr), MSG_NOSIGNAL, NET_TIMEOUT)!=sizeof(con_arr))
  return false;  

 return true;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool SOCKS5Proxy::priv_waitConnectOK(void)
{
 unsigned char con_arr[5];
 if (t_recv(sock, con_arr, 5, MSG_NOSIGNAL, NET_TIMEOUT, true)!=5)
  return false;
 
 size_t tail_len;
 switch (con_arr[3])
  {
  case 1 : // ipv4
       tail_len=3+2;
       break;
  case 3 : // symbol name
       tail_len=con_arr[4]+2;
       break;
  case 4 : // ipv6
       tail_len=15+2;
       break;
  default :
       return false;
       break;
  }
  
 unsigned char tmp_arr[256];
 if (t_recv(sock, tmp_arr, tail_len, MSG_NOSIGNAL, NET_TIMEOUT, true)!=tail_len)
  return false;
 
 if (con_arr[0]!=5 || con_arr[1]!=0) return false;
 
 return true;
}
