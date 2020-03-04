#include "include.h"
#include "icq.h"
#include "icqsocket.h"
#include "log_widget.h"
#include "icq_packet.h"
//#include "icq_snac.h"


/* This was borrowed from libfaim */
char *icq_encode_password(char *password) {
	char encoding_table[] = {
			0xf3, 0x26, 0x81, 0xc4,
			0x39, 0x86, 0xdb, 0x92,
			0x71, 0xa3, 0xb9, 0xe6,
			0x53, 0x7a, 0x95, 0x7c
	};
	char *encoded;
        //sprintf(encoded,password);
	int i;
	
	for (i = 0; i < strlen(password); i++)
		encoded[i] = (password[i] ^ encoding_table[i]);
	
	return encoded;
}
///////////////////////////////////////////////////////
/////////////////////////////////////////////
void _WriteLog2(char *buf)
{
  int flog=-1;
  unsigned int err;
  flog = fopen("4:\\ZBin\\sieicq\\logs\\sieicq.log",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
        if (flog!=-1)
	{
		char msg[512];

		TTime t;
		TDate d;
		GetDateTime(&d,&t);
		sprintf(msg, "%02d:%02d:%02d %s\n", t.hour,t.min,t.sec,buf);
		//  sprintf(msg, "%s\n", buf);
		fwrite(flog,msg,strlen(msg),&err);
	}
  fclose(flog,&err);      
}
/////////////////////////////////////////////
void _WriteLogICQ(char *buf, int size)
{
  int flog=-1;
  unsigned int err;
  flog = fopen("4:\\ZBin\\sieicq\\logs\\icq.log",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE,&err);
        if (flog!=-1)
	{
		char msg[512];

		TTime t;
		TDate d;
		GetDateTime(&d,&t);
		sprintf(msg, "%02d:%02d:%02d ", t.hour,t.min,t.sec);
		//  sprintf(msg, "%s\n", buf);
		fwrite(flog,msg,strlen(msg),&err);
		fwrite(flog,buf,size,&err);
	}
  fclose(flog,&err);      
}
//////////////////////////////////////////


ICQClient::ICQClient()
{
  Socket = new SocketAbstract();
  PASS=new char[64];
  Host=new char[64];
  auth_cookie=NULL;
}

ICQClient::~ICQClient()
{
  delete PASS;
  delete Host;
  delete Socket;

}


void ICQClient::Create()
{
  Socket->Create();
}

void ICQClient::Close()
{
  Socket->Close();
}


void ICQClient::Login() 
{
  int ip=str2ip(Host);
  if (ip!=0xFFFFFFFF)
  {
   LogWidget::Active->Status="Connect IP!";
   LogWidget::Active->Redraw();
   Socket->Start(ip,Port);
  }
  else
  {
   LogWidget::Active->Status="Connect host!";
   LogWidget::Active->Redraw();
   Socket->Start(Host,Port);
  }
}

void ICQClient::onConnected()
{
  // auth
  
}

void ICQClient::onDataRead()
{
  //главная функция
  //_WriteLog("DATA_READ");
  
   char tmp_buf[4096]; // Буфер приема даннях
   char *pack;
   int nrecv = Socket->Recv(tmp_buf, sizeof(tmp_buf)); // Получаем даные
   
   if (!nrecv)
   {
     _WriteLog("Empty recv!!");
     LogWidget::Active->Status="Empty recv!!";
     LogWidget::Active->Redraw();
     return;
   }
   
   _WriteLogICQ(tmp_buf,nrecv);
   
   FLAP_HEAD *flap = new FLAP_HEAD;
   memcpy(flap, tmp_buf, sizeof(FLAP_HEAD));
   
 //  char *tmp_msg=new char[64];
 //  sprintf(tmp_msg, percent_d, flap->data_size);
 //  _WriteLog(tmp_msg);
 //  delete tmp_msg;
   
   flap->data_size = htons(flap->data_size);
   flap_seqno=htons(flap->seq_number);
   pack = tmp_buf + sizeof(FLAP_HEAD);
   
   switch(flap->channel) {
	  case 0x01:
		if (auth_cookie) {
			send_cookie();
		} else 
			send_login();
		break;
		
	  case 0x02:
		 parse_snac(pack,flap->data_size);
		break;
		
	  case 0x04:
		if (!auth_cookie)
		       parse_auth(pack,flap->data_size);
		break;
		
	  default:
            ;
	};
  // delete tmp_buf;
  // delete pack;
  // delete flap;
}

void ICQClient::send_packet(int channel,Packet* packet)
{
        FLAP_HEAD header;
	char *packet_stream;
	
	if (!packet) return;
	
        //_WriteLogICQ((char*)&header,sizeof(FLAP_HEAD));
        
	header.id = 0x2a;
	header.channel = channel;
	header.seq_number = htons(flap_seqno);
	header.data_size = htons(packet->size);
	
        //_WriteLogICQ((char*)&header,sizeof(FLAP_HEAD));

	/* Sequence number rolls over at 65535 */
	if (++flap_seqno > 0xffff) flap_seqno = 0;
	
	packet_stream = (char*)malloc(sizeof(FLAP_HEAD) + packet->size);
	memcpy(packet_stream, &header, sizeof(FLAP_HEAD));
	memcpy(packet_stream + sizeof(FLAP_HEAD), packet->data, packet->size);
	
        _WriteLogICQ(packet_stream, sizeof(FLAP_HEAD) + packet->size);
        
	Socket->Send(packet_stream, sizeof(FLAP_HEAD) + packet->size);
	

	
	delete(packet_stream); 
	PackFree(packet);
}

void ICQClient::send_login() 
{
   LogWidget::Active->Status="Send Login";
   LogWidget::Active->Redraw();
   
        Packet *packet=PackNew();
	char *encoded = new char[128];
	
       /* Raw start */
	PackAdd32(packet, 0x00000001);
        

       
	/* ICQ Number (login) */
        char *tmp = new char(32);
        sprintf(tmp,"%d",UIN);
	PackAddTLVStr(packet, 0x0001, tmp);
        delete tmp;
        
	/* Encoded password */
	sprintf(encoded,icq_encode_password(PASS));
	PackAddTLVStr(packet, 0x0002, encoded);
	delete encoded;
	
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
	PackAddTLVStr(packet, 0x000f, "en");
	PackAddTLVStr(packet, 0x000e, "us");

	send_packet(0x01, packet);
}

void ICQClient::parse_auth(char *data, int size) {
	
	short int type, length=0;
	char *uin, *host;
	int port;
	
	 LogWidget::Active->Status="Parsing auth message";
         LogWidget::Active->Redraw();
	
	Packet *packet = PackNew();
	//memcpy(packet->data,data,size);
        packet->data=data;
	packet->size = size;
        
        _WriteLogICQ(packet->data, packet->size);
        
        char *tmp_msg=new char[64];
        
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
          case 0x04:
            LogWidget::Active->Status="Error !!!";
            LogWidget::Active->Redraw();
            //обработка ошибки
            
            
            Disconnect();
          return;
          case 0x05:
          /* Get new host */
            host = (char*)malloc(length + 1);
            PackGet(packet, (char*)host, length);
            host[length] = 0;
         
            int fl=0;
            int cs=0;
            char strport[5];
            for (int j=0;j<length;j++)
            {
              if (host[j]==':')
              {
                fl=1;
                cs=0;
              }
              else
              {
              if (fl)
                strport[cs++]=host[j]; 
              }
            }
            strport[cs]=0;
            
            port = str2int(strport);
            host=(char*)realloc(host, length-cs);
            host[length-cs-1]=0;
            _WriteLog(host);
          break;
          
          case 0x06:
          /* Get auth cookie */
            _WriteLog("cookie");
          if (auth_cookie) delete auth_cookie;
          auth_cookie = (char*)malloc(length);
          auth_length=length;
          PackGet(packet, (char*)auth_cookie, length);
           
          //n_debug("receiving auth cookie of length %i", account->auth_length);
           break;
           default:
             char *tmp = (char*)malloc(length);
             PackGet(packet, (char*)tmp, length);
             delete tmp;
          }
        }
        delete tmp_msg;
        
	Disconnect();
        
        //if (auth_cookie)
        icq_connect(host, port);

        _safe_delete(uin);
        _safe_delete(host);
        
}

