#include "..\inc\swilib.h"
#include "rect_patcher.h"
#include "items_menu.h"
#include "view_item.h"
#include "main.h"
#include "strings.h"
#include "print.h"
/*
static const HEADER_DESC items_menu_hdr = {0, 0, 0, 0, NULL, (int)"RSS2.0 Reader", LGP_NULL};

static const int items_menu_softkeys[] = {0,1,2};

static const SOFTKEY_DESC items_menu_sk[]=
{
  {0x0018, 0x0000, (int)"View"},
  {0x0001, 0x0000, (int)"Close"},
  {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB items_menu_skt =
{
  items_menu_sk, 0
};

static int items_menu_onkey(void *data, GUI_MSG *msg)
{
  int i;
  RSS_ITEM *p=rss_first;
  i=GetCurMenuItem(data);
  for (int n=0; p && n!=i; n++)
  {
    p=p->next;    
  }
  if (msg->keys==0x3D)
  {
    if (p)
    {
      create_view_item(p);
    }
    return(-1);
  }
  return (0); 
}

static void items_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  } 
}

static void items_menu_iconhndl(void *data, int curitem, void *user_pointer)
{
  RSS_ITEM *p=rss_first;
  void *item=AllocMLMenuItem(data);
  
  WSHDR *ws2;
  WSHDR *ws4;

  WSHDR ws1loc, *ws1;
  unsigned short num[128];
  ws1=CreateLocalWS(&ws1loc,num,128);

  WSHDR ws3loc, *ws3;
  unsigned short num3[128];
  ws3=CreateLocalWS(&ws3loc,num3,128);
  
  for (int i=0; p && i!=curitem; i++)
  {
    p=p->next;    
  }
  if (p)
  {
    if (p->title)
    {
      ascii2ws(ws1, p->title);
    }
    else
    {
      ascii2ws(ws1, "Unknown title");
    }
    
    
    if (p->description)
    {
      ascii2ws(ws3, p->description);
    }
    else
    {
      ascii2ws(ws3, "No description");
    }
  }
  else
  {
    ascii2ws(ws1, "Error!");
  }

  ws2=AllocMenuWS(data,ws1->wsbody[0]);
  wstrcpy(ws2,ws1);
  
  ws4=AllocMenuWS(data,ws3->wsbody[0]);
  wstrcpy(ws4,ws3);
  
  SetMLMenuItemText(data, item, ws2, ws4, curitem);
}


static const ML_MENU_DESC items_menu_desc=
{
  8,items_menu_onkey,items_menu_ghook,NULL,
  items_menu_softkeys,
  &items_menu_skt,
  0x10, //+0x400
  items_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0,   //n
  1 //Добавочных строк  
};

int create_rssitems_menu(void)
{
  int i=0;
  RSS_ITEM *p=(RSS_ITEM *)&rss_first;
  while((p=p->next)) i++;
  patch_header(&items_menu_hdr);
  return CreateMultiLinesMenu(0,0,&items_menu_desc,&items_menu_hdr,0,i);
}


*/

/*------------------------------------------------------------------------*/
/*-------------------- Весь процесс отрисовки списка ---------------------*/
/*------------------------------------------------------------------------*/

int itemFromDisp;//кол-во на экране
int totalItems;//общее количество итемов
int curpos=0;//позичия курсора
int curpage=1;//текущая страница
int collum=0;//какая колонка
int scr_w, scr_h;
void *canvasdata;
int font=108;
int space=1;
const char clrSelect[4];//={0xFF,0x00,0x00,0x20};

char clrBlack[]={0x00,0x00,0x00,0x64};
char clrWhite[]={0xFF,0xFF,0xFF,0x64};
char clrRed[]={0xFF,0x00,0x48,0x64};
char transparent[]={0x00,0x00,0x00,0x00};
const char _t[]="%t";
char *CursorClr="\xFF\x00\x00\x1B";

#define CLIST_Y1 (YDISP + 2 + FontH(font))
//#define CLIST_Y1 (2 + ( FontH(font)>(headpic->h+1)?(FontH(font)):(headpic->h+1)) )
#define SF_Y  scr_h - FontH(font)-2 //SoftKey Y
#define Y1 FontH(font)+2  


int DrwImage(IMGHDR *img, int x, int y)
{
  if(!img->bitmap) return 0;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0);
  DrawObject(&drwobj);
  return 1;
}

