#include <swilib.h>
#include <unistd.h>
#include <conf_loader.h>
#include <stdio.h>
#include <errno.h>
#include "xvid.h"

unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;
int my_csm_id=0;

typedef struct
{
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;

typedef struct
{
    GUI gui;
    WSHDR *ws;
} MAIN_GUI;

static void *dec_handle = NULL;
#define BUFFER_SIZE (2*1024*1024)
static int XDIM = 0;
static int YDIM = 0;
static int ARG_SAVEDECOUTPUT = 0;
static int ARG_SAVEMPEGSTREAM = 0;
static const int display_buffer_bytes = 0;
static int POSTPROC = 0;
static int BPP = 1;
static int CSP = XVID_CSP_I420;
#define MIN_USEFUL_BYTES 1
/*------------------------------------------------------------------------*/
/*--------------------- Создание цсм процесса и гуя ----------------------*/
/*------------------------------------------------------------------------*/

char clrWhite[]= {0xFF,0xFF,0xFF,0x64};
char clrBlack[]= {0x00,0x00,0x00,0x64};
int scr_w,scr_h;

/* init decoder before first run */
static int
dec_init(int use_assembler, int debug_level)
{
	int ret;

	xvid_gbl_init_t   xvid_gbl_init;
	xvid_dec_create_t xvid_dec_create;
	xvid_gbl_info_t   xvid_gbl_info;

	/* Reset the structure with zeros */
	memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init_t));
	memset(&xvid_dec_create, 0, sizeof(xvid_dec_create_t));
	memset(&xvid_gbl_info, 0, sizeof(xvid_gbl_info));

	/*------------------------------------------------------------------------
	 * Xvid core initialization
	 *----------------------------------------------------------------------*/

	xvid_gbl_info.version = XVID_VERSION;
	xvid_global(NULL, XVID_GBL_INFO, &xvid_gbl_info, NULL);

	if (xvid_gbl_info.build != NULL) {
		fprintf(stderr, "xvidcore build version: %s\n", xvid_gbl_info.build);
	}

	/* Version */
	xvid_gbl_init.version = XVID_VERSION;

	/* Assembly setting */
	if(use_assembler)
#ifdef ARCH_IS_IA64
		xvid_gbl_init.cpu_flags = XVID_CPU_FORCE | XVID_CPU_IA64;
#else
	xvid_gbl_init.cpu_flags = 0;
#endif
	else
		xvid_gbl_init.cpu_flags = XVID_CPU_FORCE;

	xvid_gbl_init.debug = debug_level;

	xvid_global(NULL, 0, &xvid_gbl_init, NULL);

	/*------------------------------------------------------------------------
	 * Xvid decoder initialization
	 *----------------------------------------------------------------------*/

	/* Version */
	xvid_dec_create.version = XVID_VERSION;

	/*
	 * Image dimensions -- set to 0, xvidcore will resize when ever it is
	 * needed
	 */
	xvid_dec_create.width = 0;
	xvid_dec_create.height = 0;

	xvid_dec_create.num_threads = 1;

	ret = xvid_decore(NULL, XVID_DEC_CREATE, &xvid_dec_create, NULL);

	dec_handle = xvid_dec_create.handle;

	return(ret);
}


/* decode one frame  */
static int
dec_main(unsigned char *istream,
		 unsigned char *ostream,
		 int istream_size,
		 xvid_dec_stats_t *xvid_dec_stats)
{

	int ret;

	xvid_dec_frame_t xvid_dec_frame;

	/* Reset all structures */
	memset(&xvid_dec_frame, 0, sizeof(xvid_dec_frame_t));
	memset(xvid_dec_stats, 0, sizeof(xvid_dec_stats_t));

	/* Set version */
	xvid_dec_frame.version = XVID_VERSION;
	xvid_dec_stats->version = XVID_VERSION;

	/* No general flags to set */
	if (POSTPROC == 1)
		xvid_dec_frame.general          = XVID_DEBLOCKY | XVID_DEBLOCKUV;
	else if (POSTPROC==2)
		xvid_dec_frame.general          = XVID_DEBLOCKY | XVID_DEBLOCKUV | XVID_DERINGY | XVID_DERINGUV;
	else
		xvid_dec_frame.general          = 0;

	/* Input stream */
	xvid_dec_frame.bitstream        = istream;
	xvid_dec_frame.length           = istream_size;

	/* Output frame structure */
	xvid_dec_frame.output.plane[0]  = ostream;
	xvid_dec_frame.output.stride[0] = XDIM*BPP;
	xvid_dec_frame.output.csp = CSP;

	ret = xvid_decore(dec_handle, XVID_DEC_DECODE, &xvid_dec_frame, xvid_dec_stats);

	return(ret);
}


