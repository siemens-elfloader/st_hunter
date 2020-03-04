#ifndef _MAIN_H_
#define _MAIN_H_
#include "../inc/swilib.h"
typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  int i1;
}MAIN_GUI;
/*
typedef struct
{
  void *next;
  char *filename;
  char *name;
  char *text;
  char *dat;
  int is_first;
}MNU;
*/
extern "C"
{
  extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
} 

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}



int create();
void SaveMark(char *name,char *text);
unsigned int char16to8(unsigned int c);
void win1251_2ws(WSHDR *ws, const char *s);
void ascii2ws(WSHDR *ws, const char *s);
void koi2ws(WSHDR *ws, const char *s);


#endif
