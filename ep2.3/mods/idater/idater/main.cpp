#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"
#include "conf_loader.h"
#include "print.h"

#define IPC_TEXTINFO_NAME "iDater" 
#define IPC_UPDATE_STAT 1 

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ my_ipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

void TimerProc(void)
{  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);//эта херня отправляет ипц хтаску
}


int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);

extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}



#define TMR_SECOND 216
GBSTMR mytmr;
extern void InitConfig();
int my_csm_id;
unsigned int *ErrorNumber;

const char percent_t[]="%t";

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;   

// ----------------------------------------------------------------------------
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}
#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

int get_string_width(WSHDR *ws, int font)
{
  int width=0;
  unsigned short *body=ws->wsbody;
  int len=body[0];
  while(len)
  {
    width+=GetSymbolWidth(body[len],font);
    len--;
  }
  return (width+1);//хз, без этого тупит
}

const char per_t[]="%t";

extern const unsigned int X1;
extern const unsigned int Y1;
extern const unsigned int X2;
extern const unsigned int Y2;
extern const unsigned int X3;
extern const unsigned int Y3;
extern const unsigned int X4;
extern const unsigned int Y4;

extern const int month_font;
extern const int day_font;
extern const int week_font;
extern const int font;
extern const int date_font;

extern const int DRAW_MODE;
extern const int MONTH_MODE;
extern const int WEEK_MODE;
extern const int DATE_MODE;

extern const char col_month[];
extern const char col[];
extern const char col_week[];
extern const char col_date[];

extern const int align;
extern const int align_month;
extern const int align_week;
extern const int align_date;
extern const int space;

extern const char FontPath[128];
extern const char FontPath2[128];
extern const char FontPath3[128];
extern const int fullset;

