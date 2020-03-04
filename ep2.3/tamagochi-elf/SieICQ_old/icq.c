//// Eraser, 2008
#include "include.h"
#include "icq.h"
#include "main.h"
#include "icq_packet.h"
#include "items.h"
#include "capabilities.h"
#include "Random.h"

#include "gui_begin.h"

#define TMR_SECOND 216
#define time_keep 30

#define NOT_IN_LIST "Not In List"


/* This was borrowed from libfaim */
char *icq_encode_password(char *password) {
	char encoding_table[] = {
			0xf3, 0x26, 0x81, 0xc4,
			0x39, 0x86, 0xdb, 0x92,
			0x71, 0xa3, 0xb9, 0xe6,
			0x53, 0x7a, 0x95, 0x7c
	};
	char *encoded = malloc(128);
        //sprintf(encoded,password);
	int i;
	
	for (i = 0; i < strlen(password); i++)
		encoded[i] = (password[i] ^ encoding_table[i]);
	
	return encoded;
}

/////////////////////////////////////////////
void _WriteLogICQ(char *buf, int size, int in_out)
{
 // return;
  int flog=-1;
  unsigned int err;
  flog = fopen("4:\\ZBin\\sieicq\\logs\\icq.log",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
        if (flog!=-1)
	{
		char msg[4096];

		TTime t;
		TDate d;
		GetDateTime(&d,&t);
                if (in_out)
		sprintf(msg, " ->%02d:%02d:%02d ", t.hour,t.min,t.sec);
                else
		sprintf(msg, " <-%02d:%02d:%02d ", t.hour,t.min,t.sec);
		//  sprintf(msg, "%s\n", buf);
		fwrite(flog,msg,strlen(msg),&err);
		fwrite(flog,buf,size,&err);
	}
  fclose(flog,&err);      
}

   unsigned int UIN;
   char PASS[9];
   
   char *auth_cookie;
   int auth_length;
   int flap_seqno;
   int curr_reqid;
   short int icq_seqno;
   
   int login_bos;
   
   char * HostBoss;
   unsigned int PortBoss;
   short int ICQStatus = ICQ_STATUS_OFFLINE;
   short int XStatus=0;
   short int flags_status;
  
   int tenseconds_to_ping;
//////////////////////////////////////////
void CreateICQ()
{
 
  flags_status =0x0101;// INVISIBLE IP
 // flags_status =0x0000; // VISIBLE IP
  auth_cookie=NULL;
  curr_reqid = 0;
  flap_seqno=0;
  icq_seqno = 2;
  Randomize();
}

void CloseICQ()
{

}


extern void Send(char* data,int size);

void send_packet(int channel,Packet* packet)
{

        FLAP_HEAD header;
	
	if (!packet) return;
	
	header.id = 0x2a;
	header.channel = channel;
	header.seq_number = htons(flap_seqno);
	header.data_size = htons(packet->size);
	
	/* Sequence number rolls over at 65535 */
	if (++flap_seqno > 0xffff) flap_seqno = 0;
	
	char *packet_stream;
	packet_stream = (char*)malloc(sizeof(FLAP_HEAD) + packet->size);
	memcpy(packet_stream, &header, sizeof(FLAP_HEAD));
	memcpy(packet_stream + sizeof(FLAP_HEAD), packet->data, packet->size);
	
        _WriteLogICQ(packet_stream, sizeof(FLAP_HEAD) + packet->size,0);
        
	Send(packet_stream, sizeof(FLAP_HEAD) + packet->size);
	
	mfree(packet_stream); 
	PackFree(packet);
        //mfree(header);
}

void send_message(Packet *packet) {
	Packet *full_packet;
	
	/* Create the actual packet that gets sent */
	full_packet = PackNew();
        snac_new(full_packet, 0x0015, 0x0002, NULL, NULL);
	
	/* Add TLV of contents */
	PackAddTLV(full_packet, 0x0001, packet->size + 2);
	
	/* The request length in little endian.  Isn't it great how ICQ
	 * wants to use OSCAR but still has to have it's little endian shit
	 * everywhere? */
	PackAdd16LE(full_packet, packet->size);
	
	/* Add what we had previously built */
	PackAddPack(full_packet, packet);
	
	send_packet(0x02, full_packet);
	PackFree(packet);
}


void send_login() 
{

        login_bos=0;
        Packet *packet=PackNew();
	
       /* Raw start */
	PackAdd32(packet, 0x00000001);
       
	/* ICQ Number (login) */
        char *tmp = malloc(32);
        sprintf(tmp,"%d",UIN);
	PackAddTLVStr(packet, 0x0001, tmp);
        mfree(tmp);
        
	/* Encoded password */
	char *encoded = malloc(128);
	sprintf(encoded,icq_encode_password(PASS));
	PackAddTLVStr(packet, 0x0002, encoded);
	mfree(encoded);
	
	PackAddTLVStr(packet, 0x0003, ICQ_CLIENT_STRING);
	
	/* Unknown */

        PackAddTLV16(packet, 0x0016, 0x010a);
	
	/* Client version */
	PackAddTLV16(packet, 0x0017, ICQ_CLIENT_MAJOR);
	PackAddTLV16(packet, 0x0018, ICQ_CLIENT_MINOR);
	PackAddTLV16(packet, 0x0019, ICQ_CLIENT_BUILD1);
	PackAddTLV16(packet, 0x001a, ICQ_CLIENT_BUILD2);
	PackAddTLV32(packet, 0x0014, ICQ_CLIENT_BUILD3);
	
	/* Language and Country code */
	PackAddTLVStr(packet, 0x000f, Language_Code);
	PackAddTLVStr(packet, 0x000e, Country_Code);

	send_packet(0x01, packet);
}


extern void icq_connect(char *host, int port);
extern char logmsg[256];
extern void SMART_REDRAW(void);

void parse_auth(char *data, int size) {
	
	short int type, length=0;
	char *uin;
	
	Packet *packet = PackNew();
        packet->data=data;
	packet->size = size;
        
        char *tmp_msg=malloc(64);
        
        while (packet->offset<packet->size)
        {
          PackGetTLV(packet, &type, &length);
          switch (type)
          {
          case 0x01:
          uin = (char*)malloc(length + 1);
          PackGet(packet, (char*)uin, length);
          uin[length] = 0;
          break;
          case 0x08:
          {
            short int error_code;
            PackGet16(packet,&error_code);
            switch (error_code)
           {
            //обработка ошибки
	      case 0x0001: strcpy(logmsg, "Invalid nick or password");break; 
              case 0x0002: strcpy(logmsg, "Service temporarily unavailable");break; 
              case 0x0003: strcpy(logmsg, "All other errors");break; 
              case 0x0004: strcpy(logmsg, "Incorrect nick or password, re-enter");break; 
              case 0x0005: strcpy(logmsg, "Mismatch nick or password, re-enter");break; 
              case 0x0006: strcpy(logmsg, "Internal client error (bad input to authorizer)");break; 
              case 0x0007: strcpy(logmsg, "Invalid account");break; 
              case 0x0008: strcpy(logmsg, "Deleted account");break; 
              case 0x0009: strcpy(logmsg, "Expired account");break; 
              case 0x000A: strcpy(logmsg, "No access to database");break; 
              case 0x000B: strcpy(logmsg, "No access to resolver");break; 
              case 0x000C: strcpy(logmsg, "Invalid database fields");break; 
              case 0x000D: strcpy(logmsg, "Bad database status");break; 
              case 0x000E: strcpy(logmsg, "Bad resolver status");break; 
              case 0x000F: strcpy(logmsg, "Internal error");break; 
              case 0x0010: strcpy(logmsg, "Service temporarily offline");break; 
              case 0x0011: strcpy(logmsg, "Suspended account");break; 
              case 0x0012: strcpy(logmsg, "DB send error");break; 
              case 0x0013: strcpy(logmsg, "DB link error");break; 
              case 0x0014: strcpy(logmsg, "Reservation map error");break; 
              case 0x0015: strcpy(logmsg, "Reservation link error");break; 
              case 0x0016: strcpy(logmsg, "The users num connected from this IP has reached the maximum");break; 
              case 0x0017: strcpy(logmsg, "The users num connected from this IP has reached the maximum (reservation)");break; 
              case 0x0018: strcpy(logmsg, "Rate limit exceeded (reservation). Please try to reconnect in a few minutes");break; 
              case 0x0019: strcpy(logmsg, "User too heavily warned");break; 
              case 0x001A: strcpy(logmsg, "Reservation timeout");break; 
              case 0x001B: strcpy(logmsg, "You are using an older version of ICQ. Upgrade required");break; 
              case 0x001C: strcpy(logmsg, "You are using an older version of ICQ. Upgrade recommended");break; 
              case 0x001D: strcpy(logmsg, "Rate limit exceeded. Please try to reconnect in a few minutes");break; 
              case 0x001E: strcpy(logmsg, "Can't register on the ICQ network. Reconnect in a few minutes");break; 
              case 0x0020: strcpy(logmsg, "Invalid SecurID");break; 
              case 0x0022: strcpy(logmsg, "Account suspended because of your age (age < 13)");break; 
            }    

            EndStep();
            SMART_REDRAW();
            Disconnect();
            return;
          }
          case 0x05:
          /* Get new host */
            HostBoss = (char*)malloc(length + 1);
            PackGet(packet, (char*)HostBoss, length);
            HostBoss[length] = 0;
         
            int fl=0;
            int cs=0;
            char strport[5];
            for (int j=0;j<length;j++)
            {
              if (HostBoss[j]==':')
              {
                fl=1;
                cs=0;
              }
              else
              {
              if (fl)
                strport[cs++]=HostBoss[j]; 
              }
            }
            strport[cs]=0;
            
            PortBoss = str2int(strport);
            HostBoss=(char*)realloc(HostBoss, length-cs);
            HostBoss[length-cs-1]=0;
            _WriteLog(HostBoss);
          break;
          
          case 0x06:
          /* Get auth cookie */
          if (auth_cookie) mfree(auth_cookie);
          auth_cookie = (char*)malloc(length);
          auth_length=length;
          PackGet(packet, (char*)auth_cookie, length);
           
          _WriteLog("receiving auth cookie of length ");
           break;
           default:
             {
             char *tmp;
             tmp = malloc(length);
             PackGet(packet, (char*)tmp, length);
             mfree(tmp);
             }
          }
        }
        mfree(tmp_msg);
        
	//Disconnect();
        
        //if (auth_cookie)
        //icq_connect(host, port);

        mfree(uin);
        
}

void send_cookie() 
{
   _WriteLog("send cookie");

        Packet *packet = PackNew();
	/* Raw start */
	PackAdd32(packet, 0x00000001);
	/* Auth cookie */
	PackAddTLVRAW(packet, 0x0006, auth_length, 
				      auth_cookie);
        
	send_packet( 0x01, packet);
}

void parse_snac(char *data, int size) {
	Packet *packet = PackNew();
	packet->data = data;
	packet->size = size;
	packet->offset = 0;
        
        SNAC_HEAD *snac = malloc(sizeof(SNAC_HEAD));
        memcpy(snac, data, sizeof(SNAC_HEAD));
   
        snac->service_id=htons(snac->service_id);
        snac->subtype_id=htons(snac->subtype_id);
        snac->flags=htons(snac->flags);
        snac->request_id=htonl(snac->request_id);
        
        packet->data = packet->data + 2+2+2+4;
        packet->size = size - sizeof(SNAC_HEAD);

	
          char *tmp_msg=malloc(64);
         sprintf(tmp_msg, "SNAC -> 0x%04x, 0x%04x", snac->service_id, snac->subtype_id);
         _WriteLog(tmp_msg);
         mfree(tmp_msg);

        switch (snac->service_id){
        case 0x0001:
          switch (snac->subtype_id)
            {
            case 0x0003: snac_server_ready(snac->flags, snac->request_id,packet);break;
//          case 0x0007: snac_rate_response(snac->flags, snac->request_id,packet);break;
            case 0x000f: snac_user_info(snac->flags, snac->request_id,packet);break;
            case 0x0013: snac_motd(snac->flags, snac->request_id,packet);break;
            case 0x0018: snac_host_versions(snac->flags, snac->request_id,packet);break;
//          case 0x001f: icq_snac_mem_request;break;
            }
          break;
        case 0x0002:
          switch (snac->subtype_id)
            {
            case 0x0003: snac_location_rights(snac->flags, snac->request_id,packet);break;
            }
          break;
        case 0x0003:
          switch (snac->subtype_id)
            {
            case 0x0003: snac_list_rights(snac->flags, snac->request_id,packet);break;
            case 0x000b: snac_online_notify(snac->flags, snac->request_id,packet);break;
            case 0x000c: snac_offline_notify(snac->flags, snac->request_id,packet);break;
            }
          break;  
        case 0x0004:
          switch (snac->subtype_id)
            {
            case 0x0005: snac_im_rights(snac->flags, snac->request_id,packet);break;
            case 0x0007: snac_incoming_msg(snac->flags, snac->request_id,packet);break;
            case 0x0014: snac_typing_msg(snac->flags, snac->request_id,packet);break;
            }
          break;  
        case 0x0009:
          switch (snac->subtype_id)
            {
            case 0x0003: snac_bos_rights(snac->flags, snac->request_id,packet);break;
            }
          break;  
        case 0x000b:
          switch (snac->subtype_id)
            {
//		{ 0x000b, 0x0002, icq_snac_report_interval },
            }
          break;  
        case 0x0013:
          switch (snac->subtype_id)
            {
	    case 0x0006: snac_contactlist(snac->flags, snac->request_id,packet);break;
            }
          break;    
        case 0x0015:
          switch (snac->subtype_id)
            {
	    case 0x0003: snac_server_message(snac->flags, snac->request_id,packet);break;
            }
          break;  

        }
	//n_error("Unhandled SNAC packet family 0x%04x, subtype 0x%04x:", family, subtype);
	//icq_packet_dump(&packet);
  
      
}

void Disconnect() 
{
// отключаемся
  DoneConnected = 0;
        Packet *new_packet = PackNew();
        send_packet(0x04, new_packet);

}

void snac_new(Packet* packet,short int family, short int subtype, short int *flags, int reqid) 
{
	/* Add SNAC values before giving the packet away */
        //packet->size=0;  
        
          char *tmp_msg=malloc(64);
         sprintf(tmp_msg, "SNAC <- 0x%04x, 0x%04x", family, subtype);
         _WriteLog(tmp_msg);
         mfree(tmp_msg);
  
	PackAdd16(packet, family);
	PackAdd16(packet, subtype);
	
	if (flags)
		PackAdd(packet, (char *)flags, 2);
	else
		PackAdd16(packet, 0x0000);
	
	if (reqid)
          PackAdd32(packet, reqid);
//          *reqid = curr_reqid;
        else
	PackAdd32(packet, curr_reqid++);
	
	if (curr_reqid > 0xffffff00) curr_reqid = rand() % 47857;
	
}

void message_new(Packet* packet, short int type, short int *seqno) {
	PackAdd32LE(packet, UIN);
	PackAdd16(packet, type);
	/* ICQ Message Sequence number */
	PackAdd16LE(packet, icq_seqno);
	if (seqno) *seqno = icq_seqno;
	icq_seqno++;
}

void message_parse(Packet *packet) {  
	
  //ICQMessageHandler *handler = message_handlers;
	short int length, seqno, type = 0;
	int uin;
	
	PackGet16LE(packet, &length);
	PackGet32LE(packet, &uin);
	
	PackGet16(packet, &type);
	PackGet16LE(packet, &seqno);
	
	/* check to see if there is any single type handlers for this 
	 * packet 
        */
/*
	for (; handler->type; handler++) {
		if (type == handler->type) {
			(*handler->func)(account, seqno, packet);
			return;
		}
	}
	*/
        
	/* If we don't have enough to check for a subtype then return */
/*	if (packet->size - packet->offset < sizeof(guint16)) {
		n_debug("Not enough remaining bytes to continue, unknown server message "
				"type (0x%04x), seqno (0x%04x):", type, seqno);
		icq_packet_dump(packet);
		return;
	}
	
	PackGet16(packet, subtype);
	*/
	/* Continue on again looking for both type and subtype */
/*	for (handler++; handler->type; handler++) {
		if (type == handler->type && subtype == handler->subtype) {
			(*handler->func)(account, seqno, packet);
			return;
		}
	}
	*/
//	n_debug("Unhandled packet type (0x%04x), subtype (0x%04x), seqno (0x%04x):",
//			type, subtype, seqno);
//	icq_packet_dump(packet);
}


void snac_server_ready(short int flags, int request_id, Packet *packet) 
{
  NextStep("Sending versions");
  
	Packet *new_packet = PackNew();
        snac_new(new_packet,0x0001, 0x0017, NULL, NULL);
	
	_WriteLog("Server ready for session, sending versions");
	
	/* Send some versions packet */

	PackAdd32(new_packet, 0x00010004);
	PackAdd32(new_packet, 0x00130004);
	PackAdd32(new_packet, 0x00020001);
	PackAdd32(new_packet, 0x00030001);
	PackAdd32(new_packet, 0x00150001);
	PackAdd32(new_packet, 0x00040001);
	PackAdd32(new_packet, 0x00060001);
	PackAdd32(new_packet, 0x00090001);
	PackAdd32(new_packet, 0x000a0001);
	PackAdd32(new_packet, 0x000b0001);
       
       	
	send_packet(0x02, new_packet);
}


void snac_motd(short int flags, int request_id, Packet *packet) 
{
  //
  
   Packet *new_packet = PackNew();
   
   
   if (TotalItems()==0)     
   {
     //если пусто, то запрос контакт листа
        snac_new(new_packet, 0x0013, 0x0005, NULL, NULL);
	PackAdd16(new_packet, 0x0000);
	PackAdd16(new_packet, 0x0000);
	PackAdd16(new_packet, 0x0000);
	send_packet(0x02, new_packet);
        
        NextStep("Loading contact list");
   }
   
   
    snac_im_rights(0, 0,NULL);
    
    // чтобы всем рассылалось уведомление о моем статусе
    new_packet = PackNew();
    snac_new(new_packet, 0x0013, 0x0007, NULL, 0x0007);
    send_packet( 0x02, new_packet);
            
    snac_bos_rights(0, 0,NULL);
    
    send_user_info();
   
}


void snac_host_versions(short int flags, int request_id, Packet *packet) {

NextStep("Sending rate request");
      
        Packet *new_packet = PackNew(); 
        snac_new(new_packet, 0x0001, 0x0006, NULL, NULL);
	
	_WriteLog("Server sent us the versions response");
	_WriteLog("Sending rate request");
	
	send_packet(0x02, new_packet);
        
}


void snac_rate_response(short int flags, int request_id, Packet *packet) {
	
NextStep("Rate response");
   
  Packet *new_packet = PackNew();
  

	
	/* Acknowledge rate info */
	snac_new(new_packet, 0x0001, 0x0008, NULL, NULL);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0002);
	PackAdd16(new_packet, 0x0003);
	PackAdd16(new_packet, 0x0004);
	PackAdd16(new_packet, 0x0005);
	send_packet(0x02, new_packet);
	
	/* Request info about ourself */
	new_packet = PackNew();
        snac_new(new_packet, 0x0001, 0x000e, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request location rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0002, 0x0002, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request list management rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0003, 0x0002, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request messaging rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0004, 0x0004, NULL, NULL);
	send_packet( 0x02, new_packet);
	
	/* Request BOS rights info */
	new_packet = PackNew();
        snac_new(new_packet, 0x0009, 0x0002, NULL, NULL);
	send_packet( 0x02, new_packet);
        
}


