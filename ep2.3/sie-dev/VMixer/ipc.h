#ifndef _IPC_H_
  #define _IPC_H_

#define IPC_MY_NAME "VMixerD"

void IPC_Send(const char *name_to, const char *name_from, void *data, const int submess);
  
#endif
