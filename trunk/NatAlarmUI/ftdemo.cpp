#include <alib/img.h>
#include <alib/io.h>
#include <alib/font.h>
#include <alib/ui.h>

#include <swilib.h>

#include <ft2build.h>
#include <freetype/freetype.h>

#include "ftdemo.h"

#include <alib/sysdraw.h>

extern unsigned int CFG_FONT_SIZE;
extern unsigned int CFG_FONT_TIME_SIZE;

///////////////////////NatAlarm

extern unsigned const int CFG_ALARM_ICON_NUM;

extern unsigned const int ALARM_TIME_x;
extern unsigned const int ALARM_TIME_y;
extern unsigned const int ALARM_ICON_x;
extern unsigned const int ALARM_ICON_y;

extern unsigned const int ALARM_ON_x;
extern unsigned const int ALARM_ON_y;
extern unsigned const int REPEAT_ON_x;
extern unsigned const int REPEAT_ON_y;
extern unsigned const int REPEAT_TIME_x;
extern unsigned const int REPEAT_TIME_y;

extern unsigned const int DAYS_x;
extern unsigned const int DAYS_y;

extern const unsigned int CFG_WIDTH_BETWEEN_DAYS;

extern const unsigned int CFG_DISABLE_ICONBAR;
extern const unsigned int CFG_SHOW_CLOCK;

#define smesh2 0

#ifdef ELKA
#define CBOX_CHECKED 0x53A
#define CBOX_UNCHECKED 0x53C
//#define ICON_ALARM 0x003
#else
#define CBOX_CHECKED 0x566
#define CBOX_UNCHECKED 0x568
//#define ICON_ALARM 0x33F
#endif



int curpos=0;//позичия курсора
int curitem=0;//позиция итема под курсором
int edittime=0;//Если редактим время
int timepos=0;//позиция 0-4
int editsnooze=0;
int snozeepos=0;

char days[7];
char const *day_names[7]={"Пн","Вт","Ср","Чт","Пт","Сб","Вс"};
TTime alarm_time;
int snooze_time;
int alarm_active;
int auto_snooze;


void MoveCurItem(int i,int j);
void EnterButton();
void EditTime(int key);
void EditSnooze (int key);

///////////////////////////
bool IsWinStarted=0;
bool IsFontCfgUpdate=1;
Image *mybuffer;

//TFont font;
ft_font *ftf;
ft_font *ft_time_font;

ft_font *GetFontFT (){ return ftf;}

const char *GetCurFont (){
    return "0:\\Zbin\\fonts\\Ubuntu-R.ttf";
}

char ibuf[128];
void MSG (int i){
    sprintf (ibuf, "%d", i);
    ShowMSG (1, (int)ibuf);
}
#define TEST_FONT

UI myui;

GBSTMR tmr;
int delay=5;
//ConfigInfo themeConfig;

#define MODE_LANDSCAPE

void Parser();
void SavePD();

void UI_OnCreate (){
    IsWinStarted=1;
    int scrH=ScreenH ();
    int scrW=ScreenW ();

    Parser();

#ifdef TEST_FONT
    ftf = ft_open(GetCurFont (), CFG_FONT_SIZE);
    ft_time_font = ft_open (GetCurFont (), CFG_FONT_TIME_SIZE);
#endif

    myui.SetHeader ("Будильник");
    myui.SetSofts ("Обновить", "Выход");

    mybuffer=GetImgBuffer();


    myui.OnFocus ();
}

