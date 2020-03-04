
#include "mp3Dec\mp3dec.h"
#include "mp3Dec\mp3common.h"

#define MP3_BUFFER_SIZE 16384

extern int samplerate;
int Mp3PlaySamples;

signed short * framewavebuff = 0;
unsigned char * MP3FileBuff = 0, * MP3FileBuffPos;
int mp3filehandle = 0;
signed int err;
char MP3eof, MP3EndSong, MP3DisableSound;
unsigned short  samplesize;
unsigned long FramePos;
HMP3Decoder MP3Decoder;
int bytesLeft, bytes;
MP3FrameInfo mp3FrameInfo;
unsigned long SongLen = 18;
char MP3Opened = 0;

void MP3Close()
{
  if ( !MP3Opened ) return;
  if ( mp3filehandle != 0 )
  {
    fclose( mp3filehandle, & err );
    mp3filehandle = 0;
  }
  if ( MP3FileBuff )
  {
    mfree( MP3FileBuff );
    MP3FileBuff = 0;
  }
  if ( framewavebuff )
  {
    mfree( framewavebuff );
    framewavebuff = 0;
  }
  //MP3FreeDecoder(MP3DEC); //Вырубает телефон
  MP3Opened = 0;
}


inline void ReadMp3_io()
{
  bytes = fread( mp3filehandle, ( MP3FileBuff + bytesLeft ), MP3_BUFFER_SIZE - bytesLeft, & err );
  bytesLeft = bytesLeft + bytes;  
  if ( bytes == 0 )
  {
    MP3eof = 1;
    fclose( mp3filehandle, & err );
    mp3filehandle = 0;
  }  
}

void ReadMp3File()
{
  if ( MP3eof != 0 )
  {
    return;
  }
  memcpy( MP3FileBuff, MP3FileBuffPos, bytesLeft );
  MP3FileBuffPos = MP3FileBuff;
  ReadMp3_io();
  //SUBPROC((void*)ReadMp3_io,255);
  
}

int GetMp3PlayingTime()
{
  return ( Mp3PlaySamples / samplerate);
}



void GetMp3Sound( unsigned short * ptr, int nsamples )
{
  int offset;
  if ( !AudioActive )
  {
    zeromem( ptr, nsamples * 2 );
    return;
  }
  if ( MP3EndSong )
  {
    AudioActive = 0;
    playlist[plcurrent].songtime = GetPlayingTime() * 50;
    SavePlayList( PlayListName );
    zeromem( ptr, nsamples * 2 );
    return;
  }

  Mp3PlaySamples += nsamples / 2;

  if ( channels == 2 )
  {

    while ( nsamples )
    {
      if ( FramePos >= samplesize )
  {
    offset = MP3FindSyncWord( MP3FileBuffPos, bytesLeft );
    MP3FileBuffPos = MP3FileBuffPos + offset;
    bytesLeft = bytesLeft - offset;
    err = MP3Decode( MP3Decoder, & MP3FileBuffPos, & bytesLeft, framewavebuff, 0 );
    bitrate = ( bitrate + ( ( MP3DecInfo * ) MP3Decoder )->bitrate ) / 2;
    if ( err == -6 )
    {
      MP3EndSong = 1;
      zeromem( ptr, nsamples * 2 );
      return;
    }
    if ( bytesLeft < ( MAINBUF_SIZE * 2 ) )
    {
      ReadMp3File();
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
    offset = MP3FindSyncWord( MP3FileBuffPos, bytesLeft );
    MP3FileBuffPos = MP3FileBuffPos + offset;
    bytesLeft = bytesLeft - offset;
    err = MP3Decode( MP3Decoder, & MP3FileBuffPos, & bytesLeft, framewavebuff, 0 );
    bitrate = ( bitrate + ( ( MP3DecInfo * ) MP3Decoder )->bitrate ) / 2;
    if ( err == -6 )
    {
      MP3EndSong = 1;
      zeromem( ptr, nsamples * 2 );
      return;
    }
    if ( bytesLeft < ( MAINBUF_SIZE * 2 ) )
    {
      ReadMp3File();
    }
    FramePos = 0;
  }
      * ptr++ = framewavebuff[FramePos];
      * ptr++ = framewavebuff[FramePos++];
      nsamples -= 2;
    }
  }
}

int MP3Open( char * mp3filename )
{
  int offset;
  int recbytesLeft;
  GetSound = GetMp3Sound;

  //mp3filehandle = fopen( mp3filename, A_ReadOnly + A_BIN, P_READ, & err );
  mp3filehandle = fopen( mp3filename, A_BIN, P_READ, & err );
  if ( mp3filehandle == -1 ) return ( 1 );
  MP3Decoder = MP3InitDecoder();
  if ( !( MP3FileBuff = malloc( MP3_BUFFER_SIZE ) ) ) return ( 1 );
  if ( !( framewavebuff = malloc( 2304 * 2 ) ) )
  {
    mfree( MP3FileBuff );
    return ( 1 );
  }
  MP3eof = 0;
  MP3DisableSound = 0;
  MP3EndSong = 0;
  MP3FileBuffPos = MP3FileBuff;
  FramePos = 0;
  zeromem( MP3FileBuff, MP3_BUFFER_SIZE );
  zeromem( framewavebuff, 2304 * 2 );
  bytesLeft = 0;
  //
  ReadMp3_io();
  
  //
  recbytesLeft = bytesLeft;
  offset = MP3FindSyncWord( MP3FileBuffPos, bytesLeft );
  MP3FileBuffPos = MP3FileBuffPos + offset;
  bytesLeft = bytesLeft - offset;
  err = MP3Decode( MP3Decoder, & MP3FileBuffPos, & bytesLeft, framewavebuff, 0 );
  if ( err )
  {
    mfree( MP3FileBuff );
    return ( 1 );
  }
  MP3GetLastFrameInfo( MP3Decoder, & mp3FrameInfo );
  samplerate = mp3FrameInfo.samprate;
  bitrate = mp3FrameInfo.bitrate;
  channels = mp3FrameInfo.nChans;
  samplesize = mp3FrameInfo.outputSamps;
  MP3FileBuffPos = MP3FileBuff;
  MP3Opened = 1;
  bytesLeft = recbytesLeft;
  Mp3PlaySamples = 0;
  GetPlayingTime = GetMp3PlayingTime;
  return ( 0 );
}
