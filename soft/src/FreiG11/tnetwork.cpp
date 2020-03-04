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

#include "tnetwork.h"

#ifndef _WIN32
 #include <sys/time.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <errno.h>
#endif

#include <sys/timeb.h>

// timeout in seconds * 10-3 (millisec)

int t_connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen, int timeout)
{
 int my_timeout = timeout;
 return t_connect(sockfd, serv_addr, addrlen, &my_timeout);
}

int t_connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen, int * timeout)
{
 if (*timeout<=0) return TNETWORK_ERR;
 timeb t1;
 ftime(&t1);
// Make socket non-blocked
#ifndef _WIN32
 int cur_flg=-1;
 if ((cur_flg=fcntl(sockfd, F_GETFL))==-1) return TNETWORK_ERR;
 if (fcntl(sockfd, F_SETFL, cur_flg | O_NONBLOCK)==-1) return TNETWORK_ERR;
#else
 unsigned long int ctl_cmd=1;
 if (ioctlsocket(sockfd, FIONBIO, &ctl_cmd)!=0) return TNETWORK_ERR;
 ctl_cmd=0;
#endif
// Try to connect
 if (connect(sockfd, serv_addr, addrlen)!=0)
  {
#ifndef _WIN32
  if (errno!=EINPROGRESS && errno!=EALREADY)
   {
   fcntl(sockfd, F_SETFL, cur_flg);
   return TNETWORK_ERR;
   }
#else
  if (WSAGetLastError()!=WSAEINPROGRESS && WSAGetLastError()!=WSAEALREADY && \
      WSAGetLastError()!=WSAEWOULDBLOCK && WSAGetLastError()!=WSAEINVAL)
   {
   ioctlsocket(sockfd, FIONBIO, &ctl_cmd);
   return TNETWORK_ERR;
   }
#endif
  }
// Make poll in timeout currency
 fd_set wfds;
 struct timeval tv;
 tv.tv_sec=*timeout/1000;
 tv.tv_usec=(*timeout%1000)*1000;
 int sel_ret;
 do
  {
  FD_ZERO(&wfds);
  FD_SET(sockfd, &wfds);
  sel_ret = select(sockfd+1, NULL, &wfds, NULL, &tv);
#ifndef _WIN32
  } while (sel_ret<0 && errno==EINTR);
#else
  } while (sel_ret<0 && WSAGetLastError()==WSAEINTR);
#endif
 if (sel_ret<=0)
  {
#ifndef _WIN32
  fcntl(sockfd, F_SETFL, cur_flg);
#else
  ioctlsocket(sockfd, FIONBIO, &ctl_cmd);
#endif
  if (sel_ret<0) return TNETWORK_ERR;
  if (sel_ret==0) return TNETWORK_TIMEOUT;
  }
 if (!FD_ISSET(sockfd, &wfds))
  {
#ifndef _WIN32
  fcntl(sockfd, F_SETFL, cur_flg);
#else
  ioctlsocket(sockfd, FIONBIO, &ctl_cmd);
#endif
  return TNETWORK_ERR;
  }
// Get return code
 int gso_ret = 0;
 socklen_t gso_ret_len = (socklen_t)sizeof(gso_ret);
 
 if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)(&gso_ret), &gso_ret_len)!=0)
  {
#ifndef _WIN32
  fcntl(sockfd, F_SETFL, cur_flg);
#else
  ioctlsocket(sockfd, FIONBIO, &ctl_cmd);
#endif
  return TNETWORK_ERR;
  }
 if (gso_ret!=0)
  {
  errno=gso_ret;
#ifndef _WIN32
  fcntl(sockfd, F_SETFL, cur_flg);
#else
  ioctlsocket(sockfd, FIONBIO, &ctl_cmd);
  WSASetLastError(gso_ret);
#endif
  return TNETWORK_ERR;
  }
// Make socket blocked
#ifndef _WIN32
 if (fcntl(sockfd, F_SETFL, cur_flg)!=0) return TNETWORK_ERR;
#else
 if (ioctlsocket(sockfd, FIONBIO, &ctl_cmd)!=0) return TNETWORK_ERR;
#endif
 timeb t2;
 ftime(&t2);
 *timeout=((t2.time-t1.time)*1000+t2.millitm-t1.millitm);
 return 0;
}