void DrawAlarm ();
char print_buf[128];
void UI_OnRedraw (){
    bool need_redraw=0;
    if (IsFontCfgUpdate){
#ifdef TEST_FONT
        ftf = ft_open(GetCurFont (), CFG_FONT_SIZE);
        ft_time_font = ft_open (GetCurFont (), CFG_FONT_TIME_SIZE);
        IsFontCfgUpdate=0;
#endif
    }

    if (myui.IsFocus ()){

        DrawBG (mybuffer);
        DrawUpPanel (mybuffer);

        DrawDownPanel(mybuffer);
        DrawHeaderPanel (mybuffer);

        myui.DrawHeader (mybuffer, ftf);
        myui.DrawSofts (mybuffer, ftf);

        //Alarm Time
        if (alarm_time.hour>=24) alarm_time.hour=23;
        if (alarm_time.min>=60)  alarm_time.min=0;

        int cbox_w=GetImgWidth(CBOX_CHECKED);
        int cbox_h=GetImgHeight(CBOX_CHECKED);


        int time_fontH=GetFontH (ft_time_font);
        int fontH=GetFontH (ftf);

        int offset_cbox=(cbox_h-fontH)/2;

        sprintf (print_buf, "%02d:%02d", alarm_time.hour, alarm_time.min);
        int str_w=GetStringW (print_buf, ft_time_font, UTF8);


        if (curpos) DrawString_UTF8 (mybuffer, print_buf, ft_time_font, ALARM_TIME_x, ALARM_TIME_y, mybuffer->GetW(), mybuffer->GetH(), TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));
        int letter_w=0;

         //DrawTimeAlarm
        if (curpos==0){
            if (!edittime){
                //if (curitem==0){
                    DrawString_UTF8 (mybuffer, print_buf, ft_time_font, ALARM_TIME_x, ALARM_TIME_y, mybuffer->GetW(), mybuffer->GetH(), TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));
                    mybuffer->DrawLine (ALARM_TIME_x, ALARM_TIME_y+time_fontH, ALARM_TIME_x+str_w, ALARM_TIME_y+time_fontH, 0xFFFFFFFF);
                    //;//DrawCursor(ALARM_TIME_x,ALARM_TIME_x+Get_WS_width(ws,ALARM_TIME_FONT)/2*0.9, ALARM_TIME_y+GetFontYSIZE(ALARM_TIME_FONT)+1);
               // }
               // else{
                  //  mybuffer->DrawLine (ALARM_TIME_x, ALARM_TIME_y+time_fontH, ALARM_TIME_x+str_w, ALARM_TIME_y+time_fontH, 0xFFFFFFFF);
                    //;//DrawCursor(ALARM_TIME_x+Get_WS_width(ws,ALARM_TIME_FONT)*85/150*0.9,ALARM_TIME_x+Get_WS_width(ws,ALARM_TIME_FONT)*160/150*0.9, ALARM_TIME_y+GetFontYSIZE(ALARM_TIME_FONT)+1);
                //}

            }
            else {
                int step=0;//Get_WS_width(ws,ALARM_TIME_FONT)/4;
                //DrawCursor(ALARM_TIME_x+(step*timepos),ALARM_TIME_x+(step*timepos)+step,ALARM_TIME_y+/*GetFontYSIZE(ALARM_TIME_FONT)*/+1);//
                int w=0;

                int line_x=0;
                int line_len=0;
                MyRECT rc={0,0,240,320};

                FT_DrawLetter (mybuffer, ft_time_font, print_buf[0], ALARM_TIME_x, ALARM_TIME_y, &rc, get_color_from_set(CLR_FONT_MENU),0);
                letter_w=FT_GetLetterW (ft_time_font, print_buf[0]);
                if (timepos==0) line_x=ALARM_TIME_x; line_len=letter_w;
                w+=letter_w;

                FT_DrawLetter (mybuffer, ft_time_font, print_buf[1], ALARM_TIME_x+w, ALARM_TIME_y, &rc, get_color_from_set(CLR_FONT_MENU),0);
                letter_w=FT_GetLetterW (ft_time_font, print_buf[1]);
                if (timepos==1) line_x=ALARM_TIME_x+w; line_len=letter_w;
                w+=letter_w;
                //':'
                FT_DrawLetter (mybuffer, ft_time_font, print_buf[2], ALARM_TIME_x+w, ALARM_TIME_y, &rc, get_color_from_set(CLR_FONT_MENU),0);
                letter_w=FT_GetLetterW (ft_time_font, print_buf[2]); w+=letter_w;

                FT_DrawLetter (mybuffer, ft_time_font, print_buf[3], ALARM_TIME_x+w, ALARM_TIME_y, &rc, get_color_from_set(CLR_FONT_MENU),0);
                letter_w=FT_GetLetterW (ft_time_font, print_buf[3]);
                if (timepos==2) line_x=ALARM_TIME_x+w; line_len=letter_w;
                w+=letter_w;

                FT_DrawLetter (mybuffer, ft_time_font, print_buf[4], ALARM_TIME_x+w, ALARM_TIME_y, &rc, get_color_from_set(CLR_FONT_MENU),0);
                if (timepos==3) line_x=ALARM_TIME_x+w; line_len=letter_w;

                mybuffer->DrawLine (line_x, ALARM_TIME_y+time_fontH, line_x+line_len, ALARM_TIME_y+time_fontH, 0xFFFFFFFF);
            }
        }


        DrawString_UTF8(mybuffer, "Будильник вкл.", ftf, ALARM_ON_x+cbox_w+2, ALARM_ON_y+offset_cbox, mybuffer->GetW(), mybuffer->GetH(), TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));

        DrawString_UTF8(mybuffer, "Повтор вкл.", ftf, REPEAT_ON_x+cbox_w+2,REPEAT_ON_y+offset_cbox, mybuffer->GetW(), mybuffer->GetH(), TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));

        MyRECT rc={0,0,240,320};
        sprintf(print_buf,"%s ","Повтор через");
        DrawString_UTF8 (mybuffer, print_buf, ftf, REPEAT_TIME_x, REPEAT_TIME_y, mybuffer->GetW(), mybuffer->GetH(), TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));
        str_w=GetStringW (print_buf, ftf, UTF8);

        sprintf(print_buf,"%02d",snooze_time);
        FT_DrawLetter (mybuffer, ftf, print_buf[0], REPEAT_TIME_x+str_w, REPEAT_TIME_y, &rc, get_color_from_set(CLR_FONT_MENU),0); letter_w=FT_GetLetterW (ftf, print_buf[0]);
        if (curpos==3){
            if(!curitem) mybuffer->DrawLine (REPEAT_TIME_x+str_w, REPEAT_TIME_y+fontH, REPEAT_TIME_x+str_w+letter_w, REPEAT_TIME_y+fontH, 0xFFFFFFFF);
        }
        str_w+=letter_w;

        FT_DrawLetter (mybuffer, ftf, print_buf[1], REPEAT_TIME_x+str_w, REPEAT_TIME_y, &rc, get_color_from_set(CLR_FONT_MENU),0); letter_w=FT_GetLetterW (ftf, print_buf[0]);
        if (curpos==3){
            if (curitem) mybuffer->DrawLine (REPEAT_TIME_x+str_w, REPEAT_TIME_y+fontH, REPEAT_TIME_x+str_w+letter_w, REPEAT_TIME_y+fontH, 0xFFFFFFFF);
        }
        str_w+=letter_w;

        sprintf(print_buf," %s", "минут");
        DrawString_UTF8 (mybuffer, print_buf, ftf, REPEAT_TIME_x+str_w, REPEAT_TIME_y, mybuffer->GetW(), mybuffer->GetH(), TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));


        int a=DAYS_x;
        int b=CFG_WIDTH_BETWEEN_DAYS;

        for (int i=0; i<7; i++){

            int offset_day=(cbox_w-GetStringW((char*)day_names[i], ftf, UTF8))/2;
            DrawString_UTF8 (mybuffer, (char*)day_names[i], ftf, a+offset_day, DAYS_y-GetFontH(ftf), mybuffer->GetW(), mybuffer->GetH(), TEXT_ALIGNLEFT, get_color_from_set(CLR_FONT_MENU));
            a+=b;
        }

        if (CFG_SHOW_CLOCK){
            TTime t;
            GetDateTime(NULL,&t);
            sprintf(print_buf, "%02d:%02d",t.hour,t.min);
            DrawHeaderText (mybuffer, ftf, print_buf, TEXT_ALIGNRIGHT);
        }

        if (!CFG_DISABLE_ICONBAR){
            MyRECT rc={0, YDISP, mybuffer->GetW(), mybuffer->GetH()};
            DrawImageRect (mybuffer, 0, YDISP, &rc);
        }
        else{
            DrawImage (mybuffer, 0, 0);
        }

        DrawAlarm ();
    }

    if (need_redraw){
        if (myui.IsFocus())
        GBS_StartTimerProc(&tmr, 216/delay, (void(*)(GBSTMR*))&UI_OnRedraw);
    }
}

