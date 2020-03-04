



typedef unsigned int size_t;
typedef long time_t;


typedef int jmp_buf[11];

























































































































































 


























































































 









 




 







 























































 






 

 


 

 


 

 






 

 




























 





 

 





 





 




 

 





 












 

                         







 





typedef unsigned char  Byte;   
typedef unsigned int   uInt;   
typedef unsigned long  uLong;  

   typedef Byte   Bytef;
typedef char   charf;
typedef int    intf;
typedef uInt   uIntf;
typedef uLong  uLongf;

   typedef void const *voidpc;
   typedef void    *voidpf;
   typedef void       *voidp;




 




































 

typedef voidpf (*alloc_func) (voidpf opaque, uInt items, uInt size);
typedef void   (*free_func)  (voidpf opaque, voidpf address);

struct internal_state;

typedef struct z_stream_s {
    Bytef    *next_in;   
    uInt     avail_in;   
    uLong    total_in;   

    Bytef    *next_out;  
    uInt     avail_out;  
    uLong    total_out;  

    char     *msg;       
    struct internal_state  *state;  

    alloc_func zalloc;   
    free_func  zfree;    
    voidpf     opaque;   

    int     data_type;   
    uLong   adler;       
    uLong   reserved;    
} z_stream;

typedef z_stream  *z_streamp;




 
typedef struct gz_header_s {
    int     text;        
    uLong   time;        
    int     xflags;      
    int     os;          
    Bytef   *extra;      
    uInt    extra_len;   
    uInt    extra_max;   
    Bytef   *name;       
    uInt    name_max;    
    Bytef   *comment;    
    uInt    comm_max;    
    int     hcrc;        
    int     done;       
 
} gz_header;

typedef gz_header  *gz_headerp;































 

                         

 



 

 

 

 

 


 

                         

extern const char *  zlibVersion (void);




 





















 


extern int  deflate (z_streamp strm, int flush);


















































































 


extern int  deflateEnd (z_streamp strm);










 




















 


































































































 











 

                         



 


























































 

extern int  deflateSetDictionary (z_streamp strm, const Bytef *dictionary, uInt dictLength);


































 

extern int  deflateCopy (z_streamp dest, z_streamp source);














 

extern int  deflateReset (z_streamp strm);








 

extern int  deflateParams (z_streamp strm, int level, int strategy);
















 

extern int  deflateTune (z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain);










 

extern uLong  deflateBound (z_streamp strm, uLong sourceLen);





 

extern int  deflatePrime (z_streamp strm, int bits, int value);











 

extern int  deflateSetHeader (z_streamp strm, gz_headerp head);




















 










































 

extern int  inflateSetDictionary (z_streamp strm, const Bytef *dictionary, uInt dictLength);

















 

extern int  inflateSync (z_streamp strm);












 

extern int  inflateCopy (z_streamp dest, z_streamp source);












 

extern int  inflateReset (z_streamp strm);







 

extern int  inflatePrime (z_streamp strm, int bits, int value);











 

extern int  inflateGetHeader (z_streamp strm, gz_headerp head);




































 





















 

typedef unsigned (*in_func) (void *, unsigned char * *);
typedef int (*out_func) (void *, unsigned char *, unsigned);

extern int  inflateBack (z_streamp strm, in_func in, void *in_desc, out_func out, void *out_desc);
































































 

extern int  inflateBackEnd (z_streamp strm);





 

extern uLong  zlibCompileFlags (void);






































 


                         







 

extern int  compress (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);











 

extern int  compress2 (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level);











 

extern uLong  compressBound (uLong sourceLen);




 

extern int  uncompress (Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);














 


typedef voidp gzFile;

extern gzFile  gzopen  (const char *path, const char *mode);














 

extern gzFile  gzdopen  (int fd, const char *mode);










 

extern int  gzsetparams (gzFile file, int level, int strategy);





 

extern int     gzread  (gzFile file, voidp buf, unsigned len);





 

extern int     gzwrite (gzFile file, voidpc buf, unsigned len);




 

extern int    gzprintf (gzFile file, const char *format, ...);










 

extern int  gzputs (gzFile file, const char *s);




 

extern char *  gzgets (gzFile file, char *buf, int len);






 

extern int     gzputc (gzFile file, int c);



 

extern int     gzgetc (gzFile file);



 

extern int     gzungetc (int c, gzFile file);







 

extern int     gzflush (gzFile file, int flush);







 

extern long     gzseek (gzFile file, long offset, int whence);














 

extern int     gzrewind (gzFile file);




 

extern long     gztell (gzFile file);






 

extern int  gzeof (gzFile file);



 

extern int  gzdirect (gzFile file);



 

extern int     gzclose (gzFile file);




 

extern const char *  gzerror (gzFile file, int *errnum);






 

extern void  gzclearerr (gzFile file);




 

                         





 

extern uLong  adler32 (uLong adler, const Bytef *buf, uInt len);













 

extern uLong  adler32_combine (uLong adler1, uLong adler2, long len2);





 

extern uLong  crc32   (uLong crc, const Bytef *buf, uInt len);













 

extern uLong  crc32_combine (uLong crc1, uLong crc2, long len2);







 


                         



 
extern int  deflateInit_ (z_streamp strm, int level, const char *version, int stream_size);
extern int  inflateInit_ (z_streamp strm, const char *version, int stream_size);
extern int  deflateInit2_ (z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char *version, int stream_size);

 
extern int  inflateBackInit_ (z_streamp strm, int windowBits, unsigned char *window, const char *version, int stream_size);


    struct internal_state {int dummy;};  

extern const char   *  zError           (int);
extern int             inflateSyncPoint (z_streamp z);
extern const uLongf *  get_crc_table    (void);



 








 





 






 

 




















 




 












 


 


 



 









 

































 











 










 






 

 





 


    





 




 














 

 









 






 







 


 





               




 








 




















 


 





 


 



 

 

 






 

 
 




 

 


 

 



 





 












 

 
      









 

typedef unsigned long png_uint_32;
typedef long png_int_32;
typedef unsigned short png_uint_16;
typedef short png_int_16;
typedef unsigned char png_byte;


 
   typedef size_t png_size_t;








 


 





 

 

 

 


 
typedef png_int_32 png_fixed_point;

 
typedef void             * png_voidp;
typedef png_byte         * png_bytep;
typedef png_uint_32      * png_uint_32p;
typedef png_int_32       * png_int_32p;
typedef png_uint_16      * png_uint_16p;
typedef png_int_16       * png_int_16p;
typedef const char   * png_const_charp;
typedef char             * png_charp;
typedef png_fixed_point  * png_fixed_point_p;


typedef double           * png_doublep;

 
typedef png_byte         *  * png_bytepp;
typedef png_uint_32      *  * png_uint_32pp;
typedef png_int_32       *  * png_int_32pp;
typedef png_uint_16      *  * png_uint_16pp;
typedef png_int_16       *  * png_int_16pp;
typedef const char   *  * png_const_charpp;
typedef char             *  * png_charpp;
typedef png_fixed_point  *  * png_fixed_point_pp;
typedef double           *  * png_doublepp;

 
typedef char             *  *  * png_charppp;

 
 



 
typedef charf *         png_zcharp;
typedef charf *  *   png_zcharpp;
typedef z_stream  *  png_zstreamp;

















 




 




 







 










 



 



 

 

 



 









 



 





 


 


 
extern  const char png_libpng_ver[18];
   

 
 
extern  const int png_pass_start[7];
extern  const int png_pass_inc[7];
extern  const int png_pass_ystart[7];
extern  const int png_pass_yinc[7];
extern  const int png_pass_mask[7];
extern  const int png_pass_dsp_mask[7];


 





 
typedef struct png_color_struct
{
   png_byte red;
   png_byte green;
   png_byte blue;
} png_color;
typedef png_color  * png_colorp;
typedef png_color  *  * png_colorpp;

typedef struct png_color_16_struct
{
   png_byte index;     
   png_uint_16 red;    
   png_uint_16 green;
   png_uint_16 blue;
   png_uint_16 gray;   
} png_color_16;
typedef png_color_16  * png_color_16p;
typedef png_color_16  *  * png_color_16pp;

typedef struct png_color_8_struct
{
   png_byte red;    
   png_byte green;
   png_byte blue;
   png_byte gray;   
   png_byte alpha;  
} png_color_8;
typedef png_color_8  * png_color_8p;
typedef png_color_8  *  * png_color_8pp;




 
typedef struct png_sPLT_entry_struct
{
   png_uint_16 red;
   png_uint_16 green;
   png_uint_16 blue;
   png_uint_16 alpha;
   png_uint_16 frequency;
} png_sPLT_entry;
typedef png_sPLT_entry  * png_sPLT_entryp;
typedef png_sPLT_entry  *  * png_sPLT_entrypp;




 

typedef struct png_sPLT_struct
{
   png_charp name;            
   png_byte depth;            
   png_sPLT_entryp entries;   
   png_int_32 nentries;       
} png_sPLT_t;
typedef png_sPLT_t  * png_sPLT_tp;
typedef png_sPLT_t  *  * png_sPLT_tpp;








 
typedef struct png_text_struct
{
   int  compression;       



 
   png_charp key;           
   png_charp text;         
 
   png_size_t text_length;  
} png_text;
typedef png_text  * png_textp;
typedef png_text  *  * png_textpp;


 






 
typedef struct png_time_struct
{
   png_uint_16 year;  
   png_byte month;    
   png_byte day;      
   png_byte hour;     
   png_byte minute;   
   png_byte second;   
} png_time;
typedef png_time  * png_timep;
typedef png_time  *  * png_timepp;





 
typedef struct png_unknown_chunk_t
{
    png_byte name[5];
    png_byte *data;
    png_size_t size;

     
    png_byte location;  
}
png_unknown_chunk;
typedef png_unknown_chunk  * png_unknown_chunkp;
typedef png_unknown_chunk  *  * png_unknown_chunkpp;







































 
typedef struct png_info_struct
{
    
   png_uint_32 width;        
   png_uint_32 height;       
   png_uint_32 valid;        
   png_uint_32 rowbytes;     
   png_colorp palette;       
   png_uint_16 num_palette;  
   png_uint_16 num_trans;    
   png_byte bit_depth;       
   png_byte color_type;      
    
   png_byte compression_type;  
   png_byte filter_type;     
   png_byte interlace_type;  

    
   png_byte channels;        
   png_byte pixel_depth;     
   png_byte spare_byte;      
   png_byte signature[8];    

   



 

   


 
   float gamma;  

     
     
   png_byte srgb_intent;  

   






 
   int num_text;  
   int max_text;  
   png_textp text;  

   

 
   png_time mod_time;

   




 
   png_color_8 sig_bit;  

   







 
   png_bytep trans;  
   png_color_16 trans_values;  

   




 
   png_color_16 background;

   



 
   png_int_32 x_offset;  
   png_int_32 y_offset;  
   png_byte offset_unit_type;  

   


 
   png_uint_32 x_pixels_per_unit;  
   png_uint_32 y_pixels_per_unit;  
   png_byte phys_unit_type;  

   




 
   png_uint_16p hist;

   




 
   float x_white;
   float y_white;
   float x_red;
   float y_red;
   float x_green;
   float y_green;
   float x_blue;
   float y_blue;

   









 
   png_charp pcal_purpose;   
   png_int_32 pcal_X0;       
   png_int_32 pcal_X1;       
   png_charp pcal_units;     
   png_charpp pcal_params;   
   png_byte pcal_type;       
   png_byte pcal_nparams;    

 
   png_uint_32 free_me;      

    
   png_unknown_chunkp unknown_chunks;
   png_size_t unknown_chunks_num;

    
   png_charp iccp_name;      
   png_charp iccp_profile;   
                             
   png_uint_32 iccp_proflen;   
   png_byte iccp_compression;  

    
   png_sPLT_tp splt_palettes;
   png_uint_32 splt_palettes_num;

   





 
   png_byte scal_unit;          
   double scal_pixel_width;     
   double scal_pixel_height;    
   png_charp scal_s_width;      
   png_charp scal_s_height;     

    
    
   png_bytepp row_pointers;         

   png_fixed_point int_gamma;  

   png_fixed_point int_x_white;
   png_fixed_point int_y_white;
   png_fixed_point int_x_red;
   png_fixed_point int_y_red;
   png_fixed_point int_x_green;
   png_fixed_point int_y_green;
   png_fixed_point int_x_blue;
   png_fixed_point int_y_blue;

} png_info;

typedef png_info  * png_infop;
typedef png_info  *  * png_infopp;

 
 

 
 

 
 

 

 

 

 

 

 

 

 

 

 





 




 
typedef struct png_row_info_struct
{
   png_uint_32 width;  
   png_uint_32 rowbytes;  
   png_byte color_type;  
   png_byte bit_depth;  
   png_byte channels;  
   png_byte pixel_depth;  
} png_row_info;

typedef png_row_info  * png_row_infop;
typedef png_row_info  *  * png_row_infopp;






 
typedef struct png_struct_def png_struct;
typedef png_struct  * png_structp;

typedef void ( *png_error_ptr) (png_structp, png_const_charp);
typedef void ( *png_rw_ptr) (png_structp, png_bytep, png_size_t);
typedef void ( *png_flush_ptr) (png_structp);
typedef void ( *png_read_status_ptr) (png_structp, png_uint_32, int);
typedef void ( *png_write_status_ptr) (png_structp, png_uint_32, int);

typedef void ( *png_progressive_info_ptr) (png_structp, png_infop);
typedef void ( *png_progressive_end_ptr) (png_structp, png_infop);
typedef void ( *png_progressive_row_ptr) (png_structp, png_bytep, png_uint_32, int);

typedef void ( *png_user_transform_ptr) (png_structp, png_row_infop, png_bytep);

typedef int ( *png_user_chunk_ptr) (png_structp, png_unknown_chunkp);
typedef void ( *png_unknown_chunk_ptr) (png_structp);

 

 

typedef png_voidp (*png_malloc_ptr) (png_structp, png_size_t);
typedef void (*png_free_ptr) (png_structp, png_voidp);






 

struct png_struct_def
{
   jmp_buf jmpbuf;             
   png_error_ptr error_fn;     
   png_error_ptr warning_fn;   
   png_voidp error_ptr;        
   png_rw_ptr write_data_fn;   
   png_rw_ptr read_data_fn;    
   png_voidp io_ptr;           

   png_user_transform_ptr read_user_transform_fn;  

   png_user_transform_ptr write_user_transform_fn;  

 
   png_voidp user_transform_ptr;  
   png_byte user_transform_depth;     
   png_byte user_transform_channels;  

   png_uint_32 mode;           
   png_uint_32 flags;          
   png_uint_32 transformations;  

   z_stream zstream;           
   png_bytep zbuf;             
   png_size_t zbuf_size;       
   int zlib_level;             
   int zlib_method;            
   int zlib_window_bits;       
   int zlib_mem_level;         
   int zlib_strategy;          

   png_uint_32 width;          
   png_uint_32 height;         
   png_uint_32 num_rows;       
   png_uint_32 usr_width;      
   png_uint_32 rowbytes;       
   png_uint_32 irowbytes;      
   png_uint_32 iwidth;         
   png_uint_32 row_number;     
   png_bytep prev_row;         
   png_bytep row_buf;          
   png_bytep sub_row;          
   png_bytep up_row;           
   png_bytep avg_row;          
   png_bytep paeth_row;        
   png_row_info row_info;      

   png_uint_32 idat_size;      
   png_uint_32 crc;            
   png_colorp palette;         
   png_uint_16 num_palette;    
   png_uint_16 num_trans;      
   png_byte chunk_name[5];     
   png_byte compression;       
   png_byte filter;            
   png_byte interlaced;        
   png_byte pass;              
   png_byte do_filter;         
   png_byte color_type;        
   png_byte bit_depth;         
   png_byte usr_bit_depth;     
   png_byte pixel_depth;       
   png_byte channels;          
   png_byte usr_channels;      
   png_byte sig_bytes;         

   png_uint_16 filler;            

   png_byte background_gamma_type;
   float background_gamma;
   png_color_16 background;    
   png_color_16 background_1;  

   png_flush_ptr output_flush_fn; 
   png_uint_32 flush_dist;     
   png_uint_32 flush_rows;     

   int gamma_shift;       
   float gamma;           
   float screen_gamma;    

   png_bytep gamma_table;      
   png_bytep gamma_from_1;     
   png_bytep gamma_to_1;       
   png_uint_16pp gamma_16_table;  
   png_uint_16pp gamma_16_from_1;  
   png_uint_16pp gamma_16_to_1;  

   png_color_8 sig_bit;        

   png_color_8 shift;          

   png_bytep trans;            
   png_color_16 trans_values;  

   png_read_status_ptr read_row_fn;    
   png_write_status_ptr write_row_fn;  
   png_progressive_info_ptr info_fn;  
   png_progressive_row_ptr row_fn;    
   png_progressive_end_ptr end_fn;    
   png_bytep save_buffer_ptr;         
   png_bytep save_buffer;             
   png_bytep current_buffer_ptr;      
   png_bytep current_buffer;          
   png_uint_32 push_length;           
   png_uint_32 skip_length;           
   png_size_t save_buffer_size;       
   png_size_t save_buffer_max;        
   png_size_t buffer_size;            
   png_size_t current_buffer_size;    
   int process_mode;                  
   int cur_palette;                   

     png_size_t current_text_size;    
     png_size_t current_text_left;    
     png_charp current_text;          
     png_charp current_text_ptr;      



   png_bytep palette_lookup;          
   png_bytep dither_index;            

   png_uint_16p hist;                 

   png_byte heuristic_method;         
   png_byte num_prev_filters;         
   png_bytep prev_filters;            
   png_uint_16p filter_weights;       
   png_uint_16p inv_filter_weights;   
   png_uint_16p filter_costs;         
   png_uint_16p inv_filter_costs;     


 

   png_uint_32 free_me;        

   png_voidp user_chunk_ptr;
   png_user_chunk_ptr read_user_chunk_fn;  

   int num_chunk_list;
   png_bytep chunk_list;

 
   png_byte rgb_to_gray_status;
    
   png_uint_16 rgb_to_gray_red_coeff;
   png_uint_16 rgb_to_gray_green_coeff;
   png_uint_16 rgb_to_gray_blue_coeff;

 
 
   png_uint_32 mng_features_permitted;

 
   png_fixed_point int_gamma;

 
   png_byte filter_type;


 
   png_byte     mmx_bitdepth_threshold;
   png_uint_32  mmx_rowbytes_threshold;
   png_uint_32  asm_flags;

 
   png_voidp mem_ptr;                 
   png_malloc_ptr malloc_fn;          
   png_free_ptr free_fn;              

 
   png_bytep big_row_buf;          

 
   png_bytep dither_sort;             
   png_bytep index_to_palette;        
                                      
   png_bytep palette_to_index;        
                                      

 
   png_byte compression_type;

   png_uint_32 user_width_max;
   png_uint_32 user_height_max;

};




 
typedef png_structp version_1_2_12;

typedef png_struct  *  * png_structpp;





 

 
extern  png_uint_32  png_access_version_number (void);



 







 
extern  int  png_sig_cmp (png_bytep sig, png_size_t start, png_size_t num_to_check);



 


 




 
extern  png_structp  png_create_write_struct
   (png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn);

extern  png_uint_32  png_get_compression_buffer_size
   (png_structp png_ptr);

extern  void  png_set_compression_buffer_size
   (png_structp png_ptr, png_uint_32 size);

 
extern  int  png_reset_zstream (png_structp png_ptr);

 




extern  png_structp  png_create_write_struct_2
   (png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn);

 
extern  void  png_write_chunk (png_structp png_ptr, png_bytep chunk_name, png_bytep data, png_size_t length);

 
extern  void  png_write_chunk_start (png_structp png_ptr, png_bytep chunk_name, png_uint_32 length);

 
extern  void  png_write_chunk_data (png_structp png_ptr, png_bytep data, png_size_t length);

 
extern  void  png_write_chunk_end (png_structp png_ptr);

 



 
extern  void  png_info_init (png_infop info_ptr);

extern  void  png_info_init_3 (png_infopp info_ptr, png_size_t png_info_struct_size);

 
extern  void  png_write_info_before_PLTE (png_structp png_ptr, png_infop info_ptr);
extern  void  png_write_info (png_structp png_ptr, png_infop info_ptr);

 




 
 
#pragma diag_suppress=Pe231
extern  void  png_convert_from_struct_tm (png_timep ptime, struct tm * ttime);
#pragma diag_default=Pe231

 
extern  void  png_convert_from_time_t (png_timep ptime, time_t ttime);

 
extern  void  png_set_expand (png_structp png_ptr);
extern  void  png_set_expand_gray_1_2_4_to_8 (png_structp png_ptr);


 


 
extern  void  png_set_bgr (png_structp png_ptr);

 


 
extern  void  png_set_rgb_to_gray (png_structp png_ptr, int error_action, double red, double green );
extern  void  png_set_rgb_to_gray_fixed (png_structp png_ptr, int error_action, png_fixed_point red, png_fixed_point green );
extern  png_byte  png_get_rgb_to_gray_status (png_structp png_ptr);

extern  void  png_build_grayscale_palette (int bit_depth, png_colorp palette);

extern  void  png_set_strip_alpha (png_structp png_ptr);

extern  void  png_set_swap_alpha (png_structp png_ptr);

extern  void  png_set_invert_alpha (png_structp png_ptr);

 


 
 
extern  void  png_set_add_alpha (png_structp png_ptr, png_uint_32 filler, int flags);

 
extern  void  png_set_swap (png_structp png_ptr);

 


 
extern  void  png_set_packswap (png_structp png_ptr);

 
extern  void  png_set_shift (png_structp png_ptr, png_color_8p true_bits);

 
extern  int  png_set_interlace_handling (png_structp png_ptr);

 


 
extern  void  png_set_background (png_structp png_ptr, png_color_16p background_color, int background_gamma_code, int need_expand, double background_gamma);

 


 
extern  void  png_set_dither (png_structp png_ptr, png_colorp palette, int num_palette, int maximum_colors, png_uint_16p histogram, int full_dither);

 
extern  void  png_set_gamma (png_structp png_ptr, double screen_gamma, double default_file_gamma);

 
 
extern  void  png_permit_empty_plte (png_structp png_ptr, int empty_plte_permitted);

 
extern  void  png_set_flush (png_structp png_ptr, int nrows);
 
extern  void  png_write_flush (png_structp png_ptr);

 
extern  void  png_start_read_image (png_structp png_ptr);

 



 
extern  void  png_read_rows (png_structp png_ptr, png_bytepp row, png_bytepp display_row, png_uint_32 num_rows);

 




 



 
extern  void  png_write_row (png_structp png_ptr, png_bytep row);

 
extern  void  png_write_rows (png_structp png_ptr, png_bytepp row, png_uint_32 num_rows);

 
extern  void  png_write_image (png_structp png_ptr, png_bytepp image);

 
extern  void  png_write_end (png_structp png_ptr, png_infop info_ptr);

 



 
extern  void  png_destroy_info_struct (png_structp png_ptr, png_infopp info_ptr_ptr);

 



 
extern void png_read_destroy (png_structp png_ptr, png_infop info_ptr, png_infop end_info_ptr);

 
extern  void  png_destroy_write_struct
   (png_structpp png_ptr_ptr, png_infopp info_ptr_ptr);

 
extern void png_write_destroy (png_structp png_ptr);

 
extern  void  png_set_crc_action (png_structp png_ptr, int crit_action, int ancil_action);









 







 



 
extern  void  png_set_filter (png_structp png_ptr, int method, int filters);





 



 




























 
extern  void  png_set_filter_heuristics (png_structp png_ptr, int heuristic_method, int num_weights, png_doublep filter_weights, png_doublep filter_costs);



 







 
extern  void  png_set_compression_level (png_structp png_ptr, int level);

extern  void  png_set_compression_mem_level
   (png_structp png_ptr, int mem_level);

extern  void  png_set_compression_strategy
   (png_structp png_ptr, int strategy);

extern  void  png_set_compression_window_bits
   (png_structp png_ptr, int window_bits);

extern  void  png_set_compression_method (png_structp png_ptr, int method);








 








 

extern  void  png_set_error_fn (png_structp png_ptr, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warning_fn);

 
extern  png_voidp  png_get_error_ptr (png_structp png_ptr);





 
extern  void  png_set_write_fn (png_structp png_ptr, png_voidp io_ptr, png_rw_ptr write_data_fn, png_flush_ptr output_flush_fn);

 



 


extern  void  png_set_read_status_fn (png_structp png_ptr, png_read_status_ptr read_row_fn);

extern  void  png_set_write_status_fn (png_structp png_ptr, png_write_status_ptr write_row_fn);

 
extern  void  png_set_mem_fn (png_structp png_ptr, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn);
 
extern  png_voidp  png_get_mem_ptr (png_structp png_ptr);

extern  void  png_set_read_user_transform_fn (png_structp png_ptr, png_user_transform_ptr read_user_transform_fn);

extern  void  png_set_write_user_transform_fn (png_structp png_ptr, png_user_transform_ptr write_user_transform_fn);

extern  void  png_set_user_transform_info (png_structp png_ptr, png_voidp user_transform_ptr, int user_transform_depth, int user_transform_channels);
 
extern  png_voidp  png_get_user_transform_ptr
   (png_structp png_ptr);

extern  void  png_set_read_user_chunk_fn (png_structp png_ptr, png_voidp user_chunk_ptr, png_user_chunk_ptr read_user_chunk_fn);
extern  png_voidp  png_get_user_chunk_ptr (png_structp png_ptr);



 
extern  void  png_set_progressive_read_fn (png_structp png_ptr, png_voidp progressive_ptr, png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn, png_progressive_end_ptr end_fn);

 
extern  png_voidp  png_get_progressive_ptr
   (png_structp png_ptr);

 
extern  void  png_process_data (png_structp png_ptr, png_infop info_ptr, png_bytep buffer, png_size_t buffer_size);



 
extern  void  png_progressive_combine_row (png_structp png_ptr, png_bytep old_row, png_bytep new_row);




 
extern  png_voidp  png_malloc_warn (png_structp png_ptr, png_uint_32 size);

 
extern  void  png_free (png_structp png_ptr, png_voidp ptr);


 
extern  void  png_free_data (png_structp png_ptr, png_infop info_ptr, png_uint_32 free_me, int num);

 
extern  void  png_data_freer (png_structp png_ptr, png_infop info_ptr, int freer, png_uint_32 mask);
 
 

extern  png_voidp  png_malloc_default (png_structp png_ptr, png_uint_32 size);
extern  void  png_free_default (png_structp png_ptr, png_voidp ptr);

extern  png_voidp  png_memcpy_check (png_structp png_ptr, png_voidp s1, png_voidp s2, png_uint_32 size);

extern  png_voidp  png_memset_check (png_structp png_ptr, png_voidp s1, int value, png_uint_32 size);


 
extern  void  png_error (png_structp png_ptr, png_const_charp error_message);

 
extern  void  png_chunk_error (png_structp png_ptr, png_const_charp error_message);

 
extern  void  png_warning (png_structp png_ptr, png_const_charp warning_message);

 
extern  void  png_chunk_warning (png_structp png_ptr, png_const_charp warning_message);












 
 



 




 
extern  png_bytepp  png_get_rows (png_structp png_ptr, png_infop info_ptr);

 
extern  void  png_set_rows (png_structp png_ptr, png_infop info_ptr, png_bytepp row_pointers);

 
extern  png_byte  png_get_channels (png_structp png_ptr, png_infop info_ptr);

 
extern  png_uint_32  png_get_image_width (png_structp png_ptr, png_infop info_ptr);

 
extern  png_uint_32  png_get_image_height (png_structp png_ptr, png_infop info_ptr);

 
extern  png_byte  png_get_bit_depth (png_structp png_ptr, png_infop info_ptr);

 
extern  png_byte  png_get_color_type (png_structp png_ptr, png_infop info_ptr);

 
extern  png_byte  png_get_filter_type (png_structp png_ptr, png_infop info_ptr);

 
extern  png_byte  png_get_interlace_type (png_structp png_ptr, png_infop info_ptr);

 
extern  png_byte  png_get_compression_type (png_structp png_ptr, png_infop info_ptr);

 
extern  png_uint_32  png_get_pixels_per_meter (png_structp png_ptr, png_infop info_ptr);
extern  png_uint_32  png_get_x_pixels_per_meter (png_structp png_ptr, png_infop info_ptr);
extern  png_uint_32  png_get_y_pixels_per_meter (png_structp png_ptr, png_infop info_ptr);

 
extern  float  png_get_pixel_aspect_ratio (png_structp png_ptr, png_infop info_ptr);

 
extern  png_int_32  png_get_x_offset_pixels (png_structp png_ptr, png_infop info_ptr);
extern  png_int_32  png_get_y_offset_pixels (png_structp png_ptr, png_infop info_ptr);
extern  png_int_32  png_get_x_offset_microns (png_structp png_ptr, png_infop info_ptr);
extern  png_int_32  png_get_y_offset_microns (png_structp png_ptr, png_infop info_ptr);


 
extern  png_bytep  png_get_signature (png_structp png_ptr, png_infop info_ptr);

extern  png_uint_32  png_get_bKGD (png_structp png_ptr, png_infop info_ptr, png_color_16p *background);

extern  void  png_set_bKGD (png_structp png_ptr, png_infop info_ptr, png_color_16p background);

extern  png_uint_32  png_get_cHRM (png_structp png_ptr, png_infop info_ptr, double *white_x, double *white_y, double *red_x, double *red_y, double *green_x, double *green_y, double *blue_x, double *blue_y);
extern  png_uint_32  png_get_cHRM_fixed (png_structp png_ptr, png_infop info_ptr, png_fixed_point *int_white_x, png_fixed_point *int_white_y, png_fixed_point *int_red_x, png_fixed_point *int_red_y, png_fixed_point *int_green_x, png_fixed_point *int_green_y, png_fixed_point *int_blue_x, png_fixed_point *int_blue_y);

extern  void  png_set_cHRM (png_structp png_ptr, png_infop info_ptr, double white_x, double white_y, double red_x, double red_y, double green_x, double green_y, double blue_x, double blue_y);
extern  void  png_set_cHRM_fixed (png_structp png_ptr, png_infop info_ptr, png_fixed_point int_white_x, png_fixed_point int_white_y, png_fixed_point int_red_x, png_fixed_point int_red_y, png_fixed_point int_green_x, png_fixed_point int_green_y, png_fixed_point int_blue_x, png_fixed_point int_blue_y);

extern  png_uint_32  png_get_gAMA (png_structp png_ptr, png_infop info_ptr, double *file_gamma);
extern  png_uint_32  png_get_gAMA_fixed (png_structp png_ptr, png_infop info_ptr, png_fixed_point *int_file_gamma);

extern  void  png_set_gAMA (png_structp png_ptr, png_infop info_ptr, double file_gamma);
extern  void  png_set_gAMA_fixed (png_structp png_ptr, png_infop info_ptr, png_fixed_point int_file_gamma);

extern  png_uint_32  png_get_hIST (png_structp png_ptr, png_infop info_ptr, png_uint_16p *hist);

extern  void  png_set_hIST (png_structp png_ptr, png_infop info_ptr, png_uint_16p hist);






extern  void  png_set_IHDR (png_structp png_ptr, png_infop info_ptr, png_uint_32 width, png_uint_32 height, int bit_depth, int color_type, int interlace_method, int compression_method, int filter_method);

extern  png_uint_32  png_get_oFFs (png_structp png_ptr, png_infop info_ptr, png_int_32 *offset_x, png_int_32 *offset_y, int *unit_type);

extern  void  png_set_oFFs (png_structp png_ptr, png_infop info_ptr, png_int_32 offset_x, png_int_32 offset_y, int unit_type);

extern  png_uint_32  png_get_pCAL (png_structp png_ptr, png_infop info_ptr, png_charp *purpose, png_int_32 *X0, png_int_32 *X1, int *type, int *nparams, png_charp *units, png_charpp *params);

extern  void  png_set_pCAL (png_structp png_ptr, png_infop info_ptr, png_charp purpose, png_int_32 X0, png_int_32 X1, int type, int nparams, png_charp units, png_charpp params);

extern  png_uint_32  png_get_pHYs (png_structp png_ptr, png_infop info_ptr, png_uint_32 *res_x, png_uint_32 *res_y, int *unit_type);

extern  void  png_set_pHYs (png_structp png_ptr, png_infop info_ptr, png_uint_32 res_x, png_uint_32 res_y, int unit_type);

extern  png_uint_32  png_get_PLTE (png_structp png_ptr, png_infop info_ptr, png_colorp *palette, int *num_palette);

extern  void  png_set_PLTE (png_structp png_ptr, png_infop info_ptr, png_colorp palette, int num_palette);

extern  png_uint_32  png_get_sBIT (png_structp png_ptr, png_infop info_ptr, png_color_8p *sig_bit);

extern  void  png_set_sBIT (png_structp png_ptr, png_infop info_ptr, png_color_8p sig_bit);

extern  png_uint_32  png_get_sRGB (png_structp png_ptr, png_infop info_ptr, int *intent);

extern  void  png_set_sRGB (png_structp png_ptr, png_infop info_ptr, int intent);
extern  void  png_set_sRGB_gAMA_and_cHRM (png_structp png_ptr, png_infop info_ptr, int intent);

extern  png_uint_32  png_get_iCCP (png_structp png_ptr, png_infop info_ptr, png_charpp name, int *compression_type, png_charpp profile, png_uint_32 *proflen);
    

extern  void  png_set_iCCP (png_structp png_ptr, png_infop info_ptr, png_charp name, int compression_type, png_charp profile, png_uint_32 proflen);
    

extern  png_uint_32  png_get_sPLT (png_structp png_ptr, png_infop info_ptr, png_sPLT_tpp entries);

extern  void  png_set_sPLT (png_structp png_ptr, png_infop info_ptr, png_sPLT_tp entries, int nentries);

 
extern  png_uint_32  png_get_text (png_structp png_ptr, png_infop info_ptr, png_textp *text_ptr, int *num_text);







 

extern  void  png_set_text (png_structp png_ptr, png_infop info_ptr, png_textp text_ptr, int num_text);

extern  png_uint_32  png_get_tIME (png_structp png_ptr, png_infop info_ptr, png_timep *mod_time);

extern  void  png_set_tIME (png_structp png_ptr, png_infop info_ptr, png_timep mod_time);

extern  png_uint_32  png_get_tRNS (png_structp png_ptr, png_infop info_ptr, png_bytep *trans, int *num_trans, png_color_16p *trans_values);

extern  void  png_set_tRNS (png_structp png_ptr, png_infop info_ptr, png_bytep trans, int num_trans, png_color_16p trans_values);


extern  png_uint_32  png_get_sCAL (png_structp png_ptr, png_infop info_ptr, int *unit, double *width, double *height);

extern  void  png_set_sCAL (png_structp png_ptr, png_infop info_ptr, int unit, double width, double height);









 
extern  void  png_set_keep_unknown_chunks (png_structp png_ptr, int keep, png_bytep chunk_list, int num_chunks);
extern  void  png_set_unknown_chunks (png_structp png_ptr, png_infop info_ptr, png_unknown_chunkp unknowns, int num_unknowns);
extern  void  png_set_unknown_chunk_location
   (png_structp png_ptr, png_infop info_ptr, int chunk, int location);
extern  png_uint_32  png_get_unknown_chunks (png_structp png_ptr, png_infop info_ptr, png_unknown_chunkpp entries);
 int  png_handle_as_unknown (png_structp png_ptr, png_bytep chunk_name);



 
extern  void  png_set_invalid (png_structp png_ptr, png_infop info_ptr, int mask);

 
extern  void  png_read_png (png_structp png_ptr, png_infop info_ptr, int transforms, png_voidp params);
extern  void  png_write_png (png_structp png_ptr, png_infop info_ptr, int transforms, png_voidp params);





 


extern  png_charp  png_get_copyright (png_structp png_ptr);
extern  png_charp  png_get_header_ver (png_structp png_ptr);
extern  png_charp  png_get_header_version (png_structp png_ptr);
extern  png_charp  png_get_libpng_ver (png_structp png_ptr);

extern  png_uint_32  png_permit_mng_features (png_structp png_ptr, png_uint_32 mng_features_permitted);

 

 




 
extern  png_uint_32  png_get_mmx_flagmask
   (int flag_select, int *compilerID);

 
extern  png_uint_32  png_get_asm_flagmask
   (int flag_select);

 
extern  png_uint_32  png_get_asm_flags
   (png_structp png_ptr);

 
extern  png_byte  png_get_mmx_bitdepth_threshold
   (png_structp png_ptr);

 
extern  png_uint_32  png_get_mmx_rowbytes_threshold
   (png_structp png_ptr);

 
extern  void  png_set_asm_flags
   (png_structp png_ptr, png_uint_32 asm_flags);

 
extern  void  png_set_mmx_thresholds
   (png_structp png_ptr, png_byte mmx_bitdepth_threshold, png_uint_32 mmx_rowbytes_threshold);


 
extern  int  png_mmx_support (void);


 
extern  void  png_set_strip_error_numbers (png_structp png_ptr, png_uint_32 strip_mode);


 
extern  void  png_set_user_limits (png_structp png_ptr, png_uint_32 user_width_max, png_uint_32 user_height_max);
extern  png_uint_32  png_get_user_width_max (png_structp png_ptr);
extern  png_uint_32  png_get_user_height_max (png_structp png_ptr);

 











 

  











 
extern  png_uint_32  png_get_uint_32 (png_bytep buf);
extern  png_uint_16  png_get_uint_16 (png_bytep buf);
extern  png_int_32  png_get_int_32 (png_bytep buf);
extern  png_uint_32  png_get_uint_31
  (png_structp png_ptr, png_bytep buf);
 


 
extern  void  png_save_uint_32
   (png_bytep buf, png_uint_32 i);
extern  void  png_save_int_32
   (png_bytep buf, png_int_32 i);




 
