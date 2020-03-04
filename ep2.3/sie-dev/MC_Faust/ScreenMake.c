#include "inc\ScreenMake.h"
#include "inc\mc.h"
#include "inc\gui.h"
#include "inc\ColorMap.h"
////////////////////////////////////////////////////////////////////////////////
IMGHDR screen={0,0,8,NULL};

void ClearScreen()
{
  if (screen.bitmap)
  {
    mfree(screen.bitmap);
    screen.bitmap=NULL;
    screen.w=0;
    screen.h=0;    
  }
}

void DoScreen()
{
  if(IsGuiOnTop(MAINGUI_ID))
  {
    ClearScreen();
    char *ms=RamScreenBuffer();
    screen.w=scr_w;
    screen.h=scr_h1;
    screen.bitmap=malloc(scr_w*scr_h1*2);
    memcpy(screen.bitmap, ms, scr_w*scr_h1*2);
  }
}

void DrwIMGHDR(IMGHDR * onse, int x, int y, int xRect, int yRect,int xxRect, int yyRect)
{  
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,xxRect,yyRect);
  SetProp2ImageOrCanvas(&drwobj, &rc, 0, onse, xRect, yRect);  
  SetColor(&drwobj,NULL,NULL);
  DrawObject(&drwobj);
}

void DrwImage(IMGHDR * onse, int x, int y, int xRect, int yRect, int xxRect, int yyRect, int isScreen)
{
  if(onse->h>=yyRect)
    DrwIMGHDR(onse, x, y, xRect, yRect, xxRect, yyRect);
  else
  {
    int ii=0;
    do
    {
      DrwIMGHDR(onse, x, y+ii, xRect, yRect, xxRect, yyRect-ii); 
      
      if((yyRect-ii)>=onse->h)
        ii+=onse->h;
      else
        ii+=(yyRect-ii);
    }while(yyRect-ii);
  }
 
  if(isScreen==1)DrawRectangle(x, y, x+xxRect-1, y+yyRect-1, 0,(char*)&Colors[clScreenBD],(char*)&Colors[clScreenBG]);
}

void DrawScreen()
{DrwImage(&screen, 0, YDISP1, 0, YDISP1, scr_w, scr_h1-YDISP1, 1);}
////////////////////////////////////////////////////////////////////////////////
