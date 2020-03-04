typedef struct
{
  char Marker1[4];
  long BytesFollowing;
  char Marker2[4];
  char Marker3[4];
  long Fixed1;
  unsigned short FormatTag, Channels;
  unsigned long SampleRate, BytesPerSecond;
  short BlockSize, BitsPerSample;
  unsigned long DataBytes;
  char Marker4[4];
  unsigned long unk7, NumOfSamples;
  char Marker5[4];
  unsigned long DataSize;
}
IMA_ADPCM_WAV_HDR;

static int ima_index_adjust[16] =
{
  -1, -1, -1, -1, /* +0 - +3, decrease the step size */
  2, 4, 6, 8, /* +4 - +7, increase the step size */
  - 1, -1, -1, -1, /* -0 - -3, decrease the step size */
  2, 4, 6, 8, /* -4 - -7, increase the step size */
};

static int ima_step_size[89] =
{
  7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130,
       143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166,
       1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845,
       8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};
short ima_step;

void ImaCodeBlock( signed short * block, unsigned char * ima )
{
  signed int step,diff, previous, k, spos;
  unsigned char bytecode,mask;
  signed int vpdiff;
  
  *(signed short*)ima = (block[0]+block[1])/2;
  
  ima[2] = ima_step;
  ima[3] = 0;
  spos = 1;
  k = 4;
  previous = (block[0]+block[1])/2;
  while ( k < 256 )
  { //Code from cdex_src_140b3
    
    diff = (block[spos]+block[spos+1])/2 - previous;
    bytecode = 0;
    step = ima_step_size[ima_step];
    vpdiff = step >> 3;
    if ( diff < 0 )
    {
      bytecode = 8;
      diff = -diff;
    };
    mask = 4;
    while ( mask )
    {
      if ( diff >= step )
      {
        bytecode |= mask;
        diff -= step;
        vpdiff += step;
      };
      step >>= 1;
      mask >>= 1;
    };

    if ( bytecode & 8 )
      previous -= vpdiff;
    else
      previous += vpdiff;

    if ( previous > 32767 )
      previous = 32767;
    else if ( previous < -32767 )
      previous = -32767;

    ima_step += ima_index_adjust[bytecode];
    if ( ima_step < 0 )
      ima_step = 0;
    else if ( ima_step > 88 )
      ima_step = 88;

    ima[k] = bytecode;
    spos+=2;
    //
    diff = (block[spos]+block[spos+1])/2 - previous;

    bytecode = 0;
    step = ima_step_size[ima_step];
    vpdiff = step >> 3;
    if ( diff < 0 )
    {
      bytecode = 8;
      diff = -diff;
    };
    mask = 4;
    while ( mask )
    {
      if ( diff >= step )
      {
        bytecode |= mask;
        diff -= step;
        vpdiff += step;
      };
      step >>= 1;
      mask >>= 1;
    };

    if ( bytecode & 8 )
      previous -= vpdiff;
    else
      previous += vpdiff;

    if ( previous > 32766 )
      previous = 32766;
    else if ( previous < -32767 )
      previous = -32767;

    ima_step += ima_index_adjust[bytecode];
    if ( ima_step < 0 )
      ima_step = 0;
    else if ( ima_step > 88 )
      ima_step = 88;

    ima[k] = ima[k] + ( bytecode << 4 );
    spos+=2;
    k++;

  };

  k++;
}

