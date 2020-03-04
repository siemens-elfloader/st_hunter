
#include "Siemens_mp3.c"
extern char Paused;

void MP3Play(){

  if(Playing==0){
AudioStart();
}else Paused=0; 
}

void MP3Pause(){
Paused=!Paused;  
}
