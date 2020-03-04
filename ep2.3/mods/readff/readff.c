/***************************************************************************
 *   Copyright (C) 2008 by NForce                                          *
 *   nforce25@gmail.com                                                    *
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
 ***************************************************************************/

#include "..\inc\swilib.h"                      

#ifdef NEWSGOLD
unsigned char *addr = (unsigned char *)0xA003E410;
#else
unsigned char *addr = (unsigned char *)0xA000065C;
#endif

/*
*  conversion table/таблица цифр
*  0x0A = 0
*  0x0E = 1
*  0x1A = 2
*  0x1E = 3
*  0x2A = 4
*  0x2E = 5
*  0x3A = 6
*  0x3E = 7
*  0x4A = 8
*  0x4E = 9 
*/

#define length 2               //сколько имей'ев в нашем блеклисте

int black[length][15]={
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A, //здесь имей из нулей 000000000000000 (см. таблицу цифр) 
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A //здесь имей из нулей 000000000000000 (см. таблицу цифр)
};

char temp[16];

char *convert(int k)
{
  int i;
  for(i=0; i<15; i++)
  {
    switch(black[k][i]){
    case 0x0A: temp[i] = '0'; break;
    case 0x0E: temp[i] = '1'; break;
    case 0x1A: temp[i] = '2'; break;
    case 0x1E: temp[i] = '3'; break;
    case 0x2A: temp[i] = '4'; break;
    case 0x2E: temp[i] = '5'; break;
    case 0x3A: temp[i] = '6'; break;
    case 0x3E: temp[i] = '7'; break;
    case 0x4A: temp[i] = '8'; break;
    case 0x4E: temp[i] = '9'; break;
    default: temp[i] = '0'; break;
    }
  }
  temp[15] = '\0';
  return temp;
}
int init()
{
  char imei[16];
  int i;
  for(i=0; i<15; i++)
  {
    imei[i] = *addr;
    addr++;
  }
  imei[15] = '\0';
  
  for(i=0; i<length; i++)
  {
     if(strcmp(imei,convert(i))==0)
       return 0;
  }
   
  return 1;
}


