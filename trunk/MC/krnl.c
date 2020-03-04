#include "inc/mc.h"
#include "inc/mui.h"
#include "inc/lng.h"
#include "inc/ColorMap.h"
#include "inc/config.h"
#include "inc/zslib.h"
#include "inc/gui.h"
#include "inc/newmenu.h"
#include "inc/ScreenMake.h"
#include "inc/popup.h"

typedef int (*altrun)(WSHDR *filename, WSHDR *ext, void *param);
const char txt_ext[] = "txt";

char in_open_path[MAX_PATH] = "";
char mcpath[MAX_PATH];
int back_tab;

int progr_stop  = 0;
int progr_act   = 0;

int progr_max   = 0;
int progr_cur   = 0;
int progr_start = 0;

double progrsp_max = 0;
double progrsp_cur = 0;
int progrsp_start = 0;

int show_hidden = 0;
int show_system = 0;
int show_hiddrv = 0;

volatile int AutoExitCounter=0;

DRVINFO Drives[MAX_DRV] =
{
 {0, "0:", psz_drv_0, 1, 0},
 {1, "1:", psz_drv_1, 1, 0},
 {2, "2:", psz_drv_2, 1, 0},
 {4, "4:", psz_drv_4, 1, 0}
};

int curtab=0;
TABINFO* tabs[MAX_TABS+1];
FILEINF* FileListBase[MAX_TABS+1];

const char ext_file[] = "mcfileico.cfg";
const char keys_file[] = "mckeys.cfg";
char etc_path[] = DEFAULT_DISK":\\ZBin\\etc";

const char* def_filter = "*.*";
const char* def_new_file = ".txt";

char* str_empty = "";

GBSTMR offtm;

void SetTabIndex(int tab, int num, int slide)
{
 TABINFO *tmp = tabs[tab];

 if (tmp->ccFiles == 0)
 {
  num = -1;
 }
 else
 {
  if (slide)
  {
   if ( num >= tmp->ccFiles) num = 0;
   else if (num < 0) num = tmp->ccFiles - 1;
  }
  else
  {
   if (num >= tmp->ccFiles) num = tmp->ccFiles - 1;
   else if (num < 0) num = 0;
  }
 }
 tmp->iIndex[tmp->CurDrv] = num;
}

void SetCurTabIndex(int num, int slide)
{
 SetTabIndex(curtab, num, slide);
}

int GetTabIndex(int tab)
{
 return tabs[tab]->iIndex[ tabs[tab]->CurDrv ];
}

char* GetTabPath(int tab)
{
 if (IsArchiveOpened(tab))
  return tabs[tab]->zipInfo->szCurDir;
 else
  return (char*)&tabs[tab]->szDirs[tabs[tab]->CurDrv];
}

int file_inlist(FILEINF *list, char *name)
{
  if (list==NULL || !itms_max) return 0;

  FILEINF *itm = list;
  while(itm)
  {
    if (strstr(itm->sname, name)) return 1;
    itm=itm->next;
  }
  return 0;
}

void FillFileInfo(FILEINF *file)
{
 if (!file || file->inited) return;

 char buf[MAX_PATH];
 if (!file->ws_attr)
 {
  attr2s(file->attr, buf);
  file->ws_attr = AllocWS(strlen(buf)+1);
  str_2ws(file->ws_attr, buf, 64);
 }
 if (!file->ws_size)
 {
  sz2s(file->size, buf);
  file->ws_size = AllocWS(strlen(buf)+1);
  wsprintf(file->ws_size, _t, buf);
 }
 if (!file->ws_time)
 {
  fdt2s(file->time, buf);
  file->ws_time = AllocWS(strlen(buf)+1);
  str_2ws(file->ws_time, buf, 64);
 }
 if (!file->ext)
 {
    file->ext = GetExt(file->sname, file->attr);
 }
 if (file->ftype == TYPE_ZIP_FILE && !file->ws_ratio)
 {
  if (file->size != 0 && file->csize != 0)
  {
   float ratio = (float)file->csize * (float)100 / (float)file->size;
   sprintf(buf, "%.0f%%", ratio); // без цифр после точки
   file->ws_ratio = AllocWS(strlen(buf) + 1);
   str_2ws(file->ws_ratio, buf, 64);
  }
 }

 char* sext = GetFileExt(file->sname);
 if (sext && !file->ws_showname && ((stricmp(sext, (char*)mcbm_ext)==0) || !CONFIG_SHOW_EXT))
 {
  // Откидываем расширение
  strcpy(buf, file->sname);
  buf[sext - file->sname - 1] = '\0';
  file->ws_showname = AllocWS(strlen(buf) + 1);
  str_2ws(file->ws_showname, buf, MAX_PATH);
 }
 file->inited = 1;
}

FILEINF* CreateFileInfo(int findex, char* fnameOriginal,
    unsigned int fsize, short fattr, unsigned int ftime,
    int fcsize, int ftype, char* ffolderorg)
{
 FILEINF* file = malloc(sizeof(FILEINF));
 zeromem(file, sizeof(FILEINF));

 int len = strlen(fnameOriginal);
 char* fname = malloc(len + 1);
 strcpy(fname, fnameOriginal);

 WSHDR* wsname = AllocWS(len + 1);
 str_2ws(wsname, fname, len);

 len = strlen(ffolderorg);
 char* folder = malloc(len + 1);
 strcpy(folder, ffolderorg);

 file->id  = findex;
 file->sname  = fname;
 file->folder  = folder;
 file->attr  = fattr;
 file->size  = fsize;
 file->time  = ftime;
 file->ws_name = wsname;

 file->csize  = fcsize;
 file->ftype  = ftype;

 return file;
}

void AddFile(int tab, int findex, char* fname, unsigned int fsize, short fattr,
      unsigned int ftime, int fcsize, int ftype, char* ffolder)
{
 FILEINF* file = CreateFileInfo(findex, fname, fsize, fattr, ftime, fcsize, ftype, ffolder);

 LockSched();

 file->next = FileListBase[tab]->next;
 FileListBase[tab]->next = file;

 UnlockSched();
}

void AddFileFromDE(int tab, int findex, DIR_ENTRY* pde)
{
 AddFile(tab,
         findex,
         pde->file_name,
         pde->file_size,
         pde->file_attr,
         pde->create_date_time,
         0,
         (pde->file_attr & FA_DIRECTORY) ? TYPE_COMMON_DIR : TYPE_COMMON_FILE,
         pde->folder_name);
}

