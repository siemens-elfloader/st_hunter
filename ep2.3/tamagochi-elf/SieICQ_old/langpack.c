#include "include.h"
#include "langpack.h"


int loaded = NULL;

extern char elf_path[256];

void LoadLanguage()
{
  FreeLanguage();
  loaded = NULL;
  volatile int hFile;
  unsigned int io_error = 0;
  char * buf; // Буфер под файл
  char line[128]; // Текущая строка
  int line_size = 0; // Длина текущей строки
  int cur_id=0; // Текущий LGP_ID
  int buf_pos = 0; // Позиция в буфере
  FSTATS fstat;
  char * lang_file =  malloc(256);
  sprintf(lang_file,"%s\\lang.txt",elf_path);
  if (GetFileStats(lang_file, &fstat, &io_error)!=-1) // Получаем размер файла
  {
    if((hFile=fopen(lang_file, A_ReadOnly + A_BIN, P_READ, &io_error))!=-1) // Открываем файл для чтения
    {
      if (buf = malloc(fstat.size+1)) // Выделяем память под буфер
      {
        buf[fread(hFile, buf, fstat.size, &io_error)]=0; // Читаем файл в буфер

        while(buf[buf_pos] && buf_pos < fstat.size && cur_id < LGP_DATA_NUM) // Пока не конец файла и не заполнены все поля массива
        {
          if (buf[buf_pos]=='\n' || buf[buf_pos]=='\r') // Если конец строки
          {
            if (line_size) // Если длина строки > 0
            {
              LangPack[cur_id] =malloc(line_size + 1); // Выделяем память в массиве под строку
              memcpy(LangPack[cur_id], line, line_size); // Копируем строку в массив
              LangPack[cur_id][line_size] = NULL; // Конец строки
              cur_id ++;
              line_size = NULL;
            }
          }
          else
            line[line_size++]=buf[buf_pos]; // Добавляем в строку символы из буфера, пока не конец сроки
          buf_pos++;
        }
        if (line_size && cur_id < LGP_DATA_NUM) // eof
        {
          LangPack[cur_id] = malloc(line_size + 1);
          memcpy(LangPack[cur_id], line, line_size);
          LangPack[cur_id][line_size] = NULL;
          cur_id ++;
          line_size = NULL;
        }
        mfree(buf); // Удаляем буфер. он нам уже не понадобится
        loaded = 1;
        fclose(hFile, &io_error);
        for (int i = 0; i < LGP_DATA_NUM; i ++)
        {
          if (!LangPack[i])
          {
            LangPack[i] = malloc(64);
            strcpy(LangPack[i], "Error! Update lang.txt!");
          }
        }
        return;
      }
      fclose(hFile, &io_error);
    }
  }
  mfree(lang_file);
  // Если по каким-то причинам файл нельзя прочитать, то устанавливаем английский ленгпак
// меню  
  LangPack[LGP_Connect] = "Connect";
  LangPack[LGP_SetStatus] = "Change status";
  LangPack[LGP_CList] = "Contact list";
  LangPack[LGP_Option] = "Options";
  LangPack[LGP_About] = "About";
  LangPack[LGP_Exit] = "Exit";
//  


}

void FreeLanguage()
{
  if (loaded)
  {
    for (int i = 0; i < LGP_DATA_NUM; i ++)
      mfree(LangPack[i]);
  }
  else
  {
    for (int i = 0; i < LGP_DATA_NUM; i ++)
       LangPack[i] = NULL;
  }
  loaded = NULL;
}
