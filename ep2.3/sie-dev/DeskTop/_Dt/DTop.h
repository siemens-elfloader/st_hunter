#include "../plglib.h"
#include "../plglib.c"
DT Dt;

IPC_REQ gipc;

extern IMGHDR *resample(IMGHDR *img, int px, int py, int fast, int del);
extern int ICON_REDRAW(void *dt, DT_ICON *ic);
extern int GetBitmapMapPos(IMGHDR *img, int x, int y);
extern int DrwImg2Img(IMGHDR *img, RECT p, IMGHDR *buf, char alfa/* , IMGHDR *fn */);
extern int FillRectByNum(IMGHDR *buf, RECT p, char n);
extern void fill_icons(int t);
extern IMGHDR* create_imghdr(const char *fname, int type);
extern IMGHDR* img_24_16(IMGHDR *img, int del);
// PLG_FUNCS PLG_funcs={resample, ICON_REDRAW, GetBitmapMapPos, DrwImg2Img,
//  FillRectByNum, fill_icons, create_imghdr, img_24_16};

extern void add_icon(DT_ICON *TI);
extern void del_icon(DT_ICON *TI);
EXT_FUNCS EXT_funcs={add_icon, del_icon, resample, ICON_REDRAW, GetBitmapMapPos, DrwImg2Img,FillRectByNum, fill_icons, create_imghdr, img_24_16};

extern void dr_m( char fnd, char inf );
extern void curkey(GUI_MSG *msg);
extern void stdmove(GUI_MSG *msg);
M_FUNCS m_funcs={dr_m, curkey, stdmove};

// #define DT_ERR	0
// #define DT_MAIN	1
// #define DT_WAIT	2
// #define DT_MENU	3
// #define DT_MOVE	4
// #define DT_RESZ	5
// #define DT_CHTR	6
// #define DT_LPLG	7
// #define DT_PGUI	8


#define DT_MAIN	Dt.states->st_MAIN
#define DT_MENU	Dt.states->st_MENU
#define DT_MOVE	Dt.states->st_MOVE
#define DT_RESZ	Dt.states->st_RESZ
#define DT_CHTR	Dt.states->st_CHTR
#define DT_WAIT	Dt.states->st_WAIT
#define DT_LPLG	Dt.states->st_LPLG
#define DT_PGUI	Dt.states->st_PGUI
#define DT_ERR	Dt.states->st_ERR



#define State Dt.st
#define cur Dt.cur
//={0,0,0,0};

//extern void ShowWMSG(int flag, const char* msg, int font, const char* foncolor, const char* fringcolour, int ena);


#define IPC_NPLG	1

// menu point parameter
#define MNP_NORM 0
#define MNP_SUBM 1
#define MNP_L_UP 2
#define MNP_L_DN 3
#define MNP_L	 4

#define MNP_NACT 10


// int MAINGUI_ID=0;

void SMART_REDRAW(void)
{
	if(!Dt.MAINGUI_ID) return;
  int f;
  LockSched();
  f=IsGuiOnTop(Dt.MAINGUI_ID);
  UnlockSched();
  if (f) REDRAW();
}


void Stop_SSV_Timer();
void Start_SSV_Timer();


void fill_icons(int t); // t=1 only icons, t=2 only nmap, t=3 both


//static IPC_REQ gipc;
const char ipc_my_name[]=IPC_DT_NAME;

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


RECT currc,bgrc;
DRWOBJ curobj,fonobj;


char v=0, firstmsg=1, idle_ch=0;
extern const int CH_IDLE/*  = 0 */;

extern const char BG_IMG[], CUR_IMG[], DT_ROOT[], MC_PATH[];
extern const int KEY_CPU_HI, MN_SEL_MOD, CH_IDLE, CUR_JUMP/*, CUR_ENABLE*/;
extern const unsigned int CUR_BIG_STEP, CUR_TIMER, L_STEP_COUNT;
// const static char *icon_config=DEFAULT_DISK":\\ZBin\\EXPROJECT\\DeskTop\\icon.dt",
	// *fon_img=DEFAULT_DISK":\\ZBin\\EXPROJECT\\DeskTop\\fon.png",
	// *cur_img=DEFAULT_DISK":\\ZBin\\EXPROJECT\\DeskTop\\cur.png";

DT_ICON /* *AI, */ *TAI;
//DT_MENU_P *AMP;

typedef struct
{
  GUI gui;
}MAIN_GUI;

WSHDR *ws, *ews;
extern void kill_data(void *p, void (*func_p)(void *));
const int minus11=-11;
//short scr_w, scr_h;
#define scr_w Dt.w
#define scr_h Dt.h

unsigned int //MAINCSM_ID = 0,
		exitt=0,
		f_h; // высота шрифта
		
struct{
	signed char x,y, st, t;
}movcr={0,0,10};

GBSTMR curtmr,
	inftmr;

IMGHDR *mfn, *cursor;

void bar(), m_redr();

 // ничего не делающие функции :)
static int int_0(void){return(0);}
static void void_0(){}

/* char _09[]=
{
	0x78, 0x84, 0x84, 0x84, 0x84, 0x7c, 0x04, 0x08, 0x70
}; */




/* RamAccPoint:


#pragma swi_number=0x80D0
__swi __arm char* RamAccPoint(void);

	int* pointer = (int*)RamAccPoint()+2;
	if (*pointer != 0)
{
		int* device = (int*)*pointer+3;
		int* id = (int*)*pointer+2;
		if (*id != 0)
{
			wsprintf(ws, "%t:%t", *device, *id);
			DrawString(ws,10,10,230,320,FONT_SMALL,1,white,black);
}
		else
{
			wsprintf(ws, "%t", *device);
			DrawString(ws,10,10,230,320,FONT_SMALL,1,white,black);
}
} */

extern DtSt	st_MAIN,st_MENU,st_MOVE,st_RESZ,st_CHTR,st_WAIT,st_PGUI,st_LPLG,st_ERR, st_PGM,st_PGR;