void FreeFileInfo(FILEINF* file)
{
 if (file)
 {
  if (file->ws_showname) FreeWS(file->ws_showname);
  if (file->ws_ratio) FreeWS(file->ws_ratio);
  if (file->ws_time) FreeWS(file->ws_time);
  if (file->ws_size) FreeWS(file->ws_size);
  if (file->ws_attr) FreeWS(file->ws_attr);
  if (file->ws_name) FreeWS(file->ws_name);
  if (file->sname) mfree(file->sname);
  if (file->folder) mfree(file->folder);
  mfree(file);
 }
}

void DelFiles(int tab)
{
 if (tabs[tab]->ccFiles)
 {
  LockSched();
  scfile=NULL;
  while(FileListBase[tab]->next!=FileListBase[tab])
  {
   FILEINF *file = FileListBase[tab]->next; // Второй элемент
   FileListBase[tab]->next = file->next;  // Следующий у FileListBase - на третий
   FreeFileInfo(file);
   tabs[tab]->ccFiles--;
  }
  UnlockSched();
 }
}


int FillRealPathFiles(int tab, char* dname)
{
 DIR_ENTRY de;
 int num = 0;

 if (pathbuf)
 {
  sprintf(pathbuf, _s_stars, dname);

  if (FindFirstFile(&de, pathbuf, &err))
  {
   do
   {
    if (((!tabs[tab]->szFilter[0] || de.file_attr & FA_DIRECTORY) && selmode!=2) || (selmode==2 && de.file_attr & FA_DIRECTORY))
    {
     if ( (show_hidden || !(de.file_attr & FA_HIDDEN))
      && (show_system || !(de.file_attr & FA_SYSTEM)) )
      AddFileFromDE(tab, num++, &de);
    }
   }
   while (FindNextFile(&de, &err));
#ifdef NEWSGOLD
   FindClose(&de, &err);
#endif
  }
#ifndef NEWSGOLD
  FindClose(&de, &err);
#endif

if(selmode!=2)
{
  if (tabs[tab]->szFilter[0])
  {
   sprintf(pathbuf, _s_s, dname, tabs[tab]->szFilter);
   if (FindFirstFile(&de, pathbuf, &err))
   {
    do
    {
     if (!(de.file_attr & FA_DIRECTORY))
     {
      if ((show_hidden || !(de.file_attr & FA_HIDDEN))
          && (show_system || !(de.file_attr & FA_SYSTEM)))
       AddFileFromDE(tab, num++, &de);
     }
    }
    while (FindNextFile(&de, &err));
#ifdef NEWSGOLD
    FindClose(&de, &err);
#endif
   }
#ifndef NEWSGOLD
   FindClose(&de, &err);
#endif
  }
 }
 }

 return num;
}

// Заполняет список файлов текущей директории
int FillFiles(int tab, char * dname)
{
  if (tabs[tab]->ccFiles) DelFiles(tab);

  if (isseach==1)
    tabs[tab]->ccFiles = AddSearchFile(tab);
  else
  {
    if (IsArchiveOpened(tab))
    {
      if (IsArchiveOpened(tab)==ZIP_ARCH)
        tabs[tab]->ccFiles = FillZipFiles(tab, dname);
      if (IsArchiveOpened(tab)==_7Z_ARCH)
        tabs[tab]->ccFiles = Fill7ZFiles(tab, dname);
    }else
      tabs[tab]->ccFiles = FillRealPathFiles(tab, dname);
  }
  SortFiles(tab);
  return tabs[tab]->ccFiles;
}

int SetTabDrv(int tab, int num)
{
 if (num >= MAX_DRV) num = 0;
 else if (num < 0) num = MAX_DRV - 1;

 tabs[tab]->CurDrv = num;
 UpdateCSMname();
 return FillFiles(tab, GetTabPath(tab));
}

int SetCurTabDrv(int num)
{
 return SetTabDrv(curtab, num);
}

int RefreshTab(int tab)
{
 FILEINF* cfile = _CurTabFile(tab);
 char* lpname;
 if (cfile)
 {
  lpname = malloc(strlen(cfile->sname) + 1);
  strcpy(lpname, cfile->sname);
 }

 int res = FillFiles(tab, GetTabPath(tab));

 int ind;
 if (cfile)
 {
  ind = GetFileIndex(tab, lpname);
  mfree(lpname);
 }
 else
  ind = 0;

 SetTabIndex(tab, ind, 0);
 return res;
}

int DoRefresh()
{
#ifdef LOG
 // DEBUG
 fn_log(&buffer);
#endif

 return RefreshTab(curtab);
}

void UpdateAll()
{
 for(int ii=0; ii < MAX_TABS + 1; ii++)
  RefreshTab(ii);
}

void _cd_tab(int tab, int drv, char* dname)
{
 if (IsArchiveOpened(tab))
 {
  if (strcmp(tabs[tab]->zipInfo->szCurDir, dname))
  {
   tabs[tab]->iBase[drv] = 0;
   tabs[tab]->iIndex[drv] = 0;
   strcpy(tabs[tab]->zipInfo->szCurDir, dname);
  }
 }
 else
 {
  if (strcmp(tabs[tab]->szDirs[drv], dname))
  {
   tabs[tab]->iBase[drv] = 0;
   tabs[tab]->iIndex[drv] = 0;
   strcpy(tabs[tab]->szDirs[drv], dname);
  }
 }
}

int InitTab(int tab)
{
 tabs[tab] = malloc(sizeof(TABINFO));
 {
  tabs[tab]->sort = ST_FIRST;
  tabs[tab]->szFilter[0]=0;
 }

 tabs[tab]->zipInfo = malloc(sizeof(ARCHINFO));
 {
  zeromem(tabs[tab]->zipInfo, sizeof(ARCHINFO));
 }

 FileListBase[tab] = malloc(sizeof(FILEINF));
 {
  zeromem(FileListBase[tab], sizeof(FILEINF));
  FileListBase[tab]->id = -1;
  FileListBase[tab]->sname = psz_errfile;
  FileListBase[tab]->next = FileListBase[tab];
 }

 for(int num = 0; num < 4; num++)
 {
  _cd_tab(tab, num, Drives[num].path);
 }
 return SetTabDrv(tab, 0);
}

void FreeTab(int tab)
{
 mfree(FileListBase[tab]);
 CloseTabZip(tab);
 mfree(tabs[tab]->zipInfo);
 mfree(tabs[tab]);
}

