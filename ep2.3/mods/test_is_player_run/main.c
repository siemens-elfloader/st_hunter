#include "../inc/swilib.h"

#ifdef ELKA
#define HOBJ_LOC 0x54
#else
#define HOBJ_LOC 0x50
#endif

unsigned int playercsmid=0;
void *playercsmadr=NULL;


//el=A087B454 e=A087109C
char CSMADR[9];

int strcmp_nocase(const char *s, const char *d)
{
  int cs;
  int ds;
  do
  {
    cs=*s++;
    if (cs&0x40) cs&=0xDF;
    ds=*d++;
    if (ds&0x40) ds&=0xDF;
    cs-=ds;
    if (cs) break;
  }
  while(ds);
  return(cs);
}

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

int main(char *exename, char *fname)
{ 
  int hobj=0;
  char *phone = Get_Phone_Info(PI_MODEL);
  CSM_RAM *csmp;
  zeromem(CSMADR,9);
 // if(!strcmp_nocase(phone,"el71")) strcpy(CSMADR,"A087B454");
  //if(!strcmp_nocase(phone,"e71")) 
    strcpy(CSMADR,"A087109C");
  ShowMSG(1,(int)phone);  
  
  playercsmadr=(void *)strtoul(CSMADR,NULL,16);
  
  CSM_RAM *p=CSM_root()->csm_q->csm.first;
  while(p)
  {
    if (p->constr==playercsmadr) break;
    p=p->next;
  }
  if (p)
  {
    playercsmid=p->id;
    csmp=FindCSMbyID(playercsmid);
  }
  
  if(playercsmid)
  {
    hobj = ((int*)csmp)[HOBJ_LOC/4];    
    if (hobj)
      ShowMSG(1,(int)"Player run"); 
     else
      ShowMSG(1,(int)"hobj not found");  
  }
  else
      ShowMSG(1,(int)"Not run");  
  
  return 0;
}


int Is_Pleer_Run()
{
  #ifdef ELKA
  #define HOBJ_LOC 0x54
  #else
  #define HOBJ_LOC 0x50
  #endif

  unsigned int playercsmid=0;
  void *playercsmadr=NULL;
  //el=A087B454 e=A087109C
  char CSMADR[9];
  int hobj=0;
  CSM_RAM *csmp;
  char *phone = Get_Phone_Info(PI_MODEL);
  
  zeromem(CSMADR,9);
  
  if(!strcmp_nocase(phone,"E71"))  strcpy(CSMADR,"A087109C");
  if(!strcmp_nocase(phone,"EL71")) strcpy(CSMADR,"A087B454");

 // ShowMSG(1,(int)phone);  
  
  playercsmadr=(void *)strtoul(CSMADR,NULL,16);
  
  CSM_RAM *p=CSM_root()->csm_q->csm.first;
  while(p)
  {
    if (p->constr==playercsmadr) break;
    p=p->next;
  }
  if (p)
  {
    playercsmid=p->id;
    csmp=FindCSMbyID(playercsmid);
  }
  
  if(playercsmid)
  {
    hobj = ((int*)csmp)[HOBJ_LOC/4];    
    if (hobj) return 1;
    /*
      ShowMSG(1,(int)"Player run"); 
     else
      ShowMSG(1,(int)"hobj not found");  */
  }
 // else
   //   ShowMSG(1,(int)"Not run");  
  
  return 0;
}
