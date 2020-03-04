#include <alib/img.h>
#include <png.h>

#define SIG_LEN 8
#define DEFAULT_COLOR 2
#define ALPHA_THRESHOLD 128
#define PNG_1 0xFF
#define PNG_8 1
#define PNG_16 2
#define PNG_24 3
//#define PNG_LIBPNG_VER_STRING "1.2.5"

int
Image::CreateFromPNG (const char *fname){
    int type=0;
    int error=0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    uint8_t sig[SIG_LEN];

    FILE *fp=fopen (fname, "rb");
    if (!fp) return ERR_OPEN_FILE;

    fread (sig, 1, SIG_LEN, fp);
    if (!png_check_sig(sig, SIG_LEN)){
        fclose (fp);
        return ERR_BAD_HEADER;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr){
        fclose (fp);
        return ERR_ALLOC_MEM;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose (fp);
        return ERR_ALLOC_MEM;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose (fp);
        return ERR_JUMP;
    }

    png_init_io(png_ptr, fp);

    png_set_sig_bytes(png_ptr, SIG_LEN);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth, color_type;

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

    if (type==0){
        if (color_type == PNG_COLOR_TYPE_GRAY) type=PNG_1;
        else type=DEFAULT_COLOR+1;
    }

    if (bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);

    if (bit_depth == 16) png_set_strip_16(png_ptr);

    if (bit_depth < 8) png_set_packing(png_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_GRAY) png_set_gray_to_rgb(png_ptr);

    png_set_filler(png_ptr,0xFF,PNG_FILLER_AFTER);
    png_read_update_info(png_ptr, info_ptr);

    png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    char *row=new char [rowbytes];

    if (type==PNG_1){
        error=ERR_UNSUPPORTED_BITMAP_TYPE;
        goto L_ERROR;
    }
    else{

        switch (type){

            case PNG_8:{
                error=ERR_UNSUPPORTED_BITMAP_TYPE;
                goto L_ERROR;
            }break;

            case PNG_16:{
                if (Create (width, height, T_16COL)){
                    error=ERR_ALLOC_BITMAP;
                    goto L_ERROR;
                }
                color16_t *bm=(color16_t*)bitmap_;
                for (unsigned int y = 0; y<height; y++){
                    png_read_row(png_ptr, (png_bytep)row, NULL);
                    int line=y*width;
                    for (unsigned int x = 0; x<width; x++){
                        if (row[x*4+3]<ALPHA_THRESHOLD) *(bm + x + line)=T16_PIXEL;
                        else{
                            *(bm + x + line)=rgb_rgb16 (row[x*4+0], row[x*4+1], row[x*4+2]);
                        }
                    }
                }

            }break;

            case PNG_24:{
                if (Create (width, height, T_32COL)){
                    error=ERR_ALLOC_BITMAP;
                    goto L_ERROR;
                }

                color32_t *bm=(color32_t*)bitmap_;
                for (unsigned int y = 0; y<height; y++){
                    png_read_row(png_ptr, (png_bytep)row, NULL);
                    int line=y*width;
                    for (unsigned int x = 0; x<width; x++){
                        *(bm + x + line)=rgb_rgb32 (row[x*4+0], row[x*4+1], row[x*4+2], row[x*4+3]);
                    }
                }

            }break;

            default:{
                error=ERR_UNSUPPORTED_BITMAP_TYPE;
                goto L_ERROR;
            }
        }
    }

    png_read_end(png_ptr, info_ptr);

L_ERROR:
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    if (row) delete [] row;
    fclose (fp);
    return error;
}

int
Image::
SaveInPNG (const char *path){

    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_byte ** row_pointers = NULL;

    int status = -1;

    int pixel_size = 3;
    int depth = 8;

    fp = fopen (path, "wb");
    if (!fp) {
        return ERR_OPEN_FILE;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }

    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }

    if (setjmp (png_jmpbuf (png_ptr))){
        goto png_failure;
    }

    //Set image attributes.
    png_set_IHDR (png_ptr,
                  info_ptr,
                  w_,
                  h_,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);

    row_pointers = (png_byte**)png_malloc (png_ptr, h_ * sizeof (png_byte *));
    for (int y = 0; y < h_; ++y){
        png_byte *row = (png_byte*) png_malloc (png_ptr, sizeof (uint8_t) * w_ * pixel_size);
        row_pointers[y] = row;
        for (int x = 0; x < w_; ++x){
            color32_t pixel = GetColor (x, y);

            *row++ = GetR(pixel);
            *row++ = GetG(pixel);
            *row++ = GetB(pixel);
        }
    }

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    status = 0;

    for (int y = 0; y < h_; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);

    png_failure:
    png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);

    png_create_write_struct_failed:
    fclose (fp);

    return status;
}