void CB_CS(int id)
{
 if ((id==IDYES) && pathbuf)
  if (!LoadCS(pathbuf))
    DoShowMsg(muitxt(ind_err_badformat));
  else
  {
    if (CONFIG_LOAD_CS) SaveCS(NULL);
  }
}

void CB_LG(int id)
{
 if ((id==IDYES) && pathbuf)
  if (!LoadMUI(pathbuf))
    DoShowMsg(muitxt(ind_err_badformat));
  else
  {
   SaveMUI(NULL);
   sprintf(msgbuf, _ss, muitxt(ind_lng), muitxt(ind_lngname));
   DoShowMsg(msgbuf);
  }
}
//####### Commands #######

void DoUnPack()
{
  if (IsInArchive()) return;
  if (_CurIndex < 0) return;

  FILEINF* file = _CurFile();
  CurFullPath(file);
  int isArh=IsItArh(pathbuf);

  strcpy(zippathbuf, pathbuf);
  if((isArh == 1) && CONFIG_ZIP_ENABLE)
  {
    SUBPROC((void*)_UnZip);
    return;
  }

  if((isArh == 2) && CONFIG_7Z_ENABLE)
  {
    SUBPROC((void*)_Un7Z);
    return;
  }
}

void DoEditCS()
{
  FILEINF* file = _CurFile();
  str_2ws (wsbuf, MCFilePluginsPath("ColEdit.elf"), MAX_PATH*2);
  if(fexist(MCFilePluginsPath("ColEdit.elf")))
    ExecuteFile(wsbuf,0,CurFullPath(file));
}

void RunAct(char* app)
{
  if(IsMedia(app) && !IsInArchive())
  {
    if (CONFIG_MEDIA_ENABLE)
    {
      DoPlayList(1);
      return;
    }
  }

  if (*CONFIG_VIEW_PUT!='\0' && IsPict(app))
  {
    str_2ws(wsbuf, (const char*)CONFIG_VIEW_PUT,MAX_PATH);
    ExecuteFile(wsbuf,0,CurFullPath(scfile));
    return;
  }

  str_2ws(wsbuf,app,MAX_PATH*2);
  ExecuteFile(wsbuf ,NULL ,NULL);
}

void AltRunAct(char* run, char *ext)
{
  if (*CONFIG_VIEW_PUT!='\0' && IsPict(run))
  {
    str_2ws(wsbuf,run,MAX_PATH*2);
    ExecuteFile(wsbuf ,NULL ,NULL);
    return;
  }

  WSHDR* wsext = AllocWS(MAX_EXT);
  str_2ws(wsext, ext, MAX_EXT);
  int uid = GetExtUid_ws(wsext);
  if (!uid)
  {
    str_2ws(wsext, txt_ext, MAX_EXT);
    uid = GetExtUid_ws(wsext);
  }
  TREGEXPLEXT* regext = get_regextpnt_by_uid(uid);
  if (regext->altproc)
  {
    str_2ws(wsbuf, run, MAX_PATH*2);
    altrun altproc = (altrun)(regext->altproc);
    altproc(wsbuf, wsext, NULL);
  }else
  {
    str_2ws(wsbuf,run,MAX_PATH*2);
    ExecuteFile(wsbuf ,NULL ,NULL);
  }
  FreeWS(wsext);
}

void S_ZipOpen(void)
{
 if (zippathbuf && zippathbuf[0])
 {
  int zerr = OpenTabZip(curtab, zippathbuf);
  if (zerr == UNZ_OK)
  {
   cd(curtab, str_empty);
   SetCurTabIndex(0, 0);
  }
  else if (zerr != -11111) // ignore propr_stop
  {
   sprintf(msgbuf, "OpenZip error %i", zerr);
   DoShowMsg(msgbuf);
  }
  if (IsGuiOnTop(MAINGUI_ID)) REDRAW();
 }
}

void S_7ZOpen(void)
{
  if (zippathbuf && zippathbuf[0])
  {
    int zerr = OpenTab7Z(curtab, zippathbuf);
    if (zerr == UNZ_OK)
    {
      cd(curtab, (char *)str_empty);
      SetCurTabIndex(0, 0);
    }
    else if (zerr != -11111) // ignore propr_stop
    {
      sprintf(msgbuf, "7Zip error %i", zerr);
      DoShowMsg(msgbuf);
    }
    if (IsGuiOnTop(MAINGUI_ID)) REDRAW();
  }
}

void S_ZipOpenFile(char* ext, int isAltOpen)
{
 Busy = 1;

 FILEINF* file = _CurFile();
 CurFullPath(file);

 // Берем имя временного файла
 char szTempFilePath[MAX_PATH];
 sprintf(szTempFilePath, _s_s, CONFIG_TEMP_PATH, GetFileName(pathbuf));
 // Грохаем если он там есть
 if (fexist(szTempFilePath) && !isdir2(szTempFilePath, &err))
  _unlink(szTempFilePath, &err);
 // Извлекаем наш файл или выходим
 int res = ExtractFileByID(tabs[curtab]->zipInfo, file->id,
  (char*)CONFIG_TEMP_PATH, 0, 1);
 // Сохраняем имя в списке временных файлов
 if (CONFIG_DELETE_TEMP_FILES_ON_EXIT)
  fn_add(&tmp_files, FNT_NONE, TYPE_COMMON_FILE, 0, szTempFilePath, NULL, NULL);

  Busy = 0;
  if (res == UNZ_OK && res!=-1)
  {
   char* sz = GetFileExt(file->sname);
   if ((int)sz)
   {
     char szext[MAX_EXT];
     strtolower(sz, szext, MAX_EXT);

     if (!strncmp(szext, mccs_ext, MAX_EXT))
     {
        DoMsgBox(muitxt(ind_pmt_impcs), CB_CS);
        return;
     }
     if (!strncmp(szext, mclg_ext, MAX_EXT))
     {
        DoMsgBox(muitxt(ind_pmt_implg), CB_LG);
        return;
      }
    }

    if (isAltOpen)
      AltRunAct(szTempFilePath, ext);
    else
      RunAct(szTempFilePath);
  }
}

