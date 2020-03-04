
#include "OPTIONS.c"

int AboutOnKey( unsigned char key )
{

  switch ( key )
  {
  case RED_BUTTON:
  case RIGHT_SOFT:
  case ENTER_BUTTON:
  ShowMainMenu();
  RestorePos();
  }
  
  return ( 0 );
}

void ShowAbout()
{
  _rgb pen =
  {
    0, 0, 0, 255
  };

  ClearMenu();
  MenuOnKeySub = AboutOnKey;
  AddMenuItem( "MediaPlayer 1.2", 0, & pen );
  AddMenuItem( "(c) ILYAZ_ZX", 0, & pen );
  AddMenuItem( "rusman2005@mail.ru", 0, & pen );  
  sprintf( & MenuTitle[0], "About:");
  strcpy( Button1Text, nullstr );
  strcpy( Button2Text, "Back" );
  MenuActive = 1;
  ShowCursor=0;
}


int MainMenuOnKey( unsigned char key )
{

  switch ( key )
  {

    case RED_BUTTON:
    case LEFT_SOFT:
      MenuActive = 0;
    break;
    case RIGHT_SOFT:
    case ENTER_BUTTON:
      switch ( Selected )
      {
        case 0:
          if ( NumPLItems )
          {
            PlayListActive = 1;
            MenuActive = 0;
          }
        break;
        case 1:
          StorePos();
          ShowDiscMenu();
        break;
      case 2:
        StorePos();
        ShowOptionsMenu();
        break;
      case 3:
        StorePos();
        ShowAbout();
        break;
      }
  }
  return ( 0 );
}


void ShowMainMenu()
{
  _rgb pen =
  {
    0, 0, 0, 255
  };

  ClearMenu();
  AddMenuItem( "Playlist", 0, & pen );
  AddMenuItem( "Add files to playlist", 0, & pen );
  AddMenuItem( "Player options", 0, & pen );
  AddMenuItem( "About", 0, & pen );
  MenuOnKeySub = MainMenuOnKey;
  //BackMenuDraw = MainMenuRedraw;
  strcpy( Button1Text, "Back" );
  strcpy( Button2Text, "Select" );
  strcpy( MenuTitle, "Main menu" );
  MenuActive = 1;
}

