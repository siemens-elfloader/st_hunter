#ifndef CSGUI_H
#define CSGUI_H

#include "CPP/gui.h"

class csGUI : public AbstractGUI
{
	public:
		csGUI();
		void redraw ();
		virtual void onRedraw();
		virtual void onCreate();
		virtual void onFocus();
		virtual void onUnFocus();
		virtual void onClose();
		virtual int  onKey(char key_code, int key_msg, short keys);
		virtual ~csGUI();
};

#endif // CSGUI_H
