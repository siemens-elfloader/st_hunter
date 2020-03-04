#include "..\inc\swilib.h"
#include "main.h"
extern char *file;
extern char *file_name;
extern int curpos;
extern int atoi(char *attr);
extern const char per_s[];
extern const char HIST_PATH[128];
//extern int file_size;
//-----------------LOAD INI-------------------//
/*
format
path /?codepage?/ position
*/
/**********************
*.hst ,потом может сделаю задание как параметра если понадобится...
**********************/
void LoadINI()
{
  unsigned int err; 
  int plhandle; 
  char *mem;
  char *pos;
  char path[256];//="0:\\zbin\\TextReader\\history\\";
  sprintf(path,per_s,HIST_PATH);
  strcat(path,file_name);
  strcat(path,".hst");//history
  
  if(isdir(HIST_PATH,&err))
     plhandle = fopen(/* "0:\\TR_history2.txt"*/path, A_ReadOnly + A_BIN, P_READ, & err ); 
  else //ShowMSG(1,(int)"Directory not found!");
    mkdir(HIST_PATH,&err);
  if(plhandle!=-1 ) 
  {
    mem=malloc(100); 
    fread(plhandle,mem,99,&err); 
    pos=strstr(mem,file)+strlen(file)+1;
    curpos=atoi(pos);  
  }
  else curpos=1;
fclose(plhandle,&err); 
mfree(mem); 
}


void SaveINI()//save history
{
 unsigned int err; 
 int f; 
 char *ss=malloc(300);
 char path[256];
 sprintf(path,per_s,HIST_PATH);
 strcat(path,file_name);
 strcat(path,".hst");//history
// ShowMSG(2,(int)path);
 f = fopen( path, A_ReadWrite + A_BIN+A_Create /*+ A_Append*/, P_READ + P_WRITE, & err );
 if ( f ==-1 ) 
  {
    fclose( f, & err ); 
    return; 
  }
 sprintf(ss,"%s %i\n",file,curpos+1);
 fwrite(f,ss,strlen(ss),&err);
 mfree(ss);
 fclose(f,&err);
}

