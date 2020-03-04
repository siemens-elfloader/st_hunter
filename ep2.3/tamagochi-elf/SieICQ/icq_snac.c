#include "include.h"
#include "icq.h"
#include "icqsocket.h"
#include "log_widget.h"
#include "icq_packet.h"
#include "icq_snac.h"
#include "icq.h"

#define guint16=int;
#define guint8=short int;
#define guint32=long;

static SNACHandler snac_handlers[] = {
    /* family, subtype, handler */
		{ 0x0001, 0x0003, icq_snac_server_ready },
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
	
		{ 0, 0, NULL }
};

void icq_parse_snac(char *data, int size) {
	SNACHandler *handler;
	Packet packet;
	int family, subtype;
	short int flags[2];
	long request_id;
	
	packet.data = data;
	packet.size = size;
	packet.offset = 0;
	
	PackGet16(&packet, family);
	PackGet16(&packet, subtype);
	PackGet(&packet, flags, sizeof(flags));
	PackGet32(&packet, request_id);
	
	//n_debug("Incoming 0x%04x / 0x%04x packet", family, subtype);
	
	for (handler = snac_handlers; handler->func; handler++) {
		if (handler->family == family && handler->subtype == subtype) {
			if (handler->func)
				(*handler->func)( flags, request_id, &packet);
			
			return;
		}
	}
	
	//n_error("Unhandled SNAC packet family 0x%04x, subtype 0x%04x:", family, subtype);
//	icq_packet_dump(&packet);
}

Packet *icq_snac_new(ICQAccount *account, guint16 family, guint16 subtype, guint8 *flags, guint32 *reqid) {
	Packet *retval;
	
	retval = d_new(Packet);
	
	/* Add SNAC values before giving the packet away */
	PACKET_ADD_16(retval, family);
	PACKET_ADD_16(retval, subtype);
	
	if (flags)
		icq_packet_add(retval, flags, 2);
	else
		PACKET_ADD_16(retval, 0x0000);
	
	if (reqid) *reqid = account->curr_reqid;
	PACKET_ADD_32(retval, account->curr_reqid++);
	
	if (account->curr_reqid > 0xffffff00) account->curr_reqid = rand() % 47857;
	
	return retval;
}

SNAC_HANDLER_FUNC(icq_snac_server_ready) {
	Packet *new_packet = icq_snac_new(account, 0x0001, 0x0017, NULL, NULL);
	
	n_debug("Server ready for session, sending versions");
	
	/* Send some versions packet */
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0003);
	PACKET_ADD_16(new_packet, 0x0002);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0003);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0015);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0004);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0006);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0009);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x000a);
	PACKET_ADD_16(new_packet, 0x0001);
	
	icq_send_packet(account, 0x02, new_packet);
}

SNAC_HANDLER_FUNC(icq_snac_rate_response) {
	Packet *new_packet;
	
	/* Acknowledge rate info */
	new_packet = icq_snac_new(account, 0x0001, 0x0008, NULL, NULL);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0002);
	PACKET_ADD_16(new_packet, 0x0003);
	PACKET_ADD_16(new_packet, 0x0004);
	icq_send_packet(account, 0x02, new_packet);
	
	/* Request info about ourself */
	new_packet = icq_snac_new(account, 0x0001, 0x000e, NULL, NULL);
	icq_send_packet(account, 0x02, new_packet);
	
	/* Request location rights info */
	new_packet = icq_snac_new(account, 0x0002, 0x0002, NULL, NULL);
	icq_send_packet(account, 0x02, new_packet);
	
	/* Request list management rights info */
	new_packet = icq_snac_new(account, 0x0003, 0x0002, NULL, NULL);
	icq_send_packet(account, 0x02, new_packet);
	
	/* Request messaging rights info */
	new_packet = icq_snac_new(account, 0x0004, 0x0004, NULL, NULL);
	icq_send_packet(account, 0x02, new_packet);
	
	/* Request BOS rights info */
	new_packet = icq_snac_new(account, 0x0009, 0x0002, NULL, NULL);
	icq_send_packet(account, 0x02, new_packet);
}

SNAC_HANDLER_FUNC(icq_snac_user_info) {
	guint16 warn_level, tlv_count;
	guint8 user_len;
	char *user;
	
	/* username length */
	PACKET_GET_8(packet, user_len);
	
	/* username */
	PACKET_GET_STR(packet, user, user_len);
	
	/* Warning level */
	PACKET_GET_16(packet, warn_level);
	PACKET_GET_16(packet, tlv_count);
	
	n_debug("User info for %s: Warning level = %lu (%lu remaining TLVs)",
			user, warn_level, tlv_count);
	
	/* Only set our info if it's the first time we receive this packet */
	if (PERSON(account)->status == CONNECTING) icq_send_user_info(account);
}

