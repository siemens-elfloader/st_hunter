#ifndef _URL_INPUT_H_
#define _URL_INPUT_H_

#include "url.h"

class URLInput
{
public:
  void Show(const char * url_str, const char * ref_str);
  int onKey(GUI *gui, GUI_MSG *msg);
  void gHook(GUI *gui, int cmd);
private:
  int url_pos;
  int path_pos;
  int fname_pos;
  int referer_pos;
  int cookies_pos;
};

void input_options_select_bookmark(USR_MENU_ITEM *item);
void input_options_select_folder(USR_MENU_ITEM *item);

#endif
