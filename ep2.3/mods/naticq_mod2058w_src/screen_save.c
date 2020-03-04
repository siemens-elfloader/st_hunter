#include "../inc/swilib.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "main.h"
#include "rect_patcher.h"
#include "screen_save.h"
#include "strings.h"

typedef struct
{
  GUI gui;
  WSHDR *ws;
}SCREEN_GUI;

int battery;
int iks,igrek;
char laiks[32];
TTime time;
extern const unsigned int O_ED_FONT_SIZE;

void Draw()
{
  WSHDR *ws=AllocWS(32);
  GetDateTime(NULL, &time);
  wsprintf(ws, "%02d:%02d", time.hour, time.min);
  DrawString(ws,iks-10,igrek-40,iks-10+Get_WS_width(ws,O_ED_FONT_SIZE)+2,igrek-40+GetFontYSIZE(O_ED_FONT_SIZE),O_ED_FONT_SIZE,0,GetPaletteAdrByColorIndex(5),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

void Draw2()
{
  battery = *RamCap();
  iks=iks-10;
  igrek = igrek-7+GetFontYSIZE(11);
  WSHDR *ws=AllocWS(32);
  wsprintf(ws,"Bat:%d%", battery);
  DrawString(ws,iks,igrek,iks+Get_WS_width(ws,11)+2,igrek+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

extern int total_unread;
extern int total_unread2;

void Draw3()
{
  iks=iks-15;
  igrek = igrek-2+GetFontYSIZE(11);
  WSHDR *ws=AllocWS(32);
  wsprintf(ws, "Unread: %d/%d", total_unread, total_unread2);
  DrawString(ws,iks,igrek,iks+Get_WS_width(ws,11)+2,igrek+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

extern const unsigned int UIN;

void Draw5()
{
  WSHDR *ws=AllocWS(32);
  wsprintf(ws, "NatICQ: %d", UIN);
  DrawString(ws,40,40,40+Get_WS_width(ws,11)+2,40+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

//GBSTMR screen;
extern int saver_on;
extern int dace_unread;
extern const unsigned int color_SS;

void reftes()
{
  iks=95;
  igrek=150;
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  DrawRectangle(0,0,132,176,0,
                GetPaletteAdrByColorIndex(color_SS),
                GetPaletteAdrByColorIndex(color_SS)); 
  Draw();
  Draw2();
  Draw3();
  Draw5();
//  GBS_StartTimerProc(&screen, 216*10, reftes);
}

static void method0(SCREEN_GUI *data)
{
  
}

static void method1(SCREEN_GUI *data,void *(*malloc_adr)(int))
{
  data->ws=AllocWS(50);
  data->gui.state=1;
  DrawRectangle(0,0,132,176,0,
                GetPaletteAdrByColorIndex(color_SS),
                GetPaletteAdrByColorIndex(color_SS)); 
  reftes();
}

static void method2(SCREEN_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
  FreeWS(data->ws);
}

static void method3(SCREEN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))//fokus
{
//  char* iconbars = RamIconBar();
//  *iconbars = 0;
  saver_on=1;
  DisableIDLETMR();
  data->gui.state=2;
  reftes();
}

static void method4(SCREEN_GUI *data,void (*mfree_adr)(void *))//unfokus
{
//  char* iconbars = RamIconBar();
//  *iconbars = 1;
  saver_on=0;
//  GBS_DelTimer(&screen);
  if (data->gui.state!=2)
    return;
  data->gui.state=1;
}

static int method5(SCREEN_GUI *data,GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  int key=msg->gbsmsg->submess;
  /* if (m==KEY_DOWN)
  {
    if ((key==ENTER_BUTTON)||(key==0x18))
    { 
      return (1);
    }
  } */ //ubiraem nazhatie dzhojstika (w)
  if (m==LONG_PRESS)
    {
      if (key=='#')
    { 
      return (1);
    }
  }
  DirectRedrawGUI();
  return(0);
}

static int method8(void){return(0);}

static int method9(void){return(0);}

extern void kill_data(void *p, void (*func_p)(void *));
static const void * const gui_methods[11]={
  (void *)method0,  //Redraw
  (void *)method1,  //Create
  (void *)method2,  //Close
  (void *)method3,  //Focus
  (void *)method4,  //Unfocus
  (void *)method5,  //OnKey
  0,
  (void *)kill_data, //method7, //Destroy
  (void *)method8,
  (void *)method9,
  0
};

int CreateScreenSelectGUI()
{
  saver_on=1;
//  char* iconbars = RamIconBar();
//  *iconbars = 0;
  static const RECT Canvas={0,0,0,0};
  SCREEN_GUI *screen_gui=malloc(sizeof(SCREEN_GUI));
  zeromem(screen_gui,sizeof(SCREEN_GUI));
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  screen_gui->gui.canvas=(void *)(&Canvas);
  screen_gui->gui.methods=(void *)gui_methods;
  screen_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  return CreateGUI(screen_gui);
}
