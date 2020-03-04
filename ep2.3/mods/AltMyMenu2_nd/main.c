#include "..\inc\swilib.h"
#include "local_ipc.h"
#include "../inc/xtask_ipc.h"
#include "main.h"
#include "language.h"
#include "print.h"
#include "..\inc\myapi.h"
//Сдесь все криво,мне лень переписывать....работает и хуй с ним....

#define FUNC_PT_START	0xA085DEA4
#define FUNC_PT_END	0xA0865BA3
#define FUNC_PT_PSIZE	0x80
#define FUNC_PT_OFFSET	0x10
#define MAX_FUNC	0xFA

GBSTMR mytmr;
int menus_id;
int active;
int MAINGUI_ID;

const char ipc_my_name[32]=IPC_TEXTINFO_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
IPC_REQ gipc;

#define TMR_SECOND 216

const char _s[] = "%s";
const char _d[] = "%d";
const char _t[] = "%t";
char empty_str[] = "";

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

//Взято с китайского свн sieelf.googlecode.com/svn (r)BingK

#ifdef NEWSGOLD
unsigned int GetFunctionPointByName(char *name)
{
  char *p=(char *)FUNC_PT_START;
  int len=strlen(name);
  if(len==0)
     return 0;
  while((char *)FUNC_PT_END-p>0)
   {
     if(!strncmpNoCase(p, name, len))
	return (*(unsigned int *)(p+FUNC_PT_OFFSET));
     p+=FUNC_PT_PSIZE;
   }
  return 0;
}
#endif


void run_address(unsigned int address) 
{
  void (*runaddr)(void);
  runaddr=(void (*)())address;
  if (runaddr) 
  {
    runaddr();
  }
}

void run_shortcut(char *shortcut) 
{
#ifdef NEWSGOLD
   run_address(GetFunctionPointByName(shortcut));
#else
   run_address((unsigned int)GetFunctionPointer(shortcut));
#endif
}

int chr2num(char chr)
{
  if(chr>='0'&&chr<='9')
     return (chr-('0'-0));
  if(chr>='A'&&chr<='Z')
     return (chr-('A'-0xA));
  if(chr>='a'&&chr<='z')
     return (chr-('a'-0xA));
  return 0;
}

int RunBM2(int bm)
{
 const char *s=bmfile(bm);
 if((s)&&strlen(s))
 {
   
  if(strstr(s,"SetProfile"))//SetProfile(1;3)
  {
    (GetProfile()!=2) ? SetProfile(2) : SetProfile(0);
    return 1;
  }
  //MakeVoiceCall(const char *number,int _0x10, int _0x20C0);
  
  if(s[0]=='8' || s[0]=='+' || strstr(s,"call:"))
  {
    const char *p;
    if(strstr(s,"call:")) p=strrchr(s,':')+1;
    else p=s;

    MakeVoiceCall(p,0x10,0x20C0); 
    return 1;
  }
  
  if ((s[2]=='\\')&&((s[(strlen(s))-3]=='.')||//Проверяем строку на наличие символов '\\' и '.'
     (s[(strlen(s))-4]=='.')||(s[(strlen(s))-5]=='.')))
       {
        WSHDR *ws;
        ws=AllocWS(256);
        str_2ws(ws,s,256);
        if(ExecuteFile(ws,0,0))
        {
          FreeWS(ws);
          return(1);
        }
        else
        {
          FreeWS(ws);
          return 0;
        }
       }
    
    if ((s[2]!='\\')&&(s[(strlen(s))-3]!='.')&&
       (s[(strlen(s))-4]!='.')&&(s[(strlen(s))-5]!='.')&&
       ((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))  
        {
          int i;
          unsigned int addr=0;
          char *p=(char*)s;
          for(i=0;i<strlen(s);i++)
          {
             addr=chr2num(p[i])+addr*0x10;
          }
          run_address(addr);
          return 1;
        }
    else
    {
          char ShortCut[32];
          char *p=(char*)s;
          int i=0;
          int j=0;
          for(;j<strlen(s);j++)
          {
            if(p[j]>='A')
            {
              ShortCut[i]=p[j];
              i++;
            }
          }
          ShortCut[i]=0;
          run_shortcut(ShortCut);     
          return 1;
    }  
  }
 return(0);
}


int num_book()
{
  int i=0;
  for(i=0;i<28;i++)
  {
    if(strlen(bmfile(i))==0) return i;    
  }
  return i;
}

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}


