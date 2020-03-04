#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
int main(char *filename)
{
  extern void InitConfig();
  InitConfig();
  ShowMSG(1,(int)"ok!");  
  return 0;
}
