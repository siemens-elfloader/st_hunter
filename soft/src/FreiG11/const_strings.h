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

#ifndef _ICQKID2_TEST_CONST_STRINGS_H_
#define _ICQKID2_TEST_CONST_STRINGS_H_

#include <string>
using namespace std;

void init_strings_maps(void);

string x_status2string(unsigned int ind);
string country2string(unsigned int ind);
string gender2string(unsigned int ind);
string lang2string(unsigned int ind);
string marital2string(unsigned int ind);
string occupation2string(unsigned int ind);
string interest2string(unsigned int ind);
string past2string(unsigned int ind);
string affil2string(unsigned int ind);

#endif
