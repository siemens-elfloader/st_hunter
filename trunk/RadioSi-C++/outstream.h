#ifndef AUDIO_H
#define AUDIO_H

#include "Thread.h"
#include "WaitCondition.h"
#include "Mutex.h"
#include "aaccontext.h"
#include "multivariatebuffer.h"


typedef struct
{
    char riff[4];
    unsigned int file_size;
    char wave[4];
    char fmt[4];
    unsigned int wave_section_size;
    unsigned short format;
    unsigned short channels;
    unsigned int samplerate;
    unsigned int bytes_per_sec;
    unsigned short align;
    unsigned short bits_per_sample;
    char data[4];
    unsigned int data_size;
}wave_header;


class OutStream : public Thread
{

public:
    OutStream(AACContext *context, MultivariateBuffer *mbuffer);
    virtual ~OutStream();

    int flushBuffer();
    ssize_t read(void *buf, size_t size);
    void waitForFlushed();

protected:
    void run() override;

private:
    int first_decode, readed_size, switch_status, can_switch;
    bool flushing;
    WaitCondition wait, wait_for_flushed;
    AACContext *aac_context;
    MegaBuffer temp;
    MultivariateBuffer *mbuffer;

};

#endif // AUDIO_H