extern  void  png_save_uint_16
   (png_bytep buf, unsigned int i);
 

 





 



 

 























































 




 























































 







  










 



 





 







typedef struct bignum_st
	{
	unsigned long *d;	 
	int top;	 
	 
	int max;	 
	int neg;	 
	int flags;
	} BIGNUM;

 
typedef struct bignum_ctx
	{
	int tos;
	BIGNUM bn[12];
	int flags;
	int depth;
	int pos[12];
	int too_many;
	} BN_CTX;

typedef struct bn_blinding_st
	{
	int init;
	BIGNUM *A;
	BIGNUM *Ai;
	BIGNUM *mod;  
	} BN_BLINDING;

 
typedef struct bn_mont_ctx_st
	{
	int ri;         
	BIGNUM RR;      
	BIGNUM N;       
	BIGNUM Ni;     
 
	unsigned long n0;    
	int flags;
	} BN_MONT_CTX;



 
typedef struct bn_recp_ctx_st
	{
	BIGNUM N;	 
	BIGNUM Nr;	 
	int num_bits;
	int shift;
	int flags;
	} BN_RECP_CTX;







 


 
 

BIGNUM *BN_value_one(void);
char *	BN_options(void);
BN_CTX *BN_CTX_new(void);
void	BN_CTX_init(BN_CTX *c);
void	BN_CTX_free(BN_CTX *c);
void	BN_CTX_start(BN_CTX *ctx);
BIGNUM *BN_CTX_get(BN_CTX *ctx);
void	BN_CTX_end(BN_CTX *ctx);
int     BN_rand(BIGNUM *rnd, int bits, int top,int bottom);
int     BN_pseudo_rand(BIGNUM *rnd, int bits, int top,int bottom);
int	BN_num_bits(const BIGNUM *a);
int	BN_num_bits_word(unsigned long);
BIGNUM *BN_new(void);
void	BN_init(BIGNUM *);
void	BN_clear_free(BIGNUM *a);
BIGNUM *BN_copy(BIGNUM *a, const BIGNUM *b);

int	BN_bn2bin(const BIGNUM *a, unsigned char *to);
BIGNUM *BN_mpi2bn(unsigned char *s,int len,BIGNUM *ret);
int	BN_bn2mpi(const BIGNUM *a, unsigned char *to);
int	BN_sub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_usub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_uadd(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int	BN_mod(BIGNUM *rem, const BIGNUM *m, const BIGNUM *d, BN_CTX *ctx);
int	BN_div(BIGNUM *dv, BIGNUM *rem, const BIGNUM *m, const BIGNUM *d,
	       BN_CTX *ctx);
int	BN_mul(BIGNUM *r, BIGNUM *a, BIGNUM *b, BN_CTX *ctx);
int	BN_sqr(BIGNUM *r, BIGNUM *a,BN_CTX *ctx);
unsigned long BN_mod_word(const BIGNUM *a, unsigned long w);
unsigned long BN_div_word(BIGNUM *a, unsigned long w);
int	BN_mul_word(BIGNUM *a, unsigned long w);
int	BN_add_word(BIGNUM *a, unsigned long w);
int	BN_sub_word(BIGNUM *a, unsigned long w);
int	BN_set_word(BIGNUM *a, unsigned long w);
unsigned long BN_get_word(BIGNUM *a);
int	BN_cmp(const BIGNUM *a, const BIGNUM *b);
void	BN_free(BIGNUM *a);
int	BN_is_bit_set(const BIGNUM *a, int n);
int	BN_lshift(BIGNUM *r, const BIGNUM *a, int n);
int	BN_lshift1(BIGNUM *r, BIGNUM *a);
int	BN_exp(BIGNUM *r, BIGNUM *a, BIGNUM *p,BN_CTX *ctx);
int	BN_mod_exp(BIGNUM *r, BIGNUM *a, const BIGNUM *p,
		   const BIGNUM *m,BN_CTX *ctx);
int	BN_mod_exp_mont(BIGNUM *r, BIGNUM *a, const BIGNUM *p,
			const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *m_ctx);
int	BN_mod_exp2_mont(BIGNUM *r, BIGNUM *a1, BIGNUM *p1,BIGNUM *a2,
		BIGNUM *p2,BIGNUM *m,BN_CTX *ctx,BN_MONT_CTX *m_ctx);
int	BN_mod_exp_simple(BIGNUM *r, BIGNUM *a, BIGNUM *p,
	BIGNUM *m,BN_CTX *ctx);
int	BN_mask_bits(BIGNUM *a,int n);
int	BN_mod_mul(BIGNUM *ret, BIGNUM *a, BIGNUM *b, const BIGNUM *m, BN_CTX *ctx);
int	BN_print(void *fp, const BIGNUM *a);
int	BN_reciprocal(BIGNUM *r, BIGNUM *m, int len, BN_CTX *ctx);
int	BN_rshift(BIGNUM *r, BIGNUM *a, int n);
int	BN_rshift1(BIGNUM *r, BIGNUM *a);
void	BN_clear(BIGNUM *a);
BIGNUM *BN_dup(const BIGNUM *a);
int	BN_ucmp(const BIGNUM *a, const BIGNUM *b);
int	BN_set_bit(BIGNUM *a, int n);
int	BN_clear_bit(BIGNUM *a, int n);
char *	BN_bn2hex(const BIGNUM *a);
char *	BN_bn2dec(const BIGNUM *a);
int 	BN_hex2bn(BIGNUM **a, const char *str);
int 	BN_dec2bn(BIGNUM **a, const char *str);
int	BN_gcd(BIGNUM *r,BIGNUM *in_a,BIGNUM *in_b,BN_CTX *ctx);
BIGNUM *BN_mod_inverse(BIGNUM *ret,BIGNUM *a, const BIGNUM *n,BN_CTX *ctx);
BIGNUM *BN_generate_prime(BIGNUM *ret,int bits,int safe,BIGNUM *add,
		BIGNUM *rem,void (*callback)(int,int,void *),void *cb_arg);
int	BN_is_prime(const BIGNUM *p,int nchecks,
		void (*callback)(int,int,void *),
		BN_CTX *ctx,void *cb_arg);
int	BN_is_prime_fasttest(const BIGNUM *p,int nchecks,
		void (*callback)(int,int,void *),BN_CTX *ctx,void *cb_arg,
		int do_trial_division);
void	ERR_load_BN_strings(void );

BN_MONT_CTX *BN_MONT_CTX_new(void );
void BN_MONT_CTX_init(BN_MONT_CTX *ctx);
int BN_mod_mul_montgomery(BIGNUM *r,BIGNUM *a,BIGNUM *b,BN_MONT_CTX *mont,
			  BN_CTX *ctx);
int BN_from_montgomery(BIGNUM *r,BIGNUM *a,BN_MONT_CTX *mont,BN_CTX *ctx);
void BN_MONT_CTX_free(BN_MONT_CTX *mont);
int BN_MONT_CTX_set(BN_MONT_CTX *mont,const BIGNUM *modulus,BN_CTX *ctx);
BN_MONT_CTX *BN_MONT_CTX_copy(BN_MONT_CTX *to,BN_MONT_CTX *from);

BN_BLINDING *BN_BLINDING_new(BIGNUM *A,BIGNUM *Ai,BIGNUM *mod);
void BN_BLINDING_free(BN_BLINDING *b);
int BN_BLINDING_update(BN_BLINDING *b,BN_CTX *ctx);
int BN_BLINDING_convert(BIGNUM *n, BN_BLINDING *r, BN_CTX *ctx);
int BN_BLINDING_invert(BIGNUM *n, BN_BLINDING *b, BN_CTX *ctx);

void BN_set_params(int mul,int high,int low,int mont);
int BN_get_params(int which);  

void	BN_RECP_CTX_init(BN_RECP_CTX *recp);
BN_RECP_CTX *BN_RECP_CTX_new(void);
void	BN_RECP_CTX_free(BN_RECP_CTX *recp);
int	BN_RECP_CTX_set(BN_RECP_CTX *recp,const BIGNUM *rdiv,BN_CTX *ctx);
int	BN_mod_mul_reciprocal(BIGNUM *r, BIGNUM *x, BIGNUM *y,
		BN_RECP_CTX *recp,BN_CTX *ctx);
int	BN_mod_exp_recp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
			const BIGNUM *m, BN_CTX *ctx);
int	BN_div_recp(BIGNUM *dv, BIGNUM *rem, BIGNUM *m,
		BN_RECP_CTX *recp, BN_CTX *ctx);

 

BIGNUM *bn_expand2(BIGNUM *a, int words);


unsigned long bn_mul_add_words(unsigned long *rp, unsigned long *ap, int num, unsigned long w);
unsigned long bn_mul_words(unsigned long *rp, unsigned long *ap, int num, unsigned long w);
void     bn_sqr_words(unsigned long *rp, unsigned long *ap, int num);
unsigned long bn_div_words(unsigned long h, unsigned long l, unsigned long d);
unsigned long bn_add_words(unsigned long *rp, unsigned long *ap, unsigned long *bp,int num);
unsigned long bn_sub_words(unsigned long *rp, unsigned long *ap, unsigned long *bp,int num);


 


 

 

 

 



 























































 















 

 
 



 





 



 

 
 



 
typedef struct bio_st BIO_dummy;

typedef struct crypto_ex_data_st
	{
	void *sk;
	int dummy;  
	} CRYPTO_EX_DATA;

 
typedef int CRYPTO_EX_new(void *parent, void *ptr, CRYPTO_EX_DATA *ad,
					int idx, long argl, void *argp);
 
typedef void CRYPTO_EX_free(void *parent, void *ptr, CRYPTO_EX_DATA *ad,
					int idx, long argl, void *argp);
 
typedef int CRYPTO_EX_dup(CRYPTO_EX_DATA *to, CRYPTO_EX_DATA *from, void *from_d, 
					int idx, long argl, void *argp);


 

typedef struct crypto_ex_data_func_st
	{
	long argl;	 
	void *argp;	 
	CRYPTO_EX_new *new_func;
	CRYPTO_EX_free *free_func;
	CRYPTO_EX_dup *dup_func;
	} CRYPTO_EX_DATA_FUNCS;





 






 



 

int CRYPTO_mem_ctrl(int mode);
int CRYPTO_is_mem_check_on(void);

 

 




 


const char *SSLeay_version(int type);
unsigned long SSLeay(void);

int CRYPTO_get_new_lockid(char *name);

int CRYPTO_num_locks(void);  
void CRYPTO_lock(int mode, int type,const char *file,int line);
void CRYPTO_set_locking_callback(void (*func)(int mode,int type,
					      const char *file,int line));
void (*CRYPTO_get_locking_callback(void))(int mode,int type,const char *file,
		int line);
void CRYPTO_set_add_lock_callback(int (*func)(int *num,int mount,int type,
					      const char *file, int line));
int (*CRYPTO_get_add_lock_callback(void))(int *num,int mount,int type,
					  const char *file,int line);
void CRYPTO_set_id_callback(unsigned long (*func)(void));
unsigned long (*CRYPTO_get_id_callback(void))(void);
unsigned long CRYPTO_thread_id(void);
const char *CRYPTO_get_lock_name(int type);
int CRYPTO_add_lock(int *pointer,int amount,int type, const char *file,
		    int line);


 
int CRYPTO_set_mem_functions(void *(*m)(size_t),void *(*r)(void *,size_t), void (*f)(void *));
int CRYPTO_set_locked_mem_functions(void *(*m)(size_t), void (*free_func)(void *));
int CRYPTO_set_mem_debug_functions(void (*m)(),void (*r)(),void (*f)(),void (*so)(),long (*go)());
void CRYPTO_get_mem_functions(void *(**m)(size_t),void *(**r)(void *, size_t), void (**f)(void *));
void CRYPTO_get_locked_mem_functions(void *(**m)(size_t), void (**f)(void *));
void CRYPTO_get_mem_debug_functions(void (**m)(),void (**r)(),void (**f)(),void (**so)(),long (**go)());

void *CRYPTO_malloc_locked(int num, const char *file, int line);
void CRYPTO_free_locked(void *);
void *CRYPTO_malloc(int num, const char *file, int line);
void CRYPTO_free(void *);
void *CRYPTO_realloc(void *addr,int num, const char *file, int line);
void *CRYPTO_remalloc(void *addr,int num, const char *file, int line);

void CRYPTO_set_mem_debug_options(long bits);
long CRYPTO_get_mem_debug_options(void);

int CRYPTO_push_info_(const char *info, const char *file, int line);
int CRYPTO_pop_info(void);
int CRYPTO_remove_all_info(void);





 
void CRYPTO_dbg_malloc(void *addr,int num,const char *file,int line,int before_p);
void CRYPTO_dbg_realloc(void *addr1,void *addr2,int num,const char *file,int line,int before_p);
void CRYPTO_dbg_free(void *addr,int before_p);








 
void CRYPTO_dbg_set_options(long bits);
long CRYPTO_dbg_get_options(void);

void CRYPTO_mem_leaks(struct bio_st *bio);
 
void CRYPTO_mem_leaks_cb(void (*cb)());

void ERR_load_CRYPTO_strings(void);

 


 

 

 

 




typedef struct rsa_st RSA;

typedef struct rsa_meth_st
	{
	const char *name;
	int (*rsa_pub_enc)(int flen,unsigned char *from,unsigned char *to,
			   RSA *rsa,int padding);
	int (*rsa_pub_dec)(int flen,unsigned char *from,unsigned char *to,
			   RSA *rsa,int padding);
	int (*rsa_priv_enc)(int flen,unsigned char *from,unsigned char *to,
			    RSA *rsa,int padding);
	int (*rsa_priv_dec)(int flen,unsigned char *from,unsigned char *to,
			    RSA *rsa,int padding);
	int (*rsa_mod_exp)(BIGNUM *r0,BIGNUM *I,RSA *rsa);  
	int (*bn_mod_exp)(BIGNUM *r, BIGNUM *a, const BIGNUM *p,
			  const BIGNUM *m, BN_CTX *ctx,
			  BN_MONT_CTX *m_ctx);  
	int (*init)(RSA *rsa);		 
	int (*finish)(RSA *rsa);	 
	int flags;			 
	char *app_data;			 






 
	int (*rsa_sign)(int type, unsigned char *m, unsigned int m_len,
             unsigned char *sigret, unsigned int *siglen, RSA *rsa);
	int (*rsa_verify)(int dtype, unsigned char *m, unsigned int m_len,
             unsigned char *sigbuf, unsigned int siglen, RSA *rsa);

	} RSA_METHOD;

struct rsa_st
	{
	
 
	int pad;
	int version;
	RSA_METHOD *meth;
	BIGNUM *n;
	BIGNUM *e;
	BIGNUM *d;
	BIGNUM *p;
	BIGNUM *q;
	BIGNUM *dmp1;
	BIGNUM *dmq1;
	BIGNUM *iqmp;
	 
	CRYPTO_EX_DATA ex_data;
	int references;
	int flags;

	 
	BN_MONT_CTX *_method_mod_n;
	BN_MONT_CTX *_method_mod_p;
	BN_MONT_CTX *_method_mod_q;

	
 
	char *bignum_data;
	BN_BLINDING *blinding;
	};







 


 



RSA *	RSA_new(void);

int	RSA_size(RSA *);
RSA *	RSA_generate_key(int bits, unsigned long e,void
		(*callback)(int,int,void *),void *cb_arg);
int	RSA_check_key(RSA *);
	 
int	RSA_public_encrypt(int flen, char *from,
		char *to, RSA *rsa,int padding);
int	RSA_private_encrypt(int flen, char *from,
		char *to, RSA *rsa,int padding);
int	RSA_public_decrypt(int flen, unsigned char *from, 
		unsigned char *to, RSA *rsa,int padding);
int	RSA_private_decrypt(int flen, unsigned char *from, 
		unsigned char *to, RSA *rsa,int padding);


int	RSA_flags(RSA *r);

void RSA_set_default_method(RSA_METHOD *meth);
RSA_METHOD *RSA_get_default_method(void);
RSA_METHOD *RSA_get_method(RSA *rsa);
RSA_METHOD *RSA_set_method(RSA *rsa, RSA_METHOD *meth);

 
int RSA_memory_lock(RSA *r);

 
RSA_METHOD *RSA_PKCS1_RSAref(void);

 
RSA_METHOD *RSA_PKCS1_SSLeay(void);

RSA_METHOD *RSA_null_method(void);

void	ERR_load_RSA_strings(void );

RSA *	d2i_RSAPublicKey(RSA **a, unsigned char **pp, long length);
int	i2d_RSAPublicKey(RSA *a, unsigned char **pp);
RSA *	d2i_RSAPrivateKey(RSA **a, unsigned char **pp, long length);
int 	i2d_RSAPrivateKey(RSA *a, unsigned char **pp);


int i2d_Netscape_RSA(RSA *a, unsigned char **pp, int (*cb)());
RSA *d2i_Netscape_RSA(RSA **a, unsigned char **pp, long length, int (*cb)());

 
RSA *d2i_Netscape_RSA_2(RSA **a, unsigned char **pp, long length, int (*cb)());


 


int RSA_verify(int type, unsigned char *m, unsigned int m_len,
	unsigned char *sigbuf, unsigned int siglen, RSA *rsa);


 
int RSA_sign_ASN1_OCTET_STRING(int type, unsigned char *m, unsigned int m_len,
	unsigned char *sigret, unsigned int *siglen, RSA *rsa);
int RSA_verify_ASN1_OCTET_STRING(int type, unsigned char *m, unsigned int m_len,
	unsigned char *sigbuf, unsigned int siglen, RSA *rsa);

int RSA_blinding_on(RSA *rsa, BN_CTX *ctx);
void RSA_blinding_off(RSA *rsa);

int RSA_padding_add_PKCS1_type_1(unsigned char *to,int tlen,
	unsigned char *f,int fl);
int RSA_padding_check_PKCS1_type_1(unsigned char *to,int tlen,
	unsigned char *f,int fl,int rsa_len);
int RSA_padding_add_PKCS1_type_2(unsigned char *to,int tlen,
	unsigned char *f,int fl);
int RSA_padding_check_PKCS1_type_2(unsigned char *to,int tlen,
	unsigned char *f,int fl,int rsa_len);
int RSA_padding_add_PKCS1_OAEP(unsigned char *to,int tlen,
			       unsigned char *f,int fl,unsigned char *p,
			       int pl);
int RSA_padding_check_PKCS1_OAEP(unsigned char *to,int tlen,
				 unsigned char *f,int fl,int rsa_len,
				 unsigned char *p,int pl);
int RSA_padding_add_SSLv23(unsigned char *to,int tlen,
	unsigned char *f,int fl);
int RSA_padding_check_SSLv23(unsigned char *to,int tlen,
	unsigned char *f,int fl,int rsa_len);
int RSA_padding_add_none(unsigned char *to,int tlen,
	unsigned char *f,int fl);
int RSA_padding_check_none(unsigned char *to,int tlen,
	unsigned char *f,int fl,int rsa_len);

int RSA_get_ex_new_index(long argl, void *argp, CRYPTO_EX_new *new_func,
	CRYPTO_EX_dup *dup_func, CRYPTO_EX_free *free_func);
int RSA_set_ex_data(RSA *r,int idx,void *arg);
void *RSA_get_ex_data(RSA *r, int idx);

 


 

 

 

 



 























































 









 



typedef struct SHAstate_st
	{
	unsigned int h0,h1,h2,h3,h4;
	unsigned int Nl,Nh;
	unsigned int data[16];
	int num;
	} SHA_CTX;

void SHA_Init(SHA_CTX *c);
void SHA_Update(SHA_CTX *c, const void *data, unsigned long len);
void SHA_Final(unsigned char *md, SHA_CTX *c);
unsigned char *SHA(const unsigned char *d, unsigned long n,unsigned char *md);
void SHA_Transform(SHA_CTX *c, const unsigned char *data);



unsigned char *SHA1(const unsigned char *d, unsigned long n,unsigned char *md);
void SHA1_Transform(SHA_CTX *c, const unsigned char *data);

 























































 









 



typedef struct MD5state_st
	{
	unsigned int A,B,C,D;
	unsigned int Nl,Nh;
	unsigned int data[(64/4)];
	int num;
	} MD5_CTX;

void MD5_Init(MD5_CTX *c);
void MD5_Update(MD5_CTX *c, const void *data, unsigned long len);
void MD5_Final(unsigned char *md, MD5_CTX *c);
unsigned char *MD5(const unsigned char *d, unsigned long n, unsigned char *md);
void MD5_Transform(MD5_CTX *c, const unsigned char *b);

typedef struct
{
	void *ChanBannerQ; 
}SFN_DATA;














typedef struct{
  int pid_from;
  int msg; 
  void *pkt;
  char unk1;
  char encoding_type;
  char pkt_length;
  char unk2;
  void *data1;
} GBS_USSD_MSG;



typedef struct{
  unsigned long year;
  unsigned char month;
  unsigned char day;
} TDate;

typedef struct{
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
  unsigned long millisec;
} TTime;

typedef struct{
  unsigned int type;
  unsigned int param2;
  unsigned short param3;
  unsigned short wstr[25];
}StructUSSDStr;

typedef struct{
  void *p1;
  void *p2;
  StructUSSDStr *pussdstr;
}StructUSSD;

typedef struct
{
  void *first;
  void *last;
  void (*data_mfree)(void *);
}LLQ;

typedef struct{
  int param0;
  int param1;
  int param2;
  int param3;
} GBSTMR;

typedef struct{
  int pid_from;
  int msg;
  int submess;
  void *data0;
  void *data1;
} GBS_MSG;

typedef struct{
  void *p;
  int i;
}MUTEX;

typedef struct{
  void *next;
  void *prev;
  void   *constr;
  int id;
  int state;
  int unk1;
  int unk2;
  LLQ gui_ll;
} CSM_RAM;

typedef struct{
  int (*onMessage)(CSM_RAM*,GBS_MSG*);
  void (*onCreate)(CSM_RAM*);
  int zero1;
  int zero2;
  int zero3;
  int zero4;
  void (*onClose)(CSM_RAM*);
  int datasize;
  int statesize;
  const int *states;
} CSM_DESC;

typedef struct{
  int id;
  CSM_RAM *current_msg_processing_csm;
  LLQ csm;
  LLQ cmd;
  LLQ csm_background;
} CSMQ;

typedef struct{
  void *next;
  void *prev;
  int flag1;
  int parent_id;
  int prio;
  int (*proc)(CSMQ *, void *);
  CSMQ *csm_q;
  int id;
  int flag2;
}MMICMD;



typedef struct{
  int under_idle_id;
  int idle_id;
  CSMQ *csm_q;
} CSMROOT;

typedef struct{
  short x;
  short y;
  short x2;
  short y2;
} RECT;

typedef struct{
  char dummy[0x24];
} DRWOBJ;

typedef struct{
  unsigned short *wsbody;
  void *(*ws_malloc)(int);
  void (*ws_mfree)(void *);
  int isbody_allocated;
  unsigned short maxlen;
  unsigned short unk1;
} WSHDR;

typedef struct
{
  void *next;
  void *prev;
  void *data;
}EDITQ;

typedef struct
{
  void *next;
  void *prev;
}LLIST;

typedef struct
{
  const char *ext;
  int unical_id;
  char enabled_options;
  char obex_path_id;
  short menu_flag;
  const int *icon1;
  const int *icon2;
  int Open_lgpid;	
  int Options_lgpid;	
  int Enter_lgpid;	
  void *proc;
  void *altproc;
}REGEXPLEXT;

typedef struct
{
  int zero;
  unsigned const int *icon1;
  int unical_id;
  char obex_path_id;
  char unk;
  unsigned short menu_flag;
  char enabled_options;
  char not_used[3];
  WSHDR* ext;
  void *proc;
  void *altproc;
  unsigned const int *icon2;
}REGEXPLEXT_ARM_NEW;

typedef struct
{
  RECT *canvas;
  void *methods;
  void *definition;
  char state;
  char unk2;
  char unk3;
  char unk4;
  int color1; 
  int color2;
  LLQ item_ll;
  int unk5;
  char unk6;
  char unk7;
  char unk8;
  char unk9;
  int unk10;
  int flag30; 
}GUI;


typedef struct
{
  char zero;
  char unk1;
  short keys;
  GBS_MSG *gbsmsg;
}GUI_MSG;

















































typedef struct
{
  unsigned short w;
  unsigned short h;
  int bpnum; 
  char *bitmap;
}IMGHDR;

typedef struct
{
  RECT rc;
  int *icon;
  int lgp_id;
  int lgp_null; 
}HEADER_DESC;

typedef struct
{
  int *icon;
  int lgp_id_small;
  int lgp_id_large;
  int zero;
  const int *softkeys; 
  int flag1; 
  int flag2; 
}MENUITEM_DESC;

typedef struct
{
  short key1;
  short key2;
  int lgp_id;
}SOFTKEY_DESC;

typedef struct
{
  const SOFTKEY_DESC *desc;
  int n;
}SOFTKEYSTAB;

typedef void (__interwork *MENUPROCS_DESC)(GUI *);

typedef struct
{
  int flag; 
  int (*onkey)(void *, GUI_MSG *);
  void (*ghook)(void *, int ); 
  void *proc3;
  const int *softkeys; 
  const SOFTKEYSTAB *softkeystab;
  int flags2; 


















  void (*itemproc)(void *, int, void *); 
  const MENUITEM_DESC *items; 
  const MENUPROCS_DESC *procs;
  int n_items; 
}MENU_DESC;

typedef struct
{
  int flag; 
  int (*onkey)(void *, GUI_MSG *);
  void (*ghook)(void *, int ); 
  void *proc3;
  const int *softkeys; 
  const SOFTKEYSTAB *softkeystab;
  int flags2; 
  void (*itemproc)(void *, int, void *); 
  const MENUITEM_DESC *items; 
  const MENUPROCS_DESC *procs;
  int n_items; 
  int n_lines; 
}ML_MENU_DESC;

#pragma pack(1)
typedef struct{
  char unk0[40];
  unsigned int file_size;
  short file_attr;
  unsigned int create_date_time;
  char unk1[2];
  char folder_name[128];
  char unk2;
  char file_name[128];
  char unk3[11];
  short wsfolder_name[128+1];
  short wsfile_name[128+1];
}DIR_ENTRY;
#pragma pack()

typedef struct
{
  char unk1;
  char unk2;
  char unk3;
  char unk4;
  unsigned int size;
  char unk5[28];
  int file_attr;
}FSTATS;


typedef struct
{
  unsigned short ch_number;
  unsigned short ci;
  unsigned short lac;
  char power;
  char c1;
  char c2;
  char unk;
  unsigned short unk2;
}RAMNET;



typedef struct
{
  int one;
  int (*onKey)(GUI *gui, GUI_MSG *msg);
  void (*global_hook_proc)(GUI *gui, int cmd);
  void *locret;
  int zero1;
  const SOFTKEYSTAB *softkeystab;
  RECT rc;
  int font;
  int _100;
  int _101;
  int zero2;






  int zero3;



  int _0x40000000;
}INPUTDIA_DESC;

typedef struct
{
  char type;
  void *user_pointer;
  unsigned short cur_item;
  WSHDR *ws;
}USR_MENU_ITEM;

typedef struct
{
  unsigned short invert;  
  unsigned short bitmask;
  char font;              
  char align;             
  char pen[4];            
  char brush[4];          
}EDITC_OPTIONS;

#pragma pack(2)
typedef struct
{
  char type;
  char unk2[3];
  int flag;
  char zero_cbox;  
  char unk5;
  EDITC_OPTIONS ed_options;
  short maxlen;
  short num_of_items_cbox;
  short start_item_cbox;
  short unk9;
  short unk10;
  short unk11;
  int unk12;
  WSHDR *pWS;
}EDITCONTROL;
#pragma pack()






















 



typedef struct
{
  short field_0;
  char no_data;
  char item_type;
  int field_4;
  void *data;
}AB_UNPRES_ITEM;

typedef struct{
  short NrOfFlds;
  short number_of_records;
  short NrAllocFlds;
  short field_6;
  short NrJ;
  char field_A;
  char field_B;
  AB_UNPRES_ITEM *record_list;
  char EntTp;
  char field_11;
  short EntId;
  int field_14;
  int field_18;
}AB_UNPRES;

#pragma pack(1)
typedef struct
{
  char _1D;
  char format;
  char data_size;
  char _0_1;
  char _0_2;

  char data[128];
}PKT_NUM;
#pragma pack()




typedef struct
{
  unsigned short family; 
  unsigned short port; 
  unsigned int ip; 
  unsigned int unk1;
  unsigned int unk2;
}SOCK_ADDR;

#pragma pack(2)
typedef struct
{
  unsigned short _0x0080;
  unsigned short _0xFFFF;
  unsigned short _0x0000;
}REGSOCKCEPID_DATA;
#pragma pack()

typedef struct
{
  char body[0x204];
}CTX_NAPDEF;

typedef struct
{
  unsigned int len;
  CTX_NAPDEF *ctx_napdef;
  unsigned int zero;
  CTX_NAPDEF napdef;
}NAP_PARAM_CONT;

typedef struct
{
  int zero;
  void *native_profile;
  char _1;
  char _4;
  unsigned short _0;
  NAP_PARAM_CONT *NapParameterContainer;
}LMAN_DATA;


typedef struct{
  unsigned short repeat_num;  
  unsigned short unk;
  int time_between_play;
  int play_first; 
  int volume;
  int unk5;
  int unk6;
  int unk7;
  int unk8;
  int unk9;
}PLAYFILE_OPT;


typedef struct
{
 char data[0xB2];
}PDU;

typedef struct
{
  const char *name_to;
  const char *name_from;
  void *data;
}IPC_REQ;

typedef struct
{
  unsigned short type; 
  WSHDR *wfilename; 
  int unk_08 ; 
  int unk_0C; 
  int unk_10 ; 
  int unk_14 ; 
  long length; 
  int unk_1C; 
  int unk_20 ; 
} TWavLen;


typedef struct
{
  char yearNormBudd; 
  char dateFormat;
  char timeFormat; 
  char timeZone; 
  char isAutoTime1; 
  char isAutoTime2;
}TDateTimeSettings;

typedef struct
{
  unsigned int type;  
  WSHDR *filename;   
  WSHDR *resolution_ws;
  int uid;            
  unsigned short width; 
  unsigned short height;
  WSHDR *duration_mp3_ws;
  unsigned int duration_mp3;
  WSHDR *duration_wav_ws; 
  unsigned int duration_wav;
  WSHDR *tag_title_ws;
  WSHDR *tag_artist_ws;
  WSHDR *tag_album_ws; 
  WSHDR *tag_track_ws; 
  WSHDR *tag_year_ws;  
  WSHDR *unk_tag_2_ws; 
  WSHDR *bitrate_ws;   
  WSHDR *audio_frequency_ws; 
  WSHDR *size_ws;   
} FILE_PROP;

typedef struct
{
  int mode; 
  int dir_enum; 
  WSHDR* path_to_file; 
  WSHDR* file_name; 
  int unk5;
  int is_exact_dir;
  int unk7;
  int unk8;
  int unk9;
  int unk10;
  int unk11;
  WSHDR* full_filename;
  int unk13;
  int (*user_handler)(void*); 
  void* this_struct_addr;
  int unk16;
  int unk17_26[10]; 
}NativeExplorerData;

typedef int  (__interwork MenuSearchCallBack)(void *gui,WSHDR * searchstr);  

#pragma diag_suppress=Ta035

#pragma swi_number=0
__swi __arm void loopback0();



#pragma swi_number=1
__swi __arm void loopback1();



#pragma swi_number=2
__swi __arm void loopback2();



#pragma swi_number=3
__swi __arm void loopback3();



#pragma swi_number=4
__swi __arm void loopback4();



#pragma swi_number=6
__swi __arm int GetAkku(int param1,int param2);



#pragma swi_number=8
__swi __arm long SetIllumination(unsigned char dev,unsigned long param1,unsigned short bright,unsigned long delay);



#pragma swi_number=0x9
__swi __arm void SwitchPhoneOff();



#pragma swi_number=10
__swi __arm int fopen(const char * cFileName, unsigned int iFileFlags, unsigned int iFileMode, unsigned int *ErrorNumber);



#pragma swi_number=11
__swi __arm int fread(int FileHandler, void *cBuffer, int iByteCount, unsigned int *ErrorNumber);



#pragma swi_number=12
__swi __arm unsigned int fwrite(int FileHandler, void const * cBuffer, int iByteCount, unsigned int *ErrorNumber);



#pragma swi_number=13
__swi __arm void fclose(int FileHandler, unsigned int *ErrorNumber);



#pragma swi_number=15
__swi __arm unsigned int lseek(int FileHandler, unsigned int offset, unsigned int origin, unsigned int *ErrorNumber, unsigned int *ErrorNumber2);



#pragma swi_number=16
__swi __arm int mkdir(const char * cFileName, unsigned int *ErrorNumber);



#pragma swi_number=18
__swi __arm int GetFileAttrib(const char *cFileName, unsigned char *cAttribute, unsigned int *ErrorNumber);



#pragma swi_number=19
__swi __arm int SetFileAttrib(const char *cFileName, unsigned char cAttribute, unsigned int *ErrorNumber);



#pragma swi_number=20
__swi __arm void *malloc(unsigned int size);



#pragma swi_number=0x8014
__swi __arm void *malloc_adr(void);

#pragma swi_number=21
__swi __arm void mfree(void *);



#pragma swi_number=0x8015
__swi __arm void *mfree_adr(void);

#pragma swi_number=22
__swi __arm int sprintf(char *buf, const char *str, ...);



#pragma swi_number=23
__swi	__arm	char * strcat (char *,const char *);



#pragma swi_number=24
__swi	__arm	char * strchr (const char *,int);



#pragma swi_number=25
__swi	__arm	int  strcmp (const char *,const char *);



#pragma swi_number=26
__swi	__arm	char * strcpy (char *dest,const char *source);



#pragma swi_number=27
__swi __arm unsigned int strlen(const char *);



#pragma swi_number=28
__swi	__arm	void  wsAppendChar(WSHDR *ws,int wchar);



#pragma swi_number=29
__swi	__arm int wsInsertChar(WSHDR *ws,int wchar,int pos);



#pragma swi_number=30
__swi	__arm IMGHDR *GetPITaddr(int num);



#pragma swi_number=33
__swi __arm int GetImgHeight(int picture);



#pragma swi_number=34
__swi __arm int GetImgWidth(int picture);



#pragma swi_number=35
__swi __arm void DrawImg(unsigned int x, unsigned int y, unsigned int picture);



#pragma swi_number=37
__swi __arm void DrawCanvas(void *data, int x1, int y1, int x2, int y2, int flag_one);



#pragma swi_number=38
__swi __arm void DrawImgBW(unsigned int x, unsigned int y, unsigned int picture, char *pen, char *brush);





#pragma swi_number=0x2C
__swi __arm  int EEFullGetBlockInfo(unsigned int block,int *size,char *version); 

#pragma swi_number=50
__swi __arm void PlaySound(long param1, long param2, long param3, long tone, long param5);



#pragma swi_number=0x0034
__swi __arm void PlaySoundLoop(int _0x2F, int param2, int duration);

#pragma swi_number=0x0035
__swi __arm void AlarmClockRing();



#pragma swi_number=42
__swi __arm int *GetEELiteBlockAddress(short blocknum);



#pragma swi_number=60
__swi __arm int setfilesize(int FileHandler, unsigned int iNewFileSize, unsigned int *ErrorNumber);



#pragma swi_number=0x0042
__swi __arm int IsScreenSaver(void);





#pragma swi_number=67
__swi __arm int IsUnlocked(void);



#pragma swi_number=0x45
__swi __arm int GetWavLen(char *filename); 

#pragma swi_number=0x0046
__swi __arm int HasNewSMS(void);



#pragma swi_number=0x4A
__swi __arm short PlayFile(int flags, WSHDR *foldername, WSHDR *filename, int CepId, int Msg, const PLAYFILE_OPT *sfo);



#pragma swi_number=0x004C
__swi __arm int GetMissedCallsCount(void);





#pragma swi_number=77
__swi __arm void GBS_StartTimerProc(void *htimer, long ms, void ptr());





#pragma swi_number=0x4E
__swi __arm int MsgBoxError(int flag, int LgpId);







#pragma swi_number=0x4F
__swi __arm int MsgBoxYesNo(int flag, int LgpID, void CallBackProc(int));







#pragma swi_number=0x50
__swi __arm int MsgBoxOkCancel(int flag, int LgpID, void CallBackProc(int));







#pragma swi_number=0x8052
__swi __arm char *GetIlluminationDataTable();



#pragma swi_number=0x0054
__swi __arm int StrToInt(char *s, char **endp);

#pragma swi_number=92
__swi __arm unsigned char GetCPUClock();



#pragma swi_number=93
__swi __arm unsigned char GetCPULoad();






#pragma swi_number=95
__swi __arm int GetFreeRamAvail();



#pragma swi_number=107
__swi	__arm	int  FindFirstFile (DIR_ENTRY *DIRENTRY,char *mask,unsigned int *ErrorNumber);



#pragma swi_number=108
__swi	__arm	int  FindNextFile (DIR_ENTRY *DIRENTRY,unsigned int *ErrorNumber);



#pragma swi_number=109
__swi	__arm	int  FindClose (DIR_ENTRY *DIRENTRY,unsigned int *ErrorNumber);



#pragma swi_number=0x06E
__swi	__arm int IsCalling(void);

#pragma swi_number=111
__swi __arm void RefreshGPRSTraffic();



#pragma swi_number=112
__swi __arm int *GetGPRSTrafficPointer();



#pragma swi_number=0x0073
__swi __arm int GetTypeOfCall();



#pragma swi_number=116
__swi __arm void* GetFunctionPointer(char * _shortcut);

#pragma swi_number=124
__swi __arm unsigned int SetVibration(unsigned int power);



#pragma swi_number=128
__swi __arm int ShowCallList(int list, int zero);



