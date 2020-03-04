#include "..\inc\swilib.h"
#include "rect_patcher.h"

#define UTF16_FONT_SMALL_BOLD (0xE013)

typedef struct
{
  GUI gui;
  WSHDR *ws;
  unsigned int ScanCode;
  unsigned int *ModeCode;
  char needexit;
}KEY_GUI;

void KOnReadraw(KEY_GUI *data)
{

  char black[4]={0x00,0x00,0x00,0x64};
  char white[4]={0xFF,0xFF,0xFF,0x64};
  char trans[4]={0x00,0x00,0x00,0x00};

  int scr_w=ScreenW();
  int scr_h=ScreenH();
  int fsize=GetFontYSIZE(FONT_SMALL)+1;
    DrawRectangle(0,YDISP,scr_w-1,scr_h-1,0,white,white);
    ////////////////////////////////////////////////////////////////////////////
    // tridog, 05.08.09 ////////////////////////////////////////////////////////
    // 翌麒?铌铠觐 恹犷疣 CFG_KEY 磬 咫赅?////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    if(data->needexit)
    {
      wsprintf(data->ws,"%t","按<确认>键接受.\n按<红键>或<左键>取消.");
      DrawString(data->ws,3,YDISP + 1 + fsize,scr_w-4,YDISP + fsize*5,FONT_SMALL,1,black,trans);
    }
    else
    {
      wsprintf(data->ws,"%t","按 #任意# 键");
      DrawString(data->ws,3,YDISP + 1 + fsize,scr_w-4,YDISP + fsize*5,FONT_SMALL,1,black,trans);
    }   
    wsprintf(data->ws,"HEX: %c%02X",UTF16_FONT_SMALL_BOLD,data->ScanCode);
    DrawString(data->ws,3,YDISP + 1 + fsize*4,scr_w-4,YDISP + fsize*5,FONT_SMALL,1,black,trans);    
    wsprintf(data->ws,"DEC: %c%u",UTF16_FONT_SMALL_BOLD,data->ScanCode);
    DrawString(data->ws,3,YDISP + 1 + fsize*5,scr_w-4,YDISP + fsize*6,FONT_SMALL,1,black,trans); 
    ////////////////////////////////////////////////////////////////////////////
}

void KOnCreate(KEY_GUI *data, void *(*malloc_adr)(int))
{
  data->ws=AllocWS(256);
  data->gui.state=1;
}

void KOnClose(KEY_GUI *data, void (*mfree_adr)(void *))
{
  FreeWS(data->ws);
  data->gui.state=0;
}

void KOnFocus(KEY_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA  
  DisableIconBar(1);
#endif  
  data->gui.state=2;
}

void KOnUnFocus(KEY_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int KOnKeyPress(KEY_GUI *data, GUI_MSG *msg)
{
  if ((msg->gbsmsg->msg==KEY_DOWN)||(msg->gbsmsg->msg==LONG_PRESS))
  {
    if (msg->gbsmsg->msg==KEY_DOWN)
    {
      if(data->needexit)
      {
       switch(msg->gbsmsg->submess)
       {
       case RED_BUTTON:
       case LEFT_SOFT: return (1);
       case ENTER_BUTTON:
         data->ModeCode[0] = data->ScanCode;
         return(1);
       }
       data->needexit = 0;
      } else
      data->needexit = 1;
    }
  }  
  data->ScanCode = msg->gbsmsg->submess;
  DirectRedrawGUI();
  return(0);
}

void KOnDestroy(KEY_GUI *data, void (*mfree_adr)(void *))
{
  extern void kill_data(void *p, void (*func_p)(void *));
  kill_data(data,mfree_adr);
}

int method8_key(void){return(0);}

int method9_key(void){return(0);}

const void * const kgui_methods[11]={
  (void *)KOnReadraw,//Redraw
  (void *)KOnCreate,	//Create
  (void *)KOnClose,	//Close
  (void *)KOnFocus,	//Focus
  (void *)KOnUnFocus,	//Unfocus
  (void *)KOnKeyPress,	//OnKey
  0,
  (void *)KOnDestroy,	//Destroy
  (void *)method8_key,
  (void *)method9_key,
  0
};

const RECT KCanvas={0,0,0,0};

void EditKeys(void *key)
{
  KEY_GUI *keygui=malloc(sizeof(KEY_GUI));
  zeromem(keygui,sizeof(KEY_GUI));
  unsigned int *modekode=key;
    keygui->ModeCode=key;
    keygui->ScanCode=modekode[0];
    keygui->needexit=0;
  patch_rect((RECT*)&KCanvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  keygui->gui.canvas=(void *)(&KCanvas);
  keygui->gui.flag30=2;
  keygui->gui.methods=(void *)kgui_methods;
  keygui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(keygui);
}