void snac_location_rights(short int flags, int request_id, Packet *packet) {
	_WriteLog("Recieved location rights");
}

void snac_list_rights(short int flags, int request_id, Packet *packet) {
	_WriteLog("Sending contact list");
	send_contact_list();
}

void snac_im_rights(short int flags, int request_id, Packet *packet) {

NextStep("Initial IM parameter");
// Set message parameters for channel 1 (CLI_SET_ICBM_PARAMS)   
        Packet *new_packet = PackNew(); 
        snac_new(new_packet, 0x0004, 0x0002, NULL, 0x0002);
	/* Some Initial IM parameter */
	PackAdd16(new_packet, 0x0001);
        //client supports typing notifications
	PackAdd32(new_packet, 0x0000000b);
	PackAdd16(new_packet, 0x1f40);
	PackAdd16(new_packet, 0x03e7);
	PackAdd16(new_packet, 0x03e7);
	PackAdd32(new_packet, 0x00000000);
	send_packet(0x02, new_packet);
// Set message parameters for channel 2 (CLI_SET_ICBM_PARAMS)
        new_packet = PackNew(); 
        snac_new(new_packet, 0x0004, 0x0002, NULL, 0x0002);
	PackAdd16(new_packet, 0x0002);
	PackAdd32(new_packet, 0x00000003);
	PackAdd16(new_packet, 0x1f40);
	PackAdd16(new_packet, 0x03e7);
	PackAdd16(new_packet, 0x03e7);
	PackAdd32(new_packet, 0x00000000);
	send_packet(0x02, new_packet);
// Set message parameters for channel 4 (CLI_SET_ICBM_PARAMS)
        new_packet = PackNew(); 
        snac_new(new_packet, 0x0004, 0x0002, NULL, 0x0002);
	PackAdd16(new_packet, 0x0004);
	PackAdd32(new_packet, 0x00000003);
	PackAdd16(new_packet, 0x1f40);
	PackAdd16(new_packet, 0x03e7);
	PackAdd16(new_packet, 0x03e7);
	PackAdd32(new_packet, 0x00000000);
	send_packet(0x02, new_packet);

	_WriteLog("Sending initial IM stuff");
}

