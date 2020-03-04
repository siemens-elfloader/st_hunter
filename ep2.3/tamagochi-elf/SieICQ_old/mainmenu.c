#include "include.h"
#include "gui_begin.h"
#include "gui_mainmenu.h"
#include "iconpack.h"
#include "main.h"
#include "c_list_util.h"

#include "gui.h"

// (c) Vedan


// Количество всех меню
#define ALL_MENU_NUM 3 

//Количество пунктов главного меню
#define MAIN_MENU_NUMS 7 

#define MAIN_MENU_Y_DISP 3
#define MAIN_MENU_X_DISP 2



static void ToConnect(GUI *data)
{
  TYPE_DRAW = Draw_Connect;
  Create_Connect();
}


static void ContactList(GUI *data)
{
  Init_C_List();
  TYPE_DRAW = Draw_C_List;
}


static void ChangeStatus(GUI *data)
{
  TYPE_DRAW = Draw_StatusChange;
}

static void ChangeXStatus(GUI *data)
{
  TYPE_DRAW = Draw_XStatusChange;
}

static void EditConfig(GUI *data)
{
  OpenSettings();
}

static void ExitSieICQ(GUI *data)
{
  Close_MAIN_CSM();
}

static void AboutSieICQ(GUI *data)
{
  ShowMSG(1,(int)"SieICQ\n(c) Eraser, Vedan");
}

// *****************************************************************************

MENU_ITEM mainmenu_text[MAIN_MENU_NUMS]=
{
  (int)"Подключиться",
  (int)"Сменить статус",
  (int)"Сменить X статус",
  (int)"Список контактов",
  (int)"Настройки",
  (int)"О программе",
  (int)"Выход"
};



const MENU_PROCS mainmenu_procs[MAIN_MENU_NUMS]=
{
  ToConnect,
  ChangeStatus,
  ChangeXStatus,
  ContactList,
  EditConfig,
  AboutSieICQ,
  ExitSieICQ
};
  

MENU_STRUCT main_menu_struct=
{
  MAIN_MENU_NUMS,
  2,
  MAIN_MENU_X_DISP,
  "Меню",
  0,
  mainmenu_text,
  mainmenu_procs
};

char is_status[128]="";

void RenameMainMenuItem(int id, char * two)
{
  //mainmenu_text[item]=(MENU_ITEM)two;
  //main_menu_struct.items[id]=(int) two;
}

// *****************************************************************************


// ********* SOFT BUTTON *******************************************************

// Инициализируются софт надписи главного меню
SOFT_BUTTON_STRUCT mainmemu_soft={3, 3, "Выбор", "Выйти", 1, 0};

// *****************************************************************************

#define IS_MAIN_MENU 0

//CURSOR_ITEM menu_cursors[ALL_MENU_NUM]={0,0,0};

int menu_cursors[ALL_MENU_NUM]=
{
  0,
  0,
  0
};

int max_num_menus[ALL_MENU_NUM]=
{
  MAIN_MENU_NUMS,
  0,
  0
};


int MoveCursor_MainMenu(int pressed_mode, int key_kode)
{
  switch(pressed_mode)
  {
  case KEY_DOWN:
    {
      switch(key_kode)
      {
      case UP_BUTTON:
        if(menu_cursors[IS_MAIN_MENU]==0)menu_cursors[IS_MAIN_MENU]=max_num_menus[IS_MAIN_MENU]-1;
        else menu_cursors[IS_MAIN_MENU]--;
      break;
      
      case DOWN_BUTTON:
        if((menu_cursors[IS_MAIN_MENU])==max_num_menus[IS_MAIN_MENU]-1)menu_cursors[IS_MAIN_MENU]=0;
        else menu_cursors[IS_MAIN_MENU]++;
      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:
        ((void (*)(void))(mainmenu_procs[menu_cursors[IS_MAIN_MENU]]))();
      break;
      
      case RIGHT_SOFT: Close_MAIN_CSM();break;
      }
    }
    break;
  }
  return 0;
}

//==============================================================================
unsigned int GUI_MAINMENU_ID = 0,
             Quit_GUI_MAINMENU = 0;
             


void OnRedraw_MainMenu()
{
 // DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
  DrawImg(0,0,IconPack[IMG_BG]);
  DrawMenuList(&main_menu_struct, IMG_TOCONNECT, menu_cursors[IS_MAIN_MENU]);
  DrawSoftButton(&mainmemu_soft);
}







// Инициализация параметров меню
void Init_MainMenu()
{
   //RenameMainMenuItem(0, "Сработало!");
   
  // Пока отсюда ничего не надобно
  //InitSoftButton(&mainmemu_soft,3,3, "Выбор", "Выйти", 1, 0);
  //InitMenuList(&main_menu_struct, "Меню", 3, (char*)mainmenu_text);
}

