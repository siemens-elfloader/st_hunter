/*
  Проект SieGet Downloader
                          */
#include "include.h"
#include "socket.h"
#include "dns.h"
#include "icqsocket.h"
#include "mem.h"
#include "log_widget.h"
#include "log.h"
#include "langpack.h"

//Проверить процесс (сокеты только в хелпере)
inline int CheckCepId()
{
  if (GBS_GetCurCepid()==MMI_CEPID) return 1;
  return 0;
}

//---------------------------------------------------------------

SocketDNR::SocketDNR()
{
  Callback = NULL;
}

void SocketDNR::Bind(SocketAbstract * sock)
{
  Callback = sock;
  log = sock->log;
}

void SocketDNR::onResolve(DNR_RESULT_MSG result_msg, int value)
{
  if (Callback)
    Callback->onResolve(result_msg, value);
}

//---------------------------------------------------------------

SocketAbstract::SocketAbstract()
{
  ip = 0;
  port = 0;
  req_buf = NULL;
  recv_buf = NULL;

  Resolver = new SocketDNR();
  Resolver->Bind(this);
}

SocketAbstract::~SocketAbstract()
{

  _safe_delete(recv_buf);
  _safe_delete(req_buf);
  _safe_delete(Resolver);
}

void SocketAbstract::Start(int _ip, short _port)
{
  if (!IsGPRSEnabled())
  {
    log->Print(LangPack::Active->data[LGP_EnableGPRSFirst], CLR_Red);
    onError(SOCK_ERROR_CONNECTING);
    return;
  }
  _safe_delete(recv_buf);


  recv_buf = new Buffer();
  ip = _ip;
  port = _port;
  doConnect();
}

void SocketAbstract::Start(const char * host, short _port)
{
  if (!IsGPRSEnabled())
  {
    log->Print(LangPack::Active->data[LGP_EnableGPRSFirst], CLR_Red);
    onError(SOCK_ERROR_CONNECTING);
    return;
  }
  _safe_delete(recv_buf);

  recv_buf = new Buffer();
  ip = 0;
  port = _port;
  log->Print(LangPack::Active->data[LGP_ResolvingHost], CLR_Green);
  Resolver->Start(host, 3);
//  if (SieGetDialog::Active)
//    SieGetDialog::Active->RefreshList();
}

void _do_connect(SocketAbstract *obj)
{
  obj->doConnect();
}

void SocketAbstract::doConnect()
{
  if (CheckCepId())
    SUBPROC((void *)_do_connect, this);
  else
    Create();
}

void SocketAbstract::onResolve(DNR_RESULT_MSG result_msg, int value)
{
  char * msg = new char[64];
  sprintf(msg, "HttpAbstract::onResolve(%d, 0x%X)", result_msg, value);
  log->Print(msg, CLR_Yellow);

  switch(result_msg)
  {
  case DNR_RESULT_OK:
    ip = value;
 //   if (SieGetDialog::Active)
 //     SieGetDialog::Active->RefreshList();
    LogWidget::Active->Status="DNR Ok";
    LogWidget::Active->Redraw();
    doConnect();
    break;
  case DNR_RESULT_ERROR:
    sprintf(msg, LangPack::Active->data[LGP_DNRError], value);
    log->Print(msg, CLR_Red);
    LogWidget::Active->Status="DNR ERROR";
    LogWidget::Active->Redraw();
    onError(SOCK_ERROR_CONNECTING);
    break;
  case DNR_RESULT_OUT_OF_TRIES:
    log->Print(LangPack::Active->data[LGP_DNROutOfTries], CLR_Red);
    LogWidget::Active->Status="DNR_RESULT_OUT_OF_TRIES";
    LogWidget::Active->Redraw();
    onError(SOCK_ERROR_CONNECTING);
    break;
  }
  delete msg;
}

