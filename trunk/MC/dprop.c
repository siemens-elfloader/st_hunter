#include "inc/mc.h"
#include "inc/mui.h"
#include "inc/newmenu.h"
#include "inc/gui.h"
#include "inc/ColorMap.h"
#include "inc/ScreenMake.h"
#include "inc/siemens_unicode.h"

int drwinfo=0;
int drv;
void DoDrvInf()
{
  allclose();
  DoScreen();
  drwinfo=1;
  Busy1=1;
  drv=_CurDrv;
  if (drv==3) drv=4;
}

void DoDrvIn_close()
{
  ClearScreen();
  drwinfo=0;
  Busy1=0;
}

void DrvPlus()
{
  if (show_hiddrv)
  {
    if ((drv < MAX_TABS) || (CONFIG_SHOW_MMC && drv==2))
    {drv++; if (drv==3) drv++;}
    else
      drv=0;
  }else
  {if (drv!=0) drv=0; else if (CONFIG_SHOW_MMC) drv=4;}
}

void DrvMinus()
{
  if (show_hiddrv)
  {
    if (drv > 0)
    {
      drv--;
      if (drv==3)
        drv--;
    }
    else
    {
      if (CONFIG_SHOW_MMC)
        drv=4;
      else
        drv=MAX_TABS;
    }
  }else
  {
    if (drv!=0)
      drv=0;
    else
      if (CONFIG_SHOW_MMC)
        drv=4;
  }
}

void DPROP_key(int key)
{
  switch(key)
  {
  case LEFT_BUTTON:
    DrvMinus();
    break;
  case RIGHT_BUTTON:
    DrvPlus();
    break;
  case ENTER_BUTTON:
  case RIGHT_SOFT:
  case LEFT_SOFT:
    DoDrvIn_close();
    break;
  }
}

void DrvInfo()
{
  unsigned int iTotal, iFree, iBusy;
  float fFree;
  char szTotal[18];//Полный обьем диска
  char szFree[18];//Свободно
  char szBusy[18];//Занято
  char szPFree[8];//Свободно в процентах
  int drv1; //диск

  if (drv==4)
    drv1=drv-1;
  else
    drv1=drv;

  iTotal = GetTotalFlexSpace(drv, &err);//Всего
  iFree = GetFreeFlexSpace(drv, &err);//Свободно
  iBusy = iTotal - iFree;
  fFree = (float)iFree * 100 / (float)iTotal;//Свободно в процентах

  sz2s(iTotal, szTotal);
  sz2s(iFree,  szFree );
  sz2s(iBusy,  szBusy );
  sprintf(szPFree, "%.2f%%", fFree);

  RECT rc = DrawINF();
  unsigned int total=(rc.x2-rc.x-2-smes*2)*fFree/100;

#ifdef ELKA
  wsprintf(guibuf, "%t", muitxt(ind_drv_info));
  DrawString(guibuf, rc.x+smes, rc.y+3, rc.x2-smes, rc.y+prop_h+5, FONT_PROP, 2+32,
             (char*)&Colors[clInfo],NULL);

  wsprintf(guibuf, _t, muitxt(ind_ok));
  DrawString(guibuf,rc.x+smes,rc.y2-5-GetFontYSIZE(FONT_PROP),rc.x2-smes,rc.y2-5,FONT_PROP,1,(char*)&Colors[clInfo],NULL);
#endif

  wsprintf(guibuf, "%t: %t (%d:)\n%t: %t\n%t: %t\n%t: %t\n%c%t%c", muitxt(ind_drv_driver),Drives[drv1].name, drv
                                                         ,muitxt(ind_drv_all),szTotal
                                                         ,muitxt(ind_drv_free),szFree
                                                         ,muitxt(ind_drv_busy),szBusy
                                                         ,UTF16_DIS_CENTER, szPFree, UTF16_ENA_CENTER);

  DrawRectangle(rc.x+smes,
                rc.y+SDVIG_H-3+(prop_h+1)*4,
                rc.x2-smes,
                rc.y+SDVIG_H-3+(prop_h+1)*4+prop_h,
                0,
                (char*)&Colors[clProgrBD],( char*)&Colors[clProgrBG]);
  DrawRectangle(rc.x+smes+1,
                rc.y+SDVIG_H-3+(prop_h+1)*4+1,
                rc.x+smes+1+total,
                rc.y+SDVIG_H-3+(prop_h+1)*4+prop_h-1,
                0,
                NULL, (char*)&Colors[clCurProgrBG]);

    DrawString(guibuf,
             rc.x+smes,
             rc.y+SDVIG_H,
             rc.x2-smes,
             rc.y+prop_h*5+SDVIG_H,
             FONT_PROP,
             1+32,
             (char*)&Colors[clInfo],
             NULL);

  DrawImg(
#ifdef ELKA
          rc.x2-GetImgWidth((int)&Drives[drv1].icon)-8,
#else
          rc.x2-GetImgWidth((int)&Drives[drv1].icon)-2,
#endif
          rc.y+SDVIG_H-2, (int)&Drives[drv1].icon);
}
