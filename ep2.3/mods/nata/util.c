#include "../inc/swilib.h"

IMGHDR screen={0,0,8,""};
IMGHDR screen2={0,0,8,""};
/*
void DoScreen()
{
  int SCREENSIZE = ScreenH()*ScreenW();
//  int h=(icons[IS_ONLINE]->h > FontH(clfont)) ? (icons[IS_ONLINE]->h) : (FontH(clfont));
 // int hy=(icons[IS_ONLINE]->h > FontH(108)) ? (icons[IS_ONLINE]->h) : (FontH(108));
  int HSIZE = (hy+2)*ScreenW();
  int ScrH=ScreenH()-(hy+2);
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen.w=ScrW;
  screen.h=ScrH;
  screen.bitmap=malloc(ScrW*ScrH*2);
  ms+=(HSIZE*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
}

IMGHDR screen2={0,0,8,""};
void DoScreen2()
{
  int SCREENSIZE = ScreenH()*ScreenW();
  int HSIZE = (hy+2)*ScreenW();
  int ScrH=ScreenH()-(hy+2);
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen2.w=ScrW;
  screen2.h=ScrH;
  screen2.bitmap=malloc(ScrW*ScrH*2);
  ms+=(HSIZE*2);
  memcpy(screen2.bitmap, ms, ScrW*ScrH*2);
}
*/

IMGHDR DoScreen(int ScrH,int HSIZE)
{
  IMGHDR screen={0,0,8,""};
  //int HSIZE = (h+2)*w //*ScreenW();
//  int ScrH=ScreenH()-h;
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen.w=ScrW;
  screen.h=ScrH;
  char *bitmap=malloc(ScrW*ScrH*2);
  screen.bitmap=bitmap;
  ms+=(HSIZE*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
  mfree(bitmap);
  return screen;
}