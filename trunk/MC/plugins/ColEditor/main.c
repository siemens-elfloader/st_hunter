#include "../../../inc/swilib.h"
#include "ColorMap.h"

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;
int my_csm_id=0;
char filename[128];
int res;
void EditColors(char *color);

const char _tc[] = "%d. %t:\n";
int leng=0;

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

char *name[(clMAX+1)*2]=
{
  "Фон - обводка",
  "Фон - фон",
  "Список файлов - обводка",
  "Список файлов - фон",
  "Курсор - обводка",
  "Курсор - фон",
  "Инфо - обводка",
  "Инфо - фон",
  "Меню - обводка",
  "Меню - фон",
  "Меню курсор - обводка",
  "Меню курсор - фон",
  "Скрин - обводка",
  "Скрин - фон",

  "Прогрес бар - обводка",
  "Прогрес бар - фон",
  "Прогрес бар - заполнение",

  "Полоса прокрутки - полоса",
  "Полоса прокрутки - бегунок",

  "Чекбокс - обводка",
  "Чекбокс - фон",
  "Чекбокс - нажатый",
  "Чекбокс - выделенный",

  "Файл - норм. не выделенный",
  "Файл - норм. выделенный",
  "Файл - скрытый. не выделенный",
  "Файл - скрытый. выделенный",
  "Файл - только чтение не выделенный",
  "Файл - только чтение выделенный",
  "Файл - информация",

  "Шапка",
  "Шапка - обводка",

  "Софты",
  "Софты - обводка",

  "Меню - не выделенный",
  "Меню - выделенный",

  "Инфо текст",

  //////////////////////////////////////

  "Background - border",
  "Background - background",
  "List of files - border",
  "List of files - background",
  "Cursor - border",
  "Cursor - background",
  "Info - border",
  "Info - background",
  "Menu - border",
  "Menu - background",
  "Menu cursor - border",
  "Menu cursor - background",
  "Screen - border",
  "Screen - background",

  "Progresbar - border",
  "Progresbar - background",
  "Progresbar - filling",

  "Band scrolling - band",
  "Band scrolling - runner",

  "Checkbox - border",
  "Checkbox - background",
  "checkbox - clicked",
  "Checkbox - selected",

  "File - normal is not selected",
  "File - normal  selected",
  "File - hidden, not selected",
  "File - hidden, selected",
  "File - read-only, not selected",
  "File - read-only, selected",
  "File information",

  "Title",
  "Title-border",

  "Soft buttons",
  "Soft buttons - border",

  "Menu - is not selected",
  "Menu - selected",

  "Info - text",
};


extern char Colors[];

int isElka()
{
  if (ScreenW()==132 && ScreenH()==176)
    return 0;
  if (ScreenW()==240 && ScreenH()==320)
    return 24;

  return -1;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=isElka();
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+isElka()-1;
}

