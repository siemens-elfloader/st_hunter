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

#include "httpproxy.h"
#include "tnetwork.h"
#include "Base64.h"

#ifndef _WIN32
 #include <netdb.h>
 #define CLOSE_SOCK(arg) close(arg); arg=-1;
#else
 #define CLOSE_SOCK(arg) closesocket(arg); arg=-1;
#endif

#include <string.h>
#include <sstream>

#define NET_TIMEOUT 30000

// ----------------=========ooooOOOOOOOOOoooo=========----------------
HttpProxy::HttpProxy(void)
{
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
HttpProxy::HttpProxy(string ahost, int aport, string auid, string apass)
{
 setHost(ahost);
 setPort(aport);
 setUser(auid);
 setPassword(apass);
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
HttpProxy::~HttpProxy()
{
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void HttpProxy::setHost(string ahost)
{
 proxy_host=ahost;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void HttpProxy::setPort(int aport)
{
 proxy_port=aport;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void HttpProxy::setUser(string auid)
{
 proxy_user=auid;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void HttpProxy::setPassword(string apass)
{
 proxy_password=apass;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
int HttpProxy::connectTo(string ahost, int aport)
{
 dst_host = ahost;
 dst_port = aport;
 
 if (!priv_connectToProxy()) return -1;
 if (!priv_doQuery()) return -1;
 
 return sock;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool HttpProxy::priv_connectToProxy(void)
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
string HttpProxy::priv_createProxyQuery(void)
{
 string query;

 ostringstream ss;
 ss << "CONNECT " << dst_host << ":" << dst_port << " HTTP/1.0\r\n";
 ss << "Host: " << dst_host << ":" << dst_port << "\r\n";
 query = ss.str();

 if (proxy_user!="" && proxy_password!="")
  {
  string b64_str;
  Base64 codec;
  codec.encode(proxy_user+":"+proxy_password, b64_str, false);
  query += "Proxy-Authorization: basic " + b64_str + "\r\n";
  }
 
 query += "\r\n";
 return query;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool HttpProxy::priv_checkProxyAnswer(string answ)
{
 if (answ.find("HTTP/1.0 200 ")!=(string::size_type)0) return false;
 return true;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool HttpProxy::priv_doQuery(void)
{
 string quest = priv_createProxyQuery();
 if (t_send(sock, quest.c_str(), quest.length(), MSG_NOSIGNAL, NET_TIMEOUT)!=(int)quest.length())
  {
  CLOSE_SOCK(sock);
  return false;
  }
 
 char tmp_buf[4097]={0};
 if (t_recv(sock, tmp_buf, 4096, MSG_NOSIGNAL, NET_TIMEOUT)<=0)
  {
  CLOSE_SOCK(sock);
  return false;
  }

 if (!priv_checkProxyAnswer(tmp_buf))
  {
  CLOSE_SOCK(sock);
  return false;
  }

 return true;
}
