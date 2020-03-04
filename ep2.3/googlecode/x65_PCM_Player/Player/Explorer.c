char curpath[256];
extern void ShowDiscMenu();
int FilesMenuOnKey( unsigned char key );

char * sym_back = "..";
char * dbslash = "\\";
char * p_sss = "%s%s%s";
char * p_ss = "%s%s";
char * nullc = "";

void FilesMenuRedraw()
{
 
}

void DoFolderBack( char * folder )
{
  int i = 0;
  while ( folder[i] != 0 ) i++;
  while ( folder[i] != '\\' ) i--;
  i--;
  while ( folder[i] != '\\' ) i--;
  folder[i] = 0;
}

void SortFilesByName()
{
  _rgb rgbtemp;
  char typetemp;
  char changed;
  int i;
  char * temp = malloc( 256 );
loop:
  changed = 0;
  i = 2;
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

void Explore( char * Folder )
{
  _rgb pen =
  {
    0, 0, 0, 255
  };
  ClearMenu();
  zeromem( curpath, 256 );
  MenuOnKeySub = FilesMenuOnKey;
  //BackMenuDraw = FilesMenuRedraw;
  strcpy( curpath, nullc );
  EnumFolder( Folder );
  strcpy( Button1Text, "Back" );
  strcpy( Button2Text, "Open" );
  MenuActive = 1;

}

void EnumFolder( char * folder )
{
  _rgb pen =
  {
    0, 0, 0, 255
  };
  _rgb inlistpen =
  {
    0, 255, 0, 255
  };
  DIR_ENTRY * dir;
  unsigned char * mask, * temp;

  temp = malloc( 256 );
  mask = malloc( 256 );
  dir = malloc( sizeof( DIR_ENTRY ) );
  zeromem( dir, sizeof( DIR_ENTRY ) );
  if ( strlen( curpath ) && ( strcmp( folder, sym_back ) ) ) StorePos();
  ClearMenu();

  if ( !strcmp( folder, sym_back ) )
  {

    if ( strlen( curpath ) == 3 )
    {
      ShowDiscMenu();
      mfree( mask );
      mfree( temp );
      mfree( dir );
      RestorePos();
      return;
    }
    RestorePos();
    DoFolderBack( curpath );
    folder[0] = 0;
  }

  sprintf( mask, p_sss, curpath, folder, "\\*.*" );
  sprintf( temp, p_sss, curpath, folder, "\\" );
  strcpy( curpath, temp );
  AddMenuItem( sym_back, 1, & pen );
  if ( FindFirstFile( dir, mask, & err ) == 1 )
  {
    sprintf( temp, ( char * ) p_ss, curpath, dir->file_name );
#ifdef INPLCOLOR
    if ( TestIsInPlayList( dir->file_name ) ) AddMenuItem( dir->file_name, isdir( temp, & err ), & inlistpen );
    else
#endif
      AddMenuItem( dir->file_name, isdir( temp, & err ), & pen );

    while ( FindNextFile( dir, & err ) == 1 )
    {
      sprintf( temp, ( char * ) p_ss, curpath, dir->file_name );

#ifdef INPLCOLOR
      if ( TestIsInPlayList( dir->file_name ) ) AddMenuItem( dir->file_name, isdir( temp, & err ), & inlistpen );
      else
#endif
        AddMenuItem( dir->file_name, isdir( temp, & err ), & pen );
    }
  }
  FindClose( dir, & err );
  mfree( mask );
  mfree( temp );
  mfree( dir );
  SortFilesByName();
  strcpy( & MenuTitle[0], curpath );
  MenuActive = 1;
}

int FilesMenuOnKey( unsigned char key )
{
  char * temp = malloc( 256 );
  switch ( key )
  {
    case RIGHT_SOFT:
    case ENTER_BUTTON:
      sprintf( temp, p_ss, curpath, MenuItems[Selected].caption );
      if ( ( GetFileType( temp ) ) == FT_ZIP )
      {
        curpath[strlen( curpath ) - 1] = 0;
        StorePos();
        OpenZipArchive( temp );
        mfree( temp );
        return ( 0 );
      }
      if ( MenuItems[Selected].type ) EnumFolder( MenuItems[Selected].caption );
      if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_NSF )
      {
        PLAddFile( temp, MenuItems[Selected].caption, GetNsfDefTrack( temp, MenuItems[Selected].caption ),
             NsfDefClock, NsfDefTime );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
      
      if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_MOD )
      {
        PLAddFile( temp, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
      
      if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_WB)
      {
        PLAddFile( temp, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
      
        if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_MID )
      {
        PLAddFile( temp, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
      if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_MP3 )
      {
        PLAddFile( temp, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
       
            if ( GetFileType( & MenuItems[Selected].caption[0] ) == AT_AAC)
      {
        PLAddFile( temp, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
      
      
      if ( GetFileType( & MenuItems[Selected].caption[0] ) < AT_MP3 )
      {
        PLAddFile( temp, MenuItems[Selected].caption, 0, 0, 0 );
        memcpy( & MenuItems[Selected].pen, & inlistpen, 4 );
      }
      
    break;
 case RED_BUTTON:
      MenuActive=0;
      return(0);
      
    case LEFT_SOFT:
      EnumFolder( sym_back );
    break;
  }
  if ( MenuItems[Selected].type ) strcpy( Button2Text, "Open" );
  else
    strcpy( Button2Text, nullc );
  if ( ( GetFileType( & MenuItems[Selected].caption[0] ) <= AT_MOD )
      |( GetFileType( & MenuItems[Selected].caption[0] ) >= AT_MP3 ) )
    
         strcpy( Button2Text, "Add" );
  
  mfree( temp );
  return ( 0 );
}
