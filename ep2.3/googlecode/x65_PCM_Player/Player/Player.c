
unsigned short AkkuLimit=20;

extern void ShowMainMenu();

;

/* Audio types */
#define AT_UNK 255
#define AT_SRT 1
#define AT_IMY 2
#define AT_MID 10
#define AT_AMR 4
#define AT_WAV 5
#define AT_MP3 6
#define AT_NSF 7
#define AT_MOD 8
#define AT_AAC 9
#define AT_WB 11

#define FT_ZIP 128
#define FT_PLS 129

char AudioActive = 0;
char CurrentAT;
/* Громкость */
signed char Volume;
GBSTMR playtimer;
BYTE PlayerCmd;
char OutHook=1;
char OutHookCurrent=1;
int( * GetPlayingTime ) ();


#define CMD_NEXT 1
#define CMD_STOP 2
#define CMD_PLAY 4
#define CMD_OPEN 8
#define CMD_PREV 16

int SRTable[9] =
{
  8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
};

#include "audiohook\PCMaudiohook.c"
#include "audiohook\adpcmaudiohook.c"


void AudioStart()
{
if(OutHook)Ad_AudioStart();else P_AudioStart(); 
OutHookCurrent=OutHook;
}

void AudioStop()
{
if(OutHookCurrent)Ad_AudioStop();else P_AudioStop();   
}



unsigned int Get32BitVal( unsigned int * last )
{
  unsigned int r = * last;

  /* use same coefs as MPEG reference code (classic LCG)
  * use unsigned multiply to force reliable wraparound behavior in C (mod 2^32) */
  r = ( 1664525U * r ) + 1013904223U;
  * last = r;

  return r;
}


//#include "Amr\Amrplayer.c"
#include "MP3\Mp3player.c"
#include "aac\aacplayer.c"
#include "MOD\Modplayer.c"
#include "NSF\Nsfplayer.c"
#include "MIDI\MidiPlayer.c"
#include "Standart\Standart.c"


void SaveParams()
{
  int handle;

  
  unlink( "0:\\Zbin\\MediaPlayer\\Config.bin", & err );
  handle = fopen( "0:\\Zbin\\MediaPlayer\\Config.bin", A_ReadWrite + A_BIN + A_Append + A_Create, P_READ + P_WRITE, & err );
  if ( handle == -1 ) return;
  NsfSaveParams( handle );
  ModSaveParams( handle );

  fwrite( handle, & Volume, sizeof( Volume ), & err );
  fwrite( handle, & AkkuLimit, sizeof( AkkuLimit ), & err );
  fwrite( handle, & OutHook, sizeof( OutHook ), & err );
  fclose( handle, & err );
}

void LoadParams()
{
  int handle;

  handle = fopen( "0:\\Zbin\\MediaPlayer\\Config.bin", A_ReadWrite + A_BIN, P_READ, & err );
  if ( handle == -1 ) return;
  NsfLoadParams( handle );
  ModLoadParams( handle );
  fread( handle, & Volume, sizeof( Volume ), & err );
  fread( handle, & AkkuLimit, sizeof( AkkuLimit ), & err );
  fread( handle, & OutHook, sizeof( OutHook ), & err );
  fclose( handle, & err );
}

int NullTime()
{
  return ( 0 );
}

