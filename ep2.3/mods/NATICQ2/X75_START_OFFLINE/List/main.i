





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


typedef struct {
  int type;
  void *func;
}OBSevent;














































































































 

















typedef struct{
  short pid_from;
  short msg; 
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
  short pid_from;
  short msg;
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
  int unk;
  int unk1;
  void *methods;
  int unk2;
  void *tmr;
  short cepid;
} CSMQ;

typedef struct{
  void *next;
  void *prev;
  char flag1;
  int parent_id;
  int (*proc)(CSMQ *, void *);
  CSMQ *csm_q;
  CSM_RAM *cmd_csm;
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
  signed char enabled_options;
  unsigned char obex_path_id;
  unsigned short menu_flag;
  const int *icon1;
  const int *icon2; 
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
  char w;
  char h;
  unsigned short bpnum; 
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
  char unk1[14];
  short file_attr;
  unsigned int create_date_time;
  unsigned int file_size;
  char unk2[16];
  char folder_name[128];
  char unk3;
  char file_name[271];
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
  char field_0;
  char no_data;
  char field_2;
  char item_type;
  int field_4;
  void *data;
}AB_UNPRES_ITEM;

typedef struct{
  short field_0;
  short field_2;
  short field_4;
  short number_of_records;
  short field_8;
  short field_A;
  AB_UNPRES_ITEM *record_list;
  char field_10;
  char field_11;
  short field_12;
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
  char unk1;
  char *unk;
  char one;
  char isactivated;
}LMAN_DATA;


typedef struct{
  unsigned short repeat_num;  
  unsigned short unk;
  int time_between_play;
  int play_first; 
  int volume;
  int unk4;     
  int unk5;     
  int unk6;     
  int unk7;
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
  unsigned short type;  
  WSHDR *filename;
  WSHDR *text_resol;
  int uid;
  unsigned short width;
  unsigned short height;
  WSHDR *duration_wav_ws;
  unsigned int duration_wav;
  WSHDR *video_ws;
  int video_len;
} FILE_PROP;

typedef struct
{
  int mode;
  int fixed_directory_key; 
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
  int unk17_32[10]; 
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
__swi __arm int GetWavLen(TWavLen *wl); 

#pragma swi_number=0x0046
__swi __arm int HasNewSMS(void);



#pragma swi_number=0x0047
__swi __arm int HasFlashSMS(void);



#pragma swi_number=0x4A
__swi __arm short PlayFile(int flags, WSHDR *foldername, WSHDR *filename, int zero, int CepId, int Msg, const PLAYFILE_OPT *sfo);



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







#pragma swi_number=0x0052
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



#pragma swi_number=0x99
__swi __arm int PlayMelodyInMem(char Unk_0x11, void * MelAddr, int MelSize, int CepId, int Msg, int Unk_0); 



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

#pragma swi_number=0x80D8
__swi __arm char * RamMenuAnywhere();

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
__swi __arm REGEXPLEXT_ARM_NEW *get_regextpnt_by_uid(int uid);





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



















 
#pragma swi_number=0x240
__swi __arm char *MD5_Hash(const unsigned char *data, unsigned long n, unsigned char *md); 



#pragma swi_number=0x241
__swi __arm void SetDateTime(TDate *, TTime *);



#pragma swi_number=0x244
__swi __arm int Base64Encode(void *inbuf, int insize, void *outbuf, int outsize);



#pragma swi_number=0x245
__swi __arm int Base64Decode(char *inbuf, int insize, void *outbuf, int outsize, char *_null, int *unk5);



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
__swi __arm HObj Obs_CreateObject(int uid_in, int uid_out, int prio, int msg_callback, int emb4, int sync,unsigned int *ErrorNumber);




#pragma swi_number=0x25A
__swi __arm int Obs_DestroyObject(HObj hObj);




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
__swi __arm int Obs_SetScaling (HObj hObj, int unk5);





 
#pragma swi_number=0x265
__swi __arm int Obs_Pause (HObj hObj);


#pragma swi_number=0x266
__swi __arm int Obs_Resume (HObj hObj);


#pragma swi_number=0x267
__swi __arm int Obs_Stop (HObj hObj);


#pragma swi_number=0x268
__swi __arm int Obs_Prepare (HObj hObj);

#pragma swi_number=0x269
__swi __arm int Obs_SetRenderOffset (HObj hObj,short x,short y);

#pragma swi_number=0x26A
__swi __arm int Obs_SetPosition (HObj hObj,int ms);


#pragma swi_number=0x26B
__swi __arm int Obs_Mam_SetPurpose (HObj hObj,char purpose); 

#pragma swi_number=0x26C
__swi __arm int Obs_Sound_SetVolumeEx (HObj hObj, char vol, char delta);

#pragma swi_number=0x26D
__swi __arm int Obs_Sound_GetVolume (HObj hObj, char *vol);

#pragma swi_number=0x26E
__swi __arm int Obs_Sound_SetPurpose (HObj hObj,int purpose);








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
  char clientid;
  int isunread;
  int req_add;
  int req_drawack;
  LOGQ *log;
  char *answer;
  LOGQ *last_log;
  int isactive;
  int msg_count;
  char *xtext;
  int req_xtext;
  int local;  
  
  int unread_msg;
  int isonline;
  int online;
  int automsgcount;
  char *sttime;
  TTime time_struct;
  int recv;
  int send;
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




typedef struct
{
  int curpos; 
  unsigned int itemFromDisp; 
  unsigned int totalItems; 
  unsigned int curpage; 
  unsigned int column; 
  unsigned int startItem; 
  unsigned int nlines;
  unsigned int font;
  unsigned int FontH;
  unsigned int curitem;
  
  
  int i;
  char s[64];
  IMGHDR *img;
  RECT rc;
}GUIMENU;



extern int sit_file_size(char*);


enum ISTATUS
{
  IS_OFFLINE,
  IS_INVISIBLE,
  IS_AWAY,
  IS_NA,
  IS_OCCUPIED,
  IS_DND,
  IS_DEPRESSION,
  IS_EVIL,
  IS_HOME,
  IS_LUNCH,
  IS_WORK,
  IS_ONLINE,
  IS_FFC,

  IS_MSG,
  IS_UNKNOWN,
  IS_GROUP,
  IS_GROUPOFF,
  IS_NULLICON,

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

  ICON_CHECK,
  ICON_UNCHECK,

  ICON_BG,

  TOTAL_ICONS,

  IS_ANY=0xFFFFFFFF,IS_FILTER_T9KEY=0xFFFFFFFE
};























 





void Add2History(CLIST *CListEx, char *header, char *message, int direction);
void LogStatusChange(CLIST *CListEx);
int GetHistory(CLIST *t, int bufsize);
void do_StatusChangeWrite(char *text,CLIST *t, int delitel);





 


void InitConfig();
void SaveConfig(void);
extern const char *successed_config_filename;
int SaveConfigData(const char *fname);



void ShowMainMenu();









void SendAnswer(int dummy, TPKT *p);
void CreateEditChat(CLIST *t);
void set_my_status(void);
void set_my_xstatus(void);

void RequestXText(unsigned int uin);

void ReadDefSettings(void);
void WriteDefSettings(void);
extern int connect_state;

int GetIconIndex(CLIST *t);


void FreeLOGQ(LOGQ **pp);
void RecountMenu(CLIST *req, int needfocus);
void start_vibra();
void ClearContactT9Key(void);
void Get_Online_Contact();

void RedrawOnline();
void SetVisibleScreen();
char* GetColor(CLIST *t);

extern int CountXIcon();

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
  off_contacts:1,
  show_groups:1;
  char def_status;
  char def_xstatus;  
  unsigned char ActiveUp:1;
  
  unsigned char popup_state:1;
  unsigned char remind_state:1,
                ilu_mode:1,
                ilu_online:1,
                extsnd_mode:1,
                extsnd_flag:1,
                def_private_status:1;
}DEF_SETTINGS;
#pragma pack(pop)

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);


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


void FreeStatusToIMGHDR();
void ProcessNextImg(void);

S_SMILES *FindSmileById(int n);
S_SMILES *FindSmileByUni(int wchar);








typedef struct{
  unsigned long uin;
  char *msg;
}IPCMsg;

typedef struct
{
  unsigned long uin;
  char *msg;
  int drop_msg; 
  IPC_REQ *ipc; 
}IPCMsg_RECVMSG;





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
void log(char *fname, void *text);


int CreateManageCLMenu(void);
int CreateAddContactGrpDialog(CLIST *cl_sel);
int CreatePrivateStatusMenu(void);
void SetPrivateStatus(int status);



void FreeXText(CLIST *t);
void FreeStatusTime(CLIST *t);
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











extern char *get_rev();

void lgpLoadLangpack(void);
void lgpFreeLangPack(void);
void lgpUpdateClHdr(void);




enum LGP_ID
{
  LGP_AlreadyStarted=0,
  LGP_ClTitle,
  LGP_ClT9Inp,
  LGP_ClError,
  LGP_ClLoopback,
  LGP_GroupNotInList,

  
  LGP_Menu,
  LGP_Options,
  LGP_Close,
  LGP_Clear,
  LGP_DoIt,
  LGP_PasteSm,
  
  LGP_Select,
  LGP_Text,
  LGP_Back,
  
  LGP_Add,
  LGP_AddCnt,
  LGP_EnterUin,
  
  LGP_AddRen,
  
  
  LGP_AddSmil,
  
  LGP_AddCont,
  LGP_AuthReq,
  LGP_AuthGrant,
  
  LGP_MnuStatus,
  LGP_MnuXStatus,
  LGP_MnuAddCont,
  LGP_MnuAwayMsg,
  LGP_MnuVibra,
  LGP_MnuSound,
  
  LGP_MnuExtSnd,
  LGP_MnuIndSnd,
  LGP_MnuIluMode,
  LGP_MnuIluOnlineMode,
  
  LGP_MnuReminder,
  LGP_MnuPopup,
  LGP_MnuShowOff,
  LGP_MnuShowGroup,
  LGP_MnuActiveUp,
  LGP_MnuEdCfg,
  LGP_MnuDisconnect,
  LGP_MnuPing,
  LGP_MnuAbout,
  
  LGP_MnuQuote,
  LGP_MnuQuoteNotFocus,
  LGP_MnuAddSml,
  LGP_MnuShInfo,
  LGP_MnuAddRen,
  LGP_MnuSAuthReq,
  LGP_MnuSAuthGrt,
  LGP_MnuOpenLog,
  LGP_MnuClearCht,
  
  LGP_MnuEdNextAct,
  LGP_MnuEdPrevAct,
  
  LGP_ChgStatus,
  LGP_ChgXStatus,
  
  LGP_StOffline,
  LGP_StOnline,
  LGP_StAway,
  LGP_StNa,
  LGP_StDnd,
  LGP_StOccup,
  LGP_StFfc,
  LGP_StInvis,
  LGP_StDepression,
  LGP_StEvil,
  LGP_StHome,
  LGP_StLunch,
  LGP_StWork,
  LGP_Comment,
  
  
  
  LGP_All_Can_See,
  LGP_Vislist_Can_See,
  LGP_Invislist_Cannot_See,
  LGP_ContactList_Can_See,
  LGP_Nobody_Can_See,
  
  LGP_EnterName,
  LGP_ChooseGroup,
  LGP_PrivStatus,
  LGP_ManageList,
  
  
  
  LGP_MsgIllegmsgcon,
  LGP_MsgIllegmsgrea,
  LGP_MsgCantConn,
  LGP_MsgCantCrSc,
  LGP_MsgHostNFnd,
  LGP_MsgVibraDis,
  LGP_MsgVibraEna,
  LGP_MsgSndDis,
  LGP_MsgSndEna,
  LGP_MsgNoUinPass,
  LGP_MsgSmileNotFnd,
  
  
  LGP_GrsKeyExit,
  LGP_GrsKeyClist,
  
  LGP_LangCode, 
  
  LGP_DATA_NUM
};


typedef struct LGP_Item LGP_Item;

struct LGP_Item
{
  char *tag;
  int   id;
};



extern LGP_Item LGPTAGS[];



extern char * lgpData[LGP_DATA_NUM];













 





 

void FreeAllList();
void LoadingImages();






extern const int LINE_MODE;
extern const int VIEW_MODE;
extern const unsigned int SOFT_MODE;


extern const int BlinkIconByOnline;
   
extern const unsigned int ShowProcessSend;
extern const int SelBoldActiveChat;
extern const char CLBackgrClr[4];

extern const char SoftBackgrClr[4];   
extern const char SoftFontClr[4];

extern const char HeadBackgrClr[4];   
extern const char HeadFontClr[4];
extern const char ScrollBarClr[4];

extern const char ClCursorClr[4];
extern const char ClCursorBordClr[4];
extern const char ActiveClr[4];
extern const char NotActiveClr[4];
extern const char GroupClr[4];
extern const char CLDescriptClr[4];

extern const char UnAckedClr[4];
extern const char SendClr[4];
   
   
extern const char AddInfoClr[4];
extern const char FontInfoClr[4];

extern const unsigned int ClFont;
extern const unsigned int ClBoldFont;
extern const unsigned int HeadFont;
extern const unsigned int MMFont;


extern const unsigned int ON_OFF_TIME;
extern const int HEAD_STATUS;
extern const int HEAD_XSTATUS;
extern const int HEAD_PRIVATE;
extern const int HEAD_PING;
extern const int isShowCont;
extern const int isShowUnread;
extern const int isShowI;
extern const int isShowV;
extern const int isShowS;
extern const int isShowBat;
extern const int isShowA;
extern const int isShowVr;


extern const int ShowXStatusSecLine;
extern const char MMFontClr[4];
extern const char MMBackgrClr[4];
extern const char MMBackgrBordClr[4];
extern const char MMCursorBGClr[4];
extern const char MMCursorClr[4]; 
extern const unsigned int MainMenuAutoRect;     
extern const unsigned int MainMenuItems;
extern const RECT MMRect;
extern const RECT SRect;
extern const RECT XSRect;


extern void DrawSoftKeys(char *right,char *left);
extern void DrawContactList();
extern void DrawHeader();
extern IMGHDR DoScreen(int ScrH,int HSIZE);
extern unsigned int HeaderY;





extern const char CLColorSelCont[];
extern const int isIndividualColor;
extern const char CLColorsOnline[];         
extern const char CLColorsAway[];         
extern const char CLColorsNA[];
extern const char CLColorsOccupied[];
extern const char CLColorsDND[];
extern const char CLColorsDepression[];
extern const char CLColorsFFC[];
extern const char CLColorsOff[];


extern const int isLogStatusChange;



extern const int LongRightAction;
extern const int LongLeftAction;
extern const int ActionResh;
extern const int LongVolUpAction;
extern const int LongVolDownAction ;
extern const int isVolUpButton;
extern const int isVolDownButton;

