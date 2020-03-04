#include "include.h"
#include "gui_begin.h"

#include "main.h"
#include "c_list_util.h"

// (c) Vedan

typedef struct
{
  GUI gui;
} CHAT_GUI;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
} CHAT_CSM;

// Optoins ---------------------------------------------------------------------

#define OPEN 1
#define CLOSE 0

int TYPE_MENU;
int Options=CLOSE;
int Options_Cursor;

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

  Create_Connect();
}


static void ContactList(GUI *data)
{

}


static void ChangeStatus(GUI *data)
{

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

MENU_ITEM options_text[MAIN_MENU_NUMS]=
{
  (int)"Подключиться",
  (int)"Сменить статус",
  (int)"Список контактов",
  (int)"Настройки",
  (int)"О программе",
  (int)"Выход"
};


const MENU_PROCS options_procs[MAIN_MENU_NUMS]=
{
  ToConnect,
  ChangeStatus,
  ContactList,
  EditConfig,
  AboutSieICQ,
  ExitSieICQ
};
  

MENU_STRUCT options_struct=
{
  MAIN_MENU_NUMS,
  2,
  MAIN_MENU_X_DISP,
  "Меню",
  0,
  options_text,
  options_procs
};


// ********* SOFT BUTTON *******************************************************


SOFT_BUTTON_STRUCT options_soft={3, 3, "Выбор", "Назад", 1, 0};




unsigned int CHAT_ID = 0,
             Quit_CHAT = 0;
             


static void DrawMenuFon()
{
  DrawRoundedFrame(0, 0, ScrW, ScrH ,0, 0, 0, GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
}

static void DrawOptions()
{
  if(Options)
  {
    
  }
}

static void OnRedraw(CHAT_GUI *data)
{
  if (data->gui.state==2)
  {
    DrawMenuFon();
    DrawMenuList(&options_struct,0, Options_Cursor);
    DrawSoftButton(&options_soft);
    DrawOptions();
  }
}

static void OnCreate(CHAT_GUI *data,void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

static void OnClose(CHAT_GUI *data,void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

static void OnFocus(CHAT_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(1);
#endif
  DisableIDLETMR();
  data->gui.state=2;
}

static void OnUnfocus(CHAT_GUI *data,void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(0);
#endif
  if (data->gui.state!=2) return;
  data->gui.state=1;
}


static int OnKey(CHAT_GUI *data, GUI_MSG *msg)
{
  if(Quit_CHAT)return 1;
 
  switch(msg->gbsmsg->msg)
  {
  case KEY_DOWN:
    {
      switch(msg->gbsmsg->submess)
      {
      case LEFT_SOFT:
        if(Options)Options=CLOSE;else Options=OPEN;
        break;
        
      case RIGHT_SOFT:
        if(Options)Options=CLOSE;
        else return 1;
      }
    }
  }
  
    
  DirectRedrawGUI();
  return 0;
}


static int met0(void){return(0);}
static const void * const CHAT_GUI_methods[11]={
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






void CreateEditChat()
{
  Quit_CHAT = 0;
  static const RECT Canvas={0,0,0,0};
  patch_rect((RECT*)&Canvas,0,0,ScrW-1,ScrH-1);
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  CHAT_GUI *main_gui=malloc(sizeof(CHAT_GUI));
  zeromem(main_gui,sizeof(CHAT_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)CHAT_GUI_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  CreateGUI(main_gui);
}
