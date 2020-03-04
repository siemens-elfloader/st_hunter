//// Eraser, 2008
#ifndef _ICQ_H
#define _ICQ_H

#include "include.h"
#include "icq_packet.h"
//#include "icq_snac.h"

#define ICQ_CLIENT_STRING "ICQ Client"

#define ICQ_CLIENT_MAJOR 0x0006
#define ICQ_CLIENT_MINOR 0x0000
#define ICQ_CLIENT_BUILD1 0x0000
#define ICQ_CLIENT_BUILD2 0x1797
#define ICQ_CLIENT_BUILD3 0x00007535

#define ICQ_REQUEST_EMPTY (0x3c00)
#define ICQ_REQUEST_VALUE (0xd007)

#define Language_Code "en"
#define Country_Code "en"

typedef struct
{
  char id; 
  char channel; 
  short int  seq_number; 
  short int  data_size; 
}FLAP_HEAD;

typedef struct
{
  FLAP_HEAD flap;
  char data[64000];
}TPKT;

typedef struct
{
 short int   service_id; 
 short int   subtype_id;
 short int   flags; 
 int  request_id; 
}SNAC_HEAD;

//enum (OFFLINE,ONLINE)


// статусы
#define ICQ_STATUS_OFFLINE          0xffff
#define ICQ_STATUS_ONLINE           0x0000
#define ICQ_STATUS_AWAY             0x0001
#define ICQ_STATUS_NA               0x0005
#define ICQ_STATUS_OCCUPIED         0x0011
#define ICQ_STATUS_DND              0x0013
#define ICQ_STATUS_FREE4CHAT        0x0020
#define ICQ_STATUS_INVISIBLE        0x0100
#define ICQ_STATUS_EVIL		    0x3000
#define ICQ_STATUS_DEPRESSION	    0x4000
#define ICQ_STATUS_HOME		    0x5000
#define ICQ_STATUS_WORK		    0x6000
#define ICQ_STATUS_LUNCH	    0x2001

#define STATUS_WEBAWARE   0x0001 //  Status webaware flag 
#define STATUS_SHOWIP     0x0002 //  Status show ip flag 
#define STATUS_BIRTHDAY   0x0008 //  User birthday flag 
#define STATUS_WEBFRONT   0x0020 //  User active webfront flag 
#define STATUS_DCDISABLED 0x0100 //  Direct connection not supported 
#define STATUS_DCAUTH     0x1000 //  Direct connection upon authorization 
#define STATUS_DCCONT     0x2000 //  DC only with contact users 


extern    unsigned int UIN;
extern    char PASS[9];
   
extern       char *auth_cookie;
extern       int auth_length;
extern       int flap_seqno;
extern       int curr_reqid;
extern       short int icq_seqno;

extern int login_bos;
   
extern  char * HostBoss;
extern  unsigned int PortBoss;
extern  short int ICQStatus;
extern  short int XStatus;
extern  short int flags_status;
  
extern  int tenseconds_to_ping;
  
   void CreateICQ();
   void CloseICQ();
  
   void Disconnect();
   
  void _WriteLogICQ(char *buf, int size, int in_out); 

  void Keep_alive();
  
  void SetStatus(int Status);
  void SetXStatus(int _xStatus); // 0-нет Х статуса, 1-37 - Х статус
  
  
  void send_login();
  void parse_auth(char *data, int size);
  void parse_snac(char *data, int size);
  void send_cookie();
  void send_packet(int chanell, Packet* packet);
  void send_message(Packet *packet);
  
  void message_parse(Packet *packet) ;

  void snac_new(Packet* packet, short int family, short int subtype, short int *flags, int reqid);
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
  void snac_typing_msg(short int flags, int request_id, Packet *packet);

  void send_key_data(char *data);
  void snac_motd(short int flags, int request_id, Packet *packet);



  void snac_contactlist(short int flags, int request_id, Packet *packet);
  
  void snac_online_notify(short int flags, int request_id, Packet *packet);
  void snac_offline_notify(short int flags, int request_id, Packet *packet);



  void send_contact_list();

  void send_msg(char* dUIN, char *msg);




#endif /* _ICQ_H */

