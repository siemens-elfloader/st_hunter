#include "swilib.h"
#include "images.c"
#include "readimg.h"
#include "colors.h"
unsigned short maincsm_name_body[140];

unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
//  WSHDR *ws2;
//  int i1;
}MAIN_GUI;


enum types
{
  FON_IMAGE        ,  //фон
  BATTARY_LOW      ,  //знак разряженой батареи (восклицательный знак в треугольнике)
  BAT_LEFT_GREEN   ,  //левая часть батареи  [зеленая]
  BAT_LEFT_RED     ,  //левая часть батареи  [красная]  
  
  BAT_RIGHT_GREEN  ,  //правая часть батареи  [зеленая]  
  BAT_RIGHT_EMPTY  ,  //правая часть батареи  [пустая] 
  
  BAT_CENTER_GREEN ,  //средняя часть батареи [зеленая] (ширина 8пикс) 
  BAT_CENTER_EMPTY ,  //средняя часть батареи [пустая] (ширина 8пикс)
  
  LINE_RIGHT       ,  //зеленая полоска справа
  CLOCK            ,  //Часы
  SLIDE            ,  //подложка со стрелкой (всплывающая подсказка)
  ICONBAR          ,  //иконбар
  
  IMAGE_NUM           //кол-во картинок
};

IMGHDR *FImage[IMAGE_NUM];
IMGHDR *Dig[11];  //циферки


//Рисует область из 1 рисунка в точку (х,y) второго рисунка.
int drawimg2img(IMGHDR * onse, IMGHDR * timage2, int x, int y, int xRect, int yRect,int xxRect, int yyRect)
{
  //последние параметры - рисуемая область из картинки
  if (!onse) return 1;
  if (yRect<0 || yRect>=onse->h) yRect = 0;
  if (xRect<0 || xRect>=onse->w) xRect = 0;  // область в картинке не должна начинаться за ней:)
  if (yyRect==0 || (yRect+yyRect >= onse->h)) yyRect = onse->h-yRect; //eсли размер области нулевой или вылазит за картинку, то берем до конца картинки 
  if (xxRect==0 || (xRect+xxRect >= onse->w)) xxRect = onse->w-xRect; //eсли размер области нулевой или вылазит за картинку, то берем до конца картинки 
  int *scr = (int*)((char*)timage2->bitmap);
  int *pic = (int*)((char*)onse->bitmap);
  int i,j;

  for (i = xRect; i < xRect+xxRect; i++)
  {
    for(j = yRect; j < yRect+yyRect; j++)
    {
      //i в диапазоне области ВОП рисунка (от xRect до xRect+xxRect)                                                                         //j в диапазоне области ВОП рисунка (от yRect до yRect+yyRect)
      if (((y+j-yRect)<timage2->h) && ((x+i-xRect)<timage2->w) && ((y+j-yRect)>=0) && ((x+i-xRect)>=0) 
          &&(i>=xRect)&&(j>=yRect)&&(i<xRect+xxRect)&&(j<yRect+yyRect))
       {   
         scr[(y+j-yRect)*timage2->w+i+x-xRect] = pic[(j)*onse->w+i];
       }
    }
  }
 return 0;
}

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

int battery = 100;
void DrawBattary(int x,int y)
{
  battery = *RamCap();
  //левая часть
  if (battery > 4) DrwPngImg(FImage[BAT_LEFT_GREEN], x,y);
  else  DrwPngImg(FImage[BAT_LEFT_RED], x,y);
  
  //центр
  int x3 = 1;
  for (int x2 = x+15; x2 <= x+175; x2+=8)
   {
     if(battery < x3*5) DrwPngImg(FImage[BAT_CENTER_EMPTY], x2, y);
     else DrwPngImg(FImage[BAT_CENTER_GREEN], x2, y);
     x3++;
   }
  
  //правая часть
//  if (battery > 99) DrwPngImg(FImage[BAT_RIGHT_GREEN], x+175, y);
  //else
  DrwPngImg(FImage[BAT_RIGHT_GREEN],x+175,y); 
    DrwPngImg(FImage[BAT_CENTER_EMPTY], x+175, y);
  //аккум сел)))
  if (battery<5)   DrwPngImg(FImage[BATTARY_LOW], x+20,y+25);  

}


