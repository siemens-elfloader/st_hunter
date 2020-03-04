
// RECT pop={2,50,100,200};
// unsigned short px=0,py=0;

// void popupp()
// {
// 	char ch=2;
// 	px+=20;
// 	py+=20;
// 	if(px>pop.x2-pop.x){ch--; px=pop.x2-pop.x;}
// 	if(py>pop.y2-pop.y){ch--; py=pop.y2-pop.y;}
// 	DrwImg(mfn, 0,0, NULL,NULL);
// 	bar();
// 	DrawRectangle(pop.x,pop.y,pop.x+px,pop.y+py,0,red,white0);
// 	DrwImg(cursor, cur.x,cur.y, NULL,NULL);
// 	if(ch) GBS_StartTimerProc(&inftmr, 5, popupp);
// 	else DirectRedrawGUI();
// }

// void chota()
// {
// 	DrawRectangle(cur.x,cur.y,cur.x+70,cur.y+90,0,red,white0);
// 	//DrwImg(cursor, cur.x+50,cur.y+50, NULL,NULL);
// }

extern PGUI_methods infgui;
const char *weekdays[7]={
	"Понедельник",
 "Вторник",
 "Среда",
 "Четверг",
 "Пятница",
 "Суббота",
 "Воскресенье"
};
const char *months[12]={
	"Январь",
 "Февраль",
 "Март",
 "Апрель",
 "Май",
 "Июнь",
 "Июль",
 "Август",
 "Сентябрь",
 "Октябрь",
 "Декабрь"
};

void onInf()
{
}
 //Redraw
static void gOnRedraw()
{
//   if ((data->gui.state==2)&&(py>=pop.y2-pop.y))
	{
		GetDateTime(&date,&time);
	//DrawRectangle(0,0,scr_w,scr_h,0,black,black);
	//DrwImg(mfn, 0,0, NULL,NULL);
	//bar();
	DrawRectangle(infgui.p.x,infgui.p.y,infgui.p.x2,infgui.p.y2,0,black,black);
// 	char *txt[256];
// 	zeromem(txt, 256);
	wsprintf(ews, "%t\n%02d:%02d\n%02d,%t,%d%t\n%t", "Дата/Время", time.hour,time.min,
			date.day, months[date.month-1], date.year, "г.", weekdays[GetWeek(&date)] );
	
	//wsprintf(ews, perc_t, "Проверка пгуя");
	DrawString(ews,infgui.p.x, infgui.p.y,
			   infgui.p.x2,infgui.p.y2,
	  FONT_SMALL,TEXT_ALIGNMIDDLE,white,NULL);
	
// 	RAMNET *rn=RamNet();
//     RAMNET *rn1=RamNet()+1;
//     RAMNET *rn2=RamNet()+2;
//     RAMNET *rn3=RamNet()+3;
//     RAMNET *rn4=RamNet()+4;
//     RAMNET *rn5=RamNet()+5;
//     RAMNET *rn6=RamNet()+6;
		// if((cur.x<100)&&(cur.y<50)) DrawString(ws,2,13,scr_w/3,scr_h,FONT_SMALL,1,red,NULL);
		// else DrawString(ws,2,13,scr_w/3,scr_h,FONT_SMALL,1,white,NULL);
		//if(f.rn->power!=0) DrawRoundedFrame(2,2,2+(scr_w/3-(f.rn->power-60)*(scr_w/3)/60),12,0,0,0,white,white0);
// 		if(rn->power!=0) DrawRoundedFrame(3,43,3+(scr_w/3-(rn->power-60)*(scr_w/3)/60),50,0,0,0,red,black );
// 		if(rn1->power!=0) DrawRoundedFrame(3,53,3+(scr_w/3-(rn1->power-60)*(scr_w/3)/60),60,0,0,0,red,black );
// 		if(rn2->power!=0) DrawRoundedFrame(3,63,3+(scr_w/3-(rn2->power-60)*(scr_w/3)/60),70,0,0,0,red,black );
// 		if(rn3->power!=0) DrawRoundedFrame(3,73,3+(scr_w/3-(rn3->power-60)*(scr_w/3)/60),80,0,0,0,red,black);
// 		if(rn4->power!=0) DrawRoundedFrame(3,83,3+(scr_w/3-(rn4->power-60)*(scr_w/3)/60),90,0,0,0,red,black );
// 		if(rn5->power!=0) DrawRoundedFrame(3,93,3+(scr_w/3-(rn5->power-60)*(scr_w/3)/60),100,0,0,0,red,black );
// 		if(rn6->power!=0) DrawRoundedFrame(3,103,3+(scr_w/3-(rn6->power-60)*(scr_w/3)/60),110,0,0,0,red,black );
	
	
	
	//DrwImg(cursor, cur.x,cur.y, NULL,NULL);
	// if(IsTimerProc(&chcurtmr)) GBS_DelTimer(&chcurtmr);
	// GBS_StartTimerProc(&chcurtmr, 108, chota);
	//fillTriangle(cur.x, cur.y, cur.x+10, cur.y+5, cur.x+5, cur.y+10, 0, white, white0);
  }
}

 //Create
