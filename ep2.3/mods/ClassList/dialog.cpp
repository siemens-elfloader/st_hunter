#include "dialog.h"
#include "..\inc\swilib.h"
#include "../inc/xtask_ipc.h"

#define IPC_REFRESH_LIST 1

char xtask_ipc_name[]= IPC_XTASK_NAME;

char my_ipc_name[]="Marks";

MainDialog* MainDialog::Active = NULL;

void MainDialog::onCreate()
{
  Active = this;
  SetName(DIALOG_CSM_NAME);
  list->Show();
}

void MainDialog::Close()
{
  delete list;
  CloseCSM(csm_id);
}

void MainDialog::Show()
{
  if (!csm_id) Create();
  else // Иначе посылаем сообщение в XTask для получения диалогом фокуса
  {
    Xipc.name_to = xtask_ipc_name;
    Xipc.name_from = my_ipc_name;
    Xipc.data = (void *)csm_id;
    GBS_SendMessage(MMI_CEPID, MSG_IPC, IPC_XTASK_SHOW_CSM, &Xipc);
  }
}

int MainDialog::onMessage(GBS_MSG * msg)
{
  if (msg->msg==MSG_GUI_DESTROYED && (int)msg->data0 == list->gui_id)
  {
    data->state=-3;
    list->gui_id = NULL;
    Close();
  }
  
  if (msg->msg==MSG_IPC )
  {
    IPC_REQ *ipc;
    if (ipc = (IPC_REQ *)msg->data0)
      // Обработка IPC сообщений
      if(strcmp(ipc->name_to, my_ipc_name)==0)
        ProcessIPC(ipc->name_from, msg->submess, ipc->data);
  }
  return CSM_MSG_RESULT_CONTINUE;
}

void MainDialog::ProcessIPC(const char * from, int submsg, void * data)
{
  switch(submsg)
  {
  case IPC_REFRESH_LIST:
    if (list->gui_id)
    {
      if (IsGuiOnTop(list->gui_id))
        RefreshGUI(); // Это нужно запускать из диалога. Иначе - пикофф :(
    }
    break;
  }
}

void MainDialog::RefreshList()
{
  if (!list->gui_id) return;
  if (!IsGuiOnTop(list->gui_id)) return;
  Lipc.name_to = my_ipc_name;
  Lipc.name_from = my_ipc_name;
  Lipc.data = NULL;
  GBS_SendMessage(MMI_CEPID, MSG_IPC, IPC_REFRESH_LIST, &Lipc);
}

void MainDialog::onClose()
{
}

MainDialog::MainDialog()
{

}

MainDialog::~MainDialog()
{

}