extern const int longAct_0;
extern const int longAct_1;
extern const int longAct_2;
extern const int longAct_3;
extern const int longAct_4;
extern const int longAct_5;
extern const int longAct_6;
extern const int longAct_7;
extern const int longAct_8;
extern const int longAct_9;
extern const int longAct_10;
extern const int longAct_11;




extern const int Auto_Status;
extern const int IsOnNAStatus;
extern const unsigned int NAStatusTime;   
extern const int isAutoStatusOn;
extern const unsigned int AUTO_ENGADE;
extern const unsigned int automsgcount;
extern const int time_zone;
extern const char awayHelloMsg[24];
extern const char awayMsg[256];
extern const int showAwayIdle;  
extern const unsigned int auto_x;
extern const unsigned int auto_y;
extern const int Reset_timer;  






extern const int PopupShow;
extern const char PopupMsgString[9];
extern const char PopupOnlineString[9];
extern const unsigned int PopupFont;
extern const char PopupBGCl[4];
extern const char PopupOnlineCl[4];    
extern const char PopupMsgCl[4];

extern const char PopupOffBGCl[4];
extern const char PopupOffCl[4];
extern const int isPopupOff;



extern const char pingsnd[64];
extern const char sndLostConn[64];    

extern const unsigned int ADDITION_UIN;



extern char clrWhite[];
extern char clrRed[];
extern char transparent[];
extern char blue[];

extern int Is_Vibra_Enabled;
extern unsigned int Is_Sounds_Enabled; 
extern int Is_Show_Offline;
extern int Is_Show_Groups;
extern int IsActiveUp;
extern int Is_POPUP_On;
extern int Is_SLI_On;
extern int Is_Double_Vibra_On;
extern int Is_ILU_On;


extern int msg_away_timer;
extern void away_timer();
extern int away_m;
extern int change_back_status;
extern int status_rem;
extern GBSTMR away_tmr;


extern int DrwImage(IMGHDR *img, int x, int y,int rettype);

extern IMGHDR *GetIconByItem(int curitem);
extern IMGHDR *GetXIconByItem(int curitem);

extern int GetFontYSIZE(int font);
extern void DrawStr(char *str, int x, int y, int x2,int y2, int align, int font, char *color,int needcut);

extern int get_file_size(char* fname);


void aa(TPKT *);
void ab(int, int);
void ad(TPKT *);
void ae(TPKT *);


extern volatile int total_smiles;
extern volatile int total_xstatuses;
extern volatile int xstatuses_load;
extern volatile int pictures_max;
extern volatile int pictures_loaded;

extern const unsigned int IllByONCantact;
extern const int DEVELOP_IF;
    




const char ipc_my_name[32]= "NatICQ";
const char ipc_xtask_name[]= "XTask";
IPC_REQ gipc;

volatile CLIST *cltop;
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

int IsActiveUp = 0;
volatile int vibra_count;

int Is_Double_Vibra_On = 0;
int Is_Vibra_Enabled = 0;
unsigned int Is_Sounds_Enabled = 0;
int Is_Show_Offline = 0;
int Is_Show_Groups = 0;
int Is_POPUP_On = 1;
int Is_SLI_On = 1;
int Is_ILU_By_Online = 1;
int Is_Extd_Sound = 0;

int S_ICONS[TOTAL_ICONS+1];

int CurrentStatus;
int CurrentXStatus;

int total_unread;
int total_unread_all;

char last_msg_buffer[4096];
extern const int IsBufferMenu;
WSHDR *wsBuffer;    

int develop_by_unlock = 0;
int ShowLoadScreen=1;

static GUIMENU *clistm;

char on_off_buf[128]; 
int on_off_timer = 0; 






 
int ClrOnOffBuffer()
{
  int retval = 0;
  if(on_off_timer > 0)
    {
      on_off_timer = 0;
      zeromem(on_off_buf,sizeof(on_off_buf));             
      retval = 1;
    }
  return retval;
}


int online_contact;
int total_contact;

int isonline;
void *canvasdata;

int DoRefreshCL=0;
void RefreshCL()
{
  DoRefreshCL=1; 
}


int CountMessage[2];
int blocked_spam = 0;

int LongPressTimer=0;

char prmsg[256];
int PrintTimer=0;









 
void PrintMessage(char *s,int sec)
{
  strcpy(prmsg,s);
  PrintTimer = sec;
}






 
int ClrPrintMess()
{
  int retval=0;
  if(PrintTimer > 0 && strlen(prmsg))
  {
    PrintTimer=0;
    LongPressTimer=0;
    zeromem(prmsg,256);
    retval=1;
  }
  return retval;
}

WSHDR *ews;

extern const unsigned int UIN;
extern const char PASS[];


const char *icons_names[TOTAL_ICONS]=
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
  "vis5.png",
  "check.png",
  "uncheck.png",
  "bg.png"
};

extern const char ICON_PATH[];


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

extern const unsigned int IDLEICON_X;
extern const unsigned int IDLEICON_Y;

extern const unsigned int I_COLOR;
extern const unsigned int TO_COLOR;
extern const unsigned int X_COLOR;
extern const unsigned int O_I_COLOR;
extern const unsigned int O_TO_COLOR;
extern const unsigned int O_X_COLOR;

extern const unsigned int ED_FONT_SIZE;
extern const unsigned int ED_H_FONT_SIZE;
extern const unsigned int ED_X_FONT_SIZE;
extern const unsigned int O_ED_FONT_SIZE;
extern const unsigned int O_ED_H_FONT_SIZE;
extern const unsigned int O_ED_X_FONT_SIZE;

extern const unsigned int ACK_COLOR;
extern const unsigned int UNACK_COLOR;

extern const int ENA_AUTO_XTXT;
extern const int NOT_LOG_SAME_XTXT;
extern const int LOG_XTXT;

extern const int HISTORY_BUFFER;

const char percent_t[]="%t";
const char percent_d[]="%d";
const char empty_str[]="";
extern const char I_str[];
const char x_status_change[]="X-Status change";

char logmsg[256];



extern const unsigned int ILL_DISP_RECV;
extern const unsigned int ILL_KEYS_RECV;
extern const unsigned int ILL_DISP_SEND;
extern const unsigned int ILL_KEYS_SEND;
extern const unsigned int ILL_SEND_TMR;
extern const unsigned int ILL_SEND_FADE;
extern const unsigned int ILL_RECV_TMR;
extern const unsigned int ILL_RECV_FADE;
extern const unsigned int ILL_OFF_FADE;
int Is_ILU_Mode;

GBSTMR tmr_illumination;

void IlluminationOff(){
  SetIllumination(0,1,0,ILL_OFF_FADE);
  SetIllumination(1,1,0,ILL_OFF_FADE);
}

void IlluminationOn(const int disp, const int key, const int tmr, const int fade){
  if(!tmr) return;
  if(!Is_ILU_Mode)return;
  GBS_DelTimer(&tmr_illumination);
  SetIllumination(0,1,disp,fade);
  SetIllumination(1,1,key,fade);
  GBS_StartTimerProc(&tmr_illumination,tmr*216,IlluminationOff);
}

volatile int silenthide;    
volatile int disautorecconect;	

int Is_Vibra_Enabled;
unsigned int Is_Sounds_Enabled;
int Is_Show_Offline;
int Is_Show_Groups;
int CurrentStatus;
int CurrentXStatus;
int CurrentPrivateStatus;



  
int msg_away_timer=0; 
char away_msg[256]; 
char away_msg_time[64]; 
char away_msg_time_dace[16];  
int away_m; 
int status_rem; 
int change_back_status; 
GBSTMR away_tmr;














































































































 






 

void away_timer()
{
  TTime tt; 
  if(msg_away_timer==(AUTO_ENGADE*12))
  {
    GetDateTime(0,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "Отсутствую примерно с %02d:%02d",tt.hour, tt.min );
    away_m=1;
    
    if(CurrentStatus!=2 && CurrentStatus!=isAutoStatusOn)
    {
      status_rem=CurrentStatus;
      CurrentStatus=2;
      set_my_status();
      change_back_status=1;
    }    
  }
  
  if( (IsOnNAStatus) && (msg_away_timer==(AUTO_ENGADE*12)+(NAStatusTime*12)) )
  {
    GetDateTime(0,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "Недоступен примерно с %02d:%02d",tt.hour, tt.min );
    if(CurrentStatus != 3)
    {
      CurrentStatus = 3;
      set_my_status();
    }
  }
  
  if(Auto_Status && connect_state == 3)  msg_away_timer++;
  GBS_StartTimerProc(&away_tmr, 216*5, away_timer);
}






GBSTMR remind_tmr;
void Reminder_timer();
int double_on;






 

void Remind()
{
  double_on = 0;
  if(total_unread && Is_Double_Vibra_On)
  {
    vibra_count = 2;
    double_on = 1; 
    start_vibra();
    GBS_StartTimerProc(&remind_tmr, 216*30, Remind);
  }
}

void Reminder_timer()
{
  double_on = 0;
  GBS_StartTimerProc(&remind_tmr, 216*30, Remind);  
}




TTime uptime;






 
void UpTime()
{
  if(connect_state==3)
  {
    uptime.sec++;
    if(uptime.sec==60)
    {
      uptime.sec=0;
      uptime.min++;
    }
    if(uptime.min==60)
    {
      uptime.min=0;
      uptime.hour++;
    }
  }
}


char ContactT9Key[32];








 
int LongPressActionOnChat(CLIST *t,int i)
{
   switch(i)
    {
    case 1:
        Is_Sounds_Enabled=!(Is_Sounds_Enabled);
        PrintMessage(Is_Sounds_Enabled?"Sound 0n":"Sound Off",3);
        SUBPROC((void*)WriteDefSettings);
        break;
    case 2:
        Is_Vibra_Enabled=!(Is_Vibra_Enabled);
        SUBPROC((void*)WriteDefSettings);
        PrintMessage(Is_Vibra_Enabled?"Vibra On":"Vibra Off",3);
        break;
    case 3:        
         if(CurrentPrivateStatus!= 2)
         {
           SetPrivateStatus(2 );
           PrintMessage("Инвиз включен",2);
         }
         else
         {
           SetPrivateStatus(3);
           PrintMessage("Инвиз выключен",2);
         }
         break;
    case 4:
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        PrintMessage(Is_Double_Vibra_On?"Vibra rem On":"Vibra rem Off",3);
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
        PrintMessage("All chat clear!",2);
        break;
      }
    case 6:
      if(!away_m)
      {
        away_m=!away_m;
        GBS_StopTimer(&away_tmr);
        msg_away_timer=AUTO_ENGADE;
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
      break;
    case 7:
      {
        WSHDR *ws;
        ws=AllocWS(150);
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
      break;
    case 8:
      {
        TPKT *p;
        p=malloc(sizeof(PKT)+sizeof(TTime));
        GetDateTime(0,(TTime *)p->data);
        p->pkt.uin=0;
        p->pkt.type=18;
        p->pkt.data_len=sizeof(TTime);
        SUBPROC((void *)SendAnswer,0,p);
      }
      break;
    case 9:
      {
        ClearContactT9Key();
        RecountMenu(0,1);
        silenthide=1;
        gipc.name_to=ipc_xtask_name;
        gipc.name_from=ipc_my_name;
        gipc.data=0;
        GBS_SendMessage(0x4209,0x3F2,1,&gipc);    
      }
      break;
    case 10:
      {        
        if(DEVELOP_IF == 3) develop_by_unlock = 1;
        
        ClearContactT9Key();
        RecountMenu(0,1);
        gipc.name_to=ipc_xtask_name;
        gipc.name_from=ipc_my_name;
        gipc.data=0;
        GBS_SendMessage(0x4209,0x3F2,1,&gipc);
        if (IsUnlocked())
        {
          KbdLock();
        }
      }
      break;
    case 11: 
      {
        extern void ChangeShowOfflineMode(void);
        ChangeShowOfflineMode();
        PrintMessage(Is_Show_Offline?"Оффлайн контакты\nвключены":"Оффлайн контакты\nотключены",1);
      }
      break;
    case 12: 
      {
        extern void ChangeShowGroupsMode(void);
        ChangeShowGroupsMode();
        PrintMessage(Is_Show_Groups?"Группы включены":"Группы отключены",1);
      }
      break;
    case 13: 
      {
        int FindMessage(CLIST* );
        LongPressTimer=0;
        return FindMessage(t);        
      }
      
    default : LongPressTimer=0; break;
    }
   return 0;
}










 
void LongPressAction(CLIST *t,int key)
{
  LongPressTimer = 2;
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
    default : LongPressTimer=0;break;
  }
}

















 
int DrwImage(IMGHDR *img, int x, int y,int rettype)
{
  if(!img) return 0;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0);
  DrawObject(&drwobj);
  return ( rettype==1 ? ((img->w > 32) ? 32 : img->w )  : ((img->h > ScreenH()-1) ? 1 : img->h) );
}






typedef struct
{
  char name[64];
  int timer;
  GBSTMR tmr;

  
  IMGHDR *icon;
  IMGHDR *xicon;
  int xiconf;
}POPUP;



POPUP popup[3];

int FIRST_START = 0;









 
int IsNoJavaRun()
{
  
  int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
  char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
  if((memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8))) return 1;
 return 0;
}







 
void show_on_contact()
{
  if(FIRST_START)
  {
    popup[0].timer=0; 
    return;
  }

  if(popup[0].timer > 0)
  {


 
    if(IsNoJavaRun())
    {      
      WSHDR *ws=AllocWS(64);
      ascii2ws(ws,popup[0].name);
      popup[0].timer--;
      
      int x=0;
      if(popup[0].icon) x = popup[0].icon->w;
      if(popup[0].xicon && popup[0].xiconf) x += popup[0].xicon->w;
      DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+x+5,GetFontYSIZE(PopupFont)+5,0,PopupOnlineCl,PopupBGCl);
      if(popup[0].icon)
      {
        DrwImage(popup[0].icon,2,3,0);
        if(popup[0].xiconf) DrwImage(popup[0].xicon,4 + popup[0].icon->w,3,0);
      }
      DrawString(ws,x+3,2, 3+Get_WS_width(ws,PopupFont)+x ,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupOnlineCl,GetPaletteAdrByColorIndex(23));
      FreeWS(ws);
    }
   GBS_StartTimerProc(&popup[0].tmr, 15, show_on_contact);    
  }
}






 
void show_off_contact()
{
  if(!isPopupOff) return;
  if(FIRST_START)
  {
    popup[1].timer=0; 
    return;
  }
  
  if(popup[1].timer>0 )
  {


 
    if(IsNoJavaRun())
    {
      WSHDR *ws=AllocWS(64);
      ascii2ws(ws,popup[1].name);
      popup[1].timer--;  
      DrawRectangle(ScreenW()-Get_WS_width(ws,PopupFont)-4-popup[1].icon->w,0,ScreenW()-1,GetFontYSIZE(PopupFont)+4,0,PopupOffCl,PopupOffBGCl);
      DrwImage(popup[1].icon,ScreenW()-Get_WS_width(ws,PopupFont)-2-popup[1].icon->w,3,0);
      DrawString(ws,ScreenW()-Get_WS_width(ws,PopupFont)-3,2,ScreenW()-1,GetFontYSIZE(PopupFont)+2,PopupFont,4,PopupOffCl,GetPaletteAdrByColorIndex(23));
      FreeWS(ws);
    }
   GBS_StartTimerProc(&popup[1].tmr, 15, show_off_contact);    
  }
}










 
void show_pp_sender()
{
  if(FIRST_START)
  {
    popup[2].timer=0; 
    return;
  }
 
  if(popup[2].timer > 0 )
  {


 
    if(IsNoJavaRun())
    {
      WSHDR *ws=AllocWS(256);
      ascii2ws(ws,popup[2].name);
      popup[2].timer--;
      popup[2].icon = GetIconByItem(IS_MSG);
      DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+6+popup[2].icon->w,GetFontYSIZE(PopupFont)+4,0,PopupMsgCl,PopupBGCl);
      DrwImage(popup[2].icon,2,2,0);
      DrawString(ws,popup[2].icon->w+4,1,popup[2].icon->w+Get_WS_width(ws,PopupFont)+4,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupMsgCl,GetPaletteAdrByColorIndex(23));
      FreeWS(ws);
    }
    GBS_StartTimerProc(&popup[2].tmr, 15, show_pp_sender);    
  }
}



