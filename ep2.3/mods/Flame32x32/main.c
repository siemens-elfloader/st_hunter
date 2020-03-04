#include "..\inc\swilib.h"
#define DEBUG
// попытка сделать эффект горящего пламени

//делаем стандартное для всех эльфов////////////////////////////////////////
const int minus11=-11;
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
/////////////////////////////////////////всякие дефайны///////////////
#ifdef ELKA
#define width 240
#define heighty 320
#else
#define width 132
#define heighty 176
#endif

int height=heighty;

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
#define MAX width*heighty*4

 int veter;// направление ветра
 int visota=1;// высота пламени
 int dirka=1;// дырки в пламени
  short screen[width*heighty];
  char screen3[MAX];
  int Rc=0;// красный
  int Gc=0;// зеленый 
  int Bc=0;// синий
 
  
  char palletka2[255*4]={0};
  int Rcv=0;
  int Gcv=0;
  int Bcv=0;
  int Acv=0;
  
   //IMGHDR img;
   IMGHDR imgRGB={width,heighty,0xA,screen3};
      

/////////////////////////////// создадим палитру
void createpallet ()
{
  
  int ix=0;
 int countpallet=0;
 int countpallet1=0;
 
 
 for (int i=1; i<85; i++)  // с черного в красный
{
  Rc=ix;
  Gc=0;
  Bc=0;

  countpallet++;
palletka2[countpallet1]= Rc;
palletka2[countpallet1+1]= Gc;
palletka2[countpallet1+2]= Bc;
if (countpallet<30) palletka2[countpallet1+3]=0;
 if (countpallet>50) palletka2[countpallet1+3]=countpallet+110;
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
    Rc=255;
    Gc= ix;
    Bc=0;

  countpallet++;

palletka2[countpallet1]= Rc;
palletka2[countpallet1+1]= Gc;
palletka2[countpallet1+2]= Bc;
palletka2[countpallet1+3]=255;
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
    Rc=255;
    Gc= 255;
    Bc= ix;


  countpallet++;

palletka2[countpallet1]= Rc;
palletka2[countpallet1+1]= Gc;
palletka2[countpallet1+2]= Bc;
palletka2[countpallet1+3]=255;

//if (countpallet>130) palletka2[countpallet1+3]=255;
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
int countnew=0;
height=heighty-visota;
  for (int i=0; i<width*heighty; i++)
  {
    indexx= screen[i];
    Rcv=palletka2[indexx*4];
    Gcv=palletka2[indexx*4+1];
    Bcv=palletka2[indexx*4+2];
     Acv=palletka2[indexx*4+3];
     if (Acv>255) Acv=255;
     
    screen3[countnew+3]=Acv;
    screen3[countnew+2]=Rcv;
    screen3[countnew+1]=Gcv;
    screen3[countnew]=Bcv;
    countnew++;
    countnew++;
    countnew++;
    countnew++;
  }
}

//================================================================
unsigned int randseed;
#define random randint
#define randomize Randomize
unsigned int randint( int max )
{
  randseed *=0x08088405;


  randseed++;
  return (  (randseed % max)  );
}
void Randomize(){ // Вызываем при старте для случайной последовательности
  TDate date;
  TTime time;
  GetDateTime(&date,&time);
  randseed=(time.min | (time.sec<<5));
}
//================================================================

  
  GBSTMR timer;
void timer_proc(void){DirectRedrawGUI(); GBS_StartTimerProc(&timer,4,timer_proc);}





inline void SetPix(int x,int y,int cl)// установить цвет пикселя 
{  
  if(x<0 || x>width-1 || y<1 || y>height) return;
 
  if (cl<0) cl=0;
  else screen[x+y*width] =cl;
}
inline short GetPix(int x,int y)// прочитать пиксель
{  
  return screen[x+y*width];
}


int clor=0;
IMGHDR screen2={0,0,8,""};
void DrwImg(IMGHDR *img)  // рисует картинку из дравимж
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,0,0,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  DrawObject(&drwobj);
}
void DoScreen2()          // делает скриншот
{
    int ScrH=ScreenH();
    int ScrW=ScreenW();
    char *ms=RamScreenBuffer();
    screen2.w=ScrW;
    screen2.h=ScrH;
    screen2.bitmap=malloc(ScrW*ScrH*2);
    memcpy(screen2.bitmap, ms, ScrW*ScrH*2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnRedraw(MAIN_GUI *data)// вся прорисовка происходит здеся
{  
  
        //dirka=(randint(2));  
for (int i=1; i!=width-1; i++)SetPix(i,height-1,(randint(155)+100));// заполняем нижнюю строку случайными числами от 155 до 255
  int ypos=height-2;
  int ypos2=height-3;
  for(int x=0; x!=width; x++)
  {
        clor=((GetPix(x-1,ypos+1)+GetPix(x,ypos)+GetPix(x,ypos+1)+GetPix(x+1,ypos+1)+visota)/4);//вычисляем и заполняем следующую строку
        //if ((dirka==1&&clor==GetPix(x,ypos)||clor<0))clor=0;
        SetPix(x,ypos,clor);
        clor=((GetPix(x-1,ypos2+1)+GetPix(x,ypos2)+GetPix(x,ypos2+1)+GetPix(x+1,ypos2+1)+visota)/4);//вычисляем и заполняем следующую строку
       // if ((dirka==1&&clor==GetPix(x,ypos2))||clor<0) clor=0;
        SetPix(x,ypos2,clor);
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
     if ((dirka>0&&clor==GetPix(x,y))||clor<0) clor=clor-50;
     if (clor>255) clor=255;
     if (clor<10) clor=0;
        SetPix(x,y,clor);
      }
  } 
  
  filtr ();
  
  DrwImg(&screen2); //нарисуем скриншот
  DrwImg(&imgRGB);  //нарисуем наш битмап
}


void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  DoScreen2();//сделаем скриншот
  createpallet ();//создадим палитру
#ifdef ELKA
  DisableIconBar(1);
#endif
   data->gui.state=1;
  
  

  
  
  
  


  //////////////////////////////////
}
void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  mfree(screen2.bitmap);
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
}
void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
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
extern void kill_data(void *p, void (*func_p)(void *));
void onDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
kill_data(data,mfree_adr);
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

const RECT Canvas={0,0,width-1,heighty-1};
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
  csm->gui_id=CreateGUI(main_gui);
  MAINGUI_ID=csm->gui_id;
}

void ElfKiller(void)
{
  extern void kill_data(void *p, void (*func_p)(void *));
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
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
  WSHDR *ws=AllocWS(256);
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Flame2 by Z80");// имя в менеджере задач
  FreeWS(ws);
}
int main(char *exename, char *fname)
{
  
  
  
  
  char dummy[sizeof(MAIN_CSM)];
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  UpdateCSMname();
  Randomize();
  timer_proc();
  return 0;
}
