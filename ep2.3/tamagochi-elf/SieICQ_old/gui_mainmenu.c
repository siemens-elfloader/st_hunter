#include "include.h"
#include "gui_begin.h"
#include "gui_mainmenu.h"

#include "main.h"
#include "c_list_util.h"

#include "gui.h"
// (c) Vedan

typedef struct
{
  GUI gui;
} GUI_MAINMENU_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
} GUI_MAINMENU_CSM;

int TYPE_MENU;

// Структуры ===================================================================


// ********* MAIN_MENU *********************************************************

// Количество всех меню
#define ALL_MENU_NUM 3 

//Количество пунктов главного меню
#define MAIN_MENU_NUMS 6 

#define MAIN_MENU_Y_DISP 3
#define MAIN_MENU_X_DISP 2

// ********* MAIN_MENU - functions *********************************************

static void ToConnect(GUI *data)
{
  RUN_GUI_BEGIN(1);
  Create_Connect();
}


static void ContactList(GUI *data)
{
  RUN_GUI_C_LIST(0);
}


static void ChangeStatus(GUI *data)
{
  Gui_Run(0);
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
  (int)"Список контактов",
  (int)"Настройки",
  (int)"О программе",
  (int)"Выход"
};


const MENU_PROCS mainmenu_procs[MAIN_MENU_NUMS]=
{
  ToConnect,
  ChangeStatus,
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

// K E Y  C O N T R O L ========================================================

int CountMenuCursors(int type_menu, int mode_key, int type_key)
{
  switch(mode_key)
  {
  case KEY_DOWN:
    {
      switch(type_key)
      {
      case UP_BUTTON:
        if(menu_cursors[type_menu]==0)menu_cursors[type_menu]=max_num_menus[type_menu]-1;
        else menu_cursors[type_menu]--;
      break;
      
      case DOWN_BUTTON:
        if((menu_cursors[type_menu])==max_num_menus[type_menu]-1)menu_cursors[type_menu]=0;
        else menu_cursors[type_menu]++;
      break;
      
      case LEFT_SOFT:
      case ENTER_BUTTON:
        ((void (*)(void))(mainmenu_procs[menu_cursors[type_menu]]))();
      break;
      
      case RIGHT_SOFT: return 1;
      }
    }
    break;
  }
  return 0;
}

//==============================================================================
unsigned int GUI_MAINMENU_ID = 0,
             Quit_GUI_MAINMENU = 0;
             


static void DrawMenuFon()
{
  DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
}


static void OnRedraw(GUI_MAINMENU_GUI *data)
{
  if (data->gui.state==2)
  {
    LockSched(); 
    DrawMenuFon();

    switch(TYPE_MENU)
    {
    case IS_MAIN_MENU:
      {
        DrawMenuList(&main_menu_struct, &mainmenu_text[MAIN_MENU_NUMS], menu_cursors[IS_MAIN_MENU]);
        DrawSoftButton(&mainmemu_soft);
      } break;
    }
    UnlockSched(); 
  }
}

static void OnCreate(GUI_MAINMENU_GUI *data,void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

static void OnClose(GUI_MAINMENU_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

static void OnFocus(GUI_MAINMENU_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  DisableIDLETMR();
  data->gui.state=2;
}

static void OnUnfocus(GUI_MAINMENU_GUI *data,void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(0);
#endif
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


static int OnKey(GUI_MAINMENU_GUI *data, GUI_MSG *msg)
{
  if(Quit_GUI_MAINMENU)return 1;
  int sh=msg->gbsmsg->msg;
  
  switch(sh)
  {
  case KEY_DOWN:
    switch(msg->gbsmsg->submess)
    {
      case RIGHT_SOFT: if(TYPE_MENU==IS_MAIN_MENU)Close_MAIN_CSM(); else return 1;
    }
  }
  
  CountMenuCursors(IS_MAIN_MENU, sh, msg->gbsmsg->submess);
    
  DirectRedrawGUI();
  return 0;
}


static int met0(void){return(0);}
static const void * const GUI_MAINMENU_GUI_methods[11]={
  (void *)OnRedraw,
  (void *)OnCreate,
  (void *)OnClose,
  (void *)OnFocus,
  (void *)OnUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data,
  (void *)met0,
  (void *)met0,
  0
};





// Инициализация параметров меню
void InitMenu()
{
  // Пока отсюда ничего не надобно
  //InitSoftButton(&mainmemu_soft,3,3, "Выбор", "Выйти", 1, 0);
  //InitMenuList(&main_menu_struct, "Меню", 3, (char*)mainmenu_text);
}

void RUN_GUI_MAINMENU(int menu_mode)
{
  Quit_GUI_MAINMENU = 0;
  TYPE_MENU = menu_mode;
  InitMenu();
  static const RECT Canvas={0,0,0,0};
  patch_rect((RECT*)&Canvas,0,0,ScrW-1,ScrH-1);
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  GUI_MAINMENU_GUI *main_gui=malloc(sizeof(GUI_MAINMENU_GUI));
  zeromem(main_gui,sizeof(GUI_MAINMENU_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)GUI_MAINMENU_GUI_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
  //DirectRedrawGUI();
  ///return (0);
}