void snac_bos_rights(short int flags, int request_id, Packet *packet) {

  // устанавливаем статус
  NextStep("Set status");
  
    SetStatus(ICQ_STATUS_ONLINE);
  
    Packet *new_packet = PackNew();

    new_packet = PackNew();
        snac_new(new_packet, 0x0001, 0x0002, NULL, NULL);
	
	/* Send "Client Ready" */
	PackAdd32(new_packet, 0x00010003);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00020001);
	PackAdd32(new_packet, 0x0101028A);
	PackAdd32(new_packet, 0x00030001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00150001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00040001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00060001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x00090001);
	PackAdd32(new_packet, 0x0110028A);
	PackAdd32(new_packet, 0x000a0003);
	PackAdd32(new_packet, 0x0110028A);
	send_packet( 0x02, new_packet);
        
 
        EndStep();
        
        _WriteLog("STATUS_ONLINE");

        NextStep("Get offline message");

        // запрос оффлайн сообщений
        new_packet = PackNew();
        message_new(new_packet, 0x3c00, NULL);
	send_message(new_packet);
        
         Keep_alive();
         
	//send_key_data("<key>DataFilesIP</key>");
	//send_key_data("<key>BannersIP</key>");
	//send_key_data("<key>ChannelsIP</key>");
        
       // process_active_timer();
       // process_recv_timer();
}

