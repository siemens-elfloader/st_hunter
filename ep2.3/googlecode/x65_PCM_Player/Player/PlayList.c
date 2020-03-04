
//extern signed short * AudioBuf;
extern void ShowNsfFileSetup( short FIndex );
extern int( * GetPlayingTime ) ();

#define AT_MP3 6
#define AT_MOD 8
#define AT_AAC 9
#define AT_WB 11

#define MAX_PL_ITEMS 512
#define FN1_MAX_SYM 256
#define FN2_MAX_SYM 128

#define AT_NSF   7

#define CMD_NEXT 1
#define CMD_STOP 2
#define CMD_PLAY 4
#define CMD_OPEN 8

const BYTE SymbolH = 14;
const BYTE SymbolW = 6;
char PlayListActive = 0;
extern char GetFileType( char * filename );


typedef struct
{
  char filename1[FN1_MAX_SYM];
  char filename2[FN2_MAX_SYM];
  WORD param1, param2;
  WORD songtime;
}
plitem;

extern BYTE PlayerCmd;
plitem * playlist;
WORD NumPLItems = 0;
BYTE plx = 1, ply = 1, plw = 131, plh = 175;
BYTE CurrNsfSongs;
char PlayListName[256] = "";
char PlayListTitle[32] = "Playlist";
int plselected, plcurrent;
int pltextattr = 2;
//Selected
_rgb plselcolor =
{
  0, 0, 198, 128
};
_rgb plselborder =
{
  255, 255, 255, 255
};

//Currplaying
_rgb plcurpen =
{
  255, 255, 255, 255
};
_rgb plcurbrush =
{
  0, 0, 0, 0
};

//Item
_rgb plpen =
{
  0, 255, 0, 255
};
_rgb plbrush =
{
  0, 0, 0, 0
};

void ExchPLItens( unsigned short I1, unsigned short I2 )
{
  plitem temp;
  memcpy( & temp, & playlist[I1], sizeof( temp ) );
  memcpy( & playlist[I1], & playlist[I2], sizeof( temp ) );
  memcpy( & playlist[I2], & temp, sizeof( temp ) );
}

void PLDeleteItem( unsigned short ItemIndex )
{
  int i;
  while ( ItemIndex < ( NumPLItems - 1 ) )
  {
    memcpy( & playlist[ItemIndex], & playlist[ItemIndex + 1], sizeof( plitem ) );
    ItemIndex++;
  }
  NumPLItems--;
}

void PlayListInit()
{
  /* int i = 0; while ( i < MAX_PL_ITEMS ) { playlist[i] = malloc( sizeof( plitem ) );
  playlist[i].filename1 = malloc( FN1_MAX_SYM ); playlist[i].filename2 = malloc( FN2_MAX_SYM ); i++; } */
  playlist = malloc( sizeof( plitem ) * MAX_PL_ITEMS );
  NumPLItems = 0;
  plx = 1;
  plw = ScreenW() - 2;
  ply = 14;
  plh = ScreenH() - 14 - 20;
}

void FreePlayList()
{
  /* int i = 0; while ( i < MAX_PL_ITEMS ) { mfree( playlist[i].filename1 ); mfree( playlist[i].filename2 );
  mfree( playlist[i] ); i++; } */
  mfree( playlist );
}

