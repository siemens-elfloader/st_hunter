
extern int samplerate;

extern char GetFileType( char * filename );
char NSFOpened;
//Saved
int NsfSampleRate = 6;
int NsfDefTime = 6000;
int NsfDefClock = 50;
int NsfFilter = 0;
//
int NsfTime = 0;
int NsfSongLen = 6000;

#include "NES APU.C"
#include "NSF_FILE_SETUP.C"

void NsfSaveParams( int handle )
{
  fwrite( handle, & NsfSampleRate, sizeof( NsfSampleRate ), & err );
  fwrite( handle, & NsfDefTime, sizeof( NsfDefTime ), & err );
  fwrite( handle, & NsfDefClock, sizeof( NsfDefClock ), & err );
  fwrite( handle, & NsfFilter, sizeof( NsfDefTime ), & err );
}

void NsfLoadParams( int handle )
{
  fread( handle, & NsfSampleRate, sizeof( NsfSampleRate ), & err );
  fread( handle, & NsfDefTime, sizeof( NsfDefTime ), & err );
  fread( handle, & NsfDefClock, sizeof( NsfDefClock ), & err );
  fread( handle, & NsfFilter, sizeof( NsfDefTime ), & err );
}

int NSFOpen( char * filename1, char * filename2, WORD Song, WORD timing, WORD songtime )
{
  char * zipbuff; //=malloc(65536);
  int unzipsize = 0;
  int handle;
  NsfSongLen = songtime;
  samplerate = SRTable[NsfSampleRate];
  channels=2;
  NsfTime = 0;
  GetPlayingTime = GetNsfTime;
  speed = samplerate / timing;
  NsfSongLen = songtime;
  if ( GetFileType( filename1 ) != FT_ZIP )
  {
    handle = fopen( filename1, A_ReadOnly + A_BIN, P_READ, & err );
    if ( handle == -1 ) return ( 1 );

    fread( handle, & NSF_HDR, 128, & err );
    gameImage = malloc( 98304 );
    fread32( handle, ( void * ) ( gameImage + NSF_HDR.load ), 65536, & err );
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
    memcpy( & NSF_HDR, zipbuff, 128 );
    gameImage = malloc( 98304 );
    memcpy( ( void * ) ( gameImage + NSF_HDR.load ), zipbuff + 128, unzipsize );
  }
  MapperTest();
  GetSound = GetNsfSound;
  NSF_Init( Song );
  NSFOpened = 1;
  if ( unzipsize )
  {
    mfree( zipbuff );
  }
 // StBuff=malloc(16000*2);
 // zeromem(StBuff,16000*2);
 // StBuffPos=0;
  return ( 0 );
}

void NSFPlay()
{
  if ( Playing == 0 )
  {
    AudioStart();
  }
  else
    Paused = 0;
}

void NSFClose()
{
  if ( !NSFOpened ) return;
  //mfree(StBuff);
  mfree( gameImage );
  NSFOpened = 0;
}

int NSFLoatToPlayList( char * filename )
{
  int handle, i;
  char * sname = malloc( 256 );
  char * temp = malloc( 256 );
  handle = fopen( filename, A_ReadOnly + A_BIN, P_READ, & err );
  if ( handle == -1 ) return ( 1 );
  fread( handle, & NSF_HDR, 128, & err );
  fclose( handle, & err );
  ExtractFileName( filename, sname );
  i = 0;
  while ( i < NSF_HDR.tsongs )
  {
    sprintf( temp, "(%d)%s", i + 1, sname );
    PLAddFile( filename, temp, i, NsfDefClock, NsfDefTime );
    i++;
  }
  mfree( sname );
  mfree( temp );
}
