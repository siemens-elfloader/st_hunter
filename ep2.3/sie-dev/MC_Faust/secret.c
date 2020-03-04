#include "inc\color.h"
#include "inc\mc.h"
#include "inc\gui.h"
#include "inc\ScreenMake.h"
#include "inc\secret.h"
#include "inc\ColorMap.h"
#include "inc\mui.h"
#include "inc\mc.h"

int disks_opt=0, dicks_cur=0;

int disks_open[3];

void DoShowDisks()
{
 DoScreen();
 disks_opt=1;
 Busy1=1;
 disks_open[0]=show_hidden;
 disks_open[1]=show_system;
 disks_open[2]=show_hiddrv;
 dicks_cur=0;
}

void DisksClose(int mode)
{
  ClearScreen();
  Busy1=0;
  disks_opt=0;
  if (mode)
  {
    show_hidden=disks_open[0];
    show_system=disks_open[1];
    show_hiddrv=disks_open[2];
    Drives[DRV_IDX_Cache].enabled = show_hiddrv;
    Drives[DRV_IDX_Config].enabled = show_hiddrv;
  }
  DoRefresh();
}

void DoDisksKey(int key)
{
  switch(key)
  {
    case LEFT_SOFT:
      DisksClose(1);
    break;
    case RIGHT_SOFT:
      DisksClose(0);
    break;
    case ENTER_BUTTON:
      disks_open[dicks_cur]=!disks_open[dicks_cur];
      {
        if (dicks_cur==2) 
          dicks_cur=0;
        else
          dicks_cur++;
      }
    break;
    case UP_BUTTON:
      if (dicks_cur==0) 
        dicks_cur=2;
      else
        dicks_cur--;
    break;
    case DOWN_BUTTON:
      if (dicks_cur==2) 
        dicks_cur=0;
      else
        dicks_cur++;
    break;
  }
  
  if (key>='1' && key<='3')
    disks_open[key-1-'0']=!disks_open[key-1-'0'];
}

static DrawBox(int x, int y)
{  
  RECT box;
  box.x  = x+2;
  box.y  = y;
  box.x2 = box.x+prop_h-2;
  box.y2 = box.y+prop_h-2;

  int tc;
  for (int i=0; i<3; i++)
  {
    if (dicks_cur==i)
      tc=clCheckSel;
    else
      tc=clCheckBD;
    
    DrawRectangle(box.x, box.y, box.x2, box.y2,0, (char*)&Colors[tc],NULL);
    
    if(disks_open[i])
      DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                    (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
    
    box.y  += prop_h;
    box.y2 += prop_h;
  }
}

#ifdef ELKA
void DrawDisks()
{  
  RECT rc = DrawINF();
  
  wsprintf(guibuf, "%t:", muitxt(ind_viz));
  DrawString(guibuf,rc.x+smes,rc.y+3,rc.x2-smes,rc.y+prop_h+5,FONT_PROP,2,(char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, "%t:\n%t:\n%t:", muitxt(ind_hid_file), muitxt(ind_sys_file), muitxt(ind_hid_drv));
  DrawString(guibuf,
             rc.x+smes,
             rc.y+40,
             rc.x2-smes,
             rc.y2-5,
             FONT_PROP,1,(char*)&Colors[clInfo],NULL);

  DrawBox(rc.x2-(prop_h+smes), rc.y+7+prop_h/2+prop_h);
  
  wsprintf(guibuf, _t, muitxt(ind_ok));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, _t, muitxt(ind_cncl));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,4,(char*)&Colors[clInfo],NULL);
}
#else
void DrawDisks()
{
  RECT rc = DrawINF();
  
  wsprintf(guibuf, "%t:", muitxt(ind_viz));
  DrawString(guibuf,rc.x+smes,rc.y+5,rc.x2-smes,rc.y2-5,FONT_PROP,2,(char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, "%t:\n%t:\n%t:", muitxt(ind_hid_file), muitxt(ind_sys_file), muitxt(ind_hid_drv));
  DrawString(guibuf,rc.x+smes,rc.y+5+prop_h/2+prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,1,(char*)&Colors[clInfo],NULL);

  DrawBox(rc.x2-smes-prop_h,rc.y+7+prop_h/2+prop_h);

  wsprintf(guibuf, _t, muitxt(ind_ok));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,1,(char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_cncl));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,4,(char*)&Colors[clInfo],NULL);
}
#endif
