
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "pxvid.h"

/*****************************************************************************
 *               Global vars in module and constants
 ****************************************************************************/

extern int FORMAT;
extern int XDIM;
extern int YDIM;
extern int ARG_SAVEDECOUTPUT;
extern int ARG_SAVEMPEGSTREAM ;
extern int CSP;
extern int BPP;
static int POSTPROC = 0;
static 	int ARG_THREADS = 0;
static char filepath[256] = "./";
extern void *dec_handle;


double
msecond()
{
    return 0;
	/*struct timeval  tv;
	gettimeofday(&tv, 0);
	return((double)tv.tv_sec*1.0e3 + (double)tv.tv_usec*1.0e-3);*/
}


const char * type2str(int type)
{
    if (type==XVID_TYPE_IVOP)
        return "I";
    if (type==XVID_TYPE_PVOP)
        return "P";
    if (type==XVID_TYPE_BVOP)
        return "B";
    return "S";
}

/* init decoder before first run */
int
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

	xvid_dec_create.num_threads = ARG_THREADS;

	ret = xvid_decore(NULL, XVID_DEC_CREATE, &xvid_dec_create, NULL);

	dec_handle = xvid_dec_create.handle;

	return(ret);
}

/* decode one frame  */
int
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


/* close decoder to release resources */
int
dec_stop()
{
	int ret;

	ret = xvid_decore(dec_handle, XVID_DEC_DESTROY, NULL, NULL);

	return(ret);
}





/*****************************************************************************
 *              output functions
 ****************************************************************************/
int write_image(char *prefix, unsigned char *image, int filenr)
{
	char filename[1024];
	char *ext;
	int ret;

	if (FORMAT == USE_PNM && BPP == 1) {
		ext = "pgm";
	} else if (FORMAT == USE_PNM && BPP == 3) {
		ext = "pnm";
	} else if (FORMAT == USE_YUV) {
		ext = "yuv";
	} else if (FORMAT == USE_TGA) {
		ext = "tga";
	} else {
		fprintf(stderr, "Bug: should not reach this path code -- please report to xvid-devel@xvid.org with command line options used");
		return -1;
	}

	if (FORMAT == USE_YUV) {
		sprintf(filename, "%s.%s", prefix, ext);

		if (!filenr) {
			FILE *fp = fopen(filename, "wb");
			fclose(fp);
		}
	} else
		sprintf(filename, "%s%05d.%s", prefix, filenr, ext);

	if (FORMAT == USE_PNM) {
		ret = write_pnm(filename, image);
	} else if (FORMAT == USE_YUV) {
		ret = write_yuv(filename, image);
	} else {
		ret = write_tga(filename, image);
	}

	return(ret);
}


int write_tga(char *filename, unsigned char *image)
{
	FILE * f;
	char hdr[18];

	f = fopen(filename, "wb");
	if ( f == NULL) {
		return -1;
	}

	hdr[0]  = 0; /* ID length */
	hdr[1]  = 0; /* Color map type */
	hdr[2]  = (BPP>1)?2:3; /* Uncompressed true color (2) or greymap (3) */
	hdr[3]  = 0; /* Color map specification (not used) */
	hdr[4]  = 0; /* Color map specification (not used) */
	hdr[5]  = 0; /* Color map specification (not used) */
	hdr[6]  = 0; /* Color map specification (not used) */
	hdr[7]  = 0; /* Color map specification (not used) */
	hdr[8]  = 0; /* LSB X origin */
	hdr[9]  = 0; /* MSB X origin */
	hdr[10] = 0; /* LSB Y origin */
	hdr[11] = 0; /* MSB Y origin */
	hdr[12] = (XDIM>>0)&0xff; /* LSB Width */
	hdr[13] = (XDIM>>8)&0xff; /* MSB Width */
	if (BPP > 1) {
		hdr[14] = (YDIM>>0)&0xff; /* LSB Height */
		hdr[15] = (YDIM>>8)&0xff; /* MSB Height */
	} else {
		hdr[14] = ((YDIM*3)>>1)&0xff; /* LSB Height */
		hdr[15] = ((YDIM*3)>>9)&0xff; /* MSB Height */
	}
	hdr[16] = BPP*8;
	hdr[17] = 0x00 | (1<<5) /* Up to down */ | (0<<4); /* Image descriptor */

	/* Write header */
	fwrite(hdr, 1, sizeof(hdr), f);

#ifdef ARCH_IS_LITTLE_ENDIAN
	/* write first plane */
	fwrite(image, 1, XDIM*YDIM*BPP, f);
#else
	{
		int i;
		for (i=0; i<XDIM*YDIM*BPP;i+=BPP) {
			if (BPP == 1) {
				fputc(*(image+i), f);
			} else if (BPP == 2) {
				fputc(*(image+i+1), f);
				fputc(*(image+i+0), f);
			} else if (BPP == 3) {
				fputc(*(image+i+2), f);
				fputc(*(image+i+1), f);
				fputc(*(image+i+0), f);
			} else if (BPP == 4) {
				fputc(*(image+i+3), f);
				fputc(*(image+i+2), f);
				fputc(*(image+i+1), f);
				fputc(*(image+i+0), f);
			}
		}
	}
#endif

	/* Write Y and V planes for YUV formats */
	if (BPP == 1) {
		int i;

		/* Write the two chrominance planes */
		for (i=0; i<YDIM/2; i++) {
			fwrite(image+XDIM*YDIM + i*XDIM/2, 1, XDIM/2, f);
			fwrite(image+5*XDIM*YDIM/4 + i*XDIM/2, 1, XDIM/2, f);
		}
	}


	/* Close the file */
	fclose(f);

	return(0);
}

int write_pnm(char *filename, unsigned char *image)
{
	FILE * f;

	f = fopen(filename, "wb");
	if ( f == NULL) {
		return -1;
	}

	if (BPP == 1) {
		int i;
		fprintf(f, "P5\n%i %i\n255\n", XDIM, YDIM*3/2);

		fwrite(image, 1, XDIM*YDIM, f);

		for (i=0; i<YDIM/2;i++) {
			fwrite(image+XDIM*YDIM + i*XDIM/2, 1, XDIM/2, f);
			fwrite(image+5*XDIM*YDIM/4 + i*XDIM/2, 1, XDIM/2, f);
		}
	} else if (BPP == 3) {
		int i;
		fprintf(f, "P6\n#xvid\n%i %i\n255\n", XDIM, YDIM);
		for (i=0; i<XDIM*YDIM*3; i+=3) {
#ifdef ARCH_IS_LITTLE_ENDIAN
			fputc(image[i+2], f);
			fputc(image[i+1], f);
			fputc(image[i+0], f);
#else
			fputc(image[i+0], f);
			fputc(image[i+1], f);
			fputc(image[i+2], f);
#endif
		}
	}

	fclose(f);

	return 0;
}


int write_yuv(char *filename, unsigned char *image)
{
	FILE * f;

	f = fopen(filename, "ab+");
	if ( f == NULL) {
		return -1;
	}

	fwrite(image, 1, 3*XDIM*YDIM/2, f);

	fclose(f);

	return 0;
}









