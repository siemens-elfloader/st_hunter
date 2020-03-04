#include "..\inc\swilib.h"
#include "..\lib.h"

const int LibID = 5;

MESGExecStruct execparam;
MESGParamStruct *param;

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

void CloseLib()
{
  SUBPROC((void *)ElfKiller);   
}

void ShowMessage(const char *Message) // Показ сообщений
{
  ShowMSG(param->TypeMes,(int)Message);
}

int main(char *exe, MESGParamStruct *in)
{
  param = in;
  execparam.LibID = LibID;
  execparam.CloseLib = *CloseLib;
  execparam.ShowMessage = *ShowMessage;
  param->Struct = &execparam;
  return (0);
}
