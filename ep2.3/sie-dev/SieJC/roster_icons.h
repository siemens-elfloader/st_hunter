/*
    SieNatJabber Project
    Функции отрисовки иконок ростера
*/


// рисуем картинку с номером по заданным координатам
void Roster_DrawIcon(unsigned short x, unsigned short y, unsigned int img_num);
// Просто "обертки" к стандартным фунциям
int Roster_getIconWidth(char* path_to_pic);
int Roster_getIconHeight(char* path_to_pic);

// получаем номер картинки
void Roster_getIcon(char* path_to_pic, CLIST* ClEx, TRESOURCE* resEx);
//заменяем картинки на png
void Roster_fillIcons();

//вернет номер картинки по статусу
char* Roster_getIconByStatus(char* path_to_pic, char status);
