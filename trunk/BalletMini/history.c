#include <swilib.h>
#include <stdlib.h>
#include "history.h"
#include "file_works.h"
#include "string_works.h"
#include "lang.h"

extern const int HISTORY_DEPTH;
const int MAX_FILE_SIZE = 32768;
const char *NEW_LINE = "\r\n";

void CheckHistory(const char *url)
{
  char *default_url;
  int f;
  unsigned ul;
  char * history_file = getSymbolicPath("$urlcache\\history.txt");
  if ((f = _open(history_file, A_ReadOnly + A_Create + A_BIN, P_READ + P_WRITE, &ul)) == -1)
  {
    if ((f = _open(history_file, A_WriteOnly+A_Create+A_BIN,P_READ+P_WRITE,&ul)) == -1)
    {
      LockSched();
      ShowMSG(1,(int)lgpData[LGP_HistoryFileFailed]);
      UnlockSched();
      mfree(history_file);
      return;
    }

    default_url = (char *) malloc(strlen(url) + 3);
    strcpy(default_url, url); strcat(default_url, NEW_LINE);

    _write(f,default_url,strlen(default_url), &ul);
    _close(f, &ul);

    mfree(default_url);
    mfree(history_file);
    return;
  }
  _close(f, &ul);
  mfree(history_file);
}

//------------------------------------------------------------------------------

char **GetHistory(int *cnt)
{
  char *history_buf,*s,*tmp;
  char **history;
  int f,flen,history_depth=0,i;
  unsigned ul;
  char * history_file = getSymbolicPath("$urlcache\\history.txt");
  f=_open(history_file, A_ReadOnly+A_Create+A_BIN,P_READ+P_WRITE,&ul);
  if (f==-1)
  {
    LockSched();
    ShowMSG(1,(int)lgpData[LGP_HistoryFileFailed]);
    UnlockSched();
    *cnt=history_depth;
    mfree(history_file);
    return 0;
  }
  flen=_lseek(f,0,2,&ul,&ul)+1;
  _lseek(f,0,0,&ul,&ul);

  flen=(flen>MAX_FILE_SIZE)?MAX_FILE_SIZE:flen;
  history_buf=(char*)malloc(MAX_FILE_SIZE);
  history_buf[flen-1]=0;
  _read(f,history_buf,flen,&ul);
  _close(f,&ul);

  history=(char**)malloc(sizeof(char *)*HISTORY_DEPTH);
  for(i=0;i<HISTORY_DEPTH;i++)
  history[i]=0;
  s=history_buf;
  tmp=history_buf;
  for(i=0;i<HISTORY_DEPTH&&s&&tmp<history_buf+flen;i++)
  {
    s=strstr(tmp,NEW_LINE);
    if(s)
    {
      history[i]=(char*)malloc(s-tmp+1);
      memcpy(history[i],tmp,s-tmp);
      history[i][s-tmp]=0;
      tmp=s+2;
      history_depth++;
    }
  }
  *cnt=history_depth;
  mfree(history_buf);
  mfree(history_file);
  return history;
}

//------------------------------------------------------------------------------

void AddURLToHistory(const char *url)
{
  char *history_buf, **history, *s, *tmp;
  int f, flen, history_depth = 0, i;
  unsigned ul;
  char * history_file = getSymbolicPath("$urlcache\\history.txt");
  if ((f=_open(history_file, A_ReadWrite+A_Create+A_BIN,P_READ+P_WRITE,&ul))==-1)
  {
    LockSched();
    ShowMSG(1,(int)lgpData[LGP_HistoryFileFailed]);
    UnlockSched();
    mfree(history_file);
    return;
  }

  flen = _lseek(f, 0, 2, &ul, &ul)+1;
  _lseek(f, 0, 0, &ul, &ul);


  flen = (flen>MAX_FILE_SIZE)?MAX_FILE_SIZE:flen;
  history_buf = (char*)malloc(MAX_FILE_SIZE);
  history_buf[flen-1] = 0;
  _read(f,history_buf,flen,&ul);
  _close(f,&ul);

  history = (char**)malloc(sizeof(char *) * HISTORY_DEPTH);
  for(i = 0; i < HISTORY_DEPTH; i++)
  history[i] = 0;

  s = history_buf;
  tmp = history_buf;
  for(i = 0; i < HISTORY_DEPTH && s && tmp < history_buf + flen; i++)
  {
    s = strstr(tmp, NEW_LINE);
    if(s)
    {
      history[i] = (char*)malloc(s - tmp + 1);
      memcpy(history[i], tmp, s-tmp);
      history[i][s-tmp] = 0;
      tmp = s + 2;
      history_depth++;
    }
  }
  for(i = 0; i < history_depth; i++)
  {
    if(!strcmp(history[i], url))
    {
      while(i)
      {
        s = history[i];
        history[i] = history[i-1];
        history[i-1] = s;
        i--;
      }
      break;
    }
  }
  if(HISTORY_DEPTH && strcmp(history[0], url))
  {
    if(history[HISTORY_DEPTH-1])
      mfree(history[HISTORY_DEPTH-1]);
    for(i = HISTORY_DEPTH-1; i>0; i--)
      history[i] = history[i-1];
    history[0] = (char*)malloc(strlen(url)+1);
    strcpy(history[0], url);
    history_depth++;
    history_depth = (history_depth>HISTORY_DEPTH)?HISTORY_DEPTH:history_depth;
  }


  tmp = history_buf;
  tmp[0] = 0;
  for(i = 0; i < history_depth; i++)
  {
    strcat(tmp, history[i]);
    strcat(tmp, NEW_LINE);
    mfree(history[i]);
  }
  mfree(history);

  _unlink(history_file, &ul);

  f=_open(history_file,A_WriteOnly+A_Create+A_BIN,P_READ+P_WRITE,&ul);
  if (f==-1)
  {
    LockSched();
    ShowMSG(1,(int)lgpData[LGP_HistoryFileFailed]);
    UnlockSched();
    mfree(history_buf);
    mfree(history_file);
    return;
  }
  _write(f,history_buf,strlen(history_buf), &ul);
  _close(f, &ul);
  mfree(history_buf);
  mfree(history_file);
}