int t_send(int s, const void *msg, size_t len, int flags, int timeout)
{
 int my_timeout = timeout;
 return t_send(s, msg, len, flags, &my_timeout);
}

int t_send(int s, const void *msg, size_t len, int flags, int * timeout)
{
 if (*timeout<=0) return TNETWORK_ERR;
 int curr_len=len;
 int curr_timeout=*timeout;
 while (curr_len>0 && curr_timeout>0)
  {
  timeb t1;
  ftime(&t1);
// Make poll in timeout currency
  fd_set wfds;
  struct timeval tv;
  tv.tv_sec=curr_timeout/1000;
  tv.tv_usec=(curr_timeout%1000)*1000;
  FD_ZERO(&wfds);
  FD_SET(s, &wfds);
  int sel_ret = select(s+1, NULL, &wfds, NULL, &tv);
  if (sel_ret<0)
   {
   #ifndef _WIN32
   if (errno==EINTR) // Interrupted by signal
   #else
   if (WSAGetLastError()==WSAEINTR) // Interrupted by signal
   #endif
    {
    timeb t2;
    ftime(&t2);
    curr_timeout-=((t2.time-t1.time)*1000+t2.millitm-t1.millitm);
    continue;
    }
   else return TNETWORK_ERR;
   }
  if (sel_ret==0) 
   return TNETWORK_TIMEOUT;
  if (!FD_ISSET(s, &wfds))
   return TNETWORK_ERR;
  int l=send(s, (char *)msg+(len-curr_len), curr_len, flags);
  if (l<0)
   {
   #ifndef _WIN32
   if (errno==EINTR) // Interrupted by signal
   #else
   if (WSAGetLastError()==WSAEINTR) // Interrupted by signal
   #endif
    {
    timeb t2;
    ftime(&t2);
    curr_timeout-=((t2.time-t1.time)*1000+t2.millitm-t1.millitm);
    continue;
    }
   else return TNETWORK_ERR;
   }
  curr_len-=l;
  timeb t2;
  ftime(&t2);
  curr_timeout-=((t2.time-t1.time)*1000+t2.millitm-t1.millitm);
  }
 *timeout-=curr_timeout;
 return (len-curr_len);
}

int t_recv(int s, void *buf, size_t len, int flags, int timeout, bool waitall)
{
 int my_timeout = timeout;
 return t_recv(s, buf, len, flags, &my_timeout, waitall);
}

int t_recv(int s, void *buf, size_t len, int flags, int * timeout, bool waitall)
{
 if (*timeout<=0) return TNETWORK_ERR;
 int curr_len=len;
 int curr_timeout=*timeout;
 while (curr_len>0 && curr_timeout>0)
  {
  timeb t1;
  ftime(&t1);
// Make poll in timeout currency
  fd_set rfds;
  struct timeval tv;
  tv.tv_sec=curr_timeout/1000;
  tv.tv_usec=(curr_timeout%1000)*1000;
  FD_ZERO(&rfds);
  FD_SET(s, &rfds);
  int sel_ret = select(s+1, &rfds, NULL, NULL, &tv);
  if (sel_ret<0)
   {
   #ifndef _WIN32
   if (errno==EINTR) // Interrupted by signal
   #else
   if (WSAGetLastError()==WSAEINTR) // Interrupted by signal
   #endif
    {
    timeb t2;
    ftime(&t2);
    curr_timeout-=((t2.time-t1.time)*1000+t2.millitm-t1.millitm);
    continue;
    }
   else return TNETWORK_ERR;
   }
  if (sel_ret==0) 
   return TNETWORK_TIMEOUT;
  if (!FD_ISSET(s, &rfds))
   return TNETWORK_ERR;
  int l=recv(s, (char *)buf+(len-curr_len), curr_len, flags);
  if (l<0)
   {
   #ifndef _WIN32
   if (errno==EINTR) // Interrupted by signal
   #else
   if (WSAGetLastError()==WSAEINTR) // Interrupted by signal
   #endif
    {
    timeb t2;
    ftime(&t2);
    curr_timeout-=((t2.time-t1.time)*1000+t2.millitm-t1.millitm);
    continue;
    }
   else return TNETWORK_ERR;
   }
  curr_len-=l;
  timeb t2;
  ftime(&t2);
  curr_timeout-=((t2.time-t1.time)*1000+t2.millitm-t1.millitm);
  if (!waitall) break;
  }
 *timeout-=curr_timeout;
 return (len-curr_len);
}
