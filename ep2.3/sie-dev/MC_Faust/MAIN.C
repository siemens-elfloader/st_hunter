#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\lng.h"
#include "inc\ColorMap.h"
#include "inc\config.h"
#include "inc\exts.h"
#include "inc\gui.h"
#include "inc\zslib.h"
#include "inc\newmenu.h"
#include "inc\ScreenMake.h"

int selmode=0;

NativeExplorerData* NatExpStr;

const int minus11=-11; // стремная константа
unsigned short maincsm_name_body[140];
volatile int GUIStarted = 0; //Elf вызван
volatile int Terminate = 0; // флаг необходимости завершения работы
volatile int Busy = 0;
volatile int Busy1 = 0;

unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;

int MsgBoxResult = -1;

unsigned int err;

#ifdef LOG
int flog=-1;
#endif

WSHDR *guibuf;
WSHDR *progr_name;
WSHDR *wsbuf;
char* pathbuf;
char* pathbuf2;
char* pathbuf_fn;
char* msgbuf;
char* szLastNewFile;
char* szLastNewDir;
char* szLastNewArchive;
char* zippathbuf;
FN_LIST tmp_files;

char *conf_name;

typedef struct
{
 CSM_RAM csm;
 int gui_id;
} MAIN_CSM;

typedef struct
{
 GUI gui;
} MAIN_GUI;

void FreeData()
{
 LockSched();
 if(selmode!=0) GBS_SendMessage(MMI_CEPID,MSG_CSM_DESTROYED,0,9999);
 GBS_DelTimer(&sctm);
 GBS_DelTimer(&offtm);
 
 freemenu();
 main_gui_free();
 ClearScreen();
 
 if(CONFIG_LOAD_CS) SaveCS(NULL);
 
#ifdef DUMP_MUI
 void SaveMUI2(char* filename);
 SaveMUI2(NULL);
#else
 if (CONFIG_LOAD_MUI) SaveMUI(NULL);
#endif
 SaveCfg();
 fn_free(&buffer);
 FreeMUI();
 FreeExt();
 
 FreeInfo();
 
 if(CONFIG_FREE_PNG_CACHE_ON_CLOSE)
  FreeGrap();
 
 if (zippathbuf) mfree(zippathbuf);
 if (szLastNewDir) mfree(szLastNewDir);
 if (szLastNewFile) mfree(szLastNewFile);
 if (msgbuf) mfree(msgbuf);
 if (pathbuf_fn) mfree(pathbuf_fn);
 if (pathbuf2) mfree(pathbuf2);
 if (pathbuf) mfree(pathbuf);
 if (wsbuf) FreeWS(wsbuf);
 if (guibuf) FreeWS(guibuf);
 if (progr_name) FreeWS(progr_name); 
 guibuf = wsbuf = 0;
 zippathbuf = pathbuf = pathbuf2 = pathbuf_fn = msgbuf = 0;
 for(int ii=0; ii<MAX_TABS+1; ii++)
 {
  DelFiles(ii);
  FreeTab(ii);
 }
 UnlockSched();

 
#ifdef LOG
 _WriteLog("FreeData");
 fclose(flog,&err);
#endif
}

void PeReDraw()
{
  LockSched();
   if (Busy1)
   {
     if (filepropshow){ DrwFileProp();goto END;}
     if (showmenu){     MenuVide();goto END;}
     if (drwinfo){      DrvInfo();goto END;}
     if (showabout){    _AboutDLG();goto END;}
     if (disks_opt){    DrawDisks();goto END;}
     if (smsgopen){     DrawShowMsg();goto END;}
     if (msgopen){      DrawMsgBox();goto END;}
#ifdef NEWSGOLD
     if (arch){         DrawArch();goto END;}
#endif
     if (seach){        DrawSeach();goto END;}
   }else
   {
    if (progr_start)
      ShowProgr();
    else 
      ShowFiles();
   } 
END:
   UnlockSched();
}