/*------------------------------------------------------------------------*/
/*-------------------- Весь процесс отрисовки списка ---------------------*/
/*------------------------------------------------------------------------*/

char img_dir[];
extern const char fntSelect[4];//={0xFF,0xFF,0x00,0x60};
extern const char bordSelect[4];//={0x00,0x00,0x00,0x60};

const char clrNotSelect[4]={0xFF,0x00,0x00,0x60};
const char bordNotSelect[4]={0x00,0x00,0x00,0x60};
const char fntNotSelect[4]={0xFF,0xFF,0xFF,0x60};


typedef struct
{
  void *next;
  char *name;
  int name_size;
  char *descript;
  int desc_len;
  IMGHDR *icon;
  int icon_size;
}BOOKMARKS;

typedef struct
{
  int curpos; //позичия курсора
  unsigned int itemFromDisp; //кол-во на экране
  unsigned int totalItems; //общее количество итемов
  unsigned int curpage; //текущая страница
  unsigned int column; //какая колонка
  
  int font;
  char *fntSelect;
  char *bordSelect;
  
  char *fntNotSelect;
  char *clrNotSelect;
  char *bordNotSelect;
  
  char *CursorBGClr;
  char *CursorClr;

  IMGHDR *noicon;
  IMGHDR *bgpic;
  IMGHDR *headpic;
  
  IMGHDR ScreenSoft;
  int soft;

  IMGHDR ScreenHeader;
  int header;
  
  IMGHDR ScreenCursor;
  int cursor;
    
  RECT rc;
}GUIMENU;

GUIMENU *listm;

void FreeMenu()
{
 if(listm)
 {
   if(listm->noicon)  mfree(listm->noicon);
   if(listm->bgpic)   mfree(listm->bgpic);
   if(listm->headpic) mfree(listm->headpic);
   if(listm->ScreenHeader.bitmap) mfree(listm->ScreenHeader.bitmap);
   if(listm->ScreenHeader.bitmap) mfree(listm->ScreenHeader.bitmap);
   if(listm->ScreenCursor.bitmap) mfree(listm->ScreenCursor.bitmap);
   mfree((void*)listm);
 }
  PrintLibFree();
}

int InitMenu()
{  
  if(!isSystemFont)
  {
    if(!strlen(fontpath))
    {      
      WSHDR *ws;
      ws=AllocWS(150);
      extern const char *successed_config_filename;
      str_2ws(ws,successed_config_filename,128);
      ExecuteFile(ws,0,0);
      FreeWS(ws);
      ShowMSG(1,(int)"Set fonts!!!");
      void ElfKiller(void);
      SUBPROC((void *)ElfKiller);
      return 1;
    }
  }
  
  FreeMenu();
 
  listm = malloc(sizeof(GUIMENU));
  
  listm->curpos = 0;
  listm->itemFromDisp = 0;
  listm->totalItems = 0;
  listm->curpage = 1;
  listm->column = 0;
  
  if(isSystemFont) listm->font = 100 + pngFont;
  else listm->font = 1;

  listm->ScreenSoft.bpnum =  8;
  listm->ScreenSoft.bitmap = empty_str;
//    = {0,0,8,""};
  listm->soft = 0;
  listm->ScreenHeader.bpnum = 8;// = {0,0,8,""};
  listm->ScreenHeader.bitmap = empty_str;
  listm->header = 0;
  listm->ScreenCursor.bpnum = 8;//{0,0,8,""};
  listm->ScreenCursor.bitmap = empty_str;
  listm->cursor = 0;
  
  listm->fntSelect = (char*)fntSelect;
  listm->bordSelect = (char*)bordSelect;
  
  listm->fntNotSelect = (char*)fntNotSelect;
  listm->clrNotSelect = (char*)clrNotSelect;
  listm->bordNotSelect = (char*)bordNotSelect;  

  listm->CursorBGClr = (char*)CursorBGClr;
  listm->CursorClr = (char*)CursorClr;
  
  PrintLibInit(1);  
  SetCanvasFlag(1);
  
  FontPathInit((char *)fontpath, fullfont);
  if(isSystemFont)
    SetFontColor((char*)listm->fntSelect,(char*)listm->bordSelect,isOutline); 
  
  char s[128];
  sprintf(s,"%s\\no_icon.png",img_dir);
  listm->noicon=CreateIMGHDRFromPngFile((char*)s,2);  

  sprintf(s,"%s\\bg.png",img_dir);
  listm->bgpic=CreateIMGHDRFromPngFile(s,2);  

  sprintf(s,"%s\\head.png",img_dir);
  listm->headpic=CreateIMGHDRFromPngFile((char*)s,2);    
  return 0;
}

