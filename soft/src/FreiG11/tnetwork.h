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

#ifndef _TNETWORK_H_
#define _TNETWORK_H_

#ifdef _WIN32
 #include <winsock2.h>
 #define MSG_NOSIGNAL 0
 typedef int socklen_t;
#else
 #include <sys/types.h>
 #include <sys/socket.h>
#endif

#ifdef __FreeBSD__
# include <netinet/in.h>
#endif

#define TNETWORK_CLOSE 0
#define TNETWORK_ERR -1
#define TNETWORK_TIMEOUT -2

// timeout in seconds * 10-3 (millisec)

int t_connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen, int * timeout);
int t_connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen, int timeout);

int t_send(int s, const void *msg, size_t len, int flags, int * timeout);
int t_send(int s, const void *msg, size_t len, int flags, int timeout);

int t_recv(int s, void *buf, size_t len, int flags, int * timeout, bool waitall=false);
int t_recv(int s, void *buf, size_t len, int flags, int timeout, bool waitall=false);

#endif
