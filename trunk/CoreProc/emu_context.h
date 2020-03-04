#ifndef __SNIFFER_H__
#define __SNIFFER_H__

#include "arm_defs.h"

#pragma pack(1)
typedef struct
{
  unsigned int r0;
  unsigned int r1;
  unsigned int r2;
  unsigned int r3;
  unsigned int r4;
  unsigned int r5;
  unsigned int r6;
  unsigned int r7;
  unsigned int r8;
  unsigned int r9;
  unsigned int r10;
  unsigned int r11;
  unsigned int r12;
  unsigned int sp;
  unsigned int lr;
  unsigned int pc;
  unsigned int cpsr;
}REGISTERS;


typedef struct
{
    REGISTERS    s;
}CONTEXT;
#pragma pack()

#endif // __SNIFFER_H__

