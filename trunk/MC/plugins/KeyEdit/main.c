#include "../../../inc/swilib.h"

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;
int my_csm_id=0;

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

int leng=0;

int isElka()
{
  if (ScreenW()==132 && ScreenH()==176)
    return 0;
  if (ScreenW()==240 && ScreenH()==320)
    return 24;

  return -1;
}

const char _t[] = "%t";

int cur[46];
char filename[128];

typedef struct
{
  char act[16];
  char nameR[64];
  char nameE[64];
}ACTIONS;

ACTIONS Keys[]=
{
  	{"lsoft", "Левый софт", "Left soft"},
        {"lsoft+", "Левый софт(LP)", "left soft(LP)"},

	{"rsoft", "Правый софт", "Right soft"},
        {"rsoft+", "Правый софт(LP)", "Правый софт(LP)"},

	{"green", "Зеленая", "Green"},
        {"green+", "Зеленая(LP)", "Green(LP)"},

	{"red", "Красная", "Red"},
        {"red+", "Красная(LP)", "Red(LP)"},

	{"enter", "Энтер", "Enter"},
        {"enter+", "Энтер(LP)", "Enter(LP)"},

	{"up", "Вверх", "Up(LP)"},
        {"up+", "Вверх(LP)", "Up(LP)"},

	{"down", "Вниз", "Down"},
        {"down+", "Вниз(LP)", "Down(LP)"},

	{"left", "Влево", "Left"},
        {"left+", "Влево(LP)", "Left(LP)"},

	{"right", "Вправо", "Right"},
        {"right+", "Вправо(LP)", "Right(LP)"},

	{"volup", "Громкость +", "Loudness +"},
        {"volup+", "Громкость +(LP)", "Loudness +(LP)"},

	{"voldown", "Громкость -", "Loudness -"},
        {"voldown+", "Громкость -(LP)", "Loudness -(LP)"},

	{"0", "0", "0"},
        {"0+", "0(LP)", "0(LP)"},

	{"1", "1", "1"},
        {"1+", "1(LP)", "1(LP)"},

	{"2", "2", "2"},
        {"2+", "2(LP)", "2(LP)"},

	{"3", "3", "3"},
        {"3+", "3(LP)", "3(LP)"},

	{"4", "4", "4"},
        {"4+", "4(LP)", "4(LP)"},

	{"5", "5", "5"},
        {"5+", "5(LP)", "5(LP)"},

	{"6", "6", "6"},
        {"6+", "6(LP)", "6(LP)"},

	{"7", "7", "7"},
        {"7+", "7(LP)", "7(LP)"},

	{"8", "8", "8"},
        {"8+", "8(LP)", "8(LP)"},

	{"9", "9", "9"},
        {"9+", "9(LP)", "9(LP)"},

	{"*", "*", "*"},
        {"*+", "*(LP)", "*(LP)"},

	{"#", "#", "#"},
        {"#+", "#(LP)", "#(LP)"},
};

int proc[] =
{
  	11,//{"lsoft", "Левый софт"},
        NULL,//{"lsoft+", "Левый софт(LP)"},

	3,//{"rsoft", "Правый софт"},
        NULL,//{"rsoft+", "Правый софт(LP)"},

	44,//{"green", "Зеленая"},
        NULL,//{"green+", "Зеленая(LP)"},

	NULL,//{"red", "Красная"},
        NULL,//{"red+", "Красная(LP)"},

	2,//{"enter", "Энтер"},
        NULL,//{"enter+", "Энтер(LP)"},

	21,//{"up", "Вверх"},
        NULL,//{"up+", "Вверх(LP)"},

	22,//{"down", "Вниз"},
        NULL,//{"down+", "Вниз(LP)"},

	15,//{"left", "Влево"},
        NULL,//{"left+", "Влево(LP)"},

	16,//{"right", "Вправо"},
        NULL,//{"right+", "Вправо(LP)"},

	NULL,//{"volup", "Громкость +"},
        NULL,//{"volup+", "Громкость +(LP)"},

	NULL,//{"voldown", "Громкость -"},
        NULL,//{"voldown+", "Громкость -(LP)"},

	NULL,//{"0", "0"},
        NULL,//{"0+", "0(LP)"},

	14,//{"1", "1"},
        NULL,//{"1+", "1(LP)"},

	21,//{"2", "2"},
        NULL,//{"2+", "2(LP)"},

	23,//{"3", "3"},
        NULL,//{"3+", "3(LP)"},

	17,//{"4", "4"},
        NULL,//{"4+", "4(LP)"},

	42,//{"5", "5"},
        NULL,//{"5+", "5(LP)"},

	16,//{"6", "6"},
        NULL,//{"6+", "6(LP)"},

	NULL,//{"7", "7"},
        NULL,//{"7+", "7(LP)"},

	22,//{"8", "8"},
        NULL,//{"8+", "8(LP)"},

	24,//{"9", "9"},
        NULL,//{"9+", "9(LP)"},

	NULL,//{"*", "*"},
        NULL,//{"*+", "*(LP)"},

	45,//{"#", "#"},
        NULL,//{"#+", "#(LP)"},
};

