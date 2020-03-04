
#include "math.h"


#include "pcm.c"

double SinePos;
int SmpPos=0;

void GetSound( signed short * ptr, int nsamples )
{ int offset=0;
  unsigned int tmp;
while(nsamples)
  {
    tmp=(Example[SmpPos])*128;
    *(ptr+offset)=(cos(SinePos)*tmp);   //Left channel
    *(ptr+offset+1)=(sin(SinePos)*tmp); //Right channel
    SmpPos++;
    if(SmpPos>=sizeof(Example)) SmpPos=0;
    offset+=2;
    nsamples-=2;
    SinePos+=3.14/16000/4;
}
  
}

