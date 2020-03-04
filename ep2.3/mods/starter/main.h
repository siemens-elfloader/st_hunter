#ifndef _MAIN_H_
#define _MAIN_H_
typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

typedef struct
{
  void *next;
  char *filename;
  char *name;
  char *text;
  char *dat;
  int is_first;
  int is_pass;//1 есть пароль, 0 нет
}MNU;

int DrawText(int i,MNU *fl);
int create();
//void SaveMark(char *name,char *text);
void SaveMark(char *name,char *text,int ii);
void SavePass(WSHDR *ws);
//void GetPass();
void InputPass();

#pragma inline
void patch_header(HEADER_DESC* head)
{
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}

#pragma inline
void patch_input(INPUTDIA_DESC* inp)
{
  inp->rc.x=0;
  inp->rc.y=HeaderH()+1+YDISP;
  inp->rc.x2=ScreenW()-1;
  inp->rc.y2=ScreenH()-SoftkeyH()-1;
}

#endif
