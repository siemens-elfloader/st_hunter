#ifndef __IO_UI_H__
#define __IO_UI_H__


#include <alib/config.h>

#ifndef NUCLEUS

#include <string.h>
#include <alib/key.h>
#define ScreenH() 320
#define ScreenW() 240
#define strcmpi strcasecmp

#endif


//#include <swilib.h>


int GetFileDir (char *fname, char *buf);

void iLog (unsigned int i);
void Log (char *txt);

int str2int (char *str);

void RunAction (char *s);

class SimpleParser{
  char *buf_;
  int count_;
  int cur_;
public:
	SimpleParser ();
	~SimpleParser (){}

	int GetCount (){ return count_;}
	int GetCur (){ return cur_;}
    unsigned char GetCurCh ();

	int Open (char *fname);
	void Close ();

	int GoToSymb (char s);
	int CopyToSymb ( char *str,  char s);
};

#endif