void UI_OnFocus (){
    myui.OnFocus ();
}

void UI_UnFocus (){
    myui.UnFocus ();
}

//////////////////////
/*
#define MakeFunc(prot,name,addr) prot* const name =  (prot*)addr

typedef void V_TYPE_I (int);
typedef void V_TYPE_V (void);

MakeFunc(V_TYPE_I,DrawIconbar,0xA05C15F5);

MakeFunc(V_TYPE_V,DrawIconbar2,0xA05C0D45);
*/

///////////////////////

void DrawWindow (){
    if (!IsWinStarted){
        UI_OnCreate ();
    }
    UI_OnRedraw ();
/*
#ifdef ELKA
    DrawIconbar (0xAD209AB4);
#endif
*/
}

int MyUIOnKey (int mess, int key){
    if (mess==KEY_DOWN || mess==LONG_PRESS){

        switch(key){

            case '#' :
            DoConfig ();
            break;

            case RIGHT_SOFT:
            SavePD();
            RefreshAlarmClock();
            return (1);
            break;

            case UP_BUTTON:
            edittime=0;
            curitem=0;
            if(curpos>0) curpos--;
            else curpos=4;
            break;

            case DOWN_BUTTON:
            edittime=0;
            curitem=0;
            if(curpos<4) curpos++;
            else curpos=0;
            break;

            case RIGHT_BUTTON:
            MoveCurItem(curpos,0);
            break;

            case LEFT_BUTTON:
            MoveCurItem(curpos,1);
            break;

            case ENTER_BUTTON:
            EnterButton();
            break;

            case LEFT_SOFT:
            SavePD();
            RefreshAlarmClock();
            Parser();
            break;
        }

        if ((key>='0')&&(key<='9') && mess==KEY_DOWN)
        {
            if(curpos==0){
                edittime=1;
                EditTime(key-'0');
            }
            if(curpos==3){
                EditSnooze(key-'0');
            }
        }

        DrawWindow ();
    }

    return 0;

}

