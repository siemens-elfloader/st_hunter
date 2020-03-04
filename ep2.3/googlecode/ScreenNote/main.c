#include "..\inc\swilib.h"
#include "conf_loader.h"

#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])
CSM_DESC icsmd;
void (*old_icsm_onClose)(CSM_RAM*);
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);

extern const unsigned int cfgx;
extern const unsigned int cfgy;
extern const unsigned int cfgx2;
extern const unsigned int cfgy2;
extern const unsigned int cfgcolor;
extern const unsigned int cfgfont;
extern const unsigned int cfgxd;
extern const unsigned int cfgyd;
extern const unsigned int cfgxd2;
extern const unsigned int cfgyd2;
extern const unsigned int cfgdcolor;
extern const unsigned int cfgoutlcolor;
extern const unsigned int cfgdoutlcolor;
extern const unsigned int cfgdfont;
extern const unsigned int cfgdate;
extern const unsigned int cfgweek;
//extern const unsigned int cfgcheck;
extern const unsigned int cfgtime1;
extern const unsigned int cfgtime2;
extern const unsigned int cfgupdate_time;
extern const char root_dir[128];

char filename[128];//星期
char filenamed[128];//日期
char tnote[128];
char tnoted[128];
int len_t;
int len_td;

GBSTMR update_tmr;


int MyIDLECSM_onMessage(CSM_RAM* data, GBS_MSG* msg)
{
  int csm_result;
  csm_result=old_icsm_onMessage(data, msg);
  
  void What_times();
  What_times();
  WSHDR *ws = AllocWS(256);
  WSHDR *wsd = AllocWS(256);
  str_2ws(ws,tnote,len_t);
  str_2ws(wsd,tnoted,len_td);
	
  if (cfgweek && IsGuiOnTop(idlegui_id))
  {
    GUI *igui=GetTopGUI();
    if (igui)
    {
      void* idata = GetDataOfItemByID(igui, 2);
      if (idata)
      {
//	void *canvasdata=((void **)idata)[DISPLACE_OF_IDLECANVAS/4];
//      DrawCanvas(canvasdata,cfgx,cfgy,cfgx+(wstrlen(ws))*6,cfgy+12,1);
        DrawString(ws,cfgx,cfgy,cfgx2,cfgy2,cfgfont,34,GetPaletteAdrByColorIndex(cfgcolor),GetPaletteAdrByColorIndex(cfgoutlcolor));
        DrawString(wsd,cfgxd,cfgyd,cfgxd2,cfgyd2,cfgdfont,34,GetPaletteAdrByColorIndex(cfgdcolor),GetPaletteAdrByColorIndex(cfgdoutlcolor));
      }
    }
  }
  if(cfgscrsav && IsScreenSaver())
    DrawString(ws,cfgxs,cfgys,cfgxs2,cfgys2,cfgsfont,2,GetPaletteAdrByColorIndex(cfgscolor),GetPaletteAdrByColorIndex(23));
 //屏保显示
  FreeWS(ws);
  FreeWS(wsd);
  return(csm_result);
}


int Month_days(int mon,int years)
{
  int mthdays;
  switch(mon)
  {
//   case 0 : mthdays=days; break;
   case 2 : mthdays=( ((years%4 == 0 && years%100 != 0) || years%400 == 0) ? 29 : 28 ); break;
   case 4 :
   case 6 :
   case 9 :
   case 11 : mthdays=30; break;
   default : mthdays=31;
  }
  return(mthdays);
}  
  

int Get_week()  //以2007.4.1 sunday为判断星期起始点,对于之前的日期不做判断
{
  unsigned int days,daycout,months,mon,years,tweek;
  TTime time;
  TDate date;
  GetDateTime (&date,&time);
  
  days=date.day;
  months=date.month;
  years=date.year;

  daycout=days;
  mon=months;
  
  int i;
  
  if(years==2007)
   for(i=1;mon>4;i++)
    {
      mon=months-i;
      daycout+=Month_days(mon,years);
    }

  if(years>2007)
   {
    daycout+=275;     //从2007.4.1 sunday开始到2007年结束的天数
    for(i=1;mon>1;i++)//当年当日累计天数
     {
      mon=months-i;
      daycout+=Month_days(mon,years);
     }
    if(years>2008)    //另加一年的天数
      daycout+=(years-2008)*((((years-1)%4 == 0 && (years-1)%100 != 0) || (years-1)%400 == 0) ? 366 : 365 );
   }
  
  tweek=daycout%7;//0,星期6;//1,星期7;//2,星期1;//3,星期2;//......
  switch(tweek)
   {
    case 0 : tweek=6; break;
    case 1 : tweek=7; break;
    default : tweek-=1;
   }
  return(tweek);
}