int scr_w, scr_h;

void *canvasdata;

char clrBlack[] = {0x00,0x00,0x00,0x64};
char clrWhite[] = {0xFF,0xFF,0xFF,0x64};
char clrRed[] = {0xFF,0x00,0x48,0x64};
char transparent[] = {0x00,0x00,0x00,0x00};

char img_dir[128];

volatile BOOKMARKS *bmktop;

#define CLIST_Y1 (YDISP + 2 + FontH(listm->font))
#define SF_Y  scr_h - FontH(listm->font)-2 //SoftKey Y
#define Y1 FontH(listm->font)+2  


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



void SMART_REDRAW(void)
{
  int f;
  LockSched();
  f=IsGuiOnTop(menus_id);
  UnlockSched();
  if (f) REDRAW();
}

static DrawSoftKeys(char *right,char *left)
{
  TTime t;
  GetDateTime(0,&t);
  char *s=malloc(32);
  LockSched();
  int FntH=FontH(listm->font);
  DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h,0,0,0,clrBlack,clrBlack);
  if(listm->soft!=2)
  {
    for(int i=FontH(listm->font);i>0;i-=2)      
        DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i,0,0,0,CursorClr,CursorClr);      
      
    int ScrH = FntH+2;
    int HSIZE = (scr_h-FntH)*ScreenW();
    listm->ScreenSoft = DoScreen(ScrH, HSIZE);
    listm->soft++;
    SMART_REDRAW();
    return;
  }
  else
    DrwImage(&listm->ScreenSoft,0,scr_h-FntH-2);
  
  if(isSystemFont) SetFontColor(clrWhite,transparent,0);  
  PrintField(2,scr_h-1-FontH(listm->font),right,0,listm->font,space);
  PrintField(scr_w,scr_h-1-FontH(listm->font),left,2,listm->font,space);
  
  sprintf(s,"%02d:%02d",t.hour,t.min);
  PrintField(scr_w/2,scr_h-1-FontH(listm->font),s,1,listm->font,space);
  UnlockSched();
  
  mfree(s);
}

