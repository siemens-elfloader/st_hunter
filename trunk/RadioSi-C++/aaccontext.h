#ifndef AACCONTEXT_H
#define AACCONTEXT_H

#include <stdio.h>
#include <unistd.h>
#include <neaacdec.h>
#include "ibstream.h"



/* FAAD file buffering routines */
typedef struct {
    long bytes_into_buffer;
    long bytes_consumed;
    long file_offset;
    unsigned char *buffer;
    int at_eof;
    int infile;
    void *handler;
    ssize_t (*read)(void *h, void *data, size_t size);
} aac_buffer;


#define MAX_CHANNELS 6




class AACContext
{
public:
    AACContext(ibstream *stream);
    ~AACContext();

    int initialize();
    uint32_t decode();

    inline void *sampleBuffer() {
        return c_sample_buffer;
    }

    inline int samples() {
        return c_samples;
    }

    inline int channels() {
        return _channels;
    }

    inline int samplerate() {
        return _samplerate;
    }

    inline int lastError() {
        return last_err;
    }

    inline int channelMask() {
        return _channel_mask;
    }

private:

    int last_err;
    aac_buffer buffer;
    ibstream *stream;


    NeAACDecHandle hDecoder;
    NeAACDecFrameInfo frameInfo;
    NeAACDecConfigurationPtr config;

    unsigned long _samplerate;
    uint8_t _channels;
    int _channel_mask;

    int c_samples;
    int c_channels;
    int c_chanels_mask;
    void *c_sample_buffer;

    int first_decode;
};

#endif // AACCONTEXT_H
