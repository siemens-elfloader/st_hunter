#include "inc/mc.h"
#include "inc/mui.h"
#include "inc/popup.h"
#include "inc/zslib.h"

#if GOLD_ZIP == 0
uLong AddDate()
{
  TDate date;
  TTime time;
  GetDateTime(&date, &time);

  if (date.year>1980)
    date.year-=1980;
  else if (date.year>80)
    date.year-=80;
  return
	  (uLong) (((date.day) + (32 * (date.month)) + (512 * date.year)) << 16) |
		((time.sec/2) + (32* time.min) + (2048 * (uLong)time.hour));
}


int getFileCrc2(const char* filenameinzip,unsigned long* result_crc)
{
   unsigned long calculate_crc=0;

   unsigned long size_buf = BUF_SIZE;
   void *buf = malloc(BUF_SIZE);

   int err2 = ZIP_OK;
   int fin = _open(filenameinzip, A_ReadOnly + A_BIN, P_READ, &err);
   unsigned long size_read = 0;
   if (fin== 0)
       err2 = ZIP_ERRNO;

    if (err2 == ZIP_OK)
        do
        {
            err2 = ZIP_OK;
            size_read = _read(fin, buf, size_buf, &err);

            if (size_read>0) calculate_crc = crc32(calculate_crc,buf,size_read);

        }while ((err2 == ZIP_OK) && (size_read>0));

    if (fin) _close(fin, &err);;

    *result_crc=calculate_crc;
    mfree(buf);
    return err2;
}

int Copy2Zip(zipFile file, char* fnamezip, char* fname, int level, char *password)
{
  int attr=0;
  int cb, left;
  zip_fileinfo zi;

  GetFileAttrib(fname, (unsigned char*)&attr, &err);
  zi.external_fa = attr;
  zi.dosDate = AddDate();

  char temp[256];
  uni2dos(fnamezip, temp, 256);

  strreplace(temp, '\\', '/');

  incfname(GetFileName(fname));

  int ret=Z_OK;

  if(attr & FA_DIRECTORY)
  {
    strcat(temp, "/");
    ret = zipOpenNewFileInZip3(file, temp,
                               &zi, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0);
    zipCloseFileInZip(file);
    return 1;
  }

  if(ret==Z_OK)
  {

    uLong crc=0;
    if(strlen(password)!=0) getFileCrc2(fname, &crc);

    ret = zipOpenNewFileInZip3(file, temp,
                                &zi, 0,0, 0,0, 0,
                                (level != 0) ? Z_DEFLATED : 0,
                                level,0,-MAX_WBITS, DEF_MEM_LEVEL,Z_DEFAULT_STRATEGY,
                                strlen(password)!=0?password: 0, crc);
    int fi = -1;
    if (fname!= 0) fi = _open(fname, A_ReadOnly + A_BIN, P_READ, &err);

    if (fi != -1)
    {
      left = _lseek(fi, 0, S_END, &err, &err);
      _lseek(fi, 0, S_SET, &err, &err);
      char *buff = malloc(BUF_SIZE);
      if (left)
      {
        if (!buff) goto L_EXIT;
        progrsp_start = 1;
        progrsp_max = left;
        incprogrsp(0);
      }
      while (left && !progr_stop)
      {
        cb = left < BUF_SIZE ? left : BUF_SIZE;
        left -= cb;
        incprogrsp(cb);

        if (_read(fi, buff, cb, &err) != cb) goto L_EXIT;
        if (zipWriteInFileInZip(file, buff, cb)!=UNZ_OK) goto L_EXIT;
      }
      endprogrsp();
      if(buff) mfree(buff);
    }else
      return 0;
    L_EXIT:
    if (fi !=- 1) _close(fi, &err);
    zipCloseFileInZip(file);
    progrsp_start = 0;
    return 1;
  }else
    return 0;
}