void OnRedraw(MAIN_GUI *data) // OnRedraw
{
 if (!GUIStarted)
 {   
 progr_name=AllocWS(MAX_PATH);
 guibuf=AllocWS(MAX_PATH*2);
 wsbuf=AllocWS(MAX_PATH*2);
 pathbuf=malloc(MAX_PATH);
 pathbuf2=malloc(MAX_PATH);
 pathbuf_fn=malloc(MAX_PATH);
 msgbuf=malloc(MAX_PATH);
 szLastNewFile=malloc(MAX_PATH);
 szLastNewDir=malloc(MAX_PATH);
 zippathbuf=malloc(MAX_PATH);
 *szLastNewFile = '\0';
 *szLastNewDir = '\0';
 
 crtmenu();
 
 LoadKeys();
#ifdef LOG
 _WriteLog("LoadExts");
#endif
 LoadExts();
#ifdef LOG
 _WriteLog("LoadCfg");
#endif
 LoadCfg();

 for(int ii=0; ii < MAX_TABS+1; ii++)
  InitTab(ii);

#ifdef LOG
  _WriteLog("Init Last Dirs");
#endif
  for(int ii=0;ii<MAX_TABS;ii++)
  {
    int srt=MCConfig.tabs[ii].sort;
    int srtH=(srt & STD_MASK);
    int srtL=(srt & STV_MASK);
    srt=(srtL<=ST_LAST?srtL:0) | srtH;
    tabs[ii]->sort=srt;
    if (MCConfig.tabs[ii].LastPath[0] && CONFIG_SAVE_PATH)
    {
      cd(ii, MCConfig.tabs[ii].LastPath);
      SetTabIndex(ii, MCConfig.tabs[ii].LastInd, 0);
    }else
      SetTabIndex(ii, 0, 0);
  }
  if (CONFIG_SAVE_PATH) curtab = MCConfig.curtab;
#ifdef LOG
 _WriteLog("InitCS");
#endif
 InitScr();
 FillInfo();
 InitCS();
 InitAllMD();
 if (CONFIG_LOAD_MUI)
  LoadMUI(NULL);
 else
  InitMUI();
#ifdef LOG
 _WriteLog("Work...");
#endif
 
 if (CONFIG_SCROLL_TEXT_SPEED)
  GBS_StartTimerProc(&sctm,CONFIG_SCROLL_TEXT_SPEED,DrwName);

 GUIStarted = 1;

 if (in_open_path[0])
 {
   if(isdir(in_open_path, &err))
   {
    cd(curtab = 0, in_open_path);
   }
   else
   {
     char* sz = GetFileExt(in_open_path);
     char szext[MAX_EXT];
     strtolower(sz, szext, MAX_EXT);
     
     if (!strncmp(szext, mcbm_ext, MAX_EXT))
     {
        UseBM(in_open_path);
     }
     else
     {
       cd(curtab = 0, GetFileDir(in_open_path, pathbuf));
       int idx = GetCurTabFileIndex(GetFileName(in_open_path));
       SetCurTabIndex(idx, 0);
     
       int IsArh;
       if((IsArh=IsItArh(in_open_path))!=0)
       {
         strcpy(zippathbuf, in_open_path);
         if(IsArh==1)   
           SUBPROC((void*)S_ZipOpen);
         if(IsArh==2) 
           SUBPROC((void*)S_7ZOpen);
       }
     }
   }
 }
 }
 PeReDraw();
}

void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int)) //Create
{
#ifdef LOG
 _WriteLog("gui.onCreate");
#endif
 data->gui.state = 1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *)) //Close
{
#ifdef LOG
 _WriteLog("gui.onClose");
#endif
 data->gui.state = 0;
 Terminate = progr_stop = 1; 
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))//Focus
{
 if (ext_pnglist != NULL)
 {
  PNGTOP_DESC* pltop = PNG_TOP();
  pltop->dyn_pltop = &ext_pnglist->dpl;
 }
 data->gui.state = 2;
/* 
#ifdef ELKA
 DisableIconBar(!iconbar);
 if (iconbar) GBS_SendMessage(0x4209, 0x642C, 0, 0, 0);
 DisableIconBar(0);
 //if (iconbar) GBS_SendMessage(0x4209, 0x642C, 0, 0, 0);
#endif
*/ 
  if (Terminate && !Busy)
    GeneralFuncF1(1); // рубим верхний гуй. И приложение закрывается.
  else
    PeReDraw();
 
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *)) //Unfocus
{
 PNGTOP_DESC* pltop = PNG_TOP();
 pltop->dyn_pltop = NULL;

#ifdef ELKA
 DisableIconBar(0);
#endif
 
 if (data->gui.state != 2) return; 
 data->gui.state = 1;
}

void CB_Stop(int id)
{if (id==IDYES) progr_stop = 1;}

