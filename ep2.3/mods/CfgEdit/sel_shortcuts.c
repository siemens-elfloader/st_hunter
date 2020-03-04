#include "..\inc\swilib.h"
#include "rect_patcher.h"

#ifdef NEWSGOLD
  const char SHORTCUTS_FILE[]="4:\\zbin\\utilities\\shortcuts.txt";
#else
  const char SHORTCUTS_FILE[]="0:\\zbin\\utilities\\shortcuts.txt";
#endif

int NumOfItems=0;

char *templates_chars; //Собственно файл
char **templates_lines; //Массив указателей на строки

void FreeLines(void)
{
  if (templates_lines) mfree(templates_lines);
  if (templates_chars) mfree(templates_chars);
  templates_lines=NULL;
  templates_chars=NULL;
}

int LoadLines(void)
{
  FSTATS stat;
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p, *pp;
  int c;
  if (GetFileStats(SHORTCUTS_FILE,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if ((f=fopen(SHORTCUTS_FILE,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;

  p=templates_chars=malloc(fsize+1);
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
	templates_lines=realloc(templates_lines,(i+1)*sizeof(char *));
	templates_lines[i++]=pp;
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

extern void ascii2ws(WSHDR *ws, const char *s);

HEADER_DESC ShortcutsMenuHeader={NULL,NULL,NULL,NULL,NULL,NULL,LGP_NULL};

void ShortcutsMenuItemHandler(void *data, int curitem, void *unk)
{
  void *item=AllocMenuItem(data);
  char *p=strrchr(templates_lines[curitem],';')+1;
  WSHDR *ws=AllocMenuWS(data,strlen(p));
  ascii2ws(ws,p);
  SetMenuItemText(data, item, ws, curitem);
}

void ShortcutsMenuGHook(void *data, int cmd)
{
  if (cmd==TI_CMD_FOCUS)
  {
    DisableIDLETMR();
  }
  if (cmd==TI_CMD_DESTROY)
  {
    FreeLines();
  }
}

static int ShortcutsMenuKeyHook(void *data, GUI_MSG *msg)
{
  int Selected=GetCurMenuItem(data);
  void *edgui=MenuGetUserPointer(data);
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    if (NumOfItems)
    {
      EDITCONTROL ec;
      char *str=malloc(128);
      const char *p=strrchr(templates_lines[Selected],';')+1;
      strncpy(str,templates_lines[Selected],p-templates_lines[Selected]-1); 
      str[p-templates_lines[Selected]-1]='\0';

      EDIT_ExtractFocusedControl(edgui,&ec);
      WSHDR *ws=AllocWS(ec.maxlen);
      str_2ws(ws,str,ec.maxlen);
      EDIT_SetTextToFocused(edgui,ws);
      
      mfree(str);
      FreeWS(ws);
      return(1);
    }
  }
  return(0);
}

const int MenuSoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};
const SOFTKEYSTAB Select_Back_skt=
{
  menu_sk2,0
};
static const MENU_DESC ShortcutsMenuStruct=
{
  8,ShortcutsMenuKeyHook,ShortcutsMenuGHook,NULL,
  MenuSoftkeys,
  &Select_Back_skt,
  0x10,
  ShortcutsMenuItemHandler,
  NULL, //menuitems,
  NULL, //menuprocs,
  NULL
};

int DispShortcutsMenu(void *ed_gui)
{
  NumOfItems=LoadLines();
  if(NumOfItems)
  {
    *((int *)(&ShortcutsMenuHeader.lgp_id))=(int)"Select shortcut";
    patch_header(&ShortcutsMenuHeader);
    return CreateMenu(0,0,&ShortcutsMenuStruct,&ShortcutsMenuHeader,0,NumOfItems,ed_gui,0);
  }
  MsgBoxError(1, (int)"File not found!");
  return(0);
}