void send_key_data(char *data) {
	Packet *packet = PackNew();
        message_new(packet, 0xd007, NULL);
	int size = strlen(data) + 1;
	
	/* Subtype */
	PackAdd16(packet, 0x9808);
	/* Key size */
	PackAdd16LE(packet, size);
	/* actual data */
	PackAdd(packet, data, size);
	
	send_message(packet);
}

void snac_server_message(short int flags, int request_id, Packet *packet) {
	short int tlv_type, tlv_length;
        
        EndStep();
	
	PackGetTLV(packet, &tlv_type, &tlv_length);
	
	if (tlv_type != 0x0001) return;
	
        message_parse(packet);
}

void send_contact_list() {
	Packet *packet = PackNew();
	
//	if (!ACCOUNT(account)->contacts) return;
	
	snac_new(packet, 0x0003, 0x0004, NULL, NULL);
	
	/* Build a packet with all contacts in it */
//	g_list_foreach(ACCOUNT(account)->contacts, (GFunc)icq_send_contact_cb, packet);
        /*
        for (int i=0;i<ContactList::Active->list->GetCount();i++)
        {
          CLIST* item=ContactList::Active->list->GetItem(i);
          if (!item->isgroup)
          {
            char *str=(char*)malloc(64);
            sprintf(str,percent_d,item->uin);
            PackAdd8(packet,(char)strlen(str));
            PackAddStr(packet,str);
          }
        }
	*/
	send_packet( 0x02, packet);
}

