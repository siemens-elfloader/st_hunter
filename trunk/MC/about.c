#include "inc/about.h"
#include "inc/mc.h"
#include "inc/mui.h"
#include "inc/lng.h"
#include "inc/gui.h"
#include "inc/ColorMap.h"
#include "inc/color.h"
#include "inc/ScreenMake.h"
#include "revision.h"
int showabout;

void DoAboutDlg()
{
 allclose();
 showabout=1;
 Busy1=1;
}

void DoAboutKey(int key)
{
  if (key==LEFT_SOFT || key==RIGHT_SOFT)
  {
    showabout=0;
    Busy1=0;
  }  
}

void _AboutDLG()
{
  DrawFon();  
  DrwSoftButton(muitxt(ind_ok), muitxt(ind_cncl));  
  wsprintf(guibuf, "MC v%t rev %d", version, ELF_REVISION);
  DrawString(guibuf,
             0,
             YDISP1+1,
             scr_w-1,
             TOP_Y+TOP_B+txt_h+5,
             FONT_MEDIUM,
             2+32,
             (char*)&Colors[clHeadTxt],
             (char*)&Colors[clHeadTxtBD]);
  
  wsprintf(guibuf, "%t",psz_autors);
  DrawString(guibuf,
             2,
             TOP_Y+TOP_B+txt_h+8,
             scr_w-2,
             scr_h1,
             FONT_SMALL,
             2+32,
             (char*)&Colors[clFileNormal],
             NULL);   
  
  wsprintf(guibuf, "%t:\n%t", "Сайт обновления","http://vados.at.ua/");
  DrawString(guibuf,
             2,
             scr_h1-txt_h*3-4,
             scr_w-2,
             scr_h1,
             FONT_SMALL,
             2+32,
             (char*)&Colors[clFileNormal],
             NULL);  
}
