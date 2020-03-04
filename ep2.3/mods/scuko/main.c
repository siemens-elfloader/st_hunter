#include "..\inc\swilib.h"
#include "local_ipc.h"
#include "../inc/xtask_ipc.h"
#include "main.h"
#include "language.h"
//#include "1_png.c"
//#include "readimg.h"

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
  TDate dd;
  GetDateTime(&dd,&tt);
  dd.year=dd.year%100;
  iday=DMonth[tt.millisec%12];
  iday = (tt.millisec+60*(tt.sec+60*(tt.hour+24* iday)));
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

//int yy[10][2];
//int xx[10][2];

int w;

char white[] = "\xFF\xFF\xFF\x64";

void drawpoint(int h, int w)
{
 //SetPixel(int x,int y,char *color);

  if(h%2) SetPixel(w,h,white);
  else SetPixel(w+5,h,white);
}

typedef struct
{
 int x;
 int xi;
 int y;
 int yi;
}CRD;

CRD c[10];


int FONT = FONT_MEDIUM;
#define FONTH GetFontYSIZE(FONT_MEDIUM); 

char *GetColor(int i)
{
  char *col=malloc(4);
  int R = Random(0,255);
  int G = Random(0,255);
  int B = Random(0,255);
  if(i > 7) i = 0;
  if(R > 0 || R < 10) R += 10;
    if(G > 0 || B < 10) R += 15;
      if(G > 0 || B < 10) R += 20;
  col[0] = R<<i; i++;
  col[1] = G<<i; i++;
  col[2] = B<<i; i++;
  col[3] = 100;
  return col;
}

char* GetColor2(int i)
{
  char *col=malloc(4);
  int R = Random(0,255);
  int G = Random(0,255);
  int B = Random(0,255);
    if(R > 0 || R < 10) R += 10;
    if(G > 0 || B < 10) R += 15;
      if(G > 0 || B < 10) R += 20;
  if(i > 7) i = 0;
  col[1] = R<<i; i++;
  col[2] = G<<i; i++;
  col[0] = B<<i; i++;
  col[3] = 100;
  return col;
}


int drawcanvas = 0;

int DrawChar(char *s,int x,int y,char *col)
{
  y += (!ys ? 3 : 0);
  ys = !ys;  
  int wslen;
  wsprintf(ws1,per_t,s); 
  int font = FONT;
  wslen = get_string_width(ws1, font);
  int fonth = GetFontYSIZE(font);
  //if(drawcanvas)
  //DrawCanvas(canvasdata,x-1,y-2,x+wslen+1, y+fonth,1);
  DrawString(ws1,x,y,x+wslen, y+fonth, font,TEXT_OUTLINE,col,(char*)GetColor2(Random(0,(Random(0,255)%7))));
  return get_string_width(ws1,font);
}

int ScrW;
int ScrH;

CRD upd(int x,CRD c,int speedx,int speedy)
{ 
  if((c.y < (ScrH - 1 - GetFontYSIZE(FONT))) && (!c.yi)) c.y +=speedy;
  else
  {
    c.yi = 1;
    if(c.y > 0) c.y-=5;
    else c.yi = 0;
  }
  
  if((x < ScrW-5) && (!c.xi)) c.x+=speedx;
  else
  {
    c.xi = 1;
    if(c.x > 0)  c.x -= 5;
    else c.xi = 0;  
  }
  
  return c;
}

char clock[]="%02d:%02d";
char date[]="%02d.%02d.%04d";
void DrawClock()
{
  TTime t;
  TDate d;
  GetDateTime(&d,&t);
  int fonth = GetFontYSIZE(FONT_LARGE);
  wsprintf(ws1,clock,t.hour,t.min);
  int wslen = get_string_width(ws1,FONT_LARGE);
  char *col=GetColor(t.sec%7);
  DrawString(ws1,0,10,ScrW, 10+fonth, FONT_LARGE,TEXT_OUTLINE+2,col,(char*)GetColor2(Random(0,(Random(0,255)%7))));
  mfree(col);
//  DrawString(ws1,c[6].x,c[6].y,c[6].x+wslen, c[6].y+(fonth), FONT_LARGE,0,col,GetPaletteAdrByColorIndex(23));

//  c[6] = upd(wslen,c[6],10,15);
}


void DrawDate()
{
  TTime t;
  TDate d;
  GetDateTime(&d,&t);
  int fonth = GetFontYSIZE(FONT_LARGE);
  wsprintf(ws1,date,d.day,d.month,d.year);
  int wslen = get_string_width(ws1,FONT_LARGE);
  char *col=GetColor(t.sec%7);
  DrawString(ws1,0,ScrH- fonth,ScrW, ScrH, FONT_LARGE,TEXT_OUTLINE+2,col,(char*)GetColor2(Random(0,(Random(0,255)%7))));
  mfree(col);
//  DrawString(ws1,c[7].x,c[7].y,c[7].x+wslen, c[7].y+(fonth), FONT_LARGE,0,col,GetPaletteAdrByColorIndex(23));  
  //c[7] = upd(wslen,c[7],15,10);
}

char tt[]="т";
char ee[]="е";
char cc[]="c";
char oo[]="o";
char nn[]="н";
char kk[]="к";
char aa[]="a";
char C2[]="C";
char ii[]="и";
char ll[]="л";
char vv[]="в";
char y2[]="y";
char CE[]="ц";
char space[]=" ";