void SavePlayList( char * filename )
{
  DWORD handle, err, i;
  BYTE ii;
  char * tempbuf = malloc( 32768 );
  char * tempbufpos = tempbuf;


  filename[strlen( filename ) - 1] --;
  unlink( filename, & err );
  //
  handle = fopen( filename, A_ReadWrite + A_BIN + A_Append + A_Create, P_READ + P_WRITE, & err );
  
  fwrite( handle, & plcurrent, sizeof( plcurrent ), & err );
  fwrite( handle, & NumPLItems, sizeof( NumPLItems ), & err );
  
  if ( !NumPLItems )
  {
    mfree( tempbuf );
    fclose( handle, & err );
    return;
  }

  i = 0;
  while ( i < NumPLItems )
  {
    ii = strlen( playlist[i].filename1 );
    //fwrite( handle, & ii, 1, & err );
    memcpy( tempbufpos, & ii, 1 );
    tempbufpos += 1;
    //fwrite( handle, playlist[i].filename1, ii, & err );
    if ( ii ) memcpy( tempbufpos, playlist[i].filename1, ii );
    tempbufpos += ii;
    ii = strlen( playlist[i].filename2 );
    //fwrite( handle, & ii, 1, & err );
    memcpy( tempbufpos, & ii, 1 );
    tempbufpos += 1;
    //fwrite( handle, playlist[i].filename2, ii, & err );
    if ( ii ) memcpy( tempbufpos, playlist[i].filename2, ii );
    tempbufpos += ii;
    //fwrite( handle, & playlist[i].param1, 6, & err );
    memcpy( tempbufpos, & playlist[i].param1, 6 );
    tempbufpos += 6;
    i++;
  }
  fwrite32( handle, tempbuf, tempbufpos - tempbuf, & err );
  fclose( handle, & err );

  //
  strcpy( tempbuf, filename );
  filename[strlen( filename ) - 1] ++;
  unlink( filename, & err );
  fmove( tempbuf, filename, & err );
  mfree( tempbuf );

}

void LoadPlayList( char * filename )
{
  DWORD handle, i;
  int err;
  BYTE ii;


  handle = fopen( filename, A_ReadOnly + A_BIN, P_READ, & err );
  NumPLItems = 0;
  fread( handle, & plcurrent, sizeof( plcurrent ), & err );
  fread( handle, & NumPLItems, sizeof( NumPLItems ), & err );
  if ( !NumPLItems )
  {
    fclose( handle, & err );
    return;
  }

  i = 0;
  while ( i < NumPLItems )
  {
    fread( handle, & ii, 1, & err );
    fread( handle, playlist[i].filename1, ii, & err );
    playlist[i].filename1[ii] = 0;
    fread( handle, & ii, 1, & err );
    fread( handle, playlist[i].filename2, ii, & err );
    playlist[i].filename2[ii] = 0;
    fread( handle, & playlist[i].param1, 6, & err );
    i++;
  }
  fclose( handle, & err );



}