IMGHDR DoScreen(int ScrH,int HSIZE)
{
  IMGHDR screen={0,0,8,""};
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen.w=ScrW;
  screen.h=ScrH;
  char *bitmap=malloc(ScrW*ScrH*2);
  screen.bitmap=bitmap;
  ms+=(HSIZE*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
  return screen;
}

IMGHDR SoftShot={0,0,8,""};
int soft_shot=0;

IMGHDR HeaderShot={0,0,8,""};
int header_shot=0;

DrawSoftKeys(char *right,char *left)
{
  TTime t;
  GetDateTime(0,&t);
  char *s=malloc(32);
  LockSched();
  int FntH=FontH(font);
  DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h,0,0,0,clrBlack,clrBlack);
  if(soft_shot!=2)
  {
    for(int i=FontH(font);i>0;i-=2)
      {
        DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i,0,0,0,CursorClr,CursorClr);
      }
      
    int ScrH = FntH+2;
    int HSIZE = (scr_h-FntH)*ScreenW();
    SoftShot = DoScreen(ScrH, HSIZE);
    soft_shot++;
    REDRAW();
    return;
  }
  else
    DrwImage(&SoftShot,0,scr_h-FntH-2);
  
  SetFontColor(clrWhite,transparent,0);  
  PrintField(2,scr_h-1-FontH(font),right,0,font,space);
  PrintField(scr_w,scr_h-1-FontH(font),left,2,font,space);
  
  sprintf(s,"%02d:%02d",t.hour,t.min);
  PrintField(scr_w/2,scr_h-1-FontH(font),s,1,font,space);
  UnlockSched();
  
  mfree(s);
}

DrawHeader(char *s)
{
   int hy = FontH(font) + YDISP;
   DrawRoundedFrame(0,YDISP,scr_w,hy+2,0,0,0,clrBlack,clrBlack);
   if(header_shot!=2)
   {
     for(int i=hy - YDISP; i>0 ;i-=2)
        DrawRoundedFrame(0,YDISP,scr_w,hy-i+4,0,0,0,CursorClr,CursorClr);
     
    // DoScreen(hy-YDISP+2, (hy-YDISP+2+8)*ScreenW());
    int ScrH  =  hy - YDISP + 2;
    int HSIZE = (hy - YDISP + 2 + 6)*ScreenW();
    HeaderShot = DoScreen(ScrH, HSIZE);
    header_shot++;
    REDRAW();
    return;
  }
  else
    DrwImage(&HeaderShot,0,YDISP);    
  
  SetFontColor(clrWhite,transparent,0);
  PrintText(2,YDISP,"NRSS v3.0",0,font,space,0,1,0,scr_w-2);
  PrintField(scr_w,YDISP,s,2,font,space);
}

static DrawScrollBar(int cur,int total)
{
  int y=scr_h-CLIST_Y1-FontH(font)-2;
  int pos=y*cur/total;
  int y2=y/total;
  DrawLine(scr_w-2,CLIST_Y1,scr_w-2,scr_h-FontH(font),1,clrBlack);
  int i=0;
  DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+y2,0,0,0,clrBlack,clrBlack);
  for(i=0;i<y2;i+=1)
  {
    DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+i,0,0,0,CursorClr,CursorClr);
  }
}


int cursor_shot=0;
IMGHDR cursor={0,0,8,""};

RSS_ITEM *GetItem(int curitem)
{
  RSS_ITEM *p=rss_first;
  for (int i=0; p && i!=curitem; i++)
        p=p->next;    
  
  return p;
}

//const char CursorClr[4]={0xFF,0x00,0x00,0x32};
const char CursorBGClr[4]={0x00,0x00,0x00,0x60};

