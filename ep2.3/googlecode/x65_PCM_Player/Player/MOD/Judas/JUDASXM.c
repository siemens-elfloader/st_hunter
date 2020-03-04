/* * JUDAS V2.0 Extended Module loading/playing */


#define NULL 0
#define ENV_ON 1
#define ENV_SUSTAIN 2
#define ENV_LOOP 4
#define SMP_LOOP 1
#define SMP_PINGPONGLOOP 2
#define SMP_16BIT 16
#define MAX_CHANNELS 32
#define MAX_PATTERNS 256
#define MAX_INSTRUMENTS 128
#define MAX_SAMPLES 16
#define KEYOFF 0x61
int judas_error;
typedef struct
{
  char id[17];
  char modname[20];
  unsigned char idbyte;
  char trackername[20];
  unsigned short version;
}
XM_ID;

typedef struct
{
  unsigned char note;
  unsigned char instrument;
  unsigned char voleffect;
  unsigned char effect;
  unsigned char effectdata;
}
XM_NOTE;

typedef struct
{
  unsigned headersize;
  unsigned short songlength;
  unsigned short restartpos;
  unsigned short channels;
  unsigned short patterns;
  unsigned short instruments;
  unsigned short uselinear;
  unsigned short defaulttempo;
  unsigned short defaultbpmtempo;
  unsigned char order[256];
}
XM_HEADER;

typedef struct
{
  unsigned long headersize;
  unsigned char packingtype;
  unsigned short rows;
  unsigned short packsize;
}
XM_PATTERN;

typedef struct
{
  unsigned short x;
  unsigned short y;
}
ENVELOPE;

typedef struct
{
  unsigned headersize;
  char name[22];
  unsigned char type;
  unsigned short numbersamples;

  unsigned long sampleheadersize;
  unsigned char sampletable[96];
  ENVELOPE volenv[12];
  ENVELOPE panenv[12];
  unsigned char volenvpoints;
  unsigned char panenvpoints;

  unsigned char volsustpoint;
  unsigned char volloopstart;

  unsigned char volloopend;
  unsigned char pansustpoint;

  unsigned char panloopstart;
  unsigned char panloopend;

  unsigned char volenvflags;
  unsigned char panenvflags;

  unsigned char vibratotype;
  unsigned char vibratosweep;

  unsigned char vibratodepth;
  unsigned char vibratorate;

  short fadeout;
  short reserved;
  unsigned char c_volenv[325];
  unsigned char c_panenv[325];
}
XM_INSTRUMENT;

typedef struct
{
  unsigned length;
  unsigned loopstart;
  unsigned looplength;
  unsigned char vol;
  signed char finetune;
  unsigned char sampleflags;
  unsigned char panning;
  signed char relativenote;
  unsigned char reserved;
  char name[22];
}
XM_SAMPLE;

typedef struct
{
  unsigned short rows;
  unsigned char * data;
}
OUR_PATTERN;

typedef struct
{
  XM_INSTRUMENT * ip;
  XM_SAMPLE * xsp;
  SAMPLE * sp;
  unsigned char newnote;
  short realnote;
  unsigned char note;
  unsigned char instrument;
  unsigned char newinstrument;
  unsigned char voleffect;
  unsigned char effect;
  unsigned char effectdata;
  unsigned char nybble1;
  unsigned char nybble2;
  unsigned char smp;
  signed char notevol;
  signed char vol;
  signed char finetune;
  short baseperiod;
  short period;
  short targetperiod;
  unsigned char notepanning;
  unsigned char keyon;
  int volenvpos;
  int panenvpos;
  int fadeoutvalue;
  unsigned char tp;
  unsigned char tpspeed;
  unsigned char volspeedup;
  unsigned char volspeeddown;
  unsigned char portaspeedup;
  unsigned char portaspeeddown;
  unsigned char panspeedright;
  unsigned char panspeedleft;
  unsigned char vibratotype;
  unsigned char vibratospeed;
  unsigned char vibratodepth;
  unsigned char vibratophase;
  unsigned char sampleoffset;
  unsigned char glissando;
  unsigned char tremolotype;
  unsigned char tremolospeed;
  unsigned char tremolodepth;
  unsigned char tremolophase;
  unsigned char instrvibratophase;
  unsigned short instrvibratodepth;
  unsigned char retrigcount;
  unsigned char retriginterval;
  unsigned char retrigvolchange;
  unsigned char patternloopline;
  unsigned char patternloopcount;
  unsigned char tremorcount;
  unsigned char tremorontime;
  unsigned char tremorofftime;
  unsigned char tremorstatus;
}
TRACK;

/* Prototypes */
int judas_loadxm( char * name );
void judas_freexm( void );
void judas_playxm( void );
void judas_stopxm( void );
unsigned char judas_getxmpos( void );
unsigned char judas_getxmline( void );
unsigned char judas_getxmtick( void );
unsigned char judas_getxmchannels( void );
char * judas_getxmname( void );
static int init_xmplayer( void );
static void xmplayer_code_lock_start( void );
static void xmplayer( void );
static void startnewnote( TRACK * tptr, CHANNEL * chptr );
static void changeinstrument( TRACK * tptr );
static void extendedcommand( TRACK * tptr, CHANNEL * chptr );
static int getamigaperiod( int note, int finetune );
static void xmplayer_code_lock_end( void );

/* Variables and structures */
static XM_ID xm_id;
static XM_HEADER xm_header;
static XM_PATTERN load_pattern;
static OUR_PATTERN pattern[MAX_PATTERNS];
static XM_INSTRUMENT * first_instr_ptr;
static XM_SAMPLE * xm_smp_ptr[MAX_INSTRUMENTS * MAX_SAMPLES];
static SAMPLE * smp_ptr[MAX_INSTRUMENTS * MAX_SAMPLES];
static TRACK track[MAX_CHANNELS];
static char xm_loaded = 0;
static char xmplayer_firsttime = 1;
static unsigned short xm_pos;
static unsigned short xm_line;
static unsigned short xm_ticktempo;
static unsigned short xm_tickcount;
static signed char xm_globalvol;
static unsigned char xm_globalvolspeedup;
static unsigned char xm_globalvolspeeddown;
static unsigned char xm_patternbreak;
static unsigned char xm_patterndelay;
static int lowperiod;
static int highperiod;

/* External variables */
//unsigned char judas_bpmtempo;
//unsigned judas_bpmcount;
char FromFile;
char *memstream;
int streampos;

int judas_open( char * name )
{
  if(FromFile) return(fopen( name, A_BIN,P_READ, & err ));
  return(1);
}

int judas_seek( int handle, int bytes, int whence )
{
  if(FromFile)return ( lseek( handle, bytes, whence ,&err,&err) );
  streampos+=bytes;
  return(bytes);
}

int judas_read( int handle, void * buffer, int size )
{
  if(FromFile)return ( fread( handle,buffer,  size, &err ) );
  memcpy(buffer,&memstream[streampos],size);
  streampos+=size;
  return(size);
}

void judas_close( int handle )
{
  if(FromFile)fclose( handle,&err );
}

