#ifndef _MENU_H_
#define _MENU_H_
//#include "include.h"
//#include "List.h"
//#include "print.h"
#define ERR_SET_FONT 601


typedef struct
  {
    char *fntSelect;
    char *bordSelect;
    char *fntNotSelect;
    char *clrNotSelect;
    char *bordNotSelect;    
    char *CursorBGClr;
    char *CursorClr;
  }COLOR;

typedef struct
 {
   int itemFromDisp;
   int totalItems;
   int curpos;
   int curpage;
   int column;
   int nlines;
 }SET_MENU;

typedef struct
{
  IMGHDR Shot;
  int shot_n;
  IMGHDR *pic;
  char name[32];
}HEADER;

typedef struct
{
  char right[16];
  char left[16];
  IMGHDR shot;
  int shot_n;
}SOFT_KEY;

typedef struct
{
  SOFT_KEY *softkey;
  HEADER *header;
  SET_MENU *setting;
  COLOR *color;
}GUI_MENU;

/*
#define img_noicon  0
#define img_bg      1
#define img_headpic 2
*/
class GuiMenu
{
private:
    GUI_MENU *menu;
    COLOR clr;
    HEADER head;
    SET_MENU set;
    SOFT_KEY soft_key;    
    _LIST_STRUCT *bmk;
    
    int font;    
    int scr_w, scr_h;

    IMGHDR cursor;
    int cursor_shot;
    
    char s[128];
    int menus_id;

    int start_y;
    int start_x;
    
    int error;    
public:
  void DrawSoftKeys(char *right,char *left);
  void DrawHeader(BOOKMARKS *bmk,char *s,int x);
  void DrawScrollBar(int cur,int total);
  void PrintAbout();  
  void DrawList();
  void SMART_REDRAW(void);
  void MoveUp(int i);
  void MoveDown(int i);
  
  void SetParams(COLOR*,SET_MENU*);
  void SetSoftKey(SOFT_KEY*);
  void SetHeader(HEADER*);
  
  IMGHDR DoScreen(int ScrH,int HSIZE);
  int DrwImage(IMGHDR *img, int x, int y);
  int InitMenu();

  int GetErr() { return error; }
  int SetError(const int err) { return (error = err); }
  
  GuiMenu(GUI_MENU create_menu);
  GuiMenu();
  ~GuiMenu();
};
#endif
