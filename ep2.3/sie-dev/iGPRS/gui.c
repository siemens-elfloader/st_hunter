#include "..\inc\swilib.h"
//#include "string_util.h"
#include "GPRS MAN.h"
#include "main.h"
#include "gui.h"
#include "..\inc\csm.h"
#include "..\inc\str.h"
#include "rect_patcher.h"

const CSM_DESC maincsm;

int my_csm_id;

int n = 5;  // Отступ от текста
int top_x = 10; // Отступ от левого края для верхней надписи
int top_y = -1; // Отступ от верха для верхней надписи
int stat_x = 58;  // Отступ от левого края для картинки статуса GPRS
int stat_y = 105; // Отступ от верха для картинки статуса GPRS
int text_x = 15;  // Отступ текст слева
int text_y = 40l; // Отстуа текст сверху
int all_x = 83; // Коодинаты надписи Всего
int all_y = 105;
int ses_x = 9;  // Координаты надписи Сессия
int ses_y = 105;
int time_y = 160; // Положение времени
char perc_gui[10] = "%t";

MAIN_GUI *MYGUI;
int CLOSE = 1;
#define TOTAL_ICONS 3

char * S_ICONS[TOTAL_ICONS];

static const char * const icons_names[TOTAL_ICONS]=
{
  "background.png", // 0
  "net_on.png", // 1
  "net_off.png",  // 2
};

//extern void ShowStat(char *ses_traf, char *all_traf, char *ses_money, char *all_money , char *time, char *maxtime, char *srtime, int num, int update);
  int numb;
  char s_traf[128], a_traf[128], s_money[128], a_money[128], tm[128], mt[128], sr[128];
  int up = 0;
  int XX, YY;
  
void setup_ICONS(void)
{
  int i=0;
  do
  {
    if (!S_ICONS[i]) S_ICONS[i]=MakeGlobalString(SEE_FOLDER,'\\',icons_names[i]);
    i++;
  }
  while(i<TOTAL_ICONS);
  
  return;
}

void free_ICONS(void)
{
  int i=0;
  do
  {
    mfree((void*)S_ICONS[i]);
    S_ICONS[i]=0;
    i++;
  }
  while(i<TOTAL_ICONS);
  up = 0;
  return;
}
 
