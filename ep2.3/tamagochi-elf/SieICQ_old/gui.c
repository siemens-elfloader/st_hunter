#include "include.h"

#include "gui.h"

#include "status_change.h"
#include "xstatus_change.h"
#include "c_list_util.h"
#include "mainmenu.h"
#include "gui_begin.h"
               



void OnRedraw()
{
  switch(TYPE_DRAW)
  {
  case Draw_Begin:
  case Draw_Connect:
    OnRedraw_Begin();
    break;
    
  case Draw_StatusChange:
    OnRedraw_StatusChange();
    break;

  case Draw_XStatusChange:
    OnRedraw_XStatusChange();
    break;
    
  case Draw_MainMenu:
    OnRedraw_MainMenu();
    break;
    
  case Draw_C_List:
    OnRedraw_C_List();
    break;
    
  }
}

int OnKey(int pressed_mode, int key_kode)
{
  int pm = pressed_mode;
  int kk = key_kode;
  
  switch(TYPE_DRAW)
  {
    
  case Draw_Begin:
  case Draw_Connect:
    MoveCursor_Begin(pm, kk);
    break;
  
  case Draw_StatusChange:
    MoveCursor_StatusChange(pm, kk);
    break;

  case Draw_XStatusChange:
    MoveCursor_XStatusChange(pm, kk);
    break;
      
  case Draw_MainMenu:
    MoveCursor_MainMenu(pm, kk);
    break;
      
  case Draw_C_List:
    MoveCursor_C_List(pm, kk);
    break;
    
  }
  
  //DirectRedrawGUI();
  return 0;
}

void InitData()
{
  Init_Status_Change();
  Init_XStatus_Change();
  Init_MainMenu();
  Init_C_List();
}


