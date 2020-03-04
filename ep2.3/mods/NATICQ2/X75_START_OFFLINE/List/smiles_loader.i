





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























 



unsigned int char8to16(int c);
unsigned int char16to8(unsigned int c);
void ascii2ws(WSHDR *ws, const char *s);
char *MakeGlobalString(const char *first, int breakchar, const char *last);
int strcmp_nocase(const char *s, const char *d);
void log(char *fname, void *text);


IMGHDR *read_pngimg(const char *buf);

IMGHDR *CreateImgr(int width, int height);

IMGHDR *CreateImgr16(int width, int height);









 





 

void FreeAllList();
void LoadingImages();




S_SMILES *s_top=0;

DYNPNGICONLIST *SmilesImgList;
DYNPNGICONLIST *XStatusesImgList;

int *XStatusesIconArray;







 
extern const char SMILE_FILE[];
extern const char SMILE_PATH[];
extern const char XSTATUSES_PATH[];
extern const char ipc_my_name[32];
extern  int S_ICONS[];


static char *p_buf;
static char *s_buf;
static S_SMILES *s_bot;
static int n_pic;

int cur_smile=0;

DYNPNGICONLIST *smilestop = 0;
DYNPNGICONLIST *iconstop = 0;
DYNPNGICONLIST *xiconstop = 0;

extern int drawimg2img(IMGHDR * onse, IMGHDR * timage2, int x, int y, int xRect, int yRect,int xxRect, int yyRect);
extern void PressImg(IMGHDR *imgsrc, char freemap);
extern const char *icons_names[];
extern const char ICON_PATH[];


S_SMILES *FindSmileById(int n)
{
  int i=0;
  S_SMILES *sl=(S_SMILES *)s_top;
  while(sl && i!=n)
  {
    sl=sl->next;
    i++;
  }
  return sl;
}

S_SMILES *FindSmileByUni(int wchar)
{
  S_SMILES *sl=(S_SMILES *)s_top;
  while(sl)
  {
    if (sl->uni_smile == wchar) return (sl);
    sl=sl->next;
  }
  return (0);
}

void FreeSmiles(void)
{
  S_SMILES *s_smile;
  STXT_SMILES *n;
  STXT_SMILES *st;
  DYNPNGICONLIST *d;
  DYNPNGICONLIST *nd;
  LockSched();
  
  s_smile=(S_SMILES *)s_top;
  s_top=0;
  s_bot=0;
  UnlockSched();
  while(s_smile)
  {
    S_SMILES *s;
    s=s_smile;
    st=s->lines;
    while(st)
    {
      n=st->next;
      mfree(st);
      st=n;
    }
    s_smile=(S_SMILES *)(s_smile->next);
    mfree(s);
  }
  LockSched();
  d=SmilesImgList;
  SmilesImgList=0;
  UnlockSched();
  while(d)
  {
    if (d->img)
    {
      mfree(d->img->bitmap);
      mfree(d->img);
    }
    nd=d->next;
    mfree(d);
    d=nd;
  }
  mfree(s_buf);
}

void InitSmiles(void)
{
  int f;
  unsigned int err;
  int fsize;
  char *buf;
  FSTATS stat;

  FreeSmiles();

  
  if (GetFileStats(SMILE_FILE,&stat,&err)==-1)
    return;

  if ((fsize=stat.size)<=0)
    return;

  if ((f=fopen(SMILE_FILE,0+0x8000,0x80,&err))==-1)
    return;

  buf=s_buf=p_buf=malloc(fsize+1);
  buf[fread(f,buf,fsize,&err)]=0;
  fclose(f,&err);
  




 
}

void CheckSmiles(void)
{
  int f;
  unsigned int err;
  int fsize;
  char *buf, *p_buf;
  FSTATS stat;

  if (GetFileStats(SMILE_FILE,&stat,&err)==-1)
    return;

  if ((fsize=stat.size)<=0)
    return;

  if ((f=fopen(SMILE_FILE,0+0x8000,0x80,&err))==-1)
    return;

  buf=p_buf=malloc(fsize+1);
  buf[fread(f,buf,fsize,&err)]=0;
  fclose(f,&err);
  

  for(buf=p_buf;*buf; buf++)
    if(*buf == ':')
    {
      buf++;
      while(*buf && *buf != 0x0D) buf++;
      
    }
  mfree(p_buf);
}









 
static void FreeList(DYNPNGICONLIST *icon)
{
  if(!icon) return;
  LockSched();
  icon=0;
  UnlockSched();
  while(icon)
  {
    DYNPNGICONLIST *icon_prev = icon;
    icon = icon->next;

    if(icon_prev)
    {
      if(icon_prev->img)
      {
        mfree(icon_prev->img->bitmap);
        mfree(icon_prev->img);
      }
      mfree(icon_prev);
    }
  }
}








 
static IMGHDR *GetPicByItem(DYNPNGICONLIST *icon, int curitem)
{
  int i=0;

  while(icon)
  {
    if(i==curitem) return (icon->img);
    i++;
    icon=icon->next;  
  }
  return 0;
}







 
static int CountPic(DYNPNGICONLIST *icon)
{
  int i=0;
  while(icon) { i++; icon = icon->next; }
  return i;
}








 
static void AddToTopList(DYNPNGICONLIST *top, DYNPNGICONLIST *newitem)
{
  DYNPNGICONLIST *icon = (DYNPNGICONLIST *)top;
  while(icon->next) icon = icon->next;
  icon->next = newitem;
}







 
DYNPNGICONLIST* NewIcon(IMGHDR *img)
{
  DYNPNGICONLIST *icon = malloc(sizeof(DYNPNGICONLIST));
  zeromem(icon,sizeof(icon));
  icon->next = 0;
  icon->img = img;
  return icon;
}











 
static void AddSmile(IMGHDR *img,int x,int y)
{
   DYNPNGICONLIST *icon =  NewIcon(CreateImgr(28,28));
   drawimg2img(img,icon->img,0,0,x,y,x+27,y+27);
   PressImg(icon->img,1); 
   
   if(!smilestop)
     smilestop = icon; 
   else
     AddToTopList(smilestop, icon);
}



IMGHDR *GetSmileByItem(int curitem)
{
  return GetPicByItem(smilestop, curitem);  
}

void FreeSmileList()
{
  FreeList(smilestop);
}


void ProcessNextSmile(void)  
{
  int c;
 
  DYNPNGICONLIST *dp;
  S_SMILES *si;
  STXT_SMILES *st;
  char *buf;
next_s:
  buf=p_buf;
  if (!buf) return;
  while ((c=*buf))
  {
    char *p;
    if ((c==10)||(c==13))
    {
      buf++;


 
     
      p_buf=buf;
      goto next_s;
    
    }
    p=strchr(buf,':');
    if (!p) break;





 


    buf=p;
    dp=malloc(sizeof(DYNPNGICONLIST));
    zeromem(dp,sizeof(DYNPNGICONLIST));
    dp->icon=GetPicNByUnicodeSymbol(n_pic);
    dp->img=GetSmileByItem(cur_smile++);
    LockSched();
    if (SmilesImgList)
    {
      dp->next=SmilesImgList;
    }
    SmilesImgList=dp;
    UnlockSched();
    si=malloc(sizeof(S_SMILES));
    si->next=0;
    si->lines=0;
    si->botlines=0;
    si->uni_smile=n_pic;
    if (s_bot)
    {
      
      s_bot->next=si;
      s_bot=si;
    }
    else
    {
      
      s_top=si;
      s_bot=si;
    }
    n_pic++;
    while (*buf!=10 && *buf!=13 && *buf!=0)
    {
      buf++;
      int i=0;
      while (buf[i]!=0&&buf [i]!=','&&buf [i]!=10&&buf[i]!=13)  i++;
      st=malloc(sizeof(STXT_SMILES)+i);
      strncpy(st->text,buf,i);
      st->text[i]=0;
      
      st->next=0;
      st->key=*((unsigned long *)st->text);
      st->mask=~(0xFFFFFFFFUL<<(8*i));
      st->key&=st->mask;
      if (si->botlines)
      {
	si->botlines->next=st;
	si->botlines=st;
      }
      else
      {
	si->lines=st;
	si->botlines=st;
      }
      buf+=i;
    }
    
    
  }
  
  
  p_buf=0;
  mfree(s_buf);
  s_buf=0;
  REDRAW();
}

void FreeXStatusesImg(void)
{
  void FreeXIconList();
  FreeXIconList();
}










 
void AddXIcon(IMGHDR *img,int x)
{
   DYNPNGICONLIST *icon = malloc(sizeof(IMGHDR));
   icon->icon = GetPicNByUnicodeSymbol(n_pic);
   n_pic++;

   icon->img = img;
   if(!xiconstop)
     xiconstop = icon;    
   else
     AddToTopList(xiconstop, icon);
}

void FreeXIconList()
{
  FreeList(xiconstop);
}
 
int CountXIcon()
{
  return CountPic(xiconstop);
}

IMGHDR *GetXIconByItem(int curitem)
{
  return GetPicByItem(xiconstop, curitem);
}


int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}










 
void AddIcon(IMGHDR *img,int x)
{
   DYNPNGICONLIST *icon = malloc(sizeof(IMGHDR));;
   icon->img = img;
   if(!iconstop)
     iconstop=icon; 
   else
     AddToTopList(iconstop, icon);
}


void FreeIconList()
{
  FreeList(iconstop);
}
 
int CountIcon()
{
  return CountPic(iconstop);
}

IMGHDR *GetIconByItem(int curitem)
{
  return GetPicByItem(iconstop, curitem);
}



