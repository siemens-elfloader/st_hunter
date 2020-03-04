
#include "aacDec\aacdec.h"
#include "aacDec\aaccommon.h"


#define AAC_BUFFER_SIZE 16384

extern int samplerate;
int AacPlaySamples;

unsigned char * AACFileBuff = 0, * AACFileBuffPos;
int aacfilehandle = 0;
signed int err;
char AACeof, AACEndSong, AACDisableSound;
unsigned short samplesize;
unsigned long FramePos;
HAACDecoder AACDecoder;
int bytesLeft, bytes;
AACFrameInfo aacFrameInfo;
char AACOpened = 0;

void AACClose()
{



  if ( !AACOpened ) return;
  if ( aacfilehandle != 0 )
  {
    fclose( aacfilehandle, & err );
    aacfilehandle = 0;
  }
  if ( AACFileBuff )
  {
    mfree( AACFileBuff );
    AACFileBuff = 0;
  }
  if ( framewavebuff )
  {
    mfree( framewavebuff );
    framewavebuff = 0;
  }
  AACFreeDecoder( AACDecoder );
  AACOpened = 0;


}

void ReadAacFile()
{
  if ( AACeof != 0 )
  {
    return;
  }
  memcpy( AACFileBuff, AACFileBuffPos, bytesLeft );
  //bytes = fread( ( void * )( AACFileBuff + bytesLeft ),1,  AAC_BUFFER_SIZE - bytesLeft ,aacfilehandle);
  
  bytes = fread( aacfilehandle, ( AACFileBuff + bytesLeft ), AAC_BUFFER_SIZE - bytesLeft, & err );

  bytesLeft = bytesLeft + bytes;
  AACFileBuffPos = AACFileBuff;
  if ( bytes == 0 )
  {
    AACeof = 1;
    fclose( aacfilehandle, & err );
    aacfilehandle = 0;
  }
}

int GetAacPlayingTime()
{
  return ( AacPlaySamples / samplerate );
}


void GetAacSound( unsigned short * ptr, int nsamples )
{
  int offset;
  if ( !AudioActive )
  {
    zeromem( ptr, nsamples * 2 );
    return;
  }
  if ( AACEndSong )
  {
    AudioActive = 0;
    playlist[plcurrent].songtime = GetPlayingTime() * 50;
    SavePlayList( PlayListName );
    zeromem( ptr, nsamples * 2 );
    return;
  }

  AacPlaySamples += nsamples / 2;

  if ( channels == 2 )
  {

    while ( nsamples )
    {
      if ( FramePos >= samplesize )
      {
        offset = AACFindSyncWord( AACFileBuffPos, bytesLeft );
        AACFileBuffPos = AACFileBuffPos + offset;
        bytesLeft = bytesLeft - offset;        
        err = AACDecode( AACDecoder, & AACFileBuffPos, & bytesLeft, framewavebuff );
        //bitrate=(bitrate+((AACDecInfo *)AACDecoder)->bitRate)/2;
        //bitrate=((AACDecInfo *)AACDecoder)->bitRate;
        if ( err == ERR_AAC_INDATA_UNDERFLOW )
        {
          AACEndSong = 1;
          zeromem( ptr, nsamples * 2 );
          return;
        }
        if ( bytesLeft < ( MAINBUF_SIZE * 4 ) )
        {
          ReadAacFile();
        }
        FramePos = 0;
      }
      * ptr++ = framewavebuff[FramePos++];
      nsamples -= 1;
    }
  }
  else
  {
    while ( nsamples )
    {
      if ( FramePos >= samplesize )
      {
        offset = AACFindSyncWord( AACFileBuffPos, bytesLeft );
        AACFileBuffPos = AACFileBuffPos + offset;
        bytesLeft = bytesLeft - offset;
        err = AACDecode( AACDecoder, & AACFileBuffPos, & bytesLeft, framewavebuff );
        //bitrate=(bitrate+((AACDecInfo *)AACDecoder)->bitRate)/2;
        //bitrate=((AACDecInfo *)AACDecoder)->bitRate;
        if ( err == ERR_AAC_INDATA_UNDERFLOW )
        {
          AACEndSong = 1;
          zeromem( ptr, nsamples * 2 );
          return;
        }
        if ( bytesLeft < ( MAINBUF_SIZE * 4 ) )
        {
          ReadAacFile();
        }
        FramePos = 0;
      }
      * ptr++ = framewavebuff[FramePos];
      * ptr++ = framewavebuff[FramePos++];
      nsamples -= 2;
    }
  }
}


