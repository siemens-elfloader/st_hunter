#include "ColorMap.h"
#include "../../../inc/swilib.h"

char Colors[mccs_size];
unsigned int err;
extern char filename[128];

void SaveCS()
{
 int f;
 SetFileAttrib(filename, NULL, &err);
 if ((f=_open(filename,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
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

int LoadCS(char *byf)
{
 int res = 0;
 int f;
 if ((f=_open(byf,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
 {
  MCCS_hdr hdr;
  if (_read(f, &hdr, sizeof(hdr), &err)==sizeof(hdr))
  {
   if ((hdr.sig==mccs_sig) && (hdr.size>=mccs_size))
   {
    _lseek(f,hdr.offset,S_SET,&err,&err);
    res = _read(f, &Colors, sizeof(Colors), &err)==sizeof(Colors);
    res = 1;
   }
  }
  _close(f, &err);
 }
 return res;
}
