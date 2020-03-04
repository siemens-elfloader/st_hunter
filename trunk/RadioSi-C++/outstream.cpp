
#include <stdio.h>
#include <spl/async_print.h>
#include "outstream.h"

unsigned char WavHdr[44] =
{
    0x52, 0x49, 0x46, 0x46, 0x6A, 0x88, 0x04, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
    0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x40, 0x1F, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00,
    0x02, 0x00, 0x08, 0x00, 0x64, 0x61, 0x74, 0x61, 0x06, 0x88, 0x04, 0x00
};


void to16BitAudio(void *sample_buffer, char *data, unsigned int samples, int channels, int channelMask)
{
    unsigned int i;
    short *sample_buffer16 = (short*)sample_buffer;

    if (channels == 6 && channelMask)
    {
        for (i = 0; i < samples; i += channels)
        {
            short r1, r2, r3, r4, r5, r6;
            r1 = sample_buffer16[i];
            r2 = sample_buffer16[i+1];
            r3 = sample_buffer16[i+2];
            r4 = sample_buffer16[i+3];
            r5 = sample_buffer16[i+4];
            r6 = sample_buffer16[i+5];
            sample_buffer16[i] = r2;
            sample_buffer16[i+1] = r3;
            sample_buffer16[i+2] = r1;
            sample_buffer16[i+3] = r6;
            sample_buffer16[i+4] = r4;
            sample_buffer16[i+5] = r5;
        }
    }

    for (i = 0; i < samples; i++)
    {
        data[i*2] = (char)(sample_buffer16[i] & 0xFF);
        data[i*2+1] = (char)((sample_buffer16[i] >> 8) & 0xFF);
    }
}



OutStream::OutStream(AACContext *context, MultivariateBuffer *_mbuffer) :
    Thread(350*1024, 100),
    readed_size(0),
    switch_status(0),
    can_switch(1),
    aac_context(context),
    temp(FAAD_MIN_STREAMSIZE*MAX_CHANNELS * 4),
    mbuffer(_mbuffer)

{
    int e;
    char s[128];
    if( (e = start()) < 0 ) {
        sprintf(s, "Failed to start %d", e);
        ShowMSG(1, (int)s);
    }
}


OutStream::~OutStream()
{
    terminate();
}




extern char info[128];
void OutStream::run()
{
    const int max_samples = FAAD_MIN_STREAMSIZE*MAX_CHANNELS;
    char *sampleBuf = new char[FAAD_MIN_STREAMSIZE*MAX_CHANNELS *2];
    //int32_t can_write = 0;
    first_decode = 1;

ololo:


    if(first_decode) {
        first_decode--;
        //if(first_decode < 1)
            //mbuffer->nextFlushBuffer();
    }
    else {
        flushing = false;
        wait.wait();
        flushing = true;
    }

    mbuffer->resetFlushBuffer();
    //can_write = mbuffer->size();
    printf("Decoding...\n");
    sprintf(info, "Decoding...");
    REDRAW();
    int err = 0;

    while(mbuffer->availableFlushSpace() > 0)
    {
        int dec_size = aac_context->decode();

        if(!dec_size && aac_context->lastError()) {
            //qWarning() << "Decoding error " << NeAACDecGetErrorMessage(aac_context->lastError());
            err = aac_context->lastError();
            break;
        }


        /* write available data */
        if(temp.availableData() > 0) {
            int can_read = std::min(temp.availableData(),  mbuffer->availableFlushSpace());
            int r = temp.read(sampleBuf, can_read);
            mbuffer->write(sampleBuf, r);

            if(temp.isEmpty())
                temp.reset();
        }

        /* write new data */
        int samples = aac_context->samples();
        char *sample_data = (char*)aac_context->sampleBuffer();
        while(samples > 0) {

            int s = std::min(samples, max_samples);
            int32_t data_size = s*2;
            int writed = 0;

            if(samples > max_samples) {
                //qWarning() << "O_O";
            }

            to16BitAudio(sample_data, sampleBuf, s,
                         aac_context->channels(), aac_context->channelMask());

            /* can write into buffer */
            if( mbuffer->availableFlushSpace() > 0 ) {
                int r = 0;
                int c_w = std::min((int)data_size, (int)mbuffer->availableFlushSpace());

                if(c_w > 0)
                    r = mbuffer->write(sampleBuf, c_w);

                if(r > 0)
                    writed += r;
            }

            if(writed < data_size) {
                int r = temp.write(sampleBuf + writed, data_size-writed);
                if(r < 1) {
                    ShowMSG(1, (int)"Fatal error");
                } else {
                    writed += r;
                }
            }


            sample_data += data_size;
            samples -= s;
        }

        //qWarning() << "total_size " << mbuffer->availableFlushData();
    }

    if(!first_decode)
        wait_for_flushed.wakeAll();



    sprintf(info, "Done %d %d %d %s", err, mbuffer->availableFlushData(), mbuffer->size(), err? NeAACDecGetErrorMessage(err) : "" );
    REDRAW();

    mbuffer->nextFlushBuffer();

    goto ololo;
}


ssize_t OutStream::read(void *data, size_t size)
{
    int r = mbuffer->read(data, size);

    if(r > 0) {
        if( readed_size == 0 ) {
            flushBuffer();
        }

        readed_size += r;
    }


    if( readed_size >= mbuffer->size() ) {
        readed_size = 0;
        mbuffer->nextReadBuffer();
    }

    return r;
}


int OutStream::flushBuffer()
{
    wait.wakeOne();
    return 0;
}


void OutStream::waitForFlushed()
{
    wait_for_flushed.wait();
}