int judas_loadxm( char * name )
{
  int handle;
  int count;

  /* Don't waste memory if Nosound */
  judas_error = JUDAS_OK;
  //if (judas_device == DEV_NOSOUND) return 1;

  judas_error = JUDAS_OUT_OF_MEMORY;
  if ( xmplayer_firsttime )
  {
    if ( !init_xmplayer() ) return 0;
  }
  judas_freexm();
  /* * Try to open the file */
  judas_error = JUDAS_OPEN_ERROR;
  handle = judas_open( name );
  if ( handle == -1 )
  {
    return 0;
  }
  /* * Read in the identification */
  judas_error = JUDAS_WRONG_FORMAT;
  if ( judas_read( handle, & xm_id, sizeof xm_id ) != sizeof xm_id )
  {
    judas_close( handle );
    return 0;
  }
  /* * Check that it's an XM and the version is correct */
  if ( ( memcmp( "Extended Module:", xm_id.id, 16 ) ) || ( xm_id.idbyte != 0x1a ) )
  {
    judas_close( handle );
    return 0;
  }
  xm_id.idbyte = 0x0;
  /* Serves as the songname endzero from now on */
  if ( xm_id.version < 0x103 )
  {
    judas_close( handle );
    return 0;
  }
  /* * Read in the header */
  judas_error = JUDAS_READ_ERROR;
  if ( judas_read( handle, & xm_header, sizeof xm_header ) != sizeof xm_header )
  {
    judas_close( handle );
    return 0;
  }
  /* * Check that there aren't too many channels */
  if ( xm_header.channels > CHANNELS )
  {
    xm_header.channels = CHANNELS;
    judas_error = JUDAS_OUT_OF_CHANNELS;
    judas_close( handle );
    return 0;
  }
  /* * If header is longer that what we use of it, skip the extra bytes */
  if ( judas_seek( handle, xm_header.headersize - sizeof xm_header, SEEK_CUR ) == -1 )
  {
    judas_close( handle );
    return 0;
  }
  /* * Load the patterns */
  for ( count = 0; count < xm_header.patterns; count++ )
  {
    unsigned char * packptr;
    unsigned char * unpackptr;
    int packleft;
    /* * Load pattern header */
    judas_error = JUDAS_READ_ERROR;
    if ( judas_read( handle, & load_pattern, 5 ) != 5 )
    {
      judas_freexm();
      judas_close( handle );
      return 0;
    }
    if ( judas_read( handle, & load_pattern.rows, 4 ) != 4 )
    {
      judas_freexm();
      judas_close( handle );
      return 0;
    }

    /* * Skip extra data in pattern header (future expansion :-) ) */
    if ( judas_seek( handle, load_pattern.headersize - 9, SEEK_CUR ) == -1 )
    {
      judas_freexm();
      judas_close( handle );
      return 0;
    }
    /* * Allocate memory for unpacked pattern & clear it */
    pattern[count].rows = load_pattern.rows;
    pattern[count].data = locked_malloc( 5 * load_pattern.rows * xm_header.channels );
    judas_error = JUDAS_OUT_OF_MEMORY;
    if ( !pattern[count].data )
    {
      judas_freexm();
      judas_close( handle );
      return 0;
    }
    memset( pattern[count].data, 0, 5 * load_pattern.rows * xm_header.channels );
    if ( load_pattern.packsize )
    {
      unsigned char * pack_buffer;
      /* * Allocate packeddata-buffer */
      pack_buffer = malloc( load_pattern.packsize );
      if ( !pack_buffer )
      {
        judas_freexm();
        judas_close( handle );
        return 0;
      }
      /* * Read the packed data in */
      judas_error = JUDAS_READ_ERROR;
      if ( judas_read( handle, pack_buffer, load_pattern.packsize ) != load_pattern.packsize )
      {
        mfree( pack_buffer );
        judas_freexm();
        judas_close( handle );
        return 0;
      }
      packptr = pack_buffer;
      unpackptr = pattern[count].data;
      packleft = load_pattern.packsize;

      while ( packleft > 0 )
      {
        unsigned char control = * packptr++;
        packleft--;
        /* Packed? */
        if ( control & 0x80 )
        {
          /* Note? */
          if ( control & 0x01 )
          {
            * unpackptr++ = * packptr++;
            packleft--;
          }
          else
            unpackptr++;
          /* Instrument? */
          if ( control & 0x02 )
          {
            * unpackptr++ = * packptr++;
            packleft--;
          }
          else
            unpackptr++;
          /* Volume column? */
          if ( control & 0x04 )
          {
            * unpackptr++ = * packptr++;
            packleft--;
          }
          else
            unpackptr++;
          /* Effect? */
          if ( control & 0x08 )
          {
            * unpackptr++ = * packptr++;
            packleft--;
          }
          else
            unpackptr++;
          /* Effect parameter? */
          if ( control & 0x10 )
          {
            * unpackptr++ = * packptr++;
            packleft--;
          }
          else
            unpackptr++;
        }
        else
        {
          * unpackptr++ = control;
          /* Note */
          * unpackptr++ = * packptr++;
          /* Instrument */
          * unpackptr++ = * packptr++;
          /* Volume c. */
          * unpackptr++ = * packptr++;
          /* Effect */
          * unpackptr++ = * packptr++;
          /* Effect p. */
          packleft -= 4;
        }
      }
      mfree( pack_buffer );
    }
  }
  /* * Load instruments. This is heavy shit. */
  first_instr_ptr = locked_malloc( sizeof( XM_INSTRUMENT ) * xm_header.instruments );
  judas_error = JUDAS_OUT_OF_MEMORY;
  if ( !first_instr_ptr )
  {
    judas_freexm();
    judas_close( handle );
    return 0;
  }
  for ( count = 0; count < xm_header.instruments; count++ )
  {
    XM_INSTRUMENT * instr_ptr = first_instr_ptr + count;

    /* * Read the part common to all instruments */
    judas_error = JUDAS_READ_ERROR;
    if ( judas_read( handle, instr_ptr, 27 ) != 27 )
    {
      judas_freexm();
      judas_close( handle );
      return 0;
    }
    if ( judas_read( handle, &instr_ptr->numbersamples, 2 ) != 2 )
    {
      judas_freexm();
      judas_close( handle );
      return 0;
    }

    if ( instr_ptr->numbersamples )
    {
      int s;

      /* * There are samples; read the rest in */
      if ( judas_read( handle, &instr_ptr->sampleheadersize, 214 )
           != 214 )
           {
             judas_freexm();
             judas_close( handle );
             return 0;
      }
      /* * Skip over extra shit in instr. header */
      if ( judas_seek( handle, instr_ptr->headersize - 243, SEEK_CUR ) == -1 )
      {
        judas_freexm();
        judas_close( handle );
        return 0;
      }
      /* * Precalculate envelopes */
      if ( instr_ptr->volenvflags & ENV_ON )
      {
        for ( s = 0; s < instr_ptr->volenvpoints; s++ )
        {
          /* Paranoid check */
          if ( instr_ptr->volenv[s].x > 324 ) instr_ptr->volenv[s].x = 324;
        }
        for ( s = 0; s < instr_ptr->volenvpoints - 1; s++ )
        {
          int x, y, dx, dy;

          x = instr_ptr->volenv[s].x;
          y = instr_ptr->volenv[s].y;
          dx = instr_ptr->volenv[s + 1].x - instr_ptr->volenv[s].x;
          dy = instr_ptr->volenv[s + 1].y - instr_ptr->volenv[s].y;
          if ( dx )
          {
            for ( x = 0; x < dx; x++ )
            {
              instr_ptr->c_volenv[x + instr_ptr->volenv[s].x] = y + dy * x / dx;
            }
          }
          if ( s == instr_ptr->volenvpoints - 2 )
          {
            for ( x = instr_ptr->volenv[s + 1].x; x < 325; x++ )
            {
              instr_ptr->c_volenv[x] = instr_ptr->volenv[s + 1].y;
            }
          }
        }
        instr_ptr->volsustpoint = instr_ptr->volenv[instr_ptr->volsustpoint].x;
        instr_ptr->volloopstart = instr_ptr->volenv[instr_ptr->volloopstart].x;
        instr_ptr->volloopend = instr_ptr->volenv[instr_ptr->volloopend].x;
        /* If zero length loop, must deactivate loop */
        if ( instr_ptr->volloopstart == instr_ptr->volloopend )
        {
          instr_ptr->volenvflags &= ~ENV_LOOP;
        }
      }
      if ( instr_ptr->panenvflags & ENV_ON )
      {
        for ( s = 0; s < instr_ptr->panenvpoints; s++ )
        {
          /* Paranoid check */
          if ( instr_ptr->panenv[s].x > 324 ) instr_ptr->panenv[s].x = 324;
        }
        for ( s = 0; s < instr_ptr->panenvpoints - 1; s++ )
        {
          int x, y, dx, dy;

          x = instr_ptr->panenv[s].x;
          y = instr_ptr->panenv[s].y;
          dx = instr_ptr->panenv[s + 1].x - instr_ptr->panenv[s].x;
          dy = instr_ptr->panenv[s + 1].y - instr_ptr->panenv[s].y;
          if ( dx )
          {
            for ( x = 0; x < dx; x++ )
            {
              instr_ptr->c_panenv[x + instr_ptr->panenv[s].x] = y + dy * x / dx;
            }
          }
          if ( s == instr_ptr->panenvpoints - 2 )
          {
            for ( x = instr_ptr->panenv[s + 1].x; x < 325; x++ )
            {
              instr_ptr->c_panenv[x] = instr_ptr->panenv[s + 1].y;
            }
          }
        }
        instr_ptr->pansustpoint = instr_ptr->panenv[instr_ptr->pansustpoint].x;
        instr_ptr->panloopstart = instr_ptr->panenv[instr_ptr->panloopstart].x;
        instr_ptr->panloopend = instr_ptr->panenv[instr_ptr->panloopend].x;
        /* If zero length loop, must deactivate loop */
        if ( instr_ptr->panloopstart == instr_ptr->panloopend )
        {
          instr_ptr->panenvflags &= ~ENV_LOOP;
        }
      }
      /* * First round, allocate xmsample & sample structures, * but do not read any audio data yet */
      for ( s = 0; s < instr_ptr->numbersamples; s++ )
      {
        XM_SAMPLE * xsp;
        SAMPLE * sp;
        unsigned reserve;

        /* * Allocate memory for sample header */
        xsp = locked_malloc( sizeof( XM_SAMPLE ) );
        judas_error = JUDAS_OUT_OF_MEMORY;
        if ( !xsp )
        {
          judas_freexm();
          judas_close( handle );
          return 0;
        }
        xm_smp_ptr[count * MAX_SAMPLES + s] = xsp;

        /* * Read in sample header */
        judas_error = JUDAS_READ_ERROR;
        if ( judas_read( handle, xsp, sizeof( XM_SAMPLE ) ) != sizeof( XM_SAMPLE ) )
        {
          judas_freexm();
          judas_close( handle );
          return 0;
        }
        /* * Hop over extra shit in sample header */
        if ( judas_seek( handle, instr_ptr->sampleheadersize - sizeof( XM_SAMPLE ), SEEK_CUR ) == -1 )
        {
          judas_freexm();
          judas_close( handle );
          return 0;
        }
        /* * If looplength is zero, loop must be de- * activated */
        if ( !xsp->looplength )
        {
          xsp->sampleflags &= ~( SMP_LOOP | SMP_PINGPONGLOOP );
        }
        reserve = xsp->length;
        if ( xsp->sampleflags & SMP_PINGPONGLOOP ) reserve += xsp->looplength;
        /* * There might be zero length samples, don't * go to hell because of them */
        if ( reserve )
        {
          /* * Reserve sample, go to hell if fail */
          sp = judas_allocsample( reserve );
          if ( !sp )
          {
            judas_freexm();
            judas_close( handle );
            return 0;
          }
          smp_ptr[count * MAX_SAMPLES + s] = sp;
        }
      }
      /* * Second round: time for the big shit! */
      for ( s = 0; s < instr_ptr->numbersamples; s++ )
      {
        XM_SAMPLE * xsp = xm_smp_ptr[count * MAX_SAMPLES + s];
        SAMPLE * sp = smp_ptr[count * MAX_SAMPLES + s];
        /* * Check if this sample exists */
        if ( sp )
        {
          /* * Read sample data (delta values) */
          judas_error = JUDAS_READ_ERROR;
          if ( judas_read( handle, sp->start, xsp->length ) != xsp->length )
          {
            judas_freexm();
            judas_close( handle );
            return 0;
          }
          sp->voicemode = VM_ON;
          if ( xsp->sampleflags & ( SMP_LOOP | SMP_PINGPONGLOOP ) ) sp->voicemode |= VM_LOOP;
          /* * Convert to normal signed data */
          if ( xsp->sampleflags & SMP_16BIT )
          {
            int cc = xsp->length >> 1;
            unsigned short old = 0;
            unsigned short * cptr = ( unsigned short * ) sp->start;

            sp->voicemode |= VM_16BIT;
            while ( cc-- )
            {
              * cptr += old;
              old = * cptr++;
            }
          }
          else
          {
            int cc = xsp->length;
            unsigned char old = 0;
            unsigned char * cptr = sp->start;

            while ( cc-- )
            {
              * cptr += old;
              old = * cptr++;
            }
          }
          /* * Ugly shit: "unroll" pingpong-loop */
          if ( xsp->sampleflags & SMP_PINGPONGLOOP )
          {
            sp->voicemode |= VM_LOOP;
            if ( xsp->sampleflags & SMP_16BIT )
            {
              int uc = xsp->looplength >> 1;
              short * source = ( short * ) ( sp->start + xsp->loopstart + xsp->looplength - 2 );
              short * dest = ( short * ) ( sp->start + xsp->loopstart + xsp->looplength );

              while ( uc-- ) * dest++ = * source--;
            }
            else
            {
              int uc = xsp->looplength;
              char * source = sp->start + xsp->loopstart + xsp->looplength - 1;
              char * dest = sp->start + xsp->loopstart + xsp->looplength;

              while ( uc-- ) * dest++ = * source--;
            }
            xsp->looplength <<= 1;
          }
          /* * Fix repeat & end in "our" sample structure */
          if ( sp->voicemode & VM_LOOP )
          {
            sp->repeat = sp->start + xsp->loopstart;
            sp->end = sp->start + xsp->loopstart + xsp->looplength;
          }
          else
            sp->end = sp->start + xsp->length;
          /* * Interpolation-correction */
          judas_ipcorrect( sp );
          /* * We're done with this sample! */
        }
      }
    }
    else
    {
      /* * Header without samples; skip over extra if any */
      judas_error = JUDAS_READ_ERROR;
      if ( judas_seek( handle, instr_ptr->headersize - 29, SEEK_CUR ) == -1 )
      {
        judas_freexm();
        judas_close( handle );
        return 0;
      }
    }
  }
  judas_close( handle );
  judas_error = JUDAS_OK;
  xm_loaded = 1;
  return 1;
}