void DrawIconbar()
{
  DrwPngImg(FImage[ICONBAR], 4,3);//иконбар

  //net
  RAMNET *net_data;
  char cl4[]={0xD1,0xDE,0xE7,0x64};
  char cl2[]={0x91,0x9E,0xE7,0x64};
//  char colba24[]={0xFF,0xFF,0xFF,0x64};
  net_data=RamNet();
  if (((110-net_data->power)/2)> 0)
    DrawRoundedFrame(20,15,22,17,0,0,0, ( ((net_data->ch_number)<255)?cl2:cl4 ),( ((net_data->ch_number)<255)?cl2:cl4) );
  else
    DrawRoundedFrame(20,16,22,17,0,0,0,    colba24, colba24 );

  if(((110-net_data->power)/2)> 5)
    DrawRoundedFrame(24,13,26,17,0,0,0,   ( ((net_data->ch_number)<255)?cl2:cl4 ),( ((net_data->ch_number)<255)?cl2:cl4) );
  else
    DrawRoundedFrame(24,16,26,17,0,0,0,    colba24, colba24 );
  
  if(((110-net_data->power)/2)> 10)
     DrawRoundedFrame(28,11,30,17,0,0,0,   ( ((net_data->ch_number)<255)?cl2:cl4 ),( ((net_data->ch_number)<255)?cl2:cl4) );
  else
     DrawRoundedFrame(28,16,30,17,0,0,0,    colba24, colba24 );
  
  if(((110-net_data->power)/2)> 15)
     DrawRoundedFrame(32,9,34,17,0,0,0,   ( ((net_data->ch_number)<255)?cl2:cl4 ),( ((net_data->ch_number)<255)?cl2:cl4) );
  else
     DrawRoundedFrame(32,16,34,17,0,0,0,    colba24, colba24 );
  
  if(((110-net_data->power)/2)> 20)
     DrawRoundedFrame(36,7,38,17,0,0,0,   ( ((net_data->ch_number)<255)?cl2:cl4 ),( ((net_data->ch_number)<255)?cl2:cl4) );
  else
     DrawRoundedFrame(36,16,38,17,0,0,0,    colba24, colba24 );
  
  if(((110-net_data->power)/2)> 25) 
     DrawRoundedFrame(40,5,42,17,0,0,0,   ( ((net_data->ch_number)<255)?cl2:cl4 ),( ((net_data->ch_number)<255)?cl2:cl4) );
  else
     DrawRoundedFrame(40,16,42,17,0,0,0,    colba24, colba24 );


  //battary
  DrawRoundedFrame( 0,0,239,23, 0, 0, 0, co1ba2, co1ba2); 
  DrawRoundedFrame( 213,7,229,14, 0, 0, 0,  GetPaletteAdrByColorIndex(23), GetPaletteAdrByColorIndex(1)); 
  DrawRoundedFrame( 214,8,214+((14*battery)/100),13, 0, 0, 0,  GetPaletteAdrByColorIndex(4), GetPaletteAdrByColorIndex(4)); 
  DrawRoundedFrame( 12,13,13,14, 0, 0, 0,  colba24, colba24);
  DrawRoundedFrame( 12,14,13,16, 0, 0, 0,  colba23, colba23);
  DrawRoundedFrame( 11,14,14,15, 0, 0, 0,  colba23, colba23);
  DrawRoundedFrame( 116,10,125,16, 0, 0, 0,  colba23, colba23); 
}

