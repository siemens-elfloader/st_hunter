/*
  Интерфейс и функции смены своего статуса
*/
#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "main.h"
#include "status_change.h"
#include "strings.h"
#include "language.h"
#include "print.h"
#include "externs.h"
//===============================================================================================
// ELKA Compatibility
#pragma inline
static void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC *)head)->rc.x=0;
  ((HEADER_DESC *)head)->rc.y=YDISP;
  ((HEADER_DESC *)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC *)head)->rc.y2=HeaderH()-1+YDISP;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC *)inp)->rc.x = 0;
  ((INPUTDIA_DESC *)inp)->rc.y = HeaderH() + 1 + YDISP;
  ((INPUTDIA_DESC *)inp)->rc.x2=ScreenW() - 1;
  ((INPUTDIA_DESC *)inp)->rc.y2=ScreenH() - SoftkeyH() - 1;
}
//===============================================================================================                        
extern char TEMPLATES_PATH[];
static const char XS_FILE_NAME[]="\\XStatuses.txt";

extern void DrawHeader();

char *XStatusText;


   
#pragma inline
static void Change_Status(int status)
{
  CurrentStatus=status;
  set_my_status();

  if(isAutoStatusOn!=12 && isAutoStatusOn==status)
  {
    msg_away_timer=(AUTO_ENGADE);
    away_timer();
  }
  else if(status==IS_ONLINE || status==IS_FFC || status== IS_HOME)
  {
    msg_away_timer=0;
  }
  
  SUBPROC((void*)WriteDefSettings);
}

#pragma inline
static void Change_XStatus(int xstatus)
{
  CurrentXStatus=xstatus;
  set_my_xstatus();
  SUBPROC((void*)WriteDefSettings);
}

///////////////////////////////////////////////////////////////////////////////
#pragma inline
static unsigned int GetStatusIndexInMenu(unsigned int status)
{
  switch(status)
  {
  case IS_ONLINE: {return 0;}
  case IS_AWAY: {return 1;}
  case IS_NA: {return 2;}
  case IS_DND: {return 3;}
  case IS_OCCUPIED: {return 4;}
  case IS_FFC: {return 5;}
  case IS_INVISIBLE: {return 6;}
  case IS_DEPRESSION: return 7;
  case IS_EVIL: return 8;
  case IS_HOME: return 9;
  case IS_LUNCH: return 10;
  case IS_WORK: return 11;
  }
  return 0;
}

#pragma inline 
static unsigned int GetStatusInMenuByPos(int pos)
{
  switch(pos)
  {
  case 0:
    return IS_ONLINE;
  case 1:
    return IS_AWAY;
  case 2:
    return IS_NA;
  case 3:
    return IS_DND;
  case 4:
    return IS_OCCUPIED;
  case 5:
    return IS_FFC;
  case 6:
    return IS_INVISIBLE;
  case 7:
    return IS_DEPRESSION;
  case 8:
    return IS_EVIL;
  case 9:
    return IS_HOME;
  case 10:
    return IS_LUNCH;
  case 11:
    return IS_WORK;
  }
  return IS_UNKNOWN;
}

    
///////////////////////////////////////////////////////////////////////////////

#define STATUSES_NUM 12

char *menutexts2[STATUSES_NUM]=
{
  LG_STONLINE,
  LG_STAWAY,
  LG_STNA,
  LG_STDND,
  LG_STOCCUP,
  LG_STFFC,
  LG_STINVIS,
  LG_STDEPRESSION,
  LG_STEVIL,
  LG_STHOME,
  LG_STLUNCH,
  LG_STWORK
};


int _itemFromDisp;//кол-во на экране
int _totalItems;//общее количество итемов
int _curpos=0;//позичия курсора

RECT _rc;
int _font;