/*
21,22,23,0,1,2;(3+2+2)/2
23,0,1,2,3;(1+3+2)/2
23456;(4+2)/2
234567;(5+2)/2
*/

void What_times()
{
  unsigned int k=1;
  unsigned int time1,time2;
  time1=cfgtime1;
  time2=cfgtime2;
  volatile int hFiled = -1;
  void Todays_note();
  
  TTime time;
  TDate date;
  GetDateTime (&date,&time);
  
  if(cfgtime1>cfgtime2)
    k=(24-cfgtime1+cfgtime2+2)/2;
  else
   {
    if(cfgtime1<cfgtime2)
      k=(cfgtime2-cfgtime1+2)/2;
    else
      if(cfgtime1==cfgtime2 && cfgtime1==0)//between 0 and 0 ,全天显示。
        Todays_note();
   }
  
  while(k)
   {
    if(time.hour==time1 || time.hour==time2)
     {
      TTime time;
      TDate date;
      GetDateTime (&date,&time);
      
      unsigned int errcode;      
      sprintf(filenamed,"%s\\0\\%d-%d,%d.tmo",root_dir,date.month,date.day,time.hour);//日期
      hFiled = fopen(filenamed,A_ReadWrite,P_READ+P_WRITE,&errcode);
      if(hFiled==-1)
       {
        sprintf(filenamed,"%s\\0\\%d-%d.tmo",root_dir,date.month,date.day);//日期
        hFiled = fopen(filenamed,A_ReadWrite,P_READ+P_WRITE,&errcode);
       }
      
      Todays_note(hFiled);
      
      break;
     }
    if(time1==24)
      time1=0;
    if(time2==0)
      time2=24;
    time1++;
    time2--;
    k--;
   }
  
  if(!k || hFiled == -1)
  {
    WSHDR *ws=AllocWS(6);
    wsprintf(ws," ");
    
    if(!k)
      ws_2str(ws,tnote,6);
    
    ws_2str(ws,tnoted,6);
    FreeWS(ws);
  }
  GBS_StartTimerProc(&update_tmr,262*cfgupdate_time,What_times);
}



void Todays_note(volatile int hFiled)
{
  unsigned int errcode;
  unsigned int week;
  int i=0;

  TTime time;
  TDate date;
  GetDateTime (&date,&time);
  
  week=Get_week();
  if(!cfgcheck || cfgcheck==2)
    sprintf(filename,"%s\\%d\\%d.tmo",root_dir,week,time.hour);//星期
  else
    if(cfgcheck==1)
      sprintf(filename,"%s\\%d\\0.tmo",root_dir,week);
  
  volatile int hFile = -1;
  loop:
  hFile = fopen(filename,A_ReadWrite,P_READ+P_WRITE,&errcode);
  
  if(hFile==-1)//星期
   {
     i++;
     if(i==1 && cfgcheck==2)
     {
      sprintf(filename,"%s\\%d\\0.tmo",root_dir,week);
      goto loop;
     }
          
    char file[128];
    sprintf(file,"%s\\%d",root_dir,week);//

    if(!isdir(file,&errcode))
     {
      mkdir(file,&errcode);
     }
    WSHDR *ws=AllocWS(6);
    wsprintf(ws," ");
    ws_2str(ws,tnote,6);
    FreeWS(ws);
   }
  else
    {
      int str_len = 0;
      fread(hFile,&str_len,2,&errcode);//
      WSHDR *ws=AllocWS(str_len);
      errcode=0;
      lseek(hFile,0,0,&errcode,&errcode); // 
      fread(hFile,ws->wsbody,(str_len+1)*2,&errcode);
      len_t=(str_len)*4;
      ws_2str(ws,tnote,len_t);
      FreeWS(ws);
     }
  
  if(hFiled==-1)//日期
   {
   WSHDR *ws=AllocWS(6);
     wsprintf(ws," ");
     ws_2str(ws,tnoted,6);
     FreeWS(ws);
   }
  else
    {
      int str_len = 0;
      fread(hFiled,&str_len,2,&errcode);//
      WSHDR *ws=AllocWS(str_len);
      errcode=0;
      lseek(hFiled,0,0,&errcode,&errcode); // 
      fread(hFiled,ws->wsbody,(str_len+1)*2,&errcode);
      len_td=(str_len)*4;
      ws_2str(ws,tnoted,len_td);
      FreeWS(ws);
     }
  fclose(hFile, &errcode);
  fclose(hFiled, &errcode);
}

