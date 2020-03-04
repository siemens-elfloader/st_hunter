#include "include.h"
#include "gui.h"




void gui_onredraw(MAIN_GUI *data)
{
    data->ab->onRedraw();
}

void gui_oncreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->gui.state=1;
  data->ab->onCreate();
}

void gui_onclose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
  data->ab->onClose();
}

void gui_onfocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  data->ab->onFocus();
}

void gui_onunfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2)
    return;
  data->gui.state=1;
  data->ab->onUnFocus();
}

int gui_onkey(MAIN_GUI *data, GUI_MSG *msg)
{
  return data->ab->onKey(msg->gbsmsg->submess, msg->gbsmsg->msg, msg->keys);
}

void gui_ondestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
}


void gui_method8(){
}

const void * const gui_methods[11]={
  (void *)gui_onredraw,
  (void *)gui_oncreate,
  (void *)gui_onclose,
  (void *)gui_onfocus,
  (void *)gui_onunfocus,
  (void *)gui_onkey,
  0,
  (void *)gui_ondestroy,
  (void *)gui_method8,
  (void *)gui_method8,
  0
};

//==========================   AbstractGUI   ========================

void AbstractGUI::Create()
{
  mg = new MAIN_GUI();
  zeromem(&mg->gui, sizeof(GUI));
  mg->ab = this;
  mg->gui.canvas = &Canvas;
  mg->gui.flag30 = 0;
  mg->gui.methods = (void *)gui_methods;
  mg->gui.item_ll.data_mfree = (void (*)(void *))mfree_adr();
  gui_id = CreateGUI(mg);
}


void AbstractGUI::Close()
{
  if (gui_id)
    GeneralFunc_flag1(gui_id, 0);

  delete this->mg;
}

AbstractGUI::AbstractGUI()
{
  gui_id = 0;
  StoreXYXYtoRECT(&Canvas, 0, YDISP, ScreenW()-1, ScreenH()-1);
}

AbstractGUI::~AbstractGUI()
{
  if (gui_id)
    Close();
}