int CreateZip(ZPARAMS *ZipP)
{
  if(fexist(ZipP->destname))
  {
    int ret = MsgBoxYesNoWithParam(ind_pmt_exists, strrchr(ZipP->destname,'\\')+1);
    if (ret == IDNO || ret == IDNO2ALL)
    {
      ParseFileNameStruct fns;
      parse_name(ZipP->destname, &fns);

      if (find_next_name(&fns))
	sprintf(ZipP->destname, _s_i_s, fns.cleanName, fns.number, fns.cleanExt);
    }
  }

  zlib_filefunc_def ffunc;
  fill__open_filefunc(&ffunc);

  zipFile dzip = zipOpen2(ZipP->destname, APPEND_STATUS_CREATE,  0, &ffunc);
  if (dzip== 0)
    return -1;

  FN_ITM* itm = buffer.items;

  int isnewprogr = 0;
  if (!progr_start)
  {
    initprogr(ind_do_arch);

    while(itm)
    {
      if (itm->ftype == TYPE_COMMON_DIR || itm->ftype == TYPE_COMMON_FILE)
        progr_max += GetFilesCnt(itm->full);
      itm = itm->next;
    }

    incprogr(0);
    isnewprogr = 1;
  }

  itm = buffer.items;
  int res;
  while(itm && !progr_stop)
  {
    char *sname = itm->full + itm->pname;
    incfname(sname);
    sprintf(pathbuf, _s, sname);

    char* fname = pathbuf[0] == '\\' ? pathbuf + 1 : pathbuf;
    strreplace(fname, '\\', '/');

    res &= ZipCopy(dzip, fname, itm->full, ZipP->password);

    itm=itm->next;
  }

  zipClose(dzip,  0);
  fn_free(&buffer);

  if(progr_stop)
    fsrm(ZipP->destname, 0);
  else
  {
    DoRefresh();
    // Устанавливаем курсор на созданном архиве
    int ind = GetCurTabFileIndex(GetFileName(ZipP->destname));
    SetCurTabIndex(ind, 0);
  }
  if (isnewprogr)
  {
    endprogr();
    progr_start = 0;
  }
  return 0;
}

int Copy2Zip2(zipFile file, char* fnamezip, char* fname, char *pass)
{return Copy2Zip(file, fnamezip, fname, Z_BEST_COMPRESSION, pass);}

int cp2ziptree(zipFile file, char* src, char* dst, char *pass)
{
  FN_LIST fnlist;
  fn_zero(&fnlist);
  fn_fill(&fnlist, src);
  fn_rev(&fnlist);

  char dstfull[MAX_PATH];
  int psrc = strlen(src)+1;
  int res = 1;

  FN_ITM *itm = fnlist.items;

  while(itm && !progr_stop)
  {
    if (itm->ftype == TYPE_COMMON_DIR) // TODO: ZIP_DIR...
    {
      char* pdst;
      if (itm->full[psrc-1])
      {
        char* psrcname = itm->full+psrc;
	sprintf(dstfull, _s_s, dst, psrcname);
	pdst = dstfull;
      }else
	pdst = dst;

      res &= Copy2Zip2(file, pdst, itm->full, pass);
      incprogr(1);
    }
    itm=itm->next;
  }

  itm = fnlist.items;
  while(itm && !progr_stop)
  {
    if (itm->ftype == TYPE_COMMON_FILE) // TODO: ZIP_FILE...
    {
      char* psrcname = itm->full+psrc;
      sprintf(dstfull, _s_s, dst, psrcname);
      res &= Copy2Zip2(file, dstfull, itm->full, pass);
      incprogr(1);
    }
    itm=itm->next;
  }

  fn_free(&fnlist);
  return res;
}

int ZipCopy(zipFile file, char* dst, char* src, char *pass)
{
  int res=1;
  if (isdir2(src, &err))
    res &= cp2ziptree(file, src, dst, pass);
  else
  {
    res &= Copy2Zip2(file, dst, src, pass);
    incprogr(1);
  }
  return res;
}

int M_ZipDel(FILEINF *file, int param)
{
  int pname = strlen(_CurPath) + 1;

  CurFullPath(file);
  if(file->attr & FA_CHECK)file->attr = file->attr & ~FA_CHECK;
  fn_add(&buffer, param, file->ftype, pname, pathbuf, IsInZip() ? _CurTab->zipInfo->szZipPath :  0, file->sname);

  if(file->ftype == TYPE_ZIP_DIR)
    EnumZipFiles(_CurTab->zipInfo, pathbuf, M_ZipDel, param);

  return 1;
}

int fn_inlist2del(FN_LIST *list, char *full)
{
	if (list== 0 || !list->count) return 0;

	FN_ITM *itm = list->items;
	while(itm)
	{
            if(full[strlen(full)-1]=='\\') full[strlen(full)-1]=0;
            if(itm->full[strlen(itm->full)-1]=='\\') itm->full[strlen(itm->full)-1]=0;

            if (!strcmp(itm->full[0]=='\\'?itm->full+1:itm->full, full[0]=='\\'?full+1:full))
              return 1;

            itm=itm->next;
	}
	return 0;
}

