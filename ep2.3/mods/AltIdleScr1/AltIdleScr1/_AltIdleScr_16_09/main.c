#include "..\inc\swilib.h"
#include "draw.c"
#include "ReadCFG.c"
#include "main.h"
//#include "fileman.c"
//#include "menu_desc.c"
//#include "relocated.c"

#define TMR_SECOND 216

//#pragma swi_number=0x80CF: RamAlarm
//__swi __arm int *RamAlarm;

const int minus11=-11;
//extern void ShowBMmenu1(void);
unsigned short maincsm_name_body[140];

int Cross_x=50, Cross_y=50;
int DrawMamory=0;
int TypeLine=0;
int kdblock=0;
int moveclock=0;


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

int fl[48]/*={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}*/;
#define FLAG_NET_STD fl[0]
#define FLAG_NET_ADV fl[1]
#define FLAG_NET_DB  fl[2]
#define FLAG_NET_EXT fl[3]

#define FLAG_BAT_STD  fl[4]
#define FLAG_BAT_EXT  fl[5]
#define FLAG_BAT_CAP  fl[6]
#define FLAG_BAT_VOLT fl[7]

#define FLAG_CSM fl[8]
//#define FLAG_NET_STD fl[9]
//#define FLAG_NET_STD fl[10]
int Pusk=0;
int menu=1;
int startmenu=0;

//=============================================================================
GBSTMR tmr_vibra;
volatile int vibra_count=1;

void start_vibra(void)
{
  void stop_vibra(void);
  if(!IsCalling())
  {
    unsigned int vibraPower=10;
    SetVibration(vibraPower);
    GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,stop_vibra);
  }
}

void stop_vibra(void)
{
  SetVibration(0);
  if (--vibra_count)
  {
    GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,start_vibra);
  }
}
//=============================================================================
void BG1()
{
 DrawBG();
 if(crd[clock][0].show) DrawClock (crd[clock][0].x, crd[clock][0].y);
 if(crd[BattCap][0].show)  DrawBattCap (crd[BattCap][0].x, crd[BattCap][0].y);
 if(crd[BattVolt][0].show) DrawBattVolt (crd[BattVolt][0].x, crd[BattVolt][0].y);
// if(crd[Dialogs].show)  DrawDialogs (crd[Dialogs].x, crd[Dialogs].y);
 if(crd[FreeRam][0].show)  DrawFreeRam(crd[FreeRam][0].x,crd[FreeRam][0].y); 
  
  if(!startmenu && !kdblock)
  {
    if(Cross_x>131)Cross_x=131;
    else if(Cross_y>175)Cross_y=175;
    else if(Cross_x<1)Cross_x=1;
    DrawCross(Cross_x,Cross_y,white);
  }
  
  
  if(Cross_x<=17 && Cross_y<=(name[0].NumItems*20) && !(startmenu))
  {
    DrawPanel_w();
    DrawNameContr();
  }
 // if(Cross_x>=40 && Cross_x<=132 && Cross_y<=17)DrawPanel_h();
  else GBS_DelTimer(&timer);
 DrawStrInt(workdesctop,120,158);  
  
 if(kdblock) DrawStrChar("KeyBoard Lock",25,40);
 
 if(menu<1)menu=6;
 else if(menu>6)menu=1;
 if(startmenu)
 {
   DrawMenu(menu);
   DrawNameMenu(menu);
 }
 else GBS_DelTimer(&menu_tmr);
}
//--------------------
void BG3()
{
 DrawBG();
 if(crd[clock][workdesctop-1].show) DrawClock (crd[clock][workdesctop-1].x, crd[clock][workdesctop-1].y);
 //if(crd[BattCap].show)  DrawBattCap (crd[BattCap].x, crd[BattCap].y);
// if(crd[BattVolt].show) DrawBattVolt (crd[BattVolt].x, crd[BattVolt].y);
// if(crd[Dialogs].show)  DrawDialogs (crd[Dialogs].x, crd[Dialogs].y);
 if(crd[FreeRam][workdesctop-1].show)  DrawFreeRam(crd[FreeRam][workdesctop-1].x,crd[FreeRam][workdesctop-1].y); 
  
  if(!startmenu && !kdblock)
  {
    if(Cross_x>131)Cross_x=131;
    else if(Cross_y>175)Cross_y=175;
    else if(Cross_x<1)Cross_x=1;
    DrawCross(Cross_x,Cross_y,white);
  }
  
  
  if(Cross_x<=17 && Cross_y<=(name[0].NumItems*20) && !(startmenu))
  {
    DrawPanel_w();
    DrawNameContr();
  }
 // if(Cross_x>=40 && Cross_x<=132 && Cross_y<=17)DrawPanel_h();
  else GBS_DelTimer(&timer);
  
  
 if(kdblock) DrawStrChar("KeyBoard Lock",25,40);
  DrawStrInt(workdesctop,120,158);

 if(menu<1)menu=6;
 else if(menu>6)menu=1;
 if(startmenu)
 {
   DrawMenu(menu);
   DrawNameMenu(menu);
 }
 else GBS_DelTimer(&menu_tmr);
}

