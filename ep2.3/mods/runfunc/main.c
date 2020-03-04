#include "../inc/swilib.h"

void run(void *func) 
{
  void (*runaddr)(void);
  runaddr=(void (*)())func;
  if (runaddr) 
  {
    runaddr();
  }
}

void ff()
{
  ShowMSG(1,(int)"test");
}



typedef int (*func)();

typedef struct
{
  char name[32];
  func run;
  IMGHDR *img;//картинка меню
  int h; //высота картинки
  int checkbox;
  int flag;
  void *next;
}MAINMENU;

static MAINMENU *mmtop;

void AddMenuItem(char *name,func myfunc,IMGHDR *img,int checkbox,int flag)
{
  MAINMENU *m = malloc(sizeof(MAINMENU));
  //strncpy(m->name,name, 32);
//  m->img = img;
//  m->h = img->h;  
  m->run = myfunc;
//  m->checkbox = checkbox;
//  m->flag = flag;
  
  if(!mmtop)
  {
    mmtop = m;   
  }
  else
  {
    MAINMENU *mm = mmtop;
    while(mm->next) mm = mm->next;    
    mm->next = m;
  }
}

static MAINMENU *GetItem(int curitem)
{
   MAINMENU *mm = mmtop;
   int i=0;
   
   while(mm/*->next && ((i++) <= curitem)*/)
   {
     if(i==curitem) return mm;
     i++;
     if(mm->next) mm = mm->next;    
     else return 0;
   }
   return 0;// ((i==j) ? (mm) : (0));
}

void FreeList()
{  
  LockSched();
  MAINMENU *bmk=(MAINMENU *)mmtop;
  mmtop=0;
  UnlockSched();
  while(bmk)
  {
    MAINMENU *bmk_prev = bmk;
    bmk = bmk->next;
    if(bmk_prev) mfree(bmk_prev);
  }
}


int CreateSCMenu()
{
  ShowMSG(1,(int)"123");
}

int CreateSCMenu2()
{
  ShowMSG(1,(int)"67890");
}

static void Init()
{  
 // int i=0;
  AddMenuItem("menutexts[i++]", CreateSCMenu,       0, 0,0);
  AddMenuItem("menutexts2",     CreateSCMenu2,      0, 0,0);
//  AddMenuItem(menutexts[i++], CreateXSMenu,       GetXIconByItem(CurrentXStatus), 0,0);
/*  AddMenuItem(menutexts[i++], (func)CreateManageCLMenu, GetIconByItem(ICON_ADDCONTACT), 0,0);
  
  //checkbox
  AddMenuItem(menutexts[i++], AutoAwayMSG,           0, 1, away_m);
  AddMenuItem(menutexts[i++], ChangeVibraMode,       0, 1, Is_Vibra_Enabled);
  AddMenuItem(menutexts[i++], ChangeSoundMode,       0, 1, Is_Sounds_Enabled);
#ifdef ELKA
  AddMenuItem(menutexts[i++], ChangeSLIMode,         0, 1, Is_SLI_On);
#endif
  AddMenuItem(menutexts[i++], ChangeDoubleVibra,     0, 1, Is_Double_Vibra_On);
  AddMenuItem(menutexts[i++], ChangePOPUPMode,       0, 1, Is_POPUP_On);
  AddMenuItem(menutexts[i++], ChangeShowOfflineMode, 0, 1, Is_Show_Offline);
  AddMenuItem(menutexts[i++], ChangeShowGroupsMode,  0, 1, Is_Show_Groups);
  AddMenuItem(menutexts[i++], ChangeActiveUp,        0, 1, IsActiveUp);
  //
  
  AddMenuItem(menutexts[i++], EditConfig,   GetIconByItem(ICON_SETTINGS), 0, 0);
  AddMenuItem(menutexts[i++], Disconnect,   GetIconByItem(IS_OFFLINE),    0, 0);
  AddMenuItem(menutexts[i++], PingToServer, GetIconByItem(ICON_PING),     0, 0);
  AddMenuItem(menutexts[i++], AboutDlg,     GetIconByItem(IS_UNKNOWN),    0, 0);
  */

}



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
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

const int minus11=-11;
unsigned short maincsm_name_body[140];

int my_csm_id=0;


void MM_OnRedraw(MAIN_GUI *data)
{
 // LockSched();
//  if(MMScreen.bitmap) DrwImage(&MMScreen,0,HeaderY+2,RET_Y);
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x64","\x00\x00\x00\x64");
 // UnlockSched();
  //DrawSoftKeys(LG_SELECT,LG_CLOSE);
//  MM_DrawList();
  //if(isAbout) DrawAbout();
}


//------------------------------------------


void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  Init();
#ifdef ELKA
    DisableIconBar(1);
#endif
  data->gui.state=1;
}

void MM_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  FreeList();
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
    case '1' :
      {
        MAINMENU *m = GetItem(0);
        if(m) (m->run)();
        else ShowMSG(1,(int)"Not found");
      }break;
    case '2' :
      {
        MAINMENU *m = GetItem(1);
        if(m) (m->run)();
        else ShowMSG(1,(int)"Not found");
      }break;
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
static void maincsm_oncreate(CSM_RAM *data)
{

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


//---------------------------------------------------------------------------//


#pragma inline=forced
int toupper(int c)
{
  if((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
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
 // FreeLog();
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"FishGUI");
}


int main(char *filename)
{
 
  MAIN_CSM main_csm;
//  InitConfig();
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
  
//  Init();
//  run((void*)ff);
  return 0;
}
