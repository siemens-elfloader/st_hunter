
typedef struct
{
  signed char NESM[6];
  unsigned char tsongs, csong;
  unsigned short load, init, play;
  signed char SongName[32];
  signed char Artist[32];
  signed char Copyright[32];
  unsigned short speednoisedevidersc;
  unsigned char BankSwitch[8];
  unsigned short speedpal;
  unsigned char PNBits, extra, al1, al2, al3, al4;
}
NSFHDR;

unsigned short speed;
unsigned char * gameImage;
WORD addrmask = 0xFFFF;
NSFHDR NSF_HDR;

#include "6502.h"
//
#define A a_reg
#define X x_reg
#define Y y_reg
#define P flag_reg
#define S s_reg
#define PC pc_reg
extern BYTE opcode;
extern BYTE a_reg, x_reg, y_reg, flag_reg, s_reg;
extern WORD pc_reg;
extern void( * instruction[256] ) ();
//
signed long square1devider, square2devider, triangledevider, noisedevider;
unsigned char square1seqpos, square2seqpos, triangleseqpos;
unsigned long square1volume, square2volume, trianglevolume, noisevolume;
signed long square1period, square2period, triangleperiod, square1lencounter, square2lencounter,
     trianglelencounter, noiselencounter;
unsigned char sw1d, sw2d;
unsigned long noiseoutput;
signed short dmcdevider;
unsigned char daccounter;
unsigned char dmcbuffer, dmcbuffercount, dmcsilence;
signed long dmcdmaadr, dmcdatalen;
signed char noiseenvelope, noiseenvdiv;
signed char square1envelope, square1envdiv;
signed char square2envelope, square2envdiv;
unsigned char triabglelinearcounter, twhaltflag;
unsigned short MainClock;
unsigned long randseed;
short frameseqcount, framesequence;
int lastvalue;
signed char square1sweep, sweep1shift, sweep1negative, sq1sweepperiod;
signed char square2sweep, sweep2shift, sweep2negative, sq2sweepperiod;

int DMC_STEREO_POS,Sq1Pan=6,Sq2Pan=10;


//short *StBuff;
//int StBuffPos;

unsigned char trianglewave[32] =
{
  0xF, 0xE, 0xD, 0xC, 0xB, 0xA, 0x9, 0x8, 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,
       0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF
};

const signed char squarewave[4] [8] =
{
  -1, 1, -1, -1, -1, -1, -1, -1, -1, 1, 1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, 1, -1, -1, 1, 1, 1, 1, 1
};

const short dmcperiod[16] =
{
  0x1AC, 0x17C, 0x154, 0x140, 0x11E, 0x0FE, 0x0E2, 0x0D6, 0x0BE, 0x0A0, 0x08E, 0x080, 0x06A, 0x054, 0x048, 0x036
};

unsigned short noiseperiod[16] =
{
  4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
};


unsigned char lengthtable[32] =
{
  5, 127, 10, 1, 19, 2, 40, 3, 80, 4, 30, 5, 7, 6, 13, 7, 6, 8, 12, 9, 24, 10, 48, 11, 96, 12, 36, 13, 8, 14, 16, 15
};




unsigned short randint( long max )
{
  randseed = randseed * 0x08088405;
  randseed++;
  return ( ( randseed >> 16 ) * max ) >> 16;
}

void NSF_Init( unsigned char Tune )
{
  init6502();
  reset6502();
  daccounter = 64;
  MainClock = 0;
  zeromem( gameImage, 32767 );
  randseed=0;
  //normal=96;
  trianglevolume = 16;
  A = Tune;
  X = 1;
  PC = NSF_HDR.init;
  gameImage[0x100 + S--] = 0xFF;
  gameImage[0x100 + S--] = 0xFF;
  while ( PC > 4 )
  {
    opcode = gameImage[PC++];
    instruction[opcode] ();
  }
  gameImage[0x4015] = 0x0F;
  PC = NSF_HDR.play;
  gameImage[0x100 + S--] = 0xFF;
  gameImage[0x100 + S--] = 0xFF;
  while ( PC > 4 )
  {
    opcode = gameImage[PC++];
    instruction[opcode] ();
  }
}