static DrawHeader(BOOKMARKS *bmk,char *s,int x)
{
   int hy = FontH(listm->font) + YDISP;
   DrawRoundedFrame(0,YDISP,scr_w,hy+2,0,0,0,clrBlack,clrBlack);
   if(listm->header!=2)
   {
     for(int i=hy - YDISP; i>0 ;i-=2)
        DrawRoundedFrame(0,YDISP,scr_w,hy-i+4,0,0,0,CursorClr,CursorClr);
     
    // DoScreen(hy-YDISP+2, (hy-YDISP+2+8)*ScreenW());
    int ScrH  =  hy - YDISP + 2;
    int HSIZE = (hy - YDISP + 2 + 6)*ScreenW();
    listm->ScreenHeader = DoScreen(ScrH, HSIZE);
    listm->header++;
    SMART_REDRAW();
    return;
  }
  else
    DrwImage(&listm->ScreenHeader,0,YDISP);    
  
   DrwImage(listm->headpic,2,YDISP);
   if(isSystemFont) SetFontColor(clrWhite,transparent,0);
   PrintField(scr_w,YDISP,s,2,listm->font,space);//description in header
   int x2=GetWidth(scr_w,YDISP);
   if(!LINE_MODE)
   {
     if(bmk->desc_len)
        PrintText(x+2,1,bmk->descript,0,listm->font,space,0,1,0,scr_w-x2-2);//description in header
//       PrintField(x+2,1,bmk->descript,0,listm->font,space);//description in header
     else 
        PrintText(x+2,YDISP,LG_NOT_DESCRIPT,0,listm->font,space,0,1,0,scr_w-x2-2);
//       PrintField(x+2,YDISP,LG_NOT_DESCRIPT,0,listm->font,space);
   }
   else
   {
      PrintText(x+2,YDISP,"AltMyMenu2 v0.4",0,listm->font,space,0,1,0,scr_w-x2-2);
     //PrintField(x+2,YDISP,"AltMyMenu2 v0.4",0,listm->font,space);
     
   }
}

static DrawScrollBar(int cur,int total)
{
  int y=scr_h-CLIST_Y1-FontH(listm->font)-2;
  int pos=y*cur/total;
  int y2=y/total;
  DrawLine(scr_w-2,CLIST_Y1,scr_w-2,scr_h-FontH(listm->font),1,clrBlack);
  int i=0;
  DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+y2,0,0,0,clrBlack,clrBlack);
  for(i=0;i<y2;i+=1)
  {
    DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+i,0,0,0,CursorClr,CursorClr);
  }
}

void PrintAbout()
{
  char *s=malloc(128);
  sprintf(s,"AltMyMenu2 v0.4\n(c)kluchnik\n%s at %s",__DATE__,__TIME__);
  DrawRoundedFrame(0,YDISP+FontH(listm->font),scr_w,scr_h,0,0,0,clrWhite,clrWhite);  
//  DrwImage(bgpic,1,YDISP+FontH(listm->font)+4);
  if(isSystemFont) SetFontColor(clrBlack,transparent,0);
  PrintText(ScreenW()/2,(ScreenH()/2)-(FontH(listm->font)*1.5),s,1,listm->font,space,1,0,0,ScreenW()-1);
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  mfree(s);
}

void AddToBmk(int numbmk,char *name,char *descript,char *icon)
{
 BOOKMARKS *bmk=malloc(sizeof(BOOKMARKS));
 bmk->next=0;
 bmk->name=malloc(strlen(name)+5);
 bmk->descript=descript;
 bmk->desc_len=strlen(descript);

 bmk->icon=CreateIMGHDRFromPngFile(icon, 2);   
 bmk->icon_size=get_file_size(icon);
 sprintf(bmk->name,"%d. %s",numbmk,name);
 
 if(!bmktop)
 {
   bmktop=bmk; 
 }
 else
 {
   BOOKMARKS *bm=(BOOKMARKS *)bmktop;
   while(bm->next)
       bm=bm->next;
   bm->next=bmk;   
 } 
}

void FreeBmkList()
{  
  LockSched();
  BOOKMARKS *bmk=(BOOKMARKS *)bmktop;
  bmktop=0;
  UnlockSched();
  while(bmk)
  {
    BOOKMARKS *bmk_prev=bmk;
    bmk=bmk->next;
    mfree(bmk_prev);
  }
}

BOOKMARKS *GetBmkByItem(int curitem)
{
  BOOKMARKS *bmk;
  bmk=(BOOKMARKS *)bmktop;
  int i=0;
  while(bmk)
  {
    if(i==curitem)
      return bmk;
    i++;
    if(bmk->next) bmk=bmk->next;  
    else return 0;
  }
  return 0;
}

