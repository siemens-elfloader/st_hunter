#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"
extern void InitConfig();
extern const unsigned int light0;
extern const unsigned int light1;
extern const unsigned int light2;
extern const unsigned int light3;
extern const unsigned int light4;
extern const unsigned int light5;
extern const unsigned int light6;
extern const unsigned int light7;
extern const unsigned int light8;
extern const unsigned int light9;
extern const unsigned int light10;

GBSTMR mytmr;
short light;
char *ram=(char*)0xA8000000;
long a;

void change(void)
{ //int cap=RamCap();
  if(RamCap()==100) {ram[a]=light0;SetIllumination(0,1,light0,0);}
  else if(RamCap()>=90 && RamCap()<=100) {ram[a]=light1;SetIllumination(0,1,light1,0);}
  else if(RamCap(>=80 && RamCap()<=90)  {ram[a]=light2;SetIllumination(0,1,light2,0);}
  else if(RamCap(>=70 && RamCap()<=80)  {ram[a]=light3;SetIllumination(0,1,light3,0);}
  else if(RamCap(>=60 && RamCap()<=70)  {ram[a]=light4;SetIllumination(0,1,light4,0);}
  else if(RamCap(>=50 && RamCap()<=60)  {ram[a]=light5;SetIllumination(0,1,light5,0);}
  else if(RamCap(>=40 && RamCap()<=50)  {ram[a]=light6;SetIllumination(0,1,light6,0);}
  else if(RamCap(>=30 && RamCap()<=40)  {ram[a]=light7;SetIllumination(0,1,light7,0);}
  else if(RamCap(>=20 && RamCap()<=30)  {ram[a]=light8;SetIllumination(0,1,light8,0);}
  else if(RamCap(>=10 && RamCap()<=20)  {ram[a]=light9;SetIllumination(0,1,light9,0);}
  else if(RamCap(<=10){ram[a]=light10;SetIllumination(0,1,light10,0);}
GBS_StartTimerProc(&mytmr,216*10,change);
}

int main(void)
{
  InitConfig();
//  ShowMSG(1,(int)"Start");
  unsigned char *l=(unsigned char*)0xA0FD435C; //Начальный указатель BFL(смещение)  
  a=l[2]*0x10000+l[1]*0x100+l[0];
  light=ram[a];
  change();
  return 0;
}
