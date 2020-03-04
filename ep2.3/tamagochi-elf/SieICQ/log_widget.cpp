
#include "include.h"
#include "rect_patcher.h"
#include "log_widget.h"
//#include "url_input.h"
#include "sieicqmain.h"
#include "langpack.h"



/*******************************************************************************
 LogWidget
*******************************************************************************/

char clLOGWIDGETBG  [4] = {0x0, 0x0, 0x0, 0x64};
char clLOGWIDGETTEXT[4] = {0xFF, 0xFF, 0xFF, 0x64};
char clLOGWIDGETSCROLLBAR[4] = {0xFF, 0xFF, 0xFF, 0x64};

LogWidget * LogWidget::Active = NULL;

void LogWidget::onRedraw()
{
  //DrawRectangle(Canvas.x, Canvas.y, Canvas.x2, Canvas.y2, 0, 0, clLOGWIDGETBG);
  DrawImg(0,  0, IconPack::Active->data[IMG_Logo]);
  
    WSHDR * ws = AllocWS(32);
    ascii2ws(ws, LangPack::Active->data[LGP_Close]);
    DrawString(ws, Canvas.x, Canvas.y2-20, Canvas.x2, Canvas.y2, FONT_SMALL, 0, clLOGWIDGETTEXT, 0);
    ascii2ws(ws, LangPack::Active->data[LGP_Close]);
    DrawString(ws, Canvas.x2-50, Canvas.y2-20, Canvas.x2, Canvas.y2, FONT_SMALL, 0, clLOGWIDGETTEXT, 0);
    ascii2ws(ws, Status);
    DrawString(ws, Canvas.x, Canvas.y+30, Canvas.x2, Canvas.y2, FONT_SMALL, 0, clLOGWIDGETTEXT, 0);
    char traf[64];
    sprintf(traf,"Traf %d/%d",Socket::GlobalTx,Socket::GlobalRx);
    ascii2ws(ws, traf);
    DrawString(ws, Canvas.x, Canvas.y+50, Canvas.x2, Canvas.y2, FONT_SMALL, 0, clLOGWIDGETTEXT, 0);
    FreeWS(ws);
 
}

void LogWidget::onCreate()
{

}

void LogWidget::onFocus()
{
  isFocused = 1;
}

void LogWidget::onUnFocus()
{
  isFocused = 0;
}

void LogWidget::onClose()
{
 // delete this;
}

int  LogWidget::onKey(char key_code, int key_msg, short keys)
{
  if (key_msg == KEY_DOWN || key_msg == LONG_PRESS)
  {
    switch (key_code)
    {
    case RED_BUTTON:
    case LEFT_SOFT:
      return GUI_RESULT_CLOSE;
    case RIGHT_SOFT:
      ContactList::Active->Show();
      break;
    }
  }
  return GUI_RESULT_OK;
}

void LogWidget::Redraw()
{
  if (isFocused)
    REDRAW();
}


LogWidget::LogWidget()
{
  Active = this;
  Status = new char[128];
}

LogWidget::~LogWidget()
{
  delete Status;
}
