#include "include.h"
#include "main.h"
#include "csm.h"
#include "gui.h"
//#include "List.h"
#include "Menu.h"

extern "C"
{
  extern void kill_data(void *p, void (*func_p)(void *));
}

void Killer(void){
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

//---------------------------------------------------------------

class MyGUI: public AbstractGUI
{
  virtual void onRedraw();
  virtual void onCreate();
  virtual void onFocus();
  virtual void onUnFocus();
  virtual void onClose();
  virtual int  onKey(char key_code, int key_msg, short keys);
};

//extern GuiMenu *menu;
//extern CList *list;

const SET_MENU set={0,0,0,0,1,0};
const HEADER header = {NULL,0,NULL,"My menu"};
const SOFT_KEY softkey = {"Select","Close",NULL,0};

extern char clrBlack[];// = {0x00,0x00,0x00,0x64};
extern char clrWhite[];// = {0xFF,0xFF,0xFF,0x64};
extern char clrRed[];// = {0xFF,0x00,0x48,0x64};
extern char transparent[];// = {0x00,0x00,0x00,0x00};

extern const char fntSelect[4];//={0xFF,0xFF,0x00,0x60};
extern const char bordSelect[4];//={0x00,0x00,0x00,0x60};

extern const char clrNotSelect[4];//={0xFF,0x00,0x00,0x60};
extern const char bordNotSelect[4];//={0x00,0x00,0x00,0x60};
extern const char fntNotSelect[4];//={0xFF,0xFF,0xFF,0x60};

const COLOR clr=
 {
  (char*)fntSelect,
  (char*)bordSelect,
  (char*)fntNotSelect,
  (char*)clrNotSelect,
  (char*)bordNotSelect,
  (char*)CursorBGClr,
  (char*)CursorClr
 };  

const GUI_MENU mymenu =
 {  
   (SOFT_KEY*)&softkey,
   (HEADER*)&header,
   (SET_MENU*)&set,
   (COLOR*)&clr
 };

void MyGUI::onRedraw()
{/*
  WSHDR *ws = AllocWS(200);
  wsprintf(ws, "%t", "Hello World");
  DrawString(ws, Canvas.x, Canvas.y, Canvas.x2, Canvas.y2, FONT_LARGE, 0, GetPaletteAdrByColorIndex(0), NULL);
  FreeWS(ws);*/
  //menu->DrawList();
}

void MyGUI::onCreate()
{
  //list = new CList;
 // menu = new GuiMenu(mymenu);
}

void MyGUI::onFocus()
{
}

void MyGUI::onUnFocus()
{
}

void MyGUI::onClose()
{
  //delete list;
 // delete menu;
}

int MyGUI::onKey(char key_code, int key_msg, short keys)
{
  if (key_code==RIGHT_SOFT)
    return GUI_RESULT_CLOSE;
  
 /* switch(key_code)
  {
    case UP_BUTTON:   if(VIEW_MODE) menu->MoveUp(1);   else menu->MoveUp(0);   break;
    case DOWN_BUTTON: if(VIEW_MODE) menu->MoveDown(1); else menu->MoveDown(0); break;
    case LEFT_BUTTON: if(VIEW_MODE) menu->MoveUp(0);   else menu->MoveUp(1);   break;
    case RIGHT_BUTTON:if(VIEW_MODE) menu->MoveDown(0); else menu->MoveDown(1); break;
  default: break;
  }*/
  return GUI_RESULT_REDRAW;
}

//---------------------------------------------------------------


class MyDialogCSM: public DialogCSM
{
  virtual void onCreate();
  virtual int onMessage(GBS_MSG *msg);
  virtual void onClose();
  virtual ~MyDialogCSM();

  MyGUI *gui;
};

void MyDialogCSM::onCreate()
{
  SetName("AltMyMenu2_ND");
  gui = new MyGUI();
  gui->Create();
}

void MyDialogCSM::onClose()
{
  delete gui;
  delete this;
  SUBPROC((void *)Killer);
}

int MyDialogCSM::onMessage(GBS_MSG *msg)
{
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==gui->gui_id))
  {
    data->state=-3;
    gui->gui_id = 0;
  }
  return(1);
}

MyDialogCSM::~MyDialogCSM()
{
}

MyDialogCSM *csm;
char img_dir[128];
int main(char *exename, char *fname)
{
  strcpy(img_dir,fname);
  csm = new MyDialogCSM();
  csm->Create();
  return 0;
}