char XST_PNG[] = "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a\x00\x00\x00\x0d\x49\x48\x44\x52\x00\x00\x02\x52\x00\x00\x00\x10\x08\x06\x00\x00\x00\xdd\xd0\x91\x3d\x00\x00\x00\x01\x73\x52\x47\x42\x00\xae\xce\x1c\xe9\x00\x00\x00\x04\x67\x41\x4d\x41\x00\x00\xb1\x8f\x0b\xfc\x61\x05\x00\x00\x00\x20\x63\x48\x52\x4d\x00\x00\x7a\x26\x00\x00\x80\x84\x00\x00\xfa\x00\x00\x00\x80\xe8\x00\x00\x75\x30\x00\x00\xea\x60\x00\x00\x3a\x98\x00\x00\x17\x70\x9c\xba\x51\x3c\x00\x00\x00\x18\x74\x45\x58\x74\x53\x6f\x66\x74\x77\x61\x72\x65\x00\x50\x61\x69\x6e\x74\x2e\x4e\x45\x54\x20\x76\x33\x2e\x33\x36\xa9\xe7\xe2\x25\x00\x00\x1b\x85\x49\x44\x41\x54\x78\x5e\xd5\x9d\x61\xac\x65\xd5\x55\xc7\xdf\x58\x1b\x07\xe3\x07\x3e\xf8\x61\xa6\x0a\xd8\xe9\x88\x0c\xc4\xe2\x74\xd2\xd8\x21\xd5\x90\x19\x89\x3a\xc5\x58\x66\x48\x2a\x43\x1a\x1a\x1e\x0c\x90\x87\x1a\xfa\xd0\x0f\xbe\xc6\x09\xe1\x55\xf9\xf0\x22\x69\x7d\x14\x86\x3e\xc4\x06\x5f\x11\x93\x47\xa3\xe1\x35\x26\xe4\xe2\x20\x72\xfd\xc0\xcc\x25\x05\x73\x8d\x94\xb9\xa6\x9a\x72\xfb\x41\xb9\x34\xa5\xb9\xda\x22\xdb\xf5\xdf\xe7\xfc\xcf\x5b\x67\xdd\xb5\xcf\xd9\xf7\xce\x1b\xa2\x37\x59\x39\xe7\xec\x73\xce\xbe\xfb\xec\xb3\xcf\xde\xbf\xfb\x5f\xeb\xec\xbb\x63\x6e\x9b\x3f\x27\x0e\xcc\x85\x03\x07\x9a\x33\xbd\xe3\xd1\xb9\x1d\x4d\x47\xf4\x56\xe6\x82\xde\xbf\x7b\xa1\xb6\x39\xb7\xfb\xa2\x1d\xf1\xfc\x95\xce\x30\xee\xb8\xf7\xf0\xee\x1d\xa7\x4f\x9f\x0e\xd7\x5e\x7b\x6d\x4c\x9f\xdf\x5f\x3f\xdf\x7e\xd7\x63\x2f\x37\x7f\x7f\x4e\x95\xf4\xe6\xe6\xc3\x47\xe6\x1e\x9b\xb8\x0e\x5d\x0e\x9d\xcf\x52\xb7\x28\x2b\x3e\x9f\xbf\x66\x77\xe3\xf5\xa7\xf2\xf0\xca\x75\xf1\xc1\xe5\x2a\xdf\xb7\xfe\xf1\x73\x55\xbe\xb9\x79\xac\xaa\xba\xba\xdb\xd4\x4b\x32\x8f\xfd\xf3\x61\xee\x65\x75\xed\x76\x5b\x15\x34\x95\xc7\x60\xbc\x56\xbf\xa9\xe5\x39\x7b\x2e\xba\x2d\xab\x4e\xd7\x3a\x83\x70\xdb\xe1\x3d\xf1\xd8\xd4\xba\xae\x2f\x96\x63\x75\x33\x84\xbb\x3f\x51\xb4\x1f\x5d\x77\xb6\x6e\x59\x97\xfa\xf8\xd4\xb5\xa4\xda\x9b\xd7\xce\x6a\x79\xa0\xde\xf8\xb1\xf5\xe9\xa5\x97\x69\xb6\x1c\xf8\xfe\xfd\xfb\xda\x5b\xed\xdd\x5f\xdd\x6a\xf7\xb9\xed\xa3\x29\xd7\x54\x1e\x21\x74\xdd\x7b\xbb\x63\xc7\x35\x59\xf7\x16\xdf\xd9\x1f\xf7\xdc\x3c\xae\xbc\xe8\x23\xd9\x79\xd8\xb2\x77\x37\xb6\xda\xdc\x35\xc7\xea\xed\x6c\xe6\xfa\x38\xd4\x29\xca\xf9\xdc\xe1\x5a\x1f\x54\x7d\x37\xf7\xdb\xc2\xc8\xf1\x5e\xdd\xb2\x1c\x6c\x53\x68\x43\xab\x83\x41\x55\x17\x2f\x1f\xdb\x13\x4f\xdb\xf9\xf5\x30\xf7\xd0\xee\xa2\x1d\xdb\x4f\xf2\x5a\xe6\xc3\x64\x9d\x3e\x36\x5d\x1e\xb3\xb6\xf5\x8d\x8d\x43\xee\xfd\xf4\xca\x7f\xec\xd8\x73\x53\xf7\x63\xbf\xf5\xeb\x3f\x5e\xcb\xff\xc9\xaf\x7f\x7f\x6e\x87\x7c\x90\xff\xb4\xf7\xf6\xc0\x81\x03\x31\xaf\xb3\x67\xcf\x4e\x55\x0e\x19\xf7\xc2\xd9\xb3\xe9\xb1\x65\xda\x72\xa0\x0c\x52\x90\xda\x75\x3d\x7f\xfa\xf4\x1c\xc7\x39\x5d\x77\x27\x4f\x9e\x74\xeb\xf7\xbe\xfb\xee\xcb\x7e\x5e\x3e\xfc\xf9\x1f\x73\xf3\x78\x65\xe9\xbf\xb3\xf3\x68\xef\x85\xb6\x8e\x98\xa5\x3e\xbc\xb6\x7e\xcf\x5f\x5c\x36\x77\xe9\x8d\xf7\x4e\xf3\xd5\xd5\xb1\x7f\xfd\x6b\x1b\xfe\x73\x3b\x53\x6e\xdb\x70\x12\x00\xea\xd4\x89\xb9\x80\xa5\xb6\x53\x65\x3a\xf6\xd5\xd6\x65\xdb\xfb\x5a\x40\x54\x08\xeb\x62\x2b\x62\x0b\x62\x87\x02\xd2\x06\xa3\x50\x33\x40\x14\x3f\x58\xc7\x4d\x41\x7e\x78\xd0\x7b\x6b\xcd\xd6\x06\x5a\x6d\xd5\x01\x88\x0a\xfd\x61\xc0\xd2\xbb\xb1\x4c\x03\x3c\xd1\xaa\xc2\xca\x8a\x4e\x4f\x75\x84\x6d\x65\xc0\x7e\x80\x80\xfe\x68\x30\x60\x7d\x34\xe5\x03\x88\x0a\xbd\xfd\xa1\xbf\x3a\x17\x36\x97\xe6\x02\xb6\x7b\xf2\xe0\x86\xf9\xe2\xba\x1a\xf3\x00\x04\xd0\x1a\xbe\xc4\xe6\x81\xba\x87\x01\xa4\xac\xed\x3a\xba\x1c\x60\x16\xb2\xbc\x72\x00\x9e\xf8\xb5\xa9\x75\x5d\x2c\x9b\x47\x13\x44\xb1\x6e\xf5\xf9\x00\x2a\xb7\x1c\xf3\xc5\xf5\x84\x9e\xb4\x09\x65\x48\x5b\x93\x7d\xc9\xf6\x21\x75\x77\x7c\xb5\x1b\xed\xe0\x82\x0c\xf0\x84\xaa\x54\xba\xca\x48\xb7\xf5\xd5\xe3\xc5\xf7\x6b\x43\x5a\x93\xd9\x7b\xbb\xf3\xf8\x72\x98\xc6\x58\x14\x5b\x1f\xe3\x71\x27\x68\xab\xda\x87\x5c\x53\x2d\xdd\xb9\x16\x26\x01\xa0\x72\xac\xe9\xde\xb2\x1f\x60\x9d\x60\x5b\x43\x94\xb7\x9d\xf3\xbc\x4c\x34\x73\x81\x24\x3c\x2b\x78\x66\xe6\x64\x7d\x22\x0f\xec\x4f\x7c\x70\x7c\x34\xf3\xd1\xf7\xd6\xde\xd7\xd8\xb7\x49\x7e\xb0\x05\xe9\xfe\x68\x4d\x7d\x50\xdc\x07\x80\x12\x1b\x86\xa3\x13\xc6\x7d\x6d\x79\xb0\x2c\x21\xec\x97\x6f\x9f\x34\x5d\xd7\x5e\xfb\x00\x48\x85\xb0\xd6\x6a\x16\xb8\x72\xee\x0b\x20\xea\xbb\xaf\x7d\xae\xb2\xff\xe8\xdd\x10\x90\x16\xce\x9c\x6a\xef\xc7\xca\xc2\x02\x9e\x60\x77\xdc\x75\x67\xb8\xe3\xce\xbb\xe2\x3a\x81\xaa\xb5\x2f\xac\xf2\x98\x0b\x67\xce\x9c\x49\x02\x63\xce\xb5\xa4\xba\x52\x02\x95\xcd\x03\x00\x05\x4b\x7d\xb8\xbf\xe9\x79\x01\x40\xc1\x7a\xbd\x5e\xcd\xfa\xfd\x7e\x80\x71\xbf\x97\x07\xca\x35\xad\x79\xed\xe3\x94\xe4\x43\xf3\xea\x20\xb5\x1f\xf5\xb1\xff\xb6\x7f\x0d\xef\xbe\xfb\xee\xd4\xf6\x2f\x57\x5e\x96\xdd\x3e\x6c\x99\x76\x0a\x63\xc0\x52\xf7\x6b\xa6\x74\x82\x13\x60\x29\x9c\x39\x51\xb3\x1a\x54\x01\xa6\x0c\x6c\xd9\x2f\x2c\x40\x6a\x45\xc6\x24\x2c\x8f\x88\x5d\x1c\xc2\x48\x3a\x10\x49\x07\x30\xf5\x05\xa8\x60\x04\x29\x9c\x4f\x90\xca\x81\x28\x42\xd6\xac\x30\x45\x88\x0a\x6b\xd2\xe8\x1a\x40\x0a\xb0\x64\xe1\x09\xdb\x5e\x7a\x5b\x27\xe6\xdd\x14\x42\x14\x3b\x56\x2c\x37\xe4\x97\x2b\x01\xa1\xed\xa1\xf5\x20\x8a\x83\x82\x06\x29\xaf\x33\x9f\x26\x4d\x97\x23\x02\xc7\x7a\x61\x16\xa6\x00\x50\x4b\xeb\xbd\x68\x16\xa6\xda\xae\x25\xa7\xd1\x22\x0f\xc0\x10\x8e\xd5\x10\x75\xe8\xe8\x52\xb0\xc6\xfc\x78\x1c\xcf\x4b\x81\x14\x00\x0a\x40\x44\x90\xe2\xfa\x85\x04\x29\xde\x03\x00\x53\xfc\x85\x31\x42\x7b\xeb\x8b\x75\x2a\xa8\xd2\x30\x65\xef\x19\xaf\x05\x00\xd5\x19\xcb\x59\x53\x18\xce\xb1\x83\xcb\x78\xbc\x29\xb0\xb4\x65\x80\x28\xb6\xa7\x08\x1a\x11\xa6\xd4\xfe\xb2\x92\x75\x9d\x7a\x00\xa5\x61\xcc\xee\xf7\x3a\xe4\xc8\x0d\x46\x91\xc6\xf6\xb6\x83\x94\x82\xa8\x35\x0f\xa4\x4a\x88\xea\xc8\x2d\x81\xa1\x0d\x79\xeb\x16\xa8\x52\x20\x85\xeb\x82\x32\xa5\x9f\x77\xac\xb7\x01\x4c\x0a\xa0\x2c\x54\x45\xa0\x52\x9f\x89\xe7\x36\x01\x50\x1a\xaa\x36\x36\xf6\xc5\xf2\xbc\x97\x20\x65\x21\x4a\x03\x15\x55\xaa\xb6\xfe\x03\xc0\x74\xeb\x6d\xb7\x89\xdd\x1e\x6e\xbd\xfd\xb6\x30\x2f\x16\xb7\x65\x49\x98\xb2\x79\x40\x7d\x4a\xd9\xee\x67\x9e\x71\x07\x58\x9b\xc7\xd5\x57\x5f\x1d\x1e\x7e\xf8\xe1\x80\xa5\x36\xdb\x9f\x69\x55\x4a\xe7\x61\x01\xea\xcc\x99\x03\x02\x72\x5b\xa6\xc7\x1f\xad\x58\xe9\x3c\x34\x40\x11\x98\xec\x12\x30\x35\x90\xb6\xa7\x15\x2b\xe6\x31\x2d\x44\x79\xd7\x02\x48\x92\x82\x17\x56\x02\x95\xae\x83\xa6\xfd\x16\xa4\xde\xf8\xaf\x1f\x86\xce\x99\x37\xc3\x8d\xbf\xf7\x8d\x70\xf8\x96\xaf\x85\xbd\xbf\xfa\x42\x34\xac\x23\xed\xd1\xa7\x5f\x0b\x38\xe6\x07\xef\xfc\x67\x48\x81\xd4\x1b\xeb\x3b\x03\xcd\x1b\x5b\x00\x50\xbb\x43\x09\x52\xd7\x6c\x23\x4c\x45\x35\xca\x28\x51\x16\xa8\xb0\x6d\xd5\x2a\x6c\xb3\xa0\xe8\x6c\xa3\x1a\x02\x8b\x00\x75\x30\x84\x71\x01\x51\x61\x30\x17\xd6\x8f\x22\xbd\x00\xa8\xde\x70\x1c\x0d\xeb\x25\x44\xfd\x04\x2a\x54\x83\x54\x18\x4b\xcf\xe0\xda\x42\xa5\x58\xe5\xc2\x14\x3b\xab\x0a\xa2\x36\xa4\x67\x9c\x97\x5f\x57\x62\x16\xa6\xaa\x46\x2a\xfb\xe6\x72\xcc\xb9\x53\x6d\x0f\x3e\x61\x00\xf5\x31\x0b\x48\xb9\x10\xb5\x2c\xea\x96\x18\xee\x01\xdd\x7d\xac\xd3\x51\xef\x60\xab\xca\xe7\xa9\x80\xa8\x37\x5e\x0b\x54\xa6\x69\x41\x8a\xca\x54\xdb\xaf\xf4\x14\xd8\xa5\x06\x06\x00\x12\xe1\x69\x63\xf3\x60\xb0\xc6\x7d\x56\xb5\x6a\x02\x29\xab\x48\x61\xbb\x0d\xa4\xa0\x44\xd1\xb4\x22\xe5\xa6\x9b\x41\x0e\xd7\xdc\x11\x25\x51\x5f\xbb\xa0\x74\xed\x17\xbf\xad\x97\xb0\xd1\xad\x8e\xc7\xb5\x10\xa2\x96\x06\x02\xf8\xa5\xa5\x94\x29\x7d\x0c\xd6\x71\x1c\xeb\x63\x3c\xde\x10\x48\xaa\x1b\xae\x87\xcf\x74\x04\x8d\x08\x52\xe6\x18\xb9\x26\xe6\xa1\x21\x49\xc3\x13\x55\x3b\x2c\x3d\xa8\xb2\x40\xa7\x41\x0a\xf0\x04\x23\x48\x71\xdb\x5b\xe6\x3c\x73\x15\xf4\x38\x10\x35\x51\x8e\x12\xa4\x34\x40\x01\xa4\x98\xc7\xc2\xea\x16\x58\x61\x9d\xea\x94\x05\x18\x0d\x4a\x00\x29\x1a\x21\x2a\xac\xed\x8f\xd7\x07\x75\xca\x05\x4b\xa3\x44\x55\x0a\x54\xa9\x52\x11\xb4\x72\x41\x0a\xb0\x04\x78\xb2\x4b\xa6\xb5\x81\x14\x14\xa7\x94\x41\xb1\x9a\x56\x91\x6a\x02\xa9\xd7\x9e\xd9\x15\x95\xa9\xa6\x7b\x0b\x50\xfa\xf4\x67\x6e\x89\x75\x87\xe5\xa7\x6f\xb9\x25\xbc\xf0\x0f\x2f\x54\x4b\x2a\x53\x3a\x0f\x00\x54\x78\xe9\x63\x49\xdb\xb7\x4f\x06\x5a\x07\xa6\x74\x1e\x1a\xa2\x9a\x60\xca\x82\x4a\x1b\x48\x9d\x3a\x25\x63\xb1\x18\x80\xaa\x0d\xa4\x2c\x44\xfd\xf9\x4b\x0f\x06\xcf\x70\x1c\x40\x4a\xc3\xd4\x76\x81\x54\x05\x49\x46\xd9\x42\x3a\xee\x49\xdb\x7e\x82\x14\xc0\x08\x80\x04\x50\x02\x38\xfd\xfe\x1f\x7d\x63\x42\xa1\x02\x48\x5d\xfa\xf3\x8f\x44\x88\x82\x82\xd5\x04\x52\xef\xbc\xf3\x8e\xb0\x47\x6f\x02\xa8\x34\x44\x45\x45\x4a\x40\x0a\xd0\x95\xf3\x63\xbe\xf5\x18\x82\x54\xc0\xcd\x2b\x15\x29\xa4\x69\x98\x2a\xb6\x65\xbf\xdc\x64\x2c\x09\x5e\x2e\x48\x8d\x77\x0a\x04\xc9\x71\x25\x44\x49\x8f\x1c\x41\xaa\x3b\x28\xe0\x09\x4b\x6d\xec\xc4\xe2\x40\x3d\x5e\xa9\x06\x8b\x30\x3a\x2e\x79\x14\x56\x49\xd3\x23\xf4\x2c\x47\x23\x18\xe4\x80\x54\x05\x67\xa5\x3b\x2f\x88\x4b\x09\x6a\x54\xb4\x16\x90\xb2\xee\x48\xbb\x3d\x01\x5a\xce\xaf\x74\xd6\x8f\x07\x0b\x18\xe8\xa1\x42\xd1\x72\x5c\x7b\x29\x77\x1e\x07\x3d\xec\xa7\x9b\x4f\x83\x14\xa0\x20\x57\x89\xc2\xb1\xac\x5f\x0d\x52\x84\x29\xab\x46\xd1\xc5\xa7\x5d\x7b\x4c\xd3\x03\x54\xbc\xbf\xea\x9e\xba\xeb\x83\x23\x61\xb8\xb1\xab\x56\x56\x9d\x07\xea\x28\xd6\x9b\x03\x50\x36\x8d\xc7\xba\x03\x54\x99\x88\x6b\x25\x44\x61\xdd\x6e\xdb\x87\xa7\xd6\xa9\xa7\xdc\xa3\x4d\xe9\x25\x7c\xa0\x2e\x36\x57\x76\x86\x61\x77\xbe\x32\xa4\x8d\x65\x30\xa2\x61\x9f\xde\x0e\xdd\xcd\x78\x0e\x01\xd7\x82\x14\xb6\x8f\x6e\xf4\x5c\xc3\x3e\x0b\x5c\xbc\x96\xd1\x68\x2d\x58\x23\x48\x01\xa2\x08\x52\x13\xc7\x18\x90\x22\x28\x69\x78\x4a\xad\x6b\xb7\x32\xdb\x69\x6e\xfb\x1c\xf5\xa5\xbc\xa5\x0d\x7b\x05\x6c\xb5\x82\x14\x15\x26\xf9\x01\xd7\xc1\x0f\x39\x5e\x97\x72\xcf\xd5\xf2\x68\x51\xa4\x00\xeb\x84\x29\x0f\xa4\xd8\x37\x79\xd7\x44\x58\x04\x44\x75\x97\x8b\xfb\xa9\xdd\x7c\x5b\x3f\xe6\xea\xee\x3c\xeb\xe2\x73\xb7\x9d\x3e\x28\x3e\x77\xa5\x22\x75\xbe\x20\xd5\xe6\xde\x9b\x05\xa4\xe0\xca\xd3\x4a\x14\xd6\x91\x36\x7c\x7e\x6f\x23\x48\x01\x92\x3e\x75\xd3\x4d\x71\x10\xbc\xea\x4b\xdf\x8d\x4b\x6e\x7f\xea\x78\x91\x8e\x25\x8e\xf3\x40\xea\xde\xc7\x3f\x17\x68\x2b\x4f\xde\x1f\xd7\x01\x58\x00\xad\x27\x9e\x78\x62\x02\xa6\x98\x47\x1b\x44\x51\x9d\xf2\x06\x5e\x0b\x52\xe7\xce\x9d\x0b\x6f\xbe\xf9\x66\xd2\xb5\x07\x57\x23\xcc\x53\xa4\x08\x52\x58\xa6\x20\x8a\xe9\x84\x29\xaa\x52\xad\xcf\x4b\x2b\x35\x14\x3f\xa2\x22\x28\x25\xac\x6d\x1f\xfb\x75\xb8\xf6\x00\x47\x50\xa2\x34\x44\x21\x8d\xd0\x04\x70\x22\x6c\x71\xbd\x0d\xa4\x00\x53\xb0\xf1\x60\x3d\xc2\x92\x07\x51\xe3\xde\x42\x1a\xa4\x78\x57\x58\x17\x76\xdb\xd6\x11\xa0\x68\x52\x81\x3a\x50\x53\xa0\x0a\xc8\xa2\xc1\xfd\x27\x30\xe5\xc4\x49\x45\x55\x8a\x20\x35\x94\xf5\x3e\x06\xaa\x02\xa4\x00\x51\x9b\xfd\x71\x65\x70\x11\x21\x1d\xc6\xce\x34\x8c\x24\xae\x6a\x58\x02\x14\x96\x34\x0e\xc0\xd8\x96\xc1\x36\x6c\xd6\x65\x68\xef\xbe\x27\x21\x0a\x6a\xd4\x14\x20\x05\x60\x22\x44\xa5\xd6\xb1\x3f\x82\x95\x1a\x5c\x34\x44\x59\xc5\x67\x75\x41\x94\x23\x31\x0e\xf6\x39\xea\x49\x53\x4c\x14\xdc\x30\x84\x28\xc4\x4c\x61\x5d\x83\x54\xec\xd8\x3b\x75\x5b\x1f\xae\x4f\xa4\xe1\x18\xd6\x1b\x07\x6c\x0d\x50\xda\xb5\xc7\x41\x82\x00\xa5\x5d\x7b\x74\xef\xe1\x5c\xad\x48\x4d\x03\x52\xba\xfe\x90\x87\xae\x2b\x42\xd3\x48\xdc\xc8\xa3\xf1\xa2\xd8\x42\xb9\x5c\xaa\x20\xcb\xd6\x6d\xaa\xf3\xd0\xf0\xa4\xa1\xaa\xb1\x23\xd4\xb0\xd4\xb6\x8e\x8c\x54\x3c\x1a\xca\xb1\x21\x40\x34\x16\x57\x9e\x0c\xe9\x61\x38\x5a\xad\xc1\x54\x01\x54\xeb\x2e\xf8\x02\xae\x70\x2e\x15\x29\x0d\x4e\x16\x96\x08\x4e\x38\xc6\x42\x16\x15\xa9\x91\x7c\x77\xca\xb4\x82\x94\x3a\x06\xe5\x88\x81\xe5\x72\x7d\x2b\x1d\xb9\x1b\x62\x39\x20\xc5\x63\x71\x5e\xf5\xec\xf7\x76\xba\xca\x69\x5f\x7e\x34\xf5\xa5\xaf\xe8\x2c\x17\x6d\x93\x10\x35\x1e\xac\x85\x2c\x90\x72\x20\x6a\xb5\x2b\x31\x7a\x26\xc6\x89\xed\x03\x90\x84\x0f\x5d\x79\x76\x09\x95\x0a\x46\x98\xca\x05\x29\x02\x54\x04\x1b\x81\x28\xaa\x51\x87\x36\x37\x5b\x41\xca\x8b\x93\x7a\x4f\x40\xaa\x6c\xb7\x8c\x91\xca\x52\xa4\x4c\x5b\xf7\x9e\x23\x28\x4d\xda\x00\x4e\xd6\xbe\xf6\x85\x9f\x4c\x82\x14\xe0\xe8\x93\xc7\x8e\x55\x10\x05\x90\x22\x4c\x41\x91\xba\x41\xf6\xd1\x6c\x9f\x1c\x15\xa9\xef\xfc\x55\x04\xa7\xbb\x5e\x79\x25\x3c\xfa\xad\x6f\xc5\x25\x0c\xe9\xd8\x0f\x78\x81\x2a\xa5\x95\x29\xb6\x0f\x2a\x50\x9e\x12\xe5\xb9\xf8\x08\x1a\xb6\x1c\x74\xed\x01\xa6\x60\x84\x26\xbd\x64\xba\x05\x29\x4f\x8d\xf2\x5c\x7b\x16\xa4\x86\xc3\x61\x74\xf1\xbd\x17\x20\xd5\xe4\x36\xa4\x62\x45\x45\x0a\x6a\x14\xdd\x79\x04\x25\x82\x94\x86\x29\xc6\x52\xa5\x5c\x7b\x00\x26\x80\x13\x00\x6a\x6d\x65\x14\x06\x2f\x8e\xc2\x1b\x22\x56\x78\x10\x15\x21\xab\x09\xa4\x70\xc3\x2c\xe2\x36\x01\xa6\x8d\x8d\xda\x52\xa3\xea\xf0\x54\xa9\x52\x51\xb5\x2a\x54\x29\x9b\xef\x57\x3e\x09\x90\x92\x4e\x62\xb4\x4b\x20\xa8\x00\x29\x80\xd2\x46\x6f\x5c\x19\x07\xe3\x20\x83\x2c\xdc\x77\x35\x90\x02\x24\x89\x79\xbf\xe4\xc2\xe0\xa8\xe4\x27\xfb\x25\xc0\xba\x0d\xa4\x2a\x88\x62\xe0\x3a\x7a\x1d\x02\x54\x09\x51\x54\xa4\x5c\x39\x3b\x03\x9e\xb4\x22\x95\x02\xa9\x89\x72\x48\x79\x38\xc0\x13\xa6\x3c\x65\xcd\x36\xf4\xb6\x98\x28\x0d\x51\x9e\x22\x55\x81\x94\x7c\xff\x4a\x7f\x25\xcc\xa9\x65\x6d\xbd\x1c\xac\xb4\x22\x15\x3b\xfe\x32\x36\xca\x5b\xea\xf8\x28\xc2\x94\x8e\x95\x6a\x02\xa9\xce\xe2\x07\x03\xac\x02\x2c\xa5\x48\x35\x81\x94\xe0\x78\x18\x85\xe5\x30\x14\x80\xf2\x6d\x7e\x02\x52\x9b\x3a\x0f\xaa\x51\x5c\xa6\x9e\x97\x2a\x0f\x19\x2c\x96\x37\x87\xad\xa6\x55\x1a\xba\xff\x90\xc7\x7a\x04\xa9\xae\x5c\x43\x27\x82\x14\xdb\x12\x40\x69\x34\x5e\x8f\x86\x75\x9d\x1e\xf7\x0d\x17\xe2\xb9\x2c\x87\x76\xe5\x59\xf7\x9d\xde\x26\x4c\xf1\x78\x76\xea\xa3\xd1\xb2\x80\x94\x6f\x28\x2f\x5d\x72\xa9\x63\x34\x48\xf1\x5a\x09\x49\x16\xaa\x74\xba\x86\xb4\x26\x90\x22\x44\x0d\x36\xea\x20\x05\x88\x02\x50\xb5\x82\x54\x09\x51\x6b\x6b\x6b\x95\x12\x05\x45\xaa\x09\xa4\xe8\xbe\x23\x30\x79\x4b\x1c\x03\x90\xc2\xd2\x82\x94\x7e\x9e\xd9\x8f\xe9\xb6\x4c\x88\xa2\x1a\xa5\x41\x0a\xca\x94\xa7\x48\xcd\x0a\x52\x5a\x8d\xf2\x5c\x7a\x54\xa8\xb4\x62\xc5\xf2\xc7\x72\xa8\x1f\x09\x6d\x6a\x14\xf6\xeb\xe3\xbd\x1f\x95\x48\x4b\x05\x97\x23\x1d\x2a\x14\x00\x8a\x10\xe5\xe5\x01\x88\x3a\x72\xfd\xf5\x71\xec\xb9\xf2\x8f\xcf\x85\x2b\x1f\x38\x17\xae\x90\xe5\x15\x0f\xbc\x2e\x76\x2e\xa6\x63\xff\x91\xdf\xb8\xde\x8d\x91\x8a\x20\xf5\xf6\x37\x23\x48\x69\x88\x8a\x20\xf5\xf6\xeb\x35\x90\x22\x50\xe9\x72\x00\x96\x6c\xff\x60\xe3\xa4\xec\x31\x8c\x2d\xb2\x8a\x14\xc6\x69\x28\x52\x84\x29\x2e\x9f\x7d\xf6\xd9\x1a\x5c\xe5\x82\x14\xe3\xa1\xe8\xc6\xfb\xff\x04\x52\x50\xa6\x00\x53\x1e\x38\x69\x80\x6a\x72\xed\x01\xa4\xa8\x44\xc1\xb5\x07\x90\x4a\x41\x54\x16\x48\x69\x98\x6a\x82\x28\xec\xb3\x8a\x94\x75\xeb\xd1\xe5\x57\x4f\x9f\x04\x29\x40\xd4\xb7\x07\xbf\x23\x70\x04\x45\x49\x06\x47\x80\x94\xc4\x47\x4d\x0c\xbe\x32\x48\x84\x68\x12\xd7\x23\xc0\x55\x07\xa9\xf9\x22\x26\x42\x58\xd0\x5a\x4a\x26\xf7\xae\xcf\x02\x0c\x80\x0d\xf1\x50\x9e\xa5\x40\x2a\x02\x9b\x18\x06\xb3\xd4\x7a\xfc\xee\x12\xba\xac\x22\x95\x82\x28\xc6\x46\x01\xa8\x52\x30\xa5\x1f\xb8\x9c\x98\x28\xc0\x93\x7e\x7b\x8f\x0f\x7e\xfc\x05\x2f\x31\x52\x1a\xa4\x00\x4e\x16\xa4\x98\x36\xd7\x00\x52\x16\xa8\xb4\x22\x65\x01\x2a\x07\xa4\x34\x44\x55\x30\x95\x09\x52\xa3\xb0\x1a\x0e\x2d\xf6\xa2\x69\x90\x62\xda\x40\xda\x60\x52\x91\x32\x0a\x52\xab\xfb\xc9\xfb\x85\x5d\x82\x54\x93\xfa\xc2\xb7\xf9\xec\x9b\x7d\x11\xa4\xc4\xa5\x33\x1a\x0b\x48\xc9\x9b\x72\x35\x90\x92\x38\xa4\xa1\xb2\x0a\xa4\x90\x06\xe5\x6a\x28\x3f\x3c\xe4\x5c\x0d\x52\x08\x34\x27\x34\xa1\x6d\x01\x96\xec\x12\xfb\xa9\x5e\xe9\x60\xf3\xe1\x70\x51\xf2\xf4\x4d\x83\x54\xea\x18\x0d\x52\x8c\x93\xd2\x83\x69\x0a\x9e\x74\x4c\x55\x0a\xa4\x34\x44\x69\x45\x8a\x10\xd5\x0a\x52\x8e\x12\x05\x88\x6a\x02\x29\xaa\x51\xa8\x3f\x1d\x53\x65\xd7\x37\x25\x9f\x35\x09\x51\xa0\x2a\xa5\x63\xa4\x52\x20\xa5\x95\x28\xed\xd6\xbb\x50\x20\x65\x21\x2a\xf5\xc6\x9e\x4d\x67\xd0\x79\xed\x87\x47\x9b\xea\x9a\x70\x67\xdb\x1f\x2f\x80\xa5\x37\x5f\x3d\x31\xe1\xca\xa3\x1b\x4f\xab\x54\xec\xd7\x75\x1e\x80\xa8\xc3\xd7\x5d\x17\x41\xe6\xf2\xcf\x3e\x1f\x2e\xff\xec\xe9\x70\xf9\x3d\x58\x96\xeb\xb2\xdc\x7b\x4f\xf1\x06\x38\x8e\xf3\x82\xcd\x23\x48\xfd\xd3\xcd\x95\x22\x45\x35\xea\xec\x5b\x6f\x85\xf0\xea\xcd\x35\x90\x42\x3b\xa0\x2a\x65\xaf\x25\x7c\xf3\xc5\x40\x6b\x1a\x63\xdb\x82\xcd\x01\x52\x84\x29\x02\x14\x95\xa8\x36\x45\x4a\xbf\x95\x07\xa5\xc9\x6e\x6b\x90\x82\x1a\x35\xab\x22\x15\xde\xff\xfe\xca\x85\x67\xef\x4b\x93\x0b\xcf\x53\xa5\xa8\x46\x71\x8c\x02\x40\xbd\xfe\xc6\x38\xba\xf5\xda\x40\x4a\xab\x55\xd6\xb5\x07\x88\x1a\x6d\x5e\x53\x53\xa3\x9a\x20\x0a\xa0\x45\xb7\x5f\xf2\xfe\x9d\x8f\x22\xb5\xe5\xe6\x13\x45\x2a\xbe\xc9\xb7\x15\x3b\x55\xec\x2b\x62\xa5\xb4\x6b\x0f\x10\xf5\xea\x99\x3f\x28\xe1\x48\x94\x23\x82\x14\x60\x0a\xeb\x70\xcd\x01\x9c\xc6\x12\x99\x19\x4d\xe4\xf3\x91\x04\xa4\x97\x81\xe8\x55\x67\x3a\x68\x07\x29\x5e\x5b\xe9\x7a\x7a\x9f\xfd\xd5\x17\xd9\x26\xc6\x5b\xe1\x3b\xb6\xe2\xac\xbc\x75\x1b\x6b\xb5\xf5\x6b\x30\x0d\x4f\x1a\xaa\xa6\x01\xa9\x18\x3c\x2f\x2e\x49\x0e\xf0\x58\x8f\x80\xe7\xc4\x7b\xb1\x1c\xb9\x31\x51\x16\xa2\x52\x20\x45\x35\xca\x03\xa9\xb8\x6f\x4a\x90\x22\x30\xcd\x0a\x52\xb8\x27\x35\x55\xaa\x21\x46\x4a\x83\xd1\x50\xee\xed\x50\xea\x2e\x65\x83\xd1\x51\x1f\xa4\xca\x0e\x9f\x00\x14\xdf\x4e\x2b\x3f\x4c\xab\x5c\xe1\x6a\x2a\x03\xc2\x81\x55\xa4\x72\x40\x0a\xe7\xea\x29\x12\x90\xc7\x9a\xc0\xd0\x50\x5c\x62\x43\x81\xa9\x81\xbc\x0d\x97\x65\x12\xcb\x34\x90\xe7\x03\xe7\xa6\x40\xaa\x49\x95\x22\x4c\xd5\x41\x4a\xea\x50\xe0\xac\xa6\x24\xa8\x41\xd1\x06\x89\xe3\x58\x6d\x29\x90\x3a\x38\xbf\x12\x60\x3a\x5f\x9d\xd6\x06\x52\x16\xa2\xb0\x6d\x5d\x7b\x8d\x20\x95\x50\xa2\xa0\x4c\x79\x6a\x14\x9f\x17\x82\x14\xde\x2c\xee\x96\xd0\x45\xf8\xd2\xcb\x0d\xbc\x2c\x93\x00\x29\xdb\x29\xb3\x7f\xa2\x12\x65\x21\xea\x42\x82\x94\xf7\xe3\x33\x37\xcd\x82\xd4\xc2\x5a\x3f\xb4\x59\xf5\xd2\x45\x59\x09\x1e\x48\x79\x31\x51\x80\x2b\xad\x42\xe9\x3a\x64\x1e\x80\xa2\x8f\xff\xf2\x2f\x45\x48\xfa\xd0\x4d\x5f\x09\x7b\x6e\x7a\x3c\x7c\xe8\xe6\xc7\x65\x89\xf5\x2d\xc3\xfe\x3f\x7b\xfc\xf1\xe4\xf4\x07\x05\x48\xdd\x24\xd0\xa4\x0c\xdb\x65\x9a\x76\xed\x59\x90\x7a\xe9\x9e\x4f\x54\xf0\x64\x41\x4a\x6f\xff\xcf\xe9\x07\x2a\xd5\xaa\xcd\xb5\x97\x02\x29\xed\xee\xf3\x14\x29\x40\x13\xa6\x3c\xa0\x0a\x45\x88\xb2\xf1\x52\x00\x2c\x42\xd4\x68\x34\xca\x76\xed\x69\x78\xd2\x40\xe4\x01\x6e\x2e\x4c\x69\x88\xd2\x20\x05\xd7\x1e\xdf\xce\xf3\xa6\x42\xd0\xf1\x51\xa9\x18\x29\x80\x14\xe0\x88\x81\xe6\x4f\x3f\xfd\x74\x5c\xd7\x6f\xe8\x71\x1f\x8e\x6b\x05\xa9\xf3\x8f\x91\x2a\x5d\x7a\x65\x60\xb9\x0e\x32\xb7\x01\xe7\xac\xd4\xf8\x56\x9e\xb8\x08\x42\x0c\x06\x17\x25\x67\x78\x71\xf1\xa6\x9e\x2c\xe3\xf6\x48\x5c\x72\xda\x3d\x84\x34\xa8\x55\x65\xfc\x54\x1b\x48\x55\x9d\x91\x5c\x5c\x84\x24\x15\x84\xce\x60\x74\xfd\xf0\x15\xc7\x2c\x34\x07\xae\x23\xde\xca\xc4\x5a\x4d\x0d\x52\xe5\x9b\x7d\x9e\x6b\x2f\x09\x73\x00\x28\x9a\x5c\x47\x0a\xa4\xa6\x89\x89\xa2\x3b\xcf\x76\x40\x17\x5a\x91\xb2\xf1\x51\x1a\xac\x18\x27\x75\xbe\xae\x3d\x3e\x70\x58\x56\x01\xe4\x32\x50\x42\x75\x82\x41\x0d\xd0\x4b\xae\xbb\xc1\xe6\xca\x5d\x45\x48\xc0\xf3\x82\x75\x9c\xb7\x26\xb1\x2b\x71\xfa\x03\x28\x12\xce\xb1\x33\x81\x14\x0a\x6e\x54\xad\xd5\xa5\x8b\xe5\xfb\xa4\x13\x1c\x75\x63\x30\x77\x7f\xd4\x69\xb4\x78\xcc\x70\x35\xf4\x05\xa4\x70\xee\x2c\x8a\x94\x07\x52\x03\x71\x97\xa3\x6c\x9e\x9b\x52\x5f\x3f\xf7\x23\x0d\xe7\xd0\x52\x20\x75\x64\x71\x3d\xd0\x08\x50\xb9\x20\xe5\x41\x14\x9e\x91\x6c\x90\x9a\x52\x89\xd2\x03\x03\x41\x0a\xb3\x9f\x78\x00\xc5\xb4\x75\x81\xa8\x25\x89\xf7\xa4\x7b\xcf\x1b\x5c\x98\xd6\xa4\x44\x01\xa2\xe6\x65\x30\xd4\xf3\x4a\xe5\xb8\xf6\xf4\xdb\x7b\x9c\x0a\x41\xbf\xb9\xa7\x01\x26\x7e\xbf\xee\x73\x1a\xd6\x5d\x75\xbe\x6c\xbf\x80\x28\x7c\x8e\x2c\xe1\xed\x52\x7f\x39\x2b\x48\x01\xae\x72\x40\xea\xa3\x1f\xfb\xc5\xf8\x56\x1e\x8a\x74\xd9\xe1\xfb\x43\xcd\xae\xbb\x3f\xa6\x3f\xfa\xe5\x2f\xd7\x20\x4a\xf7\x1f\x58\x8f\x20\xf5\xbd\x7f\x96\x69\xaa\x64\x9c\x7b\xbb\x5c\x7e\xaf\x5f\x6c\xcb\x32\x07\xa4\x4e\xfe\xf6\xad\x01\xa6\xe1\x09\xeb\x7f\xf8\x99\x5f\x89\x06\x90\x02\x74\x59\xa8\x4e\xb9\xf6\x66\x51\xa4\x38\x4f\x14\x97\x9e\x3b\xcf\x42\x54\x0e\x48\xa5\x00\x4a\x03\xa1\xad\x53\x6c\xb7\xc1\x94\x85\x28\x0b\x52\x50\xa6\xf0\x56\x1e\xd4\x29\x0d\x53\x80\x2c\xbc\xc5\x47\xc5\x8a\x6f\xf4\x79\x8a\x94\x06\x25\x80\x14\xe2\xa3\x00\x4c\x78\x3b\x8f\xfb\x18\x80\xde\x0a\x52\xf6\xe6\xe5\x6c\x17\x71\x52\xe5\x5b\x79\x02\x50\xde\x54\x07\xf1\x8d\xbd\xd2\x6c\x7c\x54\x01\x4d\xa2\x44\x45\x40\x12\x78\x02\x24\x49\x5c\x43\x4c\x1f\x20\x5e\x0a\xae\x3e\x01\x08\x2c\x01\x54\x25\x44\x05\x09\x6e\xce\x71\xed\x15\x32\xf5\x16\x44\x69\x98\xf2\xe2\xa5\xe2\x7e\x1d\xac\x2e\xeb\x71\x6e\x2b\x9d\x86\x98\xab\xf3\x04\x29\xfd\x26\x9f\xe7\xda\x9b\x05\xe6\xd8\xc0\x00\x52\x5a\x69\xd2\xf3\x44\x79\x31\x51\xde\x43\x9b\x03\x52\x5a\xa5\x4a\x29\x52\x15\xc8\x2a\x18\x46\x1a\x82\xc9\x6d\x8c\x14\xe1\xca\x82\x14\xca\x17\xef\x4b\x19\x07\xe7\x2e\xe9\x86\x34\x17\xa3\x3b\x20\x0c\x5e\x83\xb1\x00\x40\x39\x29\x22\x81\x8a\x00\x85\xf4\xae\xb4\xc3\x38\xc8\xa9\x0f\x63\x3e\x08\x50\x56\x95\x02\x44\xcd\xa3\x8d\x28\x88\xd2\xc7\x32\x30\x3a\x66\x59\xc2\x87\x9e\xea\xc0\x5b\xaf\x0d\x2c\x80\x29\xf9\xa0\x1c\x2b\x02\x43\x3d\x81\x28\xd8\x72\xa7\x80\xa9\x26\xc3\x31\xbd\x81\x4c\xfd\x21\x31\x82\x38\x77\xbb\x14\x29\xaa\x65\x04\x25\x99\xa7\x36\x82\x24\xb6\x2d\x48\x71\x5f\x01\x53\x87\xa2\xe5\x80\x94\x85\x2a\x9c\x9f\x52\xa4\x52\x10\x95\x0d\x52\x33\x28\x51\x1e\x48\xa1\x0d\x6d\x0a\x2c\xc1\xb8\x6e\xd3\x16\x25\xee\xd3\x73\xed\xe9\x36\x37\x2d\x44\xd5\x62\xa4\xe2\x03\xb3\xf5\xe6\x9e\x85\xa7\x89\xa9\x10\x6c\x5b\x2f\xb7\xcf\xfb\x47\xa5\xdc\x2f\xad\x44\xed\x3b\xbe\x12\x52\x86\xe3\x74\x9b\xf7\x14\x29\xc6\x42\x01\x9e\xa0\x44\xf1\x0d\xbd\x36\x90\xfa\xf0\xfe\x5f\x08\x85\xed\x0f\xcf\xff\xfd\xf3\xf1\x59\xba\xe4\xa3\xbf\x1b\x68\xd8\x7e\xf8\x91\x47\x26\x20\xca\x05\xa9\xef\x3c\x15\x4e\x2e\xde\x28\xc1\xe5\x7f\x19\x4e\xde\x8b\xa5\x6c\x63\x39\x7c\xaa\x15\xa4\x34\x40\xd9\x75\x00\x14\x0c\x30\x75\xbe\x20\x95\xa3\x48\xe9\x98\x28\x80\x94\x76\xed\x61\x9d\x4a\x14\x96\x80\xa8\x26\x90\x6a\x03\xa8\x36\x90\x6a\x82\x29\x0f\xa2\x34\x48\x41\x71\xc2\xd4\x07\x00\x29\xb8\xf7\x74\x4c\x14\xd6\xf1\x46\x1f\xf7\x03\xa4\x6c\xb0\x39\x83\xcc\xb5\xd2\x04\x78\x7a\x71\x5d\xc6\x7d\x99\xc3\x92\x01\xe8\x7a\x3f\xd2\x38\x45\x42\x0e\x23\x65\x1d\x13\xe3\xa4\x00\x49\xe5\x84\x9b\x1a\x9a\xb8\x1e\x63\xa4\xca\x63\xec\x1b\x7b\x15\x30\x0d\x44\x5e\x03\x24\xd1\x04\xa6\xba\x8b\x32\xe0\xae\x0a\x5c\x71\x10\x2e\xdf\xe4\x03\x44\x11\xb6\xf0\xc0\x89\xfd\x28\x7f\x3d\x79\xf2\x73\xfc\x55\x55\x4e\x85\x50\x73\xd3\x39\x6f\xf0\x15\x03\xb6\xf4\x42\xd5\xa0\x2d\xd0\x04\x70\x12\xab\x06\xf3\xbe\xcc\xd6\x7b\x9e\x20\x55\x9b\x02\xa1\xac\xe9\x9a\x02\x33\x03\xcc\x69\x90\xa2\xd2\x84\x39\xa2\xf4\x3c\x51\x5e\x4c\x54\x0e\x48\x2d\x76\x16\xa3\xfb\x0e\xa6\xd7\x99\xe6\x81\x14\xf2\xd5\xf3\x48\x31\x56\xca\x82\x14\x81\x4a\xbf\xb1\x67\x3b\x31\xc2\x14\xa6\x38\xf0\x4c\xff\x22\x9e\x80\xa0\x32\x01\x83\x57\x6f\x24\x33\xe6\x8b\xca\xd9\x1f\xd7\x0d\xe9\x80\xa8\xcd\xc1\x07\x5b\x41\x0a\xe0\xa3\x55\x29\x0f\xa4\x74\x9c\x93\x05\xa9\x94\x3b\x6c\x22\x3d\x01\x85\xcb\x8b\x02\x53\xa2\x46\x74\x86\x05\x4c\xc1\x86\x87\x0e\x45\x03\x54\x61\xc9\xf4\xce\x60\x35\x0c\xd6\xd7\x03\xce\xd1\x75\xea\xcd\x25\x95\x13\x74\x5e\xe5\x51\xaa\x6e\xb8\x4e\xc0\x13\xeb\x83\x20\xc5\x6b\x71\xf7\x95\x50\x18\x2f\x4f\xc1\x11\xd6\x35\x3c\x35\x81\x14\xcb\x11\xdb\x94\xbc\xb5\x87\x58\x28\x04\x96\x63\x09\xa8\xd2\x6f\xbb\xb6\x2a\x52\x33\x2a\x51\x1e\x48\x6d\x94\x00\xa5\xe1\x89\x50\x85\xb4\x25\x81\xa8\xe3\x2d\x20\x95\x82\x28\xc0\x12\x95\x28\xab\x46\x4d\x3c\x2f\x89\x19\xcd\xa7\x9e\x90\x73\x86\x7e\x48\xab\xaf\x39\x6a\x14\x55\xaa\x36\x90\xc2\x8c\xe5\x80\x29\x80\x93\x17\x5c\xee\xf5\x63\x70\xeb\x5d\x71\xd5\x95\x51\x8d\xfa\x39\x59\x5e\x71\xd5\xbe\xf0\x77\x08\x86\x2f\x61\x0a\xcb\xd5\x87\x1e\x72\x21\xca\x05\xa9\x33\x1f\x2f\x14\x28\x67\xd9\xa4\x48\x21\xaf\x94\x7b\x0f\x8a\x14\x41\xea\x86\x2f\x6d\x84\x13\xbd\xef\x47\x4b\xf5\x63\x7c\x6b\x6f\x96\x18\x29\xe4\xa9\xe7\x87\xd2\xf3\x44\xd9\xb8\x28\x02\x14\x21\xca\xeb\x93\x73\x21\x2a\x15\xef\xa5\xaf\xd1\x2a\x53\x29\x88\x62\x39\x38\xb3\x39\xe0\x08\x90\x04\x98\xc2\x04\x9c\xff\xfe\xed\x1f\x4e\xcc\x25\x85\x7d\xb9\x20\x05\x50\xc2\x5b\x79\xd1\x4a\x68\xa2\x0a\x95\xe5\xda\xb3\x0d\x31\x77\x3b\xc2\x94\x80\xd4\x99\x72\xa9\xd7\xa3\x62\x55\x9a\xf7\xb6\x5e\x04\x29\x71\xd3\x55\x06\x58\xf2\x0c\xc7\x74\x21\x35\x6f\x41\x94\xbd\xb1\x29\x29\xba\x4a\xb7\x71\x4f\x89\xa9\x10\xe2\xf1\x5d\x89\xc3\x2a\x55\x0e\x0d\x67\xa1\x2b\xca\x59\x67\x72\x0a\x85\xa9\x5d\x7b\x4e\xe5\xd6\x40\xaa\x23\x71\x5a\x0e\xcc\x55\x69\x0e\xcc\xe9\xfa\xe0\xdf\xbe\xd8\x79\xa2\xbc\x98\x28\xaf\x03\xa2\x22\xb5\x1d\xf3\x48\x69\x80\xd2\xb3\x9b\x6b\x80\xd2\x41\xe6\x7a\x42\x4e\x0f\x8c\x73\xd3\x6c\xfb\x00\x48\x75\x04\x96\x3a\xa2\x70\x62\xa9\x03\x81\x01\x50\x1b\xa2\x80\xc2\x72\x14\x29\x0d\x53\x04\x29\x2a\x31\x7a\x1f\x01\xc3\xfe\xc2\xae\xea\x5c\xb9\x01\xdb\x9e\x37\xe6\xb1\x24\x50\xb4\xd9\xef\x4a\x59\x37\xc5\x3a\x15\x48\x41\x79\x04\x48\x61\x09\x90\xda\xe8\xaf\xca\x1b\xaf\x0b\x02\x16\xeb\x01\xe7\x58\x90\x4a\xcd\x1d\xd5\x34\xa7\x54\xad\x4e\x0d\x4c\xe5\xbc\x8d\x48\xd0\xa2\xca\x47\xe0\xf2\x02\xce\x2d\x58\xd6\x26\xef\x2c\x61\x8c\x20\x95\x82\x28\xad\x48\xe1\x4d\x3d\x6d\xd5\x3c\x52\x02\x38\x74\xbd\xf1\x2d\xbd\xa6\x98\x28\xef\x79\x41\x1a\xda\xcd\x9a\xb8\xed\x60\xeb\xc6\x90\xb6\x0c\x88\xea\x2a\xb7\x9e\xfa\xbb\x98\x18\xa2\x20\x70\x8c\xeb\xc1\x5b\x79\xda\x90\x06\x88\x8a\xfb\x4b\xd3\x2e\x3d\x4e\xca\x39\xd1\xc6\x1a\xfe\x22\x66\x28\xa1\x14\x76\x32\x4e\xb7\x3f\x9d\xa1\x1f\xe2\xbd\xcd\x55\xa3\xa8\x52\x69\x55\xca\x5e\x0b\x21\xca\x4e\x7f\x60\xff\x6b\xcf\xc2\x87\xfe\xcb\x17\xc0\xd4\x9e\xbd\x7b\xc3\x9e\x9f\xdd\x1b\x3a\xcf\x3d\x17\x9f\x87\x2f\x7c\xf1\x8b\x49\x88\x4a\x81\xd4\xc9\x13\x97\x46\x90\xb2\xcb\x1c\x90\xd2\xe5\x23\x3c\x55\x69\x77\x3e\x18\xfe\xf4\xa9\xbf\x09\x4f\xfe\xdb\x0f\xa2\xba\xad\x61\xca\xd6\x07\xff\x02\xc6\x73\xed\x41\x91\x6a\xfa\x8b\x98\xed\x02\xa9\x69\x20\x2a\x07\xa4\x50\x0f\x84\xa9\x26\x88\xb2\x20\x05\xe5\x09\xf0\x44\xe5\x09\xd0\xc4\x99\xcd\xf5\x0c\xe7\x50\xa7\xec\x84\x9c\x50\x9d\x68\x08\x38\x8f\x10\x55\x82\x14\xb7\x19\x3f\x85\x74\xa4\x41\xc5\xc2\x39\xdb\x36\x21\x67\x8d\x26\x05\xa2\x00\x4d\x80\x29\x6b\xf1\xbf\xf6\x9c\x29\x0f\x78\x7e\x84\x29\xa8\x4c\x30\xc0\x92\x67\xa5\x0a\xc5\x39\xa4\xa4\x61\x55\x7f\x4c\xa9\xcb\xd1\x34\xc8\x7a\xfe\xfe\x94\x92\x51\x74\xd0\x8b\x51\x79\xaa\x66\x5f\x87\xb2\xd5\x2d\xa6\x58\x48\x75\xa6\x31\x5d\xbd\xad\xc7\x00\xf3\x89\x40\xf3\x06\x90\x8a\x59\x24\x60\x0e\x70\x97\x82\x39\xfd\xe0\x63\x30\xa5\xe5\xb8\xf3\x6c\x07\x84\xef\x6f\xfb\xef\xc2\xd4\x7e\x9c\x3b\x11\x6b\x51\xfe\x3d\x4c\x0e\x48\xb1\x2c\x1a\x2c\x53\x4a\x54\x9b\x3a\x65\x3b\xa0\x6a\x66\x73\x40\x53\xbf\x6e\xdc\xe7\xde\x5b\x13\xf7\xe4\x81\x94\xfe\x0f\xbd\xa4\x6b\x4f\x67\xce\x3c\x25\xad\x0a\xce\x76\xda\x85\xad\x0f\x6c\x2f\x2e\xee\x12\xf7\x6d\x37\x74\x36\x36\x65\xe0\x96\x37\xf3\x44\x85\xd2\xf7\x7c\x4d\xda\x6e\x57\x00\xaa\x2f\xef\xe0\xe3\x58\x2f\x8f\x69\xfe\x67\x8f\x81\xe6\x76\x70\xd1\xf0\xd8\xe6\xae\xd4\xb3\xb9\xa7\x40\x6a\x9a\xff\xdb\x8b\xf0\x21\x6d\x0d\x8a\x53\x9b\x45\x40\x29\x67\x3c\xd7\xcb\x58\x0e\x01\x1a\x04\x92\x13\xa2\x52\x6f\xe7\xa5\x6e\x0d\xdb\x18\xda\x0f\x14\xa7\x45\x81\xa5\x25\x31\x0d\xea\xf3\xb2\x7d\x54\x5e\x93\x84\x59\x50\x67\x9d\x12\xa4\x6c\x1f\x96\x03\x51\x13\xf7\xa5\xd6\x29\x16\xff\xb9\xd7\xe4\xd2\xf3\xda\xc7\xac\xfd\x10\xef\x2d\x62\xa2\xa6\x35\xaa\x52\xfa\xb9\xe5\x7f\xe7\x35\x3c\x1a\xee\x2e\xe6\x01\x98\xba\xe4\x67\x8a\xff\x57\xbb\xe4\xb2\xcb\xe2\x3a\x96\x7f\xf2\xe0\x83\x8d\x10\x65\xeb\x34\xc6\x48\x09\x40\xa5\x8c\x20\xc5\xa9\x0f\xf8\xff\x7b\x2c\x87\x55\xa4\xb0\xcd\x34\x40\xd5\xed\x9b\xdd\x0a\xa4\x00\x53\x4d\x20\xc5\x0b\x26\x30\xe9\xb7\xf6\x52\x7f\x64\xac\xeb\xd4\xc2\x94\x75\xed\x79\x6a\x54\x6d\x7c\x51\x6f\xe3\x35\xcd\xfb\xd4\x16\x6c\xee\xdd\xb8\x36\x88\x62\x39\xb4\x22\x45\x97\x1e\x80\xca\x1a\x62\xa7\xf4\xdc\x52\xa9\x09\x39\x2d\x54\x01\x9a\xa2\x12\x55\xce\x19\x75\xc1\xe0\xc9\xad\x84\x12\xa8\xe2\x9f\x15\x97\x96\xf3\x00\x70\x82\x4d\xb8\xec\x26\xac\x74\xed\xa9\x49\x38\xdf\x97\x93\xa7\x3d\xc6\x83\xac\xa6\x7c\x52\x50\xe6\x9d\x33\xf9\x6b\x30\xf1\x57\x31\x99\x03\x65\xd5\x89\xad\xc8\x5b\x80\x36\xc8\x73\xbd\x08\x86\x6f\x2a\x87\x85\x28\xb8\xf4\xbc\xc0\xf2\x54\x1e\xb9\xaa\x4f\xea\xb8\x59\x41\x4a\x97\xc7\xe6\x91\x5b\xa6\x54\x1e\x3a\xbd\xe9\xbf\xf6\x6c\x9d\x70\x60\xb0\x31\x52\x1a\xa6\xac\x1a\x75\xa1\x41\x0a\x65\x5c\x58\xd8\x15\x60\xab\xdd\x85\x68\xd5\x3d\x97\x79\xa3\x56\xbb\xc7\xe3\x3e\x58\x4e\x7d\x4c\xf3\x3c\x4d\xb4\x75\xf5\x96\x62\xce\xa4\x9a\x35\x57\x67\xf9\xc5\x39\x00\x15\x27\xf0\x54\x1f\x82\xd4\x2c\xed\x62\x02\x1c\x4a\x98\x02\x50\xa5\xde\xce\x4b\xd5\x91\xae\x0f\xb6\x2b\x42\x93\x5e\xa6\x40\x9d\x03\x83\x55\x99\x72\xb6\x2f\xf8\xbd\x95\x2f\x88\x3f\xea\xa6\xe8\x87\xb4\x6b\x2f\xdb\x8d\xad\x5e\xaa\xa8\x0d\xd8\xe5\x1f\x10\x4f\xd3\x3e\x27\xee\xad\x24\x50\x99\xda\xf5\x53\x1f\x08\xbb\x7e\xfa\x03\x13\x7f\x4e\x9c\x73\x6f\x71\x4c\xd3\x7f\xed\x71\x42\x4c\x1c\xa7\xff\xc4\x58\x83\x54\xca\xbd\x07\x90\x82\x5b\x8f\x8a\x54\x2e\x48\xb1\xdc\x80\x27\x2a\x51\xb9\xd7\xc2\x89\x38\x75\x3c\x94\x8d\x89\xd2\xff\xb3\xa7\xef\xcb\x2c\xf7\xc3\xbb\x2f\xb3\xe6\x83\x3a\xfd\xcd\xbf\x3d\x36\x93\xe5\xfc\x69\xb1\x86\xaa\x0b\xa2\x3c\xcd\x7a\xe1\xb9\xe7\x11\x96\xbc\x65\x6e\x1e\xea\x86\xfd\x08\x94\x2b\xda\xb4\xe7\x4f\x73\xfc\xc4\xe0\x32\xcd\xc9\xe5\xb1\x5e\x1e\xd3\xc0\x5c\xaa\xa1\x43\x91\xa2\xe5\x14\x6b\xbb\xaf\xc5\xbb\x06\x06\x9b\xeb\x00\x73\xac\xbf\x17\x03\x43\x4e\x1d\x78\xe5\xb0\x20\xc5\x79\x9e\xb4\x12\x63\xff\x23\x2e\x9e\x23\x9f\xed\xae\x53\x5d\x3e\x02\x13\xe3\xa4\x3c\x80\xda\xee\x4e\x6c\xa2\x0e\xa7\x9c\x2f\x28\x55\x1f\x29\xa0\xf2\xee\xd9\xb6\xd7\x29\x5d\x6d\x66\xe6\xf2\xb6\xf6\xe2\x95\x63\x1a\x50\xbf\xd0\xed\xa3\xad\xfc\x39\xcf\xdc\x34\xfd\xd0\xb6\xdf\x97\x69\x2e\x40\x1d\x6b\xcb\xa1\xdd\x7c\xb9\x59\x7a\xd7\xe2\xcd\x24\xae\x21\xca\xe6\x6d\xf3\xa0\x12\x65\x97\x74\xe7\xb5\xc5\x48\x79\x65\xf7\x5c\x79\x6d\xe5\xe0\xfe\xd4\x9f\x16\xbf\x27\xcf\x5c\xee\x8d\x30\xc7\xfd\x5f\x69\x63\xff\x0b\xbd\x2d\x6e\x25\x38\x49\x4a\x60\x00\x00\x00\x00\x49\x45\x4e\x44\xae\x42\x60\x82";   
char STATUS_PNG[] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52\x00\x00\x00\xD0\x00\x00\x00\x10\x08\x03\x00\x00\x00\x50\x92\x01\xF2\x00\x00\x00\x19\x74\x45\x58\x74\x53\x6F\x66\x74\x77\x61\x72\x65\x00\x41\x64\x6F\x62\x65\x20\x49\x6D\x61\x67\x65\x52\x65\x61\x64\x79\x71\xC9\x65\x3C\x00\x00\x01\x3E\x50\x4C\x54\x45\x00\x63\x00\x00\x54\x00\x00\x4D\x00\x4A\x29\x10\x00\x99\x00\x00\x5B\x00\x00\x00\x00\x00\xAB\x00\xFF\xFF\x00\x00\xFF\x00\x00\xB1\x00\x00\xB8\x00\x00\xDC\x00\xB5\x7B\x00\x00\xC9\x00\x63\xFF\xFF\xFF\x00\x00\xDB\x00\x00\x59\x00\x00\x84\x00\x00\x9D\x00\x00\x63\x00\x00\xC4\x00\x00\xB6\x00\x00\x07\x19\x68\x5C\x3F\x00\xEC\xEC\xEC\x4F\x00\x00\xF2\xF2\xF2\x00\xC0\x00\x0C\x29\x74\x00\xD5\x00\x63\x63\x00\xF9\xF9\xF9\xF6\xF6\xF6\xFF\x24\x24\x6D\x3A\x11\x83\x00\x00\x00\x43\x00\x00\x80\x00\x69\x48\x00\x13\x40\x85\xFD\x00\x00\xFF\xD1\x25\xFF\xFF\x31\x5D\x2D\x10\xD2\xD2\xD2\xA9\x00\x00\x00\x39\x00\xFF\xFF\xFF\xFF\xE8\x12\x75\x8F\xCF\x1F\x26\x95\xDE\xDE\xDE\xDE\xD6\x19\x36\x37\x36\xFF\xF3\x09\xE2\xE2\xE2\x7C\x45\x12\xE6\xE6\xE6\x9C\x00\x00\x7A\x4C\x22\x4F\x4F\x4F\xC9\xC9\xC9\xD9\xD9\xD9\x16\x4B\x8D\xFF\xDE\x1A\x40\x4E\xAB\x0F\x32\x7A\xF8\xC6\xB5\x78\x51\x00\xFF\xC6\x1A\x0A\x21\x6E\x6C\x6C\x6C\xF5\xC5\xBB\xAE\xAE\xAE\x91\x91\x91\xA6\xCA\xF0\x10\x38\x7F\x87\x79\x72\x1F\x1F\x1F\xFF\xB1\x3E\xF6\xC2\xAE\xF4\xC1\xB0\x5C\x70\xBE\x73\x4E\x00\x5F\x74\xC0\xBD\xBD\xBD\xFF\xEE\x0D\xFE\xC9\xB3\xFF\x48\x48\x9C\x9C\x9C\xFB\xCA\xBA\xBD\x00\x00\x31\x00\x00\x23\x27\x1D\xEE\xBA\xA9\xFF\x6C\x6C\xFF\x94\x94\xEF\x00\x00\x62\x62\x62\xBA\x3C\x00\xCE\x00\x00\x73\x08\x00\x9D\xA3\x9C\xFF\xFF\xFF\x32\x3E\xC3\xD7\x00\x00\x00\x6A\x74\x52\x4E\x53\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\xBC\xE4\xDC\xE3\x00\x00\x04\xBA\x49\x44\x41\x54\x78\xDA\xDC\x97\x69\x7B\x9B\x46\x10\xC7\x17\x56\x2C\x06\x84\x8C\x10\x08\x59\xB2\x25\xD7\x46\x58\x47\x75\xFA\x90\x23\x4B\xB6\xE3\xF8\xB6\xEB\xB3\x69\x52\x1F\x39\x9A\xF4\xE0\xFB\x7F\x81\xCE\xEC\x02\x96\xF3\x54\xE9\xD3\xBE\xC8\x8B\x8C\xB4\xA0\xBF\xE7\x80\x1F\x3B\x2C\x98\x84\xDF\x99\x11\xDC\xB8\xAE\x1B\xDA\xB6\xFD\xF4\x47\x42\x42\x45\x51\x12\x5D\xAF\xD7\xC3\x8B\xFD\xFD\xA9\x5A\xF6\xE5\x50\x0E\x82\xA9\xF9\xDF\x52\x23\x90\x9B\xCF\x7A\x38\x62\x22\x92\x49\xAB\x38\xA4\x48\xBF\xFC\x79\xF5\x97\x3A\x8C\xF2\x14\x2D\x0D\xC7\x43\x19\xE2\x83\x29\xF9\xCF\x34\x7D\xA6\x69\xE2\xCF\x4D\x89\xE7\x9A\x52\x9A\xF9\x9A\x7F\x42\x73\xA0\x6C\x23\x8F\x23\x01\x4A\x6B\xC0\xAC\x28\x09\xD0\xEA\x4F\xAB\x75\x18\xE5\x29\x5A\x1A\xFE\x3A\x94\xD3\x9A\x91\x00\xA5\x1D\xB0\xAA\x91\x1C\x30\x7D\xF8\xBA\x56\xAB\x09\x4D\x29\xD6\x37\x70\x48\x28\xC4\xF1\x8C\x9C\x95\x9B\x88\x8F\xFD\xB1\xA6\x33\xF4\x99\xFE\xD2\x3F\x51\x9F\x40\xBF\xE5\xE1\x03\x5B\x3B\x6B\x97\xF8\xFC\x2D\x10\x72\x7E\xB2\x3D\xD8\x96\x24\x0C\xA9\xBF\x44\xAB\xD7\x2F\xF6\xCB\xE5\x63\xD0\xD9\x06\x58\x16\x2C\x9F\xF7\x3C\xEC\x37\x69\x01\x81\x64\x23\xD0\x82\xAE\xC8\xEF\x7C\xBC\x5B\x5E\x5E\xAE\x6A\x3C\x1F\xF4\xE1\x2C\x5A\x8D\x6B\x6A\x51\x4A\x0C\xB8\x60\x86\x42\xE9\x88\x8A\xE3\x01\x8F\x65\xE5\x92\x78\x62\xC6\x16\x69\x00\x82\xF8\x27\xBF\xC8\x7F\xAA\xBF\xB9\xF9\xB6\xBF\x27\xEA\x13\xB7\xD1\xC8\x3B\xEE\x66\xBF\x69\xC3\x1C\x39\xA5\x90\x68\x1A\xB9\x3F\xDF\xBE\x1C\xBC\x07\x22\x55\x0E\xEB\x3F\x80\x2D\x2D\xCD\xCF\x17\x8B\x5B\x2F\xD6\x0B\x61\xF6\x99\xEE\x84\xB2\xA6\x21\x50\xBF\x89\x3F\xD4\x2E\xE6\x3B\x1F\x7B\xC0\xD3\x5B\xAE\x2E\x40\x3E\xE8\xC3\x59\xC6\xF0\x3B\x87\x9A\x5A\xEF\x18\x25\x70\x3C\x85\xB2\x91\x45\xD1\xBF\xA0\x22\x0F\x10\x45\xF1\x0B\xE6\x8F\xC2\x36\x80\x08\x35\x8D\x2D\xF2\xAB\x3C\x1F\x7F\x88\xFA\x9B\x8C\xC1\xB7\xC9\xEB\x13\xD7\xF3\x1C\xD7\xED\xEF\x1D\xD9\xB6\xE7\x54\x01\x48\x27\xF7\x97\x30\x3D\x57\xEC\xEA\xFD\x40\x12\x40\xC9\xF9\xEF\x72\x20\xB6\xB4\xC4\xE6\x8B\x8C\x81\x06\x20\xDD\x50\x87\x43\xDF\xF7\x83\xC0\x50\x53\x00\xA4\x1B\xCE\xDD\x5D\x0F\x78\xB2\x55\x35\x25\xA3\x7E\x1D\x03\xA1\xA6\x96\xCB\x7A\x14\x8E\x47\x59\xCF\x42\x20\xC8\xCF\xC5\x26\xE2\x23\x9E\x8D\x8D\x8D\x45\x93\xFB\xE9\x8C\x30\x2A\xFC\x2A\x21\x90\xAF\x28\x46\x5C\x3F\x06\xE2\x1A\x97\x38\xCF\x6D\x1E\x5D\xDB\x0D\xBB\xC4\x5B\xEE\xFC\x64\xFD\x6C\x67\x9B\xB1\xAB\xED\x01\x4C\xE1\xC5\x24\x4F\x5B\x00\xB1\x79\x56\x64\x5B\x2F\xD8\x6E\xBB\x03\x2D\x27\xFB\xE3\xCB\xD3\x03\x1F\x3B\x4E\xB4\x9C\xC3\xE7\x27\xEB\x55\xA3\x96\xA8\xCD\x0A\x9B\x13\x2D\x97\x63\xBD\x11\x3D\xA2\x23\x01\x84\x4B\x94\x01\x2D\x85\x9F\x28\xDE\xE4\x30\x1B\x02\x08\xFD\x91\x1B\x02\xC4\x92\x66\x10\x38\x5F\xEC\xB8\xA8\xFE\x5B\xDE\x72\x4D\x5E\x9F\x40\xA7\x65\x3D\x07\x16\x6D\xBE\x2F\xC1\xF2\x77\xD6\x2E\x7E\x46\x9E\xC1\xE5\xB6\x2C\x87\xFB\x93\x3C\x1C\x08\xA6\x87\x01\x10\x58\xBB\xED\x84\x41\x5A\xF3\x0F\x4E\x4F\x0F\xC6\x30\x49\x06\xCC\x90\x92\xD6\x1C\xE4\xE9\x79\x9D\xAA\x01\x57\x0C\x34\x00\x41\x30\x00\x19\x7C\x86\x46\xEC\x37\x6B\x44\x47\x9F\x60\x07\x40\xE0\x4F\x1B\x51\x47\x45\xF1\xE6\x9F\x8B\x91\xE5\x4C\xEE\x4F\x80\x84\x3F\x6D\xAC\xC0\x22\x8D\xFB\xA8\x7E\x9F\xF5\xFB\x7B\x30\x43\xA8\x89\x9D\xCF\xE6\x3B\xB6\x07\x2B\x02\xEC\x11\x28\xA3\x28\x3B\x67\x03\xB0\x93\x7B\x09\x02\xF6\x27\x79\x76\x0A\x61\x5E\xF4\xDB\x16\x5B\xDF\x65\xED\x1D\x00\xCA\x54\x00\x68\xEC\xDF\xF8\x37\x2D\x1D\x81\x32\x95\x2A\xCE\xCF\xEF\x3D\xA7\xAA\xE3\x01\x33\x95\xDA\x2C\xD0\xC0\x98\x43\x4D\x47\x8F\x2E\xDC\x2F\x23\xCB\x72\x1F\x81\x0B\xFD\x19\x5D\xF4\x14\x8D\xE2\x33\x66\xDC\x82\xA6\xD0\x93\x2D\xC7\xE3\x15\x43\xA9\x48\xB0\x8F\xEA\xEF\xED\xC1\x05\x6B\x5E\xF3\xFA\xC4\xEE\x78\x1D\x1B\x96\xEC\x46\xA3\xD4\xC1\x7B\x48\x52\x75\x55\x29\x9E\x9D\x7C\x3E\x39\x97\x52\x2B\x72\x58\x9E\xE4\xE1\x40\x93\x1A\x80\x54\xDD\x1F\x1F\xF8\x1F\x3E\x00\x50\x6A\xA5\xCB\xF3\xAB\xD0\x6F\x50\x4C\xC5\x7C\xD4\xA2\xE7\xE6\x84\xA6\x9F\xDE\xC1\x42\x27\x51\xFA\xC7\xA3\x45\x85\x5F\xA1\x33\x6F\xE8\x1B\x38\x5F\x11\x6F\xCE\x00\x8A\x09\xAD\x65\x42\xCB\xA1\x9F\x03\xC1\x86\x46\xF5\x14\x2D\x23\x69\x9A\xF4\x54\xBF\x09\x76\x2D\xEA\x13\x7C\x45\xC8\xC2\x43\x15\x46\x89\xDF\x43\xD0\x99\x15\x49\xDA\xF9\xEB\x4C\x92\xA1\xE3\xC2\xB0\x1C\xDB\x71\xA1\x50\x00\x9D\x17\xE6\x79\x5E\xA7\xE3\x80\x0E\x82\xC0\x3F\x18\xB7\x5A\xE3\x9B\x16\xBF\x87\x30\xBF\x8A\x66\x88\x7C\xD4\x73\x68\xB1\xE6\xCD\x25\x55\x70\x17\xFB\x23\xA0\x28\xDE\xE4\xD3\x01\x8B\xF6\x22\xDE\x43\xE0\x4F\x80\xE2\xF8\x8A\x81\xE3\x1F\xEB\xE3\x83\xD5\x86\x85\xAE\x84\x23\x7E\x50\xEA\x7A\x0A\xC2\x52\x72\xA4\xCB\xAF\x5E\xAD\xE1\x28\x4C\xD1\x81\xFF\xF0\x70\x3B\xBE\x1D\xB7\x26\xF3\x75\x3D\xC9\xFF\x77\x1D\xDD\x43\xB1\x46\x14\x34\xFE\x1C\x8A\x2E\xC1\x44\xC0\x57\xEB\x21\x50\xC9\x01\x96\x12\xF6\x5B\x14\x90\x4A\x49\xA1\x8C\xF3\x27\xEC\x78\x6D\xED\x38\x2C\xAC\x25\x00\x5F\xEA\x6E\x6B\xFC\x70\x7A\x7B\xDB\xEA\x4E\xC9\xFF\x1F\x3A\x79\xAC\xFE\xE7\x7C\xFE\x72\xCA\x5B\xAD\x54\x7A\x7A\xD9\x94\xE3\x8D\x30\xDE\x6A\x85\xC2\x54\xDD\x6D\x71\x9B\x9A\xFF\x2D\x35\xF9\xDE\xFE\x7D\xF8\x5B\x80\x01\x00\xA1\x57\xB4\xED\x1C\x13\xDE\xD1\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82";

