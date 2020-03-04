#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\lng.h"
#include "inc\gui.h"
#include "inc\ScreenMake.h"
#include "inc\color.h"
#include "inc\ColorMap.h"
#include "inc\popup.h"

#ifdef NEWSGOLD
ZPARAMS zpp;
int s_arh_pos;
int arch;
char spass[32],sarhname[32];
int level;

typedef struct
{
  int level;
  int name;
}COMP;

COMP cmp[]=
{
  {Z_NO_COMPRESSION, ind_no_compres},
  {Z_BEST_SPEED, ind_hi_speed},
  {Z_DEFAULT_COMPRESSION, ind_default},
  {Z_BEST_COMPRESSION, ind_hi_compress}
};

void _NewArchive()
{ 
  sprintf(zpp.destname, _s_s,_CurPath,sarhname);
  strcat(zpp.destname, ".zip");
  
  strcpy(zpp.password,spass);
  zpp.level = level;
  SUBPROC((void*)CreateZip, &zpp);
}

int arh_cur=0;

void _Change(WSHDR *wsname)
{
  if (arh_cur==0)
    ws_2str(wsname, sarhname, 32);
  if (arh_cur==1)
    ws2char8(wsname, spass, 32, 1);
}

void DoArchive()
{
  if (IsInArchive()) return;
  
  if (buffer.count==0)
    DoCopy();
  
  if (buffer.count==0)    
    return;

  allclose();  
  DoScreen();
  arch=1;
  arh_cur=0;
  Busy1=1;
  level=3;
  
  FN_ITM* itm = buffer.items;
  FN_ITM* last_itm = itm;
  while(itm)
  {
    last_itm = itm;
    itm = itm->next;
  }
  
  if(buffer.count==1 || itm->ftype == TYPE_COMMON_DIR)
    strcpy(sarhname,last_itm->full + last_itm->pname);
  else
  {
    if(strlen(_CurPath)!=2)
    {
      char *name=strrchr(_CurPath,'\\')+1;
      strcpy(sarhname,name);
    }
    else
      strcpy(sarhname,"Archive");
  }
  
  strcpy(spass,_fullnull);
}

void DoArchClose()
{
  ClearScreen();
  arch=0;
  Busy1=0;
}

void Arch_key(int key)
{
  switch(key)
  {
  case RIGHT_SOFT:
    DoArchClose();
    break;
    
  case LEFT_SOFT:
    DoArchClose();
    _NewArchive();
    break;
    
  case ENTER_BUTTON:
    if (arh_cur==0)
    {
      str_2ws(wsbuf, sarhname, MAX_PATH);
      TextInput(muitxt(ind_arh_name), muitxt(ind_name), 1, wsbuf, _Change);
    }
    if (arh_cur==1)
    {
      str_2ws(wsbuf, spass, MAX_PATH);
      TextInput(muitxt(ind_pass), muitxt(ind_pass), 0, wsbuf, _Change);
    }
    break;
    
  case UP_BUTTON:
    arh_cur=(arh_cur?arh_cur--:2);
    break;
    
  case DOWN_BUTTON:
    arh_cur=(arh_cur!=2?arh_cur++:0);
    break;
    
  case LEFT_BUTTON:
    if(arh_cur==2)
      level=(level?level--:3);
    break;
    
  case RIGHT_BUTTON:
    if(arh_cur==2)
      level=(level!=3?level++:0);
    break;
  }
}

void DrawArch()
{  
#ifdef ELKA
#define SDV 0
#else
#define SDV 7
#endif
  RECT rc = DrawPROP();
  
  wsprintf(guibuf, _t, muitxt(ind_do_arch)); 
  
  DrawString(guibuf, rc.x, rc.y+3, rc.x2, rc.y+prop_h+5, FONT_PROP, 2,
             (char*)&Colors[clInfo], NULL);

  wsprintf(guibuf, "%t:", muitxt(ind_name));
  DrawString(guibuf,rc.x+smes,rc.y+SDVIG_H+SDV,rc.x2-smes,rc.y+prop_h+SDVIG_H+SDV,
             FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  
  DrawRoundedFrame(rc.x+2, rc.y+SDVIG_H+1+prop_h+SDV, rc.x2-2, rc.y+prop_h*2+SDVIG_H+3+SDV,0,0,0,
                   (char*)&Colors[arh_cur==0?clCheckSel:clCheckBD],(char*)&Colors[clCheckBG]);

  str_2ws(guibuf, sarhname, MAX_PATH);
  DrawScrollString(guibuf,rc.x+smes+1,rc.y+SDVIG_H+2+prop_h+SDV,rc.x2-smes-1,rc.y+prop_h*2+SDVIG_H+2+SDV,1,
             FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, "%t:", muitxt(ind_pass));
  DrawString(guibuf,rc.x+smes+1,rc.y+SDVIG_H+5+prop_h*2+SDV,rc.x2-smes-1,rc.y+prop_h*3+SDVIG_H+5+SDV,
             FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  
  DrawRoundedFrame(rc.x+2, rc.y+SDVIG_H+8+prop_h*3+SDV, rc.x2-2,rc.y+prop_h*4+SDVIG_H+8+SDV,
                   0,0,0,(char*)&Colors[arh_cur==1?clCheckSel:clCheckBD],(char*)&Colors[clCheckBG]);
  
  str_2ws(guibuf, spass, MAX_PATH);  
  DrawString(guibuf,rc.x+smes,rc.y+SDVIG_H+10+prop_h*3+SDV,rc.x2-smes,rc.y+prop_h*4+SDVIG_H+10+SDV,
             FONT_PROP,1+64,(char*)&Colors[clInfo],NULL);
  
  //////////////////////////////////////////////////////////////////////////////
  wsprintf(guibuf, "%t:", muitxt(ind_met_compres));
  DrawString(guibuf,rc.x+smes,rc.y+SDVIG_H+10+prop_h*4+SDV,rc.x2-smes,rc.y+prop_h*5+SDVIG_H+10+SDV,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  DrawRoundedFrame(rc.x+2, rc.y+SDVIG_H+12+prop_h*5+SDV, rc.x2-2,rc.y+prop_h*6+SDVIG_H+14+SDV,
                   0,0,0,(char*)&Colors[arh_cur==2?clCheckSel:clCheckBD],(char*)&Colors[clCheckBG]);
  wsprintf(guibuf, "%t", muitxt(cmp[level].name));
  DrawScrollString(guibuf,rc.x+smes+1,rc.y+SDVIG_H+12+prop_h*5+SDV,rc.x2-smes-1,rc.y+prop_h*6+SDVIG_H+12+SDV,1,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  //////////////////////////////////////////////////////////////////////////////
    
  wsprintf(guibuf, _t, muitxt(ind_yes));
  DrawString(guibuf,rc.x+smes,rc.y2-4-prop_h,rc.x2-smes,rc.y2-2,FONT_PROP,1,(char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, _t, muitxt(ind_no));
  DrawString(guibuf,rc.x+smes,rc.y2-4-prop_h,rc.x2-smes,rc.y2-4,FONT_PROP,4,(char*)&Colors[clInfo],NULL);
}
#endif

