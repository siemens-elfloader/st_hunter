#ifdef NEWSGOLD
#include "../inc/swilib.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "main.h"
#include "rect_patcher.h"
#include "screen_save.h"
#include "strings.h"

typedef struct
{
  GUI gui;
  WSHDR *ws;
}SCREEN_GUI;

int battery;
int iks,igrek;
char laiks[32];
TTime time;
int color_SS;
void Draw()
{
  WSHDR *ws=AllocWS(32);
  GetDateTime(NULL, &time);
  wsprintf(ws, "%02d:%02d", time.hour, time.min);
  DrawString(ws,iks-10,igrek-40,iks-10+Get_WS_width(ws,FONT_MEDIUM_BOLD)+2,igrek-40+GetFontYSIZE(FONT_MEDIUM_BOLD),FONT_SMALL,0,GetPaletteAdrByColorIndex(5),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

void Draw2()
{
  battery = *RamCap();
  iks=iks-10;
  igrek = igrek-7+GetFontYSIZE(11);
  WSHDR *ws=AllocWS(32);
  wsprintf(ws,"Bat:%d%", battery);
  DrawString(ws,ScreenW()-Get_WS_width(ws,FONT_SMALL_ITALIC_BOLD)-2,igrek,ScreenW()-1,igrek+GetFontYSIZE(FONT_SMALL_ITALIC_BOLD),
                FONT_SMALL_ITALIC_BOLD,TEXT_ALIGNRIGHT,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

extern int total_unread;
extern int total_unread2;

void Draw3()
{
  iks=iks-15;
  igrek = igrek-2+GetFontYSIZE(11);
  WSHDR *ws=AllocWS(32);
  wsprintf(ws, "Unread: %d/%d", total_unread, total_unread2);
  DrawString(ws,ScreenW()-Get_WS_width(ws,FONT_SMALL_ITALIC_BOLD)-2,igrek,ScreenW()-1,igrek+GetFontYSIZE(FONT_SMALL_ITALIC_BOLD),
             FONT_SMALL_ITALIC_BOLD,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

extern unsigned int UIN;

void Draw5()
{
  WSHDR *ws=AllocWS(32);
  wsprintf(ws, "NatICQ: %d", UIN);
  DrawString(ws,0,YDISP+15,ScreenW()-1,40+GetFontYSIZE(FONT_SMALL_ITALIC_BOLD),FONT_SMALL_ITALIC_BOLD,TEXT_ALIGNMIDDLE,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(23));
  FreeWS(ws);
}

//GBSTMR screen;
extern int saver_on;
extern int dace_unread;
char black[]={0x00,0x00,0x00,0x20};
void reftes()
{
  iks=95;
  igrek=130;
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  WSHDR *ws=AllocWS(128);
  
//  void *canvasdata=BuildCanvas();
//  DrawCanvas(canvasdata, 0, YDISP, ScreenW()-1,ScreenH()-1, 1 );
  DrawRectangle(0,YDISP,scr_w,scr_h,0,black,black); 
  char *sws=malloc(64);
  *(sws)=0;
  
  extern int away_m;
  extern int CurrentPrivateStatus;
  extern int Is_Vibra_Enabled;
  extern unsigned int Is_Sounds_Enabled;
  extern int Is_Double_Vibra_On;
  extern char away_msg_time_dace[];
  int y=YDISP+30;
  char ss[64];
  if(away_m){ sprintf(ss,"[A:%s]",away_msg_time_dace); strcat(sws,ss); }    
  if(CurrentPrivateStatus==1) { strcat(sws,"[I]");  }  
  if(Is_Vibra_Enabled)        { strcat(sws,"[V]");  }  
  if(Is_Sounds_Enabled)       { strcat(sws,"[S]");  }  
  if(Is_Double_Vibra_On)      { strcat(sws,"[Vr]"); }

  wsprintf(ws,"%s",sws);
  DrawString(ws,2,ScreenH()-GetFontYSIZE(FONT_SMALL_ITALIC_BOLD)-4,ScreenW(),ScreenH(),FONT_SMALL_ITALIC_BOLD,TEXT_ALIGNMIDDLE,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  
  extern int uptimeHour;
  extern int uptimeMin;
  extern int uptimeSec;
  extern char hostbuf2[128];
  extern char PINGO[256];
  extern int main2_count;
  extern int main_count;
  extern int msg_away_timer;
  extern int connect_state;
  if(connect_state==3)   
     wsprintf(ws,"Online: %02d:%02d:%02d\nServer: %s \n%s\n", uptimeHour, uptimeMin, uptimeSec, hostbuf2,PINGO);   
  else
     wsprintf(ws,"Offline\n");
     
  DrawString(ws,2,y,ScreenW()-1,y+GetFontYSIZE(FONT_SMALL_ITALIC_BOLD),FONT_SMALL_ITALIC_BOLD,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  if(connect_state==3)   
    y+=(GetFontYSIZE(FONT_SMALL_ITALIC_BOLD)*3)+5;
  else
    y+=(GetFontYSIZE(FONT_SMALL_ITALIC_BOLD)*1)+5;

  wsprintf(ws, "Send mess: %d \nRecv mess: %d \n", main2_count, main_count);
  DrawString(ws,2,y,ScreenW()-1,y+GetFontYSIZE(FONT_SMALL_ITALIC_BOLD),FONT_SMALL_ITALIC_BOLD,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  y+=(GetFontYSIZE(FONT_SMALL_ITALIC_BOLD)*2)+5;
   
  wsprintf(ws,"Not action %d sec",msg_away_timer*5);
  DrawString(ws,2,y,ScreenW()-1,y+GetFontYSIZE(FONT_SMALL_ITALIC_BOLD),FONT_SMALL_ITALIC_BOLD,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
  y+=(GetFontYSIZE(FONT_SMALL_ITALIC_BOLD)*1)+5;
   
  extern volatile unsigned long ALLTOTALRECEIVED;	//by BoBa 10.07
  extern volatile unsigned long ALLTOTALSENDED;
  extern volatile int sendq_l; //Длинна очереди для send
  unsigned long RX=ALLTOTALRECEIVED;
  unsigned long TX=ALLTOTALSENDED;	//sendq_l	
  extern int kopa_online;
  extern int kopa_kontaktu;
  wsprintf(ws, "Traffic: Rx: %db Tx: %db \nQueue: %db \n"
               "Online contact: %d [total: %d] \n",RX,TX,sendq_l,kopa_kontaktu, kopa_online);
  DrawString(ws,2,y,ScreenW()-1,y+(GetFontYSIZE(FONT_SMALL_ITALIC_BOLD)*3),FONT_SMALL_ITALIC_BOLD,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
      
//  Draw();
  Draw2();
  Draw3();
  Draw5();
  
  FreeWS(ws);
  mfree(sws);
//  GBS_StartTimerProc(&screen, 216*10, reftes);
}

static void method0(SCREEN_GUI *data)
{
  reftes();
}

static void method1(SCREEN_GUI *data,void *(*malloc_adr)(int))
{
  data->ws=AllocWS(50);
  data->gui.state=1;
  DrawRectangle(0,YDISP,ScreenW()-1,ScreenH()-1,0,black,black); 
  color_SS=0;
  reftes();
}

static void method2(SCREEN_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
  FreeWS(data->ws);
}

static void method3(SCREEN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))//fokus
{
//  char* iconbars = RamIconBar();
//  *iconbars = 0;
  saver_on=1;
  DisableIDLETMR();
  data->gui.state=2;
  reftes();
}

static void method4(SCREEN_GUI *data,void (*mfree_adr)(void *))//unfokus
{
//  char* iconbars = RamIconBar();
//  *iconbars = 1;
  saver_on=0;
//  GBS_DelTimer(&screen);
  if (data->gui.state!=2)
    return;
  data->gui.state=1;
}

static int method5(SCREEN_GUI *data,GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  int key=msg->gbsmsg->submess;
  if(m==KEY_DOWN)
  {
    if(key==UP_BUTTON)return 0;
    else return 1;
/*    if ((key>='0' || key<='9') || key==RIGHT_SOFT || key==RED_BUTTON)
    {
      return (1);
    }*/
  } 
  
  if(m==LONG_PRESS)
    {
      if (key=='#')
      { 
        return (1);
      }
    }
  DirectRedrawGUI();
  return(0);
}

static int method8(void){return(0);}

static int method9(void){return(0);}

extern void kill_data(void *p, void (*func_p)(void *));
static const void * const gui_methods[11]={
  (void *)method0,  //Redraw
  (void *)method1,  //Create
  (void *)method2,  //Close
  (void *)method3,  //Focus
  (void *)method4,  //Unfocus
  (void *)method5,  //OnKey
  0,
  (void *)kill_data, //method7, //Destroy
  (void *)method8,
  (void *)method9,
  0
};

int CreateScreenSelectGUI()
{
  saver_on=1;
//  char* iconbars = RamIconBar();
//  *iconbars = 0;
  static const RECT Canvas={0,0,0,0};
  SCREEN_GUI *screen_gui=malloc(sizeof(SCREEN_GUI));
  zeromem(screen_gui,sizeof(SCREEN_GUI));
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  screen_gui->gui.canvas=(void *)(&Canvas);
  screen_gui->gui.methods=(void *)gui_methods;
  screen_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  return CreateGUI(screen_gui);
}
#endif
