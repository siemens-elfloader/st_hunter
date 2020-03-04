#include "swilib.h"
#include "readimg.h"

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

IPC_REQ gipc;
const char ipc_send_name[]="Checker";
const char ipc_my_name[]="test";

typedef struct
{
  void *next;
  int index;
  IMGHDR *img;
}ICONS;

ICONS *icontop;/*
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
*/
void DrwImg(IMGHDR *img, int x, int y)
{
  if(!img) return;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0/*pen,brush*/);
  DrawObject(&drwobj);
}



static void OnRedraw(MAIN_GUI *data)
{
  DisableIconBar(1);  
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x64","\x00\x00\x00\x64");
  if(!icontop)return;
  ICONS *icon = icontop;
  int y=0;
  int x=3;
//  for(int i=0;i<35;i++)
  int i=0;
  while(icon->next)
  {
    DrwImg(icon->img,x,y);
    y+=icon->img->h+2;
    icon = icon->next;
    if(i++ == 18) { x = 60; y = 0; }
  }   
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->ws1 = AllocWS(128);
  data->gui.state=1;
}

//int MAINCSM_ID;
static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  data->gui.state=0;
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  DisableIconBar(1);
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
      
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
      case RED_BUTTON: return 1;
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
/*

const char *icons_names[13]=
{
  "offline.png",
  "online.png",
  "away.png",
  "ffc.png",
  "dnd.png",
  "evil.png",
  "na.png",
  "occupied.png",
  "depression.png",
  "invisible.png",  
  "lunch.png",
  "work.png",
  "home.png"
};

typedef struct
{
  int index;
  int w;
  int h;
}POS;

enum ISTATUS
{
  IS_OFFLINE,
  IS_INVISIBLE,
  IS_AWAY,
  IS_NA,
  IS_OCCUPIED,
  IS_DND,
  IS_DEPRESSION,
  IS_EVIL,
  IS_HOME,
  IS_LUNCH,
  IS_WORK,
  IS_ONLINE,
  IS_FFC,
//
  IS_MSG,
  IS_UNKNOWN,
  IS_GROUP,
  IS_GROUPOFF,
  IS_NULLICON,
//
  ICON_ADDCONTACT,
  ICON_HEAD,
  ICON_PING,
  ICON_SETTINGS,
//
  ICON_ALL_CAN_SEE,
  ICON_VISLIST_CAN_SEE,
  ICON_INVISLIST_CANNOT_SEE,
  ICON_CONTACTLIST_CAN_SEE,
  ICON_NOBODY_CAN_SEE,
//
  ICON_CHECK,
  ICON_UNCHECK,
//
  TOTAL_ICONS,
//
};

POS status[]=
{
  {IS_OFFLINE,    15,14},
  {IS_INVISIBLE,  14,14},
  {IS_AWAY,       15,14},
  {IS_NA,         15,14},
  {IS_OCCUPIED,   14,14},
  {IS_DND,        14,14},
  {IS_DEPRESSION, 15,14},
  {IS_EVIL,       15,14},
  {IS_HOME,       15,14},
  {IS_LUNCH,      16,14},
  {IS_WORK,       15,14},
  {IS_ONLINE,     14,14},
  {IS_FFC,        15,14},
  {IS_MSG,        14,14},
  {IS_UNKNOWN,    15,14},
  {IS_GROUP,      14,14},
  {IS_GROUPOFF,   14,14},
  {IS_NULLICON,   13,14},
  {ICON_ADDCONTACT,    14,14},
  {ICON_HEAD,       15,14},
  {ICON_PING,       15,14},
  {ICON_SETTINGS,   16,14},
  {ICON_ALL_CAN_SEE,          17,16},
  {ICON_VISLIST_CAN_SEE,      17,16},
  {ICON_INVISLIST_CANNOT_SEE, 17,16},
  {ICON_CONTACTLIST_CAN_SEE,  17,16},
  {ICON_NOBODY_CAN_SEE,       17,16},
  {ICON_CHECK,      14,14},
  {ICON_UNCHECK,    14,14}
//  TOTAL_ICONS
};
*/



