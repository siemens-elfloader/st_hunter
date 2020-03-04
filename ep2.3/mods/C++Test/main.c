#include "..\inc\swilib.h"
#include "local_ipc.h"
#include "../inc/xtask_ipc.h"
#include "main.h"
#include "language.h"
#include "print.h"
#include "File.h"

GBSTMR mytmr;
int menus_id;

const char ipc_my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ gipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

#define TMR_SECOND 216

void TimerProc(void)
{
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&gipc);
}

const char _s[]="%s";

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


void DrwImage(IMGHDR *img, int x, int y)
{
  if(!img) return;
  if(!strlen(img->bitmap))return;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0/*pen,brush*/);
  DrawObject(&drwobj);
}

//IMGHDR *img;

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

IMGHDR imgg = {0,0,8,""};

void MM_OnRedraw(MAIN_GUI *data)
{
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x64","\x00\x00\x00\x64");
  DrwImage(&imgg,0,0);
}


//------------------------------------------


void MM_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
    DisableIconBar(1);
#endif

  CFile file("4:\\1.img");
  if(!file.Size()
  {
    IMGHDR *img;
    img = CreateIMGHDRFromPngFile("4:\\1.png",0);  
    int size = strlen(img->bitmap);
    
    unsigned int io_error;
//  int hFile = fopen("4:\\1.img",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE, &io_error);
    file.Open(A_ReadWrite + A_Create + A_Append + A_BIN);
    if(hFile!=-1)
    {
      file.Write(img->bitmap, strlen(img->bitmap));
  //  fwrite(hFile, img->bitmap, size, &io_error);
    }
    file.Close();
    mfree(img);
  }
  else
  {
    unsigned int err;
    int f;
    f = fopen("4:\\1.img",A_ReadOnly + A_BIN,P_READ, &err);
    if(f!=-1)
    {
  //    unsigned int size = 0;
//      fread(f,&size,sizeof(unsigned int),&err);
    //  lseek(f,sizeof(unsigned int)+1,0,&err,&err);      
//      char *s = (char*)malloc(size);

      imgg.w = ScreenW();
      imgg.h = ScreenH();
      imgg.bpnum = 8;
      
      imgg.bitmap = malloc(ScreenW()*ScreenH()*2);//malloc(strlen(s));
      int i = fread(f, imgg.bitmap , get_file_size("4:\\1.img")/*size - (sizeof(unsigned int)+1)*/, &err);

      fclose(f, &err);
  //    char ss[64];
//      sprintf(ss,"Bytes %d size = %d",i,size);
    //  ShowMSG(1,(int)ss);
  //   ShowMSG(1,(int)"Read");
    }
  }
  
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


int main(char *exename, char *fname)
{ 
  MAIN_CSM main_csm;
//  InitConfig();
  LockSched();
  UpdateCSMname();
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();

  PrintLibInit(1);
  return 0;
}