#pragma swi_number=132
__swi __arm int GetFileStats(const char *cFileName, FSTATS * StatBuffer, unsigned int *errornumber);



#pragma swi_number=0x0085
__swi __arm int strcmpi(const char *s1, const char *s2);

#pragma swi_number=0x86
__swi __arm void EEFullReadBlock(int block, void *buf, int offset, int size, int cepid, int msg); 





#pragma swi_number=0x87
__swi __arm void EEFullWriteBlock(int block, void *buf, int offset, int size, int cepid, int msg);





#pragma swi_number=138
__swi __arm int GetFreeFlexSpace(int DriveNum, unsigned int *ErrorNumber);



#pragma swi_number=139
__swi __arm int GetTotalFlexSpace(int DriveNum, unsigned int *ErrorNumber);



#pragma swi_number=140
__swi __arm void RegExplorerExt(REGEXPLEXT const *);



#pragma swi_number=142
__swi __arm int fmove(const char * SourceFileName, const char * DestFileName, unsigned int *ErrorNumber);



#pragma swi_number=143
__swi __arm int rmdir(const char * cDirectory, unsigned int *ErrorNumber);



#pragma swi_number=144
__swi __arm int truncate(int FileHandler, int length, int *errornumber);



#pragma swi_number=145
__swi __arm int isdir(const char * cDirectory, unsigned int *ErrorNumber);



#pragma swi_number=146
__swi __arm void *calloc(size_t nelem, size_t elsize);



#pragma swi_number=148
__swi __arm int ExecuteFile (WSHDR *filepath, WSHDR *mimetype, void *param);



#pragma swi_number=149
__swi __arm int UnRegExplorerExt(REGEXPLEXT const *);



#pragma swi_number=154
__swi int ChangeVolume(unsigned char Volume);





#pragma swi_number=163
__swi __arm void ws_2str(WSHDR *ws, char *str, unsigned int size);



#pragma swi_number=166
__swi __arm void ResetGPRSTraffic();



#pragma swi_number=170
__swi __arm char *GetCurrentLocale();




#pragma swi_number=171
__swi __arm void loopback171();



#pragma swi_number=180
__swi __arm void GetDateTime(TDate *, TTime *);




#pragma swi_number=181
__swi __arm char GetWeek(TDate*);



#pragma swi_number=182
__swi __arm char GetProfile();



#pragma swi_number=183
__swi __arm char SetProfile(char);



#pragma swi_number=184
__swi __arm int sdiv(int denom,int number);



#pragma swi_number=167
__swi __arm char IsGPRSEnabled();



#pragma swi_number=168
__swi __arm char IsGPRSConnected();



#pragma swi_number=169
__swi __arm void KbdLock();



#pragma swi_number=185
__swi __arm void KbdUnlock();



#pragma swi_number=186
__swi __arm void *realloc(void *ptr, int size);



#pragma swi_number=187
__swi __arm void *memset(void *s, int c, int n);



#pragma swi_number=188
__swi __arm int udiv(unsigned int denom,unsigned int number);



#pragma swi_number=189
__swi __arm  int DivBy10(int divident);



#pragma swi_number=0x00BF
__swi __arm void SpellTime();

#pragma swi_number=0x00C4
__swi __arm void *GetBuffer(int patch_id);


#pragma swi_number=198
__swi __arm  char GetAkkuCapacity();




#pragma swi_number=0x088
__swi	__arm char GetAccessoryType(void);



#pragma swi_number=0x93
__swi __arm void RebootPhone();



#pragma swi_number=0x80C8
__swi __arm unsigned short *RamCap();


#pragma swi_number=0x80C9
__swi __arm unsigned short *RamLS();

#pragma swi_number=0x80CA
__swi __arm  RAMNET * RamNet();


#pragma swi_number=0x80CC
__swi __arm char * RamMissedCalls();

#pragma swi_number=0x80CD
__swi __arm char * RamMissedSMS();

#pragma swi_number=0x80D4
__swi __arm char * RamMissedMessage();

#pragma swi_number=0x80DF
__swi __arm char *RamPressedKey();

#pragma swi_number=0x80E0
__swi __arm void * RamScreenBuffer();

#pragma swi_number=0x80E1
__swi	__arm	 unsigned int *RamMPlayer_CtrlCode();



#pragma swi_number=0x80E4
__swi __arm void *FreeRAM();

#pragma swi_number=0x80F1
__swi __arm void *BuildCanvas(void);

#pragma swi_number=0x80FF
__swi __arm unsigned int AddrLibrary();

#pragma swi_number=0x0100
__swi __arm void GBS_SendMessage(int cepid_to, int msg, ...); 



#pragma swi_number=0x0101
__swi __arm int GBS_ReciveMessage(GBS_MSG *);



#pragma swi_number=0x0102
__swi __arm void GBS_AcceptMessage(void);



#pragma swi_number=0x0103
__swi __arm void GBS_ClearMessage(void);



#pragma swi_number=0x0104
__swi __arm void GBS_PendMessage(GBS_MSG *);



#pragma swi_number=0x0105
__swi __arm void CreateGBSproc(int cepid, const char *name, void (*onMsg)(void), int prio, int unk_zero);



#pragma swi_number=0x8106
__swi __arm CSMROOT *CSM_root();

#pragma swi_number=0x0107
__swi __arm int CreateCSM(const CSM_DESC*,void *,int);



#pragma swi_number=0x0108
__swi	__arm	CSM_RAM * FindCSMbyID (int id);



#pragma swi_number=0x0109
__swi __arm void DoIDLE(void);



#pragma swi_number=0x010A
__swi __arm void *GetConfig(unsigned int id); 

#pragma swi_number=0x810B
__swi __arm void *GetMinAdrScan(); 


#pragma swi_number=0x810C
__swi __arm void *GetMaxAdrScan(); 


#pragma swi_number=0x810D
__swi __arm unsigned short *GetCI();

#pragma swi_number=0x010E
__swi __arm void Vibration(unsigned int power, unsigned int duration); 

#pragma swi_number=0x010F
__swi __arm void freeUSSD();



#pragma swi_number=0x0110
__swi __arm char *wstrcopy(unsigned short *dst, const unsigned short *src);



#pragma swi_number=0x0111
__swi __arm char GetTypeUSSD();



#pragma swi_number=0x0112
__swi __arm unsigned int SetUSSD(StructUSSDStr *);



#pragma swi_number=0x0113
__swi	__arm	char * strpbrk (const char *s1,const char *s2);

#pragma swi_number=0x0113
__swi __arm int strcspn(const char *s1, const char *s2);



#pragma swi_number=0x0114
__swi	__arm	char * strncat (char *dest,const char *substr,int maxSubLen);



#pragma swi_number=0x0115
__swi	__arm	int  strncmp (const char *s1,const char *s2,int n);



#pragma swi_number=0x0116
__swi	__arm	int  strncpy (char *dest,const char *source,int maxlen);



#pragma swi_number=0x0117
__swi	__arm	char * strrchr (const char *s,int c);



#pragma swi_number=0x0118
__swi	__arm	char * strstr (const char *s1,const char *s2);















#pragma swi_number=0x011B
__swi	__arm	int snprintf (char *,int n,const char *format,...);



#pragma swi_number=0x011C
__swi	__arm	int  memcmp (const void *m1,const void *m2,int n);



#pragma swi_number=0x011D
__swi	__arm	void  zeromem (void *dest,int n);




#pragma swi_number=0x011E
__swi	__arm	void * memcpy (void *dest,const void *source,int cnt);



#pragma swi_number=0x011F
__swi	__arm	WSHDR * wstrcpy (WSHDR *wshdr_d,WSHDR *wshdr_s);



#pragma swi_number=0x0120
__swi	__arm	WSHDR * wstrncpy (WSHDR *,WSHDR *,int);



#pragma swi_number=0x0121
__swi	__arm	WSHDR * wstrcat (WSHDR *wshdr_d,WSHDR *wshdr_s);



#pragma swi_number=0x0122
__swi	__arm	WSHDR * wstrncat (WSHDR *wshdr_d,WSHDR *wshdr_s,int n);



#pragma swi_number=0x0123
__swi	__arm	int  wstrlen (WSHDR *wshdr);



#pragma swi_number=0x0124
__swi	__arm	int wsprintf (WSHDR *,const char *format,...);



#pragma swi_number=0x0125
__swi	__arm	WSHDR * AllocWS (int len);



#pragma swi_number=0x0126
__swi	__arm	void  CutWSTR (void *WSHDR,int len);



#pragma swi_number=0x0127
__swi	__arm	WSHDR * CreateLocalWS (WSHDR *wshdr,unsigned short *wsbody,int len);



#pragma swi_number=0x0128
__swi	__arm	int  CreateWS (void *malloc,void *mfree,int len);



#pragma swi_number=0x0129
__swi	__arm	void  FreeWS (WSHDR *wshdr);




#pragma swi_number=0x012A
__swi	__arm	void  DrawObject (DRWOBJ *);



#pragma swi_number=0x012B
__swi	__arm	void  AddKeybMsgHook (void *proc);



#pragma swi_number=0x012C
__swi	__arm	int  AddKeybMsgHook_end (void *proc);



#pragma swi_number=0x012D
__swi	__arm	void  RemoveKeybMsgHook (void *proc);



#pragma swi_number=0x012E
__swi	__arm	char *GetPaletteAdrByColorIndex (int ColorIndex);



#pragma swi_number=0x012F
__swi	__arm	void  getRGBcolor (int ColorIndex,void *dest);



#pragma swi_number=0x0130
__swi	__arm	void  getRGBbyPaletteAdr (int *paletteAdr,void *dest);



#pragma swi_number=0x0131
__swi	__arm	void  setColor (int a,int r,int g,int b,void *dest);



#pragma swi_number=0x0132
__swi __arm void *memmove(void *dest,const void *source,int cnt);




#pragma swi_number=0x0133
__swi	__arm	void  StoreXYWHtoRECT (void *RECT,int x,int y,int w,int h);



#pragma swi_number=0x0134
__swi	__arm	void  StoreXYXYtoRECT (void *RECT,int x,int y,int x2,int y2);



#pragma swi_number=0x0135
__swi	__arm	int  IsGuiOnTop (int id);



#pragma swi_number=0x0136
__swi	__arm	int  CreateGUI_ID (void *gui,int id);



#pragma swi_number=0x0137
__swi	__arm	int  CreateGUI (void *gui);



#pragma swi_number=0x0138
__swi	__arm	int  CreateGUI_30or2 (void *gui);



#pragma swi_number=0x0139
__swi	__arm	int  CreateGUIWithDummyCSM (void *gui,int flag);



#pragma swi_number=0x013A
__swi	__arm	int  CreateGUIWithDummyCSM_30or2 (void *gui,int flag);



#pragma swi_number=0x013B
__swi	__arm	void  GeneralFuncF1 (int cmd);



#pragma swi_number=0x013C
__swi	__arm	void  GeneralFuncF0 (int cmd);

#pragma swi_number=0x013D
__swi	__arm	void  GeneralFunc_flag1 (int id,int cmd);

#pragma swi_number=0x013E
__swi	__arm	void  GeneralFunc_flag0 (int id,int cmd);



#pragma swi_number=0x013F
__swi	__arm	void DirectRedrawGUI (void);



#pragma swi_number=0x0140
__swi	__arm	void  DirectRedrawGUI_ID (int id);



#pragma swi_number=0x0141
__swi	__arm	void PendedRedrawGUI (void);




#pragma swi_number=0x0142
__swi	__arm	CSM_RAM * FindCSM (void *csm_q,int id);



#pragma swi_number=0x0143
__swi	__arm	void  GBS_StartTimer (GBSTMR *tmr,int time,int msg,int unk,int cepid);



#pragma swi_number=0x0144
__swi	__arm	void  GBS_StopTimer (GBSTMR *tmr);



#pragma swi_number=0x0145
__swi	__arm	int GBS_GetCurCepid (void);



#pragma swi_number=0x0146
__swi	__arm	void LockSched(void);



#pragma swi_number=0x0147
__swi	__arm	void UnlockSched(void);



#pragma swi_number=0x0148
__swi	__arm	extern int ShowMSG(int flag, int lgp_id);







#pragma swi_number=0x0149
__swi	__arm	void SetPropTo_Obj1(DRWOBJ *drwobj,void *rect,int rect_flag,WSHDR *wshdr,int font,int text_flag);



#pragma swi_number=0x014A
__swi	__arm	void FreeDrawObject_subobj(DRWOBJ *drwobj);



#pragma swi_number=0x014B
__swi	__arm	void SetColor(DRWOBJ *drwobj, const char *color1, const char *color2);



#pragma swi_number=0x014C
__swi	__arm	void DrawString(WSHDR *WSHDR,int x1,int y1,int x2,int y2,int font,int text_attribute,const char *Pen,const char *Brush);



#pragma swi_number=0x014D
__swi	__arm	GUI *GetTopGUI(void);



#pragma swi_number=0x014E
__swi	__arm	void *FindItemByID(GUI *gui,int id);



#pragma swi_number=0x014F
__swi	__arm	void *GetDataOfItemByID(GUI *gui,int id);



#pragma swi_number=0x0150
__swi	__arm	void DrawRoundedFrame(int x1,int y1,int x2,int y2,int x_round,int y_round,int flags,const char *pen,const char *brush);



#pragma swi_number=0x0151
__swi	__arm	void SetPropTo_Obj5(DRWOBJ *, RECT *, int zero, IMGHDR *Image);



#pragma swi_number=0x0152
__swi	__arm	int CreateMenu(int is_small,int zero1,const MENU_DESC *menu, const HEADER_DESC *hdr,int start_item,int n_items,void *user_pointer,int *to_remove);



#pragma swi_number=0x0153
__swi	__arm	int GBS_RecActDstMessage(GBS_MSG *);



#pragma swi_number=0x0154
__swi	__arm	void KillGBSproc(int cepid);








#pragma swi_number=0x0155
__swi __arm png_structp png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn);



#pragma swi_number=0x0156
__swi __arm png_infop png_create_info_struct(png_structp png_ptr);



#pragma swi_number=0x0157
__swi __arm void png_destroy_read_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr);



#pragma swi_number=0x0158
__swi __arm void png_set_sig_bytes(png_structp png_ptr,int num_bytes);



#pragma swi_number=0x0159
__swi __arm void png_read_info(png_structp png_ptr, png_infop info_ptr);



#pragma swi_number=0x015A
__swi __arm png_uint_32 png_get_IHDR(png_structp png_ptr,png_infop info_ptr, png_uint_32 *width, png_uint_32 *height, int *bit_depth, int *color_type, int *interlace_method, int *compression_method, int *filter_method);



#pragma swi_number=0x015B
__swi __arm png_voidp png_malloc(png_structp png_ptr, png_uint_32 size);



#pragma swi_number=0x015C
__swi __arm void png_read_image(png_structp png_ptr, png_bytepp image);



#pragma swi_number=0x015D
__swi __arm void png_read_end(png_structp png_ptr, png_infop info_ptr);



#pragma swi_number=0x015E
__swi __arm png_voidp png_get_io_ptr(png_structp png_ptr);



#pragma swi_number=0x015F
__swi __arm png_uint_32 png_get_rowbytes(png_structp png_ptr,png_infop info_ptr);



#pragma swi_number=0x0160
__swi __arm void png_read_row(png_structp png_ptr,png_bytep row,png_bytep dsp_row);



#pragma swi_number=0x0161
__swi __arm void png_init_io(png_structp png_ptr, int fp);







#pragma swi_number=0x0162
__swi __arm int setjmp(jmp_buf jmpbuf);





#pragma swi_number=0x0163
__swi __arm EDITQ *AllocEQueue(void *malloc_a, void *mfree_a);



#pragma swi_number=0x0164
__swi __arm EDITCONTROL *PrepareEditControl(EDITCONTROL *);



#pragma swi_number=0x0165
__swi __arm void ConstructEditControl(EDITCONTROL *EditControl,int type,int flag,WSHDR *ws,int maxlen);



#pragma swi_number=0x0166
__swi __arm int AddEditControlToEditQend(void *EditQ,EDITCONTROL *EditControl,void *malloc_a);



#pragma swi_number=0x0167
__swi __arm int CreateInputTextDialog(const INPUTDIA_DESC *input_desc, const HEADER_DESC *header_desc,void *editq,int do_mfree,void *user_pointer);




#pragma swi_number=0x0168
__swi __arm void SetSoftKey(void *gui,const SOFTKEY_DESC *,int n);



#pragma swi_number=0x0169
__swi __arm int ExtractEditControl(void *guidata,int n,EDITCONTROL *);


#pragma swi_number=0x016A
__swi __arm int StoreEditControl(void *guidata,int n,EDITCONTROL *);

#pragma swi_number=0x016B
__swi __arm int unlink(const char *cFileName,unsigned int *errornumber);



#pragma swi_number=0x016C
__swi __arm int str_2ws(WSHDR *ws,const char *str,unsigned int size);



#pragma swi_number=0x016D
__swi __arm int UnpackABentry(AB_UNPRES *,void *abentry,int filesz,int flag);



#pragma swi_number=0x016E
__swi __arm int FreeUnpackABentry(AB_UNPRES *, void *mfree_adr);



#pragma swi_number=0x016F
__swi __arm int GetTypeOfAB_UNPRES_ITEM(int);



#pragma swi_number=0x0170
__swi __arm int MakeVoiceCall(const char *number,int _0x10, int _0x20C0);



#pragma swi_number=0x0171
__swi __arm void SUBPROC(void *,...); 

#pragma swi_number=0x0172
__swi __arm void REDRAW(void);




#pragma swi_number=0x0173
__swi __arm int socket(int af,int type,int protocol);

#pragma swi_number=0x0174
__swi __arm int connect(int sock,SOCK_ADDR *,int name_length);

#pragma swi_number=0x0175
__swi __arm int bind(int sock,SOCK_ADDR *,int name_length);

#pragma swi_number=0x0176
__swi __arm int closesocket(int socket);

#pragma swi_number=0x0177
__swi __arm int shutdown(int socket,int how);

#pragma swi_number=0x0178
__swi __arm int recv(int socket,void *buf,int len,int flag);

#pragma swi_number=0x0179
__swi __arm int send(int socket,const void *buf,int len,int flag);

#pragma swi_number=0x017A
__swi __arm unsigned short htons(unsigned short);

#pragma swi_number=0x017B
__swi __arm unsigned int htonl(unsigned int);

#pragma swi_number=0x817C
__swi __arm int *socklasterr(void);




#pragma swi_number=0x017D
__swi __arm void SetIDLETMR(int time_ms,int msg);



#pragma swi_number=0x017E
__swi __arm void RestartIDLETMR(void);



#pragma swi_number=0x017F
__swi __arm void DisableIDLETMR(void);



#pragma swi_number=0x0180
__swi __arm int EDIT_GetItemNumInFocusedComboBox(void *gui);



#pragma swi_number=0x0181
__swi __arm void EDIT_SetTextToFocused(void *gui,WSHDR *ws);



#pragma swi_number=0x0182
__swi __arm int EDIT_GetFocus(void *gui);



#pragma swi_number=0x0183
__swi __arm int EDIT_GetUnFocus(void *gui);



#pragma swi_number=0x0184
__swi __arm void ConstructComboBox(EDITCONTROL *EditControl,int type,int attr,WSHDR*,int maxlen,int unk_zero,int num_of_items,int start_item);



#pragma swi_number=0x0185
__swi __arm int GBS_WaitForMsg(const int *msg_list, int msgs_num, GBS_MSG*, int timeout);



#pragma swi_number=0x0186
__swi __arm int RegisterCepidForSocketWork(REGSOCKCEPID_DATA *);



#pragma swi_number=0x0187
__swi __arm int RequestLMANConnect(LMAN_DATA *);



#pragma swi_number=0x8188
__swi __arm int ScreenW(void);


#pragma swi_number=0x8189
__swi __arm int ScreenH(void);


#pragma swi_number=0x818A
__swi __arm int HeaderH(void);


#pragma swi_number=0x818B
__swi __arm int SoftkeyH(void);


#pragma swi_number=0x018C
__swi __arm void GBS_DelTimer(GBSTMR *);



#pragma swi_number=0x018D
__swi __arm int available0(int sock);



#pragma swi_number=0x018E
__swi __arm void GPRS_OnOff(int on, int _1);



#pragma swi_number=0x018F
__swi __arm int GetCurMenuItem(void *gui);



#pragma swi_number=0x0190
__swi __arm void MutexCreate(MUTEX *mtx);



#pragma swi_number=0x0191
__swi __arm void MutexDestroy(MUTEX *mtx);



#pragma swi_number=0x0192
__swi __arm void MutexLock(MUTEX *mtx);



#pragma swi_number=0x0193
__swi __arm void MutexLockEx(MUTEX *mtx, int flag);



#pragma swi_number=0x0194
__swi __arm void MutexUnlock(MUTEX *mtx);



#pragma swi_number=0x0195
__swi __arm void SetMenuItemIcon(void *gui,int item_n,int icon_n);



#pragma swi_number=0x0196
__swi __arm void RefreshGUI(void);



#pragma swi_number=0x0197
__swi __arm void *AllocMenuItem(void *gui);



#pragma swi_number=0x0198
__swi __arm WSHDR *AllocMenuWS(void *gui, int len);



#pragma swi_number=0x0199
__swi __arm void SetMenuItemIconArray(void *gui,void *item,const int *icons);



#pragma swi_number=0x019A
__swi __arm void SetMenuItemText(void *gui,void *item,WSHDR *text,int item_n);



#pragma swi_number=0x019B
__swi __arm void EDIT_SetFocus(void *gui, int item);



#pragma swi_number=0x019C
__swi __arm void SEQKILLER(void *data, void(*next_in_seq)(void *), void *data_to_kill);
#pragma swi_number=0x819C
__swi __arm void *SEQKILLER_ADR();


#pragma swi_number=0x019D
__swi __arm long long getCpuUsedTime_if_ena(void);



#pragma swi_number=0x019E
__swi __arm void StartCpuUsageCount(void);




#pragma swi_number=0x019F
__swi __arm int inflateInit2_ (z_streamp strm, int windowBits,const char *version, int stream_size);



#pragma swi_number=0x01A0
__swi __arm int inflate (z_streamp strm, int flush);



#pragma swi_number=0x01A1
__swi __arm int inflateEnd (z_streamp strm);




#pragma swi_number=0x01A2
__swi __arm int ActivateDialUpProfile(unsigned int cep_id,int profile_num);



#pragma swi_number=0x01A3
__swi __arm int GetCurrentGPRSProfile(void);



#pragma swi_number=0x01A4
__swi __arm int GetDisconnectTime(void);



#pragma swi_number=0x01A5
__swi __arm void RegisterCepIdForCurProfile(unsigned int cep_id,int profile_num,unsigned int csd_or_gprs);



#pragma swi_number=0x01A6
__swi __arm int IsTimerProc(GBSTMR *);



#pragma swi_number=0x1A7
__swi __arm char* Get_CC_NC();







#pragma swi_number=0x1A8
__swi __arm png_structp png_create_read_struct_2(png_const_charp user_png_ver, png_voidp error_ptr,
                                                 png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr,
                                                 png_malloc_ptr malloc_fn, png_free_ptr free_fn);



#pragma swi_number=0x1A9
__swi __arm void png_set_read_fn(png_structp png_ptr, png_voidp io_ptr, png_rw_ptr read_data_fn);



#pragma swi_number=0x1AA
__swi __arm void png_read_update_info(png_structp png_ptr,void *info_ptr);



#pragma swi_number=0x1AB
__swi __arm void png_set_palette_to_rgb(png_structp png_ptr);



#pragma swi_number=0x1AC
__swi __arm png_uint_32 png_get_valid(png_structp png_ptr,png_infop info_ptr, png_uint_32 flag);



#pragma swi_number=0x1AD
__swi __arm void png_set_tRNS_to_alpha(png_structp png_ptr);



#pragma swi_number=0x1AE
__swi __arm void png_set_filler(png_structp png_ptr,png_uint_32 filler, int flags);



#pragma swi_number=0x1AF
__swi __arm void png_set_strip_16(png_structp png_ptr);



#pragma swi_number=0x1B0
__swi __arm void png_set_packing(png_structp png_ptr);



#pragma swi_number=0x1B1
__swi __arm void png_set_gray_to_rgb(png_structp png_ptr);



#pragma swi_number=0x1B2
__swi __arm int png_check_sig(png_bytep sig, int num);






#pragma swi_number=0x1B3
__swi __arm void DrawLine(int x,int y,int x2,int y2,int type,const char *pen);



#pragma swi_number=0x1B4
__swi __arm void DrawRectangle(int x,int y,int x2,int y2,int flags,const char *pen,const char *brush);



#pragma swi_number=0x81B5
__swi int isnewSGold(void);


#pragma swi_number=0x1B6
__swi __arm int async_gethostbyname(const char *host_name,int ****DNR_RES,int *DNR_ID);




#pragma swi_number=0x1B7
__swi __arm unsigned int str2ip(const char *);




#pragma swi_number=0x81B8
__swi __arm void *EXT_TOP(void);

#pragma swi_number=0x81B9
__swi __arm void *PNG_TOP(void);

#pragma swi_number=0x81BA
__swi __arm void *LIB_TOP(void);

#pragma swi_number=0x81BB
__swi __arm SFN_DATA *DATA_N_SFB(void);





#pragma swi_number=0x1BC
__swi __arm void SHA1_Init(SHA_CTX *c);



#pragma swi_number=0x1BD
__swi __arm void SHA1_Update(SHA_CTX *c, const void *data, unsigned long len);



#pragma swi_number=0x1BE
__swi __arm void SHA1_Final(char *md, SHA_CTX *c);



#pragma swi_number=0x1BF
__swi __arm RSA * RSA_new_method(RSA_METHOD *method);



#pragma swi_number=0x1C0
__swi __arm void RSA_free (RSA *r);



#pragma swi_number=0x1C1
__swi __arm int RSA_sign(int type, char *m, unsigned int m_len, char *sigret, unsigned int *siglen, RSA *rsa);



#pragma swi_number=0x1C2
__swi __arm BIGNUM *BN_bin2bn(const char *s,int len,BIGNUM *ret);






#pragma swi_number=0x1C3
__swi __arm void SetMenuItemCount(void *gui,int n);




#pragma swi_number=0x1C4
__swi __arm void SetCursorToMenuItem(void *gui,int n);




#pragma swi_number=0x1C5
__swi __arm int GetFontYSIZE(int font);




#pragma swi_number=0x1C6
__swi __arm void EDIT_ExtractFocusedControl(void *gui,EDITCONTROL *ec);



#pragma swi_number=0x1C7
__swi __arm int EDIT_StoreControlToFocused(void *gui,EDITCONTROL *ec);



#pragma swi_number=0x1C8
__swi __arm EDITC_OPTIONS *PrepareEditCOptions(EDITC_OPTIONS *ec_options);



#pragma swi_number=0x1C9
__swi __arm void SetInvertToEditCOptions(EDITC_OPTIONS *ec_options, int invert);



#pragma swi_number=0x1CA
__swi __arm void SetFontToEditCOptions(EDITC_OPTIONS *ec_options, int font);



#pragma swi_number=0x1CB
__swi __arm void SetAlignToEditCOptions(EDITC_OPTIONS *ec_options, int align);



#pragma swi_number=0x1CC
__swi __arm void SetPenColorToEditCOptions(EDITC_OPTIONS *ec_options, int pen);



#pragma swi_number=0x1CD
__swi __arm void SetBrushColorToEditCOptions(EDITC_OPTIONS *ec_options, int brush);



#pragma swi_number=0x1CE
__swi __arm void CopyOptionsToEditControl(EDITCONTROL *ec,EDITC_OPTIONS *ec_options);




#pragma swi_number=0x1CF
__swi __arm REGEXPLEXT *get_regextpnt_by_uid(int uid);





#pragma swi_number=0x1D0
__swi __arm int GetExtUid_ws(WSHDR * ext);





#pragma swi_number=0x1D1
__swi __arm void EDIT_RemoveEditControl(void *gui,int n);



#pragma swi_number=0x1D2
__swi __arm void EDIT_InsertEditControl(void *gui,int n,EDITCONTROL *);



#pragma swi_number=0x1D3
__swi __arm int EDIT_GetCursorPos(void *gui);



#pragma swi_number=0x1D4
__swi __arm void EDIT_SetCursorPos(void *gui, int pos);



#pragma swi_number=0x1D5
__swi __arm char *Get_Phone_Info(int InfoIndex);



#pragma swi_number=0x1D6
__swi __arm void png_set_gray_1_2_4_to_8(png_structp png_ptr);



#pragma swi_number=0x1D7
__swi __arm void png_set_invert_mono(png_structp png_ptr);



#pragma swi_number=0x1D8
__swi __arm int EDIT_IsMarkModeActive(void *gui);



#pragma swi_number=0x1D9
__swi __arm int EDIT_GetMarkedText(void *gui,WSHDR *dest);



#pragma swi_number=0x1DA
__swi __arm void SendSMS(WSHDR *ws,const char *num,int cepid,int msg,int flag);



#pragma swi_number=0x81DB
__swi __arm PDU *IncommingPDU(void);


#pragma swi_number=0x1DC
__swi __arm short wstrchr(WSHDR *ws,unsigned int start_pos,unsigned int wchar);



#pragma swi_number=0x1DD
__swi __arm short wstrrchr(WSHDR *ws,unsigned int max_pos,unsigned int wchar);



#pragma swi_number=0x1DE
__swi __arm int EDIT_SetTextToEditControl(void *gui,int n,WSHDR *ws);



#pragma swi_number=0x1DF
__swi __arm void *EDIT_GetUserPointer(void *gui);







#pragma swi_number=0x1E0
__swi __arm void *MenuGetUserPointer(void *gui);





#pragma swi_number=0x1E1
__swi __arm int GetSymbolWidth(int wchar,int font);





#pragma swi_number=0x1E2
__swi __arm  int ws_2utf8( WSHDR *from, char *to , int *result_length, int max_len);



#pragma swi_number=0x1E3
__swi __arm int utf8_2ws(WSHDR *ws,const char *utf8_str, unsigned int maxLen);



#pragma swi_number=0x1E4
__swi __arm int GetPicNByUnicodeSymbol(int wchar);




#pragma swi_number=0x1E5
__swi __arm int EDIT_OpenOptionMenuWithUserItems(void *gui, void (*itemhandler)(USR_MENU_ITEM *item), void *user_pointer, int to_add);



#pragma swi_number=0x1E6
__swi __arm void Menu_SetItemCountDyn(void *gui, int n);





#pragma swi_number=0x1E7
__swi __arm char *GetLastJavaApplication();



#pragma swi_number=0x1E8
__swi __arm int IsNoJava();




#pragma swi_number=0x1E9
__swi __arm IMGHDR *CreateIMGHDRFromPngFile(const char *fname, int type);

#pragma swi_number=0x1EA
__swi __arm int GetMaxDynIcon(void);





#pragma swi_number=0x81EB
__swi __arm char *RAM_IMSI(void);


#pragma swi_number=0x1EC
__swi __arm void FreeDynIcon(int picture);





#pragma swi_number=0x1ED
__swi __arm int IsDynIconBusy(int picture);





#pragma swi_number=0x1EF
__swi __arm void SetDynIcon(int picture,IMGHDR *img,char *bitmap);





#pragma swi_number=0x1F3
__swi __arm unsigned int GetSessionAge(void);



#pragma swi_number=0x1F5
__swi __arm int sscanf(const char *, const char *, ...);



#pragma swi_number=0x1F6
__swi __arm IMGHDR* GetPitAdrBy0xE200Symbol(int uni_symbol);





#pragma swi_number=0x1F8
__swi __arm int GetPlayStatus(void);






#pragma swi_number=0x81FB
__swi __arm char *RamMediaIsPlaying();



#pragma swi_number=0x1FC
__swi __arm void CloseCSM(int id);





#pragma swi_number=0x201
__swi __arm void SetProp2ImageOrCanvas(DRWOBJ *, RECT *, int zero, IMGHDR *Image, int bleed_x, int bleed_y);



#pragma swi_number=0x202
__swi __arm void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int flag, char *pen, char *brush);



#pragma swi_number=0x203
__swi __arm void SetPixel(int x,int y,char *color);



#pragma swi_number=0x204
__swi __arm void drawArc(int x1,int y1,int x2,int y2,int a1,int a2,int flag,char *pen,char *brush);



#pragma swi_number=0x205
__swi __arm void SetProp2Square_v1(void);



#pragma swi_number=0x206
__swi __arm void srand(unsigned int seed);



#pragma swi_number=0x207
__swi __arm void DrawScrollString(WSHDR *WSHDR,int x1,int y1,int x2,int y2,int xdisp,int font,int text_attribute,const char *Pen,const char *Brush);



#pragma swi_number=0x208
__swi __arm int Get_WS_width(WSHDR *ws, int font);





#pragma swi_number=0x209
__swi __arm void *FindGUIbyId(int id, CSM_RAM **csm);







#pragma swi_number=0x20A
__swi __arm void EDIT_SetDate(void *gui,int n,TDate *dd);




#pragma swi_number=0x20B
__swi __arm void EDIT_SetTime(void *gui,int n,TTime *tt);



#pragma swi_number=0x20C
__swi __arm void EDIT_GetDate(void *gui,int n,TDate *dd);




#pragma swi_number=0x20D
__swi __arm void EDIT_GetTime(void *gui,int n,TTime *tt);




#pragma swi_number=0x20E
__swi __arm int ConstructEditTime(EDITCONTROL *EditControl,TTime *time);



#pragma swi_number=0x20F
__swi __arm int ConstructEditDate(EDITCONTROL *EditControl,TDate *date);



#pragma swi_number=0x212
__swi __arm int CardExplGetCurItem(void *csm);



#pragma swi_number=0x213
__swi __arm void CardExplGetFName(void *csm, int cur_item, WSHDR *dest);



#pragma swi_number=0x214
__swi __arm void wstrcpybypos(WSHDR *dest,WSHDR *src,int from,int len);



#pragma swi_number=0x215
__swi __arm void wsRemoveChars(WSHDR *ws,int from,int to);





#pragma swi_number=0x216
__swi __arm void wstrcatprintf(WSHDR *,const char *format,...);



#pragma swi_number=0x217
__swi __arm int CreateMultiLinesMenu(int dummy,int dummy2,const ML_MENU_DESC *,const HEADER_DESC *,int to_item,int n);





#pragma swi_number=0x218
__swi __arm void SetMLMenuItemText(void *data,void *item,WSHDR *ws1,WSHDR *ws2,int n);





#pragma swi_number=0x219
__swi __arm void *AllocMLMenuItem(void *data);





#pragma swi_number=0x21A
__swi __arm int EDIT_SetTextInvert(void *gui,int cursor_pos,int len);





#pragma swi_number=0x21B
__swi __arm void EDIT_RemoveTextInvert(void *gui);





#pragma swi_number=0x21C
__swi __arm int EDIT_IsBusy(void *gui);



#pragma swi_number=0x21D
__swi __arm int GSMTXT_Decode(WSHDR *dest,void *pkt,int len,int encoding_type, void *(*malloc_adr)(int), void (*mfree_adr)(void));



#pragma swi_number=0x21E
__swi __arm void FocusGUI(int id);







#pragma swi_number=0x21F
__swi __arm void UnfocusGUI(void);







#pragma swi_number=0x225
__swi __arm int PlayMelody_ChangeVolume(int handle,int volume);





#pragma swi_number=0x226
__swi __arm int PlayMelody_StopPlayback(int handle);





#pragma swi_number=0x227
__swi __arm int PlayMelody_PausePlayback(int handle);





#pragma swi_number=0x228
__swi __arm int PlayMelody_ResumePlayBack(int handle);





#pragma swi_number=0x229
__swi __arm void longjmp(jmp_buf, int);



#pragma swi_number=0x22A
__swi __arm void FreeGSMTXTpkt(void *pkt);





#pragma swi_number=0x8237
__swi __arm char *RamIconBar();



#pragma swi_number=0x822F
__swi __arm  TDateTimeSettings *RamDateTimeSettings(void);


#pragma swi_number=0x230
__swi __arm int GetTimeZoneShift(TDate *, TTime *, int timeZone);


#pragma swi_number=0x238
__swi __arm void SendAutoUSSDack(void);



#pragma swi_number=0x239
__swi __arm int EEFullDeleteBlock(unsigned int block,int cepid,int msg); 





#pragma swi_number=0x23A
__swi __arm  int EEFullCreateBlock(unsigned int block,int size,int version,int cepid,int msg);





#pragma swi_number=0x23B
__swi __arm void DisableIconBar(int disable); 




 
#pragma swi_number=0x23C
__swi __arm int SetCurrentGPRSProfile(int profile);





#pragma swi_number=0x23D
__swi __arm void MD5_Init(MD5_CTX *c);



#pragma swi_number=0x23E
__swi __arm void MD5_Update(MD5_CTX *c, const void *data, unsigned long len); 



#pragma swi_number=0x23F
__swi __arm void MD5_Final(unsigned char *md, MD5_CTX *c);



#pragma swi_number=0x240
__swi __arm char *MD5_Hash(const unsigned char *data, unsigned long n, unsigned char *md); 



#pragma swi_number=0x241
__swi __arm void SetDateTime(TDate *, TTime *);



#pragma swi_number=0x246
__swi __arm int GetFileProp(FILE_PROP *wl,WSHDR *file,WSHDR *folder); 






#pragma swi_number=0x24B
__swi __arm void SetMenuSearchCallBack(void *gui, MenuSearchCallBack proc);



#pragma swi_number=0x24C
__swi __arm int  GetPeripheryState (int device, int check_state); 




#pragma swi_number=0x24D
__swi __arm int PlayMelody_SetPosition(int handle, int pos_ms);




#pragma swi_number=0x24E
__swi __arm void* GetPlayObjById(int handle);



#pragma swi_number=0x24F
__swi __arm int GetPlayObjPosition(void *playobj_3D0, int *pos_ms);


