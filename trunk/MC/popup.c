#include "inc/color.h"
#include "inc/mc.h"
#include "inc/gui.h"
#include "inc/ScreenMake.h"
#include "inc/popup.h"
#include "inc/ColorMap.h"
#include "inc/mui.h"
#include "inc/mc.h"
#include <xtask_ipc.h>

//--------------------Вопросительные окна---------------------------------------
int msgopen=0;
char txt[256];
void (*CallBack)(int);

void DoMsgBox(const char *lgpid, void CallFunc(int))
{
 if (!progr_start)
  DoScreen();
 allclose();
 msgopen=1;
 Busy1=1;
 strcpy(txt, lgpid);
 CallBack=CallFunc;

 if(!IsGuiOnTop(MAINGUI_ID) && IsUnlocked())
 {
   IPC_REQ ipc;
   ipc.name_to=IPC_XTASK_NAME;
   ipc.name_from=MYNAME;
   ipc.data=(void*)MAINGUI_ID;
   GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_SHOW_CSM,&ipc);
 }

 REDRAW();
}

void MsgBoxClose()
{
  if (!progr_start)
    ClearScreen();
  Busy1=0;
  msgopen=0;
}

void DoMsgBoxKey(int key)
{
  if (key==LEFT_SOFT || key==ENTER_BUTTON)
    if(CallBack!=NULL)CallBack(IDYES);

  if (key==LEFT_BUTTON)
    if(CallBack!=NULL)CallBack(IDYES2ALL);

  if (key==RIGHT_SOFT)
    if(CallBack!=NULL)CallBack(IDNO);

  if (key==RIGHT_BUTTON)
    if(CallBack!=NULL)CallBack(IDNO2ALL);

  if (key==RIGHT_SOFT ||
      key==LEFT_SOFT ||
      key==ENTER_BUTTON ||
      key==LEFT_BUTTON ||
      key==RIGHT_BUTTON)
    MsgBoxClose();

  REDRAW();
}
#ifdef ELKA

void DrawMsgBox()
{
  RECT rc = DrawINF();

  wsprintf(guibuf, "%t", muitxt(ind_support));
  DrawString(guibuf,rc.x+smes,rc.y+3,rc.x2-smes,rc.y+popup_h+5,FONT_MSG,2,
             (char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, txt);

  int count=GetStrCount(guibuf,iinfo->w-10, FONT_MSG);
  int yy=(iinfo->h-count*popup_h)/2-40-popup_h;

  DrawString(guibuf,rc.x+smes,rc.y+35+yy,rc.x2-smes,rc.y2-5,FONT_MSG,2,
             (char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_yes));
  DrawString(guibuf,rc.x+smes,rc.y2-5-popup_h,rc.x2-smes,rc.y2-5,FONT_MSG,1,(char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_no));
  DrawString(guibuf,rc.x+smes,rc.y2-5-popup_h,rc.x2-smes,rc.y2-5,FONT_MSG,4,(char*)&Colors[clInfo],NULL);

}

#else
void DrawMsgBox()
{
  RECT rc = DrawINF();

  wsprintf(guibuf, _t, txt);

  int count=GetStrCount(guibuf,iinfo->w-10, FONT_MSG);
  int yy=(iinfo->h-count*popup_h)/2-5-popup_h;

  DrawString(guibuf,rc.x+smes,rc.y+yy,rc.x2-smes,rc.y2-5,FONT_MSG,2,(char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_yes));
  DrawString(guibuf,rc.x+smes,rc.y2-5-popup_h,rc.x2-smes,rc.y2-5,FONT_MSG,1,(char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_no));
  DrawString(guibuf,rc.x+smes,rc.y2-5-popup_h,rc.x2-smes,rc.y2-5,FONT_MSG,4,(char*)&Colors[clInfo],NULL);

}
#endif
//------------------------------------------------------------------------------

//--------------------------------Информационные--------------------------------

int smsgopen=0;

void DoShowMsg(const char *lgpid)
{
 if (!progr_start)
  DoScreen();

 allclose();
 smsgopen=1;
 Busy1=1;
 strcpy(txt, lgpid);
 REDRAW();
}

void ShowMsgClose()
{
  if (!progr_start)
    ClearScreen();

  Busy1=0;
  smsgopen=0;
}

void DoShowMsgKey(int key)
{
  if (key==LEFT_SOFT || key==ENTER_BUTTON)
    ShowMsgClose();
}
#ifdef ELKA
void DrawShowMsg()
{
  RECT rc = DrawINF();

  wsprintf(guibuf, _t, txt);

  int count=GetStrCount(guibuf,iinfo->w-10, FONT_MSG);
  int yy=(iinfo->h-count*popup_h)/2-5-popup_h;

  DrawString(guibuf,rc.x+smes,rc.y+yy,rc.x2-smes,rc.y2-5,FONT_MSG,2,(char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_ok));
  DrawString(guibuf,rc.x+smes,rc.y2-5-popup_h,rc.x2-smes,rc.y2-5,FONT_MSG,1,(char*)&Colors[clInfo],NULL);
}
#else
void DrawShowMsg()
{
  RECT rc = DrawINF();

  wsprintf(guibuf, _t, txt);
  int count=GetStrCount(guibuf,iinfo->w-10, FONT_MSG);
  int yy=(iinfo->h-count*popup_h)/2-5-popup_h;

  DrawString(guibuf,rc.x+smes,rc.y+yy,rc.x2-smes,rc.y2-5,FONT_MSG,2,(char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_ok));
  DrawString(guibuf,rc.x+smes,rc.y2-5-popup_h,rc.x2-smes,rc.y2-5,FONT_MSG,1,(char*)&Colors[clInfo],NULL);

}
#endif
//------------------------------------------------------------------------------