char IMAGE_PNG[] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A\x00\x00\x00\x0D\x49\x48\x44\x52\x00\x00\x01\x00\x00\x00\x00\x10\x08\x03\x00\x00\x00\xCE\x0D\x51\x7B\x00\x00\x00\x19\x74\x45\x58\x74\x53\x6F\x66\x74\x77\x61\x72\x65\x00\x41\x64\x6F\x62\x65\x20\x49\x6D\x61\x67\x65\x52\x65\x61\x64\x79\x71\xC9\x65\x3C\x00\x00\x03\x00\x50\x4C\x54\x45\xE4\x9A\x2D\x52\xAF\x24\xFA\xFA\xFA\x73\x72\x72\xB7\xDC\xB1\x8C\x73\x65\xE9\xE8\xE8\x00\x86\x47\x4D\x92\xD8\x8B\x55\x55\x2E\x99\x06\x9B\xC2\xE7\x49\x9C\x47\xF5\x95\x94\x00\x62\x01\xEC\xD4\x01\xE6\xCF\xE5\x03\xA7\x00\xDA\xA9\xCE\x77\x6F\x8C\x8D\xDB\x3F\xC5\x7B\x73\x84\x78\x84\x7A\x67\x7B\x4C\xA1\x17\xFE\xE7\x01\x00\x50\x00\x41\x3D\x37\xFE\xFE\x72\xF6\xF5\xF5\xC5\xBD\x72\xED\xE9\x8E\xDA\xD8\xD8\xD9\xD5\xCA\xEE\xED\xEE\xFB\xF4\xA7\x85\x84\x7A\xD4\xA3\xA3\xCF\xC8\xBB\x58\x99\x62\x58\xC7\x5A\x98\x8C\x67\xA0\x59\x59\x7A\xB5\x7B\xD3\xCF\xCF\xB5\xC6\xE6\xED\xE4\x4F\xC2\xB8\xA8\x1A\x62\x3F\xFE\xFB\x64\xCD\x94\x94\xE7\xDF\xBE\xFD\xF4\x96\xCB\xB6\xB6\xFA\xF2\x01\x66\x79\xBB\xA5\x84\xA5\x68\x94\x78\x6E\xD5\x70\x00\x6F\xD6\xF1\xF0\xF1\xE9\xAB\x47\xAA\xC2\xE5\x86\x7C\x7C\x00\x9A\xFF\xB0\xD8\x9A\x4E\x4D\x4D\xE0\xDA\x6D\x00\x6A\x26\x41\xBB\x1E\x00\xB4\x00\x32\x7B\x29\x01\x01\x01\xAD\xB6\xDD\xAD\xB5\xAD\xD3\xCC\xC0\x00\x97\x00\xF7\xA4\xA6\xE7\xC3\x8B\xB9\x6D\x6D\xF1\xF1\xEC\xFD\xF5\xA0\xF7\xF6\xF3\x49\xBF\x4D\x00\xC5\x00\x37\x80\x2D\x00\xDA\x00\x54\x74\xB4\xF7\xEA\xDE\x0B\x78\xB3\xE5\xB6\xD5\xFC\xFC\xFB\xFF\xEF\x9C\xFE\xFE\x01\x74\xD7\x4B\xA2\x4E\x4E\xD3\x9B\xC6\x15\xA6\x2E\xB9\xF7\xB5\x03\x87\xBA\xED\xE6\x65\x03\x73\x01\xB6\xB6\xB6\xE5\xE1\xDA\xAE\xD6\x94\x11\x9A\x90\xF8\xAC\xA5\xAA\xD4\x88\xFF\xEF\x94\xEE\xE6\xE5\xED\xE9\x7E\xF7\xB1\x4B\x30\xB6\x15\x2C\x2A\x25\x73\x7B\x7B\x53\x64\x5B\xED\xE9\x01\xD9\xA1\xC9\xF0\xF8\xF0\x9B\xBA\xE1\xFE\xFE\x97\x6C\x64\x7C\x73\x7B\x73\xFE\xFE\x7F\x00\x82\x00\x9C\x73\x6B\x52\xA5\x5A\x8C\x7B\x5C\x53\x64\x53\x00\x57\xC6\x6B\x84\x73\xA3\xBB\xE2\x84\x6B\x84\x94\x7C\x74\x5A\x64\x5A\x31\x87\xBA\xC1\xE1\xC6\xD9\xED\xDB\xCF\xCE\xCE\xDE\xAE\xD1\xEF\xEE\xE6\xE0\xF0\xE3\x3A\x7C\x42\x52\x6B\x52\x5F\x8C\x6B\x74\x74\x79\x43\x7C\x4B\x4A\x73\x53\x52\x6B\x5B\xFF\xFF\xFF\xF0\xEF\xEF\xB5\xB5\xB5\x1A\x64\x9C\xA8\xA2\xA2\x80\x80\x80\xB1\xAC\xAC\x47\x22\x07\xB5\xB5\xB6\xE2\xE0\xE0\xBB\xB7\xB7\x6D\xB5\xD0\xC7\xC3\xC3\xBA\x7F\x29\xB5\xB6\xB6\xAD\xCF\xEF\xDF\xDD\xE0\x97\x96\x96\x60\xCB\x2E\x57\xC5\x08\xE7\xE6\xE8\x00\x5E\x68\x3A\xA8\x03\x1F\x8C\x1F\x01\x7B\xE6\x68\xD3\x36\x00\xFF\x00\x9A\xEA\x48\x36\x62\x78\xF0\xF0\xF0\x7E\x3C\x0B\x78\xE3\x3F\x41\x7B\x1E\xF3\xD2\x9C\x1F\x79\x1E\x26\x72\xC6\x7B\x73\x73\xF0\xF7\xF0\xFD\xFC\xFD\xB6\xB6\xB5\x63\xCB\x1B\xF7\xA4\x9C\xF4\xA0\x37\x74\x6B\x6E\x94\x87\x87\x71\x4C\x4C\x5D\x45\x45\xDC\xB2\xB2\xE2\xBE\xBC\x97\xC3\xDF\x62\x9C\x6B\x1E\x8A\x02\xB6\xD8\xFA\x2D\xA6\x06\xDB\x8F\x25\xE6\xE5\xE7\x93\xBE\xD7\xB2\xA0\xA0\x8C\xDF\x68\x68\xCF\x36\xE4\xE3\xE5\x4F\xC7\x26\x11\xA4\x11\x00\x39\x9C\xEE\xB5\xAD\x60\xCB\xFF\xAD\x8C\xAD\xEB\xEA\xEB\xEF\xEF\xEF\x82\x76\x6B\xC5\xBD\xD6\xDE\xC5\xD6\x7B\xB5\x84\xAD\xEF\x9C\x00\x66\x15\x00\x72\x15\xDD\xBD\xDA\x19\x90\x19\x3F\x8A\x84\x00\x46\xB5\xFD\xEE\xF4\x00\x76\x60\xF9\xBC\xB5\xAF\x5D\x5D\xEE\xDD\xCE\x3F\xA7\x17\xE6\x95\x12\xCE\xCE\x6A\xED\xD8\x3B\xF0\xE7\xCF\xFF\xF7\xF6\x9C\xEB\xAD\x00\x92\xDE\x26\x8A\xCE\x26\xB2\xFF\xEF\xE4\xEE\xF0\xEF\xE6\xE0\xC4\x01\x00\x7A\x3F\x6E\xAE\x42\xB5\xB6\xB5\xEF\xF0\xAD\xFB\xF6\xB7\xE5\xE0\xE5\xF7\xF7\xF5\x4F\x72\x94\xFF\xFF\xFF\x4A\x54\x52\x6F\x00\x00\x01\x00\x74\x52\x4E\x53\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x00\x53\xF7\x07\x25\x00\x00\x06\xAB\x49\x44\x41\x54\x78\xDA\xE4\x97\x79\x5C\x93\x75\x1C\xC7\x67\x03\x52\xC2\xD9\x90\x0D\xD9\xDA\xE2\x52\x44\x40\x85\x8A\x0D\x09\x68\x33\x70\x72\x18\x1E\xD3\x54\xB4\x3C\x98\x49\xA1\x18\xA6\x19\x16\x58\x1C\x1E\xA1\xA0\xFE\x36\x22\x07\xE1\x96\x22\x47\x20\x5E\x05\x6A\x9E\x78\x5F\xD9\x21\x74\xDF\x76\x5F\xDA\xA9\x3D\x7D\xBF\xBF\xE7\xD9\x9E\x67\x20\x56\xFE\xD5\xEB\xD5\xFB\xC5\xB3\x3D\x9F\x67\xCF\x6B\xEC\xFB\x7E\xBE\xDF\xE7\x10\x31\xFF\x73\x44\xDC\xBB\xC1\x60\x10\x6E\x96\x48\x24\x8C\x44\x21\xB9\xC1\x2F\xDD\x88\x08\xB2\xD1\x68\xE4\x5E\x04\xDF\x2F\xE9\xE9\xDB\xC3\x78\x68\xD6\xF2\xD0\xAC\xE6\xA1\x59\xC6\x43\xB3\x86\x07\xE3\x26\xBD\x80\x4D\xDD\xB2\x53\xC0\x36\xA1\x00\xC9\x84\x89\x52\x5C\x14\x37\x56\xFF\x89\xB8\xB8\xB8\x13\x02\x03\xC6\xE6\xFA\x51\xA3\x9A\x6B\x85\x02\xA4\x52\xA9\x43\x80\x58\x00\xD6\x4F\x78\xD0\x80\x56\x90\xD1\x80\x5A\x90\xD1\x80\x4C\x90\xD1\x80\x46\x90\xD1\x80\x5E\x90\x89\xBE\x5B\xEE\x41\xC0\xC4\x4A\x24\xC9\x29\x40\xC9\xB2\xA0\xA7\xEC\x2A\x20\x6E\xE7\xCE\x9D\x71\x1B\xF9\xE3\x3F\xA0\xF6\x68\xAD\xE7\x9E\x7A\x47\x0F\xE0\xD1\x4F\x4C\x4C\xE4\x9A\x40\x2C\xFC\x41\x62\x26\xEC\xD3\xD6\xD6\xD6\x45\xAD\x8B\x80\xD6\x45\xBA\x30\x46\xFB\x5D\x8B\xB9\xC5\x6C\x36\x77\xB4\xC0\xBB\xB7\x96\x51\xAB\x3A\xCC\xB0\xA1\xA3\x03\xFF\x32\xD4\x8C\x4C\x05\x9F\x42\x08\xC4\x2D\x69\x32\x46\x73\xB5\xA5\xA3\x03\xD7\x5B\xCC\x81\xE6\x78\x30\xA0\x9F\x2A\x40\xDF\x2D\x8B\x98\x05\x06\xCA\x36\xF6\x6D\x01\xFD\x81\x13\x2A\xE9\xCF\xA9\x6C\x52\xB0\x0A\x94\xDC\xEF\x1B\xA7\x64\xAE\x99\xBB\x0A\x40\x1C\x02\x8C\xBF\x7A\x19\x3D\x41\x40\xED\x51\x63\x2F\x4F\x6A\xC0\x2E\xD9\xCC\x92\x69\x47\x01\x6D\xED\x25\xED\x6D\x6C\x03\xB4\x81\x00\x52\x5A\x5A\xFA\x34\xA5\xB4\x54\x17\x96\xAC\x25\x50\xD0\x13\x94\x8E\x0E\x6F\xED\x6E\x35\x81\xC2\x1E\xA5\x98\x41\x80\x45\x46\xA0\x5C\x2E\xB7\xA4\xC9\x4C\x1A\x02\x7E\xD8\xFD\xCD\x2D\xF1\x1A\x0B\xA3\x57\xA9\xF6\x9F\xBE\x0B\x38\xBD\x5F\xA5\x42\x01\x2A\x01\x28\xC0\x30\xF2\x76\x00\x04\xCC\x03\x46\x42\x1F\x48\x9A\x9A\xA0\x7E\x90\x01\x06\x26\xB0\x53\xA0\x64\x7D\x8D\x73\x0A\x80\xA0\x54\xF2\xB9\x8B\x80\xF9\xF3\xE7\xC7\x55\x00\xAC\x00\x9F\xDA\x01\x46\x4F\xCF\x51\x5F\x1A\x7B\xD5\x1E\x47\x01\x75\x75\xF6\xCC\x97\x90\x4C\xFB\xEB\x28\xE0\xA6\x92\xC9\x25\x37\x71\xA0\x80\x15\x2B\xCA\x96\xC7\x00\xCB\xCB\x56\x50\x01\x97\x2E\x89\xC3\xE5\x40\xB8\xF8\x92\xB7\xD6\xA2\x26\x97\x9F\x15\x07\x04\x03\x01\xE2\xCB\x19\x6A\x93\x8C\x64\x67\x8B\x53\x12\x80\x94\xF2\xEC\x34\x99\x4D\x03\xB9\x7C\x25\xE6\x95\xE2\xEC\x78\x8D\x95\xD1\x9B\xCD\xFB\x06\x1E\x3A\x7C\xF8\xD0\xA1\x7D\x66\x33\x0A\x30\x0B\xA0\x02\xE6\x3D\x0E\x40\xBD\x79\x79\x79\xB7\xCD\x46\x01\x49\x49\xB4\x01\x50\x80\x34\x95\x15\x70\x15\x48\x4F\x9F\x33\xC7\x21\x00\xA2\x52\xC9\xE7\x2E\x02\x5E\x05\x2A\x8E\x1D\xE3\x04\xD4\x7B\xD6\x1F\x37\x46\x47\x1B\xEB\xEB\x3D\x8F\x1A\xB1\xFE\xAD\xC3\xED\x92\xED\xDB\xB7\x4B\xEC\xCB\x3E\x79\x19\x04\x5C\x3D\x55\x72\x4A\xA5\xC2\x06\x80\x57\x26\x6C\xEA\x8A\x32\x12\xB3\x7C\xDD\xBA\x98\x18\xB7\x32\x56\x80\x98\xC8\x6F\x5D\xBF\x3E\x41\xFE\x8E\x98\x0A\xB8\x5F\x4C\x82\xBF\xCD\xCD\x0D\x0E\xBE\x43\x4C\x3B\x00\xF3\x17\x98\x0F\x8B\xD3\x64\x16\x0D\xB9\x02\xFB\xFB\x17\x17\xCB\xE5\x6F\x8B\x69\x07\x78\x6C\xD1\x47\x45\x0D\x1C\x18\x15\xA5\xF7\xF0\x40\x01\x1E\x02\xA8\x80\xD9\x79\x58\x3A\x10\x19\x19\x39\xD4\x40\x47\xA0\x92\xAD\x9F\x54\x3A\x46\x60\x1C\x3B\xF3\x4E\x01\x5D\x72\x17\x01\x22\x91\xE8\x44\x45\x7B\x7B\x85\xF3\x1A\x00\xF5\x83\x01\xF6\x1C\x50\x27\x19\x2E\xC9\xB4\xBF\x0F\xF5\xC7\xC6\x2E\x43\x01\x77\x0B\xC0\x0E\x08\x69\x23\x47\xDE\x5D\xBB\x76\xED\x11\xE2\xE6\x85\x02\xE0\x24\x21\x7F\x6B\xCD\x9A\x35\x72\xF2\xA1\xAF\xD6\xAA\x26\xAF\x10\x12\xFC\x75\x2E\x54\x4C\x7E\xCF\x47\x01\x90\x13\x64\xC5\xB9\xB9\x09\xE4\x0D\x77\xD9\x6E\x0D\xDD\x7F\x52\x31\x18\x20\xA3\x97\x68\x4C\x8C\xBE\xAA\x4A\x79\xF1\x3E\x44\x79\xA5\x0A\x05\x54\x09\xA0\x02\x86\x6E\x00\x22\x37\x50\xBE\xC1\x0E\x98\xD8\x54\xC9\xCD\x78\x65\x12\xD7\x01\xE9\x5D\x04\xA4\x5F\x4F\x00\xB3\x11\xFB\x1F\x04\xD0\x29\x30\xFA\xF8\x34\xEF\xC1\x0E\x68\x6E\xF6\xC1\x0B\x41\x9D\x44\x92\x99\x79\xB3\xFD\x5E\xFB\x8C\xF3\x93\x63\x97\x81\x80\xC0\x31\xA2\x31\x17\xD8\x73\x40\x20\x76\x40\x88\x1B\x09\x09\x59\xBD\x3A\x24\x84\x13\xB0\x6F\x0A\x29\x94\x17\x14\x14\xCA\xC9\x14\x5F\xAD\x0D\x04\x04\x91\xD0\xD0\xDC\xDC\xD0\x50\x12\x94\xAF\xB6\x82\x80\x2C\x52\x54\x54\x50\x50\x54\x44\xB2\xDC\x71\x04\xC4\xA3\x49\x79\xFF\x82\x82\xFE\x0E\x01\xFE\xFE\x07\x1F\x40\x2E\x1E\xF4\xF7\x47\x01\xFE\x02\xA8\x80\x85\xD3\x28\xD1\xD1\x7E\x7E\x7E\xFD\x0C\xF4\x1A\xE8\xEC\x00\x87\x80\x39\x01\x01\x01\x29\x29\x83\x06\x39\x04\x40\x56\x2A\xF9\xEC\xCA\x8E\x1D\x15\x2F\x70\x80\x00\xE2\x02\x15\xF0\xC1\xD2\x59\xF7\xD8\xC7\xE7\xE4\x9C\x8F\x9D\x05\x02\x3C\x44\x5F\x89\x3C\xB6\x50\x3C\xB0\x03\xDC\xBC\x74\xE4\x29\x80\xE8\xBC\xE8\x08\x4C\x29\x5C\x45\x0A\x01\xB2\xCA\xD7\x1B\x3B\xE0\x8F\xCE\xB1\x60\x00\xEA\x1F\xDB\x49\xCF\x01\x9F\x77\xEE\x25\x4F\x02\x64\x6F\x27\x1D\x81\x88\xF2\x67\x48\x7F\xA0\x6D\x71\x39\x1D\x81\xF0\xF0\x03\x54\xC0\x90\x03\xE1\xE1\x28\x20\x5C\x00\x2B\x00\x8E\xFC\x34\x58\xFC\x60\x41\x01\x0A\x69\x92\x94\xBB\x0A\x48\x53\x97\xB2\x02\x52\x80\x61\x80\x43\x00\x44\xA5\x92\xCF\x2E\x34\x8C\x18\xE1\x22\x00\x4F\xA7\x8E\x05\x04\x6C\x85\x06\x48\xFD\xE1\xA7\x37\xCF\x9E\x7D\x2D\x67\x16\x8E\xC0\x39\xE8\x80\x2B\x6C\x07\x9C\x43\x01\x3A\x9D\xD7\x43\x14\x2F\x1D\x15\xE0\xED\xED\xFB\x18\xC5\x77\x15\x3D\x07\x04\x05\xE5\xDF\x49\xC9\x0F\xCA\xC0\x0E\xC8\xCA\x72\xEF\x4B\x71\xCF\xA2\x57\x81\x88\x08\xDF\xC5\x14\xDF\x88\x78\x8D\x8D\xD1\xD7\xD4\xF4\x19\x42\xE9\x53\x53\x83\x02\x6A\x04\xF0\x1D\x40\x8F\x3F\xDB\x01\x8C\x42\xA1\x70\xDC\x07\x38\xCE\x01\xC3\x38\x1C\x02\xBA\x64\x97\xE3\x3F\x78\xF0\x60\x6C\xFE\xDE\xBD\xD9\x11\xA0\xBD\x64\xA0\x2D\x45\x05\x30\x5B\x3F\xFB\xA5\x71\xC6\xF8\xE9\x33\xDF\x3B\xB3\x59\x8A\x57\x81\xAA\x17\x9D\x54\xB1\x02\x74\x5E\x14\x9D\x43\x80\xB7\x2F\x05\x56\x50\x40\x46\x46\x46\x3E\x05\x56\x50\x40\x5A\x5A\x9A\x7B\xBE\x3B\x00\x2B\xD8\x01\xF1\xF1\xF1\x4B\x28\xB0\x82\x23\x50\x5D\xED\x10\x50\x5D\x8D\x02\xAA\x05\x50\x01\xFD\x16\x02\xFD\x58\x7E\x66\x6F\x87\x14\x49\xD0\xFB\x49\x5C\xFF\x0B\x6E\x7C\x94\x3D\x65\x01\x1F\x8D\xA7\xA0\x00\x9A\xBB\x0B\xF8\xB3\xB1\x71\x6E\xE2\xF4\x99\xDF\x9F\xC9\x49\x45\x01\x27\x2F\x38\x39\x89\x02\x1E\xE6\xA1\x02\x1E\xE4\xA1\x02\x1E\xE1\xA1\x57\x81\xBE\x3C\xB4\x03\x6E\xE1\xA1\x23\xF0\xBC\x00\x14\xE0\x9A\x45\x8C\xC1\x15\x56\x40\x2A\xD4\xAE\xE0\xFA\xFF\x5F\xD2\xE0\x10\x30\x89\x13\x20\x18\x01\x03\x0A\x60\x1A\x1B\x7F\x9C\x9B\x03\x48\x53\xAF\x79\x2B\xAC\xE3\x09\x4B\x4E\x66\xB4\xDE\x3C\x5A\x8B\x89\x51\x67\xF0\xA8\xAD\x16\x46\x96\xC6\x23\xB3\x99\x18\x4D\x3C\x8F\xC6\x04\x23\xF0\x9C\x00\x14\xE0\x9A\x45\xD7\xAE\x82\xB6\xBE\xE2\xC6\x1E\x05\x1A\x1A\x1A\x76\xED\xDA\xE5\xBC\x11\xEA\x7A\x12\x64\x15\x7C\x2C\xE5\xCA\xBF\xEE\xC3\x10\xD4\xEF\xF2\x30\x64\xB1\xB8\x3E\x0C\x99\x4C\xAE\x0F\x43\x16\xAB\xEB\xC3\x10\x7E\xFE\x0F\x1F\x86\xFE\x43\x24\x0B\xC0\x6C\xB2\x5A\x2D\x16\x8B\xD5\x64\xFB\xCD\x6A\xC3\x6C\x41\x4C\xEC\x1F\xCD\x56\x8B\xCD\x66\xB2\x9A\x4C\xB0\x60\xDE\x6D\x83\xFD\x4D\x26\x0B\x6C\xB3\xFC\xFD\x7F\xFB\x4B\x80\x01\x00\xDB\x46\x77\x95\x47\x42\x82\x7E\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82";