const char def_setting[]="%sdef_settings_%d";
int ExtSndModeIndivd;

void ReadDefSettings(void)
{
  DEF_SETTINGS def_set;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if(get_file_size(str) != sizeof(DEF_SETTINGS)) unlink(str,&err);
  if ((f=fopen(str,0+0x8000,0x80,&err))!=-1)
  {
    fread(f,&def_set,sizeof(DEF_SETTINGS),&err);
    fclose(f,&err);
    Is_Vibra_Enabled     = def_set.vibra_status;
    Is_Sounds_Enabled    = def_set.sound_status;
    Is_Show_Offline      = def_set.off_contacts;
    Is_Show_Groups       = def_set.show_groups;
    CurrentStatus        = def_set.def_status + 1;
    CurrentXStatus       = def_set.def_xstatus;
    IsActiveUp           = def_set.ActiveUp;
    Is_POPUP_On          = def_set.popup_state;
    Is_Double_Vibra_On   = def_set.remind_state;
    Is_ILU_Mode          = def_set.ilu_mode;
    Is_ILU_By_Online     = def_set.ilu_online;
    Is_Extd_Sound        = def_set.extsnd_mode;
    ExtSndModeIndivd     = def_set.extsnd_flag;
    CurrentPrivateStatus = def_set.def_private_status;
  }
  else
  {
    Is_Vibra_Enabled     = 1;
    Is_Sounds_Enabled    = 0;
    Is_Show_Offline      = 0;
    Is_Show_Groups       = 0;
    CurrentStatus        = IS_ONLINE;
    CurrentXStatus       = 0;
    CurrentPrivateStatus = 2;
    
    IsActiveUp = 1;
        
    Is_POPUP_On        = 1;
    Is_Double_Vibra_On = 0;
    Is_ILU_Mode        = 1;
    Is_ILU_By_Online   = 1;
    Is_Extd_Sound      = 0;
    ExtSndModeIndivd   = 0;
    
  }
}

void WriteDefSettings(void)
{
  DEF_SETTINGS def_set;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if ((f=fopen(str,1+0x8000+0x100+0x200,0x100,&err))!=-1)
  {
    def_set.vibra_status=Is_Vibra_Enabled;
    def_set.sound_status=Is_Sounds_Enabled;
    def_set.off_contacts=Is_Show_Offline;
    def_set.show_groups=Is_Show_Groups;
    def_set.def_status=CurrentStatus-1;
    def_set.def_xstatus=CurrentXStatus;
    def_set.ActiveUp = IsActiveUp;
    def_set.popup_state=Is_POPUP_On;
    def_set.remind_state=Is_Double_Vibra_On;
    def_set.ilu_mode = Is_ILU_Mode;
    def_set.ilu_online = Is_ILU_By_Online;
    def_set.extsnd_mode = Is_Extd_Sound;
    def_set.extsnd_flag = ExtSndModeIndivd;
    def_set.def_private_status = CurrentPrivateStatus;
    fwrite(f,&def_set,sizeof(DEF_SETTINGS),&err);
    fclose(f,&err);
  }
}


extern S_SMILES *s_top;
extern DYNPNGICONLIST *SmilesImgList;
extern DYNPNGICONLIST *XStatusesImgList;


extern const char sndStartup[];
extern const char sndSrvMsg[];
extern const char sndGlobal[];
extern const char sndMsg[];
extern const char sndMsgSent[];
extern const unsigned int sndVolume;


int ExtSndMode_flag=0;






 
char *GetExtSnd(const char *extsnd)
{
  char *mp3,*wav,*amr;
  mp3 = MakeGlobalString(extsnd,'.',"mp3");
  wav = MakeGlobalString(extsnd,'.',"wav");
  amr = MakeGlobalString(extsnd,'.',"amr");
  if(get_file_size(mp3)) return mp3;
  else
    if(get_file_size(wav)) return wav;
  else
    if(get_file_size(amr)) return amr;
  return 0;
}

void Play(const char *fname)
{
  if ((!IsCalling()) && ( Is_Sounds_Enabled || ExtSndMode_flag))
  {
    ExtSndMode_flag = 0;
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
      _sfo1.unk4=0x80000000;
      _sfo1.unk5=1;
      PlayFile(0xC, sndPath, sndFName, 0,GBS_GetCurCepid(), 0x174, &_sfo1);
      FreeWS(sndPath);
      FreeWS(sndFName);
    }
  }
}

GBSTMR tmr_vibra;

void start_vibra(void)
{
  extern const int VIBR_TYPE;
  void stop_vibra(void);
  if((Is_Vibra_Enabled || (Is_Double_Vibra_On && double_on))&&(!IsCalling()))
  {
    extern const unsigned int vibraPower;
    SetVibration(vibraPower);
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,216>>2,stop_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,216>>1,stop_vibra);
  }
}

void stop_vibra(void)
{
  extern const int VIBR_TYPE;
  SetVibration(0);
  if (--vibra_count)
  {
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,216/40,start_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,216>>1,start_vibra);
  }
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

extern const char TEMPLATES_PATH[];

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
  ((HEADER_DESC*)head)->rc.y=(0);
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+(0)-1;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+(0);
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}



volatile unsigned int GROUP_CACHE; 

volatile int contactlist_menu_id;

GBSTMR tmr_active;

volatile int edchat_id;






char clm_hdr_text[48];
static char def_clm_hdr_text[32] = "";
static char key_clm_hdr_text[32] = "";

char clmenu_sk_r[16];
static char def_clmenu_sk_r[32] = "";
static char key_clmenu_sk_r[32] = "";

static const int menusoftkeys[] = {0,1,2};

static SOFTKEY_DESC menu_sk[] =
{
  {0x0018, 0x0000, 0 },
  {0x0001, 0x0000, 0 },
  {0x003D, 0x0000, (int)0x7FFFC0FB}
};

void lgpUpdateClHdr()
{
  strcpy(def_clm_hdr_text, (char*) lgpData[LGP_ClTitle]);
  strcpy(key_clm_hdr_text, (char*) lgpData[LGP_ClT9Inp]);
  strcpy(def_clmenu_sk_r,  (char*) lgpData[LGP_Close]);
  strcpy(key_clmenu_sk_r,  (char*) lgpData[LGP_Clear]);
}

const SOFTKEYSTAB menu_skt =
{
  menu_sk, 0
};




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






int DNR_ID=0;
int DNR_TRIES=3;

extern const char NATICQ_HOST[];
extern const unsigned int NATICQ_PORT;
extern const unsigned int RECONNECT_TIME;
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
  char hostbuf[128];
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

  sprintf(hostname, "%s:%d", hostbuf, hostport);

  SMART_REDRAW();

  ip=str2ip(hostbuf);
  if (ip!=0xFFFFFFFF)
  {
    sa.ip=ip;
    strcpy(logmsg,"Connect by IP!");
    SMART_REDRAW();
    goto L_CONNECT;
  }
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
      strcpy(logmsg,"DNR Ok, connecting...");
      SMART_REDRAW();
      DNR_TRIES=0;
      sa.ip=p_res[3][0][0];
    L_CONNECT:
      sprintf(hostname, "%s:%d\n(IP: %d.%d.%d.%d)", hostbuf, hostport,
              sa.ip&0xFF, (sa.ip>>8)&0xFF, (sa.ip>>16)&0xFF, (sa.ip>>24)&0xFF);

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
	  ShowMSG(1,(int)lgpData[LGP_MsgCantConn]);
	  UnlockSched();
	  GBS_StartTimerProc(&reconnect_tmr,216*RECONNECT_TIME,do_reconnect);
	}
      }
      else
      {
	LockSched();
	ShowMSG(1,(int)lgpData[LGP_MsgCantCrSc]);
	UnlockSched();
	
	GPRS_OnOff(0,1);
      }
    }
  }
  else
  {
    DNR_TRIES--;
    LockSched();
    ShowMSG(1,(int)lgpData[LGP_MsgHostNFnd]);
    UnlockSched();
  }
}

GBSTMR send_tmr;

void ClearSendQ(void)
{
  mfree((void *)sendq_p);
  sendq_p=0;
  sendq_l=0;
  SMART_REDRAW();
  GBS_DelTimer(&send_tmr);
}













































 
void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
  }
  

      Play(sndLostConn);
  GBS_DelTimer(&send_tmr);
}

static void resend(void)
{
  void SendAnswer(int dummy, TPKT *p);
  SUBPROC((void*)SendAnswer,0,0);
}
























 

char *get_rev()
{
  char rev[9] = "";
  char txt[4];
  int f;
  unsigned int err;
  char fn[64];


   strcpy(fn,TEMPLATES_PATH);
   strcat(fn,"\\rev.txt");
   f = fopen(fn,0 + 0x8000,0x80, &err );
     
   if( f != -1)
   {
     fread(f, txt, 4, &err);
     snprintf(rev,8,"Sie_%s",txt);
   }
   else
     snprintf(rev,8,"Sie_3689",txt);
   
   fclose(f,&err);
   rev[8] = 0;
   return rev;
}


void SendAnswer(int dummy, TPKT *p)
{
  if(p->pkt.type==32)
  {
    char rev[9]="Sie_3680";
    memcpy(p->data,rev,8);
  } 
  
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
      
      GBS_StartTimerProc(&send_tmr,216*5,resend);
      return; 
    }
    tenseconds_to_ping=0; 
  }
  mfree((void *)sendq_p);
  sendq_p=0;
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

void get_answer(void)
{
  void *p;
  int i=RXstate;
  int j;
  int n;
  char rb[7168];
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
         aa(&RXbuf);
	switch(RXbuf.pkt.type)
	{
        
        
        
     
















 
	case 4:
	  
	  
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  GBS_SendMessage(0x4209,0x3F0,0,p,sock);
	  Play(sndStartup);
	  
	  snprintf(logmsg,255,"T_LOGIN %s",RXbuf.data);
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
	  GBS_SendMessage(0x4209,0x3F0,0,p,sock);
	  
          if(RXbuf.pkt.type == 25)
            snprintf(logmsg,255,"T_XTEXT_ACK %d %s",n,RXbuf.data);
          if(RXbuf.pkt.type == 20)
            snprintf(logmsg,255,"T_GROUPID %d %s",n,RXbuf.data);
          
          if(RXbuf.pkt.type == 21)
            snprintf(logmsg,255,"T_GROUPFOLLOW %d %s",n,RXbuf.data);
          
          if(RXbuf.pkt.type == 7)
            snprintf(logmsg,255,"T_CLENTRY %d %s",n,RXbuf.data);
          
          if(RXbuf.pkt.type == 36)
            snprintf(logmsg,255,"PickOFF xD %d %s",n,RXbuf.data);
	  break;
	case 9:
        case 37:
	  n=i+sizeof(PKT);
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  snprintf(logmsg,255,"SC%d: %04X",RXbuf.pkt.uin,*((unsigned short *)(RXbuf.data)));
	  GBS_SendMessage(0x4209,0x3F0,0,p,sock);
	  break;
	case 6:
          if(strstr(RXbuf.data, "Error code 1") || strstr(RXbuf.data, "110"))
          {
            snprintf(logmsg,255,"ERR: %s","Номером зашли с другого клиента");
            disautorecconect = 1;
          }
          else
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
	  snprintf(logmsg,255,"MSG%d: %s",RXbuf.pkt.uin,RXbuf.data);
	  SendMSGACK(TOTALRECEIVED);
	  GBS_SendMessage(0x4209,0x3F0,0,p,sock);
          
          void GotoFirstPosition();
          GotoFirstPosition();
	  SMART_REDRAW();
          if(Is_Extd_Sound)
          {
            char extsnd[128];
            zeromem(extsnd,128);
            strcpy(extsnd,sndMsg);
            for(int i=strlen(extsnd);extsnd[i]!='\\';i--) extsnd[i]=0;
            sprintf((char*)(extsnd+strlen(extsnd)),"msg\\%u",RXbuf.pkt.uin); 
  
            char *p = GetExtSnd(extsnd);
            if(p)
              Play(p);
            else
              Play(sndMsg);
            
            if(p) mfree(p);
          }
          else
            Play(sndMsg);
	  break;
	case 13:
	  LockSched();
	  ShowMSG(1,(int)RXbuf.data);
	  UnlockSched();
	  break;
	case 16:
	  if (FindContactLOGQByAck(&RXbuf))
          {
            RefreshCL();
            SMART_REDRAW();
            Play(sndMsgSent);          
          }
	case 17:
	  p=malloc(sizeof(PKT)+2);
	  memcpy(p,&RXbuf,sizeof(PKT)+2);
	  GBS_SendMessage(0x4209,0x3F0,0,p,sock);
           snprintf(logmsg,255,"T_CLIENT_ACK %d %s",sizeof(PKT)+2,RXbuf.data);
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
            if(IsGuiOnTop(maingui_id))
              PrintMessage(logmsg,5);
            else
            {
	      LockSched();            
	      ShowMSG(1,(int)logmsg);
	      UnlockSched();
            }
            Play(pingsnd);
	  }
	  break;
        case 31:
          n=i+sizeof(PKT);
          p=malloc(n);
          memcpy(p,&RXbuf,n);
	  GBS_SendMessage(0x4209,0x3F0,0,p,sock);
          
    snprintf(logmsg,255,"T_LASTPRIVACY %d %s",n,RXbuf.data);
	  break;
	}
          












 
         ad(&RXbuf);
	i=-(int)sizeof(PKT); 
      }
    }
  }
  RXstate=i;
  
  
}



int NEW_MSG;