#pragma swi_number=0x250
__swi __arm int GetPlayObjDuration(void *playobj_3D0, int *pos_ms);


#pragma swi_number=0x254
__swi __arm int StartNativeExplorer(NativeExplorerData* data);





#pragma swi_number=0x255
__swi __arm void MenuSetUserPointer(void *gui,void *user_pointer);



#pragma swi_number=0x258
__swi __arm int CalcBitmapSize(short w,short h, char typy);



typedef int HObj; 

#pragma swi_number=0x259
__swi __arm HObj Obs_CreateObject(int uid, int unk1, int unk2, int unk3, int unk4, int unk6,unsigned int *ErrorNumber);



#pragma swi_number=0x25A
__swi __arm Obs_DestroyObject(HObj hObj);



#pragma swi_number=0x25B
__swi __arm int Obs_SetInput_File (HObj hObj, int unk_zero, WSHDR *path);



#pragma swi_number=0x25C
__swi __arm int Obs_GetInputImageSize (HObj hObj, short *w, short *h);



#pragma swi_number=0x25D
__swi __arm int Obs_SetOutputImageSize (HObj hObj, short w, short h);



#pragma swi_number=0x25E
__swi __arm int Obs_Start(HObj hObj);




#pragma swi_number=0x25F
__swi __arm int Obs_Output_GetPictstruct(HObj hObj, IMGHDR **img); 



#pragma swi_number=0x260
__swi __arm int Obs_Graphics_SetClipping(HObj hObj, short x, short y, short w, short h);



#pragma swi_number=0x261
__swi __arm int Obs_SetRotation(HObj hObj, int angle);



#pragma swi_number=0x262
__swi __arm int Obs_GetInfo (HObj hObj, int unk_0or1);




#pragma swi_number=0x263
__swi __arm Obs_SetScaling (HObj hObj, int unk5);



#pragma swi_number=0x09E
__swi __arm int GetMissedEventCount(unsigned int Event);








typedef struct
{
  int type;
  char name[32];
  unsigned int min;
  unsigned int max;
}CFG_HDR;

typedef struct
{
  char cbox_text[32];
}CFG_CBOX_ITEM;





typedef struct
{
  void *next;
  char *pngname;
  IMGHDR * img;
}PNGLIST;

typedef struct
{
  void *next;
  int icon;
  IMGHDR * img;
}DYNPNGICONLIST;

typedef struct
{
  PNGLIST * pltop;
  char *bitmap;
  DYNPNGICONLIST *dyn_pltop;
}PNGTOP_DESC;  














extern int DEVELOP_IF_MOD;
extern int SHOW_ACTIVE;
extern int HISTORY_SAVE_TYPE;
extern int HISTORY_BUFFER_MOD;
extern unsigned int extended_sounds;
extern char sndGlobal2[64];
extern char sndMsg2[64];
extern char pingsnd[64];




extern int LongRightAction;
extern int LongLeftAction;
extern int ActionResh;
extern int LongVolUpAction;
extern int LongVolDownAction ;
extern int isVolUpButton;
extern int isVolDownButton;

extern int longAct_0;
extern int longAct_1;
extern int longAct_2;
extern int longAct_3;
extern int longAct_4;
extern int longAct_5;
extern int longAct_6;
extern int longAct_7;
extern int longAct_8;
extern int longAct_9;
extern int longAct_10;
extern int longAct_11;
extern int longAct_RB;




extern int ShowStatusIcon;
extern int show_xstatus;
extern unsigned int IDLEICON_XX;
extern unsigned int IDLEICON_YX;
extern int ind_set_xstatus;
extern unsigned int NumStatusToIconBar;
extern unsigned int NumXStatusToIconBar;
extern unsigned int ADDITION_UIN;
extern int show_add_state;
extern int show_add_xstate;




extern unsigned int fonto;
extern char fontpath[64];
extern unsigned int msg_d_x;
extern unsigned int msg_d_y;
extern unsigned int main_msg_timer;
extern char fontColor[];
extern char bgrColor[];
extern unsigned int main_lenght;
extern int use_cutter;
extern unsigned int spaco1;
extern unsigned int spaco2;
  



extern char SmBGcolor[4];
extern char SmSelectColor[4];
extern char SmFontColor[4];





extern unsigned int onlineTime_x;
extern unsigned int onlineTime_y;
extern int Pause_Time;  




extern unsigned int Ping_X;
extern unsigned int Ping_Y;
extern unsigned int ping_time_config;

extern int Is_Unread_On;
extern unsigned int unread_X;
extern unsigned int unread_Y;

extern int Is_Online_On;
extern unsigned int online_x;
extern unsigned int online_y;  




extern int Auto_Status;
extern int IsOnNAStatus;
extern unsigned int NAStatusTime;   
extern int isAutoStatusOn;
extern unsigned int AUTO_ENGADE;
extern unsigned int automsgcount;
extern int time_zone;
extern char awayHelloMsg[24];
extern char awayMsg[256];
extern int showAwayIdle;  
extern unsigned int auto_x;
extern unsigned int auto_y;
extern int Reset_timer;  




extern TTime tm_status_1;
extern unsigned int status_1;
extern TTime tm_status_2;
extern unsigned int status_2;
extern TTime tm_status_3;
extern unsigned int status_3;
extern TTime tm_status_4;
extern unsigned int status_4;
extern TTime tm_status_5;
extern unsigned int status_5;    
    



extern unsigned int screenSaverTime;
extern int isScreenSaverEna;



extern int PopupShow;
extern char PopupMsgString[9];
extern char PopupOnlineString[9];
extern unsigned int PopupFont;
extern char PopupBGCl[4];
extern char PopupOnlineCl[4];    
extern char PopupMsgCl[4];




extern int TIME_OR_MSG;
extern unsigned int ON_OFF_TIME;
extern int ShowTraff;
extern int isShowCont;
extern int isShowUnread;
extern int isShowI;
extern int isShowV;
extern int isShowS;
extern int isShowR;
extern int isShowA;
extern int isShowTmXs;
extern int isShowTime;
extern int isShowBat;




extern unsigned int Vibra_cc;
extern char playing[13];
extern char CsmAdr[9];
extern int show_send_info;  
extern int isLogStatusChange;
extern int IsShowLoopback;
extern int IsOpenBrowser;
extern int IsMultiBuffer;




extern RECT PBRECT;
extern char PBcolor[4];
extern char BGPBcolor[4];
extern int IsInvertSoftKey;
extern int IsShowInfo;


extern char ignor_list[256];
extern char I_str[32];




extern unsigned int UIN;
extern char PASS[9];

extern int VIBR_ON_CONNECT;
extern int SHOW_ACTIVE;
extern int VIBR_TYPE;
extern int DEVELOP_IF;

extern int LOG_ALL;
extern int LOG_STATCH;
extern int FIRST_LETTER;
extern int SORT_CLIST;


extern int HISTORY_TYPE;
extern unsigned int vibraPower;

extern unsigned int IDLEICON_X;
extern unsigned int IDLEICON_Y;
  
extern unsigned int I_COLOR;
extern unsigned int TO_COLOR;
extern unsigned int X_COLOR;
extern unsigned int O_I_COLOR;
extern unsigned int O_TO_COLOR;
extern unsigned int O_X_COLOR;
  
extern unsigned int ED_FONT_SIZE;
extern unsigned int ED_H_FONT_SIZE;
extern unsigned int ED_X_FONT_SIZE;
extern unsigned int O_ED_FONT_SIZE;
extern unsigned int O_ED_H_FONT_SIZE;
extern unsigned int O_ED_X_FONT_SIZE;
  
extern unsigned int ACK_COLOR;
extern unsigned int UNACK_COLOR;
  
extern int ENA_AUTO_XTXT;
extern int NOT_LOG_SAME_XTXT;
extern int LOG_XTXT;
extern int HISTORY_BUFFER;  
  


extern unsigned int ILL_DISP_RECV;
extern unsigned int ILL_KEYS_RECV;
extern unsigned int ILL_RECV_TMR;
extern unsigned int ILL_DISP_SEND;
extern unsigned int ILL_KEYS_SEND;
extern unsigned int ILL_SEND_TMR;
extern unsigned int ILL_SEND_FADE;
extern unsigned int ILL_RECV_FADE;
extern unsigned int ILL_OFF_FADE;

extern char sndStartup[64];
extern char sndSrvMsg[64];
extern char sndGlobal[64];
extern char sndGlobal2[64];
extern char sndMsg[64];
extern char sndMsgSent[64];
extern unsigned int sndVolume;
  
extern char SMILE_FILE[64];
extern char TEMPLATES_PATH[64];
extern char ICON_PATH[64];
extern char SMILE_PATH[64];
extern char XSTATUSES_PATH[64];
extern char HIST_PATH[64];
extern char NATICQ_HOST[128];
extern unsigned int NATICQ_PORT;
extern unsigned int RECONNECT_TIME;


extern int janis;

extern char suc_config_name[64];
extern char suc_config_name_mod[64];

extern int get_file_size(char* fname);
extern int UpdateConfig();


typedef struct
{
  unsigned int uin;
  char pass[9];
}ADDUIN;





 

typedef struct
{
  void *next;
  int acked;
  unsigned long ID;
  int type;
  char hdr[80]; 
  char text[];
}LOGQ;


typedef struct
{
  void *next;
  void *prev;
  unsigned int isgroup;
  unsigned int uin;
  unsigned int group;
  char name[64];
  unsigned short state;
  unsigned short xstate;
  int isIndividSet;
  int isvibra;
  int issound;
  int isautomsg;
  int just_now;
  int just_i;
  int isShowXst;
  
  int signals;
  int isaccess;  
  int automsgcount;
  
  int change;
  int isunread;
  int unread_msg;
  int req_add;
  int req_drawack;
  LOGQ *log;
  char *answer;
  LOGQ *last_log;
  int isactive;
  int msg_count;
  char *xtext;
  char *xtext2;
  int req_xtext;
  int local;  
}CLIST;


typedef struct
{
  unsigned long uin;
  unsigned short type;
  unsigned short data_len;
}PKT;

typedef struct
{
  PKT pkt;
  char data[16384];
}TPKT;


enum ISTATUS {IS_OFFLINE,IS_INVISIBLE,IS_AWAY,IS_NA,IS_OCCUPIED,IS_DND,IS_DEPRESSION,IS_EVIL,IS_HOME,IS_LUNCH,
IS_WORK,IS_ONLINE,IS_FFC,IS_MSG,IS_UNKNOWN,IS_GROUP,IS_GROUPOFF,IS_NULLICON,

ICON_ADDCONTACT,
ICON_HEAD,
ICON_LOGO,
ICON_PING,
ICON_SETTINGS,

ICON_ALL_CAN_SEE,
ICON_VISLIST_CAN_SEE,
ICON_INVISLIST_CANNOT_SEE,
ICON_CONTACTLIST_CAN_SEE,
ICON_NOBODY_CAN_SEE,

TOTAL_ICONS,

IS_ANY=0xFFFFFFFF,IS_FILTER_T9KEY=0xFFFFFFFE};























 





void Add2History(CLIST *CListEx, char *header, char *message, int direction);
void LogStatusChange(CLIST *CListEx);
int GetHistory(CLIST *t, int bufsize);





 



int InitConfig(void* cfgstruct, int len, char *name,int disk);

extern const char *successed_config_filename;
int SaveConfigData(const char *fname);



void ShowMainMenu();



void SendAnswer(int dummy, TPKT *p);
void CreateEditChat(CLIST *t);
void set_my_status(void);
void set_my_xstatus(void);
void set_my_xstatus2(void);

void RequestXText(unsigned int uin);

void ReadDefSettings(void);
void WriteDefSettings(void);
extern int connect_state;

int GetIconIndex(CLIST *t);


void FreeLOGQ(LOGQ **pp);

void Get_Online_total();
void DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush);

typedef struct
{
  CLIST *ed_contact;
  GUI *ed_chatgui;
  int ed_answer;
  int requested_decrement_total_unread;
  int loaded_templates;
}EDCHAT_STRUCT;

#pragma pack(push)
#pragma pack(1)
typedef struct{
  unsigned char
  vibra_status:1,
  sound_status:1,
  off_contacts,
  show_groups;
  char def_status;
  char def_xstatus;
  unsigned char sli_state:1;
  unsigned char smartcpu_state;
  unsigned char inc_state:1;
  unsigned char ilu_state:1;
  unsigned char remind_state;
  unsigned char ping_state;
  unsigned char uptime_state;
  unsigned char Id3tag_status;
  unsigned char isXTX;
}DEF_SETTINGS;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct{
  unsigned char
  vibra_status:1,
  sound_status:1,
  off_contacts,
  show_groups;
  unsigned char sli_state:1;
  unsigned char smartcpu_state;
  unsigned char inc_state:1;
  unsigned char ilu_state:1;
  unsigned char remind_state;
  unsigned char ping_state;
  unsigned char uptime_state;
  unsigned char Id3tag_status;
  unsigned char isXTX;
}DEF_SETTINGS2;
#pragma pack(pop)

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);


































































 




                                                















#pragma pack(1)
typedef struct
{
  void *next;
  unsigned int key; 
  unsigned int mask;
  char text[1];
}STXT_SMILES;
#pragma pack()

typedef struct
{
  void *next;
  unsigned int uni_smile;
  STXT_SMILES *lines; 
  STXT_SMILES *botlines;
}S_SMILES;

void FreeSmiles(void);
void InitSmiles(void);
void ProcessNextSmile(void);

void FreeXStatusesImg(void);
void InitXStatusesImg(void);
void ProcessNextXStatImg(void);

S_SMILES *FindSmileById(int n);
S_SMILES *FindSmileByUni(int wchar);







typedef struct{
  unsigned long uin;
  char *msg;
}IPCMsg;


void DispStatusChangeMenu();


void DispXStatusChangeMenu(void);


void LoadXStatusText(void);
void SaveXStatusText(void);
void FreeXStatusText(void);
char *GetXStatusStr(int n, int *len);

unsigned int char8to16(int c);
unsigned int char16to8(unsigned int c);
void ascii2ws(WSHDR *ws, const char *s);
char *MakeGlobalString(const char *first, int breakchar, const char *last);
int strcmp_nocase(const char *s, const char *d);
int convWS_to_ANSI(WSHDR *ws, char *buf);

int CreateManageCLMenu(void);
int CreateAddContactGrpDialog(CLIST *cl_sel);
int CreatePrivateStatusMenu(void);
void SetPrivateStatus(int status);



void FreeXText(CLIST *t);
void FreeCLIST(void);
void FillAllOffline(void);

CLIST *FindContactByUin(unsigned int uin);
CLIST *FindGroupByID(unsigned int grp);

CLIST *FindContactByNS(int *i, int si, int act_flag, CLIST *search_contact);
int CountContacts(void);
CLIST *FindContactByN(int i);
int FindContactByContact(CLIST *p);
void SwapContacts(CLIST *first, CLIST *second);
void ChangeContactPos(CLIST *p);
int CompareContacts(CLIST *t, CLIST *p);

void ResortCL(void);
CLIST *AddContactOrGroup(CLIST **top, CLIST *p);
CLIST *AddContact(unsigned int uin, char *name, unsigned int group, int local);
CLIST *AddGroup(unsigned int grp, char *name);

void LoadLocalCL(void);



int CreateSmileSelectGUI(EDCHAT_STRUCT *ed_struct);












extern void* canvasdata;
  
void PrintLibInit(int maxPngFontNum);
void PrintLibFree();
void FontPathInit(char* path, int isFullSet);

void PrintPic( int x, int y, int pic ); 
void ClearPic( int x, int y);

void ReserveRect( int x, int y, int width, int height);

void PrintField( int x, int y, char *str, int align, int font, int space);

void PrintText( int x, int y, char *str, int align, int font, int space, int lineSpace, int isScroll, short fixSize, int maxLen);

int FontH(int font);

void ClearScreen();

void ClearData();

void PrintTest(char *str);

void SetScrollEdgeDelay(int delay);

short GetNLines(int x, int y);

short GetWidth(int x, int y);

void SetFontColor(char* fontColor, char* bgrColor, char isOutline);

void SetCanvasFlag(int flag);



int CreateScreenSelectGUI();
void reftes(void);


  extern  void *pcsmadr;
  extern   WSHDR *wstag;
  extern const RECT cTgr;  
  extern int scr_w;
  extern const char clr[];
  extern int TopG (int);

  void initTags(void);
  void GetTags();

typedef  unsigned char byte;
typedef  unsigned short word;
typedef  unsigned int dword;




#pragma swi_number=0x80D6
__swi __arm unsigned int* RamNetOnline();

#pragma swi_number=0x1FF
__swi __arm void MEDIA_PLAYLAST();

#pragma swi_number=0x1F8
__swi __arm unsigned int _GetPlayStatus(void);

#pragma swi_number=0x22C
__swi __arm int pdcache_getval(char* ret, int maxlen_of_return_str, char cachetype, char *pd_entry_identifier_class, char  *pd_entry_identifier_sub);
#pragma swi_number=0x22D
__swi __arm int pdcache_setval(char *new_value, char cachetype,  char *pd_entry_identifier_class, char *pd_entry_identifier_sub);


#pragma swi_number=0x41
__swi __arm char GetProvider(WSHDR *ws, int zero);



#pragma swi_number=54
__swi __arm void SLI_SetState(unsigned char state);


  
  #pragma swi_number=0x020          
  __swi __arm int DrawScreenSaver();
  
  
  #pragma swi_number=0x09C
  __swi __arm int CloseScreensaver();

  #pragma swi_number=0x0C5
  __swi __arm int IsIdleUiOnTop();


#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=(24);
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+(24)-1;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+(24);
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}

void CheckComand2(char *txt, CLIST *t);
void DoScreen();

extern int *XStatusesIconArray;






int main_d;
int align=0;

void *canvasdata;
int paused_height;
WSHDR *wsBuffer;

char talk_sound[128];
int uins;


char unread_msg_info[64];

int saver_on;

  extern char teast2[256];


  char last_msg_buffer[4096];




  
  

  
GBSTMR ping_tmr;
static void PingToServer2(void)
{
  TPKT *p;
  p=malloc(sizeof(PKT)+sizeof(TTime));
  GetDateTime(0,(TTime *)p->data);
  p->pkt.uin=0;
  p->pkt.type=18;
  p->pkt.data_len=sizeof(TTime);
  SUBPROC((void *)SendAnswer,0,p);
}

int Is_PING_On;

void Pinger2()
{
  if(Is_PING_On)
  {
    PingToServer2();
    GBS_StartTimerProc(&ping_tmr, 12960, Pinger2);
  }
  else
    GBS_StartTimerProc(&ping_tmr, 2160, Pinger2);
}





extern volatile int total_smiles;
extern volatile int total_xstatuses;
extern volatile int xstatuses_load;
extern volatile int pictures_max; 
extern volatile int pictures_loaded; 

  int Is_SLI_On;

int Is_SMARTCPU_On;
int Is_INC_On;
int Is_ILU_On;
int Is_Double_Vibra_On;
int total_unread;
int total_unread2;

  const int SLI_TYPE=1;

char PINGO[256];

  extern const unsigned int SLI_time;
  extern const unsigned int SLI_time2;




const char ipc_my_name[32]= "NatICQ";
const char ipc_xtask_name[]= "XTask";
const char ipc_test_name[]="test";
IPC_REQ gipc;


char elf_path[256];
int maincsm_id;
int maingui_id;

void SMART_REDRAW(void)
{
  int f;
  LockSched();
  f=IsGuiOnTop(maingui_id);
  UnlockSched();
  if (f) REDRAW();
}







const char S_OFFLINE[]="Offline";
const char S_INVISIBLE[]="Invisible";
const char S_AWAY[]="Away";
const char S_NA[]="N/A";
const char S_OCCUPIED[]="Occupied";
const char S_DND[]="DND";
const char S_ONLINE[]="Online";
const char S_FFC[]="FFC";

volatile int SENDMSGCOUNT;
  int IsActiveUp=0;
int Is_Vibra_Enabled;
int Is_Id3tag_Enabled;
unsigned int Is_Sounds_Enabled;
int Is_Show_Offline;
int Is_Show_Groups;

int S_ICONS[TOTAL_ICONS+1];

int CurrentStatus;
int CurrentXStatus;

WSHDR *ews;

static const char * const icons_names[TOTAL_ICONS]=
{
  "offline.png",
  "invisible.png",
  "away.png",
  "na.png",
  "occupied.png",
  "dnd.png",
  "depression.png",
  "evil.png",
  "home.png",
  "lunch.png",
  "work.png",
  "online.png",
  "ffc.png",
  "message.png",
  "unknown.png",
  "groupon.png",
  "groupoff.png",
  "null.png",
  "addcont.png",
  "head.png",
  "logo.png",
  "ping.png",
  "settings.png",
  "vis1.png",
  "vis2.png",
  "vis3.png",
  "vis4.png",
  "vis5.png"  
};



  


GBSTMR scrsTmr;
int scrsTime;
void ScreenSaverTimer()
{
 if(scrsTime==screenSaverTime)
 {
   scrsTime=screenSaverTime;
   CloseScreensaver();
   DrawScreenSaver();  
   GBS_StartTimerProc(&scrsTmr, 216*10, ScreenSaverTimer);
 }
 if(isScreenSaverEna)
       scrsTime++;
  GBS_StartTimerProc(&scrsTmr, 216*5, ScreenSaverTimer);
}



  
  
int msg_away_timer=0;
char away_msg[264];
char away_msg_time[64];
char away_msg_time_dace[16];

char tim_zone[16];
int timeh;
int timemin;
int away_m;
int status_rem;
int change_back_status;
GBSTMR away_tmr;


int Show_XTX;
void FreeXText2(CLIST *t)
{
  if (t->xtext2) { mfree(t->xtext2); t->xtext2=0; }
}

void away_timer()
{
  if(msg_away_timer==(AUTO_ENGADE*12))
  {
    TTime tt;
    GetDateTime(0,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "( Отсутствую примерно с %02d:%02d [%s] )", tt.hour, tt.min, tim_zone);
    away_m=1;
    if(CurrentStatus!=2 && CurrentStatus!=isAutoStatusOn)
    {
      status_rem=CurrentStatus;
      CurrentStatus=2;
      set_my_status();
      void SetIconBarHandler();
      SetIconBarHandler();
      change_back_status=1;
    }
  }
  
  if( (IsOnNAStatus) && (msg_away_timer==((AUTO_ENGADE*12)+(NAStatusTime*12))) )
  {
    TTime tt;
    GetDateTime(0,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "( Недоступен примерно с %02d:%02d [%s] )", tt.hour, tt.min, tim_zone);
    if(CurrentStatus!=3)
    {
      status_rem=CurrentStatus;
      CurrentStatus=3;
      set_my_status();
      void SetIconBarHandler();
      SetIconBarHandler();
    }
  }
  
  if(Auto_Status)
     msg_away_timer++;
  GBS_StartTimerProc(&away_tmr, 216*5, away_timer);
}


void init_time_zone()
{
  switch(time_zone)
  {
  case 0:
    sprintf(tim_zone, "GMT -12:00");
    break;
  case 1:
    sprintf(tim_zone, "GMT -11:00");
    break;
  case 2:
    sprintf(tim_zone, "GMT -10:00");
    break;
  case 3:
    sprintf(tim_zone, "GMT -09:00");
    break;
  case 4:
    sprintf(tim_zone, "GMT -08:00");
    break;
  case 5:
    sprintf(tim_zone, "GMT -07:00");
    break;
  case 6:
    sprintf(tim_zone, "GMT -06:00");
    break;
  case 7:
    sprintf(tim_zone, "GMT -05:00");
    break;
  case 8:
    sprintf(tim_zone, "GMT -04:00");
    break;
  case 9:
    sprintf(tim_zone, "GMT -03:30");
    break;
  case 10:
    sprintf(tim_zone, "GMT -03:00");
    break;
  case 11:
    sprintf(tim_zone, "GMT -02:00");
    break;
  case 12:
    sprintf(tim_zone, "GMT -01:00");
    break;
  case 13:
    sprintf(tim_zone, "GMT 00:00");
    break;
  case 14:
    sprintf(tim_zone, "GMT +01:00");
    break;
  case 15:
    sprintf(tim_zone, "GMT +02:00");
    break;
  case 16:
    sprintf(tim_zone, "GMT +03:00");
    break;
  case 17:
    sprintf(tim_zone, "GMT +03:30");
    break;
  case 18:
    sprintf(tim_zone, "GMT +04:00");
    break;
  case 19:
    sprintf(tim_zone, "GMT +04:30");
    break;
  case 20:
    sprintf(tim_zone, "GMT +05:00");
    break;
  case 21:
    sprintf(tim_zone, "GMT +05:30");
    break;
  case 22:
    sprintf(tim_zone, "GMT +05:45");
    break;
  case 23:
    sprintf(tim_zone, "GMT +06:00");
    break;
  case 24:
    sprintf(tim_zone, "GMT +06:30");
    break;
  case 25:
    sprintf(tim_zone, "GMT +07:00");
    break;
  case 26:
    sprintf(tim_zone, "GMT +08:00");
    break;
  case 27:
    sprintf(tim_zone, "GMT +09:00");
    break;
  case 28:
    sprintf(tim_zone, "GMT +09:30");
    break;
  case 29:
    sprintf(tim_zone, "GMT +10:00");
    break;
  case 30:
    sprintf(tim_zone, "GMT +11:00");
    break;
  case 31:
    sprintf(tim_zone, "GMT +12:00");
    break;
  case 32:
    sprintf(tim_zone, "GMT +13:00");
    break;
  }
}




void SetIconBarHandler();
GBSTMR autoChangeStatus;
#pragma inline 
static unsigned int GetStatusByIndex(int pos)
{
  switch(pos)
  {
  case 1:
    return IS_ONLINE;
  case 2:
    return IS_AWAY;
  case 3:
    return IS_NA;
  case 4:
    return IS_DND;
  case 5:
    return IS_OCCUPIED;
  case 6:
    return IS_FFC;
  case 7:
    return IS_INVISIBLE;
  case 8:
    return IS_DEPRESSION;
  case 9:
    return IS_EVIL;
  case 10:
    return IS_HOME;
  case 11:
    return IS_LUNCH;
  case 12:
    return IS_WORK;
  }
  return IS_UNKNOWN;
}

void AutoChangeStatus_timer()
{
  TTime tt;
  GetDateTime(0,&tt);
    
  if(status_1!=0 && tt.hour==tm_status_1.hour && tt.min==tm_status_1.min)
  {
      CurrentStatus=GetStatusByIndex(status_1);
      set_my_status();
  }
  
  if(status_2!=0 && tt.hour==tm_status_2.hour && tt.min==tm_status_2.min)
  {
      CurrentStatus=GetStatusByIndex(status_2);
      set_my_status();
  }
  
  if(status_3!=0 && tt.hour==tm_status_3.hour && tt.min==tm_status_3.min)
  {
      CurrentStatus=GetStatusByIndex(status_3);
      set_my_status();
  }
  
  if(status_4!=0 && tt.hour==tm_status_4.hour && tt.min==tm_status_4.min)
  {
      CurrentStatus=GetStatusByIndex(status_4);
      set_my_status();
  }
  
  if(status_5!=0 && tt.hour==tm_status_5.hour && tt.min==tm_status_5.min)
  {
      CurrentStatus=GetStatusByIndex(status_5);
      set_my_status();
  }
  SetIconBarHandler();
  
  GBS_StartTimerProc(&autoChangeStatus, 216*10, AutoChangeStatus_timer);
}



void DrawPing(void *canv)
{
  PrintField(Ping_X,Ping_Y, PINGO, align, fonto, spaco1);
}



GBSTMR remind_tmr;
volatile int vibra_count;
  void SLI_Off(void);
void start_vibra2(void);



void Remind_Timer2()
{
  if (total_unread)
  {
    vibra_count=Vibra_cc;
    start_vibra2();
    GBS_StartTimerProc(&remind_tmr, 6480, Remind_Timer2);
  }
}

void Reminder_timer()
{
  GBS_StartTimerProc(&remind_tmr, 6480, Remind_Timer2);
}



int Is_UPTIME_On;
int uptimeHour;
int uptimeMin;
int uptimeSec;

void UptimeMinHour()
{
  uptimeSec++;
  if(uptimeSec==60)
  {
    uptimeSec=0;
    uptimeMin++;
  }
  if(uptimeMin==60)
  {
    uptimeMin=0;
    uptimeHour++;
  }
}

GBSTMR uptime_tmr;
  GBSTMR tracktmr;
  int tracktimer;

void upTime()
{
  if(connect_state==3)
  {
   UptimeMinHour();
  if (tracktimer>=10) 
    {
      GBS_StartTimerProc(&tracktmr, 216*5, GetTags);
      tracktimer=0;
    }
    tracktimer++;
  GBS_StartTimerProc(&uptime_tmr, 216, upTime);
  }
  else
  {
    if(!Pause_Time)
    {
     uptimeHour=0;
     uptimeMin=0;
     uptimeSec=0;
    }
    GBS_StartTimerProc(&uptime_tmr, 216, upTime);
  }
}

char onlineTime[128];
void DrawOnlineTime(void *canv)
{  
  if(connect_state==3)
  {   
   sprintf(onlineTime,"Online: %02d:%02d:%02d", uptimeHour, uptimeMin, uptimeSec);
  }
  else
    sprintf(onlineTime,"Offline");
  PrintField(onlineTime_x,onlineTime_y, onlineTime, align, fonto, spaco1);
}





void DrawAutoMSG(void *canv)
{  
  char awayTime[64];
  if(away_m && showAwayIdle)
  {
    sprintf(awayTime,"Away from [%s]",away_msg_time_dace);
    PrintField(auto_x,auto_y, awayTime, align, fonto, spaco1);
  }  
}





int input_awaymsg_id;

const HEADER_DESC input_awaymsg_hdr={0,0,131,21,0,(int)"Ввод автосообщения",0x7FFFFFFF};
void UpdateCLheader(void);
int input_awaymsg_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws;
  if (msg->keys==0x0FFF || msg->keys==0x0E)
  {
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);	
    ws=ec.pWS;
    zeromem(awayMsg,256);
    int i=0;
    for(i=0;i<ws->wsbody[0];i++)
      awayMsg[i]=char16to8(ws->wsbody[i+1]);
      awayMsg[i]=0;

    msg_away_timer=(AUTO_ENGADE*12);
    away_m=1;
    away_timer();
    UpdateCLheader();
    GeneralFunc_flag1(input_awaymsg_id,1);

  }
  
  if(msg->keys==0x01 || msg->keys==0x0C)
  {
     msg_away_timer=0;
     away_m=0;
     if(change_back_status)
      {
        CurrentStatus=status_rem;
        set_my_status();
        change_back_status=0;
      }
     UpdateCLheader();
  }
return(0);
}

void input_awaymsg_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Сохранить"};
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,0);
  }
  if(cmd==0xA)
  {
    DisableIDLETMR(); 
  }
  if (cmd==0x0C)
  {
    EDIT_SetCursorPos(gui,1);
  }
  if(cmd==0x03){}
}

void input_awaymsg_locret(void){}

SOFTKEY_DESC input_awaymsg_sk[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)0x7FFFC0FB}
};

SOFTKEYSTAB input_awaymsg_skt=
{
  input_awaymsg_sk,0
};

const INPUTDIA_DESC input_awaymsg_desc=
{
  1,
  input_awaymsg_onkey,
  input_awaymsg_ghook,
  (void *)input_awaymsg_locret,
  0,
  &input_awaymsg_skt,
  {0,0,0,0},
  8,
  100,
  101,
  0,
  0x00000000,
  0x40000000
};

void InputAwayMsg()
{
   WSHDR *wss=AllocWS(256);
   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());
   EDITCONTROL ec;
   PrepareEditControl(&ec);
   wsprintf(wss,"%t","Сообщение:\n");
   ConstructEditControl(&ec,1,0,wss,32);
   AddEditControlToEditQend(eq,&ec,ma);  

   PrepareEditControl(&ec);
   CutWSTR(wss,0);
   ascii2ws(wss,awayMsg);
   ConstructEditControl(&ec,3,0x40,wss,255);
   AddEditControlToEditQend(eq,&ec,ma);
   patch_header(&input_awaymsg_hdr);
   patch_input(&input_awaymsg_desc);      
 
   input_awaymsg_id=CreateInputTextDialog(&input_awaymsg_desc,&input_awaymsg_hdr,eq,1,0);
}



GBSTMR sli_tmr;
void SLI_On()
{
  if(Is_SLI_On)
    SLI_SetState(1);   
  GBS_StartTimerProc(&sli_tmr, (2 * 216) / 20, SLI_Off);
}

void SLI_Off()
{
  SLI_SetState(2);
  if(total_unread)
     GBS_StartTimerProc(&sli_tmr, (2 * 216) / 20, SLI_On); 
  else
     SLI_SetState(0);
}

void SLI_check()
{
  if (total_unread)
    SLI_On();
}

void SLI_Timer()
{
  GBS_StartTimerProc(&sli_tmr, 262, SLI_check); 
}

void setup_ICONS(void)
{
  int i=0;
  do
  {
    if (!S_ICONS[i]) S_ICONS[i]=(int)MakeGlobalString(ICON_PATH,'\\',icons_names[i]);
    i++;
  }
  while(i<TOTAL_ICONS);
  return;
}

IMGHDR *icons[TOTAL_ICONS];

void StatusToIMGHDR()
{
 
  int i=0;
  do
  {
    icons[i]=CreateIMGHDRFromPngFile( MakeGlobalString("4:\\ZBin\\NatICQ\\img2",'\\',icons_names[i]),2);   	

    i++;
  }
  while(i<TOTAL_ICONS);
}

void FreeStatusToIMGHDR()
{
 
  int i=0;
  do
  {
    if(icons[i]) mfree(icons[i]->bitmap);
    i++;
  }
  while(i<TOTAL_ICONS);
}

void free_ICONS(void)
{
  int i=0;
  do
  {
    mfree((void*)S_ICONS[i]);
    S_ICONS[i]=0;
    i++;
  }
  while(i<TOTAL_ICONS);
  return;
}

const char percent_t[]="%t";
const char percent_s[]="%s";
const char percent_d[]="%d";
const char empty_str[]="";

const char x_status_change[]="X-Status change";

char logmsg[256];


GBSTMR tmr_illumination;

void IlluminationOff(){
  SetIllumination(0,1,0,ILL_OFF_FADE);
  SetIllumination(1,1,0,ILL_OFF_FADE);
}

void IlluminationOn(const int disp, const int key, const int tmr, const int fade)
{
  if(!tmr) return;
  GBS_DelTimer(&tmr_illumination);
  SetIllumination(0,1,disp,fade);
  SetIllumination(1,1,key,fade);
  GBS_StartTimerProc(&tmr_illumination,tmr*216,IlluminationOff);
}

volatile int silenthide;    
volatile int disautorecconect;	


unsigned int Is_Sounds_Enabled;
int Is_Show_Offline;
int Is_Show_Groups;


int CurrentPrivateStatus;



const char def_setting[]="%sdef_settings_mod_%d";   
const char def_sett[]="%sdef_statusset_mod_%d";
DEF_SETTINGS2 def_set[35];
void ReadDefSettings(void)
{
  DEF_SETTINGS def_se;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if ((f=fopen(str,0+0x8000,0x80,&err))!=-1)
  {
    fread(f,&def_se,sizeof(DEF_SETTINGS),&err);
    fclose(f,&err);
    Is_Vibra_Enabled=def_se.vibra_status;
    Is_Sounds_Enabled=def_se.sound_status;
    Is_SLI_On=def_se.sli_state;
    Is_INC_On=def_se.inc_state;
    Is_ILU_On=def_se.ilu_state;
    Is_PING_On=def_se.ping_state;
    Is_UPTIME_On=def_se.uptime_state;
    Is_Show_Offline=def_se.off_contacts;
    Is_Show_Groups=def_se.show_groups;
    CurrentStatus=def_se.def_status+1;
    CurrentXStatus=def_se.def_xstatus;
    Is_Double_Vibra_On=def_se.remind_state;
    Show_XTX=def_se.isXTX;

    if(ind_set_xstatus)
    {      
      snprintf(str,127,def_sett,elf_path,UIN);
      if ((f=fopen(str,0+0x8000,0x80,&err))!=-1)
      {
        for(int i=0,y=0 ; i<34 ; i++,y+=sizeof(DEF_SETTINGS2))
        {
          lseek(f,y,0,&err,&err);
          fread(f,&def_set[i],sizeof(DEF_SETTINGS2),&err);      
        }
      }
    }
  fclose(f,&err);
  }
  else
  {
    Is_Vibra_Enabled=1;
    Is_Id3tag_Enabled=0;
    Is_Sounds_Enabled=1;
    Is_Show_Offline=0;
    Is_Show_Groups=0;
    CurrentStatus=IS_ONLINE;
    CurrentXStatus=0;
    Is_SLI_On=1;
    Is_SMARTCPU_On=0;
    Is_INC_On=1;
    Is_Double_Vibra_On=0;
    Is_ILU_On=1;
    Is_PING_On=0;
    Is_UPTIME_On=0;
    Show_XTX=1;
    
    if(ind_set_xstatus)
    {
      for(int i=0;i<34;i++)
      {
        def_set[i].vibra_status=Is_Vibra_Enabled;
        def_set[i].Id3tag_status=Is_Id3tag_Enabled;
        def_set[i].sound_status=Is_Sounds_Enabled;
        def_set[i].off_contacts=Is_Show_Offline;
        def_set[i].show_groups=Is_Show_Groups;
        def_set[i].sli_state=Is_SLI_On;
        def_set[i].smartcpu_state=Is_SMARTCPU_On;
        def_set[i].inc_state=Is_INC_On;
        def_set[i].remind_state=Is_Double_Vibra_On;
        def_set[i].ilu_state=Is_ILU_On;
        def_set[i].ping_state=Is_PING_On;
        def_set[i].uptime_state=Is_UPTIME_On;
        def_set[i].isXTX=Show_XTX;
      }
    }
  }
  fclose(f,&err);
}

