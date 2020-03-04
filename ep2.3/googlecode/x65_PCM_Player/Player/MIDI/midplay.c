

#include "MIDITBL.H"

int err;
typedef struct
{
  unsigned long length;
  unsigned long loopstart;
  unsigned long looplength;
  unsigned char vol;
  signed char finetune;
  signed char relativenote;
  signed char * data;
}
MIDI_SAMPLE;

typedef struct
{
  unsigned volenvcnt;
  unsigned volsustpoint;
  unsigned char sampletable[96];
  unsigned char volenv[325];
  unsigned char smpcnt;
  MIDI_SAMPLE * samples[128];
}
MIDI_PROGRAM;

typedef struct
{
  signed char * pos;
  /* Position which is currently played */ //0
  signed char * repeat;
  /* Goes here after hitting end */ //4
  signed char * end;
  /* Position of last sample + 1 */ //8
  unsigned long freq;
  /* Playing frequency in hertz */ // 16
  unsigned long fractpos;
  /* 16bit fractional pos */ // 20
  unsigned long vol, vel;
  /* Note volume, ranges from 0 to 64 */ //24
  unsigned char panning;
  /* 0 left, 128 middle, 255 right */ //32
  unsigned long voicemode;
  /* See the bit definitions above */ //36
  signed int LastVal; //40
  unsigned volenvpos;
  unsigned channel;
  unsigned note;
  unsigned playtime;
}
M_CHANNEL;

typedef struct
{
  unsigned Volume;
  unsigned char panning;
  signed int Pitch;
  unsigned PitchSensitivity;
  unsigned RPN, NRPN;
  MIDI_PROGRAM * instrument;
  unsigned ProgramNum;
}
MIDI_M_CHANNEL;

typedef struct
{
  char Signature[4];
  long Lenght;
  unsigned short Format;
  unsigned short MTrkNum;
  unsigned short Tempo;
}
TMThd;

int NoteOnPos = 0;
unsigned char * MidiData, * track_pos;
unsigned track_len;
int midi_tempo, midi_tempocount, midi_tempo_base;
int midi_pause, midi_pausecount;
#define MaxPoly 16
int EnvCnt=0;
M_CHANNEL smp_channel[MaxPoly];
MIDI_M_CHANNEL midi_channel[16];
MIDI_PROGRAM * midi_prog[129];
void midiplayer();
long GetPause();
void NoteOff( unsigned char channel, unsigned char note );
unsigned char lastcmd;

extern void loadprogram( unsigned char Number  );
void freeprogram(unsigned char Number);

unsigned short swap_word( unsigned short data )
{
  return ( ( ( data & 0x00FF ) << 8 ) + ( ( data & 0xFF00 ) >> 8 ) );
}

unsigned swap_dword( unsigned data )
{
  return ( ( ( data & 0xFF000000 ) >> 24 ) + ( ( data & 0x00FF0000 ) >> 8 ) + ( ( data & 0x0000FF00 ) << 8 )
       + ( ( data & 0x000000FF ) << 24 ) );
}

void MidiInit( char * mptr )
{
  int i;
  TMThd * mthd = ( TMThd * ) mptr;
  MidiData = mptr;
  //Init channels
  zeromem(&midi_channel[0],sizeof(midi_channel));
  zeromem(&midi_prog,sizeof(midi_prog));
  zeromem(&smp_channel,sizeof(smp_channel));
  
  loadprogram(0);
  loadprogram(128);
  
  i = 0;
  while ( i < 16 )
  {
    midi_channel[i].Volume = 127;
    midi_channel[i].Pitch = 0;
    midi_channel[i].panning=128;
    midi_channel[i].instrument = midi_prog[0];
    midi_channel[i].PitchSensitivity = 2;
    midi_channel[i].ProgramNum=0;
    i++;
  }
  //midi_prog[128]->volenvcnt=26;
  midi_channel[9].instrument = midi_prog[128];
  midi_tempo = 120;
  midi_tempo_base = swap_word( mthd->Tempo );
  memcpy( & track_len, MidiData + 14 + 4, 4 );
  track_len = swap_dword( track_len );
  track_pos = MidiData + 14 + 8;
  midi_pause = 0;
  midi_tempocount = 1;
  midi_pausecount = GetPause();
  NoteOnPos = 0;
  if ( midi_pausecount == 0 )
  {
    midi_pausecount = 1;
    midiplayer();
  }

}


