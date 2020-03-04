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

#include "miif.h"
#include "const_strings.h"
#include <iostream>
#include <iomanip>
#include <fstream>

#include "naticq.h"

#ifndef _WIN32
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
#else
 #include <winsock2.h>
#endif
 
#ifdef __FreeBSD__
# include <sys/times.h>
# include <sys/timeb.h>
#endif
   
using namespace std;

// ----------------=========ooooOOOOOOOOOoooo=========----------------
MyIcqInterface::MyIcqInterface(void)
               :ICQKid2(), miif_break_flag(false), now_connect(false), idle_marker(0), idle_deep(0)
{
 ftime(&mark_tmstamp);
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
MyIcqInterface::~MyIcqInterface()
{
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
bool MyIcqInterface::Connect()
{
 now_connect=true;
 bool ret=doConnect(STATUS_BIRTHDAY|STATUS_FREE4CHAT);
 now_connect=false;
 
 if (ret) printContactList();
 return ret;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onIdle(void)
{
 if (idle_deep>0) return; // Prevent against recursion deeplock
 ++idle_deep;
 
 timeb curr_time;
 ftime(&curr_time);
 int tmout=(curr_time.time-mark_tmstamp.time)*1000+curr_time.millitm-mark_tmstamp.millitm;
 if (tmout<250) 
  {
  --idle_deep;
  return;
  }

 if (now_connect)
  {
  //cout << "\rConnect percentage " << getConnectPercentage() << "%   " << flush;
  --idle_deep;
  return;
  }
 /*
 cout << "\ronIdle() is working [";
 switch(idle_marker)
  {
  case 0 : cout << "|"; break;
  case 1 : cout << "/"; break;
  case 2 : cout << "-"; break;
  case 3 : cout << "\\"; break;
  }
 cout << "]" << flush;
 if (++idle_marker>3) idle_marker=0;
 */
 --idle_deep;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onIncomingMTN(string from, uint16_t atype)
{
  /*
 cout << endl << "Incoming MTN from ";
 int uin_ind = findCLUIN(from);
 if (uin_ind<0) cout << from << " ";
 else cout << ContactListUins[uin_ind].nick << "(" << from << ") ";
 
 switch(atype)
  {
  case MTN_BEGIN : cout << "MTN_BEGIN" << endl; break;
  case MTN_TYPED : cout << "MTN_TYPED" << endl; break;
  case MTN_FINISH : cout << "MTN_FINISH" << endl; break;
  default : cout << "MTN_UNKNOWN" << endl; break;
  }
  */
}

void MyIcqInterface::onIncomingSrvAck(string from, uint32_t msg_cookie)
{
  /*cout << endl << "Incoming SRV_ACK from ";
  int uin_ind = findCLUIN(from);
  if (uin_ind<0) cout << from << " ";
  else cout << ContactListUins[uin_ind].nick << "(" << from << ") ";
  cout << endl;
  cout << "Message cookie:" << endl;
  cout << msg_cookie << endl;*/
  if(msg_cookie > 0xFFFF) return;
  
  TPKT *p = (TPKT *)malloc(sizeof(PKT) + 2);
  p->pkt.uin = atoi(from.c_str());
  p->pkt.type = NAT_SRV_ACK;
  p->pkt.data_len = 2;
  ((unsigned short *)p->data)[0] = (unsigned short)(msg_cookie&0x7FFF);
  send_packet(p);
  free(p);
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onIncomingMsg(ICQKid2Message msg)
{
  /*
 cout << endl << (msg.is_offline ? "Offline" : "Online") << " ";
 switch (msg.enc_type)
  {
  case ICQKid2Message::USASCII :
       cout << "US-ASCII encoded";
       break;
  case ICQKid2Message::LOCAL8BIT :
       cout << "8 bit encoded with " << msg.codepage << " codepage";
       break;
  case ICQKid2Message::UCS2BE :
       cout << "UCS-2 Big endian encoded";
       //msg.text=USC2BEto8BIT(msg.text);
       break;
  case ICQKid2Message::UTF8 :
       cout << "UTF-8 encoded";
       break;
  }
  
 cout << " message from ";
 int uin_ind = findCLUIN(msg.uin);
 if (uin_ind<0) cout << msg.uin << " :" << endl;
 else cout << ContactListUins[uin_ind].nick << "(" << msg.uin << ") :" << endl;

 cout << msg.text << endl;*/
 /*cout << "Text color" \
      << setfill('0') << setw(2) << hex << uppercase \
      << " R:" << (uint16_t)msg.text_color[0] << " G:" << (uint16_t)msg.text_color[1] \
      << " B:" << (uint16_t)msg.text_color[2] << " N:" << (uint16_t)msg.text_color[3] << endl << setw(0) << dec << nouppercase;
 cout << "Background color" \
      << setfill('0') << setw(2) << hex << uppercase \
      << " R:" << (uint16_t)msg.bg_color[0] << " G:" << (uint16_t)msg.bg_color[1] \
      << " B:" << (uint16_t)msg.bg_color[2] << " N:" << (uint16_t)msg.bg_color[3] << endl << setw(0) << dec << nouppercase;
 cout << "Message sended " << ctime(&msg.timestamp);
 */
 // NATICQ
 const char *mesg = msg.text.c_str();
 int dlen = msg.text.length();
 
 TPKT *p = (TPKT *)malloc(sizeof(PKT) + dlen + 1);
 switch (msg.enc_type)
 {
  case ICQKid2Message::USASCII :
  case ICQKid2Message::LOCAL8BIT :
       memcpy(p->data, mesg, dlen);
       p->data[dlen] = 0;
       break;
       
  case ICQKid2Message::UCS2BE :
       ucs2win(mesg, p->data, dlen);
       break;
       
  case ICQKid2Message::UTF8 :
  default:
       utf2win(mesg, p->data);
       break;
 }
 //printf("After conversion: %s\n", p->data);
 dlen = strlen(p->data);
 p->pkt.uin = atoi(msg.uin.c_str());
 p->pkt.type = NAT_RECVMSG;
 p->pkt.data_len = dlen;
 send_packet(p);
 free(p);

 /* put message in ACK queue */
 if(pend_id != 0xFFFFFFFF)
 {
   pending_id[pend_id].nat_id = total_sent_bytes;
   pend_id = 0xFFFFFFFF;
 }
 
#if 0
 if (msg.is_offline) return;
 
 string::size_type start_i = msg.text.find_first_not_of(" \t\r\n");
 string::size_type start_cmd;
 
 if ((start_cmd=msg.text.find("help"))==start_i && start_cmd!=msg.text.npos)
  sendMessage(ICQKid2Message(msg.uin, "Available commands:\r\n" \
                                      "  status online\r\n" \
		                      "  status away\r\n" \
		                      "  status dnd\r\n" \
		                      "  status na\r\n" \
		                      "  status busy\r\n" \
		                      "  status chat\r\n" \
		                      "  status invis\r\n" \
		                      "  status evil\r\n" \
		                      "  status depression\r\n" \
		                      "  status home\r\n" \
		                      "  status work\r\n" \
		                      "  status lunch\r\n" \
		                      "  testutf8\r\n" \
		                      "  testcp1251\r\n" \
		                      "  testucs2be\r\n" \
		                      "  visible_status all_can_see\r\n" \
		                      "  visible_status nobody_can_see\r\n" \
		                      "  visible_status vislist_can_see\r\n" \
		                      "  visible_status invislist_cannot_see\r\n" \
		                      "  visible_status contactlist_can_see\r\n" \
		                      "  exit", ICQKid2Message::USASCII));
 else if ((start_cmd=msg.text.find("status online"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_ONLINE);
 else if ((start_cmd=msg.text.find("status away"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_AWAY);
 else if ((start_cmd=msg.text.find("status dnd"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_DND);
 else if ((start_cmd=msg.text.find("status na"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_NA);
 else if ((start_cmd=msg.text.find("status busy"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_OCCUPIED);
 else if ((start_cmd=msg.text.find("status chat"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_FREE4CHAT);
 else if ((start_cmd=msg.text.find("status invis"))==start_i && start_cmd!=msg.text.npos) setStatus(getMyOnlineStatus()|STATUS_INVISIBLE);
 else if ((start_cmd=msg.text.find("status evil"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_EVIL);
 else if ((start_cmd=msg.text.find("status depression"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_DEPRESSION);
 else if ((start_cmd=msg.text.find("status home"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_HOME);
 else if ((start_cmd=msg.text.find("status work"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_WORK);
 else if ((start_cmd=msg.text.find("status lunch"))==start_i && start_cmd!=msg.text.npos) setStatus(STATUS_LUNCH);

 else if ((start_cmd=msg.text.find("visible_status all_can_see"))==start_i && start_cmd!=msg.text.npos) setMyPrivacyStatus(PRIV_ALL_CAN_SEE);
 else if ((start_cmd=msg.text.find("visible_status nobody_can_see"))==start_i && start_cmd!=msg.text.npos) setMyPrivacyStatus(PRIV_NOBODY_CAN_SEE);
 else if ((start_cmd=msg.text.find("visible_status vislist_can_see"))==start_i && start_cmd!=msg.text.npos) setMyPrivacyStatus(PRIV_VISLIST_CAN_SEE);
 else if ((start_cmd=msg.text.find("visible_status invislist_cannot_see"))==start_i && start_cmd!=msg.text.npos) setMyPrivacyStatus(PRIV_INVISLIST_CANNOT_SEE);
 else if ((start_cmd=msg.text.find("visible_status contactlist_can_see"))==start_i && start_cmd!=msg.text.npos) setMyPrivacyStatus(PRIV_CONTACTLIST_CAN_SEE);

 else if ((start_cmd=msg.text.find("testcp1251"))==start_i && start_cmd!=msg.text.npos)
  {
  uint8_t cp1251arr[] = { 0x5B, 0x6C, 0x69, 0x62, 0x49, 0x43, 0x51, 0x4B, 0x69, 0x64, 0x32, 0x20, 0x74, 0x65, 0x73, 0x74, \
                          0x5D, 0x20, 0xDD, 0xF2, 0xEE, 0x20, 0x38, 0x2D, 0xE1, 0xE8, 0xF2, 0xED, 0xEE, 0xE5, 0x20, 0xF1, \
                          0xEE, 0xEE, 0xE1, 0xF9, 0xE5, 0xED, 0xE8, 0xE5, 0x2C, 0x20, 0xEA, 0xEE, 0xE4, 0xEE, 0xE2, 0xE0, \
                          0xFF, 0x20, 0xF1, 0xF2, 0xF0, 0xE0, 0xED, 0xE8, 0xF6, 0xE0, 0x20, 0x31, 0x32, 0x35, 0x31, 0x0A };
  if (!sendMessage(ICQKid2Message(msg.uin, string((char *)cp1251arr, sizeof(cp1251arr)), ICQKid2Message::LOCAL8BIT, 1251)))
    cout << endl << "ERROR: Cannot send cp1251 message!" << endl << flush;
  }
 else if ((start_cmd=msg.text.find("testucs2be"))==start_i && start_cmd!=msg.text.npos)
  {
  uint8_t ucs2bearr[] = { 0x00, 0x5B, 0x00, 0x6C, 0x00, 0x69, 0x00, 0x62, 0x00, 0x49, 0x00, 0x43, 0x00, 0x51, 0x00, 0x4B, \
                          0x00, 0x69, 0x00, 0x64, 0x00, 0x32, 0x00, 0x20, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74, \
                          0x00, 0x5D, 0x00, 0x20, 0x04, 0x2D, 0x04, 0x42, 0x04, 0x3E, 0x00, 0x20, 0x00, 0x31, 0x00, 0x36, \
                          0x00, 0x2D, 0x04, 0x31, 0x04, 0x38, 0x04, 0x42, 0x04, 0x3D, 0x04, 0x3E, 0x04, 0x35, 0x00, 0x20, \
                          0x04, 0x4E, 0x04, 0x3D, 0x04, 0x38, 0x04, 0x3A, 0x04, 0x3E, 0x04, 0x34, 0x00, 0x20, 0x04, 0x41, \
                          0x04, 0x3E, 0x04, 0x3E, 0x04, 0x31, 0x04, 0x49, 0x04, 0x35, 0x04, 0x3D, 0x04, 0x38, 0x04, 0x35, \
                          0x00, 0x20, 0x04, 0x32, 0x00, 0x20, 0x04, 0x3A, 0x04, 0x3E, 0x04, 0x34, 0x04, 0x38, 0x04, 0x40, \
                          0x04, 0x3E, 0x04, 0x32, 0x04, 0x3A, 0x04, 0x35, 0x00, 0x20, 0x00, 0x55, 0x00, 0x43, 0x00, 0x53, \
                          0x00, 0x2D, 0x00, 0x32, 0x00, 0x42, 0x00, 0x45, 0x00, 0x0A};
  int uen_ind=findCLUIN(msg.uin);
  if (uen_ind<0 || ContactListUins[uen_ind].online_status==STATUS_OFFLINE || !ContactListUins[uen_ind].unicode_cap)
   sendMessage(ICQKid2Message(msg.uin, "Unfortunately you cannot receive UCS-2BE messages :(", ICQKid2Message::USASCII));
  else
   if (!sendMessage(ICQKid2Message(msg.uin, string((char *)ucs2bearr, sizeof(ucs2bearr)), ICQKid2Message::UCS2BE)))
    cout << endl << "ERROR: Cannot send usc2be message!" << endl << flush;
  }
 else if ((start_cmd=msg.text.find("testutf8"))==start_i && start_cmd!=msg.text.npos)
  {
  uint8_t text_color[]={0x00, 0xff, 0xff, 0x00};
  uint8_t bg_color[]={0x00, 0x00, 0x4f, 0x00};
  
  int uen_ind=findCLUIN(msg.uin);
  if (uen_ind<0 || ContactListUins[uen_ind].online_status==STATUS_OFFLINE || !ContactListUins[uen_ind].unicode_cap || !ContactListUins[uen_ind].srv_relay_cap)
   sendMessage(ICQKid2Message(msg.uin, "Unfortunately you cannot receive UTF-8 messages :(", ICQKid2Message::USASCII));
  else
   if (!sendMessage(ICQKid2Message(msg.uin, \
     "Мороз и солнце; день чудесный!\r\n" \
     "Еще ты дремлешь, друг прелестный -\r\n" \
     "Пора, красавица, проснись:\r\n" \
     "Открой сомкнуты негой взоры\r\n" \
     "Навстречу северной Авроры,\r\n" \
     "Звездою севера явись!\r\n" \
     "\r\n" \
     "Вечор, ты помнишь, вьюга злилась,\r\n" \
     "На мутном небе мгла носилась;\r\n" \
     "Луна, как бледное пятно,\r\n" \
     "Сквозь тучи мрачные желтела,\r\n" \
     "И ты печальная сидела -\r\n" \
     "А нынче... погляди в окно:\r\n" \
     "\r\n" \
     "Под голубыми небесами\r\n" \
     "Великолепными коврами,\r\n" \
     "Блестя на солнце, снег лежит;\r\n" \
     "Прозрачный лес один чернеет,\r\n" \
     "И ель сквозь иней зеленеет,\r\n" \
     "И речка подо льдом блестит.\r\n" \
     "\r\n" \
     "Вся комната янтарным блеском\r\n" \
     "Озарена. Веселым треском\r\n" \
     "Трещит затопленная печь.\r\n" \
     "Приятно думать у лежанки.\r\n" \
     "Но знаешь: не велеть ли в санки\r\n" \
     "Кобылку бурую запречь?\r\n" \
     "\r\n" \
     "Скользя по утреннему снегу,\r\n" \
     "Друг милый, предадимся бегу\r\n" \
     "Нетерпеливого коня\r\n" \
     "И навестим поля пустые,\r\n" \
     "Леса, недавно столь густые,\r\n" \
     "И берег, милый для меня.", ICQKid2Message::UTF8, 0, false, text_color, bg_color))) cout << endl << "ERROR: Cannot send utf-8 message!" << endl << flush;
  }
 else if ((start_cmd=msg.text.find("exit"))==start_i && start_cmd!=msg.text.npos) miif_break_flag=true;
 else sendMessage(ICQKid2Message(msg.uin, "Unknown command, try to type \"help\"...", ICQKid2Message::USASCII));
#endif
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onAuthRequest(string from, string text)
{/*
 char answ;
 cout << endl << "Auth request from ";
 int uin_ind = findCLUIN(from);
 if (uin_ind<0) cout << from << " : " << text << endl;
 else cout << ContactListUins[uin_ind].nick << "(" << from << ") : " << text << endl;
 cout << "Accept it? (y/n): ";
 
 cin >> answ;
 if (answ=='n' || answ=='N') authReply(from, "Sorry :(", AUTH_DECLINED);
 else authReply(from, "You are welcome!", AUTH_ACCEPTED);*/
 char a[1024];
 int dlen = snprintf(a, 1024, "Auth REQ: %s", text.c_str());
 TPKT *p = (TPKT *)malloc(sizeof(PKT) + dlen);
 utf2win(p->data, a);
 p->pkt.uin = atoi(from.c_str());
 p->pkt.type = NAT_AUTHREQ;
 p->pkt.data_len = dlen;
 send_packet(p);
 free(p);

}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onAuthReply(string from, string text, uint8_t aflag)
{/*
 cout << endl << "Auth reply from ";
 int uin_ind = findCLUIN(from);
 if (uin_ind<0) cout << from;
 else cout << ContactListUins[uin_ind].nick << "(" << from << ")";
 cout << " : " << text << " : " << ((aflag==AUTH_DECLINED) ? "AUTH_DECLINED" : "AUTH_ACCEPTED") << endl;
 */
 // NATICQ
 char *a;
 if(aflag == AUTH_DECLINED)
   a = "Auth resp failed!";
 else
   a = "Auth resp OK";

 int dlen = strlen(a);
 TPKT *p = (TPKT *)malloc(sizeof(PKT) + dlen);
 strncpy(p->data, a, dlen);
 p->pkt.uin = atoi(from.c_str());
 p->pkt.type = NAT_AUTHGRANT;
 p->pkt.data_len = dlen;
 send_packet(p);
 free(p);
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onUserNotify(string uin, uint32_t stat1, uint32_t stat2, bool invis_flag)
{
 unsigned short status = 0;
#if 1
 cout << "User ";
 int uin_ind = findCLUIN(uin);
 if (uin_ind<0) cout << uin;
 else cout << ContactListUins[uin_ind].nick << "(" << uin << ")";
 cout << " changed status to [";
 /*
 if (stat1&STATUS_WEBAWARE) cout << "WEBAWARE ";
 if (stat1&STATUS_SHOWIP) cout << "SHOWIP ";
 if (stat1&STATUS_BIRTHDAY) cout << "BIRTHDAY ";
 if (stat1&STATUS_WEBFRONT) cout << "WEBFRONT ";
 if (stat1&STATUS_DCDISABLED) cout << "DCDISABLED ";
 if (stat1&STATUS_DCAUTH) cout << "DCAUTH ";
 if (stat1&STATUS_DCCONT) cout << "DCCONT ";*/

 switch(stat2)
  {
  case STATUS_OFFLINE    : cout << "OFFLINE"; break;
  case STATUS_ONLINE     : cout << "ONLINE"; break;
  case STATUS_AWAY       : cout << "AWAY"; break;
  case STATUS_DND        : cout << "DND"; break;
  case STATUS_NA         : cout << "NA"; break;
  case STATUS_OCCUPIED   : cout << "OCCUPIED"; break;
  case STATUS_FREE4CHAT  : cout << "FREE4CHAT"; break;
  case STATUS_EVIL       : cout << "STATUS_EVIL"; break;
  case STATUS_DEPRESSION : cout << "STATUS_DEPRESSION"; break;
  case STATUS_HOME       : cout << "STATUS_HOME"; break;
  case STATUS_WORK       : cout << "STATUS_WORK"; break;
  case STATUS_LUNCH      : cout << "STATUS_LUNCH"; break;
  default                : cout << "UNKNOWN";
  }
  cout << "]";// << endl;
#endif
 //NATICQ
 TPKT *p = (TPKT *)malloc(sizeof(PKT) + 3);
 if(invis_flag)
   stat2 = STATUS_INVISIBLE;

 status = getNatStatus(stat2);

 cout << " (Nat: " << status << ")" << endl;
 
 /*p->data[0] = status&0xFF;
 p->data[1] = (status&0xFF00)>>8;*/
 *((unsigned short *)p->data) = status;
 p->data[2] = 0;
 p->pkt.uin = atoi(uin.c_str());
 p->pkt.data_len = 3;
 p->pkt.type = NAT_STATUSCHANGE;
 send_packet(p); 
 free(p);
 /*
 if (invis_flag) cout << " (INVIS)";
 cout << "]" << endl;

 if (uin_ind>=0)
  {
  struct in_addr tmp_addr;
  tmp_addr.s_addr=ContactListUins[uin_ind].last_internal_ip;
  cout << "Internal address " << inet_ntoa(tmp_addr) << ":" << ContactListUins[uin_ind].last_internal_port << endl;
  tmp_addr.s_addr=ContactListUins[uin_ind].last_external_ip;
  cout << "External address " << inet_ntoa(tmp_addr) << endl;
  cout << "Online since " << ctime((const time_t*)&ContactListUins[uin_ind].online_since);
  cout << "Idle since " << ctime((const time_t*)&ContactListUins[uin_ind].idle_since);
  cout << (ContactListUins[uin_ind].srv_relay_cap?"User has SRV_RELAY_CAP":"User hasn't SRV_RELAY_CAP") << endl;
  cout << (ContactListUins[uin_ind].unicode_cap?"User has UNICODE_CAP":"User hasn't UNICODE_CAP") << endl;
  }*/
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onContactListChanged(void)
{
  /*
 cout << endl << "Contact list was changed : " << endl;
 printContactList(); */
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onWasAdded(string from)
{
  /*
 cout << endl << "You was added by ";
 int uin_ind = findCLUIN(from);
 if (uin_ind<0) cout << from;
 else cout << ContactListUins[uin_ind].nick << "(" << from << ")";
 cout << endl;
 */
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onIconChanged(string uin)
{
  /*
 int uin_ind=findCLUIN(uin);
 if (uin_ind<0) return;
 
 cout << endl << ContactListUins[uin_ind].nick << "(" << uin << ") changed avatar picture" << endl;
 
 int old_net_tm=getNetworkTimeout();
 setNetworkTimeout(10000); // I'm going to try download only during 10 seconds
    
 if (!getBuddyIcon(uin)) 
  {
  cout << endl << "ERROR: Cannot download avatar for " << ContactListUins[uin_ind].nick << "(" << uin << ")" << endl;
  setNetworkTimeout(old_net_tm);
  return;
  }
  
 setNetworkTimeout(old_net_tm);
 
 string fname = string("icons/")+uin+string(".")+ContactListUins[uin_ind].nick+string(".avatar");
 ofstream myFile(fname.c_str(), ios::out|ios::binary);
 if (!myFile.is_open())
  {
  cout << endl << "ERROR: Cannot open file " << fname << " for saving avatar" << endl;
  return;
  }
 if (!ContactListUins[uin_ind].icon_data.empty()) myFile.write((const char*)(&ContactListUins[uin_ind].icon_data[0]), ContactListUins[uin_ind].icon_data.size());
 if (!myFile.good()) cout << endl << "ERROR: Cannot write avatar to file " << fname << endl;
 else cout << endl << "Avatar was saved to " << fname << endl;
 myFile.close();
 */
 return;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onXstatusChanged(string uin, size_t x_status, string x_title, string x_descr)
{
  
 cout << endl;
 int uin_ind = findCLUIN(uin);
 if (uin_ind<0) cout << uin;
 else cout << ContactListUins[uin_ind].nick << "(" << uin << ")";
 cout << " changed xStatus to [";
 cout << x_status2string(x_status);
 cout << "] [" << x_title << "] [" << x_descr << "]" << endl;
 
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onRegisterControlPicture(vector<uint8_t> & pic_data, string mime_type, string & pic_str)
{
  /*
 string fname;
 if (mime_type.find("jpeg")!=mime_type.npos) fname="register.jpg";
 else if (mime_type.find("gif")!=mime_type.npos) fname="register.gif";
 else fname="register.pic";
 
 ofstream myFile(fname.c_str(), ios::out|ios::binary);
 if (!myFile.is_open())
  {
  cout << endl << "ERROR: Cannot open file " << fname << " for saving registration picture" << endl;
  return;
  }
 if (!pic_data.empty()) myFile.write((const char*)(&pic_data[0]), pic_data.size());
 if (!myFile.good())
  {
  cout << endl << "ERROR: Cannot write registration picture to file " << fname << endl;
  myFile.close();
  return;
  }
 myFile.close();
 
 cout << endl << "Please, look at the picture " << fname << " and enter letters from it : ";
 cin >> pic_str; 
 */
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onIncomingAutoStatusMsg(ICQKid2Message msg, uint8_t type)
{
  /*
 cout << endl << "Autostatus ";
 switch (msg.enc_type)
  {
  case ICQKid2Message::USASCII :
       cout << "US-ASCII encoded";
       break;
  case ICQKid2Message::LOCAL8BIT :
       cout << "8 bit encoded with " << msg.codepage << " codepage";
       break;
  case ICQKid2Message::UCS2BE :
       cout << "UCS-2 Big endian encoded";
       msg.text=USC2BEto8BIT(msg.text);
       break;
  case ICQKid2Message::UTF8 :
       cout << "UTF-8 encoded";
       break;
  }
 
 switch(type)
  {
  case MSG_TYPE_AUTOAWAY  :
       cout << " MSG_TYPE_AUTOAWAY";
       break;
       
  case MSG_TYPE_AUTOBUSY  :
       cout << " MSG_TYPE_AUTOBUSY";
       break;
       
  case MSG_TYPE_AUTONA    :
       cout << " MSG_TYPE_AUTONA";
       break;
       
  case MSG_TYPE_AUTODND   :
       cout << " MSG_TYPE_AUTODND";
       break;
       
  case MSG_TYPE_AUTOFFC   :
       cout << " MSG_TYPE_AUTOFFC";
       break;
  }

 cout << " message from ";
 int uin_ind = findCLUIN(msg.uin);
 if (uin_ind<0) cout << msg.uin << " :" << endl;
 else cout << ContactListUins[uin_ind].nick << "(" << msg.uin << ") :" << endl;
 cout << msg.text << endl;
 cout << "Text color" \
      << setfill('0') << setw(2) << hex << uppercase \
      << " R:" << (uint16_t)msg.text_color[0] << " G:" << (uint16_t)msg.text_color[1] \
      << " B:" << (uint16_t)msg.text_color[2] << " N:" << (uint16_t)msg.text_color[3] << endl << setw(0) << dec << nouppercase;
 cout << "Background color" \
      << setfill('0') << setw(2) << hex << uppercase \
      << " R:" << (uint16_t)msg.bg_color[0] << " G:" << (uint16_t)msg.bg_color[1] \
      << " B:" << (uint16_t)msg.bg_color[2] << " N:" << (uint16_t)msg.bg_color[3] << endl << setw(0) << dec << nouppercase;
      */
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::printContactList(void)
{
  /*
 cout << endl << "Downloaded SSI Contact-list: " << endl;
 cout << "There are " << ContactListGroups.size() << " groups :" << endl;
 for (size_t i=0; i<ContactListGroups.size(); ++i)
 {
  cout << "  " << ContactListGroups[i].name << endl;
 }

 cout << endl << "There are " << ContactListUins.size() << " contacts :" << endl;
 for (size_t i=0; i<ContactListUins.size(); ++i)
  {
  cout << "  [" << ContactListUins[i].uin << "] [" << ContactListUins[i].nick << "] [" << ContactListUins[i].groupname.c_str() << "] ";
  cout << hex << "item_id=" << ContactListUins[i].itemid << " group_id=" << ContactListUins[i].groupid << dec;
  if (ContactListUins[i].waitauth) cout << " Need auth" << endl;
  else cout << " Dont need auth" << endl;

  }

 cout << endl << "There are " << VisibleList.size() << " visible list records :" << endl;
 for (size_t i=0; i<VisibleList.size(); ++i)
  {
  cout << "  [" << VisibleList[i].uin << "] [" << VisibleList[i].nick << "] ";
  cout << hex << "item_id=" << VisibleList[i].itemid << " group_id=" << VisibleList[i].groupid << dec << endl;
  }

 cout << endl << "There are " << InvisibleList.size() << " invisible list records :" << endl;
 for (size_t i=0; i<InvisibleList.size(); ++i)
  {
  cout << "  [" << InvisibleList[i].uin << "] [" << InvisibleList[i].nick << "] ";
  cout << hex << "item_id=" << InvisibleList[i].itemid << " group_id=" << InvisibleList[i].groupid << dec << endl;
  }

 cout << endl << "There are " << IgnoreList.size() << " ignore list records :" << endl;
 for (size_t i=0; i<IgnoreList.size(); ++i)
  {
  cout << "  [" << IgnoreList[i].uin << "] [" << IgnoreList[i].nick << "] ";
  cout << hex << "item_id=" << IgnoreList[i].itemid << " group_id=" << IgnoreList[i].groupid << dec << endl;
  }
 
 cout << endl << "My privacy status: ";
 switch(getMyPrivacyStatus())
  {
  case PRIV_ALL_CAN_SEE : cout << "PRIV_ALL_CAN_SEE" << endl;
    break;
    
  case PRIV_NOBODY_CAN_SEE : cout << "PRIV_NOBODY_CAN_SEE" << endl;
    break;

  case PRIV_VISLIST_CAN_SEE : cout << "PRIV_VISLIST_CAN_SEE" << endl;
    break;
 if(invis_flag)
   stat2 = STATUS_INVISIBLE;
  case PRIV_INVISLIST_CANNOT_SEE : cout << "PRIV_INVISLIST_CANNOT_SEE" << endl;
    break;

  case PRIV_CONTACTLIST_CAN_SEE : cout << "PRIV_CONTACTLIST_CAN_SEE" << endl;
    break;

  default : cout << "UNKNOWN" << endl;
  }
  */
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
string MyIcqInterface::USC2BEto8BIT(string str)
{
 string ret_str;
 size_t steps = str.length()/2;
 for (size_t i=0; i<steps; ++i) ret_str += str[i*2+1];
 return ret_str;
}

// ----------------=========ooooOOOOOOOOOoooo=========----------------
void MyIcqInterface::onSingOff(uint16_t err_code, string err_url)
{
  /*
 cout << endl << "Got SignOff message from server, reason: ";
 switch(err_code)
  {
  case SIGNOFF_OTHER_LOCATION :
       cout << "SIGNOFF_OTHER_LOCATION";
       break;
  default :
       cout << "UNKNOWN";
       break;
  }
 cout << ", error description url: " << err_url << endl;
 */
}

