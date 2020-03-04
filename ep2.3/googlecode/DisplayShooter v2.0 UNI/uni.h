//В этом файле описаны дефайны и структуры для создания всеплатформенных эльфов
//В проекты нужно включать этот файл, а не swilib.h
#include "swilib.h"

/*
  Дефайны теперь содержат вызовы функций (isnewSGold()), в следствие чего как константы их использовать нельзя; задавать всякие MENUITEM_DESC следует где-нибудь еще, например в main();

Большинство UNI типов - объединение структур для sgold и для newsgold.
Пример:

//Сначала пишем так:
struct
{
  UNI_CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM;

//А затем (хоть в том же main()) так:
  maincsm.sgold.onMessage=maincsm_onmessage;
  maincsm.sgold.onCreate=maincsm_oncreate;
  if(isnewSGold())
  {
    MAINCSM.maincsm.newsgold.onClose=maincsm_onclose;
    MAINCSM.maincsm.newsgold.datasize=sizeof(MAIN_CSM),
    MAINCSM.maincsm.newsgold.statesize=1,
    MAINCSM.maincsm.newsgold.states=&minus11;
  }
  else
  {
    MAINCSM.maincsm.sgold.onClose=maincsm_onclose;
    MAINCSM.maincsm.sgold.datasize=sizeof(MAIN_CSM),
    MAINCSM.maincsm.sgold.statesize=1,
    MAINCSM.maincsm.sgold.states=&minus11;
  }
//Таким же образом колбасим maincsm_name

onMessage и onCreate здесь присваиваются безусловно, потому что и для sgold и для newsgold они имеют одинаковое расположение в структуре. Остальные элементы различаются.

А вместо обычного
  CreateCSM(&MAINCSM.maincsm,dummy,0);
пишем так:
  CreateCSM((CSM_DESC*)&MAINCSM.maincsm,dummy,0);
И везде, где для UNI типа требуется его неUNI аналог, надо приводить тип
*/

#define isElka() (isnewSGold()==2)

#define UNI_DISPLACE_OF_EDGUI (isnewSGold()?(isElka()?0x90:0x88):0x50)
#define UNI_DISPLACE_OF_EDGUI_ID 0x4C // Чтоб картина не портилась №1)
#define UNI_DISPLACE_OF_IDLEGUI_ID (isnewSGold()?0x2C:0x28)
#define UNI_DISPLACE_OF_IDLECANVAS (isnewSGold()?(isElka()?0x20:0x18):0x14)
#define UNI_MSG_HELPER_TRANSLATOR (isnewSGold()?0xDEAE:0x3F0)
#define UNI_MSG_RECONFIGURE_REQ (isnewSGold()?0xDEAF:0x3F1)
#define UNI_MSG_GUI_DESTROYED (isnewSGold()?0x640E:0x98)
#define UNI_MSG_IDLETMR_EXPIRED (isnewSGold()?0x6401:0x95)
#define UNI_MMI_CEPID 0x4209 // №2)
#define UNI_MIDDLE_FONT (isnewSGold()?5:3)
#define UNI_SMALL_FONT (isnewSGold()?11:7)
#define UNI_LGP_NULL (isnewSGold()?0x7FFFFFFF:0x7FFF)
#define UNI_SET_SOFT_KEY_N (isnewSGold()?0:1)
#define UNI_LGP_DOIT_PIC 0x7FFFC0FB // №3)

// For kbd msg:
// submsg - scan Code:
#define UNI_VOL_UP_BUTTON (isnewSGold()?0x0D:0x14)
#define UNI_VOL_DOWN_BUTTON (isnewSGold()?0x0E:0x15)

//---
#define UNI_EDIT_CURSOR_POS(DATA) (isnewSGold()?(isElka()?\
                              (((int ****)DATA)[0x90/4][0x8C/4][0x28/4][0x38/4]):\
                              (((short ****)DATA)[0x88/4][0x84/4][0x28/4][0x34/2])):\
                              (((short ****)DATA)[0x50/4][0x4C/4][0x28/4][0x30/2]))

//Menu flags and screen displace
#define UNI_MENU_FLAG3 3 // №4)
#define UNI_MENU_FLAG2 (isnewSGold()?(isElka()?0x5A2:0x59D):0x578)
#define UNI_YDISP (isElka()?24:0)


typedef union
{
  struct
  {
    short pid_from;
    short msg;
    int submess;
    void *data0;
    void *data1;
  }sgold;
  struct
  {
    int pid_from;
    int msg;
    int submess;
    void *data0;
    void *data1;
  }newsgold;
}UNI_GBS_MSG;

