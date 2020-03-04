#include "../inc/swilib.h"
#include "../inc/xtask_ipc.h"
#include "graphics.h"
#include "conf_loader.h"
#include "stopwatch.h"
#include "write_read.h"
#include "interface.h"
#include "lang.h"

#define VERSION "1.5"

extern void kill_data(void *p, void (*func_p)(void *));
extern const int CFG_KEEP_LIGHT;

const char *t="%t";

char *root_folder;  //каталог, откуда запущен эльф
unsigned int csm_id, gui_id;

typedef struct
{
  GUI gui;
  unsigned int cur_button, cur_list, list_total;
  unsigned int load_status;
}MAIN_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

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

void IllumHook(int disable)//удержание подсветки
{
	extern const int CFG_KEEP_LIGHT;
	if(!CFG_KEEP_LIGHT) return;
	
	if(disable)
	{
		if(CFG_KEEP_LIGHT==2)
			IllumTimeRelease(4, 3);
		else
			IllumTimeRelease(1, 1);
	}
	else
	{
		if(CFG_KEEP_LIGHT==2)
			IllumTimeRequest(4, 3);
		else
			IllumTimeRequest(1, 1);
	}
}

void OnRedraw(MAIN_GUI *data)
{
	if(data->load_status<IMG_TOTAL) DrawLoadingScreen();
	DrawIMG(IMG[IMG_BG], 0, 0, 0, 0);
	DrawCurTime(2);
	DrawButtons(data->cur_button);
	DrawSplitTimes(1, data->cur_list);
}

void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	SUBPROC((void*)Graphics_Init, &data->load_status);
	data->list_total=data->cur_list=(res.total-1)/10;
	data->gui.state=1;
}

void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	Graphics_Free();
	StopWatch_Stop();
	data->gui.state=0;
}

void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
	DisableIDLETMR();
	IllumHook(0);
	data->gui.state=2;
}

void OnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	if (data->gui.state != 2) return;
	IllumHook(1);
	data->gui.state=1;
}

void AboutMSG(void)
{
	char *msg=malloc(strlen(LGP_COPYRIGHT)+strlen(VERSION)+20+1);
	sprintf(msg, LGP_COPYRIGHT, VERSION, __DATE__);
	ShowMSG(1,(int)msg);
	mfree(msg);
}

inline void ProcClose(int data)
{
	if(!data) CloseCSM(csm_id);
}

void CloseE()
{
	extern const int CFG_CFM_ON_EXIT;
	if(CFG_CFM_ON_EXIT) MsgBoxYesNo(1, (int)LGP[LGP_CFM_ON_EXIT], ProcClose);
	else CloseCSM(csm_id);
}

void RunFile(const char *fname)
{
	const int len=strlen(fname);
	WSHDR *ws=AllocWS(len);
	str_2ws(ws, fname, len);
	ExecuteFile(ws, NULL, NULL);
	FreeWS(ws);
}

inline void Turn(void)
{
	extern const int CFG_STAY_AFTER_LAUNCH;
	if(CFG_STAY_AFTER_LAUNCH)
	{
		static IPC_REQ IPC;
		IPC.name_to=IPC_XTASK_NAME;
		IPC.name_from="NStopWatch";
		IPC.data=NULL;
		GBS_SendMessage(MMI_CEPID, MSG_IPC, IPC_XTASK_IDLE, &IPC);
	}
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
	extern const int CFG_ACTION_RIGHT_SOFT;
	extern const int CFG_BUTTON_START_SPLIT, CFG_BUTTON_SAVE, CFG_BUTTON_OPEN_CFG, CFG_BUTTON_SHOW_ABOUT;
	extern const int CFG_SWITCH_NEXT_PAGES, CFG_SWITCH_PREV_PAGES;
	int key=msg->gbsmsg->submess;
	int style=msg->gbsmsg->msg;
	if(style==KEY_DOWN || style==LONG_PRESS)
	{
		if(key==RIGHT_SOFT)
		{
			enum {exit, stop_and_exit};
			switch(CFG_ACTION_RIGHT_SOFT)
			{
				case exit: CloseE(); break;
				case stop_and_exit: if(IsStart()) goto R_STOP; else CloseE(); break;
			}
		}
		if(data->load_status>=IMG_TOTAL)
		{
			if(key==CFG_BUTTON_SAVE) WriteResults();
			if(key==CFG_BUTTON_OPEN_CFG) RunFile(successed_config_filename);
			if(key==CFG_BUTTON_SHOW_ABOUT) AboutMSG();
			if(key==CFG_SWITCH_NEXT_PAGES)
			{
				if(data->cur_list<data->list_total)
				{
					DrawSplitTimes(1, ++data->cur_list);
				}
			}
			if(key==CFG_SWITCH_PREV_PAGES)
			{
				if(data->cur_list>0)
				{
					DrawSplitTimes(1, --data->cur_list);
				}
			}
			if(key==RIGHT_BUTTON || key==LEFT_BUTTON) //навигация по кнопкам
			{
				data->cur_button=(data->cur_button) ? 0 : 1;
				DrawButtons(data->cur_button);
			}
			if(style==KEY_DOWN)
			{
				if(key==CFG_BUTTON_START_SPLIT)
					goto L_BUTTON;
				if(key==ENTER_BUTTON)
				{
					switch(data->cur_button)
					{
						case 0:
							L_BUTTON:
							if(!IsStart())
							{
								Turn();
								StopWatch_Start();
								DrawButtons(data->cur_button);
							}
							else
							{
								Results_Rem();   //запомнить результаты
								if(res.total!=1 && res.total%10==1) //каждый новый список
								{
									data->list_total++;
									//data->navi.cur_list++;
									DrawSplitTimes(1, ++data->cur_list);
								}
								if(data->cur_list!=data->list_total)
								{
									data->cur_list=data->list_total;
									DrawSplitTimes(1, data->cur_list);
								}
								else
								{
									DrawSplitTimes(0, data->cur_list);
								}
							}
						break;
						case 1:
							if(!IsStart())
							{
								Results_Free();
								data->cur_list=data->list_total=0;
								DirectRedrawGUI();
							}
							else
							{
								R_STOP:
								StopWatch_Stop();
								DrawButtons(data->cur_button);
							}
						break;
					}
				}
			}
		}
	}
	return 0;
}

