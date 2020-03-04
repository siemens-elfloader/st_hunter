#include "include.h"
#include "status_change.h"
#include "iconpack.h"
#include "icq.h"

SOFT_BUTTON_STRUCT 
  status_change_soft={3, 3, "Выбор", "Назад", 1, 0};

HEADER_ITEM 
  status_change_head;


#define STATUS_CHANGE_NUMS 13

static unsigned int 
  Status_Change_Cursor = 0;

//MENU_ITEM status_change_images[TOTAL_ICONS_NUM];

/*=
{
  (int)"online.png",
  
  
  (int)"away.png",
  (int)"dnd.png",
  (int)"na.png",
  (int)"occupied.png",
  (int)"ffc.png",
  (int)"invisible.png",
  (int)"EVIL.png",
  (int)"DEPRESSION.png",	   
  (int)"HOME.png",   
  (int)"WORK.png",
  (int)"LUNCH.png",
    (int)"offline.png",
};

*/
MENU_ITEM status_change_text[STATUS_CHANGE_NUMS]=
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

static void ChangeStatus(GUI *data)
{
  switch(Status_Change_Cursor)
  {
  case 0:
    SetStatus(ICQ_STATUS_ONLINE);
    break;
  case 1:
    SetStatus(ICQ_STATUS_FREE4CHAT);
    break;
  case 2:
    SetStatus(ICQ_STATUS_AWAY);
    break;
  case 3:
    SetStatus(ICQ_STATUS_DND);
    break;
  case 4:
    SetStatus(ICQ_STATUS_OCCUPIED);
    break;
  case 5:
    SetStatus(ICQ_STATUS_NA);
    break;
  case 6:
    SetStatus(ICQ_STATUS_INVISIBLE);
    break;
  case 7:
    SetStatus(ICQ_STATUS_EVIL);
    break;
  case 8:
    SetStatus(ICQ_STATUS_DEPRESSION);
    break;
  case 9:
    SetStatus(ICQ_STATUS_HOME);
    break;
  case 10:
    SetStatus(ICQ_STATUS_WORK);
    break;
  case 11:
    SetStatus(ICQ_STATUS_LUNCH);
    break;
  case 12:
    SetStatus(ICQ_STATUS_OFFLINE);
    break;
  }
}

const MENU_PROCS status_change_procs[1]=
{
  ChangeStatus
};

MENU_STRUCT status_change_struct=
{
  STATUS_CHANGE_NUMS,
  2,
  2,
  "Сменить статус",
  0,
  status_change_text,
  status_change_procs
};

void OnRedraw_StatusChange()
{
//  DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
   DrawImg(0,0,IconPack[IMG_BG]);
  DrawMenuList(&status_change_struct, 
               IMG_ONLINE,
               Status_Change_Cursor);
  DrawSoftButton(&status_change_soft);
}

int MoveCursor_StatusChange(int pressed_mode, int key_kode)
{
  switch(pressed_mode)
  {
  case KEY_DOWN:
    {
      switch(key_kode)
      {
      case UP_BUTTON:
        if(Status_Change_Cursor==0)Status_Change_Cursor=STATUS_CHANGE_NUMS-1;
        else Status_Change_Cursor--;
      break;
      
      case DOWN_BUTTON:
        if(Status_Change_Cursor==STATUS_CHANGE_NUMS-1)Status_Change_Cursor=0;
        else Status_Change_Cursor++;
      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:
        ((void (*)(void))(status_change_procs[0]))();
        TYPE_DRAW = Draw_MainMenu;
      break;

            
      case RIGHT_SOFT: TYPE_DRAW = Draw_MainMenu;
      break;
      
      case '1': Status_Change_Cursor = 0; break;
      case '7': Status_Change_Cursor = STATUS_CHANGE_NUMS-1; break;
      }
    }
    break;
  }
  //DirectRedrawGUI();
  return 0;
}

void Init_Status_Change()
{
  //InitHeaderData(&status_change_head, "Сменить статус", 2, FONT, 4, COLOUR, COLOUR_FRING);
}