typedef union
{
  struct
  {
    int (*onMessage)(CSM_RAM*,UNI_GBS_MSG*);
    void (*onCreate)(CSM_RAM*);
    void (*onClose)(CSM_RAM*);
    int datasize;
    int statesize;
    const int *states;
  }sgold;
  struct
  {
    int (*onMessage)(CSM_RAM*,UNI_GBS_MSG*);
    void (*onCreate)(CSM_RAM*);
    int zero1;
    int zero2;
    int zero3;
    int zero4;
    void (*onClose)(CSM_RAM*);
    int datasize;
    int statesize;
    const int *states;
  }newsgold;
}UNI_CSM_DESC;

typedef union
{
  struct
  {
    const char *ext;
    int unical_id;
    signed char enabled_options;
    unsigned char obex_path_id;   
    unsigned short menu_flag;
    const int *icon1;
    const int *icon2; //skip on reg
    void *proc;
    void *altproc;
  }sgold;
  struct
  {
    const char *ext;
    int unical_id;
    char enabled_options;
    char obex_path_id;
    short menu_flag;
    const int *icon1;
    const int *icon2;
    int Open_lgpid;	//0x109
    int Options_lgpid;	//0x10A
    int Enter_lgpid;	//0x7FFFC0FB
    void *proc;
    void *altproc;
  }newsgold;
}UNI_REGEXPLEXT;

typedef union
{
  struct
  {
    RECT *canvas;
    void *methods;
    void *definition;
    char state;
    char unk2;
    char unk3;
    char unk4;
    int color1; //Параметр GeneralFunc пишется сюда?????
  //  int color2;
    LLQ item_ll;
    int unk5;
    char unk6;
    char unk7;
    char unk8;
    char unk9;
    int unk10;
    int flag30; //Используется при создании (бывает |=0x02)
  }sgold;
  struct
  {
    RECT *canvas;
    void *methods;
    void *definition;
    char state;
    char unk2;
    char unk3;
    char unk4;
    int color1; //Параметр GeneralFunc пишется сюда?????
    int color2;
    LLQ item_ll;
    int unk5;
    char unk6;
    char unk7;
    char unk8;
    char unk9;
    int unk10;
    int flag30; //Используется при создании (бывает |=0x02)
  }newsgold;
}UNI_GUI;

typedef struct
{
  char zero;
  char unk1;
  short keys;
  UNI_GBS_MSG *gbsmsg;
}UNI_GUI_MSG;

typedef union
{
  struct
  {
    char w;
    char h;
    unsigned short bpnum; //For BW=1, 8bit=5, 16bit=8, 0x80 - packed
    char *bitmap;
  }old;
  struct
  {
    unsigned short w;
    unsigned short h;
    int bpnum; //For BW=1, 8bit=5, 16bit=8, 0x80 - packed
    char *bitmap;
  }elka;
}UNI_IMGHDR;

typedef union
{
#pragma pack(1)
  struct
  {
    char unk1[14];
    short file_attr;
    unsigned int create_date_time; 
    unsigned int file_size;
    char unk2[16];
    char folder_name[128];
    char unk3;
    char file_name[271];
  }sgold;
#pragma pack()//наверно не надо, но я хз, так что пусть будет
#pragma pack(1)
  struct
  {
    char unk0[40];
    unsigned int file_size;
    short file_attr;
    unsigned int create_date_time;
    char unk1[2];
    char folder_name[128];
    char unk2;
    char file_name[128];
    char unk3[11];
    short wsfolder_name[128+1];
    short wsfile_name[128+1];
  }newsgold;
#pragma pack()
}UNI_DIR_ENTRY;

typedef struct
{
  int one;
  int (*onKey)(UNI_GUI *gui, UNI_GUI_MSG *msg);
  void (*global_hook_proc)(UNI_GUI *gui, int cmd);
  void *locret;
  int zero1;
  SOFTKEYSTAB *softkeystab;
  RECT rc;
  int font;
  int _100;
  int _101;
  int zero2;
  int zero3;
  int _0x40000000;
}UNI_INPUTDIA_DESC;

typedef union
{
  struct
  {
    char field_0;
    char no_data;
    char field_2;
    char item_type;
    int field_4;
    void *data;
  }sgold;
  struct
  {
    short field_0;
    char no_data;
    char item_type;
    int field_4;
    void *data;
  }newsgold;
}UNI_AB_UNPRES_ITEM;

typedef union
{
  struct
  {
    short field_0;
    short field_2;
    short field_4;
    short number_of_records;
    short field_8;
    short field_A;
    UNI_AB_UNPRES_ITEM *record_list;
    char field_10;
    char field_11;
    short field_12;
    int field_14;
    int field_18;
  }sgold;
  struct
  {
    short NrOfFlds;
    short number_of_records;
    short NrAllocFlds;
    short field_6;
    short NrJ;
    char field_A;
    char field_B;
    UNI_AB_UNPRES_ITEM *record_list;
    char EntTp;
    char field_11;
    short EntId;
    int field_14;
    int field_18;
  }newsgold;
}UNI_AB_UNPRES;