static void SC_DrawList()
{
  int font_y = FontH(_font)+1;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  
  LockSched();
  
  int i=0;
  int start_y=0;
  int start_x=_rc.x+2;
 
  char *s=malloc(128);
  RECT rc;
  while(i<STATUSES_NUM)
  {
   sprintf(s,"%s",menutexts2[i]);
   
   start_y = (_rc.y + 2 + (i/* - (_curpage-1)*_itemFromDisp*/)*font_y);    //определяем начальное смещение по у      
   StoreXYXYtoRECT(&rc,start_x,start_y,_rc.x2,start_y+font_y);            
   SetFontColor(clrWhite,transparent,0);           
   if(i==_curpos)//Если наш итем то выделяем
     DrawRoundedFrame(rc.x,rc.y-1,rc.x2-5,rc.y2-2,0,0,0,MMCursorClr,MMCursorBGClr);//cursor        
   
   DrwImage(icons[GetStatusInMenuByPos(i)],_rc.x+4,start_y);
   PrintText(rc.x+5+icons[GetStatusInMenuByPos(i)]->w,rc.y-1,s,0,_font,1,0,0,0,rc.x2-5);//item name
 
   if(i<10 && ShowNumbMenu)
   {
     if(i==9)sprintf(s,"%d",0);   
     else sprintf(s,"%d",i+1);   
     PrintField(rc.x2-7,start_y,s,2,_font,0);
   }
   
   i++;
   if(i >= STATUSES_NUM) break;
  }
  mfree(s);
  UnlockSched();  
}

// Управление курсором
void SC_MoveUp(int i)
{ 
  //if(_curpos > _itemFromDisp){ _curpos--; }
//  else
    if(_curpos > 0){ _curpos--; /*if(startItem>0) startItem--; */}
  else
  {
    _curpos=STATUSES_NUM-1;
    //startItem=STATUSES_NUM-_itemFromDisp;
  }
}


void SC_MoveDown(int i)
{
  //if(_curpos <_itemFromDisp - 5) _curpos++;
  //else
    if(_curpos < STATUSES_NUM - 1 ){/*if(startItem < STATUSES_NUM-mm_itemFromDisp) startItem++;*/ _curpos++; }
  else   
  {
    _curpos=0;
    //startItem=0;
  }
}  

IMGHDR SCScreen;

static void SC_OnRedraw(MAIN_GUI *data)
{
//  ClearScreen();
  LockSched();
 // DrawContactList();
  DrwImage(&SCScreen,0,hy+2);
//  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x30","\x00\x00\x00\x30");
  DrawRoundedFrame(_rc.x,_rc.y-2,_rc.x2,_rc.y2,0,0,0,MMBackgrBordClr,MMBackgrClr);//"\xF5\x22\x19\x64"
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  UnlockSched();
  SC_DrawList();
}

static void SC_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  _font=clfont;  

  int HSIZE = (hy+2)*ScreenW();
  int ScrH=ScreenH()-(hy+2);
  SCScreen = DoScreen(ScrH,HSIZE);
  
  _rc=SRect;
  /*
#ifdef ELKA
  _rc.x=30;
  _rc.x2=ScreenW()-53;
#else
  _rc.x=10;
  _rc.x2=ScreenW();
#endif
  _rc.y=YDISP+2;*/
  _rc.y2=_rc.y+(FontH(_font)*(STATUSES_NUM+1));
  _curpos=GetStatusIndexInMenu(CurrentStatus);
//  DrawRoundedFrame(0,0,ScreenW()-1,hy+2,0,0,0,"\x00\x00\x00\x15","\x00\x00\x00\x15");
  SetCanvasFlag(1);
#ifndef ELKA
    REDRAW();
#endif
  data->gui.state=1;
}

static void SC_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

static void SC_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  data->gui.state=2;
  DisableIDLETMR();
}

static void SC_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int SC_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   SC_MoveUp(0);   break;
      case DOWN_BUTTON: SC_MoveDown(0); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: return (1);
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON: case LEFT_SOFT:case RIGHT_BUTTON:
          Change_Status(GetStatusInMenuByPos(_curpos));
          DrawHeader();
          return (1);
    }

   if(k>='0' && k<='9')
    {
      if(k=='0') _curpos=9;
      else _curpos=k-'1';
      Change_Status(GetStatusInMenuByPos(_curpos));
      return 1;
    }
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int SC_method8(void){return(0);}
int SC_method9(void){return(0);}