void ICQClient::icq_connect(char *host, int port) {
  int ip=str2ip(host);
  if (ip!=0xFFFFFFFF)
  { 
    _WriteLog("BOS");
   LogWidget::Active->Status="Connect BOS!";
   LogWidget::Active->Redraw();
   Socket->Start(ip,port);
  }
/*
	person_change_status(PERSON(account), CONNECTING);
*/
}

void ICQClient::send_cookie() 
{
   _WriteLog("send cookie");
   LogWidget::Active->Status="send cookie";
   LogWidget::Active->Redraw();
   
       Packet *packet = PackNew();
	
	/* Raw start */
	PackAdd32(packet, 0x00000001);
	/* Auth cookie */
	PackAddTLVRAW(packet, 0x0006, auth_length, 
				      auth_cookie);
	
	send_packet( 0x01, packet);
}

void ICQClient::parse_snac(char *data, int size) {
	Packet packet;
	packet.data = data;
	packet.size = size;
	packet.offset = 0;
        
        SNAC_HEAD *snac = new SNAC_HEAD;
        memcpy(snac, data, sizeof(SNAC_HEAD));
   
 //  char *tmp_msg=new char[64];
 //  sprintf(tmp_msg, percent_d, flap->data_size);
 //  _WriteLog(tmp_msg);
 //  delete tmp_msg;

 
        snac->service_id=htons(snac->service_id);
        snac->subtype_id=htons(snac->subtype_id);
        snac->flags=htons(snac->flags);
        snac->request_id=htonl(request_id);
   
        packet.data = packet.data + sizeof(SNAC_HEAD);
	
//	PackGet16(&packet, &family);
//	PackGet16(&packet, &subtype);
//	PackGet(&packet, &flags, sizeof(flags));
//	PackGet32(&packet, &request_id);
	
	//n_debug("Incoming 0x%04x / 0x%04x packet", family, subtype);
        switch (snac->service_id){
          
        case 0x0001:
          switch (snac->subtype_id)
            {
            case 0x0003:snac_server_ready(flags, request_id, &packet);break;
            }
          break;
          
  
          
        }
        //{ 0x0001, 0x0003, icq_snac_server_ready },
                /*
		{ 0x0001, 0x0007, icq_snac_rate_response },
		{ 0x0001, 0x000f, icq_snac_user_info },
		{ 0x0001, 0x0013, icq_snac_motd },
		{ 0x0001, 0x0018, icq_snac_host_versions },
		{ 0x0001, 0x001f, icq_snac_mem_request },
	
		{ 0x0002, 0x0003, icq_snac_location_rights },
	
		{ 0x0003, 0x0003, icq_snac_list_rights },
		{ 0x0003, 0x000b, icq_snac_online_notify },
		{ 0x0003, 0x000c, icq_snac_offline_notify },
	
		{ 0x0004, 0x0005, icq_snac_im_rights },
		{ 0x0004, 0x0007, icq_snac_incoming_msg },
	
		{ 0x0009, 0x0003, icq_snac_bos_rights },
	
		{ 0x000b, 0x0002, icq_snac_report_interval },
	
		{ 0x0015, 0x0003, icq_snac_server_message },
	
	for (handler = snac_handlers; handler->func; handler++) {
		if (handler->family == family && handler->subtype == subtype) {
			if (handler->func)
				(*handler->func)( flags, request_id, &packet);
			
			return;
		}
	}
	*/
	//n_error("Unhandled SNAC packet family 0x%04x, subtype 0x%04x:", family, subtype);
	//icq_packet_dump(&packet);
  
}

