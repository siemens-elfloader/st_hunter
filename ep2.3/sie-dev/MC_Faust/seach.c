#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\gui.h"
#include "inc\newmenu.h"
#include "inc\color.h"
#include "inc\ColorMap.h"
#include "inc\zslib.h"
#include "inc\ScreenMake.h"
#include "inc\popup.h"

int isseach=0;
int seach=0;
char mask[64];
int disks[5];
int sis[2];
int scur=0;
int stype=1;

void wsregister(WSHDR *ws)
{
  for(int i=1; i<=ws->wsbody[0]; i++)
  {
    if(ws->wsbody[i] >= 0x410 && ws->wsbody[i] <= 0x42F) ws->wsbody[i]+=0x20;
    if(ws->wsbody[i] >= 0x41 && ws->wsbody[i] <= 0x5a) ws->wsbody[i]+=0x20;
  }
}

char *CompareStr(char* str1, char* find)
{
  str_2ws(wsbuf, str1, 256);
  wsregister(wsbuf);  
  ws2char8(wsbuf, msgbuf, 256, 2);
  
  return strstr(msgbuf,find);
}

int TypeSearch(char* file, char *ext)
{
  char* sz = GetFileExt(file);
  
  int sl2 = strlen(ext);
  for(int j=0; j<sl2; j++)
    if (ext[j] >= 'A' && ext[j] <= 'Z') ext[j] = ext[j] - 'A' + 'a';
  
  if ((int)sz)
  {
    char szext[MAX_EXT];
    strtolower(sz, szext, MAX_EXT);
    
    if (!strncmp(szext, ext, MAX_EXT))   
      return 1;
  }
  return 0;
}

int static ind=0;
int _SearchAdd(DIR_ENTRY *de, int param)
{
  int ignore=0;
  
  if((sis[1] || !(de->file_attr & FA_HIDDEN)) && (sis[0] || !(de->file_attr & FA_SYSTEM)))
  {
    switch(stype)
    {
      case 0:
        if(TypeSearch(de->file_name, mask)) ignore=1;
        break;
      case 1:
        if(CompareStr(de->file_name, mask))ignore=1;
        break;  
    }
  }
  
  if(ignore) AddFileFromDE(param, ind++, de);
  return 1;
}

int AddSearchFile(int tab)
{  
  str_2ws(wsbuf, mask, 64);
  wsregister(wsbuf);
  ws2char8(wsbuf, mask, 64, 2);
  ind=0;
  
  for(int ii=0; ii<4; ii++)
  {
    if(disks[ii]==1)EnumFiles(Drives[ii].path, _SearchAdd, tab);
  }
  
  if(disks[4]==1)EnumFiles(_CurPath, _SearchAdd, tab);
  
  if (ind==0)
  {
    isseach=0;
    DoShowMsg(muitxt(ind_err_search));
  }
  
  return ind;
}

void _ChangeMask(WSHDR *wsname)
{
  ws_2str(wsname, mask, 64);
}

void DoSeach()
{
  if (IsInArchive()) return; // потом надо добавить

  allclose();
  DoScreen();
  seach=1;
  Busy1=1;
  
  disks[0]=0;
  disks[1]=0;
  disks[2]=0;
  disks[3]=0;
  disks[4]=0;
  
  disks[_CurDrv]=1;
  
  sis[0]=0;
  sis[1]=0;
  
  strcpy(mask, _fullnull);
}

void DoSeachClose()
{
  ClearScreen();
  seach=0;
  Busy1=0;
}