void AddStringToLog(CLIST *t, int code, char *s, const char *name, unsigned int IDforACK,int SaveXST2History)
{
  char hs[128], *lastX;
  TTime tt;
  TDate d;
  GetDateTime(&d,&tt);
  int i;

  if ((code==3 || code==4) && NOT_LOG_SAME_XTXT)
  {
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    lastX = GetLastXTextLOGQ(t);
    if(lastX)
      if(strcmp(lastX, s) == 0) return;
  }

  snprintf(hs,127,"%02d:%02d:%02d %02d.%02d.%02d %s:\r\n",
           tt.hour,tt.min,tt.sec,d.day,d.month,d.year,name
             );
  
  if((code != 3 && code!=4) || ((LOG_XTXT) && (SaveXST2History)))
    Add2History(t, hs, s, code); 
  LOGQ *p=NewLOGQ(s);
  snprintf(p->hdr,79,"[%d]%02d:%02d:%02d %02d.%02d %s:",
           ( (strcmp(name,I_str)==0) ? t->send : t->recv ),
           tt.hour,tt.min,tt.sec,d.day,d.month,name);
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
  if (code==3 || code==4)
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
    total_unread_all++;
    NEW_MSG=0;
  }
  
  t->isunread=1;
L_NOINC:
  ChangeContactPos(t);
}

void ParseAnswer(WSHDR *ws, const char *s);

int time_to_stop_t9;

void ParseXStatusText(WSHDR *ws, const char *s, int color,int xstate)
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
            color = (type==1)?O_I_COLOR:((type==3 || type==4)?O_X_COLOR:O_TO_COLOR);
            font = (type==3 || type==4)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
          }
          else
          {
            color = (type==1)?I_COLOR:((type==3 || type==4)?X_COLOR:TO_COLOR);
            font = (type==3 || type==4)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
          }
        }
        else
	  color=UNACK_COLOR; 
      }
      PrepareEditControl(&ec);
      if ((p->type&0x0F)!=3 && (p->type&0x0F)!=4)
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
      if ((p->type&0x0F)!=3 && (p->type&0x0F)!=4)
      {
        ParseAnswer(ews,p->text);
      }
      else
      {
        ParseXStatusText(ews, p->text, (p->type&0x10)?O_X_COLOR:X_COLOR, ed_struct->ed_contact->xstate);
      }
      PrepareEditControl(&ec);
      ConstructEditControl(&ec,
                           3,
                           ews->wsbody[0] ? 0x40|0x2000 : 0x20,
                           ews,ews->wsbody[0]);
      PrepareEditCOptions(&ec_options);

      if ((p->type&0x0F)!=3 && (p->type&0x0F)!=4)
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
    total_unread--;
  else
    ed_struct->requested_decrement_total_unread++;
  
  ed_struct->ed_contact->isunread=0;

  total_unread_all -= ed_struct->ed_contact->unread_msg;
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

void set_my_xstatus(void)
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

void to_develop(void)
{
  if (silenthide) return;
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)maincsm_id;
  GBS_SendMessage(0x4209,0x3F2,2,&gipc);
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























 






 
const char *GetStatusByIconIndex(int icon)
{  
  switch(icon)
   {
    case IS_ONLINE :     return lgpData[LGP_StOnline];
    case IS_OFFLINE :    return lgpData[LGP_StOffline];
    case IS_INVISIBLE :  return lgpData[LGP_StInvis];
    case IS_AWAY :       return lgpData[LGP_StAway];
    case IS_NA :         return lgpData[LGP_StNa];
    case IS_OCCUPIED:    return lgpData[LGP_StOccup];
    case IS_DND :        return lgpData[LGP_StDnd];
    case IS_DEPRESSION : return lgpData[LGP_StDepression];
    case IS_EVIL :       return lgpData[LGP_StEvil];
    case IS_HOME :       return lgpData[LGP_StHome];
    case IS_LUNCH :      return lgpData[LGP_StLunch];
    case IS_WORK :       return lgpData[LGP_StWork];
    case IS_FFC :        return lgpData[LGP_StFfc];

   }
   return lgpData[LGP_StOffline];
}


int req_info_contact = 0; 
extern const int Send_Auto_Status;



int FindSubWord(char *buffer, char *subword)
{
  
 
  int len,i;
  i = 0;
  len = strlen(subword);
  while(buffer[i] != 0)
  {
    if(buffer[i] == subword[0])
       if(!memcmp(&buffer[i],&subword[0],len)) return(1);
    i++;
  }
  return(0);
}










 
int GetCLV(CLIST *t)
{
  unsigned int io_error;
  char path[128];
  sprintf(path,"%s\\%u.clv",TEMPLATES_PATH,UIN);
  int hFile = fopen(path,0,0x80, &io_error);
  char *buf = 0;
  char tmp[9];
  int retval = 0;
  const char _u[]="%u";
  if(hFile!=-1)
  {
    buf = malloc(10000);
    fread(hFile, buf, 9999, &io_error);
    
    sprintf(tmp, _u, t->uin);
    tmp[strlen(tmp)] = 0;
    int fuin = FindSubWord(buf,tmp);
    
    if(fuin > 0)
        retval = 1;
  }
  fclose(hFile, &io_error); 
  if(buf) mfree(buf);
  return retval;
}








 

int AutoAnswer(CLIST *t)
{
  extern const char exept_list[];
  int retval = 0;
 
  
  if(req_info_contact) { req_info_contact = 0; return 0; }

  if(Send_Auto_Status && away_m == 1 && ((t->automsgcount++) < automsgcount))
  {
      
      if(CurrentPrivateStatus == 2  && !GetCLV(t)) return 0;
      if(CurrentPrivateStatus == 1) return 0;

      retval = 1;
      TPKT *p;    
 
      int m = (msg_away_timer*5)/60;
      int sec = (msg_away_timer*5)%60;
      int h = m/60;
      
      
      char *hour_text = malloc(64);
      
      if(h>0) sprintf(hour_text,"%02d %s",h, (h == 1 ? "час" : ( (h > 1 && h < 5) ? "часа" : "часов" )) );
      else    strcpy(hour_text, empty_str);
      
      char *min_text = (m == 1 ? "минуту" :( (m > 1 && m < 5) ? "минуты" : "минут"));
      
      sprintf(away_msg, "Автоответчик: %s %s! %s ( %s [бездествую %s %02d %s %02d %s] )",
                         awayHelloMsg, t->name, awayMsg, away_msg_time, hour_text, m%60, min_text, sec, "секунд");
      
      p=malloc(sizeof(PKT)+strlen(away_msg)+1);
      p->pkt.uin=t->uin;
      p->pkt.type=2;
      p->pkt.data_len=strlen(away_msg);
      strcpy(p->data,away_msg);
      AddStringToLog(t,0x01,t->automsgcount < 1 ? away_msg : "Send autoaway text...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
  
      SUBPROC((void *)SendAnswer,0,p);
      mfree(hour_text);
  }
  return retval;
}



void ProcessPacket(TPKT *p)
{
  extern const int VIBR_TYPE, VIBR_ON_CONNECT;
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
	RecountMenu(t, 0);
      }
      else
      {
        RecountMenu(AddContact(p->pkt.uin,p->data, GROUP_CACHE, 0),0);
      }
    }
    else
    {
      if(VIBR_ON_CONNECT)
      {
        vibra_count=1;
        start_vibra();
      }
      FIRST_START = 1;
      GROUP_CACHE=0;
      
      if (ShowLoadScreen==0 && maingui_id)
      {
	
      }
      else
      {
        if(ShowLoadScreen) ShowLoadScreen=0;        
        SMART_REDRAW();  
      }
    }
    break;
  case 20:
    if (t=FindGroupByID(GROUP_CACHE=p->pkt.uin))
    {
      strncpy(t->name,p->data,63);
      ChangeContactPos(t);
      RecountMenu(t, 1);
    }
    else
    {
      RecountMenu(AddGroup(p->pkt.uin,p->data), 1);
    }
    break;
  case 21:
    GROUP_CACHE=p->pkt.uin;
    break;
  case 37:
    if (t=FindContactByUin(p->pkt.uin)){
      t->clientid=*(char *)p->data;
    }
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
      if(!ShowLoadScreen)
        oldt=FindContactByN(clistm->curpos);
      t->state=*((unsigned short *)(p->data));
      LogStatusChange(t);
      ChangeContactPos(t);
      RecountMenu(oldt, 1);

     
      
      GetDateTime(0,&t->time_struct);
      char *txt=malloc(64);
      sprintf(txt," %s: %02d:%02d:%02d",GetStatusByIconIndex(GetIconIndex(t)),t->time_struct.hour,t->time_struct.min,t->time_struct.sec);      
       
      if(isLogStatusChange && t->state != i)
      {

              do_StatusChangeWrite(txt, t, 0);
        
        FreeStatusTime(t);
        t->sttime=malloc(64);
        zeromem(t->sttime,sizeof(t->sttime));
        snprintf(t->sttime,64,txt);
        AddStringToLog(t,0x04,txt,"Status change",0xFFFFFFFF, 0);
        ReqAddMsgToChat(t);  
        
      }
       sprintf(on_off_buf,"[%s: %s]\n",txt+1,t->name);
       mfree(txt);

       if ((t->state!=0xFFFF)&&(i==0xFFFF))
       {
         {
            char extsnd_path[128];
            zeromem(extsnd_path,sizeof(extsnd_path));
            if(t->uin == ADDITION_UIN && ExtSndModeIndivd)
            {
                 ExtSndMode_flag = 1;
                 goto p_extsnd;
            }
            if(  (Is_Extd_Sound && Is_Sounds_Enabled))
            {
            p_extsnd:

              strcpy(extsnd_path,sndMsg);
              for( int i=strlen(extsnd_path) ; extsnd_path[i] != '\\' ; i-- ) extsnd_path[i] = 0;
              sprintf((char*)(extsnd_path+strlen(extsnd_path)),"online\\%u",t->uin);
              

              char *p = GetExtSnd(extsnd_path);
                
              if(p) Play(p);
              else  Play(sndGlobal);
                
              if(p) mfree(p);                       
            }
            else
              Play(sndGlobal);
         }
          
        if(Is_POPUP_On && (PopupShow==2 || PopupShow==3))
        {
          popup[0].timer = 20;

   
          popup[0].icon = GetIconByItem(GetIconIndex(t));
          popup[0].xiconf = t->xstate;
          popup[0].xicon = GetXIconByItem(t->xstate);
          sprintf(popup[0].name,PopupOnlineString,t->name);
          show_on_contact();
        }

       if(Is_ILU_By_Online)
         IlluminationOn(IllByONCantact,0,ILL_SEND_TMR,ILL_RECV_FADE);
       t->isonline = 2;
       isonline = 1;
       on_off_timer = ON_OFF_TIME;
      if(ShowLoadScreen == 0)
          RedrawOnline();
     }
      
      if ((i!=0xFFFF)&&(t->state==0xFFFF))
      {
        on_off_timer=ON_OFF_TIME;
        if(Is_POPUP_On && isPopupOff)
        {
          popup[1].icon = GetIconByItem(IS_OFFLINE);
          popup[1].timer = 20;
        
      
          
          sprintf(popup[1].name,PopupOnlineString,t->name);
          show_off_contact();        
        }  
        Play(sndSrvMsg);        
      }
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
    Get_Online_Contact();    
     if(ShowLoadScreen == 0) RefreshCL();
    break;
  case 3:
    NEW_MSG=1;    
    t=FindContactByUin(p->pkt.uin);
      
    if (!t)
    {
      
      
      if(strcmp(p->data,"Auth REQ: ") == 0)
      {
        blocked_spam++;
        char log_path[128];
        char log1[128];
        zeromem(log1,128);
        TDate date;
        TTime time;
        GetDateTime(&date,&time);
        strcpy(log_path,TEMPLATES_PATH);
        strcat(log_path,"\\antispam.log");
        sprintf(log1,"%02d:%02d:%02d %02d.%02d.%04d Auth REQ noname contact blocked =) (uin %d %s)\n",
                                    time.hour,time.min,time.sec, date.day, date.month,date.year, t->uin,t->name);
        log(log_path,log1);
        PrintMessage("Block spam", 15);
        return;
      }
      
      sprintf(s,percent_d,p->pkt.uin);
      t=AddContact(p->pkt.uin,s,GROUP_CACHE,0);
    }
    
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    t->isactive=360;
    
    CountMessage[0]++;
    t->recv++;
    
    
    if(VIBR_TYPE)
      vibra_count=2;
    else
      vibra_count=1;
    start_vibra();
    if(Is_Double_Vibra_On) Reminder_timer();

    IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); 

    if (t->name[0]=='#')
    {
      
      char *s=strchr(p->data,'>');
      
      if (s)
      {
	if ((s[1]==' ')&&((s-p->data)<40))
	{
	  *s=0; 
	  AddStringToLog(t,0x02,s+2,p->data,0xFFFFFFFF,1); 
	  goto L1;
	}
      }
    }

    AutoAnswer(t);
    {
      AddStringToLog(t,0x02,p->data,t->name,0xFFFFFFFF,1);
      if(Is_POPUP_On && (PopupShow==1 || PopupShow==3))
      {
        sprintf(popup[2].name,PopupMsgString,t->name);
        popup[2].timer = 20;
     
     
        show_pp_sender();
      }
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
    RecountMenu(t, 1);
    extern const int DEVELOP_IF;
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
    }
    RefreshCL();
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
	      else
              {
		DrawAck(data);
                if (p->pkt.type==16)
                  IlluminationOn(ILL_DISP_SEND,ILL_KEYS_SEND,ILL_SEND_TMR,ILL_RECV_FADE); 
                RefreshCL();
                SMART_REDRAW();
              }
	    }
	  }
	}
      }
    }
    RefreshCL();
    SMART_REDRAW();
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

      
      if(IsGuiOnTop(maingui_id) && strlen(p->data))
        PrintMessage(s,5);
      else
       if(!edchat_id && strlen(p->data))
       {
         ShowMSG(0,(int)s);
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
  GBS_SendMessage(0x4209,0x3F2,4,&tmr_gipc);
  GBS_StartTimerProc(&tmr_active,216*10,process_active_timer);
}








 




 


char clrBlack[]    = {0x00,0x00,0x00,0x64};
char clrWhite[]    = {0xFF,0xFF,0xFF,0x64};
char clrRed[]      = {0xFF,0x00,0x00,0x64};
char transparent[] = {0x00,0x00,0x00,0x00};
char blue[]        = "\x00\x00\xFF\x64";

const char _t[] = "%t";
const char _d[] = "%d";
const char _s[] = "%s";

unsigned int HeaderY;

int PNG_SOFTKEY=0;
int PNG_HEADER=0;  




 






