#include "inc/color.h"
#include "inc/mc.h"
#include "inc/ColorMap.h"

//Colors
#define clNULL 23
#define ITM_C_BG clWhite
#define ITM_C_BD (clBlue+clDark)
#define ITM_C_BGS (clBlue+clLight)
#define ITM_C_BDS clWhite

#define TC_NORMAL clBlack
#define TC_HIDDEN clGrey
#define TC_HIDDENS clDarkGrey
#define TC_READONLY (clRed+clDark)

#define DRV_C_BG (clBlue+clDark)
#define DRV_C_BD clWhite
#define DRV_C_BGS (clBlue+clLight)
#define DRV_C_BDS clWhite

#define PRG_C_BD clBlack
#define PRG_C_BG (clBlue+clDark)
#define PRGP_C_BG clRed

const char mccs_deffile[]="default.mccs";
const char mccs_ext[]="mccs";

int Colors[clMAX+1];
const int ColorIndex[clMAX+1] = 
{
  clBlack,
  clBlack,
  clWhite,
  clWhite,
  clBlack,
  clBlue,
  clBlack,
  clWhite,  
  clBlack,
  clWhite,  
  clBlue,
  clBlue,
  clNULL,
  clNULL,
  PRG_C_BD,
  PRG_C_BG,
  PRGP_C_BG,
  
  clBlue,
  clBlack,
  
  clBlack,
  clWhite,
  clBlack,
  PRGP_C_BG,
  
  TC_NORMAL,
  TC_NORMAL,
  TC_HIDDEN,
  TC_HIDDEN,
  TC_READONLY,
  TC_READONLY,
  clWhite,
  
  clWhite,
  clBlack,
  clWhite,  
  clBlack,
  
  clBlack,
  clWhite,
  
  clBlack
};

void InitCS()
{
  for(int ii=0;ii<=clMAX;ii++)
    nricp(GetPaletteAdrByColorIndex(ColorIndex[ii]), (char*)&Colors[ii]);
  
  if(CONFIG_LOAD_CS)
    LoadCS(NULL);
}

void SaveCS(char* filename)
{
 char* fn = filename?filename:MCFilePath(mccs_deffile);
 int f;
 if (fn && (f=_open(fn,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
 {
  MCCS_hdr hdr;
  hdr.sig=mccs_sig;
  hdr.ver=mccs_ver;
  hdr.size=mccs_size;
  hdr.offset=mccs_offset;
  _write(f,&hdr,sizeof(hdr),&err);
  _write(f,&Colors,sizeof(Colors),&err);
  _close(f,&err);
 }
}

int LoadCS(char* filename)
{
 int res = 0;
 char* fn = filename?filename:MCFilePath(mccs_deffile);
 int f;
 if (fn && (f=_open(fn,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
 {
  MCCS_hdr hdr;
  if (_read(f, &hdr, sizeof(hdr), &err)==sizeof(hdr))
  {
   if ((hdr.sig==mccs_sig) && (hdr.size>=mccs_size) && (hdr.ver==mccs_ver))
   {
    _lseek(f,hdr.offset,S_SET,&err,&err);
    res = _read(f, &Colors, sizeof(Colors), &err)==sizeof(Colors);
   }
  }
  _close(f, &err);
 }
 if (res && GUIStarted) REDRAW();
 return res;
}