void WriteDefSettings(void)
{
  DEF_SETTINGS def_se;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if ((f=fopen(str,1+0x8000+0x100+0x200,0x100,&err))!=-1)
  {
    def_se.vibra_status=Is_Vibra_Enabled;
    def_se.sound_status=Is_Sounds_Enabled;
    def_se.off_contacts=Is_Show_Offline;
    def_se.show_groups=Is_Show_Groups;
    def_se.def_status=CurrentStatus-1;
    def_se.def_xstatus=CurrentXStatus;
    def_se.sli_state=Is_SLI_On;
    def_se.inc_state=Is_INC_On;
    def_se.remind_state=Is_Double_Vibra_On;
    def_se.ilu_state=Is_ILU_On;
    def_se.uptime_state=Is_UPTIME_On;
    def_se.isXTX=Show_XTX;
    def_se.ping_state=Is_PING_On;  
    fwrite(f,&def_se,sizeof(DEF_SETTINGS),&err);    
  }  
  fclose(f,&err);

if(ind_set_xstatus)
{
  snprintf(str,127,def_sett,elf_path,UIN);
  unlink(str,&err);

  if ((f=fopen(str,1+0x8000+0x100+8,0x100,&err))!=-1)
  {
    def_set[CurrentXStatus].vibra_status=Is_Vibra_Enabled;
    def_set[CurrentXStatus].Id3tag_status=Is_Id3tag_Enabled;
    def_set[CurrentXStatus].sound_status=Is_Sounds_Enabled;
    def_set[CurrentXStatus].off_contacts=Is_Show_Offline;
    def_set[CurrentXStatus].show_groups=Is_Show_Groups;
    def_set[CurrentXStatus].sli_state=Is_SLI_On;
    def_set[CurrentXStatus].smartcpu_state=Is_SMARTCPU_On;
    def_set[CurrentXStatus].inc_state=Is_INC_On;
    def_set[CurrentXStatus].remind_state=Is_Double_Vibra_On;
    def_set[CurrentXStatus].ilu_state=Is_ILU_On;
    def_set[CurrentXStatus].ping_state=Is_PING_On;
    def_set[CurrentXStatus].uptime_state=Is_UPTIME_On;
    def_set[CurrentXStatus].isXTX=Show_XTX;



    for(int i=0;i<34;i++)
    {
      fwrite(f,&def_set[i],sizeof(DEF_SETTINGS2),&err);
    }
  }
}  
 fclose(f,&err);
 UpdateCLheader();
}




extern S_SMILES *s_top;
extern DYNPNGICONLIST *SmilesImgList;
extern DYNPNGICONLIST *XStatusesImgList;



void Play(const char *fname)
{
  if ((!IsCalling())&&Is_Sounds_Enabled)
  {
    FSTATS fstats;
    unsigned int err;
    if (GetFileStats(fname,&fstats,&err)!=-1)
    {
      PLAYFILE_OPT _sfo1;
      WSHDR* sndPath=AllocWS(128);
      WSHDR* sndFName=AllocWS(128);
      char s[128];
      const char *p=strrchr(fname,'\\')+1;
      str_2ws(sndFName,p,128);
      strncpy(s,fname,p-fname);
      s[p-fname]='\0';
      str_2ws(sndPath,s,128);
      
      zeromem(&_sfo1,sizeof(PLAYFILE_OPT));
      _sfo1.repeat_num=1;
      _sfo1.time_between_play=0;
      _sfo1.play_first=0;
      _sfo1.volume=sndVolume;
      _sfo1.unk6=1;
      _sfo1.unk7=1;
      _sfo1.unk9=2;
      PlayFile(0x10, sndPath, sndFName, GBS_GetCurCepid(), 0x70BC, &_sfo1);
      FreeWS(sndPath);
      FreeWS(sndFName);
    }
  }
}

GBSTMR tmr_vibra;

volatile int vibra_count; 

void start_vibra(void)
{
  void stop_vibra(void);
  if((Is_Vibra_Enabled)&&(!IsCalling()))
  {
    SetVibration(vibraPower);
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,216>>2,stop_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,216>>1,stop_vibra);
  }
}

void stop_vibra(void)
{ 
  SetVibration(0);
  if (--vibra_count)
  {
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,216>>5,start_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,216>>1,start_vibra);
  }
}

void start_vibra2(void)
{  
  void stop_vibra2(void);
 
  SetVibration(vibraPower);
  if(VIBR_TYPE)
    GBS_StartTimerProc(&tmr_vibra,216>>2,stop_vibra2);
   else
     GBS_StartTimerProc(&tmr_vibra,216>>1,stop_vibra2);
}

void stop_vibra2(void)
{
  SetVibration(0);
  if (--vibra_count)
  {
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,216>>5,start_vibra2);
    else
      GBS_StartTimerProc(&tmr_vibra,216>>1,start_vibra2);
  }
}

void ChangeVibra(void)
{
  if (!(Is_Vibra_Enabled=!(Is_Vibra_Enabled)))
    ShowMSG(1,(int)"Вибра отключена!");
  else
    ShowMSG(1,(int)"Вибра включена!");
}

void ChangeSound(void)
{
  if (!(Is_Sounds_Enabled=!(Is_Sounds_Enabled)))
    ShowMSG(1,(int)"Звуки отключены!");
  else
    ShowMSG(1,(int)"Звуки включены!");
}



char *templates_chars; 
char **templates_lines; 

void FreeTemplates(void)
{
  if (templates_lines) mfree(templates_lines);
  if (templates_chars) mfree(templates_chars);
  templates_lines=0;
  templates_chars=0;
}

int LoadTemplates(unsigned int uin)
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  FreeTemplates();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\%d.txt",uin);
  if (GetFileStats(fn,&stat,&ul)==-1) goto L1;
  if ((fsize=stat.size)<=0) goto L1;
  if ((f=fopen(fn,0+0x8000,0x80,&ul))==-1)
  {
  L1:
    strcpy(fn+i,"\\0.txt");
    if (GetFileStats(fn,&stat,&ul)==-1) return 0;
    if ((fsize=stat.size)<=0) return 0;
    f=fopen(fn,0+0x8000,0x80,&ul);
  }
  if (f==-1) return 0;
  p=templates_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	templates_lines=realloc(templates_lines,(i+1)*sizeof(char *));
	templates_lines[i++]=pp;
      }
      pp=0;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==0) pp=p;
    }
    p++;
  }
  return i;
}









int RXstate= -10; 

TPKT RXbuf;
TPKT TXbuf;

int connect_state=0;

int sock=-1;

volatile unsigned long TOTALRECEIVED;
volatile unsigned long TOTALSENDED;
volatile unsigned long ALLTOTALRECEIVED;	
volatile unsigned long ALLTOTALSENDED;

volatile int sendq_l=0; 
volatile void *sendq_p=0; 

volatile int is_gprs_online=1;

GBSTMR reconnect_tmr;

extern void kill_data(void *p,void (*func_p)(void *));

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

int total_unread; 

volatile CLIST *cltop;

volatile unsigned int GROUP_CACHE; 

volatile int contactlist_menu_id;

GBSTMR tmr_active;

volatile int edchat_id;







char clm_hdr_text[64];

static const char key_clm_hdr_text[] = "Ввод T9: ";
static const HEADER_DESC contactlist_menuhdr = {0, 0, 0, 0, S_ICONS+ICON_HEAD, (int)clm_hdr_text, 0x7FFFFFFF};

static const int menusoftkeys[] = {0,1,2};
static const SOFTKEY_DESC menu_sk[] =
{
  {0x0018, 0x0000, (int)"Выбор"},
  {0x0001, 0x0000, (int)"Закрыть"},
  {0x003D, 0x0000, (int)0x7FFFC0FB}
};

char clmenu_sk_r[16];
static const char def_clmenu_sk_r[] = "Закрыть";
static const char key_clmenu_sk_r[] = "<C";







 
const SOFTKEYSTAB menu_skt =
{
  menu_sk, 0
};




 

void contactlist_menu_ghook(void *data, int cmd);
int contactlist_menu_onkey(void *data, GUI_MSG *msg);
void contactlist_menu_iconhndl(void *data, int curitem, void *unk);



int tenseconds_to_ping;

LOGQ *NewLOGQ(const char *s)
{
  LOGQ *p=malloc(sizeof(LOGQ)+1+strlen(s));
  zeromem(p,sizeof(LOGQ));
  strcpy(p->text,s);
  return p;
}

LOGQ *LastLOGQ(LOGQ **pp)
{
  LOGQ *q=*pp;
  if (q)
  {
    while(q->next) q=q->next;
  }
  return(q);
}


void FreeLOGQ(LOGQ **pp)
{
  LOGQ *p=*pp;
  *pp=0; 
  while(p)
  {
    LOGQ *np=p->next;
    mfree(p);
    p=np;
  }
}


void RemoveLOGQ(LOGQ **queue, LOGQ *p)
{
  LOGQ *q=(LOGQ *)queue;
  LOGQ *qp;
  while(qp=q->next)
  {
    if (qp==p)
    {
      q->next=p->next;
      break;
    }
    q=qp;
  }
  mfree(p); 
}


int AddLOGQ(LOGQ **queue, LOGQ *p)
{
  LOGQ *q=(LOGQ *)queue;
  LOGQ *qp;
  int i=0;
  while(qp=q->next) {q=qp;i++;}
  p->next=0;
  q->next=p;
  return(i+1); 
}

char *GetLastXTextLOGQ(CLIST *t)
{
  LOGQ *p = t->log;
  char *s = 0;
  if(!p) return 0;
  while(p->next)
  {
    if((p->type&0x0F)==3)
      s = p->text;
    p = p->next;
  }
  if((p->type&0x0F)==3)
    s = p->text;
  return s;
}

int GetIconIndex(CLIST *t)
{
  unsigned short s;
  if (t)
  {
    s=t->state;
    if (t->isgroup) return(IS_GROUP);
    if (t->isunread)
      return(IS_MSG);
    else
    {
      if (s==0xFFFF) return(IS_OFFLINE);
      if ((s&0xF000)==0x2000) return (IS_LUNCH);
      if ((s&0xF000)==0x3000) return (IS_EVIL);
      if ((s&0xF000)==0x4000) return (IS_DEPRESSION);
      if ((s&0xF000)==0x5000) return (IS_HOME);
      if ((s&0xF000)==0x6000) return (IS_WORK);
      if (s & 0x0020) return(IS_FFC);
      if (s & 0x0001) return(IS_AWAY);
      if (s & 0x0005) return(IS_NA);
      if (s & 0x0011) return(IS_OCCUPIED);
      if (s & 0x0013) return(IS_DND);
      if (s & 0x0002) return(IS_FFC);
      if (s & 0x0100) return(IS_INVISIBLE);
    }
  }
  else
  {
    return(IS_UNKNOWN);
  }
  return(IS_ONLINE);
}

LOGQ *FindContactLOGQByAck(TPKT *p)
{
  CLIST *t;
  LockSched();
  t=FindContactByUin(p->pkt.uin);
  UnlockSched();
  unsigned int id=*((unsigned short*)(p->data));
  LOGQ *q;
  if (!t) return 0;
  LockSched();
  q=t->log;
  while(q)
  {
    if (q->ID==id) break;
    q=q->next;
  }
  UnlockSched();
  return q;
}




 

void CreateCLFile(void)
{
  CLIST *t;
  t=(CLIST *)(&cltop);
  char *s=malloc(128);
  volatile int hFile_cl;
  unsigned int io_error_cl = 0;
  char fn[256];
  strcpy(fn,TEMPLATES_PATH);
  strcat(fn,"\\local.cl"); 
  unlink(fn,&io_error_cl);
  hFile_cl = fopen(fn,2 + 0x100 + 8 + 0x8000,0x80+0x100, &io_error_cl);
  if(hFile_cl!=-1)
  {
    while(t=(CLIST *)(t->next))
    {    
      if(!t->isgroup && t->uin!=UIN)
      {
        sprintf(s, ";%d;%s;;;\r\n", t->uin, t->name);      
        fwrite(hFile_cl, s, strlen(s), &io_error_cl);
      }
     }
    ShowMSG(2,(int)"Saved local.cl");
  }
  else ShowMSG(2,(int)"Can't save local.cl");
  fclose(hFile_cl, &io_error_cl);
  mfree(s);  
}

 

char on_off_buf[128];
int on_off_timer=0;
char ContactT9Key[32];
int kopa_online;
int kopa_kontaktu;
char status[64];
void UpdateCLheader(void){};


void ClearContactT9Key(void)
{
  zeromem(ContactT9Key,sizeof(ContactT9Key));
}

void AddContactT9Key(int chr)
{
  int l=strlen(ContactT9Key);
  if (l<(sizeof(ContactT9Key)-1))
  {
    ContactT9Key[l]=chr;
  }
}

void BackSpaceContactT9(void)
{
  int l=strlen(ContactT9Key);
  if (l)
  {
    l--;
    ContactT9Key[l]=0;
  }
}


void RecountMenu(CLIST *req, int needfocus);




static int uinContact;

static void ChangeVibraContact(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isvibra=!(t->isvibra);
  RefreshGUI();
}

static void ChangeSoundContact(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->issound=!(t->issound);
  RefreshGUI();
}

static void ChangeAutoMsg(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isautomsg=!(t->isautomsg);
  RefreshGUI();
}

static void ChangeShowXstatus(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isShowXst=!(t->isShowXst);
  RefreshGUI();
}








 
static void contact_list_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}
static int icon_array[2];

static const void *menuproc[4]=
{ 
  (void *)ChangeVibraContact,
  (void *)ChangeSoundContact,
  (void *)ChangeAutoMsg,
  (void *)ChangeShowXstatus,
};

static const char * const menutext[4]=
{
  "Вибра",
  "Звук",
  "Автоответчик",
  "Показ.хстатус",
};

static int contact_keyhook(void *data, GUI_MSG *msg)
{
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    CLIST *t;
    t=FindContactByUin(uinContact);
    t->isIndividSet=1;
    ((void (*)(void))(menuproc[GetCurMenuItem(data)]))();
  }
  
  return(0);
}

static void contacthandler(void *data, int curitem, void *unk)
{
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  extern const char percent_t[];
  ws=AllocMenuWS(data,strlen(menutext[curitem]));
  wsprintf(ws,percent_t,menutext[curitem]);
  CLIST *t;
  t=FindContactByUin(uinContact);
  switch(curitem)
  {
  case 0:
    SetMenuItemIconArray(data,item,icon_array+(t->isvibra?0:1));
    break;
  case 1:
    SetMenuItemIconArray(data,item,icon_array+(t->issound?0:1));
   break;
  case 2:
    SetMenuItemIconArray(data,item,icon_array+(t->isautomsg?0:1));
    break;
  case 3:
    SetMenuItemIconArray(data,item,icon_array+(t->isShowXst?0:1));
    break;
  }
  SetMenuItemText(data, item, ws, curitem);
}

char cont_hdr[64];
static const HEADER_DESC contact_list_HDR={0,0,0,0,0,(int)cont_hdr,0x7FFFFFFF};
static const int _softkeys[] = {0,1,2};

static const SOFTKEY_DESC _sk[] =
{
  {0x0018, 0x0000, (int)"Выбрать"},
  {0x0001, 0x0000, (int)"Закрыть"},
  {0x003D, 0x0000, (int)0x7FFFC0FB}
};

const SOFTKEYSTAB _skt =
{
  _sk, 0
};
static const MENU_DESC contact_list_MNU=
{
  8,contact_keyhook,contact_list_ghook,0,
  _softkeys,
  &_skt,
  0x11,
  contacthandler,
  0,
  0,
  4
};


int CreateContactMenu(CLIST *t)
{
  uinContact=t->uin;
  icon_array[0]=GetPicNByUnicodeSymbol(0xE116);
  icon_array[1]=GetPicNByUnicodeSymbol(0xE117);
  sprintf(cont_hdr,"Меню кoнт. %s",t->name);
  patch_header(&contact_list_HDR);
  return CreateMenu(0,0,&contact_list_MNU,&contact_list_HDR,0,4,0,0);  
}




void LongPressActionOnChat(CLIST *t,int i)
{
   switch(i)
    {
    case 1:
        Is_Sounds_Enabled=!(Is_Sounds_Enabled);
        if(Is_Sounds_Enabled)
          ShowMSG(1,(int)"Sound 0n");
        else
          ShowMSG(1,(int)"Sound Off");
        SUBPROC((void*)WriteDefSettings);
        break;
    case 2:
        Is_Vibra_Enabled=!(Is_Vibra_Enabled);
        SUBPROC((void*)WriteDefSettings);
        if(Is_Vibra_Enabled)
          ShowMSG(1,(int)"Vibra On");
        else
          ShowMSG(1,(int)"Vibra Off");
        break;
    case 3:        
         if(CurrentPrivateStatus!=1)
         {
           SetPrivateStatus(1);
           ShowMSG(1,(int)"Инвиз включен");
         }
         else
         {
           SetPrivateStatus(3);
           ShowMSG(1,(int)"Инвиз выключен");
         }
         break;
    case 4:
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        if(Is_Double_Vibra_On)
          ShowMSG(1,(int)"Vibra rem On");
        else 
          ShowMSG(1,(int)"Vibra rem Off");
        break;
    case 5:
      {
        CLIST *cl;
        cl=(CLIST *)(&cltop);
        while(cl=(CLIST *)(cl->next))
        {
         if(cl->log)
          {
            FreeLOGQ(&cl->log);
            cl->msg_count=0;
            cl->isactive=0;
          }           
        }      
        if(strlen(ContactT9Key)) ClearContactT9Key();
        RecountMenu(0,1);
        ShowMSG(1,(int)"All chat clear!");
        break;
      }
    case 6:
      if(!away_m)
      {
        away_m=!away_m;
        GBS_StopTimer(&away_tmr);
        msg_away_timer=AUTO_ENGADE*12;
        InputAwayMsg();
        
      }
      else
      {
        msg_away_timer=0;
        away_m=!away_m;
        if(change_back_status)
        {
          CurrentStatus=status_rem;
          set_my_status();
          change_back_status=0;
        }
      }
      UpdateCLheader();
      break;
    case 7:
      {
        Show_XTX=!(Show_XTX);
        if(Show_XTX)
          ShowMSG(1,(int)"Default show x-status");
        else
          ShowMSG(1,(int)"Default show time");
      }
      break;      
    case 8:
      {        
        WSHDR *ws;
        ws=AllocWS(150);
        str_2ws(ws,suc_config_name,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
      break;
    case 9:
      {        
        WSHDR *ws;
        ws=AllocWS(150);
        str_2ws(ws,suc_config_name_mod,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
      break;
    case 10:
      {
        janis=1;
        TPKT *p;
        p=malloc(sizeof(PKT)+sizeof(TTime));
        GetDateTime(0,(TTime *)p->data);
        p->pkt.uin=0;
        p->pkt.type=18;
        p->pkt.data_len=sizeof(TTime);
        SUBPROC((void *)SendAnswer,0,p);
      }
      break;
    case 11:
      {
        ClearContactT9Key();
        RecountMenu(0,1);
        silenthide=1;
        gipc.name_to=ipc_xtask_name;
        gipc.name_from=ipc_my_name;
        gipc.data=0;
        GBS_SendMessage(0x4209,0xDEB0,1,&gipc);    
      }
      break;
    case 12:
      {        
        ClearContactT9Key();
        RecountMenu(0,1);
        gipc.name_to=ipc_xtask_name;
        gipc.name_from=ipc_my_name;
        gipc.data=0;
        GBS_SendMessage(0x4209,0xDEB0,1,&gipc);
        if (IsUnlocked())
        {
          KbdLock();
        }

      }
      break;
    case 13:
     {
       if(!t->isgroup)
         CreateContactMenu(t);    
     }
     break;
    }
}



void LongPressAction(CLIST *t,int key)
{ 
  switch(key)
  {
    case '0': LongPressActionOnChat(t,longAct_0); break;
    case '1': LongPressActionOnChat(t,longAct_1); break;
    case '2': LongPressActionOnChat(t,longAct_2); break;
    case '3': LongPressActionOnChat(t,longAct_3); break;
    case '4': LongPressActionOnChat(t,longAct_4); break;
    case '5': LongPressActionOnChat(t,longAct_5); break;
    case '6': LongPressActionOnChat(t,longAct_6); break;
    case '7': LongPressActionOnChat(t,longAct_7); break;
    case '8': LongPressActionOnChat(t,longAct_8); break;
    case '9': LongPressActionOnChat(t,longAct_9); break;  
    case '*': LongPressActionOnChat(t,longAct_10); break;  
    case '#': LongPressActionOnChat(t,longAct_11); break;  
    case 0x04: LongPressActionOnChat(t,longAct_RB); break;  
  }
}

char hostbuf2[128];

void GetOnTotalContact(int group_id,int *_onlinetotal)
{
  CLIST *t;
  t=(CLIST *)&cltop;
  int online=0,total=0;
  while((t=t->next))
  {
    if(t->group==group_id && !t->isgroup)
    {
      total++;
      if (t->state!=0xFFFF) online++;
    }
  }
  _onlinetotal[0]=online;
  _onlinetotal[1]=total;
}


 

 
 
 

int itemFromDisp;
int totalItems;
int curpos=0;
int curpage=1;
int collum=0;

const char clrSelect[4]={0xFF,0x00,0x00,0x20};
const char fntSelect[4]={0xFF,0xFF,0x00,0x60};
const char bordSelect[4]={0x00,0x00,0x00,0x60};

const char clrNotSelect[4]={0x00,0x00,0x00,0x00};
const char bordNotSelect[4]={0x00,0x00,0x00,0x00};
const char fntNotSelect[4]={0xFF,0xFF,0xFF,0x60};

char clrBlack[]={0x00,0x00,0x00,0x64};
char clrWhite[]={0xFF,0xFF,0xFF,0x64};
char clrRed[]={0xFF,0x00,0x00,0x64};
char transparent[]={0x00,0x00,0x00,0x00};
char blue[]="\x00\x00\xFF\x64";

const int clfont=(8+100);
const char _t[]="%t";
const char _d[]="%d";




void RecountMenu(CLIST *req, int needfocus)
{
  int i;
  int j;
  if (!maingui_id) return; 
  if (req==0)
  {
    j=0;
  }
  else
  {
    j=FindContactByContact(req);
  }
  i=CountContacts();
  if (j>=i) j=i-1;
  if (j<0) j=0;
  if(needfocus) curpos=j;

    SMART_REDRAW();
}


void DrawStr(int x,int y,char*s,char *color,int font,int align)
{
  SetFontColor(color,transparent,0);  
  PrintField(x,y,s,align,font,1);

}

void DrawText(int x,int y,int x2,char*s,char *color,int font,int align,int isScroll)
{
  SetFontColor(color,transparent,0);  

  PrintText(x,y,s,align,font,1,350,isScroll,0,x2-x);

}

void DrawSoftKeys(char *right,char *left)
{
  TTime t;  
  char *s=malloc(32);
  int FntH=FontH(clfont);
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  char clrRed[]={0xFF,0x00,0x00,0x15};
  LockSched();
  DrawRoundedFrame(0,ScreenH()-1-FntH-2,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);
  for(int i=FontH(clfont);i>0;i-=2)
  {
    DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i,0,0,0,clrRed,clrRed);
  }
 

  
    DrawStr(2,scr_h-1-FntH,right,clrWhite,clfont,0);

  PrintField(scr_w,scr_h-1-FntH,left,2,clfont,1);          
  GetDateTime(0,&t);
  sprintf(s,"%02d:%02d",t.hour,t.min);
  PrintField(scr_w/2,scr_h-1-FntH,s,1,clfont,1);
  UnlockSched();
  
  mfree(s);
}

extern DYNPNGICONLIST *GetXSByItem(int curitem);

void DrawHeader()
{
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  char *s=malloc(128);
  char clrRed[]={0xFF,0x00,0x00,0x15};

  LockSched();
 
  SetFontColor(clrWhite,transparent,0); 
  DrawRoundedFrame(0,0,scr_w,FontH(clfont)+2,0,0,0,clrBlack,clrBlack);

  for(int i=FontH(clfont);i>0;i-=2)
  {
    DrawRoundedFrame(0,0,scr_w,FontH(clfont)-i+2,0,0,0,clrRed,clrRed);
  }
  
  int x=1;
  int icon;  
  if(total_unread) icon = IS_MSG;  
  else
  {
    switch(connect_state)
    {
     case 0:  icon=IS_OFFLINE;    break;
     case 3:  icon=CurrentStatus; break;
     default: icon=IS_UNKNOWN;    break;
    }
  }
  

  DrwImage(icons[icon],x,0+1,0,0);
  x+=icons[icon]->w+2;
 

  if(total_unread)
  {
    sprintf(s,"%d/%d",total_unread,total_unread2);
  
    DrawStr(x,0+1,s,clrWhite,clfont,0);
    x+=GetWidth(x,0+1)+1;
  }
  
  if(CurrentXStatus!=0)
  {
    DYNPNGICONLIST *xs=GetXSByItem(CurrentXStatus);
    DrwImage(xs->img,x,0+1,0,0);
    x+=xs->img->w+1;  
    
    
  }
  
  int priv=0;
  char A_s[]="[A [%s] ]";
  char t[32];
  
  switch(CurrentPrivateStatus)
  {
    case 0: priv=0; break;
    case 1: priv=4; break;
    case 2: priv=3; break;
    case 3: priv=2; break;
    case 4: priv=1; break;
    
  }
  

  

  DrwImage(icons[TOTAL_ICONS-(5-priv)],x,0+1,0,0);
  x+=icons[TOTAL_ICONS-(5-priv)]->w;
  int cap;   
  int x2=scr_w;    
  
  if(on_off_timer > 0 && strlen(on_off_buf)!=0)
  {
     SetScrollEdgeDelay(1);
     DrawText(x+2,0+1,scr_w,on_off_buf,clrWhite,clfont,0,1);
  }
  else
    if(strlen(ContactT9Key))
    {
      sprintf(s,key_clm_hdr_text);
      strcat(s,ContactT9Key);
  
      strcpy(clmenu_sk_r,key_clmenu_sk_r);
      PrintField(scr_w,0+1,s,2,clfont,1);
    }
  else
  {
    x2=scr_w;
    cap=*RamCap();   
    sprintf(s,"%d%%",cap); 
    PrintField(scr_w,0+1,s,2,clfont,1);
    x2=scr_w-GetWidth(scr_w,0+1);
    strcpy(clmenu_sk_r,def_clmenu_sk_r);   
    Get_Online_total();
    sprintf(s,"[%d/%d]",kopa_kontaktu,kopa_online);
    PrintField(x2-2,0+1,s,2,clfont,1);
    x2-=GetWidth(x2-2,0+1);
    sprintf(s,"");
    if(away_m && isShowA) {  sprintf(t,A_s,away_msg_time_dace); strcat(s,t); } 
    if(Is_Vibra_Enabled && isShowV)        strcat(s,"[V]") ;
    if(Is_Sounds_Enabled && isShowS)       strcat(s,"[S]") ;
    if(Is_Double_Vibra_On && isShowR)      strcat(s,"[R]") ;
    PrintField(x2-2,0+1,s,2,clfont,1);
  }




 
  UnlockSched();
  mfree(s);
}

void DrawScrollBar(int cur,int total)
{
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  int y=scr_h-(0+FontH(clfont)+4)-FontH(clfont)-2;
  int pos=y*cur/total;
  int y2=y/total;
  DrawLine(scr_w-2,(0+FontH(clfont)+4),scr_w-2,scr_h-FontH(clfont)-2,1,clrBlack);
  int i=0;
  DrawRoundedFrame(scr_w-4,(0+FontH(clfont)+4)+pos+1,scr_w,(0+FontH(clfont)+4)+pos+y2,0,0,0,clrBlack,clrBlack);
  for(i=0;i<y2;i+=1)
  {
    DrawRoundedFrame(scr_w-4,(0+FontH(clfont)+4)+pos+1,scr_w,(0+FontH(clfont)+4)+pos+i,0,0,0,clrRed,clrRed);
  }
}


void DrwImage(IMGHDR *img, int x, int y, char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}


int LINE_MODE=0;
int VIEW_MODE=1;


GBSTMR cltmr;
void MyRedraw()
{
  
  SMART_REDRAW();
}


void DrawJust(RECT *rc,CLIST *cl,char *name)
{
  

  
  if(!cl->just_i)
  {
    DrawText(rc->x+1,rc->y,rc->x2,name,blue,clfont+1,0,0);
    




 

  }
  else
  {
    DrawText(rc->x+1,rc->y,rc->x2,name,clrRed,clfont+1,0,0);





 
  }



 cl->just_i=!cl->just_i;

}


void DrawList()
{
  if(!cltop) return;
  int font_y = FontH(clfont)+2;
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  int y2;
  if(VIEW_MODE)
    itemFromDisp = (sdiv(font_y,ScreenH()-(0+FontH(clfont)+4)-FontH(clfont)-2)*2)-1;
  else
    itemFromDisp = sdiv(font_y,ScreenH()-(0+FontH(clfont)+4)-FontH(clfont)-2)-1;
  CLIST *cl;
  int i=0;
  int start_y;
  int x=0;  
  RECT rc;
  int icon;
  collum=0;
  int clfont=108;
  char *name=malloc(128);
  int yy=0;
  DYNPNGICONLIST *xs;
  
  LockSched();
  totalItems = CountContacts(); 
 
  
  int pageNum = sdiv(itemFromDisp, totalItems);
  
  if(itemFromDisp > ((0+FontH(clfont)+4) - (2-FontH(clfont)))) itemFromDisp--; 
  if(LINE_MODE) itemFromDisp--;

  
  if( itemFromDisp*pageNum < totalItems ) pageNum++; 

  while(i<totalItems)
  {
     if((i>=(curpage-1)*itemFromDisp) && (i<=itemFromDisp*curpage))
        {
          cl=FindContactByN(i);
          
          if(cl)
          {
            start_y=0;
            if(LINE_MODE)
            { 
              if(i>curpos) start_y+=font_y+3;
              y2=2; 
            }
            else y2=1;
          
            if(VIEW_MODE)
            {
              if(!collum)
              {
                start_y += ((0+FontH(clfont)+4) + (((i - (curpage-1)*itemFromDisp)*font_y)/2) );    
                yy=start_y;
                StoreXYXYtoRECT(&rc, 1, start_y, (scr_w/2)-1, start_y+(font_y*y2));
              }
              else
              {
                StoreXYXYtoRECT(&rc, scr_w/2, yy, scr_w-5, yy+(font_y*y2));
              }
              collum=!collum; 
            }
            else
            {
              start_y += ((0+FontH(clfont)+4) + 2 + (i - (curpage-1)*itemFromDisp)*font_y);    
              StoreXYXYtoRECT(&rc,2,start_y,scr_w-5,start_y+(font_y*y2));                          
            }
            
            icon=GetIconIndex(cl);
            if (icon!=IS_GROUP)
            {
              sprintf(name,percent_s,cl->name);
            }
            else
            {
              int onlinetotal[2];
              GetOnTotalContact(cl->group,onlinetotal);
              if(!VIEW_MODE)
              {
                char *s=malloc(32);
                sprintf(name,percent_s,cl->name);
                sprintf(s,"(%d/%d)",onlinetotal[0],onlinetotal[1]);
                SetFontColor((char*)clrBlack,transparent,0);  
                PrintField(scr_w-5,start_y,s,2,clfont,1);
                mfree(s);
              }
              else
               sprintf(name,"%s%c%c(%d/%d)",cl->name,0xE01D,0xE012,onlinetotal[0],onlinetotal[1]);
              if (cl->state) icon++; 
            }
            
           
            DrwImage(icons[icon],rc.x,rc.y,0,0);
           
            rc.x+=icons[i]->w+2;
          
            if(i==curpos)
            {
              DrawRoundedFrame(rc.x-1,rc.y-1,rc.x2-1,rc.y2-1,0,0,0,bordSelect,clrSelect);
              if(LINE_MODE && icon!=IS_GROUP)
              {
                char s[64];
                if ((cl->xtext && cl->xtext[0]!=0) && (cl->isShowXst) && (cl->state!=0xFFFF))
                  {
                    int i;
                    zeromem(s,64);
                    i=cl->xtext[0];
                    if (i>63) i=63;
                    strncpy(s,cl->xtext+1,i);                    
                  }
                  else
                    if(cl->xtext2 && cl->xtext2[0]!=0 && isLogStatusChange)
                    {
                      int i;
                      zeromem(s,64);
                      i=cl->xtext2[0];
                      if (i>63) i=63;
                      strncpy(s,cl->xtext2+1,i);
                    }
                  else
                    sprintf(s,percent_d,cl->uin);
                SetFontColor((char*)clrBlack,transparent,0);  
                PrintText(rc.x+x+1,rc.y+font_y,s,0,clfont,1,0,0,0,rc.x2);
              }
              
              if(totalItems>1)
                DrawScrollBar(i,totalItems);
            }
            
            if(cl->xstate!=0)
            {
              xs=GetXSByItem(cl->xstate);
              if(xs)
              {
                DrwImage(xs->img,rc.x-x,rc.y,0,0);
                rc.x+=xs->img->w;
              }

  
            }

            if(cl->just_now)
            {
              DrawJust(&rc,cl,name);              
            }
            else

              DrawText(rc.x+1,rc.y,rc.x2,name,(cl->isactive!=0)?(clrRed):(clrBlack),clfont,0,0);
               

  
    

   
            
     
         }
       }
      i++;
      if(i > (itemFromDisp*curpage)) break;  
  }
  UnlockSched();  
  mfree(name);
}




void MoveUp(int i)
{
  if(!itemFromDisp)return;



 
  
  if(i) curpos-=2;
  else  curpos--;    
  
  if(i && curpos+2==2)
  {
    curpos=0;
    curpage=1;
  }
  else
    if(curpos<0)
    {
      curpos=totalItems-1;
    
      if(itemFromDisp!=totalItems)
      {
        curpage =  sdiv(itemFromDisp,totalItems)+1;
      }
     else
      curpage = 1;
    }
 




 
}


void MoveDown(int i)
{
  if(!totalItems)return;
  if(i) curpos+=2;
  else  curpos++;  
  if(curpos>=totalItems) { curpos=0; curpage=1; }
  if(curpage*itemFromDisp<curpos){ curpos--; curpage++; } 
}

void DrawContactList()
{
  DrawRoundedFrame(0,0+1,ScreenW()-1,ScreenH()-1,0,0,0,clrWhite,clrWhite);
 
 
  DrawSoftKeys("Меню",clmenu_sk_r);
  DrawHeader();
 
  DrawList();
}













































 

int ShowLoadScreen=1;

int ClOnKey(GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  CLIST *t;
  t=FindContactByN(curpos); 
     
  if (msg->gbsmsg->msg==0x195)
  {
    switch(msg->gbsmsg->submess)
    {
    case 0x3B:    if(VIEW_MODE) MoveUp(1);   else MoveUp(0);   break;
    case 0x3C:  if(VIEW_MODE) MoveDown(1); else MoveDown(0); break;
    case 0x3E: if(VIEW_MODE) MoveDown(0); else MoveDown(1); break;
    case 0x3D:  if(VIEW_MODE) MoveUp(0);   else MoveUp(1);   break;
    }

    LongPressAction(t,msg->gbsmsg->submess);
    
    int key=msg->gbsmsg->submess;
    if ((key>='0')&&(key<='9'))
    {
      ClearContactT9Key();
      RecountMenu(0,1);
    }
  }
  
  
  if (msg->gbsmsg->msg==0x193)
  {
    switch(msg->gbsmsg->submess)
    {
    case 0x3B:    if(VIEW_MODE) MoveUp(1);   else MoveUp(0);   break;
    case 0x3C:  if(VIEW_MODE) MoveDown(1); else MoveDown(0); break;
    case 0x3E: if(VIEW_MODE) MoveDown(0); else MoveDown(1); break;
    case 0x3D:  if(VIEW_MODE) MoveUp(0);   else MoveUp(1);   break;
      case 0x01: 
      {
        extern void CreateMainMenu();
     
        CreateMainMenu();

      }break;
    case 0x04:
    case 0x0C:
      if (strlen(ContactT9Key))
      {
        BackSpaceContactT9();
        RecountMenu(0,1);
        return(-1);
      }
      else ShowLoadScreen=1;
      break;
    case 0x1A:
     {
      if (t)
      {
        if (t->isgroup)
        {
          t->state^=0xFFFF;
          RecountMenu(t,1);
	  return(-1);
        }
        if (strlen(ContactT9Key))
        {
    	  ClearContactT9Key();
	  RecountMenu(0,1);
       }
       if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
       CreateEditChat(t);
       }
      return(-1);
      }
      case '*':
        if(CountContacts()>0)
         {
          while(t=(CLIST *)(t->next))
           {
             if (t->isunread) goto doit;
           }
          t=(CLIST *)(&cltop);
          while(t=(CLIST *)(t->next))
          {
            if (t->isunread) goto doit;
          }
          if (t && Is_Show_Groups)
          {
           if (!t->isgroup) t=FindGroupByID(t->group);
           if (t && !t->state)
           {
            t->state^=0xFFFF;
            goto does;
           }
          }
          
    









 
       doit:
          {
            CLIST *g=FindGroupByID(t->group);
            if (g)
            {
              if (g->state)
              {
                g->state=0;
              }
            }
          }
        does:
          RecountMenu(t,1);
          return(-1);
         }
      break;
    case 0x0B:
      {
        IsActiveUp=!IsActiveUp;
        RecountMenu(0,1);
      }
      break;
  
    case 0x0D:
       if(isVolUpButton)
         CreateScreenSelectGUI();
       break;
    case 0x0E:
      {
        if(isVolDownButton)
        {
         if(!away_m)
          {
            away_m=!away_m;
            main_d=1;
            InputAwayMsg();
          }
          else
          {
            msg_away_timer=0;
            away_m=!away_m;
            if(change_back_status)
            {
              CurrentStatus=status_rem;
              set_my_status();
              change_back_status=0;
            }
          }
        }
      }
      break;      
    }
    
    if (key=='#' && (CountContacts()>0))
    {
      if(ActionResh)
      {
        while(t=(CLIST *)(t->next))
         {
           if (t->isactive) goto doit4;
         }
        t=(CLIST *)(&cltop);
        while(t=(CLIST *)(t->next))
        {
          if (t->isactive) goto doit4;
        }
        return(-1);
      doit4:
        {
        CLIST *g=FindGroupByID(t->group);
         if (g)
         {
          if (g->state)
          {
            g->state=0;
          }
         }
        }
       RecountMenu(t,1);
      }
      else
      {
       if(t->log && !(t->just_now))
        {
          FreeLOGQ(&t->log);
          t->msg_count=0;
          t->isactive=0;
        }
       
       if(t->just_now)
       {
         CLIST *tt;
         tt=FindContactByUin(t->uin);
         tt->just_now=0;
       }
       
       if(t->uin==UIN)
       {
        CLIST *tt;
        tt=(CLIST *)(&cltop);
        while(tt=(CLIST *)(tt->next))
        {
          if(tt->just_now) tt->just_now=0;
        }
       }
       if(strlen(ContactT9Key)) ClearContactT9Key();
       RecountMenu(0,0);
      }
    }   
  }  
 
  if ((key>='0')&&(key<='9') && msg->gbsmsg->msg==0x193)
    {
      AddContactT9Key(key);
      RecountMenu(0,1);
      return(-1);
    }   
  return 0;  
}











































 




 





int DNR_ID=0;
int DNR_TRIES=3;

char hostname[128]; 

int host_counter = 0;



const char *GetHost(int cnt, const char *str, char *buf)
{
  const char *tmp = str, *begin, *end;
  if(cnt)
  {
    for(;cnt;cnt--)
    {
      for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
      if(!*str) str = tmp;
      for(;(*str==';' || *str==' ' || *str=='\x0D' || *str=='\x0A') && *str; str++);
      if(!*str) str = tmp;
    }
  }
  tmp = buf;
  begin = str;
  for(;*str!=';' && *str!=':' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
  end = str;
  for(;begin<end; *buf = *begin, begin++, buf++);
  *buf = 0;
  return tmp;
}

int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (int k=0; ; k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A) {ret=ret*10+attr[k]-0x30;} else { if ( attr[k]=='-') {neg=-1;} else {return(ret*neg);}}
  }
}

int GetPort(int cnt, const char *str)
{
  const char *tmp = str;
  char numbuf[6], numcnt = 0;
  if(cnt)
  {
    for(;cnt;cnt--)
    {
      for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
      if(!*str) str = tmp;
      for(;(*str==';' || *str==' ' || *str=='\x0D' || *str=='\x0A') && *str; str++);
      if(!*str) str = tmp;
    }
  }
  for(;*str!=';' && *str!=':' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
  if(*str!=':') return NATICQ_PORT;
  str++;
  numbuf[5] = 0;
  for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str && numcnt<5; numbuf[numcnt] = *str, str++, numcnt++);
  numbuf[numcnt] = 0; 
  return atoi(numbuf);

}


int GetHostsCount(const char *str)
{
  char cnt = 1;
  for(;cnt;cnt++)
  {
    for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
    if(!*str) return cnt;
    for(;(*str==';' || *str==' ' || *str=='\x0D' || *str=='\x0A') && *str; str++);
    if(!*str) return cnt;
  }
  return 0;
}


void create_connect(void)
{

  char hostbuf[128], buf[128];
  int hostport;
  int ***p_res=0;
  void do_reconnect(void);
  SOCK_ADDR sa;
  
  connect_state = 0;
  int err;
  unsigned int ip;
  GBS_DelTimer(&reconnect_tmr);
  if (!IsGPRSEnabled())
  {
    is_gprs_online=0;
    strcpy(logmsg,"Waiting for GPRS up...");
    SMART_REDRAW();
    return;
  }
  DNR_ID=0;
  *socklasterr()=0;
  if(host_counter > GetHostsCount(NATICQ_HOST)-1) host_counter = 0; 
  GetHost(host_counter, NATICQ_HOST, hostbuf);
  hostport = GetPort(host_counter, NATICQ_HOST);
  host_counter++;

  sprintf(hostname, "Connect to %s:%d", hostbuf, hostport); 
  
  zeromem(hostbuf2,128);
  sprintf(hostbuf2, percent_s,hostbuf);
  strcpy(logmsg,buf);
  SMART_REDRAW();
    
  ip=str2ip(hostbuf);
  if (ip!=0xFFFFFFFF)  
  {
    sa.ip=ip;
    strcpy(logmsg,"\nConnect by IP!"); 
    SMART_REDRAW();
    goto L_CONNECT;
  }  
  strcat(logmsg,"\n");
  strcpy(logmsg,"Send DNR..."); 
  SMART_REDRAW();
  err=async_gethostbyname(hostbuf,&p_res,&DNR_ID); 
  if (err)
  {
    if ((err==0xC9)||(err==0xD6))
    {
      if (DNR_ID)
      {
       host_counter--;
       return; 
      }
    }
    else
    {
      snprintf(logmsg,255,"DNR ERROR %d!",err);
      SMART_REDRAW();
      GBS_StartTimerProc(&reconnect_tmr,216*RECONNECT_TIME,do_reconnect);
      return;
    }
  }
  if (p_res)
  {
    if (p_res[3])
    {
      strcpy(logmsg,buf);
      strcat(logmsg,"\n");
      strcpy(logmsg,"DNR Ok, connecting...");
      SMART_REDRAW();
      DNR_TRIES=0;
      sa.ip=p_res[3][0][0];
    L_CONNECT:
      sock=socket(1,1,0);
      if (sock!=-1)
      {
	sa.family=1;
	sa.port=htons(hostport);
	
	if (connect(sock,&sa,sizeof(sa))!=-1)
	{
	  connect_state=1;
	  TOTALRECEIVED=0;
	  TOTALSENDED=0;
	  SMART_REDRAW();
	}
	else
	{
	  closesocket(sock);
	  sock=-1;
	  LockSched();
	  ShowMSG(1,(int)"Невозможно подключиться!");
	  UnlockSched();
	  GBS_StartTimerProc(&reconnect_tmr,216*RECONNECT_TIME,do_reconnect);

	}
      }
      else
      {
	LockSched();
	ShowMSG(1,(int)"Невозможно создать сокет, перезапуск GPRS!");
	UnlockSched();
	
	GPRS_OnOff(0,1);
      }
    } 
  }
  else
  {
    DNR_TRIES--;
    LockSched();
    ShowMSG(1,(int)"Сервер не найден!");
    UnlockSched();
  }
}


void ClearSendQ(void)
{
  mfree((void *)sendq_p);
  sendq_p=0;
  sendq_l=0;
}

void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
  }
}