long GetPause()
{
  long rv;
  unsigned char b;
  if ( ( rv = * track_pos++ ) & 0x80 )
  {
    rv &= 0x7F;
    do
    {
      rv = ( rv << 7 ) + ( ( b = * track_pos++ ) & 0x7F );
    }
    while ( b & 0x80 );
  }
  return rv;
}

/*
static int getamigaperiod( int note, int finetune )
{
  int noteoffset = ( note % 12 ) << 3;
  if ( finetune >= 0 )
  {
    int roughfinetune = finetune / 16;
    int finefinetune = finetune & 15;
    return ( ( amigafreqtable[8 + noteoffset + roughfinetune] * ( 16 - finefinetune )
         + amigafreqtable[9 + noteoffset + roughfinetune] * finefinetune ) * 2 ) >> ( note / 12 );
  }
  else
  {
    int roughfinetune = ( finetune - 15 ) / 16;
    int finefinetune = finetune & 15;
    return ( ( amigafreqtable[8 + noteoffset + roughfinetune] * ( 16 - finefinetune )
         + amigafreqtable[9 + noteoffset + roughfinetune] * finefinetune ) * 2 ) >> ( note / 12 );
  }
}

*/

void NoteOn( unsigned char channel, unsigned char note, unsigned char vel )
{
  int MaxTime = 0;
  MIDI_SAMPLE * smp;
  unsigned finalperiod = 0;
  
  
  if ( vel == 0 )
  {
    NoteOff( channel, note );
    return;
  }
  
    if(midi_channel[channel].ProgramNum==124)
  {
    SetVibration(16);
    return;
  };
  
  while ( finalperiod < MaxPoly )
  {
    if ( smp_channel[finalperiod].voicemode == 0 ) break;
    finalperiod++;
  }
  NoteOnPos = finalperiod;
  if ( finalperiod == MaxPoly )
  {
    finalperiod = 0;
    NoteOnPos = 0;

    while ( finalperiod < MaxPoly )
    {
      if ( smp_channel[finalperiod].playtime > MaxTime )
      {
        MaxTime = smp_channel[finalperiod].playtime;
        NoteOnPos = finalperiod;
      }
      finalperiod++;
    }

  }


  // if(channel==9)return;
  if ( ( ( note - 12 ) > 96 ) | ( ( note - 12 ) < 0 ) ) return;

  smp = midi_channel[channel].instrument->samples[midi_channel[channel].instrument->sampletable[note - 12]];
  if ( smp == 0 ) return;
  if ( channel == 9 ) note += 12;
  //finalperiod = ( 7680 - ( note + smp->relativenote - 12 ) * 64 -  smp->finetune / 2 );
  finalperiod = getamigaperiod( note + smp->relativenote - 12, smp->finetune );
  //err = track_pos - MidiData;
  //
  //  sprintf( & temp, "%d NoteOn Ch%d N%d V%d - J%d\n", err, channel, note, vel, NoteOnPos );
  // fputs( temp, stdout );
  //
  /* if ( vel == 0 ) { NoteOff( channel, note ); return; } */
  smp_channel[NoteOnPos].panning = midi_channel[channel].panning;
  smp_channel[NoteOnPos].playtime = 0;
  smp_channel[NoteOnPos].volenvpos = 0;
  smp_channel[NoteOnPos].pos = smp->data;
  smp_channel[NoteOnPos].end = smp->data + smp->length - 1;
  if ( smp->loopstart ) smp_channel[NoteOnPos].repeat = smp->data + smp->loopstart;
  else  smp_channel[NoteOnPos].repeat = 0;
  smp_channel[NoteOnPos].freq = 14317456 / finalperiod; //;linearfreqtable[finalperiod % 768] >> ( finalperiod / 768 );
  smp_channel[NoteOnPos].fractpos = 0;
  smp_channel[NoteOnPos].voicemode = 1;
  smp_channel[NoteOnPos].LastVal = 0;
  smp_channel[NoteOnPos].vel = vel;
  smp_channel[NoteOnPos].vol = ( vel * midi_channel[channel].Volume * midi_channel[channel].instrument->volenv[0] ) / 16384;

  smp_channel[NoteOnPos].channel = channel;
  smp_channel[NoteOnPos].note = note;
  //NoteOnPos++;
  //if ( NoteOnPos >= MaxPoly ) NoteOnPos = 0;
}

