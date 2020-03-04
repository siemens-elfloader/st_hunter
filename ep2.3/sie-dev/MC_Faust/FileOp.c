#include "inc\mc.h"
#include "inc\mui.h"
#include "inc\zslib.h"
#include "inc\popup.h"

void ShowErr1(int lgind, char* str)
{
 sprintf(msgbuf, muitxt(lgind), str);
 DoShowMsg(msgbuf);
}

void CB_WaitMsgBoxYesNo(int id)
{
 MsgBoxResult = id;
}

int ShowWaitMsgBoxYesNo()
{
  MsgBoxResult = -1;
  DoMsgBox(msgbuf, CB_WaitMsgBoxYesNo);
  while (MsgBoxResult < 0)
  {
    GetCPUClock();
  }
  MsgBoxClose();
  return MsgBoxResult;
}

int MsgBoxYesNoWithParam(int lgind, char* str)
{
 if(MsgBoxResult==IDYES
    || MsgBoxResult==IDNO
    || MsgBoxResult==-1)
 {
   str_2ws(wsbuf, str, MAX_PATH);
   Win1251(wsbuf);
   ws_2str(wsbuf, pathbuf_fn, MAX_PATH);
   sprintf(msgbuf, muitxt(lgind), pathbuf_fn);

   return ShowWaitMsgBoxYesNo();
 }else
   return MsgBoxResult;
}
///////////////////////////////////////////////
int isdir2(char* dname, unsigned int *Err)
{
  int attr=0;
  GetFileAttrib(dname, (unsigned char*)&attr, Err);
  return(attr & FA_DIRECTORY);
}

int isDir(int tab, char* dname)
{
 if (IsArchiveOpened(tab))
  return 1;
 else
  return isdir2(dname, &err);
  //return isdir(dname, &err);

}
///////////////////////////////////////////////
int cd(int tab, char *dname)
{
 unsigned int drv;
 if (IsArchiveOpened(tab))
 {
   drv = _CurDrv;
 }
 else
 {
  if (!isDir(tab, dname))
  {
   char buf[MAX_PATH];
   sprintf(buf, "%t\\", dname);
   if (!isdir2(buf, &err))
   {
    ShowErr1(ind_err_badname_t, dname);
    return 0;
   }
  }
  drv = dname[0] - '0';
  if (drv == 4) drv = 3;
  if (drv >= MAX_DRV)
  {
   ShowErr1(ind_err_baddrv_t, dname);
   return 0;
  }
 }

 _cd_tab(tab, drv, dname);
 int res = SetTabDrv(tab, drv);
 if (!res) SetTabIndex(tab, 0, 0);

 return res;
}

int cdsys(char *dname)
{
 if (pathbuf2)
 {
  if (cd(systab, dname))
  {
   back_tab = curtab;
   curtab = systab;
   return 1;
  }
 }
 return 0;
}

int _DirSize(DIR_ENTRY *de, int param)
{
 if (param)
 {
  CHKDS_PARAM* p = (CHKDS_PARAM*)param;
  p->gsize += de->file_size;
  return 1;
 }
 return 0;
}

int _ZipDirSize(FILEINF *file, int param)
{
 if (param)
 {
  CHKDS_PARAM* p = (CHKDS_PARAM*)param;
  p->gsize += file->size;
  p->gcsize += file->csize;
  return 1;
 }
 return 0;
}

int _7ZDirSize(FILEINF *file, int param)
{
 if (param)
 {
  CHKDS_PARAM* p = (CHKDS_PARAM*)param;
  p->gsize += file->size;
  return 1;
 }
 return 0;
}

int M_DirSize(FILEINF *file, int param)
{
 if (file && pathbuf && strlen(file->sname))
 {
  CHKDS_PARAM* p = (CHKDS_PARAM*)param;
  CurFullPath(file);
  switch (file->ftype)
  {
   case TYPE_COMMON_FILE:
    p->gsize += file->size;
    p->count++;
    break;
   case TYPE_COMMON_DIR:
    p->count += EnumFiles(pathbuf, _DirSize, (int)p) + 0x10000;
    break;
   case TYPE_ZIP_FILE:
    p->gsize += file->size;
    p->gcsize += file->csize;
    p->count++;
    break;
   case TYPE_ZIP_DIR:
    p->count += EnumZipFiles(_CurTab->zipInfo, pathbuf, _ZipDirSize, (int)p) + 0x10000;
    break;
  case TYPE_7Z_FILE:
    p->gsize += file->size;
    p->count++;
    break;
   case TYPE_7Z_DIR:
    p->count += Enum7ZFiles(_CurTab->zipInfo, pathbuf, _7ZDirSize, (int)p) + 0x10000;
    break;
   default:
    break;
  }
  return 1;
 }
 return 0;
}

