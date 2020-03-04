#include "..\inc\swilib.h"

#define GAME 0
#define CHECK 1

extern char sudokuname[32];
extern char level;
extern char cfgfolder[256];

extern int rcell[9][9];
extern int gcell[9][9];

extern char timesave[5][20];
extern char loadname[5][32];


char buffer[512];
char sbuffer[8];
extern char timegame[6];
extern int tmr;


int char2int(char c)
 {
   int result;   
   for (int i=0x30;i<0x3A;i=i+0x01) if (c == i) result=i-0x30;
   return result; 
 }

int twocharToint(char c1,char c2)
 {
   int result;
   if (c1 == '0') for (int i=0x31;i<0x3A;i=i+0x01) if (c2 == i) result=i-0x30;
   if (c1 == '1') for (int i=0x31;i<0x3A;i=i+0x01) if (c2 == i) result=i-0x26; 
   if (c1 == '2') for (int i=0x31;i<0x3A;i=i+0x01) if (c2 == i) result=i-0x1C;
   return result;
}


char intTochar(int n)
{
 char result;
 for (int i=0x01;i<0x0A;i=i+0x01) if (n == i) result=i+0x30;
 return result;
}

void intToTWOchars(int n)
{
 for (int i=1;i<30;i++) if (n == i) sprintf(sbuffer,"%02d",i);
}


int checkgame(int n)
{
  char savefile[128];
  FSTATS stat;
  unsigned int err;
  sprintf(savefile,"%s%s_%02d%s",cfgfolder,"SuDoKu",n,".save");
  if (GetFileStats(savefile, &stat, &err)!=-1 && stat.size) return 1;else return 0;
}

void getLoadName(int n)
{
  char buf[512],savefile[128];
  FSTATS stat;
  int sav;
  unsigned int err;
  sprintf(savefile,"%s%s_%02d%s",cfgfolder,"SuDoKu",n,".save");
  if (GetFileStats(savefile, &stat, &err)!=-1 && stat.size)
  {
   sav = fopen(savefile, A_ReadOnly, P_READ, &err);
   fread(sav, buf, stat.size, &err);
   fclose(sav, &err);
   
    for (int i=278;i<stat.size;i++) {loadname[n-1][i-278] = buf[i];}//str[stat.size-270]='\0';}
  }

}

void getTimeSave(int n)
{
  char buf[512],savefile[128];
  FSTATS stat;
  int sav;
  unsigned int err;
  sprintf(savefile,"%s%s_%02d%s",cfgfolder,"SuDoKu",n,".save");
  if (GetFileStats(savefile, &stat, &err)!=-1 && stat.size)
  {
   sav = fopen(savefile, A_ReadOnly, P_READ, &err);
   fread(sav, buf, stat.size, &err);
   fclose(sav, &err);
   
    for (int i=0;i<16;i++) timesave[n-1][i]=buf[i+261];
  }

}

void getTSLN()//(int s,int e)
{
 int s=1,e=5;
 for (int i=s;i<(e+1);i++)
 {
  getLoadName(i);
  getTimeSave(i);
 }
}



void savegame(int n)
{
 char td[20],savefile[128];
 int size;
 for (int i=0;i<9;i++) for (int j=0;j<9;j++) buffer[i*10+j] = intTochar(rcell[i][j]);
 for (int i=0;i<9;i++) for (int j=0;j<9;j++) {intToTWOchars(gcell[i][j]); buffer[90+j*2+i*18]=sbuffer[0];buffer[90+j*2+i*18+1]=sbuffer[1];}
 for (int i=0;i<6;i++) buffer[253+i]=timegame[i];
 buffer[259]=level;
 
 TDate date; TTime time; 
 GetDateTime(&date,&time);
 sprintf(td,"%02d:%02d %02d.%02d.%04d",time.hour,time.min,date.day,date.month,date.year);
 
 for (int i=0;i<16;i++) buffer[i+261]=td[i];
 for (int i=0;i<strlen(sudokuname);i++) buffer[i+278]=sudokuname[i];
 
 size = 278 + strlen(sudokuname);
 
 int sav;
 unsigned int err;
 sprintf(savefile,"%s%s_%02d%s",cfgfolder,"SuDoKu",n,".save");
 unlink(savefile,&err); 
 sav = fopen (savefile,A_WriteOnly+A_Create,P_WRITE,&err);
 fwrite(sav, buffer, size, &err);
 fclose(sav, &err);
 
}

