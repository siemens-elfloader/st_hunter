#include "..\inc\cfg_items.h"
#include "..\inc\swilib.h"


#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

// Настройка клавиш

__root const CFG_HDR cfghdr0={CFG_LEVEL,"Клавиши",1,0};

  __root const CFG_HDR cfghdr0_1={CFG_KEYCODE,"Открытие",0,100};
  __root const unsigned int BCFG_CALL_BTN=RIGHT_SOFT;

  __root const CFG_HDR cfghdr0_2={CFG_CBOX,"Тип нажатия",0,2};
  __root const unsigned int BCFG_CALL_TYPE = 0;
  __root const CFG_CBOX_ITEM cfgcbox02[2]={"Короткое","Длинное"};

  __root const CFG_HDR cfghdr0_3={CFG_CBOX,"Состояние клавиатуры",0,3};
  __root const unsigned int BCFG_KBD_STATE = 2;
  __root const CFG_CBOX_ITEM cfgcbox04[3]={"Заблокировано","Разблокировано", "Без разницы"};

__root const CFG_HDR cfghdr1={CFG_LEVEL,"",0,0};

// Настройка отображения

__root const CFG_HDR cfghdr2={CFG_LEVEL,"Отображение",1,0};

	__root const CFG_HDR cfghdr2_0={CFG_UINT,"Уровень подсветки",0,100};
	__root const unsigned int light=10;

//Настройка фона

  __root const CFG_HDR cfghdr2_1={CFG_LEVEL,"Фон",1,0};

    __root const CFG_HDR cfghdr2_1_1={CFG_CBOX,"Тип фона",0,2};
    __root const unsigned int BG_TYPE = 1;
    __root const CFG_CBOX_ITEM cfgcbox211[2]={"Цвет", "Картинка"};

    __root const CFG_HDR cfghdr2_1_2={CFG_STR_UTF8,"Путь к картинке",0,127};
    __root const char bg_img[128]=DEFAULT_DISK ":\\ZBin\\img\\ClockSaver\\bg.png";

    __root const CFG_HDR cfghdr2_1_3={8,"Цвет фона",0,0};
    __root const char bg_col[4]={0,0,0,100};

  __root const CFG_HDR cfghdr2_2={CFG_LEVEL,"",0,0};


//Настройка часов

  __root const CFG_HDR cfghdr2_3={CFG_LEVEL,"Часы",1,0};

	__root const CFG_HDR cfghdr2_3_1={CFG_CHECKBOX,"Фон для часов",0,2};
    __root const unsigned int USE_CLOCK_BG = 1;

    __root const CFG_HDR cfghdr2_3_2={CFG_STR_UTF8,"Путь к фону часов",0,127};
    __root const char clock_bg_img[128]=DEFAULT_DISK ":\\ZBin\\img\\ClockSaver\\clock_bg.png";

    __root const CFG_HDR cfghdr2_3_3={CFG_UINT,"Координаты фона по Y",0,
#ifdef ELKA
	320
#else
	176
#endif
	};
    __root const unsigned int clock_bg_y=100;

    __root const CFG_HDR cfghdr2_3_4={CFG_UINT,"Координаты часов по Y",0,
#ifdef ELKA
	320
#else
	176
#endif
	};
    __root const unsigned int clock_y=102;

    __root const CFG_HDR cfghdr2_3_5={CFG_CBOX,"Шрифт Часов",0,2};
    __root const unsigned int CLOCK_FONT_TYPE = 1;
    __root const CFG_CBOX_ITEM cfgcbox233[2]={"Системный", "PNG"};

    __root const CFG_HDR cfghdr2_3_6={CFG_LEVEL,"Системный",1,0};

	  __root const CFG_HDR cfghdr2_3_6_1={CFG_UINT,"Размер",0,17};
      __root const unsigned int CLOCK_FONT_SIZE=0;

      __root const CFG_HDR cfghdr2_3_6_2={8,"Цвет",0,0};
      __root const char clock_font_col[4]={255,255,255,100};

    __root const CFG_HDR cfghdr2_3_7={CFG_LEVEL,"",0,0};

    __root const CFG_HDR cfghdr2_3_8={CFG_LEVEL,"PNG",1,0};

	  __root const CFG_HDR cfghdr2_3_8_1={CFG_STR_UTF8,"Путь к шрифту",0,127};
      __root const char clock_font_path[128]=DEFAULT_DISK ":\\ZBin\\img\\Fonts\\zekton_big\\";

      __root const CFG_HDR cfghdr2_3_8_2={CFG_UINT,"Расстояние между символами",0,255};
      __root const unsigned int clock_font_gap=0;

    __root const CFG_HDR cfghdr2_3_9={CFG_LEVEL,"",0,0};

  __root const CFG_HDR cfghdr2_4={CFG_LEVEL,"",0,0};


//Настройка даты

  __root const CFG_HDR cfghdr2_5={CFG_LEVEL,"Дата",1,0};

	__root const CFG_HDR cfghdr2_5_1={CFG_UINT,"Размер шрифта",0,17};
	__root const unsigned int DATE_FONT_SIZE=0;

	__root const CFG_HDR cfghdr2_5_2={8,"Цвет",0,0};
	__root const char date_font_col[4]={255,255,255,100};

	__root const CFG_HDR cfghdr2_5_3={CFG_UINT,"Координаты даты по Y",0,
#ifdef ELKA
	320
#else
	176
#endif
	};
    __root const unsigned int date_y=105;

  __root const CFG_HDR cfghdr2_6={CFG_LEVEL,"",0,0};

__root const CFG_HDR cfghdr3={CFG_LEVEL,"",0,0};