static void gOnCreate()
{
// 	px=0;py=0;
// 	data->gui.state=1;
// 	popupp();
	//REDRAW();
}

 //Close
static void gOnClose()
{
// 	data->gui.state=0;
}

 //Focus
static void gOnFocus()
{
// 	#ifdef ELKA
// 		DisableIconBar(1);
// 	#endif
// 	DisableIDLETMR();
// 	data->gui.state=2;
}

 //Unfocus
static void gOnUnfocus()
{
// 	#ifdef ELKA
// 		DisableIconBar(0);
// 	#endif
//   if (data->gui.state!=2) return;
//   data->gui.state=1;
}

 //OnKey
static int gOnKey(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  switch(m)
  {
   //case LONG_PRESS: st=10;
   case KEY_DOWN:
	//movcr.st=1;
	switch(msg->gbsmsg->submess)
	{
		case '0':
			infgui.type=0;
			Dt.pf->Cr_PGUI(&infgui);
			return -1;
		case '1':
			infgui.type=1;
			Dt.pf->Cr_PGUI(&infgui);
			return -1;
		case '2':
			infgui.type=2;
			Dt.pf->Cr_PGUI(&infgui);
			return -1;
		case '3':
			infgui.type=3;
			Dt.pf->Cr_PGUI(&infgui);
			return -1;
	 case RIGHT_SOFT:
		return 1;
	 case RED_BUTTON:
// 		ShowWMSG(0, "проверка вывода сообщения", FONT_SMALL, GetPaletteAdrByColorIndex(3), GetPaletteAdrByColorIndex(7), 1);
	   break;
	}
	//DirectRedrawGUI();
	break;
  }
  //curkey(msg);
	//DirectRedrawGUI();
  return 0;
}

PGUI_methods infgui={
  0,0,
  "time/date",
  0,
  3,
  0,
  0,
  {10,30+YDISP,100,120+YDISP},{0,0,0,0},
  gOnRedraw,
  gOnCreate,
  gOnClose,
  gOnFocus,
  gOnUnfocus,
  gOnKey,
  NULL, NULL,
  {0,0,0,0}
  // 	void *next;
};


// typedef struct{
//   void *next, *prev;
//   char *name; // name
//   char st, // PGUI state 
//   type, // PGUI type (0-without cursor, 1-with cursor, +2-with window, +4-disactive desctop)
//   bn, // надо
//   n; // number of this window
//   RECT p, wp; // position
//   void (*OnRedraw)(),
//   (*OnCreate)(),
//   (*OnClose)(),
//   (*OnFocus)(),
//   (*OnUnfocus)();
//   int (*OnKey)(GUI_MSG *msg);
//   //  DT_ICON *ic; // надо :) (dont touch this)
//   IMGHDR *w, *bi;
//   PG_BTN btn;
//   // 	void *next;
// }PGUI_methods;