int _SetAttr(DIR_ENTRY *de, int param)
{
 if (pathbuf2)
 {
  sprintf(pathbuf2, _s_s, de->folder_name, de->file_name);
  SetFileAttrib(pathbuf2, param, &err);
  return 1;
 }
 return 0;
}

int M_SetAttr(FILEINF *file, int param)
{
 if (file && pathbuf && strlen(file->sname))
 {
  CurFullPath(file);
  SetFileAttrib(pathbuf, fa_attr, &err);
  if (fa_sb)
   EnumFiles(pathbuf, _SetAttr, fa_attr);
  return 1;
 }
 return 0;
}

void _NewDir(WSHDR *wsname)
{
 if (pathbuf && pathbuf2)
 {
  ws_2str(wsname, pathbuf2, MAX_PATH);
  strcpy(szLastNewDir, pathbuf2); // Сохраняем введенное имя
  CurFullPathByName(pathbuf2);

  if (!mkdir(pathbuf, &err))
    DoShowMsg(muitxt(ind_err_makedir));
  else
  {
   DoRefresh();
   //Ищим папку которую создали
   int ind = GetCurTabFileIndex(pathbuf2);
   SetCurTabIndex(ind, 0);
  }
 }
 GeneralFuncF1(1);
}

void CB_NewFile(int id)
{
 if (id==IDYES || id==-1 || id==-2)
 {
  if(fexist(pathbuf))
    fsrm(pathbuf, 0);

  int f;
  if ((f=fopen(pathbuf,A_ReadWrite+A_Create+A_Truncate+A_BIN,P_READ+P_WRITE,&err))!=-1)
  {
   DoRefresh();
   //Ищем новый файл
   int ind = GetCurTabFileIndex(pathbuf2);
   SetCurTabIndex(ind, 0);

   lseek(f, 0, S_SET, &err, &err);
   fclose(f, &err);
  }
  else
    DoShowMsg(muitxt(ind_err_makefile));
 }
}

void _NewFile(WSHDR *wsname)
{
 if (pathbuf && pathbuf2)
 {
  ws_2str(wsname, pathbuf2, MAX_PATH);
  strcpy(szLastNewFile, pathbuf2); // Сохраняем введенное имя
  CurFullPathByName(pathbuf2);

  if (CONFIG_CONFIRM_REPLACE && fexist(pathbuf))
  {
    sprintf(msgbuf, muitxt(ind_pmt_exists), _fullnull);
    DoMsgBox(msgbuf, CB_NewFile);
  }
  else
   CB_NewFile(IDYES);
 }
 GeneralFuncF1(1);
}

void CB_RenEx(int id)
{
 if (id==IDYES || id==-1 || id==-2)
 {
  char src[MAX_PATH];
  FILEINF *file = _CurFile();
  sprintf(src, _s_s, _CurPath, file->sname);
  if (id==IDYES) unlink(pathbuf, &err);
  int res;
  if (id==-2)
  {
   char tmpBuf[MAX_PATH];
   TmpFullPath2(tmpBuf, file->sname);
   res = fmove(src, tmpBuf, &err) && fmove(tmpBuf, pathbuf, &err);
  }
  else
   res = fmove(src, pathbuf, &err);

  DoRefresh();
  if (res)
  {
   //Ищем
   int ind = GetCurTabFileIndex(pathbuf2);
   SetCurTabIndex(ind, 0);
  }
  else
    DoShowMsg(muitxt(ind_err_rename));
 }
}

void _Rename(WSHDR *wsname)
{
 if (pathbuf && pathbuf2)
 {
  ws_2str(wsname, pathbuf2, MAX_PATH);
  CurFullPathByName(pathbuf2);

  if (wstrcmpi(wsname, _CurFile()->ws_name))
  {
   if (fexist(pathbuf))
   {
    if (CONFIG_CONFIRM_REPLACE)
    {
      wstrcpy(wsbuf, wsname);
      Win1251(wsbuf);
      ws_2str(wsbuf, pathbuf_fn, MAX_PATH);
      sprintf(msgbuf, muitxt(ind_pmt_exists), pathbuf_fn);

      DoMsgBox(msgbuf, CB_RenEx);
    }
    else
     CB_RenEx(IDYES);
   }
   else
   {
    CB_RenEx(-1);
   }
  }
  else
   CB_RenEx(-2);
 }
}