void test()
{
    unsigned char *mp4_buffer = NULL;
	unsigned char *mp4_ptr    = NULL;
	unsigned char *out_buffer = NULL;

	int useful_bytes;
	int chunk;
	xvid_dec_stats_t xvid_dec_stats;

	double totaldectime;

	long totalsize;
	int status;

	int use_assembler = 0;
	int debug_level = 0;
	int filenr;
	int i;

	/* ======================= */

    FILE *in_file = fopen("0:\\Misc\\elf3\\test.avi", "rb");
    if (in_file == NULL) {
        fprintf(stderr, "Error opening input file %s\n", "0:\\Misc\\elf3\\test.avi");
        return(-1);
    }

    mp4_buffer = (unsigned char *) malloc(BUFFER_SIZE);
	mp4_ptr = mp4_buffer;
	if(!mp4_buffer)
        goto release_all;


	status = dec_init(use_assembler, debug_level);
	if (status) {
		fprintf(stderr,
				"Decore INIT problem, return value %d\n", status);
		goto release_all;
	}

	useful_bytes = (int) fread(mp4_buffer, 1, BUFFER_SIZE, in_file);

	totaldectime = 0;
	totalsize = 0;
	filenr = 0;
	mp4_ptr = mp4_buffer;
	chunk = 0;

	do {
		int used_bytes = 0;
		double dectime;

		/*
		 * If the buffer is half empty or there are no more bytes in it
		 * then fill it.
		 */
		if (mp4_ptr > mp4_buffer + BUFFER_SIZE/2) {
			int already_in_buffer = (int)(mp4_buffer + BUFFER_SIZE - mp4_ptr);

			/* Move data if needed */
			if (already_in_buffer > 0)
				memcpy(mp4_buffer, mp4_ptr, already_in_buffer);

			/* Update mp4_ptr */
			mp4_ptr = mp4_buffer;

			/* read new data */
            if(!feof(in_file)) {
				useful_bytes += (int) fread(mp4_buffer + already_in_buffer,
									        1, BUFFER_SIZE - already_in_buffer,
									        in_file);
			}
		}


		/* This loop is needed to handle VOL/NVOP reading */
		do {

			/* Decode frame */
			//dectime = msecond();
			used_bytes = dec_main(mp4_ptr, out_buffer, useful_bytes, &xvid_dec_stats);
			//dectime = msecond() - dectime;

			/* Resize image buffer if needed */
			if(xvid_dec_stats.type == XVID_TYPE_VOL) {

				/* Check if old buffer is smaller */
				if(XDIM*YDIM < xvid_dec_stats.data.vol.width*xvid_dec_stats.data.vol.height) {

					/* Copy new witdh and new height from the vol structure */
					XDIM = xvid_dec_stats.data.vol.width;
					YDIM = xvid_dec_stats.data.vol.height;

					/* Free old output buffer*/
					if(out_buffer) free(out_buffer);

					/* Allocate the new buffer */
					out_buffer = (unsigned char*)malloc(XDIM*YDIM*4);
					if(out_buffer == NULL)
						goto release_all;

					fprintf(stderr, "Resized frame buffer to %dx%d\n", XDIM, YDIM);
				}

			}

			/* Update buffer pointers */
			if(used_bytes > 0) {
				mp4_ptr += used_bytes;
				useful_bytes -= used_bytes;

				/* Total size */
				totalsize += used_bytes;
			}

			if (display_buffer_bytes) {
				printf("Data chunk %d: %d bytes consumed, %d bytes in buffer\n", chunk++, used_bytes, useful_bytes);
			}
		} while (xvid_dec_stats.type <= 0 && useful_bytes > MIN_USEFUL_BYTES);

		/* Check if there is a negative number of useful bytes left in buffer
		 * This means we went too far */
        if(useful_bytes < 0)
            break;

    	/* Updated data - Count only usefull decode time */
		totaldectime += dectime;


		if (!display_buffer_bytes) {
			//printf("Frame %5d: type = %s, dectime(ms) =%6.1f, length(bytes) =%7d\n",
			//		filenr, type2str(xvid_dec_stats.type), dectime, used_bytes);
		}


		/* Save output frame if required */
		/*if (ARG_SAVEDECOUTPUT) {
			sprintf(filename, "%sdec", filepath);

			if(write_image(filename, out_buffer, filenr)) {
				fprintf(stderr,
						"Error writing decoded frame %s\n",
						filename);
			}
		}*/

		filenr++;

	} while (useful_bytes>MIN_USEFUL_BYTES || !feof(in_file));

    //return;
release_all:
	free(out_buffer);
	free(mp4_buffer);

    ShowMSG(1, (int)"O-o");
}


