#ifndef _ICQ_SNAC_H
#define _ICQ_SNAC_H

//#include "icq2_module.h"
#include "icq.h"




typedef void (*SNACHandlerFunc)(short int *flags,long request_id, Packet *packet);

#define SNAC_HANDLER_FUNC(func) \
   void func (short int *flags,long request_id, Packet *packet)

typedef struct {
	short int family;
	short int subtype;
	
	SNACHandlerFunc func;
} SNACHandler;

/* prototypes */
void icq_parse_snac(char *data, int size);
Packet *icq_snac_new(short int family, int subtype, short int *flags, long *reqid);
/* !prototypes */

SNAC_HANDLER_FUNC(icq_snac_server_ready);
SNAC_HANDLER_FUNC(icq_snac_rate_response);
SNAC_HANDLER_FUNC(icq_snac_user_info);
SNAC_HANDLER_FUNC(icq_snac_motd);
SNAC_HANDLER_FUNC(icq_snac_host_versions);
SNAC_HANDLER_FUNC(icq_snac_mem_request);

SNAC_HANDLER_FUNC(icq_snac_location_rights);

SNAC_HANDLER_FUNC(icq_snac_list_rights);
SNAC_HANDLER_FUNC(icq_snac_online_notify);
SNAC_HANDLER_FUNC(icq_snac_offline_notify);

SNAC_HANDLER_FUNC(icq_snac_im_rights);
SNAC_HANDLER_FUNC(icq_snac_incoming_msg);

SNAC_HANDLER_FUNC(icq_snac_bos_rights);

SNAC_HANDLER_FUNC(icq_snac_report_interval);

SNAC_HANDLER_FUNC(icq_snac_server_message);

#endif /* _ICQ_SNAC_H */

