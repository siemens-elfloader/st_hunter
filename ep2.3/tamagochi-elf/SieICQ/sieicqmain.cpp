/*******************************************************************************
  SieGetDaemon
*******************************************************************************/

#include "include.h"
#include "sieicqmain.h"
#include "conf_loader.h"
#include "icq.h"

/////////////////////////////////////////////
/////////////////////////////////////

char sieicq_ipc_name[] = SIEICQ_IPC_NAME;

void SieICQMain::onCreate()
{
  
 // Active = this;
  SetName(SIEICQ_CSM_NAME);
  
  Lipc.name_to = sieicq_ipc_name;
  Lipc.name_from = sieicq_ipc_name;
  Lipc.data = &tint_ipc;
  tint_ipc.code = IPC_DOUBLEREQ;
  tint_ipc.csm_id = csm_id;
  GBS_SendMessage(MMI_CEPID, MSG_IPC, IPC_SIEICQ_INTERNAL, &Lipc);
 
  status->Create();
  Client->Create();
  status->Status="SieICQ";
  sprintf(Client->Host,SieICQ_HOST);
  Client->Port=SieICQ_PORT;
  Client->UIN=UIN;
  sprintf(Client->PASS,PASS);
  Client->Login();
}

void MessageToHelper(SieICQMain * main, GBS_MSG * msg)
{
  main->onMessageHelper(msg);
  delete msg;
}

