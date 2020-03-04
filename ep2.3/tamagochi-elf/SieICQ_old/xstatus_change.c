#include "include.h"
#include "xstatus_change.h"
#include "iconpack.h"
#include "icq.h"
#include "capabilities.h"

SOFT_BUTTON_STRUCT 
  xstatus_change_soft={3, 3, "Выбор", "Назад", 1, 0};

HEADER_ITEM 
  xstatus_change_head;


#define XSTATUS_CHANGE_NUMS XIMGCount

static unsigned int 
  XStatus_Change_Cursor = 0;

//MENU_ITEM status_change_images[TOTAL_ICONS_NUM];

MENU_ITEM xstatus_change_text[XSTATUS_CHANGE_NUMS];
/*=
{
  (int)"В сети",
  (int)"Готов чатиться",
  (int)"Отошел",
  (int)"Не беспокоить",
  (int)"Занят",
  (int)"Недоступен",
  (int)"Невидим",
  (int)"Злой",
  (int)"Депрессия",
  (int)"Дома",
  (int)"На работе",
  (int)"Жру",
  (int)"Отключён"
};
*/
static void XChangeStatus(GUI *data)
{
  SetXStatus(XStatus_Change_Cursor);
}

const MENU_PROCS xstatus_change_procs[1]=
{
  XChangeStatus
};

MENU_STRUCT xstatus_change_struct=
{
  XSTATUS_CHANGE_NUMS,
  2,
  2,
  "Сменить X статус",
  0,
  xstatus_change_text,
  xstatus_change_procs
};

void OnRedraw_XStatusChange()
{
//  DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
  DrawImg(0,0,IconPack[IMG_BG]);
  DrawMenuList(&xstatus_change_struct, 
               IMG_XStatus0,
               XStatus_Change_Cursor);
  DrawSoftButton(&xstatus_change_soft);
}

int MoveCursor_XStatusChange(int pressed_mode, int key_kode)
{
  switch(pressed_mode)
  {
  case KEY_DOWN:
    {
      switch(key_kode)
      {
      case UP_BUTTON:
        if(XStatus_Change_Cursor==0)XStatus_Change_Cursor=XSTATUS_CHANGE_NUMS-1;
        else XStatus_Change_Cursor--;
      break;
      
      case DOWN_BUTTON:
        if(XStatus_Change_Cursor==XSTATUS_CHANGE_NUMS-1)XStatus_Change_Cursor=0;
        else XStatus_Change_Cursor++;
      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:
        ((void (*)(void))(xstatus_change_procs[0]))();
        TYPE_DRAW = Draw_MainMenu;
      break;

            
      case RIGHT_SOFT: TYPE_DRAW = Draw_MainMenu;
      break;
      
      case '1': XStatus_Change_Cursor = 0; break;
      case '7': XStatus_Change_Cursor = XSTATUS_CHANGE_NUMS-1; break;
      }
    }
    break;
  }
  //DirectRedrawGUI();
  return 0;
}

void Init_XStatus_Change()
{
  //InitHeaderData(&status_change_head, "Сменить статус", 2, FONT, 4, COLOUR, COLOUR_FRING);
  xstatus_change_text[0].item_id=(int)"";
  for (int i=1; i<XSTATUS_COUNT+1;i++)
  xstatus_change_text[i].item_id=(int)nameXStatus[i-1];
}
