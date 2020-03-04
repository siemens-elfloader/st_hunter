
#include <swilib.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <png.h>
#include <de/drawing.h>
#include <de/pixtypes.h>
#include <dlfcn.h>


int save_png_to_file (image_t *bitmap, const char *path)
{
    void *dl = dlopen("libpng-12.so", 0);
    if(!dl) {
        ShowMSG(1, (int)"Can`t load png library!");
        return -1;
    }

    png_structp (*png_create_write_struct)(png_const_charp user_png_ver, png_voidp error_ptr,
                    png_error_ptr error_fn, png_error_ptr warn_fn) = dlsym(dl, "png_create_write_struct");
    if(!png_create_write_struct) goto err;

    png_infop (*png_create_info_struct)(png_structp) = dlsym(dl, "png_create_info_struct");
    if(!png_create_info_struct) goto err;

    void (*png_destroy_write_struct)(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr) = dlsym(dl, "png_destroy_write_struct");
    if(!png_destroy_write_struct) goto err;

    void (*png_set_IHDR)(png_structp png_ptr,
        png_infop info_ptr, png_uint_32 width, png_uint_32 height, int bit_depth,
        int color_type, int interlace_method, int compression_method,
        int filter_method) = dlsym(dl, "png_set_IHDR");
    if(!png_set_IHDR) goto err;

    png_voidp (*png_malloc)(png_structp png_ptr, png_uint_32 size) = dlsym(dl, "png_malloc");
    if(!png_malloc) goto err;

    void (*png_init_io)(png_structp png_ptr, FILE *) = dlsym(dl, "png_init_io");
    if(!png_init_io) goto err;

    void (*png_set_rows)(png_structp png_ptr,
        png_infop info_ptr, png_bytepp row_pointers) = dlsym(dl, "png_set_rows");
    if(!png_set_rows) goto err;

    void (*png_write_png)(png_structp png_ptr,
                        png_infop info_ptr,
                        int transforms,
                        png_voidp params) = dlsym(dl, "png_write_png");
    if(!png_write_png) goto err;

    void (*png_free)(png_structp png_ptr, png_voidp ptr) = dlsym(dl, "png_free");
    if(!png_free) goto err;

    goto success;
err:
    ShowMSG(1, (int)"Some png functions not found");
    dlclose(dl);
    return -1;

success:;


    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
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
                  bitmap->w,
                  bitmap->h,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */

    row_pointers = png_malloc (png_ptr, bitmap->h * sizeof (png_byte *));
    for (y = 0; y < bitmap->h; ++y) {
        png_byte *row = png_malloc (png_ptr, sizeof (uint8_t) * bitmap->w * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->w; ++x) {
            uint32_t pixel = pixel32 (bitmap, x, y);

            *row++ = pixel >> 16 & 0xff;
            *row++ = pixel >> 8 & 0xff;
            *row++ = pixel & 0xff;
        }
    }

    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status = 0;

    for (y = 0; y < bitmap->h; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);


 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);

 png_create_write_struct_failed:
    fclose (fp);

 fopen_failed:
    dlclose(dl);
    return status;
}


