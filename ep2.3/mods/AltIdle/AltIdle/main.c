#include "..\inc\swilib.h"
#include "draw.c"
//#include "ReadCFG.c"
#include "main.h"

//-----------------------------------------------------//

const int minus11=-11;
unsigned short maincsm_name_body[140];

unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;


//----------------------------------//
void *canvasdata;

static void OnRedraw(MAIN_GUI *data)//эта функция нужна для перерисовки экрана,тут рисуем все что нам нужно
{
 canvasdata=BuildCanvas();
 DrawCanvas(canvasdata,0,0,ScreenW()-1,ScreenH()-1,1);
 DrawImg(0,0,(int)"0:\\shell\\Idle_Wallpaper.png");
 DrawNetExt(NetExt_x,NetExt_y);//2,2
// DrawNetStd(2,100);
 //DrawNetAdv (crd[NetAdv][1].x, crd[NetAdv][1].y);
 DrawNetDb(NetDb_x,NetDb_y);//2,10
 DrawClock(Clock_x,Clock_y);//2,ScreenH()-15
 DrawBattCap(BattCap_x,BattCap_y);//203.10
// DrawBattVolt(BattVolt_x,BattVolt_y)//196.26;
 DrawDialogs(Dialogs_x,Dialogs_y);//192.290
 DrawFreeRam(FreeRam_x,FreeRam_y);//6,ScreenH()-30
 DrawBattStd(DattStd_x,BattStd_y);//142.2
// DrawWorkDesc();
 DrawCross(Cross_x,Cross_y,blue);
 
 if(Cross_x<10)
   DrawStrInt(Cross_x,88+114,ScreenH()-15);
 else if(Cross_x<100)
   DrawStrInt(Cross_x,82+114,ScreenH()-15);
 else DrawStrInt(Cross_x,76+114,ScreenH()-15);
   DrawStrChar(":",95+114,ScreenH()-15);
 DrawStrInt(Cross_y,214,ScreenH()-15);
 

  
  if(Cross_x<=35 && Cross_y<=205 && !(startmenu))
  {
    DrawPanel_w();
    DrawNameContr();
  }
 // if(Cross_x>=40 && Cross_x<=132 && Cross_y<=17)DrawPanel_h();
  else GBS_DelTimer(&timer);
  //DrawIconBar();
 /*
  if(Cross_x<=35 && Cross_y>=190 && Cross_y<=320 && !(startmenu))
  {
    DrawPusk(50,1);
  }*/

 //DrawStrChar(descmenu[1].file,20,60);
 if(menu<1)menu=12;
 else if(menu>12)menu=1;
 if(startmenu)
 {
   DrawMenu(menu);
   DrawNameMenu(menu);
 }
 else GBS_DelTimer(&menu_tmr);
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))//Тут делаем что либо при создании гуи
{
  data->ws1=AllocWS(256);//Выделяем память под строку
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))//Сдесь делаем что нить при закрытии гуи:)
{
  data->gui.state=0;
  FreeWS(data->ws1);//например, освобождаем память
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))//Если гуи на верху то опять же делаем что нужно
{
#ifdef ELKA//Если елка,то отключаем иконбар
  DisableIconBar(1);
#endif
  data->gui.state=2;
  DisableIDLETMR();//отключаем таймер,иначе через 2 минуты эльф закроется
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))//а если гуи не вверху,то можно закрыть эльф например:)
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)//Ну это кей хук
{
   DirectRedrawGUI();  
  
  if (msg->gbsmsg->msg==/*LONG_PRESS*/KEY_DOWN && msg->gbsmsg->submess=='#') kdblock=!kdblock; 
  if (msg->gbsmsg->msg==KEY_DOWN && !kdblock)
  {
    switch(msg->gbsmsg->submess)
    {   
    case RED_BUTTON:case '0':return 1;
    case LEFT_SOFT:startmenu=!startmenu; break;
    case '4': case LEFT_BUTTON:
      if(Cross_x>0 && (!startmenu)) Cross_x--;TypeLine=1;
      if(startmenu)menu--;
      if(moveclock)crd[clock][workdesctop-1].x--;
      break;
    case '6': case RIGHT_BUTTON: 
      if(Cross_x<(ScreenW()-1)&& (!startmenu))Cross_x++;TypeLine=2;
      if(startmenu)menu++;
      if(moveclock)crd[clock][workdesctop-1].x++;
      break;
    case '2': case UP_BUTTON:    
      if(Cross_y>0&& (!startmenu))  Cross_y--;TypeLine=3;
      if(moveclock) crd[clock][workdesctop-1].y--;
      break;
    case '8': case DOWN_BUTTON:  
      if(Cross_y<(ScreenH()-1)&& (!startmenu))Cross_y++;TypeLine=4;
      if(moveclock)crd[clock][workdesctop-1].y++;
      break;
    case '5': case ENTER_BUTTON:
      StartFilePanel_w();
      if(startmenu)StartFileMenu(menu);
      // StartFilePanel_h();
      // StartFilePusk();
      if(Cross_x>=121 && Cross_x<=128 && Cross_y>=130 && Cross_y<=143)workdesctop=1;
      else if(Cross_x>=121 && Cross_x<=128 && Cross_y>=144 && Cross_y<=157)workdesctop=2;
      else if(Cross_x>=121 && Cross_x<=128 && Cross_y>=158 && Cross_y<=170)workdesctop=3;
      break;
    case '1'://vibra_count=1;start_vibra();//SetVibration(0);
      if(Cross_x>=crd[clock][workdesctop-1].x && Cross_x<=crd[clock][workdesctop-1].x+20 
         && Cross_y>=crd[clock][workdesctop-1].y && Cross_y<=crd[clock][workdesctop-1].y+7)
      moveclock=!moveclock; 
      break;
    // case '7':  if(Cross_y<172)Cross_y+=18;break;
  //  case '3':ShowBMmenu1();break;
    case '7':if(workdesctop>1) workdesctop--; else workdesctop=3;break;
    case '9':if(workdesctop<3) workdesctop++; else workdesctop=1;break;    
    }    
  }
  if (msg->gbsmsg->msg==LONG_PRESS && !kdblock)
  {
    switch(msg->gbsmsg->submess)
    {
    
    case '4': case LEFT_BUTTON: 
      if(Cross_x>0&& (!startmenu))Cross_x-=speed_cur;
      if(startmenu)menu--;/*DrawCross(Cross_x+7,Cross_y,white);*/ 
      if(moveclock)crd[clock][workdesctop-1].x-=speed_cur;
      break;
    case '6': case RIGHT_BUTTON:
      if(Cross_x<(ScreenW()-1)&& (!startmenu)) Cross_x+=speed_cur;
      if(startmenu)menu++;/*DrawCross(Cross_x-7,Cross_y,white);*/ 
      if(moveclock)crd[clock][workdesctop-1].x+=speed_cur;
      break;
    case '2': case UP_BUTTON:   
      if(Cross_y>0&& (!startmenu))   Cross_y-=speed_cur;/*DrawCross(Cross_x,Cross_y+7,white);*/
      if(moveclock)crd[clock][workdesctop-1].y-=speed_cur;
      break;
    case '8': case DOWN_BUTTON: 
      if(Cross_y<(ScreenH()-1)&& (!startmenu)) Cross_y+=speed_cur;/*DrawCross(Cross_x,Cross_y-7,white);*/
      if(moveclock)crd[clock][workdesctop-1].y+=speed_cur;
      break;
    case '1': if(Cross_x>0 && Cross_y>0 && (!startmenu)){ Cross_x-=speed_cur;Cross_y-=speed_cur; }
              else if (Cross_y>0)Cross_y-=speed_cur; 
              else if (Cross_x>0)Cross_x-=speed_cur; break;
    case '3': if(Cross_x<131 && Cross_y>0&& (!startmenu)){ Cross_x+=speed_cur;Cross_y-=speed_cur;}
              else if (Cross_y>0)Cross_y-=speed_cur; 
              else if (Cross_x<131)Cross_x+=speed_cur;break;
    case '7': if(Cross_x>0 && Cross_y<175&& (!startmenu)){Cross_x-=speed_cur;Cross_y+=speed_cur;}
              else if (Cross_y<175)Cross_y+=speed_cur; 
              else if (Cross_x>0)Cross_x-=speed_cur;break;
    case '9': if(Cross_x<131 && Cross_y<175&& (!startmenu)){Cross_x+=speed_cur;Cross_y+=speed_cur;}
              else if (Cross_y<175)Cross_y+=speed_cur; 
              else if (Cross_x<131)Cross_x+=speed_cur;break;
    case ENTER_BUTTON: case '5':break;
    
   }
    
  }  
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));//Эта функция убивает эльф и освобождает память

