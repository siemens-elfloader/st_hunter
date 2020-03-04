//Исходник MOD декодера
//Информация о формате взята из нескольких источников
//Часть найдена методом научного тыка)
//ILYA_ZX

int ModPlaySamples;
typedef struct
{
  signed char Name[22];
  unsigned short Len;
  unsigned char FineTune, DefVol;
  unsigned short LoopStart, LoopLen;
  //Added
  signed char IsLoop;
}
TMODInstr;

typedef struct
{
  signed char Name[20];
  //Samples
  unsigned char MelLen, Reserved;
  unsigned char PattOrder[128];
  unsigned char NumOfChannels[4];
}
TMODHDR;

typedef struct
{
  signed char Note, Vol, Instr;
  unsigned long Period, SmpPos;
  unsigned long Freq, TPP;
  unsigned char Arp, ArpC, PU, PD, TPS;
  unsigned char VibSpeed, VibDepth, VibIndex;
  signed char VibNegative;
  signed char VolSlide;

}
MODChannel;

//Для арпеджио
unsigned long NMul[16] =
{
  65536, 69429, 73554, 77923, 82552, 87456, 92652, 98156, 103987, 110164, 116709, 123642, 130987, 138769, 147012, 155746
};

//Для вибрато
int sineTable[32] =
{
  0, 24, 49, 74, 97, 120, 141, 161, 180, 197, 212, 224, 235, 244, 250, 253, 255, 253, 250, 244, 235, 224, 212, 197, 180, 161,
       141, 120, 97, 74, 49, 24
};


TMODHDR * MODHeader;
MODChannel * Channels[32];
unsigned char NumOfChann;
unsigned char * Patterns;
signed char * Instruments[31];
TMODInstr * Instrs[31];
short Pattern,  Speed, Tempo, SpeedC, TempoC, TempoM;
signed short Row;
int handle, eom;
short *mem;
char *memstream;
int streampos=0;
int FromFile=1;

signed short IntPLast;
GBSTMR timer;
void PB1();

void Swap( signed char * data )
{
  signed char c;
  c = data[1];
  data[1] = data[0];
  data[0] = c;
}