void free_bitmap(IMGHDR *img)
{
  if(img) 
  {
    mfree(img->bitmap);
    mfree(img);
  }
}













 
IMGHDR *LoadImg(char *picname, int max_x, char loadFromElf)
{
  IMGHDR *img;
  
  if(get_file_size(MakeGlobalString(ICON_PATH,'\\', picname)))
  {
    img = CreateIMGHDRFromPngFile(MakeGlobalString(ICON_PATH,'\\',picname ),0);
    if(img && (img->w > ( max_x) || img->h > 16))
    {
      free_bitmap(img);   
      char *path = (loadFromElf==0) ? (STATUS_PNG) : ((loadFromElf==1) ? (IMAGE_PNG) : (XST_PNG));
      img = read_pngimg(path);
    }
  }
 
  
  if(!get_file_size(MakeGlobalString(ICON_PATH,'\\',picname )))
    img = read_pngimg((loadFromElf==0) ? (STATUS_PNG) : ((loadFromElf==1) ? (IMAGE_PNG) : (XST_PNG)));
  
  return img;
}








 
void LoadImages()
{
  IMGHDR *img;
  
  img = LoadImg("status.png", (IS_FFC+1)*16, 0);

  int start_x = 0;

  for(int i = 0 ; i < TOTAL_ICONS ; i++)
  {
      extern const char *icons_names[];
      char fn[128];
      sprintf(fn,"%s\\%s",ICON_PATH, icons_names[i]);
      if(get_file_size(fn))
        AddIcon(CreateIMGHDRFromPngFile(fn,0), 0);
      else
        AddIcon(0,1);
   }
  
  
  char fn[128];
  
  for(int i = 0 ; i<35 ; i++)
   {
     sprintf(fn,"%s\\%d.png",XSTATUSES_PATH, i);
     AddXIcon(CreateIMGHDRFromPngFile(fn,0), 0);
   }
  free_bitmap(img);
}







 
void LoadingSmiles(void)
{
  
  if(get_file_size(MakeGlobalString(ICON_PATH,'\\',"smiles.png")))
  {
    int x=0,y=0;
    IMGHDR *img = CreateIMGHDRFromPngFile(MakeGlobalString(ICON_PATH,'\\',"smiles.png"),0);
    for(int count=0;count<96;count++)
    {
      if((count > 0 && count%12 == 0) || (x > img->w)) { x = 0; y+=28; }
      AddSmile(img,x,y);
      x += 28;
      if(y > img->h) break;
    }
    free_bitmap(img);
  }
  else
  {
    smilestop = 0;
    
    FreeSmiles();
  }
}







 
void LoadingImages()
{
   n_pic = 0xE137;
   CheckSmiles();
   LoadingSmiles(); 
   InitSmiles();
   ProcessNextSmile();
   LoadImages();
}

void FreeAllList()
{
   FreeSmileList();
   FreeSmiles();
   FreeIconList();
   FreeXIconList();
}
