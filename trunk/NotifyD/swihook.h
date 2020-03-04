#ifndef SWIHOOK_H
 #define SWIHOOK_H

#define SHOWMSG_SWI_ID 0x148
#define MSGBOXERROR_SWI_ID 0x4E

 void SetSWIHook(int *swi_addr, int swi_id, void *func);
 void DestroySWIHook(int swi_addr, int swi_id);

#endif