int method8(void){return(0);}
int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)OnRedraw,
  (void *)onCreate,
  (void *)onClose,
  (void *)onFocus,
  (void *)onUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data,
  (void *)method8,
  (void *)method9,
  0
};

static void maincsm_oncreate(CSM_RAM *data)//Вызывается при открытии эльфа,делаем что нужно
{
  //тут хитрые махинации для получения ид гуя чтобы мона было с ним манипулировать
  static const RECT Canvas={0,0,0,0};
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
}

void ElfKiller(void)//освобождаем все занятое место эльфом в оперативе
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)//функция закрытия эльфа,тут освобождаем памчть если нужно и т.п.
{
  GBS_DelTimer(&menu_tmr);
  GBS_DelTimer(&timer);
  SUBPROC((void *)ElfKiller);
}


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

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)//тут вообщем протекают все сообщения поступаемые из эльфа так сказать:)
{
  //сообщение о реконфигурации например,ипц и т.д. и т.п.
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }

  return(1);
}


static const struct//это структура цсм,разбираться не надо:)
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
  sizeof(MAIN_CSM),
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

void UpdateCSMname(void)//ну собственно имя эльфа передаваемое хтаску
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"example");
}


int main(char *exename, char *fname)//exename-путь запускаемого эльфа, fname параметр передаваемый эльфу
{
  MAIN_CSM main_csm;
  LockSched();
  UpdateCSMname();//обновляем имя для хтаска
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);//создаем цсм
  UnlockSched();
  return 0;
}