int M_Delit(FILEINF *file, int param)
{
 if (file && pathbuf && strlen(file->sname))
 {
  incfname(file->sname);
  CurFullPath(file);

  if (file->attr & FA_READONLY)
  {
   int ret=MsgBoxYesNoWithParam(ind_pmt_rodel, file->sname);
   if (!CONFIG_CONFIRM_DELETERO ||
       ret == IDYES || ret == IDYES2ALL)
   {
    SetFileAttrib(pathbuf, 0, &err);

    if (!fsrm(pathbuf, 1))
     *(int*)param = 0;
   }
  }
  else
  {
   if (!fsrm(pathbuf, 1))
    *(int*)param = 0;
  }

  return 1;
 }
 return 0;
}

int M_FileCnt(FILEINF *file, int param)
{
 if (file && pathbuf)
 {
  CurFullPath(file);
  *(int*)param += GetFilesCnt(pathbuf);
  return 1;
 }
 return 0;
}

void S_Delit(void)
{
 if (_CurIndex < 0) return;
 Busy = 1;
 int res = 1;

 initprogr(ind_deling);
 EnumSel(M_FileCnt, (int)&progr_max);
 incprogr(0);

 // Store current file index
 int ind = _CurIndex;
 EnumSel(M_Delit, (int)&res);

 if (ind < _CurTab->ccFiles && res) // Только если есть куда идти вниз
  // List is not refreshed yet, so move index to the next file if there are no errors
  SetCurTabIndex(ind + 1, 0);

 DoRefresh();

 endprogr();
 Busy = 0;

 if (!res)DoShowMsg(muitxt(ind_err_delete));
}

int M_MoveCopy(FILEINF *file, int param)
{
 if (file && pathbuf && strlen(file->sname))
 {
  int pname = strlen(_CurPath) + 1;
  CurFullPath(file);
  if(file->attr & FA_CHECK)file->attr = file->attr & ~FA_CHECK;
  fn_add(&buffer, param, file->ftype, pname, pathbuf, IsInZip() ? _CurTab->zipInfo->szZipPath : NULL, file->sname);

  return 1;
 }
 return 0;
}

int GetFilesCntArh(FN_ITM* pi)
{
  int res=1;
  ARCHINFO *pzi=&zi;

  if (pi && pi->zipPath && pi->full && (pi->ftype == TYPE_ZIP_DIR || pi->ftype == TYPE_7Z_DIR))
  {
    int type = (pi->ftype == TYPE_7Z_DIR) ? _7Z_ARCH : ZIP_ARCH;

    int usingZipFromTab = 0;
    for (int i = 0; i < MAX_TABS; i++)
    {
      if (IsArchiveOpened(i)==type && stricmp(tabs[i]->zipInfo->szZipPath, pi->zipPath) == 0)
      {
        // Если уже открыт в табе, то используем его
        pzi = tabs[i]->zipInfo;
        usingZipFromTab = 1;
        break;
      }
    }
    if (!usingZipFromTab)
    {
      if (pzi->type==ZIP_ARCH && stricmp(pzi->szZipPath, pi->zipPath) != 0)
        CloseZip(pzi);
      if (pzi->type==_7Z_ARCH && stricmp(pzi->szZipPath, pi->zipPath) != 0)
        Close7Z(pzi);

      // Если файл еще не открыт - то открываем
      switch(type)
      {
      case ZIP_ARCH:
        if (pzi->type == 0)
          if (OpenZip(pzi, pi->zipPath) != UNZ_OK)
            return 0;
        break;
      case _7Z_ARCH:
        if (pzi->type == 0)
          if (Open7ZFile(pzi, pi->zipPath) != SZ_OK)
            return 0;
        break;
      }
    }

    int sub;

    switch(type)
    {
      case ZIP_ARCH:
        sub = EnumZipFiles(pzi, pi->full, NULL, 0);
        break;
      case _7Z_ARCH:
        sub = Enum7ZFiles(pzi, pi->full, NULL, 0);
        break;
    }

    res += (sub>>16) + (sub & 0xffff);

    if (type==ZIP_ARCH)
      CloseZip(&zi);
    else
      if (type==_7Z_ARCH)
        Close7Z(&zi);
  }

  return res;
}