void Get_Online_Contact()
{
  CLIST *t;
  online_contact=0;
  total_contact=0;
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (!t->isgroup)
    {
      total_contact++;
      if (t->state!=0xFFFF) online_contact++;
    }
  }
}











 
IMGHDR DoScreen(int ScrH,int ShiftY)
{
  IMGHDR screen = {0,0,8,""};
  int ScrW = ScreenW();
  char *ms = RamScreenBuffer();
  screen.w = ScrW;
  screen.h = ScrH;  
  screen.bitmap = malloc(ScrW*ScrH*2);
  ms += (ShiftY*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
  return screen;
}





void RecountMenu(CLIST *req, int needfocus)
{
  int i;
  int j;
  if(!needfocus) return;
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
  
    clistm->curpos=j;
  SMART_REDRAW();
}









 










 










 
int GetWidth(char *s, int font)
{
  WSHDR *ws=AllocWS(strlen(s));
  wsprintf(ws,_t,s);
  unsigned short *body=ws->wsbody;
  int len=body[0],width;
  if(len>0) width=1; else width=0;
  while(len>0)
  {
    width+=GetSymbolWidth(body[len],font);
    len--; 
  }
  FreeWS(ws);
  return (width+1);
}









 
void DrawWSStr(WSHDR *ws, int x, int y,int w,int h, int font,int flag,char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,w,h);
  SetPropTo_Obj1(&drwobj,&rc,0,ws,font,flag);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}










 










 
void DrawStr(char *str, int x, int y, int x2,int y2, int align, int font, char *color,int needcut)
{
  if(!str) return;
  if(!strlen(str)) return;
  WSHDR *ws = AllocWS(strlen(str));
  wsprintf(ws,_t,str);    

  if(needcut)
  {
    int len = GetWidth(str,font);
    if(len > x2-x)
    {
      int pos = ws->wsbody[0] + 1;
      while((len > x2-x) && pos)
      {
        CutWSTR(ws,pos);
        len = Get_WS_width(ws,font);
        pos--;
      }  
    }
  }
 

  
  DrawString(ws, x, y, x2, y2, font, align, color, 0);
  FreeWS(ws);
}





static volatile int scroll_disp;
static volatile int max_scroll_disp;
GBSTMR tmr_scroll;






 
static void scroll_timer_proc(void)
{
  int i=max_scroll_disp;
  if (i)
  {
    if (scroll_disp>=i)
    {
      scroll_disp=0;
      GBS_StartTimerProc(&tmr_scroll,216,scroll_timer_proc);
    }
    else
    {
      scroll_disp+=3;
      GBS_StartTimerProc(&tmr_scroll,scroll_disp<i ? 216 : 216*2,scroll_timer_proc);
    }

  }
}

static void DisableScroll(void)
{
  GBS_DelTimer(&tmr_scroll);
  max_scroll_disp=0;
  scroll_disp=0;
}













 
void DrawScrollStr(char *s,int x,int y,int x2,int y2,int font,char *color)
{
  WSHDR *ws = AllocWS(strlen(s));
  wsprintf(ws,_t,s);
  DrawScrollString(ws, x, y, x2, y2, scroll_disp + 1, font, 0x80, color, 0);
  FreeWS(ws);
}

















 






 
char *GetLongPressText()
{
  char *text[]={"Без действия","Звук","Вибра","Инвиз","Реж.напоминания","Очистить чаты","Автоответчик","Конфиг","Пинг","Заблокировать","На гэ","Офф. контакты","Группы"};  
  char *ret = malloc(32);
  zeromem(ret,32);

  switch(ContactT9Key[0])
  {
    case '0': strncpy(ret,text[longAct_0],32); break;    
    case '1': strncpy(ret,text[longAct_1],32); break;    
    case '2': strncpy(ret,text[longAct_2],32); break;    
    case '3': strncpy(ret,text[longAct_3],32); break;    
    case '4': strncpy(ret,text[longAct_4],32); break;    
    case '5': strncpy(ret,text[longAct_5],32); break;    
    case '6': strncpy(ret,text[longAct_6],32); break;    
    case '7': strncpy(ret,text[longAct_7],32); break;    
    case '8': strncpy(ret,text[longAct_8],32); break;    
    case '9': strncpy(ret,text[longAct_9],32); break;    
    case '*': strncpy(ret,text[longAct_10],32);  break;
    case '#': strncpy(ret,text[longAct_11],32);  break;
    default : strncpy(ret,"error",32);  break;
  }
  
  return ret;
}

