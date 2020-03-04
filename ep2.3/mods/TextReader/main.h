#ifndef _MAIN_H_
  #define _MAIN_H_
#include "language.h"
extern void SaveFile(char *filename);
extern void pos();
extern int DrawText(char *filename,int cp);
extern int start(const char *f);
int atoi(char *attr);
extern void open_fm();
extern void CreateRootMenu();
extern void *about();
extern char *GetName(char *file,int type);

extern void ShowMainMenu(void);
extern int ShowMenuCode();
extern void ShowMenuGoTo(void);
extern void ShowMenuLast(void);
extern int LoadHist();
extern void new_file_ec();

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

#pragma inline
void patch_header2(const HEADER_DESC* headc)
{
  HEADER_DESC *head=(HEADER_DESC *)headc;
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}
#pragma inline
void patch_input2(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}
#endif

