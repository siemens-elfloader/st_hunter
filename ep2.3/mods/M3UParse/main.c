#include "../inc/swilib.h"
#include "m3uparse.h"

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
int cnt=0;


/*
void SaveFile()
{
#ifdef LOG
  TTime t;
  char ss[256] = "";
  GetDateTime(0,&t);
  sprintf(ss,"saved %02d:%02d:%03d",t.min,t.sec,t.millisec);
  log(ss);
#endif
  M3U *list = m3utop;
  unsigned int ul;
  char s[256];
  zeromem(s,256);
//  int fsize = 0;
  unlink("4:\\m3u.txt",&ul);
  int f = fopen("4:\\m3u.txt",A_ReadWrite+A_Create+A_Append,P_READ+P_WRITE,&ul);
  if(f!=-1)
  {
    char *s3=malloc(128);
    char *s2=malloc(128);
    while(list)
    {
      ///sprintf(s,"%d. %s(%s) %02d:%02d\r\n",list->index+1,list->track,list->path,list->time.min,list->time.sec);
      ws_2str(list->track,s3,wstrlen(list->track));
      ws_2str(list->path,s2,wstrlen(list->path));
      sprintf(s,"%d. %s(%s) %02d:%02d\r\n",list->index+1,s3,s2,list->time.min,list->time.sec);
      fwrite(f,s,strlen(s),&ul);
      list = list->next;
    }
    mfree(s3);
    mfree(s2);
  }
  fclose(f,&ul);
  ShowMSG(1,(int)"File save");
  
#ifdef LOG
  GetDateTime(0,&t);
  sprintf(ss,"saved %02d:%02d:%03d",t.min,t.sec,t.millisec);
  log(ss);
#endif
}
*/


#pragma inline=forced
int toupper(int c)
{
  if((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmpNoCase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}

extern M3U *m3utop;

void MM_OnRedraw(MAIN_GUI *data)
{
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x64","\x00\x00\x00\x64");
  if(cnt && m3utop)
  {
    DrawString(m3utop->path,3,YDISP, ScreenW()-1,ScreenH()-1,FONT_SMALL,0,"\xFF\xFF\xFF\x64",GetPaletteAdrByColorIndex(23));  
    DrawString(m3utop->track,3,YDISP+50, ScreenW()-1,ScreenH()-1,FONT_SMALL,0,"\xFF\xFF\xFF\x64",GetPaletteAdrByColorIndex(23));      
  }
}


//------------------------------------------


void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
    DisableIconBar(1);
#endif
  data->gui.state=1;
}

void MM_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
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
  cnt = ParseM3UFile("4:\\playlist.m3u");  
}


//---------------------------------------------------------------------------//

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
  FreeM3UList();
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



void main(char *filename)
{
  MAIN_CSM main_csm;
//  InitConfig();
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();

  
#ifdef LOG1
  unsigned int ul;  
  unlink("4:\\m3udeb.txt",&ul);

  TTime t;
  GetDateTime(0,&t);
  char s[256] = "";
  sprintf(s,"start %02d:%02d:%03d",t.min,t.sec,t.millisec);
  log(s);
#endif

//  Parse("4:\\playlist.m3u");
//  SaveFile();
 // FreeBmkList();
}
