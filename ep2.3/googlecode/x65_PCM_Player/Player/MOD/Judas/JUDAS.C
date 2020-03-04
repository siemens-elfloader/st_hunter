/*
 * JUDAS Apocalyptic Softwaremixing Sound System V2.0 by Faust
 *
 * Supported:
 * - SOUND BLASTER (8bit, mono, up to 22050 Hz)
 * - SOUND BLASTER PRO (8bit, stereo, up to 22050 Hz or mono up to 44100 Hz)
 * - SOUND BLASTER 16 (16bit, stereo, up to 44100 Hz)
 * - ULTRASOUND (16bit, stereo, up to 44100 Hz)
 * - XMs, MODs, S3Ms, WAVs, raw samples
 *
 * Other features:
 * - Clipping of sound output
 * - Optional interpolation (fast!)
 *
 * This is the main module, where soundcards & mixer are initialized. It's not
 * very good coding style, but who cares!
 */

#include "judasmem.h"
#include "judaserr.h"
#include "judastbl.h"
#include "judascfg.h"
#include "judas.h"
#include "JUDASTBL.H"

/*
 * Sound device numbers
 */
#define DEV_NOSOUND 0
#define DEV_SB 1
#define DEV_SBPRO 2
#define DEV_SB16 3
#define DEV_GUS 4

/*
 * Interrupt controller ports
 */
#define PICPORT1 0x20
#define PICPORT2 0xa0
#define PICMASK1 0x21
#define PICMASK2 0xa1

/*
 * Mixmode bits
 */
#define MONO 0
#define STEREO 1
#define EIGHTBIT 0
#define SIXTEENBIT 2

/*
 * Voicemode bits
 */
#define VM_OFF 0
#define VM_ON 1
#define VM_LOOP 2
#define VM_16BIT 4

/*
 * Sample & channel structures
 */
/*
typedef struct
{
        char *start;
        char *repeat;
        char *end;
        unsigned char voicemode;
} SAMPLE;
*/
/*
typedef struct
{
        volatile char *pos;
        char *repeat;
        char *end;
        SAMPLE *sample;
        unsigned freq;
        volatile unsigned short fractpos;
        signed char vol;
        unsigned char mastervol;
        unsigned char panning;
        volatile unsigned char voicemode;
} CHANNEL;
*/

/*
 * Prototypes
 */


/*
 * Assembler functions in JUDASASM.ASM
 */
/*
void judas_update(void);
void interrupt sb_handler(void);
void interrupt sb_aihandler(void);
void interrupt sb16_handler(void);
void interrupt gus_handler(void);
void gus_poke(unsigned location, unsigned char data);
unsigned char gus_peek(unsigned location);
void gus_startchannels(void);
void gus_dmaprogram(unsigned pos, unsigned length);
void gus_dmainit(void);
void gus_dmawait(void);
void mix(void *address, int length);
void normalmix(void);
void ipmix(void);
void judas_code_lock_start(void);
void judas_code_lock_end(void);
*/

/*
 * Inline function to get the value of DS
 */
unsigned short judas_get_ds(void);


/*
 * Variables
 */
int judas_error = JUDAS_OK;
void (*judas_player)(void) = 0;
unsigned judascfg_port = 0xffffffff;
unsigned judascfg_irq = 0xffffffff;
unsigned judascfg_dma1 = 0xffffffff;
unsigned judascfg_dma2 = 0xffffffff;
unsigned judas_irqcount = 0;
unsigned judas_device;
unsigned judas_port;
unsigned judas_irq;
unsigned judas_dma;
unsigned judas_int;
unsigned judas_mixpos;
unsigned judas_bufferlength;
unsigned judas_buffermask;
unsigned judas_bpmcount;
int *judas_clipbuffer;
short *judas_cliptable;
int *judas_volumetable;
unsigned short judas_ds;
static unsigned short dsp_version;
static unsigned char mixer_firsttime = 1;
static unsigned char judas_locked = 0;
static unsigned char judas_oldpicmask1;
static unsigned char judas_oldpicmask2;
unsigned char judas_initialized = 0;
unsigned char judas_mixmode;
unsigned char judas_bpmtempo;
unsigned char judas_samplesize;
CHANNEL judas_channel[CHANNELS];

/*
 * JUDAS device names
 */
char *judas_devname[] =
{
        "No Sound",
        "Sound Blaster",
        "Sound Blaster Pro",
        "Sound Blaster 16",
        "UltraSound",
};

