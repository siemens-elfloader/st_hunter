#include "..\inc\cfg_items.h"

__root const CFG_HDR cfghdr0={CFG_CBOX,"Показывать окошко приветствия",0,2};
__root const int bcfg_hello_ena=1;
__root const CFG_CBOX_ITEM cfgcbox0[2]={"Нет","Да"};

__root const CFG_HDR cfghdr1 = {CFG_UINT, "Таймер (сек):", 0, 10};
__root const unsigned int bcfg_timer=30;

__root const CFG_HDR cfghdr2 = {CFG_CBOX, "До запуска", 0, 8};
__root const int bcfg_sec = 4;
__root const CFG_CBOX_ITEM cfgcbox1[8] = {"5 сек",
                                          "10 сек",
                                          "30 сек",
                                          "1 мин",
                                          "2 мин",
                                          "5 мин",
                                          "10 мин",
                                          "30 мин"};

__root const CFG_HDR cfghdr3={CFG_STR_UTF8,"Скринсейвер",0,255};
__root const char bcfg_ssaver_path[256]="0:\\Zbin\\ssavers\\ss.elf";

__root const CFG_HDR cfghdr4={CFG_STR_UTF8,"Параметр",0,255};
__root const char bcfg_ssaver_par[256]="- parametr";


__root const CFG_HDR cfghdr5={CFG_CBOX,"Подсветка",0,2};
__root const int bcfg_illum_ena = 1;
__root const CFG_CBOX_ITEM cfgcbox2[2] = {"Зажигать","Нет"};


__root const CFG_HDR cfghdr6 = {CFG_CBOX,"Клавиатура",0,3};
__root const int bcfg_lock_ena = 2;
__root const CFG_CBOX_ITEM cfgcbox3[3] = {"Блокировка","Разблокировка","Без действия"};
