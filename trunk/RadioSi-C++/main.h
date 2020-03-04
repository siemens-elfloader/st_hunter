
#ifndef __MAIN_H__
#define __MAIN_H__

#include "sourcestream.h"
#include "pcmplayer.h"
#include "aaccontext.h"
#include "multivariatebuffer.h"
#include "outstream.h"
#include "widechar.h"


class MainPlayer : public PcmPlayer, SourceStream
{
public:

    MainPlayer();
    virtual ~MainPlayer();


    void connectStatus(int code) override;
    void headerFetched() override;
    void icyMetaUpdated() override;

protected:
    int readBuffer(void *data, uint32_t size) override;

public:
    AACContext *aac_context;
    MultivariateBuffer *mbuffer;
    OutStream *pcm_stream;

    struct {
        widechar station,
                 description,
                 title;

        std::string channels,
                    bitrate,
                    samplerate,
                    status;
    } info;


    WSHDR *screen_ws;
};




#endif
