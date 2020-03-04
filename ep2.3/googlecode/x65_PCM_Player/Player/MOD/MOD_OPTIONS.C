
extern void ShowOptionsMenu();
void ShowModOptions();

int ModOptionsOnKey( unsigned char key )
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
        if(modsamplerate>0)modsamplerate--;
      break;
      case 1:
        if(ModFilter>0)ModFilter-=4096;
      break;
    }
    
    StorePos();
    ShowModOptions();
    RestorePos();    
    break;
    
  case RIGHT_BUTTON:
    switch ( Selected )
    {
      case 0:
        if(modsamplerate<8)modsamplerate++;
      break;
      case 1:
        if(ModFilter<61440)ModFilter+=4096;
      break;
    }
    StorePos();
    ShowModOptions();
    RestorePos();
    break;
  }


return(0);
}

void ShowModOptions()
{
  _rgb pen =
  {
    0, 0, 0, 255
  };
  char * temp = malloc( 256 );
  ClearMenu();
  MenuOnKeySub=ModOptionsOnKey;
  sprintf( temp, "%s <%d> Hz", "Mix freq.", SRTable[modsamplerate] );
  AddMenuItem( temp, 0, & pen );
  sprintf( temp, "%s %d", "LF rate ", ModFilter/4096 );
 // AddMenuItem( temp, 0, & pen );
  strcpy( & MenuTitle[0], "XM Options:" );
  
  strcpy( Button1Text, nullstr );
  strcpy( Button2Text, "Back" );
  MenuActive = 1;
  mfree( temp );
}