void MapperTest()
{
  signed char * MapBuffer;
  int i, a;
  i = a = 0;
  while ( i < 8 )
  {
    a = a + NSF_HDR.BankSwitch[i];
    i++;
  }
  if ( a == 0 ) return;
  MapBuffer = malloc( 65536 );
  memcpy( MapBuffer, ( void * ) ( gameImage + 32768 ), 65536 );
  i = 0;
  while ( i < 8 )
  {
    memcpy( ( void * ) ( gameImage + 32768 + i * 4096 ), ( void * ) ( MapBuffer + 4096 * NSF_HDR.BankSwitch[i] ), 4096 );
    i++;
  }
  //mfree(MapBuffer);
}

void APUWrite( WORD address, BYTE data )
{
  switch ( address )
  {
    //Square1
    //ddle nnnn   duty, loop enoisevolume/disable length, enoisevolume disable, vol/enoisevolume
    case 0x4000:
      //if(sw1d!=(gameImage[0x4000] >> 6))
      sw1d = gameImage[0x4000] >> 6;
      Sq1Pan=10-(sw1d)/2;
      square1volume = gameImage[0x4000] & 0x0F;
      //if(gameImage[0x4000]&32)gameImage[0x4015]=gameImage[0x4015]&254;else
      //     gameImage[0x4015]=gameImage[0x4015]|1;
    break;
      //eppp nsss   enable sweep, period, negative, shift
    case 0x4001:
      square1sweep = ( gameImage[0x4001] >> 4 ) & 7;
      sweep1shift = gameImage[0x4001] & 7;
      sweep1negative = gameImage[0x4001] & 8;
      sq1sweepperiod = ( gameImage[0x4001] >> 4 ) & 7;
    break;
      //pppp pppp   period low
    case 0x4002:
      square1period = ( gameImage[0x4002] + ( ( gameImage[0x4003] & 7 ) << 8 ) );
    break;
      //llll lppp length index, period high
    case 0x4003:
      square1period = ( gameImage[0x4002] + ( ( gameImage[0x4003] & 7 ) << 8 ) );
      square1lencounter = lengthtable[gameImage[0x4003] >> 3];
      square1devider = 0;
      square1envelope = 15;
      square1envdiv = ( gameImage[0x4000] & 15 ) + 1;
    break;
      //Square2
      //ddle nnnn   duty, loop enoisevolume/disable length, enoisevolume disable, vol/enoisevolume
    case 0x4004:
      //if(sw2d!=(gameImage[0x4004] >> 6))Sq2Pan=16-Sq2Pan;
      sw2d = gameImage[0x4004] >> 6;
      Sq2Pan=6+sw2d/2;
      square2volume = gameImage[0x4004] & 0x0F;
      // if(gameImage[0x4004]&32)gameImage[0x4015]=gameImage[0x4015]&253;else
      //      gameImage[0x4015]=gameImage[0x4015]|2;
    break;
      //eppp nsss   enable sweep, period, negative, shift
    case 0x4005:
      square2sweep = ( gameImage[0x4005] >> 4 ) & 7;
      sweep2shift = gameImage[0x4005] & 7;
      sweep2negative = gameImage[0x4005] & 8;
      sq2sweepperiod = ( gameImage[0x4005] >> 4 ) & 7;
    break;
      //pppp pppp   period low
    case 0x4006:
      square2period = ( gameImage[0x4006] + ( ( gameImage[0x4007] & 7 ) << 8 ) );
    break;
      //llll lppp length index, period high
    case 0x4007:
      square2period = ( gameImage[0x4006] + ( ( gameImage[0x4007] & 7 ) << 8 ) );
      square2lencounter = lengthtable[gameImage[0x4007] >> 3];
      square2devider = 0;
      square2envelope = 15;
      square2envdiv = ( gameImage[0x4004] & 15 ) + 1;

    break;
      //Triangle
      //clll llll   conoisedeviderrol, linear counoisedeviderer load
    case 0x4008:
      if ( ( gameImage[0x4008] & 128 ) > 0 ) trianglelencounter = lengthtable[gameImage[0x400B] >> 3];

    break;
      //pppp pppp   period low
    case 0x400A:
      triangleperiod = ( gameImage[0x400A] + ( ( gameImage[0x400B] & 7 ) << 8 ) );
    break;
      //llll lppp   length index, period high
    case 0x400B:
      triangleperiod = ( gameImage[0x400A] + ( ( gameImage[0x400B] & 7 ) << 8 ) );
      twhaltflag = 128;
    break;
      //Noise
      //--le nnnn   loop enoisevolume/disable length, enoisevolume disable, vol/enoisevolume period
    case 0x400C:

      noisevolume = gameImage[0x400C] & 15;
      //if(gameImage[0x400C]&32)gameImage[0x4015]=gameImage[0x4015]&247;else
      //      gameImage[0x4015]=gameImage[0x4015]|8;
    break;
      //s--- pppp   short mode, period index
    case 0x400E:
      noisedevider = 0;
    break;
      //llll l---   length index
    case 0x400F:
      noiselencounter = lengthtable[gameImage[0x400F] >> 3];
      noiseenvelope = 15;
      noiseenvdiv = ( gameImage[0x400C] & 15 ) + 1;
    break;
      //DMC
      //il-- ffff   IRQ enable, loop, frequency index
    case 0x4010:


    break;
      //-ddd dddd   DAC
    case 0x4011:
      daccounter = ( gameImage[0x4011] & 127 );
    break;
      //aaaa aaaa   sample address
    case 0x4012:
      dmcdmaadr = gameImage[0x4012] * 0x40 + 0xC000;      
    break;
      //llll llll   sample length
    case 0x4013:
      dmcdatalen = gameImage[0x4013] * 0x10 + 1;
      DMC_STEREO_POS=!DMC_STEREO_POS;
    break;
      //Common
      //---d noisedevider21   length ctr enable: DMC, noise, triangle, pulse 2, 1
    case 0x4015:

    break;
      //fd-- ----   5-frame cycle, disable frame inoisedevidererrupt
    case 0x4017:
      framesequence = 0;
      frameseqcount = 0;
    break;

  }
}

