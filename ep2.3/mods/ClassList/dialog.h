#ifndef _DIALOG_H_
#define _DIALOG_H_
#include "..\inc\swilib.h"
#include "csm.h"
#include "mlist.h"
#include "list.h"

#define DIALOG_CSM_NAME "kMarks"

class MainDialog: public DialogCSM
{
public:
  virtual void onCreate();
  virtual int onMessage(GBS_MSG *msg);
  virtual void onClose();
  void Show();
  void ProcessIPC(const char * from, int submsg, void * data);
  void Close();
  
  MainDialog();
  ~MainDialog();
  static MainDialog * Active;
  
  StandartList *list;
  void RefreshList();
private:
  IPC_REQ Xipc; // Для связи с XTASK
  IPC_REQ Lipc;

};

#endif