IMGHDR SoftShot={0,0,8,""};
int shot_soft=0;








 
void DrawSoftKeys(char *left,char *right)
{
  TTime t;  
  char *s=malloc(32);
  
  int FntH = GetFontYSIZE(HeadFont);
  int scr_w = ScreenW()-1;
  int scr_h = ScreenH()-1;

  
  
    if(SoftBackgrClr[3]!=0)
    {
      DrawRoundedFrame(0,ScreenH()-1-FntH-2,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);
    
     for(int i=GetFontYSIZE(ClFont);i>0;i-=2)
       DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i+2,0,0,0,SoftBackgrClr,SoftBackgrClr);
    }
  
 
  
  if(sendq_l)
  {
    sprintf(s,"Q: %d b",sendq_l);
    DrawStr(s, scr_w - GetWidth(s,ClFont), scr_h-1-FntH, scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
  }
  else
    DrawStr(right, scr_w-GetWidth(right,ClFont), scr_h-1-FntH, scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
  

  GetDateTime(0,&t);
  sprintf(s,"%02d:%02d",t.hour,t.min);
  DrawStr(s,0,scr_h-1-FntH,scr_w,scr_h,2,HeadFont,(char*)SoftFontClr,0);
  
  if(!strlen(ContactT9Key))
      DrawStr(left,2,scr_h-1-FntH,scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
  else
    if(strlen(ContactT9Key)==1)
      DrawStr(GetLongPressText(),2,scr_h-1-FntH,(scr_w/2)-(GetWidth(s,HeadFont)/2),scr_h,0,HeadFont,(char*)SoftFontClr,0);
  else
    DrawStr("Очистить Т9",2,scr_h-1-FntH,scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
    

  mfree(s);
}





int shot_header=0;
IMGHDR HeaderShot={0,0,8,""};

extern IMGHDR *GetIconByItem(int curitem);
extern IMGHDR *GetXIconByItem(int curitem);






 
int GetPrivateIcon()
{
  int private_icon = 0;  
  switch(CurrentPrivateStatus)
  {
    case 0: private_icon = ICON_ALL_CAN_SEE;          break;
    case 1: private_icon = ICON_NOBODY_CAN_SEE;       break;
    case 2: private_icon = ICON_VISLIST_CAN_SEE;      break;
    case 3: private_icon = ICON_INVISLIST_CANNOT_SEE; break;
    case 4: private_icon = ICON_CONTACTLIST_CAN_SEE;  break;
  } 
 return private_icon;
}






 
void DrawHeader()
{
  int scr_w = ScreenW()-1;
  int scr_h = ScreenH()-1;
  char *s = malloc(128);
  int drawFont = HeadFont;
  HeaderY = GetFontYSIZE(drawFont);

  HeaderY = (clistm->FontH < 16) ? 16 : (clistm->FontH);
  HeaderY += (0);       
  

  

    if(HeadBackgrClr[3]!=0)
    {
      DrawRoundedFrame(0,(0),scr_w,HeaderY+2,0,0,0,clrBlack,clrBlack);    
      for(int i = HeaderY-(0) ; i > 0 ; i-=2)
        DrawRoundedFrame(0,(0),scr_w,HeaderY-i+2,0,0,0,HeadBackgrClr,HeadBackgrClr); 
    }
  

  int x=1;
  if(HEAD_STATUS) 
  {
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
    x += DrwImage(GetIconByItem(icon),x,(0),1) + 2;
  }
  
  if(total_unread && isShowUnread)
  {
    sprintf(s,"%d/%d",total_unread,total_unread_all);
    DrawStr(s,x,(0)+1,x+GetWidth(s,drawFont),(0)+1+HeaderY,0,drawFont,(char*)HeadFontClr,0);
    x += GetWidth(s,ClFont)+1;
  }
  
  if(CurrentXStatus!=0 && HEAD_XSTATUS)
  {
    if(GetXIconByItem(CurrentXStatus)) 
      x += DrwImage(GetXIconByItem(CurrentXStatus),x,(0)+1,1) + 2;
  }
 
  if(HEAD_PRIVATE)
  {
    x += DrwImage(GetIconByItem(GetPrivateIcon()),x,(0),1) + 2;
  }
  
  int y = 1;
  if(strlen(ContactT9Key))
  {
    sprintf(s,key_clm_hdr_text);
    strcat(s,ContactT9Key);
    strcpy(clmenu_sk_r,key_clmenu_sk_r);
    DrawStr(s,scr_w - GetWidth(s,drawFont),y, scr_w,y+GetFontYSIZE(drawFont),4,drawFont,(char*)HeadFontClr,1);
  }
  else 
    if(on_off_timer > 0 && strlen(on_off_buf)!=0)
    {
      int i=GetWidth(on_off_buf, drawFont);
      if((i<0) || (i < scr_w-x-2))
       {
         int xx = scr_w-i-2;
	 DisableScroll();
         DrawStr(on_off_buf,xx,y,scr_w,y+GetFontYSIZE(drawFont),4,drawFont,(char*)HeadFontClr,0);
       }
      else
      {
	if(!max_scroll_disp)
           GBS_StartTimerProc(&tmr_scroll,216,scroll_timer_proc);
        
	max_scroll_disp = (x - (scr_w-i)) - 4;
        DrawScrollStr(on_off_buf,x,y,scr_w,y+HeaderY,drawFont,(char*)HeadFontClr);
      }
      
      strcpy(clmenu_sk_r,def_clmenu_sk_r);   
    }
  else
  {
    strcpy(clmenu_sk_r,def_clmenu_sk_r);   
    strcpy(s,empty_str);
    strcpy(clistm->s,empty_str);
    




 
    
    if(Is_Vibra_Enabled && isShowV)    strcat(s,"[V]") ;
    if(Is_Sounds_Enabled && isShowS)   strcat(s,"[S]") ;
    

   
    {
      sprintf(clistm->s,"[%d/%d]",online_contact,total_contact);
      strcat(s,clistm->s);
    }
    
  
    {
      sprintf(clistm->s,"[%d%%]",*RamCap()); 
      strcat(s,clistm->s);
    }
    
    DrawStr(s,(scr_w/2),y,scr_w,y+GetFontYSIZE(drawFont),4,drawFont,(char*)HeadFontClr,0);

















 
  }
  mfree(s);
}











 
void DrawScrollBar(int cur,int total)
{
  int scr_h = ScreenH()-1;
  int scr_w = ScreenW()-1;
  int y = scr_h - ((0)+GetFontYSIZE(ClFont)+4) - clistm->FontH - 2;
  int pos = sdiv(total, y*cur);
  int y2 = sdiv(total, y);
  int i = 0;
  
  DrawLine(scr_w - 2, ((0)+GetFontYSIZE(ClFont)+4), scr_w - 2, scr_h - GetFontYSIZE(ClFont) - 2, 1, clrBlack);
  DrawRoundedFrame(scr_w-4,((0)+GetFontYSIZE(ClFont)+4)+pos+1,scr_w,((0)+GetFontYSIZE(ClFont)+4)+pos+y2,0,0,0,clrBlack,clrBlack);
  
  for(i=0;i<y2;i+=1)
    DrawRoundedFrame(scr_w-4,((0)+GetFontYSIZE(ClFont)+4)+pos+1,scr_w,((0)+GetFontYSIZE(ClFont)+4)+pos+i,0,0,0,ScrollBarClr,ScrollBarClr);  
}






GBSTMR online_tmr;






 
void RedrawOnline()
{
  int f = 0;
  CLIST *t=(CLIST*)cltop;
  while(t=t->next)
   {
     if(t->isonline)
     {
       f = 1;       
       t->online =! t->online;
     }
   }
  
  if(f)
  {
     isonline = 1;
     SMART_REDRAW();
     GBS_StartTimerProc(&online_tmr,216,RedrawOnline);
  }
  else
  {
    FIRST_START = 0;
    t=(CLIST*)cltop;
    while(t=t->next){ t->online=0; t->isonline=0; }
    GBS_DelTimer(&online_tmr);
    isonline = 0;
  }
}








 
void DrawOnlineContact(RECT *rc,CLIST *cl)
{   
  DrawStr(cl->name,rc->x,rc->y,rc->x2,rc->y2,0,ClBoldFont,((!cl->online) ? (blue) : (clrRed)) ,1);
}






typedef struct
{
 unsigned int type; 
 unsigned int count;
}ACKED;

ACKED *myack;






 
static ACKED *GetAck(CLIST *t)
{
  if(!ShowProcessSend) return 0;
  if(!t) return 0;
  if(!t->isactive) return 0;
  if(t->state==0xFFFF) return 0;
  
  ACKED *ack = myack;
  ack->type=0;
  ack->count=0;
  LOGQ *lp;
  lp=t->log;

  while(lp)
  {
    if(lp->acked)
    {
      if (lp->acked==1)
      {
        ack->type = 1;
        ack->count++;
      } 
      else
      {
        ack->type = 0;

        if(!lp->next)
        {
          ack->type=0;
          ack->count=0;
        }
      }
    }
    else
     if (lp->ID!=0xFFFFFFFF)
     {
       ack->type = 2; 
       ack->count++;
     }
    lp=lp->next;
  }
  return ack;
}











 
char* GetColor(CLIST *t)
{
  if(t->isgroup) return (char*)GroupClr;
  if(isIndividualColor)
  {
    if(t->isactive) return (char*)ActiveClr;
    switch(GetIconIndex(t))
    {
      case IS_ONLINE: case IS_HOME: case IS_LUNCH: case IS_WORK: return (char*)CLColorsOnline;
      case IS_AWAY:       return (char*)CLColorsAway;
      case IS_NA:         return (char*)CLColorsNA;
      case IS_OCCUPIED:   return (char*)CLColorsOccupied;
      case IS_DND:        return (char*)CLColorsDND;
      case IS_DEPRESSION: return (char*)CLColorsDepression;
      case IS_FFC:        return (char*)CLColorsFFC;
      case IS_OFFLINE:    return (char*)CLColorsOff;
      default :           return t->isactive ? (char*)ActiveClr : (char*)NotActiveClr;
    }
  }
  
  return t->isactive ? (char*)ActiveClr : (char*)NotActiveClr;
}




unsigned int scr_w;
unsigned int scr_h;







 
void DrawList()
{  
  unsigned int font_y = 0; 
  unsigned int SoftH = 0;  
  unsigned int start_y; 
  unsigned int icon;   
  
  unsigned int oldY = 0;
  unsigned int drawFont; 
  char *drawColor; 

  if(!cltop) return;  
    
  SetVisibleScreen();

  font_y = clistm->FontH + 2; 

  SoftH = 0;  
  if(SOFT_MODE) SoftH = GetFontYSIZE(HeadFont);
  clistm->itemFromDisp = sdiv(font_y, ScreenH()-((0)+GetFontYSIZE(ClFont)+4)-SoftH-2 ); 
  if(VIEW_MODE) clistm->itemFromDisp = (clistm->itemFromDisp*2)-2; 
  if(LINE_MODE) clistm->itemFromDisp--; 

  CLIST *cl;
  RECT rc;
  ACKED *ack;
 
  char *name = malloc(32);
  
  drawFont = ClFont;  
  clistm->column = 0; 
  clistm->nlines = 0; 
  clistm->curitem = 0; 
  clistm->totalItems = CountContacts(); 
  
  if( clistm->itemFromDisp > clistm->totalItems ) clistm->itemFromDisp = clistm->totalItems;
    
  int start_item = ((VIEW_MODE==0) ? (clistm->startItem) : (0));
  for(; (clistm->curitem < ((VIEW_MODE!=0)?(clistm->totalItems):(clistm->itemFromDisp))) ; clistm->curitem++)
  {
     if(!VIEW_MODE) goto next;
     if(clistm->curitem >= (clistm->curpage-1)*clistm->itemFromDisp)
        {
        next:
          cl = FindContactByN(clistm->curitem + start_item);
          if(!cl) return;
          
          {
            icon = GetIconIndex(cl);
            start_y = 0;
            if(LINE_MODE) 
            {
              
              if(clistm->curitem + start_item > clistm->curpos ) start_y += font_y + 3;
              clistm->nlines = 2;
            }
            else clistm->nlines = 1;
 
            if(VIEW_MODE) 
            {
              if(!clistm->column) 
              {
                
                start_y += ( ((0)+GetFontYSIZE(ClFont)+4) + ( sdiv(2,(clistm->curitem - (clistm->curpage-1)*clistm->itemFromDisp)*font_y) ) ); 
                oldY = start_y;
                patch_rect(&rc, 1, start_y, (scr_w/2)-5, start_y+(font_y*clistm->nlines));
              }
              else
              {
                patch_rect(&rc, (scr_w/2)-4, oldY, scr_w-5, oldY+(font_y*clistm->nlines));
              }
              clistm->column = !clistm->column;
            }
            else
            {
              start_y += (((0)+GetFontYSIZE(ClFont)+4) + 2 + (clistm->curitem*font_y));
              patch_rect(&rc,2,start_y,scr_w-5,start_y+(font_y*clistm->nlines));
            }

            if(cl->isgroup && Is_Show_Groups)
            {
              int onlinetotal[2];
              GetOnTotalContact(cl->group,onlinetotal);
              if(!VIEW_MODE) 
              {
                snprintf(name,32,_s,cl->name);
                snprintf(clistm->s,32,"(%d/%d)",onlinetotal[0],onlinetotal[1]);
                DrawStr(clistm->s,scr_w-7-GetWidth(clistm->s,ClFont),start_y,scr_w-7,start_y+font_y,4,ClFont,(char*)GroupClr,1);
              }
              else
               snprintf(name,32,"%s(%d/%d)",cl->name,onlinetotal[0],onlinetotal[1]);
              if (cl->state) icon++; 
            }
            else
            {
              ack = GetAck(cl);
              snprintf(name,32,_s,cl->name);
            }
            
            
            

             
            rc.x += DrwImage(GetIconByItem((cl->online && BlinkIconByOnline  && FIRST_START == 0)?(IS_OFFLINE):(icon)),rc.x,rc.y+1,1) + 2;            
            
            if(cl->unread_msg!=0)
            {
              snprintf(clistm->s,32,_d,cl->unread_msg);
              DrawStr(clistm->s,rc.x,rc.y,rc.x2,rc.y2,0,ClFont,clrRed,0);
              rc.x += GetWidth(clistm->s,ClFont)+2;
            }
            
            
            if(ack && ack->count!=0 && (ShowProcessSend==1 || ShowProcessSend==3))
            {
              if(!cl->isgroup && cl->state!=0xFFFF)
              {
                
                snprintf(clistm->s,3, (cl->unread_msg ? "/%d" : _d),ack->count);
                drawColor = (ack->type==1) ? ((char*)SendClr) : ((ack->type==2)?((char*)UnAckedClr):0);
                DrawStr(clistm->s,rc.x,rc.y, rc.x2,rc.y2,0,drawFont,drawColor,0);
                rc.x += GetWidth(clistm->s,drawFont) + 2;
              }
            }
         
           if( clistm->curitem + start_item == clistm->curpos )
            {
              DrawRoundedFrame(rc.x-1,rc.y-1,rc.x2-1,rc.y2-1,0,0,0,ClCursorBordClr,ClCursorClr);
              if(LINE_MODE && !cl->isgroup)
              {
                if ((cl->xtext && cl->xtext[0]!=0) && (cl->state!=0xFFFF))
                  {
                    int i;
                    zeromem(clistm->s,64);
                    i=cl->xtext[0];
                    if (i>63) i=63;
                    strncpy(clistm->s,cl->xtext+1,i);
                  }
                  else
                    snprintf(clistm->s,64,percent_d,cl->uin);
                
                DrawStr(clistm->s,rc.x+1,rc.y+font_y,rc.x2-rc.x-3,rc.y2+font_y,0,ClFont,(char*)CLDescriptClr,1);
              }
              if(clistm->totalItems > 1)
                DrawScrollBar(clistm->curitem+clistm->startItem,clistm->totalItems);
            }

            
            if((cl->xstate!=0 && cl->state!=0xFFFF) )
            {
               unsigned int i;
               i = DrwImage(GetXIconByItem(cl->xstate),rc.x,rc.y,1);            
               rc.x += i ;
             
            }

            
            if(cl->isgroup) drawFont = ClBoldFont; 
            else drawFont = ( (SelBoldActiveChat==1 && (cl->isactive && cl->state!=0xFFFF)) ? ClBoldFont : ClFont );
            
            if(ShowProcessSend==2 || ShowProcessSend==3)
            {
              if(ack && ack->type!=0)
              {
                drawColor = (ack->type==1) ? ((char*)SendClr) : ((ack->type==2)?((char*)UnAckedClr):0);
                clistm->i = (VIEW_MODE) ? (rc.x2-1) : (rc.x+GetWidth(name, drawFont)+3);
                DrawRoundedFrame(rc.x,rc.y,clistm->i,rc.y2-2,0,0,0,0,drawColor);
              }
            }
            
            rc.x+=3;            
            if(cl->isonline && cl->state!=0xFFFF && FIRST_START == 0)
              DrawOnlineContact(&rc,cl);
            else
            {
         
              if( clistm->curitem + start_item == clistm->curpos )
                 drawColor = (char*)CLColorSelCont;
              else
                  drawColor = GetColor(cl);
              DrawStr(name, rc.x, rc.y, rc.x2, rc.y2, 0, drawFont, drawColor, 1);
            }
         }
       }

     
      if( (clistm->curitem > clistm->itemFromDisp*(clistm->curpage)) && (VIEW_MODE) ) break;
   
    }
  mfree(name);
}


 






 
void GotoFirstPosition()
{
  clistm->startItem = 0;
  clistm->curpage = 1;
  clistm->curpos = 0;
}







 
void SetVisibleScreen()
{
           int *curpos       = &clistm->curpos;
  unsigned int *itemFromDisp = &clistm->itemFromDisp;
  unsigned int *curpage      = &clistm->curpage;
  unsigned int *totalItems   = &clistm->totalItems;
  unsigned int *startItems   = &clistm->startItem; 
  
  if(VIEW_MODE)
  {
    if(*curpos > *itemFromDisp)
    {
      int i = 1;
      int j = *curpos;
      if(*itemFromDisp == *totalItems)
        *curpage = 1;
      else
      {
        while(j > *itemFromDisp)
        {
           j -= *itemFromDisp;
           i++;
           RefreshCL();
        }
        *curpage = i;
      }
    }
    else
    {
      *curpage = 1;
      RefreshCL();
    }
    
    if(*curpage == 1 && *itemFromDisp != *totalItems)
    {
      
      if(*curpos > *itemFromDisp)
      {
        while(*curpos > *itemFromDisp && *curpos > 0)
        {
          *curpos -= *itemFromDisp;
          RefreshCL();
        }
      }
    }
  }
  else 
  {
    
    if( ((*totalItems - *itemFromDisp) <= 0) && (*curpos > *itemFromDisp))
    {
      unsigned int start = (sdiv(2,*itemFromDisp)+1) > 5 ? (sdiv(2,*itemFromDisp)+1) : (5);
      
      if(*curpos > start && *curpos < *totalItems - *itemFromDisp)
        *startItems = *curpos - start;
      else
        if(*curpos > *totalItems - *itemFromDisp)
           *startItems = *totalItems - *itemFromDisp;
    }
  }
  RefreshCL();
}








 
void MoveUpOneCollumn(int i)
{
  unsigned int *itemFromDisp = &clistm->itemFromDisp;
  unsigned int *totalItems   = &clistm->totalItems;
  unsigned int *startItem    = &clistm->startItem;
           int *curpos       = &clistm->curpos;
  
  
  int start = (sdiv(2,*itemFromDisp)+1) > 5 ? (sdiv(2,*itemFromDisp)+1) : (5);

  if( (*curpos > *itemFromDisp + 2) && (*totalItems!=*itemFromDisp) )
    {
      *curpos -= i; 
      if(*startItem > 0 && *curpos > start && (*curpos < *totalItems - start))   *startItem -= i;
    }
  else
    if(*curpos > 0)
    {
      *curpos -= i;
      if(*startItem > 0 && *curpos > start && *totalItems != *itemFromDisp)  *startItem -= i;
      else if(*curpos < start) *startItem = 0; 
    }





 
  
  
  if(*curpos < 0)
  {
    
    *curpos = *totalItems-1;
    if((*totalItems - *itemFromDisp) <= 0) *startItem = 0; 
    else  *startItem = *totalItems - *itemFromDisp;
  }
}







 
void MoveDownOneCollumn(int i)
{
           int *curpos       = &clistm->curpos;
  unsigned int *itemFromDisp = &clistm->itemFromDisp;
  unsigned int *totalItems   = &clistm->totalItems;
  unsigned int *startItem    = &clistm->startItem;
  
  int start = (sdiv(2,*itemFromDisp)+1) > 5 ? (sdiv(2,*itemFromDisp)+1) : (5);
  
  
  if( *curpos < start && *totalItems!=*itemFromDisp )
  {
    *curpos += i;
  }
  else
    if( *curpos < *totalItems - 1 )
    {
      if( *totalItems - *itemFromDisp > 0 )
      {
        if(*startItem < (*totalItems - *itemFromDisp)) *startItem+=i;
      }
      *curpos+=i; 
    }
  else   
    {
      *curpos=0;
      *startItem=0;
    }

  
  if(*curpos >= *totalItems){ *curpos = 0; *startItem = 0; }
}






 
void MoveUpTwoCollumn(int i)
{
  if(!clistm->itemFromDisp)return;  

  clistm->curpos -= i;
  
  if(clistm->curpos < 0)
    {
      clistm->curpos = clistm->totalItems - 1;
    




 
    }



 
}






 
void MoveDownTwoCollumn(int i)
{
  if(!clistm->totalItems) return;
  clistm->curpos += i;
  

  
  if(clistm->curpos >= clistm->totalItems) { clistm->curpos = 0; clistm->curpage=1; }
  
  
  if(clistm->curpos >= clistm->curpage*(clistm->itemFromDisp + i)) { clistm->curpos -= i; clistm->curpage++; }
  
}









 
void PrintMess()
{
  int Font = 3;
  int len = GetWidth(prmsg,Font);
  int nlines = (len > (ScreenW()-10)) ? sdiv((ScreenW()-10),len) : (1);
  
  for(int i=0 ; i<strlen(prmsg) ; i++)
    if(prmsg[i]=='\n') nlines++; 
  
  nlines--;
  
  int y=0;
  int y2=0;
 
  y  = (ScreenH()/2) - (GetFontYSIZE(Font)*(sdiv(2,nlines) + 2));
  y2 = (ScreenH()/2) + (GetFontYSIZE(Font)*(sdiv(2,nlines) + ((nlines>1) ? 2 : 1)) );
  
  DrawRoundedFrame(0,y,ScreenW()-5,y2,0,0,0,clrWhite,"\x00\x00\x00\x40");
  DrawStr(prmsg,5,y+GetFontYSIZE(Font),ScreenW()-5,y2,2,Font,clrWhite,0);
}









 
void DrawContactList()
{
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,CLBackgrClr,CLBackgrClr);
  DrawImg(0,(0),S_ICONS[ICON_BG]);
  DrawHeader();
  DrawList();
  if(strlen(prmsg)!=0 && PrintTimer > 0)  PrintMess();
  if(SOFT_MODE)  DrawSoftKeys("Меню",clmenu_sk_r);
}









 
void ZeroOnline(CLIST *t)
 {
  if(t)
  {
    if(t->isonline)
    {
      CLIST *tt;
      tt=FindContactByUin(t->uin);
      tt->isonline=0;
      tt->online=0;
    }
   else
    if(t->log && !(t->isonline))
     {
       FreeLOGQ(&t->log);
       t->msg_count=0;
       t->isactive=0;
     }     
   
   if(t->uin==UIN)
   {
     isonline=0;
     CLIST *tt;
     tt=(CLIST *)(&cltop);
     while(tt=(CLIST *)(tt->next))
     {
       if(tt->isonline){ tt->isonline=0; tt->online=0; }
     }
   }
  }
  if(strlen(ContactT9Key)) ClearContactT9Key();
  RecountMenu(0,0);      
};










 
int FindMessage(CLIST *t)
{
  if(CountContacts()>0 && t)
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
    
   GotoFirstPosition();           
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
  return 0;
};







 
int ClOnKey(GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  CLIST *t;
      
  if (msg->gbsmsg->msg==0x195)
  {           
    int key=msg->gbsmsg->submess;
    if (((key>='0')&&(key<='9')) || key == '*' || key == '#' )
    {
      if(LongPressTimer==0)
      {
        LongPressAction(FindContactByN(clistm->curpos),msg->gbsmsg->submess);      
      }
      ClearContactT9Key();
      RecountMenu(0,1);
      return 0;
    }
    
    switch(msg->gbsmsg->submess)
    {
      case 0x3B:    if(VIEW_MODE) MoveUpTwoCollumn(2);   else MoveUpOneCollumn(1);   break;
      case 0x3C:  if(VIEW_MODE) MoveDownTwoCollumn(2); else MoveDownOneCollumn(1); break;
      case 0x3E: if(VIEW_MODE) MoveDownTwoCollumn(1); else MoveDownOneCollumn(2); break;
      case 0x3D:  if(VIEW_MODE) MoveUpTwoCollumn(1);   else MoveUpOneCollumn(2);   break;
      case 0x14:
        {
         t=FindContactByN(clistm->curpos);    
         
          if (t)
          {
            if (!t->isgroup)
            {
              FreeXText(t);
              t->req_xtext=0;
              RequestXText(t->uin);
            }
          }
        }
       break;
    }
  };
  
  if (msg->gbsmsg->msg==0x193)
  {
    switch(msg->gbsmsg->submess)
    {
     case 0x3B:    if(VIEW_MODE) MoveUpTwoCollumn(2);   else MoveUpOneCollumn(1);   break;
     case 0x3C:  if(VIEW_MODE) MoveDownTwoCollumn(2); else MoveDownOneCollumn(1); break;
     case 0x3E: if(VIEW_MODE) MoveDownTwoCollumn(1); else MoveDownOneCollumn(2); break;
     case 0x3D:  if(VIEW_MODE) MoveUpTwoCollumn(1);   else MoveUpOneCollumn(2);   break;
     case 0x01: 
      {
        extern void CreateMainMenu();
        if(!ClrPrintMess())
        {
          if(!strlen(ContactT9Key))
           CreateMainMenu();
          else            
            if(strlen(ContactT9Key)==1)
            { 
              LongPressAction(FindContactByN(clistm->curpos),ContactT9Key[0]);
              ClearContactT9Key();
              RecountMenu(0,1);
            }
          else
          {
            ClearContactT9Key();
            RecountMenu(0,1);
          }
        }
      }break;
    case 0x04:
    case 0x0C:
      {
        if(msg->gbsmsg->submess==0x0C && (isonline || on_off_timer > 0))
        {
          
          if(isonline)
          {
            isonline=0;
            CLIST *tt;
            tt=(CLIST *)(&cltop);
            while(tt=(CLIST *)(tt->next))
            {
              if(tt->isonline){ tt->isonline=0; tt->online=0; }
            }
          }
          else
          {
            
            ClrOnOffBuffer();
            DisableScroll();
          }
          break;
        }

        if(!ClrPrintMess())
         {
           if(strlen(ContactT9Key))
            {
              BackSpaceContactT9();
              RecountMenu(0,1);
              return(-1);
            }
           else
            {
             ShowLoadScreen=1;
            }
         }
      }
      break;
    case 0x1A:
     {         
      t=FindContactByN(clistm->curpos);  
      if(!ClrPrintMess())
      {
        if(t)
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
      }
      return(-1);
      }
      case '*':
        t=FindContactByN(clistm->curpos);  
        extern const int Action_by_Star;
        return LongPressActionOnChat(t,Action_by_Star);

      
    case 0x0B:
      {
          t=FindContactByN(clistm->curpos);    
         
          if (t)
          {
            if (!t->isgroup)
            {
              char s[256];
              char xt[256];
              zeromem(xt, sizeof(xt));
              zeromem(s,sizeof(s));

              t = FindContactByN(clistm->curpos);    
              if(t->xtext == 0)
                strcpy(xt,"Нет текста хстатуса");
              else
              {
                int i = t->xtext[0];                
                int len = strlen(t->xtext);
                if( (len - i - 1) > 0)
                {
                  strncpy(xt,t->xtext+1,i);
                  sprintf(xt+i, "\n%s\0",t->xtext+i+1);
                }
                else
                  sprintf(xt, "%s\0",t->xtext+1);
              }
              snprintf(s,255,"[   %s   ]\n%s\n%s\0", t->name,
                                                     t->sttime == 0 ? "" : t->sttime, 
                                                     t->xstate == 0 ? "Хстатус не установлен" : xt);
              s[strlen(s)] = 0;
              PrintMessage(s,10);
            }
          }
      }
      break;
      
    case 0x14:
      { 
        if(!isVolUpButton) break;
      }break;      
    }
    
    
    if (key=='#' && (CountContacts()>0) && ActionResh)
    {
      t=FindContactByN(clistm->curpos);     
      if(ActionResh == 1) ZeroOnline(t);
      if(ActionResh == 2) return FindMessage(t);
    }   
  }  
 
  if ((key>='0')&&(key<='9') && msg->gbsmsg->msg==0x193)
    {
      GotoFirstPosition();
      AddContactT9Key(key);
      RecountMenu(0,1);
      return(-1);
    }
  return 0;  
}




 







 
void DrawLoadScreen(MAIN_GUI *data)
{
  int scr_w = ScreenW();
  int scr_h = ScreenH();






   


 

  DrawRectangle(0,(0),scr_w-1,scr_h-1,0,
		   GetPaletteAdrByColorIndex(1),
		   GetPaletteAdrByColorIndex(1));
  DrawImg(0,0,S_ICONS[ICON_LOGO]);

  unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;			
  wsprintf(data->ws1,"State: %d, RXstate: %d\nRx:%db,Tx:%db\nQueue: %db\n%s\n%t",connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);











 
   




 
 



 
    


 
  DrawString(data->ws1,3,3+(0),scr_w-4,scr_h-4-GetFontYSIZE(3),
	     7,32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
 
  

  wsprintf(data->ws2,"Uin: %d\nRecv: %d Send: %d Blocked: %d",UIN, CountMessage[0],CountMessage[1], blocked_spam);
  DrawString(data->ws2,1,scr_h-10-5*GetFontYSIZE(8)+1,ScreenW()-1,scr_h-10-GetFontYSIZE(3)-3
             ,7,32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));

  
  if(connect_state==3)
  {
    if(Auto_Status)
    {
      int m = (msg_away_timer*5)/60;
      int s = (msg_away_timer*5)%60;
      int h = m/60;  
      wsprintf(data->ws2,"Online: %02d:%02d:%02d   not action %02d:%02d:%02d", uptime.hour, uptime.min, uptime.sec,h,m%60,s);
    }
    else
      wsprintf(data->ws2,"Online: %02d:%02d:%02d",uptime.hour, uptime.min, uptime.sec);
  }
  else
    wsprintf(data->ws2,"Offline [%02d:%02d:%02d]",uptime.hour, uptime.min, uptime.sec);

  DrawString(data->ws2,1,scr_h-10- 3*GetFontYSIZE(8)+1,ScreenW()-1,scr_h - 10 - GetFontYSIZE(3)-3
             ,7,32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
 
  
  wsprintf(data->ws2,percent_t,cltop ? lgpData[LGP_GrsKeyClist] : empty_str);
  DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(3),
	     scr_w-4,scr_h-4,3,4+32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
  wsprintf(data->ws2,percent_t, lgpData[LGP_GrsKeyExit] );
  DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(3),
	     scr_w>>1,scr_h-4,3,1+32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
}


void method0(MAIN_GUI *data)
{
  if(edchat_id) return;
  if(IsGuiOnTop(maingui_id))
  {
    if(ShowLoadScreen)    
    {
 

 
      DrawLoadScreen(data);
    }
    else
    {
      DrawContactList();    
    }
  }
}


void method1(MAIN_GUI *data,void *(*malloc_adr)(int))
{
  data->ws1=AllocWS(256);
  data->ws2=AllocWS(256);


  scr_w = ScreenW()-1;
  scr_h = ScreenH()-1;
  clistm = malloc(sizeof(GUIMENU));
  
  clistm->font = ClFont;
  clistm->FontH = GetFontYSIZE(ClFont);
  
  menu_sk[0].lgp_id=(int)lgpData[LGP_Select];
  menu_sk[1].lgp_id=(int)lgpData[LGP_Close];  
 
  
  
  
  myack = malloc(sizeof(ACKED));
  clistm->totalItems = 0;
  clistm->startItem = 0;
  clistm->curpos = 0;
  
  data->gui.state=1;
}

void method2(MAIN_GUI *data,void (*mfree_adr)(void *))
{

  FreeWS(data->ws1);
  FreeWS(data->ws2);
  if(clistm) mfree(clistm);
  clistm = 0;
  if(myack) mfree(myack);
  myack = 0;
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
  data->gui.state=1;
}


int LoadOnKey(GUI_MSG *msg)
{
  DirectRedrawGUI();
  if (msg->gbsmsg->msg==0x193)
  {
    switch(msg->gbsmsg->submess)
    {
    case 0x01:
      if(ShowLoadScreen) return(1);
      else  ShowLoadScreen=1;
    case 0x04:
      
       if(ShowLoadScreen && (cltop || sock==-1)) ShowLoadScreen=0;
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
    case '0':
      disautorecconect=0;
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
         {
           SetPrivateStatus(1);
         }
         else
         {
           SetPrivateStatus(3);
         }
         break;
    case '4':
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        break;
    case 0x0C:
    
       disautorecconect=1;
       extern void end_socket(void);
       SUBPROC((void*)end_socket);
       ShowMSG(1,(int)"Disconnect!");
       break;
    }
  }
  return(0);
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

GBSTMR OneSecTmr;
void OneSecondTimer()
{
  tmr_gipc.name_to=ipc_my_name;
  tmr_gipc.name_from=ipc_my_name;
  tmr_gipc.data=0;
  GBS_SendMessage(0x4209,0x3F2,8,&tmr_gipc);  
  GBS_StartTimerProc(&OneSecTmr,216,OneSecondTimer);
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
  wsBuffer=AllocWS(4096);
  

  DNR_TRIES = 3;

  
  
  CountMessage[0] = 0;
  CountMessage[1] = 0;
 
  FreeAllList();

  GBS_StartTimerProc(&OneSecTmr,216*10,OneSecondTimer);
  GBS_StartTimerProc(&tmr_active,216*10,process_active_timer);
  sprintf((char *)ipc_my_name+6,percent_d,UIN);
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(0x4209,0x3F2,1,&gipc);
}

void maincsm_onclose(CSM_RAM *csm)
{
  WriteDefSettings();
  do_StatusChangeWrite(0, 0, 1);  
  GBS_DelTimer(&online_tmr);  
  
  
  GBS_DelTimer(&OneSecTmr);
  GBS_DelTimer(&tmr_active);
  GBS_DelTimer(&tmr_vibra);
  GBS_DelTimer(&reconnect_tmr);
  GBS_DelTimer(&tmr_illumination);

  
  GBS_DelTimer(&remind_tmr);
  GBS_DelTimer(&popup[0].tmr);
  GBS_DelTimer(&popup[1].tmr);
  GBS_DelTimer(&popup[2].tmr);
  GBS_DelTimer(&away_tmr);
  
  FreeAllList();   
  FreeWS(wsBuffer);

  SetVibration(0);
  FreeTemplates();
  FreeCLIST();
  free_ICONS();
  
  FreeWS(ews);
  FreeXStatusText();
  lgpFreeLangPack();
  

 
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
    GBS_SendMessage(0x4209,0x3F2,2,&gipc);
    LockSched();
    CloseCSM(maincsm_id);
    
    UnlockSched();
  }
  else
  {   
    LoadingImages();
    LoadLocalCL();
  }
}

int maincsm_onmessage(CSM_RAM *data,GBS_MSG *msg)
{
  extern const int VIBR_ON_CONNECT;
  
  if(DEVELOP_IF == 3) 
  {
    if(!IsUnlocked()) develop_by_unlock = 1;
    if(IsUnlocked() && develop_by_unlock == 1)
    {
      develop_by_unlock = 0;
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)) to_develop();   
    }  
  }
  
  
  MAIN_CSM *csm=(MAIN_CSM*)data;
  {
    
    if (msg->msg==0x3F2)
    {
      IPC_REQ *ipc;
      if ((ipc=(IPC_REQ*)msg->data0))
      {
	if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
	{
	  switch (msg->submess)
	  {
	  case 1:
	    
	    if (ipc->name_from==ipc_my_name)
            {
                SUBPROC((void *)CheckDoubleRun);
            }
            else ipc->data=(void *)maincsm_id;
	    break;
	 




















 
        






 
          case 8:
            if (ipc->name_from==ipc_my_name)
            {
               UpTime();
               if(ShowLoadScreen && !edchat_id) DoRefreshCL = 1; 
             

               if((--on_off_timer) > 0)
                {
                   if(!edchat_id) DoRefreshCL = 1;
                   
                }
                else
                  if(on_off_timer != -1)
                  {
                    on_off_timer = -1;
                    isonline = 0;
                    zeromem(on_off_buf,128);
                    DisableScroll();
                    if(!edchat_id) DoRefreshCL = 1;
                  }
                
                if(LongPressTimer>0) LongPressTimer--;
               
                if((PrintTimer--) > 0 && !edchat_id) DoRefreshCL = 1;
                
               
               if(!DoRefreshCL) break;

               SMART_REDRAW();
               DoRefreshCL=0;
            }
            break;
	  case 4:
	    
	    if (ipc->name_from==ipc_my_name)
	    {
	      CLIST *t=(CLIST *)cltop;
	      int f=0;
	      while(t)
	      {
               if(t->isonline)
                {
                  if (!(--(t->isonline))){ f=1; t->online=0; }
                  SMART_REDRAW();
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
                if(!ShowLoadScreen)
     		  oldt=FindContactByN(clistm->curpos);
		RecountMenu(oldt, 1);
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
     	  case 5:                                   
	    {
	      int l=strlen(((IPCMsg *)(ipc->data))->msg);
	      TPKT *msg=malloc(sizeof(PKT)+l);
	      msg->pkt.uin=((IPCMsg *)(ipc->data))->uin;
	      msg->pkt.type=2;
	      msg->pkt.data_len=l;
	      memcpy(msg->data,((IPCMsg *)(ipc->data))->msg,l);
	      
	      SENDMSGCOUNT++; 
	      SUBPROC((void *)SendAnswer,0,msg);
	    }
            break;
	  case 7:
	    ;
	    IPCMsg_RECVMSG *fmp=((IPCMsg_RECVMSG *)(ipc->data));
	    if (ipc->name_to!=ipc_my_name) break;
	    if (!fmp->drop_msg)
	    {
	      
	      int l=strlen(fmp->msg);
	      TPKT *msg=malloc(sizeof(PKT)+l+1);
	      msg->pkt.uin=fmp->uin;
	      msg->pkt.type=3;
	      msg->pkt.data_len=l;
	      memcpy(msg->data,fmp->msg,l+1);
               ae(msg);
	      
	    }
	    mfree(fmp->msg); 
	    mfree(fmp->ipc); 
	    mfree(fmp); 
	    mfree(ipc); 
	    return 0; 
	  }
	}
      }
    }
    
    CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
    if (IsGuiOnTop((((int *)icsm)[0x28/4])) ) 
    {
      extern const int ICON_IDLE_ENABLE;
      if(ICON_IDLE_ENABLE)
      {
        GUI *igui=GetTopGUI();
        if (igui) 
        {
          void *canvasdata=BuildCanvas();
          int icn;
          if (total_unread)
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
          
          
          DrawCanvas(canvasdata,IDLEICON_X,IDLEICON_Y,IDLEICON_X+GetImgWidth((int)S_ICONS[icn])-1,
                     IDLEICON_Y+GetImgHeight((int)S_ICONS[icn])-1,1);
          DrawImg(IDLEICON_X,IDLEICON_Y,S_ICONS[icn]);
        }
      }
    }
  }
  if (msg->msg==0x3F1)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"NatICQ config updated!");
      InitConfig();
      
      void UpdateCSMname(void);
      UpdateCSMname();

      sprintf((char *)ipc_my_name+6,percent_d,UIN);
      clistm->font = ClFont;
      clistm->FontH = GetFontYSIZE(ClFont);
      free_ICONS();
      setup_ICONS();
      ResortCL();
      RecountMenu(0, 1);
      
      
      shot_header=0;
      shot_soft=0;
    }
  }
  
  if (msg->msg==0x98)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    if ((int)msg->data0==edchat_id)
    {
      edchat_id=0;
      SMART_REDRAW();
    }
  }
  if (msg->msg==0x3F0)
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
      
      snprintf(logmsg, 255, "GPRS сессия запущена, подключение через %d секунд...", RECONNECT_TIME);
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
	TPKT *p=(TPKT *)msg->data0;
	
	if (p->pkt.type==3)
	{
	  

	  int l=p->pkt.data_len+1; 
	  IPC_REQ *ripc=malloc(sizeof(IPC_REQ));
	  IPC_REQ *fipc=malloc(sizeof(IPC_REQ));
	  IPCMsg_RECVMSG *msg=malloc(sizeof(IPCMsg_RECVMSG));
	  memcpy(msg->msg=malloc(l),p->data,l);
	  msg->uin=p->pkt.uin;
	  msg->drop_msg=0;
	  msg->ipc=ripc;
	  ripc->data=msg;
	  fipc->data=msg;
	  ripc->name_from=ipc_my_name;
	  ripc->name_to=ipc_my_name;
	  fipc->name_from=ipc_my_name;
	  fipc->name_to=ipc_my_name;
	  GBS_SendMessage(0x4209,0x3F2,6,ripc);
	  GBS_SendMessage(0x4209,0x3F2,7,fipc);
	  mfree(p);
	}
	else
	{
	  
          
	  ProcessPacket(p);
	}
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
            SUBPROC((void *)ab,0,0);
	   





 
	  }
	  GROUP_CACHE=0;
	  SENDMSGCOUNT=0; 
	  if (!FindGroupByID(0)) AddGroup(0,lgpData[LGP_GroupNotInList]);
          extern const int IS_SHOW_LOOPBACK;
	  if (!FindContactByUin(UIN) && IS_SHOW_LOOPBACK) AddContact(UIN, lgpData[LGP_ClLoopback], 0,1);
	  SUBPROC((void *)LoadLocalCL);
	  SMART_REDRAW();
	}
	else
	{
	  ShowMSG(1,(int)lgpData[LGP_MsgIllegmsgcon]);
	}
	break;
      case 0x8C:
	if (connect_state>=2)
	{
	  
	  SUBPROC((void *)get_answer);
	  
	}
	else
	{
	  ShowMSG(1,(int)lgpData[LGP_MsgIllegmsgrea]);
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
	RecountMenu(0, 1);
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
          snprintf(logmsg,255,"%s\nОшибка, переподключение через %d секунд...",logmsg, RECONNECT_TIME);
        }
	break;
      }
    }
  }
  return(1);
}