void snac_user_info(short int flags, int request_id, Packet *packet) {
	short int warn_level, tlv_count;
	char user_len;
	char *user;
	
        
	/* username length */
	PackGet8(packet, &user_len);
	/* username */
        user = (char*)malloc((int)user_len + 1);
        PackGet(packet, (char*)user, (int)user_len);
        user[(int)user_len] = 0;
        
	//PackGetStr(packet, user, (int)user_len);
	
	/* Warning level */
	PackGet16(packet, &warn_level);
	PackGet16(packet, &tlv_count);
	
	//n_debug("User info for %s: Warning level = %lu (%lu remaining TLVs)",
	//		user, warn_level, tlv_count);
	
	/* Only set our info if it's the first time we receive this packet */
//	if (Status == CONNECTING) 
//        send_user_info();
}

void send_user_info() {
  
         // Set User Info (capability)
NextStep("Set User Info (capability)");

	Packet *packet = PackNew();
        snac_new(packet, 0x0002, 0x0004, NULL, NULL);
  
  int wData=0x0030;     
  
  if (XStatus)
    wData += 0x10;
	
	PackAddTLV(packet, 0x0005, wData);
	
	/* This is the capability block for the icq client */
        //Client support "see as I type" IMs
        PackAdd(packet, (char*)&capTyping[0], 0x10);
        //SERVER RELAY
        PackAdd(packet, (char*)&capSrvRelay[0],0x10);

        if (XStatus)
        {
          PackAdd(packet, (char*)&capXStatus[(XStatus-1)*0x10], 0x10);
        }

        //здесь можно будет замутить подмену ид клиента
        // а пока пусть будет SieICQ
        PackAdd(packet, (char*)&capSieICQ, 0x10);
        ////
        
	send_packet( 0x02, packet);
}

void SetXStatus(int _xStatus)
{
  XStatus=_xStatus;
  send_user_info();
}

