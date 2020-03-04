#include "..\inc\cfg_items.h"
/*
extern const int align;
extern const int align_month;
extern const int align_week;
extern const int align_date;
extern const int spase;

extern const char *FontPath;
extern const int fullset;
*/
__root const CFG_HDR cfghdr0={CFG_LEVEL,"Настройки шрифта",1,0};

__root const CFG_HDR cfghdr1112={CFG_STR_UTF8,"Font path 1",0,127};
__root const char FontPath[128]="4:\\zbin\\img\\font2\\";

__root const CFG_HDR cfghdr1_112={CFG_STR_UTF8,"Font path 2",0,127};
__root const char FontPath2[128]="4:\\zbin\\img\\font2\\";

__root const CFG_HDR cfghdr11_12={CFG_STR_UTF8,"Font path 3",0,127};
__root const char FontPath3[128]="4:\\zbin\\img\\font2\\";

__root const CFG_HDR cfghdr_q_2={CFG_CHECKBOX,"Использовать полный шрифт",0,2};
__root const int fullset = 0;

__root const CFG_HDR cfghdr1_10={CFG_CBOX,"Выравнивание общее",0,3};
__root const int align=0;
__root const CFG_CBOX_ITEM cfgcbox1[3]={"Left","Center","Right"};

__root const CFG_HDR cfghdr11_0={CFG_CBOX,"Выравнивание месяца",0,3};
__root const int align_month=0;
__root const CFG_CBOX_ITEM cfgcbox12[3]={"Left","Center","Right"};

__root const CFG_HDR cfghdr110s={CFG_CBOX,"Выравнивание дня недели",0,3};
__root const int align_week=0;
__root const CFG_CBOX_ITEM cfgcbox13[3]={"Left","Center","Right"};

__root const CFG_HDR cfghdr1130={CFG_CBOX,"Выравнивание числа",0,3};
__root const int align_date=0;
__root const CFG_CBOX_ITEM cfgcbox14[3]={"Left","Center","Right"};

__root const CFG_HDR cfghdr3={CFG_UINT,"Расстояние между буквами",0,65535};
__root const int space=1;

__root const CFG_HDR cfghdr1_16={CFG_LEVEL,"",0,0};
/*
Сб 12 Янв +5
Суббота 12 Января +6
Суббота 12 Янв +7
Сб 12.01 +3
Суббота 12.01 +4
Сб 12 +8
12 Сб +9
12.01.08 +1
12.01.2008 +2
*/
__root const CFG_HDR cfghdr110={CFG_CBOX,"Способ вывода",0,10};
__root const int DRAW_MODE=1;
__root const CFG_CBOX_ITEM cfgcbox15[10]={"Пользовательский","12.01.08","12.01.2008","Сб 12.01","Суббота 12.01","Сб 12 Янв","Суббота 12 Января","Суббота 12 Янв","Сб 12","12 Сб"};

__root const CFG_HDR cfghdr116={CFG_LEVEL,"Обшие настройки",1,0};
__root const CFG_HDR cfghdr5={CFG_COORDINATES,"Координаты",0,0};
__root const unsigned int X4=0;
__root const unsigned int Y4=170;

__root const CFG_HDR cfghdr13={CFG_UINT,"Шрифт",0,65535};
__root const int font=1;

__root const CFG_HDR cfghdr63={CFG_COLOR,"Цвет",0,0};
__root const char col[4]={255,255,255,100};
__root const CFG_HDR cfghdr111={CFG_LEVEL,"",0,0};


__root const CFG_HDR cfghdr115={CFG_LEVEL,"Настройки для месяца",1,0};
__root const CFG_HDR cfghdr2={CFG_COORDINATES,"Месяц",0,0};
__root const unsigned int X1=0;
__root const unsigned int Y1=70;

__root const CFG_HDR cfghdr210={CFG_CBOX,"Способ вывода",0,4};
__root const int MONTH_MODE=0;
__root const CFG_CBOX_ITEM cfgcbox51[4]={"Не покаызвать","Январь","Января","Янв"};

__root const CFG_HDR cfghdr113={CFG_UINT,"Шрифт",0,65535};
__root const int month_font=1;

__root const CFG_HDR cfghdr633={CFG_COLOR,"Цвет",0,0};
__root const char col_month[4]={255,255,255,100};
__root const CFG_HDR cfghdr118={CFG_LEVEL,"",0,0};
//----------------------------------------//


__root const CFG_HDR cfghdr11_6={CFG_LEVEL,"Настройки для дня недели",1,0};
__root const CFG_HDR cfghdr4={CFG_COORDINATES,"День недели",0,0};
__root const unsigned int X2=30;
__root const unsigned int Y2=70;

__root const CFG_HDR cfghdr21={CFG_CBOX,"Способ вывода",0,3};
__root const int WEEK_MODE=0;
__root const CFG_CBOX_ITEM cfgcbox2[3]={"Не покаызвать","Четверг","Чт"};

__root const CFG_HDR cfghdr135={CFG_UINT,"Шрифт",0,65535};
__root const int week_font=1;

__root const CFG_HDR cfghdr623={CFG_COLOR,"Цвет",0,0};
__root const char col_week[4]={255,255,255,100};
__root const CFG_HDR cfghdr11_16={CFG_LEVEL,"",0,0};

//----------------------------------------//

__root const CFG_HDR cfghdr1216={CFG_LEVEL,"Настройки для числа",1,0};
__root const CFG_HDR cfghdr6={CFG_COORDINATES,"Число",0,0};
__root const unsigned int X3=100;
__root const unsigned int Y3=100;

__root const CFG_HDR cfghdr22={CFG_CBOX,"Показывать",0,2};
__root const int DATE_MODE=0;
__root const CFG_CBOX_ITEM cfgcbox3[2]={"Нет","Да"};

__root const CFG_HDR cfghdr134={CFG_UINT,"Шрифт",0,65535};
__root const int date_font=1;

__root const CFG_HDR cfghdr163={CFG_COLOR,"Цвет",0,0};
__root const char col_date[4]={255,255,255,100};

__root const CFG_HDR cfghdr1164={CFG_LEVEL,"",0,0};