const RECT Canvas={0,0,0,0};

static void maincsm_oncreate(CSM_RAM *data)
{
  /* IMGHDR *img = CreateIMGHDRFromPngFile("4:\\xst.png",0);
   int start = 0;
   for(int i=0;i<35;i++)
   {
      XStatus[i] = CreateImgr(16,16);   
      drawimg2img(img,XStatus[i],0,0,start,0,start+16,16);
      start+=17;
      if(start > img->w) break;
   }*/
  //    int size = 0;
   /*   int start = 0;
      int i2 = 0;
      for(int i=0;i<TOTAL_ICONS-1;i++)
      {
        if(i < IS_FFC)
        {
         // if(i == online || i == invisible || i == occupied) size = 14;
          //else size = 15;
//          size = status[i].w;
          SImg[i] = CreateImgr(status[i].w,status[i].h);
          drawimg2img(img,SImg[i],0,0,start,0,start + status[i].w,status[i].h);
          start += status[i].w;
        }
        else
        {
          if(!i2)
          {
            mfree(img);
            img = CreateIMGHDRFromPngFile("4:\\image.png",0);
            i2 = 1;
           // size = 0;
            start = 0;
          }
          SImg[i] = CreateImgr(status[i].w, status[i].h);
          drawimg2img(img,SImg[i],0,0,start,0,start + status[i].w,status[i].h);
          start += status[i].w;
          if(start > img->w) break;
        }
       }
      */
  /*
  int size = 0;
  IMGHDR *hdr = CreateIMGHDRFromPngFile("4:\\status.png",0);
  IMGHDR *hd2 = CreateIMGHDRFromPngFile("4:\\image.png",0);
  int start = 0;
  for(int i=0;i<13;i++)
  {
    if(i == online || i == invisible || i == occupied) size = 14;
    else size = 15;
    Img[i] = CreateImgr(size,14);
    drawimg2img(hdr,Img[i],0,0,start,0,start+size,14);
    start += size;
    if(start > hdr->w) break;
  }
  
  size = 0;
  start = 0;
  
  for(int i=0;i<16;i++)
  {
    if(i == null || i == setting || i == unknown) size = 15;
    else if(i == ping) size = 13;
    else if(i >= vis1 && i <= vis5) size = 16;
    else size = 14;
    Pic[i] = CreateImgr(size, (i >= vis1 && i <= vis5) ? 16 : 14);
    drawimg2img(hd2,Pic[i],0,0,start,0,start+size,(i >= vis1 && i <= vis5) ? 16 : 14);
    start += size;
    if(start > hd2->w) break;
  }*/
//  mfree(hdr);
  
  
  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  MAINCSM_ID = csm->gui_id=CreateGUI(main_gui);
  
  gipc.name_to=ipc_send_name;
  gipc.name_from=ipc_my_name;
  gipc.data = 0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,2,&gipc);
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
  SUBPROC((void *)ElfKiller);
}

int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
    
  {
    //IPC
    if (msg->msg==MSG_IPC)
    {
      IPC_REQ *ipc;
      if ((ipc=(IPC_REQ*)msg->data0))
      {
	if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
	{
	  switch (msg->submess)
	  {
          case 1:
             //gipc.name_to=ipc_send_name;
             //gipc.name_from=ipc_my_name;
             icontop = ipc->data;
             //GBS_SendMessage(MMI_CEPID,MSG_IPC,1,&gipc);
             break;
          }
        }
      }
    }
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

//  CFile *file = new CFile();

  /*
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
                   {39,23,37,39, 39, 39, 39, 38, 38, 39, 13} };//смещение во втором имжхдр,куда записывать
  
  //циферки часов
  for(int i=0;i<=10;i++)
  {
    Dig[i] = CreateImgr(40, 55);
    drawimg2img(FImage[CLOCK], Dig[i], 0, 0, x[0][i], 0, x[1][i], 55);
  }*/
  return 0;
}