void S_7ZOpenFile(char* ext, int isAltOpen)
{
 Busy = 1;

 FILEINF* file = _CurFile();
 CurFullPath(file);
 // Берем имя временного файла
 char szTempFilePath[MAX_PATH];
 sprintf(szTempFilePath, _s_s, CONFIG_TEMP_PATH, GetFileName(pathbuf));
 // Грохаем если он там есть
 if (fexist(szTempFilePath) && !isdir2(szTempFilePath, &err))
  _unlink(szTempFilePath, &err);
 // Извлекаем наш файл или выходим
 int res = ExtractFileByID7Z(tabs[curtab]->zipInfo, file->id, (char*)CONFIG_TEMP_PATH, 0, 1);
 // Сохраняем имя в списке временных файлов
 if (CONFIG_DELETE_TEMP_FILES_ON_EXIT)
  fn_add(&tmp_files, FNT_NONE, TYPE_COMMON_FILE, 0, szTempFilePath, NULL, NULL);

 Busy = 0;
 if (res == UNZ_OK && res!=-1)
 {
   char* sz = GetFileExt(file->sname);
   if ((int)sz)
   {
     char szext[MAX_EXT];
     strtolower(sz, szext, MAX_EXT);
     if (!strncmp(szext, mccs_ext, MAX_EXT))
     {
        DoMsgBox(muitxt(ind_pmt_impcs), CB_CS);
        return;
     }
     if (!strncmp(szext, mclg_ext, MAX_EXT))
     {
        DoMsgBox(muitxt(ind_pmt_implg), CB_LG);
        return;
      }
    }

    if (isAltOpen)
      AltRunAct(szTempFilePath, ext);
    else
      RunAct(szTempFilePath);
 }
}

void SentNatExpStr()
{
  FILEINF *file = _CurFile();
  wstrcpy(NatExpStr->file_name, file->ws_name);
  str_2ws(NatExpStr->path_to_file, file->folder, 256);
  wsprintf(NatExpStr->full_filename,"%w\\%w", NatExpStr->path_to_file, NatExpStr->file_name);

  selmode=0;
  GBS_SendMessage(MMI_CEPID,MSG_CSM_DESTROYED,0,9999);
  DoExit();
}

void _Open(int isAltOpen)
{
 if (_CurIndex < 0) return;

 FILEINF* file = _CurFile();
 if (file && pathbuf && strlen(file->sname))
 {
  CurFullPath(file);
  if (file->attr & FA_DIRECTORY)
  {
    if(isseach==1)
    {
      if (curtab >= MAX_TABS)
        curtab = back_tab < MAX_TABS ? back_tab : 0;
      isseach=0;
    }

    if(selmode==1 && isAltOpen)
    {
      CurFullPath(file);
      SentNatExpStr();
    }
    else
      cd(curtab, pathbuf);

    return;
  }else
  {
    int isItZip  = IsItZipFile(pathbuf);
    int isIt7Z  = IsIt7ZipFile(pathbuf);
    char* sz = GetFileExt(file->sname);
    if (!IsInArchive()) // Если не в архиве
    {
      if ((int)sz)
      {
        char szext[MAX_EXT];
        strtolower(sz, szext, MAX_EXT);
        char sn[64];
        strtolower(file->sname, sn, 64);

        if(selmode==0)
        {
          if (!strncmp(szext, mcbm_ext, MAX_EXT))
          {
            UseBM(pathbuf);
            return;
          }
          if(!strncmp(sn, keys_file, 64))
          {
            DoKeyHelp();
            return;
          }
          if (!strncmp(szext, mccs_ext, MAX_EXT))
          {
            if (isAltOpen==0)
              DoMsgBox(muitxt(ind_pmt_impcs), CB_CS);
            if (isAltOpen==1)
              DoEditCS();
            return;
          }
          if (!strncmp(szext, mclg_ext, MAX_EXT))
          {
            DoMsgBox(muitxt(ind_pmt_implg), CB_LG);
            return;
          }
        }
      }

      if (CONFIG_7Z_ENABLE || CONFIG_ZIP_ENABLE)
      {
        strcpy(zippathbuf, pathbuf);
        if (((isIt7Z == 1)&&(!isAltOpen)) || ((isIt7Z == -1) && (isAltOpen)))
        {
          SUBPROC((void*)S_7ZOpen);
          return;
        }
        if (((isItZip == 1)&&(!isAltOpen)) || ((isItZip == -1) && (isAltOpen)))
        {
          SUBPROC((void*)S_ZipOpen);
          return;
        }
      }

      if(selmode==1)
      {
        if (isAltOpen)
          RunAct(pathbuf);
        else
        {
          CurFullPath(file);
          SentNatExpStr();
        }
      }else
      {
        if (isAltOpen)
          AltRunAct(pathbuf, sz);
        else
          RunAct(pathbuf);
      }

    }else
    {
      if (IsInArchive()==ZIP_ARCH && !(isItZip || isIt7Z))
        SUBPROC((void*)S_ZipOpenFile, sz, isAltOpen);
      if (IsInArchive()==_7Z_ARCH && !(isItZip || isIt7Z))
        SUBPROC((void*)S_7ZOpenFile, sz, isAltOpen);
      return;
    }
  }
 }
}

void DoOpen()
{_Open(0);}

void DoAltOpen()
{_Open(1);}
////////////////////////////////////////////

int IsMedia(char* file)
{
  static const char AAC[]="aac";
  char* sz = GetFileExt(file);
  if ((int)sz)
  {
    char szext[MAX_EXT];
    strtolower(sz, szext, MAX_EXT);
    if (!strncmp(szext, EXT_MP3, MAX_EXT) ||
        !strncmp(szext, AAC, MAX_EXT))
      return 1;
  }
  return 0;
}

int IsVideo(char* file)
{
  static const char MP4[]="mp4";
  static const char GP[]="3gp";
  char* sz = GetFileExt(file);
  if ((int)sz)
  {
    char szext[MAX_EXT];
    strtolower(sz, szext, MAX_EXT);
    if (!strncmp(szext, MP4, MAX_EXT) ||
        !strncmp(szext, GP, MAX_EXT))
      return 1;
  }
  return 0;
}

int IsPict(char* file)
{
  static const char JPG[]="jpg";
  static const char JPEG[]="jpeg";
  static const char BMP[]="bmp";
  static const char PNG[]="png";
  static const char GIF[]="gif";
  char* sz = GetFileExt(file);
  if ((int)sz)
  {
    char szext[MAX_EXT];
    strtolower(sz, szext, MAX_EXT);

    if (!strncmp(szext, JPG, MAX_EXT) ||
        !strncmp(szext, JPEG, MAX_EXT)||
        !strncmp(szext, BMP, MAX_EXT) ||
        !strncmp(szext, PNG, MAX_EXT) ||
        !strncmp(szext, GIF, MAX_EXT))
      return 1;
  }
  return 0;
}