int lastKey = -1;
int lastIsLongPress = 0;
int OnKey(MAIN_GUI *data, GUI_MSG *msg) //OnKey
{
 if (GUIStarted) // Обрабатываем только после полной инициализации
 {
 // Для сброса счетчика таймера автовыхода отслеживаем только клавиши
 ResetAutoExit();

 short key = msg->gbsmsg->submess;
 int isLongPress = 0;
 int ignore = 1;

 if (progr_start)
 {
  if (msg->gbsmsg->msg == KEY_DOWN)
  {
    if (!msgopen)
    {
      if(key==RIGHT_SOFT)
        DoMsgBox(muitxt(ind_pmt_stop), CB_Stop);
    }else
      DoMsgBoxKey(key);
  }
 }
 else if (!Busy)
 {
  if (GetKeyprocLongByKey(key) == NULL)
  {
  // Не нужна обработка двойного нажатия
  // Обрабатываем только KEY_DOWN и LONG_PRESS
  ignore = (msg->gbsmsg->msg != KEY_DOWN && msg->gbsmsg->msg != LONG_PRESS);
  }
  else
  {
  isLongPress = (msg->gbsmsg->msg == LONG_PRESS);

  // Игнорим KEY_UP если уже обработали долгое нажатие этой же кнопки
  // Обрабатываем только KEY_UP и LONG_PRESS
  ignore = (lastKey == key && lastIsLongPress && msg->gbsmsg->msg == KEY_UP)
   || (msg->gbsmsg->msg != KEY_UP && msg->gbsmsg->msg != LONG_PRESS);
  }
 }

 if (!ignore)
 {
   if (Busy1)
   { 
     if (drwinfo)      {DPROP_key(key);     goto END;}
     if (filepropshow) {fileprop_key(key);  goto END;}
     if (showabout)    {DoAboutKey(key);    goto END;}
     if (smsgopen)     {DoShowMsgKey(key);  goto END;}
     if (msgopen)      {DoMsgBoxKey(key);   goto END;}
     if (disks_opt)    {DoDisksKey(key);    goto END;}
     if (showmenu)     {menu_key(key);      goto END;}
#ifdef NEWSGOLD
     if (arch)         {Arch_key(key);      goto END;}
#endif
     if (seach)        {Seach_key(key);     goto END;}
   }
   else
     DoKey(isLongPress, key);
END:
  PeReDraw();
 }
 lastKey = key;
 lastIsLongPress = isLongPress;
 }

 return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

void Killer(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

int method8(void){return(0);}

int method9(void){return(0);}

const void * const gui_methods[11]={
 (void *)OnRedraw, //Redraw
 (void *)onCreate, //Create
 (void *)onClose, //Close
 (void *)onFocus, //Focus
 (void *)onUnfocus, //Unfocus
 (void *)OnKey, //OnKey
 0,
 (void *)kill_data, //Destroy
 (void *)method8,
 (void *)method9,
 0
};

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
 rc->x=x;
 rc->y=y;
 rc->x2=x2;
 rc->y2=y2;
}
const RECT Canvas={0,0,0,0};

void maincsm_oncreate(CSM_RAM *data)
{
#ifdef LOG
 _WriteLog("maincsm_oncreate");
#endif
 MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
 MAIN_CSM *csm=(MAIN_CSM*)data;
 zeromem(main_gui,sizeof(MAIN_GUI));
 patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
 main_gui->gui.canvas=(void *)(&Canvas);
 main_gui->gui.flag30=2;
 main_gui->gui.methods=(void *)gui_methods;
 main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
 csm->csm.state=0;
 csm->csm.unk1=0;
 csm->gui_id=CreateGUI(main_gui);
 MAINGUI_ID=csm->gui_id;
}

void maincsm_onclose(CSM_RAM *csm)
{
#ifdef LOG
 _WriteLog("maincsm_onclose");
#endif 
 FreeData();
 SUBPROC((void *)Killer);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
 MAIN_CSM *csm=(MAIN_CSM*)data;
 if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
 {
 csm->csm.state=-3;
 }
 else
 {
 if (msg->msg == MSG_IDLETMR_EXPIRED)
 {
  return(0);
 }
 else
 {
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    if (!strcmp(successed_config_filename,(char *)msg->data0))
    {
      InitConfig();
      crtmenu();
      menugrafic();
      UpdateAll();
      if(!IsTimerProc(&offtm))
        AutoExitProc();
      ResetAutoExit();
    }
  }
 }
 }
 
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if(ipc=(IPC_REQ*)msg->data0)
    {
      if (strcmp(ipc->name_to,MYNAME)==0)
      {
        switch (msg->submess)
        {
          case IPC_SetCur:
            if(strcmp((char*)ipc->data,CurPath())==0)
            {
              int ind = GetCurTabFileIndex((char*)ipc->name_from);
              SetCurTabIndex(ind, 0);
            }
            break;
          case IPC_UPD_KEY:
              LoadKeys();
            break; 
          case IPC_UPD_ALL:
              UpdateAll();
            break;   
        }
      }
    }
  }
 return(1);
}

