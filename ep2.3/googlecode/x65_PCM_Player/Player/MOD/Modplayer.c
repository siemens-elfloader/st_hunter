
//Process
char XmSurround=0;
short *XmSBuff=0;
int XmSBuffPos;

#include "judas\JUDAS.C"
#include "judas\JUDASXM.c"
#include "judas\JUDASMEM.C"
#include "judas\JUDASSMP.C"
#include "judas\zx_judasmix.C"

extern char GetFileType( char * filename );
extern int samplerate;

char MODOpened;

int modsamplerate = 6;
int ModFilter = 0;
int lastvalue;



//#include "mod.c"

void ModSaveParams( int handle )
{
  fwrite( handle, & modsamplerate, sizeof( modsamplerate ), & err );
  fwrite( handle, & ModFilter, sizeof( ModFilter ), & err );
}

int GetModPlayingTime()
{
  return ( XMPlaySamples / samplerate );
}

void ModLoadParams( int handle )
{
  fread( handle, & modsamplerate, sizeof( modsamplerate ), & err );
  fread( handle, & ModFilter, sizeof( ModFilter ), & err );
}

int MODOpen( char * filename1, char * filename2 )
{
  samplerate = SRTable[modsamplerate];
  channels=2;
  char * zipbuff; //=malloc(1024*512);
  int unzipsize = 0;

  if ( GetFileType( filename1 ) == FT_ZIP )
  {
    FromFile = 0;
    unzipsize = UnZipByFileName( filename1, filename2, & zipbuff, 1024 * 512 );
    memstream = zipbuff;
    streampos = 0;
    if ( !unzipsize ) return ( 1 );
  }
  else
  {
    FromFile = 1;
  }

  if(!judas_loadxm( filename1 ))return(1);
  judas_playxm();
  SmpSwit = samplerate / 50;
  SmpCnt = 1;
  if ( samplerate != 32000 ) GetSound = GetXmSound;
  else
  {
    if(XmSurround)GetSound = GetXmSound32Surr;else GetSound = GetXmSound32;
  }
  /* surround init */
  if(XmSurround)
  {
  XmSBuff=malloc(32000);
  zeromem(XmSBuff,32000);
  XmSBuffPos=0;
  }
  
  MODOpened = 1;
  if ( unzipsize ) mfree( zipbuff );
  XMPlaySamples = 0;
  GetPlayingTime = GetModPlayingTime;
  return ( 0 );
}

void MODPlay()
{
  bitrate = xm_header.channels * 1000;
  if ( Playing == 0 ) AudioStart();
  else Paused = 0;
}

void MODClose()
{
  if(XmSBuff)
  {
   mfree(XmSBuff);
   XmSBuff=0;
  }
  if ( MODOpened )
  {
    judas_freexm();

    MODOpened = 0;
  }
}
