#ifndef _DNS_H_
#define _DNS_H_

#include "include.h"
#include "log.h"

#define DNR_SEND_TIMER

enum DNR_RESULT_MSG
{
  DNR_RESULT_OK,
  DNR_RESULT_ERROR,
  DNR_RESULT_OUT_OF_TRIES
};

class DNR
{
public:
  DNR();
  ~DNR();
  void Start(const char *host, int tries);
  void SendReq();

#ifdef DNR_SEND_TIMER
  static DNR * Top;
#endif
  virtual void onResolve(DNR_RESULT_MSG result_msg, int value) = 0; //� HELPER
  
  int DNR_ID;
  Log * log;
private:
  int DNR_TRIES;
  char * host;
#ifdef DNR_SEND_TIMER
  GBSTMR send_tmr;
#endif
};

class DNRHandler
{
public:
  void RegisterDNR(DNR *dnr);
  void DeleteDNR(DNR *dnr);
  DNR * GetDNR(int DNR_ID);
  
  static DNRHandler * Top;
  
  DNRHandler();
  ~DNRHandler();
  
private:
  struct DNRQ
  {
    DNR * dnr;
    DNRQ * next;
  };
  DNRQ *queue;
};
#endif