void ExecPatterns()
{
  short Ch, Period, Instr, Eff, EffParam;
  unsigned short Pos;
  signed char PBreak;
  PBreak = 0;
  if ( Pattern >= MODHeader->MelLen )
  {
    return;
  }
  Ch = 0;
  while ( Ch < NumOfChann )
  {
    Pos = MODHeader->PattOrder[Pattern] * 4 * NumOfChann * 64 + Row * 4 * NumOfChann + Ch * 4;
    Instr = ( Patterns[Pos] & 0xF0 ) + ( ( Patterns[Pos + 2] & 0xF0 ) >> 4 ) - 1;
    Period = ( ( Patterns[Pos] & 0x0F ) << 8 ) + Patterns[Pos + 1];
    Eff = ( Patterns[Pos + 2] & 0x0F );
    EffParam = Patterns[Pos + 3];
    //
    if ( Instr >= 0 )
    {
      Channels[Ch]->Instr = Instr;
      if ( Channels[Ch]->Note == -1 )
      {
        Channels[Ch]->Vol = Instrs[Instr]->DefVol;
      }
      Channels[Ch]->SmpPos = 0;
    }
    if ( Period > 0 )
    {
      if ( Eff == 0x03 )
      {
        Channels[Ch]->TPP = Period;
      }
      else
      {
        Channels[Ch]->TPP = 0;
        Channels[Ch]->Period = Period;
        Channels[Ch]->Freq = 113000 / Channels[Ch]->Period * 8000 / samplerate;
        Channels[Ch]->SmpPos = 0;
      }
    }
    //Effects
    Channels[Ch]->Arp = 0;
    Channels[Ch]->ArpC = 0;
    Channels[Ch]->PU = 0;
    Channels[Ch]->PD = 0;
    Channels[Ch]->VolSlide = 0;

    if ( ( Eff != 0x04 ) && ( Eff != 0x06 ) )
    {
      Channels[Ch]->VibDepth = 0;
      Channels[Ch]->Freq = 113000 / Channels[Ch]->Period * 8000 / samplerate;
    }
    switch ( Eff )
    {
      case 0x0D: //PatBreak
        PBreak = 1;
      break;
      
      //PatternJump
      case 0x0B:
        //Pattern=EffParam;
        //Row=-1;
       break;  
      case 0x03: //TonePort
        if ( EffParam != 0 )
        {
          Channels[Ch]->TPS = EffParam;
        }
      break;

      case 0x04: //Vibrato
        if ( EffParam != 0 )
        {
          Channels[Ch]->VibSpeed = ( EffParam >> 4 );
          Channels[Ch]->VibDepth = ( EffParam & 0x0F );
          Channels[Ch]->VibIndex = 0;
        }
      break;

      case 0x06: //VolSlide+Vibrato
        Channels[Ch]->VolSlide = ( ( EffParam & 0xF0 ) >> 4 ) - ( EffParam & 0x0F );
      break;
      case 0x0A: //VolSlide
        Channels[Ch]->VolSlide = ( ( EffParam & 0xF0 ) >> 4 ) - ( EffParam & 0x0F );
      break;
      case 0x01: //Pup
        Channels[Ch]->PU = EffParam;
      break;
      case 0x02: //PD
        Channels[Ch]->PD = EffParam;
      break;
      case 0x0F: //SetTempo
        if ( EffParam < 32 )
        {
          Speed = EffParam;
        }
        else
        {
          Tempo = EffParam; TempoM = samplerate / ( 24 * Tempo / 60 );
        }
      break;
      case 0x0C: //SetTempo
        Channels[Ch]->Vol = EffParam;
      break;
      case 0x00: //Arpeggo
        Channels[Ch]->Arp = EffParam;
        Channels[Ch]->ArpC = 0;
      break;
    }
    Ch++;
  }

  Row++;
  if ( ( Row > 63 ) | ( PBreak > 0 ) )
  {
    Row = 0;
    Pattern++;
  }
}

signed short ExecChannel( MODChannel * Ch )
{
  //if ( Instrs[Ch->Instr]->Len <Instrs[Ch->Instr]->LoopLen )return 0;
  if ( Instrs[Ch->Instr]->Len==0)return(0);
  if ( Ch->Vol <= 0 ) return 0;
  
  Ch->SmpPos = Ch->SmpPos + Ch->Freq;
  if ( ( Ch->SmpPos >> 8 ) >= ( Instrs[Ch->Instr]->Len) )
  {
    if ( Instrs[Ch->Instr]->IsLoop > 0 )
    {
      Ch->SmpPos = Ch->SmpPos - ( Instrs[Ch->Instr]->LoopLen << 8 );
    }
    else
    {
      return 0;
    }
  }
  int temp,smppos=(Ch->SmpPos >> 8),smpnext=((Ch->SmpPos >> 8)+1);
  
  
  if ( smpnext >= ( Instrs[Ch->Instr]->Len ) )smpnext=smppos;
    /*
  {
    if ( Instrs[Ch->Instr]->IsLoop > 0 )
    {
      smpnext = (Ch->SmpPos - ( Instrs[Ch->Instr]->LoopLen << 8 ))>>8;
    }else smpnext--;
  }  */
  
  
  temp=Ch->SmpPos-(Ch->SmpPos&0xFFFFFF00);
  
  return (((Instruments[Ch->Instr] [smppos]*(256-temp))+
         (Instruments[Ch->Instr] [smpnext]*temp))*Ch->Vol/256);
}