void CloseTimer (){
  GBS_DelTimer (&tmr);
}


void UI_OnClose (){
    CloseTimer ();
}


void OnCloseWin (){
#ifdef TEST_FONT
    ft_close(ftf);
    ft_close(ft_time_font);
#endif

    UI_OnClose ();
}


///////////////////////NatAlarm



char *temp_chars; //Собственно файл
char **temp_lines; //Массив указателей на строки

void FreePD(void){
  if (temp_lines) mfree(temp_lines);
  if (temp_chars) mfree(temp_chars);
  temp_lines=NULL;
  temp_chars=NULL;
}

extern const char alarmclock[128];

int LoadPD(){
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  FreePD();
  strcpy(fn,alarmclock);
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if((f=_open(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;
  p=temp_chars=(char*)malloc(fsize+1);
  p[_read(f,p,fsize,&ul)]=0;
  _close(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	temp_lines=(char**)realloc(temp_lines,(i+1)*sizeof(char *));
	temp_lines[i++]=pp;
      }
      pp=NULL;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==NULL) pp=p;
    }
    p++;
  }
  return i;
}

void GetTime(TTime *t,int time)
{
  time=time/60;
  int hour=time/60;
  int min=time%60;
  t->hour=hour;
  t->min=min;
}

int SetAlarmTime(TTime *t)
{
  int i=0;
  i=t->hour*60*60;//hour;
  i+=t->min*60;//min
  return i;
}


void ParseSnoozeTime(char *s){
  int time=strtoul(s,0,10);
  snooze_time=time/60;
}

void ParseAlarmTime(char *s){
  int time=strtoul(s,0,10);//28500
  GetTime(&alarm_time,time);
}

void ParseDayInUse(char *s){
  for(int i=0;i<7;i++){
    if (s[i]=='1') days[i]=1;
    else days[i]=0;
  }
}