const char per_s[]="%s";
void *canvasdata;
char *s,*s1,*s2,*s3;

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  void *icsm;
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"iDater config updated!");
      InitConfig();
      
      FontPathFree();
      
      sprintf(s,per_s,FontPath);
      FontPathInit(1,s,fullset);
      
      sprintf(s,per_s,FontPath2);
      FontPathInit(2,s,fullset);
        
      sprintf(s,per_s,FontPath3);
      FontPathInit(3,s,fullset);
    }
  }
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
        case IPC_UPDATE_STAT:
          GBS_StartTimerProc(&mytmr, 10*TMR_SECOND/10, TimerProc);
        }
      }
    }
  }
  icsm=FindCSMbyID(CSM_root()->idle_id);
  
 
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm))) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {
           canvasdata = BuildCanvas();
           TDate date;
           TTime time;
           GetDateTime(&date, &time); 
          /* WSHDR *ws1,*ws2,*ws3;
           ws1=AllocWS(128);
           ws2=AllocWS(128);
           ws3=AllocWS(128);
           */
           char *mmonth[12]={"Янв","Фев","Мар","Апр","Май","Июн","Июл","Авг","Сен","Окт","Ноя","Дек"};
           char *mmonth2[12]={"Январь","Февраль","Март","Апрель","Май","Июнь","Июль","Август","Сенябрь","Октябрь","Ноябрь","Декабрь"};
           char *mmonth3[12]={"Января","Февраля","Марта","Апреля","Мая","Июня","Июля","Августа","Сенября","Октября","Ноября","Декабря"};
   
           char *dday[7]={"Пн","Вт","Ср","Чт","Пт","Сб","Вс"};
           char *dday2[7]={"Понедельник","Вторник","Среда","Четверг","Пятница","Суббота","Воскресенье"};
                       
           if(DRAW_MODE==0)
           {         
             if(MONTH_MODE!=0)//если показываем месяц,на всякий случай...
             {
               if(MONTH_MODE==1)//Январь
               {
                //wsprintf(ws1,per_t,mmonth2[date.month-1]);
                 sprintf(s1,per_s,mmonth2[date.month-1]);
               }
               else if(MONTH_MODE==2)//Января
               {
                //wsprintf(ws1,per_t,mmonth[date.month-1]);
                 sprintf(s1,per_s,mmonth3[date.month-1]);
               }
               else if(MONTH_MODE==3)//Янв
               {
                //wsprintf(ws1,per_t,mmonth[date.month-1]);
                 sprintf(s1,per_s,mmonth[date.month-1]);
               }
               PrintField(X1,Y1,s1,align_month,month_font,space);               
               //void PrintField( int x, int y, char *str, int align, int font, int space); //вывод строки
               // DrawCanvas(canvasdata, X1, Y1, X1+Get_WS_width(ws1, month_font), Y1+GetFontYSIZE(month_font), 1);                 
               //DrawString(ws1,X1,Y1,X1+Get_WS_width(ws1, month_font)+4, Y1+GetFontYSIZE(month_font), month_font,0,col_month,GetPaletteAdrByColorIndex(23)); 
             }
            
            if(WEEK_MODE!=0)
              {
                if(WEEK_MODE==2)
                {
                  //wsprintf(ws2,per_t,dday[GetWeek(&date)]);
                    sprintf(s2,per_s,dday[GetWeek(&date)]);
                }
                else if(WEEK_MODE==1)
                {
                  //wsprintf(ws2,per_t,dday2[GetWeek(&date)]);
                    sprintf(s2,per_s,dday2[GetWeek(&date)]);
                }  
                  PrintField(X2,Y2,s2,align_week,week_font,space);  
//                DrawCanvas(canvasdata, X2, Y2, X2+Get_WS_width(ws1, week_font), Y1+GetFontYSIZE(week_font), 1); 
  //              DrawString(ws2,X2,Y2,X2+Get_WS_width(ws2, week_font)+4, Y2+GetFontYSIZE(week_font), week_font,0,col_week,GetPaletteAdrByColorIndex(23)); 
              }

             if(DATE_MODE)
             {
             // wsprintf(ws3,"%d",date.day);
                sprintf(s3,"%d",date.day);
                PrintField(X3,Y3,s3,align_date,date_font,space);  
         //     DrawCanvas(canvasdata, X3, Y3, X3+Get_WS_width(ws1,date_font), Y3+GetFontYSIZE(date_font), 1); 
           //   DrawString(ws3,X3,Y3,X3+Get_WS_width(ws3, date_font), Y3+GetFontYSIZE(date_font), date_font,0,col_date,GetPaletteAdrByColorIndex(23)); 
             }
           }

          if(DRAW_MODE==1)//17.01.08
           {     
           //  wsprintf(ws1, "%02d.%02d.%02d", date.day,date.month,date.year-2000);           
             //DrawCanvas(canvasdata, X4, Y4, X4+Get_WS_width(ws1,font), Y4+GetFontYSIZE(font), 1); 
             //DrawString(ws1,X4,Y4,X4+Get_WS_width(ws1,font), Y4+GetFontYSIZE(font), font,0,col,GetPaletteAdrByColorIndex(23)); 
             sprintf(s1, "%02d.%02d.%02d", date.day,date.month,date.year-2000); 
             PrintField(X4,Y4,s1,align,font,space);  
//( int x, int y, char *str, int align, int font, int space, int lineSpace, int isScroll, short fixSize, int maxLen); //вывод текста
//             PrintText(X4,Y4,s1,align,1,space,1,0,strlen(s1),strlen(s1));
           }
           
           if(DRAW_MODE==2)//17.01.2008
           {     
             ///wsprintf(ws1, "%02d.%02d.%04d", date.day,date.month,date.year);           
             //DrawCanvas(canvasdata, X4, Y4, X4+Get_WS_width(ws1,font), Y4+GetFontYSIZE(font), 1); 
             //DrawString(ws1,X4,Y4,X4+Get_WS_width(ws1,font), Y4+GetFontYSIZE(font), font,0,col,GetPaletteAdrByColorIndex(23)); 
               sprintf(s1, "%02d.%02d.%04d", date.day,date.month,date.year);           
               PrintField(X4,Y4,s1,align,font,space);  
           }
           
           if(DRAW_MODE==3)//сб 12.01
           {     
            // wsprintf(ws1, "%t %02d.%02d",dday[GetWeek(&date)],date.day,date.month);           
            // DrawCanvas(canvasdata, X4, Y4, X4+Get_WS_width(ws1,font), Y4+GetFontYSIZE(font), 1); 
            // DrawString(ws1,X4,Y4,X4+Get_WS_width(ws1,font), Y4+GetFontYSIZE(font), font,0,col,GetPaletteAdrByColorIndex(23)); 
               sprintf(s1, "%s %02d.%02d",dday[GetWeek(&date)],date.day,date.month); 
               PrintField(X4,Y4,s1,align,font,space);  
           }
           
           if(DRAW_MODE==4)//Суббота 12.01
           {     
             sprintf(s1, "%s %02d.%02d",dday2[GetWeek(&date)],date.day,date.month); 
             PrintField(X4,Y4,s1,align,font,space);  
           }
           
           if(DRAW_MODE==5)//Сб 12 Янв
           {     
             sprintf(s1, "%s %02d %s",dday[GetWeek(&date)],date.day,mmonth[date.month-1]); 
             PrintField(X4,Y4,s1,align,font,space);  
           }
           
           if(DRAW_MODE==6)//Сб 12 Января
           {     
             sprintf(s1, "%s %02d %s",dday2[GetWeek(&date)],date.day,mmonth3[date.month-1]); 
             PrintField(X4,Y4,s1,align,font,space);  
           }
           
           if(DRAW_MODE==7)//Суббота 12 Янв
           {     
             sprintf(s1, "%s %02d %s",dday2[GetWeek(&date)],date.day,mmonth[date.month-1]); 
             PrintField(X4,Y4,s1,align,font,space);  
           }
           
           if(DRAW_MODE==8)//Сб 12 
           {     
             sprintf(s1, "%s %02d",dday2[GetWeek(&date)],date.day); 
             PrintField(X4,Y4,s1,align,font,space);  
           }
           
           if(DRAW_MODE==9)//12 Сб
           {     
             sprintf(s1, "%02d %s",date.day,dday2[GetWeek(&date)]); 
             PrintField(X4,Y4,s1,align,font,space);  
           }
       }
    }
  }
 return(1);
}

/*
Сб 12 Янв +5
Суббота 12 Января +6
Суббота 12 Янв +7
Сб 12.01 +3
Суббота 12.01 +4
Сб 12 +8
12 Сб +9
12.01.08 +1
12.01.2008 +2
*/
static void maincsm_oncreate(CSM_RAM *data)
{ 
  s =malloc(128);
  s1=malloc(256);
  s2=malloc(256);
  s3=malloc(256);
  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}

void maincsm_onclose(CSM_RAM *data)
{ 
//  mfree(s1);
  //mfree(s2);
  //mfree(s2);

  FontPathFree();
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

static unsigned short maincsm_name_body[140];
const int minus11=-11;

static const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  maincsm_onclose,
  sizeof(CSM_RAM),
  1,
  &minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};


static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"iDater");
}
// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  InitConfig();
  UpdateCSMname();
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();

  char *s=malloc(128);
  sprintf(s,per_s,FontPath);
  FontPathInit(1,s,fullset);
  
  sprintf(s,per_s,FontPath2);
  FontPathInit(2,s,fullset);
  
  sprintf(s,per_s,FontPath3);
  FontPathInit(3,s,fullset);
  return (0);
}