signed short GetModSound()
{
  signed char c;

  signed short Rslt;
  if(!AudioActive)return(0);
  if ( Pattern >= ( MODHeader->MelLen ) )
  {
    AudioActive=0;
    playlist[plcurrent].songtime=GetPlayingTime()*50;    
    SavePlayList( PlayListName );
    return 0;
  }
  //
  TempoC++;
  if ( TempoC >= TempoM )
  {
    TempoC = 0;

    c = 0;
    while ( c < NumOfChann )
    {
      //Tone Portamento

      if ( Channels[c]->TPP > 0 )
      {

        if ( Channels[c]->Period > Channels[c]->TPP )
        {
          Channels[c]->Period = Channels[c]->Period - Channels[c]->TPS;
          if ( Channels[c]->Period < Channels[c]->TPP )
          {
            Channels[c]->Period = Channels[c]->TPP;
          }
        }
        else
        {
          Channels[c]->Period = Channels[c]->Period + Channels[c]->TPS;
          if ( Channels[c]->Period > Channels[c]->TPP )
          {
            Channels[c]->Period = Channels[c]->TPP;
          }
        }
        if ( Channels[c]->Period > 0 )
        {
          Channels[c]->Freq = 113000 / Channels[c]->Period * 8000 / samplerate;
        }
      }

      //Vibrato

      if ( Channels[c]->VibDepth > 0 )
      {

        Channels[c]->Freq = 113000 / Channels[c]->Period * 8000 / samplerate;

        if ( Channels[c]->VibNegative > 0 )
        {
          Channels[c]->Freq = Channels[c]->Freq + ( ( Channels[c]->VibDepth * sineTable[Channels[c]->VibIndex] ) >> 8 );
        }
        else
        {
          Channels[c]->Freq = Channels[c]->Freq - ( ( Channels[c]->VibDepth * sineTable[Channels[c]->VibIndex] ) >> 8 );
        }

        Channels[c]->VibIndex = Channels[c]->VibIndex + Channels[c]->VibSpeed;
        if ( Channels[c]->VibIndex > 31 )
        {
          Channels[c]->VibIndex = Channels[c]->VibIndex - 32;
          Channels[c]->VibNegative = ( -Channels[c]->VibNegative );
        }
      }

      //VolSlide
      Channels[c]->Vol = Channels[c]->Vol + Channels[c]->VolSlide;
      if ( Channels[c]->Vol > 63 )
      {
        Channels[c]->Vol = 63;
      }
      if ( Channels[c]->Vol < 0 )
      {
        Channels[c]->Vol = 0;
      }
      //PortUp
      if ( Channels[c]->PU > 0 )
      {
        Channels[c]->Period = Channels[c]->Period - Channels[c]->PU;
        if ( Channels[c]->Period > 113 )
        {
          Channels[c]->Freq = 113000 / Channels[c]->Period * 8000 / samplerate;
        }
      }
      //PortDown
      if ( Channels[c]->PD > 0 )
      {
        Channels[c]->Period = Channels[c]->Period + Channels[c]->PD;
        Channels[c]->Freq = 113000 / Channels[c]->Period * 8000 / samplerate;
      };

      //Arpeggo
      if ( Channels[c]->Arp > 0 )
      {
        //Arp
        Channels[c]->ArpC++;
        //if(Channels[c]->ArpC>=3)
        //{Channels[c]->ArpC=0;}

        switch ( Channels[c]->ArpC % 3 )
        {
          case 0:
            Channels[c]->Freq = 113000 / Channels[c]->Period * 8000 / samplerate;
          break;
          case 1:
            Channels[c]->Freq = ( ( 113000 / Channels[c]->Period ) * NMul[Channels[c]->Arp & 0x0F]
                 / 65536 ) * 8000 / samplerate;
          break;
          case 2:
            Channels[c]->Freq = ( ( 113000 / Channels[c]->Period ) * NMul[( Channels[c]->Arp ) >> 4]
                 / 65536 ) * 8000 / samplerate;
          break;
        }
      }
      c++;
    }
    SpeedC--;
    if ( SpeedC <= 0 )
    {
      SpeedC = Speed;
      ExecPatterns();
    }
  }
  c = 0;
  Rslt = 0;
  while ( c < NumOfChann )
  {
    Rslt = Rslt + ExecChannel( Channels[c] );
    c++;
  }
  //LF Filter
  //Rslt=IntPLast/2+Rslt/2;
  //IntPLast=Rslt;
  ModPlaySamples++;
  Rslt = ( Rslt * (65536-ModFilter) + ModFilter * lastvalue ) / 65536;
  lastvalue=Rslt;
  return Rslt;
}