void ControlChange( unsigned char channel, unsigned char cc, unsigned char vv )
{
  //  sprintf( & temp, "Control Change %d\n", cc );

  //fputs( temp, stdout );
  switch ( cc )
  {
    case 7:
      midi_channel[channel].Volume = vv;

    break;
    case 10:
      midi_channel[channel].panning = vv*2;

    break;
    case 100:
      midi_channel[channel].RPN = ( midi_channel[channel].RPN & 0xFF00 ) | vv;
    break;
    case 101:
      midi_channel[channel].RPN = ( midi_channel[channel].RPN & 0x00FF ) | ( vv < 8 );
    break;
    case 6:
      switch ( midi_channel[channel].RPN )
      {
        case 0: //Pitchsens
          midi_channel[channel].PitchSensitivity = vv;
        break;
      }
    break;

  }

}

void NoteOff( unsigned char channel, unsigned char note )
{
  int i = 0;

    if(midi_channel[channel].ProgramNum==124)
  {
    SetVibration(0);
    return;
  };
  
  while ( i < MaxPoly )
  {
    if ( ( smp_channel[i].channel == channel ) & ( smp_channel[i].note == note ) )
    {
      smp_channel[i].voicemode = 2;
      if(i!=9) smp_channel[i].volenvpos = midi_channel[smp_channel[i].channel].instrument->volsustpoint;
     // err = track_pos - MidiData;
      // sprintf( & temp, "%d NoteOff Ch%d N%d - J%d\n\n", err, channel, note, i );
      // fputs( temp, stdout );
    }
    i++;
  }
}

void SysEx()
{
  unsigned char msg;
  msg = * track_pos;
  track_pos++;
  switch ( msg )
  {
    case 0xFF:
      if ( * ( track_pos ) == 0x51 )
      {
        midi_tempo = ( 60000000 / ( * ( track_pos + 2 ) * 65536 + * ( track_pos + 3 ) * 256 ) );
      };
      if ( * ( track_pos ) == 0x2F )
      {
        AudioActive = 0;
          playlist[plcurrent].songtime=GetPlayingTime()*50;    
        SavePlayList( PlayListName );
        return;
      };

      track_pos += * ( track_pos + 1 ) + 2;
    break;
    case 0xF0:
      while ( * track_pos != 0xF7 ) track_pos++;
      track_pos++;
    break;
    default:
      AudioActive = 0;
  }
}

void ExecEnvelopes()
{
  //Execution

  int i = 0;
  while ( i < MaxPoly )
  {
    if ( smp_channel[i].voicemode != 0 )
    {
      if ( smp_channel[i].voicemode == 1 )
      {
        if ( smp_channel[i].volenvpos < midi_channel[smp_channel[i].channel].instrument->volsustpoint )
        {
          smp_channel[i].volenvpos++;
          smp_channel[i].playtime++;
          smp_channel[i].vol = ( smp_channel[i].vel * midi_channel[smp_channel[i].channel].Volume
               * midi_channel[smp_channel[i].channel].instrument->volenv[smp_channel[i].volenvpos] ) / 16384;
        }
      }
      else
      {
        if ( smp_channel[i].volenvpos < midi_channel[smp_channel[i].channel].instrument->volenvcnt )
        {
          smp_channel[i].volenvpos++;
          smp_channel[i].playtime++;
          smp_channel[i].vol = ( smp_channel[i].vel * midi_channel[smp_channel[i].channel].Volume
               * midi_channel[smp_channel[i].channel].instrument->volenv[smp_channel[i].volenvpos] ) / 16384;
        }
        else
          smp_channel[i].voicemode = 0;
      }
    }
    i++;
  }

}