void SendAnswer(int dummy, TPKT *p)
{
  int i;
  int j;
  if (connect_state<2)
  {
    mfree(p);
    return;
  }
  if (p)
  {
    j=sizeof(PKT)+p->pkt.data_len; 
    TOTALSENDED+=j;
    ALLTOTALSENDED+=j;			
    
    if (sendq_p)
    {
      
      sendq_p=realloc((void *)sendq_p,sendq_l+j);
      memcpy((char *)sendq_p+sendq_l,p,j);
      mfree(p);
      sendq_l+=j;
      return;
    }
    sendq_p=p;
    sendq_l=j;
  }
  
  while((i=sendq_l)!=0)
  {
    if (i>0x400) i=0x400;
    j=send(sock,(void *)sendq_p,i,0);
    snprintf(logmsg,255,"send res %d",j);
    SMART_REDRAW();
    if (j<0)
    {
      j=*socklasterr();
      if ((j==0xC9)||(j==0xD6))
      {
	
	strcpy(logmsg,"Send delayed...");
	return; 
      }
      else
      {
	
	LockSched();
	ShowMSG(1,(int)"Send error!");
	UnlockSched();
	end_socket();
	return;
      }
    }
    memcpy((void *)sendq_p,(char *)sendq_p+j,sendq_l-=j); 
    if (j<i)
    {
      
      return; 
    }
    tenseconds_to_ping=0; 
  }
  mfree((void *)sendq_p);
  sendq_p=0;
}


void send_login(int dummy, TPKT *p)
{
  connect_state=2;
  char rev[16];
  
  
  snprintf(rev,9,"Sie_%04d",2058);
  
  TPKT *p2=malloc(sizeof(PKT)+8);
  p2->pkt.uin=UIN;
  p2->pkt.type=32;
  p2->pkt.data_len=8;
  memcpy(p2->data,rev,8);
  SendAnswer(0,p2);
  SendAnswer(dummy,p);
  RXstate=-(int)sizeof(PKT);
}

void do_ping(void)
{
  TPKT *pingp=malloc(sizeof(PKT));
  pingp->pkt.uin=UIN;
  pingp->pkt.type=0;
  pingp->pkt.data_len=0;
  SendAnswer(0,pingp);
}

void SendMSGACK(int i)
{
  TPKT *ackp=malloc(sizeof(PKT));
  ackp->pkt.uin=i;
  ackp->pkt.type=23;
  ackp->pkt.data_len=0;
  SendAnswer(0,ackp);
}

void RequestXText(unsigned int uin)
{
  TPKT *p=malloc(sizeof(PKT));
  p->pkt.uin=uin;
  p->pkt.type=24;
  p->pkt.data_len=0;
  SUBPROC((void *)SendAnswer,0,p);
}

extern int main2_count;

int ping_time;
char I_UIN[9];

void get_answer(void)
{
  void *p;
  int i=RXstate;
  int j;
  int n;
  char rb[1024];
  char *rp=rb;
  if (connect_state<2) return;
  if (i== -10) return;
  j=recv(sock,rb,sizeof(rb),0);
  while(j>0)
  {
    if (i<0)
    {
      
      n=-i; 
      if (j<n) n=j; 
      memcpy(RXbuf.data+i,rp,n); 
      i+=n;
      j-=n;
      rp+=n;
    }
    if (i>=0)
    {
      
      n=RXbuf.pkt.data_len; 
      if (n>16383)
      {
	
	strcpy(logmsg,"Bad packet");
	end_socket();
	RXstate= -10;
	return;
      }
      n-=i; 
      if (n>0)
      {
	if (j<n) n=j; 
	memcpy(RXbuf.data+i,rp,n);
	i+=n;
	j-=n;
	rp+=n;
      }
      if (RXbuf.pkt.data_len==i)
      {
	
	TOTALRECEIVED+=(i+8);
	ALLTOTALRECEIVED+=(i+8);			
	
	RXbuf.data[i]=0; 
	switch(RXbuf.pkt.type)
	{
	case 4:
	  
	  
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  GBS_SendMessage(0x4209,0xDEAE,0,p,sock);
	  Play(sndStartup);
	  
	  snprintf(logmsg,255,"%s",RXbuf.data);
	  connect_state=3;
          host_counter--;
	  SMART_REDRAW();
	  break;
	case 25:
	case 20:
	case 21:
	case 7:
	  
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  GBS_SendMessage(0x4209,0xDEAE,0,p,sock);
	  
	  break;
	case 9:
	  n=i+sizeof(PKT);
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  snprintf(logmsg,255,"SC%d: %04X",RXbuf.pkt.uin,*((unsigned short *)(RXbuf.data)));
	  GBS_SendMessage(0x4209,0xDEAE,0,p,sock);
	  break;
	case 6:
	  snprintf(logmsg,255,"ERR: %s",RXbuf.data);
	  SMART_REDRAW();
	  break;
	case 3:
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  {
	    char *s=p;
	    s+=sizeof(PKT);
	    int c;
	    while((c=*s))
	    {
	      if (c<3) *s=' ';
	      s++;
	    }
	  }          
          

      
         
          void SetIconBarHandler();
          SetIconBarHandler();
          sprintf(I_UIN, "uin%d", RXbuf.pkt.uin);
          if(strstr(ignor_list,I_UIN)==0)
          {
            snprintf(logmsg,255,"MSG%d: %s",RXbuf.pkt.uin,RXbuf.data);
            SendMSGACK(TOTALRECEIVED);
            GBS_SendMessage(0x4209,0xDEAE,0,p,sock);
            SMART_REDRAW();
            if(!extended_sounds)
            {
              CLIST *t;
              t=FindContactByUin(RXbuf.pkt.uin);
              if(t->isIndividSet)
              {
                if(t->issound) Play(sndMsg);
                break;
              }
              else
                if(t->automsgcount < automsgcount && away_m)
                {
                  Play(sndMsg);
                  break;
                }
              else
               if(t->isaccess)
                {
                  if(t->signals) Play(sndMsg);
                  break;
                }              
              else
                Play(sndMsg);
            }
            else
            {
              uins=RXbuf.pkt.uin;
              volatile int hFile_ex;
              unsigned int io_error_ex = 0;
              sprintf(talk_sound, "%s%d.wav", sndMsg2, uins);
              hFile_ex = fopen(talk_sound,0 + 0x8000,0x80, &io_error_ex);
              if(hFile_ex!=-1)
              {
                fclose(hFile_ex, &io_error_ex);
                Play(talk_sound);
              }
              else
                Play(sndMsg);
            }
          }
	  break;
	case 13:
	  LockSched();
	  ShowMSG(1,(int)RXbuf.data);
	  UnlockSched();
	  break;
	case 16:
	  if (FindContactLOGQByAck(&RXbuf)) Play(sndMsgSent);
	case 17:
	  p=malloc(sizeof(PKT)+2);
	  memcpy(p,&RXbuf,sizeof(PKT)+2);
	  GBS_SendMessage(0x4209,0xDEAE,0,p,sock);
	  break;
	case 19:
	  {
	    TDate d;
	    TTime t;
	    TTime *pt=(TTime *)(RXbuf.data);
	    int s1;
	    int s2;
	    GetDateTime(&d,&t);
	    s1=t.hour*3600+t.min*60+t.sec;
	    s2=pt->hour*3600+pt->min*60+pt->sec;
	    s1-=s2;
	    if (s1<0) s1+=86400;
	    snprintf(logmsg,255,"Ping %d-%d seconds!",s1,s1+1);
            sprintf(PINGO, "Ping: %d sec",s1);
	    LockSched();
            if(janis==1)
            {
              ShowMSG(1,(int)logmsg);
              Play(pingsnd);
              janis=0;
            }
            else
            {
              if(ping_time==0)
              {
                ping_time=ping_time_config;
                Play(pingsnd);
              }
              else
                ping_time--;
            }
	    UnlockSched();
	  }
	  break;
        case 31:
          n=i+sizeof(PKT);
          p=malloc(n);
          memcpy(p,&RXbuf,n);
	  GBS_SendMessage(0x4209,0xDEAE,0,p,sock);
	  break;
	}
	i=-(int)sizeof(PKT); 
      }
    }
  }
  RXstate=i;
  
  
}


extern int main_count;





char a_txt[45];
char s_txt[45];
char d_txt[45];
char f_txt[45];
char g_txt[45];
unsigned int main_msg_timer2;
void AddToMain(const char *nameu,char *si)
{
  char *de=malloc(64);
  int text_l;
  int iz;
  int name_l;
  name_l=0;
  text_l=0;
  name_l=strlen(nameu);
  iz=0;
  text_l=main_lenght-name_l;
  
  if(strlen(si)>text_l)
  {
    while(text_l!=0)
    {
      de[iz]=si[iz];
      iz++;
      text_l--;
    }
    if(use_cutter)
    {
      de[iz]='.';
      iz++;
      de[iz]='.';
      iz++;
      de[iz]='.';
      iz++;
    }
    de[iz]='\0';
  }
  else
     strcpy(de, si);
  iz=0;
  while(strlen(de)!=iz)
   {
     if(de[iz]==0x0D) de[iz]=0x20;
     if(de[iz]==0x0A) de[iz]=0x20;
     iz++;
   }
  de[iz]='\0';
  if(main_count==0) sprintf(a_txt, "%s: %s", nameu,de);
  if(main_count==1) sprintf(s_txt, "%s: %s", nameu,de);
  if(main_count==2) sprintf(d_txt, "%s: %s", nameu,de);
  if(main_count==3) sprintf(f_txt, "%s: %s", nameu,de);
  if(main_count==4) sprintf(g_txt, "%s: %s", nameu,de);
  if(main_count>4)
  {
    strcpy(a_txt, s_txt);
    strcpy(s_txt, d_txt);
    strcpy(d_txt, f_txt);
    strcpy(f_txt, g_txt);
    sprintf(g_txt, "%s: %s", nameu,de);
  }
  mfree(de);
  main_d=1;
}



int NEW_MSG;

void AddStringToLog(CLIST *t, int code, char *s, const char *name, unsigned int IDforACK,int flag)
{
  char hs[128], *lastX;
  TTime tt;
  TDate d;
  GetDateTime(&d,&tt);
  int i;
  

  if ( code==3 && NOT_LOG_SAME_XTXT )
  {
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    lastX = GetLastXTextLOGQ(t);
    if(lastX)
      if(strcmp(lastX, s) == 0) return;
  }
  
  snprintf(hs,127,"%02d:%02d:%02d %02d.%02d %s:\r\n",tt.hour,tt.min,tt.sec,d.day,d.month,name);
  if(code != 3 || ((LOG_XTXT) && (flag)))
   Add2History(t, hs, s, code); 

  LOGQ *p=NewLOGQ(s);
  snprintf(p->hdr,79,"%02d:%02d:%02d %02d.%02d %s:",tt.hour,tt.min,tt.sec,d.day,d.month,name);
  p->type=code;
  p->ID=IDforACK;  
  i=AddLOGQ(&t->log,p);
  while(i>(20))
  {
    if (t->log==t->last_log) t->last_log=t->last_log->next;
    RemoveLOGQ(&t->log,t->log);
    i--;
  }
  t->msg_count=i;
  if (!t->last_log) t->last_log=p;
  if (code==3)
  {
    if (edchat_id)
    {
      void *data=FindGUIbyId(edchat_id,0);
      if (data)
      {
	EDCHAT_STRUCT *ed_struct;
	ed_struct=EDIT_GetUserPointer(data);
	if (ed_struct) 
	{
	  if (ed_struct->ed_contact==t)
	  {
	    goto L_INC;
	  }
	}
      }
    }
    goto L_NOINC;
  }
L_INC:
  if (!t->isunread) total_unread++;  
  if(NEW_MSG==1)
  {
    t->unread_msg++;
    total_unread2++;
    NEW_MSG=0;
  }
  t->isunread=1;
L_NOINC:
  ChangeContactPos(t);
}

void ParseAnswer(WSHDR *ws, const char *s);

int time_to_stop_t9;

void ParseXStatusText(WSHDR *ws, const char *s, int color)
{
  int c;
  int flag=0;
  CutWSTR(ws,0);
  if (strlen(s)==1) return;
  wsAppendChar(ws,0xE008);
  wsAppendChar(ws,color);
  wsAppendChar(ws,0xE013);
  while((c=*s++))
  {
    if (c==13)
    {
      if (!flag)
      {
        flag=1;
        wsAppendChar(ws,0xE012);
        c=' ';
      }
    }
    wsAppendChar(ws,char8to16(c));
  }
}


void AddMsgToChat(void *data)
{
  LOGQ *p;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  EDCHAT_STRUCT *ed_struct;
  int j;
  int color, font, type;
  int zc;
  if (!data) return;
  ed_struct=EDIT_GetUserPointer(data);
  if (!ed_struct) return;
  if (!ed_struct->ed_contact->isunread) return;
    
  p=ed_struct->ed_contact->last_log;
  if (p)
  {
    while(p)
    {
      font = ED_H_FONT_SIZE;
      if ((zc=p->acked&3))
      {
	if (zc==1)
	  color=ACK_COLOR; 
	else
	  color=I_COLOR;
      }
      else
      {
	if (p->ID==0xFFFFFFFF)
	  {
          type = p->type&0x0F;
          if(p->type&0x10)
          {
            color = (type==1)?O_I_COLOR:((type==3)?O_X_COLOR:O_TO_COLOR);
            font = (type==3)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
          }
          else
          {
            color = (type==1)?I_COLOR:((type==3)?X_COLOR:TO_COLOR);
            font = (type==3)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
          }
        }
        else
	  color=UNACK_COLOR; 
      }
      PrepareEditControl(&ec);
      if ((p->type&0x0F)!=3)
      {
        ascii2ws(ews,p->hdr);
        ConstructEditControl(&ec,1,0x40,ews,ews->wsbody[0]);
      }
      else
        ConstructEditControl(&ec,1,0x20,ews,0);      
      PrepareEditCOptions(&ec_options);
      SetPenColorToEditCOptions(&ec_options,color);
      SetFontToEditCOptions(&ec_options,font);
      CopyOptionsToEditControl(&ec,&ec_options);
      
      EDIT_InsertEditControl(data,ed_struct->ed_answer-1,&ec);
      ed_struct->ed_answer++;
      if ((p->type&0x0F)!=3)
      {
        ParseAnswer(ews,p->text);
      }
      else
      {
        ParseXStatusText(ews, p->text, (p->type&0x10)?O_X_COLOR:X_COLOR);
      }
      PrepareEditControl(&ec);
      ConstructEditControl(&ec,
                           3,
                           ews->wsbody[0] ? 0x40|0x2000 : 0x20,
                           ews,ews->wsbody[0]);
      PrepareEditCOptions(&ec_options);

      if ((p->type&0x0F)!=3)
      {
        SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_FONT_SIZE:ED_FONT_SIZE);
      }
      else
      {
        SetPenColorToEditCOptions(&ec_options,(p->type&0x10)?O_X_COLOR:X_COLOR); 
        SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_X_FONT_SIZE:ED_X_FONT_SIZE);
      }



      CopyOptionsToEditControl(&ec,&ec_options);
      EDIT_InsertEditControl(data,ed_struct->ed_answer-1,&ec);
      ed_struct->ed_answer++;
      p=p->next;
    }
    j=((ed_struct->ed_contact->msg_count+1)*2); 
    while(j<ed_struct->ed_answer)
    {
      EDIT_RemoveEditControl(ed_struct->ed_chatgui,1);
      ed_struct->ed_answer--;
    }
  }
  ed_struct->ed_contact->last_log=0;
  if (IsGuiOnTop(edchat_id)) 
  {
    total_unread--;
  }
  else
    ed_struct->requested_decrement_total_unread++;
  ed_struct->ed_contact->isunread=0;
  total_unread2=total_unread2-ed_struct->ed_contact->unread_msg;
  ed_struct->ed_contact->unread_msg=0;
  ChangeContactPos(ed_struct->ed_contact);
  
}


void DrawAck(void *data)
{
  LOGQ *p;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  EDCHAT_STRUCT *ed_struct;
  int j;
  int color;
  int dorefresh=0;
  if (!data) return;
  ed_struct=EDIT_GetUserPointer(data);
  if (!ed_struct) return;
  p=ed_struct->ed_contact->log;
  j=ed_struct->ed_answer;
  j-=(ed_struct->ed_contact->msg_count*2)+1; 
  while(p)
  {
    if (j>=1) 
    {
      if (!(p->acked&4))
      {
	ExtractEditControl(data,j,&ec);
	PrepareEditCOptions(&ec_options);
	if (p->acked)
	{
	  if (p->acked==1)
	    color=ACK_COLOR; 
	  else
	    color=I_COLOR;
	}
	else
	{
	  if (p->ID==0xFFFFFFFF)
	    color=(p->type&0x10)?(((p->type&0x0F)==1)?O_I_COLOR:O_TO_COLOR):(((p->type&0x0F)==1)?I_COLOR:TO_COLOR);
	  else
	    color=UNACK_COLOR; 
	}
	SetPenColorToEditCOptions(&ec_options,color );
	SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_H_FONT_SIZE:ED_H_FONT_SIZE);
	CopyOptionsToEditControl(&ec,&ec_options);
	StoreEditControl(data,j,&ec);
	p->acked|=4; 
	dorefresh=1;
      }
    }
    j+=2;
    p=p->next;
  } 
  if (IsGuiOnTop(edchat_id)&&dorefresh) RefreshGUI();
}

void ask_my_info(void)
{
  






 
}

void set_my_status(void)
{
  TPKT *p;
  p=malloc(sizeof(PKT)+1);
  p->pkt.uin=0;               
  p->pkt.type=15; 
  p->pkt.data_len=1;          
  p->data[0]=CurrentStatus;
  SUBPROC((void *)SendAnswer,0,p);
}

extern int Send0;

void set_my_xstatus(void)
{
  if(Is_Id3tag_Enabled)
  {
    if(CurrentXStatus!=11)
    {
      TPKT *p;
      char *s1;
      char *s2;
      int l1;
      int l2;
      p=malloc(sizeof(PKT)+1);
      p->pkt.uin=0;               
      p->pkt.type=22; 
      p->pkt.data_len=1;          
      p->data[0]=CurrentXStatus;
      SUBPROC((void *)SendAnswer,0,p);
      s1=GetXStatusStr(CurrentXStatus*3+1,&l1);
      s2=GetXStatusStr(CurrentXStatus*3+2,&l2);
      if ((!s1)||(!s2)) return;
      p=malloc(sizeof(PKT)+l1+l2+1);
      p->pkt.uin=0;
      p->pkt.type=26;
      p->pkt.data_len=l1+l2+1;
      strncpy(p->data+0,s1,l1);
      p->data[l1]=0;
      strncpy(p->data+l1+1,s2,l2);
      SUBPROC((void *)SendAnswer,0,p);
    }
    else
    {
      tracktimer=0;
      GetTags();
    }
  }
  else
  {
    TPKT *p;
    char *s1;
    char *s2;
    int l1;
    int l2;
    p=malloc(sizeof(PKT)+1);
    p->pkt.uin=0;               
    p->pkt.type=22; 
    p->pkt.data_len=1;          
    p->data[0]=CurrentXStatus;
    SUBPROC((void *)SendAnswer,0,p);
    s1=GetXStatusStr(CurrentXStatus*3+1,&l1);
    s2=GetXStatusStr(CurrentXStatus*3+2,&l2);
    if ((!s1)||(!s2)) return;
    p=malloc(sizeof(PKT)+l1+l2+1);
    p->pkt.uin=0;
    p->pkt.type=26;
    p->pkt.data_len=l1+l2+1;
    strncpy(p->data+0,s1,l1);
    p->data[l1]=0;
    strncpy(p->data+l1+1,s2,l2);
    SUBPROC((void *)SendAnswer,0,p);
    tracktimer=0;
    GetTags();
  }
}


void set_my_xstatus2(void)
{
  TPKT *p;


  int l1;
  int l2;
  p=malloc(sizeof(PKT)+1);
  p->pkt.uin=0;               
  p->pkt.type=22; 
  p->pkt.data_len=1;          
  p->data[0]=11;
  SUBPROC((void *)SendAnswer,0,p);

  char *sk2=malloc(256);


  strcpy(sk2,teast2);
  l1=strlen(playing);
  l2=strlen(sk2);
  if ((!playing)||(!sk2)) return;
  p=malloc(sizeof(PKT)+l1+l2+1);
  p->pkt.uin=0;
  p->pkt.type=26;
  p->pkt.data_len=l1+l2+1;
  strncpy(p->data+0,playing,l1);
  p->data[l1]=0;
  strncpy(p->data+l1+1,sk2,l2);
  mfree(sk2);
  SUBPROC((void *)SendAnswer,0,p);


}
void to_develop(void)
{
  if (silenthide) return;
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)maincsm_id;
  GBS_SendMessage(0x4209,0xDEB0,2,&gipc);  
}

void ReqAddMsgToChat(CLIST *t)
{
  if (edchat_id)
  {
    void *data=FindGUIbyId(edchat_id,0);
    {
      EDCHAT_STRUCT *ed_struct;
      ed_struct=EDIT_GetUserPointer(data);
      if (ed_struct) 
      {
        if (ed_struct->ed_contact==t)
        {
          if (EDIT_IsBusy(data))
          {
            t->req_add=1;
            time_to_stop_t9=3;
          }
          else
          {
            AddMsgToChat(data);
            DirectRedrawGUI_ID(edchat_id);
	  }
	}
      }
    }
  }
}

GBSTMR develop;

void begin_to_develop()
{
  if(IsUnlocked())
    to_develop();
  else
    GBS_StartTimerProc(&develop, 100, begin_to_develop);
}





char *cmd_chars; 
char **cmd_lines; 

void cmdFree(void)
{
  if (cmd_lines) mfree(cmd_lines);
  if (cmd_chars) mfree(cmd_chars);
  cmd_lines=0;
  cmd_chars=0;
}

int LoadCmd()
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  cmdFree();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\cmd.txt");
  GetFileStats(fn,&stat,&ul);
  fsize=stat.size;
  f=fopen(fn,0+0x8000,0x80,&ul);
  if (f==-1) return 0;
  p=cmd_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	cmd_lines=realloc(cmd_lines,(i+1)*sizeof(char *));
	cmd_lines[i++]=pp;
      }
      pp=0;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==0) pp=p;
    }
    p++;
  }
  return i;
}

int NumOfItems=0;
HEADER_DESC CmdMenuHeader={0,0,0,0,0,0,0x7FFFFFFF};

void CmdMenuItemHandler(void *data, int curitem, void *unk)
{
  void *item=AllocMenuItem(data);
  char *p=cmd_lines[curitem];
  WSHDR *ws=AllocMenuWS(data,strlen(p));
  ascii2ws(ws,p);
  SetMenuItemText(data, item, ws, curitem);
}

void CmdMenuGHook(void *data, int cmd)
{
  if (cmd==10)
  {
    DisableIDLETMR();
  }
  if (cmd==3)
  {
    cmdFree();
  }
}

static int CmdMenuKeyHook(void *data, GUI_MSG *msg)
{
  int Selected=GetCurMenuItem(data);
  EDCHAT_STRUCT *ed_struct=MenuGetUserPointer(data);
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    if (NumOfItems)
    {
      const char *p=cmd_lines[Selected];
      WSHDR *ws;
      ws=AllocWS(256);
      wsprintf(ws,p);
      EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ws);
      EDIT_SetCursorPos(ed_struct->ed_chatgui,wstrlen(ws)+1);
      
      FreeWS(ws);
      return(1);
    }
  }
  return(0);
}

const int CmdSoftkeys[]={0,1,2};

const SOFTKEY_DESC cmd_sk[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)"Закрыть"},
  {0x003D,0x0000,(int)0x7FFFC0FB}
};
const SOFTKEYSTAB cmd_Back_skt=
{
 cmd_sk,0
};
static const MENU_DESC CmdMenuStruct=
{
  8,CmdMenuKeyHook,CmdMenuGHook,0,
  CmdSoftkeys,
  &cmd_Back_skt,
  0x10,
  CmdMenuItemHandler,
  0, 
  0, 
  0
};

int DispCmdMenu(void *ed_gui)
{
  NumOfItems=LoadCmd();
  if(NumOfItems)
  {
    *((int *)(&CmdMenuHeader.lgp_id))=(int)"Select command";
    patch_header(&CmdMenuHeader);
    return CreateMenu(0,0,&CmdMenuStruct,&CmdMenuHeader,0,NumOfItems,ed_gui,0);
  }
  MsgBoxError(1, (int)"FILE NOT FOUND!");
  return(0);
}























 

char *GetStatusByIconIndex(int icon)
{
  char *stat=malloc(32);  
  switch(icon)
   {
    case IS_OFFLINE :    sprintf(stat,"Отключен");     break;
    case IS_INVISIBLE :  sprintf(stat,"Невидимый");    break;
    case IS_AWAY :       sprintf(stat,"Отсутсвует");   break;
    case IS_NA :         sprintf(stat,"Не доступен");  break;
    case IS_OCCUPIED:    sprintf(stat,"Занят");        break;
    case IS_DND :        sprintf(stat,"Не беспокоить");break;
    case IS_DEPRESSION : sprintf(stat,"Депрессия");    break;
    case IS_EVIL :       sprintf(stat,"Злой");         break;
    case IS_HOME :       sprintf(stat,"Дома");         break;
    case IS_LUNCH :      sprintf(stat,"Жру");          break;
    case IS_WORK :       sprintf(stat,"Работа");       break;
    case IS_ONLINE :     sprintf(stat,"В сети");       break;
    case IS_FFC :        sprintf(stat,"Готов болтать");break;
    default :            sprintf(stat,"В сети");       break;
   }
   return stat;
}



int comanda;

char requests1[]="Sending online time...";
char requests2[]="Sending msg info...";
char requests3[]="Sending battery info...";
char requests4[]="Sending mod info...";
char requests6[]="Sending offline info...";
char mod_info[264];
         



void SendData(CLIST *t,char *s,int signal,int count)
{
  comanda=0;    
  t->signals=signal;
  TPKT *p;
  SENDMSGCOUNT+=count;
  p=malloc(sizeof(PKT)+strlen(s)+1);
  p->pkt.uin=t->uin;
  p->pkt.type=2;
  p->pkt.data_len=strlen(s);
  strcpy(p->data,s);  
  SUBPROC((void *)SendAnswer,0,p);  
}

unsigned int *errornumber;