static void DrawList()
{
  int font_y = FontH(font)+4;//16+2
  int ii;

  itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(font)-2)-1;

  LockSched(); 
 
  // Определяем количество страниц списка контактов
  int pageNum = sdiv(itemFromDisp, totalItems);
  
  RSS_ITEM *p;
  
  int i=0;
  int y2=1;
  int start_y=0;
  char *s=malloc(64);
  RECT rc;
  collum=0;

  while(i<totalItems)
  {
    if((i>=(curpage-1)*itemFromDisp) && (i<=itemFromDisp*curpage))//если те итемы который нужно выводить на экран со страници curpage
     {   
        p=GetItem(i);
        if (p)
        {
          sprintf(s,"%d/%d",curpos+1,totalItems);
          start_y=0;

          if(i>curpos)  start_y+=font_y+3;
          y2=2; 

          start_y += (CLIST_Y1 + 2 + (i - (curpage-1)*itemFromDisp)*font_y);    //определяем начальное смещение по у      
          StoreXYXYtoRECT(&rc,2,start_y,scr_w,start_y+(font_y*y2));            
          
          if(i==curpos)//Если наш итем то выделяем
          {
             SetFontColor("\xFF\x00\x00\x15",0,0);              
             DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2,0,0,0,clrBlack,clrBlack);//cursor        
              if(cursor_shot!=2)
              {
                for(ii=(font_y*y2);ii>0;ii-=4)
                   DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2-ii,0,0,0,CursorBGClr,CursorClr);

                int ScrH  =  rc.y + 2;
                int HSIZE = (rc.y2 - rc.y + 2 + 6)*ScreenW();
                cursor = DoScreen(ScrH, HSIZE);        
                
                cursor_shot++;
                REDRAW();
               return;
              }
              else
                DrwImage(&cursor,rc.x,rc.y);

            DrawHeader(s);
            DrawScrollBar(i,totalItems);
            
            PrintText(rc.x+5,rc.y+font_y,p->description ?  p->description : "No description",0,font,space,350,0,0,rc.x2);//description under item name
          }

          SetFontColor((char*)clrBlack,(char*)transparent,0); 
          PrintText(rc.x+5,rc.y+1,(p->title ? p->title : "Unknown title"),0,font,space,350,0,0,rc.x2-5);//item name          
         }
       }
      i++;
      if(i>(pageNum*itemFromDisp)) break;
  }  
  mfree(s);
  UnlockSched();  
}



// Управление курсором
void MoveUp(int i)
{
  if(!itemFromDisp)return;
  if(curpos<1)
  {
    curpos=totalItems-1;
    
    if(itemFromDisp!=totalItems)
    {
      curpage = sdiv(itemFromDisp,totalItems)+1;
    }
    else
      curpage = 1;
  }
  else
  {
    if(i) curpos-=2;
    else curpos--;
    if(curpos<(curpage-1)*itemFromDisp){ curpos++; curpage--; }
  }
 // REDRAW();
}


void MoveDown(int i)
{
  if(!totalItems)return;
  if(i) curpos+=2;  
  else curpos++;
  if(curpos>=totalItems) { curpos=0; curpage=1; }
  if(curpage*itemFromDisp<curpos){ curpos--; curpage++; } 
}

static void OnRedraw(MAIN_GUI *data)
{
  LockSched();
  DrawRoundedFrame(0,YDISP,scr_w,scr_h,0,0,0,clrWhite,clrWhite);
  
  DrawLine(0,CLIST_Y1,scr_w,CLIST_Y1,0,clrWhite);
  
  DrawSoftKeys("Выбрать","Закрыть");
  UnlockSched();

  DrawList();
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  scr_w=ScreenW()-1;
  scr_h=ScreenH()-1;
  data->ws1=AllocWS(256);
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  
  FreeWS(data->ws1);
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
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {  
      case UP_BUTTON:    MoveUp(0);   break;
      case DOWN_BUTTON:  MoveDown(0); break;
      case LEFT_BUTTON:  MoveUp(1);   break;
      case RIGHT_BUTTON: MoveDown(1); break;
      case '#' :
      {
        WSHDR *ws;
        ws=AllocWS(150);
        extern const char *successed_config_filename;
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);       
      }
      break;
      case RIGHT_SOFT:case RED_BUTTON: return (1);
    }
  } 


  
  if ((msg->gbsmsg->msg==KEY_DOWN) && (k==ENTER_BUTTON || k==LEFT_SOFT))
  {
    RSS_ITEM *p=GetItem(curpos);
    if (p)
    {
      create_view_item(p);
    }
    return(-1);
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

static int method8(void){return(0);}
static int method9(void){return(0);}

static const void * const gui_methods[11]={
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


int create_rssitems_menu(void)
{
  totalItems=0;
  RSS_ITEM *pp=(RSS_ITEM *)&rss_first;
  while((pp=pp->next)) totalItems++;  
  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
//  main_gui->gui.canvas=(void *)(&MM_Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  return CreateGUI(main_gui);   
}


