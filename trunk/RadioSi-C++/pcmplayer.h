
#ifndef __PCM_PLAYER_H__
#define __PCM_PLAYER_H__

#include <stdlib.h>
#include <limits.h>
#include "GSProperty"
#include "thread.h"
#include "queue.h"



class AudioBuffer
{
public:
    AudioBuffer();
    ~AudioBuffer();


};


class PcmPlayer : public Thread
{
public:

    PcmPlayer(uint32_t lenght = UINT_MAX,
              uint32_t samplerate = 44100,
              uint32_t channels = 2,
              uint32_t bits_per_sample = 16);

    ~PcmPlayer();

    void setAudioParameters(uint32_t lenght = UINT_MAX,
                           uint32_t samplerate = 44100,
                           uint32_t channels = 2,
                           uint32_t bits_per_sample = 16);

    int play();
    int stop();
    int setVolume(int vol);
    int addVolume(int vol);
    int subVolume(int vol);

    int volume() const {
        return _volume;
    }

protected:
    virtual int readBuffer(void *data, uint32_t size) { return  0; }
    HObj toOBSHandle(int handle);

private:
    HObj obsHandle;
    int _volume;
    GSProperty<int> playerId;
    GSProperty<uint32_t> lenght;
    GSProperty<uint32_t> samplerate;
    GSProperty<uint32_t> channels;
    GSProperty<uint32_t> bits_per_sample;


private:

    struct Message {
        int type;
    } __attribute__((aligned(4)));


    static void frame_request(int unk, uint16_t *frame);
};







#endif