const void * const gui_methods_SC[11]={
  (void *)SC_OnRedraw,
  (void *)SC_onCreate,
  (void *)SC_onClose,
  (void *)SC_onFocus,
  (void *)SC_onUnfocus,
  (void *)SC_OnKey,
  0,
  (void *)kill_data,
  (void *)SC_method8,
  (void *)SC_method9,
  0
};

const RECT SC_Canvas={0,0,0,0};

void CreateSCMenu()
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&SC_Canvas);
  main_gui->gui.methods=(void *)gui_methods_SC;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();  
  CreateGUI(main_gui);   
}


//--------------------------------
// XStatuses
//--------------------------------
extern DYNPNGICONLIST *XStatusesImgList;
//extern int *XStatusesIconArray;
extern volatile int total_xstatuses;

SOFTKEY_DESC edit_x_sk[]=
{
  {0x0018,0x0000,(int)"Save!"},
  {0x0001,0x0000,(int)""},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB edit_x_skt=
{
  edit_x_sk,0
};

void edit_xstatus_locret(void){}
void ReplaceStr(char **str, int start_prev, int prev_len, char *next, int next_len)
{
  char *d, *buf;
  char *s=*str;
  if (s)
  {
    d=buf=malloc(strlen(s)+next_len-prev_len+1);
    if (start_prev)
    {
      memcpy(buf, s, start_prev);
      buf+=start_prev;
    }
    if (next_len)
    {
      memcpy(buf,next,next_len);
      buf+=next_len;
    }
    strcpy(buf, s+start_prev+prev_len);
    mfree(s);
    *str=d;
  }
}
int edit_xstatus_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws;
  char *s, *d, tmp[132];
  int cur_x_st;
  int len, l;
  int is_any_change=0;
  if (msg->keys==0xFFF)
  {
    cur_x_st=(int)EDIT_GetUserPointer(data);
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);   // ???????????
    ws=ec.pWS;
    
    s=GetXStatusStr(cur_x_st*3,&len);
    d=tmp;

    if (s)
    {
      l=ws->wsbody[0];
      for (int i=0; i<l; i++) 
      {
        *d++=char16to8(ws->wsbody[i+1]);
      }
      *d=0;
      if (len!=l || strncmp(s,tmp,len))
      {
        ReplaceStr(&XStatusText,s-XStatusText,len,tmp,l);
        is_any_change=1;
      }
    }

    ExtractEditControl(data,4,&ec);   // ???????? ??? ??????
    ws=ec.pWS;
    
    s=GetXStatusStr(cur_x_st*3+1,&len);
    d=tmp;

    if (s)
    {
      l=ws->wsbody[0];
      for (int i=0; i<l; i++) 
      {
        *d++=char16to8(ws->wsbody[i+1]);
      }
      *d=0;
      if (len!=l || strncmp(s,tmp,len))
      {
        ReplaceStr(&XStatusText,s-XStatusText,len,tmp,l);
        is_any_change=1;
      }
    }
    
    ExtractEditControl(data,6,&ec);   // ??????? ??? ??????
    ws=ec.pWS;
    
    s=GetXStatusStr(cur_x_st*3+2,&len);
    d=tmp;

    if (s)
    {
      l=ws->wsbody[0];
      for (int i=0; i<l; i++) 
      {
        *d++=char16to8(ws->wsbody[i+1]);
      }
      *d=0;
      if (len!=l || strncmp(s,tmp,len))
      {
        ReplaceStr(&XStatusText,s-XStatusText,len,tmp,l);
        is_any_change=1;
      }
    }
    
    if (is_any_change)
    {
      SaveXStatusText();
    }
    void InitXText();
    InitXText();
    return (1);
  }
  return (0);
}

void edit_xstatus_ghook(GUI *data, int cmd)
{
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Save!"};
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
  if (cmd==7)
  {
    SetSoftKey(data,&sk,SET_SOFT_KEY_N);
  }
}

HEADER_DESC edit_xstatus_hdr={0,0,0,0,NULL,(int)"X-Статус",LGP_NULL};

