
short SmpCnt, SmpSwit;

int ExecuteMidiChannel( register M_CHANNEL * chptr )
{

  if ( ( !chptr->pos ) | ( !chptr->vol ) | ( !chptr->voicemode ) ) return ( 0 );

  chptr->fractpos += chptr->freq;
  register int chfractpos = chptr->fractpos;
  register signed char * chpos = chptr->pos;


Loop1:
  if ( chfractpos >= 32000 )
  {
    chptr->fractpos -= 32000;
    chfractpos = chptr->fractpos;
    if ( chpos < chptr->end ) chptr->LastVal = * chpos;
    chptr->pos++;
    chpos++;

    if ( chpos >= chptr->end )
    {
      if ( chptr->channel == 9 )
      {
        chptr->voicemode = 0;
        return ( 0 );
      }
      chptr->pos = chptr->repeat;
      chpos = chptr->pos;
    }

    goto Loop1;
  }

  if ( chpos >= chptr->end )
  {
    if ( chptr->repeat == 0 )
    {
      chptr->voicemode = 0;
      return ( 0 );
    }

    chptr->pos = chptr->repeat;
    chpos = chptr->pos;
  }

  if ( !chpos ) return ( 0 );

  return ( ( ( * chpos * chfractpos ) + ( chptr->LastVal * ( 32000 - chfractpos ) ) ) / 32768 * chptr->vol );
}


void GetMidiSound( unsigned short * ptr, int nsamples )
{
  if ( !AudioActive )
  {
    zeromem( ptr, nsamples * 2 );
    return;
  }

  while ( nsamples )
  {

    MidiSmpCnt++;
    register int i = 0;
    register int ChL, ChR;

    EnvCnt--;
    if ( EnvCnt <= 0 )
    {
      EnvCnt = samplerate / 50;
      ExecEnvelopes();
    }
    midi_tempocount--;
    if ( !midi_tempocount )
    {
      midiplayer();
      // midi_tempocount = samplerate*64/(midi_tempo*midi_tempo_base);
      midi_tempocount = samplerate * 60 / ( midi_tempo_base * midi_tempo );
    }

    register int temp;
    ChL = 0;
    ChR = 0;
    while ( i < MaxPoly )
    {
      temp = ExecuteMidiChannel( & smp_channel[i] );
      ChL += ( ( 256 - smp_channel[i].panning ) * temp ) / 256;
      ChR += ( smp_channel[i].panning * temp ) / 256;
      i++;
    }

    if ( ChR > 32767 ) ChR = 32767;
    if ( ChR < -32767 ) ChR = -32767;

    if ( ChL > 32767 ) ChL = 32767;
    if ( ChL < -32767 ) ChL = -32767;

    * ptr++ = ChL;
    * ptr++ = ChR;

    nsamples -= 2;
  }
}
