#include "../../inc/swilib.h"
#include "conf_loader.h"
#include "../rect_patcher.h"
#include "../strings.h"
#include "../lang.h"
#include "DTop.h"
#include "bitmap_funcs.h"
//#include "inf.c"
#include "screensaver.c"
#include "indicators.c"
#include "files.c"
#include "Loads.c"

#include "menu.c"

CSM_DESC *oldcsmd, *icsmd;
GUI *old_idlegui, *main_gui;
int idle_gui_id;
void *old_methods;
/*CSM_RAM  *olddata,  oldicsm;*/
//void (*old_icsm_onClose)(CSM_RAM*);
//void (*old_icsm_onCreate)(CSM_RAM*);
int (*old_icsm_onMessage)(CSM_RAM*, GBS_MSG *msg);

typedef struct{
	void *prev, *next;
	GUI *gui;
}CSM_GUI;

typedef struct
{
  CSM_RAM csm;
}MAIN_CSM; // структура csm

		#define idlegui_id (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

//===============================================================================================


#include "draw_scr.c"

 //Redraw
// static void OnRedraw(GUI *data)
// {
//   //if (data->state==2)
//   {
// 	m_redr();
//   }
// }


//==========Ояебу=======
char *getobj(char *dir)//Вырвать то,что находиться после последнего слешера
{
   int i=strlen(dir);
   if(i>0&&dir[0])
   while(dir[i]!='\\')
   {
      i--;
   }
   return dir+i;
}

char *getfolder(char *file)// Получить папку в которой находиться файл..
{
  int i=strlen(file);
  char *folder=0;
    while(i>0&&file[i]!='\\')
      i--; 
  if(i)
  {
    folder=malloc(i+2);
    memcpy(folder,file,i+1);
    folder[i+1]='\0';
  }
  return folder;
}

//======================

extern void * gui_methods[];

 //Redraw
static void OnRedraw0(GUI *data)
{
  if (data->state==2)
  {
	gui_methods[0]=(void *)OnRedraw;
	if(idle_ch==3){ idle_ch=1; ShowMSG(1, (int)lg.instead_idle );}
	else REDRAW();
  }
}

DtStates myStates;

void init_funcs()
{
// 	extern DtStates States;
	Dt.states=&myStates;
	Dt.states->st_MAIN=&st_MAIN;
	Dt.states->st_MENU=&st_MENU;
	Dt.states->st_MOVE=&st_MOVE;
	Dt.states->st_RESZ=&st_RESZ;
	Dt.states->st_CHTR=&st_CHTR;
	Dt.states->st_WAIT=&st_WAIT;
	
// 	Dt.states->st_PGUI=&st_PGUI;
// 	Dt.states->st_PGM=&st_PGM;
// 	Dt.states->st_PGR=&st_PGR;
	
	Dt.states->st_LPLG=&st_LPLG;
	Dt.states->st_ERR=&st_ERR;
	
	Dt.mf=&m_funcs;
	Dt.ef=&EXT_funcs;
// 	Dt.plgf=&PLG_funcs;
	
	wsprintf(ews,"%taddons\\pgui.elf", Dt.rootadr);
	ExecuteFile(ews,NULL,&Dt);
}

void Create()
{
	ws=AllocWS(256);
	ews=AllocWS(256);
	init_funcs();
	scr_w=ScreenW()-1;
	scr_h=ScreenH()-1;
	State=DT_WAIT;
	cur.x=scr_w/2;
	cur.y=scr_h/2;
	dofn();
	f_h=GetFontYSIZE(FONT_SMALL); // высота мелкого шрифта

	Dt.inf=0;
	Dt.first=Dt.last=Dt.act=&dt_first;
	if(State!=DT_ERR) SUBPROC((void*)initic);
}

 //Create
static void OnCreate(GUI *data,void *(*malloc_adr)(int))
{
	data->state=1;
	//REDRAW();
}

