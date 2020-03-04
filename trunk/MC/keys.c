#include "inc/mc.h"
#include "inc/secret.h"
#include "inc/mui.h"

typedef struct{
	int key;
	char* name;
} KEYNM_MAP;

const KEYNM_MAP keynames[] =
{
	{LEFT_SOFT, "lsoft"},
	{RIGHT_SOFT, "rsoft"},
	{GREEN_BUTTON, "green"},
	{RED_BUTTON, "red"},
	{ENTER_BUTTON, "enter"},
	{UP_BUTTON, "up"},
	{DOWN_BUTTON, "down"},
	{LEFT_BUTTON, "left"},
	{RIGHT_BUTTON, "right"},
	{VOL_UP_BUTTON, "volup"},
	{VOL_DOWN_BUTTON, "voldown"},
	{'0', "0"},
	{'1', "1"},
	{'2', "2"},
	{'3', "3"},
	{'4', "4"},
	{'5', "5"},
	{'6', "6"},
	{'7', "7"},
	{'8', "8"},
	{'9', "9"},
	{'*', "*"},
	{'#', "#"},
};

char keyMap[0x40];

KEY_PROC procmap[] =
{
	DoMenu,
	(KEY_PROC)DoBack,
	DoSent,
	NULL,
	DoOpen,
	DoUp,
	DoDwn,
	DoPrvDrv,
	DoNxtDrv,
	NULL,
	NULL,
	DoGetFF,
	DoSwapTab,
	DoUp,
	DoPgUp,
	DoPrvDrv,
	DoAltOpen,
	DoNxtDrv,
	NULL,
	DoDwn,
	DoPgDwn,
	NULL,
	DoShowDisks,
};

// long press
KEY_PROC procmapl[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
        NULL,
};
#define KEYS_COUNT (sizeof(procmap)/sizeof(procmap[0]))


void DoKey(int isLongPress, int key)
{
  char idx = GetKeynameIndexByKey(key);
  if (idx != 'x')
  {
    if (isLongPress)
    {
      if (procmapl[idx]) procmapl[idx]();
    }
    else
    {
      if (procmap[idx]) procmap[idx]();
    }
  }
}

typedef struct{
	char* name;
	KEY_PROC proc;
} PROC_MAP;

const PROC_MAP keyprocs[]=
{
	{"none", NULL},
	{"open", DoOpen},
	{"back", (KEY_PROC)DoBack},
	{"rename", DoRen},
	{"past", DoPaste},
	{"copy", DoCopy},
	{"move", DoMove},
	{"del", DoDel},
	{"cancel", DoCancel},
	{"newdir", DoNewDir},
	{"menu", DoMenu},
        {"fileprop", DoFilePropNew},
	{"drvinf", DoDrvInf},
	{"swaptab", DoSwapTab},
	{"prvdrv", DoPrvDrv},
	{"nxtdrv", DoNxtDrv},
	{"invchk", DoInvChk},
	{"chk", DoChk},
	{"chkall", DoChkAll},
	{"unchall", DoUnCAll},
	{"up", DoUp},
	{"dwn", DoDwn},
	{"pgup", DoPgUp},
	{"pgdn", DoPgDwn},
	{"bmlist", DoBMList},
	{"bmadd", DoBMAdd},
	{"filter", DoFilter},
	{"refresh", (KEY_PROC)DoRefresh},
	{"root", DoRoot},
	{"exit", DoExit},
	{"showpath", DoShowPath},
	{"begin", DoBegin},
	{"sortn", DoSortN},
	{"sorte", DoSortE},
	{"sorts", DoSortS},
	{"sortd", DoSortD},
	{"sortr", DoSortR},
	{"setsort", DoSetSort},
	{"newfile", DoNewFile},
	{"tabcopy", DoTabCopy},
	{"tabmove", DoTabMove},
        {"altopen", DoAltOpen},
        {"natexpl", DoNatExpl},
        {"sent", DoSent},
        {"disks", DoShowDisks},
        {"getff", DoGetFF},
        #if GOLD_ZIP == 0
        {"crtarh", DoArchive},
        #endif
        {"crtpl", DoCrtPL},
        {"keyhelp", DoKeyHelp},
        {"unpack", DoUnPack},
        {"md5", DoMD5},
        {"getlng", DoLang}
};

void KeysProc(char *name, char *value)
{
	strtolower(name, name, -1);
	strtolower(value, value, -1);

	int inkey = -1;
	int islong = 0;
	KEY_PROC keyproc = DoErrKey;

	int nameLen = strlen(name);
	if (nameLen > 0 && name[nameLen - 1] == '+')
	{
		// Последний символ +, значит долгое нажатие
		islong = 1;
		name[nameLen - 1] = 0; // Убираем плюс
	}

	for(uint32_t cc=0; cc < KEYS_COUNT; cc++)
	{
		if (strcmp(keynames[cc].name, name)==0)
		{
			inkey = cc;
			break;
		}
	}
	if (inkey == -1) return;

	for(uint32_t cc = 0; cc < sizeof(keyprocs) / sizeof(keyprocs[0]); cc++)
	{
		if (strcmp(keyprocs[cc].name, value) == 0)
		{
			keyproc = keyprocs[cc].proc;
			break;
		}
	}

	if (islong)
		procmapl[inkey] = keyproc;
	else
		procmap[inkey] = keyproc;
}

void InitializeKeyMap()
{
	// Инициируем массив, 'x' - значит нет такой клавиши
	memset(keyMap, 'x', 0x40);
	// Сохраняем индексы
	for (uint32_t i = 0; i < KEYS_COUNT; i++)
		keyMap[keynames[i].key] = i;
}

char GetKeynameIndexByKey(char key)
{
	if (key >= 0x40) return 'x';
	else return keyMap[key];
}

KEY_PROC GetKeyprocByKey(char key)
{
	char idx = GetKeynameIndexByKey(key);
	if (idx != 'x')
		return procmap[idx];
	else
		return NULL;
}

KEY_PROC GetKeyprocLongByKey(char key)
{
	char idx = GetKeynameIndexByKey(key);
	if (idx != 'x')
		return procmapl[idx];
	else
		return NULL;
}

void LoadKeys()
{
	if (CONFIG_LOAD_KEYS)
		EnumIni(1,(char*)&keys_file,KeysProc);

	InitializeKeyMap();
}
