#include "inc\mc.h"
#include "inc\mui.h"

int fwrite32(int fh, void *buf, int len, unsigned int *err){
  int clen, rlen, total=0;
  
  progrsp_start = 1;
  progrsp_max = len;
  incprogrsp(0);
  
  while(len && !progr_stop)
  {
    clen = len>16384 ? 16384 : len;
    total+=(rlen=fwrite(fh, buf, clen, err));
    if (rlen!=clen) break;
    buf=(char*)buf+rlen;
    len-=clen;
    incprogrsp(clen);
  }
  endprogrsp();
  return(total);
}

void GetFF()
{
  char fname[256];
  TDate date;
  TTime time;
  GetDateTime(&date, &time);
  sprintf(fname,"%s\\FF_%02d%02d%d_%02d%02d.bin",_CurPath, date.day, date.month, date.year,time.hour,time.min);
  int f,

#ifdef NEWSGOLD
  size=0x4000000;
#else
  size=0x2000000;
#endif  
  
  Busy = 1;
  initprogr(ind_rec);
  progr_max=1;
  incprogr(0);
  
  if(fname && (f=fopen(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))!=-1)
  {
    fwrite32(f,(char*)0xA0000000,size,&err);
    fclose(f,&err);
  }
  
  if(progr_stop) fsrm(fname, 0);
  
  DoRefresh();
  //Èשול ןונגûי פאיכ
  int ind = GetCurTabFileIndex(GetFileName(fname));
  SetCurTabIndex(ind, 0);
  
  endprogr();
  Busy = 0;
}

void DoGetFF()
{ 
  SUBPROC((void *)GetFF);
}