void ParseAlarmActive(char *s){
  alarm_active=strtoul(s,0,10);
}

void ParseAutoSnooze(char *s){
  auto_snooze=strtoul(s,0,10);
}

void Parser(){
  int i=LoadPD();
  if(!i)
  {
    ShowMSG(1,(int)"Don't load file!");
    return;
  }
  int j=0;
  char *s;
  while(j<i)
  {
    s=strrchr(temp_lines[j],'=')+1;
    if(strstr(temp_lines[j],"snooze_time"))  ParseSnoozeTime(s);
    if(strstr(temp_lines[j],"days_in_use"))  ParseDayInUse(s);
    if(strstr(temp_lines[j],"alarm_time"))   ParseAlarmTime(s);
    if(strstr(temp_lines[j],"alarm_active")) ParseAlarmActive(s);
    if(strstr(temp_lines[j],"auto_snooze"))  ParseAutoSnooze(s);
    j++;
  }
}

void SavePD(){
  volatile int hFile;
  unsigned int io_error = 0;
  char fullname[128];
  snprintf(fullname,127,alarmclock);
  _unlink(fullname,&io_error);
  hFile = _open(fullname,A_ReadWrite + A_Create +  A_Append + A_BIN,P_READ+P_WRITE, &io_error);
  char *s=(char*)malloc(128);
  char *p=(char*)malloc(10);
  *(p)=0;

  for(int i=0;i<7;i++)
    if(days[i]>1) days[i]=0;

  if(hFile!=-1)
  {
    sprintf(s,"000024:T:snooze_time=%d\r\n",snooze_time*60);
    _write(hFile, s, strlen(s), &io_error);

    sprintf(p,"%01d%01d%01d%01d%01d%01d%01d",days[0],days[1],days[2],days[3],days[4],days[5],days[6]);
    snprintf(s,31,"000028:T:days_in_use=%s\r\n",p);
    _write(hFile, s, strlen(s), &io_error);

    sprintf(p,"%d",SetAlarmTime(&alarm_time));
    sprintf(s,"0000%d:T:alarm_time=%d\r\n",20+strlen(p),SetAlarmTime(&alarm_time));
    _write(hFile, s, strlen(s), &io_error);

    sprintf(s,"000023:T:alarm_active=%d\r\n",alarm_active);
    _write(hFile, s, strlen(s), &io_error);

    sprintf(s,"000022:T:auto_snooze=%d\r\n",auto_snooze);
    _write(hFile, s, strlen(s), &io_error);

    _close(hFile, &io_error);
  }
  else ShowMSG(1,(int)"Can't save file!!!");
  mfree(s);
  mfree(p);
}

/*
000024:T:snooze_time=540
000028:T:days_in_use=1111111
000025:T:alarm_time=28500
000023:T:alarm_active=0
000022:T:auto_snooze=0
*/


void DrawCheckBox (int x, int y, int flag){
    if (!flag) DrawImg (x,y,CBOX_UNCHECKED);
    else DrawImg (x,y,CBOX_CHECKED);
}
char color_cursor[4]={0xFF, 0xFF, 0xFF, 0x64};
void DrawCursor (int x, int x2, int y){
  DrawLine(x,y,x2,y,0,color_cursor);
}


