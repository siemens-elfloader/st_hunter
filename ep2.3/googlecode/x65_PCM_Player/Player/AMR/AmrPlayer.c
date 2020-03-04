
#include "typedef.h"
#include "dec_if.h"

#include "..\Global.h"
extern const UWord8 block_size[];

int AmrSamples;
int AmrHFile;
char AMROpened = 0;
void * destate;
short synth[L_FRAME16k];
int SynthPos;
unsigned char * analysis;
int amrframepos = 0;
int AmrEof = 0;

Word16 dec_mode; 
int read_size;

#define AMR_BUFF_SIZE 4096
  

void AMRWBPlay()
{

  if ( Playing == 0 )
  {
    AudioStart();
  }
  else
    Paused = 0;
}

void AMRWBPause()
{
  Paused = !Paused;
}

int GetAMRWBTime()
{
  return ( AmrSamples/samplerate );
}

void ReadAmr()
{
  int err, readsize;
  if ( AmrEof )
  {
    AudioActive = 0;
    SavePLTime();
    return;
  }
  memcpy( analysis, analysis + amrframepos, AMR_BUFF_SIZE - amrframepos );
  readsize = fread( AmrHFile, analysis +AMR_BUFF_SIZE-amrframepos, amrframepos, & err );
  if ( readsize < amrframepos ) AmrEof = 1;
  amrframepos = 0;
}

void GetAMRWBSound( unsigned short * ptr, int nsamples )
{
  

  while ( nsamples )
  {
    * ptr++ = synth[SynthPos];
    * ptr++ = synth[SynthPos++];
    nsamples--;
    
    if ( SynthPos >= L_FRAME16k )
    {
      SynthPos=0;
      
      if ( AudioActive )
      {    
        AmrSamples+=L_FRAME16k;
        dec_mode = (Word16)(analysis[amrframepos] >> 4);
        read_size=block_size[dec_mode];        
        D_IF_decode( destate, & analysis[amrframepos], synth, 0);        
        amrframepos += read_size;
        if ( amrframepos > ( AMR_BUFF_SIZE - 128 ) ) ReadAmr();
        
      }
      else
      {
        zeromem( synth, sizeof( synth ) );
      }
    }
    
  nsamples-=1;
  }
}

int AMRWBOpen( char * filename1)
{
  int err;   
  AmrHFile = fopen( filename1, A_BIN, P_READ, & err );
  samplerate = 16000;
  //fread( AmrHFile, & samplerate, 2, & err );
  channels = 1;
  GetPlayingTime = GetAMRWBTime;
  GetSound = GetAMRWBSound;
  AMROpened = 1;
  destate = D_IF_init();
  //Decoder_Interface_Decode
  SynthPos = 0;
  amrframepos = 0;
  AmrEof = 0;
  AmrSamples=0;
  analysis = malloc( AMR_BUFF_SIZE );
  fread( AmrHFile, analysis, AMR_BUFF_SIZE, & err );
  AMROpened=1;
  return ( 0 );
}

int AmrWBClose()
{
  int err;
if(AMROpened)
{
fclose(AmrHFile,&err);
mfree(analysis);
D_IF_exit(destate);
AMROpened=0;
}
}