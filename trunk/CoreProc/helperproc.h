
#ifndef __HELPERPROC_H__
#define __HELPERPROC_H__




void helperproc_schedule(void *func, void *d1, void *d2, void *d3);


#define _R0        0
#define _R1        1
#define _R2        2
#define _R3        3
#define _R4        4
#define _R5        5
#define _R6        6
#define _R7        7
#define _R8        8
#define _R9        9
#define _R10       10
#define _R11       11
#define _R12       12
#define _SP        13
#define _LR        14
#define _PC        15
#define _CPSR      16

#define MUSR           0x10 // User Mode
#define MFIQ           0x11 // FIQ Mode
#define MIRQ           0x12 // IRQ Mode
#define MSVC           0x13 // Supervisor Mode
#define MABT           0x17 // Abort Mode
#define MUND           0x1B // Undefined Mode
#define MSYS           0x1F // System Mode
#define MMSK           0x1F // Mask Mode

#endif
