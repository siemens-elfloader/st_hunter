#include "../inc/swilib.h"
#include "string.h"

int main(char *filename)
{
  char *str="jfghjfgj_fkfkf,ssss;";
  char *p = strtok(str,"_,;");
  if(p)
   ShowMSG(1,(int)(p-(strlen(str)-strlen(p))));  
  p = strtok(NULL,"_,;");
  if(p)
   ShowMSG(1,(int)p);  
  return 0;
}
