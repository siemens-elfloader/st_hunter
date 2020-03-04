#ifndef _MAIN_H
#define _MAIN_H

#include "../inc/swilib.h"
#include "CPP/csm.h"
#include "CPP/gui.h"
#include "pbar.h"

extern "C"
{
    extern void kill_data(void *p, void (*func_p)(void *));
}

void CreateOpenCSM(void);

class MY_GUI: public AbstractGUI
{
public:
	virtual void onRedraw();
	virtual void onCreate();
	virtual void onFocus();
	virtual void onUnFocus();
	virtual void onClose();
	virtual int  onKey(char key_code, int key_msg, short keys);

	MY_GUI();
	~MY_GUI();
//  static QMSG_GUI * Active;
	void Redraw();
	void DrawIMG(IMGHDR *img, int x=0, int y=0, int x2=0, int y2=0);
private:
	int isFocused;
	PBar *bar[MAX_BAR];
	Rectangle *bg;
	int countbar;
	WSHDR *ws;
	IMGHDR *img;
};



class MY_CSM: public DialogCSM
{
public:
	virtual void onCreate();
	virtual int onMessage(GBS_MSG* msg);
	virtual void onClose();
	void Close();

	MY_CSM();
	virtual ~MY_CSM();
private:
	MY_GUI* gui;
};

class MY_DAEMON_CSM: public DaemonCSM
{
public:
	virtual void onCreate();
	virtual int onMessage(GBS_MSG* msg);
	virtual void onClose();
	void Close();

	MY_DAEMON_CSM();
	virtual ~MY_DAEMON_CSM();
};
#endif // MAIN_H

