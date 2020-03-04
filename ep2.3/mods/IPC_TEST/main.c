#include "..\inc\swilib.h"
#include "readimg.h"
const int minus11=-11;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

CSM_RAM *under_idle;
IPC_REQ gipc;
const char ipc_my_name[]="Checker";
const char ipc_send_name[]="test";

extern void kill_data(void *p, void (*func_p)(void *));
typedef struct
{
  void *next;
  int index;
  IMGHDR *img;
}ICONS;

ICONS *iconstop;

int drawimg2img(IMGHDR * onse, IMGHDR * timage2, int x, int y, int xRect, int yRect,int xxRect, int yyRect);

void AddIcon(IMGHDR *img,int x,int y)
{
   ICONS *icon = malloc(sizeof(ICONS));
   icon->next = 0;
   icon->img = CreateImgr(28,28);
   drawimg2img(img,icon->img,0,0,x,y,x+27,y+27);
 
   if(!iconstop)
   {
     iconstop=icon; 
   }
   else
   {
      ICONS *ico=(ICONS *)iconstop;
      while(ico->next) ico = ico->next;
      ico->next=icon;
   }   
}


void FreeIconList()
{  
  LockSched();
  ICONS *icon=(ICONS *)iconstop;
  iconstop=0;
  UnlockSched();
  while(icon)
  {
    ICONS *icon_prev = icon;
    icon = icon->next;
    if(icon_prev) mfree(icon_prev);
  }
}


int drawimg2img(IMGHDR * onse, IMGHDR * timage2, int x, int y, int xRect, int yRect,int xxRect, int yyRect)
{
  
  if (!onse) return 1;
  if (yRect<0 || yRect>=onse->h) yRect = 0;
  if (xRect<0 || xRect>=onse->w) xRect = 0;  
  if (yyRect==0 || (yRect+yyRect >= onse->h)) yyRect = onse->h-yRect;  
  if (xxRect==0 || (xRect+xxRect >= onse->w)) xxRect = onse->w-xRect;  
  int *scr = (int*)((char*)timage2->bitmap);
  int *pic = (int*)((char*)onse->bitmap);
  int i,j;

  for (i = xRect; i < xRect+xxRect; i++)
  {
    for(j = yRect; j < yRect+yyRect; j++)
    {
      //i ? ????????? ??????? ??? ??????? (?? xRect ?? xRect+xxRect)                                                                         //j ? ????????? ??????? ??? ??????? (?? yRect ?? yRect+yyRect)
      if (((y+j-yRect)<timage2->h) && ((x+i-xRect)<timage2->w) && ((y+j-yRect)>=0) && ((x+i-xRect)>=0) 
          &&(i>=xRect)&&(j>=yRect)&&(i<xRect+xxRect)&&(j<yRect+yyRect))
       {   
         scr[(y+j-yRect)*timage2->w+i+x-xRect] = pic[(j)*onse->w+i];
       }
    }
  }
 return 0;
}


void LoadImages()
{
  int x=0,y=0;
  IMGHDR *img = CreateIMGHDRFromPngFile("4:\\smiles.png",0);//("4:\\status.png",0);
  for(int count=0;count<96;count++)
  {
    if((count > 0 && count%12 == 0) || (x > img->w)) { x = 0; y+=28; }
    AddIcon(img,x,y);
    x += 28;
    //if(x > img->w) 
    if(y > img->h) break;
  }
  
  mfree(img->bitmap);
  mfree(img);
}


unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

unsigned short maincsm_name_body[140];

int my_csm_id=0;

void DrwImage(IMGHDR *img, int x, int y)
{
  if(!img) return;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0/*pen,brush*/);
  DrawObject(&drwobj);
}

void MM_OnRedraw(MAIN_GUI *data)
{
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x64","\x00\x00\x00\x64");
  ICONS *icon = iconstop;
  int x=0;
  int y=YDISP+1;
  for(;;)
  {
    DrwImage(icon->img,x+1,y);
    x+=icon->img->w;
    if(x>240){ x=0; y+=icon->img->h;}    
    icon=icon->next;
    if(!icon) break;
  }
}


//------------------------------------------


void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
    DisableIconBar(1);
#endif
  data->gui.state=1;
}

void MM_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

void MM_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
    DisableIconBar(1);
#endif
  
  data->gui.state=2;
  DisableIDLETMR();
}

void MM_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int MM_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
    case RED_BUTTON : return 1;
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int MM_method8(void){return(0);}
int MM_method9(void){return(0);}

static const void * const gui_methods[11]={
  (void *)MM_OnRedraw,
  (void *)MM_onCreate,
  (void *)MM_onClose,
  (void *)MM_onFocus,
  (void *)MM_onUnfocus,
  (void *)MM_OnKey,
  0,
  (void *)kill_data,
  (void *)MM_method8,
  (void *)MM_method9,
  0
};


const RECT Canvas={0,0,0,0};


#pragma inline=forced

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

int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
/* MAIN_CSM *csm=(MAIN_CSM*)data;
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
          case 2:
             gipc.name_to=ipc_send_name;
             gipc.name_from=ipc_my_name;
             gipc.data = iconstop;
             GBS_SendMessage(MMI_CEPID,MSG_IPC,1,&gipc);
	     ShowMSG(1,(int)"Checker: ipc recv & send");
             break;
          }
        }
      }
    }
  }*/
  return (1);  
}

static void maincsm_oncreate(CSM_RAM *data)
{
  LoadImages(); 
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

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
  FreeIconList();
  SUBPROC((void *)Killer);
}

static unsigned short maincsm_name_body[140];

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

static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NSD");
}

int main()
{
  LockSched(); 
  MAIN_CSM main_csm;
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);  
  UnlockSched();  
 
  /* CSM_RAM *save_cmpc;
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();  
  LockSched();
  save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
  CSM_root()->csm_q->current_msg_processing_csm=CSM_root()->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,dummy,0);
  CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
*/
  return 0;
}
