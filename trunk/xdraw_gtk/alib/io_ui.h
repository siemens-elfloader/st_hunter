#ifndef __IO_UI_H__
#define __IO_UI_H__

#include <swilib.h>

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
	SimpleParser (){buf_=NULL; count_=0; cur_=0;}
	~SimpleParser (){}

	int GetCount (){ return count_;}
	int GetCur (){ return cur_;}
    unsigned char GetCurCh (){ cur_++; return buf_[cur_-1];}

	int Open (char *fname);
	void Close ();

	int GoToSymb (char s);
	int CopyToSymb ( char *str,  char s);
};

#endif
