#include <alib\img.h>
#include <alib\io.h>


#ifdef WIN

#include "include/jpeglib.h"
#include "include/jerror.h"
#pragma comment(lib, "lib/jpeg.lib")

#else

#include <jpeglib.h>

#endif


const char jpg_hdr[]={0xFF, 0xD8};
int
AIMG::
CreateFromJPEG (char *fname, int num, int denom){

    JSAMPARRAY buffer=0;
	unsigned char *row;

	int size=GetFileSize (fname);

	if (size<=0) return -1;

	char *buf=new char [size];

	if (!buf) return -2;

	FileReadToBuffer (fname, buf, size);

    if (memcmp((void *)buf,(void *)jpg_hdr,sizeof(jpg_hdr))) return -3;


    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);


    jpeg_mem_src(&cinfo, (unsigned char*)buf, size);

    jpeg_read_header(&cinfo, TRUE);

    //cinfo.dct_method = JDCT_IFAST;

    if (num>0 && denom>1){
        cinfo.scale_num = num;
        cinfo.scale_denom = denom;
        jpeg_calc_output_dimensions(&(cinfo));
    }

    jpeg_start_decompress(&cinfo);

	if (!Create (cinfo.output_width, cinfo.output_height, T_16COL)){
	    jpeg_destroy_decompress(&cinfo);
	    if (buf) delete [] buf;
	    return -4;
    }

    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, w_*cinfo.output_components, 1);

    color16_t *bm=(color16_t*)bitmap_;
    while (cinfo.output_scanline < h_){
        JDIMENSION num_scanlines=jpeg_read_scanlines(&cinfo, buffer, 1);
        row=(unsigned char*)buffer[0];

        int line=(cinfo.output_scanline-1)*w_;
        for (int x=0; x<w_; x++){
            *(bm + x + line)=rgb_rgb16 (row[0], row[1], row[2]);
            row+=3;
        }
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    if (buf) delete [] buf;

    return 0;

}

int
AIMG::
SaveInJPEG (char *filename, int quality){


    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;
    /* This struct represents a JPEG error handler.  It is declared separately
     * because applications often want to supply a specialized error handler
     * (see the second half of this file for an example).  But here we just
     * take the easy way out and use the standard error handler, which will
     * print a message on stderr and call exit() if compression fails.
     * Note that this struct must live as long as the main JPEG parameter
     * struct, to avoid dangling-pointer problems.
     */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * outfile;		/* target file */
    JSAMPLE *scanline;	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
     * step fails.  (Unlikely, but it could happen if you are out of memory.)
     * This routine fills in the contents of struct jerr, and returns jerr's
     * address which we place into the link field in cinfo.
     */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
     * stdio stream.  You can also write your own code to do something else.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to write binary files.
     */
    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        ShowMSG(1, (int)"Error opening file for writing");

        return 0;
    }


    setvbuf(outfile, 0, _IOFBF, 4096);
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
     * Four fields of the cinfo struct must be filled in:
     */
    cinfo.image_width = w_; 	/* image width and height, in pixels */
    cinfo.image_height = h_;
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
     * (You must set at least cinfo.in_color_space before calling this,
     * since the defaults depend on the source color space.)
     */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
     * Here we just illustrate the use of quality (quantization table) scaling:
     */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
     * Pass TRUE unless you are very sure of what you're doing.
     */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
     * loop counter, so that we don't have to keep track ourselves.
     * To keep things simple, we pass one scanline per call; you can pass
     * more if you wish, though.
     */
    row_stride = w_ * 3;	/* JSAMPLEs per row in image_buffer */

    scanline = (JSAMPLE*)malloc( row_stride );

    for(int posy = 0; posy < h_; posy++)
    {
        for(int posx = 0; posx < w_; posx++)
        {
            color32_t pixel = GetColor (posx, posy);

            scanline[posx*3+0] = GetR (pixel);
            scanline[posx*3+1] = GetG (pixel);
            scanline[posx*3+2] = GetB (pixel);
        }
        jpeg_write_scanlines(&cinfo, &scanline, 1);
    }

    free(scanline);
    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    return 0;
}



