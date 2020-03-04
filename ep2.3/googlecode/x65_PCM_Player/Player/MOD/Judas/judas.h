
/*
 * JUDAS V2.0 main header file
 */

/*
 * Mixmode bits
 */
#define MONO 0
#define STEREO 1
#define EIGHTBIT 0
#define SIXTEENBIT 2

/*
 * Sound device numbers
 */
#define DEV_NOSOUND 0
#define DEV_SB 1
#define DEV_SBPRO 2
#define DEV_SB16 3
#define DEV_GUS 4

/*
 * Voicemode bits
 */
#define VM_OFF 0
#define VM_ON 1
#define VM_ONESHOT 0
#define VM_LOOP 2
#define VM_16BIT 4

/*
 * Panning values (these specific panning positions are mixed with optimized
 * routines which greatly reduce processing time!)
 */
#define LEFT 0
#define MIDDLE 128
#define RIGHT 255

/*
 * Sample structure
 */
typedef struct
{
        signed char *start;
        signed char *repeat;
        signed char *end;
        unsigned char voicemode;
} SAMPLE;

/*
 * Channel structure
 */
typedef struct
{
        signed char *pos;            /* Position which is currently played */  //0        
        signed char *repeat;                   /* Goes here after hitting end *///4
        signed char *end;                      /* Position of last sample + 1 *///8
        SAMPLE *smp;                    /* Pointer to sample currently used */  //12
        unsigned long freq;                  /* Playing frequency in hertz */       // 16
        unsigned long fractpos; /* 16bit fractional pos */                               // 20
        unsigned long vol;                /* Note volume, ranges from 0 to 64 */  //24
        long mastervol;        /* 255 = whole volume range of output *///28
        unsigned char panning;          /* 0 left, 128 middle, 255 right */     //32
        unsigned long voicemode; /* See the bit definitions above */   //36
        signed int LastVal; //40
} CHANNEL;

/*
 * General functions
 */
void judas_config(void);
int judas_init(unsigned mixrate, unsigned mixmode, int interpolation);
void judas_update(void);
void judas_uninit(void);

/*
 * Sample & channel functions
 */
SAMPLE *judas_allocsample(int length);
void judas_ipcorrect(SAMPLE *smp);
void judas_freesample(SAMPLE *smp);
void judas_playsample(SAMPLE *smp, unsigned chnum, unsigned frequency, unsigned char volume, unsigned char panning);
void judas_stopsample(unsigned chnum);
void judas_preventdistortion(unsigned active_channels);
void judas_setmastervolume(unsigned chnum, unsigned char mastervol);
void judas_setmusicmastervolume(unsigned musicchannels, unsigned char mastervol);
void judas_setsfxmastervolume(unsigned musicchannels, unsigned char mastervol);

/*
 * RAW sample & WAV functions
 */
SAMPLE *judas_loadrawsample(char *name, int repeat, int end, unsigned char voicemode);
SAMPLE *judas_loadwav(char *name);

/*
 * XM player functions
 */
int judas_loadxm(char *name);
void judas_freexm(void);
void judas_playxm(void);
void judas_stopxm(void);
unsigned char judas_getxmpos(void);
unsigned char judas_getxmline(void);
unsigned char judas_getxmtick(void);
unsigned char judas_getxmchannels(void);
char *judas_getxmname(void);

/*
 * MOD player functions
 */
int judas_loadmod(char *name);
void judas_freemod(void);
void judas_playmod(void);
void judas_stopmod(void);
unsigned char judas_getmodpos(void);
unsigned char judas_getmodline(void);
unsigned char judas_getmodtick(void);
unsigned char judas_getmodchannels(void);
char *judas_getmodname(void);

/*
 * S3M player functions
 */
int judas_loads3m(char *name);
void judas_frees3m(void);
void judas_plays3m(void);
void judas_stops3m(void);
unsigned char judas_gets3mpos(void);
unsigned char judas_gets3mline(void);
unsigned char judas_gets3mtick(void);
unsigned char judas_gets3mchannels(void);
char *judas_gets3mname(void);

/*
 * JUDAS IO functions
 */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
int judas_open(char *name);
int judas_seek(int handle, int bytes, int whence);
int judas_read(int handle, void *buffer, int size);
void judas_close(int handle);

/*
 * DPMI memory locking functions (use them, and no more crashes under
 * multitaskers!)
 */
int judas_memlock(void *start, unsigned size);
int judas_memunlock(void *start, unsigned size);
void *locked_malloc(int size);
void locked_free(void *address);

/*
 * Sound configuration (may be changed anytime, used only by judas_init())
 */
extern unsigned judascfg_device;
extern unsigned judascfg_port;
extern unsigned judascfg_irq;
extern unsigned judascfg_dma1;
extern unsigned judascfg_dma2;          /* SB High DMA, not needed for GUS */

/*
 * Device, port, IRQ, DMA, mixrate & mixmode currently in use.
 * Don't change them!
 */
//extern unsigned judas_device;
//extern unsigned judas_port;
//extern unsigned judas_irq;
//extern unsigned judas_dma;
//extern unsigned judas_mixrate;
//extern unsigned char judas_mixmode;

/*
 * Sound device names
 */
extern char *judas_devname[];

/*
 * Mixmode names
 */
extern char *judas_mixmodename[];

/*
 * Interpolation mode names
 */
extern char *judas_ipmodename[];

/*
 * Error code
 */
extern int judas_error;

/*
 * Text strings for the error codes
 */
extern char *judas_errortext[];

/*
 * Channel array
 */
extern CHANNEL judas_channel[CHANNELS];

/*
 * Fake sample returned by the routines when in NOSOUND mode
 */
extern SAMPLE fakesample;
