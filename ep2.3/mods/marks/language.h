#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_

#define LG_COPYRIGHT "Marks v0.1\n(c)kluchnik"
//Оставляем только один дефайн иначе будет кака
#define LANG_RU
//#define LANG_EN

#ifdef LANG_RU
#define LG_CHANGE      "Изменить"
#define LG_SAVE        "Сохранить"
#define LG_SET         "Настройки"
#define LG_ABOUT       "Об эльфе"
#define LG_EXIT        "Выход"
#define LG_MENU        "Меню"

#define LG_SELECT      "Выбрать"
#define LG_CLOSE       "Закрыть"
#define LG_PATHFILE    "Путь к файлу: "
#define LG_DELFILE     "Файл %s удален!"
#define LG_DELETE      "Удалить"

#define LG_FILESAVE    "Файл сохранен!!!"
#define LG_NOTSAVE     "Не могу сохранить!:("

#define LG_NEW_FILE    "Новая запись"
#define LG_CREATE      "Создать"
#endif

#ifdef LANG_EN
#define LG_SAVE        "Save"
#define LG_SET         "Setting"
#define LG_ABOUT       "About"
#define LG_EXIT        "Exit"
#define LG_MENU        "Menu"

#define LG_SELECT      "Select"
#define LG_CLOSE       "Close"
#define LG_PATHFILE    "Path to file: "
#define LG_DELFILE     "File %s delete!"

#define LG_FILESAVE    "File save!!!"
#define LG_NOTSAVE     "Can't save!:("

#define LG_NEW_FILE    "New file"
#define LG_CREATE      "Create"
#endif

#endif

 
