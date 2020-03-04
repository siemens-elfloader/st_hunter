
int sf_h, lbf_h, mf_h;


typedef struct
{
  GUI gui;
  WSHDR *ws;
  int ID;
}SSAVER_GUI;
SSAVER_GUI *ssv_gui;

extern const char *weekdays[];
extern const char *months[];
extern TDate date;
extern TTime time;
extern void Start_Refresh_Timer();
extern void Stop_Refresh_Timer();


GBSTMR ssvtmr;

void Refresh_Screen_Saver()
{
  Stop_SSV_Timer();
  if(!ssv_gui->ID) return;
  int f;
  LockSched();
  f=IsGuiOnTop(ssv_gui->ID);
  UnlockSched();
  if (f) REDRAW();
  GBS_StartTimerProc(&ssvtmr, TMR_SEC*20, Refresh_Screen_Saver);
}

void ssvOnRedraw(SSAVER_GUI *data)
{
  GetDateTime(&date,&time);
  DrawRectangle(0,0,scr_w,scr_h,0, black, black);
  
  wsprintf(data->ws, "%d:%02d", time.hour,time.min);
  DrawString(data->ws, 0,Dt.h/2-lbf_h, Dt.w,Dt.h/2, FONT_LARGE_BOLD,TEXT_ALIGNMIDDLE,white,NULL);
  
  wsprintf(data->ws, "%t\n%02d, %t\n%d%t", weekdays[GetWeek(&date)], date.day, months[date.month-1], date.year, "г" );
  DrawString(data->ws, 0,Dt.h-4*mf_h, Dt.w,Dt.h, FONT_MEDIUM,TEXT_ALIGNMIDDLE,white,NULL);
}


void ssvOnCreate(SSAVER_GUI *data, void *(*malloc_adr)(int))
{
  KbdLock();
  Stop_Refresh_Timer();
  SetIllumination(0,1,0,0);
  sf_h=GetFontYSIZE(FONT_SMALL);
  mf_h=GetFontYSIZE(FONT_MEDIUM);
  lbf_h=GetFontYSIZE(FONT_LARGE_BOLD);
//   if(data->type) data->max=(scr_h*2/3-YDISP-3*sf_h)/sf_h;
//   else data->max=(scr_h-YDISP-2*sf_h)/sf_h;
  data->ws=AllocWS(256);
//   data->ws2=AllocWS(256);
//   ews=AllocWS(256);
//   if(IsTimerProc(&ssvtmr))
//   {
// 	GBS_StopTimer(&ssvtmr);
// 	GBS_DelTimer(&ssvtmr);
//   }
//   Stop_SSV_Timer();
  Refresh_Screen_Saver();
  data->gui.state=1;
}

void ssvOnClose(SSAVER_GUI *data, void (*mfree_adr)(void *))
{
  KbdUnlock();
  Start_Refresh_Timer();
  Start_SSV_Timer();
//   if(IsTimerProc(&ssvtmr))
//   {
// 	GBS_StopTimer(&ssvtmr);
// 	GBS_DelTimer(&ssvtmr);
//   }
//   Free_FLIST();
  FreeWS(data->ws);
//   FreeWS(data->ws2);
//   FreeWS(ews);
  data->gui.state=0;
//   SUBPROC((void *)ElfKiller);
}

void ssvOnFocus(SSAVER_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
//   if(IsTimerProc(&ssvtmr))
//   {
// 	GBS_StopTimer(&ssvtmr);
// 	GBS_DelTimer(&ssvtmr);
//   }
  Refresh_Screen_Saver();
  DisableIDLETMR();
  #ifdef ELKA
  DisableIconBar(1);
  #endif
  data->gui.state=2;
}

void ssvOnUnfocus(SSAVER_GUI *data, void (*mfree_adr)(void *))
{
  if(IsTimerProc(&ssvtmr))
  {
	GBS_StopTimer(&ssvtmr);
	GBS_DelTimer(&ssvtmr);
  }
//   Refresh_Screen_Saver();
  #ifdef ELKA
  DisableIconBar(0);
  #endif
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


int ssvOnKey(SSAVER_GUI *data, GUI_MSG *msg)
{
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
	switch(msg->gbsmsg->submess)
	{
	  case 24:
		return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
	  case ENTER_BUTTON:
	  case UP_BUTTON:
	  case DOWN_BUTTON:
	  case LEFT_BUTTON:
	  case RIGHT_BUTTON:
	  case LEFT_SOFT:
	  case RIGHT_SOFT:
		SetIllumination(0,1,30,0);
	}
  }
  if (msg->gbsmsg->msg==KEY_UP)
  {
	switch(msg->gbsmsg->submess)
	{
	  case ENTER_BUTTON:
	  case UP_BUTTON:
	  case DOWN_BUTTON:
	  case LEFT_BUTTON:
	  case RIGHT_BUTTON:
	  case LEFT_SOFT:
	  case RIGHT_SOFT:
		SetIllumination(0,1,0,0);
	}
  }
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
	switch(msg->gbsmsg->submess)
	{
	  case GREEN_BUTTON:
		return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
	}
  }
//   DirectRedrawGUI();
  return (0);
}


int ssvmethod8(void){return(0);}

int ssvmethod9(void){return(0);}

const void * const ssvgui_methods[11]={
  (void *)ssvOnRedraw,	//Redraw
  (void *)ssvOnCreate,	//Create
  (void *)ssvOnClose,	//Close
  (void *)ssvOnFocus,	//Focus
  (void *)ssvOnUnfocus,	//Unfocus
  (void *)ssvOnKey,	//OnKey
  0,
  (void *)kill_data, //method7,	//Destroy
  (void *)ssvmethod8,
  (void *)ssvmethod9,
  0
};

RECT SSVCanvas={0,0,0,0};

void Start_Screen_Saver()
{
  LockSched();
  ssv_gui=malloc(sizeof(SSAVER_GUI));
  zeromem(ssv_gui,sizeof(SSAVER_GUI));
  SSVCanvas.x2=ScreenW()-1;
  SSVCanvas.y2=ScreenH()-1;
  ssv_gui->gui.canvas=(void *)(&SSVCanvas);
  ssv_gui->gui.flag30=2;
  ssv_gui->gui.methods=(void *)ssvgui_methods;
  ssv_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  ssv_gui->ID=CreateGUI(ssv_gui);
  UnlockSched();
}
