
extern void EnumFolder( char * folder );
extern char curpath[256];
extern void ShowFilesMenu();
extern int FilesMenuOnKey( unsigned char key );
extern void FilesMenuRedraw();
extern void Explore( char * Folder );

char * nulls = "";
char * saddstr = "Add";

char ZipFileName[256];
_rgb noinlistpen =
{
  0, 0, 0, 255
};
_rgb inlistpen =
{
  0, 255, 0, 255
};

int ZipOnKey( unsigned char key )
{
  char * temp = malloc( 256 );
  switch ( key )
  {
    case RIGHT_SOFT:
    case ENTER_BUTTON:
      if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_NSF )
      {
        PLAddFile( ZipFileName, MenuItems[Selected].caption, GetNsfDefTrack( ZipFileName, MenuItems[Selected].caption )-1,
             NsfDefClock, NsfDefTime );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
      if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_MOD )
      {
        PLAddFile( ZipFileName, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
            if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_MID )
      {
        PLAddFile( ZipFileName, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
    break;
    case RED_BUTTON:
      MenuActive=0;
      return(0);
    case LEFT_SOFT:    
      strcpy( temp, curpath );
      Explore( temp );
      RestorePos();
    break;

  }

  Button2Text[0] = 0;
  if ( ( ( GetFileType( & MenuItems[Selected].caption[0] ) ) == AT_NSF )
       || ( ( GetFileType( & MenuItems[Selected].caption[0] ) ) == AT_MOD ) )
         strcpy( Button2Text, saddstr );
  mfree( temp );
  return ( 0 );
}

void ZipExplorerDraw()
{
  _rgb brush =
  {
    255, 255, 255, 255
  };
  _rgb pen =
  {
    0, 0, 128, 255
  };
  _rgb pen1 =
  {
    128, 128, 128, 255
  };
  DrawRoundedFrame( 0, 0, ScreenW(), 14, 0, 0, 0, ( char * ) & brush, ( char * ) & pen );
  DrawRoundedFrame( 0, ScreenH() - 14, ScreenW(), ScreenH() - 1, 0, 0, 0, ( char * ) & brush, ( char * ) & pen );
  DrawRoundedFrame( 0, 15, ScreenW(), ScreenH() - 15, 0, 0, 0, ( char * ) & brush, ( char * ) & pen1 );
}

void SortZipFilesByName()
{
  _rgb rgbtemp;
  char typetemp;
  char changed;
  int i;
  char * temp = malloc( 256 );
loop:
  changed = 0;
  i = 1;
  while ( i < NumOfItems )
  {
    if ( MenuItems[i].caption[0] < MenuItems[i - 1].caption[0] )
    {
      strcpy( temp, MenuItems[i].caption );
      strcpy( MenuItems[i].caption, MenuItems[i - 1].caption );
      strcpy( MenuItems[i - 1].caption, temp );
      typetemp = MenuItems[i].type;
      MenuItems[i].type = MenuItems[i - 1].type;
      MenuItems[i - 1].type = typetemp;
      memcpy( & rgbtemp, & MenuItems[i].pen, 4 );
      memcpy( & MenuItems[i].pen, & MenuItems[i - 1].pen, 4 );
      memcpy( & MenuItems[i - 1].pen, & rgbtemp, 4 );
      changed = 1;
    }
    i++;
  }
  if ( changed ) goto loop;
  mfree( temp );
}

void OpenZipArchive( char * FileName )
{
  int zip, err;
  _ziphdr ziphdr;
  char * NameBuf = malloc( 256 );
  strcpy( ZipFileName, FileName );
  ClearMenu();
  zip = fopen( FileName, A_ReadOnly + A_BIN, P_READ, & err );
  while ( fread( zip, & ziphdr, 14, & err ) == 14 )
  {
    if ( ziphdr.signature != 0x04034b50 ) break;
    fread( zip, & ziphdr.crc32, 16, & err );
    zeromem( NameBuf, 256 );
    if ( ziphdr.fnamelen > 256 ) break;
    if ( ziphdr.fnamelen == 0 ) break;
    fread( zip, NameBuf, ziphdr.fnamelen, & err );
    lseek( zip, ziphdr.extralen + ziphdr.compsize, S_CUR, & err, & err );
#ifdef INPLCOLOR
    if ( TestIsInPlayList( NameBuf ) ) AddMenuItem( NameBuf, 0, & inlistpen );
    else
#endif
      AddMenuItem( NameBuf, 0, & noinlistpen );
  }
  ExtractFileName( FileName, & MenuTitle[0] );
  SortZipFilesByName();
  strcpy( Button1Text, "Back" );
  Button2Text[0] = 0;
  fclose( zip, & err );
  mfree( NameBuf );
  ShowCursor = 1;
  MenuActive = 1;
  MenuOnKeySub = ZipOnKey;
  Button2Text[0] = 0;
  if ( ( ( GetFileType( & MenuItems[Selected].caption[0] ) ) == AT_NSF )
       || ( ( GetFileType( & MenuItems[Selected].caption[0] ) ) == AT_MOD ) )
         strcpy( Button2Text, saddstr );
}


