/***************************************************************************
 *   Copyright (C) 2007 by Andrey "Ice" Sploshnov                          *
 *   ice.nightcrawler@gmail.com                                            *
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

#ifndef ICQKID_COMMON_H
#define ICQKID_COMMON_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
 typedef unsigned char uint8_t;
 typedef unsigned short int uint16_t;
 typedef unsigned long int uint32_t;
#define snprintf _snprintf_c
//#elif defined STDC_HEADERS
#else
# include <stdint.h>
//#else
# include <sys/types.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <signal.h>


#ifdef WIN32
#include <winsock2.h>
#include <process.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <pthread.h>
#include <iconv.h>
#endif

#ifdef WIN32
#define GET_ERROR() WSAGetLastError()
#define HANDLER unsigned int __stdcall
#define IOCTL(x, y, z) ioctlsocket(x, y, (u_long *)z)
#define TIME() _time64(0)
#define SHUT_RDWR 2
#define snprintf _snprintf_c
#else
#define GET_ERROR() errno
#define HANDLER void *
#define IOCTL(x, y, z) ioctl(x, y, z)
#define TIME() time(0)
#endif

#endif /* ICQKID_COMMON_H */