void DrawPlayList()
{
  int Y, N, sec, min;
  _rgb pen =
  {
    0, 0, 0, 255
  };
  char temptime[5];
  if ( !NumPLItems ) return;

  DrawBitmap( 0, 0, 132, 176, ( short * ) PlayListBackground );
  DrawScreen();

  N = 0;
  if ( ( plselected > ( N + plh / SymbolH / 2 ) ) ) N = plselected - plh / SymbolH / 2;
  if ( N >= NumPLItems ) N = NumPLItems - 1;
  if ( N < 0 ) N = 0;
  Y = 2;
  WSHDR * ws = AllocWS( 256 );
  //DrawRoundedFrame( plx, ply, plx + plw, ply + plh, 0, 0, 0, GetPaletteAdrByColorIndex( 1 ), GetPaletteAdrByColorIndex( 1 ) );
  while ( Y < ( plh - ( SymbolH / 2 ) ) )
  {
    if ( GetFileType( playlist[N].filename2 ) == AT_NSF )
      wsprintf( ws, "<%d>%t", playlist[N].param1 + 1, playlist[N].filename2 );
    else
         //wsprintf( ws, "%t", playlist[N].filename2 );
           str_2ws( ( WSHDR * ) ws, ( char * ) playlist[N].filename2, strlen( playlist[N].filename2 ) );
    //Calc Time
    min = playlist[N].songtime / 3000;
    sec = ( playlist[N].songtime / 50 ) - min * 60;
    if ( min > 9 )
    {
      if ( sec > 9 )
      {
        sprintf( temptime, "%d:%d", min, sec );
      }
      else
      {
        sprintf( temptime, "%d:0%d", min, sec );
      }
    }
    else
    {
      if ( sec > 9 )
      {
        sprintf( temptime, "0%d:%d", min, sec );
      }
      else
      {
        sprintf( temptime, "0%d:0%d", min, sec );
      }
    }
    if ( N == plselected )
      DrawRoundedFrame( plx + 1, Y - 2 + ply, plx + plw - 2, Y + 12 + ply, 0, 0, 0, ( char * ) & plselborder,
           ( char * ) & plselcolor );
    if ( N == plcurrent )
    {
      DrawString( ws, plx + 2, Y + ply, plx + plw - 2 - 6 * SymbolW, Y + ply + 11, 7, 1, ( char * ) & plcurpen,
           ( char * ) & plcurbrush );
      wsprintf( ws, p_t, temptime );
      DrawString( ws, plx + plw - 2 - 5 * SymbolW, Y + ply, plx + plw - 2, Y + ply + 11, 7, 1, ( char * ) & plcurpen,
           ( char * ) & plcurbrush );
    }
    else
    {
      DrawString( ws, plx + 2, Y + ply, plx + plw - 2 - 6 * SymbolW, Y + ply + 11, 7, 1, ( char * ) & plpen,
           ( char * ) & plbrush );
      wsprintf( ws, p_t, temptime );
      DrawString( ws, plx + plw - 2 - 5 * SymbolW, Y + ply, plx + plw - 2, Y + ply + 11, 7, 1, ( char * ) & plpen,
           ( char * ) & plbrush );
    }
    Y += SymbolH;
    N++;
    if ( N >= NumPLItems ) break;
  }
  wsprintf( ws, p_t, & PlayListTitle[0] );
  DrawString( ws, 1, 2, ScreenW() - 2, 14, 7, 2, ( char * ) & pen, ( char * ) & plbrush );
  wsprintf( ws, p_t, "Delete" );
  DrawString( ws, 8, ScreenH() - 14, ScreenW() - 2, ScreenH() - 1, 7, 1, ( char * ) & pen, ( char * ) & plbrush );
  wsprintf( ws, p_t, "Options" );
  DrawString( ws, 2, ScreenH() - 14, ScreenW() - 8, ScreenH() - 1, 7, 4, ( char * ) & pen, ( char * ) & plbrush );

  FreeWS( ws );

}

void PLAddFile( char * filename1, char * filename2, WORD param1, WORD param2, WORD songtime )
{


  strcpy( playlist[NumPLItems].filename1, filename1 );
  strcpy( playlist[NumPLItems].filename2, filename2 );
  playlist[NumPLItems].param1 = param1;
  playlist[NumPLItems].param2 = param2;
  playlist[NumPLItems].songtime = songtime;
  NumPLItems++;


}

void PLAddFolder( char * folder )
{
  DIR_ENTRY * dir = malloc( sizeof( DIR_ENTRY ) );;
  int err;
  unsigned char * mask, * temp;
  temp = malloc( 256 );
  mask = malloc( 256 );
  zeromem( dir, sizeof( DIR_ENTRY ) );

  zeromem( dir, sizeof( DIR_ENTRY ) );
  sprintf( mask, "%s%s", folder, "\\*.*" );

  if ( FindFirstFile( dir, mask, & err ) == 1 )
  {
    sprintf( temp, ( char * ) "%s\\%s", folder, dir->file_name );
    if ( ( !isdir( temp, & err ) )
         & ( ( GetFileType( ( char * ) & dir->file_name ) == AT_MP3 )
         | ( GetFileType( ( char * ) & dir->file_name ) == AT_MOD )
         | ( GetFileType( ( char * ) & dir->file_name ) == AT_AAC )
         | ( GetFileType( ( char * ) & dir->file_name ) == AT_WB )  ) )
           PLAddFile( temp, dir->file_name, 0, 0, 0 );

    while ( FindNextFile( dir, & err ) == 1 )
    {
      sprintf( temp, ( char * ) "%s\\%s", folder, dir->file_name );
      if ( ( !isdir( temp, & err ) )
           & ( ( GetFileType( ( char * ) & dir->file_name ) == AT_MP3 )
           | ( GetFileType( ( char * ) & dir->file_name ) == AT_MOD )
           | ( GetFileType( ( char * ) & dir->file_name ) == AT_AAC ) 
            | ( GetFileType( ( char * ) & dir->file_name ) == AT_WB ) ) )
             PLAddFile( temp, dir->file_name, 0, 0, 0 );
    }
  }
  FindClose( dir, & err );

  mfree( mask );
  mfree( temp );
  mfree( dir );
}