INPUTDIA_DESC edit_xstatus_desc =
{
  1,
  edit_xstatus_onkey,
  edit_xstatus_ghook,
  (void *)edit_xstatus_locret,
  0,
  &edit_x_skt,
  {0,0,0,0},
  FONT_SMALL,
  100,
  101,
  0,
  //  0x00000001 - ????????? ?? ??????? ????
  //  0x00000002 - ????????? ?? ??????
  //  0x00000004 - ???????? ?????????
  //  0x00000008 - UnderLine
  //  0x00000020 - ?? ?????????? ?????
  //  0x00000200 - bold
  0,
  //  0x00000002 - ReadOnly
  //  0x00000004 - ?? ????????? ??????
  //  0x40000000 - ???????? ??????? ????-??????
  0x40000000
};



static void EditXStatus(int xstatus)
{
  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  char *s;
  int len;
  int c;
   
  eq=AllocEQueue(ma,mfree_adr());    
  WSHDR *ews=AllocWS(128);
  ascii2ws(ews,"Комментарий:");   // Коментарий
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);    //1
  
  CutWSTR(ews,0);
  s=GetXStatusStr(xstatus*3, &len);
  if (s)
  {
    while((c=*s++) && len)
    {
      wsAppendChar(ews,char8to16(c));
      len--;
    }
  }
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,20);
  AddEditControlToEditQend(eq,&ec,ma);   //2
  
  ascii2ws(ews,"Short X:");      // Короткий Икс Статус
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   //3
  
  CutWSTR(ews,0);
  s=GetXStatusStr(xstatus*3+1, &len);
  if (s)
  {
    while((c=*s++) && len)
    {
      wsAppendChar(ews,char8to16(c));
      len--;
    }
  }
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);   //4
  
  ascii2ws(ews,"Large X:");     // Длинный Икс Статус
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,0x40,ews,ews->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   //5
  
  CutWSTR(ews,0);
  s=GetXStatusStr(xstatus*3+2, &len);
  if (s)
  {
    while((c=*s++) && len)
    {
      wsAppendChar(ews,char8to16(c));
      len--;
    }
  }
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,0x40,ews,128);
  AddEditControlToEditQend(eq,&ec,ma);   //6
  
  FreeWS(ews);
  patch_header(&edit_xstatus_hdr);
  patch_input(&edit_xstatus_desc);
  CreateInputTextDialog(&edit_xstatus_desc,&edit_xstatus_hdr,eq,1,(void *)xstatus);  
}


void LoadXStatusText(void)
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int fsize;
  char *p, *s;
  int c;
  FreeXStatusText();
  strcpy(fn,TEMPLATES_PATH);
  strcat(fn,XS_FILE_NAME);
  if (GetFileStats(fn,&stat,&ul)==-1) return;
  if ((fsize=stat.size)<=0) return;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return;
  p=XStatusText=malloc(fsize+2);
  s=p+1;
  s[fread(f,s,fsize,&ul)]=0;
  fclose(f,&ul);
  while((c=*s++)!=0)
  {
    if (c==10)
    {
      c=13;
      if (*s==13) s++;   // пропускаем
    }
    else if (c==13)
    {
      if (*s==10) s++;  // пропускаем
    }
    *p++=c;
  }
  *p=0;
}

void SaveXStatusText(void)
{
  int f;
  char fn[256];
  unsigned int ul;
  if (!XStatusText) return;
  strcpy(fn,TEMPLATES_PATH);
  strcat(fn,XS_FILE_NAME);
  if ((f=fopen(fn,A_ReadWrite+A_Create+A_Truncate+A_BIN,P_READ+P_WRITE,&ul))==-1) return;
  fwrite(f,XStatusText,strlen(XStatusText),&ul);
  fclose(f,&ul);
}

void FreeXStatusText(void)
{
  mfree(XStatusText);
  XStatusText=NULL;
}

char *GetXStatusStr(int n, int *len)
{
  int l=0;
  int c;
  char *p=XStatusText;
  char *pp;
  if (len) *len=0;
  if (!p) return p;
  while(n)
  {
    while((c=*p++)>=32);
    if (!c) return NULL;
    n--;
  }
  pp=p;
  while((*pp++)>=32) l++;
  if (len) *len=l;
  return p;
}



