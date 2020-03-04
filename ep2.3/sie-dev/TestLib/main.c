#include "..\inc\swilib.h"

#pragma swi_number=0x8300
__swi __arm  int isSGoldX75();

#pragma swi_number=0x0301
__swi __arm  int EDL_load(char *fname);

#pragma swi_number=0x0302
__swi __arm  int EDL_unload(char *name, short version);

#pragma swi_number=0x0303
__swi __arm  int EDL_addlib(void *edl_struct);

#pragma swi_number=0x0304
__swi __arm  int EDL_remove(void *edl_struct);

#pragma swi_number=0x0305
__swi __arm  int EDL_use(char *name, short version, void *addr_pnt_first_func);

#pragma swi_number=0x0306
__swi __arm  int isEDL_exist();

#pragma swi_number=0x0307
__swi __arm  int isEDL_support();

#pragma swi_number=0x0308
__swi __arm void *getEDL_baseaddr();

#pragma swi_number=0x0309
__swi __arm  int getEDL_curLibs();

#pragma swi_number=0x0310
__swi __arm  int getEDL_maxLibs();

/////////////////////////////////////////////////////////////////////////////////////

//IDs for EDL file lib
#define EDL_ID1  0xDDDDEEEE
#define EDL_ID2  0x33212133
#define EDL_ID3  0xEEEEDDDD

//Small phone list
#define EDL_PHONE_NONE        "NONE"  // Не важно (Not important)

#define EDL_PHONE_SGX65_C65   "C65"   // C65
#define EDL_PHONE_SGX65_SL65  "SL65"  // SL65
#define EDL_PHONE_SGX65_M65   "M65"   // M65
#define EDL_PHONE_SGX65_CX65  "CX65"  // CX65
#define EDL_PHONE_SGX65_CX70  "CX70"  // CX70
#define EDL_PHONE_SGX65_SK65  "SK65"  // SK65
#define EDL_PHONE_SGX65_S65   "S65"   // S65

#define EDL_PHONE_SGX75_C72   "C72"   // C72
#define EDL_PHONE_SGX75_CF75  "CF75"  // CF75
#define EDL_PHONE_SGX75_ME75  "ME75"  // ME75
#define EDL_PHONE_SGX75_C75   "C75"   // C75
#define EDL_PHONE_SGX75_M75   "M75"   // M75
#define EDL_PHONE_SGX75_CX75  "CX75"  // CX75

#define EDL_PHONE_NSG_S68     "S68"   // S68
#define EDL_PHONE_NSG_C81     "C81"   // C81
#define EDL_PHONE_NSG_M81     "M81"   // M81
#define EDL_PHONE_NSG_SL75    "SL75"  // SL75
#define EDL_PHONE_NSG_S75     "S75"   // S75

#define EDL_PHONE_ELKA_EL71   "EL71"  // EL71
#define EDL_PHONE_ELKA_E71    "E71"   // E71

//Platform list
#define EDL_PLATFORM_NONE     0x00    // Не важно (Not important)
#define EDL_PLATFORM_SGX65    0x01    // SGold X65
#define EDL_PLATFORM_SGX75    0x02    // SGold X75
#define EDL_PLATFORM_NSG      0x03    // NewSGold
#define EDL_PLATFORM_ELKA     0x04    // NewSGold E/EL71 (ELKA)

//Firmware
#define EDL_FIRMWARE_NONE     0x00    // Не важно (Not important)

////////////////////////////////////////////////////////////////////////////////////////

#define TESTLIB_NAME    "TestLib"
#define TESTLIB_VERSION 0x0001
#define TESTLIB_COUNT   3

typedef struct {
  int   id1;
  int   id2;
  int   id3;
  char  name[32];
  short version;
  short count;
  char  platform;  //0 - Не важно, 1 - SGold X65, 2 - SGold X75, 3 - NewSGold, 4 - NewSGold E/EL71
  char  phone[7];  //0 - Не важно или Mодель телефона
  short sw;        //0 - Не важно или прошивка 
  void (*killproc)();
  //Functions:
  void (*m0)();
  void (*m1)();
  void (*m2)();
}EDL_STRUCT;



void EDLUnload()
{
  extern void *ELF_BEGIN;
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


void m1()
{
 ShowMSG(1, (int)"TestLib!");
}

void m2()
{
 ShowMSG(1, (int)"Work!");
}

void m3()
{
 ShowMSG(1, (int)"You use EDL lib!");
}


EDL_STRUCT edl=
{
 EDL_ID1,
 EDL_ID2,
 EDL_ID3,
 TESTLIB_NAME,
 TESTLIB_VERSION,
 TESTLIB_COUNT,
 EDL_PLATFORM_NONE,
 EDL_PHONE_NONE,
 EDL_FIRMWARE_NONE,
 EDLUnload,
 m1,m2,m3
};




int main(void)
{
  EDL_addlib((void *)&edl);

  return 0;
}
