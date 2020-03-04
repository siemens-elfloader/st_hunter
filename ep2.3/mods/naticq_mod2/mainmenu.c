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
#include "configs.h"
#include "addon.h"
#include "configs.h"
//==============================================================================
int MainMenu_ID;

extern int CurrentStatus;
extern int CurrentXStatus;
extern DYNPNGICONLIST *XStatusesImgList;
extern  int S_ICONS[];
extern const SOFTKEYSTAB menu_skt;
extern int Is_INC_On;
extern int Is_SLI_On;
extern int Is_ILU_On;
extern int Is_Double_Vibra_On;
int janis;
extern int Is_PING_On;
extern int Is_Vibra_Enabled;
#ifdef NEWSGOLD
extern int Is_Id3tag_Enabled;
#endif
extern unsigned int Is_Sounds_Enabled; 
extern int Is_Show_Offline;
extern int Is_Show_Groups;
extern int IsActiveUp;

int isAbout=0;

void ChangeActiveUp(void)
{
  IsActiveUp=!(IsActiveUp);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

void ChangeVibraMode(void)
{
  Is_Vibra_Enabled=!(Is_Vibra_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
#ifdef NEWSGOLD
void ChangeId3tagMode(void)
{
  Is_Id3tag_Enabled=!(Is_Id3tag_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
  set_my_xstatus();
}
#endif

extern int msg_away_timer;
extern GBSTMR away_tmr;
extern void away_timer();
extern int main_d;
extern int away_m;
extern int change_back_status;
extern int status_rem;
//extern void UpdateCLheader(void);

void AutoAwayMSG(void)
{
 if(!away_m)
  {
   away_m=!away_m;
   GBS_StopTimer(&away_tmr);
   msg_away_timer=AUTO_ENGADE*12;
   away_timer();
   main_d=1;
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
// UpdateCLheader();
 RefreshGUI();
}


void ChangeSoundMode(void)
{
  Is_Sounds_Enabled=!(Is_Sounds_Enabled);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

void ChangeShowOfflineMode(void)
{
  void RecountMenu(CLIST *req, int needfocus);
  Is_Show_Offline=!(Is_Show_Offline);
  SUBPROC((void*)WriteDefSettings);
  RecountMenu(NULL,1);
  RefreshGUI();
}

void ChangeShowGroupsMode(void)
{
  void RecountMenu(CLIST *req,int needfocus);
  Is_Show_Groups=!(Is_Show_Groups);
  SUBPROC((void*)WriteDefSettings);
  ResortCL();
  RecountMenu(NULL,1);
  RefreshGUI();
}

//extern int get_file_size(char* fname);
extern char suc_config_name[64];
//extern char suc_config_name_mod[64];
extern const char *suc_config_name_mod;
void EditConfig(void)
{
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,  suc_config_name,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  GeneralFuncF1(1);
}

void EditConfigMod(void)
{
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,suc_config_name_mod,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  GeneralFuncF1(1);
}


void Disconnect(void)
{
  extern volatile int disautorecconect;
  disautorecconect=1;
  extern void end_socket(void);
  SUBPROC((void*)end_socket);
  GeneralFuncF1(1);
}

extern int uins;
char hmm1[64];

void PingToServer(void)
{
  janis=1;
  TPKT *p;
  p=malloc(sizeof(PKT)+sizeof(TTime));
  GetDateTime(NULL,(TTime *)p->data);
  p->pkt.uin=0;
  p->pkt.type=T_ECHO;
  p->pkt.data_len=sizeof(TTime);
  SUBPROC((void *)SendAnswer,0,p);
}

void AboutDlg()
{
  isAbout=1;
  //char s[256];
  //snprintf(s,255,LG_COPYRIGHT,__SVN_REVISION__);
//  ShowMSG(2, (int)s);
}

void SaveCL()
{
  extern void CreateCLFile(void);
  CreateCLFile();
}

#ifdef NEWSGOLD
  #ifdef ELKA
      #define MENU_ITEMS 22//ELKA
  #else
      #define MENU_ITEMS 21//NEWSGOLD
  #endif
#else
  #define MENU_ITEMS 20//SGOLD
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
//  LG_MNUSMARTCPU,
  LG_REMINDERON,
  LG_MNUILUON,
  LG_MNUPINGON,
//  LG_MNUUPTIMEON,
  LG_MNUSHOWOFF,
  LG_MNUSHOWGROUP,
  LG_MNUEDCFG,
  LG_MNUEDCFG_MOD,
  LG_MNUDISCONNECT,
//  LG_MSGINFO,
  LG_SAVECL,
  LG_MNUPING,
  LG_MNUABOUT,
  LG_ACTUP,
  "Открыть лог",
#ifdef NEWSGOLD
  LG_ID3TAG,
#endif
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
#endif/*
void ChangeSMARTCPUMode(void)
{
  Is_SMARTCPU_On=!(Is_SMARTCPU_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}*/
//#ifdef NEWSGOLD
void ChangeINCMode(void)
{
  Is_INC_On=!(Is_INC_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
//#endif
 ChangeILUMode(void)
{
  Is_ILU_On=!(Is_ILU_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

void ChangePINGMode(void)
{
  Is_PING_On=!(Is_PING_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}
/*
void ChangeUPTIMEMode(void)
{
  Is_UPTIME_On=!(Is_UPTIME_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

int main_count;
int main2_count;

void MSG_Info(void)
{
  char *info=malloc(64);
  sprintf(info, "Отправлено: %d\n"
                "Принято: %d", main2_count, main_count);
  ShowMSG(1,(int)info);
  mfree(info);
}*/

void ChangeDoubleVibra(void)
{
  Is_Double_Vibra_On=!(Is_Double_Vibra_On);
  SUBPROC((void*)WriteDefSettings);
  RefreshGUI();
}

void CreateLogs(void)
{
  extern void CreateLog();
  CreateLog();
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
  (void *)ChangeINCMode,
//  (void *)ChangeSMARTCPUMode,
  (void *)ChangeDoubleVibra,
  (void *)ChangeILUMode,
  (void *)ChangePINGMode,
//  (void *)ChangeUPTIMEMode,
  (void *)ChangeShowOfflineMode,
  (void *)ChangeShowGroupsMode,
  (void *)EditConfig,
  (void *)EditConfigMod,
  (void *)Disconnect, 
//  (void *)MSG_Info,
  (void*)SaveCL,
  (void *)PingToServer,
  (void *)AboutDlg,
  (void *)ChangeActiveUp,
  (void *)CreateLogs,
#ifdef NEWSGOLD
  (void *)ChangeId3tagMode,
#endif
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


int startItem=0;
int x2_cb=10;

extern char clrBlack[];
extern char clrWhite[];
extern char clrRed[];
extern char transparent[];
extern char blue[];
extern int clfont;
extern IMGHDR *icons[];
//extern int clfont;
int mm_itemFromDisp;//кол-во на экране
int mm_totalItems;//общее количество итемов
int mm_curpos=0;//позичия курсора

//extern void DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush);
extern DYNPNGICONLIST *GetXSByItem(int curitem);
RECT mm_rc;//MainMenu Rect
int mm_font;

#define CLIST_Y1 (YDISP+2)

#pragma inline 
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
    case 1:
    {
     // DYNPNGICONLIST *xs=GetXSByItem(CurrentXStatus);
     // DrwImage(xs->img,x,y);
       DrwImage(XStatusImg[CurrentXStatus],x+1,y);
    }
    break;
    case 2: DrwImage(icons[ICON_ADDCONTACT],x+1,y);         break;//DrawImg(x,y,S_ICONS[ICON_ADDCONTACT]);
    case 3: DrawCheckBox(x,y,away_m?1:0,0);                   break;
    case 4: DrawCheckBox(x,y,Is_Vibra_Enabled?1:0,0);         break;
    case 5: DrawCheckBox(x,y,Is_Sounds_Enabled?1:0,0);        break;
    case 6: DrawCheckBox(x,y,Is_INC_On?1:0,0);                break;
    case 7: DrawCheckBox(x,y,Is_Double_Vibra_On?1:0,0);       break;
    case 8: DrawCheckBox(x,y,Is_ILU_On?1:0,0);                break;
    case 9: DrawCheckBox(x,y,Is_PING_On?1:0,0);               break;
    case 10: DrawCheckBox(x,y,Is_Show_Offline?1:0,0);         break;
    case 11: DrawCheckBox(x,y,Is_Show_Groups?1:0,0);          break;    
    case 12: DrwImage(icons[ICON_SETTINGS],x+1,y);          break;
    case 13: DrwImage(icons[ICON_SETTINGS],x+1,y);          break;
    case 14: DrwImage(icons[IS_OFFLINE],x+1,y);             break;
    case 15: DrwImage(icons[IS_UNKNOWN],x+1,y);             break;
    case 16: DrwImage(icons[ICON_PING],x+1,y);              break;
    case 17: DrwImage(icons[IS_UNKNOWN],x+1,y);             break;    
    case 18: DrawCheckBox(x,y,IsActiveUp?1:0,0);              break;
    case 19: DrwImage(icons[ICON_SETTINGS],x+1,y);          break;
#ifdef NEWSGOLD
    case 20: DrawCheckBox(x,y,Is_Id3tag_Enabled?1:0,0);       break;
  #ifdef ELKA
    case 21: DrawCheckBox(x,y,Is_SLI_On?1:0,0);               break;
  #endif
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
//  mm_itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(mm_font)-2)-1;
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
//   sprintf(s,"%s",menutexts[startItem+i]);
        
   start_y = (mm_rc.y + 2 + (i/* - (mm_curpage-1)*mm_itemFromDisp*/)*font_y);    //определяем начальное смещение по у      
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
     else
       PrintText(rc.x+3,rc.y-1,"Ошибка!",0,mm_font,1,0,0,0,rc.x2-x2);//item name
     
     DrawMMScrollBar(startItem+i,MENU_ITEMS);
    } 
   else
   {
     if(menutexts[startItem+i])
       PrintText(rc.x+3,rc.y-1,(isLoadLangMenu!=0 ? menutexts[startItem+i] : menutexts2[startItem+i]),0,mm_font,1,350,0,0,rc.x2-x2-2);//item name
     else
       PrintText(rc.x+3,rc.y-1,"Ошибка!",0,mm_font,1,350,0,0,rc.x2-x2-2);//item name
   }
   
   DrawIcon(1,start_y+1,startItem+i);   

   /*
   if(menutexts[startItem+i])
     PrintField(rc.x+3,rc.y-1,menutexts[startItem+i],0,mm_font,1);//item name
   else
     PrintField(rc.x+3,rc.y-1,"Ошибка!",0,mm_font,1);//item name
   */
   
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
  sprintf(s,"NATICQ v2.0\nRevisiov 2058");
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

void MM_OnRedraw(MAIN_GUI *data)
{
  LockSched();
  if(screen.bitmap) DrwImage(&screen,0,hy+2);
 // DrawRoundedFrame(0,hy+2,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x15","\x00\x00\x00\x15");
  DrawRoundedFrame(mm_rc.x,mm_rc.y-2,mm_rc.x2,mm_rc.y2,0,0,0,MMBackgrBordClr,MMBackgrClr);
  UnlockSched();
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  MM_DrawList();
  if(isAbout) DrawAbout();
}
/*

static void do_logwrite(char *text)
{
  int hFile;
  unsigned int io_error = 0;
  hFile = fopen("4:\\nat.txt",A_ReadWrite + A_Create + A_Append,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
  {
    fwrite(hFile, text, strlen(text), &io_error);
    fwrite(hFile, "\r\n", 2, &io_error);
    fclose(hFile, &io_error);
  }
  else
    ShowMSG(1,(int)"Error!");
//  mfree(text);
}
*/
int one_use=0;
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
  DoScreen();
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
  mfree(screen.bitmap);
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
