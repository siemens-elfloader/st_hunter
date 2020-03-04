#include "inc\mc.h"
#include "inc\7zslib.h"
#include "inc\mui.h"
#include "inc\Encode.h"
#include "inc\popup.h"

const char _7z_ext[] = "7z";

int IsIt7ZipFile(char* fname)
{
if(IsInArchive())
  return (stricmp(GetFileExt(fname), (char*)_7z_ext) == 0);
else
{
 if (CONFIG_7Z_DETECT_BY == 0) // by content
 {
  unsigned char buf[k7zSignatureSize];
  int f = fopen(fname, A_ReadOnly + A_BIN, P_READ, &err);
  if (f != -1)
  {
   if (fread(f, &buf, k7zSignatureSize, &err) == k7zSignatureSize)
   fclose(f, &err);
  }
  return (memcmp(buf,k7zSignature,k7zSignatureSize)==0);
 }
 else // by extension
  return (stricmp(GetFileExt(fname), (char*)_7z_ext) == 0);
}
}

#ifdef _LZMA_IN_CB

#define kBufferSize (1 << 12)
Byte g_Buffer[kBufferSize];

SZ_RESULT SzFileReadImp(void *object, void **buffer, size_t maxRequiredSize, size_t *processedSize)
{
  CFileInStream *s = (CFileInStream *)object;
  size_t processedSizeLoc;
  if (maxRequiredSize > kBufferSize)
    maxRequiredSize = kBufferSize;
  processedSizeLoc = fread(s->File, g_Buffer, maxRequiredSize, &err);

  *buffer = g_Buffer;
  if (processedSize != 0)
    *processedSize = processedSizeLoc;
  return SZ_OK;
}

#else

SZ_RESULT SzFileReadImp(void *object, void *buffer, size_t size, size_t *processedSize)
{
  CFileInStream *s = (CFileInStream *)object;
  size_t processedSizeLoc = fread(s->File, buffer, size, &err);
  if (processedSize != 0)
    *processedSize = processedSizeLoc;
  return SZ_OK;
}

#endif

SZ_RESULT SzFileSeekImp(void *object, CFileSize pos)
{
  CFileInStream *s = (CFileInStream *)object;
  lseek(s->File, (long)pos, SEEK_SET, &err, &err);
  return SZ_OK;
}

int Open7ZFile(ARCHINFO *arc, char *zipFileName)
{
  int zerr = UNZ_ERRNO;

  {
    _7zinfo *zinfo=&arc->info._7z;
    zinfo->archiveStream.File = fopen(zipFileName, A_ReadOnly + A_BIN, P_READ, &err);
    arc->total_names=0;
    if (zinfo->archiveStream.File < 0)
      return 1;

    zinfo->archiveStream.InStream.Read = SzFileReadImp;
    zinfo->archiveStream.InStream.Seek = SzFileSeekImp;

    zinfo->allocImp.Alloc = SzAlloc;
    zinfo->allocImp.Free = SzFree;

    zinfo->allocTempImp.Alloc = SzAllocTemp;
    zinfo->allocTempImp.Free = SzFreeTemp;
    InitCrcTable();
    SzArDbExInit(&zinfo->db);
    zerr = SzArchiveOpen(&zinfo->archiveStream.InStream, &zinfo->db, &zinfo->allocImp, &zinfo->allocTempImp);

    if (zerr == SZ_OK)
    {
      arc->type=_7Z_ARCH;
      arc->szCurDir[0] = '\0';
      strcpy(arc->szZipPath, zipFileName);

      if (zinfo->db.Database.NumFiles >0)
      {
        arc->total_names=zinfo->db.Database.NumFiles;
        int useProgressBar = !progr_start;
        if (useProgressBar)
        {
          // запускаем прогресс бар, если уже не запущен другой
          Busy = 1;
          initprogr(ind_msg_zreading);
          progr_max = arc->total_names;
          incprogr(0);
        }
        int allocsize = arc->total_names * sizeof(char*);
        arc->pszNames = malloc(allocsize);
        if (arc->pszNames)
          zeromem(arc->pszNames, allocsize);

        int i=0;
        while(i < arc->total_names && !progr_stop)
        {
          if (!msgopen)
          {
            CFileItem *f = zinfo->db.Database.Files + i;

            arc->pszNames[i] = malloc(strlen(f->Name)+33);

            // Фиксим слеши
            strreplace(f->Name, '/', '\\');
            // Переводим
            int len= utf82filename(f->Name, arc->pszNames[i]);
            arc->pszNames[i][len]=0;

            if (useProgressBar && (i%100)==0) incprogrset(i);

            i++;
          }
        }

        if (useProgressBar)
        {
          endprogr(); // останавливаем прогресс бар
          Busy = 0;
        }
      }
    }
  }
  if (zerr != SZ_OK)
    Close7Z(arc);
  return zerr;
}