int xstartItem=0;
int xs_itemFromDisp;//кол-во на экране
//int xs_totalItems;//общее количество итемов
int xs_curpos=0;//позичия курсора
//int xs_curpage=1;//текущая страница


RECT xs_rc;//MainMenu Rect
int xs_font;

#define CLIST_Y1 (YDISP+2)

void DrawXScrollBar(int cur,int total)
{
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  int y=xs_rc.y2-xs_rc.y;//scr_h-CLIST_Y1-FontH(clfont)-2;
  int pos=y*cur/total;
  int y2=y/total;
 // int i=0;
  
  DrawLine(xs_rc.x2-2,xs_rc.y,xs_rc.x2-2,xs_rc.y2,1,clrWhite);
  DrawRoundedFrame(xs_rc.x2-3,xs_rc.y+pos+1,xs_rc.x2-1,xs_rc.y+pos+y2,0,0,0,clrWhite,clrWhite);
}

extern const char percent_s[];

typedef struct
{
 void *next;
 char *xshort;
 char *xlong;
}XSTTEXT;

volatile XSTTEXT *xtexttop;

DYNPNGICONLIST *GetXSByItem(int curitem)
{
  PNGTOP_DESC *pltop=PNG_TOP();
  pltop->dyn_pltop=XStatusesImgList;
  int i=total_xstatuses-1;
  while(pltop->dyn_pltop && i>=0)
  {
    if(i==curitem)
      return pltop->dyn_pltop;
    i--;
    pltop->dyn_pltop = pltop->dyn_pltop->next;  
  }
  return pltop->dyn_pltop;;
}
/*
    if(s=GetXStatusStr((xstartItem+i)*3+1,&l))
      snprintf(xshort,128,percent_s,s);    
    else  
      snprintf(xshort,128,"Short XS %d",xstartItem+i);
     
    if(s=GetXStatusStr((xstartItem+i)*3+2,&l))
     snprintf(xlong,128,percent_s,s);
    else
     sprintf(xlong,"Large XS %d",(xstartItem+i));    
  
   if(i+xstartItem==0) snprintf(xshort,128,"Без xстатуса");
*/

void FreeXSTList()
{  
  LockSched();
  XSTTEXT *bmk=(XSTTEXT *)xtexttop;
  xtexttop=0;
  UnlockSched();
  while(bmk)
  {
    XSTTEXT *bmk_prev=bmk;
    bmk=bmk->next;
    mfree(bmk_prev);
  }
}

void InitXText()
{
  FreeXSTList();
  char *s;
  int l;
  for(int i=0;i<total_xstatuses;i++)
  {
     XSTTEXT *bmk=malloc(sizeof(XSTTEXT));
     bmk->next=0;
     bmk->xlong=malloc(128);
     bmk->xshort=malloc(128);
     
     if(i!=0)
     {
      if(s=GetXStatusStr(i*3+1,&l))
        snprintf(bmk->xshort,128,percent_s,s);    
      else  
        snprintf(bmk->xshort,128,"Short XS %d",i);

      if(s=GetXStatusStr(i*3+2,&l))
        snprintf(bmk->xlong,128,percent_s,s);
      else
       sprintf(bmk->xlong,"Large XS %d",(i));    
     }
     
     if(!xtexttop)
     {
       snprintf(bmk->xshort,128,"Без xстатуса");
       bmk->xlong="";
       xtexttop=bmk; 
     }
     else
     {
       XSTTEXT *bm=(XSTTEXT *)xtexttop;
       while(bm->next)
           bm=bm->next;
       bm->next=bmk;   
     } 
  }
}