void Redraw(MAIN_GUI *data)
{
  int scr_w=ScreenW();
  int scr_h=ScreenH();
  int y;
  int i = 0;
       if (!up)
       {
         up = 1;
         setup_ICONS(); // Создаем список иконок
         XX = scr_w - GetImgWidth((int)S_ICONS[0]);  // Расстояние относительно картинки
         YY = scr_h - GetImgHeight((int)S_ICONS[0]);  // Расстояние относительно картинки
         if (scr_h > 176)
         {
           stat_y = 105 * 1.8;
           text_y = 41 * 1.8;
           all_y = stat_y;
           ses_y = stat_y;
         }
       }

       MYGUI = data;

  DrawImg(XX/2,YY/2,(int)S_ICONS[0]); // Фон
  if (!png_on_gui)
    strcpy(perc_gui,"%t");
  else
    strcpy(perc_gui,"%s");
  if (IsGPRSConnected())
  {
    i = 1;
    DrawImg(XX+stat_x,YY+stat_y,(int)S_ICONS[1]);  // Состояние GPRS
    wsprintf(data->ws1,perc_gui,SES_GPRS_ON);
  }
  else
  {
    i = 2;
    DrawImg(XX+stat_x,YY+stat_y,(int)S_ICONS[2]);  
    wsprintf(data->ws1,perc_gui,SES_GPRS_OFF);
  }
if (!png_on_gui)
    DrawString(data->ws1,XX + top_x, YY + top_y, scr_w-1,scr_h-1,
               SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);  // Состояние GPRS
  else
    DrawPngFontToIDLE(0,data->ws1,XX + top_x,YY + top_y);
    //  Надпись Всего
   wsprintf(data->ws1,perc_gui,SES_ALL);
if (!png_on_gui)
   DrawString(data->ws1,XX + all_x,YY + all_y+(GetImgHeight((int)S_ICONS[i])-GetFontYSIZE(IDLE_TRAF_ALL_FONT))/2,scr_w-1,scr_h-1,
              SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
  else
    DrawPngFontToIDLE(0,data->ws1,XX + all_x,YY + all_y + (GetImgHeight((int)S_ICONS[i])-GetPngFontHeight())/2);
     // Надпись Сессия
   wsprintf(data->ws1,perc_gui,SES_SES);
if (!png_on_gui)
   DrawString(data->ws1,XX + ses_x,YY + ses_y+(GetImgHeight((int)S_ICONS[i])-GetFontYSIZE(IDLE_TRAF_FONT))/2,scr_w-1,scr_h-1,
              SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
  else
    DrawPngFontToIDLE(0,data->ws1,XX + ses_x,YY + ses_y+(GetImgHeight((int)S_ICONS[i])-GetPngFontHeight())/2);
    
  y = YY + text_y;

  //////////////////////////////////////////////////////////////////////////////
   char Message[128]=""/*,Messages[128]=""*/;

  wsprintf(data->ws1, perc_gui, GetSessionTime(GetSesInfo(1,0,0),SES_IT));                
if (!png_on_gui)
  DrawString(data->ws1,XX + text_x,y,scr_w-1,scr_h-1,
             SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
  else
    DrawPngFontToIDLE(0,data->ws1,XX + text_x,y);
  //////////////////////////////////////////////////////////////////////////////

    wsprintf(data->ws1, perc_gui, GetSessionTime(GetSesInfo(0,0,1),SES_SR)); 
if (!png_on_gui)    
    DrawString(data->ws1,XX + text_x,y+GetFontYSIZE(IDLE_SES_FONT)*2,scr_w-1,scr_h-1,
               SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
  else
    DrawPngFontToIDLE(0,data->ws1,XX + text_x,y+GetPngFontHeight()*2);   
   wsprintf(data->ws1, perc_gui, GetSessionTime(GetSesInfo(0,1,0),SES_MAX));  
if (!png_on_gui)  
   DrawString(data->ws1,XX + text_x,y+GetFontYSIZE(IDLE_SES_FONT)*3,scr_w-1,scr_h-1,
              SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
  else
    DrawPngFontToIDLE(0,data->ws1,XX + text_x,y+GetPngFontHeight()*3); 

  // Кол-во подключений

//      strcpy(Messages,);
      sprintf(Message, SES_NUM,session_num);
      wsprintf(data->ws1, perc_gui, Message);   
if (!png_on_gui) 
  DrawString(data->ws1,XX + text_x,y+GetFontYSIZE(IDLE_SES_FONT),scr_w-1,scr_h-1,
             SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
    else
    DrawPngFontToIDLE(0,data->ws1,XX + text_x,y+GetPngFontHeight()); 
  //////////////////////////////////////////////////////////////////////////////  // Деньги сессию
  wsprintf(data->ws1, perc_gui, GetCharMoney(GetSessionTraffic(1),1));  
if (!png_on_gui)     
  DrawString(data->ws1,XX + ses_x,
             YY + ses_y+(GetImgHeight((int)S_ICONS[i])-GetFontYSIZE(IDLE_TRAF_FONT))/2+(GetFontYSIZE(IDLE_TRAF_FONT)+n)*2,
             scr_w-1,scr_h-1,
             SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
    else
    DrawPngFontToIDLE(0,data->ws1,XX + ses_x,YY + ses_y+(GetImgHeight((int)S_ICONS[i])-GetPngFontHeight())/2+(GetPngFontHeight()+n)*2); 
  //////////////////////////////////////////////////////////////////////////////  Деньги за всеь трафик
  wsprintf(data->ws1, perc_gui, GetCharMoney(GetAllTraffic(1),1));  
if (!png_on_gui)       
  DrawString(data->ws1,XX + all_x,
             YY + all_y+(GetImgHeight((int)S_ICONS[i])-GetFontYSIZE(IDLE_TRAF_ALL_FONT))/2+(GetFontYSIZE(IDLE_TRAF_ALL_FONT)+n)*2,
             scr_w-1,scr_h-1,
             SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
    else
    DrawPngFontToIDLE(0,data->ws1,XX + all_x,YY + all_y+(GetImgHeight((int)S_ICONS[i])-GetPngFontHeight())/2+(GetPngFontHeight()+n)*2); 
  //////////////////////////////////////////////////////////////////////////////  // Травфик за сессию
  wsprintf(data->ws1, perc_gui, GetCharTraffic(GetSessionTraffic(0),0,1));  
if (!png_on_gui)     
  DrawString(data->ws1,XX + ses_x,
             YY + ses_y+(GetImgHeight((int)S_ICONS[i])-GetFontYSIZE(IDLE_TRAF_FONT))/2+GetFontYSIZE(IDLE_TRAF_FONT)+n,
             scr_w-1,scr_h-1,
             SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
    else
    DrawPngFontToIDLE(0,data->ws1,XX + ses_x,YY + ses_y+(GetImgHeight((int)S_ICONS[i])-GetPngFontHeight())/2+GetPngFontHeight()+n); 
  //////////////////////////////////////////////////////////////////////////////  // Общий трафик
   wsprintf(data->ws1, perc_gui, GetCharTraffic(GetAllTraffic(0),1,1));  
if (!png_on_gui)                
   DrawString(data->ws1,XX + all_x,
               YY + all_y+(GetImgHeight((int)S_ICONS[i])-GetFontYSIZE(IDLE_TRAF_ALL_FONT))/2+GetFontYSIZE(IDLE_TRAF_ALL_FONT)+n,
               scr_w-1,scr_h-1,
               SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
    else
    DrawPngFontToIDLE(0,data->ws1,XX + all_x,YY + all_y+(GetImgHeight((int)S_ICONS[i])-GetPngFontHeight())/2+GetPngFontHeight()+n); 
  //////////////////////////////////////////////////////////////////////////////
  
      TDate date; TTime time; 
      
      GetDateTime(&date,&time); 

//            strcpy(Messages,);               
	    sprintf(Message, IDLE_TIME_MES,time.hour,time.min,time.sec);
	    wsprintf(data->ws1, perc_gui, Message);  
if (!png_on_gui)     
  DrawString(data->ws1,(scr_w - Get_WS_width(data->ws1,IDLE_TIME_FONT)+1)/2,
                        YY+time_y,scr_w-1,scr_h-1,
                        SES_FONT,SES_FRINGING_ENABLE ? TEXT_OUTLINE : 0,SES_COLORS,SES_FRINGING_COLORS);
    else
    DrawPngFontToIDLE(0,data->ws1,(scr_w - GetPngFontWidth2(data->ws1))/2,YY+time_y); 
  //////////////////////////////////////////////////////////////////////////////

}

void Create(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->ws1=AllocWS(256);
  data->ws2=AllocWS(256);
  data->gui.state=1;
}

void Close(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  FreeWS(data->ws2);
  data->gui.state=0;
}

void Focus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  DisableIDLETMR();
  #ifdef ELKA    
  DisableIconBar(1);
  #endif  
  data->gui.state=2;
}

void Unfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{

//  RestartIDLETMR(); 
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
    case RIGHT_SOFT:
      {
          #ifdef ELKA    
          DisableIconBar(0);
          #endif  
          CLOSE = 1;
          return(1); //?????????? ????? GeneralFunc ??? ???. GUI -> ???????? GUI
      }
    }
    if (ses_ex_key_en)
    {
      if (msg->gbsmsg->submess == ses_ex_key_off)
        {
            if (IsGPRSEnabled())
            GPRS_OnOff(0,1);
        }
      else
      if (msg->gbsmsg->submess == ses_ex_key_on)      
        {
            if (!IsGPRSEnabled())
            GPRS_OnOff(1,0);
        }
      else
      if (msg->gbsmsg->submess == ses_ex_key_res)
        {
          if (IsGPRSEnabled())
          {
            GPRS_OnOff(0,1);
            GPRS_OnOff(1,0);
          }
        }
    }
/*    else
    {
//      return(0);
    }*/
      
/*    case LEFT_SOFT:
      {
          DirectRedrawGUI();
          method0(data);
          break; //?????????? ????? GeneralFunc ??? ???. GUI -> ???????? GUI
      }*/
    }
//  }

  return(0);
}

void Destroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
//  mfree_adr(data);
}

int method8(void){return(0);}

int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)Redraw,	//Redraw
  (void *)Create,	//Create
  (void *)Close,	//Close
  (void *)Focus,	//Focus
  (void *)Unfocus,	//Unfocus
  (void *)OnKey,	//OnKey
  0,
  (void *)Destroy,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,0,0};

void maincsm_oncreate2(CSM_RAM *data)
{
  MAIN_CSM *csm=(MAIN_CSM *)data;
    MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->gui_id=CreateGUI(main_gui);
}

void maincsm_onclose2(CSM_RAM *csm)
{
  my_csm_id=0;
}

int maincsm_onmessage2(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_CSM_DESTROYED)
  {
    RefreshGUI();
  }
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    //if (csm->show_csm==-1) RefreshGUI();
  }
  if (msg->msg==MSG_INCOMMING_CALL)
  {
    csm->csm.state=-3;
  }
  return(1);
}


const CSM_DESC maincsm=
{
  maincsm_onmessage2,
  maincsm_oncreate2,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  maincsm_onclose2,
  sizeof(MAIN_CSM),
  1,
  &minus11
};

void ShowStat(int update)
{ 
   MAIN_CSM main_csm;
/*   strcpy(s_traf,ses_traf);
   strcpy(a_traf,all_traf);
   strcpy(s_money,ses_money);
   strcpy(a_money,all_money);
   strcpy(tm,time);
   strcpy(mt,maxtime);
   strcpy(sr,srtime);*/
//   numb = num;

   if (!update)
   {
     patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
     if (!my_csm_id)
    {
      CLOSE = 0;
      my_csm_id=CreateCSM(&maincsm,&main_csm,2);
    }
   }
}
