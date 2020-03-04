#include <alib/img.h>
#include <alib/io.h>

#include <jpeglib.h>


const char jpg_hdr[]={0xFF, 0xD8};
int
Image::
CreateFromJPEG (const char *fname, int num, int denom, int max_resolution){

    JSAMPARRAY buffer=0;
	unsigned char *row;

#ifdef MEM_BUFFER
	int size=GetFileSize ((char*)fname);

	if (size<=0) return ERR_OPEN_FILE;

	char *buf=new char [size];

	if (!buf) return ERR_ALLOC_MEM;

	FileReadToBuffer ((char*)fname, buf, size);

	if (memcmp((void *)buf,(void *)jpg_hdr,sizeof(jpg_hdr))) return ERR_BAD_HEADER;
#else
    FILE *fp=fopen (fname, "rb");

	if (!fp) return ERR_OPEN_FILE;
#endif

    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

#ifdef MEM_BUFFER
    jpeg_mem_src(&cinfo, (unsigned char*)buf, size);
#else
    jpeg_stdio_src(&cinfo, fp);
#endif
    jpeg_read_header(&cinfo, TRUE);

    //scale to max def
    int orig_w=cinfo.image_width;
    int orig_h=cinfo.image_height;
    int denom_adj=1;
    int max_side=max (orig_w, orig_h);

    while (max_side>max_resolution*denom_adj) denom_adj++;
    if (denom_adj!=1){
        if (denom>1) denom=denom*denom_adj;
        else{
            denom=denom_adj;
            if (num<=0) num=1;
        }
    }
    ///////////////////

    if (num>0 && denom>1){
        cinfo.scale_num = num;
        cinfo.scale_denom = denom;
        jpeg_calc_output_dimensions(&(cinfo));
    }

    //tuning
    cinfo.do_fancy_upsampling = FALSE;
    cinfo.do_block_smoothing = FALSE;
    cinfo.dct_method = JDCT_IFAST;
    cinfo.dither_mode = JDITHER_ORDERED;
    jpeg_start_decompress(&cinfo);

	if (Create (cinfo.output_width, cinfo.output_height, T_16COL)){
	    jpeg_destroy_decompress(&cinfo);
#ifdef MEM_BUFFER
	    if (buf) delete [] buf;
#endif
	    return ERR_ALLOC_BITMAP;
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

#ifdef MEM_BUFFER
    if (buf) delete [] buf;
#else
    fclose (fp);
#endif

    return 0;
}

int
Image::
SaveInJPEG (const char *filename, int quality){

    struct jpeg_compress_struct cinfo;

    struct jpeg_error_mgr jerr;

    FILE *outfile;
    JSAMPLE *scanline;
    int row_stride;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        return ERR_OPEN_FILE;
    }

    setvbuf(outfile, 0, _IOFBF, 4096);
    jpeg_stdio_dest(&cinfo, outfile);

    //Set parameters for compression
    cinfo.image_width = w_;
    cinfo.image_height = h_;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, quality, TRUE);

    //Start compressor
    jpeg_start_compress(&cinfo, TRUE);

    //Scanline bytes
    row_stride = w_ * 3;

    scanline = (JSAMPLE*)malloc( row_stride );

    for(int y = 0; y < h_; y++){
        for(int x = 0; x < w_; x++){
            color32_t pixel = GetColor (x, y);

            scanline[x*3+0] = GetR (pixel);
            scanline[x*3+1] = GetG (pixel);
            scanline[x*3+2] = GetB (pixel);
        }
        jpeg_write_scanlines(&cinfo, &scanline, 1);
    }

    free(scanline);

    jpeg_finish_compress(&cinfo);

    fclose(outfile);

    jpeg_destroy_compress(&cinfo);

    return 0;
}



