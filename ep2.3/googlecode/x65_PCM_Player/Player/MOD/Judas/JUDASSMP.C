/*
 * JUDAS V2.0 sample handling
 */




/*
 * This is something interesting...the sample pointer might go this many bytes
 * over the sample end while in the mixing inner loop. Here's the formula:
 *
 *         maximum_playspeed / minimum_mixspeed (round upwards for safety)
 * bytes = ---------------------------------------------------------------
 *                       iterations_in_mixing_inner_loop
 *
 * Maximum playspeed is 535232 (highest speed in linear freq. table), minimum
 * mixspeed is 5000 and there are 16 iterations.
 * And, of course, because there can be 16bit samples the number of bytes must
 * be multiplied with 2.
 */
#define SAFETYAREA 3456

SAMPLE *judas_allocsample(int length);
void judas_freesample(SAMPLE *smp);
void judas_playsample(SAMPLE *smp, unsigned chnum, unsigned frequency, unsigned char volume, unsigned char panning);
void judas_ipcorrect(SAMPLE *smp);
void judas_stopsample(unsigned chnum);
void judas_preventdistortion(unsigned channels);
void judas_setmastervolume(unsigned chnum, unsigned char mastervol);
void judas_setmusicmastervolume(unsigned musicchannels, unsigned char mastervol);
void judas_setsfxmastervolume(unsigned musicchannels, unsigned char mastervol);

SAMPLE *judas_allocsample(int length)
{
        SAMPLE *smp;

        judas_error = JUDAS_OUT_OF_MEMORY;
        length += SAFETYAREA;
        smp = (SAMPLE *)locked_malloc(sizeof (SAMPLE));
        if (!smp) return NULL;
        smp->start = locked_malloc(length);
        if (!smp->start)
        {
                judas_freesample(smp);
                return NULL;
        }
        smp->repeat = NULL;
        smp->end = NULL;
        judas_error = JUDAS_OK;
        return smp;
}

void judas_freesample(SAMPLE *smp)
{
        int c;
        CHANNEL *chptr;

        if (!smp) return;
        if (smp == &fakesample) return;
        /*
         * Go thru all channels; if that sample is playing then stop the
         * channel; we don't want a crash or audible shit!
         */
        chptr = &judas_channel[0];
        for (c = CHANNELS; c > 0; c--)
        {
                if (chptr->smp == smp)
                {
                        chptr->smp = NULL;
                        chptr->voicemode = VM_OFF;
                }
                chptr++;
        }
        /*
         * Free the sample data and then the sample structure itself
         */
        if (smp->start) locked_free(smp->start);
        locked_free(smp);
}

void judas_ipcorrect(SAMPLE *smp)
{
        if (!smp) return;
        if (smp == &fakesample) return;

        if (smp->voicemode & VM_LOOP)
        {
                int count, lcount = smp->end - smp->repeat;
                unsigned char *src = &smp->repeat[0], *dest = &smp->end[0];

                if (!src) return;
                if (!dest) return;

                if (smp->voicemode & VM_16BIT) count = SAFETYAREA;
                else count = SAFETYAREA / 2;
                while (count)
                {
                        if (lcount <= 0)
                        {
                                lcount = smp->end - smp->repeat;
                                src = &smp->repeat[0];
                        }
                        *dest++ = *src++;
                        lcount--;
                        count--;
                }
        }
        else
        {
                if (!smp->end) return;
                if (smp->voicemode & VM_16BIT) memset(&smp->end[0], 0, SAFETYAREA);
                else memset(&smp->end[0], 0, SAFETYAREA / 2);
        }
}

void judas_playsample(SAMPLE *smp, unsigned chnum, unsigned frequency, unsigned char volume, unsigned char panning)
{
        CHANNEL *chptr;

        if (!smp) return;
        if (smp == &fakesample) return;
        if (chnum >= CHANNELS) return;
        chptr = &judas_channel[chnum];
        chptr->voicemode = VM_OFF;
        chptr->pos = smp->start;
        chptr->repeat = smp->repeat;
        chptr->end = smp->end;
        chptr->smp = smp;
        chptr->fractpos = 0;
        chptr->freq = frequency;
        chptr->vol = volume;
        chptr->panning = panning;
        chptr->voicemode = smp->voicemode;
}

void judas_stopsample(unsigned chnum)
{
        CHANNEL *chptr;

        if (chnum >= CHANNELS) return;
        chptr = &judas_channel[chnum];
        chptr->voicemode = VM_OFF;
        chptr->smp = NULL;
}

void judas_preventdistortion(unsigned active_channels)
{
        int count;
        unsigned char mastervol;

        if (active_channels < 2) mastervol = 255;
        else mastervol = 256 / active_channels;
        for (count = 0; count < CHANNELS; count++)
        {
                judas_setmastervolume(count, mastervol);
        }
}

void judas_setmastervolume(unsigned chnum, unsigned char mastervol)
{
        CHANNEL *chptr;

        if (chnum >= CHANNELS) return;
        chptr = &judas_channel[chnum];
        chptr->mastervol = mastervol;
}

void judas_setmusicmastervolume(unsigned musicchannels, unsigned char mastervol)
{
        CHANNEL *chptr = &judas_channel[0];
        int count;

        if (musicchannels > CHANNELS) musicchannels = CHANNELS;
        for (count = 0; count < musicchannels; count++)
        {
                chptr->mastervol = mastervol;
                chptr++;
        }
}

void judas_setsfxmastervolume(unsigned musicchannels, unsigned char mastervol)
{
        CHANNEL *chptr;
        int count;

        if (musicchannels >= CHANNELS) return;
        chptr = &judas_channel[musicchannels];
        for (count = musicchannels; count < CHANNELS; count++)
        {
                chptr->mastervol = mastervol;
                chptr++;
        }
}
