

#include "NSF\NSF_OPTIONS.c"
#include "MOD\MOD_OPTIONS.c"
char *WavFmt[]={"PCM","ADPCM(mono)"};

int OptionsOnKey( unsigned char key )
{

  switch ( key )
  {
    case RIGHT_SOFT:
    case ENTER_BUTTON:
      switch ( Selected )
      {
        case 0:
        break;
        case 1:
          StorePos();
          ShowNsfOptions();
        break;
        case 2:
          StorePos();
          ShowModOptions();
        break;
      }
    break;

    case RED_BUTTON:
      ShowMainMenu();
      RestorePos();
      break;
      
  case LEFT_BUTTON:
     switch ( Selected )
     { case 3:
       if(AkkuLimit>2)AkkuLimit--;
       break;
       case 4:
       OutHook=OutHook^1;
       break;
        
      }
           StorePos();
    ShowOptionsMenu();
    RestorePos();
    break;
  case RIGHT_BUTTON:
         switch ( Selected )
     { case 3:
       if(AkkuLimit<99)AkkuLimit++;
       break;
       case 4:
       OutHook=OutHook^1;
       break;
        
      }
      StorePos();
    ShowOptionsMenu();
    RestorePos();
    break;
  }

  return ( 0 );
}

void ShowOptionsMenu()
{
  _rgb pen =
  {
    0, 0, 0, 255
  };

  ClearMenu();
  MenuOnKeySub = OptionsOnKey;
  AddMenuItem( "RealNetworks MP3/AAC Decoder", 0, & pen );
  AddMenuItem( "ILYA_ZX NSF Decoder", 0, & pen );
  AddMenuItem( "Judas XM Decoder", 0, & pen );
  //
  sprintf(& MenuTitle[0],"AkkuLimit %d percents",AkkuLimit);
  AddMenuItem(& MenuTitle[0],0,&pen);
  //
  sprintf(& MenuTitle[0],"Output <%s>",WavFmt[OutHook]);
  AddMenuItem(& MenuTitle[0],0,&pen);
  //
  sprintf( & MenuTitle[0], "Options" );
  strcpy( Button1Text, nullstr );
  strcpy( Button2Text, "Select" );
  MenuActive = 1;
}