void judas_freexm( void )
{
  int count;

  if ( xmplayer_firsttime )
  {
    if ( !init_xmplayer() ) return;
  }
  judas_stopxm();
  xm_loaded = 0;
  for ( count = 0; count < MAX_PATTERNS; count++ )
  {
    if ( pattern[count].data )
    {
      locked_free( pattern[count].data );
      pattern[count].data = NULL;
    }
  }
  if ( first_instr_ptr )
  {
    locked_free( first_instr_ptr );
    first_instr_ptr = NULL;
  }
  for ( count = 0; count < MAX_SAMPLES * MAX_INSTRUMENTS; count++ )
  {
    if ( xm_smp_ptr[count] )
    {
      locked_free( xm_smp_ptr[count] );
      xm_smp_ptr[count] = NULL;
    }
    if ( smp_ptr[count] )
    {
      judas_freesample( smp_ptr[count] );
      smp_ptr[count] = NULL;
    }
  }
}

void judas_playxm( void )
{
  int count;
  TRACK * tptr = & track[0];
  CHANNEL * chptr = & judas_channel[0];

  if ( !xm_loaded ) return;
  xm_pos = 0;
  xm_line = 0;
  xm_tickcount = 0;
  xm_ticktempo = xm_header.defaulttempo;
  xm_globalvol = 64;
  xm_patterndelay = 0;
  judas_bpmcount = 0;
  judas_bpmtempo = xm_header.defaultbpmtempo;
  for ( count = xm_header.channels; count > 0; count-- )
  {
    chptr->smp = NULL;
    chptr->voicemode = VM_OFF;
    tptr->ip = first_instr_ptr;
    tptr->xsp = NULL;
    tptr->sp = NULL;
    tptr->instrument = 0;
    tptr->smp = 0;
    tptr->notevol = 0;
    tptr->vol = 0;
    tptr->effect = 0;
    tptr->effectdata = 0;
    tptr->nybble1 = 0;
    tptr->nybble2 = 0;
    tptr->voleffect = 0;
    tptr->volenvpos = 0;
    tptr->panenvpos = 0;
    tptr->keyon = 0;
    tptr->tp = 0;
    tptr->tpspeed = 0;
    tptr->volspeedup = 0;
    tptr->volspeeddown = 0;
    tptr->glissando = 0;
    tptr->retriginterval = 0;
    tptr->retrigvolchange = 0;
    tptr->patternloopline = 0;
    tptr->patternloopcount = 0;
    tptr->sampleoffset = 0;
    tptr->tremolotype = 0;
    tptr->vibratotype = 0;
    chptr++;
    tptr++;
  }
  if ( xm_header.uselinear )
  {
    lowperiod = 7743;
    highperiod = 64;
  }
  else
  {
    lowperiod = 29024;
    highperiod = 28;
  }
}

