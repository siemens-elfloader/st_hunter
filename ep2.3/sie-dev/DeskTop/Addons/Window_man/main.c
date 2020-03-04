#include "..\..\..\inc\swilib.h"
#include "..\..\plglib.h"
#include "..\..\plglib.c"
#include "..\..\rect_patcher.h"



RECT temp_wim_p;
DT *dt;
WSHDR *ews;
 // ничего не делающие функции :)
// static int int_0(void){return(0);}
// static void void_0(){}

int f_h;
extern DtSt st_PGM, st_PGR;//, st_PGUI;

void SMART_REDRAW()
{
	if(!dt->MAINGUI_ID) return;
	int f;
	LockSched();
	f=IsGuiOnTop(dt->MAINGUI_ID);
	UnlockSched();
	if (f) REDRAW();
}
#include "dk.c"

void dr_null(){}

extern void kill_data(void *p, void (*func_p)(void *));
static void Killer(void)
{
	extern void *ELF_BEGIN;
	kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

// static DT_ICON dt_window={
// 	NULL,NULL, NULL,NULL,NULL,NULL, // dont touch
//  {0,0,0,0}, // position
//  {0,0,0,0},
//  NULL, // image
//  100, // transparency
//  0xFE, // number
//  0, //type
//  1, // 1-mfree when clear
//  NULL, // name
//  NULL, // popup info text
//  NULL, // image file
//  NULL, // file, shortcut or entrypoint
//  1, // 0-static icon, 1-dinamic icon
//  0, // refresh by timer
//  1, // nothing
//  dr_null, // draw function
//  int_0, // action on enter button
//  void_0, // create info text
//  NULL, // action on close (for plagins)
//  NULL, // refresh (for plagins)
//  NULL, // methods for subgui
// };



// extern void add_icon(DT_ICON *TI), del_icon(DT_ICON *TI);

typedef struct{
	IMGHDR *lr,
 *c,
 *min,
 *max,
 *rsz,
 *exit;
}PG_winp;
PG_winp PG_winparts={0,0,0,0,0,0};


inline short BGR_24_16(char *clr) {return(((clr[2]<<8)&0xF800)|((clr[1]<<3)&0x07E0)|((clr[0]>>3)&0x001F));}
inline short RGB_24_16(char *clr) {return(((clr[0]<<8)&0xF800)|((clr[1]<<3)&0x07E0)|((clr[2]>>3)&0x001F));}



void Add_PGUI(PGUI_methods *p)
{
  PGUI_methods *TPG=dt->pglist.last;
  p->prev=TPG;
  if(dt->pglist.first==0) dt->pglist.first=p;
  dt->pglist.last=p;
  if(TPG) TPG->next=dt->pglist.last;
  dt->pglist.first->prev=NULL;
  dt->pglist.last->next=NULL;
  if(dt->st!=&st_PGM && dt->st!=&st_PGR) dt->pglist.act=dt->pglist.last;
  dt->pglist.n++;
  p->n=(dt->pglist.n+128);
  dt->pglist.show=1;
  
//   dt->ef->fill_icons(2);
//   SMART_REDRAW();
}

void Del_PGUI(PGUI_methods *p)
{
//   if(p->next==0 && p->prev==0)
// 	dt->pglist.first=dt->pglist.last=dt->pglist.act=0;
  if(p->prev) ((PGUI_methods*)p->prev)->next=p->next;
  else dt->pglist.first=p->next;
  if(p->next) ((PGUI_methods*)p->next)->prev=p->prev;
  else dt->pglist.last=p->prev;
  if(dt->pglist.first) dt->pglist.first->prev=NULL;
  if(dt->pglist.last) dt->pglist.last->next=NULL;
  dt->pglist.act=dt->pglist.last;
  remun_PGUI_list();
//   dt->pglist.n--;
  if(!dt->pglist.n) dt->pglist.show=0;
  
//   dt->ef->fill_icons(2);
//   SMART_REDRAW();
}



PG_BUTTON btn_exit={
  0,0, //void *prev,*next;
  0, //int n;
  {0,0,0,0}, //RECT r;
  0, //char *inf;
  key_PG_exit //int (*Enter)();
};

PG_BUTTON btn_move={
  0,0, //void *prev,*next;
  0, //int n;
  {0,0,0,0}, //RECT r;
  0, //char *inf;
  key_PG_move //int (*Enter)();
};

PG_BUTTON btn_razv={
  0,0, //void *prev,*next;
  0, //int n;
  {0,0,0,0}, //RECT r;
  0, //char *inf;
  key_PG_razv //int (*Enter)();
};

PG_BUTTON btn_resz={
  0,0, //void *prev,*next;
  0, //int n;
  {0,0,0,0}, //RECT r;
  0, //char *inf;
  key_PG_resz //int (*Enter)();
};


void add_btn(PGUI_methods *p, PG_BUTTON *btn, RECT *r)
{
  PG_BTN *pb=&p->btn;
  PG_BUTTON *tb=pb->last;
  pb->last=btn;
  if(pb->first==0) pb->first=btn;
  pb->last->prev=tb;
  if(tb) tb->next=pb->last;
  pb->first->prev=NULL;
  pb->last->next=NULL;
  pb->cnt++;
  btn->n=pb->cnt;
  
  memcpy(&btn->r, r, sizeof(RECT));
  dt->ef->FillRectByNum(p->bi, btn->r, btn->n);
}

void crpgwind(PGUI_methods *p, int t)
{
  zeromem(&p->btn, sizeof(PG_BTN));
  //   p->btn.first=p->btn.last=0;
  //   p->btn.cnt=0;
  if(p->w)
  {
	if(p->w->bitmap) mfree(p->w->bitmap);
	mfree(p->w);
  }
  p->w=malloc(sizeof(IMGHDR));
  //memcpy(p->w,p->ic->img,sizeof(IMGHDR));
  p->w->w=p->wp.x2-p->wp.x+1;
  p->w->h=p->wp.y2-p->wp.y+1;
  if((p->w->w<32)||(p->w->w>ScreenW()))
  {
	p->w->w=32;
	p->bi->w=32;
	p->wp.x2=p->wp.x+32;
  }
  if((p->w->h<32)||(p->w->h>ScreenH()))
  {
	p->w->h=32;
	p->bi->h=32;
	p->wp.y2=p->wp.y+32;
  }
  p->w->bpnum=10;
  p->w->bitmap=malloc(p->w->w*p->w->h*4);
  zeromem(p->w->bitmap, p->w->w*p->w->h*4);
  if(!PG_winparts.lr)
  {
  }
  for(int j=0; j<f_h+2; j++)
  {
	memcpy(p->w->bitmap+dt->ef->GetBitmapMapPos(p->w, 0, j), PG_winparts.lr->bitmap+j*4, 4);
	memcpy(p->w->bitmap+dt->ef->GetBitmapMapPos(p->w, p->w->w-1, j), PG_winparts.lr->bitmap+j*4, 4);
	for(int i=1; i<p->w->w-1; i++)
	{
	  memcpy(p->w->bitmap+dt->ef->GetBitmapMapPos(p->w, i, j), PG_winparts.c->bitmap+j*4, 4);
	}
  }
  for(int j=f_h+2; j<p->w->h; j++)
  {
	memcpy(p->w->bitmap+dt->ef->GetBitmapMapPos(p->w, 0, j), PG_winparts.lr->bitmap+(f_h+1)*4, 4);
	memcpy(p->w->bitmap+dt->ef->GetBitmapMapPos(p->w, p->w->w-1, j), PG_winparts.lr->bitmap+(f_h+1)*4, 4);
  }
  for(int i=1; i<p->w->w-1; i++)
  {
	memcpy(p->w->bitmap+dt->ef->GetBitmapMapPos(p->w, i, p->w->h-1), PG_winparts.c->bitmap+(f_h+1)*4, 4);
  }
  RECT tp;
  
  // Кнопка закрытия
  StoreXYWHtoRECT(&tp, p->w->w-f_h, 2, f_h-2, f_h-2);
  dt->ef->DrwImg2Img(PG_winparts.exit, tp, p->w, 100);
  add_btn(p, &btn_exit, &tp);
  // 	dt->ef->FillRectByNum(p->ic->img, tp, 1);
  
  // Кнопка развёртывания
  StoreXYWHtoRECT(&tp, p->w->w-f_h*2, 2, f_h-2, f_h-2);
  dt->ef->DrwImg2Img(PG_winparts.max, tp, p->w, 100);
  add_btn(p, &btn_razv, &tp);
  // 	dt->ef->FillRectByNum(p->ic->img, tp, 4);
  
  // 	StoreXYWHtoRECT(&tp, p->w->w-f_h*2, 2, f_h-2, f_h-2);
  // 	DrwImg2Img(PG_winparts.exit, tp, p->w, 100);
  
  // Поле для передвижения
  if(p->st!=PG_st_max)
  {
	StoreXYXYtoRECT(&tp,  1,  1, p->wp.x2-p->wp.x-f_h*2, f_h-1);
	add_btn(p, &btn_move, &tp);
	// 	  dt->ef->FillRectByNum(p->ic->img, tp, 2); // move
  }
  
  // Добавляем кнопку ресайза
  StoreXYWHtoRECT(&tp, p->w->w-PG_winparts.rsz->w, p->w->h-PG_winparts.rsz->h, PG_winparts.rsz->w, PG_winparts.rsz->h);
  dt->ef->DrwImg2Img(PG_winparts.rsz, tp, p->w, 100);
  add_btn(p, &btn_resz, &tp);
  // 	dt->ef->FillRectByNum(p->ic->img, tp, 3); // resz
  
  p->OnFocus();
  
  p->w=dt->ef->img_24_16(p->w, 1);
}

void reset_WIND(PGUI_methods *p, char t) // t=0 - only rects; t=1 - rects+buttonmap
{
  StoreXYXYtoRECT(&p->p, p->wp.x+1, p->wp.y+2+f_h, p->wp.x2-1, p->wp.y2-1);
  if(t)
  {
	if(t==1)
	{
	  if(!p->bi)
	  {
		p->bi=malloc(sizeof(IMGHDR));
		zeromem(p->bi, sizeof(IMGHDR));
	  }
	  p->bi->w=p->wp.x2-p->wp.x+1;
	  p->bi->h=p->wp.y2-p->wp.y+1;
	  p->bi->bpnum=5;
	  if(p->bi->bitmap)
		mfree(p->bi->bitmap);
	  p->bi->bitmap=malloc(p->bi->w*p->bi->h);
	  zeromem(p->bi->bitmap, p->bi->w*p->bi->h);
	}
	crpgwind(p, t);
  }
  if(t<2)
	dt->ef->fill_icons(2);
}

void Close_PGUI(PGUI_methods *p)
{
  Del_PGUI(p);
  if(p->st==PG_st_max)
  {
	p->st=PG_st_norm;
	memcpy(&p->wp, &temp_wim_p, sizeof(RECT));
	// 	reset_WIND(APG, 1);
  }
//   mfree(p->bi->bitmap);
//   mfree(p->bi);
  p->OnClose();
//   //fill_icons(3);
//   //   p=NULL;
//   // 	dt->st=dt->states->st_MAIN;
  remun_PGUI_list();
  dt->pglist.fullscr=is_fullscreen_PGUI();
  dt->ef->fill_icons(2);
//   dt->ef->fill_icons(2);
}

void Create_PGUI(PGUI_methods *p)
{
// 	if(dt->actpg) Close_PGUI();
// 	dt->actpg=p;
	Add_PGUI(p);
	p->OnCreate();	
	
	if(p->st==PG_st_max)
	{
	  StoreXYXYtoRECT(&p->p, 1, 2+f_h+YDISP, dt->w-1, dt->h-1);
	}
	
	if(!(p->type&4))
	{
// 		p->ic=malloc(sizeof(DT_ICON));
// 		memcpy(p->ic, &dt_window, sizeof(DT_ICON));
		//memcpy(&p->ic->p, &p->p, sizeof(RECT));
		StoreXYXYtoRECT(&p->wp, p->p.x-1, p->p.y-2-f_h, p->p.x2+1, p->p.y2+1);
		memcpy(&temp_wim_p, &p->wp, sizeof(RECT));
// 		p->ic->img=malloc(sizeof(IMGHDR));
// 		p->ic->img->w=p->ic->p.x2-p->ic->p.x+1;
// 		p->ic->img->h=p->ic->p.y2-p->ic->p.y+1;
// 		p->ic->img->bpnum=5;
// 		p->ic->img->bitmap=malloc(p->ic->img->w*p->ic->img->h);
// 		zeromem(p->ic->img->bitmap, p->ic->img->w*p->ic->img->h);
		//RECT tp;
// 		p->ic->inf=malloc(256);
// 		zeromem(p->ic->inf, 256);
		
// 		dt->ef->add_icon(p->ic);
		reset_WIND(p, 1);
		
// 		crpgwind(p);
		
		//FillRectByNum(&Dt.ni, p->p, 0xFE);
	}
	// 	dt->st=&st_PGUI;
	dt->pglist.fullscr=is_fullscreen_PGUI();
	SMART_REDRAW();
}

void OnClose()
{
	FreeWS(ews);
	SUBPROC((void *)Killer);
}

PGUI_FUNCS PGUI_funcs={Create_PGUI, PG_Draw, PG_Keys, Close_PGUI, reset_WIND, add_btn, OnClose};


int main(char *exename, void *t)
{
	if((int)t>=0xa0000000)
	{
		dt=t;
		dt->pf=&PGUI_funcs;
	}
	else
	{
		SUBPROC((void *)Killer);
		return 1;
	}
	
	zeromem(&dt->pglist, sizeof(PGUI_LIST));
	f_h=GetFontYSIZE(FONT_SMALL); // высота мелкого шрифта
	
	char tmp[256];
	ews=AllocWS(256);
	
	zeromem(tmp, 256);
	sprintf(tmp,"%simg\\win\\lr.png",dt->rootadr);
#if NEWSGOLD || X75
	PG_winparts.lr=CreateIMGHDRFromPngFile(tmp, 3);
#else
	PG_winparts.lr=dt->ef->create_imghdr(tmp, 3);
#endif
	PG_winparts.lr=dt->ef->resz_img(PG_winparts.lr, 1, f_h+2, 0, 1);
		
	zeromem(tmp, 256);
	sprintf(tmp,"%simg\\win\\c.png",dt->rootadr);
#if NEWSGOLD || X75
	PG_winparts.c=CreateIMGHDRFromPngFile(tmp, 3);
#else
	PG_winparts.c=dt->ef->create_imghdr(tmp, 3);
#endif
	PG_winparts.c=dt->ef->resz_img(PG_winparts.c, 1, f_h+2, 0, 1);
		
	zeromem(tmp, 256);
	sprintf(tmp,"%simg\\win\\min.png",dt->rootadr);
#if NEWSGOLD || X75
	PG_winparts.min=CreateIMGHDRFromPngFile(tmp, 3);
#else
	PG_winparts.min=dt->ef->create_imghdr(tmp, 3);
#endif
	PG_winparts.min=dt->ef->resz_img(PG_winparts.min, f_h-2, f_h-2, 0, 1);
		
	zeromem(tmp, 256);
	sprintf(tmp,"%simg\\win\\max.png",dt->rootadr);
#if NEWSGOLD || X75
	PG_winparts.max=CreateIMGHDRFromPngFile(tmp, 3);
#else
	PG_winparts.max=dt->ef->create_imghdr(tmp, 3);
#endif
	PG_winparts.max=dt->ef->resz_img(PG_winparts.max, f_h-2, f_h-2, 0, 1);
		
	zeromem(tmp, 256);
	sprintf(tmp,"%simg\\win\\exit.png",dt->rootadr);
#if NEWSGOLD || X75
	PG_winparts.exit=CreateIMGHDRFromPngFile(tmp, 3);
#else
	PG_winparts.exit=dt->ef->create_imghdr(tmp, 3);
#endif
	PG_winparts.exit=dt->ef->resz_img(PG_winparts.exit, f_h-2, f_h-2, 0, 1);
		
	zeromem(tmp, 256);
	sprintf(tmp,"%simg\\win\\rsz.png",dt->rootadr);
#if NEWSGOLD || X75
	PG_winparts.rsz=CreateIMGHDRFromPngFile(tmp, 3);
#else
	PG_winparts.rsz=dt->ef->create_imghdr(tmp, 3);
#endif
	PG_winparts.rsz=dt->ef->resz_img(PG_winparts.rsz, f_h, f_h, 0, 1);
	
	return 1;
}
