#include "..\..\..\inc\swilib.h"
#include "..\..\plglib.h"
#include "conf_loader.h"
#include "math.h"

#ifdef NEWSGOLD
#define myMSG 0xDEAD
#else
#define myMSG 0x11E
#endif

#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])

#define PI 3.1415926535

// #define TMR_SEC 216


extern void kill_data(void *p, void (*func_p)(void *));
static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}




DT *dt;
DT_ICON *MI;
EXT_FUNCS *PLG_funcs;
// GBSTMR inftmr;






typedef struct {
  int x;
  int y;
}POINT;

typedef struct {
  char R;
  char G;
  char B;
  char A;
}color;

#define CTYPE1 10
#define CTYPE2 3

TDate date; 
TTime time;

IMGHDR *clock=0, *minp=0, *mintmp=0, *hourp=0, *hourtmp=0;


int mn=61;


// extern const unsigned int CFGX;
// extern const unsigned int CFGY;

// extern const unsigned int SHOW_IN;

extern const char CLOCKH[];
extern const unsigned int CSIZE;
extern const unsigned int COP;

extern const char HOURH[];
extern const unsigned int HSIZE;
extern const unsigned int HOP;

extern const char MINH[];
extern const unsigned int MSIZE;
extern const unsigned int MOP;

const int ROT=0;

CSM_DESC icsmd;

int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);

int sqr(int x)
{
  return x*x;
}

int abs(int x)
{
  return x < 0 ? x*(-1) : x;
}

int max(int x, int y)
{
  return x > y ? x : y;
}