int method8(void){return 0;}

int method9(void){return 0;}

void * const gui_methods[11]={
  (void *)OnRedraw,	//Redraw
  (void *)OnCreate,	//Create
  (void *)OnClose,	//Close
  (void *)OnFocus,	//Focus
  (void *)OnUnfocus,	//Unfocus
  (void *)OnKey,	//OnKey
  0,
  (void *)kill_data, //method7,	//Destroy      
  (void *)method8,
  (void *)method9,
  0
};

RECT Canvas={0,0,0,0};

void maincsm_oncreate(CSM_RAM *data)
{
	MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
	MAIN_CSM*csm=(MAIN_CSM*)data;
	zeromem(main_gui, sizeof(MAIN_GUI));
	Canvas.x2=ScreenW()-1;
	Canvas.y2=ScreenH()-1;
	main_gui->gui.canvas=(void *)(&Canvas);
	main_gui->gui.flag30=2;
	main_gui->gui.methods=(void *)gui_methods;
	main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
	csm->csm.state=0;
	csm->csm.unk1=0;
	gui_id=csm->gui_id=CreateGUI(main_gui);
	ws_cur_time=AllocWS(10);
}

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{
	Langpack_Free();
	FreeWS(ws_cur_time);
	Results_Free();
	if(successed_config_skin_filename) mfree(successed_config_skin_filename);
	if(successed_config_filename) mfree(successed_config_filename);
	if(root_folder) mfree(root_folder);
	SUBPROC((void*)ElfKiller);
}

int IsIdleOnTop() //главный экран наверху
{
	return IsGuiOnTop(((int *)FindCSMbyID(CSM_root()->idle_id))[DISPLACE_OF_IDLEGUI_ID/4]);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
	MAIN_CSM *csm=(MAIN_CSM*)data;
	if (msg->msg==MSG_GUI_DESTROYED)
	{
		if ((int)msg->data0==csm->gui_id)
			csm->csm.state=-3;
	}
	if (IsUnlocked() && IsIdleOnTop() && msg->msg == MSG_IPC)
	{
		IPC_REQ *ipc;
		if ((ipc=(IPC_REQ*)msg->data0))
		{
			if (strcmp_nocase(ipc->name_to, "IdleUpd")==0)
			{
				switch (msg->submess)
				{
					case 0:
						DrawPicOnIdle();
						DrawTimeOnIdle();
					break;
				}
			}
		}
	}
	if(msg->msg == MSG_RECONFIGURE_REQ) 
	{
		if(strcmp_nocase(successed_config_filename, (char *)msg->data0)==0)
		{
			extern const int CFG_MSG_REQ;
			InitConfig();
			if(CFG_MSG_REQ) ShowMSG(1, (int)LGP[LGP_MSG_REQ]);
		}
		if(strcmp_nocase(successed_config_skin_filename, (char *)msg->data0)==0)
			InitConfigSkin();
	}
  return(1);
}

const int minus11=-11;
unsigned short maincsm_name_body[140];

const struct
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
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name), t, LGP[LGP_CSM_NAME]);
}

void CreateFolder(const char *name)
{
	char *fname=malloc(strlen(name)+strlen(root_folder)+2);
	unsigned int err;
	sprintf(fname, "%s%s\\", root_folder, name);
	mkdir(fname, &err);
	mfree(fname);
}

inline int IsNswExt(const char *fname)
{
	const char *ext="nsw";
	if(strcmp_nocase(fname+strlen(fname)-3, ext)==0) return 1;
	return 0;
}

int main(const char *elfname, const char *filename)
{
	char dummy[sizeof(MAIN_CSM)];
	
	char *path=strrchr(elfname, '\\');
	path++;
	int l=path-elfname;
	root_folder=malloc(l+1);
	zeromem(root_folder, l+1);
	memcpy(root_folder, elfname, l);
	//root_folder[l]='\0';
	
	CreateFolder("Results");
	
	Langpack_Init();
	UpdateCSMname();
	LockSched();
	if(filename &&  IsNswExt(filename) && ReadResults(filename)==-1)
		ShowMSG(1, (int)LGP[LGP_MSG_ERROR_OPEN_FILE]);
	
	csm_id=CreateCSM(&MAINCSM.maincsm,dummy,0);
	InitConfig();
	InitConfigSkin();
	UnlockSched();
	return 0;
}
