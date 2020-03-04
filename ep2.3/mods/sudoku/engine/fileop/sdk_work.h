#include "..\inc\swilib.h"

#define GAME 0
#define CHECK 1
#define REAL 2

extern char sudokuname[32];
extern char level;
extern int tmr;

int sdk2cell(char c1,char c2,int flag)
 {
   int result;
   if (c1 == '0' && flag!=1)  for (int i=0x31;i<0x3A;i=i+0x01) if (c2 == i) result=i-0x30;
   if ((c1 == '1' && flag != 1) || ((c1 == '0' || c1 == '1')  && flag == 1))  for (int i=0x31;i<0x3A;i=i+0x01) if (c2 == i) result=i-0x26;
   if ((c1 == '2' && flag != 2) || ((c1 == '0' || c1 == '1' || c1 == '2')  && flag == 2))
    {
      if (flag!=2) {for (int i=0x31;i<0x3A;i=i+0x01) if (c2 == i) result=i-0x1C;}
      if (flag==2) {for (int i=0x31;i<0x3A;i=i+0x01) if (c2 == i) result=i-0x30;}
    } 
   return result;
}


void sdk_read(char sdk_path[])
{
 FSTATS stat;
 int k,ext,header = 0;
 unsigned int err;
 char temp[512];
 if (GetFileStats(sdk_path, &stat, &err)!=-1 && stat.size)
  {
    ext = fopen(sdk_path, A_ReadOnly, P_READ, &err);
    fread(ext, temp, stat.size, &err);
    fclose(ext, &err);
  }
 
 if (temp[0]=='S') header++;
 if (temp[1]=='U') header++;
 if (temp[2]=='D') header++;
 if (temp[3]=='O') header++;
 if (temp[4]=='K') header++;
 if (temp[5]=='U') header++;
 if (temp[6]=='1') header++;
 if (temp[7]=='0') header++;
 if (temp[8]==0x4C) {header++; level=0x4C;}
 if (temp[8]==0x4D) {header++; level=0x4D;}
 if (temp[8]==0x48) {header++; level=0x48;}
 if (temp[9]==0x0D) header++;
 if (temp[10]==0x0A) header++;
 
 
 
 for (int i = 11;temp[i]!=0x0A;i++) {sudokuname[i-11]=temp[i];k=i;}
 sudokuname[k-11]='\0';
 k=k+2;
 
 for (int i=0;i<9;i++) for (int j=0;j<9;j++) gcell[i][j]=sdk2cell(temp[k+j*2+(i*18)+(i*2)],temp[k+j*2+(i*18)+(i*2)+1],GAME);
 for (int i=0;i<9;i++) for (int j=0;j<9;j++) rcell[i][j]=sdk2cell(temp[k+j*2+(i*18)+(i*2)],temp[k+j*2+(i*18)+(i*2)+1],REAL);

 tmr = 0;
}

char *getSNbySDK(char sdk_path[],char sn[])
{
 FSTATS stat;
 int k,ext;
 unsigned int err;
 char temp[512];
 if (GetFileStats(sdk_path, &stat, &err)!=-1 && stat.size)
  {
    ext = fopen(sdk_path, A_ReadOnly, P_READ, &err);
    fread(ext, temp, stat.size, &err);
    fclose(ext, &err);
  }
 
 for (int i = 11;temp[i]!=0x0A;i++) {sn[i-11]=temp[i];k=i;}
 sn[k-11]='\0';
 return sn;

}

char getLEVbySDK(char sdk_path[])
{
 char s;
 FSTATS stat;
 int ext;
 unsigned int err;
 char temp[512];
 if (GetFileStats(sdk_path, &stat, &err)!=-1 && stat.size)
  {
    ext = fopen(sdk_path, A_ReadOnly, P_READ, &err);
    fread(ext, temp, stat.size, &err);
    fclose(ext, &err);
  }
 
 if (temp[8]==0x4C) s=0x4C;
 if (temp[8]==0x4D) s=0x4D;
 if (temp[8]==0x48) s=0x48;

 return s;

}
