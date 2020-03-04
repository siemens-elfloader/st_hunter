#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "history.h"
#include "mainmenu.h"
#include "main.h"
#include "conf_loader.h"
#include "status_change.h"
#include "language.h"
#include "revision.h"
#include "manage_cl.h"
#include "rect_patcher.h"
#include "cl_work.h"
#include "print.h"
#include "externs.h"

//==============================================================================
int MainMenu_ID;

int isAbout=0;

static void ChangeVibraMode(void)
{
  Is_Vibra_Enabled=!(Is_Vibra_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
  
static void ChangeSoundMode(void)
{
  Is_Sounds_Enabled=!(Is_Sounds_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

static void ChangeShowOfflineMode(void)
{
  void RecountMenu(CLIST *req, int needfocus);
  Is_Show_Offline=!(Is_Show_Offline);
  SUBPROC((void*)WriteDefSettings);
  RecountMenu(NULL, 1);
  RefreshGUI();
}

static void ChangeShowGroupsMode(void)
{
  void RecountMenu(CLIST *req, int needfocus);
  Is_Show_Groups=!(Is_Show_Groups);
  SUBPROC((void*)WriteDefSettings);
  ResortCL();
  RecountMenu(NULL, 1);
  RefreshGUI();
}

static void EditConfig(void)
{
  extern const char *successed_config_filename;
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,successed_config_filename,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  GeneralFuncF1(1);
}

static void Disconnect(void)
{
  extern volatile int disautorecconect;
  disautorecconect=1;
  extern void end_socket(void);
  SUBPROC((void*)end_socket);
  GeneralFuncF1(1);
}

static void PingToServer(void)
{
  TPKT *p;
  p=malloc(sizeof(PKT)+sizeof(TTime));
  GetDateTime(NULL,(TTime *)p->data);
  p->pkt.uin=0;
  p->pkt.type=T_ECHO;
  p->pkt.data_len=sizeof(TTime);
  SUBPROC((void *)SendAnswer,0,p);
}

static void AboutDlg()
{
//  char s[256];
  //snprintf(s,255,LG_COPYRIGHT,__SVN_REVISION__);
  //ShowMSG(2, (int)s);
   isAbout=1;
}


void AutoAwayMSG(void)
{
 if(!away_m)
  {
   away_m=!away_m;
   GBS_StopTimer(&away_tmr);
   msg_away_timer=AUTO_ENGADE;
   away_timer();
  }
 else
  {
   msg_away_timer=0;
   away_m=!away_m;
   if(change_back_status)
    {
      CurrentStatus=status_rem;
      set_my_status();
      change_back_status=0;
    }
  }
 RefreshGUI();
}


#ifdef NEWSGOLD
  #ifdef ELKA
      #define MENU_ITEMS 18//ELKA
  #else
      #define MENU_ITEMS 17//NEWSGOLD
  #endif
#else
  #define MENU_ITEMS 17//SGOLD
#endif

static char *menutexts2[MENU_ITEMS]=
{
  LG_MNUSTATUS,
  LG_MNUXSTATUS,
  LG_MANAGELIST,
  LG_AUTOANSWER,//"Автоответчик",
  LG_MNUVIBRA,
  LG_MNUSOUND,
  LG_INCMSG,
  LG_REMINDERON,
  LG_MNUILUON,
  LG_MNUSHOWOFF,
  LG_MNUSHOWGROUP,
  LG_MNUEDCFG,
  LG_MNUDISCONNECT,
  LG_MNUPING,
  LG_MNUABOUT,
  LG_ACTUP,
#ifdef ELKA
  LG_MNUSLION,
#endif
};

#ifdef ELKA
void ChangeSLIMode(void)
{
  Is_SLI_On=!(Is_SLI_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
#endif

void ChangePOPUPMode(void)
{
  Is_POPUP_On=!(Is_POPUP_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

void ChangeDoubleVibra(void)
{
  Is_Double_Vibra_On=!(Is_Double_Vibra_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}


void ChangeILUMode(void)
{
  Is_ILU_On=!(Is_ILU_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}


void ChangeActiveUp(void)
{
  IsActiveUp=!(IsActiveUp);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}


extern void CreateSCMenu();
extern void CreateXSMenu();
static const void *menuprocs[MENU_ITEMS]=
{
  (void *)CreateSCMenu,//(void *)DispStatusChangeMenu,
  (void *)CreateXSMenu,//DispXStatusChangeMenu,
  (void *)CreateManageCLMenu,
  (void *)AutoAwayMSG,
  (void *)ChangeVibraMode,
  (void *)ChangeSoundMode,
  (void *)ChangePOPUPMode,
  (void *)ChangeDoubleVibra,
  (void *)ChangeILUMode,
  (void *)ChangeShowOfflineMode,
  (void *)ChangeShowGroupsMode,
  (void *)EditConfig,
  (void *)Disconnect, 
  (void *)PingToServer,
  (void *)AboutDlg,
  (void *)ChangeActiveUp,
#ifdef ELKA
  (void *)ChangeSLIMode,
#endif
};

static char *temp_chars; //Собственно файл
static char **menutexts; //Массив указателей на строки

void FreeMenu(void)
{
  if (menutexts) mfree(menutexts);
  if (temp_chars) mfree(temp_chars);
  menutexts=NULL;
  temp_chars=NULL;
}

int LoadMenuLang()
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  FreeMenu();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\langmenu.txt");
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;
  p=temp_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	menutexts=realloc(menutexts,(i+1)*sizeof(char *));
	menutexts[i++]=pp;
      }
      pp=NULL;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==NULL) pp=p;
    }
    p++;
  }
  return i;
}



static int startItem=0;
int x2_cb=10;

extern char clrWhite[];
extern char clrRed[];
extern char transparent[];
extern char blue[];
extern int clfont;
extern IMGHDR *icons[];
static int mm_itemFromDisp;//кол-во на экране
//static int mm_totalItems;//общее количество итемов
static int mm_curpos=0;//позичия курсора

RECT mm_rc;//MainMenu Rect
int mm_font;

#define CLIST_Y1 (YDISP+2)

void DrawCheckBox(int x,int y,int flag,int type)
{
  RECT rc;
  StoreXYXYtoRECT(&rc,x+2,y,x+x2_cb+2,y+x2_cb);
  DrawRectangle(rc.x,rc.y,rc.x2,rc.y2,0,clrWhite,transparent);
  if(flag)
  {
    if(type)
    {
      DrawLine(rc.x+2,rc.y+2,rc.x2-2,rc.y2-2,0,clrWhite);
      DrawLine(rc.x2-2,rc.y+2,rc.x+2,rc.y2-2,0,clrWhite);
    }
    else
    {
      DrawLine(rc.x+2,rc.y+5,rc.x2-7,rc.y2-2,0,clrWhite);
      DrawLine(rc.x2-7,rc.y2-2,rc.x+x2_cb,rc.y-2,0,clrWhite);
    }
  }
}

void DrawIcon(int x,int y,int curitem)
{
 switch(curitem)
  {
    case 0: DrwImage(icons[CurrentStatus],x+1,y);           break;
    case 1: DrwImage(XStatusImg[CurrentXStatus],x+1,y);     break;
    case 2: DrwImage(icons[ICON_ADDCONTACT],x+1,y);         break;
    case 3: DrawCheckBox(x,y,away_m?1:0,0);                 break;
    case 4: DrawCheckBox(x,y,Is_Vibra_Enabled?1:0,0);       break;
    case 5: DrawCheckBox(x,y,Is_Sounds_Enabled?1:0,0);      break;
    case 6: DrawCheckBox(x,y,Is_POPUP_On?1:0,0);              break;
    case 7: DrawCheckBox(x,y,Is_Double_Vibra_On?1:0,0);     break;
    case 8: DrawCheckBox(x,y,Is_ILU_On?1:0,0);              break;
    case 9: DrawCheckBox(x,y,Is_Show_Offline?1:0,0);        break;
    case 10: DrawCheckBox(x,y,Is_Show_Groups?1:0,0);        break;    
    case 11: DrwImage(icons[ICON_SETTINGS],x+1,y);          break;
    case 12: DrwImage(icons[IS_OFFLINE],x+1,y);             break;
    case 13: DrwImage(icons[IS_UNKNOWN],x+1,y);             break;
    case 14: DrwImage(icons[ICON_PING],x+1,y);              break;
    case 15: DrwImage(icons[IS_UNKNOWN],x+1,y);             break;    
    case 16: DrawCheckBox(x,y,IsActiveUp?1:0,0);            break;
    case 17: DrwImage(icons[ICON_SETTINGS],x+1,y);          break;
  #ifdef ELKA
    case 18: DrawCheckBox(x,y,Is_SLI_On?1:0,0);             break;
  #endif
  }
}


void DrawMMScrollBar(int cur,int total)
{
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  int y=mm_rc.y2-mm_rc.y-2;
  int pos=y*cur/total;
  int y2=y/total;
  
  DrawLine(mm_rc.x2-2,mm_rc.y,mm_rc.x2-2,mm_rc.y2,1,clrWhite);
  DrawRoundedFrame(mm_rc.x2-3,mm_rc.y+pos+1,mm_rc.x2-1,mm_rc.y+pos+y2,0,0,0,clrWhite,clrWhite);
}

int isLoadLangMenu=0;

void MM_DrawList()
{
  int font_y;// = FontH(mm_font)+1;
  if(icons[IS_ONLINE]->bitmap && icons[IS_ONLINE]->h  >  FontH(mm_font) )
  {
    font_y = icons[IS_ONLINE]->h-3;
    x2_cb  = font_y;
  }
  else
  {
    font_y = FontH(mm_font)+1;
    x2_cb  = icons[IS_ONLINE]->h-3;
  }
    
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;

  mm_itemFromDisp = sdiv(font_y,mm_rc.y2-mm_rc.y-2);
  
  LockSched();
  
  int i=0;
  int start_y=0;
  int start_x=x2_cb+8;
 
  char *s=malloc(32);
  *s=0;
  RECT rc;
  int x2=0;
  while(i<mm_itemFromDisp)
  { 
   start_y = (mm_rc.y + 2 + (i*font_y));    //определяем начальное смещение по у      
   StoreXYXYtoRECT(&rc,start_x,start_y,mm_rc.x2-2,start_y+font_y);            
   SetFontColor(clrWhite,transparent,0);           
   
   if(i<10 && ShowNumbMenu)
   {
     if(i==9) sprintf(s,"%d",0);   
     else sprintf(s,"%d",i+1);   
     PrintField(rc.x2-7,start_y,s,2,mm_font,0);
     x2=GetWidth(rc.x2-7,start_y);
   }
   
   if(startItem+i==mm_curpos)//Если наш итем то выделяем
    {
     DrawRoundedFrame(rc.x,rc.y-1,rc.x2-5,rc.y2-2,0,0,0,MMCursorClr,MMCursorBGClr);//cursor        
     
     if( isLoadLangMenu!=0 ? menutexts[startItem+i] : menutexts2[startItem+i] )
       PrintText(rc.x+3,rc.y-1,(isLoadLangMenu!=0 ? menutexts[startItem+i] : menutexts2[startItem+i]),0,mm_font,1,0,1,0,rc.x2-x2);//item name
     
     DrawMMScrollBar(startItem+i,MENU_ITEMS);
    }
   else
     PrintText(rc.x+3,rc.y-1,(isLoadLangMenu!=0 ? menutexts[startItem+i] : menutexts2[startItem+i]),0,mm_font,1,350,0,0,rc.x2-x2-2);//item name   
   
   DrawIcon(1,start_y+1,startItem+i);      
   i++;
   if(startItem+i >= MENU_ITEMS) break;
  }
  mfree(s);
  UnlockSched();  
}

// Управление курсором
void MM_MoveUp(int i)
{
  if(mm_curpos > mm_itemFromDisp + 2 ){ mm_curpos--; }
  else
    if(mm_curpos > 0){ mm_curpos--; if(startItem>0) startItem--; }
  else
  {
    mm_curpos=MENU_ITEMS-1;
    startItem=MENU_ITEMS-mm_itemFromDisp;
  }
}


void MM_MoveDown(int i)
{
  if(mm_curpos <mm_itemFromDisp - 5) mm_curpos++;
  else
    if(mm_curpos < MENU_ITEMS - 1 ){if(startItem < MENU_ITEMS-mm_itemFromDisp) startItem++; mm_curpos++; }
  else   
  {
    mm_curpos=0;
    startItem=0;
  }
}

extern IMGHDR screen;
extern void DrawSoftKeys(char *right,char *left);
extern void DrawContactList();
extern void DrawHeader();
extern int hy;

void DrawAbout()
{ 
#ifdef ELKA 
  int y = hy+10;//mm_rc.y+10;
  int x = 10;
#else
  int y=2;
  int x=2;
#endif
      
  DrawRoundedFrame(x,y,ScreenW()-x,y+15+(FontH(mm_font)*12),0,0,0,"\xFF\xFF\xFF\x45","\x00\x00\x00\x45");

  char *s=malloc(256);
  
  SetFontColor(clrRed,transparent,0);       
  sprintf(s,"NATICQ v2.0\nRevision %d",__SVN_REVISION__);
  PrintText((ScreenW()-x)/2,y+15,s,1,mm_font,1,1,0,0,ScreenW()-x);//item name
#ifdef ELKA
  int n=GetNLines((ScreenW()-x)/2,y+15)+1;
#else
  int n=GetNLines((ScreenW()-x)/2,y+15);
#endif
  sprintf(s,LG_COPYRIGHT);
  SetFontColor(clrWhite,transparent,0);       
  PrintText((ScreenW()-x)/2,y+25+(FontH(mm_font)*n),s,1,mm_font,1,1,0,0,ScreenW()-x);//item name

  SetFontColor(clrRed,transparent,0);       
  sprintf(s,"NATICQ v3.0\n(m)evilfox,kluchnik\nCompile \n%s at %s",__DATE__,__TIME__);
#ifdef ELKA
  n+=GetNLines((ScreenW()-x)/2,y+25+(FontH(mm_font)*n));
#else
  n+=GetNLines((ScreenW()-x)/2,y+25+(FontH(mm_font)*n))-1;
#endif
  PrintText((ScreenW()-x)/2,y+30+(FontH(mm_font)*(n+1)),s,1,mm_font,1,1,0,0,ScreenW()-x);//item name
  mfree(s);

}
IMGHDR MMScreen;
void MM_OnRedraw(MAIN_GUI *data)
{
  LockSched();
  if(MMScreen.bitmap) DrwImage(&MMScreen,0,hy+2);
 // DrawRoundedFrame(0,hy+2,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x15","\x00\x00\x00\x15");
  DrawRoundedFrame(mm_rc.x,mm_rc.y-2,mm_rc.x2,mm_rc.y2,0,0,0,MMBackgrBordClr,MMBackgrClr);
  UnlockSched();
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  MM_DrawList();
  if(isAbout) DrawAbout();
}

extern int shot_soft;

void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  isLoadLangMenu=LoadMenuLang();
  if(SOFT_MODE==2) shot_soft=0;
  mm_font=clfont;
  
  int HSIZE = (hy+2)*ScreenW();
  int ScrH=ScreenH()-(hy+2);
  MMScreen = DoScreen(ScrH,HSIZE);
  
  mm_rc = MMRect;

  int y2 = mm_rc.y2-mm_rc.y;
   
  int ii=sdiv( (FontH(clfont)+1) ,y2 )+1;   
  mm_rc.y2=mm_rc.y + (FontH(clfont)*ii);   

  isAbout=0;
  SetCanvasFlag(1);
  data->gui.state=1;
}

void MM_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeMenu();
  if(SOFT_MODE==2)  shot_soft=0;
  data->gui.state=0;
}

void MM_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  #ifndef ICONBAR
    DisableIconBar(1);
  #endif
#endif
  DrawHeader();
  //DrawSoftKeys(LG_SELECT,LG_CLOSE);//рисуем софты и фоткаем их ;)
//  DrawRoundedFrame(0,hy+2,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x15","\x00\x00\x00\x15");
  
  SetCanvasFlag(1);
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
      case UP_BUTTON:   MM_MoveUp(0);   break;
      case DOWN_BUTTON: MM_MoveDown(0); break;
      case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: if(isAbout) isAbout=0; else return (1);
     // case ENTER_BUTTON: case LEFT_SOFT: ((void (*)(void))(menuprocs[mm_curpos]))(); REDRAW(); break;
    }
  }   

  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(k)
    {  
      case ENTER_BUTTON: case LEFT_SOFT: case RIGHT_BUTTON:
        if(isAbout)
          isAbout=0;
        else
          ((void (*)(void))(menuprocs[mm_curpos]))();
        
        REDRAW();
        break;
    }
    
    if(k>='1' && k<= '9'){  mm_curpos=(k-'1'); ((void (*)(void))(menuprocs[mm_curpos]))(); REDRAW();}
    if(k=='0'){  mm_curpos=9; ((void (*)(void))(menuprocs[mm_curpos]))(); REDRAW();}
  }   

  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int MM_method8(void){return(0);}
int MM_method9(void){return(0);}

static const void * const gui_methods_2[11]={
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

const RECT MM_Canvas={0,0,0,0};

void CreateMainMenu()
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&MM_Canvas);
  main_gui->gui.methods=(void *)gui_methods_2;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  MainMenu_ID=CreateGUI(main_gui);   
}