XSTTEXT *GetXSTByItem(int curitem)
{
  XSTTEXT *bmk;
  bmk=(XSTTEXT *)xtexttop;
  int i=0;
  while(bmk)
  {
    if(i==curitem) return bmk;
    i++;
    bmk=bmk->next;  
  }
  return bmk;
}
/*
int TotalBmk()
{
  if(!xtexttop) return 0;
  XSTTEXT *bmk;
  bmk=(BOOKMARKS *)xtexttop;
  int i=1; 
  while(bmk=bmk->next) i++;
  return i;
}
*/
GBSTMR xstmr;
int XSScroll=0;
int XSC_ID;
void XSRedraw()
{
//  DirectRedrawGUI(); 
  DirectRedrawGUI_ID(XSC_ID);
}
extern IMGHDR *XStatusImg[];
static void XS_DrawList()
{
  int font_y = FontH(xs_font)+1;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  xs_itemFromDisp = sdiv(font_y,(xs_rc.y2-xs_rc.y))-1;  
 // int l;    
  
  LockSched();  
  int i=0;
  int start_y=0;
  //char *xshort=malloc(128);
  //char *s;
//  char *xlong=malloc(128);
  
  RECT rc;
  DYNPNGICONLIST *xs;
  XSTTEXT *xst;
  SetFontColor(clrWhite,transparent,0); 
  int x=0;
  while(i<xs_itemFromDisp)
  {
    xst=GetXSTByItem(xstartItem+i);
    if(!xst) break;
  /*  if(s=GetXStatusStr((xstartItem+i)*3+1,&l))
//      snprintf(xshort,128,"%d+%d/%d %s",xstartItem,i,xs_curpos,s);    
      snprintf(xshort,128,percent_s,s);    
    else  
      snprintf(xshort,128,"Short XS %d",xstartItem+i);
     
    if(s=GetXStatusStr((xstartItem+i)*3+2,&l))
     snprintf(xlong,128,percent_s,s);
    else
     sprintf(xlong,"Large XS %d",(xstartItem+i));    
  
   if(i+xstartItem==0) snprintf(xshort,128,"Без xстатуса");
*/
   if(i+xstartItem > xs_curpos) start_y = font_y+3;
   else start_y=0;
   
   start_y += (xs_rc.y + 2 + (i*font_y));    //определяем начальное смещение по у         
   StoreXYXYtoRECT(&rc,xs_rc.x+3,start_y,xs_rc.x2-2,start_y+font_y);  

//   xs=GetXSByItem(i+xstartItem);
   if(XStatusImg[i+xstartItem]) x=XStatusImg[i+xstartItem]->w;
  // if(xs) x=xs->img->w;
   if(i+xstartItem==xs_curpos)//Если наш итем то выделяем
    {
      DrawRoundedFrame(rc.x+1,rc.y-1,rc.x2-5,rc.y2+font_y-1,0,0,0,MMCursorClr,MMCursorBGClr);//cursor        
      if(xs)
        PrintText(rc.x+(x+6),rc.y+font_y,xst->xlong,0,xs_font,1,350,1,0,rc.x2-rc.x-(x+8));//description under item name   

      DrawXScrollBar(xstartItem+i,total_xstatuses);
    }
   
   if(XStatusImg[i+xstartItem])        
   {
     DrwImage(XStatusImg[i+xstartItem],rc.x+4,start_y);
     PrintText(rc.x+x+6,rc.y-1,xst->xshort,0,xs_font,1,350,0,0,rc.x2-rc.x-(x+6));//item name
   }
   else
     PrintText(rc.x+6,rc.y-1,xst->xshort,0,xs_font,1,350,0,0,rc.x2-rc.x-6);//item name
   i++;
   if(xstartItem+i >= total_xstatuses) break;
  }
//  mfree(s);
  UnlockSched();  
//  mfree(xshort);
  //mfree(xlong);
}

// Управление курсором
void XS_MoveUp()
{
  if(xs_curpos > xs_itemFromDisp + 2 )
  {
    xs_curpos--; 
    if(xstartItem > 0 && xs_curpos > 5 && xs_curpos < total_xstatuses-5)
      xstartItem--; 
  }
  else
    if(xs_curpos > 0)
    {
      xs_curpos--;
      if(xstartItem > 0 && xs_curpos > 5)
        xstartItem--; 
      else
        if(xs_curpos < 5)
            xstartItem=0; 
    }
  else
  {
    xs_curpos=total_xstatuses-1;
    xstartItem=total_xstatuses-xs_itemFromDisp;
  }
}