void PLOnKey( int key )
{
  switch ( key )
  {
    case LEFT_SOFT:
      if ( plcurrent != plselected ) PLDeleteItem( plselected );
      if ( plselected < plcurrent ) plcurrent--;
    break;
    case '2':
      if ( plselected > 0 )
      {
        ExchPLItens( plselected, plselected - 1 );
        if ( ( plcurrent ) == plselected ) plcurrent--;
        else if ( ( plcurrent + 1 ) == plselected ) plcurrent++;
        plselected--;
      }
    break;
    case '8':
      if ( plselected < ( NumPLItems - 1 ) )
      {
        ExchPLItens( plselected, plselected + 1 );
        if ( ( plcurrent ) == plselected ) plcurrent++;
        else if ( ( plcurrent - 1 ) == plselected ) plcurrent--;
        plselected++;
      }
    break;
    case DOWN_BUTTON:
      plselected++;
      CurrNsfSongs = 0;
      if ( plselected >= NumPLItems ) plselected = 0;
    break;
    case UP_BUTTON:
      plselected--;
      CurrNsfSongs = 0;
      if ( plselected < 0 ) plselected = NumPLItems - 1;
    break;
    case ENTER_BUTTON:
      PlayerCmd = CMD_STOP | CMD_OPEN | CMD_PLAY;
      plcurrent = plselected;
      Playing=0;
      //PlayMelodyInMem( 0x11, AudioBuf, 256 + 60, 0xFFFF, 0, 0 );
    break;
    case LEFT_BUTTON:
      if ( GetFileType( playlist[plselected].filename2 ) == AT_NSF )
        if ( playlist[plselected].param1 ) playlist[plselected].param1--;
    break;
    case RIGHT_BUTTON:
      if ( GetFileType( playlist[plselected].filename2 ) == AT_NSF )
        if ( !CurrNsfSongs )
          CurrNsfSongs = GetNsfNumOfTracks( playlist[plselected].filename1, playlist[plselected].filename2 );
      if ( GetFileType( playlist[plselected].filename2 ) == AT_NSF )
        if ( playlist[plselected].param1 < ( CurrNsfSongs - 1 ) ) playlist[plselected].param1++;
    break;
    case RED_BUTTON:
      PlayListActive = 0;
      MenuActive = 0;
    break;
    case RIGHT_SOFT:
      if ( GetFileType( playlist[plselected].filename2 ) == AT_NSF ) ShowNsfFileSetup( plselected );
      PlayListActive = 0;
    break;

  }
  if ( !NumPLItems )
  {
    PlayListActive = 0;
    MenuActive = 0;
  }
}
void SavePLTime()
{
playlist[plcurrent].songtime = GetPlayingTime() * 50;
    SavePlayList( PlayListName );
    
}
#ifdef INPLCOLOR
int TestIsInPlayList( char * Name )
{
  int i;
  if ( !NumPLItems ) return ( 0 );
  char * rec1 = malloc( 256 ), * rec2 = malloc( 256 );
  if ( NumPLItems == 0 ) return ( 0 );
  strcpy( rec1, Name );
  uppercase( rec1 );
  i = 0;
  while ( i < NumPLItems )
  {
    strcpy( rec2, playlist[i].filename2 );
    uppercase( rec2 );
    if ( !strcmp( rec1, rec2 ) ) break;
    i++;
  }
  mfree( rec1 );
  mfree( rec2 );
  if ( i == NumPLItems ) return ( 0 );
  return ( 1 );
}


    
#endif