void Seach_key(int key)
{
  switch(key)
  {
  case RIGHT_SOFT:
    DoSeachClose(); 
    break;
    
  case LEFT_SOFT:
    if (pathbuf && strlen(mask)>0)
    { 
      DoSeachClose();
      isseach=1;
      cdsys(Drives[1].path);
    }
    break;

  case(UP_BUTTON):
    if(scur==0) scur=8; else scur--;
    break;
  case(DOWN_BUTTON):
    if(scur!=8) scur++; else scur=0;
    break;
    
  case(ENTER_BUTTON):
    switch(scur)
    {
    case 0:
      str_2ws(wsbuf, mask, MAX_PATH);
      TextInput(muitxt(ind_name), muitxt(ind_name), 1, wsbuf, _ChangeMask);
      break;
    case 1:
      disks[0]=!disks[0];
      break;
    case 2:
      disks[1]=!disks[1];
      break;
    case 3:
      disks[2]=!disks[2];
      break;
    case 4:
      disks[3]=!disks[3];
      break;
    case 5:
      disks[4]=!disks[4];
      break;
    case 6:
      sis[0]=!sis[0];
      break;
    case 7:
      sis[1]=!sis[1];
      break;
    case 8:
      stype=!stype;
      break;
    }
    if (scur!=0)
      if(scur!=8) scur++; else scur=0;
    break;    
  }
}

