#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

typedef enum UI_STATE
{
    NONE,
    BG,
    CUR,
    BG_CUR
} UI_STATE;

extern IMGHDR* background;	//бекграунд
extern IMGHDR* cursor;		//курсор

extern UI_STATE stateUI;	//что используется из графики

void setUI();		//пуск
void freeUI ();		//зачистка

#endif // GUI_H_INCLUDED
