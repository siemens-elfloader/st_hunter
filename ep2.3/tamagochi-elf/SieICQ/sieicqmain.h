#ifndef _SIEICQMAIN_H_
#define _SIEICQMAIN_H_

#include "include.h"
#include "csm.h"
#include "gui.h"
#include "sieicq_ipc.h"
#include "internal_ipc.h"
#include "log_widget.h"
#include "contact_list.h"

#include "icq.h"

//#include "socket.h"
//#include "dns.h"
//#include "inet.h"

#include "iconpack.h"
#include "langpack.h"


#define SIEICQ_CSM_NAME "SieICQ"

class SieICQMain: public DialogCSM
{
public:
  virtual void onCreate();
//  virtual void onRedraw();
  virtual int onMessage(GBS_MSG *msg);
  void onMessageHelper(GBS_MSG *msg);
  virtual void onClose();
  

  void ProcessIPC(const char *from, int submsg, void *data);
  void ProcessSocket(int id, int event); // HELPER
  void ProcessDNR(int DNR_ID); // HELPER

  SieICQMain();
  ~SieICQMain();
  
  LogWidget * status;
  ContactList * CList;
 // static SieICQMain * Active;

  //SieGetDialog * dialog;
  
  ICQClient *Client;
  
  LangPack * lgp;
  IconPack * icp;

//  DownloadHandler * DL_Handler;
//  DNRHandler * DNR_Handler;
  
private:
  IPC_REQ Lipc;  //Для внутренней связи
  IPC_REQ Xipc; // Для связи с XTASK
  SIEICQ_INTERNAL_IPC tint_ipc;
};

#endif