int mediacount;
int _add(DIR_ENTRY *de, int param)
{
  if (IsMedia(de->file_name) &&  (show_hidden || !(de->file_attr & FA_HIDDEN))
      && (show_system || !(de->file_attr & FA_SYSTEM)))
  {
    sprintf(msgbuf, "%s\\%s\r\n", de->folder_name,de->file_name);
    _write(param, msgbuf, strlen(msgbuf), &err);
    mediacount++;
  }
  return 1;
}

void CB_CrtPL(int id)
{
  char plname[128];
  if(strlen(_CurPath)!=2)
  {
    char *pl=strrchr(_CurPath,'\\')+1;
    strcpy(plname,pl);
  }
  else
    strcpy(plname,"PL");

  sprintf(msgbuf, "%s\\%s.m3u",_CurPath,plname);
  volatile int hFile = _open(msgbuf,A_ReadWrite+A_Create+A_Truncate+ A_BIN,P_READ+P_WRITE, &err);

  sprintf(msgbuf, "#EXTM3U\r\n");
  _write(hFile, msgbuf, strlen(msgbuf), &err);

  mediacount=0;
  EnumFilesInDir(_CurPath, _add, hFile, !id, !id);

  _close(hFile, &err);

  sprintf(msgbuf, "%s\\%s.m3u",_CurPath,plname);
  if(mediacount>0)
  {
    DoRefresh();
    int ind = GetCurTabFileIndex(GetFileName(msgbuf));
    SetCurTabIndex(ind, 0);
  }else
    fsrm(msgbuf, 0);
}

void DoCrtPL()
{
  if (IsInArchive()) return;

  DoMsgBox(muitxt(ind_ms), CB_CrtPL);
}

void DoPlayList(int state)
{
  if (IsInArchive()) return;

  int crt=0;
  FILEINF *file = FileListBase[curtab];
  char *writer=malloc(128);
  unsigned int io_error = 0;
  for(int i=0; i<_CurCount; i++)
  {
    file=file->next;
    FillFileInfo(file);
    if(IsMedia(file->sname)) crt++;
  }
  if (crt)
  {
    file = FileListBase[curtab];
    char direct[128];
    sprintf(direct, "%s\\%s",CONFIG_MCCONFIG_PATH,"default.m3u");
    volatile int hFile = _open(direct,A_ReadWrite+A_Create+A_Truncate+ A_BIN,P_READ+P_WRITE, &io_error);
    if(hFile!=-1)
    {
      sprintf(writer, "#EXTM3U\r\n");
      _write(hFile, writer, strlen(writer), &io_error);
      if (state==1)
      {
        if(IsMedia(scfile->sname))
        {
          sprintf(writer, "%s\\%s\r\n",_CurPath,scfile->sname);
          _write(hFile, writer, strlen(writer), &io_error);
        }
      }
      for(int i=0; i<_CurCount; i++)
      {
        file=file->next;
        FillFileInfo(file);
        if ((state==0) || (file->sname!=scfile->sname))
        if(IsMedia(file->sname))
        {
          sprintf(writer, "%s\\%s\r\n",_CurPath,file->sname);
          _write(hFile, writer, strlen(writer), &io_error);
        }
      }
      _close(hFile, &io_error);
    }
    RunAct(direct);
  }
}
///////////////////////////////////////////////////////////////////////////

static int my_handler (void *data) {return (1);}

void DoNatExpl()
{
  if (IsInArchive()) return;

  NativeExplorerData* myExplorerStruct = malloc(sizeof(NativeExplorerData));
  zeromem(myExplorerStruct, sizeof(NativeExplorerData));
#ifdef NEWSGOLD
  myExplorerStruct->dir_enum = 0x26;
#else
#ifdef SGOLD_X75
  myExplorerStruct->dir_enum = 0x24;
#else
  myExplorerStruct->dir_enum = 0x23;
#endif
#endif
  myExplorerStruct->this_struct_addr = myExplorerStruct;
  myExplorerStruct->mode = 0;
  myExplorerStruct->unk9 = 0;
  myExplorerStruct->is_exact_dir = 1;
  myExplorerStruct->user_handler = my_handler;
  myExplorerStruct->full_filename = AllocWS(MAX_PATH);
  myExplorerStruct->path_to_file = AllocWS(MAX_PATH);
  myExplorerStruct->file_name = AllocWS(MAX_PATH);

  if (_CurIndex >= 0)
  {
   FILEINF* file = _CurFile();
   if (file && pathbuf && strlen(file->sname))
   {
    CurFullPath(file);
    str_2ws(myExplorerStruct->full_filename, pathbuf, MAX_PATH);//полный путь к файлу или папке без /
    str_2ws(myExplorerStruct->path_to_file, _CurPath, MAX_PATH);//4:
    str_2ws(myExplorerStruct->file_name, file->sname, MAX_PATH);//имя папки/файла.ехе
    StartNativeExplorer(myExplorerStruct);
   }
  }

  FreeWS(myExplorerStruct->full_filename);
  FreeWS(myExplorerStruct->path_to_file);
  FreeWS(myExplorerStruct->file_name);
  mfree(myExplorerStruct);
}
////////////////////////////////////////////////////////////////////////////

int sent=0;
GBSTMR sent_tmr;

void DoSent()
{
  if (IsInArchive()) return;

  if (!(scfile->attr & FA_DIRECTORY))
  {
    str_2ws(wsbuf, scfile->folder, MAX_PATH);
    MediaSendCSM_Open(scfile->ws_name, wsbuf);
  }
}

void DeleteTempFiles()
{
 FN_ITM* itm = tmp_files.items;
 while(itm)
 {
  if (fexist(itm->full) && !isdir2(itm->full, &err))
   _unlink(itm->full, &err);
  itm = itm->next;
 }
}

void CB_Exit(int id)
{
 MsgBoxResult = -1;
 if (id == IDYES)
 {
  // Выходим из архива на текущем табе с восстановлением позиции
  ExitFromZip();
  ExitFrom7Z();

  // Закрываем остальные зипы при выходе
  for (int i = 0; i < MAX_TABS; i++)
  {
   if (i != curtab)
   {
     if (IsArchiveOpened(i)==ZIP_ARCH)
      CloseTabZip(i);
     if (IsArchiveOpened(i)==_7Z_ARCH)
      CloseTab7Z(i);
   }
  }

  // Очищаем темп, если разрешено в конфиге
  if (CONFIG_DELETE_TEMP_FILES_ON_EXIT && tmp_files.count)
  {
   DeleteTempFiles();
   fn_free(&tmp_files);
  }

  Terminate = progr_stop = 1;
  if (!Busy) GeneralFuncF1(1);
 }
}

