int tst;
int SoundStereo=1;
char * TEST;
char TERMINATED=0;
//#define INPLCOLOR

//Externals
extern signed char Playing;
char CsmActive=0;

//#include "profile\profile.c"

#include "screen.h"
#include "..\myutils.c"
#include "zlib\unzip.c"
#include "images.c"
#include "MenuModule.c"
#include "PlayList.c"
#include "Player.c"
#include "ZipExplorer.c"
#include "Explorer.c"
#include "DiscSelect.c"
#include "MainMenu.c"
#include "MainScreen.c"
#include "siemens.c"




int onstart( char * exename, char * fname )
{ // Старт приложения. Возвращает 0 или 1 для выхода.
  //DisableIDLETMR();
  char *mininame=malloc(256);
  




  UnPackImages();
  GetPlayingTime = NullTime;
  ScreenInit();
  PlayListInit();
  MenuInit();
  TEST = malloc( 256 );
  //ExtractFileDir( "0:\\Sounds\\1.wav", TEST );
  uppercase( TEST );
  AudioInit();
  Ad_AudioInit();
  plcurrent = 0;
  //PlayerCmd = CMD_OPEN | CMD_PLAY;
  //NSFLoatToPlayList( "0:\\misc\\Treasure.nsf" );
  //PLAddFile( "0:\\Misc\\nsf\\nsf.zip", "Alien3.nsf", 0, 50, 3000 );
  //PLAddFile( "4:\\Sounds\\Music\\calypso.mp3", "Calypso.mp3", 0, 0, 0 );
  //PLAddFile( "0:\\misc\\LG\\LG1.mp3", "LG1.mp3", 0, 0, 0 );
  //PLAddFile( "0:\\misc\\mod\\mod.zip", "Scrambled mind.mod", 0, 0, 0 );
  //PLAddFile( "0:\\misc\\mod\\mod.zip", "BootMusic9.mod", 0, 0, 0 );
  //OpenFile( playlist[plcurrent].filename1, playlist[plcurrent].filename2, playlist[plcurrent].param1,
  //playlist[plcurrent].param2, playlist[plcurrent].songtime ) ;
  //Play();
  strcpy(PlayListName,"0:\\ZBin\\MediaPlayer\\Default.pls");
  
switch (GetFileType(fname)){
  
case FT_PLS:
    strcpy(PlayListName,fname);
    PlayerCmd = CMD_OPEN | CMD_PLAY;
    LoadPlayList( PlayListName );
  break;

  case AT_MP3:
  case AT_AAC:
  case AT_MOD:    
  case AT_WB: 
    //ExtractFileName(fname,mininame);
    //PLAddFile( fname, mininame, 0, 0, 0 );
    ExtractFileDir(fname,mininame);
    PLAddFolder(mininame);
    ExtractFileName(fname,mininame);
    while(strcmp((char*)&playlist[plcurrent].filename2,mininame))plcurrent++;
                  
    PlayerCmd = CMD_OPEN | CMD_PLAY;
  break;
  
case AT_NSF:
    NSFLoatToPlayList(fname);
    PlayerCmd = CMD_OPEN | CMD_PLAY;
  break;
default:
  LoadPlayList( PlayListName );
  
}
  //PLAddFile( "0:\\Sounds\\Blue Horizon.wav", "Blue Horizon.wav", 0, 0, 0 );
  //PLAddFile( "0:\\Misc\\1.mid", "1.mid", 0, 0, 0 );
  
  //SUBPROC((void*)PlayerProc);
  LoadParams(); 
  GBS_StartTimerProc( & playtimer, 16, PlayerProc );
  mfree(mininame);
  return 0;
}

void onclose()
{
  


  SavePlayList( PlayListName );
  Playing = 0;
  GBS_StopTimer( & playtimer );

 // if(IsPlayerOn)PlayMelody_StopPlayback(0xA87095C0);
  MP3Close();
  AACClose();
  NSFClose();
  MODClose();
  MidiClose();
  AmrWBClose();
  MenuFree();

  FreePlayList();

  AudioTerminate();
  Ad_AudioTerminate();
  mfree( TEST );
  ScreenFree();
  FreeImages();
  SaveParams();

}