static void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void ClearIcons()
{
	DT_ICON *TTI;
	DT_ICON *TI=Dt.first;
	if(mfn)
	{
		if(mfn->bitmap) mfree(mfn->bitmap);
		mfree(mfn);
	}
	if(cursor&&(cursor!=&defcur))
	{
		if(cursor->bitmap) mfree(cursor->bitmap);
		mfree(cursor);
	}
	
	mfree(Dt.i.bitmap);
	mfree(Dt.ni.bitmap);
	
	do
	{
		if(TI->nm){ mfree(TI->nm);TI->nm=NULL;}
		if(TI->fl){ mfree(TI->fl);TI->fl=NULL;}
		if(TI->img_fl){ mfree(TI->img_fl);TI->img_fl=NULL;}
		if(TI->inf){ mfree(TI->inf);TI->inf=NULL;}
		if(TI->img)
		{
			if(TI->img->bitmap) mfree(TI->img->bitmap);
			mfree(TI->img);
			TI->img=NULL;
		}
		if(TI->onClose){ TI->onClose();TI->onClose=NULL;}
		if(TI->next) TTI=TI->next;
		else TTI=NULL;
		if(TI->d) mfree(TI);
		if(TTI) TI=TTI;
		else break;
	}while(TTI->next);
}

void Close()
{
  Stop_SSV_Timer();
  Stop_Refresh_Timer();
	FreeWS(ws);
	FreeWS(ews);
	mfree(Dt.rootadr);
	ClearIcons();
	Dt.pf->OnClose();
	SUBPROC((void *)Killer);
}

 //Close
static void OnClose(GUI *data,void (*mfree_adr)(void *))
{
	if(idle_ch)
	{
	  if(exitt)
	  {
		CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
		((CSM_DESC*)icsm->constr)->onMessage=old_icsm_onMessage;
		Close();
	  }
	}
	else
	{
		Close();
		data->state=0;
	}
}

 //Focus
static void OnFocus(GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
  Start_SSV_Timer();
  Start_Refresh_Timer();
//   #ifdef ELKA
//   DisableIconBar(1);
//   #endif
  DisableIDLETMR();
  data->state=2;
}

 //Unfocus
static void OnUnfocus(GUI *data,void (*mfree_adr)(void *))
{
  Stop_SSV_Timer();
  Stop_Refresh_Timer();
//   #ifdef ELKA
//   DisableIconBar(0);
//   #endif
  movcr.x=0;
  movcr.y=0;
  if (data->state!=2) return;
  data->state=1;
}






#include "keys.c"
 //OnKey
static int OnKey(GUI *data,GUI_MSG *msg)
{
  return State->K(msg);//m_key(msg);
}

static void * gui_methods[11]={
  (void *)OnRedraw0,
  (void *)OnCreate,
  (void *)OnClose,
  (void *)OnFocus,
  (void *)OnUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data, //method7, //Destroy
  (void *)int_0,
  (void *)int_0,
  0
};

GUI *DT_gui;

void Create_DtGui(CSM_RAM *data)
{
	  static const RECT Canvas={0,0,0,0};
	  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
	  GUI *main_gui=DT_gui=malloc(sizeof(GUI));
	  zeromem(main_gui,sizeof(GUI));
	  main_gui->canvas=(void *)(&Canvas);
	  main_gui->methods=(void *)gui_methods;
	  main_gui->item_ll.data_mfree=(void (*)(void *))mfree_adr();
	  Dt.MAINGUI_ID=CreateGUI(main_gui);
}

static void maincsm_onclose(CSM_RAM *data)
{
 // SUBPROC((void *)Killer);
}

static void maincsm_oncreate(CSM_RAM *data)
{
  if(!idle_ch) Create_DtGui(data);
}

int OnMSG(CSM_RAM *data, GBS_MSG *msg)
{
  if (msg->msg==MSG_IPC)
  {
	IPC_REQ *ipc;
// 	PLG_P *TP;
	DT_ICON *TI;
	if ((ipc=(IPC_REQ*)msg->data0))
	{
	  if (strcmp_nocase(ipc->name_to,IPC_DT_NAME)==0)
	  {
		switch (msg->submess)
		{
		  // 			case IPC_LOAD_PLG:
		  //Если приняли свое собственное сообщение, значит запускаем чекер
		  // 			  if (ipc->name_from==ipc_my_name) CheckDoubleRun();
		  // 			  else ipc->data=(void *)DAEMON_ID;
		  // 			  break;
		  case IPC_LOAD_PLG:
			TI=ipc->data;
// 			if(((DT_ICON*)TI)->d==0xFF)
			{
			  TI=Dt.first;
			  do
			  {
				if(((DT_ICON*)TI)->d==0xFF)
				{
				  PLG_P *TP=malloc(sizeof(PLG_P));
				  TP->dt=&Dt;//&PLG_funcs;
				  TP->ic=TI;
				  wsprintf(ews,perc_t,((DT_ICON*)TI)->fl);
				  ExecuteFile(ews,NULL,TP);
				}
			  }while(TI=TI->next);
// 			  TP=malloc(sizeof(PLG_P));
// 			  TP->dt=&Dt;//&PLG_funcs;
// 			  TP->ic=TI;
// 			  wsprintf(ews,perc_t,((DT_ICON*)TI)->fl);
// 			  ExecuteFile(ews,NULL,TP);
			}
			
			Start_Refresh_Timer();
			break;
		}
	  }
	}
  }
  
  if (msg->msg==MSG_USSD_RX || msg->msg==MSG_AUTOUSSD_RX)
  {
	ProcessUSSD(data,(GBS_USSD_MSG *)msg);
  }
  
  if(msg->msg == MSG_RECONFIGURE_REQ) // Перечитывание конфига по сообщению
  {
	if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
	{
	  InitConfig();
	  refresh_dt();
	  ShowMSG(1,(int)lg.config_updated);
	}
  }
  return 1;
}