//------------------------------------------------
void OnRedraw(MAIN_GUI *data)
{
LockSched();
if(workdesctop==2)
{
 DrawBG();
//DrawNetExt(2,2);
//DrawNetStd(NetStd_x,NetStd_y);
//DrawNetAdv (crd[NetAdv].x, crd[NetAdv].y);
//DrawNetDb ( crd[NetDB].x, crd[NetDB].y);
 if(crd[clock][workdesctop-1].show) DrawClock (crd[clock][workdesctop-1].x, crd[clock][workdesctop-1].y);
//DrawBattStd(BattStd_x,BattStd_y);
 if(crd[BattCap][workdesctop-1].show)  DrawBattCap (crd[BattCap][workdesctop-1].x, crd[BattCap][workdesctop-1].y);
 if(crd[BattVolt][workdesctop-1].show) DrawBattVolt (crd[BattVolt][workdesctop-1].x, crd[BattVolt][workdesctop-1].y);
 if(crd[Dialogs][workdesctop-1].show)  DrawDialogs (crd[Dialogs][workdesctop-1].x, crd[Dialogs][workdesctop-1].y);
 if(crd[FreeRam][workdesctop-1].show)  DrawFreeRam(crd[FreeRam][workdesctop-1].x,crd[FreeRam][workdesctop-1].y); 
 
 DrawStrInt(workdesctop,120,158);//Номер раб.стола

if(Cross_x<10)
 DrawStrInt(Cross_x,88,158);
else if(Cross_x<100)
 DrawStrInt(Cross_x,82,158);
else DrawStrInt(Cross_x,76,158);
 DrawStrChar(":",95,158);
 DrawStrInt(Cross_y,100,158);
 

  if(!startmenu && !kdblock)
  {
    if(Cross_x>131)Cross_x=131;
    else if(Cross_y>175)Cross_y=175;
    else if(Cross_x<1)Cross_x=1;
    DrawCross(Cross_x,Cross_y,white);
  }
  
  
  if(Cross_x<=17 && Cross_y<=(name[0].NumItems*20) && !(startmenu))
  {
    DrawPanel_w();
    DrawNameContr();
  }
 // if(Cross_x>=40 && Cross_x<=132 && Cross_y<=17)DrawPanel_h();
  else GBS_DelTimer(&timer);
  
 if(IRDA)DrawImg(120,60,(int)"0:\\zbin\\img\\784.png");
 if(kdblock)
 {
   DrawStrChar("KeyBoard Lock",25,40);
   DrawImg(115,40,(int)"0:\\zbin\\img\\785.png");
 }
 
 if(IsGPRSEnabled())DrawImg(120,50,(int)"0:\\zbin\\img\\792.png");
 else if(IsGPRSConnected())DrawImg(120,50,(int)"0:\\zbin\\img\\779.png");

 //DrawStrChar(descmenu[1].file,20,60);
 if(menu<1)menu=6;
 else if(menu>6)menu=1;
 if(startmenu)
 {
   DrawMenu(menu);
   DrawNameMenu(menu);
 }
 else GBS_DelTimer(&menu_tmr);
 
//if(DrawMamory)DrawFreeMamory("0:\\Java\\jam\\Applications",100);
/*if(Cross_x>=0 && Cross_x <=25 && Cross_y>=159 && Cross_y <=162)Pusk=1;
if(Pusk && Cross_x>=0 && Cross_x <=80 && Cross_y>=75 && Cross_y <=162)//0,80,75,162
    DrawPusk(85,1);
else Pusk=0;*/
//TimerProc();
}

else if(workdesctop==1) BG1();
else if(workdesctop==3) BG3();

UnlockSched();
}

