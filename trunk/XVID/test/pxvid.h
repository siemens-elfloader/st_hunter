
#ifndef __PXVID_H__
#define __PXVID_H__

#include <xvid.h>


#define USE_PNM 0
#define USE_TGA 1
#define USE_YUV 2

double msecond();
const char * type2str(int type);

int
dec_init(int use_assembler, int debug_level);

int
dec_main(unsigned char *istream,
		 unsigned char *ostream,
		 int istream_size,
		 xvid_dec_stats_t *xvid_dec_stats);

int
dec_stop();



int write_tga(char *filename, unsigned char *image);
int write_pnm(char *filename, unsigned char *image);
int write_yuv(char *filename, unsigned char *image);
int write_image(char *prefix, unsigned char *image, int filenr);

#endif