void ICQClient::Disconnect() 
{
  
  Socket->Close();

}


void ICQClient::snac_server_ready(short int flags, int request_id, Packet packet) {
	Packet *new_packet = snac_new(0x0001, 0x0017, NULL, NULL);
	
	//n_debug("Server ready for session, sending versions");
	
	/* Send some versions packet */

	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0003);
	PackAdd16(new_packet, 0x0002);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0003);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0015);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0004);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0006);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x0009);
	PackAdd16(new_packet, 0x0001);
	PackAdd16(new_packet, 0x000a);
	PackAdd16(new_packet, 0x0001);
	
	send_packet(0x02, new_packet);
}

void ICQClient::snac_new(Packet* packet,short int family, short int subtype, short int *flags, int *reqid) {
	Packet *retval;
	
	retval = PackNew();
	
	/* Add SNAC values before giving the packet away */
	PackAdd16(retval, family);
	PackAdd16(retval, subtype);
	
	if (flags)
		PackAdd(retval, flags, 2);
	else
		PackAdd16(retval, 0x0000);
	
	if (reqid) *reqid = curr_reqid;
	PackAdd32(retval, curr_reqid++);
	
	if (curr_reqid > 0xffffff00) curr_reqid = rand() % 47857;
	
	//return retval;
}
