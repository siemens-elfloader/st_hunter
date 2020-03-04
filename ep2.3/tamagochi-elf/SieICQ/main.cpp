#include "include.h"
#include "sieicqmain.h"
#include "conf_loader.h"

extern const char * successed_config_filename;

int main(char * exename, char * filename)
{
  InitConfig();
  SieICQMain * Main = new SieICQMain();
  Main->Create();
  return 0;
}