static void OnRedraw(MAIN_GUI *data)
{
    DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0,clrBlack,clrWhite);

    wsprintf(data->ws, "%t", "Че");

    DrawString(data->ws, 0,28,scr_w-1,scr_h-1,FONT_SMALL,2+32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
}


static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
    data->ws = AllocWS(128);
    data->gui.state=1;

    test();
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    data->gui.state=0;
    FreeWS( data->ws );
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
    data->gui.state=2;
    DisableIDLETMR();
#ifdef ELKA
    DisableIconBar(1);
#endif
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    if (data->gui.state!=2) return;
    data->gui.state=1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
    if ((msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS))
    {
        switch(msg->gbsmsg->submess)
        {
        case RIGHT_SOFT:
            return (1);
        }
    }
    REDRAW();
    return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int method8(void)
{
    return(0);
}
int method9(void)
{
    return(0);
}

const void * const gui_methods[11]=
{
    (void *)OnRedraw,
    (void *)onCreate,
    (void *)onClose,
    (void *)onFocus,
    (void *)onUnfocus,
    (void *)OnKey,
    0,
    (void *)kill_data,
    (void *)method8,
    (void *)method9,
    0
};


const RECT Canvas= {0,0,0,0};
static void maincsm_oncreate(CSM_RAM *data)
{
    scr_w=ScreenW()-1;
    scr_h=ScreenH()-1;
    MAIN_CSM*csm=(MAIN_CSM*)data;
    MAIN_GUI *main_gui = malloc(sizeof(MAIN_GUI));
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(RECT *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    csm->csm.state=0;
    csm->csm.unk1=0;
    my_csm_id=csm->gui_id=CreateGUI(main_gui);
}

void ElfKiller(void)
{
    kill_elf();
}

static void maincsm_onclose(CSM_RAM *csm)
{
    SUBPROC((void *)ElfKiller);
      fclose(stdout);
    fclose(stderr);

}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    MAIN_CSM *csm=(MAIN_CSM*)data;
    if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
    {
        csm->csm.state=-3;
    }
    return(1);
}


static const struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
} MAINCSM =
{
    {
        maincsm_onmessage,
        maincsm_oncreate,
#ifdef NEWSGOLD
        0,
        0,
        0,
        0,
#endif
        maincsm_onclose,
        sizeof(MAIN_CSM),
        1,
        &minus11
    },
    {
        maincsm_name_body,
        NAMECSM_MAGIC1,
        NAMECSM_MAGIC2,
        0x0,
        139,
        0
    }
};

void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NatAlarm");
}


typedef struct
{
    char *d;
    int sz;
}_debug;



int main(char *exename, char *fname)
{
    char stdo[128];
    extern void *__ex;
    _debug *d = (_debug*)&__ex;
    getcwd(stdo, 128);
    strcat(stdo, "stdout.txt");
    freopen(stdo, "w+", stdout);

    getcwd(stdo, 128);
    strcat(stdo, "stderr.txt");
    freopen(stdo, "w+", stderr);

    //setvbuf(stdout, 0, _IOFBF, 512);
    //setvbuf(stderr, 0, _IOFBF, 512);

    printf("0x%X binsz: %d", d->d, d->sz);


    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();
    return 0;
}
