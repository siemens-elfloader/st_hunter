
#include "pcm.h"
#include "config\config.h"

pcm_player pcm; 

void pcm_copy_frame_h( unsigned long* out_pcm_data );
void pcm_set_hook( long* data );



/* Replace for function 08 40 2D E9  18 00 90 E5 ? ? ? E5 00 00 50 E3 ... */
__arm void pcm_set_hook( long* data )
    {
    
    if (data[6])
        pcm.dsp_stream_settings_ptr = (void *)data[6];

    /*If channels = 2 switch stereo mode */
    if (pcm.pcm_settings_ptr->channels == 2)
        pcm.dsp_stream_settings_ptr->I2S_mode = 0x294; //Stereo


    /* Value from NSG (E71) */
    //pcm.dsp_stream_settings_ptr->Unk_03=0x07;

    /* Configure PCM source frame size for source data*/
    pcm.pcm_frame_size = 160 * pcm.pcm_settings_ptr->bit_count / 8;

    switch( pcm.pcm_sample_rate )
        {
        case 8000:
            /* Configure I2S speed */
            pcm.dsp_stream_settings_ptr->MulF2 = 0x1020;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x659;
            /* For DAC sample rate hook */
            pcm.item_1 = 0;
            break;

        case 11025:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x10DC;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x1FBD;
            pcm.item_1 = 1;
            break;

        case 12000:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x1030;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x659;
            pcm.item_1 = 2;
            break;

        case 16000:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x1040;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x659;
            pcm.item_1 = 3;
            break;

        case 22050:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x11B9;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x1FBD;
            pcm.item_1 = 4;
            break;

        case 24000:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x1060;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x659;
            pcm.item_1 = 5;
            break;

        case 32000:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x1080;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x659;
            pcm.item_1 = 6;
            break;

        case 44100:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x1372;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x1FBD;
            pcm.item_1 = 7;
            break;

        case 48000:
            pcm.dsp_stream_settings_ptr->MulF2 = 0x10C0;
            pcm.dsp_stream_settings_ptr->DivF2 = 0x659;
            pcm.item_1 = 8;
            break;

        default:
          /* Wrong sample rate*/
            asm ("SWI 3");
        }
    }


/* Replace for function 10 40 2D E9 ? ? ? E5 03 10 D4 E5 01 00 51 E3 ... */
__arm void pcm_copy_frame_h( unsigned long* out_pcm_data )
    {
      
    int count = 0;

    switch( pcm.pcm_settings_ptr->bit_count ) /* 8 or 16 bit */
        {
        case 8:                               /* 8 bit */
            while (count < 160)               /* 160 samples */
                {
                /* 8bit -> 32 bit */
                out_pcm_data[count] = ((((char*)(pcm.pcm_buff_pos_ptr))[count]) ^ 128) * 256;
                count++;
                }
            /* Set new buffer position */
            pcm.pcm_buff_pos_ptr = (void*)((long)pcm.pcm_buff_pos_ptr + 160);
            break;

        case 16: /* 16 bit */
            while (count < 160)
                {
                /* 16bit -> 32 bit */
                out_pcm_data[count] = ((short*)(pcm.pcm_buff_pos_ptr))[count];
                count++;
                }
            /* Set new buffer position */
            pcm.pcm_buff_pos_ptr = (void*)((long)pcm.pcm_buff_pos_ptr + 320);
            break;
        }

    /* Set samples counter value */
    pcm.pcm_samples_count = pcm.pcm_samples_count + 160;
    }



#pragma diag_suppress=Pe177
__root static const int PATCH_CHANGE_PCM_HOOK_POINTER @ "PATCH_CHANGE_PCM_HOOK_POINTER" = (int)pcm_set_hook;
#pragma diag_default=Pe177
