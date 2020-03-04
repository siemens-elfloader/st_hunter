#ifndef _C_LIST_UTIL_H
#define _C_LIST_UTIL_H

typedef struct
{
  int y_disp; // Отсуп от верха экрана
  unsigned int max_nums; //Максимальное количество контактов
  unsigned int n_contacts; //Число контактов, видмых на экране
  unsigned int n_groups; // Общее число групп
  
  int items_font;
  int items_font_type;

}CONTACT_LIST_DESC;

// Функции работы с визуализацией списка контактов

void CList_MoveCursorHome();
void CList_MoveCursorEnd();
void OnRedraw_C_List();
int MoveCursor_C_List(int mode_key, int type_key);
void Init_C_List();


#endif