SNAC_HANDLER_FUNC(icq_snac_motd) {
	guint16 motd_length;
	char *motd;
	
	/* This is probably part of a TLV but don't really care */
	PACKET_SKIP(packet, 4);
	
	/* This is what we really wanted, the MOTD */
	PACKET_GET_16(packet, motd_length);
	
	PACKET_GET_STR(packet, motd, motd_length);
	
	n_debug("ICQ MOTD: %s", motd);
	
	d_free(motd);
}

SNAC_HANDLER_FUNC(icq_snac_host_versions) {
	Packet *new_packet = icq_snac_new(account, 0x0001, 0x0006, NULL, NULL);
	
	n_debug("Server sent us the versions response");
	n_debug("Sending rate request");
	
	/* Requesting rate information */
	icq_send_packet(account, 0x02, new_packet);
}

SNAC_HANDLER_FUNC(icq_snac_mem_request) {
	guint16 offset, length;
	
	PACKET_GET_16(packet, offset);
	PACKET_GET_16(packet, length);
	
	n_debug("Server has requested a memory chunk at 0x%04x length 0x%04x",
			offset, length);
}

SNAC_HANDLER_FUNC(icq_snac_location_rights) {
	n_debug("Recieved location rights");
}

SNAC_HANDLER_FUNC(icq_snac_list_rights) {
	n_debug("Sending contact list");
	
	icq_send_contact_list(account);
}

static void update_contact_status(ICQAccount *account, Packet *packet, Status status) {
	guint16 warning_level, tlv_count;
	ICQContact *contact;
	Packet *tmp_packet;
	guint8 uin_length;
	char *uin;
	
	/* Pull off the length of the UIN string */
	PACKET_GET_8(packet, uin_length);
	
	/* Get the UIN string */
	PACKET_GET_STR(packet, uin, uin_length);   
	
	/* Find the contact associated with this UIN */
	contact = icq_find_contact(account, uin, 0);
	
	d_free(uin);
	
	if (!contact) return;
	
	person_change_status(PERSON(contact), status);
	
	PACKET_GET_16(packet, warning_level);
	PACKET_GET_16(packet, tlv_count);
	
	/* Extract the extended ICQ status */
	tmp_packet = icq_extract_tlv(packet, 0x0006, tlv_count);
	if (tmp_packet) {
		guint32 status_field;
		
		PACKET_GET_32(tmp_packet, status_field);
		
		/* n_debug("%s's status: 0x%08x", PERSON(contact)->dname, status_field); */
		
		if (status_field & 0x00000001) 
			person_change_status(PERSON(contact), AWAY);
	}
	
	/* Extract exact signon time */
	tmp_packet = icq_extract_tlv(packet, 0x0003, tlv_count);
	if (tmp_packet)
		PACKET_GET_32(tmp_packet, PERSON(contact)->signon);
	
	/* Get the user's IP */
	tmp_packet = icq_extract_tlv(packet, 0x000a, tlv_count);
	if (tmp_packet)
		icq_packet_get(tmp_packet, &contact->ip, 4);
}

SNAC_HANDLER_FUNC(icq_snac_online_notify) {
	update_contact_status(account, packet, ONLINE);
}

SNAC_HANDLER_FUNC(icq_snac_offline_notify) {
	update_contact_status(account, packet, OFFLINE);
}

SNAC_HANDLER_FUNC(icq_snac_im_rights) {
	Packet *new_packet = icq_snac_new(account, 0x0004, 0x0002, NULL, NULL);
	
	/* Some Initial IM parameter */
	PACKET_ADD_16(new_packet, 0x0000);
	PACKET_ADD_32(new_packet, 0x00000003);
	PACKET_ADD_16(new_packet, 0x1f40);
	PACKET_ADD_16(new_packet, 0x03e7);
	PACKET_ADD_16(new_packet, 0x03e7);
	PACKET_ADD_32(new_packet, 0x00000000);
	
	n_debug("Sending initial IM stuff");
	
	icq_send_packet(account, 0x02, new_packet);
}