void Close7Z(ARCHINFO* arc)
{
  if (arc->type==_7Z_ARCH)
  {
    _7zinfo *zinfo=&arc->info._7z;
    SzArDbExFree(&zinfo->db, zinfo->allocImp.Free);
    fclose(zinfo->archiveStream.File, &err);
    FreeArchInfo(arc);
  }
}

void CloseTab7Z(int tab)
{
  Close7Z(tabs[tab]->zipInfo);
}


int OpenTab7Z(int tab, char* zipFileName)
{
  int zerr = UNZ_ERRNO;

  if (!IsArchiveOpened(tab)) // не обрабатываем вложенные архивы
    zerr = Open7ZFile(tabs[tab]->zipInfo, zipFileName);

  return zerr;
}


int Fill7ZFiles(int tab, char* subdname)
{
  int num = 0;
  char buf[MAX_PATH_INZIP];
  char* pszDirName = subdname[0] == '\\' ? subdname + 1 : subdname; // Пропустим в начале обратный слеш
  int dirLen = strlen(pszDirName);
  ARCHINFO* arc = tabs[tab]->zipInfo;

  if (arc->type==_7Z_ARCH)
  {
    _7zinfo *zinfo=&arc->info._7z;
    for (int i=0; i < zinfo->db.Database.NumFiles; i++)
    {
      int ignore = 1;
      strcpy(buf, arc->pszNames[i]);
      char* pFileNameStart = &buf[0];
      if (dirLen == 0)
      {
        // Если ищем корневые элементы
        // То добавляем все файлы у которых нет слешей '\'
        char* slashPos = strstr(buf, "\\");
        ignore = (slashPos != NULL);
        if (ignore)
        {
          // И добавляем все директории - у них один слеш, и тот последний
          ignore = (*(slashPos + 1) != 0);
          if (!ignore)
          {
            // Затрем этот слеш, чтобы не выводился
            *slashPos = 0;
          }
        }
      }
      else
      {
        // Сравниваем начало, если равно и потом слеш - то это элементы нашей директории
        pFileNameStart = pFileNameStart + dirLen;
        ignore = (strnicmp(buf, pszDirName, dirLen) != 0 || *pFileNameStart != '\\');
        if (!ignore)
        {
          pFileNameStart = pFileNameStart + 1; // пропустим слеш после имени директории
          // игнорим саму текущую, директорию
          ignore = (*pFileNameStart == 0);
          if (!ignore)
          {
            // проверка на поддиректорию
            char* pFileNameEnd = strstr(pFileNameStart, "\\");
            if (pFileNameEnd != NULL)
            {
              // проверка на то что это не сама поддиректория а ее элементы, оные проигнорим
              ignore = (*(pFileNameEnd+1) != 0);
              if (!ignore)
              {
                *pFileNameEnd = 0; // грохнем слеш в конце
              }
            }
          }
        }
      }
      if (!ignore)
      {
        CFileItem *f = zinfo->db.Database.Files + i;
        AddFile(tab, i, pFileNameStart, f->Size, f->IsDirectory?FA_DIRECTORY:0, 0,
                0, f->IsDirectory? TYPE_7Z_DIR : TYPE_7Z_FILE, subdname);
        num++;
      }
    }
  }
  return num;
}

