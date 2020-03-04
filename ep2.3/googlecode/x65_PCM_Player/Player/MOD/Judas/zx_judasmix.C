

/* typedef struct { volatile char *pos; char *repeat; char *end; SAMPLE *smp; unsigned freq; volatile unsigned long fractpos;
signed char vol; unsigned char mastervol; unsigned char panning; volatile unsigned char voicemode; } CHANNEL; */
//signed short LastVals[256];
short SmpCnt, SmpSwit;
int XMPlaySamples;

extern int asm_ExecuteChannel( CHANNEL * chptr );

inline int ExecuteChannel( register CHANNEL * chptr )
{
  if ( ( !chptr->pos ) | ( !chptr->vol ) ) return ( 0 );
  register int sr = samplerate;
  chptr->fractpos += chptr->freq;
  register int chfractpos = chptr->fractpos;
  register signed char * chpos = chptr->pos;

Loop1:
  if ( chfractpos >= sr )
  {
    chptr->fractpos -= sr;
    chfractpos = chptr->fractpos;
    if ( chpos < chptr->end ) chptr->LastVal = * chpos;
    chptr->pos++;
    chpos++;

    if ( chpos >= chptr->end )
    {
      if ( chptr->voicemode == 1 ) return ( 0 );
      //chptr->LastVal = * chpos;
      chptr->pos = chptr->repeat;
      chpos = chptr->pos;
    }

    goto Loop1;
  }

  if ( chpos >= chptr->end )
  {
    if ( chptr->voicemode == 1 ) return ( 0 );
    //chptr->LastVal = * chpos;
    chptr->pos = chptr->repeat;
    chpos = chptr->pos;
  }

  if ( !chpos ) return ( 0 );

  return ( ( ( * chpos * chfractpos ) + ( chptr->LastVal * ( sr - chfractpos ) ) ) / sr * chptr->vol );
}

inline int ExecuteChannel32( register CHANNEL * chptr )
{
  if ( ( !chptr->pos ) | ( !chptr->vol ) ) return ( 0 );
  register int sr = samplerate;
  chptr->fractpos += chptr->freq;
  register int chfractpos = chptr->fractpos;
  register signed char * chpos = chptr->pos;

Loop1:
  if ( chfractpos >= sr )
  {
    chptr->fractpos -= sr;
    chfractpos = chptr->fractpos;
    if ( chpos < chptr->end ) chptr->LastVal = * chpos;
    chptr->pos++;
    chpos++;

    if ( chpos >= chptr->end )
    {
      if ( chptr->voicemode == 1 ) return ( 0 );
      //chptr->LastVal = * chpos;
      chptr->pos = chptr->repeat;
      chpos = chptr->pos;
    }

    goto Loop1;
  }

  if ( chpos >= chptr->end )
  {
    if ( chptr->voicemode == 1 ) return ( 0 );
    //chptr->LastVal = * chpos;
    chptr->pos = chptr->repeat;
    chpos = chptr->pos;
  }

  if ( !chpos ) return ( 0 );

  return ( ( ( * chpos * chfractpos ) + ( chptr->LastVal * ( sr - chfractpos ) ) ) / 32768 * chptr->vol );
}

void GetXmSound32( unsigned short * ptr, int nsamples )
{

  if ( !AudioActive )
  {
    zeromem( ptr, nsamples * 2 );
    return;
  }
  
  while(nsamples)
  {
  XMPlaySamples++;
  SmpCnt--;
  if ( !SmpCnt )
  {
    xmplayer();
    SmpCnt = ( samplerate * 5 ) / 2 / judas_bpmtempo;
  }
  register int i = 0;
  register int ChL = 0;
  register int ChR = 0;
  register int temp;

  while ( i < xm_header.channels )
  {
    temp = ExecuteChannel32( & judas_channel[i] );
    ChL += ( ( 256 - judas_channel[i].panning ) * temp ) / 256;
    ChR += ( judas_channel[i].panning * temp ) / 256;

    i++;
  }

  if ( ChL > 32767 ) ChL = 32767;
  if ( ChL < -32767 ) ChL = -32767;

  if ( ChR > 32767 ) ChR = 32767;
  if ( ChR < -32767 ) ChR = -32767;

  *ptr++=ChL;
  *ptr++=ChR;
  nsamples-=2;
  }
}

void GetXmSound32Surr( unsigned short * ptr, int nsamples )
{

  if ( !AudioActive )
  {
    zeromem( ptr, nsamples * 2 );
    return;
  }
  
  while(nsamples)
  {
  XMPlaySamples++;
  SmpCnt--;
  if ( !SmpCnt )
  {
    xmplayer();
    SmpCnt = ( samplerate * 5 ) / 2 / judas_bpmtempo;
  }
  register int i = 0;
  register int ChL = 0;
  register int ChR = 0;
  register int temp;

  while ( i < xm_header.channels )
  {
    temp = ExecuteChannel32( & judas_channel[i] );
    ChL += ( ( 256 - judas_channel[i].panning ) * temp ) / 256;
    ChR += ( judas_channel[i].panning * temp ) / 256;

    i++;
  }

  if ( ChL > 32767 ) ChL = 32767;
  if ( ChL < -32767 ) ChL = -32767;

  if ( ChR > 32767 ) ChR = 32767;
  if ( ChR < -32767 ) ChR = -32767;
  
   //ChL=ChL-XmSBuff[XmSBuffPos];
   //ChR=ChR-XmSBuff[XmSBuffPos+1];
  
  *ptr++=ChL;
  *ptr++=ChR;
  
  //XmSBuff[XmSBuffPos++]=ChR/8;
  //XmSBuff[XmSBuffPos++]=ChL/8;
  
  //XmSBuffPos=XmSBuffPos+2;
  //if(XmSBuffPos>=16000)XmSBuffPos=0;  
  nsamples-=2;
  }
}

void GetXmSound( unsigned short * ptr, int nsamples )
{

  if ( !AudioActive )
  {
    zeromem( ptr, nsamples * 2 );
    return;
  }
  
  while(nsamples)
  {
  XMPlaySamples++;
  SmpCnt--;
  if ( !SmpCnt )
  {
    xmplayer();
    SmpCnt = ( samplerate * 5 ) / 2 / judas_bpmtempo;
  }
  register int i = 0;
  register int ChL = 0;
  register int ChR = 0;
  register int temp;

  while ( i < xm_header.channels )
  {
    temp = ExecuteChannel( & judas_channel[i] );

    ChL += ( ( 256 - judas_channel[i].panning ) * temp ) / 256;
    ChR += ( judas_channel[i].panning * temp ) / 256;

    i++;
  }

  if ( ChL > 32767 ) ChL = 32767;
  if ( ChL < -32767 ) ChL = -32767;

  if ( ChR > 32767 ) ChR = 32767;
  if ( ChR < -32767 ) ChR = -32767;

  *ptr++=ChL;
  *ptr++=ChR;
  nsamples-=2;
  }
}