const int minus11=-11;

unsigned short maincsm_name_body[140];









unsigned short maincsm_name_body[140];

struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
    maincsm_onmessage,
    maincsm_oncreate,
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
};

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "NATICQ: %d",UIN);
}


int main(char *filename)
{  
  MAIN_CSM main_csm;
  char *s;
  int len;
  extern const char *successed_config_filename;
  WSHDR *ws;
  
  InitConfig();
  
  s=strrchr(filename,'\\');
  len=(s-filename)+1;
  strncpy(elf_path,filename,len);
  elf_path[len]=0;
  
  lgpLoadLangpack();
  
  if (!UIN)
  {
    LockSched();
    ShowMSG(1,(int)lgpData[LGP_MsgNoUinPass]);
    ws=AllocWS(150);
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws,0,0);
    UnlockSched();
    lgpFreeLangPack();
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
  away_timer();
  Reminder_timer();
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
    if(IsBufferMenu)
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
        ascii2ws(item->ws,"Очистить буфер");
        break;
      default:
        i-=3;
        if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
        break;
      }
    }
    else
    {
      switch(i)
      {
      case 0:
        ascii2ws(item->ws,lgpData[LGP_MnuEdNextAct]);
        break;
      case 1:
        ascii2ws(item->ws,lgpData[LGP_MnuEdPrevAct]);
        break;
      default:
        i-=2;
        if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
        break;
      }
    }
  }
  if (item->type==1)
  {
    if(IsBufferMenu)
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
         CutWSTR(wsBuffer,0);
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
      default:
        i-=2;
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
    
    if (l==0x15)
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
    
    if (l==0x14)
    {
      WSHDR *ws=AllocWS(strlen(last_msg_buffer)+1);
      ascii2ws(ws,last_msg_buffer);
      EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
      EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
      FreeWS(ws);
      RefreshGUI();
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
              CountMessage[1]++;
              t->send++;
              msg_away_timer=0;
              away_m=0;
              t->automsgcount=0;
              if(change_back_status)
              {
                CurrentStatus=status_rem;
                set_my_status();
                change_back_status=0;
              }
	      t->isactive=360;
	      p=malloc(sizeof(PKT)+len+1);
	      p->pkt.uin=t->uin;
	      p->pkt.type=2;
	      p->pkt.data_len=len;
	      strcpy(p->data,s);
              strcpy(last_msg_buffer, s);
	      AddStringToLog(t,0x01,p->data,I_str,(++SENDMSGCOUNT)&0x7FFF,1); 
	      SUBPROC((void *)SendAnswer,0,p);
	      mfree(t->answer);
	      t->answer=0;
	      
	      EDIT_SetFocus(data,ed_struct->ed_answer);
	      CutWSTR(ews,0);
	      EDIT_SetTextToFocused(data,ews);
	      AddMsgToChat(data);
	      RecountMenu(t, 1);
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
                
        if(IsUrl(ews, pos, link))
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

          if(IsBufferMenu)
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+3);
          else
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+2);
          
          mfree(link);
          return (-1);
        }          
          
      }

    }
    if (l==0x14)
    {
      int pos;

      if ((pos=EDIT_GetFocus(data)-2)>=2)
      {
        EDIT_SetFocus(data, pos);
        EDIT_SetCursorPos(data, 1);
      }
      return (-1);
    }
    if (l==0x15)
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

