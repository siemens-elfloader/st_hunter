#ifndef _MAIN_H_
#define _MAIN_H_

#include "master.h"

//#define BUG

#ifdef NEWSGOLD
#define CBOX_CHECKED 0xE116
#define CBOX_UNCHECKED 0xE117
#else
#define CBOX_CHECKED 0xE10B
#define CBOX_UNCHECKED 0xE10C
#endif

typedef struct
{
	CSM_RAM csm;
	int gui_id;
}MAIN_CSM;

typedef struct
{
	GUI gui;
}MAIN_GUI;

#define PTC_SIZE	(2*1024)
#define CONFIG_SIZE	(48*1024)
#define MAX_WS_LEN	512
#define X_CHAR		0x20

#ifndef WINTEL_DEBUG

extern char *ptc_buf;
//extern char *cfg_buf;
extern int patch_n;
extern unsigned int MAIN_GUI_ID;
extern unsigned int MAIN_CSM_ID;
extern int PTCFG_CUR;
extern int S_ICONS[3];
extern int isRunScanerNeed;
#define PATCH_DIR	"0:\\Misc\\Patches\\"
#define PTC_FOLDR	"ptc\\"
void on_sdec(USR_MENU_ITEM *item);
#endif

#endif
