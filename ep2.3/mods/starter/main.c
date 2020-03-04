#include "..\inc\swilib.h"
#include "../inc/cfg_items.h"

CSM_DESC icsmd;
int id_ed;

int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;

extern const char dir[];
extern void InitConfig();
int start(char s[128])
{
  WSHDR *ws;
  ws=AllocWS(256);
  str_2ws(ws,s,256);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
  return 0;
}
//-----------------------------------------//

const char per_t[]="%t";
const char per_s[]="%s";
const char empty_str[]="";

//-----------------------------------------//

int Load()
{
   DIR_ENTRY de;
   unsigned int err;
   char *s=malloc(128);
   char *path=malloc(256);
   sprintf(path,per_s,dir);
   char *ptr=path+strlen(path)+1;
   strcat(path,"\\*.*");
   if(FindFirstFile(&de,path,&err))
    {
     do
     {
       strcpy(ptr,de.file_name);  
       sprintf(s,"%s\\%s",dir,de.file_name);
       start(s);
     }
     while(FindNextFile(&de,&err));
    }
   FindClose(&de,&err);
   mfree(path);
   mfree(s);
   return 1;
}

int main(char *exename, char *fname)
{ 
 InitConfig();
 Load();
 return 0;
}