void judas_stopxm( void )
{
  int count;
  CHANNEL * chptr = & judas_channel[0];

  if ( xm_header.channels > CHANNELS ) xm_header.channels = CHANNELS;
  if ( !xm_loaded ) return;
  for ( count = xm_header.channels; count > 0; count-- )
  {
    chptr->smp = NULL;
    chptr->voicemode = VM_OFF;
    chptr++;
  }
}

unsigned char judas_getxmpos( void )
{
  return xm_pos;
}

unsigned char judas_getxmline( void )
{
  return xm_line;
}

unsigned char judas_getxmtick( void )
{
  return xm_tickcount;
}

unsigned char judas_getxmchannels( void )
{
  return xm_header.channels;
}

char * judas_getxmname( void )
{
  if ( xm_loaded ) return xm_id.modname;
  else
    return NULL;
}

static int init_xmplayer( void )
{
  int count;

  xm_header.channels = 0;
  for ( count = 0; count < MAX_PATTERNS; count++ )
  {
    pattern[count].data = NULL;
  }
  first_instr_ptr = NULL;
  for ( count = 0; count < MAX_SAMPLES * MAX_INSTRUMENTS; count++ )
  {
    xm_smp_ptr[count] = NULL;
    smp_ptr[count] = NULL;
  }
  /* if (!judas_locktables()) return 0; if (!judas_memlock(&xmplayer_code_lock_start, (int)&xmplayer_code_lock_end -
  (int)&xmplayer_code_lock_start)) return 0; if (!judas_memlock(&xm_header, sizeof xm_header)) return 0;
  if (!judas_memlock(&pattern[0], sizeof pattern)) return 0; if (!judas_memlock(&first_instr_ptr, sizeof first_instr_ptr))
  return 0; if (!judas_memlock(&xm_smp_ptr[0], sizeof xm_smp_ptr)) return 0;
  if (!judas_memlock(&smp_ptr[0], sizeof smp_ptr)) return 0; if (!judas_memlock(&track[0], sizeof track)) return 0;
  if (!judas_memlock(&xm_loaded, sizeof xm_loaded)) return 0; if (!judas_memlock(&xm_pos, sizeof xm_pos)) return 0;
  if (!judas_memlock(&xm_line, sizeof xm_line)) return 0; if (!judas_memlock(&xm_ticktempo, sizeof xm_ticktempo)) return 0;
  if (!judas_memlock(&xm_tickcount, sizeof xm_tickcount)) return 0;
  if (!judas_memlock(&xm_patternbreak, sizeof xm_patternbreak)) return 0;
  if (!judas_memlock(&xm_patterndelay, sizeof xm_patterndelay)) return 0;
  if (!judas_memlock(&xm_globalvol, sizeof xm_globalvol)) return 0;
  if (!judas_memlock(&xm_globalvolspeedup, sizeof xm_globalvolspeedup)) return 0;
  if (!judas_memlock(&xm_globalvolspeeddown, sizeof xm_globalvolspeeddown)) return 0;
  if (!judas_memlock(&lowperiod, sizeof lowperiod)) return 0; if (!judas_memlock(&highperiod, sizeof highperiod)) return 0; */
  xmplayer_firsttime = 0;
  return 1;
}

static void xmplayer_code_lock_start( void )
{
}

