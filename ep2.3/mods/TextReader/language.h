#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

//Оставляем только один дефайн иначе будет кака
#define LANG_RU
//#define LANG_EN

#ifdef LANG_RU

#define LG_SELECT_FILE "Выбрать файл"
#define LG_LAST        "Последние..."
#define LG_GOTO        "Перейти к..."
#define LG_SAVE        "Сохранить"
#define LG_SET         "Настройки"
#define LG_ABOUT       "Об эльфе"
#define LG_EXIT        "Выход"
#define LG_MENU        "Меню"
//Перейти к...
#define LG_START       "В начало"
#define LG_END         "В конец"
#define LG_LINE        "На линию..."
#define LG_BOOKMARK    "Заладки..."

#define LG_SELECT      "Выбрать"
#define LG_CLOSE       "Закрыть"
#define LG_CODEPAGE    "Кодировка..."
#define LG_PATHFILE    "Путь к файлу: "
#define LG_DELFILE     "Файл %s удален!"

#define LG_FILESAVE    "Файл сохранен!!!"
#define LG_NOTSAVE     "Не могу сохранить!:("

#define LG_NEW_FILE    "Новый файл"
#define LG_CREATE      "Создать"
#endif

#ifdef LANG_EN
#define LG_SELECT_FILE "Select file"
#define LG_LAST "Last open..."
#define LG_GOTO "Go to..."
#define LG_SAVE "Save"
#define LG_SET "Setting"
#define LG_ABOUT "About"
#define LG_EXIT "Exit"
#define LG_MENU "Menu"
//Перейти к...
#define LG_START "Start"
#define LG_END "End"
#define LG_LINE "line..."
#define LG_BOOKMARK "Bookmarks..."

#define LG_SELECT "Select"
#define LG_CLOSE "Close"
#define LG_CODEPAGE "Codepage..."
#define LG_PATHFILE "Path to file: "
#define LG_DELFILE "File %s delete!"

#define LG_FILESAVE "File save!!!"
#define LG_NOTSAVE "Can't save!:("

#define LG_NEW_FILE    "New file"
#define LG_CREATE   "Create"
#endif

#endif

 