void DoExit()
{
 if (CONFIG_CONFIRM_EXIT)
   DoMsgBox(muitxt(ind_pmt_exit), CB_Exit);
 else
   CB_Exit(IDYES);
}

void ExitFromZip()
{
 if (IsInArchive()==ZIP_ARCH)
 {
  //Сохраняем имя тек. зипа
  char lpname[MAX_PATH];
  strcpy(lpname, GetFileName(_CurTab->zipInfo->szZipPath));

  CloseTabZip(curtab);
  RefreshTab(curtab);

  //Ищем файл из которого вышли
  int ind = GetCurTabFileIndex(lpname);
  SetCurTabIndex(ind, 0);

  UpdateCSMname();
 }
}

void ExitFrom7Z()
{
  if (IsInArchive()==_7Z_ARCH)
  {
    //Сохраняем имя тек. зипа
    char lpname[MAX_PATH/2];
    strcpy(lpname, GetFileName(_CurTab->zipInfo->szZipPath));
    CloseTab7Z(curtab);
    RefreshTab(curtab);

    //Ищем файл из которого вышли
    int ind = GetCurTabFileIndex(lpname);
    SetCurTabIndex(ind, 0);

    UpdateCSMname();
  }
}

int DoBack()
{
 int res = 0;
 if (pathbuf)
 {
  if (curtab < MAX_TABS)
  {
   char lpname[MAX_PATH];
   char* path = IsInArchive() ? _CurTab->zipInfo->szCurDir : _CurPath;
   // Читаем родительскуюя папку в pathbuf
   GetFileDir(path, pathbuf);
   // Проверка на корень
   int goOut = IsInArchive() ? *path == '\0' : *pathbuf == '\0';

   if (!goOut)
   {
    //Сохраняем имя тек. папки
    strcpy(lpname, GetFileName(path));

    res = cd(curtab, pathbuf);

    //Ищем папку из которой вышли
    int ind = GetCurTabFileIndex(lpname);
    SetCurTabIndex(ind, 0);
   }
   else if (IsInArchive()==ZIP_ARCH) ExitFromZip();
   else if (IsInArchive()==_7Z_ARCH) ExitFrom7Z();
   else if (CONFIG_BACK_EXIT) DoExit();
  }
  else
  {
   curtab = back_tab < MAX_TABS ? back_tab : 0;
   isseach=0;
  }
 }
 return res;
}

void DoSwapTab()
{
 curtab++;
 if (curtab >= MAX_TABS) curtab = 0;

 // Останавливаем скроллинг при смене таба
 scfile = NULL;

 UpdateCSMname();
}

int cordrv(int num, int inc)
{
  do
  {
    num = (MAX_DRV + num + inc) % MAX_DRV;
  }
  while (!Drives[num].enabled);
  return num;
}


void DoPrvDrv()
{
 if (IsInArchive()) return;

 if (curtab < MAX_TABS)
  SetCurTabDrv(cordrv(_CurDrv, -1));
}

void DoNxtDrv()
{
 if (IsInArchive()) return;

 if (curtab < MAX_TABS)
  SetCurTabDrv(cordrv(_CurDrv, +1));
}

void DoUp()
{
  SetCurTabIndex(--_CurIndex, 1);
}

void DoDwn()
{
  SetCurTabIndex(++_CurIndex, 1);
}

void DoPgUp()
{
 _CurIndex -= LONG_SCRL;
 if (_CurIndex < 0) _CurIndex = 0;
 SetCurTabIndex(_CurIndex, 0);
}

void DoPgDwn()
{
 _CurIndex += LONG_SCRL;
 _CurBase  = _CurIndex;
 if (_CurIndex >= _CurCount) _CurIndex = _CurCount - 1;
 SetCurTabIndex(_CurIndex, 0);
}

void DoBegin()
{
 SetCurTabIndex(0, 0);
}

void DoChk()
{
 ChkFile(_CurFile());
 if (_CurIndex < _CurCount - 1)
  SetCurTabIndex(++_CurIndex, 1);
}

void DoChkAll()
{
 ChkAll(1);
}

void DoUnCAll()
{
 ChkAll(0);
}

void DoInvChk()
{
 ChkAll(-1);
}

void DoOptions()
{
 if (wsbuf)
 {
  str_2ws(wsbuf, successed_config_filename, MAX_PATH);
  ExecuteFile(wsbuf, NULL, NULL);
 }
}

void DoKeyHelp()
{
  if (wsbuf)
  {
    str_2ws(wsbuf, MCFilePluginsPath("KeyEdit.elf"), MAX_PATH*2);
    ExecuteFile(wsbuf, NULL, MCFilePath(keys_file));
  }
}

void DoShowPath()
{
  CurFullPath(scfile);
  str_2ws(wsbuf, pathbuf, MAX_PATH);
  FullPatch(wsbuf);
}

void DoNewDir()
{
 if (IsInArchive()) return; // Пока не обрабатывается

 if (wsbuf)
 {
  if (*szLastNewDir)
   str_2ws(wsbuf, szLastNewDir, MAX_PATH);
  else
   str_2ws(wsbuf, def_new_dir, MAX_PATH);

  TextInput(muitxt(ind_newdir), muitxt(ind_name), 1, wsbuf, _NewDir);
 }
}

void DoNewFile()
{
 if (IsInArchive()) return; // Пока не обрабатывается

 if (wsbuf)
 {
  if (*szLastNewFile)
   str_2ws(wsbuf, szLastNewFile, MAX_PATH);
  else
   str_2ws(wsbuf, def_new_file, MAX_PATH);

  TextInput(muitxt(ind_newfile), muitxt(ind_name), 1, wsbuf, _NewFile);
 }
}

void _Filter(WSHDR *wsname)
{
 ws_2str(wsname, _CurTab->szFilter, MAX_PATH);
 if (strcmp(_CurTab->szFilter, def_filter) == 0)
  _CurTab->szFilter[0] = 0;

 DoRefresh();
 GeneralFuncF1(1);
}

void DoFilter()
{
 if (wsbuf)
 {
  if (_CurTab->szFilter[0])
   str_2ws(wsbuf, _CurTab->szFilter, MAX_PATH);
  else
   str_2ws(wsbuf, def_filter, MAX_PATH);

  TextInput(muitxt(ind_filter), muitxt(ind_name), 0, wsbuf, _Filter);
 }
}