int IDLEcsm_onmessage(CSM_RAM *icsm, GBS_MSG *msg)
{
  int rtn=OnMSG(icsm, msg);
  
//   if(!IsCalling())
  {
// 	CSM_RAM *icsm=data;//FindCSMbyID(CSM_root()->idle_id);
	if (IsGuiOnTop(idlegui_id)/*&&IsUnlocked()*/) //Если IdleGui на самом верху
	{
	  GUI *igui=GetTopGUI();
	  if (igui) //И он существует
	  {
		Create_DtGui(icsm);
	  }
	}
  }
  
  rtn=old_icsm_onMessage(icsm, msg);
  return(rtn);
}


static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  int rtn=OnMSG(data, msg);
  
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==Dt.MAINGUI_ID))
  {
	MAIN_CSM *csm=(MAIN_CSM*)data;
	csm->csm.state=-3;
  }
  
  return(rtn);
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
  sizeof(CSM_RAM),
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"DeskTop");
}

int main(char *exename, char *fname)
{
  unsigned int err;
	InitConfig();
	//int sz=strlen(exename);
	//while(exename[sz]!='\\') sz--;
	if(isdir(DT_ROOT,&err)){
	  int len=strlen(DT_ROOT);
	  Dt.rootadr=malloc(len+2);
	  zeromem(Dt.rootadr, len+2);
	  memcpy(Dt.rootadr, DT_ROOT, len);//=malloc(sz+2);
	  if(Dt.rootadr[len-1]!='\\')
	  {
		Dt.rootadr[len]='\\';
		//Dt.rootadr[len+1]='\0';
	  }
	}else Dt.rootadr=getfolder(exename);
// 	logtext(Dt.rootadr);
        
        
	//zeromem(Dt.rootadr, sz+2);
	//memcpy(Dt.rootadr, exename, sz+1);
	Create();
	LockSched();
	if(CH_IDLE)
	{
		CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
		old_icsm_onMessage=((CSM_DESC*)icsm->constr)->onMessage;
		((CSM_DESC*)icsm->constr)->onMessage=IDLEcsm_onmessage;
		idle_ch=3;
		Dt.MAINCSM_ID=0;
    }
	else
	{
		MAIN_CSM main_csm;
		UpdateCSMname();
		Dt.MAINCSM_ID=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
	}
	UnlockSched();
  return 0;
}







// DtStates myStates={
// 	{dr_MAIN, m_key_MAIN},
//  {dr_MENU, m_key_MENU},
//  {dr_MOVE, m_key_MOVE},
//  {dr_RESZ, m_key_RESZ},
//  {dr_CHTR, m_key_CHTR},
//  {dr_WAIT, m_key_WAIT},
// 
//  {dr_PGUI, m_key_PGUI},
// 
//  {dr_PGM, m_key_PGM},
//  {dr_PGR, m_key_PGR},
// 
//  {dr_LPLG, m_key_LPLG},
//  {dr_ERR, m_key_ERR}
// };

// states:
DtSt st_MAIN={dr_MAIN, m_key_MAIN},
  st_MENU={dr_MENU, m_key_MENU},
  st_MOVE={dr_MOVE, m_key_MOVE},
  st_RESZ={dr_RESZ, m_key_RESZ},
  st_CHTR={dr_CHTR, m_key_CHTR},
  st_WAIT={dr_WAIT, m_key_WAIT},
  
//   st_PGUI={dr_PGUI, m_key_PGUI},
// 
//   st_PGM={dr_PGM, m_key_PGM},
//   st_PGR={dr_PGR, m_key_PGR},

  st_LPLG={dr_LPLG, m_key_LPLG},
  st_ERR={dr_ERR, m_key_ERR};