/*
 * Mixmode names
 */
char *judas_mixmodename[] =
{
        "8-bit mono",
        "8-bit stereo",
        "16-bit mono",
        "16-bit stereo"
};

/*
 * Interpolation mode names
 */
char *judas_ipmodename[] =
{
        "normal",
        "interpolated"
};

/*
 * JUDAS error texts
 */
char *judas_errortext[] =
{
        "Everything OK",
        "Couldn't open file",
        "Couldn't read file",
        "Incorrect file format",
        "Out of memory",
        "Soundcard initialization failure",
        "Sound configuration incorrect",
        "Out of channels"
};

static unsigned char gus_dmalatch[] =
{
        0x40, 0x41, 0x40, 0x42, 0x40, 0x43, 0x44, 0x45
};

static unsigned char gus_irqlatch[] =
{
        0x40, 0x40, 0x40, 0x43, 0x40, 0x42, 0x40, 0x44,
        0x40, 0x41, 0x40, 0x45, 0x46, 0x40, 0x40, 0x47
};

/*
 * Fake sample returned by the routines when in NOSOUND mode
 */
SAMPLE fakesample = {0, 0, 0, VM_OFF};

int initmixer(void)
{
        int v, s, sv;

        /*
         * If this is the first time we are initializing, we must allocate the
         * lookup tables & clipbuffer and lock them as well. Volume table
         * needs to be calculated only once.
         */
        if (mixer_firsttime)
        {
                int *volptr;
                CHANNEL *chptr;

                judas_cliptable = locked_malloc(65536 * sizeof(short));
                if (!judas_cliptable)
                {
                        return 0;
                }
                judas_clipbuffer = locked_malloc((44100 / PER_SECOND) * 8);
                if (!judas_clipbuffer)
                {
                        locked_free(judas_cliptable);
                        return 0;
                }
                judas_volumetable = locked_malloc(256 * 256 * sizeof(int) + 1024);
                if (!judas_volumetable)
                {
                        locked_free(judas_cliptable);
                        locked_free(judas_clipbuffer);
                        return 0;
                }
                /*
                 * Adjust the volumetable to begin on a 1024 byte boundary;
                 * the mixing routines need this!
                 */
                judas_volumetable = (int *)((((unsigned)judas_volumetable) + 1023) & 0xfffffc00);
                volptr = &judas_volumetable[0];
                /*
                 * Note: although there is an optimized routine for zero volume,
                 * we need the zero volume table because in stereo mixing the
                 * other channel's volume could be zero.
                 */
                for (v = 0; v < 256; v++)
                {
                        for (s = 0; s < 256; s++)
                        {
                                sv = s;
                                if (sv > 127) sv -= 256;
                                sv *= v;
                                sv >>= (16 - SIGNIFICANT_BITS);
                                *volptr = sv;
                                volptr++;
                        }
                }
                /*
                 * The mixing routines need the address shifted, and since we
                 * don't need the pointer anymore...
                 */
                judas_volumetable = (int *)((unsigned)judas_volumetable >> 2);

                chptr = &judas_channel[0];

                /*
                 * Init all channels (no sound played, no sample, mastervolume
                 * 64)
                 */
                for (s = CHANNELS; s > 0; s--)
                {
                        chptr->voicemode = VM_OFF;
                        chptr->smp = 0;
                        chptr->mastervol = 64;
                        chptr++;
                }
                mixer_firsttime = 0;
        }

        if (judas_mixmode & SIXTEENBIT)
        {
                short *clipptr = &judas_cliptable[0];

                for (s = 0; s < 65536; s++)
                {
                        sv = s;
                        if (sv > 32767) sv -= 65536;
                        sv <<= (16 - SIGNIFICANT_BITS);
                        if (sv < -32768) sv = -32768;
                        if (sv > 32767) sv = 32767;
                        *clipptr = sv;
                        clipptr++;
                }
        }
        else
        {
                unsigned char *clipptr = (unsigned char *)&judas_cliptable[0];

                for (s = 0; s < 65536; s++)
                {
                        int sv = s;
                        if (sv > 32767) sv = s - 65536;
                        sv <<= (16 - SIGNIFICANT_BITS);
                        if (sv < -32768) sv = -32768;
                        if (sv > 32767) sv = 32767;
                        *clipptr = (sv >> 8) + 128;
                        clipptr++;
                }
        }
        return 1;
}


