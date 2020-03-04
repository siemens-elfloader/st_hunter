
#define MAX_MENU_ITEMS 512
#define MAX_MENU_NAME 128

//Selected
_rgb menuselcolor =
{
  0, 0, 198, 128
};
_rgb menuselborder =
{
  255, 255, 255, 128
};



typedef struct _menuitem
{
  char caption[MAX_MENU_NAME];
  char type;
  _rgb pen;

}
_menuitem;



_menuitem * MenuItems;

char Button1Text[128];
char Button2Text[128];
char MenuTitle[128];
short PositionStore[512];
signed short PosStorePos = 0;

unsigned char NumOfItems, MenuActive;
signed char Selected, ShowCursor;

int( * MenuOnKeySub ) ( unsigned char key );

//void( * BackMenuDraw ) ();



void AddMenuItem( char * caption, unsigned int type, _rgb * pen )
{
  strcpy( MenuItems[NumOfItems].caption, caption );
  MenuItems[NumOfItems].type = type;
  memcpy( & MenuItems[NumOfItems].pen, pen, 4 );
  NumOfItems++;
}

void DrawMenu()
{
  _rgb brush =
  {
    0, 0, 0, 0
  };
  _rgb pen =
  {
    0, 0, 0, 255
  };
  int i, y;
  //BackMenuDraw();
  DrawBitmap( 0, 0, 132, 176, ( short * ) MenuBackground );
  DrawScreen();
  if ( NumOfItems == 0 ) return;
  WSHDR * ws = AllocWS( 256 );
  //wsprintf( ws, p_t, & MenuTitle[0] );
 // str_2ws(ws,& MenuTitle[0],strlen(& MenuTitle[0]));
  str_2ws((WSHDR *) ws,(char*) & MenuTitle[0],strlen(& MenuTitle[0]));
  DrawString( ws, 1, 2, ScreenW() - 2, 14, 7, 2, ( char * ) & pen, ( char * ) & brush );
  y = 16;
  if ( NumOfItems > 7 ) i = Selected - 3;
  else
    i = 0;
  if ( i < 0 ) i = 0;
  while ( i < NumOfItems )
  {
    //wsprintf( ws, p_t, MenuItems[i].caption );
    str_2ws((WSHDR *) ws,(char*) MenuItems[i].caption,strlen(MenuItems[i].caption));
    if ( ( i == Selected ) && ( ShowCursor ) )
    {
      DrawRoundedFrame( 2, y, ScreenW() - 2, y + 14, 0, 0, 0, ( char * ) & menuselborder, ( char * ) & menuselcolor );
      DrawString( ws, 3, y + 2, 130, y + 13, 7, 1, ( char * ) & MenuItems[i].pen, ( char * ) & brush );
    }
    else
    {
      DrawString( ws, 3, y + 2, 130, y + 13, 7, 1, ( char * ) & MenuItems[i].pen, ( char * ) & brush );
    }

    i++;
    y += 14;
    if ( y > ( ScreenH() - 24 ) ) break;
  }
  wsprintf( ws, p_t, & Button1Text[0] );
  DrawString( ws, 8, ScreenH() - 14, ScreenW() - 2, ScreenH() - 1, 7, 1, ( char * ) & pen, ( char * ) & brush );
  wsprintf( ws, p_t, & Button2Text[0] );
  DrawString( ws, 2, ScreenH() - 14, ScreenW() - 8, ScreenH() - 1, 7, 4, ( char * ) & pen, ( char * ) & brush );
  FreeWS( ws );
}

void StorePos()
{
  PositionStore[PosStorePos] = Selected;
  PosStorePos++;
}

void RestorePos()
{
  PosStorePos--;
  if ( PosStorePos < 0 ) PosStorePos = 0;
  Selected = PositionStore[PosStorePos];
}

void ClearMenu()
{
  //if(!MenuActive)PosStorePos=0;
  NumOfItems = 0;
  Selected = 0;
  MenuActive = 0;
  ShowCursor = 1;
}

void MenuInit()
{
  MenuItems = malloc( sizeof( _menuitem ) * MAX_MENU_ITEMS );
}

void MenuFree()
{
  mfree( MenuItems );
}


int MenuOnKey( unsigned char key )
{
  switch ( key )
  {
    case DOWN_BUTTON:
      Selected++;
      if ( Selected >= NumOfItems ) Selected = 0;
    break;
    case UP_BUTTON:
      Selected--;
      if ( Selected < 0 ) Selected = NumOfItems - 1;
    break;
  }
  REDRAW();
  return ( MenuOnKeySub( key ) );
}