//Buffer
FN_LIST buffer;
void S_Paste(void)
{
 if (buffer.count)
 {
  if (pathbuf)
  {
   int res=1;
   Busy = 1;

   if (buffer.type == FNT_MOVE) progr_act = ind_moveing;
   else if (buffer.type == FNT_COPY) progr_act = ind_copying;

   initprogr(progr_act);

   FN_ITM* itm = buffer.items;
   FN_ITM* last_itm = itm;
   progr_max=0;
   while(itm)
   {
    if (itm->ftype == TYPE_COMMON_DIR)
     progr_max += GetFilesCnt(itm->full);
    else
      if (itm->ftype == TYPE_ZIP_DIR || itm->ftype == TYPE_7Z_DIR)
        progr_max += GetFilesCntArh(itm);
      else
        progr_max ++;
    last_itm = itm;
    itm = itm->next;
   }
   incprogr(0);

   itm = buffer.items;
   ArchiveBufferExtractBegin();
   #ifdef NEWSGOLD
   zipFile dzip;
   int iszip=IsInZip();
   if (iszip) dzip = zipOpen2 (_CurTab->zipInfo->szZipPath, APPEND_STATUS_ADDINZIP, NULL, NULL);
   #endif
   while(itm && !progr_stop)
   {
    char *sname = itm->full + itm->pname;
    incfname(sname);
    if (itm->ftype == TYPE_COMMON_DIR || itm->ftype == TYPE_COMMON_FILE)
    {
      #ifdef NEWSGOLD
      if (iszip)
      {
        sprintf(pathbuf, _s_s, _CurTab->zipInfo->szCurDir, sname);
        char* fname = pathbuf[0] == '\\' ? pathbuf + 1 : pathbuf;
        strreplace(fname, '\\', '/');

        if(dzip!=NULL)
          res &= ZipCopy(dzip, fname, itm->full, NULL);

        if(buffer.type == FNT_MOVE && res) fsrm(itm->full, 0);
      }else
      #endif
      {
        char *sname = itm->full + itm->pname;
        sprintf(pathbuf, _s_s, _CurPath, sname);
        if (buffer.type == FNT_MOVE)
        {
          progr_act = ind_moveing;
          res &= fsmv(itm->full, pathbuf);
        }
        else if (buffer.type == FNT_COPY)
        {
          progr_act = ind_copying;
          res &= fscp(itm->full, pathbuf, 1);
        }
      }
    }else
    {
      if (itm->ftype == TYPE_ZIP_FILE || itm->ftype == TYPE_ZIP_DIR ||
          itm->ftype == TYPE_7Z_FILE || itm->ftype == TYPE_7Z_DIR)
      {
        if (buffer.type == FNT_COPY)
        {
          if(itm->ftype == TYPE_ZIP_FILE || itm->ftype == TYPE_ZIP_DIR)
            res &= (ArchiveBufferExtract(ZIP_ARCH, itm, _CurPath) == UNZ_OK);

          if(itm->ftype == TYPE_7Z_FILE || itm->ftype == TYPE_7Z_DIR)
            res &= (ArchiveBufferExtract(_7Z_ARCH, itm, _CurPath) == UNZ_OK);
        }
      }
    }
    itm=itm->next;
   }
   ArchiveBufferExtractEnd();
   #ifdef NEWSGOLD
   if (iszip)
   {
     zipClose(dzip, NULL);
     strcpy(pathbuf_fn, _CurTab->zipInfo->szCurDir);
     strcpy(zippathbuf, _CurTab->zipInfo->szZipPath);
     ExitFromZip();
     S_ZipOpen();
     strcpy(_CurTab->zipInfo->szCurDir, pathbuf_fn);
   }
   #endif

   UpdateAll();

   //Ищем первый файл
   if(!progr_stop)
   {
     int ind = GetCurTabFileIndex(GetFileName(last_itm->full));
     SetCurTabIndex(ind, 0);
   }

   fn_free(&buffer);

   endprogr();

   if (!res)
     DoShowMsg(muitxt(ind_err_resnok));

   Busy = 0;
  }
 }
 else
   DoShowMsg(muitxt(ind_err_nofiles));
}