void DrawClock()
{
  TTime time; 
  GetDateTime(0,&time); 
  char zs[10];
  sprintf(zs, "%02i:%02i",time.hour,time.min);
  int y = 30;
  int x = 30;
  
  DrwPngImg(FImage[FON_IMAGE], 0,24);//фон  

  int i;
  i = zs[0]-'0';
  for(int j=0;j<2;j++)//первые две циферки,тобишь часы
  {
    switch(i)
    {
      case 0: DrwPngImg(Dig[i], x, y);   break;
      case 1: DrwPngImg(Dig[i], x+8, y); break;
      case 2: DrwPngImg(Dig[i], x, y);   break;
      case 3: DrwPngImg(Dig[i], x, y);   break;
      case 4: DrwPngImg(Dig[i], x, y);   break;
      case 5: DrwPngImg(Dig[i], x, y);   break;
      case 6: DrwPngImg(Dig[i], x, y);   break;
      case 7: DrwPngImg(Dig[i], x+1, y); break;
      case 8: DrwPngImg(Dig[i], x+1, y); break;
      case 9: DrwPngImg(Dig[i], x, y);   break;        
    }
    i = zs[1] - '0';
    if(!j) x+=40;//прибавляем один раз
  }
  
  DrwPngImg(Dig[10], x+44, y); //двоетичие нах
  
  i = zs[3] - '0';
  x+=62;
  for(int j=0;j<2;j++)//вторые две циферки,тобишь минуты
  {
    switch(i)
    {
      case 0: DrwPngImg(Dig[i], x, y);   break;
      case 1: DrwPngImg(Dig[i], x+8, y); break;
      case 2: DrwPngImg(Dig[i], x, y);   break;
      case 3: DrwPngImg(Dig[i], x, y);   break;
      case 4: DrwPngImg(Dig[i], x, y);   break;
      case 5: DrwPngImg(Dig[i], x, y);   break;
      case 6: DrwPngImg(Dig[i], x, y);   break;
      case 7: DrwPngImg(Dig[i], x+1, y); break;
      case 8: DrwPngImg(Dig[i], x+1, y); break;
      case 9: DrwPngImg(Dig[i], x, y);   break;        
    }    
    i = zs[4] - '0';
    x+=40;
  }
}
/*
int slide = 0;
int yslide = 65;
GBSTMR slide_tmr;

void DrawSlide()
{
  if(yslide < 64)// slide = 0;
//  else
  {
    yslide++;
    slide = 1; 
    DrwPngImg(FImage[SLIDE],0,ScreenH()-1-yslide);
    REDRAW();
    GBS_StartTimerProc(&slide_tmr, 216*4, DrawSlide); 
  }
//  else
  //  DrwPngImg(FImage[SLIDE],0,329-FImage[SLIDE]->h);
  GBS_DelTimer(&slide_tmr);
}*/


GBSTMR tmr;
void myRedraw()
{
  REDRAW();
}


static void OnRedraw(MAIN_GUI *data)
{
  DisableIconBar(1);  
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x64","\x00\x00\x00\x64");
  DrawBattary(20,100);    
  DrawIconbar();
  DrawClock();
  
  wsprintf(data->ws1,"%t","Разблокировать #");
  
  int yslide = ScreenH()-1-FImage[SLIDE]->h;
  DrwPngImg(FImage[FON_IMAGE],0,yslide-2);
  DrwPngImg(FImage[SLIDE],0,yslide);
  DrawString(data->ws1,35,yslide+12,239,ScreenH()-1, 8, 2, colba23,0); 
  GBS_StartTimerProc(&tmr, 216*20, myRedraw); 
}

int cur_display;
int cur_keys;
static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->ws1 = AllocWS(128);
  cur_display = *(GetIlluminationDataTable()+3);
  cur_keys = *(GetIlluminationDataTable()+152+3);
  data->gui.state=1;
}


GBSTMR tmr_illumination;

void IlluminationOff()
{
  SetIllumination(0,1,0,256);
  SetIllumination(1,1,0,256);
}

void IlluminationOn(const int disp, const int key, const int tmr, const int fade)
{
  if(!tmr) return;
  GBS_DelTimer(&tmr_illumination);
  SetIllumination(0,1,disp,fade);
  SetIllumination(1,1,key,fade);
  GBS_StartTimerProc(&tmr_illumination,tmr*216,IlluminationOff);
}

//int MAINCSM_ID;
static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if(!IsUnlocked()) KbdUnlock();
  IlluminationOn(cur_display==0?20:cur_display,cur_keys,5,256);