void xmplayer( void )
{
  TRACK * tptr = & track[0];
  CHANNEL * chptr = & judas_channel[0];
  OUR_PATTERN * pattptr = & pattern[xm_header.order[xm_pos]];
  int count;

  /* * Set new notes or do something else? */
  if ( ( !xm_tickcount ) && ( !xm_patterndelay ) )
  {
    XM_NOTE * noteptr = ( XM_NOTE * ) pattptr->data;

    if ( !noteptr ) return;
    noteptr += xm_header.channels * xm_line;

    xm_patternbreak = 0;
    for ( count = xm_header.channels; count > 0; count-- )
    {
      tptr->newnote = 0;

      /* * Get note (if any) */
      if ( noteptr->note )
      {
        tptr->note = noteptr->note - 1;
        tptr->newnote = 1;
      }
      /* * Get effect, effect data etc. */
      tptr->voleffect = noteptr->voleffect;
      tptr->effect = noteptr->effect;
      tptr->effectdata = noteptr->effectdata;
      tptr->nybble1 = noteptr->effectdata >> 4;
      tptr->nybble2 = noteptr->effectdata & 0xf;
      tptr->newinstrument = noteptr->instrument;

      /* * Set sampleoffset here */
      if ( tptr->effect == 0x9 )
      {
        if ( tptr->effectdata ) tptr->sampleoffset = tptr->effectdata;
      }

      /* * Start new note if there is one; but check there * isn't notedelay (toneportamento is handled by * startnewnote()!)
      * Also restart shit if an instrument number! */
      if ( ( tptr->effect != 0xe ) || ( tptr->nybble1 != 0xd ) || ( tptr->nybble2 == 0 ) )
      {
        if ( tptr->newnote ) startnewnote( tptr, chptr );
        if ( tptr->newinstrument ) changeinstrument( tptr );
      }

      /* * Reset period if not vibrato or toneportamento */
      if ( ( ( tptr->effect < 0x3 ) || ( tptr->effect > 0x6 ) ) && ( tptr->voleffect != 0xb ) && ( tptr->voleffect != 0xf ) )
      {
        tptr->period = tptr->baseperiod;
      }
      /* * Reset volume if not tremolo / tremor */
      if ( ( tptr->effect != 0x7 ) && ( tptr->effect != 0x1d ) )
      {
        tptr->vol = tptr->notevol;
      }
      /* * Now check some effects: volume column done first. */
      switch ( tptr->voleffect >> 4 )
      {
        case 0x0:
        break;

          /* Set volume */
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
          /* Applies only if there isn't notedelay */
          if ( ( tptr->effect != 0xe ) || ( tptr->nybble1 != 0xd ) || ( tptr->nybble2 == 0 ) )
          {
            tptr->notevol = tptr->voleffect - 0x10;
            if ( tptr->notevol > 64 ) tptr->notevol = 64;
            tptr->vol = tptr->notevol;
          }
        break;

          /* Fine volslide down */
        case 0x8:
          tptr->notevol -= tptr->voleffect & 0xf;
          if ( tptr->notevol < 0 ) tptr->notevol = 0;
          tptr->vol = tptr->notevol;
        break;

          /* Fine volslide up */
        case 0x9:
          tptr->notevol += tptr->voleffect & 0xf;
          if ( tptr->notevol > 64 ) tptr->notevol = 64;
          tptr->vol = tptr->notevol;
        break;

          /* Set vibrato speed */
        case 0xa:
          if ( tptr->voleffect & 0xf )
          {
            tptr->vibratospeed = tptr->voleffect & 0xf;
          }
        break;

          /* Vibrato */
        case 0xb:
          if ( tptr->voleffect & 0xf )
          {
            tptr->vibratodepth = tptr->voleffect & 0xf;
          }
        break;

          /* Set panning */
        case 0xc:
          /* Applies only if there isn't notedelay */
          if ( ( tptr->effect != 0xe ) || ( tptr->nybble1 != 0xd ) || ( tptr->nybble2 == 0 ) )
          {
            tptr->notepanning = ( tptr->voleffect & 0xf ) << 4 | ( tptr->voleffect & 0xf );
          }
        break;

          /* Toneportamento */
        case 0xf:
          if ( tptr->voleffect & 0xf )
          {
            tptr->tpspeed = ( tptr->voleffect & 0xf ) << 4;
          }
        break;
      }

      /* * Then the regular effects */
      switch ( tptr->effect )
      {
        case 0x0:
        break;

          /* Set portamento speed up */
        case 0x1:
          if ( tptr->effectdata ) tptr->portaspeedup = tptr->effectdata;
        break;

          /* Set portamento speed down */
        case 0x2:
          if ( tptr->effectdata ) tptr->portaspeeddown = tptr->effectdata;
        break;

          /* Set TP. speed */
        case 0x3:
          if ( tptr->effectdata ) tptr->tpspeed = tptr->effectdata;
        break;

          /* Set vibrato */
        case 0x4:
          if ( tptr->nybble1 ) tptr->vibratospeed = tptr->nybble1;
          if ( tptr->nybble2 ) tptr->vibratodepth = tptr->nybble2;
        break;

          /* Set tremolo */
        case 0x7:
          if ( tptr->nybble1 ) tptr->tremolospeed = tptr->nybble1;
          if ( tptr->nybble2 ) tptr->tremolodepth = tptr->nybble2;
        break;

          /* Set Panning */
        case 0x8:
          tptr->notepanning = tptr->effectdata;
        break;

          /* Volume slide speed set */
        case 0x5:
        case 0x6:
        case 0xa:
          if ( tptr->effectdata )
          {
            tptr->volspeedup = tptr->nybble1;
            tptr->volspeeddown = tptr->nybble2;
          }
        break;

          /* Pos. jump */
        case 0xb:
          xm_line = pattptr->rows - 1;
          if(xm_pos >= (tptr->effectdata - 1)){
          AudioActive=0;
          playlist[plcurrent].songtime=GetPlayingTime()*50;    
          SavePlayList( PlayListName );
      }
          xm_pos = tptr->effectdata - 1;
          

        break;

          /* Set volume */
        case 0xc:
          tptr->notevol = tptr->effectdata;
          if ( tptr->notevol < 0 ) tptr->notevol = 0;
          if ( tptr->notevol > 64 ) tptr->notevol = 64;
          tptr->vol = tptr->notevol;
        break;

          /* Pattern break */
        case 0xd:
          if ( !xm_patternbreak )
          {
            xm_patternbreak = 1;
            xm_line = tptr->nybble1 * 10 + tptr->nybble2 - 1;
            xm_pos++;
          }
        break;

          /* Extended command */
        case 0xe:
          extendedcommand( tptr, chptr );
        break;

          /* Set tempo */
        case 0xf:
          if ( tptr->effectdata < 32 ) xm_ticktempo = tptr->effectdata;
          else
            judas_bpmtempo = tptr->effectdata;
        break;

          /* Global volume */
        case 0x10:
          xm_globalvol = tptr->effectdata;
          if ( xm_globalvol > 64 ) xm_globalvol = 64;
        break;

          /* Global volume slide */
        case 0x11:
          if ( tptr->effectdata )
          {
            xm_globalvolspeedup = tptr->nybble1;
            xm_globalvolspeeddown = tptr->nybble2;
          }
        break;

          /* Keyoff */
        case 0x14:
          tptr->keyon = 0;
          if ( !( tptr->ip->volenvflags & ENV_ON ) )
          {
            tptr->notevol = 0;
            tptr->vol = 0;
          }
        break;

          /* Set envpos */
        case 0x15:
          tptr->volenvpos = tptr->effectdata;
          tptr->panenvpos = tptr->effectdata;
        break;

          /* Panning slide */
        case 0x19:
          if ( tptr->effectdata )
          {
            tptr->panspeedright = tptr->nybble1;
            tptr->panspeedleft = tptr->nybble2;
          }
        break;

          /* Multi retrig */
        case 0x1b:
          if ( tptr->nybble1 ) tptr->retrigvolchange = tptr->nybble1;
          if ( tptr->nybble2 ) tptr->retriginterval = tptr->nybble2;
          if ( tptr->retrigcount >= tptr->retriginterval )
          {
            startnewnote( tptr, chptr );
            tptr->retrigcount = 0;
            /* Now modify volume */
            if ( !retrigmultable[tptr->retrigvolchange] )
            {
              tptr->notevol += retrigaddtable[tptr->retrigvolchange];
              if ( tptr->notevol < 0 ) tptr->notevol = 0;
              if ( tptr->notevol > 64 ) tptr->notevol = 64;
              tptr->vol = tptr->notevol;
            }
            else
            {
              tptr->notevol = ( tptr->notevol * retrigmultable[tptr->retrigvolchange] ) >> 4;
              if ( tptr->notevol > 64 ) tptr->notevol = 64;
              tptr->vol = tptr->notevol;
            }
          }
          tptr->retrigcount++;
        break;

          /* Tremor */
        case 0x1d:
          if ( tptr->effectdata )
          {
            tptr->tremorontime = tptr->nybble1;
            tptr->tremorofftime = tptr->nybble2;
          }
        break;

          /* Extra fine portamento */
        case 0x21:
          switch ( tptr->nybble1 )
          {
            case 1:
              if ( tptr->nybble2 ) tptr->portaspeedup = tptr->nybble2;
              tptr->baseperiod -= tptr->portaspeedup;
              if ( tptr->baseperiod < highperiod ) tptr->baseperiod = highperiod;
              tptr->period = tptr->baseperiod;
            break;

            case 2:
              if ( tptr->nybble2 ) tptr->portaspeeddown = tptr->nybble2;
              tptr->baseperiod += tptr->portaspeeddown;
              if ( tptr->baseperiod > lowperiod ) tptr->baseperiod = lowperiod;
              tptr->period = tptr->baseperiod;
            break;
          }
        break;
      }
      noteptr++;
      chptr++;
      tptr++;
    }
  }
  if ( xm_tickcount )
  {
    /* * If tick isn't 0, update "continuous" effects */
    for ( count = xm_header.channels; count > 0; count-- )
    {
      /* * Volume column */
      switch ( tptr->voleffect >> 4 )
      {
        case 0x0:
        break;

          /* Volslide down */
        case 0x6:
          tptr->notevol -= tptr->voleffect & 0xf;
          if ( tptr->notevol < 0 ) tptr->notevol = 0;
          tptr->vol = tptr->notevol;
        break;

          /* Volslide up */
        case 0x7:
          tptr->notevol += tptr->voleffect & 0xf;
          if ( tptr->notevol > 64 ) tptr->notevol = 64;
          tptr->vol = tptr->notevol;
        break;

          /* Vibrato */
        case 0xb:
          tptr->vibratophase += tptr->vibratospeed * 4;
          tptr->period = tptr->baseperiod
               + ( ( vibratotable[tptr->vibratotype & 3] [tptr->vibratophase] * tptr->vibratodepth ) >> 3 );
          if ( tptr->period < highperiod ) tptr->period = highperiod;
          if ( tptr->period > lowperiod ) tptr->period = lowperiod;
        break;

          /* Panslide left */
        case 0xd:
          {
            int new = tptr->notepanning;
            new -= tptr->voleffect & 0xf;
            if ( new < 0 ) new = 0;
            tptr->notepanning = new;
          }
        break;

          /* Panslide right */
        case 0xe:
          {
            int new = tptr->notepanning;
            new += tptr->voleffect & 0xf;
            if ( new > 255 ) new = 255;
            tptr->notepanning = new;
          }
        break;

          /* Toneportamento */
        case 0xf:
          if ( tptr->tp )
          {
            if ( tptr->baseperiod < tptr->targetperiod )
            {
              tptr->baseperiod += tptr->tpspeed * 4;
              if ( tptr->baseperiod >= tptr->targetperiod )
              {
                tptr->baseperiod = tptr->targetperiod;
                tptr->tp = 0;
              }
            }
            if ( tptr->baseperiod > tptr->targetperiod )
            {
              tptr->baseperiod -= tptr->tpspeed * 4;
              if ( tptr->baseperiod <= tptr->targetperiod )
              {
                tptr->baseperiod = tptr->targetperiod;
                tptr->tp = 0;
              }
            }
            tptr->period = tptr->baseperiod;
            if ( tptr->glissando )
            {
              if ( xm_header.uselinear )
              {
                tptr->period += tptr->finetune / 2;
                tptr->period += 32;
                tptr->period &= 0xffc0;
                tptr->period -= tptr->finetune / 2;
              }
              else
              {
                int offset = 0x7fffffff;
                int sc;
                short bestperiod;

                for ( sc = 0; sc <= 118; sc++ )
                {
                  int newoffset = abs( tptr->period - getamigaperiod( sc, tptr->finetune ) );

                  if ( newoffset < offset )
                  {
                    bestperiod = getamigaperiod( sc, tptr->finetune );
                    offset = newoffset;
                  }
                }
                tptr->period = bestperiod;
              }
            }
          }
        break;
      }


      /* * Regular effects */
      switch ( tptr->effect )
      {
        /* Arpeggio */
        case 0x0:
          if ( tptr->effectdata )
          {
            char phase = xm_tickcount % 3;
            switch ( phase )
            {
              case 0:
                tptr->period = tptr->baseperiod;
              break;

              case 1:
                if ( xm_header.uselinear ) tptr->period = tptr->baseperiod - tptr->nybble1 * 64;
                else
                  tptr->period = getamigaperiod( tptr->realnote + tptr->nybble1, tptr->finetune );
                if ( tptr->period < highperiod ) tptr->period = highperiod;
              break;

              case 2:
                if ( xm_header.uselinear ) tptr->period = tptr->baseperiod - tptr->nybble2 * 64;
                else
                  tptr->period = getamigaperiod( tptr->realnote + tptr->nybble2, tptr->finetune );
                if ( tptr->period < highperiod ) tptr->period = highperiod;
              break;
            }
          }
        break;

          /* Portamento up */
        case 0x1:
          tptr->baseperiod -= tptr->portaspeedup * 4;
          if ( tptr->baseperiod < highperiod ) tptr->baseperiod = highperiod;
          tptr->period = tptr->baseperiod;
        break;

          /* Portamento down */
        case 0x2:
          tptr->baseperiod += tptr->portaspeeddown * 4;
          if ( tptr->baseperiod > lowperiod ) tptr->baseperiod = lowperiod;
          tptr->period = tptr->baseperiod;
        break;

          /* Toneportamento */
        case 0x3:
          if ( tptr->tp )
          {
            if ( tptr->baseperiod < tptr->targetperiod )
            {
              tptr->baseperiod += tptr->tpspeed * 4;
              if ( tptr->baseperiod >= tptr->targetperiod )
              {
                tptr->baseperiod = tptr->targetperiod;
                tptr->tp = 0;
              }
            }
            if ( tptr->baseperiod > tptr->targetperiod )
            {
              tptr->baseperiod -= tptr->tpspeed * 4;
              if ( tptr->baseperiod <= tptr->targetperiod )
              {
                tptr->baseperiod = tptr->targetperiod;
                tptr->tp = 0;
              }
            }
            tptr->period = tptr->baseperiod;
            if ( tptr->glissando )
            {
              if ( xm_header.uselinear )
              {
                tptr->period += tptr->finetune / 2;
                tptr->period += 32;
                tptr->period &= 0xffc0;
                tptr->period -= tptr->finetune / 2;
              }
              else
              {
                int offset = 0x7fffffff;
                int sc;
                short bestperiod;

                for ( sc = 0; sc <= 118; sc++ )
                {
                  int newoffset = abs( tptr->period - getamigaperiod( sc, tptr->finetune ) );

                  if ( newoffset < offset )
                  {
                    bestperiod = getamigaperiod( sc, tptr->finetune );
                    offset = newoffset;
                  }
                }
                tptr->period = bestperiod;
              }
            }
          }
        break;

          /* Vibrato */
        case 0x4:
          tptr->vibratophase += tptr->vibratospeed * 4;
          tptr->period = tptr->baseperiod
               + ( ( vibratotable[tptr->vibratotype & 3] [tptr->vibratophase] * tptr->vibratodepth ) >> 3 );
          if ( tptr->period < highperiod ) tptr->period = highperiod;
          if ( tptr->period > lowperiod ) tptr->period = lowperiod;
        break;

          /* Toneportamento + volslide */
        case 0x5:
          if ( tptr->tp )
          {
            if ( tptr->baseperiod < tptr->targetperiod )
            {
              tptr->baseperiod += tptr->tpspeed * 4;
              if ( tptr->baseperiod >= tptr->targetperiod )
              {
                tptr->baseperiod = tptr->targetperiod;
                tptr->tp = 0;
              }
            }
            if ( tptr->baseperiod > tptr->targetperiod )
            {
              tptr->baseperiod -= tptr->tpspeed * 4;
              if ( tptr->baseperiod <= tptr->targetperiod )
              {
                tptr->baseperiod = tptr->targetperiod;
                tptr->tp = 0;
              }
            }
            tptr->period = tptr->baseperiod;
            if ( tptr->glissando )
            {
              if ( xm_header.uselinear )
              {
                tptr->period += tptr->finetune / 2;
                tptr->period += 32;
                tptr->period &= 0xffc0;
                tptr->period -= tptr->finetune / 2;
              }
              else
              {
                int offset = 0x7fffffff;
                int sc;
                short bestperiod;

                for ( sc = 0; sc <= 118; sc++ )
                {
                  int newoffset = abs( tptr->period - getamigaperiod( sc, tptr->finetune ) );

                  if ( newoffset < offset )
                  {
                    bestperiod = getamigaperiod( sc, tptr->finetune );
                    offset = newoffset;
                  }
                }
                tptr->period = bestperiod;
              }
            }
          }
          tptr->notevol += tptr->volspeedup;
          if ( tptr->notevol > 64 ) tptr->notevol = 64;
          tptr->notevol -= tptr->volspeeddown;
          if ( tptr->notevol < 0 ) tptr->notevol = 0;
          tptr->vol = tptr->notevol;
        break;

          /* Vibrato + volslide */
        case 0x6:
          tptr->vibratophase += tptr->vibratospeed * 4;
          tptr->period = tptr->baseperiod
               + ( ( vibratotable[tptr->vibratotype & 3] [tptr->vibratophase] * tptr->vibratodepth ) >> 3 );
          if ( tptr->period < highperiod ) tptr->period = highperiod;
          if ( tptr->period > lowperiod ) tptr->period = lowperiod;
          tptr->notevol += tptr->volspeedup;
          if ( tptr->notevol > 64 ) tptr->notevol = 64;
          tptr->notevol -= tptr->volspeeddown;
          if ( tptr->notevol < 0 ) tptr->notevol = 0;
          tptr->vol = tptr->notevol;
        break;

          /* Tremolo */
        case 0x7:
          tptr->tremolophase += tptr->tremolospeed * 4;
          tptr->vol = tptr->notevol
               + ( ( vibratotable[tptr->tremolotype & 3] [tptr->tremolophase] * tptr->tremolodepth ) >> 4 );
          if ( tptr->vol < 0 ) tptr->vol = 0;
          if ( tptr->vol > 64 ) tptr->vol = 64;
        break;

          /* Volume Slide */
        case 0xa:
          tptr->notevol += tptr->volspeedup;
          if ( tptr->notevol > 64 ) tptr->notevol = 64;
          tptr->notevol -= tptr->volspeeddown;
          if ( tptr->notevol < 0 ) tptr->notevol = 0;
          tptr->vol = tptr->notevol;
        break;

          /* Extended command */
        case 0xe:
          extendedcommand( tptr, chptr );
        break;

          /* Global volume slide */
        case 0x11:
          xm_globalvol += xm_globalvolspeedup;
          if ( xm_globalvol > 64 ) xm_globalvol = 64;
          xm_globalvol -= xm_globalvolspeeddown;
          if ( xm_globalvol < 0 ) xm_globalvol = 0;
        break;

          /* Panning slide */
        case 0x19:
          {
            int new = tptr->notepanning;

            new += tptr->panspeedright;
            if ( new < 0 ) new = 0;
            new -= tptr->panspeedleft;
            if ( new > 255 ) new = 255;
            tptr->notepanning = new;
          }
        break;

          /* Multi retrig */
        case 0x1b:
          if ( tptr->retrigcount >= tptr->retriginterval )
          {
            startnewnote( tptr, chptr );
            tptr->retrigcount = 0;
            /* Now modify volume */
            if ( !retrigmultable[tptr->retrigvolchange] )
            {
              tptr->notevol += retrigaddtable[tptr->retrigvolchange];
              if ( tptr->notevol < 0 ) tptr->notevol = 0;
              if ( tptr->notevol > 64 ) tptr->notevol = 64;
              tptr->vol = tptr->notevol;
            }
            else
            {
              tptr->notevol = ( tptr->notevol * retrigmultable[tptr->retrigvolchange] ) >> 4;
              if ( tptr->notevol > 64 ) tptr->notevol = 64;
              tptr->vol = tptr->notevol;
            }
          }
          tptr->retrigcount++;
        break;

          /* Tremor */
        case 0x1d:
          if ( !tptr->tremorcount )
          {
            tptr->tremorstatus ^= 1;
            if ( tptr->tremorstatus ) tptr->tremorcount = tptr->tremorontime + 1;
            else
              tptr->tremorcount = tptr->tremorofftime + 1;
          }
          if ( tptr->tremorstatus ) tptr->vol = tptr->notevol;
          else
            tptr->vol = 0;
          tptr->tremorcount--;
        break;
      }
      chptr++;
      tptr++;
    }
  }

  /* * Update envelopes and set the frequency, volume & panning of
  * the channel (unlike in mods, these depend on many things) */
  tptr = & track[0];
  chptr = & judas_channel[0];
  for ( count = xm_header.channels; count > 0; count-- )
  {
    XM_INSTRUMENT * ip = tptr->ip;
    short finalperiod = tptr->period;

    /* * Update fadeout (must be done BEFORE before calculating * the final volume -- checked visually by recording FT2's
    * sound output to tape and then sampling it!) */
    if ( !tptr->keyon )
    {
      tptr->fadeoutvalue -= ip->fadeout;
      if ( tptr->fadeoutvalue < 0 ) tptr->fadeoutvalue = 0;
    }

    /* * Calculate final channel values */
    {
      int finalpan;
      int finalvol;
      if ( ip->volenvflags & ENV_ON )
        finalvol = ( tptr->vol * xm_globalvol * ip->c_volenv[tptr->volenvpos] * ( tptr->fadeoutvalue >> 9 ) ) >> 18;
      else
        finalvol = ( tptr->vol * xm_globalvol * ( tptr->fadeoutvalue >> 9 ) ) >> 12;
      if ( ip->panenvflags & ENV_ON )
        finalpan = tptr->notepanning + ( ip->c_panenv[tptr->panenvpos] - 32 ) * ( 128 - abs( tptr->notepanning - 128 ) ) / 32;
      else
        finalpan = tptr->notepanning;
      if ( finalpan < 0 ) finalpan = 0;
      if ( finalpan > 255 ) finalpan = 255;
      if ( finalvol > 64 ) finalvol = 64;
      chptr->vol = finalvol;
      chptr->panning = finalpan;
    }
    /* * Update instrument vibrato */
    if ( ( ip->vibratodepth ) && ( ip->vibratorate ) )
    {
      if ( ip->vibratosweep )
      {
        if ( tptr->keyon )
        {
          tptr->instrvibratodepth += ( ip->vibratodepth << 8 ) / ip->vibratosweep;
          if ( tptr->instrvibratodepth > ( ip->vibratodepth << 8 ) ) tptr->instrvibratodepth = ip->vibratodepth << 8;
        }
      }
      else
        tptr->instrvibratodepth = ip->vibratodepth << 8;
      tptr->instrvibratophase += ip->vibratorate;
      finalperiod += ( vibratotable[ip->vibratotype & 3] [tptr->instrvibratophase] * tptr->instrvibratodepth ) >> 14;
    }
    if ( finalperiod < ( highperiod - 15 ) ) finalperiod = highperiod - 15;
    if ( finalperiod > ( lowperiod + 15 ) ) finalperiod = lowperiod + 15;
    if ( xm_header.uselinear ) chptr->freq = linearfreqtable[finalperiod % 768] >> ( finalperiod / 768 );
    else
      chptr->freq = 14317456 / finalperiod;
    /* * Update envelopes (must be done AFTER setting the finalvol * or else EASTWING.XM will sound incorrect!) */
    if ( ( ip->volenvflags & ENV_ON )
         && ( ( !( ip->volenvflags & ENV_SUSTAIN ) ) || ( !tptr->keyon ) || ( tptr->volenvpos != ip->volsustpoint ) ) )
         {
           tptr->volenvpos++;
           if ( tptr->volenvpos > 324 ) tptr->volenvpos = 324;
           if ( ip->volenvflags & ENV_LOOP )
           {
             if ( tptr->volenvpos >= ip->volloopend )
               tptr->volenvpos = ip->volloopstart;
           }
    }
    if ( ( ip->panenvflags & ENV_ON )
         && ( ( !( ip->panenvflags & ENV_SUSTAIN ) ) || ( !tptr->keyon ) || ( tptr->panenvpos != ip->pansustpoint ) ) )
         {
           tptr->panenvpos++;
           if ( tptr->panenvpos > 324 ) tptr->panenvpos = 324;
           if ( ip->panenvflags & ENV_LOOP )
           {
             if ( tptr->panenvpos >= ip->panloopend )
               tptr->panenvpos = ip->panloopstart;
           }
    }

    tptr++;
    chptr++;
  }
  /* * Advance song */
  if ( xm_ticktempo )
  {
    xm_tickcount++;
    if ( xm_tickcount >= xm_ticktempo )
    {
      xm_tickcount = 0;
      if ( xm_patterndelay )
      {
        xm_patterndelay--;
      }
      if ( !xm_patterndelay )
      {
        xm_line++;
        if ( xm_line >= pattptr->rows )
        {
          xm_line = 0;
          xm_pos++;
        }
        if ( xm_pos >= xm_header.songlength ) {
          xm_pos = xm_header.restartpos;
          AudioActive=0;
          playlist[plcurrent].songtime=GetPlayingTime()*50;    
        SavePlayList( PlayListName );
        }
      }
    }
  }
}

