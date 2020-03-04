
short FileIndex; //,NumOfNsfTracks;
void ShowNsfFileSetup( short FIndex );

int GetNsfDefTrack( char * filename1, char * filename2 )
{
  char * zipbuff; //=malloc(65536);
  NSFHDR NSF_HEADER;
  int unzipsize;
  int handle;
  if ( GetFileType( filename1 ) != FT_ZIP )
  {
    handle = fopen( filename1, A_ReadOnly + A_BIN, P_READ, & err );
    if ( handle == -1 ) return ( 1 );
    fread( handle, & NSF_HEADER, 128, & err );
    fclose( handle, & err );
  }
  else
  {
    unzipsize = UnZipByFileName( filename1, filename2, & zipbuff, 65536 );
    if ( !unzipsize )
    {
      mfree( zipbuff );
      return ( 1 );
    }
    memcpy( & NSF_HEADER, zipbuff, 128 );
  }

  mfree( zipbuff );
  return ( NSF_HEADER.csong );
}

int GetNsfNumOfTracks( char * filename1, char * filename2 )
{
  char * zipbuff; //=malloc(65536);
  NSFHDR NSF_HEADER;
  int unzipsize;
  int handle;
  if ( GetFileType( filename1 ) != FT_ZIP )
  {
    handle = fopen( filename1, A_ReadOnly + A_BIN, P_READ, & err );
    if ( handle == -1 ) return ( 1 );
    fread( handle, & NSF_HEADER, 128, & err );
    fclose( handle, & err );
  }
  else
  {
    unzipsize = UnZipByFileName( filename1, filename2, & zipbuff, 65536 );
    if ( !unzipsize )
    {
      mfree( zipbuff );
      return ( 1 );
    }
    memcpy( & NSF_HEADER, zipbuff, 128 );
  }

  mfree( zipbuff );
  return ( NSF_HEADER.tsongs );
}

int NsfFileSetupOnKey( unsigned char key )
{

  switch ( key )
  {

    case LEFT_BUTTON:
      switch ( Selected )
      {
        case 0:
          if ( playlist[FileIndex].param2 > 10 ) playlist[FileIndex].param2--;
        break;
        case 1:
          if ( playlist[FileIndex].songtime > 500 ) playlist[FileIndex].songtime -= 250;
        break;
      }
    break;

    case RIGHT_BUTTON:
      switch ( Selected )
      {
        case 0:
          if ( playlist[FileIndex].param2 < 100 ) playlist[FileIndex].param2++;
        break;
        case 1:
          if ( playlist[FileIndex].songtime < 60000 ) playlist[FileIndex].songtime += 250;
        break;
      }
    break;
    case RIGHT_SOFT:
    case RED_BUTTON:
      MenuActive = 0;
      PlayListActive = 1;
      return ( 0 );
  }
  StorePos();
  ShowNsfFileSetup( FileIndex );
  RestorePos();
  return ( 0 );
}

void ShowNsfFileSetup( short FIndex )
{
  _rgb pen =
  {
    0, 0, 0, 255
  };
  char * temp = malloc( 256 );
  FileIndex = FIndex;
  // NumOfNsfTracks=GetNsfNumOfTracks(playlist[FileIndex].filename1,playlist[FileIndex].filename2);
  ClearMenu();
  MenuOnKeySub = NsfFileSetupOnKey;

  //sprintf(temp,"%s <%d>","NSF track",playlist[FileIndex].param1+1);
  // AddMenuItem( temp, 0, & pen );
  sprintf( temp, "%s <%d> Hz", "Clock ", playlist[FileIndex].param2 );
  AddMenuItem( temp, 0, & pen );
  sprintf( temp, "%s <%d> sec", "Song Len", playlist[FileIndex].songtime / 50 );
  AddMenuItem( temp, 0, & pen );
  //
  sprintf( & MenuTitle[0], "Setup for %t", playlist[FileIndex].filename2 );
  strcpy( Button1Text, "" );
  strcpy( Button2Text, "Back" );
  MenuActive = 1;
  mfree( temp );
}
