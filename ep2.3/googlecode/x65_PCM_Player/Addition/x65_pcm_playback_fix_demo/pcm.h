typedef struct
    {unsigned short
    Unk_0C,
    Unk_01,
    Unk_03,
    Unk_47,
    MulF1,
    DivF1,
    MulF2,
    DivF2,
    Unk_15,
    I2S_mode,
    Unk_0808;
    } dsp_param;

typedef struct
    {
    char  bytes_bytes[0x20];
    long    l0x2C,l0x4C6A,channels, samplerate, bytes_per_second ;
    short bytes_per_sample, bit_count;
    long l0xFFFF, pcm_data_size, pcm_file_size, item_0x40, item_0x44,
        item_0x48;
    } init_data;

typedef struct
    {
    char item_1, IsBufferFinal, item_3, pcm_fill_frame_mode;
    short pcm_player_cepid;
    short pcm_na_1;
    void* pcm_1st_buf_hdr_ptr;
    void* pcm_2st_buf_hdr_ptr, * pcm_dsp_buf_ptr, * pcm_buff_pos_ptr, * pcm_buff_end_ptr;
    long pcm_samples_count, pcm_data_size, pcm_frame_size, pcm_sample_rate, SampleSize, pcm_buf_rem_and_frame_dif,
        pcm_buf_remainder, pcm_unk_f7f8, pcm_unk_f7fc;
    init_data* pcm_settings_ptr;
    long pcm_unk_F804, pcm_unk_F808, field_4C, mask_50, field_54, IsBufferEnded, field_5C, IsPlaybackActive;
    dsp_param* dsp_stream_settings_ptr;
    long field_68, field_6C;
    } pcm_player;


