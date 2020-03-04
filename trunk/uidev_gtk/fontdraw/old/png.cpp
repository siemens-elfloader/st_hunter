#include <alib\img.h>
#include <png.h>

void* xmalloc(int x,int n)
{
  return malloc(n);
}

void xmfree(int x,void* ptr)
{
  mfree(ptr);
}

void read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
  unsigned int err;
  int f;
  f=(int)png_get_io_ptr(png_ptr);
  _read(f, data, length, &err);
}

#define number 8
#define DEFAULT_COLOR 2
#define ALPHA_THRESHOLD 128
#define PNG_1 0xFF
#define PNG_8 1
#define PNG_16 2
#define PNG_24 3

int
AIMG::CreateFromPNG (char *fname){
    int type=0;
    int f;
    char buf[number];
    unsigned int err;
    struct PP{
        char *row;
        char *img;
    }pp;

    png_structp png_ptr=NULL;
    png_infop info_ptr=NULL;
    png_uint_32 rowbytes;

    if ((f=_open(fname, A_ReadOnly+A_BIN, P_READ, &err))==-1) return 0;
    pp.row=NULL;
    pp.img=NULL;

    if (_read(f, &buf, number, &err)!=number) goto L_CLOSE_FILE;
    if  (!png_check_sig((png_bytep)buf,number)) goto  L_CLOSE_FILE;

    png_ptr = png_create_read_struct_2("1.2.5", (png_voidp)0, 0, 0, (png_voidp)0,(png_malloc_ptr)xmalloc,(png_free_ptr)xmfree);
    if (!png_ptr) goto L_CLOSE_FILE;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        goto L_CLOSE_FILE;
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        goto L_CLOSE_FILE;
    }

    png_set_read_fn(png_ptr, (void *)f, read_data_fn);

    png_set_sig_bytes(png_ptr, number);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth, color_type;

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

    if (type==0)
    {
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

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    pp.row=(char*)malloc(rowbytes);

    if (type==PNG_1)
    {
        int rowc_w=(width+7)>>3;
        int size=height*rowc_w;
        unsigned char *iimg=(unsigned char *)(pp.img=(char*)malloc(size));
        zeromem(iimg,size);
        for (unsigned int y = 0; y<height; y++)
        {
            png_read_row(png_ptr, (png_bytep)pp.row, NULL);
            for (unsigned int x = 0; x<width; x++)
            {
                if (!pp.row[x*4+0] && !pp.row[x*4+1] && !pp.row[x*4+2]) iimg[x>>3]|=(0x80>>(x&7));
            }
            iimg+=rowc_w;
        }
        bpnum_=1;
    }
    else
    {
        switch (type)
        {
            case PNG_8:
            {
                unsigned char *iimg=(unsigned char *)(pp.img=(char*)malloc(width*height));
                for (unsigned int y = 0; y<height; y++)
                {
                    png_read_row(png_ptr, (png_bytep)pp.row, NULL);
                    for (unsigned int x = 0; x<width; x++)
                    {
                        if (pp.row[x*4+3]<ALPHA_THRESHOLD) *iimg++=0xC0;
                        else
                        {
                            unsigned char c=(pp.row[x*4+0] & 0xE0);
                            c|=((pp.row[x*4+1]>>3)&0x1C);
                            c|=((pp.row[x*4+2]>>6)&0x3);
                            *iimg++=c;
                        }
                    }
                }
                bpnum_=5;
                break;
            }
            case PNG_16:
            {
                unsigned short *iimg=(unsigned short *)(pp.img=(char*)malloc(width*height*2));
                for (unsigned int y = 0; y<height; y++)
                {
                    png_read_row(png_ptr, (png_bytep)pp.row, NULL);
                    for (unsigned int x = 0; x<width; x++)
                    {
                        if (pp.row[(x<<2)+3]<ALPHA_THRESHOLD) *iimg++=0xE000;
                        else
                        {
                            *iimg++=rgb_rgb16(pp.row[(x<<2)+0], pp.row[(x<<2)+1], pp.row[(x<<2)+2]);
                        }
                    }
                }
                bpnum_=T_16COL;
                break;
            }

            case PNG_24:
            {
                unsigned char *iimg=(unsigned char *)(pp.img=(char*)malloc((width*height)<<2));
                for (unsigned int y = 0; y<height; y++)
                {
                    png_read_row(png_ptr, (png_bytep)pp.row, NULL);
                    for (unsigned int x = 0; x<width; x++)
                    {
                        *iimg++=pp.row[x*4+2];
                        *iimg++=pp.row[x*4+1];
                        *iimg++=pp.row[x*4+0];

                        *iimg++=pp.row[x*4+3];
                    }
                }
                bpnum_=T_32COL;
                break;
            }
        }
    }
    w_=width;
    h_=height;

    bitmap_=(unsigned char*)pp.img;

    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

    if (!pp.img)
    {
    L_CLOSE_FILE:
        mfree(pp.row);
        mfree(pp.img);
        _close(f, &err);
        return -1;
    }

    mfree(pp.row);
    _close(f, &err);

    return 0;
}



int
AIMG::
SaveInPNG (const char *path){

    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int x, y;
    png_byte ** row_pointers = NULL;
    /* "status" contains the return value of this function. At first
       it is set to a value which means 'failure'. When the routine
       has finished its work, it is set to a value which means
       'success'. */
    int status = -1;
    /* The following number is set by trial and error only. I cannot
       see where it it is documented in the libpng manual.
    */
    int pixel_size = 3;
    int depth = 8;

    fp = fopen (path, "wb");
    if (! fp) {
        ShowMSG(1, (int)"Error opening file for writing");
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }

    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }

    /* Set up error handling. */

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }

    /* Set image attributes. */

    png_set_IHDR (png_ptr,
                  info_ptr,
                  w_,
                  h_,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */

    row_pointers = (png_byte**)png_malloc (png_ptr, h_ * sizeof (png_byte *));
    for (y = 0; y < h_; ++y) {
        png_byte *row = (png_byte*) png_malloc (png_ptr, sizeof (uint8_t) * w_ * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < w_; ++x) {
            color32_t pixel = GetColor (x, y);

            *row++ = GetR(pixel);
            *row++ = GetG(pixel);
            *row++ = GetB(pixel);
        }
    }

    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status = 0;

    for (y = 0; y < h_; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);


 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);

 png_create_write_struct_failed:
    fclose (fp);

 fopen_failed:
    //dlclose(dl);
    return status;
}





