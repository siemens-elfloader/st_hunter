#ifndef _VISUAL_H
#define _VISUAL_H

typedef struct
{
  int y;
  int h;
  char colour[4];
  char fring_colour[4];
}PROGTESSBAR;

typedef struct
{
  int y_position;
  int font;
  char colour[4];
  char fring_colour[4];
}DATA_TIME;


typedef struct
{
  int y_disp;
  char text[256];
  
  //Для более тонкой настройки
  int font;
  int font_type;
  char colour[4];
  char fring_colour[4];
  
}HEADER_ITEM;

typedef struct
{
  int item_id;
  //int img_id;
}MENU_ITEM;

typedef struct
{
  int cursor_id;
}CURSOR_ITEM;

typedef struct 
{
  int y_disp;
  int x_disp;
  char text_left[32];
  char text_right[32];
  int ena_time;
  int ena_time_sec;
} SOFT_BUTTON_STRUCT;


typedef void (__interwork *MENU_PROCS)(GUI *);

typedef struct 
{
  int max_num;
  int y_disp;
  int x_disp;
  char header_text[128];
  int type_header_text;
  //const MENU_ITEM *images;
  const MENU_ITEM *items;
  const MENU_PROCS *procs;//  void ** procs; //Table of procs when item selected
} MENU_STRUCT;


// Софт надписи ----------------------------------------------------------------
void InitSoftButton(SOFT_BUTTON_STRUCT *data, int x_dsp, int y_dsp, char * left, char * right, int ena_t, int ena_t_s);
void DrawSoftButton(SOFT_BUTTON_STRUCT *data);
int Get_SoftButton_Height(SOFT_BUTTON_STRUCT *data);

// Головные надписи ------------------------------------------------------------
void InitHeaderData(HEADER_ITEM *data, char *txt, int y, int f, int f_t, char *c, char *f_c);
void DrawHeaderText(HEADER_ITEM *data);

void InitHeaderText(HEADER_ITEM *data, char *txt);
int Get_HeaderText_Height(HEADER_ITEM *data);

// Главное меню ----------------------------------------------------------------
void InitMenuList(MENU_STRUCT *data, char *head, int y_dsp);
void DrawMenuList(MENU_STRUCT *data, int images, int cur_count);

// Дата и время ----------------------------------------------------------------
void InitDataTime(DATA_TIME *data, int y, int dt_font, char * c1, char * c2);
void DrawDataTime(DATA_TIME *data);

                  
#endif