static void handle_simple_message(ICQAccount *account, char *source, guint16 num_tlvs, Packet *packet) {
	ICQContact *contact = icq_find_contact(account, source, 1);
	guint16 msg_tlv, msg_len, encoding, subencoding;
	char *msg;
	
	PACKET_SKIP_TLVS(packet, num_tlvs);
	
	PACKET_GET_TLV(packet, msg_tlv, msg_len);
	if (msg_tlv != 0x0002) {
		n_debug("Simple message has an unexpected TLV:");
		icq_packet_dump(packet);
		return;
	}
	
	/* Unknown, usually 05 01 00 01 01 01 01 */
	PACKET_SKIP(packet, 7);
	
	/* This is the actual message length */
	PACKET_GET_16(packet, msg_len);
	
	/* Encodings of the content */
	PACKET_GET_16(packet, encoding);
	PACKET_GET_16(packet, subencoding);
	
	if (encoding != 0x0000 || subencoding != 0x0000)
		n_debug("Warning: unsupported encoding (0x%04x/0x%04x)",
				encoding, subencoding);
	
	/* And finally we get the message */
	PACKET_GET_STR(packet, msg, msg_len - 4);
	
	contact_incoming_msg(CONTACT(contact), msg);
	d_free(msg);
}

/* Massimo Melina <rejetto@libero.it> for his research into this particular
 * message */
static void handle_advanced_message(ICQAccount *account, char *source, guint16 num_tlvs, Packet *packet) {
	ICQContact *contact = icq_find_contact(account, source, 1);
	guint16 msg_tlv, msg_len, msg_type, flag1, flag2;
	guint8 msg_subtype, msg_flags;
	char *msg;
	
	PACKET_SKIP_TLVS(packet, num_tlvs);
	
	PACKET_GET_TLV(packet, msg_tlv, msg_len);
	if (msg_tlv != 0x0005) {
		n_debug("Advanced message has an unexpected TLV:");
		icq_packet_dump(packet);
		return;
	}
	
	PACKET_GET_16(packet, flag1);
	
	/* Message cookie, again */
	PACKET_SKIP(packet, 8);
	
	/* Capability block */
	PACKET_SKIP(packet, 8);
	
	PACKET_GET_16(packet, flag2);
	
	/* Unknown */
	PACKET_SKIP(packet, 4);
	
	if (flag1 != 0x0000 || flag2 != 0x000a) {
		n_debug("Unknown advanced message type:");
		icq_packet_dump(packet);
		return;
	}
	
	PACKET_GET_16(packet, msg_type);
	if (msg_type == 0x0005) {
		n_debug("Looks to be a file transfer:");
		icq_packet_dump(packet);
		return;
	}
	
	/* Some unknown that rejetto didn't see as necessary */
	PACKET_SKIP(packet, 53);
	
	PACKET_GET_8(packet, msg_subtype);
	PACKET_GET_8(packet, msg_flags);
	
	/* Two unknowns that don't seem to mean much */
	PACKET_SKIP(packet, 8);
	
	/* Actual message length */
	PACKET_GET_16LE(packet, msg_len);
	
	/* Actual message */
	PACKET_GET_STR0(packet, msg, msg_len);
	
	contact_incoming_msg(CONTACT(contact), msg);
	d_free(msg);
}

/* rejetto did a pretty thorough job on this one */
static void handle_special_message(ICQAccount *account, char *source, guint16 num_tlvs, Packet *packet) {
	ICQContact *contact = icq_find_contact(account, source, 1);
	guint8 subtype, flags;
	guint16 length, tlv_type;
	char *msg;
	
	PACKET_SKIP_TLVS(packet, num_tlvs);
	
	PACKET_GET_TLV(packet, tlv_type, length);
	
	if (tlv_type != 0x0005) {
		n_debug("Unknown special message type 0x%04x:", tlv_type);
		icq_packet_dump(packet);
		return;
	}
	
	/* This is the uin, but we already have that from the source */
	PACKET_SKIP(packet, 4);
	
	/* Determines what we are going to do with this message */
	PACKET_GET_8(packet, subtype);
	PACKET_GET_8(packet, flags);
	
	PACKET_GET_16LE(packet, length);
	
	PACKET_GET_STR0(packet, msg, length);
	
	switch (subtype) {
	  case 0x01:
		contact_incoming_msg(CONTACT(contact), msg);
		break;
		
	  case 0x04: {
		  char *buf, *descr, *url;
		  
		  url = next_arg(msg, &url, NULL, "\xFE");
		  buf = d_strdup_printf("\002%s\001\n%s", descr, url);
		  
		  contact_incoming_msg(CONTACT(contact), buf);
		  
		  d_free(buf);
		  break;
	  }
		
	  case 0x06:
		contact_action(CONTACT(contact), "has requested authorization to add you to their list.");
		break;
		
	  case 0x07:
		contact_action(CONTACT(contact), "has denied your request for authorization:");
		break;
		
	  case 0x08:
		contact_action(CONTACT(contact), "has authorized you.");
		break;
		
	  case 0x0C:
		contact_action(CONTACT(contact), "has added you to their list.");
		break;
		
	  case 0x13:
		contact_incoming_msg(CONTACT(contact), msg);
		break;
		
	  default:
		n_debug("Unknown special message subtype 0x%02x:", subtype);
		icq_packet_dump(packet);
		break;
	}
	
	d_free(msg);
}

