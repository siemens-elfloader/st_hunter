#include <swilib.h>
#include "Random.h"
#define DEBUG
// попытка сделать эффект гор€щего пламени

//делаем стандартное дл€ всех эльфов////////////////////////////////////////
const int minus11=-11;
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
/////////////////////////////////////////вс€кие дефайны///////////////
#ifdef ELKA
#define width 240
#define height 320
#else
#define width 132
#define height 176
#endif
///////////////////////////////////////////////////////////////////////


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
#define MAX width*height*4
//объ€вим глобальные переменные
const char black[]={0,0,0,100};
 int veter;// направление ветра
 int visota=0;// высота пламени
 int dirka=1;// дырки в пламени
  short screen[width*height];
  char screen3[MAX];
  int Rc=0;// красный
  int Gc=0;// зеленый
  int Bc=0;// синий
  int countpallet=0;//
  int countpallet1=0;
  char palletka2[255*4]={0};
  int Rcv=0;
  int Gcv=0;
  int Bcv=0;
  int Acv=0;


int clor=0;
   //IMGHDR img;
   IMGHDR imgRGB={width,height,0xA,screen3};


// функци€ из трех значенний   красного синего и зеленого возвращает битмап
int rgb2bitmap16 (int R,int G,int B)  // (c) mirr
{
  return ((B>>3)+((G>>2)<<5)+((R>>3)<<11));
}

/////////////////////////////// создадим палитру
void createpallet ()



{int ix=0;
for (int i=1; i<255; i++)
{
  //palletka[i]=65535;
  palletka2[(i-1)*4+3]=i;
}

ix=0;
 for (int i=1; i<85; i++)  // с черного в красный
{
    Rc= ix;
  Gc=0;
  Bc=0;

  countpallet++;
//palletka [countpallet]= rgb2bitmap16 ( Rc, Gc,Bc);
palletka2[countpallet1]= Rc;
palletka2[countpallet1+1]= Gc;
palletka2[countpallet1+2]= Bc;
palletka2[countpallet1+3]=countpallet;
countpallet1++;
countpallet1++;
countpallet1++;
countpallet1++;
ix++;
ix++;
ix++;

}

ix=0;
 for (int i=1; i<85; i++)  // с красного в желтый
{
    Gc= ix;


  countpallet++;
//palletka [countpallet]= rgb2bitmap16 (Rc,Gc,Bc);
palletka2[countpallet1]= Rc;
palletka2[countpallet1+1]= Gc;
palletka2[countpallet1+2]= Bc;
palletka2[countpallet1+3]=countpallet;
countpallet1++;
countpallet1++;
countpallet1++;
countpallet1++;
ix++;
ix++;
ix++;


}
ix=0;

 for (int i=1; i<85; i++)  // с желтого в белый
{
    Bc= ix;


  countpallet++;
//palletka [countpallet]= rgb2bitmap16 (Rc,Gc,Bc);
palletka2[countpallet1]= Rc;
palletka2[countpallet1+1]= Gc;
palletka2[countpallet1+2]= Bc;
palletka2[countpallet1+3]=countpallet;
countpallet1++;
countpallet1++;
countpallet1++;
countpallet1++;
ix++;
ix++;
ix++;

}


}



void filtr ()  // отфильтруем через палитру
{
//int  indexcolor;
int indexx;
int countnew=1;
  for (int i=0; i<width*height; i++)
  {
    indexx= screen[i];
   // indexcolor= palletka [indexx];
    Rcv=palletka2[indexx*4];
    Gcv=palletka2[indexx*4+1];
    Bcv=palletka2[indexx*4+2];
     Acv=palletka2[indexx*4+3];
    screen3[countnew+3]=Acv;
    screen3[countnew+2]=Bcv;
    screen3[countnew+1]=Gcv;
    screen3[countnew]=Rcv;
    countnew++;
    countnew++;
    countnew++;
    countnew++;
  }
}



  GBSTMR timer;

