
#include <swilib.h>
#include "pcmplayer.h"


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

static unsigned char WavHdr[44] =
{
    0x52, 0x49, 0x46, 0x46, 0x6A, 0x88, 0x04, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20,
    0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x40, 0x1F, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00,
    0x02, 0x00, 0x08, 0x00, 0x64, 0x61, 0x74, 0x61, 0x06, 0x88, 0x04, 0x00
};

static char audio_control_fake_audio[8192] = {0};
static PcmPlayer *global_pcm_player = nullptr;
#define PCM_HOOK_FUNC *((unsigned int *)RamAudioHookProc())



void PcmPlayer::frame_request(int unk, uint16_t *frame)
{
    if(!global_pcm_player)
        return;

    global_pcm_player->readBuffer(frame, 640*2);
}




PcmPlayer::PcmPlayer(uint32_t lenght, uint32_t samplerate, uint32_t channels, uint32_t bits_per_sample) :
    obsHandle(0),
    _volume(5)
{
    playerId = -1;
    setAudioParameters(lenght, samplerate, channels, bits_per_sample);
    global_pcm_player = this;
}


PcmPlayer::~PcmPlayer()
{
    stop();
}


void PcmPlayer::setAudioParameters(uint32_t lenght, uint32_t samplerate, uint32_t channels, uint32_t bits_per_sample)
{
    this->lenght = lenght;
    this->samplerate = samplerate;
    this->channels = channels;
    this->bits_per_sample = bits_per_sample;
}


int PcmPlayer::toOBSHandle(int handle)
{
    if( handle < 0 )
        return 0;

    HObj obj;
    obj = (HObj)GetPlayObjById(handle);
    if(!obj)
        return -1;
    return (( int*)obj)[0x3d0/4];
}


int PcmPlayer::play()
{
    PCM_HOOK_FUNC = (unsigned int)(frame_request);

    memcpy((char *)audio_control_fake_audio, ( void * ) ( WavHdr ), 44 );

    wave_header *header = (wave_header*)audio_control_fake_audio;
    header->samplerate = samplerate();
    header->channels = channels();
    header->bits_per_sample = bits_per_sample();
    header->bytes_per_sec = bits_per_sample() * channels() * samplerate() / 8;
    header->data_size = ULONG_MAX;
    header->file_size = header->data_size+44;

    playerId = PlayMelodyInMem( 0x11, audio_control_fake_audio, sizeof(audio_control_fake_audio), MMI_CEPID, 0, 0 );
    obsHandle = toOBSHandle(playerId());

#ifdef NEWSGOLD
    PlayMelody_ChangeVolume(playerId(), 0);

    if(obsHandle) {
        char vol = -1;
        while( vol != 0 ) {
            Obs_Sound_GetVolume(obsHandle, &vol);
            NU_Sleep(10);
        }

        setVolume(_volume);
    }
#else
    PlayMelody_ChangeVolume(playerId(), _volume);
#endif

    return playerId();
}


int PcmPlayer::stop()
{
    if(playerId() < 0)
        return -1;

    PlayMelody_StopPlayback(playerId());
    PCM_HOOK_FUNC = 0;
    playerId = -1;
    return 0;
}


int PcmPlayer::setVolume(int vol)
{
    _volume = vol;
#ifdef NEWSGOLD
    if(_volume < 0)
        _volume = 0;
    if(_volume > 15)
        _volume = 15;

    if(obsHandle)
        return Obs_Sound_SetVolumeEx(obsHandle, _volume, 0);
    return -1;
#else
    if(_volume < 0)
        _volume = 0;
    if(_volume > 5)
        _volume = 5;

    return PlayMelody_ChangeVolume(playerId(), _volume);
#endif
}


int PcmPlayer::addVolume(int delta)
{
    return setVolume(_volume + delta);
}


int PcmPlayer::subVolume(int delta)
{
    return setVolume(_volume - delta);
}




