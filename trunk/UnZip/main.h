#ifndef __MAIN_H
#define __MAIN_H

#define WRITEBUFFERSIZE (32768) //Размер буфера для декода

#define STATE_NOTHING   0
#define STATE_OK    1
#define STATE_PROCESS    2
#define STATE_CANCEL    3

typedef struct
{
  GUI gui;
  WSHDR *ws1;
}MAIN_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

#endif