void DrawSeach()
{  
  RECT rc = DrawPROP();

#ifdef ELKA  
  wsprintf(guibuf, "%t:", muitxt(ind_find)); 
  DrawString(guibuf, rc.x+smes, rc.y+3, rc.x2-smes, rc.y+prop_h+5, FONT_PROP, 2,
             (char*)&Colors[clInfo], NULL);

////////////////////////////////////////////////////////////////////////////////
  // Имя искомого файла
  
  RECT box;
  box.x  = rc.x+4;
  box.y  = rc.y+28;
  box.x2 = box.x+prop_h-2;
  box.y2 = box.y+prop_h-2;
  
  DrawRectangle(box.x, box.y, box.x2, box.y2,0, (char*)&Colors[scur==8?clCheckSel:clCheckBD],NULL);
  if(stype)
      DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                    (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
  
  wsprintf(guibuf, "%t/%t", muitxt(ind_name), muitxt(ind_type));
  DrawString(guibuf,rc.x+prop_h+smes,rc.y+27,rc.x2-smes,rc.y+prop_h+28,
             FONT_PROP,1+32,
             (char*)&Colors[clInfo],NULL);
  
  DrawRoundedFrame(rc.x+2, rc.y+29+prop_h, rc.x2-2, rc.y+prop_h*2+32,0,0,0,
                   (char*)&Colors[scur==0?clCheckSel:clCheckBD],(char*)&Colors[clCheckBG]);

  str_2ws(guibuf, mask, MAX_PATH);
  DrawString(guibuf,rc.x+3,rc.y+30+prop_h,rc.x2-3,rc.y+prop_h*2+30,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  // Диски
  wsprintf(guibuf, "%t:", muitxt(ind_drv_driver));
  DrawString(guibuf,rc.x+smes,rc.y+32+prop_h*2,rc.x2-smes,rc.y+prop_h*3+32,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, "%t\n%t\n%t\n%t\n%t", Drives[0].name, Drives[1].name, Drives[2].name, Drives[3].name, muitxt(ind_thisdir));  
  DrawString(guibuf,rc.x+prop_h+smes,rc.y+33+prop_h*3,rc.x2-smes,rc.y+prop_h*8+33,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  box.x  = rc.x+4;
  box.y  = rc.y+35+prop_h*3;
  box.x2 = box.x+prop_h-2;
  box.y2 = box.y+prop_h-2;
  
  for (int i=0; i<5; i++)
  {
    DrawRectangle(box.x, box.y, box.x2, box.y2,0, (char*)&Colors[scur==i+1?clCheckSel:clCheckBD],NULL);
    if(disks[i])
      DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                    (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
    
    box.y  += prop_h;
    box.y2 += prop_h;
  }
  //////////////////////////////////////////////////////////////////////////////
  
  wsprintf(guibuf, "%t\n%t", muitxt(ind_sys_type), muitxt(ind_hid_type));
  
  DrawString(guibuf,rc.x+(rc.x2-rc.x)/2+prop_h+6,rc.y+33+prop_h*3,rc.x2-4,rc.y+prop_h*8+33,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
 
  box.x  = rc.x+(rc.x2-rc.x)/2+4;
  box.y  = rc.y+35+prop_h*3;
  box.x2 = box.x+prop_h-2;
  box.y2 = box.y+prop_h-2;
  
  for (int i=0; i<2; i++)
  {
    DrawRectangle(box.x, box.y, box.x2, box.y2,0, (char*)&Colors[scur==i+6?clCheckSel:clCheckBD],NULL);
    if(sis[i])
      DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                    (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
    
    box.y  += prop_h;
    box.y2 += prop_h;
  }
  //////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  
  wsprintf(guibuf, _t, muitxt(ind_yes));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,1,
             (char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, _t, muitxt(ind_no));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,4,
             (char*)&Colors[clInfo],NULL);
#else
  // Имя искомого файла
  
  RECT box;
  box.x  = rc.x+4;
  box.y  = rc.y+3;
  box.x2 = box.x+prop_h-2;
  box.y2 = box.y+prop_h-2;
  
  DrawRectangle(box.x, box.y, box.x2, box.y2,0, (char*)&Colors[scur==8?clCheckSel:clCheckBD],NULL);
  if(stype)
      DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                    (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
  
  wsprintf(guibuf, "%t/%t", muitxt(ind_name), muitxt(ind_type));
  DrawString(guibuf,rc.x+prop_h+smes+3,rc.y+2,rc.x2-smes,rc.y+prop_h+3,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  DrawRoundedFrame(rc.x+2, rc.y+4+prop_h, rc.x2-2, rc.y+prop_h*2+6,0,0,0,
                   (char*)&Colors[scur==0?clCheckSel:clCheckBD],(char*)&Colors[clCheckBG]);
  str_2ws(guibuf, mask, MAX_PATH);
  DrawString(guibuf,rc.x+smes,rc.y+5+prop_h,rc.x2-smes,rc.y+prop_h*2+5,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  // Диски
  wsprintf(guibuf, "%t:", muitxt(ind_drv_driver));
  DrawString(guibuf,rc.x+smes,rc.y+7+prop_h*2,rc.x2-smes,rc.y+prop_h*3+7,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, "%t\n%t\n%t\n%t\n%t", Drives[0].name, Drives[1].name, Drives[2].name, Drives[3].name, muitxt(ind_thisdir));
  DrawString(guibuf,rc.x+prop_h+smes,rc.y+8+prop_h*3,rc.x2-smes,rc.y+prop_h*8+8,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
  
  box.x  = rc.x+4;
  box.y  = rc.y+9+prop_h*3;
  box.x2 = box.x+prop_h-2;
  box.y2 = box.y+prop_h-2;
  
  for (int i=0; i<5; i++)
  {
    DrawRectangle(box.x, box.y, box.x2, box.y2,0, (char*)&Colors[scur==i+1?clCheckSel:clCheckBD],NULL);
    if(disks[i])
      DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                    (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
    
    box.y  += prop_h;
    box.y2 += prop_h;
  }
  //////////////////////////////////////////////////////////////////////////////
  
  wsprintf(guibuf, "%t\n%t", muitxt(ind_sys_type), muitxt(ind_hid_type));
  DrawString(guibuf,rc.x+prop_h+smes,rc.y+8+prop_h*8,rc.x2-smes,rc.y+prop_h*12+9,
             FONT_PROP,1+32,(char*)&Colors[clInfo],NULL);
 
  box.x  = rc.x+4;
  box.y  = rc.y+9+prop_h*8;
  box.x2 = box.x+prop_h-2;
  box.y2 = box.y+prop_h-2;
  
  for (int i=0; i<2; i++)
  {
    DrawRectangle(box.x, box.y, box.x2, box.y2,0, (char*)&Colors[scur==i+6?clCheckSel:clCheckBD],NULL);
    if(sis[i])
      DrawRectangle(box.x+2, box.y+2, box.x2-2, box.y2-2,0, 
                    (char*)&Colors[clCheckEnter],(char*)&Colors[clCheckEnter]);
    
    box.y  += prop_h;
    box.y2 += prop_h;
  }
  
  wsprintf(guibuf, _t, muitxt(ind_yes));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,1,
             (char*)&Colors[clInfo],NULL);
  
  wsprintf(guibuf, _t, muitxt(ind_no));
  DrawString(guibuf,rc.x+smes,rc.y2-5-prop_h,rc.x2-smes,rc.y2-5,FONT_PROP,4,
             (char*)&Colors[clInfo],NULL);
#endif  

}


