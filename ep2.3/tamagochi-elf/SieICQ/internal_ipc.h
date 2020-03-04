#ifndef _INTERNAL_IPC_H_
#define _INTERNAL_IPC_H_

#include "sieicq_ipc.h"

// Запрос запущенной копии
enum IPC_CODE
{
  IPC_DOUBLEREQ,
  IPC_DOUBLERESP
};

#define IPC_REFRESH_LIST 1
#define IPC_SIEICQ_INTERNAL 0xFACE

typedef struct
{
  int csm_id;
  enum IPC_CODE code;
} SIEICQ_INTERNAL_IPC;

#endif
