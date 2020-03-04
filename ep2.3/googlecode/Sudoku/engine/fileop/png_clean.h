#include "..\inc\swilib.h"

//Начало
typedef struct { void *next;
                 char *pngname;
                 IMGHDR * img;} PNGLIST;

typedef struct { PNGLIST *top;
                 char *bitmap; } A_pltop;

int get_number_of_png()
 {
   A_pltop *pltop=PNG_TOP();
   int i=0;
   if (pltop)
       {
        PNGLIST *pl=pltop->top;
         while (pl)
          {
           pl=pl->next;
           i++;
          }
        }
  return (i);
 }

PNGLIST* find_pic_by_n(int n)
 {
  A_pltop *pltop=PNG_TOP();
  if (pltop)
      {
       int i=0;
       PNGLIST *pl=pltop->top;
       while(i!=n && pl)
         {
          pl=pl->next;
          i++;
         }
       return (pl);
      }
    else return (0);
 }

void clear_cache() // Юзаем для очистки памяти от png, например при закрытии или сворачивании.
{
 A_pltop *pltop=PNG_TOP();
 LockSched();
 PNGLIST *pl=pltop->top;
 pltop->top=0;
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

//Конец