void DoRen()
{
 if (IsInArchive()) return; // Пока не обрабатывается

 FILEINF *cfile = _CurFile();
 if (cfile)
  TextInput(muitxt(ind_rename), muitxt(ind_name), 1, cfile->ws_name, _Rename);
}

void CB_Paste(int id)
{
  MsgBoxResult = -1;
  if(id == IDYES) SUBPROC((void *)S_Paste);
}

void DoPaste()
{
 if (IsInArchive()==_7Z_ARCH) return; // Пока не обрабатывается

 if (buffer.count)
 {
  if (buffer.type == FNT_MOVE)
  {
   if (CONFIG_CONFIRM_MOVE)
     DoMsgBox(muitxt(ind_pmt_move), CB_Paste);
   else
    CB_Paste(IDYES);
  }
  else if (buffer.type == FNT_COPY)
  {
   if (CONFIG_CONFIRM_COPY)
     DoMsgBox(muitxt(ind_pmt_copy), CB_Paste);
   else
    CB_Paste(IDYES);
  }
 }
}

void CB_Cancel(int id)
{
 MsgBoxResult = -1;
 if (id==IDYES)
 {
   fn_free(&buffer);
 }
}

void DoCancel()
{
 if (buffer.count)
   DoMsgBox(muitxt(ind_pmt_cancel), CB_Cancel);
}

void DoCopy()
{
  EnumSel(M_MoveCopy, FNT_COPY);
}

void DoMove()
{
 if (IsInArchive()) return; // Пока не обрабатывается
 EnumSel(M_MoveCopy, FNT_MOVE);
}

void CB_Del(int id)
{
 MsgBoxResult = -1;
 if (id==IDYES)
 {
   #if GOLD_ZIP == 0
   if(IsInArchive()==ZIP_ARCH)
   {
     strcpy(zippathbuf, _CurTab->zipInfo->szZipPath);
     SUBPROC((void *)DeleteFileFromZIP);
   }
   else
   #endif
     SUBPROC((void *)S_Delit);
 }
}

void DoDel()
{
#if GOLD_ZIP == 0
 if (IsInArchive()==_7Z_ARCH) return; // Пока не обрабатывается
 #else
 if (IsInArchive()) return; // Пока не обрабатывается
 #endif
 if (CONFIG_CONFIRM_DELETE)
   DoMsgBox(muitxt(ind_pmt_del), CB_Del);
 else
   CB_Del(IDYES);
}

void DoRoot()
{
 if (IsInArchive())
  cd(curtab, str_empty);
 else if (curtab < MAX_TABS)
  cd(curtab, Drives[_CurDrv].path);
}

void DoSortN()
{
 _CurTab->sort=(_CurTab->sort & STD_MASK) | ST_NAME;
 DoRefresh();
}

void DoSortE()
{
 _CurTab->sort=(_CurTab->sort & STD_MASK) | ST_EXT;
 DoRefresh();
}

void DoSortS()
{
 _CurTab->sort=(_CurTab->sort & STD_MASK) | ST_SIZE;
 DoRefresh();
}

void DoSortD()
{
 _CurTab->sort=(_CurTab->sort & STD_MASK) | ST_DATE;
 DoRefresh();
}

void DoSetSort()
{
 int srt = (_CurTab->sort & STV_MASK);
 srt=(srt>=ST_LAST)?srt=ST_FIRST:srt+1;
 _CurTab->sort=(_CurTab->sort & STD_MASK) | srt;
 DoRefresh();
}

void DoSortR()
{
 _CurTab->sort=(_CurTab->sort & STV_MASK) | (~_CurTab->sort & STD_MASK);
 DoRefresh();
}

void DoTabCopy()
{
 // Очищаем буфер
 CB_Cancel(IDYES);
 // Копируем текущие файлы в буфер
 DoCopy();
 // Переходим на другой таб
 DoSwapTab();
 // Вставляем файлы на него
 DoPaste();
}

void DoTabMove()
{
 // Очищаем буфер
 CB_Cancel(IDYES);
 // Копируем текущие файлы в буфер
 DoMove();
 // Переходим на другой таб
 DoSwapTab();
 // Вставляем файлы на него
 DoPaste();
}

void DoErrKey()
{
  DoShowMsg(muitxt(ind_err_badkey));
}

void DoMD5()
{
  if(IsInArchive()) return;

  FILEINF *file = _CurFile();
  if(!(file->attr & FA_DIRECTORY))
  {
    CurFullPath(file);
    GetMD5(pathbuf);
  }
}

//####### ######## #######


FILEINF* _CurFile()
{return _CurTabFile(curtab);}

FILEINF* _CurTabFile(int tab)
{
 int ind = GetTabIndex(tab);
 if (ind < 0) return NULL;

 FILEINF* file = FileListBase[tab];
 for(int ii=0; ii<=ind; ii++)
  if (file)
   file = file->next;
  else
   return NULL;

 return file;
}

int GetFileIndex(int tab, char* fname)
{
 if (tabs[tab]->ccFiles)
 {
  int ind=0;
  FILEINF* file = FileListBase[tab]->next;
  while(file != FileListBase[tab])
  {
   if (!strcmp(fname, file->sname))
    return ind;
   file = file->next;
   ind++;
  }
 }
 return -1;
}

int GetCurTabFileIndex(char* fname)
{
 return GetFileIndex(curtab, fname);
}

int ___IsMultiChk(FILEINF *file, int param){ return 1;}

int IsMultiChk()
{
 return EnumChk(___IsMultiChk, 0);
}

int EnumChk(ENUM_SEL_PROC EnumProc, int param)
{
 int cc = 0;
 if (tabs[curtab]->ccFiles)
 {
  FILEINF *file = FileListBase[curtab]->next;
  while(file != FileListBase[curtab])
  {
   if (file->attr & FA_CHECK)
   {
    cc++;
    if (EnumProc) if (EnumProc(file, param)==0) break;
   }
   file = file->next;
  }
 }
 return cc;
}

int EnumSel(ENUM_SEL_PROC EnumProc, int param)
{
 int cc = EnumChk(EnumProc, param);
 if (cc == 0)
 {
  if (EnumProc)
  {
   FILEINF* cfile = _CurFile();
   if (cfile)
   {
    EnumProc(cfile, param);
    cc++;
   }
  }
 }
 return cc;
}

