//config.h


#ifdef SL65sw53

#define CUTHEAP                    //Support for Q-uu patch CutHeap

#define RAMFORPATCHADDR 0xA876FD00 //Free to the needs of the patch (16 bytes) to search the network
#define RAMJAVAHEAPSIZE 0xA87E4468 //At this address is the value JavaHeap (search from the end of RAM)
#define MAINBEARER      0xA0827F74 //General address of the transfer of all "BEARER" s

#define EEPROMBLOCKNUM  5304       //Block number EEPROM (EEFULL)
#define EEPROMOFFSET    0x3D       //His shift

#define ZEROMEM         0xA154A570 //Address functions zeromem () (look swilib.vkp)
#define STARTBROWSER    0xA0B3DA67 //Address functions StartNativeBrowserCore () (look swilib.vkp)

//§²§Ñ§Ù§Þ§Ö§ä§Ü§Ñ §°§©§µ (§¯§Ñ §â§Ñ§Ù§ß§í§ç §Þ§à§Õ§Ö§Ý§ñ§ç §ä§à§Ø§Ö §à§ä§Ý§Ú§é§Ñ§Ö§ä§î§ã§ñ, §ß§à SL65 = CX70)
//Partitioning of RAM (on different models too otlichaetsya, but SL65 = CX70)
#define DEF_TOTALSIZE   0x00652000 //Memory Size NonPermMem + PermMem, ie Total Size (Standard)
#define DEF_PERMSIZE    0x002FD000 //Memory Size PermMem (Standard)

#define MAX_TOTALSIZE   0x006A2000 //Maximum Total Size (up pickoff)
#define MIN_PERMSIZE    0x002A2AB0 //Minimum PermMem (up pickoff)

#define OPT1_PERMSIZE   0x00323160 // Size PermMem the optimal mode # 1
#define OPT2_PERMSIZE   0x0041D160 // Size PermMem the optimal mode # 2
#define OPT1_JAVAHEAP   0x001F4000 // Size JavaHeap the optimal mode # 1 (2 MB)
#define OPT2_JAVAHEAP   0x002EE000 // Size JavaHeap the optimal mode # 2 (3 MB)

#define MIN_PERMSIZE2   0x0011AB70 //Minimum PermMem, zero JavaHeap and killed Java - car (up to pickoff)

#define MAX_PERMSIZE    0x00583BB0 //Maximum size PermMem, when NonPermMem 1 MB
#define MAX_JAVAHEAP    0x00426800 //Maximum size JavaHeap, when MAX_PERMSIZE

#endif

#ifdef S65sw58

#define CUTHEAP
#define RAMFORPATCHADDR 0xA8??????
#define RAMJAVAHEAPSIZE 0xA8??????
#define MAINBEARER      0xA0827F74

#define EEPROMBLOCKNUM  5304
#define EEPROMOFFSET    0x3D

#define ZEROMEM         0xA166A944
#define STARTBROWSER    0xA0B6DF5B

#endif