#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
	((INPUTDIA_DESC*)inp)->rc.x=0;
	((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+isElka();
	((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
	((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}

const SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Сохр."},
  {0x0001,0x0000,(int)"Выход"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};
const SOFTKEYSTAB menu_skt=
{menu_sk,0};

int edOnKey(GUI *data, GUI_MSG *msg)
{

  if (msg->keys==0xFFF)  // OK
  {
    SaveCS();
    return 1;
  }

  if (msg->keys==0xFFE)  // Back
    return (0xFF);

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    int key=msg->gbsmsg->submess;
    int i=EDIT_GetFocus(data)/2-1;

    if (key==ENTER_BUTTON)
       EditColors(&Colors[i*4]);
  }
  return 0;
}
void edGHook(GUI *data, int cmd)
{
  static SOFTKEY_DESC ok={0x0FFF,0x0000,(int)"Сохр."};
  static SOFTKEY_DESC close={0x0FFE,0x0000,(int)"Выход"};

  static SOFTKEY_DESC ok1={0x0FFF,0x0000,(int)"Save"};
  static SOFTKEY_DESC close1={0x0FFE,0x0000,(int)"Exit"};

  WSHDR *ews=AllocWS(64);
  int i;
  EDITCONTROL ec;
  if (cmd==TI_CMD_DESTROY)
  {
    i=EDIT_GetFocus(data);
  }

  if (cmd==TI_CMD_CREATE)
  {
    int need_to_jump=(int)EDIT_GetUserPointer(data);
    EDIT_SetFocus(data,need_to_jump);
  }

  if (cmd==TI_CMD_FOCUS)
    DisableIDLETMR();

  if (cmd==TI_CMD_REDRAW)
  {
    i=EDIT_GetFocus(data)/2-1;

    ExtractEditControl(data,i,&ec);

    wsprintf(ews,"%02X,%02X,%02X,%02X", Colors[i*4+0], Colors[i*4+1], Colors[i*4+2], Colors[i*4+3]);
    EDIT_SetTextToFocused(data,ews);

    if(leng==0)
    {
      SetSoftKey(data,&ok,SET_SOFT_KEY_N);
      SetSoftKey(data,&close,!SET_SOFT_KEY_N);
    }else
    {
      SetSoftKey(data,&ok1,SET_SOFT_KEY_N);
      SetSoftKey(data,&close1,!SET_SOFT_KEY_N);
    }
  }
  FreeWS(ews);
}

void ed_locret(void){}

HEADER_DESC ED_HDR={0,0,0,0,NULL,(int)"Редактор",LGP_NULL};
INPUTDIA_DESC ED_DESC=
{
	1,
	edOnKey,
	edGHook,
	(void *)ed_locret,
	0,
	&menu_skt,
	{0,0,0,0},
	FONT_SMALL,
	100,
	101,
	0,
	0,
	0x40000000
};

void AddColorStr(void *eq, char *name, int index)
{
  void *ma=malloc_adr();
  EDITCONTROL ec;
  WSHDR *ews=AllocWS(64);

  wsprintf(ews,_tc, index+1, name);
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ews,64);
  AddEditControlToEditQend(eq,&ec,ma);

  wsprintf(ews,"%02X,%02X,%02X,%02X", Colors[index*4+0], Colors[index*4+1], Colors[index*4+2], Colors[index*4+3]);
  ConstructEditControl(&ec,ECT_LINK,ECF_APPEND_EOL,ews,12);
  AddEditControlToEditQend(eq,&ec,ma);

  FreeWS(ews);
}

int ListCreate()
{
  void *ma=malloc_adr();
  void *eq=AllocEQueue(ma,mfree_adr());

  for(int i=0; i<(clMAX+1); i++)
    AddColorStr(eq, name[i+leng], i);

  patch_header(&ED_HDR);
  patch_input(&ED_DESC);

  return (CreateInputTextDialog(&ED_DESC, &ED_HDR, eq, 1, 0));
}

int GetLeng()
{
  int id;
  SettingsAE_Read(&id,4,"DISPLAY_LANGUAGE","CURRENT_LANGUAGE");
  if(id > 128) id-=128; //если выбран автоматический режим
  if(id==45 || id==34)
    return 0;
  else
    return clMAX+1;
}

static void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_CSM*csm=(MAIN_CSM*)data;
  leng=GetLeng();
  my_csm_id=csm->gui_id=ListCreate();
}

extern void kill_data(void *p, void (*func_p)(void *));
void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

static void maincsm_onclose(CSM_RAM *csm)
{
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"ColorMap Edit");
}

int main(char *exename, char *fname)
{
  MAIN_CSM main_csm;
  LockSched();
  if(fname!='\0')
  {
    strcpy(filename, fname);
    res=LoadCS(fname);
    if(!res) ShowMSG(1,(int)"Нет файла!!!");

    UpdateCSMname();
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();
    return 0;
  }else
  {
    ShowMSG(1,(int)"Нет файла!!!");
    UnlockSched();
    return 1;
  }

}
