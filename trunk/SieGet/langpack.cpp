#include "include.h"
#include "langpack.h"
#include "file_works.h"

/*
  LangPack
  Функции взяты из эльфа BalletMini и чуть подправлены для увеличения
  быстродействия обработки файла ;)
*/

LangPack * LangPack::Active = 0;

LangPack::LangPack()
{
  for (int i = 0; i < LGP_DATA_NUM; i ++)
    data[i] = 0;
  loaded = 0;
  Setup();
}

LangPack::~LangPack()
{
  Free();
}

void LangPack::Setup()
{
  Free();
  Active = this;
  volatile int hFile;
  unsigned int io_error = 0;
  char * buf; // Буфер под файл
  char line[128]; // Текущая строка
  int line_size = 0; // Длина текущей строки
  int cur_id=0; // Текущий LGP_ID
  int buf_pos = 0; // Позиция в буфере
  FSTATS fstat;
  char * lang_file = getSymbolicPath("$sieget\\lang.txt");
  if (GetFileStats(lang_file, &fstat, &io_error)!=-1) // Получаем размер файла
  {
    if((hFile=_open(lang_file, A_ReadOnly + A_BIN, P_READ, &io_error))!=-1) // Открываем файл для чтения
    {
      if (buf = new char[fstat.size+1]) // Выделяем память под буфер
      {
        buf[_read(hFile, buf, fstat.size, &io_error)]=0; // Читаем файл в буфер

        while(buf[buf_pos] && (unsigned int)buf_pos < fstat.size && (unsigned int)cur_id < LGP_DATA_NUM) // Пока не конец файла и не заполнены все поля массива
        {
          if (buf[buf_pos]=='\n' || buf[buf_pos]=='\r') // Если конец строки
          {
            if (line_size) // Если длина строки > 0
            {
              data[cur_id] = new char[line_size + 1]; // Выделяем память в массиве под строку
              memcpy(data[cur_id], line, line_size); // Копируем строку в массив
              data[cur_id][line_size] = 0; // Конец строки
              cur_id ++;
              line_size = 0;
            }
          }
          else
            line[line_size++]=buf[buf_pos]; // Добавляем в строку символы из буфера, пока не конец сроки
          buf_pos++;
        }
        if (line_size && cur_id < LGP_DATA_NUM) // eof
        {
          data[cur_id] = new char[line_size + 1];
          memcpy(data[cur_id], line, line_size);
          data[cur_id][line_size] = 0;
          cur_id ++;
          line_size = 0;
        }
        delete buf; // Удаляем буфер. он нам уже не понадобится
        loaded = 1;
        _close(hFile, &io_error);
        for (int i = 0; i < LGP_DATA_NUM; i ++)
        {
          if (!data[i])
          {
            data[i] = new char[64];
            strcpy(data[i], "Error! Update lang.txt!");
          }
        }
        return;
      }
      _close(hFile, &io_error);
    }
  }
  delete lang_file;
  // Если по каким-то причинам файл нельзя прочитать, то устанавливаем английский ленгпак
  data[LGP_Options] = (char*)"Options";
  data[LGP_Select] = (char*)"Select";
  data[LGP_Back] = (char*)"Back";
  data[LGP_Close] = (char*)"Close";
  data[LGP_Cancel] = (char*)"Cancel";
  data[LGP_Error] = (char*)"Error";
  data[LGP_Unknown] = (char*)"Unknown";
  data[LGP_Bookmarks] = (char*)"Bookmarks";
  data[LGP_SelectFolder] = (char*)"Select folder";
  data[LGP_NewDownload] = (char*)"New download";
  data[LGP_StartDownload] = (char*)"Start";
  data[LGP_PauseDownload] = (char*)"Stop";
  data[LGP_DeleteDownload] = (char*)"Delete";
  data[LGP_DeleteSuccesfulDownload] = (char*)"Delete succesful";
  data[LGP_Settings] = (char*)"Settings";
  data[LGP_About] = (char*)"About";
  data[LGP_ViewLog] = (char*)"View log";
  data[LGP_EditBookmark] = (char*)"Edit bookmark";
  data[LGP_AddBookmark] = (char*)"Add bookmark";
  data[LGP_RenameFolder] = (char*)"Rename folder";
  data[LGP_AddFolder] = (char*)"Add folder";
  data[LGP_DeleteBookmark] = (char*)"Delete bookmark";
  data[LGP_DeleteFolder] = (char*)"Delete folder";
  data[LGP_DownloadState] = (char*)"Download state:";
  data[LGP_FileSize] = (char*)"File size:";
  data[LGP_LoadedSize] = (char*)"Loaded size:";
  data[LGP_OpenFile] = (char*)"Open file";
  data[LGP_Sended] = (char*)"Sended:";
  data[LGP_Received] = (char*)"Received:";
  data[LGP_TotalSended] = (char*)"Total sended:";
  data[LGP_TotalReceived] = (char*)"Total received:";
  data[LGP_Waiting] = (char*)"Waiting";
  data[LGP_Connecting] = (char*)"Connecting";
  data[LGP_GettingInfo] = (char*)"Getting info";
  data[LGP_Downloading] = (char*)"Downloading";
  data[LGP_Completed] = (char*)"Completed";
  data[LGP_Stopped] = (char*)"Stopped";
  data[LGP_Info] = (char*)"Info";
  data[LGP_Traffic] = (char*)"Traffic";
  data[LGP_Name] = (char*)"Name:";
  data[LGP_DownloadFolder] = (char*)"Download to:";
  data[LGP_FileName] = (char*)"File name:";
  data[LGP_CantReadBookmark] = (char*)"Can't read bookmark!";
  data[LGP_FolderNotEmpty] = (char*)"Folder is not empty!";
  data[LGP_EnterName] = (char*)"Enter name, please!";
  data[LGP_EnterUrl] = (char*)"Enter URL, please!";
  data[LGP_EnterDownloadsFolder] = (char*)"Enter downloads folder, please!";
  data[LGP_FolderExists] = (char*)"This folder is allready exists!";
  data[LGP_FileExists] = (char*)"This file is allready exists!";
  data[LGP_SocketConnected] = (char*)"Socket connected";
  data[LGP_SocketRemoteClosed] = (char*)"Socket remote closed";
  data[LGP_SocketClosed] = (char*)"Socket closed";
  data[LGP_SocketCreateError] = (char*)"Socket create error!";
  data[LGP_SocketConnectError] = (char*)"Socket connect error!";
  data[LGP_SocketSendError] = (char*)"Socket send error!";
  data[LGP_SocketCloseError] = (char*)"Socket close error!";
  data[LGP_InvalidSocket] = (char*)"Invalid socket!";
  data[LGP_EnableGPRSFirst] = (char*)"Enable GPRS first!";
  data[LGP_DownloadStarted] = (char*)"Download started";
  data[LGP_DownloadStopped] = (char*)"Download stopped";
  data[LGP_DownloadCompleted] = (char*)"Download completed";
  data[LGP_ResolvingHost] = (char*)"Resolving host...";
  data[LGP_WaitDNR] = (char*)"DNR not ready! Wait 5 seconds...";
  data[LGP_DNRError] = (char*)"DNR Error: %d";
  data[LGP_DNROutOfTries] = (char*)"DNR out of tries!";
  data[LGP_ConnectToIPPort] = (char*)"Connecting to IP %d.%d.%d.%d, port %d";
  data[LGP_HTTPParserReturned] = (char*)"HTTP Parser returned %d b (data size %d b)";
  data[LGP_GotResponseCode] = (char*)"Got response code: %d";
  data[LGP_GotResponseReason] = (char*)"Got response reason: \"%s\"";
  data[LGP_RangesNotSupportFileDelete] = (char*)"Ranges not supported, file will be deleted!";
}

void LangPack::Free()
{
  if (loaded)
  {
    for (int i = 0; i < LGP_DATA_NUM; i ++)
      _safe_delete(data[i]);
    loaded = 0;
  }
}
