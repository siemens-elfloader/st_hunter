#include <spl/async_print.h>
#include "aaccontext.h"


#define retcode(c) this->last_err = c; return

/* MicroSoft channel definitions */
#define SPEAKER_FRONT_LEFT             0x1
#define SPEAKER_FRONT_RIGHT            0x2
#define SPEAKER_FRONT_CENTER           0x4
#define SPEAKER_LOW_FREQUENCY          0x8
#define SPEAKER_BACK_LEFT              0x10
#define SPEAKER_BACK_RIGHT             0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER   0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER  0x80
#define SPEAKER_BACK_CENTER            0x100
#define SPEAKER_SIDE_LEFT              0x200
#define SPEAKER_SIDE_RIGHT             0x400
#define SPEAKER_TOP_CENTER             0x800
#define SPEAKER_TOP_FRONT_LEFT         0x1000
#define SPEAKER_TOP_FRONT_CENTER       0x2000
#define SPEAKER_TOP_FRONT_RIGHT        0x4000
#define SPEAKER_TOP_BACK_LEFT          0x8000
#define SPEAKER_TOP_BACK_CENTER        0x10000
#define SPEAKER_TOP_BACK_RIGHT         0x20000
#define SPEAKER_RESERVED               0x80000000

static long aacChannelConfig2wavexChannelMask(NeAACDecFrameInfo *hInfo)
{
    if (hInfo->channels == 6 && hInfo->num_lfe_channels)
    {
        return SPEAKER_FRONT_LEFT + SPEAKER_FRONT_RIGHT +
            SPEAKER_FRONT_CENTER + SPEAKER_LOW_FREQUENCY +
            SPEAKER_BACK_LEFT + SPEAKER_BACK_RIGHT;
    } else {
        return 0;
    }
}




static int fill_buffer(aac_buffer *b)
{
    int bread;

    if (b->bytes_consumed > 0)
    {
        if (b->bytes_into_buffer)
        {
            memmove((void*)b->buffer, (void*)(b->buffer + b->bytes_consumed),
                b->bytes_into_buffer*sizeof(unsigned char));
        }

        if (!b->at_eof)
        {
            bread = b->read(b->handler, (void*)(b->buffer + b->bytes_into_buffer),
                b->bytes_consumed);

            if (bread != b->bytes_consumed)
                b->at_eof = 1;

            b->bytes_into_buffer += bread;
        }

        b->bytes_consumed = 0;

        if (b->bytes_into_buffer > 3)
        {
            if (memcmp(b->buffer, "TAG", 3) == 0)
                b->bytes_into_buffer = 0;
        }
        if (b->bytes_into_buffer > 11)
        {
            if (memcmp(b->buffer, "LYRICSBEGIN", 11) == 0)
                b->bytes_into_buffer = 0;
        }
        if (b->bytes_into_buffer > 8)
        {
            if (memcmp(b->buffer, "APETAGEX", 8) == 0)
                b->bytes_into_buffer = 0;
        }
    }

    return 1;
}


static ssize_t my_read(void *h, void *data, size_t size)
{
    ibstream *is = (ibstream *)h;
    int64_t total = 0;

    while(total < (int64_t)size) {
        int64_t r = is->read(((char*)data)+total, size-total);
        if(r < 0) {
            if(total)
                break;
            else
                return -1;
        }

        total += r;
    }

    return total;
}


static void advance_buffer(aac_buffer *b, int bytes)
{
    b->file_offset += bytes;
    b->bytes_consumed = bytes;
    b->bytes_into_buffer -= bytes;
    if (b->bytes_into_buffer < 0)
        b->bytes_into_buffer = 0;
}


AACContext::AACContext(ibstream *_stream) :
    last_err(0),
    stream(_stream),
    c_sample_buffer(0),
    first_decode(1)
{

}


AACContext::~AACContext()
{
    if(hDecoder)
        NeAACDecClose(hDecoder);

    c_sample_buffer = 0;
    hDecoder = 0;
}


int AACContext::initialize()
{
    int tagsize = 0, bread = 0;

    memset(&buffer, 0, sizeof(buffer));
    buffer.handler = (void*)stream;
    buffer.read = my_read;

    if (!(buffer.buffer = (unsigned char*)malloc(FAAD_MIN_STREAMSIZE*MAX_CHANNELS+2))) {
        printf("Memory allocation error\n");
        return 0;
    }

    bread = buffer.read(buffer.handler, buffer.buffer, FAAD_MIN_STREAMSIZE*MAX_CHANNELS);
    buffer.bytes_into_buffer = bread;
    buffer.bytes_consumed = 0;
    buffer.file_offset = 0;

    if (bread != FAAD_MIN_STREAMSIZE*MAX_CHANNELS)
            buffer.at_eof = 1;

    tagsize = 0;
    if (!memcmp(buffer.buffer, "ID3", 3))
    {
        /* high bit is not used */
        tagsize = (buffer.buffer[6] << 21) | (buffer.buffer[7] << 14) |
                  (buffer.buffer[8] <<  7) | (buffer.buffer[9] <<  0);

        tagsize += 10;
        advance_buffer(&buffer, tagsize);
        fill_buffer(&buffer);
    }

    hDecoder = NeAACDecOpen();
    config = NeAACDecGetCurrentConfiguration(hDecoder);
    config->defObjectType = LC;
    config->outputFormat = FAAD_FMT_16BIT;
    config->downMatrix = 0;
    config->useOldADTSFormat = 0;

    NeAACDecSetConfiguration(hDecoder, config);

    fill_buffer(&buffer);
    if ((bread = NeAACDecInit(hDecoder, buffer.buffer,
            buffer.bytes_into_buffer, &_samplerate, &_channels)) < 0)
    {
        NeAACDecClose(hDecoder);
        retcode(-1) -1;
    }

    advance_buffer(&buffer, bread);
    fill_buffer(&buffer);

    return 0;
}



uint32_t AACContext::decode()
{
    int tryis = 0;
    last_err = 0;
    /*if(buffer.at_eof) {
        c_sample_buffer = 0;
        c_samples = 0;
        return 0;
    }*/

    while(tryis < 1) {
        c_sample_buffer = NeAACDecDecode(hDecoder, &frameInfo,
                                         buffer.buffer, buffer.bytes_into_buffer);



        if (frameInfo.error > 0)
        {
            SetVibration(30);
            NU_Sleep(20);
            SetVibration(0);

            advance_buffer(&buffer, 128); // how?
            fill_buffer(&buffer);

            last_err = frameInfo.error;
        } else {
            //ShowMSG(1, (int)"Decoding fail");
            last_err = 0;
            break;
        }

        tryis++;
    }

    if ( frameInfo.samples > 0 )
    {
        if(first_decode) {
            _channel_mask = aacChannelConfig2wavexChannelMask(&frameInfo);
            first_decode = 0;
        }

        //write_audio_file(aufile, sample_buffer, frameInfo.samples, 0);
        c_samples = frameInfo.samples;
        c_channels = _channels;
        c_chanels_mask = _channel_mask;
    }else {
        c_samples = 0;
        c_channels = 0;
        c_chanels_mask = 0;
    }

    advance_buffer(&buffer, frameInfo.bytesconsumed);
    fill_buffer(&buffer);

    //if(buffer.bytes_into_buffer < 1) {
    //    buffer.at_eof = 1;
    //}

    return c_samples * 2;
}