/* void GetAacSound(unsigned short *L, unsigned short *R) { int offset; if(!AudioActive){ *L=0; *R=0; return; }
if(AACEndSong){ AudioActive=0; playlist[plcurrent].songtime=GetPlayingTime()*50;    SavePlayList( PlayListName
); *L=0; *R=0; return; }

if ( FramePos >= samplesize ) { offset = AACFindSyncWord( AACFileBuffPos, bytesLeft );
AACFileBuffPos = AACFileBuffPos + offset; bytesLeft = bytesLeft - offset;
err = AACDecode( AACDecoder, & AACFileBuffPos, & bytesLeft, framewavebuff );
//bitrate=(bitrate+((AACDecInfo *)AACDecoder)->bitRate)/2;    //bitrate=((AACDecInfo *)AACDecoder)->bitRate;
if(err==ERR_AAC_INDATA_UNDERFLOW){ AACEndSong=1; *L=0; *R=0; return; } if ( bytesLeft < (MAINBUF_SIZE*4) ) { ReadAacFile(); }

FramePos=0; }

AacPlaySamples++;

if(channels==2) { *L=framewavebuff[FramePos]; *R=framewavebuff[FramePos+1] ; } else { *L=framewavebuff[FramePos];
*R=framewavebuff[FramePos]; } FramePos+=channels; } */

int AACOpen( char * aacfilename )
{
  int offset;
  int recbytesLeft;
  GetSound=GetAacSound;


  //aacfilehandle = fopen( aacfilename, A_ReadOnly + A_BIN, P_READ, & err );
  aacfilehandle = fopen( aacfilename, A_BIN, P_READ, & err );
  if ( aacfilehandle == -1 )
  {

    return ( 1 );
  }
  AACDecoder=(HAACDecoder *)AACInitDecoder();

  if ( !( AACFileBuff = malloc( AAC_BUFFER_SIZE ) ) ) return ( 1 );
  if ( !( framewavebuff = malloc( 2304 * 2 ) ) )
  {
    mfree( AACFileBuff );
    return ( 1 );
  }
  AACeof = 0;
  AACDisableSound = 0;
  AACEndSong = 0;
  AACFileBuffPos = AACFileBuff;
  FramePos = 0;
  zeromem( AACFileBuff, AAC_BUFFER_SIZE );
  zeromem( framewavebuff, 2304 * 2 );
  bytesLeft = 0;
  ReadAacFile();
  recbytesLeft = bytesLeft;
  //
  offset = AACFindSyncWord( AACFileBuffPos, bytesLeft );
  AACFileBuffPos = AACFileBuffPos + offset;
  bytesLeft = bytesLeft - offset;
  err = AACDecode( AACDecoder, & AACFileBuffPos, & bytesLeft, framewavebuff );
  if ( err )
  {
    mfree( AACFileBuff );
    return ( 1 );
  }

  AACGetLastFrameInfo( AACDecoder, & aacFrameInfo );

  samplerate = aacFrameInfo.sampRateOut;
  bitrate = aacFrameInfo.bitRate;
  channels = aacFrameInfo.nChans;
  samplesize = aacFrameInfo.outputSamps;
  AACFileBuffPos = AACFileBuff;
  AACOpened = 1;
  bytesLeft = recbytesLeft;
  AacPlaySamples = 0;
  GetPlayingTime = GetAacPlayingTime;

  return ( 0 );
}