char GetFileType( char * filename )
{
  char * temp = malloc( 32 );

  ExtractFileExt( filename, temp );
  uppercase( temp );

  if ( !strcmp( temp, "SRT" ) )
  {
    mfree( temp );
    return ( AT_SRT );
  }

  if ( !strcmp( temp, "IMY" ) )
  {
    mfree( temp );
    return ( AT_IMY );
  }

  if ( !strcmp( temp, "MID" ) )
  {
    mfree( temp );
    return ( AT_MID );
  }

  if ( !strcmp( temp, "WB" ) )
  {
    mfree( temp );
    return ( AT_WB );
  }
  
  if ( !strcmp( temp, "AMR" ) )
  {
    mfree( temp );
    return ( AT_AMR );
  }

  if ( !strcmp( temp, "WAV" ) )
  {
    mfree( temp );
    return ( AT_WAV );
  }

  if ( !strcmp( temp, "MP3" ) )
  {
    mfree( temp );
    return ( AT_MP3 );
  }

  if ( !strcmp( temp, "NSF" ) )
  {
    mfree( temp ); return ( AT_NSF );
  }

  if ( !strcmp( temp, "XM" ) )
  {
    mfree( temp );
    return ( AT_MOD );
  }

  if ( !strcmp( temp, "AAC" ) )
  {
    mfree( temp );
    return ( AT_AAC );
  }


  if ( !strcmp( temp, "ZIP" ) )
  {
    mfree( temp );
    return ( FT_ZIP );
  }

  if ( !strcmp( temp, "PLS" ) )
  {
    mfree( temp );
    return ( FT_PLS );
  }

  return ( AT_UNK );
}

int OpenFile( char * filename1, char * filename2, WORD param1, WORD param2, WORD songtime )
{ int ret=0;


  CurrentAT = GetFileType( filename2 );

  
  AudioActive=1;
  
  
  if ( CurrentAT == AT_MP3 )
  {
    ret= ( MP3Open( filename1 ) );

  }


  if ( CurrentAT == AT_AAC )
  {
    ret= ( AACOpen( filename1 ) );

  }

  if ( CurrentAT == AT_MOD )
  {
    ret= ( MODOpen( filename1, filename2 ) );

  }

  if ( CurrentAT == AT_NSF )
  {
    ret= ( NSFOpen( filename1, filename2, param1, param2, songtime ) );

  }

  if ( CurrentAT < AT_MP3 )
  {
    ret= ( StandartOpen( filename1, filename2 ) );

  }

  if ( CurrentAT == AT_MID )
  {
    ret= ( MidiOpen( filename1, filename2 ) );

  }
  
  if ( CurrentAT == AT_WB )
  {
    ret= ( AMRWBOpen( filename1 ) );

  }
  
   AudioActive=0;
  return ( ret );
}

void Play()
{

  
  if ( CurrentAT == AT_MP3 )
  {
    AudioActive = 1;
    MP3Play();

  }
  if ( CurrentAT == AT_AAC )
  {
    AudioActive = 1;
    AACPlay();
  }

  if ( CurrentAT == AT_MOD )
  {
    AudioActive = 1;
    MODPlay();

  }

  if ( CurrentAT == AT_NSF )
  {
    NSFPlay();
    AudioActive = 1;
  }

  if ( CurrentAT < AT_MP3 )
  {
    AudioActive = 1;
    StandartPlay();

  }
  if ( CurrentAT == AT_MID )
  {
    AudioActive = 1;
    MidiPlay();

  }
   
   if ( CurrentAT == AT_WB )
  {
    AudioActive = 1;
    AMRWBPlay();

  }
}

void Stop()
{
  //PlayMelody_StopPlayback( 0xA87095C0 );

  if ( CurrentAT == AT_MP3 )
  {
    MP3Close();
  }

  if ( CurrentAT == AT_MOD )
  {
    MODClose();
  }

  if ( CurrentAT == AT_AAC )
  {
    AACClose();
  }

  if ( CurrentAT == AT_NSF )
  {
    NSFClose();
  }

  if ( CurrentAT < AT_MP3 )
  {
    StandartClose();
  }

  if ( CurrentAT == AT_MID )
  {
    MidiClose();
  }
  
  if ( CurrentAT == AT_WB )
  {
    AmrWBClose();
  }
  
}