void LenSweepC()
{

  if ( ( ( gameImage[0x4000] & 32 ) == 0 ) && ( square1lencounter > 0 ) )
  {
    square1lencounter--;
  }
  if ( ( ( gameImage[0x4004] & 32 ) == 0 ) && ( square2lencounter > 0 ) )
  {
    square2lencounter--;
  }
  if ( ( ( gameImage[0x4008] & 128 ) == 0 ) && ( trianglelencounter > 0 ) )
  {
    trianglelencounter--;
  }
  if ( ( ( gameImage[0x400C] & 32 ) == 0 ) && ( noiselencounter > 0 ) )
  {
    noiselencounter--;
  }
  //Sweep
  //Square1
  if ( gameImage[0x4001] & 128 ) square1sweep -= 1;
  if ( square1sweep < 0 )
  {
    square1sweep = sq1sweepperiod;
    if ( sweep1negative )
      square1period = square1period - ( square1period >> ( sweep1shift + 1 ) );
    else
      square1period = square1period + ( square1period >> ( sweep1shift + 1 ) );
    if ( square1period < 0 ) square1period += 2048;
    if ( square1period > 2047 ) square1period -= 2048;
  }
  //
  //Square2
  if ( gameImage[0x4005] & 128 ) square2sweep -= 1;
  if ( square2sweep < 0 )
  {
    square2sweep = sq2sweepperiod;
    if ( sweep1negative )
      square2period = square2period - ( square2period >> ( sweep1shift + 1 ) );
    else
      square2period = square2period + ( square2period >> ( sweep1shift + 1 ) );
    if ( square2period < 0 ) square2period += 2048;
    if ( square2period > 2047 ) square2period -= 2048;
  }

}

void EnvLinC()
{
  //Linear counter
  if ( twhaltflag != 0 )
  {
    triabglelinearcounter = gameImage[0x4008] & 127;
  }
  else
  {
    if ( triabglelinearcounter > 0 )
    {
      triabglelinearcounter--;
    }
  }
  if ( ( gameImage[0x4008] & 128 ) == 0 )
  {
    twhaltflag = 0;
  }

  //Envelope
  //Noise
  noiseenvdiv--;
  if ( noiseenvdiv <= 0 )
  {
    noiseenvdiv = ( gameImage[0x400C] & 15 ) + 1;
    if ( noiseenvelope > 0 ) noiseenvelope--;
    if ( ( noiseenvelope <= 0 ) && ( ( gameImage[0x400C] & 32 ) != 0 ) ) noiseenvelope = 15;
  }
  //Square1
  square1envdiv--;
  if ( square1envdiv <= 0 )
  {
    square1envdiv = ( gameImage[0x4000] & 15 ) + 1;

    if ( square1envelope > 0 ) square1envelope--;
    if ( ( square1envelope <= 0 ) && ( ( gameImage[0x4000] & 32 ) != 0 ) )
      square1envelope = 15;
  }
  //Square2
  square2envdiv--;
  if ( square2envdiv <= 0 )
  {
    square2envdiv = ( gameImage[0x4004] & 15 ) + 1;

    if ( square2envelope > 0 ) square2envelope--;
    if ( ( square2envelope <= 0 ) && ( ( gameImage[0x4004] & 32 ) != 0 ) )
      square2envelope = 15;
  }
}

