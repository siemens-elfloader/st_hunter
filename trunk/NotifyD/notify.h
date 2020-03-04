#ifndef _NOTIFY_H_
  #define _NOTIFY_H_

#define IPC_NAME_TO "NotifyD"
#define IPC_NAME_FROM "NotifyD"

void ShowMsgOk(int flag, unsigned int lgp_id);
void ShowMsgError(int flag, unsigned int lgp_id);

#endif
