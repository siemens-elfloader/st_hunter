#include "Mss3App.h"
#ifdef DEBUG
#include "dbglog.h"
#endif

int main(char *exename, char *fname)
{
#ifdef DEBUG
  InitDBG();
#endif
  return Mss3App::Start(exename, fname);
}