void ReCalcFreq( unsigned char channel )
{
  MIDI_SAMPLE * smp;
  unsigned finalperiod;

  int i = 0;
  while ( i < MaxPoly )
  {
    if ( ( smp_channel[i].channel == channel ) & ( smp_channel[i].voicemode ) )
    {
      smp = midi_channel[channel].instrument->samples[midi_channel[channel].instrument->sampletable[smp_channel[i].note
           - 12]];
      finalperiod = ( 7680 - ( smp_channel[i].note + smp->relativenote - 12 ) * 64
           - ( smp->finetune + midi_channel[channel].Pitch ) / 2 );
      smp_channel[i].freq = linearfreqtable[finalperiod % 768] >> ( finalperiod / 768 );
    }
    i++;
  }
}

void ProgramChandge(unsigned char Channel,unsigned char Program){
  unsigned char LastInstr;
  
  if(Channel==9)return;

LastInstr=midi_channel[Channel].ProgramNum;
if(!midi_prog[Program]){
loadprogram(Program);
}
midi_channel[Channel].instrument = midi_prog[Program];
midi_channel[Channel].ProgramNum=Program;


int i=0;

while(i<MaxPoly){
if(smp_channel[i].channel==Channel) smp_channel[i].voicemode=0; 
i++;  
}

i=0;
while(i<16){
if(midi_channel[i].ProgramNum==LastInstr)return;
i++;  
}

freeprogram(LastInstr);
}

void midiplayer()
{
  int i;
  midi_pausecount--;
  if ( !midi_pausecount )
  {
  Loop1:
   // err = track_pos - MidiData;
    switch ( 0xF0 & * track_pos )
    {
      case 0x80: //NoteOff
        lastcmd = * track_pos;
        NoteOff( ( 0x0F & * track_pos ), * ( track_pos + 1 ) );

        track_pos += 3;
      break;
      case 0x90: //NoteOn
        lastcmd = * track_pos;
        NoteOn( ( 0x0F & * track_pos ), * ( track_pos + 1 ), * ( track_pos + 2 ) );
        track_pos += 3;

      break;
      case 0xA0: //KeyPressure
        lastcmd = * track_pos;
        track_pos += 3;
        // sprintf( & temp, "Key Pressure\n" );
        // fputs( temp, stdout );
      break;
      case 0xB0: //ControlChange
        lastcmd = * track_pos;
        ControlChange( ( 0x0F & * track_pos ), * ( track_pos + 1 ), * ( track_pos + 2 ) );
        track_pos += 3;

      break;
      case 0xC0: //ProgramChange  2
        lastcmd = * track_pos;
        ProgramChandge(( * track_pos & 0x0F ),(* ( track_pos + 1 ))&0x7F);        
        track_pos += 2;
        // sprintf( & temp, "%d Program Change\n", err );
        // fputs( temp, stdout );
      break;
      case 0xD0: //ChannelPressure 2
        lastcmd = * track_pos;
        track_pos += 2;
        // sprintf( & temp, "%d Channel Pressure\n", err );
        // fputs( temp, stdout );
      break;
      case 0xE0: //PitchBend
        lastcmd = * ( track_pos );
        midi_channel[0x0F & * track_pos].Pitch =
             ( ( * ( track_pos + 1 ) + * ( track_pos + 2 ) * 256 ) - 16384 )
             * midi_channel[0x0F & * track_pos].PitchSensitivity / 128;
        //* midi_channel[0x0F & * track_pos].PitchSensitivity / 64;
        //sprintf( & temp, "%d Pitch\n", * ( track_pos + 1 ) + 256 * * ( track_pos + 2 ) );
        //fputs( temp, stdout );
        ReCalcFreq( 0x0F & * track_pos );
        track_pos += 3;
      break;
      case 0xF0:
        lastcmd = * track_pos;
        SysEx();
        if(!AudioActive)return;
      break;
      default:
        if ( ( * track_pos < 128 ) )
        {
          track_pos--;
          * track_pos = lastcmd;
        }
        goto Loop1;
      break;
    }
   // err = track_pos - MidiData;
    midi_pause = GetPause();
    if ( !midi_pause ) goto Loop1;
    midi_pausecount = midi_pause;
    //sprintf( & temp, "%d Pause %d\n", err, midi_pause );
    //fputs( temp, stdout );
  }

}


