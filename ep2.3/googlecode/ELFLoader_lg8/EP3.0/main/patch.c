#include "..\..\inc\swilib.h"
//#include "..\..\inc\add_swilib.h"

__arm char * Path_GetPatchByNum(int Num)
{
  return 0;
}

__arm char * Path_GetPatchByName(int Num)
{
  return 0;
}


#pragma diag_suppress=Pe177
__root static const int SWILIB_FUNC318 @ "SWILIB_FUNC300_310" = (int)Path_GetPatchByNum;
__root static const int SWILIB_FUNC319 @ "SWILIB_FUNC300_310" = (int)Path_GetPatchByName;
#pragma diag_default=Pe177