void loadgame(int n)
{
  char savefile[128];
  FSTATS stat;
  int sav;
  unsigned int err;
  sprintf(savefile,"%s%s_%02d%s",cfgfolder,"SuDoKu",n,".save");
  if (GetFileStats(savefile, &stat, &err)!=-1 && stat.size)
  {
   sav = fopen(savefile, A_ReadOnly, P_READ, &err);
   fread(sav, buffer, stat.size, &err);
   fclose(sav, &err);
  }
   for (int i=0;i<9;i++) for (int j=0;j<9;j++) rcell[i][j] = char2int(buffer[i*10+j]);
   for (int i=0;i<9;i++) for (int j=0;j<9;j++) gcell[i][j] = twocharToint(buffer[90+j*2+i*18],buffer[90+j*2+i*18+1]);
  
  tmr = char2int(buffer[253])*10000 + char2int(buffer[254])*1000 + char2int(buffer[255])*100 + char2int(buffer[256])*10 +char2int(buffer[257]);
 
  
 if (buffer[259]==0x4C) level='L';
 if (buffer[259]==0x4D) level='M';
 if (buffer[259]==0x48) level='H';

 for (int i=278;i<stat.size;i++) {sudokuname[i-278] = buffer[i];sudokuname[stat.size-278]='\0';}

}

char *loadhelp(char *buff)
{
  char helpfile[128];
  FSTATS stat;
  int helpf;
  unsigned int err;
  sprintf(helpfile,"%s%s%s",cfgfolder,"SuDoKu",".hlp");
  if (GetFileStats(helpfile, &stat, &err)!=-1 && stat.size)
  {
   helpf = fopen(helpfile, A_ReadOnly, P_READ, &err);
   fread(helpf, buff, stat.size, &err);
   fclose(helpf, &err);
  }
 return buff;
}

int checkhelp()
{
  char helpfile[128];
  FSTATS stat;
  unsigned int err;
  sprintf(helpfile,"%s%s%s",cfgfolder,"SuDoKu",".hlp");
  if (GetFileStats(helpfile, &stat, &err)!=-1 && stat.size) return 1;else return 0;
}

void fileext(char *path,char *filebody,int size)
{
 FSTATS stat;
 int file;
 unsigned int err;
 if (GetFileStats(path, &stat, &err)==-1)//если файл сущ-вует и в нём чёто есть
 { 
  file = fopen(path,A_WriteOnly+A_Create,P_WRITE,&err);
  fwrite(file,filebody,size,&err);
  fclose(file, &err); //закрываем
 }
 
}

void saverec()
{
 char td[20],rec[256],buff[128];
 int size;
 for (int i=0;i<5;i++) buff[i]=timegame[i];
 buff[6]=level;
 
 TDate date; TTime time; 
 GetDateTime(&date,&time);
 sprintf(td,"%02d:%02d %02d.%02d.%04d",time.hour,time.min,date.day,date.month,date.year);
 
 for (int i=0;i<16;i++) buff[i+8]=td[i];
 for (int i=0;i<strlen(sudokuname);i++) buff[i+25]=sudokuname[i];
 
 size = 25 + strlen(sudokuname);
 
 int rf;
 unsigned int err;
 sprintf(rec,"%s%s",cfgfolder,"SuDoKu.rec");
 unlink(rec,&err); 
 rf = fopen (rec,A_WriteOnly+A_Create,P_WRITE,&err);
 fwrite(rf, buff, size, &err);
 fclose(rf, &err);
 
}

char *timeBySeconds2(char str[], int sec)
{
 int h,m,s;
 h = sec/3600;
 m = (sec - h*3600)/60;
 s = sec - h*3600 - m*60;
 sprintf(str,"%02d:%02d:%02d",h,m,s);
 return str;
}
/*
char *loadrec()
{
  int tmrrec;
  char rec[256],buff[128];
  char tm[20];

  FSTATS stat;
  unsigned int err;
  sprintf(rec,"%s%s",cfgfolder,"SuDoKu.rec");
  if (GetFileStats(helpfile, &stat, &err)!=-1 && stat.size)
  {
   int rf = fopen(rec, A_ReadOnly, P_READ, &err);
   fread(rf, buff, stat.size, &err);
   fclose(rf, &err);
  }
 
 
 int tmrrec = char2int(buff[0])*10000 + char2int(buff[1])*1000 + char2int(buff[2])*100 + char2int(buff[3])*10 +char2int(buff[4]);

 timeBySeconds2(tm, tmrrec);
 
 
 for (int i=0;i<16;i++) buff[i+8]=td[i];
 for (int i=0;i<strlen(sudokuname);i++) buff[i+25]=sudokuname[i];
 
 size = 25 + strlen(sudokuname);
 
 
}
*/