const ACTIONS Items[]=
{
        {"", "Действия", "Action"},           //0
	{"none", "Нет действия", "No action"},//1
        {"none", "Пусто", "Empty"},//2
	{"open", "Открыть", "Open"},//3
	{"back", "Назад", "Back"},//4
	{"rename", "Переименовать", "Rename"},//5
	{"past", "Вставить", "Insert"},//6
	{"copy", "Копировать", "Copy"},//7
	{"move", "Переместить", "Move"},//8
	{"del", "Удалить", "Remove"},//9
	{"cancel", "Отмена", "Cancel"},//10
	{"newdir", "Новая папка", "New folder"},//11
	{"menu", "Меню", "Menu"},//12
        {"fileprop", "Атрибуты", "Attribute"},//13
	{"drvinf", "Диски", "Disks"},//14
	{"swaptab", "Перекл. таб", "Switch Tab"},//15
	{"prvdrv", "Пред. диск", "Previous disk"},//16
	{"nxtdrv", "След. диск", "Next disk"},//17
	{"invchk", "Инвертировать выделение", "Invert allocation"},//18
	{"chk", "Выделить", "Distinguish"},//19
	{"chkall", "Выделить все", "Distinguish all"},//20
	{"unchall", "Сбросить все", "Reset all"},//21
	{"up", "Вверх", "Up"},//22
	{"dwn", "Вниз", "Down"},//23
	{"pgup", "Страница вверх", "Page up"},//24
	{"pgdn", "Страница вниз", "Page down"},//25
	{"bmlist", "Закладки", "Bookmark"},//26
	{"bmadd", "Добавить закладку", "Add bookmark"},//27
	{"filter", "Фильтр", "Filter"},//28
	{"refresh", "Обновить", "Update"},//29
	{"root", "В корень", "At the root"},//30
	{"exit", "Выход", "Exit"},//31
	{"showpath", "Отобразить путь", "show the way"},//32
	{"begin", "В начало", "Top"},//33
	{"sortn", "Сорт. по имени", "Sort by name"},//34
	{"sorte", "Сорт. по расширению", "Sort by extension"},//35
	{"sorts", "Сорт. по размеру", "Sort by size"},//36
	{"sortd", "Сорт. по дате", "Sort by Date"},//37
	{"sortr", "Сорт. обратная", "Inverse sorting"},//38
	{"setsort", "Смена сортировки", "Change sorting"},//39
	{"newfile", "Новый файл", "New file"},//40
	{"tabcopy", "tabcopy", "tabcopy"},//41
	{"tabmove", "tabmove", "tabmove"},//42
        {"altopen", "Открыть с ...", "Open with ..."},//43
        {"natexpl", "Проводник", "Explorer"},//44
        {"sent", "Передать", "Send"},//45
        {"disks", "Отображение", "Mapping"},//46
        {"getff", "Слить FF", "Backup FF"},//47
        {"crtarh", "Заархивировать", "Archive"},//48
        {"crtpl", "Создать плейлист", "Create playlist"},//49
        {"unpack", "Распаковать", "Unpack"},//50
        {"md5", "MD5", "MD5"},//51
        {"getlng", "Получить ленг", "Get Leng"}//52
};

#define ActNum 53
void SaveCFG()
{
  int f;
  unsigned int err;
  int i=0;
  char buff[128];
  if ((f=_open(filename,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err))!=-1)
  {
    while(i<46)
    {
      if(cur[i]>1 && cur[i]!=(proc[i]+1))
      {
        sprintf(buff,"%s=%s\r\n",Keys[i].act,Items[cur[i]].act);
        _write(f,buff,strlen(buff),&err);
      }
      i++;
    }
    _close(f,&err);
  }
}

char* strtolower(char* src, char* dst, int sz)
{
 int sl = strlen(src)+1;
 int len = sz==-1?sl:sz;
 if (len>sl) len=sl;

 for(int ii=0; ii<len-1; ii++)
 {
  int ch = src[ii];

  if (ch>='A' && ch<='Z') ch=ch-'A'+'a';

  dst[ii]=ch;
 }
 dst[len-1]=0;
 return dst;
}

void SetCur(char *name,char *value)
{
  int i=0;
  int j=0;

  strtolower(name, name, -1);
  strtolower(value, value, -1);

  for(int cc=0; cc < 46; cc++)
  {
    if (strcmp(Keys[cc].act, name)==0)
    {
      i = cc;
      break;
    }
  }

  for(int cc = 2; cc < ActNum; cc++)
  {
    if (strcmp(Items[cc].act, value)==0)
    {
      j = cc;
      break;
    }
  }

  cur[i]=j-1;
}