int SieICQMain::onMessage(GBS_MSG *msg)
{
  if (msg->msg==MSG_GUI_DESTROYED && (int)msg->data0==status->gui_id)
  {
    data->state=-3;
    Close();
  }
  
  if (msg->msg==MSG_IPC ) // IPC
  {
    IPC_REQ *ipc;
    if (ipc = (IPC_REQ *)msg->data0)
      // Обработка IPC сообщений
      if(!stricmp(ipc->name_to, sieicq_ipc_name))
        ProcessIPC(ipc->name_from, msg->submess, ipc->data);
  }
  if (msg->msg==MSG_HELPER_TRANSLATOR) // HELPER
  {
    GBS_MSG *tmp_msg = new GBS_MSG;
    *tmp_msg = *msg;
    SUBPROC((void *)MessageToHelper, this, tmp_msg);
  }
  if(msg->msg == MSG_RECONFIGURE_REQ) // CONFIG 
  {
    extern const char * successed_config_filename;
    if (!stricmp(successed_config_filename,(char *)msg->data0))
    {
      InitConfig(); // Обновляем конфиг
      lgp->Setup(); // Обновляем ленгпак. Глючит меню!!!
      icp->Setup(); // Обновляем иконки
    }
  }
  // Рисуем иконку на IDLE
  if (CFG_SHOW_IDLE_ICON)
  {
    CSM_RAM * data = FindCSMbyID(CSM_root()->idle_id);
    if(IsGuiOnTop(((int *)data)[DISPLACE_OF_IDLEGUI_ID/4]))
    {
      GUI * igui = GetTopGUI();
      if (igui)
      {
        void * CanvasData;
  #ifdef ELKA
        {
          CanvasData = BuildCanvas();
  #else
        void * idata = GetDataOfItemByID(igui,2);
        if (idata)
        {
          CanvasData = ((void **)idata)[DISPLACE_OF_IDLECANVAS / 4];
  #endif
          DrawCanvas(CanvasData , CFG_IDLE_ICON_X,  CFG_IDLE_ICON_Y, 
                 CFG_IDLE_ICON_X + GetImgWidth(IconPack::Active->data[IMG_OFFLINE]), 
                 CFG_IDLE_ICON_Y + GetImgHeight(IconPack::Active->data[IMG_OFFLINE]), 1);
          DrawImg(CFG_IDLE_ICON_X,  CFG_IDLE_ICON_Y, IconPack::Active->data[IMG_OFFLINE]);
        }
      }
    }
  }
  return CSM_MSG_RESULT_CONTINUE;
}

void SieICQMain::onMessageHelper(GBS_MSG * msg) // HELPER
{
  if((int)msg->data0==ENIP_DNR_HOST_BY_NAME)
    ProcessDNR((int)msg->data1); // DNR
  else
    ProcessSocket((int)msg->data1, (int)msg->data0); // Socket
}


void SieICQMain::ProcessIPC(const char * from, int submsg, void * data) // IPC
{


  if (submsg==IPC_SIEICQ_INTERNAL)
  {
    SIEICQ_INTERNAL_IPC *int_ipc;
    if (int_ipc = (SIEICQ_INTERNAL_IPC *)data)
    {
      switch(int_ipc->code)
      {
      case IPC_DOUBLEREQ:
        if (int_ipc->csm_id!=csm_id) // Не надо отвечать на свой запрос, а то от него другие дохнут
        {
          Lipc.name_to = sieicq_ipc_name;
          Lipc.name_from = sieicq_ipc_name;
          Lipc.data = &tint_ipc;
          tint_ipc.code = IPC_DOUBLERESP;
          tint_ipc.csm_id = csm_id;
          GBS_SendMessage(MMI_CEPID, MSG_IPC, IPC_SIEICQ_INTERNAL, &Lipc);

//          dialog->Show(NULL);
        }
        break;
      case IPC_DOUBLERESP:
        if (int_ipc->csm_id != csm_id) // Есть кто-то кроме нас
        {
          Close();
        }
        break;
      }
    }
  }

}

void SieICQMain::ProcessSocket(int id, int event)
{
  char * tmp_msg;
  //Download * download = NULL;
  //if (download = DL_Handler->GetDownloadbyID(id))
  if (Client->Socket->socket_id==id)
  {
    switch(event)
    {
    case ENIP_SOCK_CONNECTED: //Соединение через сокет установлено
//      download->log->Print(LangPack::Active->data[LGP_SocketConnected], CLR_Grey);
//      download->socket_state = SOCK_CONNECTED;
//      download->onConnected();
        status->Status="Connected";
        status->Redraw();
        Client->onConnected();
        _WriteLog("Connected");
      break;

    case ENIP_SOCK_DATA_READ: //Готовность данных к получению
        Client->onDataRead();
//        status->Status="ENIP_SOCK_DATA_READ";
//        status->Redraw();
//        _WriteLog("ENIP_SOCK_DATA_READ");
      break;

    case ENIP_SOCK_REMOTE_CLOSED: //Соединение разорвано сервером
//      download->log->Print(LangPack::Active->data[LGP_SocketRemoteClosed], CLR_Grey);
//      download->onRemoteClose();
        status->Status="ENIP_SOCK_REMOTE_CLOSED";
        status->Redraw();
        _WriteLog("ENIP_SOCK_REMOTE_CLOSED");
      break;

    case ENIP_SOCK_CLOSED: //Соединение разорвано клиентом
//      download->log->Print(LangPack::Active->data[LGP_SocketClosed], CLR_Grey);
//      download->socket_id = -1;
//      download->onClose();
        status->Status="ENIP_SOCK_CLOSED";
        status->Redraw();
        _WriteLog("ENIP_SOCK_CLOSED");
      break;

    case ENIP_BUFFER_FREE: //Буфер отпраки пуст
    case ENIP_BUFFER_FREE1:
      //Досылаем очередь
        Client->Socket->Send(NULL, NULL);
//      download->log->Print("Socket buffer free", CLR_Grey);
//      download->Send(NULL, NULL);
        status->Status="ENIP_BUFFER_FREE";
        status->Redraw();
        _WriteLog("ENIP_BUFFER_FREE");
      break;

    default:
      tmp_msg=new char[64];
      sprintf(tmp_msg, "Socket %d: [0x%X]", id, event);
//      download->log->Print(tmp_msg, CLR_Grey);
      status->Status=tmp_msg;
      status->Redraw();
      _WriteLog(tmp_msg);
      delete tmp_msg;
    }
  }
}

void SieICQMain::ProcessDNR(int DNR_ID)
{
//  DNR * dnr = NULL;
//  if(dnr = DNR_Handler->GetDNR(DNR_ID))
//      dnr->SendReq();
}

void SieICQMain::onClose()
{
  status->Close();
  Client->Close();
  delete this;
}

SieICQMain::SieICQMain()
{ 
  status = new LogWidget();
  CList = new ContactList();
  lgp = new LangPack;
  icp = new IconPack;
  Client = new ICQClient;
 
}

extern "C"
{
  extern void kill_data(void *p, void (*func_p)(void *));
}

void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

SieICQMain::~SieICQMain()
{
//  GBS_DelTimer(&VibraTimer);
//  delete status;
//  delete CList;
  delete lgp;
  delete icp;
  delete Client;
  SUBPROC((void *)Killer);
}