int TotalBmk()
{
  if(!bmktop) return 0;
  BOOKMARKS *bmk;
  bmk=(BOOKMARKS *)bmktop;
  int i=1; 
  while(bmk=bmk->next) i++;
  return i;
}

void InitBmk()
{
  FreeBmkList();
  
  if(listm->bgpic)   mfree(listm->bgpic->bitmap);
  if(listm->headpic) mfree(listm->headpic->bitmap);
  if(listm->noicon)   mfree(listm->noicon->bitmap);
  
  int i=num_book();
  for(int j=0;j<i;j++)
    AddToBmk(j+1,(char*)bmname(j),(char*)description(j),(char*)S_ICONS[j]);
}

int cursor_shot=0;
IMGHDR cursor={0,0,8,""};

static void DrawList(WSHDR *ws)
{
  int font_y = FontH(listm->font)+4;//16+2
  int ii;

  if(VIEW_MODE)    
    listm->itemFromDisp = (sdiv(font_y,scr_h-CLIST_Y1-FontH(listm->font)-2)*2)-1;
  else      
    listm->itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(listm->font)-2)-1;

  LockSched();
  listm->totalItems = TotalBmk();
  
  if(listm->itemFromDisp > (CLIST_Y1 - (2-FontH(listm->font)))) listm->itemFromDisp--; //Вдруг надпись будет налазить на софты ;)
  if(LINE_MODE) listm->itemFromDisp--;
  
  BOOKMARKS *bmk;
  bmk=(BOOKMARKS *)bmktop;
  int i=0;
  int nlines=1;
  int start_y=0;
  int start_x=0;
  int x=iconSize;
  char *s=malloc(64);
  RECT rc;
  int oldY=0;  
  listm->column = 0;

  while(i<listm->totalItems)
  {
    if((i>=(listm->curpage-1)*listm->itemFromDisp) && (i<=listm->itemFromDisp*listm->curpage))//если те итемы который нужно выводить на экран со страници curpage
     {
        bmk=GetBmkByItem(i);//ищем итем
        if(!bmk) break;      
        if(bmk)//если такой есть то рисуем его
        {
          sprintf(s,"%d/%d",listm->curpos+1,listm->totalItems);
          start_y=0;

          if(LINE_MODE)//если в две линии
          { 
            if(i>listm->curpos) start_y += font_y + 3;
            nlines = 2; 
          }
          else nlines = 1;
          
          if(VIEW_MODE)//если в два столбца
          {
            if(!listm->column)
            {
              oldY = start_y += (CLIST_Y1 + 2 + (((i - (listm->curpage-1)*listm->itemFromDisp)*font_y)/2) ); 
              StoreXYXYtoRECT(&rc,2,start_y,(scr_w/2)+5,start_y+(font_y*nlines));
            }
            else 
            {
              start_x = (scr_w/2)+1;
              StoreXYXYtoRECT(&rc,start_x,oldY,scr_w-5,oldY+(font_y*nlines));
            }
            listm->column=!listm->column;
          }
          else
          {
            start_y += (CLIST_Y1 + 2 + (i - (listm->curpage-1)*listm->itemFromDisp)*font_y);    //определяем начальное смещение по у      
            StoreXYXYtoRECT(&rc,2,start_y,scr_w,start_y+(font_y*nlines));            
          }                
          
          if(i==listm->curpos)//Если наш итем то выделяем
          {
            if(isSystemFont)  SetFontColor((char*)fntSelect,(char*)bordSelect,isOutline);              
            if(isGradient)
            {
              DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2,0,0,0,clrBlack,clrBlack);//cursor        
              if(listm->cursor!=2)
              {
                for(ii=(font_y*oldY);ii>0;ii-=4)
                   DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2-ii,0,0,0,CursorBGClr,CursorClr);

                int ScrH  =  rc.y + 2;
                int HSIZE = (rc.y2 - rc.y + 2 + 6)*ScreenW();
                listm->ScreenCursor = DoScreen(ScrH, HSIZE);                
                listm->cursor++;
                SMART_REDRAW();
               return;
              }
              else
                DrwImage(&listm->ScreenCursor,rc.x,rc.y);
            }
            else DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2,0,0,0,CursorBGClr,CursorClr);//cursor        

            DrawHeader(bmk,s,x);
            DrawScrollBar(i,listm->totalItems);
            
            if(LINE_MODE)
               PrintText(rc.x+x,rc.y+font_y,bmk->descript,0,listm->font,space,0,0,0,rc.x2);//description under item name
            
          }
          else
            if(isSystemFont)  SetFontColor((char*)clrBlack,(char*)transparent,isOutline); 
          PrintText(rc.x+x,rc.y+1,bmk->name,0,listm->font,space,0,0,0,rc.x2-5);//item name
          
          if(bmk->icon_size)  DrwImage(bmk->icon,rc.x+3,rc.y);
          else  DrwImage(listm->noicon,rc.x+3,rc.y);          
         }
       }
      i++;
      if(i>(listm->curpage*listm->itemFromDisp)) break;
  }  
  mfree(s);
  UnlockSched();  
}