void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  DisableIDLETMR();
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{ DirectRedrawGUI();
  if (msg->gbsmsg->msg==/*LONG_PRESS*/KEY_DOWN && msg->gbsmsg->submess=='#') kdblock=!kdblock; 
  if (msg->gbsmsg->msg==KEY_DOWN && !kdblock)
  {
    switch(msg->gbsmsg->submess)
    {
   
    case RED_BUTTON:case '0': return(1);
    case '*': {for (int i=7; i!=MAINCSM_ID-1; i++){ if (FindCSMbyID(i)!=NULL) CloseCSM(i);} ShowMSG(1,(int)"Killed");}  break;
    case GREEN_BUTTON: MakeVoiceCall("0",0x10,0x20C0);   break;
    case RIGHT_SOFT:
     // open_fm();//Запуск файл менеджера :)
       break;
    case LEFT_SOFT:startmenu=!startmenu; break;
    case '4': case LEFT_BUTTON:
      if(Cross_x>0 && (!startmenu)) Cross_x--;TypeLine=1;
      if(startmenu)menu--;
      if(moveclock)crd[clock][workdesctop-1].x--;
      break;
    case '6': case RIGHT_BUTTON: 
      if(Cross_x<131&& (!startmenu))Cross_x++;TypeLine=2;
      if(startmenu)menu++;
      if(moveclock)crd[clock][workdesctop-1].x++;
      break;
    case '2': case UP_BUTTON:    
      if(Cross_y>0&& (!startmenu))  Cross_y--;TypeLine=3;
      if(moveclock) crd[clock][workdesctop-1].y--;
      break;
    case '8': case DOWN_BUTTON:  
      if(Cross_y<175&& (!startmenu))Cross_y++;TypeLine=4;
      if(moveclock)crd[clock][workdesctop-1].y++;
      break;
    case '5': case ENTER_BUTTON: StartFilePanel_w();
                                 if(startmenu)StartFileMenu(menu);
                                // StartFilePanel_h();
                                // StartFilePusk();
                                 break;
    case '1':start_vibra();//SetVibration(0);
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
      if(Cross_x<131&& (!startmenu)) Cross_x+=speed_cur;
      if(startmenu)menu++;/*DrawCross(Cross_x-7,Cross_y,white);*/ 
       if(moveclock)crd[clock][workdesctop-1].x+=speed_cur;
      break;
    case '2': case UP_BUTTON:   
      if(Cross_y>0&& (!startmenu))   Cross_y-=speed_cur;/*DrawCross(Cross_x,Cross_y+7,white);*/
       if(moveclock)crd[clock][workdesctop-1].y-=speed_cur;
      break;
    case '8': case DOWN_BUTTON: 
      if(Cross_y<175&& (!startmenu)) Cross_y+=speed_cur;/*DrawCross(Cross_x,Cross_y-7,white);*/
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
#ifdef NEWSGOLD
void kill_data(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  mfree_adr(data);
}
#else
extern void kill_data(void *p, void (*func_p)(void *));
#endif
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
  (void *)kill_data,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,131,172};
void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  MAINGUI_ID=csm->gui_id;
}

void ElfKiller(void)
{ GBS_DelTimer(&timer);
  GBS_DelTimer(&menu_tmr);
  extern void kill_data(void *p, void (*func_p)(void *));
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{GBS_DelTimer(&menu_tmr);
  GBS_DelTimer(&timer);
  SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  return(1);
}

const struct
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

void UpdateCSMname(void)
{
  WSHDR *ws=AllocWS(256);
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"exe");
  FreeWS(ws);
}


int main(char *exename, char *fname)
{
  char dummy[sizeof(MAIN_CSM)];
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  UpdateCSMname();

  LoadAllConfig();
  return 0;
}
