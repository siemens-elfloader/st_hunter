#include "include.h"

#include "gui_mainmenu.h"
#include "icq.h"
#include "iconpack.h"


DATA_TIME begin_data_time;
SOFT_BUTTON_STRUCT begin_soft={3, 3, " ", "Отмена", 1, 0};

void SMART_REDRAW(void)
{
  //if (TYPE_DRAW == Draw_Connect)
  DirectRedrawGUI();
}

static void EndLoad()
{
  TYPE_DRAW = Draw_MainMenu;
  DirectRedrawGUI();
}

static void DrawBeginFon()
{
  if(TYPE_DRAW==Draw_Begin)EndLoad();
  
//  DrawRectangle(0,YDISP,ScrW-1,ScrH-1,0,
//		   GetPaletteAdrByColorIndex(1),
//		   GetPaletteAdrByColorIndex(1));
  DrawImg(0,0,IconPack[IMG_LOGO]);
}

///////// progressbar /////////////
int pm,pl;
char progress_msg[128];

void BeginStep(int max)
{
  pm=max;
  pl=0;
}

void NextStep(char * msg)
{
  if(pm > pl)
  ++pl;
  sprintf(progress_msg,msg);
  SMART_REDRAW();
}

int DoneConnected = 0;

void EndStep()
{
  pl=pm;
  DoneConnected = 1;
  void Init_C_List();
  TYPE_DRAW = Draw_C_List;
  SMART_REDRAW();
}


static void DrawProgressbar()
{
  
  if(pm != pl)
  {
     DrawRectangle(0,ScrH-4-GetFontYSIZE(FONT_SMALL)-GetFontYSIZE(FONT_MEDIUM_BOLD)-2,ScrW-1,ScrH-4-GetFontYSIZE(FONT_MEDIUM_BOLD),0,
                     GetPaletteAdrByColorIndex(0),
                     GetPaletteAdrByColorIndex(0));
    int pos_status = ((ScrW-1) * pl) / pm;
    DrawRectangle(1,ScrH-4-GetFontYSIZE(FONT_SMALL)-GetFontYSIZE(FONT_MEDIUM_BOLD)-1,pos_status,ScrH-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-1,0,
                     GetPaletteAdrByColorIndex(14),
                     GetPaletteAdrByColorIndex(14));  

    WSHDR *ws_info = AllocWS(128);
    wsprintf(ws_info,"%s",progress_msg);
    DrawString(ws_info,
               1,ScrH-4-GetFontYSIZE(FONT_SMALL)-GetFontYSIZE(FONT_MEDIUM_BOLD)-1,ScrW-2,ScrH-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-1,
	       FONT_SMALL, TEXT_ALIGNMIDDLE, COLOUR,0);
    FreeWS(ws_info);
  }

}
/////////////////////////////////////

static void CheckConnected()
{
  //if(ICQStatus!=STATUS_OFFLINE)
    //Quit_GUI_BEGIN=1;
}

static void DrawInfo()
{
  WSHDR *ws_info = AllocWS(128);
  
  unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED; //by BoBa 10.07
  
  DrawDataTime(&begin_data_time);
  wsprintf(ws_info,"State: %d, RXstate: %d\nRx:%db,Tx:%db\nQueue: %db\n%s\n%t",
           connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);

  DrawString(ws_info,3,60,ScrW-4,ScrH-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     FONT,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  
  
  DrawSoftButton(&begin_soft);
  
  FreeWS(ws_info);
}

void OnRedraw_Begin()
{
    CheckConnected();
    DrawBeginFon();
    DrawInfo();
    DrawProgressbar();
}



int MoveCursor_Begin(int pressed_mode, int key_kode)
{
  switch(pressed_mode)
  {
  case KEY_DOWN:
    {
      switch(key_kode)
      {
      case RIGHT_SOFT:Disconnect(); TYPE_DRAW = Draw_MainMenu;break;
      case LEFT_SOFT: TYPE_DRAW = Draw_MainMenu;break;
      }
    }
  }
  //DirectRedrawGUI();
  return 0;
}




void Init_Begin()
{
  InitDataTime(&begin_data_time, 20, 11, COLOUR, COLOUR_FRING);
}
                

