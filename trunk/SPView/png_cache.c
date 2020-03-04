#include <swilib.h>
#include <pnglist.h>

void clear_cache()
{
  PNGTOP_DESC *ptop=PNG_TOP();
  PNGLIST *pl;
  LockSched();
  pl=ptop->pltop;
  ptop->pltop=0;
  UnlockSched();
  while(pl)
  {
    PNGLIST *pl_prev;
    pl_prev=pl;
    pl=pl->next;
    mfree(pl_prev->pngname);
    if(pl_prev->img)
    {
      mfree(pl_prev->img->bitmap);
      mfree(pl_prev->img);
    }
    mfree(pl_prev);
  }
}

void clear_bitmap()
{
  PNGTOP_DESC *ptop=PNG_TOP();
  char *bitmap;
  if ((bitmap=ptop->bitmap))
  {
    LockSched();
    zeromem(bitmap,20000/8*2);
    UnlockSched();
  }
}
