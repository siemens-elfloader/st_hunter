void StandartControlProc();
extern int samplerate;

extern char GetFileType( char * filename );
char *FilePointer;
FSTATS FS ;
WSHDR *SFile=0,*SFolder=0;
GBSTMR standarttimer;


inline char IsPlayerOn(){
//return(*((char*)0xF6000025)&0x0F);
  return *(char*)RamMediaIsPlaying();
}

int StandartOpen( char * filename1, char * filename2)
{  
int handle;
GetFileStats(filename1,&FS,&err);
if(!FS.size)return(1);
//FilePointer=malloc(FS.size);
//handle = fopen( filename1, A_ReadOnly + A_BIN, P_READ, & err );
//read32( handle, FilePointer, FS.size, & err );
//fclose(handle,&err);
 char *temp=malloc(256);
SFile=AllocWS(256);
SFolder=AllocWS(256);
ExtractFileName(filename1,temp);
wsprintf(SFile,temp);
ExtractFileDir(filename1,temp);
wsprintf(SFolder,temp);
return(0);
}

void StandartPlay()
{   
  PLAYFILE_OPT opt;  
 
  zeromem(&opt,sizeof(opt));
  
opt.repeat_num=1;
opt.unk=0xA002;
opt.time_between_play=0x01F4;
opt.volume=Volume;
opt.unk5=1;

  
  
Playing=1;  
PlayFile(0xC,SFolder,SFile,GBS_GetCurCepid(), 0x167,&opt);
  
//PlayMelodyInMem( 0x11, FilePointer, FS.size, 0xFFFF, 0, 0 );
GBS_StartTimerProc( & standarttimer, 128, StandartControlProc );

}

void StandartClose()
{
 // if(FilePointer){
 // mfree(FilePointer);
 // FilePointer=0;    
 // }
if(!SFile)return;
FreeWS(SFile);
FreeWS(SFolder);
SFile=0;
SFolder=0;
Playing=0;
}

void StandartControlProc(){
  if(!Playing){    
    AudioActive=0;
   return; 
  }
  if(!IsPlayerOn()){    
    Playing=0;
    AudioActive=0;
    PlayerCmd=CMD_OPEN | CMD_NEXT | CMD_STOP | CMD_PLAY;
  return;   
  }
GBS_StartTimerProc( & standarttimer, 128, StandartControlProc );
}