void GetNsfSound( unsigned short * ptr, int nsamples )
{
  int addL,addR, tri, squ1, squ2, noise;
  addL =addR = tri = squ1 = squ2 = noise = 0;

  if ( ( NsfSongLen - NsfTime ) < 0 )
  {
    AudioActive = 0;
    zeromem( ptr, nsamples * 2 );
    return;
  }

  while ( nsamples )
  {

    if ( gameImage[0x4015] & 16 )
    {
      dmcdevider -= 1792000 / samplerate;
      while ( dmcdevider <= 0 )
      {
        dmcdevider += dmcperiod[gameImage[0x4010] & 15];
        //
        if ( dmcdatalen > 0 )
        {
          if ( dmcbuffer & 1 )
          {
            if ( daccounter < 126 ) daccounter += 2;
          }
          else
          {
            if ( daccounter > 1 ) daccounter -= 2;
          }
        }

        dmcbuffer = dmcbuffer >> 1;
        dmcbuffercount++;
        if ( dmcbuffercount >= 8 )
        {
          dmcbuffercount = 0;
          if ( dmcdatalen > 0 ) dmcbuffer = gameImage[dmcdmaadr];
          //if(dmcbuffer==0)dmcsilence=1;else dmcsilence=0;
          dmcdmaadr++;
          if ( dmcdmaadr > 0xFFFF ) dmcdmaadr = 0x8000;
          if ( dmcdatalen > 0 ) dmcdatalen--;
          if ( dmcdatalen <= 0 )
          {
            if ( gameImage[0x4010] & 64 )
            {
              dmcdmaadr = gameImage[0x4012] * 0x40 + 0xC000;
              dmcdatalen = gameImage[0x4013] * 0x10 + 1;
            }
          }
        }
      }
    }

    if ( triangleperiod >= 8 ) triangledevider -= 1792000 / samplerate;
    while ( triangledevider <= 0 )
    {
      triangledevider += triangleperiod + 1;
      if ( ( gameImage[0x400A] + ( ( gameImage[0x400B] & 7 ) << 8 ) ) > 4 ) triangleseqpos++;
      if ( triangleseqpos >= 32 ) triangleseqpos = 0;
    }

    if ( square1period >= 8 ) square1devider -= 1792000 / samplerate / 2;
    while ( square1devider <= 0 )
    {
      square1devider += ( square1period + 1 );
      square1seqpos++;
      if ( square1seqpos >= 8 ) square1seqpos = 0;
    }
    //
    if ( square2period >= 8 ) square2devider -= 1792000 / samplerate / 2;
    while ( square2devider <= 0 )
    {
      square2devider += square2period + 1;
      square2seqpos++;
      if ( square2seqpos >= 8 ) square2seqpos = 0;
    }

    noisedevider -= 1792000 / samplerate;
    if ( noisedevider <= 0 )
    {
      noisedevider += noiseperiod[gameImage[0x400E] & 15];
      if ( ( gameImage[0x400C] & 16 ) == 0 ) noiseoutput = ( randint( 2 ) - 1 ) * noiseenvelope;
      else
        noiseoutput = ( randint( 2 ) - 1 ) * noisevolume;
    }
    //
    if ( ( ( gameImage[0x4015] & 1 ) == 0 ) || ( square1lencounter != 0 ) )
    {
      if ( ( gameImage[0x4000] & 16 ) != 0 ) squ1 = squarewave[sw1d] [square1seqpos] * square1volume;
      else
        squ1 = squarewave[sw1d] [square1seqpos] * square1envelope;
    }
    if ( ( ( gameImage[0x4015] & 2 ) == 0 ) || ( square2lencounter != 0 ) )
    {
      if ( ( gameImage[0x4004] & 16 ) != 0 ) squ2 = squarewave[sw2d] [square2seqpos] * square2volume;
      else
        squ2 = squarewave[sw2d] [square2seqpos] * square2envelope;
    }
    if ( ( ( ( gameImage[0x4015] & 4 ) == 0 ) && ( trianglelencounter != 0 ) ) || ( ( triabglelinearcounter != 0 ) ) )
      tri = ( trianglewave[triangleseqpos] - 7 ) * trianglevolume;

    if ( ( ( gameImage[0x4015] & 8 ) == 0 ) || ( noiselencounter != 0 ) )
    {
      noise = noiseoutput;
    }


    frameseqcount++;
    if ( frameseqcount >= ( speed / 4 ) )
    {
      frameseqcount = 0;
      if ( ( gameImage[0x4017] & 128 ) == 0 )
      //4 steps
      {
        if ( ( framesequence == 1 ) || ( framesequence == 3 ) )
        {
          LenSweepC();
        } //clock length counoisedeviderers and sweep units
        EnvLinC();
        if ( framesequence >= 3 ) framesequence = 0;
      }
      else
      //5 steps
      {
        if ( ( framesequence == 0 ) || ( framesequence == 2 ) )
        {
          LenSweepC();
        } //clock length counoisedeviderers and sweep units
        if ( framesequence < 4 ) EnvLinC();
        if ( framesequence >= 4 ) framesequence = 0;
      }
      framesequence++;
      LenSweepC();
    }

    //
    MainClock++;
    if ( MainClock >= speed )
    {
      NsfTime += 1;
      MainClock = 0;
      PC = NSF_HDR.play;
      gameImage[0x100 + S--] = 0xFF;
      gameImage[0x100 + S--] = 0xFF;
      while ( PC > 4 )
      {
        opcode = gameImage[PC++];
        instruction[opcode] ();
      }
      //gameImage[0x4015]=gameImage[0x4015]&(255-192);
    }
    
    int SqL,SqR;
    
    if(DMC_STEREO_POS)
    {
    //SqL=( squ1*Sq1Pan + squ2*Sq2Pan )+StBuff[StBuffPos+1]+( daccounter - 63 ) * 6;
    //SqR=( squ1*(16-Sq1Pan) + squ2*(16-Sq2Pan) )+StBuff[StBuffPos]+( daccounter - 63 ) * 8;
    SqL=( squ1*Sq1Pan + squ2*Sq2Pan )+( daccounter - 63 ) * 6;
    SqR=( squ1*(16-Sq1Pan) + squ2*(16-Sq2Pan) )+( daccounter - 63 ) * 8;
    }
    else
    {
    //SqL=( squ1*Sq1Pan + squ2*Sq2Pan )+StBuff[StBuffPos+1]+( daccounter - 63 ) * 8;
    //SqR=( squ1*(16-Sq1Pan) + squ2*(16-Sq2Pan) )+StBuff[StBuffPos]+( daccounter - 63 ) * 6;    
    SqL=( squ1*Sq1Pan + squ2*Sq2Pan )+( daccounter - 63 ) * 8;
    SqR=( squ1*(16-Sq1Pan) + squ2*(16-Sq2Pan) )+( daccounter - 63 ) * 6; 
    //addL = ( ( squ1 + squ2*2 ) * 8 + tri * 2 + noise * 16 + ( daccounter - 63 ) * 4 ) * 16;
    //addR = ( ( squ1*2 + squ2 ) * 8 + tri * 2 + noise * 16 + ( daccounter - 63 ) * 6 ) * 16;
    }
    
    addL = ( SqL*4 + tri * 6 + noise * 64 ) * 4;
    addR = ( SqR*4 + tri * 6 + noise * 64 ) * 4;
    
    //StBuff[StBuffPos++]=SqL/4;
    //StBuff[StBuffPos++]=SqR/4;
    
    //if(StBuffPos>=16000)StBuffPos=0;
    //addL = ( ( squ1*2 + squ2*3 ) * 4 + tri * 2 + noise * 16 + ( daccounter - 63 ) * 8 ) * 16;
    //addR = ( ( squ1*3 + squ2*2 ) * 4 + tri * 2 + noise * 16 + ( daccounter - 63 ) * 8 ) * 16;
    //lastvalue = add;
    /*
    if ( addL > 32767 )
    {
      addL = 32767;
    }
    if ( addL < -32767 )
    {
      add = -32767;
    }
    */
    
    if ( ( NsfSongLen - NsfTime ) < 256 )
    {
      addR = addR * ( NsfSongLen - NsfTime ) / 256;
      addL = addL * ( NsfSongLen - NsfTime ) / 256;
    }
    * ptr++ = addL;
    * ptr++ = addR;
    nsamples-=2;
  }

}

int GetNsfTime()
{
  return ( NsfTime / 50 );
}