const struct
{
 CSM_DESC maincsm;
 WSHDR maincsm_name;
} MAINCSM =
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
 if (wsbuf)
 {
    str_2ws(wsbuf,CurPath(scfile->sname),MAX_PATH);
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"MC - %w", wsbuf);  
 }
}

void GetName(char *exename)
{
  conf_name=strrchr(exename,'\\')+1;
  int i;
  for(i=strlen(conf_name); conf_name[i]!='.';i--)
    conf_name[i]=0;
  conf_name[i]=0;
}

int main(char *exename, char* fname)
{
 guibuf=wsbuf=0;
 pathbuf=pathbuf2=pathbuf_fn=msgbuf=0;

 GetFileDir(exename, mcpath);
 GetName(exename);
#ifdef LOG
 char buf[MAX_PATH];
 pathbuf = &buf[0];
 flog = fopen(MCFilePath("mc.log"),A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&err);
 pathbuf = 0;
 _WriteLog("Start");
#endif

#ifdef LOG
 _WriteLog("InitConfig");
#endif
 InitConfig();
 AutoExitProc();
 if (fname) 
 {   
   if((fname[0]>='0' && fname[0]<='4') && fname[1]==':')
   {
    strcpy(in_open_path, fname);
   }else
   {
     selmode=1;
     NatExpStr=(NativeExplorerData*)fname;
   }    
 }

 char dummy[sizeof(MAIN_CSM)];
 MAINCSM_ID = CreateCSM(&MAINCSM.maincsm, dummy, 0);
 UpdateCSMname();
 return 0;
}

#ifdef LOG
void _WriteLog(char *buf)
{
 if (flog!=-1)
 {
 char msg[512];

 TTime t;
 TDate d;
 GetDateTime(&d,&t);
 sprintf(msg, "%02d:%02d:%02d;Ram - %d; %s\n", t.hour,t.min,t.sec,GetFreeRamAvail(),buf);
 fwrite(flog,msg,strlen(msg),&err);
 }
}
void _WriteLogWS(WSHDR *buf)
{
 if (flog!=-1)
 {
 char msg[256];
/* char msg2[256];
 ws_2str(buf, msg2, 256);*/

 TTime t;
 TDate d;
 GetDateTime(&d,&t);
 sprintf(msg, "%02d:%02d:%02d;Ram - %d;  %w\n", t.hour,t.min,t.sec,GetFreeRamAvail(),buf);
 fwrite(flog,msg,strlen(msg),&err);
 }
}

void _WriteLog2(char *buf1, char *buf2)
{
 if (flog!=-1)
 {
 char msg[512];

 TTime t;
 TDate d;
 GetDateTime(&d,&t);
 sprintf(msg, "%02d:%02d:%02d;Ram - %d; %s %s\n", t.hour,t.min,t.sec,GetFreeRamAvail(),buf1,buf2);
 fwrite(flog,msg,strlen(msg),&err);
 }
}

void _WriteLog3(char *buf1, char *buf2)
{
 if (flog!=-1)
 {
 char msg[512];

 TTime t;
 TDate d;
 GetDateTime(&d,&t);
 sprintf(msg, "%02d:%02d:%02d;Ram - %d; %s%s\n", t.hour,t.min,t.sec,GetFreeRamAvail(),buf1,buf2);
 fwrite(flog,msg,strlen(msg),&err);
 }
}

void _WriteLogInt(char *buf, int ii)
{
 if (flog!=-1)
 {
 char msg[512];
 TTime t;
 TDate d;
 GetDateTime(&d,&t);
 sprintf(msg, "%02d:%02d:%02d;Ram - %d; %s %d\n", t.hour,t.min,t.sec,GetFreeRamAvail(),buf,ii);
 fwrite(flog,msg,strlen(msg),&err);
 }
}
void _WriteLogHex(char *buf, int ii)
{
 if (flog!=-1)
 {
 char msg[512];
 TTime t;
 TDate d;
 GetDateTime(&d,&t);
 sprintf(msg, "%02d:%02d:%02d;Ram - %d; %s %.8x\n", t.hour,t.min,t.sec,GetFreeRamAvail(),buf,ii);
 fwrite(flog,msg,strlen(msg),&err);
 }
}
#endif
