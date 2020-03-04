#include "..\inc\swilib.h"
#include "local_ipc.h"
#include "../inc/xtask_ipc.h"
#include "main.h"
#include "language.h"

GBSTMR mytmr;
int menus_id;

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ gipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

#define TMR_SECOND 216

void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

const char _s[]="%s";

#pragma inline=forced
int toupper(int c)
{
  if((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}


int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

const unsigned char rndtab[256] ={
  1,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
 74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
 95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
 52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   1,
175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
 25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
 94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
 80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
 24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
 28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
 71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
 17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
120, 163, 236, 249
};


unsigned int Random(int min,int max)
{
  static const int DMonth[]={0,31,59,90,120,151,181,212,243,273,304,334,365};
  unsigned int iday;
  TTime tt;
  GetDateTime(0,&tt);
  iday=DMonth[tt.millisec%12];
  iday = (tt.millisec+60*(tt.sec+60*(tt.millisec+24 * iday)));
  iday*=rndtab[iday%256];
  iday=iday%max;//получаем любое значение из диапазона до максимума
  
  if(iday<min)//если значение меньше минимума
  {
    while(iday<min)
    {
      iday=rndtab[iday%256]%(max-min);
    }    
  }
  return iday;
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

const char per_s[]="%s";
void *canvasdata;
char *s,*s1,*s2,*s3;
int X1=2,Y1=50;
int col=0;
GBSTMR tmr;
int str;
WSHDR *ws1;

int ys = 0;
int xs = 0;

int yy[10][2];
int xx[10][2];
#define FONT FONT_MEDIUM

char *GetColor(int i)
{
  char *col=malloc(4);
  int R = Random(0,255);
  int G = Random(0,255);
  int B = Random(0,255);
  if(i > 7) i = 0;
  col[0] = R<<i; i++;
  col[1] = G<<i; i++;
  col[2] = B<<i; i++;
  col[3] = 100;
  return col;
}


int DrawChar(char *s,int x,int y,char *col)
{
  y += (!ys ? 3 : 0);
  ys = !ys;  
  int wslen;
  wsprintf(ws1,per_t,s); 
  int font = FONT;
  wslen = get_string_width(ws1, font);
  int fonth = GetFontYSIZE(font);
  DrawString(ws1,x+2,y,x+wslen+4, y+fonth, font,0,col,GetPaletteAdrByColorIndex(23));
  return get_string_width(ws1,font);
}

int w;

char white[] = "\xFF\xFF\xFF\x64";

void drawpoint(int h, int w)
{
 //SetPixel(int x,int y,char *color);

  if(h%2) SetPixel(w,h,white);
  else SetPixel(w+5,h,white);
}

void upd1(int x)
{ 
  if((yy[0][0] < (ScreenH()-1 - GetFontYSIZE(FONT))) && (!yy[0][1])) yy[0][0]+=5;
  else
  {
    yy[0][1] = 1;
    if(yy[0][0] > 0) yy[0][0]-=5;
    else
      yy[0][1] = 0;
  }
  
  if((x < ScreenW()-1) && (!xx[0][1])) xx[0][0]+=5;
  else
  {
    xx[0][1] = 1;
    if(xx[0][0] > 0) xx[0][0]-=5;
    else
      xx[0][1] = 0;  
  }
}

char tt[]="т";
char ee[]="е";
void draw1()
{    
  int x = xx[0][0];
  int y = yy[0][0];
  int i = 0;
  x += DrawChar("Т",x,y,GetColor(i++));
  x += DrawChar("ы",x,y,GetColor(i++));
  x += DrawChar(" ",x,y,GetColor(i++));
  x += DrawChar(tt,x,y,GetColor(i++));
  x += DrawChar(ee,x,y,GetColor(i++));
  x += DrawChar("п",x,y,GetColor(i++));
  x += DrawChar(ee,x,y,GetColor(i++));
  x += DrawChar("р",x,y,GetColor(i++));
  x += DrawChar("ь",x,y,GetColor(i++));
  upd1(x);
}

void upd2(int x)
{ 
 int i = 1;
 if((yy[i][0] < (ScreenH()- 1 - GetFontYSIZE(FONT))) && (!yy[i][1])) yy[i][0]+=10;
  else
  {
    yy[i][1] = 1;
    if(yy[i][0] > 0) yy[i][0]-=5;
    else
      yy[i][1] = 0;
  }
  
  if((x < ScreenW()-5) && (!xx[i][1])) xx[i][0]+=5;
  else
  {
    xx[i][1] = 1;
    if(xx[i][0] > 0)  xx[i][0] -= 5;
    else
      xx[i][1] = 0;  
  }
}


void draw2()
{    
  int i = 1;
  int x = xx[i][0];
  int y = yy[i][0];
  i = 2;
  x += DrawChar("1",x,y,GetColor(i++));
  x += DrawChar("9",x,y,GetColor(i++));
  x += DrawChar(tt,x,y,GetColor(i++));
  x += DrawChar("и",x,y,GetColor(i++));

  upd2(x);
}

void upd3(int x)
{ 
 int i = 2;
 if((yy[i][0] < (ScreenH()-1 - GetFontYSIZE(FONT))) && (!yy[i][1])) yy[i][0]+=15;
  else
  {
    yy[i][1] = 1;
    if(yy[i][0] > 0) yy[i][0]-=15;
    else
      yy[i][1] = 0;
  }
  
  if((x < ScreenW()-1) && (!xx[i][1])) xx[i][0]+=15;
  else
  {
    xx[i][1] = 1;
    if(xx[i][0] > 0) xx[i][0]-=15;
    else
      xx[i][1] = 0;  
  }
}

char ja[]="я";
void draw3()
{    
  int i = 4;
  int x = xx[2][0];
  int y = yy[2][0];

  x += DrawChar("л",x,y,GetColor(i++));
  x += DrawChar(ee,x,y,GetColor(i++));
  x += DrawChar(tt,x,y,GetColor(i++));
  x += DrawChar("н",x,y,GetColor(i++));
  x += DrawChar(ja,x,y,GetColor(i++));
  x += DrawChar(ja,x,y,GetColor(i++));

  upd3(x);
}

void upd4(int x)
{ 
 int i = 3;
 if((yy[i][0] < (ScreenH()-1 - GetFontYSIZE(FONT))) && (!yy[i][1])) yy[i][0]+=5;
  else
  {
    yy[i][1] = 1;
    if(yy[i][0] > 0) yy[i][0]-=5;
    else
      yy[i][1] = 0;
  }
  
  if((x < ScreenW()-1) && (!xx[i][1])) xx[i][0]+=5;
  else
  {
    xx[i][1] = 1;
    if(xx[i][0] > 0) xx[i][0]-=5;
    else
      xx[i][1] = 0;  
  }
}


void draw4()
{    
  int i = 3;
  int x = xx[i][0];
  int y = yy[i][0];

  x += DrawChar("с",x,y,GetColor(i++));
  x += DrawChar("ц",x,y,GetColor(i++));
  x += DrawChar("у",x,y,GetColor(i++));
  x += DrawChar("к",x,y,GetColor(i++));
  x += DrawChar("о",x,y,GetColor(i++));

  upd4(x);
}

//Ты теперь 19ти летня сцуко

char clrBlack[]="\x00\x00\x00\x64";

GBSTMR tmr;
void Redr()
{
 REDRAW();
}

static void OnRedraw(MAIN_GUI *data)
{  
  DisableIconBar(1);
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,/*"\x00\x00\x00\x30"*/clrBlack);
  draw1();
  draw2();
  draw3();
  draw4();
  GBS_StartTimerProc(&tmr,216,Redr);
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
 ws1=AllocWS(256);
 yy[0][0] = 10;//ты терерь
 xx[0][0] = 2;
 
 yy[1][0] = 30;//19ти
 xx[1][0] = 70;
 
 yy[2][0] = 50;//летняя
 xx[2][0] = 100;
 
 yy[3][0] = 30;//сцуко
 xx[3][0] = 200;
 data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeWS(ws1);
  GBS_DelTimer(&tmr);
  data->gui.state=0;
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  DisableIDLETMR();
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(msg->gbsmsg->submess)
    {
    case RIGHT_SOFT:case RED_BUTTON: return (1);
    }
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

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


const RECT Canvas={0,0,0,0};
static void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  //patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
//  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  
}


//---------------------------------------------------------------------------//

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}


void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)
{

  SUBPROC((void *)ElfKiller);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }

  return(1);
}


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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"DrawList");
}


int main(char *exename, char *fname)
{ 
  MAIN_CSM main_csm;
//  InitConfig();
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();


  return 0;
}