// Управление курсором
void MoveUp(int i)
{
  if(!listm->itemFromDisp)return;
  if(!bmktop)return;
  if(listm->curpos<1)
  {
    listm->curpos = listm->totalItems-1;
    
    if(listm->itemFromDisp != listm->totalItems)
    {
      listm->curpage = sdiv(listm->itemFromDisp,listm->totalItems)+1;
    }
    else
      listm->curpage = 1;
  }
  else
  {
    if(i) listm->curpos-=2;
    else listm->curpos--;
    if( listm->curpos < (listm->curpage-1)*listm->itemFromDisp){ listm->curpos++; listm->curpage--; }
  }
 // REDRAW();
}


void MoveDown(int i)
{
  if(!listm->totalItems)return;
  if(!bmktop)return;
  if(i) listm->curpos+=2;  
  else listm->curpos++;
  if( listm->curpos >= listm->totalItems) { listm->curpos = 0; listm->curpage = 1; }
  if( listm->curpage*listm->itemFromDisp < listm->curpos){ listm->curpos--; listm->curpage++; } 
}

static void OnRedraw(MAIN_GUI *data)
{
  ClearScreen();
  LockSched();
  DrawRoundedFrame(0,YDISP,scr_w,scr_h,0,0,0,clrWhite,clrWhite);
  
  DrwImage(listm->bgpic,0,CLIST_Y1);
  DrawLine(0,CLIST_Y1,scr_w,CLIST_Y1,0,clrWhite);
  
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  UnlockSched();

  DrawList(data->ws1);
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  active=1;
  data->ws1=AllocWS(256);
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  active=0;
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

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case UP_BUTTON:   if(VIEW_MODE) MoveUp(1);   else MoveUp(0);   break;
      case DOWN_BUTTON: if(VIEW_MODE) MoveDown(1); else MoveDown(0); break;
      case LEFT_BUTTON: if(VIEW_MODE) MoveUp(0);   else MoveUp(1);   break;
      case RIGHT_BUTTON:if(VIEW_MODE) MoveDown(0); else MoveDown(1); break;
      case '*' : PrintAbout();        break;    
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
  
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
//      case UP_BUTTON:   MoveUp(1);   break;
  //    case DOWN_BUTTON: MoveDown(1); break;
  //    case LEFT_BUTTON:   MoveUp(0);   break;
    //  case RIGHT_BUTTON: MoveDown(0); break; 
      case UP_BUTTON:   if(VIEW_MODE) MoveUp(1);   else MoveUp(0);   break;
      case DOWN_BUTTON: if(VIEW_MODE) MoveDown(1); else MoveDown(0); break;
      case LEFT_BUTTON: if(VIEW_MODE) MoveUp(0);   else MoveUp(1);   break;
      case RIGHT_BUTTON:if(VIEW_MODE) MoveDown(0); else MoveDown(1); break;
      case '*' : PrintAbout();      break;         
    }
  }

  
  if (k=='0' && msg->gbsmsg->msg==KEY_DOWN)
  {
/*    curpos=9;
    REDRAW();
    if (RunBM2(9)) return 1;*/
    if(listm->curpos+9 < listm->totalItems) listm->curpos+=9;
    else { listm->curpos=0; listm->curpage=1; }
    SMART_REDRAW();
  }

  if (((k>='1')&&(k<='9')) && (msg->gbsmsg->msg==KEY_DOWN))
  { 
/*    if(curpos >  0 && curpos < 10) curpos=k-'1';
    if(curpos > 10 && curpos < 20) curpos=(k-'1')+ 10;
    if(curpos > 20 && curpos < 28) curpos=(k-'1')+ 20;*/
    listm->curpos = k-'1';
    REDRAW();
    if (RunBM2(listm->curpos)) return 1;
  }
  
  if ((msg->gbsmsg->msg==KEY_DOWN) && (k==ENTER_BUTTON || k==LEFT_SOFT))
  {
    if (RunBM2(listm->curpos)) return(1);
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


static void mainguicsm_oncreate(CSM_RAM *data)
{
/*    MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(void *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    menus_id=CreateGUI(main_gui); 
    */
  const RECT Canvas={0,0,0,0};
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  menus_id=csm->gui_id=CreateGUI(main_gui);


}

static void mainguicsm_onclose(CSM_RAM *csm)
{
  MAINGUI_ID = 0;
}


static int mainguicsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }  
  return(1);
}