void SocketAbstract::onCreate()
{
  
  char * msg=new char[128];
  sprintf(msg, LangPack::Active->data[LGP_ConnectToIPPort], ip&0xff, (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff, port);
  log->Print(msg, CLR_Green);
  delete msg;
//  if (SieGetDialog::Active)
//    SieGetDialog::Active->RefreshList();
  Connect(ip, port);
}

void SocketAbstract::onConnected()
{
  
  Send(req_buf->data, req_buf->size);
  _safe_delete(req_buf);
//  onSocketConnect();
  LogWidget::Active->Status="Connect";
  LogWidget::Active->Redraw();
}

void SocketAbstract::onDataRead()
{
  // Проверяем статус закачки, чтобы не качать при остановленной закачке
 // if (download_state == DOWNLOAD_STOPPED || download_state == DOWNLOAD_ERROR)
 //   return;

//  char tmp_buf[4096]; // Буфер приема даннях
//  int nrecv = Recv(tmp_buf, sizeof(tmp_buf)); // Получаем даные
//  int hsize; // Длина HTTP заголовка
  /*
  switch(http_state)
  {
  case HTTP_HEADER: // Принимаем заголовок HTTP
//    download_state = DOWNLOAD_GET_INFO;
    recv_buf->Write(tmp_buf, nrecv);
  
    if(hsize = HTTPResponse->Parse(recv_buf->data, recv_buf->size)) // Обработали весь заголовок
    {
      char * tmp_msg = new char[128];
      sprintf(tmp_msg, LangPack::Active->data[LGP_HTTPParserReturned], hsize, recv_buf->size);
      log->Print(tmp_msg, CLR_Green);
      delete tmp_msg;
      
      if (!onHTTPHeaders()) // Обрабатываем заголовок на предмет редиректа и т.п.
        return;
      
      if (hsize < recv_buf->size) // Если есть данные помимо заголовка
        onHTTPData(recv_buf->data + hsize, recv_buf->size - hsize); // Пишем в файл
      
      http_state = HTTP_STREAM; // Дальше у нас идет чистый файл
    }
    break;
  case HTTP_STREAM: // Принимаем данные
    if(nrecv) onHTTPData(tmp_buf, nrecv);
    break;
  }
  if (SieGetDialog::Active)
    SieGetDialog::Active->RefreshList();
  */
}

void SocketAbstract::onClose()
{
//  if(download_state!=DOWNLOAD_STOPPED && download_state!=DOWNLOAD_ERROR && http_state!=HTTP_REDIRECT)
 //   onSocketFinish();
//  http_state = HTTP_IDLE;
}

void SocketAbstract::onRemoteClose()
{
  Close();
}

void SocketAbstract::onError(SOCK_ERROR err)
{
  switch(err)
  {
  case SOCK_ERROR_CREATING:
    log->Print(LangPack::Active->data[LGP_SocketCreateError], CLR_Red);
    break;
  case SOCK_ERROR_CONNECTING:
    log->Print(LangPack::Active->data[LGP_SocketConnectError], CLR_Red);
    break;
  case SOCK_ERROR_SENDING:
    log->Print(LangPack::Active->data[LGP_SocketSendError], CLR_Red);
    break;
  case SOCK_ERROR_CLOSING:
    log->Print(LangPack::Active->data[LGP_SocketCloseError], CLR_Red);
    break;
  case SOCK_ERROR_INVALID_SOCKET:
    log->Print(LangPack::Active->data[LGP_InvalidSocket], CLR_Red);
    break;
  }
/*  
  if(download_state != DOWNLOAD_STOPPED)
  {
    download_state = DOWNLOAD_ERROR;
    StartVibra();
    Play_Sound("$sounds\\error.wav");
  }
  
  if (SieGetDialog::Active)
    SieGetDialog::Active->RefreshList();
  */
  //SUBPROC((void *)_save_queue, DownloadHandler::Top);
}

//---------------------------------------------------------------