SNAC_HANDLER_FUNC(icq_snac_incoming_msg) {
	guint8 cookie[8], source_len;
	guint16 channel, warning_level, num_tlvs;
	char *source;
	
	/* IM Cookie */
	icq_packet_get(packet, cookie, 8);
	
	/* The IM channel that this is coming in on */
	PACKET_GET_16(packet, channel);
	
	PACKET_GET_8(packet, source_len);
	PACKET_GET_STR(packet, source, source_len);
	
	PACKET_GET_16(packet, warning_level);
	PACKET_GET_16(packet, num_tlvs);
	
	n_debug("Incoming Message:");
	icq_packet_dump(packet);
	
	switch (channel) {
	  case 0x0001:
		handle_simple_message(account, source, num_tlvs, packet);
		break;
		
	  case 0x0002:
		handle_advanced_message(account, source, num_tlvs, packet);
		break;
		
	  case 0x0004:
		handle_special_message(account, source, num_tlvs, packet);
		break;
		
	  default:
		n_error("Unknown IM channel 0x%04x", channel);
		icq_packet_dump(packet);
		break;
	}
	
	d_free(source);
}

static void send_key_data(ICQAccount *account, char *data) {
	Packet *packet = icq_message_new(account, 0xd007, NULL);
	guint16 size = strlen(data) + 1;
	
	/* Subtype */
	PACKET_ADD_16(packet, 0x9808);
	
	/* Key size */
	PACKET_ADD_16LE(packet, size);
	
	/* actual data */
	icq_packet_add(packet, data, size);
	
	icq_send_message(account, packet);
}

SNAC_HANDLER_FUNC(icq_snac_bos_rights) {
	Packet *new_packet = icq_snac_new(account, 0x0001, 0x0002, NULL, NULL);
	
	/* Send "Client Ready" */
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0003);
	PACKET_ADD_16(new_packet, 0x0110);
	PACKET_ADD_16(new_packet, 0x028a);
	PACKET_ADD_16(new_packet, 0x0002);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0101);
	PACKET_ADD_16(new_packet, 0x028a);
	
	PACKET_ADD_16(new_packet, 0x0003);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0110);
	PACKET_ADD_16(new_packet, 0x028a);
	PACKET_ADD_16(new_packet, 0x0015);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0110);
	PACKET_ADD_16(new_packet, 0x028a);
	
	PACKET_ADD_16(new_packet, 0x0004);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0110);
	PACKET_ADD_16(new_packet, 0x028a);
	PACKET_ADD_16(new_packet, 0x0006);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0110);
	PACKET_ADD_16(new_packet, 0x028a);
	
	PACKET_ADD_16(new_packet, 0x0009);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0110);
	PACKET_ADD_16(new_packet, 0x028a);
	PACKET_ADD_16(new_packet, 0x000a);
	PACKET_ADD_16(new_packet, 0x0001);
	PACKET_ADD_16(new_packet, 0x0110);
	PACKET_ADD_16(new_packet, 0x028a);
	
	n_debug("Sending Client Ready");
	
	icq_send_packet(account, 0x02, new_packet);
	
	person_change_status(PERSON(account), ONLINE);
	
	new_packet = icq_message_new(account, 0x3c00, NULL);
	icq_send_message(account, new_packet);
	
	send_key_data(account, "<key>DataFilesIP</key>");
	send_key_data(account, "<key>BannersIP</key>");
	send_key_data(account, "<key>ChannelsIP</key>");
}

SNAC_HANDLER_FUNC(icq_snac_report_interval) {
	guint16 interval;
	
	PACKET_GET_16(packet, interval);
	
	n_debug("Minimum report interval = %u", interval);
}

/* These packets are a complete and utter mess.  It's basically everything 
 * OSCAR didn't have room for but ICQ still wanted to do.  It ends up being
 * a packet inside of a packet.  This would make THREE layers of
 * type/length/sequence */
SNAC_HANDLER_FUNC(icq_snac_server_message) {
	guint16 tlv_type, tlv_length;
	
	PACKET_GET_TLV(packet, tlv_type, tlv_length);
	
	if (tlv_type != 0x0001) return;
	
	icq_message_parse(account, packet);
}

 
 
