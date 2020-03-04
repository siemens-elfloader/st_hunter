#include "../inc/swilib.h"
#include "../inc/xtask_ipc.h"
#include "conf_loader.h"
#include "ipc.h"

#define MAX_IMAGES 4
#define ABOUT "VMixer v0.2\n(c)FIL\n(r)Viktor89"

int gui_csm_id;
const char *percent_s="%s";

typedef struct
{
	GUI gui;
	WSHDR *ws;
	IMGHDR *img[MAX_IMAGES];
	int vol[3];//0-system, 1-alarm, 2-call
	int cur;
}MAIN_GUI;

typedef struct
{
	CSM_RAM csm;
	int gui_id;
}MAIN_CSM;

void DrawIMG(IMGHDR *img, int x, int y, int x2, int y2)
{
	RECT rc;
	DRWOBJ drwobj;
	if(!x2 && !y2)
	{
		StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
		SetPropTo_Obj5(&drwobj, &rc, 0, img);
	}
	else
	{
		StoreXYXYtoRECT(&rc, x, y, x2, y2);
		SetProp2ImageOrCanvas(&drwobj, &rc, 0, img, x, y);
	}
	SetColor(&drwobj, NULL, NULL);
	DrawObject(&drwobj);
}

int GetFontFromCFG(const int cfg_font)
{
	switch(cfg_font)
	{
		case 0: return FONT_SMALL_ITALIC;
		case 1: return FONT_SMALL_ITALIC_BOLD;
		case 2: return FONT_SMALL;
		case 3: return FONT_SMALL_BOLD;
		case 4: return FONT_MEDIUM;
		case 5: return FONT_MEDIUM_BOLD;
		case 6: return FONT_LARGE;
		case 7: return FONT_LARGE_BOLD;
		default: return FONT_SMALL;
	}
}

inline unsigned int get_ws_width(WSHDR *ws, int font){ return Get_WS_width(ws, font)+1; }

void OnRedraw(MAIN_GUI *data)
{
	extern const int CFG_X_BG, CFG_Y_BG;
	extern const char cfg_col_bg[];
	enum {scrot, bg, slider, slider_select};
	
	DrawIMG(data->img[scrot], 0, 0, 0, 0);
	DrawRectangle(0, 0, ScreenW(), ScreenH(), 0, cfg_col_bg, cfg_col_bg);
	DrawIMG(data->img[bg], CFG_X_BG, CFG_Y_BG, 0, 0);
	
	extern const int CFG_X_VOL_ALARM, CFG_X_VOL_CALL, CFG_X_VOL_SYSTEM;
	extern const int CFG_Y_VOL_ALARM, CFG_Y_VOL_CALL, CFG_Y_VOL_SYSTEM;
	extern const int CFG_X_SLIDER_CALL, CFG_X_SLIDER_ALARM, CFG_X_SLIDER_SYSTEM;
	extern const int CFG_Y_SLIDER_CALL, CFG_Y_SLIDER_ALARM, CFG_Y_SLIDER_SYSTEM;
	extern const int CFG_STEP_SLIDERS;
	extern const char CFG_TEXT1[128], CFG_TEXT2[128], CFG_TEXT3[128];
	extern const int CFG_X_TEXT1, CFG_X_TEXT2, CFG_X_TEXT3;
	extern const int CFG_Y_TEXT1, CFG_Y_TEXT2, CFG_Y_TEXT3;
	extern const char CFG_COLOR_TEXT[4], CFG_COLOR_VOLUME[4];
	extern const int CFG_FONT_TEXT, CFG_FONT_VOLUME;
	const int x_vol[]={CFG_X_VOL_SYSTEM, CFG_X_VOL_ALARM, CFG_X_VOL_CALL};
	const int y_vol[]={CFG_Y_VOL_SYSTEM, CFG_Y_VOL_ALARM, CFG_Y_VOL_CALL};
	const int x_sliders[]={CFG_X_SLIDER_SYSTEM, CFG_X_SLIDER_ALARM, CFG_X_SLIDER_CALL};
	const int y_sliders[]={CFG_Y_SLIDER_SYSTEM, CFG_Y_SLIDER_ALARM, CFG_Y_SLIDER_CALL};
	const char *text[]={CFG_TEXT1, CFG_TEXT2, CFG_TEXT3};
	const int x_text[]={CFG_X_TEXT1, CFG_X_TEXT2, CFG_X_TEXT3};
	const int y_text[]={CFG_Y_TEXT1, CFG_Y_TEXT2, CFG_Y_TEXT3};
	
	unsigned int font;
	for(int i=0; i<3; i++)
	{
		font=GetFontFromCFG(CFG_FONT_VOLUME);
		wsprintf(data->ws, "%d", data->vol[i]);
		DrawString(data->ws, x_vol[i], y_vol[i], x_vol[i]+get_ws_width(data->ws, font), y_vol[i]+GetFontYSIZE(font), font,
			TEXT_ALIGNLEFT, CFG_COLOR_VOLUME, GetPaletteAdrByColorIndex(23));
		
		font=GetFontFromCFG(CFG_FONT_TEXT);
		wsprintf(data->ws, "%s", text[i]);
		DrawString(data->ws, x_text[i], y_text[i], x_text[i]+get_ws_width(data->ws, font),
			y_text[i]+GetFontYSIZE(font), font, TEXT_ALIGNLEFT, CFG_COLOR_TEXT,
			GetPaletteAdrByColorIndex(23));
		
		if(data->cur==i)
			DrawIMG(data->img[slider_select], x_sliders[i], y_sliders[i]-(data->vol[i]-1)*CFG_STEP_SLIDERS, 0, 0);
		else
			DrawIMG(data->img[slider], x_sliders[i], y_sliders[i]-(data->vol[i]-1)*CFG_STEP_SLIDERS, 0, 0);
		
	}
}