unsigned short mainguicsm_name_body[140];

static const struct
{
  CSM_DESC mainguicsm;
  WSHDR mainguicsm_name;
}MAINGUICSM =
{
  {
  mainguicsm_onmessage,
  mainguicsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  mainguicsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    mainguicsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};


const RECT Canvas={0,0,0,0};
void ShowMenu()
{  
  if(MAINGUI_ID)
  {
    gipc.name_to=ipc_xtask_name;
    gipc.name_from=ipc_my_name;
    gipc.data=(void *)MAINGUI_ID;
    GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_SHOW_CSM,&gipc);
  }
  else
  {
    char dummy[sizeof(MAIN_CSM)];
    wsprintf((WSHDR *)&MAINGUICSM.mainguicsm_name,"AltMyMenu2");
    CSM_RAM *save_cmpc;
    LockSched();
    save_cmpc=CSM_root()->csm_q->current_msg_processing_csm;
    CSM_root()->csm_q->current_msg_processing_csm=NULL;
    MAINGUI_ID=CreateCSM(&MAINGUICSM.mainguicsm, dummy, 0);
    CSM_root()->csm_q->current_msg_processing_csm=save_cmpc;
    UnlockSched();    
  }
}

int mode=0;
int mode_enter;

int my_keyhook(int submsg, int msg)
{
  if(submsg==ACTIVE_KEY_1 && ACTIVE_KEY_STYLE!=2 && !active)
    {      
      switch(msg)
      {
        case KEY_DOWN:
          if (!ACTIVE_KEY_STYLE)
          {
            if(IDLE_MODE)//Если запуск только на идле
            {
              if(ENA_LOCK)//если активировать при залоченной клаве
              {
                 if(IsIdle())//если идле то в бой
                 {
                  ShowMenu();
                 }
                 else//иначе пропускаем вызов и посылаем кей ап процесс
                 {
                  GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
              else//если вызывать только при разлоченной клаве
              {
                if(IsIdle() && IsUnlocked())
                 {
                  ShowMenu();
                 }
                else
                 {
                  GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
             }            
            else //вызов отовсюду
              if(ENA_LOCK) ShowMenu();
              else if(IsUnlocked())
               {
                ShowMenu();
               }
            return(2);
          }
          else
          {
            if (mode_enter==2)
            {
              GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case KEY_UP:
          if (ACTIVE_KEY_STYLE)
          {
            if (mode_enter==0)
            {
              mode_enter=2;
              GBS_SendMessage(MMI_CEPID,KEY_DOWN,ACTIVE_KEY_1);
              return (2);
            }
            if (mode_enter==2)
            {
              mode_enter=0;
              return (0);
            }
            mode_enter=0;
            return (2);
          }          
        case LONG_PRESS:
          if (ACTIVE_KEY_STYLE)
          {
            mode_enter=1;
            if(IDLE_MODE)
            {
              if(ENA_LOCK)
              {
                 if(IsIdle())
                 {
                  ShowMenu();
                 }
                 else
                 {
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
              else
              {
                if(IsIdle() && IsUnlocked())
                 {
                  ShowMenu();
                 }
              else
                 {
                   GBS_SendMessage(MMI_CEPID,KEY_UP,ACTIVE_KEY_1);break;
                 }
              }
             }
            else
            {
              if(ENA_LOCK) ShowMenu();
              else if(IsUnlocked())
              {
                ShowMenu();
              }
            }
          }
         break;
      }
    }
  else
  {
  // * + # implementation
  if ((ACTIVE_KEY_STYLE==2) && (!active))
  {
    if (msg==KEY_UP)
    {
      mode=0;
      return KEYHOOK_NEXT;
    }
    if (msg==KEY_DOWN)
    {
      switch (submsg)
      {
      case '*':
        mode=1;
        return (0);
      case '#':
        if (mode==1)
        {
          if(ENA_LOCK) ShowMenu();
          else if(IsUnlocked())
          {
           ShowMenu();
          }
          else mode=0;
         }
        else { active=0; return KEYHOOK_NEXT; }
      }
     }
    }
  }  
return 0;
}



//---------------------------------------------------------------------------//



void CheckDoubleRun(void)
{
  int csm_id;
  if ((csm_id=(int)(gipc.data))!=-1)
  {
   // gipc.name_to=ipc_xtask_name;
   // gipc.name_from=ipc_my_name;
   // gipc.data=(void *)MAINGUI_ID;
   // GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_SHOW_CSM,&gipc); 
    LockSched();
    CloseCSM(my_csm_id);
    ShowMSG(1,(int)"AltMyMenu уже запущен!");
    
    UnlockSched();
  }
  else
  {
    #ifdef ELKA
      SUBPROC((void *)InitBmk);
     #else
       InitBmk();
    #endif      
   }   
}

static void maincsm_oncreate(CSM_RAM *data)
{
  scr_w=ScreenW()-1;
  scr_h=ScreenH()-1;

  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
}


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
  FreeBmkList();  
  FreeMenu();
  
  if(MAINGUI_ID) CloseCSM(MAINGUI_ID);

  
  RemoveKeybMsgHook((void *)my_keyhook);
  SUBPROC((void *)ElfKiller);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"AltMyMenu2 config updated!");
      InitConfig();
      InitMenu();
      InitBmk();
    }
  }
  
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
         case IPC_CHECK_DOUBLERUN:
	    //Если приняли свое собственное сообщение, значит запускаем чекер
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)CheckDoubleRun);
            else ipc->data=(void *)my_csm_id;
	    break; 
         case IPC_OPEN_GUI:
	    //Если приняли свое собственное сообщение, значит запускаем чекер
	    if (ipc->name_from==ipc_my_name) ShowMenu();
	    break; 
        }
      }
    }
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"AltMyMenuD2");
}


int main(char *exename, char *fname)
{ 
 
  CSMROOT *csmr;
  CSM_RAM *save_cmpc;
  CSM_RAM main_csm;
  InitConfig();
  UpdateCSMname();
  LockSched();

  AddKeybMsgHook((void *)my_keyhook);
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  my_csm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();

  strcpy(img_dir,path);
  
  if(InitMenu()) return 0;
  
  return 0;
}
