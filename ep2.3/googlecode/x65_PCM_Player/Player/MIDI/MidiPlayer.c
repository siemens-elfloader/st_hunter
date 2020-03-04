//#include "MIDIFILE.c"

long MidiSmpCnt;

#include "midplay.c"
#include "mixer.c"
void MidiClose();
char filename[256];
 

char MidiOpened=0;
unsigned char *MidiFile;


  

void loadprogram( unsigned char Number  )
{
  char *zippos,*progpos;
 midi_prog[Number]=malloc(sizeof(MIDI_PROGRAM));
 MIDI_PROGRAM * prg=midi_prog[Number];
 sprintf(filename,"%d.mi",Number);
 //UnZipByFileName("0:\\GMBANK\\GM.zip",filename,&zipbuff,1024*512);
 zippos=(void*)*(long*)(0xA0060000+Number*4);;  
  memcpy((char*)prg,zippos,325 + 96 + 4 +4+ 1);
  zippos+=325 + 96 + 4 +4+ 1;
  int i = 0;
  while ( i < prg->smpcnt )
  {
    prg->samples[i] = malloc( sizeof( MIDI_SAMPLE ) );
    memcpy((char*)prg->samples[i],zippos,15);
    zippos+=15;
    //prg->samples[i]->data = malloc( prg->samples[i]->length );
    //memcpy(prg->samples[i]->data,zippos,prg->samples[i]->length);
    prg->samples[i]->data = zippos;
    zippos+=prg->samples[i]->length;
    i++;
  }

}

void loadprogram_zip( unsigned char Number  )
{
  char *zipbuff=0;
  char *zippos,*progpos;
 midi_prog[Number]=malloc(sizeof(MIDI_PROGRAM));
 MIDI_PROGRAM * prg=midi_prog[Number];
 sprintf(filename,"%d.mi",Number);
 UnZipByFileName("0:\\GMBANK\\GM.zip",filename,&zipbuff,1024*512);
 zippos=zipbuff;  
  memcpy((char*)prg,zippos,325 + 96 + 4 +4+ 1);
  zippos+=325 + 96 + 4 +4+ 1;
  int i = 0;
  while ( i < prg->smpcnt )
  {
    prg->samples[i] = malloc( sizeof( MIDI_SAMPLE ) );
    memcpy((char*)prg->samples[i],zippos,15);
    zippos+=15;
    prg->samples[i]->data = malloc( prg->samples[i]->length );
    memcpy(prg->samples[i]->data,zippos,prg->samples[i]->length);
    zippos+=prg->samples[i]->length;
    i++;
  }
  mfree(zipbuff);
}

void loadprogram_ffs( unsigned char Number  )
{
  midi_prog[Number]=malloc(sizeof(MIDI_PROGRAM));
  MIDI_PROGRAM * prg=midi_prog[Number];
  int i;
  int handle;
  sprintf(filename,"0:\\GMBANK\\%d.mi",Number);
  handle = fopen( filename,A_BIN,P_READ, & err );
  fread32( handle,(char*)prg, 325 + 96 + 4 +4+ 1, &err );
  i = 0;
  while ( i < prg->smpcnt )
  {
    prg->samples[i] = malloc( sizeof( MIDI_SAMPLE ) );
    fread32( handle,(char*)prg->samples[i], 15,  &err );
    prg->samples[i]->data = malloc( prg->samples[i]->length );
    fread32( handle,prg->samples[i]->data, prg->samples[i]->length, &err );
    i++;
  }
  if(Number==128)bitrate=prg->smpcnt*1000 ;
  fclose( handle,&err );
}

void freeprogram(unsigned char Number){
  int i=0;

  if(midi_prog[Number]){
    
 while ( i < midi_prog[Number]->smpcnt )
  {
    mfree(midi_prog[Number]->samples[i]);
    //mfree(midi_prog[Number]->samples[i]->data);
    i++;
  }
 mfree(midi_prog[Number]);
  }
 midi_prog[Number]=0;
 
}


int GetMidiPlayingTime(){
return(MidiSmpCnt/samplerate);
}

int MidiOpen(char *filename1,char *filename2){
int handle;
  char *zipbuff;//=malloc(1024*512);
  int unzipsize=0;
  
  SetVibration(0);
  
if(MidiOpened)MidiClose();

if(GetFileType(filename1)==FT_ZIP)
{
  FromFile=0;
  unzipsize=UnZipByFileName(filename1,filename2,&zipbuff,1024*512);
  if(!unzipsize)  return(1);
  MidiFile=zipbuff;
  
}
else
{
handle=fopen(filename1,A_BIN,P_READ, & err );  
MidiFile=malloc(lseek(handle,0,S_END,&err,&err));  
fclose(handle,&err);
handle=fopen(filename1,A_BIN,P_READ, & err );
fread32(handle ,MidiFile, 512000, &err );
fclose(handle,&err);
}


samplerate=32000;
channels=2;
MidiSmpCnt=0;
MidiInit(MidiFile);
GetSound=GetMidiSound;//GetMidiSound32;
GetPlayingTime=GetMidiPlayingTime;
MidiOpened=1;
return(0);
}



void MidiPlay(){
//bitrate=0;
if(Playing==0)AudioStart();else Paused=0; 
}

void MidiClose(){  
  
  SetVibration(0);
  
  if(MidiOpened){
  mfree(MidiFile);
  int i=0;
  while(i<129){   
    freeprogram(i);
  i++;  
  }
  
  MidiOpened=0;
  }
}