//  IlluminationOff();
  //GBS_DelTimer(&slide_tmr);
  FreeWS(data->ws1);
  data->gui.state=0;
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  DisableIconBar(1);
//  if (state>3)
  if(IsUnlocked()) KbdLock();     
  data->gui.state=2;
  DisableIDLETMR();
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  CloseCSM(MAINCSM_ID);
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI();   
  if(msg->gbsmsg->msg==LONG_PRESS)
  {    
    switch(msg->gbsmsg->submess)
    {
      case '#': return 1;//break;
//    case '*': yslide = 0; break;
    }
  }
      
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    IlluminationOn(100,cur_keys,5,256);
/*    switch(msg->gbsmsg->submess)
    {
      case RED_BUTTON: return 1;
    }*/
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


int IsAlreadyRunning()
{
    int found = 0;
    CSM_RAM *icsm=((CSM_RAM *)(CSM_root()->csm_q->csm.first))->next; 
    while((((unsigned int)(icsm->constr)>>27)==0x15)&& (!found) )
    {
      WSHDR *tws=(WSHDR *)(((char *)icsm->constr)+sizeof(CSM_DESC));
      if((tws->ws_malloc==NAMECSM_MAGIC1)&&(tws->ws_mfree==NAMECSM_MAGIC2))
      {
        if (!memcmp(tws->wsbody+1, MAINCSM.maincsm_name.wsbody+1, tws->wsbody[0]*2))
          found = 1;
      }
      icsm=icsm->next;
    }
    return found;
}

const RECT Canvas={0,0,0,0};
static void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  MAINCSM_ID = csm->gui_id=CreateGUI(main_gui);
  if(IsAlreadyRunning()) CloseCSM(MAINCSM_ID);
//  cur_display = *(GetIlluminationDataTable()+3);
 // cur_keys = *(GetIlluminationDataTable()+152+3);  
}


//---------------------------------------------------------------------------//
/*
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}
*/

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


static void maincsm_onclose(CSM_RAM *csm)
{
  for(int i=0;i<IMAGE_NUM;i++)
  {
    if(FImage[i]->bitmap) mfree(FImage[i]->bitmap);
    if(FImage[i]) mfree(FImage[i]);
  }  

  GBS_StartTimerProc(&tmr_illumination,5*216,IlluminationOff);

  GBS_DelTimer(&tmr);    
  GBS_DelTimer(&tmr_illumination);    
  
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

const int minus11=-11;

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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"iPhoneSever");
}



int main(char *exename, char *fname)
{
  LockSched(); 
  MAIN_CSM main_csm;
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);  
  UnlockSched();  
  

  FImage[FON_IMAGE] =        read_pngimg(fon1_png);
  FImage[BATTARY_LOW] =      read_pngimg(exclam_png); 

  FImage[BAT_LEFT_GREEN] =   read_pngimg(left1_png);
  FImage[BAT_LEFT_RED] =     read_pngimg(left2_png);

  FImage[BAT_RIGHT_GREEN] =  read_pngimg(right1_png);
  FImage[BAT_RIGHT_EMPTY] =  read_pngimg(right2_png);
  FImage[LINE_RIGHT] =       read_pngimg(line2_png);
  
  FImage[BAT_CENTER_GREEN] = read_pngimg(center1_png);
  FImage[BAT_CENTER_EMPTY] = read_pngimg(center3_png);

  FImage[CLOCK] =            read_pngimg(clock_png);
  FImage[SLIDE] =            read_pngimg(slide_png);
  FImage[ICONBAR] =          read_pngimg(iconbar2_png);
  

  int x[2][11] ={  {0, 40,66,102,141,180,219,258,297,335,379},//смещение по х в первом имжхдр,откуда читать
                   {39,23,37,39, 39, 39, 39, 38, 38, 39, 13} };//смещение
  
  //циферки часов
  for(int i=0;i<=10;i++)
  {
    Dig[i] = CreateImgr(40, 55);
    drawimg2img(FImage[CLOCK], Dig[i], 0, 0, x[0][i], 0, x[1][i], 55);
  }
  return 0;
}
