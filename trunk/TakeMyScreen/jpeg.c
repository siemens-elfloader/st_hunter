




#include <swilib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <de/drawing.h>
#include <dlfcn.h>




int save_jpeg_to_file(image_t *img, char* filename, int quality)
{
    void *dl = dlopen("libjpeg-62.so", 0);
    if(!dl){
        ShowMSG(1, (int)"Can`t load jpeg library!");
        return -1;
    }

    struct jpeg_error_mgr * (*jpeg_std_error)(struct jpeg_error_mgr *) = dlsym(dl, "jpeg_std_error");
    if(!jpeg_std_error) goto err;

    void (*jpeg_CreateCompress)(j_compress_ptr cinfo,
				      int version, size_t structsize) = dlsym(dl, "jpeg_CreateCompress");
	if(!jpeg_CreateCompress) goto err;

	void (*jpeg_stdio_dest)(j_compress_ptr cinfo, FILE * outfile) = dlsym(dl, "jpeg_stdio_dest");
	if(!jpeg_stdio_dest) goto err;

	void (*jpeg_set_defaults)(j_compress_ptr cinfo) = dlsym(dl, "jpeg_set_defaults");
	if(!jpeg_set_defaults) goto err;

	void (*jpeg_set_quality)(j_compress_ptr cinfo, int quality,
				   boolean force_baseline) = dlsym(dl, "jpeg_set_quality");
	if(!jpeg_set_quality) goto err;

	void (*jpeg_start_compress)(j_compress_ptr cinfo,
				      boolean write_all_tables) = dlsym(dl, "jpeg_start_compress");
	if(!jpeg_start_compress) goto err;

	void (*jpeg_write_scanlines)(j_compress_ptr cinfo,
					     JSAMPARRAY scanlines,
					     JDIMENSION num_lines) = dlsym(dl, "jpeg_write_scanlines");
	if(!jpeg_write_scanlines) goto err;

	void (*jpeg_finish_compress)(j_compress_ptr cinfo) = dlsym(dl, "jpeg_finish_compress");
	if(!jpeg_finish_compress) goto err;

	void (*jpeg_destroy_compress)(j_compress_ptr cinfo) = dlsym(dl, "jpeg_destroy_compress");
	if(!jpeg_destroy_compress) goto err;

    goto success;

err:
    dlclose(dl);
    return -1;

success:;

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
        dlclose(dl);
        return 0;
    }


    setvbuf(outfile, 0, _IOFBF, 4096);
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
     * Four fields of the cinfo struct must be filled in:
     */
    cinfo.image_width = img->w; 	/* image width and height, in pixels */
    cinfo.image_height = img->h;
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
    row_stride = img->w * 3;	/* JSAMPLEs per row in image_buffer */
    uint32_t *bit = (uint32_t*)img->bitmap;
    scanline = malloc( row_stride );

    for(int posy = 0; posy < img->h; posy++)
    {
        for(int posx = 0; posx < img->w; posx++)
        {
            uint32_t pixel = pixel32 (img, posx, posy);

            scanline[posx*3+0] = pixel >> 16 & 0xff;
            scanline[posx*3+1] = pixel >> 8 & 0xff;
            scanline[posx*3+2] = pixel & 0xff;
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

    dlclose(dl);
    return 0;
}