void draw1()
{    
  int x = c[0].x;//xx[0][0];
  int y = c[0].y;//yy[0][0];
  int i = 0;
  x += DrawChar("Т",x,y,GetColor(i++));
  x += DrawChar("ы",x,y,GetColor(i++));
  x += DrawChar(space,x,y,GetColor(i++));
  x += DrawChar(tt,x,y,GetColor(i++));
  x += DrawChar(ee,x,y,GetColor(i++));
  x += DrawChar("п",x,y,GetColor(i++));
  x += DrawChar("e",x,y,GetColor(i++));
  x += DrawChar("р",x,y,GetColor(i++));
  x += DrawChar("ь",x,y,GetColor(i++));
  c[0]=upd(x,c[0],5,5);
}

char ja[]="я";
void draw3()
{    
  int i = 4;
//  int x = xx[2][0];
//  int y = yy[2][0];
  int x = c[2].x;
  int y = c[2].y;

  x += DrawChar("1",x,y,GetColor(i++));
  x += DrawChar("9",x,y,GetColor(i++));
  x += DrawChar(tt,x,y,GetColor(i++));
  x += DrawChar(ii,x,y,GetColor(i++));
  x += DrawChar(space,x,y,GetColor(i++));
  x += DrawChar(ll,x,y,GetColor(i++));
  x += DrawChar(ee,x,y,GetColor(i++));
  x += DrawChar(tt,x,y,GetColor(i++));
  x += DrawChar(nn,x,y,GetColor(i++));
  x += DrawChar(ja,x,y,GetColor(i++));
  x += DrawChar(ja,x,y,GetColor(i++));
  
  i = 2;
  c[i] = upd(x,c[i],5,5);
  //upd3(x);
}

char q[]="=";
char q1[]=")";
void draw4()
{    
  int i = 3;
//  int x = xx[i][0];
//  int y = yy[i][0];
    int x = c[3].x;
  int y = c[3].y;

  x += DrawChar(cc,x,y,GetColor(i++));
  x += DrawChar(CE,x,y,GetColor(i++));
  x += DrawChar(y2,x,y,GetColor(i++));
  x += DrawChar(kk,x,y,GetColor(i++));
  x += DrawChar(oo,x,y,GetColor(i++));
  x += DrawChar(q,x,y,GetColor(i++));
  x += DrawChar(q1,x,y,GetColor(i++));

  i = 3;
  c[i] = upd(x,c[i],10,10);
}


char clrBlack[]="\x00\x00\x00\x64";

GBSTMR tmr;

void Redr()
{
  DirectRedrawGUI_ID(MAINGUI_ID);
// REDRAW();
}


//Ты теперь 19ти летня сцуко
void DrwPngImg(IMGHDR *img, int x, int y)
{
  if(!img) return;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0/*pen,brush*/);
  DrawObject(&drwobj);
}

IMGHDR *IMG;
static void OnRedraw(MAIN_GUI *data)
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,/*"\x00\x00\x00\x30"*/clrBlack);
/*#ifdef ELKA
  DrawImg(0,0,(int)"4:\\newyear1.png");
#else
  DrawImg(0,0,(int)"0:\\newyear1.png");
#endif*/
  DrwPngImg(IMG,0,0);
  DrawDate();
  draw1();
 // draw2();
  draw3();
  draw4();
  //draw5();
//  draw6();
  DrawClock();

  GBS_StartTimerProc(&tmr,150,Redr);
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
   ws1=AllocWS(256);
   
 int i=0;  
#ifdef ELKA
 c[i].y = 10;//ты терерь
 c[i++].x = 2;
 
 c[i].y = 30;//19ти
 c[i++].x = 70;
 
 c[i].y = 50;//летняя
 c[i++].x = 100;
 
 c[i].y = 30;//сцуко
 c[i++].x = 200;
 
 c[i].y = ScreenH()-FONTH;//от Пашко
 c[i++].x = 200;
 
 c[i].y = 130;//и Славека
 c[i++].x = 200;
 
 c[i].y = ScreenH()/2;
 c[i++].x = 10;
 
 c[i].y = ScreenH()/2+15;
 c[i++].x = 50;
#else
  c[i].y = 10;//ты терерь
 c[i++].x = 2;
 
 c[i].y = 30;//19ти
 c[i++].x = 70;
 
 c[i].y = 50;//летняя
 c[i++].x = 100;
 
 c[i].y = 30;//сцуко
 c[i++].x = 20;
 
 c[i].y = ScreenH()-FONTH;//от Пашко
 c[i++].x = 50;
 
 c[i].y = 130;//и Славека
 c[i++].x = 2;
 
 c[i].y = ScreenH()/2;
 c[i++].x = 10;
 
 c[i].y = ScreenH()/2+15;
 c[i++].x = 50;
#endif
 ScrH = ScreenH()-1;
 ScrW = ScreenW()-1;
   
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  GBS_DelTimer(&tmr);
  FreeWS(ws1);
  if(strlen(IMG->bitmap) && IMG) mfree(IMG->bitmap);
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
char dir[128];
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
  MAINGUI_ID=csm->gui_id=CreateGUI(main_gui);  
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
  strcpy(dir,exename);
  
  for( int i=strlen(dir) ; i>0 && dir[i]!='\\' ; i-- )  dir[i] = 0;
  char s[64];
  sprintf(s,"%sscuko19.png",dir);
//  ShowMSG(1,(int)s);
  IMG = CreateIMGHDRFromPngFile(s,0);  
  MAIN_CSM main_csm;
//  InitConfig();
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
//  IMG = read_pngimg(_1_png);

  return 0;
}
