
#include "Siemens_aac.c"
extern char Paused;

void AACPlay(){

  if(Playing==0){
AudioStart();
}else Paused=0; 
}

void AACPause(){
Paused=!Paused;  
}