int ExtractFileByID7Z(ARCHINFO* pzi, int id, char* extractDir, int usePaths, int ip)
{
  int res=SZ_OK;
  int fout=-1;
  char *filePathInZip = pzi->pszNames[id];
  char extractFilePath[MAX_PATH];
  CFileItem *file = pzi->info._7z.db.Database.Files + id;

  if(!file)
    return -1;

  if (filePathInZip == NULL)
  {
    char temp[MAX_PATH];
    // Считываем имя сами если нужно
    strreplace(file->Name, '/', '\\');
    utf82filename(file->Name, temp);
    filePathInZip=temp;
  }

  if(((GetFreeRamAvail()-256*1024)<(file->Size*1.8)) && !(file->IsDirectory))
  {
    DoShowMsg(muitxt(ind_err_nomemory));
    return -1;
  }

  char* fileNameInZip=GetFileName(filePathInZip);
  if (ip && progr_start)incfname(fileNameInZip);
  if (file->IsDirectory)
  {
    // Директория... создаем
    sprintf(extractFilePath, _s_s, extractDir, filePathInZip);
    mktree(extractFilePath);
  }
  else
  {
    // File
    if (usePaths)
      sprintf(extractFilePath, _s_s, extractDir, filePathInZip);
    else
      sprintf(extractFilePath, _s_s, extractDir, fileNameInZip);

    if (fexist(extractFilePath))
    {
      if(CONFIG_TEMP_PATH!=extractDir)
      {
        if(CONFIG_CONFIRM_REPLACE)
        {
          int ret=MsgBoxYesNoWithParam(ind_pmt_exists, fileNameInZip);
          if (ret==IDYES && ret==IDYES2ALL)
            unlink(extractFilePath, &err);
          else
            return 0;
        }else
          unlink(extractFilePath, &err);
      }else
        unlink(extractFilePath, &err);
    }

    if (res == SZ_OK)
    {
      fout = fopen(extractFilePath, A_ReadWrite+A_Create+A_Truncate+A_BIN, P_READ+P_WRITE, &err);

      // some zipfile don't contain directory alone before file
      if (fout == -1 && usePaths && fileNameInZip != filePathInZip)
      {
        char c = *(fileNameInZip - 1);
        *(fileNameInZip - 1) = '\0';
        mktree(GetFileDir(extractFilePath, pathbuf));
        *(fileNameInZip - 1) = c;
        fout = fopen(extractFilePath, A_ReadWrite+A_Create+A_Truncate+A_BIN, P_READ+P_WRITE, &err);
      }

      if (fout<0)
        res = 3;
    }

    if (res == UNZ_OK)
    {
      int isnewprogr = 0;

      if (ip || !progrsp_start)
      {
        if (!progr_start)
        {
          initprogr(ind_msg_zreading);
          progr_max = 1;
          incprogr(0);
          isnewprogr = 1;
        }
        ip = 1;
        progrsp_start = 1;
        progrsp_max = file->Size;
        incprogrsp(0);
      }

      UInt32 blockIndex = 0xFFFFFFFF; // it can have any value before first call (if outBuffer = 0)
      Byte *outBuffer = 0; // it must be 0 before first call for each new archive.
      size_t outBufferSize = 0;  // it can have any value before first call (if outBuffer = 0)
      size_t offset;
      size_t outSizeProcessed;
      ISzAlloc allocImp;
      ISzAlloc allocTempImp;
      allocImp.Alloc = SzAlloc;
      allocImp.Free = SzFree;

      allocTempImp.Alloc = SzAllocTemp;
      allocTempImp.Free = SzFreeTemp;
      res = SzExtract(&pzi->info._7z.archiveStream.InStream, &pzi->info._7z.db, id,
                      &blockIndex, &outBuffer, &outBufferSize,
                      &offset, &outSizeProcessed,
                      &allocImp, &allocTempImp);
      fwrite(fout,outBuffer+offset,outSizeProcessed, &err);
      fclose(fout, &err);

      if (ip && progrsp_start)
      {
        endprogrsp();
        progrsp_start = 0;

        if (isnewprogr)
        {
          endprogr();
          progr_start = 0;
        }
      }

      if(outBuffer) mfree(outBuffer);
    }
  }

  if (ip && progr_start && res == UNZ_OK) incprogr(1);

  return res;
}

int ExtractFile7Z(ARCHINFO* pzi, char* fname, char* extractDir, int usePaths)
{
  // Позиционируемся на нужном файле
  int found = 0;
  char buf[MAX_PATH + 1];
  strcpy(buf + 1, fname);
  buf[0] = '\\'; // В начало запишем слеш на тот случай если fname без слеша

  char* nameWithoutSlash = ( (buf[1] == '\\') ? (buf + 2) : (buf + 1) );
  char* nameWithSlash = ( (buf[1] == '\\') ? (buf + 1) : (buf) );

  for (int i = 0; i < pzi->total_names; i++)
  {
    found = (stricmp(nameWithoutSlash, pzi->pszNames[i]) == 0);
    if (!found)
      // Пробуем поискать со слешем
      found = (stricmp(nameWithSlash, pzi->pszNames[i]) == 0);

    if (found)
      // Извлекаем его, если нашли
      return ExtractFileByID7Z(pzi, i, extractDir, usePaths, 1);
  }
  return 0;
}