int GetModPlayingTime(){
return(ModPlaySamples/samplerate);  
}

void MODRead( void * data, unsigned short size )
{
  unsigned int err;
  if(FromFile)fread32( handle, data, size, & err );
  else
  {
    memcpy(data,&memstream[streampos],size);
    streampos+=size;
  }
}

void MODClose( void )
{
  int i;
  if(!MODOpened)return;
  i = 0;
  while ( i < 31 )
  {
    mfree( ( void * ) Instrs[i] );
    mfree( ( void * ) Instruments[i] );
    i++;
  }
  mfree( ( void * ) Patterns );
  i = 0;
  while ( i < NumOfChann )
  {
    mfree( ( void * ) Channels[i] );
    i++;
  }
  mfree( ( void * ) MODHeader );
  MODOpened=0;
}


void OpenMOD(char *filename)
{
  unsigned char np;
  short i;
  unsigned int err;
  eom = 0;
  if(FromFile)handle = fopen( filename, A_ReadOnly + A_BIN, P_READ, & err );

  if ( handle == -1 )
  {

    return;
  }

  MODHeader = malloc( sizeof( TMODHDR ) );
  zeromem(MODHeader,sizeof(MODHeader));  
  MODRead( MODHeader, 20 );
  i = 0;
  while ( i < 31 )
  {
    Instrs[i] = malloc( sizeof( TMODInstr ) );
    zeromem(Instrs[i],sizeof( TMODInstr ) );
    MODRead( Instrs[i], sizeof( TMODInstr ) - 2 );
    if ( ( Instrs[i]->LoopStart + Instrs[i]->LoopLen ) > 0 )
    {
      Instrs[i]->IsLoop = 1;
    }
    else
    {
      Instrs[i]->IsLoop = 0;
    }
    i++;
  }
  MODRead( & MODHeader->MelLen, 134 );

  //FindNumOfPattern
  i = 0;
  np = 0;
  while ( i < 127 )
  {
    if ( MODHeader->PattOrder[i] > np )
    {
      np = MODHeader->PattOrder[i];
    }
    i++;
  }
  np++;
  if ( MODHeader->NumOfChannels[1] == 0x2E )
  {
    NumOfChann = 4;
  }
  if ( MODHeader->NumOfChannels[0] < 58 )
  {
    NumOfChann = ( MODHeader->NumOfChannels[0] - 48 );
    if ( MODHeader->NumOfChannels[1] < 58 )
    {
      NumOfChann = NumOfChann * 10 + ( MODHeader->NumOfChannels[1] - 48 );
    };
  }
 
  Patterns = malloc( 4 * 64 * NumOfChann * np );
  MODRead( Patterns, 4 * 64 * NumOfChann * np );
  //LoadInstr
  i = 0;
  while ( i < 31 )
  {
    Swap( ( void * ) & Instrs[i]->Len );
    Instrs[i]->Len = Instrs[i]->Len * 2;
    Swap( ( void * ) & Instrs[i]->LoopStart );
    Instrs[i]->LoopStart = Instrs[i]->LoopStart * 2;
    Swap( ( void * ) & Instrs[i]->LoopLen );
    Instrs[i]->LoopLen = Instrs[i]->LoopLen * 2;
    Instruments[i] = malloc( Instrs[i]->Len );
    zeromem(Instruments[i],Instrs[i]->Len);
    MODRead( Instruments[i], Instrs[i]->Len );
    i++;
  }

  i = 0;
  while ( i < NumOfChann )
  {
    Channels[i] = malloc( sizeof( MODChannel ) );
    zeromem(Channels[i],sizeof( MODChannel ));
    Channels[i]->Vol = 0;
    Channels[i]->SmpPos = 0;
    Channels[i]->Instr = 0;
    Channels[i]->Note = -1;
    i++;
  }
  if(FromFile)fclose( handle, & err );
  Speed = 6;
  Tempo = 125;
  Pattern = 0;
  Row = 0;
  TempoM = samplerate / ( 24 * Tempo / 60 );
}