int DeleteFileFromZIP()
{
  EnumSel(M_ZipDel, FNT_DEL);

  int ret=0;
  //copying files
  int n_files = 0;
  int rv;
  int some_was_del = 0;

  char* zipname = (char*)malloc((strlen(zippathbuf) + 1));
  char* tmp_name = (char*)malloc((strlen(zippathbuf) + 5));
  strcpy(zipname, zippathbuf);
  strcpy(tmp_name, zippathbuf);
  strncat(tmp_name, ".tmp", 5);

  unzFile szip = unzOpen(zipname); // Открываем архив для чтения
  zlib_filefunc_def ffunc;
  fill__open_filefunc(&ffunc);
  zipFile dzip = zipOpen2(tmp_name, APPEND_STATUS_CREATE,  0, &ffunc);  // Создаем темповский архив в который будем переностить файлы

  unz_global_info glob_info;
  if (unzGetGlobalInfo(szip, &glob_info) != UNZ_OK) // считываем информацию
  {
    ret = 1;
    goto end1;
  }

  if(!progr_start)
  {
    // запускаем прогресс бар, если уже не запущен другой
    Busy = 1;
    initprogr(ind_do_arch);
    progr_max = glob_info.number_entry;
    incprogr(0);
    incfname(strrchr(zipname,'\\')+1);
  }

  rv = unzGoToFirstFile(szip);
  while (rv == UNZ_OK)
  {
    // get zipped file info
    unz_file_info unzfi;

    char dos_fn[MAX_PATH];
    if (unzGetCurrentFileInfo(szip, &unzfi, dos_fn, MAX_PATH,  0, 0,  0, 0) != UNZ_OK) break;

    char fn[MAX_PATH];
    dos2utf8_2(dos_fn, fn);

    strreplace(fn, '/', '\\');

    if(fn_inlist2del(&buffer, fn))
      some_was_del ++;
    else
    {
      char* extrafield = (char*)malloc(unzfi.size_file_extra);
      char* commentary = (char*)malloc(unzfi.size_file_comment);

      if (unzGetCurrentFileInfo(szip, &unzfi, dos_fn, MAX_PATH, extrafield, unzfi.size_file_extra, commentary, unzfi.size_file_comment) != UNZ_OK)
      {
        mfree(extrafield);
        mfree(commentary);
        break;
      }

      zip_fileinfo zfi;
      memcpy (&zfi.tmz_date, &unzfi.tmu_date, sizeof(tm_unz));
      zfi.dosDate = unzfi.dosDate;
      zfi.internal_fa = unzfi.internal_fa;
      zfi.external_fa = unzfi.external_fa;

      int method;
      int level;
      if (unzOpenCurrentFile2(szip, &method, &level, 1)!=UNZ_OK)
      {
        mfree(extrafield); mfree(commentary);
        break;
      }

      int size_local_extra = unzGetLocalExtrafield(szip,  0, 0);
      void* local_extra = malloc(size_local_extra);
      if (unzGetLocalExtrafield(szip, local_extra, size_local_extra)<0)
      {
        mfree(extrafield);
        mfree(commentary);
        mfree(local_extra);
        break;
      }
      zipOpenNewFileInZip2(dzip, dos_fn, &zfi, local_extra, size_local_extra, extrafield, unzfi.size_file_extra, commentary, method, level, 1);

      //////////////////////////////////
      progrsp_start = 1;
      progrsp_max = unzfi.compressed_size;
      incprogrsp(0);
      //////////////////////////////////

      void* buf = malloc(BUF_SIZE);
      int cb = 0;
      do
      {
        cb = (int)unzReadCurrentFile(szip, buf, BUF_SIZE);
        if (cb > 0)
        {
          zipWriteInFileInZip(dzip, buf, cb);
          incprogrsp(cb);
        }
      }
      while (cb > 0);

      ////////////////////////////////
      endprogrsp();
      progrsp_start = 0;
      ////////////////////////////////

      zipCloseFileInZipRaw(dzip, unzfi.uncompressed_size, unzfi.crc);
      unzCloseCurrentFile(szip);

      mfree(commentary);
      mfree(buf);
      mfree(extrafield);
      mfree(local_extra);

      n_files ++;
    }

    rv = unzGoToNextFile(szip);

    incprogr(1);
  }

////////////////////////////////////////////////////////////////////////////////
end1:
  zipClose(dzip,  0);
  unzClose(szip);
  strcpy(pathbuf_fn, _CurTab->zipInfo->szCurDir);
  ExitFromZip();
  fn_free(&buffer);

  _unlink(zipname, &err);
  if(n_files==0)
    _unlink(tmp_name, &err);
  else
    fmove(tmp_name, zipname, &err);

  if(n_files>0)
  {
    strcpy(zippathbuf, zipname);
    S_ZipOpen();
    strcpy(_CurTab->zipInfo->szCurDir, pathbuf_fn);
  }

  DoRefresh();

  if(progr_start)
  {
    endprogr(); // останавливаем прогресс бар
    Busy = 0;
  }

  mfree(zipname);
  mfree(tmp_name);

  return ret;
}

#endif