// static const void * const ggui_methods[11]={
//   (void *)gOnRedraw,
//   (void *)gOnCreate,
//   (void *)gOnClose,
//   (void *)gOnFocus,
//   (void *)gOnUnfocus,
//   (void *)gOnKey,
//   0,
//   (void *)kill_data, //method7, //Destroy
//   (void *)int_0,
//   (void *)int_0,
//   0
// };
// 
// static void gui()
// {
//   static const RECT Canvas={0,0,0,0};
//   StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
//   MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
//   zeromem(main_gui,sizeof(MAIN_GUI));
//   main_gui->gui.canvas=(void *)(&Canvas);
// //  view_gui->gui.flag30=2;
//   main_gui->gui.methods=(void *)ggui_methods;
//   main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
//   CreateGUI(main_gui);
// }








WSHDR *wss;
extern PGUI_methods ussdgui;


void ussdonInf()
{
}
//Redraw
static void ussdgOnRedraw()
{
  DrawRectangle(ussdgui.p.x,ussdgui.p.y,ussdgui.p.x2,ussdgui.p.y2,0,black,black);
  DrawString(wss,ussdgui.p.x, ussdgui.p.y,
			  ussdgui.p.x2,ussdgui.p.y2,
			  FONT_SMALL,TEXT_ALIGNMIDDLE,white,NULL);
}

//Create
static void ussdgOnCreate()
{
  // 	px=0;py=0;
  // 	data->gui.state=1;
  // 	popupp();
  //REDRAW();
}

//Close
static void ussdgOnClose()
{
  FreeWS(wss);
  // 	data->gui.state=0;
}

//Focus
static void ussdgOnFocus()
{
  // 	#ifdef ELKA
  // 		DisableIconBar(1);
  // 	#endif
  // 	DisableIDLETMR();
  // 	data->gui.state=2;
}

//Unfocus
static void ussdgOnUnfocus()
{
  // 	#ifdef ELKA
  // 		DisableIconBar(0);
  // 	#endif
  //   if (data->gui.state!=2) return;
  //   data->gui.state=1;
}

//OnKey
static int ussdgOnKey(GUI_MSG *msg)
{
  int m=msg->gbsmsg->msg;
  switch(m)
  {
	//case LONG_PRESS: st=10;
	case KEY_DOWN:
	  //movcr.st=1;
	  switch(msg->gbsmsg->submess)
	  {
		case RIGHT_SOFT:
		case RED_BUTTON:
		  return 1;
	  }
	  //DirectRedrawGUI();
	  break;
  }
  //curkey(msg);
  //DirectRedrawGUI();
  return 0;
}



PGUI_methods ussdgui={
  0,0,
  "USSD",
  PG_st_norm,
  3,
  0,
  0,
  {10,30,100,120},{0,0,0,0},
  ussdgOnRedraw,
  ussdgOnCreate,
  ussdgOnClose,
  ussdgOnFocus,
  ussdgOnUnfocus,
  ussdgOnKey,
  NULL, NULL,
  {0,0,0,0}
  // 	void *next;
};



int ProcessUSSD(CSM_RAM* data, GBS_USSD_MSG *msg)
{
  wss=AllocWS(256);
  int len=msg->pkt_length;
  if (len>240) len=240;
  GSMTXT_Decode(wss,msg->pkt,len,msg->encoding_type,(void*(*)(int))malloc_adr(),(void(*)(void))mfree_adr());
  FreeGSMTXTpkt(msg->pkt);
  
  StoreXYXYtoRECT(&ussdgui.p, f_h,Dt.h/2-f_h, scr_w-f_h,Dt.h-f_h);
  Dt.pf->Cr_PGUI(&ussdgui);
  
  return 1;
}
