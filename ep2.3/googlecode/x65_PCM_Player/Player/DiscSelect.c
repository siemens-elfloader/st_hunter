
char * Discs[] =
{
  "0:", "1:", "2:", "4:"
};

int DiscMenuOnKey( unsigned char key )
{

  switch ( key )
  {
    case RED_BUTTON:
    case LEFT_SOFT:
      ShowMainMenu();
      RestorePos();
    break;
    case RIGHT_SOFT:
    case ENTER_BUTTON:
      StorePos();
      Explore( Discs[Selected] );
    break;

  }
  return ( 0 );
}


void ShowDiscMenu()
{
  _rgb pen =
  {
    0, 0, 0, 255
  };
  ClearMenu();
  AddMenuItem( "[Data]", 0, & pen );
  AddMenuItem( "[Cache]", 0, & pen );
  AddMenuItem( "[Config]", 0, & pen );
  AddMenuItem( "[MMC]", 0, & pen );
  MenuOnKeySub = DiscMenuOnKey;
  //BackMenuDraw = DiscMenuRedraw;
  strcpy( Button1Text, "Back" );
  strcpy( Button2Text, "Select" );
  strcpy( MenuTitle, "Select the drive:" );
  MenuActive = 1;
}