void DrawAlarm(){
    DrawImg(ALARM_ICON_x,ALARM_ICON_y,CFG_ALARM_ICON_NUM);

  //DAYS
  int a=DAYS_x;
  int b=CFG_WIDTH_BETWEEN_DAYS;

  for (int i=0; i<7; i++){
    DrawCheckBox(a,DAYS_y,days[i]==1?1:0);
    a+=b;
  }

  if(curpos==4)
 #ifdef ELKA
    DrawCursor(DAYS_x+(curitem*b)+4,DAYS_x+(curitem*b)+(GetImgWidth(CBOX_CHECKED))*0.9-2,DAYS_y+(GetImgHeight(CBOX_CHECKED))*0.9);
 #else
    DrawCursor(DAYS_x+(curitem*b),DAYS_x+(curitem*b)+(GetImgWidth(CBOX_CHECKED)),DAYS_y+(GetImgHeight(CBOX_CHECKED)));
 #endif


    //DrawActive
    DrawCheckBox(ALARM_ON_x,ALARM_ON_y,alarm_active==1?1:0);

#ifdef ELKA
  if(curpos==1) DrawCursor(ALARM_ON_x+4,ALARM_ON_x+GetImgWidth(CBOX_CHECKED)*0.9-2,ALARM_ON_y+GetImgHeight(CBOX_CHECKED)*0.9);
#else
  //DrawCheckBox(x1,y1,alarm_active==1?1:0);
  if(curpos==1) DrawCursor(ALARM_ON_x,ALARM_ON_x+GetImgWidth(CBOX_CHECKED),ALARM_ON_y+GetImgHeight(CBOX_CHECKED));
#endif

  DrawCheckBox(REPEAT_ON_x,REPEAT_ON_y,auto_snooze==1?1:0);
#ifdef ELKA
  if(curpos==2) DrawCursor(REPEAT_ON_x+4,REPEAT_ON_x+GetImgWidth(CBOX_CHECKED)*0.9-2,REPEAT_ON_y+GetImgHeight(CBOX_CHECKED)*0.9);
#else
  if(curpos==2) DrawCursor(REPEAT_ON_x,REPEAT_ON_x+GetImgWidth(CBOX_CHECKED),REPEAT_ON_y+GetImgHeight(CBOX_CHECKED));
#endif


}

void MoveCurItem(int i,int j)//j=0-right j=1-left
{
  switch(i)
  {
    case 0:
    {
      if(!edittime) curitem=!curitem;
      else
      {
        if(j)
        {
          if(timepos>0) timepos--; else timepos=3;
        }
        else
        {
          if(timepos<3) timepos++; else timepos=0;
        }
      }
    }
    break;
    case 1:
    case 2: curitem=0;        break;
    case 3: curitem=!curitem; break;
    case 4:
      //if(pressed ENTER_BUTTON) curitem++;

      if(j)
      {
        if(curitem>0) curitem--; else curitem=6;
      }
      else
      {
        if(curitem<6) curitem++; else curitem=0;
      }
      break;
  }
}

void EnterButton()
{
  switch(curpos)
  {
    case 0: edittime=!edittime;            break;
    case 1: alarm_active=!alarm_active;   break;
    case 2: auto_snooze=!auto_snooze;    break;
    case 3: editsnooze=!editsnooze;        break;
    case 4: days[curitem]=!days[curitem];  break;
  }
  DrawWindow ();
}

void EditTime(int key)
{
 switch(timepos)
 {
   case 0:  //hours*10
     {
  //    int hour1=alarm_time.hour/10;
      int hour2=alarm_time.hour%10;
      if(key>=0 && key<=2)
      {
        alarm_time.hour=hour2+(key*10);
      }
     }
     break;
   case 1:  //hours
     {
       if(key>=0 && key<=9)
       {
         int hour1=alarm_time.hour/10;
         alarm_time.hour=(hour1*10)+key;
       }
     }
     break;
   case 2: //min*10
     {
      int hour2=alarm_time.min%10;
      if(key>=0 && key<=6)
      {
        alarm_time.min=hour2+(key*10);
      }
     }
     break;
   case 3:
     {
       if(key>=0 && key<=9)
       {
         int hour1=alarm_time.min/10;
         alarm_time.min=(hour1*10)+key;
       }
     }
     break;
   }
   MoveCurItem(curpos,0);

   DrawWindow ();
}

void EditSnooze (int key)
{
 switch(curitem)
 {
   case 0:  //hours*10
     {
  //    int hour1=alarm_time.hour/10;
      int hour2=snooze_time%10;
      if(key>=0 && key<=5)
      {
        snooze_time=hour2+(key*10);
      }
     }
     break;
   case 1:  //hours
     {
       if(key>=0 && key<=9)
       {
         if(key<2 && snooze_time<10) snooze_time=2;
          else
         { int hour1=snooze_time/10;
         snooze_time=(hour1*10)+key;
         }
       }
     }
     break;
  }
  MoveCurItem(curpos,0);
  DrawWindow ();
}