int LoadCFG(char *ininame)
{
 for(int cc=0; cc < 46; cc++)
  cur[cc]=proc[cc];

 int res=0;
 char *buf;
 unsigned int err;
 char *cfg;
 int ch;
 int p=0;
 char name[16];
 char value[16];
 unsigned int size_cfg;

 int f;
 if ((f = _open(ininame, A_ReadOnly, 0, &err)) != -1)
 {
  size_cfg = _lseek(f,0,S_END,&err,&err);
  _lseek(f, 0, S_SET, &err, &err);
  cfg = buf = malloc(size_cfg + 1);
  if (cfg)
  {
   size_cfg = _read(f, buf, size_cfg, &err);
   buf[size_cfg] = 0;
   do
   {
    // Камент
    if (*buf == ';')
    {
     while( ((ch = *buf++) >= 32));
     if (!ch) break;
     else continue;
    }

    // Название
    p=0;
    while( (ch = *buf++) != '=')
    {
     if (ch < 32) break;
     if (p < 16) name[p++] = ch;
    }
    if (ch < 32) continue;
    name[p]=0;

    // Значение
    p=0;
    while( (ch = *buf++) >= 32 )
    {
     if (p < 16) value[p++] = ch;
    }
    value[p]=0;

    SetCur(name, value);
   }
   while (ch);
  }
  mfree(cfg);
  _close(f,&err);
  res = 1;
 }
 return res;
}

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
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    int key=msg->gbsmsg->submess;
    int i=EDIT_GetFocus(data)/2-1;

    if(key==GREEN_BUTTON || key==LEFT_SOFT)
      SaveCFG();
  }

  if (msg->keys==0xFFF)  // OK
    return 1;

  if (msg->keys==0xFFE)  // Back
    return (0xFF);
  return 0;
}

void edGHook(GUI *data, int cmd)
{
  static SOFTKEY_DESC ok={0x0FFF,0x0000,(int)"Ok"};
  static SOFTKEY_DESC close={0x0FFE,0x0000,(int)"Выход"};
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
  {
    DisableIDLETMR();
  }
  if (cmd==TI_CMD_REDRAW)
  {
    i=EDIT_GetFocus(data)/2-1;

    SetSoftKey(data,&ok,SET_SOFT_KEY_N);
    SetSoftKey(data,&close,!SET_SOFT_KEY_N);
  }

  if(cmd == 0x0D) //combo
  {
    int j;

    WSHDR* buff = AllocWS(64);

    ExtractEditControl(data, i, &ec);
    j = EDIT_GetItemNumInFocusedComboBox(data);

    cur[i=EDIT_GetFocus(data)/2-1]=j;
    if(leng==0)
      wsprintf(buff, _t, Items[j].nameR);
    else
      wsprintf(buff, _t, Items[j].nameE);


    EDIT_SetTextToFocused(data, buff);
    FreeWS(buff);
  }

  FreeWS(ews);
}

void ed_locret(void){}

HEADER_DESC ED_HDR={0,0,0,0,NULL,(int)"Раскладка",LGP_NULL};
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

void AddComboBox(void *eq, const char *name, int cursor)
{
  void *ma=malloc_adr();
  EDITCONTROL ec;
  WSHDR *ews=AllocWS(64);

  wsprintf(ews,"%t",name);
  ConstructComboBox(&ec,7,ECF_APPEND_EOL,ews,32,0,ActNum-1,cursor+1);
  AddEditControlToEditQend(eq,&ec,ma);

  FreeWS(ews);
}

void AddStr(void *eq, char *name, int id)
{
  void *ma=malloc_adr();
  EDITCONTROL ec;
  WSHDR *ews=AllocWS(64);

  wsprintf(ews,"%d. %t:\n", id+1, name);
  ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,ews,64);
  AddEditControlToEditQend(eq,&ec,ma);;

  FreeWS(ews);
}

int ListCreate()
{
  void *ma=malloc_adr();
  void *eq=AllocEQueue(ma,mfree_adr());
  int i=0;
  while(i<46)
  {
    if(leng==0)
    {
      AddStr(eq, Keys[i].nameR, i);
      AddComboBox(eq, Items[cur[i]].nameR, cur[i]);
    }else
    {
      AddStr(eq, Keys[i].nameE, i);
      AddComboBox(eq, Items[cur[i]].nameE, cur[i]);
    }
    i++;
  }

  patch_header(&ED_HDR);
  patch_input(&ED_DESC);
  return (CreateInputTextDialog(&ED_DESC, &ED_HDR, eq, 1, 0));
}

void GetLeng()
{
  int id;
  SettingsAE_Read(&id,4,"DISPLAY_LANGUAGE","CURRENT_LANGUAGE");
  if(id > 128) id-=128; //если выбран автоматический режим
  if(id==45 || id==34)
    leng = 0;
  else
    leng = 1;
}

static void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_CSM*csm=(MAIN_CSM*)data;
  GetLeng();
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"KeyEdit");
}

int main(char *exename, char *fname)
{
  MAIN_CSM main_csm;
  LockSched();
  UpdateCSMname();
  strcpy(filename, fname);
  if(LoadCFG(fname)==1)
  {
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();
    return 0;
  }else
    ShowMSG(1,(int)"Нет файла!!!");
  UnlockSched();
  return 1;
}