void PlayerProc()
{
  if ( TERMINATED )
  {
    if ( !PlayStatus() ) GeneralFuncF1( 1 );
  }

  if ( NumPLItems )
  {

    if ( !PlayStatus() & ( !AudioActive ) )
    {
      if ( PlayerCmd & CMD_STOP )
      {
        SetHookState(0);
        Stop();        
        PlayerCmd ^= CMD_STOP;
        goto EXIT;
      }

      if ( PlayerCmd & CMD_NEXT )
      {
        plcurrent++;
        if ( plcurrent >= NumPLItems ) plcurrent = 0;
        PlayerCmd ^= CMD_NEXT;
        goto EXIT;
      }

      if ( PlayerCmd & CMD_PREV )
      {
        plcurrent--;
        if ( plcurrent < 0 ) plcurrent = NumPLItems - 1;
        PlayerCmd ^= CMD_PREV;
        goto EXIT;
      }

      if ( PlayerCmd & CMD_OPEN )
      {
        if ( !OpenFile( playlist[plcurrent].filename1, playlist[plcurrent].filename2, playlist[plcurrent].param1,
             playlist[plcurrent].param2, playlist[plcurrent].songtime ) )
               PlayerCmd ^= CMD_OPEN;
        else
          PlayerCmd |= CMD_NEXT;
        goto EXIT;
      }

      if ( PlayerCmd == CMD_PLAY )
      {
        if(AkkuLimit<*RamCap())
        Play();
        else ShowMSG(1,(int)"Akku Limit!");
        PlayerCmd ^= CMD_PLAY;
        goto EXIT;
      }
    }
  }
  if ( ( !MenuActive ) && ( !PlayListActive ) && ( CsmActive ) ) REDRAW();


EXIT:
  GBS_StartTimerProc( & playtimer, 64, PlayerProc );
}

void Send_MPlayer_Command( unsigned short cmd, short substract )
{ // Отправить команду плееру

  GBS_SendMessage( 0x4209, 0x8053, 0, 0x2c - substract, cmd );
}

int MainScreenOnKey( unsigned char key )
{

  switch ( key )
  {
  case '*':
    SoundStereo=SoundStereo^1;
    break;
    case '5':
      Paused = Paused ^ 1;
    break;
    //case '6':
    //  if(smpratemul<4)smpratemul+=0.02;
    //  samplerate=(float)realsamplerate*smpratemul;
    //break;
    //case '4':
    //  if(smpratemul>0.1)smpratemul-=0.02;
    //  samplerate=(float)realsamplerate*smpratemul;
   // break;
    case RIGHT_BUTTON:
      PlayerCmd = CMD_OPEN | CMD_STOP | CMD_PLAY;
      plcurrent++;      
      Playing = 0;
      //PlayMelodyInMem( 0x11, AudioBuf, 256 + 60, 0xFFFF, 0, 0 );
      if ( plcurrent >= NumPLItems ) plcurrent = 0;
    break;
    case LEFT_BUTTON:
      PlayerCmd = CMD_OPEN | CMD_STOP | CMD_PLAY;
      plcurrent--;      
      Playing = 0;
      //PlayMelody_StopPlayback(0xFFFF);
      //PlayMelodyInMem( 0x11, AudioBuf, 256 + 60, 0xFFFF, 0, 0 );
      if ( plcurrent < 0 ) plcurrent = NumPLItems - 1;
    break;

    case UP_BUTTON:
      SetVolume( Volume + 1 );
    break;
    case DOWN_BUTTON:
      SetVolume( Volume - 1 );
    break;

    case RED_BUTTON:
      TERMINATED = 1;      
      Playing = 0;
    break;
    case RIGHT_SOFT:
      PosStorePos = 0;
      ShowMainMenu();
    break;

    case '0':
      if ( ( GetFileType( playlist[plcurrent].filename2 ) ) == AT_NSF )
      {
        playlist[plcurrent].songtime = GetPlayingTime() * 50 + 250;
        NsfSongLen = playlist[plcurrent].songtime;
        SavePlayList( PlayListName );
      }
    break;

  }
  REDRAW();

  return ( 0 );
}