void CheckComand(char *txt, CLIST *t)
{
  comanda=1;
  sprintf(mod_info,"mod by Evilfox,kluchnik,windes based on revision 2058, mod date: %s at %s ","Aug  6 2008","20:09:04");

  t->signals=1;
  if(!strcmp(txt, "__TEXTINFO") || !strcmp(txt, "__TI"))
   {
     char *s=malloc(16384);
     char pl[256];
     *s=0;
     snprintf(pl,256,"%s model: %s sw: %s\r\n","Platform: ЁLKA",Get_Phone_Info(9),Get_Phone_Info(7));
     strcat(s,pl);
     char online[256];
     sprintf(online,"Время в онлайн: %02d:%02d:%02d \r\n"
                    "Сервер: %s \r\n%s \r\n", uptimeHour, uptimeMin, uptimeSec, hostbuf2,PINGO);
     strcat(s,online);
     char *send_txt=malloc(64);
     sprintf(send_txt, "Отправлено сообщений: %d \r\n"
                       "Принято сообщений: %d \r\n", main2_count, main_count);
     strcat(s,send_txt);
    
     WSHDR *ws=AllocWS(64);
     GetProvider(ws,0);
     char prov[16];
     ws_2str(ws,prov,wstrlen(ws));
     sprintf(send_txt, "Оператор: %s \r\n", prov);
     FreeWS(ws);
     strcat(s,send_txt);
    
     int bat;
     bat=*RamCap();
     sprintf(send_txt, "Заряд батареи: %d процентов \r\n", bat);
     strcat(s,send_txt);
      
     char *status=malloc(256);
     TTime the;
     GetDateTime(0,&the);
     void Get_Online_total();
     Get_Online_total();
     unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;	
     sprintf(status, "Traffic: Rx : %db Tx : %db Queue: %db \r\nTime: %02d:%02d\r\n"
                     "Online contact: %d [total: %d] \r\nUnread message: %d/%d ",
                                                          RX,TX,sendq_l,
                                                          the.hour, the.min,
                                                          kopa_kontaktu, kopa_online,
                                                          total_unread,total_unread2);
     strcat(s,status);
     comanda=0;
     if(show_send_info)
       AddStringToLog(t,0x01,"Sending textinfo...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
     else
       SENDMSGCOUNT++;
     SendData(t,s,1,0);
     mfree(s);
     mfree(send_txt);
     mfree(status);
     return;
   }
  
  if(!strcmp(txt, "__TIME") || !strcmp(txt, "__T") || !strcmp(txt, "GET_TIME"))
   {
     char *status=malloc(256);
     TTime the;
     GetDateTime(0,&the);
     sprintf(status, "Time: %02d:%02d",the.hour, the.min);
     comanda=0;
     if(show_send_info)
       AddStringToLog(t,0x01,"Sending time...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
     else
       SENDMSGCOUNT++;
     SendData(t,status,1,0);
     mfree(status);
     return;
   }
  
  if(!strcmp(txt, "GET_PLATFORM_INFO") || !strcmp(txt, "__PLATFORM") || !strcmp(txt,"__HW"))
  {
    comanda=0;
    TPKT *p;
    char pl[256];
    snprintf(pl,256,"%s model: %s sw: %s","Platform: ЁLKA",Get_Phone_Info(9),Get_Phone_Info(7));
    p=malloc(sizeof(PKT)+strlen(pl)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(pl);
    strcpy(p->data,pl);
    if(show_send_info)
      AddStringToLog(t,0x01,"Sending platform info...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
  
  if(!strcmp(txt, "GET_ONLINE_TIME") || !strcmp(txt, "__ONLINE"))
  {
    comanda=0;
    TPKT *p;
    char online[256];
    sprintf(online,"Время в онлайн: %02d:%02d:%02d \n"
                   "Сервер: %s", uptimeHour, uptimeMin, uptimeSec, hostbuf2);
    p=malloc(sizeof(PKT)+strlen(online)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(online);
    strcpy(p->data,online);
    if(show_send_info)
      AddStringToLog(t,0x01,requests1,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
  
  if(!strcmp(txt, "GET_MSG_INFO") || !strcmp(txt, "__MSG"))
  {
    comanda=0;
    TPKT *p;
    char *send_txt=malloc(64);
    sprintf(send_txt, "Отправлено сообщений: %d \n"
                      "Принято сообщений: %d"
                    , main2_count, main_count);
    p=malloc(sizeof(PKT)+strlen(send_txt)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
      AddStringToLog(t,0x01,requests2,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }



  if(!strcmp(txt, "GET_PROVIDER") || !strcmp(txt, "__PROVIDER"))
  {
    comanda=0;
    TPKT *p;
    
    WSHDR *ws=AllocWS(32);
    GetProvider(ws,0);
    char prov[16];
    ws_2str(ws,prov,wstrlen(ws));
    char *send_txt=malloc(32);
    sprintf(send_txt, "Оператор: %s", prov);
    FreeWS(ws);
    
    p=malloc(sizeof(PKT)+28+1);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
      AddStringToLog(t,0x01,"Sending provider name...",I_str,(++SENDMSGCOUNT)&0x7FFF,1); 
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }

  

  
  
  if(!strcmp(txt, "GET_MOD_INFO") || !strcmp(txt, "__MOD"))
  {
    comanda=0;
    TPKT *p;
    char *send_txt=malloc(256);

    strcpy(I_UIN, t->name);
    if(strstr(ignor_list,I_UIN)==0)
    {
      strcpy(send_txt, mod_info);
    }
    else
      strcpy(send_txt, "***Access denied***");

    strcpy(send_txt, mod_info);

    p=malloc(sizeof(PKT)+strlen(send_txt)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
       AddStringToLog(t,0x01,requests4,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
       SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }
  
  if(!strcmp(txt, "GET_BAT_INFO") || !strcmp(txt, "__BAT"))
  {
    comanda=0;
    TPKT *p;
    int bat;
    bat=*RamCap();
    char *send_txt=malloc(64);
    sprintf(send_txt, "Заряд батареи: %d процентов", bat);
    p=malloc(sizeof(PKT)+28+1);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
       AddStringToLog(t,0x01,requests3,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
       SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }
  
  if((away_m==1 || t->isautomsg) && (t->automsgcount < automsgcount))
  {
    t->automsgcount++;
    comanda=1;
    TPKT *p;

    sprintf(away_msg, "Автоответчик: %s %s! %s %s", awayHelloMsg, t->name, awayMsg, away_msg_time);
    p=malloc(sizeof(PKT)+strlen(away_msg)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(away_msg);
    strcpy(p->data,away_msg);
    if(show_send_info)
      AddStringToLog(t,0x01,requests6,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
  else
    if(away_m==1 || t->isautomsg)
    {
      t->automsgcount=automsgcount;
      return;
    }
  
  

  
  
  if(!strcmp(txt, "GET_URL_INFO") || !strcmp(txt, "__URL"))
  {
    comanda=0;
    TPKT *p;
    
    char *send_txt=malloc(256);
    sprintf(send_txt,"revision 2058, mod date: %s at %s ",
                     "http://kluchnik.jino-net.ru/naticqmod/","Aug  6 2008","20:09:04");
    
    p=malloc(sizeof(PKT)+256);
    p->pkt.uin=t->uin;
    p->pkt.type=2;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
      AddStringToLog(t,0x01,"Sending url info...",I_str,(++SENDMSGCOUNT)&0x7FFF,1); 
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }
  
 
 CheckComand2(txt,t);
}



char INC_Nick[32];
char show_nick[32];
int INC_show;

char onContact[64];
int INC_show2;
GBSTMR incs2;
extern void DrawScreenShot();
     
void show_on_contact()
{
  if(INC_show2>0)
  {
    WSHDR *ws=AllocWS(64);
    ascii2ws(ws,onContact);
    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if(!(memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))
    {
     DrawScreenShot();
     INC_show2-=2;
    }
    else
      INC_show2--;
    DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+2,GetFontYSIZE(PopupFont)+2,0,PopupBGCl,PopupBGCl);
    DrawString(ws,1,1,1+Get_WS_width(ws,PopupFont)+2,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupOnlineCl,GetPaletteAdrByColorIndex(23));
    FreeWS(ws);
    GBS_StartTimerProc(&incs2, 15, show_on_contact);
  }
  else
    RefreshGUI();
}





void show_pp_sender()
{
  if(INC_show>0)
  {
    WSHDR *ws=AllocWS(256);
    ascii2ws(ws,show_nick);
    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if(!(memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))
    {
     DrawScreenShot();
     INC_show-=2;
    }
    else
      INC_show--;
    DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+4+GetImgWidth(NumStatusToIconBar + IS_MSG),GetFontYSIZE(PopupFont)+4,0,PopupBGCl,PopupBGCl);
    DrawImg(2,2,NumStatusToIconBar + IS_MSG);
    DrawString(ws,GetImgWidth(NumStatusToIconBar + IS_MSG)+4,1,GetImgWidth(NumStatusToIconBar + IS_MSG)+Get_WS_width(ws,PopupFont)+4,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupMsgCl,GetPaletteAdrByColorIndex(23));
    FreeWS(ws);
    GBS_StartTimerProc(&develop, 10, show_pp_sender);
  }
}





void Get_Online_total()
{
  CLIST *t;
  kopa_online=0;
  kopa_kontaktu=0;
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (!t->isgroup)
    {
      kopa_online++;
      if (t->state!=65535) kopa_kontaktu++;
    }
  }
}





void SetIndividStatus()
{
  CLIST *t;
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (!t->isgroup)
    {
      t->isvibra=1;
      t->issound=1;
      t->isShowXst=Show_XTX;
      t->signals=1;
      t->isaccess=1;
      
      t->automsgcount=0;
    }
  }
}




ProcessPacket(TPKT *p)
{
  CLIST *t;
  LOGQ *q;
  char s[256];
  switch(p->pkt.type)
  {
  case 4:
    set_my_status();
    set_my_xstatus();
    break;
  case 7:
    if (p->pkt.uin)
    {
      if ((t=FindContactByUin(p->pkt.uin)))
      {
	
        if (!t->local) strncpy(t->name,p->data,63);
	t->group=GROUP_CACHE;
	ChangeContactPos(t);
	RecountMenu(t,1);
      }
      else
      {
        RecountMenu(AddContact(p->pkt.uin,p->data, GROUP_CACHE, 0),1);
      }
    }
    else
    {
      if(VIBR_ON_CONNECT)
      {
        vibra_count=1;
        start_vibra();
      }
      GROUP_CACHE=0;
      ask_my_info();
      if (maingui_id )
      {
	RecountMenu(0,1);
      }
      else
      {
      

        SetIndividStatus();
        ShowLoadScreen=0;
        SMART_REDRAW();  
      }
    }
    break;
  case 20:
    if (t=FindGroupByID(GROUP_CACHE=p->pkt.uin))
    {
      strncpy(t->name,p->data,63);
      ChangeContactPos(t);
      RecountMenu(t,1);
    }
    else
    {
      RecountMenu(AddGroup(p->pkt.uin,p->data),1);
    }
    break;
  case 21:
    GROUP_CACHE=p->pkt.uin;
    break;
  case 9:
    t=FindContactByUin(p->pkt.uin);
    if (t)
    {
      int i=t->state;
      CLIST *oldt=0;
      if (t->xstate!=p->data[2])  
      {
        t->xstate=p->data[2];
        FreeXText(t);
        if (t->xstate && ENA_AUTO_XTXT)   
        {
          t->req_xtext=1;
          if (edchat_id)   
          {
            void *data=FindGUIbyId(edchat_id,0);
            if (data)
            {
              EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
              if (ed_struct)
              {
                if (ed_struct->ed_contact==t)   
                {
                  t->req_xtext=0;
                  RequestXText(t->uin);
                }
              }
	    }
	  }
	}
        else  
        {
          t->req_xtext=0;
        }
      }
      if (contactlist_menu_id)
      {
	oldt=FindContactByN(GetCurMenuItem(FindGUIbyId(contactlist_menu_id,0)));
      }
      if(t->state==65535) t->change=1;
      t->state=*((unsigned short *)(p->data));
      if((t->state!=65535)&&(t->change==1))
      {
        t->just_now=2;
        t->change=0;
      }
      LogStatusChange(t);
      ChangeContactPos(t);
      RecountMenu(oldt,1);
     
      if(isLogStatusChange)
      {
       char *s=GetStatusByIconIndex(GetIconIndex(t));         
       FreeXText2(t);
       TTime time;
       GetDateTime(0,&time);
       char *txt=malloc(64);
       sprintf(txt," %s: %02d:%02d:%02d",s,time.hour,time.min,time.sec);      
       t->xtext2=malloc(64);
       snprintf(t->xtext2,64,txt);
       AddStringToLog(t,0x03,txt,"Status change",0xFFFFFFFF, 0);
       ReqAddMsgToChat(t);  
       RecountMenu(0, 0);
       mfree(txt);
      }
     
      if ((t->state!=0xFFFF)&&(i==0xFFFF))
      {
       if(Is_INC_On && (PopupShow==2 || PopupShow==3))
        {
          INC_show2=20;
          sprintf(onContact,PopupOnlineString,t->name);
          sprintf(on_off_buf,"[%s: %s]\n",t->xtext2+1,t->name);
          on_off_timer=ON_OFF_TIME;
          UpdateCLheader();
          show_on_contact();
        }
        else
          GBS_DelTimer(&incs2);
        if(!extended_sounds)
        {
          Play(sndGlobal);
        }
        else
        {
          FSTATS stat;
          int fsize;
          unsigned int ul = 0;
          uins=t->uin;
          sprintf(talk_sound, "%s%d.wav", sndGlobal2, uins);
          GetFileStats(talk_sound,&stat,&ul);
          fsize=stat.size;
          if(fsize>0)
            Play(talk_sound);
          else
            Play(sndGlobal);       
          t->isvibra=1;
          t->issound=1;
          t->isShowXst=Show_XTX;
          t->signals=1;
        }
      }
      
      if ((i!=0xFFFF)&&(t->state==0xFFFF))
      {
	Play(sndSrvMsg);        
        if(TIME_OR_MSG==2 || TIME_OR_MSG==3 | TIME_OR_MSG==4)
          Get_Online_total();
        sprintf(on_off_buf,"[%s: %s]\n",t->xtext2+1,t->name);
        on_off_timer=ON_OFF_TIME;
        UpdateCLheader();
      }
      
    if (edchat_id)
    {
      void *data=FindGUIbyId(edchat_id,0);
      {
	EDCHAT_STRUCT *ed_struct;
	ed_struct=EDIT_GetUserPointer(data);
	if (ed_struct) 
	{
	  if (ed_struct->ed_contact==t)
	  {
	    DirectRedrawGUI_ID(edchat_id);
	  }
	}
      }
    }
    }
    break;
  case 3:
    NEW_MSG=1;
    t=FindContactByUin(p->pkt.uin);
    if (!t)
    {
      sprintf(s,percent_d,p->pkt.uin);
      t=AddContact(p->pkt.uin,s,GROUP_CACHE,0);
    }
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    if (t->name[0]=='#')
    {
      
      char *s=strchr(p->data,'>');
      
      if (s)
      {
	if ((s[1]==' ')&&((s-p->data)<16))
	{
	  *s=0; 
	  AddStringToLog(t,0x02,s+2,p->data,0xFFFFFFFF,1); 
	  goto L1;
	}
      }
    }
    


    comanda=1;
    CheckComand(p->data, t);
    if(t->signals && t->isaccess)
    {
      scrsTime=0;
      if(SHOW_ACTIVE)
        t->isactive=360;
      if(t->isvibra)
       {
        if(VIBR_TYPE)
           vibra_count=2;
        else
            vibra_count=1;
        if(t->automsgcount<automsgcount)
            start_vibra();
       }
        if(SLI_TYPE)
           SLI_Timer();
         if(Is_Double_Vibra_On)
         {
           Reminder_timer();
         }
         
         if (Is_ILU_On)
           IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); 
    }
    else
      if(t->isIndividSet)
      {
        scrsTime=0;
        if(SHOW_ACTIVE)
          t->isactive=360;
        if(t->isvibra)
         {
          if(VIBR_TYPE)
             vibra_count=2;
          else
              vibra_count=1;
          if(t->automsgcount < automsgcount && away_m)
             start_vibra();
          else
             start_vibra();
         }
          if(SLI_TYPE)
             SLI_Timer();
           if(Is_Double_Vibra_On)
           {
             Reminder_timer();
           }
           
           if (Is_ILU_On)
             IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); 
      }
    else
      {
        scrsTime=0;
        if(SHOW_ACTIVE)
          t->isactive=360;
        if(VIBR_TYPE)
           vibra_count=2;
        else
           vibra_count=1;
        if(t->automsgcount >= automsgcount) vibra_count=0;
           start_vibra();
        if(SLI_TYPE)
            SLI_Timer();
        if(Is_Double_Vibra_On)
          {
            Reminder_timer();
          }
           
        if (Is_ILU_On)
             IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); 
        }
    
    if(comanda==1)
    {
      AddStringToLog(t,0x02,p->data,t->name,0xFFFFFFFF,1);
      main_count++;
      if(Is_INC_On && (PopupShow==1 || PopupShow==3))
      {
        sprintf(show_nick,PopupMsgString,t->name);
        if(!IsIdleUiOnTop()&&(!saver_on)) 
        {
          INC_show=20;
          show_pp_sender();
        }
      }
      else
        GBS_DelTimer(&develop);
      
      main_msg_timer2=main_msg_timer;
      if(Is_SMARTCPU_On)
        AddToMain(t->name,p->data);
    }
    L1:
    
    {
      CLIST *g=FindGroupByID(t->group);
      if (g)
      {
	if (g->state)
	{
	  g->state=0;
	}
      }
    }

    ReqAddMsgToChat(t);
    RecountMenu(t,1);
   
    switch (DEVELOP_IF)
    {
    case 0:
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)) to_develop();
      break;
    case 1:
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)&&(IsUnlocked())) to_develop();
      break;
    case 2:
      break;
    case 3:
      GBS_DelTimer(&develop);
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)&&(!IsUnlocked())) begin_to_develop();
      break;
    }
    break;
  case 16:
  case 17:
    q=FindContactLOGQByAck(p);
    if (q)
    {
      q->acked=p->pkt.type==16?1:2;
      t=FindContactByUin(p->pkt.uin);
      if (edchat_id)
      {
	void *data=FindGUIbyId(edchat_id,0);
	if (data)
	{
	  EDCHAT_STRUCT *ed_struct;
	  ed_struct=EDIT_GetUserPointer(data);
	  if (ed_struct) 
	  {
	    if (ed_struct->ed_contact==t)
	    {
	      if (EDIT_IsBusy(data))
	      {
		t->req_drawack=1;
                time_to_stop_t9=3;
	      }
	      else {
		DrawAck(data);
                if (p->pkt.type==16)
                 IlluminationOn(ILL_DISP_SEND,ILL_KEYS_SEND,ILL_SEND_TMR,ILL_RECV_FADE); 
              }
	    }
	  }
	}
      }
    }
    break;
  case 25:
    t=FindContactByUin(p->pkt.uin);
    if (t)
    {
      int i;
      int j;
      FreeXText(t);
      i=p->pkt.data_len;
      memcpy(t->xtext=malloc(i),p->data,i);
      zeromem(s,256);
      strcpy(s,t->name);
      strcat(s,":\n");
      i=strlen(s);
      j=p->data[0];
      if (j>(255-i)) j=255-i;
      strncpy(s+i,p->data+1,j);
      i+=j;
      if (i<255)
      {
	s[i]='\n';
	i++;
	j=p->pkt.data_len-p->data[0]-1;
	if (j>(255-i)) j=255-i;
	strncpy(s+i,p->data+p->data[0]+1,j);
      }

      zeromem(s,256);
      i=0;
      j=p->data[0];
      if (j>(255-i)) j=255-i;
      strncpy(s,p->data+1,j);
      i+=j;
      if (i<255)
      {
	s[i++]=13;
	j=p->pkt.data_len-p->data[0]-1;
	if (j>(255-i)) j=255-i;
	strncpy(s+i,p->data+p->data[0]+1,j);
      }
      AddStringToLog(t,0x03,s,x_status_change,0xFFFFFFFF,1);
      ReqAddMsgToChat(t);
      if (strlen(p->data))       
         RecountMenu(t, 1);
      else
         RecountMenu(0, 0);     
    }
    break;
  case 31:
    CurrentPrivateStatus=p->data[0];
    break;
  }
  mfree(p);
}


IPC_REQ tmr_gipc;
void process_active_timer(void)
{
  if (connect_state>2)
  {
    if (++tenseconds_to_ping>12)
    {
      tenseconds_to_ping=0;
      SUBPROC((void *)do_ping);
    }
  }
  tmr_gipc.name_to=ipc_my_name;
  tmr_gipc.name_from=ipc_my_name;
  tmr_gipc.data=0;
  GBS_SendMessage(0x4209,0xDEB0,4,&tmr_gipc);  
  GBS_StartTimerProc(&tmr_active,216*10,process_active_timer);
}

GBSTMR tm22;
void OneSecTmr()
{
  tmr_gipc.name_to=ipc_my_name;
  tmr_gipc.name_from=ipc_my_name;
  tmr_gipc.data=0;
  GBS_SendMessage(0x4209,0xDEB0,6,&tmr_gipc);  
  GBS_StartTimerProc(&tm22,216,OneSecTmr);
}
  





























































































































 

void DrawLoadScreen(MAIN_GUI *data)
{
  int scr_w=ScreenW();
  int scr_h=ScreenH();
  int pos_status;
  int pm = pictures_max, pl = pictures_loaded;
  RECT rc=PBRECT;
  DrawRectangle(0,24,scr_w-1,scr_h-1,0, GetPaletteAdrByColorIndex(1),  GetPaletteAdrByColorIndex(1));


  
    DrawImg(0,0,S_ICONS[ICON_LOGO]);
    unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;			
    if(IsShowInfo)
    {
      wsprintf(data->ws1,"State: %d, RXstate: %d\nRx:%db,Tx:%db\nQueue: %db\n%s\n%s",connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);
      if(pm != pl)
      {
  

 
        
         

 
    
        
        DrawRectangle(rc.x,rc.y,rc.x2,rc.y2,0,BGPBcolor,BGPBcolor);
        pos_status = (((rc.x2-rc.x)-1) * pl) / pm;
        DrawRectangle(rc.x+2,rc.y+2,rc.x+pos_status+2,rc.y2-2,0,PBcolor,PBcolor);
       
        if(total_smiles)
        {
          wstrcatprintf(data->ws1,"\nLoaded %d smiles",total_smiles);
        }
        if (xstatuses_load)
        {
          wstrcatprintf(data->ws1,"\nLoaded %d xstatus",total_xstatuses);
        }
      }  
    }
    else
    {
      wsprintf(data->ws1,"State: %d, RXstate: %d",connect_state,RXstate);     
    }
  
    DrawString(data->ws1,3,3+24,scr_w-4,scr_h-4-GetFontYSIZE(5),8,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));   








 
    
  
  if(IsInvertSoftKey)
    {
     wsprintf(data->ws2,percent_t,"Exit");
     DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(5),scr_w-4,scr_h-4,5,4,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  
     wsprintf(data->ws2,percent_t,cltop?"CList":empty_str);
     DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(5), scr_w>>1,scr_h-4,5,1,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
    }
    else
    {
     wsprintf(data->ws2,percent_t,cltop?"CList":empty_str);
     DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(5),scr_w-4,scr_h-4,5,4,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  
     wsprintf(data->ws2,percent_t,"Exit");
     DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(5), scr_w>>1,scr_h-4,5,1,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
    }
  


 
}


void method0(MAIN_GUI *data)
{
  if(IsGuiOnTop(maingui_id))
  {
    if(ShowLoadScreen)    
    {
     DisableIconBar(0);
      DrawLoadScreen(data);
    }
    else
    {
     DisableIconBar(1);
      SetCanvasFlag(1);
      DrawContactList();    
    }
  }
}

void method1(MAIN_GUI *data,void *(*malloc_adr)(int))
{



 
  data->ws1=AllocWS(256);
  data->ws2=AllocWS(256);
  StatusToIMGHDR();
  data->gui.state=1;
}

void method2(MAIN_GUI *data,void (*mfree_adr)(void *))
{


 
  FreeStatusToIMGHDR();
  FreeWS(data->ws1);
  FreeWS(data->ws2);
  data->gui.state=0;
}

void method3(MAIN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{



 

  DisableIDLETMR();
  data->gui.state=2;
}

void method4(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  if (data->gui.state!=2)
    return;


 
  GBS_DelTimer(&cltmr);
  data->gui.state=1;
}


int LoadOnKey(GUI_MSG *msg)
{
 if (msg->gbsmsg->msg==0x193)
  {
    switch(msg->gbsmsg->submess)
    {      

      case 0x01:
        if(IsInvertSoftKey)
        {
           if(ShowLoadScreen && (cltop || sock==-1)) ShowLoadScreen=0;
         
         
        }
        else
        {
         if(ShowLoadScreen)
            return(1); 
         else
            ShowLoadScreen=1;
        }
      
      break;
    case 0x04:
      if(IsInvertSoftKey)
      {
        if(ShowLoadScreen)
          return(1); 
        else
          ShowLoadScreen=1;
      }
      else
       {
         if(ShowLoadScreen && (cltop || sock==-1)) ShowLoadScreen=0;


       }
      break;
    case 0x0B:
      disautorecconect=0;
      if ((connect_state==0)&&(sock==-1))
      {
        GBS_DelTimer(&reconnect_tmr);
	DNR_TRIES=3;
        SUBPROC((void *)create_connect);
      }
      break;
    case 0x0C:
       
        disautorecconect=1;
     
        SUBPROC((void*)end_socket);
      break;
    case '0':
      SUBPROC((void*)end_socket);
      GBS_DelTimer(&reconnect_tmr);
      DNR_TRIES=3;
      SUBPROC((void *)create_connect);
      break;
      

      
    case '1':
        Is_Sounds_Enabled=!(Is_Sounds_Enabled);
        SUBPROC((void*)WriteDefSettings);
        break;
    case '2':
        Is_Vibra_Enabled=!(Is_Vibra_Enabled);
        SUBPROC((void*)WriteDefSettings);
        break;
    case '3':        
         if(CurrentPrivateStatus!=1)
           SetPrivateStatus(1);
         else
           SetPrivateStatus(3);
         break;
    case '4':
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        break;
      case '5': LINE_MODE=!LINE_MODE; break;
      case '6': VIEW_MODE=!VIEW_MODE; break;
    }
  }
  return 0;
}


int method5(MAIN_GUI *data,GUI_MSG *msg)
{
  DirectRedrawGUI();
  if(ShowLoadScreen)
   return LoadOnKey(msg);
  else
   ClOnKey(msg);
  







 
    
  return(0);
}

int method8(void){return(0);}
int method9(void){return(0);}



const void * const gui_methods[11]={
  (void *)method0,  
  (void *)method1,  
  (void *)method2,  
  (void *)method3,  
  (void *)method4,  
  (void *)method5,  
  0,
  (void *)kill_data, 
  (void *)method8,
  (void *)method9,
  0
};



int checkGui(int id);

int my_keyhook(int key, int m)
{
  if(m==0x193)
   {
     if ((key==0x14)&&(!IsUnlocked()))
     {
       GBS_StopTimer(&away_tmr);
       msg_away_timer=AUTO_ENGADE*12;
       away_timer();
       main_d=1;
     }
     else
     {
     if(Reset_timer)
     {
       msg_away_timer=0;
       away_m=0;
       if(change_back_status)
       {
         CurrentStatus=status_rem;
         set_my_status();
         change_back_status=0;
       }
     }
      }
  }
  
  void *icsm;
  if(m==0x195)
   {
     icsm=FindCSMbyID(CSM_root()->idle_id);
     if(key==0x0E && IsUnlocked() && !(checkGui((((int *)icsm)[0x2C/4])))   )
     {
        to_develop();
     }
  }
  return 0;
}


























































 
const RECT Canvas={0,0,0,0};

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);

  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  maingui_id=csm->gui_id=CreateGUI(main_gui);
  ews=AllocWS(16384);
  
  DNR_TRIES=3;
  
  
  wsBuffer=AllocWS(4096);
  OneSecTmr();





 

  GBS_StartTimerProc(&tmr_active,216*10,process_active_timer);
  
 
 
    sprintf((char *)ipc_my_name+6,percent_d,UIN);
    gipc.name_to=ipc_my_name;
    gipc.name_from=ipc_my_name;
    gipc.data=(void *)-1;
    GBS_SendMessage(0x4209,0xDEB0,1,&gipc);
  









 
}

void maincsm_onclose(CSM_RAM *csm)
{
  WriteDefSettings(); 
  FreeWS(wsBuffer);
  
  RemoveKeybMsgHook((void *)my_keyhook);
  GBS_DelTimer(&tmr_active);
  GBS_DelTimer(&tmr_vibra);
  GBS_DelTimer(&reconnect_tmr);
  GBS_DelTimer(&tmr_illumination);
  GBS_DelTimer(&tm22);
  GBS_DelTimer(&sli_tmr);
  GBS_DelTimer(&remind_tmr);
  GBS_DelTimer(&ping_tmr);
  GBS_DelTimer(&uptime_tmr);
  GBS_DelTimer(&autoChangeStatus);
  GBS_DelTimer(&tracktmr);
  GBS_DelTimer(&scrsTmr);
  GBS_DelTimer(&incs2);
  GBS_DelTimer(&develop);
  GBS_DelTimer(&away_tmr);
  SetVibration(0);
  FreeTemplates();
  cmdFree();
  FreeCLIST();
  free_ICONS();

  FreeWS(ews);
  FreeXStatusText();
  PrintLibFree();
  SUBPROC((void *)FreeSmiles);
  SUBPROC((void *)FreeXStatusesImg);
  SUBPROC((void *)end_socket);
  SUBPROC((void *)ClearSendQ);
  SUBPROC((void *)ElfKiller);
}

void do_reconnect(void)
{
  if (is_gprs_online)
  {
    DNR_TRIES=3;
    SUBPROC((void*)create_connect);
  }
}

void CheckDoubleRun(void)
{
  int csm_id;
  if ((csm_id=(int)(gipc.data))!=-1)
  {
    gipc.name_to=ipc_xtask_name;
    gipc.name_from=ipc_my_name;
    gipc.data=(void *)csm_id;
    GBS_SendMessage(0x4209,0xDEB0,2,&gipc);  
    LockSched();
    CloseCSM(maincsm_id);
    
    UnlockSched();
  }
  else

    {
      InitXStatusesImg();
      
      create_connect();
    }
  



 
    
}





int checkGui(int id)
{
 int retval = 0; 
 if (IsGuiOnTop(id)) 
 {
    GUI* igui = GetTopGUI();
    if (igui)
    {
      canvasdata = BuildCanvas();
      retval = 1;
    }  
 }
 return retval;
}




typedef struct{
  unsigned long uin;
  char *msg;
}IPCmsg;

int maincsm_onmessage(CSM_RAM *data,GBS_MSG *msg)
{

  
  MAIN_CSM *csm=(MAIN_CSM*)data;
  {
    
    if (msg->msg==0xDEB0)
    {
      IPC_REQ *ipc;
      if ((ipc=(IPC_REQ*)msg->data0))
      {
	if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
	{
	  switch (msg->submess)
	  {
	  case 1:
	    
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)CheckDoubleRun);
            else ipc->data=(void *)maincsm_id;
	    break;
	  case 2:
	    
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)ProcessNextSmile);
	    if(ShowLoadScreen) SMART_REDRAW();
	    break;
	  case 3:
	    
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)ProcessNextXStatImg);
	    if(ShowLoadScreen) SMART_REDRAW();
	    break;
          case 6:
            if (ipc->name_from==ipc_my_name)	    
            {
              
                if(on_off_timer>0)
                {
                   on_off_timer--;
                   SMART_REDRAW();
                   break;
                }
                else
                  if(on_off_timer!=-1)
                  {
                    on_off_timer=-1;
                    zeromem(on_off_buf,128);
                    SMART_REDRAW();
                
                  }
               int f=0;
               CLIST *t=(CLIST*)cltop;
               while(t && !f)
               {
                  if(t->just_now) f=1;
                  t=t->next;
               }
              if(f) MyRedraw();
            }
            break;
	  case 4:
	    
	    if (ipc->name_from==ipc_my_name)
	    {
	      CLIST *t=(CLIST *)cltop;
	      int f=0;
              if(main_msg_timer2!=0) main_msg_timer2--; else main_msg_timer2=0;
	      while(t)
	      {
                if(t->just_now)
                {
                  if (!(--(t->just_now))) f=1;
                }
		if (t->isactive)
		{
		  if (!(--(t->isactive))) 
                  {
                    f=1; 
                    if ((t->log)&&(!t->isunread))
                    {
                        FreeLOGQ(&t->log);
                        t->msg_count=0;
                    }
                  }
		}
		t=(CLIST *)(t->next);
	      }
	      if (f) 
	      {
		CLIST *oldt=0;
		if (contactlist_menu_id)
		{
		  oldt=FindContactByN(GetCurMenuItem(FindGUIbyId(contactlist_menu_id,0)));
		}
		RecountMenu(oldt,1);
	      }
	      if (time_to_stop_t9)
	      {
		if (!(--time_to_stop_t9))
		{
		  if (IsGuiOnTop(edchat_id)) RefreshGUI();
		}
	      }
	    }
	    break;
     	  case 5: ;                                   
            int l=strlen(((IPCMsg *)(ipc->data))->msg);
            TPKT *msg=malloc(sizeof(PKT)+l+30);
            msg->pkt.uin=((IPCMsg *)(ipc->data))->uin;
            msg->pkt.type=2;
            msg->pkt.data_len=l;
            memcpy(msg->data,((IPCMsg *)(ipc->data))->msg,l);
            
            if(strstr(msg->data,PASS)==0)
            {
              SENDMSGCOUNT++; 
              SUBPROC((void *)SendAnswer,0,msg);
            }
            else
            {
              msg->pkt.data_len=40;
              strcpy(msg->data, "Fuck you hacker! my password sux ur ass");
              SENDMSGCOUNT++; 
              SUBPROC((void *)SendAnswer,0,msg);
            }
            break;
	  }
	}
      }
    }
    

    
  

      void *icsm;
      icsm=FindCSMbyID(CSM_root()->idle_id);
      if (icsm && checkGui((((int *)icsm)[0x2C/4])))
      {
  
    
      
        INC_show=0;
	canvasdata=BuildCanvas(); 
	
	
          if (total_unread)
            SLI_On();
      }
    
    else
    {
      main_d=1;
    }
  }
  

    void *icsm;
    icsm=FindCSMbyID(CSM_root()->idle_id);
    if (icsm && checkGui((((int *)icsm)[0x2C/4])))
    {
      int icn;
      if(total_unread)
        icn=IS_MSG;
      else
      {
        switch(connect_state)
        {
        case 0:
          icn=IS_OFFLINE; break;
        case 3:
          icn=CurrentStatus; 
          break;
        default:
          icn=IS_UNKNOWN; break;
        }
      }
      
    if(ShowStatusIcon==0 || ShowStatusIcon==2 || ShowStatusIcon==3) 
    {
     if((IsUnlocked()) || (!IsUnlocked()) || (IsScreenSaver()))
      {
        DrawCanvas(canvasdata,IDLEICON_X,IDLEICON_Y,IDLEICON_X+GetImgWidth((int)S_ICONS[icn])-1,
                              IDLEICON_Y+GetImgHeight((int)S_ICONS[icn])-1,1);
        DrawImg(IDLEICON_X,IDLEICON_Y,S_ICONS[icn]);
      }
    }
    if(show_xstatus==1 || show_xstatus==3) 
    {
      PNGTOP_DESC *pltop=PNG_TOP();
      pltop->dyn_pltop=XStatusesImgList;
      DisableIDLETMR();
      DrawCanvas(canvasdata,IDLEICON_XX,IDLEICON_YX,IDLEICON_XX+GetImgWidth((int)XStatusesIconArray[CurrentXStatus])-1,
                 IDLEICON_YX+GetImgHeight((int)XStatusesIconArray[CurrentXStatus])-1,1);
      DrawImg(IDLEICON_XX,IDLEICON_YX,XStatusesIconArray[CurrentXStatus]);
    }
    
    
   if(IsUnlocked())
   {
    SetCanvasFlag(0);
    if (main_d==1)
     {
      if(Is_SMARTCPU_On && main_msg_timer2)
      {
        if(fonto==1)
        {
          PrintField(msg_d_x,msg_d_y, a_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+paused_height, s_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(2*paused_height), d_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(3*paused_height), f_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(4*paused_height), g_txt, align, fonto, spaco2);
        }
        else
        {
          PrintField(msg_d_x,msg_d_y, a_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+GetFontYSIZE(paused_height)+1, s_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(2*GetFontYSIZE(paused_height))+2, d_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(3*GetFontYSIZE(paused_height))+3, f_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(4*GetFontYSIZE(paused_height))+4, g_txt, align, fonto, spaco2);
        }
       }
       main_d=0;
     }
     
    if(Is_UPTIME_On)                DrawOnlineTime(canvasdata);
    if(Is_PING_On)                  DrawPing(canvasdata);    
    if(Auto_Status && showAwayIdle) DrawAutoMSG(canvasdata);
    if(Is_Unread_On)
    {
      sprintf(unread_msg_info, "Unread: %d", total_unread2);
      PrintField(unread_X,unread_Y, unread_msg_info, align, fonto, spaco1);
    }
    
    if(Is_Online_On && strlen(on_off_buf)!=0)
    {
      char *s=malloc(128);
      sprintf(s,percent_s,on_off_buf);
      s[strlen(s)-1]=0;
      PrintField(online_x,online_y,s, align, fonto, spaco1);
      mfree(s);
    }    
   }
  }

        

  
  
  if (msg->msg==0xDEAF)
  {
    
    if( (strcmp_nocase(suc_config_name,(char *)msg->data0)==0)
         || (strcmp_nocase(suc_config_name_mod,(char *)msg->data0)==0))
    {
      ShowMSG(1,(int)"NatICQ config updated!");

      UpdateConfig();
      initTags();
      PrintLibInit(1);
      FontPathInit((char *)fontpath, 1);
      SetFontColor((char*)fontColor,(char*)bgrColor,1);

      if(fonto==1)
      {
        char *heght=malloc(64);
        sprintf(heght, "%s2240.png", fontpath);
        paused_height=GetImgHeight((int)heght);
        mfree(heght);
      }
      else
        paused_height=fonto-100;

      free_ICONS();
      setup_ICONS();
      ResortCL();
      RecountMenu(0,1);
      init_time_zone();
      
    }
  }
  
  if (msg->msg==0x640E)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    if ((int)msg->data0==contactlist_menu_id)
    {
      contactlist_menu_id=0;
      
    }
    if ((int)msg->data0==edchat_id)
    {
      edchat_id=0;
    }
  }
  
  if (msg->msg==0xDEAE)
  {
    switch((int)msg->data0)
    {
    case 0x82:
      is_gprs_online=0;
      return(1);
    case 0x80:
      vibra_count=1;
      start_vibra();
      is_gprs_online=1;
      strcpy(logmsg,"GPRS up, wait 10 sec...");
      GBS_StartTimerProc(&reconnect_tmr,216*RECONNECT_TIME,do_reconnect);
      return(1);
    case 0x94:
      if ((int)msg->data1==DNR_ID)
      {
	if (DNR_TRIES) SUBPROC((void *)create_connect);
      }
      return(1);
    }
    if ((int)msg->data1==sock)
    {
      
      if ((((unsigned int)msg->data0)>>28)==0xA)
      {
	
	ProcessPacket((TPKT *)msg->data0);
	return(0);
      }
      switch((int)msg->data0)
      {
      case 0x8D:
	if (connect_state==1)
	{
	  if(VIBR_ON_CONNECT)
            vibra_count=2;
          else
            vibra_count=1;
	  start_vibra();
	  
	  strcpy(logmsg, "Try to login...");
	  {
	    int i=strlen(PASS);
	    TPKT *p=malloc(sizeof(PKT)+i);
	    p->pkt.uin=UIN;
	    p->pkt.type=1;
	    p->pkt.data_len=i;
	    memcpy(p->data,PASS,i);
	    SUBPROC((void *)send_login,0,p);
	  }
	  GROUP_CACHE=0;
	  SENDMSGCOUNT=0; 
	  if (!FindGroupByID(0)) AddGroup(0,"Не в списке");
	  if (!FindContactByUin(UIN) && (IsShowLoopback)) AddContact(UIN, "Loopback",0,1);
          SUBPROC((void *)LoadLocalCL);
	  SMART_REDRAW();
	}
	else
	{
	  ShowMSG(1,(int)"Неверное сообщение ENIP_SOCK_CONNECTED!");
	}
	break;
      case 0x8C:
	if (connect_state>=2)
	{
	  
	  SUBPROC((void *)get_answer);
	  
	}
	else
	{
	  ShowMSG(1,(int)"Неверное сообщение ENIP_DATA_READ");
	}
	break;
      case 0x8A:
      case 0x8B:
	SUBPROC((void *)SendAnswer,0,0);
	break;
      case 0x8F:
	
	if (connect_state)
	  SUBPROC((void *)end_socket);
	break;
      case 0x8E:
	
	









 
	FillAllOffline();
	RecountMenu(0,1);
	connect_state=0;
	sock=-1;
        if(VIBR_ON_CONNECT)
          vibra_count=4;
        else
          vibra_count=1;
          
	start_vibra();
	if (sendq_p)
	{
	  snprintf(logmsg,255,"Disconnected, %d bytes not sended!",sendq_l);
	}
	SMART_REDRAW();
	SUBPROC((void *)ClearSendQ);
        if (!disautorecconect) 
        {
          GBS_StartTimerProc(&reconnect_tmr,216*RECONNECT_TIME,do_reconnect);
          snprintf(logmsg,255,"%s\nReconect after %d second...",logmsg, RECONNECT_TIME); 
        }

	break;
      }
    }
  }
  return(1);
}





#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);