void Today_note()
{
  unsigned int i;
  char *s1;
  char *s11;
  char *s2;
  char *s22;
  char *check;//字符在文件中第一次出现的位置
  char *check1;
  char *check2;
  char times[16],time1,time11,time2,time22;
  unsigned int errcode;
  
  TTime time;
  TDate date;
  GetDateTime (&date,&time);
  sprintf(times,">%02d:%02d-", time.hour,time.min); 
  
  volatile int hFilet = -1;
  sprintf(filename[0],"%s",today_dir);
  hFilet = fopen(filename[0],A_ReadWrite,P_READ+P_WRITE,&errcode);
  fread(hFilet,today_tmo,32767,&errcode);
      
  if(hFilet==-1)//
   {
     Writ_first(0,times);
   }
  else
   {
    if(strstr(today_tmo,times))
     {
       check=strstr(today_tmo,times)+13;
     }
    else      //判断时间范围
     {
      i=0;    //判断时间1
      do      //hour  time1,check1
       {
        time1=time.hour-i;
        sprintf(s1,">%02d:",time1);
//        s1=strcat(">",time1);
        check1=strstr(today_tmo,s1);
        i++;
       }
      while(time1>0 || !check1);
        
      if(check1 && i==0)  //min   //i=0,只有与当前的时间相符才有必要检查分钟min
       {
        do
         {
          time11=time.min-i;
          sprintf(s11,"%s%02d-",s1,time11);
//          s11=strcat (s1,":");
//          s1=strcat (s11,time11);
          check1=strstr(today_tmo,s11);
          i++;
         }
        while(time11>0 || !check1);
       }        //判断时间1
      
      if(check1)//判断时间2
       {
        i=0;
        do      //hour  time2,check2
         {
          time2=time.hour+i;
          sprintf(s2,"-%02d:",time2);
//          s2=strcat ("-",time2);
          check2=strstr(today_tmo,s2);
          i++;
         }
        while(time2<23 || !check2);
        
        if(check1+6 !=check2)
          check2=0;
          
        if(check2 && i==0)  //min
         {
          do
           {
            time22=time.min+i;
            sprintf(s22,"%s%02d",s2,time22);
//            s22=strcat (s2,":");
//            s2=strcat (s22,time22);
            check2=strstr(today_tmo,s22);
            i++;
           }
          while(time22<59 || !check2);
         }
       }   //判断时间2
        
       if(check1 && check2)
         check=check1+13;
     }     //判断时间范围
      
      if (check && strncmp(check,"\n",16))
       {
        int str_len = 0;
        fread(hFilet,&str_len,2,&errcode);
        WSHDR *ws=AllocWS(str_len);
      
        lseek((int)check,26,0,&errcode,&errcode);
        fread(hFilet,ws->wsbody,(str_len+1)*2,&errcode);
        CutWSTR(ws,str_len-13);
        len_t=(str_len-13)*4;
        ws_2str(ws,tnote,len_t);
        FreeWS(ws);
       }
     }
  //if(errcode){ ShowMSG(1,(int)"nnnn!");}
  fclose(hFilet, &errcode);
//  GBS_StartTimerProc(&update_tmr,262*cfgupdate_time,Today_note);
}

          
void MyIDLECSM_onClose(CSM_RAM *data)
{
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  seqkill(data,old_icsm_onClose,&ELF_BEGIN,SEQKILLER_ADR());
}



int main(void)
{
  unsigned int err;
  InitConfig();
  
  if(!isdir(root_dir,&err))
  {
    mkdir(root_dir,&err);
  }
  
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);//
  memcpy(&icsmd,icsm->constr,sizeof(icsmd));//
  old_icsm_onMessage=icsmd.onMessage;//
  icsmd.onMessage=MyIDLECSM_onMessage;// 
  icsm->constr=&icsmd;//
  GBS_StartTimerProc(&update_tmr,262*cfgupdate_time,What_times);//
  return 0;
}
