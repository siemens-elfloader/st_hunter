#include "inc/mc.h"
#include <xtask_ipc.h>

IPC_REQ gipc;

void SetWallpaper(char *param)
{
  str_2ws(wsbuf, param,256);
  Registry_DeleteResourcePath(101,3);
  Registry_SetResourcePath(101,3,wsbuf);
  
  gipc.name_to=IPC_XTASK_NAME;
  gipc.name_from=IPC_XTASK_NAME;
  gipc.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
  
  REDRAW();
}

void SetLogo(char *param)
{
  str_2ws(wsbuf, param,256);
  Registry_DeleteResourcePath(102,3);
  Registry_SetResourcePath(102,3,wsbuf);
}

void SetScreenSaver(char *param)
{
  str_2ws(wsbuf, "Picture",12);
  Registry_DeleteResourcePath(111,3);
  Registry_SetResourcePath(111,3,wsbuf);
  
  str_2ws(wsbuf, param,256);
  Registry_DeleteResourcePath(110,3);  
  Registry_SetResourcePath(110,3,wsbuf);
}

void SetCall(char *param)
{
  str_2ws(wsbuf, param,256);
  Registry_DeleteResourcePath(201,3);
  Registry_SetResourcePath(201,3,wsbuf);
}

void SetMessages(char *param)
{
  str_2ws(wsbuf, param,256);
  Registry_DeleteResourcePath(213,3);
  Registry_SetResourcePath(213,3,wsbuf);
}

void SetAlarmClock(char *param)
{
  str_2ws(wsbuf, param,256);
  Registry_DeleteResourcePath(214,3);
  Registry_SetResourcePath(214,3,wsbuf);
}

void SetAppointments(char *param)
{
  str_2ws(wsbuf, param,256);
  Registry_DeleteResourcePath(212,3);
  Registry_SetResourcePath(212,3,wsbuf);
}