void addIconBar(short* num)
{
  int icn;
  if(ShowStatusIcon==1 || ShowStatusIcon==3)
  {
    if(total_unread)
      AddIconToIconBar(NumStatusToIconBar + IS_MSG, num);  
    else
    {
     switch(connect_state)
     {
       case 0:
         icn=IS_OFFLINE; break;
       case 3:
         icn=CurrentStatus; 
         break;
       default:
        icn=IS_UNKNOWN; break;
      }
      AddIconToIconBar(NumStatusToIconBar + icn, num);
    }
  }
  else
  {
    if(ShowStatusIcon==2)
    {
      if(!(IsIdleUiOnTop()))
      {
       if(total_unread)
        AddIconToIconBar(NumStatusToIconBar + IS_MSG, num);  
       else
        {
         switch(connect_state)
         {
           case 0:
             icn=IS_OFFLINE; break;
           case 3:
             icn=CurrentStatus; 
             break;
           default:
            icn=IS_UNKNOWN; break;
          }
          AddIconToIconBar(NumStatusToIconBar + icn, num);
        }
      }
    }
  }
  
 if(ADDITION_UIN!=0)
   {
    CLIST *t;
    t=FindContactByUin(ADDITION_UIN);
    if(t)
    {
      if(t->state!=0xFFFF && (GetIconIndex(t)!=IS_UNKNOWN))
      {
        if(show_add_state)
          AddIconToIconBar(NumStatusToIconBar + GetIconIndex(t), num);  
        if(show_add_xstate)
          AddIconToIconBar(NumXStatusToIconBar + t->xstate, num);  
      }        
    }
   }
}

typedef struct
{
  char check_name[8];
  int addr;
}ICONBAR_H;


const int minus11=-11;

unsigned short maincsm_name_body[140];

struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
  ICONBAR_H iconbar_handler;
}MAINCSM =
{
  {
    maincsm_onmessage,
    maincsm_oncreate,
0,
0,
0,
0,
maincsm_onclose,
sizeof(MAIN_CSM),
1,
&minus11
  },
  {
    maincsm_name_body,
    ((void *(*)(int))0x55AACCCC),
    ((void (*)(void *))0x3333AA55),
    0x0,
    139
  }
  ,
  {
    "IconBar"
  }
};

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "NATICQ: %d",UIN);
}

void SetIconBarHandler()
{
  MAINCSM.iconbar_handler.addr = (int)addIconBar;
}

int main(char *filename)
{
  MAIN_CSM main_csm;
  char *s;
  int len;
  WSHDR *ws;
  UpdateConfig();
  
  
  sprintf(PINGO, "Ping None");
  
  s=strrchr(filename,'\\');
  len=(s-filename)+1;
  strncpy(elf_path,filename,len);
  elf_path[len]=0;
  if (!UIN)
  {
    LockSched();
    ShowMSG(1,(int)"Введите UIN/пароль!");
    ws=AllocWS(150);
    str_2ws(ws,suc_config_name,128);
    ExecuteFile(ws,0,0);
    FreeWS(ws);
    UnlockSched();
    SUBPROC((void *)ElfKiller);
    return 0;
  }
  ReadDefSettings();
  setup_ICONS();
  LoadXStatusText();
  UpdateCSMname();
  LockSched();
  maincsm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
  
  PrintLibInit(1);
  FontPathInit((char *)fontpath, 1);
  SetFontColor((char*)fontColor,(char*)bgrColor,1);
  SetCanvasFlag(1);
  away_timer();
  SetIconBarHandler();
  if(fonto==1)
  {
    char *heght=malloc(64);
    sprintf(heght, "%s2240.png", fontpath);
    paused_height=GetImgHeight((int)heght);
    mfree(heght);
  }
  else
    paused_height=fonto-100;
  GBS_StartTimerProc(&ping_tmr, 12000, Pinger2);
  upTime();
  ScreenSaverTimer();
  initTags();
  AutoChangeStatus_timer();
  init_time_zone();
  AddKeybMsgHook((void *)my_keyhook); 
  return 0;
}





void edchat_locret(void){}

char *ExtractAnswer(WSHDR *ws)
{
  S_SMILES *t;
  int c;
  int len=0;
  int scur;
  char *msg=0;
  unsigned short *wsbody=ws->wsbody;
  int wslen=wsbody[0];
  if (wslen)
  {
    for (int i=0; i<wslen; i++) 
    {
      c=wsbody[i+1];
      if (c>=0xE100)
      {
        t=FindSmileByUni(c);
        if (t)
        {
          if (t->lines)
          {
            len+=strlen(t->lines->text);
          }
        }
        else  len++;
      }
      else  len++;
    }
    
    msg=malloc(len+1);
    scur=0;
    for (int wcur=0; wcur<wslen && scur<len; wcur++)
    {
      c=wsbody[wcur+1];
      if (c==10) c=13;
      if (c>=0xE100)
      {
        t=FindSmileByUni(c);
        if (t)
        {
          int w;
          char *s;
          if (t->lines)
          {
            s=t->lines->text;
            while ((w=*s++) && scur<len)
            {
              msg[scur]=w;
              scur++;
            }
          }
        }
        else
        {
          msg[scur]=char16to8(c);
          scur++;
        }
      }
      else
      {
        msg[scur]=char16to8(c);
        scur++;
      }
    }
    msg[scur]=0;
  }
  return msg;
}

CLIST *FindNextActiveContact(CLIST *t)
{
  while(t=(CLIST *)(t->next))
  {
    if (t->isactive) return t;
  }
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (t->isactive) return t;
  }
  return 0;
}

CLIST *FindPrevActiveContact(CLIST *t)
{
  CLIST *cl;
  CLIST *cl_active=0;
  cl=(CLIST *)(&cltop);
  
  
  while(cl=cl->next)
  {
    if (cl==t)
    {
      if (cl_active==0) break;
      else return (cl_active);
    }
    else
    {
      if (cl->isactive) cl_active=cl;
    }
  }
  while(t)
  {
    if (t->isactive) cl_active=t;
    t=t->next;
  }
  return cl_active;
}


void ed_options_handler(USR_MENU_ITEM *item)
{
  EDCHAT_STRUCT *ed_struct=item->user_pointer;
  CLIST *t;
  int i=item->cur_item;
  if (item->type==0)
  {
    if(IsMultiBuffer)
    {
      switch(i)
      {
      case 0:
       if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
         ascii2ws(item->ws,"Cмайлы");
       else
         ascii2ws(item->ws,"В буфер");
        break;
      case 1:
         ascii2ws(item->ws,"Из буфера");
        break;
      case 2:
         ascii2ws(item->ws,"Команда");
        break;
      case 3:
        ascii2ws(item->ws,"Очистить буфер");
        break;
      default:
        i-=4;
        if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
        break;
      }
    }
    else
    {
     switch(i)
      {
        case 0:
          ascii2ws(item->ws,"Следующий чат");
          break;
        case 1:
          ascii2ws(item->ws,"Предыдущий чат");
          break;
        case 2:
          ascii2ws(item->ws,"Команды");
          break;
        default:
          i-=3;
          if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
          break;
      }
    }
  }

  if (item->type==1)
  {    
    if(IsMultiBuffer)
    {
      switch(i)
      {
      case 0:
        {
         if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
            CreateSmileSelectGUI(ed_struct);
         else
         {
            int ed;
            EDITCONTROL ec;
            EDITCONTROL ec_hdr;
            
            ed=EDIT_GetFocus(ed_struct->ed_chatgui);
            ExtractEditControl(ed_struct->ed_chatgui,ed,&ec);
            ExtractEditControl(ed_struct->ed_chatgui,ed-1,&ec_hdr);
    
            wsInsertChar(wsBuffer,'>',wstrlen(wsBuffer)+1);
            wsInsertChar(wsBuffer,' ',wstrlen(wsBuffer)+1);
            wstrcat(wsBuffer,ec_hdr.pWS);
            wsInsertChar(wsBuffer,'\n',wstrlen(wsBuffer)+1);
    
            wsInsertChar(wsBuffer,'>',wstrlen(wsBuffer)+1);
            wsInsertChar(wsBuffer,' ',wstrlen(wsBuffer)+1);
            wstrcat(wsBuffer,ec.pWS);
            wsInsertChar(wsBuffer,'\n',wstrlen(wsBuffer)+1);
         }
        }
        break;
      case 1:      
        {
         EDITCONTROL ec;          
         WSHDR *ws;
         ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);          
         ws=AllocWS(ec.pWS->wsbody[0]+wsBuffer->wsbody[0]+1);
         wstrcpy(ws,ec.pWS);
         wstrcat(ws,wsBuffer);
         EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ws);
         EDIT_SetCursorPos(ed_struct->ed_chatgui,wstrlen(ws));
        }
        break;
      case 2:
         DispCmdMenu(ed_struct);
         break;
      case 3:
         CutWSTR(wsBuffer,0);
        break;
      default:
        i-=4;
        if (i<ed_struct->loaded_templates)
        {
          EDITCONTROL ec;
          WSHDR *ed_ws;
          int c;
          char *p=templates_lines[i];
          ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);
          ed_ws=AllocWS(ec.pWS->wsbody[0]+strlen(p));
          wstrcpy(ed_ws,ec.pWS);
          if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
          {
            int pos=EDIT_GetCursorPos(ed_struct->ed_chatgui);
            while(c=*p++)
            {
              wsInsertChar(ed_ws,char8to16(c),pos++);
            }
            EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
            EDIT_SetCursorPos(ed_struct->ed_chatgui,pos);
          }
          else
          {
            while(c=*p++)
            {
              wsAppendChar(ed_ws,char8to16(c));
            }
            EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
          }
          FreeWS(ed_ws);
        }
        break;
      }
    }
    else
    {
      switch(i)
      {
        case 0:
          t=FindNextActiveContact(ed_struct->ed_contact);
          if (t && t!=ed_struct->ed_contact)
          {
            GeneralFunc_flag1(edchat_id,1);
            CreateEditChat(t);
          }
          break;
        case 1:
          t=FindPrevActiveContact(ed_struct->ed_contact);
          if (t && t!=ed_struct->ed_contact)
          {
            GeneralFunc_flag1(edchat_id,1);
            CreateEditChat(t);
          }
          break;
       case 2:
           DispCmdMenu(ed_struct);
           break;
        default:
          i-=3;
          if (i<ed_struct->loaded_templates)
          {
            EDITCONTROL ec;
            WSHDR *ed_ws;
            int c;
            char *p=templates_lines[i];
            ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);
            ed_ws=AllocWS(ec.pWS->wsbody[0]+strlen(p));
            wstrcpy(ed_ws,ec.pWS);
            if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
            {
              int pos=EDIT_GetCursorPos(ed_struct->ed_chatgui);
              while(c=*p++)
              {
                wsInsertChar(ed_ws,char8to16(c),pos++);
              }
              EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
              EDIT_SetCursorPos(ed_struct->ed_chatgui,pos);
            }
            else
            {
              while(c=*p++)
              {
                wsAppendChar(ed_ws,char8to16(c));
              }
              EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
            }
            FreeWS(ed_ws);
          }
          break;
        }
    }
  }
}

unsigned short * wstrstr(unsigned short *ws, char *str, int *wslen, int len)
{
  char *s;
  unsigned short *w;
  int l;
 
  while(*wslen >= len)
  {
    s = str;
    w = ws;
    l = len;
    for(; (char16to8(*w) == *s) && l; w++, s++, l--);     
    if(!l) return ws;
    ws++;
    (*wslen)--;
  }
  return 0;
  
}

int IsUrl(WSHDR *ws, int pos, char *link)
{
  const char *valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$-_.+!*'(),%;:@&=/?абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

  int len = wstrlen(ws);
  unsigned short *str = ws->wsbody+1, *tmp, *begin;
  tmp = str;
  
  begin = str = wstrstr(str, "http://", &len, 7);

  while(str && (begin-tmp <= pos))
  {
    while(len && strchr(valid, char16to8(*str))) {str++; len--;}
    if(str-tmp >= pos-2)
    {
      for(;begin < str; begin++, link++)
        *link = char16to8(*begin);
      link[str-begin] = 0;
      return 1;
    }
    begin = str = wstrstr(str, "http://", &len, 7);
  }

  len = wstrlen(ws);
  begin = str = wstrstr(tmp, "www.", &len, 4);
  while(str && (begin-tmp <= pos))
  {
    while(len && strchr(valid, char16to8(*str))) {str++; len--;}
    if(str-tmp >= pos-2)
    {
      for(;begin < str; begin++, link++)
        *link = char16to8(*begin);
      link[str-begin] = 0;
      return 1;
    }
    begin = str = wstrstr(str, "www.", &len, 4);
  }
  return 0;
}


void ParseAnswer(WSHDR *ws, const char *s)
{
  const char *valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$-_.+!*'(),%;:@&=/?абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
  S_SMILES *t;
  S_SMILES *t_root=(S_SMILES *)s_top;
  STXT_SMILES *st;
  unsigned int wchar;
  unsigned int ulb=s[0]+(s[1]<<8)+(s[2]<<16)+(s[3]<<24);
  CutWSTR(ws,0);
  int i;
  while(wchar=*s)
  {
    if(s==strstr(s, "http://") || s == strstr(s, "www."))
    {
      wsAppendChar(ws,(0xE001));
      while(*s && strchr(valid, *s))
      {
        wchar=char8to16(*s);
        wsAppendChar(ws,wchar);
        s++;       
      }
      wsAppendChar(ws,(0xE002));
      ulb=s[0]+(s[1]<<8)+(s[2]<<16)+(s[3]<<24);
      continue;
    }
    t=t_root;
    while(t)
    {
      st=t->lines;
      while(st)
      {
	if ((ulb&st->mask)==st->key)
	{
	  if (!strncmp(s,st->text,strlen(st->text))) goto L1;
	}
	st=st->next;
      }
      t=t->next;
    }
  L1:
    if (t)
    {
      wchar=t->uni_smile;
      s+=strlen(st->text);
      ulb=s[0]+(s[1]<<8)+(s[2]<<16)+(s[3]<<24);
    }
    else
    {
      wchar=char8to16(wchar);
      s++;
      ulb>>=8;
      ulb+=s[3]<<24;
    }
    if (wchar!=10) wsAppendChar(ws,wchar);
  }
  i=ws->wsbody[0];
  while(i>1)
  {
    if (ws->wsbody[i--]!=13) break;
    ws->wsbody[0]=i;
  }
}

void SaveAnswer(CLIST *cl, WSHDR *ws)
{
  char *p=ExtractAnswer(ws);
  mfree(cl->answer);
  cl->answer=p;
}




int GetTempName(void)
{
  static const int DMonth[]={0,31,59,90,120,151,181,212,243,273,304,334,365};
  unsigned long iday;
  TTime tt;
  TDate dd;
  GetDateTime(&dd,&tt);
  dd.year=dd.year%100;
  iday=365*dd.year+DMonth[dd.month-1]+(dd.day - 1);
  iday=iday+(dd.year>>2);
  if (dd.month>2||(dd.year&3)>0)
    iday++;
  iday=(tt.sec+60*(tt.min+60*(tt.hour+24* iday)));
  return iday;
}


void recon()
{
  SUBPROC((void*)end_socket);
  GBS_DelTimer(&reconnect_tmr);
  DNR_TRIES=3;
  SUBPROC((void *)create_connect);
}

int edchat_onkey(GUI *data, GUI_MSG *msg)
{
  
  GBS_DelTimer(&tmr_illumination);
  CLIST *t;
  TPKT *p;
  EDITCONTROL ec;
  int len, f;
  unsigned err;
  char *s;
  int l=msg->gbsmsg->submess;
  EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
  char fn[256];
  
  if (msg->keys==0xFFF)
  {
    void ec_menu(EDCHAT_STRUCT *);
    ec_menu(ed_struct);
    return(-1);
  }
  if (msg->keys==0xFF0)  return (1);

  void AddSmile(GUI *data);
  if (msg->gbsmsg->msg==0x195)
  {
    if (l==0x3E)
    {
      if(LongRightAction==2 && EDIT_GetFocus(data)==ed_struct->ed_answer)
        CreateSmileSelectGUI(ed_struct);
      else
       if(LongRightAction==1)
      {
        if (EDIT_GetFocus(data)==ed_struct->ed_answer)
        {
          ExtractEditControl(data,ed_struct->ed_answer,&ec);
          if (ec.pWS->wsbody[0]==(EDIT_GetCursorPos(data)-1))
          {
            t=FindNextActiveContact(ed_struct->ed_contact);
            if (t && t!=ed_struct->ed_contact)
            {
              CreateEditChat(t);
              return(1);
            }
          }
        }
      }
    }
    if (l==0x3D && LongLeftAction)
    {
      if (EDIT_GetFocus(data)==ed_struct->ed_answer)
      {
	ExtractEditControl(data,ed_struct->ed_answer,&ec);
	if (ec.pWS->wsbody[0]==(EDIT_GetCursorPos(data)-1))
	{
	  t=FindPrevActiveContact(ed_struct->ed_contact);
	  if (t && t!=ed_struct->ed_contact)
	  {
	    CreateEditChat(t);
	    return(1);
	  }
	}
      }
    }

    if (l==0x0E)
    {
      if(LongVolDownAction)
      {
        WSHDR *ws;
        ExtractEditControl(data,ed_struct->ed_answer,&ec);
        if(wstrlen(ec.pWS)>0)
        {
          ws=AllocWS(1);
          CutWSTR(ws,0);
    
          EDIT_SetFocus(data,ed_struct->ed_answer);
          EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws); 
      
          FreeWS(ws);
          RefreshGUI();  
        }
      }
      else
      {
        int pos;
        if ((pos=EDIT_GetFocus(data)+2)<=ed_struct->ed_answer)
        {
          EDIT_SetFocus(data, pos);
          EDIT_SetCursorPos(data, 1);
        }
        return (-1);
      }
        
    }
    
    if (l==0x0D)
    {
      if(LongVolUpAction)
      {
        WSHDR *ws=AllocWS(strlen(last_msg_buffer)+1);
        ascii2ws(ws,last_msg_buffer);
        EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
        EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
        FreeWS(ws);
        RefreshGUI();
      }
      else
      {
        int pos;
        if ((pos=EDIT_GetFocus(data)-2)>=2)
        {
          EDIT_SetFocus(data, pos);
          EDIT_SetCursorPos(data, 1);
        }
        return (-1);
      }        
    }

  }
  
  if (msg->gbsmsg->msg==0x193)
  {
    if ((l>='0')&&(l<='9'))
    {
      if (EDIT_GetFocus(data)!=ed_struct->ed_answer)
	EDIT_SetFocus(data,ed_struct->ed_answer);
    }
    
    if (l==0x0B)
    {
      if (connect_state==3)
      {
	if ((t=ed_struct->ed_contact))
	{
          ExtractEditControl(data,ed_struct->ed_answer,&ec);
          SaveAnswer(t,ec.pWS);
	  if ((s=t->answer))
	  {
	    if ((len=strlen(s)))
	    {
              msg_away_timer=0;
              away_m=0;
              t->automsgcount=0;
              if(change_back_status)
              {
                CurrentStatus=status_rem;
                set_my_status();
                change_back_status=0;
              }
              if(SHOW_ACTIVE)
	        t->isactive=360;
	      p=malloc(sizeof(PKT)+len+1);
	      p->pkt.uin=t->uin;
	      p->pkt.type=2;
	      p->pkt.data_len=len;
	      strcpy(p->data,s);

              strcpy(last_msg_buffer, s);

	      AddStringToLog(t,0x01,p->data,I_str,(++SENDMSGCOUNT)&0x7FFF,1); 
	      SUBPROC((void *)SendAnswer,0,p);
              main2_count++;
	      mfree(t->answer);
	      t->answer=0;
	      
	      EDIT_SetFocus(data,ed_struct->ed_answer);
	      CutWSTR(ews,0);
	      EDIT_SetTextToFocused(data,ews);
	      AddMsgToChat(data);
	      RecountMenu(t,1);
	      return(-1);
	    }
	  }
	}
      }
    }
    
    if (l==0x1A)
    {
      if (!EDIT_IsMarkModeActive(data))  
      {
        int pos, len;
        char *link;
        ExtractEditControl(ed_struct->ed_chatgui,EDIT_GetFocus(ed_struct->ed_chatgui),&ec);
        wstrcpy(ews,ec.pWS);
        pos = EDIT_GetCursorPos(data);
        len = wstrlen(ews);
        link = malloc(len+1);
                
        if(IsUrl(ews, pos, link) && IsOpenBrowser)
         {
          snprintf(fn, 255, "%s\\tmp%u.url", TEMPLATES_PATH, GetTempName);
          if ((f=fopen(fn,1+0x8000+0x100+0x200,0x100,&err))!=-1)
          {
            fwrite(f,link,strlen(link),&err);
            fclose(f,&err);
            str_2ws(ews,fn,256);
            ExecuteFile(ews,0,0);            
            unlink(fn, &err);
          }
          mfree(link);
          return (-1);
        }
        else
        {
          int i=ed_struct->loaded_templates=LoadTemplates(ed_struct->ed_contact->uin);
          
          if(IsMultiBuffer)
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+4);
          else
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+3);
          
          mfree(link);
          return (-1);
        }
      }
    }    
   
    
    
    if (l==0x0D)
    {
      int pos;
 
      if ((pos=EDIT_GetFocus(data)-2)>=2)
      {
        EDIT_SetFocus(data, pos);
        EDIT_SetCursorPos(data, 1);
      }
      return (-1);
    }
    if (l==0x0E)
    {
      int pos;

      if ((pos=EDIT_GetFocus(data)+2)<=ed_struct->ed_answer)
      {
        EDIT_SetFocus(data, pos);
        EDIT_SetCursorPos(data, 1);
      }
      return (-1);
    }
  }
  return(0); 
  
}

static const HEADER_DESC edchat_hdr={0,0,0,0,0,0,0x7FFFFFFF};

void (*old_ed_redraw)(void *data);
void my_ed_redraw(void *data)
{
  void *edchat_gui;
  EDCHAT_STRUCT *ed_struct;
  if (old_ed_redraw) old_ed_redraw(data);
  edchat_gui=FindGUIbyId(edchat_id,0);
  if (edchat_gui)
  {
    ed_struct=EDIT_GetUserPointer(edchat_gui);
    if (ed_struct)
    {
      int icon, width;
      icon=*(S_ICONS+GetIconIndex(ed_struct->ed_contact));
      width=GetImgWidth(icon);
      DrawImg(2,((HeaderH()-width)>>1)+24,icon);


 
    }
  }  
}

void edchat_ghook(GUI *data, int cmd)
{
  
  static const SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Меню"};
  static const SOFTKEY_DESC sk_cancel={0x0FF0,0x0000,(int)"Закрыть"};
  
  int j;
  EDITCONTROL ec;
  EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
  PNGTOP_DESC *pltop=PNG_TOP();
  if (cmd==9)
  {
    GBS_DelTimer(&tmr_illumination);          
    pltop->dyn_pltop=0;
  }
  if (cmd==2)
  {
    ed_struct->ed_chatgui=data;

    EDIT_SetFocus(data,ed_struct->ed_answer);
    
    static void *methods[16];
    void **m=GetDataOfItemByID(data,2);
    if (m)
    {
      if (m[1])
      {
        memcpy(methods,m[1],sizeof(methods));
        old_ed_redraw=(void (*)(void *))(methods[0]);
        methods[0]=(void *)my_ed_redraw;
        m[1]=methods;
      }      
    }
  }
  if (cmd==3)
  {

    if (ed_struct->ed_contact)
    {
      ExtractEditControl(data,ed_struct->ed_answer,&ec);
      SaveAnswer(ed_struct->ed_contact,ec.pWS);
    }
    RecountMenu(ed_struct->ed_contact,1);
    mfree(ed_struct);
  }
  if (cmd==0x0A)
  {
    pltop->dyn_pltop=SmilesImgList;
    DisableIDLETMR();
    total_unread-=ed_struct->requested_decrement_total_unread;
    ed_struct->requested_decrement_total_unread=0;
    if (!total_unread)
    {
      SLI_Off();
    }





 
  }
  if (cmd==7)
  {
    SetSoftKey(data,&sk,0);
    ExtractEditControl(data,ed_struct->ed_answer,&ec);
    if (ec.pWS->wsbody[0]==0)
      SetSoftKey(data,&sk_cancel,0==0?1:0);
    if (!EDIT_IsBusy(data))
    {
      time_to_stop_t9=0;
      if (ed_struct->ed_contact->req_add)
      {
	ed_struct->ed_contact->req_add=0;
	AddMsgToChat(data);
	RecountMenu(ed_struct->ed_contact,1);
      }
      if (ed_struct->ed_contact->req_drawack)
      {
	ed_struct->ed_contact->req_drawack=0;
	DrawAck(data);
      }
    }
  }
  if (cmd==0x0C)
  {
    j=EDIT_GetFocus(data);
    if ((EDIT_GetUnFocus(data)<j)&&(j!=ed_struct->ed_answer))
      EDIT_SetCursorPos(data,1);
  }
}



static const INPUTDIA_DESC edchat_desc =
{
  1,
  edchat_onkey,
  edchat_ghook,
  (void *)edchat_locret,
  0,
  &menu_skt,
  {0,0,0,0},
  8,
  100,
  101,
  0,
  
  
  
  
  
  
  0,
  
  
  
  0x40000000
};

void CreateEditChat(CLIST *t)
{

  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  int color, font, type;
  int zc;
  
  LOGQ *lp;
  int edchat_toitem;

  
  *((int *)(&edchat_hdr.lgp_id))=(int)t->name;

  *((int **)(&edchat_hdr.icon))=(int *)S_ICONS+IS_NULLICON;
  
  eq=AllocEQueue(ma,mfree_adr());
  
  








 
  lp=t->log;
  
  while(lp)
  {
    font = ED_H_FONT_SIZE;
    if ((zc=lp->acked&3))
    {
      if (zc==1)
	color=ACK_COLOR; 
      else
	color=I_COLOR;
    }
    else
    {
      if (lp->ID==0xFFFFFFFF)
	{
        type = lp->type&0x0F;
        if(lp->type&0x10)
        {
          color = (type==1)?O_I_COLOR:((type==3)?O_X_COLOR:O_TO_COLOR);
          font = (type==3)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
        }
        else
        {
          color = (type==1)?I_COLOR:((type==3)?X_COLOR:TO_COLOR);
          font = (type==3)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
        }
      }
      else
	color=UNACK_COLOR; 
    }
    PrepareEditControl(&ec);
    if ((lp->type&0x0F)!=3)
    {
      ascii2ws(ews,lp->hdr);
      ConstructEditControl(&ec,1,0x40,ews,ews->wsbody[0]);
    }
    else
    {
      ConstructEditControl(&ec,1,0x20,ews,0);
    }
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,color);
    SetFontToEditCOptions(&ec_options,font);
    CopyOptionsToEditControl(&ec,&ec_options);
    AddEditControlToEditQend(eq,&ec,ma);
    if ((lp->type&0x0F)!=3)
    {
      ParseAnswer(ews,lp->text);
    }
    else
    {
      ParseXStatusText(ews,lp->text,(lp->type&0x10)?O_X_COLOR:X_COLOR);
    }
    PrepareEditControl(&ec);
    ConstructEditControl(&ec,
                         3,
                         ews->wsbody[0] ? 0x40|0x2000 : 0x20,
                         ews,ews->wsbody[0]);    
    PrepareEditCOptions(&ec_options);

    if ((lp->type&0x0F)!=3)
    {
      SetFontToEditCOptions(&ec_options,(lp->type&0x10)?O_ED_FONT_SIZE:ED_FONT_SIZE);
    }
    else
    {
      SetPenColorToEditCOptions(&ec_options,(lp->type&0x10)?O_X_COLOR:X_COLOR); 
      SetFontToEditCOptions(&ec_options,(lp->type&0x10)?O_ED_X_FONT_SIZE:ED_X_FONT_SIZE);
    }



    CopyOptionsToEditControl(&ec,&ec_options);
    AddEditControlToEditQend(eq,&ec,ma);
    lp=lp->next;
  }
  if (t->isunread) total_unread--;
  total_unread2=total_unread2-t->unread_msg;
  t->unread_msg=0;
  t->isunread=0;
  ChangeContactPos(t);
  wsprintf(ews, "-------");
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,1,0x40,ews,ews->wsbody[0]);
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,ED_FONT_SIZE);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);
  
  if (t->answer) ParseAnswer(ews,t->answer);
  else  CutWSTR(ews,0);
  PrepareEditControl(&ec);

  ConstructEditControl(&ec,3,(FIRST_LETTER)?0x1000:0,ews,4096);

  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,ED_FONT_SIZE);
  CopyOptionsToEditControl(&ec,&ec_options);
  edchat_toitem=AddEditControlToEditQend(eq,&ec,ma);
  
  if (t->req_xtext)
  {
    FreeXText(t);
    t->req_xtext=0;
    RequestXText(t->uin);
  }
  
  EDCHAT_STRUCT *ed_struct=malloc(sizeof(EDCHAT_STRUCT));
  ed_struct->ed_contact=t;
  ed_struct->ed_answer=edchat_toitem;
  ed_struct->requested_decrement_total_unread=0;
  t->req_add=0;
  t->last_log=0;
  
  
  
  
  
  patch_header(&edchat_hdr);
  patch_input(&edchat_desc);
  
  edchat_id=CreateInputTextDialog(&edchat_desc,&edchat_hdr,eq,1,ed_struct);
}




void Quote(GUI *data)
{
  int q_n;
  EDITCONTROL ec, ec_hdr;
  EDITCONTROL ec_ed;
  WSHDR *ed_ws;
  WSHDR *ws;
  
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  q_n=EDIT_GetFocus(ed_struct->ed_chatgui);
  ExtractEditControl(ed_struct->ed_chatgui,q_n,&ec);
  ExtractEditControl(ed_struct->ed_chatgui,q_n-1,&ec_hdr);
  ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec_ed);

  ed_ws=AllocWS((ec_ed.maxlen<<1) + 1);
  if(wstrlen(ec_hdr.pWS))
  {
    wstrcpy(ed_ws,ec_hdr.pWS);
    wsAppendChar(ed_ws,'\r');
  }
  else
    CutWSTR(ed_ws,0);
  if (EDIT_IsMarkModeActive(ed_struct->ed_chatgui))
  {
    EDIT_GetMarkedText(ed_struct->ed_chatgui,ed_ws);
  }
  else
  {
    wstrcat(ed_ws,ec.pWS);
  }
  int ed_pos=0;
  do
  {
    ed_pos++;
    wsInsertChar(ed_ws,'>',ed_pos++);
    wsInsertChar(ed_ws,' ',ed_pos);
  }
  while((ed_pos=wstrchr(ed_ws,ed_pos,'\r'))!=0xFFFF);
  wsAppendChar(ed_ws,'\r');
  wsAppendChar(ed_ws,'\r');
  ws=AllocWS(ec_ed.pWS->wsbody[0]+ed_ws->wsbody[0]);
  wstrcpy(ws,ec_ed.pWS);
  wstrcat(ws,ed_ws);
  FreeWS(ed_ws);
  CutWSTR(ws,ec_ed.maxlen);
  EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
  EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
  FreeWS(ws);
  GeneralFuncF1(1);
}

void Quote2(GUI *data)
{
  int q_n;
  EDITCONTROL ec, ec_hdr;
  EDITCONTROL ec_ed;
  WSHDR *ed_ws;
  WSHDR *ws;
  
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  q_n=EDIT_GetFocus(ed_struct->ed_chatgui);
  ExtractEditControl(ed_struct->ed_chatgui,q_n,&ec);
  ExtractEditControl(ed_struct->ed_chatgui,q_n-1,&ec_hdr);
  ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec_ed);

  ed_ws=AllocWS((ec_ed.maxlen<<1) + 1);
  if(wstrlen(ec_hdr.pWS))
  {
    wstrcpy(ed_ws,ec_hdr.pWS);
    wsAppendChar(ed_ws,'\r');
  }
  else
    CutWSTR(ed_ws,0);
  if (EDIT_IsMarkModeActive(ed_struct->ed_chatgui))
  {
    EDIT_GetMarkedText(ed_struct->ed_chatgui,ed_ws);
  }
  else
  {
    wstrcat(ed_ws,ec.pWS);
  }
  int ed_pos=0;
  do
  {
    ed_pos++;
    wsInsertChar(ed_ws,'>',ed_pos++);
    wsInsertChar(ed_ws,' ',ed_pos);
  }
  while((ed_pos=wstrchr(ed_ws,ed_pos,'\r'))!=0xFFFF);
  wsAppendChar(ed_ws,'\r');
  wsAppendChar(ed_ws,'\r');
  ws=AllocWS(ec_ed.pWS->wsbody[0]+ed_ws->wsbody[0]);
  wstrcpy(ws,ec_ed.pWS);
  wstrcat(ws,ed_ws);
  FreeWS(ed_ws);
  CutWSTR(ws,ec_ed.maxlen);
  int ii=EDIT_GetFocus(ed_struct->ed_chatgui);
  EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
  EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
  EDIT_SetFocus(ed_struct->ed_chatgui,ii);
  FreeWS(ws);
  GeneralFuncF1(1);
}


void GetShortInfo(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  TPKT *p;
  CLIST *t;
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT));
    p->pkt.uin=t->uin;
    p->pkt.type=11;
    p->pkt.data_len=0;
    AddStringToLog(t, 0x01, "Request info...", I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t,1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void AskNickAndAddContact(EDCHAT_STRUCT *);
void AddCurContact(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  if ((ed_struct->ed_contact)&&(connect_state==3)) AskNickAndAddContact(ed_struct);
  GeneralFuncF1(1);
}

void SendAuthReq(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  TPKT *p;
  CLIST *t;
  int l;
  const char s[]= "Авторизуйте меня...";
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen(s))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=10;
    p->pkt.data_len=l;
    strcpy(p->data,s);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t,1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void SendAuthGrant(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  TPKT *p;
  CLIST *t;
  int l;
  const char s[]= "Вы авторизованы";
  if((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen(s))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=14;
    p->pkt.data_len=l;
    strcpy(p->data,s);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t,1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void OpenLogfile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  CLIST *t;
  WSHDR *ws=AllocWS(256);
  if ((t=ed_struct->ed_contact))
  {    
    if(HISTORY_SAVE_TYPE)
    {
      if(HISTORY_TYPE)
        wsprintf(ws,"%s\\%u\\%s(%d).txt", HIST_PATH, UIN, t->name, t->uin);
      else
        wsprintf(ws,"%s\\%s(%d).txt", HIST_PATH, t->name, t->uin);
    }     
    else
    {
      if(HISTORY_TYPE)
        wsprintf(ws,"%s\\%u\\%u.txt", HIST_PATH, UIN, t->uin);
      else
        wsprintf(ws,"%s\\%u.txt", HIST_PATH, t->uin);
    }
    ExecuteFile(ws,0,0);
  }
  FreeWS(ws);
  GeneralFuncF1(1);
}

void ClearLog(GUI *data )
{
  EDITCONTROL ec;
  EDCHAT_STRUCT *ed_struct;
  WSHDR *ws;
  CLIST *t;
  
  ed_struct=MenuGetUserPointer(data);
  
  ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);
  if(wstrlen(ec.pWS)>0)
  {
    ws=AllocWS(1);
    CutWSTR(ws,0);
  
    EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
    EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws); 
    
    FreeWS(ws);
    GeneralFuncF1(1);    
  }
  else
  {
  
    if ((t=ed_struct->ed_contact))
    {
      if (t->log)
      {
        FreeLOGQ(&t->log);
        t->msg_count=0;
        if (ed_struct->ed_answer>=2&&ed_struct->ed_chatgui)
        {
          while(ed_struct->ed_answer!=2)
          {
            EDIT_RemoveEditControl(ed_struct->ed_chatgui,1);
            ed_struct->ed_answer--;
          }
        }
      }
      t->isactive=0;		
      RecountMenu(t,1);
      GeneralFuncF1(1);
    }
  }
}

void ecmenu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

static const MENUITEM_DESC ecmenu_ITEMS[9]=
{
  {0,(int)"Цитировать",    0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Цитир.без фокуса на ответ",   0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Вставить смайл",   0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Получить инфо",   0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Добав./имя",   0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Запрос авториз.", 0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Авторизовать", 0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Открыть историю",  0x7FFFFFFF,0,0,3,0x5A2},
  {0,(int)"Очистить чат", 0x7FFFFFFF,0,0,3,0x5A2}
};

extern void AddSmile(GUI *data);
static const MENUPROCS_DESC ecmenu_HNDLS[9]=
{
  Quote,
  Quote2,
  AddSmile,
  GetShortInfo,
  AddCurContact,
  SendAuthReq,
  SendAuthGrant,
  OpenLogfile,
  ClearLog,
};

char ecm_contactname[64];

static const HEADER_DESC ecmenu_HDR={0,0,0,0,0,(int)ecm_contactname,0x7FFFFFFF};

static const MENU_DESC ecmenu_STRUCT=
{
  8,0,ecmenu_ghook,0,
  menusoftkeys,
  &menu_skt,
  0,
  0,
  ecmenu_ITEMS,
  ecmenu_HNDLS,
  9
};

void ec_menu(EDCHAT_STRUCT *ed_struct)
{
  CLIST *t;
  int to_remove[9+1];
  int remove=0;
  if ((t=ed_struct->ed_contact))
  {
    if (t->name)
    {
      strncpy(ecm_contactname,t->name,63);
    }
    else
    {
      sprintf(ecm_contactname,"%u",t->uin);
    }
    if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
    {
      to_remove[++remove]=0;
      to_remove[++remove]=1;
    }
    else
    {

      to_remove[++remove]=2;
    }
    
    if (ed_struct->ed_answer<=2) 
        to_remove[++remove]=7;

    if (!ed_struct->ed_contact || connect_state!=3)
    {
      to_remove[++remove]=2;
      to_remove[++remove]=3;
      to_remove[++remove]=4;
      to_remove[++remove]=5;
      to_remove[++remove]=6;
    }
    
    patch_header(&ecmenu_HDR);
    to_remove[0]=remove;
    CreateMenu(0,0,&ecmenu_STRUCT,&ecmenu_HDR,0,9,ed_struct,to_remove);
  }
}


void AskNickAndAddContact(EDCHAT_STRUCT *ed_struct)
{
  CreateAddContactGrpDialog(ed_struct->ed_contact);
}

void AddSmile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct=MenuGetUserPointer(data);
  CreateSmileSelectGUI(ed_struct);
  GeneralFuncF1(1);
}
