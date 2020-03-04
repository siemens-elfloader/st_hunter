
extern void ShowOptionsMenu();
void ShowNsfOptions();
char * nullstr = "";

int NsfOptionsOnKey( unsigned char key )
{

  switch ( key )
  {
    case RED_BUTTON:
    case RIGHT_SOFT:
      ShowOptionsMenu();
      RestorePos();
    break;

    case LEFT_BUTTON:
      switch ( Selected )
      {
        case 0:
          if ( NsfDefClock > 10 ) NsfDefClock--;
        break;
        case 1:
          if ( NsfDefTime > 500 ) NsfDefTime -= 250;
        break;
        case 2:
          if ( NsfSampleRate > 0 ) NsfSampleRate--;
        break;
        case 3:
          if ( NsfFilter > 0 ) NsfFilter -= 4096;
        break;
      }
      StorePos();
      ShowNsfOptions();
      RestorePos();
    break;

    case RIGHT_BUTTON:
      switch ( Selected )
      {
        case 0:
          if ( NsfDefClock < 100 ) NsfDefClock++;
        break;
        case 1:
          if ( NsfDefTime < 60000 ) NsfDefTime += 250;
        break;
        case 2:
          if ( NsfSampleRate < 8 ) NsfSampleRate++;
        break;
        case 3:
          if ( ( NsfFilter / 4096 ) < 15 ) NsfFilter += 4096;
        break;
      }
      StorePos();
      ShowNsfOptions();
      RestorePos();
    break;
  }


  return ( 0 );
}

void ShowNsfOptions()
{
  _rgb pen =
  {
    0, 0, 0, 255
  };
  char * temp = malloc( 256 );
  ClearMenu();
  MenuOnKeySub = NsfOptionsOnKey;
  sprintf( temp, "%s <%d> Hz", "Default clock ", NsfDefClock );
  AddMenuItem( temp, 0, & pen );
  sprintf( temp, "%s <%d> sec", "Def. song len", NsfDefTime / 50 );
  AddMenuItem( temp, 0, & pen );
  sprintf( temp, "%s <%d> Hz", "Mix freq.", SRTable[NsfSampleRate] );
  AddMenuItem( temp, 0, & pen );
  sprintf( temp, "%s %d", "LF rate", NsfFilter / 4096 );
  AddMenuItem( temp, 0, & pen );
  strcpy( & MenuTitle[0], "NSF Options:" );
  strcpy( Button1Text, nullstr );
  strcpy( Button2Text, "Back" );
  MenuActive = 1;
  mfree( temp );
}