static void startnewnote( TRACK * tptr, CHANNEL * chptr )
{
  if ( tptr->newinstrument ) tptr->instrument = tptr->newinstrument - 1;
  if ( tptr->instrument < xm_header.instruments ) tptr->ip = first_instr_ptr + tptr->instrument;
  /* * Handle keyoff */
  if ( tptr->note == KEYOFF - 1 )
  {
    tptr->keyon = 0;
    if ( !( tptr->ip->volenvflags & ENV_ON ) )
    {
      tptr->notevol = 0;
      tptr->vol = 0;
    }
    return;
  }

  /* * Now get samplenum & samplepointers */
  tptr->smp = tptr->ip->sampletable[tptr->note];
  tptr->xsp = xm_smp_ptr[tptr->instrument * MAX_SAMPLES + tptr->smp];
  tptr->sp = smp_ptr[tptr->instrument * MAX_SAMPLES + tptr->smp];

  /* * Don't go further if sample doesn't exist */
  if ( ( tptr->xsp ) && ( tptr->sp ) )
  {
    tptr->finetune = tptr->xsp->finetune;
    tptr->retrigcount = 1;
    if ( !( tptr->vibratotype & 4 ) ) tptr->vibratophase = 0;
    if ( !( tptr->tremolotype & 4 ) ) tptr->tremolophase = 0;
    tptr->tremorcount = 0;
    tptr->tremorstatus = 0;
    tptr->realnote = tptr->note + tptr->xsp->relativenote;
    if ( tptr->realnote < 0 ) tptr->realnote = 0;
    if ( tptr->realnote > 118 ) tptr->realnote = 118;
    if ( ( tptr->effect == 0x3 ) || ( tptr->effect == 0x5 ) || ( ( tptr->voleffect >> 4 ) == 0xf ) )
    {
      /* * Toneportamento */
      if ( xm_header.uselinear ) tptr->targetperiod = 7680 - tptr->realnote * 64 - tptr->finetune / 2;
      else
        tptr->targetperiod = getamigaperiod( tptr->realnote, tptr->finetune );
      tptr->tp = 1;
    }
    else
    {
      /* * Normal note start */
      if ( xm_header.uselinear ) tptr->baseperiod = 7680 - tptr->realnote * 64 - tptr->finetune / 2;
      else
        tptr->baseperiod = getamigaperiod( tptr->realnote, tptr->finetune );
      tptr->period = tptr->baseperiod;
      tptr->tp = 0;
      chptr->fractpos = 0;
      chptr->pos = tptr->sp->start;
      chptr->repeat = tptr->sp->repeat;
      //chptr->dir=1;
      chptr->end = tptr->sp->end;
      chptr->voicemode = tptr->sp->voicemode;
      if ( tptr->effect == 0x9 )
      {
        char * newpos;

        if ( tptr->sp->voicemode & VM_16BIT ) newpos = tptr->sp->start + ( tptr->sampleoffset << 9 );
        else
          newpos = tptr->sp->start + ( tptr->sampleoffset << 8 );
        if ( newpos >= tptr->sp->end )
        {
          chptr->voicemode = VM_OFF;
        }
        else
          chptr->pos = newpos;
      }
    }
  }
}