void contact_incoming_msg(char * source, char* msg)
{
TTime t;
TDate d;
GetDateTime(&d,&t);

// ищем контакт
        ITEM *Contact=GetItemByUINstr(source);
        if (Contact!=0)
        {
           send_msg(source,msg);
          // есть непрочитанные
           Contact->isunread = 1;
          // запись сообщения в структуру
           AddHistory(Contact,d,t,IN_MESS,msg);
          ///
          
        }
        else
        {
          //ненайден, добавляем
          ITEM *Group = GetItemByName(NOT_IN_LIST);
          if (Group==0)
              Group = AddItem(0,  333, 0, GROUP, NOT_IN_LIST );
          Group->visible=1;
          int uin = str2int(source);
          Contact = AddItem(1,  Group->GroupID, uin, BUDDY, source);
          Contact->isunread = 1;
          // здеся можно будет еще антиспам прикрутить
          AddHistory(Contact,d,t,IN_MESS,msg);
          
        }
        
   // запись в файл
  char file[512];
  sprintf(file,"%s\\%s.log",HIST_PATH,source);

  
  int flog=-1;
  unsigned int err;
  flog = fopen(file,A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
        if (flog!=-1)
	{
		char _msg[1024];

		sprintf(_msg, "%02d:%02d:%02d %s\n", t.hour,t.min,t.sec,msg);
		//  sprintf(msg, "%s\n", buf);
		fwrite(flog,_msg,strlen(_msg),&err);
	}
  fclose(flog,&err);   
}

void contact_action(char * source, int type, char* msg)
{
TTime t;
TDate d;
GetDateTime(&d,&t);

  // запись в файл
  char file[512];
  sprintf(file,"%s\\%s.log",HIST_PATH,source);


  int flog=-1;
  unsigned int err;
  flog = fopen(file,A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
        if (flog!=-1)
	{
		char _msg[1024];

		sprintf(_msg, "%02d:%02d:%02d %s\n", t.hour,t.min,t.sec,msg);
		//  sprintf(msg, "%s\n", buf);
		fwrite(flog,_msg,strlen(_msg),&err);
	}
  fclose(flog,&err);     
  
       // ищем контакт
        ITEM *Contact=GetItemByUINstr(source);
        if (Contact!=0)
        {
          // есть непрочитанные
           Contact->isunread = 1;
          // запись сообщения в структуру
           AddHistory(Contact,d,t,type,msg);
          ///
          
        }
        else
        {
          //ненайден, добавляем
          // здеся можно будет еще антиспам прикрутить
          ITEM *Group = GetItemByName(NOT_IN_LIST);
          if (Group==0)
              Group = AddItem(0,  333, 0, GROUP, NOT_IN_LIST);
          Group->visible=1;
          int uin = str2int(source);
          Contact = AddItem(1,  Group->GroupID, uin, BUDDY, source);
          Contact->isunread = 1;
          // здеся можно будет еще антиспам прикрутить
          AddHistory(Contact,d,t,type,msg);
          
         
          
        }
  
}

void handle_simple_message(char *source, short int num_tlvs, Packet *packet) {
	
        short int msg_tlv, msg_len, encoding, subencoding;
	char *msg;
	
	PackSkipTLVs(packet, num_tlvs);
	
	PackGetTLV(packet, &msg_tlv, &msg_len);
	if (msg_tlv != 0x0002) {
		_WriteLog("Simple message has an unexpected TLV:");
		return;
	}
	
	/* Unknown, usually 05 01 00 01 01 01 01 */
	PackSkip(packet, 7);
	
	/* This is the actual message length */
	PackGet16(packet, &msg_len);
        
	/* Encodings of the content */
	PackGet16(packet, &encoding);
	PackGet16(packet, &subencoding);
	
	if (encoding != 0x0000 || subencoding != 0x0000)
		_WriteLog("Warning: unsupported encoding");
	
	/* And finally we get the message */
	msg = malloc(msg_len-4+1);
	PackGet(packet, msg, msg_len - 4);
	msg[msg_len - 4]=0;
        
        // здесь надо обработать полученное тело сообщения
        // source = UIN от кого
	contact_incoming_msg(source, msg);
        
        
	mfree(msg);
}

void handle_advanced_message(char *source, short int num_tlvs, Packet *packet) {
	short int msg_tlv, msg_len, msg_type, flag1, flag2;
	char msg_subtype, msg_flags;
	char *msg;
	
	PackSkipTLVs(packet, num_tlvs);
	
	PackGetTLV(packet, &msg_tlv, &msg_len);
	if (msg_tlv != 0x0005) {
		_WriteLog("Advanced message has an unexpected TLV:");
		return;
	}
	
	PackGet16(packet, &flag1);
	
	/* Message cookie, again */
	PackSkip(packet, 8);
	
	/* Capability block */
	PackSkip(packet, 8);
	
	PackGet16(packet, &flag2);
	
	/* Unknown */
	PackSkip(packet, 4);
	
	if (flag1 != 0x0000 || flag2 != 0x000a) {
		_WriteLog("Unknown advanced message type:");
		//return;
	}
	
	PackGet16(packet, &msg_type);
	if (msg_type == 0x0005) {
		_WriteLog("Looks to be a file transfer:");
		//return;
	}
	
	/* Some unknown that rejetto didn't see as necessary */
	PackSkip(packet, 53);
	
	PackGet8(packet, &msg_subtype);
	PackGet8(packet, &msg_flags);
	
	/* Two unknowns that don't seem to mean much */
	PackSkip(packet, 8);
	
	/* Actual message length */
	PackGet16LE(packet, &msg_len);
	
	/* Actual message */
        msg = malloc(msg_len+1);
	PackGet(packet, msg, msg_len);
	msg[msg_len]=0;
	
        // здесь надо обработать полученное тело сообщения
        // source = UIN от кого
	 contact_incoming_msg(source, msg);
	
        mfree(msg);
}

void handle_special_message(char *source,short int num_tlvs, Packet *packet) {
	char subtype, flags;
	short int length, tlv_type;
	char *msg;
	
	PackSkipTLVs(packet, num_tlvs);
	
	PackGetTLV(packet, &tlv_type, &length);
	
	if (tlv_type != 0x0005) {
		_WriteLog("Unknown special message type 0x%04x:");
		return;
	}
	
	/* This is the uin, but we already have that from the source */
	PackSkip(packet, 4);
	
	/* Determines what we are going to do with this message */
	PackGet8(packet, &subtype);
	PackGet8(packet, &flags);
	
	PackGet16LE(packet, &length);
	
	msg = malloc(length+1);
	PackGet(packet, msg, length);
	msg[length]=0;
        
	switch (subtype) {
	  case 0x01: //Plain text (simple) message
		contact_incoming_msg(source, msg);
		break;
		
	  case 0x04: // URL message 
            {
		  //char *buf, *descr, *url;
		  //url = next_arg(msg, &url, NULL, "\xFE");
		  //buf = d_strdup_printf("\002%s\001\n%s", descr, url);
		  //contact_incoming_msg(CONTACT(contact), buf);
		  //mfree(buf);
		  break;
	  }
		
	  case 0x06: // Authorization request message
		contact_action(source, AUTREQ,msg);
		break;
		
	  case 0x07: // Authorization denied message
		contact_action(source, AUTDEN,msg);
		break;
		
	  case 0x08: // Authorization given message
		contact_action(source, AUTGIV, msg);
		break;
		
	  case 0x0C: // "You-were-added" message
		contact_action(source, YOUADD, msg);
		break;
		
	  case 0x13: //Contact list message
		contact_incoming_msg(source, msg);
		break;
		
	  default:
		_WriteLog("Unknown special message subtype");
		break;
	}
	
	mfree(msg);
}

void snac_incoming_msg(short int flags, int request_id, Packet *packet) {
	long cookie;
        char source_len;
	short int channel, warning_level, num_tlvs;
	char *source;
	
	/* IM Cookie */
	PackGet(packet, &cookie, 8);
	
	/* The IM channel that this is coming in on */
	
        PackGet16(packet, &channel);
	
	PackGet8(packet, &source_len);
        source = malloc((int)source_len+1);
	PackGet(packet, source, source_len);
        source[(int)source_len]=0;
	
	PackGet16(packet, &warning_level);
	PackGet16(packet, &num_tlvs);
	
	switch (channel) {
	  case 0x0001:
		handle_simple_message(source, num_tlvs, packet);
		break;
		
	  case 0x0002:
		handle_advanced_message(source, num_tlvs, packet);
		break;
		
	  case 0x0004:
		handle_special_message(source, num_tlvs, packet);
		break;
		
	  default:
		_WriteLog("Unknown IM channel ");
		break;
	}
	
	mfree(source);
        
}

void snac_typing_msg(short int flags, int request_id, Packet *packet) {
	long cookie;
        char source_len;
	short int channel, typing=0;
	char *source;
	/* IM Cookie */
	PackGet(packet, &cookie, 8);
	/* The IM channel that this is coming in on */
        PackGet16(packet, &channel);
	PackGet8(packet, &source_len);
        source = malloc((int)source_len+1);
	PackGet(packet, source, source_len);
        source[(int)source_len]=0;
        
        PackGet16(packet,&typing);
	
//0x0000 - typing finished sign
//0x0001 - text typed sign
//0x0002 - typing begun sign
        
        // тут найти контакт uin 
        ITEM *Contact=GetItemByUINstr(source);
        if (Contact!=0)
          Contact->istyping=typing;
          
	
	mfree(source);
        
}

void snac_contactlist(short int flags, int request_id, Packet *packet) {

  FreeItemsList();
//разбираем контакт лист        
//Version number of SSI protocol (currently 0x00)
	char vnum;
	PackGet8(packet, &vnum);
        _WriteLog("parse CL");
        short int Count=0;
        PackGet16(packet,&Count);
        if (Count<=0) return;
        for (int i=0;i<Count;i++)
        {
          short int len=0;
          PackGet16(packet,&len);
          char *uin_name=(char*)malloc(64);
          char *name = (char*)malloc((int)len + 1);
          PackGet(packet, (char*)name, (int)len);
          name[(int)len] = 0;
          short int GroupID=0;
          PackGet16(packet,&GroupID);
          short int ItemID=0;
          PackGet16(packet,&ItemID);
          short int Type=0;
          PackGet16(packet,&Type);
          PackGet16(packet,&len);
          
          short int type, length=0;
          int end_info=packet->offset+len;

          while (packet->offset<end_info)
            {
              PackGetTLV(packet, &type, &length);
              switch (type)
              {
              case 0x0131:
                PackGet(packet, (char*)uin_name, length);
                uin_name[(int)length]=0;
                break;
              default:
                PackSkip(packet,length);
              }
            }
          if ((ItemID!=0)||(GroupID!=0)) //  ItemID=0 GroupID=0 корневой (Master group) элемент
          switch (Type)
          {
          case 0:// контакт
            {
            int uin = str2int(name);
            AddItem(ItemID,GroupID,uin,BUDDY,uin_name);
            break;
            }
          case 1:// group
            AddItem(ItemID,GroupID,0,GROUP,name);
            break;
          default:
            break;
          }
          
          mfree(name);
          mfree(uin_name);
          
        }
         GroupVisible(1);
}

GBSTMR tmr_active;

void Keep_alive()
{
  if (connect_state==3)
  {
  Packet* pack=PackNew();
  send_packet( 0x05, pack);
  }
  GBS_StartTimerProc(&tmr_active,TMR_SECOND*time_keep,Keep_alive);
}


void SetStatus(int Status)
{
       ICQStatus = Status;

       int long_status=0;
       long_status=flags_status*0x10000+Status;
       
       Packet *new_packet = PackNew();
       snac_new(new_packet, 0x0001, 0x001e, NULL, NULL);
       PackAddTLV32(new_packet, 0x0006, long_status);
       send_packet(0x02, new_packet);
}

void snac_online_notify(short int flags, int request_id, Packet *packet)
{       short int type, length=0;
        short int status=0;
        short int flags_status=0;
        int enableTyping = 0;
        int xstatus = 0;
        int clientid = 0;
	char uin_length;
        char *uin;
	/* Pull off the length of the UIN string */
	PackGet8(packet, &uin_length);
	/* Get the UIN string */
        uin = malloc((int)uin_length+1);
	PackGet(packet, (char*)uin, (int)uin_length);   
        uin[(int)uin_length]=0;
	// skip warning not use ICQ
        packet->offset+=2;
        // Count TLV
        short int cTLV;
        PackGet16(packet,&cTLV);
        for (int i=0;i<cTLV;i++)
        {
          PackGetTLV(packet, &type, &length);
          switch (type)
          {
            /*
            TLV.Type(0x01) - user class 
            TLV.Type(0x03) - signon time 
            TLV.Type(0x05) - member since 
            TLV.Type(0x06) - user status 
            TLV.Type(0x0A) - external ip address 
            TLV.Type(0x0C) - dc info (optional) 
            TLV.Type(0x0D) - user capabilities 
            TLV.Type(0x0F) - online time 
            TLV.Type(0x11) - times updated 
            TLV.Type(0x19) - new-style capabilities list 
            TLV.Type(0x1D) - user icon id & hash 
           */
              case 0x0D:
                {
                char * cap = malloc((int)length);
                PackGet(packet, (char*)cap, (int)length);   
                // совместимость с Typing
                enableTyping = FindCapabilities(cap, length, &capTyping[0]);
                // Xstatus
                xstatus = FindXStatus(cap, length);
                // ID клиента
                clientid= ClientID(cap, length);
                mfree(cap);
                }
              break;
            
              //user status          
              case 0x06: 
                if (length>=4)
                {
                  PackGet16(packet,&flags_status); 
                  PackGet16(packet,&status); 
                }
                else
                {
                  flags_status=0;
                  status=ICQ_STATUS_ONLINE;
                }
                break;
              default:
                 packet->offset+=length;
          }
        }
	
        //char tmp[128];
        //sprintf(tmp,"status %s - %02d", uin, status);
        //_WriteLog(tmp);
        
        // тут найти контакт uin и сменить его статус на user_status
        ITEM *Contact=GetItemByUINstr(uin);
        if (Contact!=0)
        {
           Contact->Status=status;
           Contact->enable_typing=enableTyping;
           Contact->XStatus = xstatus;
           Contact->client_id = clientid;
        }
       
	mfree(uin);
}

void snac_offline_notify(short int flags, int request_id, Packet *packet)
{
        char uin_length;
        char *uin;
	/* Pull off the length of the UIN string */
	PackGet8(packet, &uin_length);
	/* Get the UIN string */
        uin = malloc((int)uin_length+1);
	PackGet(packet, (char*)uin, (int)uin_length); 
        uin[(int)uin_length]=0;
        
        // тут найти контакт uin и сменить его статус на OFFLINE
        ITEM *Contact=GetItemByUINstr(uin);
        if (Contact!=0)
        {
         Contact->Status=ICQ_STATUS_OFFLINE;
         Contact->enable_typing=0;
         Contact->XStatus =0;
        }
        
	mfree(uin);
}


void new_message_packet(Packet *packet, char* dUIN, short int channel) {
	char cookie[8];
        
        for (int n = 0; n < 6; n++)
		cookie[n] = rand();
        cookie[6]=0;
        cookie[7]=0;
        
	snac_new(packet,0x0004, 0x0006, NULL, NULL);
	
	/* IM Cookie */
	PackAdd(packet, (char*)cookie, 8);
	/* Channel ID */
	PackAdd16(packet, channel);
	/* UIN of the recipient */
	PackAdd8(packet, strlen(dUIN));
	PackAddStr(packet, dUIN);
}

void send_msg(char* dUIN, char *msg) {

TTime t;
TDate d;
GetDateTime(&d,&t);
ITEM *Contact=GetItemByUINstr(dUIN);
        if (Contact!=0)
        {
          // запись сообщения в структуру
           AddHistory(Contact,d,t,OUT_MESS,msg);
        }

  
	Packet *packet = PackNew();
        new_message_packet(packet, dUIN, 0x0001);
	short int size = strlen(msg);
	
	/* Start of TLV for message */
	PackAddTLV(packet, 0x0002, size + 13);
	
	/* Unknown purpose */
	PackAdd16(packet, 0x0501);
	
	/* Some size value combo */
	PackAdd16(packet, 0x0001);
	PackAdd8(packet, 0x01);
	
	/* Intro to message */
	PackAdd16(packet, 0x0101);
	
	/* Size of message */
	PackAdd16(packet, size + 4);
	
	/* Encoding of message */
	PackAdd16(packet, 0x0000);
	
	/* Subencoding */
	PackAdd16(packet, 0x0000);
	
	/* And we get to the message itself! */
	PackAddStr(packet, msg);
	
	/* Some empty value (this really isn't a TLV after all) */
	PackAddTLV(packet, 0x0006, 0x0000);
	
	send_packet(0x02, packet);
}
