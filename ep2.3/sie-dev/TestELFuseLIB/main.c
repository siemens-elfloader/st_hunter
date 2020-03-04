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


#include "testlib.h"



int main(void)
{
  if (EDL_use(TESTLIB_NAME, TESTLIB_VERSION, TESTLIB_FIRST) == 1)
   {
     testlib.message2();
   } else
      ShowMSG(1, (int)"Lib Error!");
  
  return 0x89785634;
}
