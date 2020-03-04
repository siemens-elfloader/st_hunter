#ifndef MAIN_H
#define MAIN_H

#include "../inc/swilib.h"
#include "CPP/csm.h"
#include "CPP/gui.h"

extern "C"
{
    extern void kill_data(void *p, void (*func_p)(void *));
}

void Killer(void);

class MY_CSM: public DaemonCSM
{
public:
	virtual void onCreate();
	virtual int onMessage(GBS_MSG* msg);
	virtual void onClose();
	void Close();

	MY_CSM();
	virtual ~MY_CSM();
};

#endif // MAIN_H

