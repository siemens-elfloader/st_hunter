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

#ifndef _ICQKID2_TEST_MIIF_H_
#define _ICQKID2_TEST_MIIF_H_

#include "icqkid2.h"
#include <sys/timeb.h>
#include "naticq.h"

class MyIcqInterface : public ICQKid2 {
 public:
  MyIcqInterface(void);
  virtual ~MyIcqInterface();
  
  bool Connect();
  
  virtual void onIdle(void);
  virtual void onIncomingMTN(string from, uint16_t atype);
  virtual void onIncomingSrvAck(string from, uint32_t msg_cookie);
  virtual void onIncomingMsg(ICQKid2Message msg);
  virtual void onAuthRequest(string from, string text);
  virtual void onAuthReply(string from, string text, uint8_t aflag);
  virtual void onUserNotify(string uin, uint32_t stat1, uint32_t stat2, bool invis_flag);
  virtual void onContactListChanged(void);
  virtual void onWasAdded(string from);
  virtual void onIconChanged(string uin);
  virtual void onXstatusChanged(string uin, size_t x_status, string x_title, string x_descr);
  virtual void onRegisterControlPicture(vector<uint8_t> & pic_data, string mime_type, string & pic_str);
  virtual void onIncomingAutoStatusMsg(ICQKid2Message msg, uint8_t type);
  virtual void onSingOff(uint16_t err_code, string err_url);
  
  bool miif_break_flag;
 
#if 0
  int getNatStatus(int status)
  {
    switch(status)
    {
      case STATUS_OFFLINE     : return IS_OFFLINE;
      case STATUS_INVISIBLE   : return IS_INVISIBLE;
      case STATUS_AWAY        : return IS_AWAY;
      case STATUS_NA          : return IS_NA;
      case STATUS_OCCUPIED    : return IS_OCCUPIED;
      case STATUS_DND         : return IS_DND;
      case STATUS_DEPRESSION  : return IS_DEPRESSION;
      case STATUS_EVIL        : return IS_EVIL;
      case STATUS_HOME        : return IS_HOME;
      case STATUS_LUNCH       : return IS_LUNCH;
      case STATUS_WORK        : return IS_WORK;
      case STATUS_ONLINE      : return IS_ONLINE;
      case STATUS_FREE4CHAT   : return IS_FFC;
      default: return IS_OFFLINE;
    }
  }
#else
  int getNatStatus(int a1)
  {
    if ( a1 > 16 )
    {
      if ( a1 > 12288 )
      {
        if ( a1 == 16384 )
          return IS_DEPRESSION;
        if ( a1 == 20480 )
          return IS_HOME;
        if ( a1 == 24576 )
          return IS_WORK;
      }
      else
      {
        if ( a1 == 12288 )
          return IS_EVIL;
        if ( a1 > 4096 )
        {
          if ( a1 == 8193 )
            return IS_LUNCH;
        }
        else
        {
          if ( a1 != 4096 )
          {
            if ( a1 == 32 )
              return IS_FFC;
	    
            if ( a1 == 256 )
              return IS_INVISIBLE;
          }
        }
      }
      return IS_OFFLINE;
    }
    if ( a1 == 16 )
    {
      return IS_OCCUPIED;
    }
    else
    {
      switch ( a1 )
      {
        case 1:
          return IS_AWAY;
        case 4:
          return IS_NA;
        case 2:
          return IS_DND;
        case 0:
          return IS_ONLINE;
        default:
          return 0;
      }
    }
    return IS_OFFLINE;
  }
#endif
 private:
  void printContactList(void);
  string USC2BEto8BIT(string str);
  
  bool now_connect;
  timeb mark_tmstamp;
  int idle_marker;
  int idle_deep;
/*
  static int statuses13[] = {
    STATUS_OFFLINE,    //IS_OFFLINE = 0,
    STATUS_INVISIBLE,  //IS_INVISIBLE,
    STATUS_AWAY,       //IS_AWAY,
    STATUS_NA,         //IS_NA,
    STATUS_OCCUPIED,   //IS_OCCUPIED,
    STATUS_DND,        //IS_DND,
    STATUS_DEPRESSION, //IS_DEPRESSION,
    STATUS_EVIL,       //IS_EVIL,
    STATUS_HOME,       //IS_HOME,
    STATUS_LUNCH,      //IS_LUNCH,
    STATUS_WORK,       //IS_WORK,
    STATUS_ONLINE,     //IS_ONLINE,
    STATUS_FFC         //IS_FFC,
  };*/
};

#endif