void SetPix(int x,int y,int cl)
{
  if(x<0 || x>width-1 || y<1 || y>height) return;
  //cl--;
  if (cl<0) cl=0;
  else screen[x+y*width] =cl;
}
short GetPix(int x,int y)
{
  return screen[x+y*width];
}

void timer_proc(void){

for (int i=1; i!=width-1; i++)SetPix(i,height-1,(randint(100)+155));// заполн€ем нижнюю строку случайными числами
  int ypos=height-2;

  for(int x=0; x!=width; x++)
  {
        clor=((GetPix(x-1,ypos+1)+GetPix(x,ypos)+GetPix(x,ypos+1)+GetPix(x+1,ypos+1)+visota)/4);//вычисл€ем и заполн€ем следующую строку
        if ((dirka==1&&clor==GetPix(x,ypos)||clor<0))clor=0;
        SetPix(x,ypos,clor);
  }
  ypos=height-3;
  for(int x=0; x!=width; x++)
  {
        clor=((GetPix(x-1,ypos+1)+GetPix(x,ypos)+GetPix(x,ypos+1)+GetPix(x+1,ypos+1)+visota)/4);//вычисл€ем и заполн€ем следующую строку
        if ((dirka==1&&clor==GetPix(x,ypos))||clor<0) clor=0;
        SetPix(x,ypos,clor);
      }





  for(int y=height-4; y!=1; y--)
      {
  for(int x=0; x!=width; x++)
  {


    if (veter==0)clor=((GetPix(x-1,y+3)+GetPix(x,y)+GetPix(x,y+3)+GetPix(x+1,y+3)+visota)/4);
    else
       {
    clor =((GetPix(x-veter, y+2) + GetPix(x,y) + GetPix(x,y+2)+visota)/3);
    }
     if ((dirka==1&&clor==GetPix(x,y))||clor<0) clor=0;
     if (clor>255) clor=255;
        SetPix(x,y,clor);
      }
  }

  filtr ();


  memcpy(imgRGB.bitmap,screen3,MAX);//={240,320,0xA,(char*)screen3};
//=========================
	GBS_StartTimerProc(&timer,265/16,timer_proc);
	DirectRedrawGUI();
	}


void DrwImg(IMGHDR *img)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,0,0,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  DrawObject(&drwobj);
}
void OnRedraw(MAIN_GUI *data)
{
  DrawRectangle(0,0,width-1,height-1,0,black,black);
  DrwImg(&imgRGB);
}


void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  createpallet ();
   data->gui.state=1;
}
void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
  GBS_DelTimer(&timer);
}
void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
    #ifdef ELKA
  DisableIconBar(1);
#endif
  data->gui.state=2;
  DisableIDLETMR();
  GBS_StartTimerProc(&timer,265/15,timer_proc);
}
void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
#ifdef ELKA
  DisableIconBar(0);
#endif
  GBS_StopTimer(&timer);
  data->gui.state=1;
}
int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
    case  RIGHT_SOFT : return(1);
    case '2':visota++;if (visota>255) visota=255;break;
    case '8':visota--; if (visota<0) visota=0; break;
    case '6': veter++; break;
    case '4': veter--;break;
    case '0':dirka++; if (dirka>1) dirka=0; break;
    }
  }
  return(0);
}

void onDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
}

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
  #ifdef NEWSGOLD
  (void *)onDestroy,
  #else
  (void *)kill_data,
  #endif
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,width-1,height-1};
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
  MAINGUI_ID=csm->gui_id=CreateGUI(main_gui);
}

void ElfKiller(void)
{
kill_elf();
}
void maincsm_onclose(CSM_RAM *csm)
{
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
wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Flame by Z80");
}
int main(char *exename, char *fname)
{
  char dummy[sizeof(MAIN_CSM)];
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  UpdateCSMname();
  Randomize();
  return 0;
}
