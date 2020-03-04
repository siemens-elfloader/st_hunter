#include "..\..\inc\swilib.h"

const int minus11=-11; // стремная константа =)

unsigned short maincsm_name_body[140];

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


/*
  Структура, описывающая заголовок файла формата GPF.
  Находится по смещению 0 байт от начала GPF-файла
*/
typedef struct
{
  char signature[16];     // = "GraphPatchFile"
  unsigned short picnum;  // номер этой картинки в PIT
  unsigned short unk1;    // это я не понял ;)
  char w;                 // Ширина
  char h;                 // Высота
  char Compr_Bits;        // Компрессия и цветность (всё-в-одном)
                          // For BW=1, 8bit=5, 16bit=8, 0x80 - packed (из swilib.h)
}PICHDR;


/*
  Отрисовка картинки из буфера IMGHDR
*/
DrwImg(IMGHDR *img, int x, int y, int *pen, int *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}


/*
  Эта процедура рисует на экране картинку из файла формата GPF
  Сам GPF-файл в принципе может лежать где угодно и может быть 
  заблокирован в настоящий момент. Используется собственный декодер.
*/
void DrawFromGPF()
{
  char filename[]="0:\\test.gpf";
  volatile int hFile;
  PICHDR Pic_Header;
  unsigned int io_error = 0;
  // Тестовая картинка - иконка будильника, в RAMPIT ставится под номером 0x3
  hFile = fopen(filename, A_ReadOnly + A_BIN, P_READ, &io_error);
  if(!io_error)
  {
    // читаем заголовок файла
    fread(hFile, &Pic_Header, sizeof(Pic_Header), &io_error);
    // Получаем размер битовой карты картинки
    unsigned int buf_size = lseek(hFile, 0, S_END, &io_error, &io_error) - sizeof(PICHDR);
    lseek(hFile, sizeof(PICHDR), S_SET, &io_error, &io_error); 

    // Выделяем память под буфер. Причём с округлением до четырёх байт
    char *pic_buffer = malloc((buf_size+3)&(~3));
    
    // Читаем картинку в буфер и закрываем файл
    fread(hFile, pic_buffer, buf_size, &io_error);    
    fclose(hFile, &io_error);
    
    // Создаем переменную типа IMGHDR и заполняем её
    IMGHDR img;
    img.w = Pic_Header.w;
    img.h = Pic_Header.h;
    img.bpnum = Pic_Header.Compr_Bits;
    img.zero = 0;
    img.bitmap = pic_buffer;
    
    // Вызываем отрисовку
    DrwImg(&img, 0, 60, GetPaletteAdrByColorIndex(1), GetPaletteAdrByColorIndex(0));
    
    // Освобождаем память
    mfree(pic_buffer);
  }
}


/*
  Рисует картинку из PIT-таблицы.
  Требуется указать только номер и координаты вывода,
  всё остальное (включая битовую карту и инфу о сжатии
  уже предоставляет PIT-таблица.
  Замечу, что под PIT-таблицей здесь понимается стандартная
  PIT, расширенная и изменённая с помощью патча "Дополнение к PIT-таблице"
*/
void DrawFromPIT()
{
  // номер загружаемой картинки
  unsigned short picnum = 0xC6;
  
  // Стандартная отрисовка из PIT
  DrawImg(0,60, picnum);
}

void OnRedraw(MAIN_GUI *data) // OnRedraw
{
  
  DrawRoundedFrame(0,0,131,175,0,0,0,
			GetPaletteAdrByColorIndex(0),
			GetPaletteAdrByColorIndex(20));
  WSHDR *ws = AllocWS(256); // Создаём юникод-строку
  wsprintf(ws, "Press 1 to draw pic from GPF-file!\r\n Press 2 to draw Pic from std.PIT"); // Пихаем в неё текст
  DrawString(ws,1,1,128,172,7,1,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23)); // Отрисовываем
  FreeWS(ws); // Освобождаем память
}

void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int)) //Create
{
  // Тут можно создать переменные
  data->gui.state=1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *)) //Close
{
  // Тут можно освободить выделяемую память
  data->gui.state=0;
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))//Focus
{
  data->gui.state=2;
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *)) //Unfocus
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

/*
  Обработчик нажатий клавиш. Сюда передаются нажатия клавиш
  в виде сообщения GUI_MSG, пример декодирования ниже.

!!! Тут добавлена обработка нажатия клавиши 1, 2
*/
int OnKey(MAIN_GUI *data, GUI_MSG *msg) //OnKey
{
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
    case RIGHT_SOFT:
      return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
      
    case '1': {DrawFromGPF();break;} 
    case '2': {DrawFromPIT();break;} 
    }
  }
  return(0);
}

void onDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  // Это оставить как есть ;)
  mfree_adr(data);
}

// Оставить как есть
int method8(void){return(0);}

// Оставить как есть
int method9(void){return(0);}

// Массив с методами, чтобы дать ОС информацию, какие когда вызывать
const void * const gui_methods[11]={
  (void *)OnRedraw,	//Redraw
  (void *)onCreate,	//Create
  (void *)onClose,	//Close
  (void *)onFocus,	//Focus
  (void *)onUnfocus,	//Unfocus
  (void *)OnKey,	//OnKey
  0,
  (void *)onDestroy,	//Destroy
  (void *)method8,
  (void *)method9,
  0
};

// Канвас для основного GUI
const RECT Canvas={0,0,131,175};

// Вызывается при создании главного CSM. В данном примере
// создаётся GUI, его ID записывается в MAINGUI_ID
// на всякий случай - вдруг понадобится ;)
void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  MAINGUI_ID=csm->gui_id;
}

// Всё ясно из названия ;) оставить как есть
void ElfKiller(void)
{
  extern void kill_data(void *p, void (*func_p)(void *));
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

// Вызывается при закрытии главного CSM. Тут и вызывается киллер
void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)ElfKiller);
}

// Обработчик событий главного CSM
int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  return(1);
}

// Инициализация структуры MAINCSM
const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
} MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
  0,
  0,
  0,
  0,
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

// Функция, которая устанавливает название этого CSM для X-Task.
void UpdateCSMname(void)
{
  WSHDR *ws=AllocWS(256);
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"HelloWorld");
  FreeWS(ws);
}


// Основная процедура. Она первой получает управление при старте эльфа.
int main(char *exename, char *fname)
{
  char dummy[sizeof(MAIN_CSM)];
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  return 0;
}