int ExtractDir7Z(ARCHINFO* pzi, char* dname, char* extractDir, int usePaths)
{
  int found = 0;
  char buf[MAX_PATH + 1];
  strcpy(buf + 1, dname);
  buf[0] = '\\'; // В начало запишем слеш на тот случай если fname без слеша
  char* nameWithoutSlash = ( (buf[1] == '\\') ? (buf + 2) : (buf + 1) );
  char* nameWithSlash = ( (buf[1] == '\\') ? (buf + 1) : (buf) );

  int zerr = UNZ_OK;
  int dlen = strlen(nameWithoutSlash);

  // Перебираем все файлы
  for (int i = 0; i < pzi->total_names; i++)
  {
    // Проверяем начинается ли имя файла в зипе с этой директории, если да - извлекаем
    found = (strnicmp(nameWithoutSlash, pzi->pszNames[i], dlen) == 0);
    if (!found)
      // Пробуем поискать со слешем
      found = (strnicmp(nameWithSlash, pzi->pszNames[i], (dlen + 1)) == 0);

    if (found)
      ExtractFileByID7Z(pzi, i, extractDir, 1, 1);
  }
  return zerr;
}

int _Un7Z(void)
{
  int ret = 0;
  ARCHINFO *pzi=&zi;

  char *extractdir = malloc(256);
  strcpy(extractdir, zippathbuf);
  strreplace(extractdir, '.', '_');
  mktree(extractdir);

  initprogr(ind_copying);
  Open7ZFile(pzi, zippathbuf);
  progr_max = pzi->total_names;
  incprogr(0);

  // Перебираем все файлы
  int i=0;
  while(i<pzi->total_names && !progr_stop)
  {
    if (!msgopen)
    {
      ExtractFileByID7Z(pzi, i, extractdir, 1, 1);
      i++;
    }
  }

  Close7Z(pzi);
  DoRefresh();
  int ind = GetCurTabFileIndex(GetFileName(extractdir));
  SetCurTabIndex(ind, 0);
  endprogr();
  return ret;
}

int Enum7ZFiles(ARCHINFO* arc, char* subdname, ENUM_SEL_PROC enumproc, unsigned int param)
{
  unsigned int ccFiles   = 0;
  unsigned int ccSubDirs = 0;

  char buf[256];
  char * pszDirName = subdname[0] == '\\' ? subdname + 1 : subdname; // Пропустим в начале обратный слеш
  int dirLen = strlen(pszDirName);

  if (arc->type==_7Z_ARCH)
  {
    _7zinfo *zinfo=&arc->info._7z;
    for (int i=0; i < zinfo->db.Database.NumFiles; i++)
    {
      int ignore = 1;
      strcpy(buf, arc->pszNames[i]);

      char * pFileNameStart = &buf[0];
      if (dirLen == 0)
        ignore = 0;
      else
      {
        // Сравниваем начало, если равно и потом слеш - то это элементы нашей директории
        pFileNameStart = pFileNameStart + dirLen;
        ignore = (*pFileNameStart != '\\');
        if (!ignore)
        {
          pFileNameStart = pFileNameStart + 1; // пропустим слеш после имени директории
          // игнорим саму текущую, директорию
          ignore = (*pFileNameStart == 0);
        }
      }

      if (!ignore)
      {
        int stop = 0;
        CFileItem *f = zinfo->db.Database.Files + i;

        if (f->IsDirectory)
          ccSubDirs++;
        else
          ccFiles++;

        if (enumproc)
        {
          FILEINF* file = CreateFileInfo(i, pFileNameStart, f->Size,f->IsDirectory?FA_DIRECTORY:0, 0, 0,
                                       f->IsDirectory? TYPE_7Z_DIR : TYPE_7Z_FILE, pszDirName);


           if (enumproc(file, param) == 0)
            stop = 1;

           FreeFileInfo(file);
        }

        if (stop) break;
      }
    }
  }
  return (ccSubDirs << 16 | ccFiles);
}


