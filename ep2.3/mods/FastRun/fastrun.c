//////////////////// Обьявляем переменные /////////////////////////
#include "language.h"
#include "reviz.h"
#define idlegui_id (((int *)data)[DISPLACE_OF_IDLEGUI_ID/4])
#define SCREENSIZE ScreenH()*ScreenW()
#define HSIZE YDISP*ScreenW()
const int minus11=-11;

const char ipc_my_name[]=IPC_FASTRUN_NAME;

const IPC_REQ my_ipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};
int my_csm_id;

const char per_s[]="%s";
extern const char *successed_config_filename;
            
extern void kill_data(void *p, void (*func_p)(void *));
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
int EditConfig()
{
  extern const char *successed_config_filename;
  WSHDR *ws;
  ws=AllocWS(150);
  str_2ws(ws,successed_config_filename,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  return 1;
}

//--------------------------ПЕРЕМЕННЫЕ-------------------------//

#define TMR_SECOND 216// секунда
GBSTMR idle_timer;

int mode_enter;
int ConfigUpdated;
int PANEL_FS;//какая панель запущена
int sw=0;

char* percent_t="%t";
char* percent_s="%s";
unsigned char scrH, scrW;

const char white[]={0xFF,0xFF,0xFF,0x64};

void *canvasdata;

extern const int KEY_CHANGE_PANEL;
//----------------------Переменные для использования шрифта как в экстраинфо-----------------------//

extern const int align_1;
extern const int align_2;
extern const int align_3;
extern const int align_idle;
extern const int space;

extern const char FontPath[128];
extern const char FontPath2[128];
extern const char FontPath3[128];
extern const char FontPathIdle[128];
extern const int  fullset;

extern const int font_1;
extern const int font_2;
extern const int font_3;
extern const int font_idle;

//----------------------IDLE PANEL-----------------------//

extern const int IDLE_ENA;
             int IDLE_KEYHOOK_ENA;
extern const int IDLE_NAME_MODE;
extern const int FR_CALL_BUTTON;// Кнопка активации панели FastRun
extern const int FR_EXIT_BUTTON;// Кнопка закрытия панели FastRun
extern const int IDLE_CALL_BUTTON;// Кнопка активации панели FastRun
extern const int IDLE_EXIT_BUTTON;// Кнопка закрытия панели FastRun
//extern const int IDLE_CALL_BUTTON_2;// Кнопка активации панели FastRun
//extern const int IDLE_EXIT_BUTTON_2;// Кнопка закрытия панели FastRun
extern const int IDLE_PIC_SIZE;
extern const int ENA_IDLE_LONGPRESS;
extern const int IDLE_POSITION;//0-вертикально,1-Горизонтально
extern const unsigned int LEN_ID;
extern const int ENA_IDLE_LONGPRESS_2;
extern const int SHOW_NOT_ATC_IDLE_P;//Показываеть идле панель если та не активна
extern const int SHOW_KDB_LOCK;////Показывать только если клавиатура разблокирована
extern const unsigned int x1;
extern const unsigned int y1;
extern const unsigned int name_idle_x1;//Позиция для имени
extern const unsigned int name_idle_y1;

extern const char cfgPanBGCol_ID_ena[4];
extern const char cfgPanBGCol_ID_dis[4];
extern const char cfgBookBorderCol_ID[4];
extern const char cfgBookBGCol_ID[4];
extern const char cfgFontCol_ID[4];

int IdleMenuActive;
int NumOfItems_idle;// Число итемов меню 
int Selected_idle;// Позиция курсора
int idle_pic_size;
int FASTRUNMENU_GUI_IDLE_ID;

//--------------------------FASTRUN 1 PANEL-------------------------//

extern const int FR_PIC_SIZE;
extern const int ENA_FR_LONGPRESS;
extern const unsigned int LEN;
extern const int DRAW_MODE;
extern const int FASTRUN_ENA;
extern const unsigned int TEXT_FONT;
extern const int FASTRUN_NAME_1;
extern const int IDLE_1;//Запуск фастран панелей только на идле
             int FR_KEYHOOK_ENA;
             int fr_pic_size;
             int NumOfItems_fr;
             int Selected_fr;
             int FastRunMenuActive;// Флаг активности меню
extern const unsigned int x1_fr;
extern const unsigned int y1_fr;
             int FASTRUNMENU_GUI_ID;// ИД гуя
extern const unsigned int name_fr1_x1;
extern const unsigned int name_fr1_y1;
extern const unsigned int TEXT_FONT_1;

//--------------------------FASTRUN 2 PANEL-------------------------//

extern const int FASTRUN_ENA_2;
extern const int FR_CALL_BUTTON_2;// Кнопка активации панели FastRun
extern const int FR_EXIT_BUTTON_2;// Кнопка закрытия панели FastRun
extern const unsigned int TEXT_FONT_2;
extern const int FR_PIC_SIZE_2;
extern const int ENA_FR_LONGPRESS_2;
extern const unsigned int LEN_ID_2;
extern const unsigned int LEN_2;
extern const int DRAW_MODE_2;
extern const int IDLE_2;//Запуск фастран панелей только на идле
extern const int FASTRUN_NAME_2;
             int FR_KEYHOOK_ENA_2;
             int fr_pic_size_2;
             int NumOfItems_fr_2;
             int Selected_fr_2;
             int FastRunMenuActive_2;// Флаг активности меню
    unsigned int x1_2;
    unsigned int y1_2;
extern const unsigned int x1_fr_2;
extern const unsigned int y1_fr_2;

extern const char cfgPanBorderCol_2[4];
extern const char cfgPanBGCol_2[4];
extern const char cfgBookBorderCol_2[4];
extern const char cfgBookBGCol_2[4];

extern const unsigned int name_fr2_x1;
extern const unsigned int name_fr2_y1;
extern const unsigned int TEXT_FONT_2;

//--------------------------FASTRUN 3 PANEL-------------------------//

extern const int FASTRUN_ENA_3;
extern const int FR_CALL_BUTTON_3;// Кнопка активации панели FastRun
extern const int FR_EXIT_BUTTON_3;// Кнопка закрытия панели FastRun
extern const unsigned int TEXT_FONT_3;
extern const int FR_PIC_SIZE_3;
extern const int ENA_FR_LONGPRESS_3;
extern const unsigned int LEN_ID_3;
extern const unsigned int LEN_3;
extern const int DRAW_MODE_3;
extern const int IDLE_3;//Запуск фастран панелей только на идле
extern const int FASTRUN_NAME_3;
             int FR_KEYHOOK_ENA_3;
             int fr_pic_size_3;
             int NumOfItems_fr_3;
             int Selected_fr_3;
             int FastRunMenuActive_3;// Флаг активности меню
    unsigned int x1_3;
    unsigned int y1_3;
extern const unsigned int x1_fr_3;
extern const unsigned int y1_fr_3;

extern const char cfgPanBorderCol_3[4];
extern const char cfgPanBGCol_3[4];
extern const char cfgBookBorderCol_3[4];
extern const char cfgBookBGCol_3[4];

extern const unsigned int name_fr3_x1;
extern const unsigned int name_fr3_y1;
extern const unsigned int TEXT_FONT_3;

//-----------------------Обшие---------------------//

extern const char config[128];
extern const char cfgPanBorderCol[];//={0xFF,0xFF,0xFF,0x64};
extern const char cfgPanBGCol[];//={255,255,255,37};//={0xFF,0xFF,0x00,0x64};
extern const char cfgBookBorderCol[];//={255,255,255,100};
extern const char cfgBookBGCol[];//={255,255,255,37};