void ScreenBuffer2IMGHDR(IMGHDR *img)
{
	int w=ScreenW(), h=ScreenH();
	unsigned int bitsize=w*h*2;
	
	img->bitmap=malloc(bitsize);
	memcpy(img->bitmap, RamScreenBuffer(), bitsize);
	img->w=w; img->h=h;
	img->bpnum=0x08;
}

void FreeGraphics(IMGHDR **img)
{
	for(int i=0; i<MAX_IMAGES; i++)
	{
		if(img[i])
		{
			mfree(img[i]->bitmap);
			mfree(img[i]);
		}
	}
}

int LoadGraphics(IMGHDR **img)
{
	extern const char CFG_PATH_SKIN[128];
	const char *names[]={"bg", "slider", "slider_select"};
	char fname[128];
	FSTATS fs;
	unsigned int err;
	for(int i=1; i<MAX_IMAGES; i++)
	{
		sprintf(fname, "%s%s%s%s", CFG_PATH_SKIN, "img\\", names[i-1], ".png");
		if(GetFileStats(fname, &fs, &err)!=-1)
			img[i]=CreateIMGHDRFromPngFile(fname, 0);
		else return -1;
	}
	
	ScreenBuffer2IMGHDR(img[0]=malloc(sizeof(IMGHDR)));
	
	return (1);
}

void CloseGUI(void)
{
	if(gui_csm_id)
	{
		LockSched();
		CloseCSM(gui_csm_id);
		gui_csm_id=0;
		UnlockSched();
	}
}

#define AE_FAK   0
#define AE_SETUP 4

void SetGetVolume(int *vol, int set)
{
	const char *words[]={"AUDIO_SYSTEMVOLUME", "AUDIO_ALARM_CLOCK_VOLUME", "AUDIO_VOICECALLVOLUME"};
	if(set)
	{
		for(int i=0; i<3; i++)
			SettingsAE_Update(vol[i], AE_SETUP, NULL, (char*)words[i]);
	}
	else
	{
		for(int i=0; i<3; i++)
			SettingsAE_Read(vol+i, AE_SETUP, NULL, (char*)words[i]);
	}
}

void OnCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
	data->ws=AllocWS(128);
	if(LoadGraphics(data->img)==-1)
	{
		ShowMSG(1, (int)"Graphics not load!\nQuit...");
		CloseGUI();
	}
	SetGetVolume(data->vol, 0);
	data->gui.state=1;
}

void OnClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
	SetGetVolume(data->vol, 1);
	FreeGraphics(data->img);
	FreeWS(data->ws);
	gui_csm_id=data->gui.state=0;
}

void OnFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
	DisableIconBar(1);
#endif
	DisableIDLETMR();
	data->gui.state=2;
}

void OnUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
#ifdef ELKA
	DisableIconBar(0);
#endif
	if (data->gui.state!=2) return;
	data->gui.state=1;
}

void RunFile(const char *fname)
{
	const int len=strlen(fname);
	WSHDR *ws=AllocWS(len);
	str_2ws(ws, fname, len);
	ExecuteFile(ws, NULL, NULL);
	FreeWS(ws);
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
	int style=msg->gbsmsg->msg;
	int key=msg->gbsmsg->submess;
	extern const int cfg_key_about, cfg_key_open_skin;
    extern char *successed_config_skin_filename;
	if(style==KEY_DOWN || style==LONG_PRESS)
	{
		switch(key)
		{
			case RIGHT_SOFT: return 1;
			case LEFT_BUTTON:
				data->cur=(data->cur!=0) ? --data->cur : 2;
			break;
			case RIGHT_BUTTON:
				data->cur=(data->cur!=2) ? ++data->cur : 0;
			break;
			case DOWN_BUTTON: case VOL_DOWN_BUTTON:
				/*switch(data->cur)
				{
					case 2: if(data->vol[data->cur]!=1) data->vol[data->cur]--; break;
					default: */if(data->vol[data->cur]!=0) data->vol[data->cur]--;
				//}
			break;
			case UP_BUTTON: case VOL_UP_BUTTON:
				if(data->vol[data->cur]!=5) data->vol[data->cur]++;
			break;
		}
		if     (key==cfg_key_about) ShowMSG(1,(int)ABOUT);
		else if(key==cfg_key_open_skin) RunFile(successed_config_skin_filename);
		REDRAW();
	}
	return 0;
}

extern void kill_data(void *p, void (*func_p)(void *));

int method8(void){return(0);}

int method9(void){return(0);}

const void * const gui_methods[11]=
{
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
	zeromem(main_gui,sizeof(MAIN_GUI));
	Canvas.x2=ScreenW()-1;
	Canvas.y2=ScreenH()-1;
	main_gui->gui.canvas=(void *)(&Canvas);
	main_gui->gui.flag30=2;
	main_gui->gui.methods=(void *)gui_methods;
	main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
	csm->csm.state=0;
	csm->csm.unk1=0;
	csm->gui_id=CreateGUI(main_gui);
}

void maincsm_onclose(CSM_RAM *csm)
{
	if(successed_config_skin_filename)
	{
		mfree(successed_config_skin_filename);
		successed_config_skin_filename=NULL;
	}
}

extern const int minus11;
unsigned short maincsm_name_body[140];

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
	MAIN_CSM *csm=(MAIN_CSM*)data;
	if (msg->msg==MSG_GUI_DESTROYED)
	{
		if ((int)msg->data0==csm->gui_id)
			csm->csm.state=-3;
	}
	if(msg->msg == MSG_RECONFIGURE_REQ) 
	{
		extern int strcmp_nocase(const char *s1,const char *s2);
		if(strcmp_nocase(successed_config_skin_filename,(char *)msg->data0)==0)
			InitConfigSkin();
	}
	return(1);
}



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
	wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"VMixer");
}

void CreateOpenGUI(void)
{
	if(!gui_csm_id)
	{
		char dummy[sizeof(MAIN_CSM)];
		UpdateCSMname();
		InitConfigSkin();
		LockSched();
		gui_csm_id=CreateCSM(&MAINCSM.maincsm,dummy,0);
		UnlockSched();
	}
	else
		IPC_Send(IPC_XTASK_NAME, IPC_MY_NAME, (void*)gui_csm_id, IPC_XTASK_SHOW_CSM);
}
