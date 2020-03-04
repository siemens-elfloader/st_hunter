#include "..\\inc\\swilib.h"
#include "..\inc\cfg_items.h"
#include "main.h"
#define CLIST_Y1 (YDISP+2)



int totalitems;
int curline;
int drawFont;
  
void AddToLog(char *text)
{
  LogLine* tmp = malloc(sizeof(LogLine));
  strncpy(tmp->text,text,128);  
  tmp->next=0;
  
  if(!toplog)
  {
    toplog = tmp;
  }
  else
  {
    LogLine* tlog = toplog;
    while(tlog->next) tlog = tlog->next;
    tlog->next = tmp;  
  }
}

void FreeLog()
{
  LogLine* tmp = toplog;
  toplog = 0;
  while(tmp)
  {
    LogLine* tline = tmp;
    tmp = tmp->next;
    mfree(tline);
  }
}

int CountItem()
{
  LogLine* tmp = toplog;
  int i=0;
  while(tmp=tmp->next) i++;
  return i;
}