int min(int x, int y)
{
  return x < y ? x : y;
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

// DrwImg(IMGHDR *img, int x, int y, char *pen, char *brush)
// {
  // RECT rc;
  // DRWOBJ drwobj;
  // StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  // SetPropTo_Obj5(&drwobj,&rc,0,img);
  // SetColor(&drwobj,pen,brush);
  // DrawObject(&drwobj);
// }

color RGBA(char R, char G, char B, char A)
{
  color t={R,G,B,A};
  return t;
}


color getcolor(IMGHDR *img, int x, int y)
{
  color *bm=(color*)img->bitmap;
  if(x < img->w && x>=0 && y < img->h && y>=0) 
    return *(bm + x + y*img->w);
  else
    return (color){0,0,0,0};
}


void setcolor(IMGHDR *img, int x, int y, color clr)
{
  color *bm=(color*)img->bitmap;
  if(x < img->w && x>=0 && y < img->h && y>=0)
    *(bm + x + y*img->w)=clr;
}

void bmfree(IMGHDR *img)
{
  int x, y;
  for(y=0; y<img->h; y++)
    for(x=0; x<img->w; x++)
      setcolor(img, x, y, (color){0,0,0,0});
}

IMGHDR *createIMGHDR(int w, int h, int type)
{
  IMGHDR *img=malloc(sizeof(IMGHDR));
  img->w=w; 
  img->h=h; 
  img->bpnum=type;
  img->bitmap=malloc(h*w*sizeof(color));
  zeromem(img->bitmap, h*w*4);
  //bmfree(img);
  return img;
}

void deleteIMGHDR(IMGHDR *img)
{
  if(img->bitmap) mfree(img->bitmap);
  mfree(img);
  img=NULL;
}


IMGHDR *rotate(IMGHDR *img, float angle, int del)
{
  double ang = -angle*acos(0)/90;
  int newWidth, newHeight;
  int nWidth = img->w;
  int nHeight= img->h;
  double cos_angle = cos(ang);
  double sin_angle = sin(ang);
  
  POINT p1={0,0};
  POINT p2={nWidth,0};
  POINT p3={0,nHeight};
  POINT p4={nWidth-1,nHeight};
  POINT newP1,newP2,newP3,newP4, leftTop, rightTop, leftBottom, rightBottom;
  
  newP1.x = p1.x;
  newP1.y = p1.y;
  newP2.x = (long)(p2.x*cos_angle - p2.y*sin_angle);
  newP2.y = (long)(p2.x*sin_angle + p2.y*cos_angle);
  newP3.x = (long)(p3.x*cos_angle - p3.y*sin_angle);
  newP3.y = (long)(p3.x*sin_angle + p3.y*cos_angle);
  newP4.x = (long)(p4.x*cos_angle - p4.y*sin_angle);
  newP4.y = (long)(p4.x*sin_angle + p4.y*cos_angle);
  
  leftTop.x = min(min(newP1.x,newP2.x),min(newP3.x,newP4.x));
  leftTop.y = min(min(newP1.y,newP2.y),min(newP3.y,newP4.y));
  rightBottom.x = max(max(newP1.x,newP2.x),max(newP3.x,newP4.x));
  rightBottom.y = max(max(newP1.y,newP2.y),max(newP3.y,newP4.y));
  leftBottom.x = leftTop.x;
  leftBottom.y = rightBottom.y;
  rightTop.x = rightBottom.x;
  rightTop.y = leftTop.y;
  
  newWidth = rightTop.x - leftTop.x;
  newHeight= leftBottom.y - leftTop.y;
  IMGHDR *img2=createIMGHDR(newWidth, newHeight, CTYPE1);
  
  int x,y,newX,newY,oldX,oldY;
  for (y = leftTop.y, newY = 0; y<=leftBottom.y; y++,newY++){
    for (x = leftTop.x, newX = 0; x<=rightTop.x; x++,newX++){
      oldX = (long)(x*cos_angle + y*sin_angle /*- 0.5*/);
      oldY = (long)(y*cos_angle - x*sin_angle /*- 0.5*/);
      setcolor(img2,newX,newY,getcolor(img,oldX,oldY));
    }
  }
  if(del)
    deleteIMGHDR(img);
  return img2;
}




IMGHDR *alpha(IMGHDR *img, char a, int nw, int del)
{
  int i;
  color *r=(color*)img->bitmap;
  for(i=0;i<img->h*img->w; i++, r++)
    if(r->A>a)
      r->A-=a;
    else
      r->A=0;
    return img;
}


IMGHDR *resample(IMGHDR *img, int px, int py, int fast, int del)
{
  if (px==100 && py==100) return img;
  
  long newx = (img->w*px)/100,
  newy = (img->h*py)/100;
  
  float xScale, yScale, fX, fY;
  xScale = (float)img->w  / (float)newx;
  yScale = (float)img->h / (float)newy;
  
  IMGHDR *img2=createIMGHDR(newx,newy, CTYPE1);
  if (fast) {
    for(long y=0; y<newy; y++){
      fY = y * yScale;
      for(long x=0; x<newx; x++){
        fX = x * xScale;
        setcolor(img2,  x,  y, getcolor(img, (long)fX,(long)fY));
      }
    }
  } else {
    long ifX, ifY, ifX1, ifY1, xmax, ymax;
    float ir1, ir2, ig1, ig2, ib1, ib2, ia1, ia2, dx, dy;
    char r,g,b,a;
    color rgb1, rgb2, rgb3, rgb4;
    xmax = img->w-1;
    ymax = img->h-1;
    for(long y=0; y<newy; y++){
      fY = y * yScale;
      ifY = (int)fY;
      ifY1 = min(ymax, ifY+1);
      dy = fY - ifY;
      for(long x=0; x<newx; x++){
        fX = x * xScale;
        ifX = (int)fX;
        ifX1 = min(xmax, ifX+1);
        dx = fX - ifX;
        rgb1= getcolor(img, ifX,ifY);
        rgb2= getcolor(img, ifX1,ifY);
        rgb3= getcolor(img, ifX,ifY1);
        rgb4= getcolor(img, ifX1,ifY1);
        
        ir1 = rgb1.R   * (1 - dy) + rgb3.R   * dy;
        ig1 = rgb1.G * (1 - dy) + rgb3.G * dy;
        ib1 = rgb1.B  * (1 - dy) + rgb3.B  * dy;
        ia1 = rgb1.A  * (1 - dy) + rgb3.A  * dy;
        ir2 = rgb2.R   * (1 - dy) + rgb4.R   * dy;
        ig2 = rgb2.G * (1 - dy) + rgb4.G * dy;
        ib2 = rgb2.B  * (1 - dy) + rgb4.B  * dy;
        ia2 = rgb2.A  * (1 - dy) + rgb4.A  * dy;
        
        r = (char)(ir1 * (1 - dx) + ir2 * dx);
        g = (char)(ig1 * (1 - dx) + ig2 * dx);
        b = (char)(ib1 * (1 - dx) + ib2 * dx);
        a = (char)(ia1 * (1 - dx) + ia2 * dx);
        
        setcolor(img2, x,y,RGBA(r,g,b,a));
      }
    }
  }
  
  if(del)
    deleteIMGHDR(img);
  return img2;
}




POINT canvsize()
{
  POINT sz;
  sz.x=max(clock ? clock->w :0,
           max(mintmp ? mintmp->w : 0,
               hourtmp ? hourtmp->w : 0));
  sz.y=max(clock ? clock->h :0,
           max(mintmp ? mintmp->h : 0,
               hourtmp ? hourtmp->h : 0));
  return sz;
}

void RereadSettings()
{
  InitConfig();
  if(clock)
    deleteIMGHDR(clock);
  if(minp)
    deleteIMGHDR(minp);
  if(hourp)
    deleteIMGHDR(hourp);
//   if(IsTimerProc(&inftmr))
//   {
// 	  GBS_StopTimer(&inftmr);
// 	  GBS_DelTimer(&inftmr);
//   }
  
  clock=0; minp=0; hourp=0;
  
  if(strlen(CLOCKH)) 
    if(clock=CreateIMGHDRFromPngFile(CLOCKH, CTYPE2))
    {
      clock=resample(clock, CSIZE, CSIZE, 0, 1);
      if(ROT)
        clock=rotate(clock, ROT*-1, 1);
      clock=alpha(clock, 255-255*COP/100, 0, 0);
    }      
  if(strlen(MINH))
    if(minp=CreateIMGHDRFromPngFile(MINH, CTYPE2))
    {
      minp=resample(minp, MSIZE, MSIZE, 0, 1);
      if(ROT)
        minp=rotate(minp, ROT*-1, 1); 
      minp=alpha(minp, 255-255*MOP/100, 0, 0);
    } 
  if(strlen(HOURH))
    if(hourp=CreateIMGHDRFromPngFile(HOURH, CTYPE2))
    {
      hourp=resample(hourp, HSIZE, HSIZE, 0, 1);
      if(ROT)
        hourp=rotate(hourp, ROT*-1, 1);
      hourp=alpha(hourp, 255-255*HOP/100, 0, 0);
    } 
  mn=61;
}


/* 
int MyIDLECSM_onMessage(CSM_RAM* data,GBS_MSG* msg)
{
  int csm_result;
  unsigned char fShow;
  if(msg->msg == MSG_RECONFIGURE_REQ) 
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
      ShowMSG(1,(int)"Aclock\nConfig updated!");
    RereadSettings();
  }
  csm_result = old_icsm_onMessage(data, msg);
  switch (SHOW_IN)
  {
  case 0:
    fShow = !IsUnlocked();
    break;
  case 1:
    fShow = IsUnlocked();
    break;
  default:
    fShow = 1;
    break;
  }
  if (IsGuiOnTop(idlegui_id) && fShow) 
  {
    GUI *igui = GetTopGUI();
    if (igui)
    {
        void *canvasdata = BuildCanvas();
        void *idata = GetDataOfItemByID(igui, 2);
        if (idata)
        {
          void *canvasdata = ((void **)idata)[DISPLACE_OF_IDLECANVAS / 4];
          POINT sz=canvsize();
          DrawCanvas(canvasdata, 
                     CFGX - sz.x/2, 
                     CFGY - sz.y/2, 
                     CFGX + sz.x/2, 
                     CFGY + sz.y/2, 
                     1);
          
          if(clock)
            DrwImg(clock,
                   CFGX-clock->w/2,
                   CFGY-clock->h/2,
                   GetPaletteAdrByColorIndex(0),
                   GetPaletteAdrByColorIndex(1));
          
          if(hourtmp && hourp)
            DrwImg(hourtmp, 
                   CFGX - hourtmp->w/2,
                   CFGY - hourtmp->h/2,
                   GetPaletteAdrByColorIndex(0),
                   GetPaletteAdrByColorIndex(1)); 
          
          
          if(mintmp && minp)
            DrwImg(mintmp, 
                   CFGX - mintmp->w/2,
                   CFGY - mintmp->h/2,
                   GetPaletteAdrByColorIndex(0),
                   GetPaletteAdrByColorIndex(1));
          
          GetDateTime(&date,&time);
          if(mn!=time.min)
          {      
            mn=time.min;
            if(minp)
            {
              if(mintmp)
                deleteIMGHDR(mintmp); 
              mintmp=rotate(minp, (time.min*6)*-1, 0);
            }
            if(hourp)
            {
              if(hourtmp)
                deleteIMGHDR(hourtmp); 
              hourtmp=rotate(hourp, ((time.hour)*30+time.min/2)*-1, 0);
            }
          }
        }
      }
    }
    return(csm_result);
  }
 */

void DoClock()
{
  GetDateTime(&date,&time);
  if(mn!=time.min)
  {
		mn=time.min;
		if(minp)
		{
			if(mintmp)
				deleteIMGHDR(mintmp); 
			mintmp=rotate(minp, (time.min*6)*-1, 0);
		}
		if(hourp)
		{
			if(hourtmp)
				deleteIMGHDR(hourtmp); 
			hourtmp=rotate(hourp, ((time.hour)*30+time.min/2)*-1, 0);
		}
	RECT p;
	
	if(MI->img) if(MI->img->bitmap) mfree(MI->img->bitmap);
	MI->img=malloc(sizeof(IMGHDR));
	memcpy(MI->img, clock, sizeof(IMGHDR)-4);
	MI->img->bitmap=malloc(clock->w*clock->h*4);
	zeromem(MI->img->bitmap, clock->w*clock->h*4);
	//zeromem(MI->img->bitmap, clock->w*clock->h*4);
	if(clock)
	{
		StoreXYWHtoRECT (&p, 0, 0, clock->w, clock->h);
		PLG_funcs->DrwImg2Img(clock, p, MI->img, 100);
		// DrwImg(clock,
			// MI->p.x,
			// MI->p.y,
			// GetPaletteAdrByColorIndex(0),
			// GetPaletteAdrByColorIndex(1));
	}

	if(hourtmp && hourp)
	{
		StoreXYWHtoRECT (&p, clock->w/2 - hourtmp->w/2, clock->h/2 - hourtmp->h/2, hourtmp->w, hourtmp->h);
		PLG_funcs->DrwImg2Img(hourtmp, p, MI->img, 100);
		// DrwImg(hourtmp, 
			// MI->p.x+clock->w/2 - hourtmp->w/2,
			// MI->p.y+clock->h/2 - hourtmp->h/2,
			// GetPaletteAdrByColorIndex(0),
			// GetPaletteAdrByColorIndex(1)); 
	}


	if(mintmp && minp)
	{
		StoreXYWHtoRECT (&p, clock->w/2 - mintmp->w/2, clock->h/2 - mintmp->h/2, mintmp->w, mintmp->h);
		PLG_funcs->DrwImg2Img(mintmp, p, MI->img, 100);
		// DrwImg(mintmp, 
			// MI->p.x+clock->w/2 - mintmp->w/2,
			// MI->p.y+clock->h/2 - mintmp->h/2,
			// GetPaletteAdrByColorIndex(0),
			// GetPaletteAdrByColorIndex(1));
	}
	
	if(!MI->dyn)
	{
		PLG_funcs->ICON_REDRAW((void*)dt, MI);
// 		REDRAW();
	}
  }
//   GBS_StartTimerProc(&inftmr, TMR_SEC*3, DoClock);
}

void onClose()
{
// 	if(IsTimerProc(&inftmr))
// 	{
// 		GBS_StopTimer(&inftmr);
// 		GBS_DelTimer(&inftmr);
// 	}
	if(minp)
	{
		if(mintmp) deleteIMGHDR(mintmp); 
		if(minp) deleteIMGHDR(minp); 
	}
	if(hourp)
	{
		if(hourtmp) deleteIMGHDR(hourtmp); 
		if(hourp) deleteIMGHDR(hourp); 
	}
	if(clock)
	{
		if(clock) deleteIMGHDR(clock);
	}

    SUBPROC((void *)Killer);
}

void Refresh(int t) // t=1 refresh settings, t=0 without settings
{
  if(t)
	RereadSettings();
  DoClock();
}

static const char *weekdays[7]={
	"Понедельник",
	"Вторник",
	"Среда",
	"Четверг",
	"Пятница",
	"Суббота",
	"Воскресенье"
};
static const char *months[12]={
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
	zeromem(MI->inf, 256);
	sprintf(MI->inf, "%s\n%02d:%02d\n%02d,%s,%dг.\n%s", MI->nm, time.hour,time.min,
		date.day, months[date.month-1], date.year, weekdays[GetWeek(&date)] );
}

int main(char *exename, char *fname)
{
	if((int)fname>=0xa0000000)
	{
		RereadSettings();
		PLG_P *TP=(PLG_P*)fname;
		MI=TP->ic;
		dt=TP->dt;
		PLG_funcs=TP->dt->ef;
		MI->d=1;
		//MI->onDraw=onDraw;
		//MI->onEnter=onEnter;
		MI->onInf=onInf;
		MI->dyn=0;
		MI->bytmr=1; // Refresh by timer
		MI->sel=0;
		//StoreXYWHtoRECT (&MI->p, MI->p.x, MI->p.y, clock->w, clock->h);
			MI->pp.x=clock->w;
			MI->pp.y=clock->h;
		MI->img=NULL;
		MI->onClose=onClose;
		MI->Refresh=Refresh;
		
		DoClock();
		// typedef struct{
	// void *prev,*next, *up,*down,*left,*right ;
	// RECT p;
	// IMGHDR *img;
	// char alfa, n, type, d, *nm, *inf, *img_fl, *fl, dyn, csz;
	// void (*onDraw)();
	// int (*onEnter)();
	// void (*onInf)();
// }DT_ICON;
		
		//REDRAW();
	}
	else
    SUBPROC((void *)Killer);
  return 0;
}
