#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "include.h"
#include "gui.h"


class LogWidget: public AbstractGUI
{
public:
  virtual void onRedraw();
  virtual void onCreate();
  virtual void onFocus();
  virtual void onUnFocus();
  virtual void onClose();
  virtual int  onKey(char key_code, int key_msg, short keys);

  LogWidget();
  ~LogWidget();
  
  char *Status;

  static LogWidget * Active;
  void Redraw();
  
private:
  int isFocused;
};

#endif