/* * Extended commands can occur both at tick 0 and on other ticks; make it a
* function to prevent having to write it twice in the code */
static void extendedcommand( TRACK * tptr, CHANNEL * chptr )
{
  switch ( tptr->nybble1 )
  {
    /* Fine porta up */
    case 0x1:
      if ( !xm_tickcount )
      {
        if ( tptr->nybble2 ) tptr->portaspeedup = tptr->nybble2;
        tptr->baseperiod -= tptr->portaspeedup * 4;
        if ( tptr->baseperiod < highperiod ) tptr->baseperiod = highperiod;
      }
    break;

      /* Fine porta down */
    case 0x2:
      if ( !xm_tickcount )
      {
        if ( tptr->nybble2 ) tptr->portaspeeddown = tptr->nybble2;
        tptr->baseperiod += tptr->portaspeeddown * 4;
        if ( tptr->baseperiod > lowperiod ) tptr->baseperiod = lowperiod;
      }
    break;

      /* Set glissando */
    case 0x3:
      if ( !xm_tickcount ) tptr->glissando = tptr->nybble2;
    break;

      /* Set vibrato waveform */
    case 0x4:
      if ( !xm_tickcount )
      {
        tptr->vibratotype = vibratotypetable[tptr->nybble2 & 3];
        tptr->vibratotype |= tptr->nybble2 & 4;
      }
    break;

      /* Set finetune */
    case 0x5:
      if ( ( !xm_tickcount ) && ( tptr->newnote ) )
      {
        tptr->finetune = ( tptr->nybble2 << 4 ) - 128;
        if ( xm_header.uselinear ) tptr->baseperiod = 7680 - tptr->realnote * 64 - tptr->finetune / 2;
        else
          tptr->baseperiod = getamigaperiod( tptr->realnote, tptr->finetune );
        tptr->period = tptr->baseperiod;
      }
    break;

      /* Patternloop */
    case 0x6:
      if ( !xm_tickcount )
      {
        if ( !tptr->nybble2 ) tptr->patternloopline = xm_line;
        else
        {
          if ( !tptr->patternloopcount )
          {
            tptr->patternloopcount = tptr->nybble2;
            xm_line = tptr->patternloopline - 1;
          }
          else
          {
            tptr->patternloopcount--;
            if ( tptr->patternloopcount ) xm_line = tptr->patternloopline - 1;
          }
        }
      }
    break;

      /* Set tremolo waveform */
    case 0x7:
      if ( !xm_tickcount )
      {
        tptr->tremolotype = vibratotypetable[tptr->nybble2 & 3];
        tptr->tremolotype |= tptr->nybble2 & 4;
      }
    break;

      /* Retrig */
    case 0x9:
      if ( tptr->nybble2 )
      {
        if ( tptr->retrigcount > tptr->nybble2 )
        {
          tptr->retrigcount = 1;
          /* Don't retrig on tick 0 */
          if ( xm_tickcount )
          {
            startnewnote( tptr, chptr );
            tptr->keyon = 1;
            tptr->fadeoutvalue = 32768;
            tptr->volenvpos = 0;
            tptr->panenvpos = 0;
            tptr->instrvibratophase = 0;
            tptr->instrvibratodepth = 0;
          }
        }
      }
      else
      {
        /* * Special case e90: starts the note at tick 0 but * doesn't restart envelope! */
        if ( !xm_tickcount )
        {
          startnewnote( tptr, chptr );
        }
      }
      tptr->retrigcount++;
    break;

      /* Notedelay */
    case 0xd:
      /* Don't start on tick 0 */
      if ( !xm_tickcount )
      break;
      if ( xm_tickcount == tptr->nybble2 )
      {
        startnewnote( tptr, chptr );
        if ( tptr->newinstrument ) changeinstrument( tptr );
        else
        {
          tptr->keyon = 1;
          tptr->fadeoutvalue = 32768;
          tptr->volenvpos = 0;
          tptr->panenvpos = 0;
          tptr->instrvibratophase = 0;
          tptr->instrvibratodepth = 0;
        }
        if ( ( tptr->voleffect >= 0x10 ) && ( tptr->voleffect <= 0x50 ) )
        {
          tptr->notevol = tptr->voleffect - 0x10;
          tptr->vol = tptr->notevol;
        }
        if ( ( tptr->voleffect >> 4 ) == 0xc )
        {
          tptr->notepanning = ( tptr->voleffect & 0xf ) << 4 | ( tptr->voleffect & 0xf );
        }
      }
    break;

      /* Cut note */
    case 0xc:
      if ( xm_tickcount == tptr->nybble2 ) tptr->notevol = 0;
    break;

      /* Fine volslide up */
    case 0xa:
      if ( !xm_tickcount )
      {
        if ( tptr->nybble2 ) tptr->volspeedup = tptr->nybble2;
        tptr->notevol += tptr->volspeedup;
        if ( tptr->notevol > 64 ) tptr->notevol = 64;
      }
    break;

      /* Fine volslide down */
    case 0xb:
      if ( !xm_tickcount )
      {
        if ( tptr->nybble2 ) tptr->volspeeddown = tptr->nybble2;
        tptr->notevol -= tptr->volspeeddown;
        if ( tptr->notevol < 0 ) tptr->notevol = 0;
      }
    break;

      /* Patterndelay */
    case 0xe:
      if ( !xm_tickcount )
      {
        xm_patterndelay = tptr->nybble2 + 1;
      }
    break;
  }
}

static void changeinstrument( TRACK * tptr )
{
  tptr->instrument = tptr->newinstrument - 1;
  if ( tptr->xsp )
  {
    tptr->notepanning = tptr->xsp->panning;
    tptr->notevol = tptr->xsp->vol;
    tptr->vol = tptr->notevol;
  }
  /* * Must NOT restart envelopes if there's a keyoff just in that place */
  if ( ( !tptr->newnote ) || ( tptr->note != KEYOFF - 1 ) )
  {
    tptr->keyon = 1;
    tptr->fadeoutvalue = 32768;
    tptr->volenvpos = 0;
    tptr->panenvpos = 0;
    tptr->instrvibratophase = 0;
    tptr->instrvibratodepth = 0;
  }
}

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

static void xmplayer_code_lock_end( void )
{
}