static const HEADER_DESC edchat_hdr={0,0,0,0,0,0,0x7FFF};

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
      ((HEADER_DESC *)&edchat_hdr)->rc.x2=ScreenW()-1-(width=GetImgWidth(icon));
      DrawImg(ScreenW()-1-width,1,icon);
    }
  }
}

void edchat_ghook(GUI *data, int cmd)
{
  static SOFTKEY_DESC sk={0x0FFF,0x0000,0};
  sk.lgp_id=(int)lgpData[LGP_Menu];
  static SOFTKEY_DESC sk_cancel={0x0FF0,0x0000,0};
  sk_cancel.lgp_id=(int)lgpData[LGP_Close];
  
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
    RecountMenu(ed_struct->ed_contact, 1);
    mfree(ed_struct);
  }
  if (cmd==0x0A)
  {
    pltop->dyn_pltop=SmilesImgList;
    DisableIDLETMR();
    total_unread-=ed_struct->requested_decrement_total_unread;
    ed_struct->requested_decrement_total_unread=0;





 
  }
  if (cmd==7)
  {
    SetSoftKey(data,&sk,1);
    ExtractEditControl(data,ed_struct->ed_answer,&ec);
    if (ec.pWS->wsbody[0]==0)
      SetSoftKey(data,&sk_cancel,1==0?1:0);
    if (!EDIT_IsBusy(data))
    {
      time_to_stop_t9=0;
      if (ed_struct->ed_contact->req_add)
      {
	ed_struct->ed_contact->req_add=0;
	AddMsgToChat(data);
	RecountMenu(ed_struct->ed_contact, 1);
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
  7,
  100,
  101,
  0,
  
  
  
  
  
  
  0,
  
  
  
  0x40000000
};

void CreateEditChat(CLIST *t)
{
  extern const int FIRST_LETTER;
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
          color = (type==1)?O_I_COLOR:((type==3 || type==4)?O_X_COLOR:O_TO_COLOR);
          font = (type==3 || type==4)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
        }
        else
        {
          color = (type==1)?I_COLOR:((type==3 || type==4)?X_COLOR:TO_COLOR);
          font = (type==3 || type==4)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
        }
      }
      else
	color=UNACK_COLOR; 
    }
    PrepareEditControl(&ec);
    if ((lp->type&0x0F)!=3 && (lp->type&0x0F)!=4)
    {
      ascii2ws(ews,lp->hdr);
      ConstructEditControl(&ec,1,0x40,ews,ews->wsbody[0]);
    }
    else
      ConstructEditControl(&ec,1,0x20,ews,0);
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,color);
    SetFontToEditCOptions(&ec_options,font);
    CopyOptionsToEditControl(&ec,&ec_options);
    AddEditControlToEditQend(eq,&ec,ma);
    if ((lp->type&0x0F)!=3 && (lp->type&0x0F)!=4)
    {
      ParseAnswer(ews,lp->text);
    }
    else
    {
      ParseXStatusText(ews,lp->text,(lp->type&0x10)?O_X_COLOR:X_COLOR,t->xstate);
    }
    PrepareEditControl(&ec);
    ConstructEditControl(&ec,
                         3,
                         ews->wsbody[0] ? 0x40|0x2000 : 0x20,
                         ews,ews->wsbody[0]);
    PrepareEditCOptions(&ec_options);

    if ((lp->type&0x0F)!=3 && (lp->type&0x0F)!=4)
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
  t->isunread=0;

  total_unread_all -= t->unread_msg;
  t->unread_msg=0;

  
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
  ConstructEditControl(&ec,3,(FIRST_LETTER)?0x1000:0,ews,7168);

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

    int scr_w=ScreenW();
    int scr_h=ScreenH();
  

  patch_header(&edchat_hdr);
  patch_input(&edchat_desc);
  
  
  
  edchat_id=CreateInputTextDialog(&edchat_desc,&edchat_hdr,eq,1,ed_struct);
}




int Focus_Quote = 0;

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
  
  int ii=EDIT_GetFocus(ed_struct->ed_chatgui);
  
  EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
  EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
  
  if(!Focus_Quote) EDIT_SetFocus(ed_struct->ed_chatgui,ii);
  FreeWS(ws);
  GeneralFuncF1(1);
}

void QuoteFocus(GUI *data)
{
  Focus_Quote = 1;
  Quote(data);
}

void QuoteNotFocus(GUI *data)
{
  Focus_Quote = 0;
  Quote(data);
}

void GetShortInfo(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);

  TPKT *p;
  CLIST *t;
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    req_info_contact = 1;
    p=malloc(sizeof(PKT));
    p->pkt.uin=t->uin;
    p->pkt.type=11;
    p->pkt.data_len=0;
    AddStringToLog(t, 0x01, "Request info...", I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t, 1);
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
  
  
  
  
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen((char*)lgpData[LGP_AuthReq]))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=10;
    p->pkt.data_len=l;
    strcpy(p->data,(char*)lgpData[LGP_AuthReq]);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t, 1);
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
  
  
  
  
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen((char*)lgpData[LGP_AuthGrant]))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=14;
    p->pkt.data_len=l;
    strcpy(p->data,(char*)lgpData[LGP_AuthGrant]);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t, 1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void OpenLogfile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);

  extern const char HIST_PATH[64];
  extern const int HISTORY_TYPE;
  CLIST *t;
  WSHDR *ws=AllocWS(256);
  if ((t=ed_struct->ed_contact))
  {
    if (HISTORY_TYPE)
      wsprintf(ws,"%s\\%u\\%u.txt",HIST_PATH,UIN,t->uin);
    else
      wsprintf(ws,"%s\\%u.txt",HIST_PATH,t->uin);
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
      RecountMenu(t, 1);
      GeneralFuncF1(1);
    }
  }
}






 
void LoadMoreHistory(GUI *data)
{
   ClearLog(data);
  EDCHAT_STRUCT *ed_struct;
  CLIST *t;

  ed_struct = MenuGetUserPointer(data);
  t=ed_struct->ed_contact;    
  GetHistory(t, 8*1024);
  GeneralFunc_flag1(edchat_id,1);
  CreateEditChat(t);
}

void ecmenu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

static MENUITEM_DESC ecmenu_ITEMS[10]=
{
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578},
  {0,0,0x7FFF,0,0,3,0x578}
};

extern void AddSmile(GUI *data);
static const MENUPROCS_DESC ecmenu_HNDLS[10]=
{
  QuoteFocus,
  QuoteNotFocus,
  AddSmile,
  GetShortInfo,
  AddCurContact,
  SendAuthReq,
  SendAuthGrant,
  OpenLogfile,
  LoadMoreHistory,
  ClearLog,
};





















 


int ec_menu_onkey(void *data, GUI_MSG *msg)
{









   
  return 0;
}


char ecm_contactname[64];

static const HEADER_DESC ecmenu_HDR={0,0,0,0,0,(int)ecm_contactname,0x7FFF};

static const MENU_DESC ecmenu_STRUCT=
{
  8, 0,ecmenu_ghook,0,
  menusoftkeys,
  &menu_skt,
  0,
  0,
  ecmenu_ITEMS,
  ecmenu_HNDLS,
  10
};



void ec_menu(EDCHAT_STRUCT *ed_struct)
{
  CLIST *t;
  int to_remove[10+1];
  int remove=0;
  if ((t=ed_struct->ed_contact))
  {
    if(strlen(t->name))
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
    
    
    ecmenu_ITEMS[0].lgp_id_small=(int)lgpData[LGP_MnuQuote]; 
    ecmenu_ITEMS[1].lgp_id_small=(int)lgpData[LGP_MnuQuoteNotFocus]; 
    ecmenu_ITEMS[2].lgp_id_small=(int)lgpData[LGP_MnuAddSml];
    ecmenu_ITEMS[3].lgp_id_small=(int)lgpData[LGP_MnuShInfo]; 
    ecmenu_ITEMS[4].lgp_id_small=(int)lgpData[LGP_MnuAddRen];
    ecmenu_ITEMS[5].lgp_id_small=(int)lgpData[LGP_MnuSAuthReq]; 
    ecmenu_ITEMS[6].lgp_id_small=(int)lgpData[LGP_MnuSAuthGrt]; 
    ecmenu_ITEMS[7].lgp_id_small=(int)lgpData[LGP_MnuOpenLog]; 
    ecmenu_ITEMS[8].lgp_id_small=(int)"Больше истории"; 
    ecmenu_ITEMS[9].lgp_id_small=(int)lgpData[LGP_MnuClearCht]; 
    
    CreateMenu(0,0,&ecmenu_STRUCT,&ecmenu_HDR,0,10,ed_struct,to_remove);
  }
}


void AskNickAndAddContact(EDCHAT_STRUCT *ed_struct)
{
  CreateAddContactGrpDialog(ed_struct->ed_contact);
}

static void AddSmile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct=MenuGetUserPointer(data);
  CreateSmileSelectGUI(ed_struct);
  GeneralFuncF1(1);
}
