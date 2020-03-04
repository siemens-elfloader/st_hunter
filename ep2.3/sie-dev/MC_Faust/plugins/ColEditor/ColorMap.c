#include "ColorMap.h"
#include "../../../inc/swilib.h"

char Colors[mccs_size];
unsigned int err;
extern char filename[128];

void SaveCS()
{
 int f;
 SetFileAttrib(filename, NULL, &err);
 if ((f=fopen(filename,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
 {
  MCCS_hdr hdr;
  hdr.sig=mccs_sig;
  hdr.ver=mccs_ver;
  hdr.size=mccs_size;
  hdr.offset=mccs_offset;
  fwrite(f,&hdr,sizeof(hdr),&err);
  fwrite(f,&Colors,sizeof(Colors),&err);
  fclose(f,&err);
 }
}

int LoadCS(char *byf)
{
 int res = 0;
 int f;
 if ((f=fopen(byf,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
 {
  MCCS_hdr hdr;
  if (fread(f, &hdr, sizeof(hdr), &err)==sizeof(hdr))
  {
   if ((hdr.sig==mccs_sig) && (hdr.size>=mccs_size))
   {
    lseek(f,hdr.offset,S_SET,&err,&err);
    res = fread(f, &Colors, sizeof(Colors), &err)==sizeof(Colors);
    res = 1;
   }
  }
  fclose(f, &err);
 }
 return res;
}