void XS_MoveDown()
{
  if(xs_curpos < xs_itemFromDisp - 5) xs_curpos++;
  else
    if(xs_curpos < total_xstatuses - 1)
    {
      if(xstartItem < total_xstatuses-xs_itemFromDisp)
        xstartItem++;
      xs_curpos++; 
    }
  else   
  {
    xs_curpos=0;
    xstartItem=0;
  }
}


static void XS_OnRedraw(MAIN_GUI *data)
{
  LockSched();
  DrwImage(&SCScreen,0,hy+2);
  DrawRoundedFrame(xs_rc.x,xs_rc.y-2,xs_rc.x2,xs_rc.y2,0,0,0,MMBackgrBordClr,MMBackgrClr);
//  DrawSoftKeys(LG_TEXT,LG_CLOSE);
  XS_DrawList();
//  GBS_StartTimerProc(&xstmr, 216, XSRedraw);  
}

static void XS_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  if(isSystemFont) clfont=100+pngFont;
  else clfont=2;
  xs_font=clfont;  
  DrawSoftKeys(LG_SELECT,LG_CLOSE);//рисуем софты и фоткаем их ;)
  
  int HSIZE = (hy+2)*ScreenW();
  int ScrH=ScreenH()-(hy+2);
  SCScreen = DoScreen(ScrH,HSIZE);
  
  InitXText();
  xs_rc=XSRect;
/*#ifdef ELKA
  xs_rc.x=30;
  xs_rc.x2=ScreenW()-10;
#else
  xs_rc.x=10;
  xs_rc.x2=ScreenW()-5;
#endif
  xs_rc.y=YDISP+2;*/
  
  //xs_rc.y2=ScreenH()-(FontH(clfont)*2)-4;
  
  int i=sdiv((FontH(clfont)+1),(xs_rc.y2-xs_rc.y));// кол-во итемов
  
  xs_rc.y2=xs_rc.y+(FontH(clfont)*i)+2;
  
  xs_curpos=CurrentXStatus;
  if(xs_curpos > 5 && xs_curpos < total_xstatuses-xs_itemFromDisp)
    xstartItem=xs_curpos-5;
  else
    if(xs_curpos > total_xstatuses-xs_itemFromDisp)
//      xstartItem=total_xstatuses-CurrentXStatus;
      xstartItem=total_xstatuses-xs_itemFromDisp;
  else
    if(xs_curpos < 5)
      xstartItem=0;
  
  SetCanvasFlag(1);
  SetScrollEdgeDelay(1);  
#ifndef ELKA
    REDRAW();
#endif
  data->gui.state=1;
}

static void XS_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeXSTList();
  GBS_DelTimer(&xstmr);
  data->gui.state=0;
}

static void XS_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  data->gui.state=2;
  DisableIDLETMR();
}

static void XS_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int XS_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {
      case UP_BUTTON:   XS_MoveUp();   break;
      case DOWN_BUTTON: XS_MoveDown(); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: return (1);
     // case ENTER_BUTTON: case LEFT_SOFT: ((void (*)(void))(menuprocs[mm_curpos]))(); REDRAW(); break;
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON:
          Change_XStatus(xs_curpos);
          DrawHeader();
          return(1);  
      case LEFT_SOFT: case RIGHT_BUTTON:
          EditXStatus(xs_curpos);
          REDRAW();
          break;
    }
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int XS_method8(void){return(0);}
int XS_method9(void){return(0);}

const void * const gui_methods_XS[11]={
  (void *)XS_OnRedraw,
  (void *)XS_onCreate,
  (void *)XS_onClose,
  (void *)XS_onFocus,
  (void *)XS_onUnfocus,
  (void *)XS_OnKey,
  0,
  (void *)kill_data,
  (void *)XS_method8,
  (void *)XS_method9,
  0
};

const RECT XS_Canvas={0,0,0,0};

void CreateXSMenu()
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&XS_Canvas);
  main_gui->gui.methods=(void *)gui_methods_XS;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  XSC_ID=CreateGUI(main_gui);   
}
