#ifndef _ICQ_H
#define _ICQ_H

#include "include.h"
#include "icqsocket.h"
#include "icq_packet.h"
//#include "icq_snac.h"

//#define ICQ_CLIENT_STRING "Sie_ICQ"
#define ICQ_CLIENT_STRING "ICQ Inc. - Product of ICQ (TM).2002a.5.37.1.3728.85"
//#define ICQ_CLIENT_STRING "ICQ Inc. - Product of ICQ (TM).2000b.4.63.1.3279.85"

#define ICQ_CLIENT_MAJOR 0x0005
#define ICQ_CLIENT_MINOR 0x0025
#define ICQ_CLIENT_BUILD1 0x0001
#define ICQ_CLIENT_BUILD2 0x0ccf
#define ICQ_CLIENT_BUILD3 0x00000055

#define ICQ_REQUEST_EMPTY (0x3c00)
#define ICQ_REQUEST_VALUE (0xd007)

typedef struct
{
  char id; 
  char channel; 
  short int  seq_number; 
  short int  data_size; 
}FLAP_HEAD;

typedef struct
{
 short int   service_id; 
 short int   subtype_id;
 short int   flags; 
 int  request_id; 
}SNAC_HEAD;

//enum (OFFLINE,ONLINE)

#define CONNECTING        0xffff
#define STATUS_ONLINE     0x0000 //  Status is online 
#define STATUS_AWAY       0x0001 //  Status is away 
#define STATUS_DND        0x0002 //  Status is no not disturb (DND) 
#define STATUS_NA         0x0004 //  Status is not available (N/A) 
#define STATUS_OCCUPIED   0x0010 //  Status is occupied (BISY) 
#define STATUS_FREE4CHAT  0x0020 //  Status is free for chat 
#define STATUS_INVISIBLE  0x0100 //  Status is invisible 

#define STATUS_WEBAWARE   0x0001 //  Status webaware flag 
#define STATUS_SHOWIP     0x0002 //  Status show ip flag 
#define STATUS_BIRTHDAY   0x0008 //  User birthday flag 
#define STATUS_WEBFRONT   0x0020 //  User active webfront flag 
#define STATUS_DCDISABLED 0x0100 //  Direct connection not supported 
#define STATUS_DCAUTH     0x1000 //  Direct connection upon authorization 
#define STATUS_DCCONT     0x2000 //  DC only with contact users 
 

 

class ICQClient
{
public:
/*    virtual void onCreate(); // Создание сокета
    virtual void onDataRead(); // Получение данных
    virtual void onConnected(); // Соединение
    virtual void onClose(); // Закрытие сокета
    virtual void onRemoteClose(); // Удаленное закрытие
    virtual void onError(SOCK_ERROR err); // Обработчик ошибок
*/
  
   void onConnected(); // Соединение
   void onDataRead(); // Получение данных
  
   unsigned int UIN;
   char * PASS;
   
   char *auth_cookie;
   int auth_length;
   int flap_seqno;
   int curr_reqid;
   short int icq_seqno;
   
   char * Host;
   unsigned int Port;

   void Create();
   void Close();
  
   void Login();
   void Disconnect();
   
   short int ICQStatus;
   short int flags_status;
  
   ICQClient();
   ~ICQClient();
   
  static ICQClient* Active;
   
   SocketAbstract *Socket;
   
  int tenseconds_to_ping;
  void Keep_alive();
  
  void SetStatus(int Status);
  
private:
  
  void send_login();
  void parse_auth(char *data, int size);
  void parse_snac(char *data, int size);
  void send_cookie();
  void send_packet(int chanell, Packet* packet);
  void send_message(Packet *packet);
  
  void message_parse(Packet *packet) ;

  void icq_connect(char* host, int port);
  void snac_new(Packet* packet, short int family, short int subtype, short int *flags, int *reqid);
  void message_new(Packet* packet, short int type, short int *seqno);
  void snac_server_ready(short int flags, int request_id, Packet *packet);
  void snac_host_versions(short int flags, int request_id, Packet *packet);
  void snac_rate_response(short int flags, int request_id, Packet *packet);
  void snac_location_rights(short int flags, int request_id, Packet *packet);
  void snac_list_rights(short int flags, int request_id, Packet *packet);
  void snac_im_rights(short int flags, int request_id, Packet *packet);
  void snac_bos_rights(short int flags, int request_id, Packet *packet);
  void snac_server_message(short int flags, int request_id, Packet *packet);
  void snac_user_info(short int flags, int request_id, Packet *packet);
  void send_user_info();
  void snac_incoming_msg(short int flags, int request_id, Packet *packet);
  void send_key_data(char *data);
  void snac_motd(short int flags, int request_id, Packet *packet);



  void snac_contactlist(short int flags, int request_id, Packet *packet);



  void send_contact_list();



};

#endif /* _ICQ_H */