int ChkAll(int chk)
{
 if (tabs[curtab]->ccFiles)
 {
  FILEINF *file = FileListBase[curtab]->next;
  while(file!=FileListBase[curtab])
  {
   if (chk>0 || (chk<0 && !(file->attr & FA_CHECK)))
   {
    if(!CONFIG_SOFT_BUTTON)
    {
      if(file->ftype == TYPE_COMMON_FILE) SelParam.gsize += file->size;
      if(file->ftype == TYPE_ZIP_FILE)    SelParam.gcsize += file->csize;
    }
    file->attr = file->attr | FA_CHECK;
   }
   else
   {
    if(!CONFIG_SOFT_BUTTON)
    {
      if(file->ftype == TYPE_COMMON_FILE) SelParam.gsize -= file->size;
      if(file->ftype == TYPE_ZIP_FILE)    SelParam.gcsize -= file->csize;
    }
    file->attr = file->attr & ~FA_CHECK;
   }

   file = file->next;
  }
  return 1;
 }
 return 0;
}

void ChkFile(FILEINF *file)
{
 if (file)
 {
   if(!CONFIG_SOFT_BUTTON)
   {
     if(file->attr & FA_CHECK)
     {
       if(file->ftype == TYPE_COMMON_FILE) SelParam.gsize -= file->size;
       if(file->ftype == TYPE_ZIP_FILE)    SelParam.gcsize -= file->csize;
     }else
     {
       if(file->ftype == TYPE_COMMON_FILE) SelParam.gsize += file->size;
       if(file->ftype == TYPE_ZIP_FILE)    SelParam.gcsize += file->csize;
     }
   }
   file->attr = file->attr & FA_CHECK ? file->attr & ~FA_CHECK : file->attr | FA_CHECK;
 }
}

void initprogr(int act)
{
 progr_start = 1;
 progr_max = 0;
 progr_stop = 0;
 progr_act = act;
 DoScreen();
}

void incfname(char *name)
{
 str_2ws(progr_name, name,128);
 RusTeg(progr_name);
}

void incprogrset(int inc)
{
 progr_cur = inc;
 if (IsGuiOnTop(MAINGUI_ID)) REDRAW();
}

void incprogr(int inc)
{
 if (inc > 0) progr_cur += inc;
 else if (!inc) progr_cur = 0;

 if (IsGuiOnTop(MAINGUI_ID)) REDRAW();
}

void incprogrsp(int inc)
{
 if (inc) progrsp_cur += inc;
 else progrsp_cur = 0;

 if (IsGuiOnTop(MAINGUI_ID) && (((int)progrsp_cur)%(BUF_SIZE*3))) REDRAW();
}

void endprogr()
{
 progr_start = 0;
 progr_max = 0;
 progr_cur = 0;
 if (IsGuiOnTop(MAINGUI_ID)) REDRAW();
}

void endprogrsp()
{progrsp_max = progrsp_cur = 0;}

char* CurFullPath(FILEINF* sfile)
{
 if (pathbuf)
   sprintf(pathbuf, _s_s, sfile->folder, sfile->sname);

 return pathbuf;
}

char* CurFullPathByName(char* sfile)
{
 if (pathbuf)
 {
  if (IsInArchive())
   sprintf(pathbuf, _s_s, _CurTab->zipInfo->szCurDir, sfile);
  else
   sprintf(pathbuf, _s_s, _CurPath, sfile);
 }
 return pathbuf;
}

char* CurPath()
{
  static const char _s1[]="%s\\";
  if (pathbuf)
  {
    if (IsInArchive())
    {
      if (IsInArchive()==ZIP_ARCH)
        sprintf(pathbuf, "ZIP:%s\\", _CurTab->zipInfo->szCurDir);
      if (IsInArchive()==_7Z_ARCH)
        sprintf(pathbuf, "7Z:%s\\", _CurTab->zipInfo->szCurDir);
    }
    else
      sprintf(pathbuf, _s1, _CurPath);
  }
  return pathbuf;
}

char* CurFullPathBM(char* buff, char* sfile)
{
 if (buff)
  sprintf(buff, _s_s, _CurPath, sfile);
 return buff;
}

char* TmpFullPath2(char* buff, char* sfile)
{
 if (buff)
 {
  TTime t;
  GetDateTime(NULL,&t);
  sprintf(buff, _s_s_d, _CurPath, sfile, *(int*)&t);
 }
 return buff;
}

char* MCFilePath(const char* sfile)
{
 if (pathbuf)
  sprintf(pathbuf, _s_s, mcpath, sfile);
 return pathbuf;
}

char* MCFilePluginsPath(const char* sfile)
{
 if (pathbuf)
 {
   sprintf(pathbuf, "%s\\%s", mcpath, "Plugins");
   mktree(pathbuf);
   sprintf(pathbuf, "%s\\%s\\%s", mcpath, "Plugins",sfile);
 }
 return pathbuf;
}

//this procedure resets counter
void ResetAutoExit()
{AutoExitCounter = 0;}

const int AutExitCheckFrequencySec = 15;
void AutoExitProc()
{
 if (CONFIG_AUTO_EXIT_AFTER_MIN)
  GBS_StartTimerProc(&offtm, 216 * AutExitCheckFrequencySec, AutoExitProc); //sending timer in 5sec period

 if (!Busy && (IsGuiOnTop(MAINGUI_ID) || CONFIG_ENABLE_BACKGROUND_EXIT))
 {
  AutoExitCounter++;

  if(AutoExitCounter * AutExitCheckFrequencySec > CONFIG_AUTO_EXIT_AFTER_MIN * 60)
   CloseCSM(MAINCSM_ID);
 }
}

void FreeGrap()
{
  PNGTOP_DESC *ptop=PNG_TOP();
  PNGLIST *pl;
  LockSched();
  pl=ptop->pltop;
  ptop->pltop=0;
  UnlockSched();
  while(pl)
  {
    PNGLIST *pl_prev;
    pl_prev=pl;
    pl=pl->next;
    mfree(pl_prev->pngname);
    if(pl_prev->img)
    {
      mfree(pl_prev->img->bitmap);
      mfree(pl_prev->img);
    }
    mfree(pl_prev);
  }
}

void allclose()
{
  Busy1=0;
  drwinfo=0;
  filepropshow=0;
  showabout=0;
  disks_opt=0;
#if GOLD_ZIP == 0
  arch=0;
#endif
  seach=0;
  showmenu=0;
  smsgopen=0;
  msgopen=0;
}


