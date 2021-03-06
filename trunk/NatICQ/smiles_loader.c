#include <swilib.h>
#include <pnglist.h>
#include "naticq_ipc.h"
#include "smiles.h"
#include "naticq.h"

S_SMILES *s_top=0;

DYNPNGICONLIST *SmilesImgList;
DYNPNGICONLIST *XStatusesImgList;

int *XStatusesIconArray;

volatile int total_smiles;
volatile int total_xstatuses;
volatile int total_xstatuses2;
int total_clientid;
volatile int xstatuses_load;
volatile int pictures_max=0;
volatile int pictures_loaded=0;

extern const char SMILE_FILE[];
extern const char SMILE_PATH[];
extern const char XSTATUSES_PATH[];
extern const char ipc_my_name[32];
extern  int S_ICONS[];

static IPC_REQ gipc1;
static char *p_buf;
static char *s_buf;
static S_SMILES *s_bot;
static int n_pic1;

static int n_pic2;
static IPC_REQ gipc2;


S_SMILES *FindSmileById(int n)
{
  int i=0;
  S_SMILES *sl=(S_SMILES *)s_top;
  while(sl && i!=n)
  {
    sl=sl->next;
    i++;
  }
  return sl;
}

S_SMILES *FindSmileByUni(int wchar)
{
  S_SMILES *sl=(S_SMILES *)s_top;
  while(sl)
  {
    if (sl->uni_smile == (unsigned int)wchar) return (sl);
    sl=sl->next;
  }
  return (0);
}

void FreeSmiles(void)
{
  S_SMILES *s_smile;
  STXT_SMILES *n;
  STXT_SMILES *st;
  DYNPNGICONLIST *d;
  DYNPNGICONLIST *nd;
  LockSched();
  total_smiles=0;
  s_smile=(S_SMILES *)s_top;
  s_top=0;
  s_bot=0;
  UnlockSched();
  while(s_smile)
  {
    S_SMILES *s;
    s=s_smile;
    st=s->lines;
    while(st)
    {
      n=st->next;
      mfree(st);
      st=n;
    }
    s_smile=(S_SMILES *)(s_smile->next);
    mfree(s);
  }
  LockSched();
  d=SmilesImgList;
  SmilesImgList=0;
  UnlockSched();
  while(d)
  {
    if (d->img)
    {
      mfree(d->img->bitmap);
      mfree(d->img);
    }
    nd=d->next;
    mfree(d);
    d=nd;
  }
  mfree(s_buf);
}

void InitSmiles(void)
{
  int f;
  unsigned int err;
  int fsize;
  char *buf;
  FSTATS stat;

  FreeSmiles();

  n_pic1=FIRST_UCS2_BITMAP;
  if (GetFileStats(SMILE_FILE,&stat,&err)==-1)
    return;

  if ((fsize=stat.size)<=0)
    return;

  if ((f=_open(SMILE_FILE,A_ReadOnly+A_BIN,P_READ,&err))==-1)
    return;

  buf=s_buf=p_buf=malloc(fsize+1);
  buf[_read(f,buf,fsize,&err)]=0;
  _close(f,&err);
  //f=_open("4:\\smiles.cfg",A_ReadWrite+A_BIN+A_Create+A_Append,P_READ+P_WRITE,&err);
  gipc1.name_to=ipc_my_name;
  gipc1.name_from=ipc_my_name;
  gipc1.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SMILE_PROCESSED,&gipc1);
  pictures_max+=60;
}

void CheckSmiles(void)
{
  int f;
  unsigned int err;
  int fsize;
  char *buf, *p_buf;
  FSTATS stat;

  if (GetFileStats(SMILE_FILE,&stat,&err)==-1)
    return;

  if ((fsize=stat.size)<=0)
    return;

  if ((f=_open(SMILE_FILE,A_ReadOnly+A_BIN,P_READ,&err))==-1)
    return;

  buf=p_buf=malloc(fsize+1);
  buf[_read(f,buf,fsize,&err)]=0;
  _close(f,&err);

//  f = pictures_max;
  for(buf=p_buf;*buf; buf++)
    if(*buf == ':')
    {
      buf++;
      while(*buf && *buf != 0x0D) buf++;
      pictures_max++;
    }
/*  if(f == pictures_max)                 //� ����� � ��� ������ �������� �����?
  {
    for(buf=p_buf;*buf; buf++)
    if(*buf == 0x0A)
    {
      buf++;
      while(*buf && *buf != 0x0A && *buf!=':') buf++;
      if(*buf!=':') pictures_max++;
    }
  }*/
  mfree(p_buf);
}

void ProcessNextSmile(void)
{
  int c;
  char fn[128];
  const char _slash[]="\\";
  DYNPNGICONLIST *dp;
  S_SMILES *si;
  STXT_SMILES *st;
  char *buf=p_buf;
  if (!buf) return;
  while ((c=*buf))
  {
    char *p;
    if ((c==10)||(c==13))
    {
      buf++;
      gipc1.name_to=ipc_my_name;
      gipc1.name_from=ipc_my_name;
      gipc1.data=0;
      GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SMILE_PROCESSED,&gipc1);
      p_buf=buf;
      return;
    }
    p=strchr(buf,':');
    if (!p) break;
    zeromem(fn,128);
    strcpy(fn,SMILE_PATH);
    if (fn[strlen(fn)-1]!='\\') strcat(fn,_slash);
    c=p-buf;
    if ( (unsigned int)c >(127-strlen(fn)) ) break;
    strncpy(fn+strlen(fn),buf,c);
//    snprintf(logmsg,255,"Process file %s...",fn);
//    REDRAW();
    buf=p;
    dp=malloc(sizeof(DYNPNGICONLIST));
    zeromem(dp,sizeof(DYNPNGICONLIST));
    dp->icon=GetPicNByUnicodeSymbol(n_pic1);
    dp->img=CreateIMGHDRFromPngFile(fn,0);
    LockSched();
    if (SmilesImgList)
    {
      dp->next=SmilesImgList;
    }
    SmilesImgList=dp;
    UnlockSched();
    si=malloc(sizeof(S_SMILES));
    si->next=NULL;
    si->lines=NULL;
    si->botlines=NULL;
    si->uni_smile=n_pic1;
    if (s_bot)
    {
      //�� ������
      s_bot->next=si;
      s_bot=si;
    }
    else
    {
      //������
      s_top=si;
      s_bot=si;
    }
    n_pic1++;
    while (*buf!=10 && *buf!=13 && *buf!=0)
    {
      buf++;
      int i=0;
      while (buf[i]!=0&&buf [i]!=','&&buf [i]!=10&&buf[i]!=13)  i++;
      st=malloc(sizeof(STXT_SMILES)+i);
      strncpy(st->text,buf,i);
      st->text[i]=0;

      st->next=NULL;
      st->key=*((unsigned long *)st->text);
      st->mask=~(0xFFFFFFFFUL<<(8*i));
      st->key&=st->mask;
      if (si->botlines)
      {
	si->botlines->next=st;
	si->botlines=st;
      }
      else
      {
	si->lines=st;
	si->botlines=st;
      }
      buf+=i;
    }
    pictures_loaded++;
    total_smiles++;
  }
  //_close(f,&err);
  total_smiles=0;
  p_buf=NULL;
  mfree(s_buf);
  s_buf=NULL;
  REDRAW();
}

void FreeXStatusesImg(void)
{
  DYNPNGICONLIST *d;
  DYNPNGICONLIST *nd;
  LockSched();
  total_xstatuses=0;
  d=XStatusesImgList;
  XStatusesImgList=0;
  UnlockSched();
  mfree(XStatusesIconArray);
  XStatusesIconArray=NULL;
  while(d)
  {
    if (d->img)
    {
      mfree(d->img->bitmap);
      mfree(d->img);
    }
    nd=d->next;
    mfree(d);
    d=nd;
  }
}

void InitXStatusesImg(void)
{
  FreeXStatusesImg();
  total_xstatuses=0;
  total_xstatuses2=0;
  *(XStatusesIconArray=malloc(sizeof(int)))=S_ICONS[IS_NULLICON];
  xstatuses_load=1;
  n_pic2=FIRST_UCS2_BITMAP;
  gipc2.name_to=ipc_my_name;
  gipc2.name_from=ipc_my_name;
  gipc2.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XSTATUSIMG_PROCESSED,&gipc2);
  pictures_max+=50;

/*  char fn[128];
  const char _slash[]="\\";
  FSTATS stat;
  unsigned err;

  pictures_max = 0;
  do
  {
    strcpy(fn,XSTATUSES_PATH);
    if (fn[strlen(fn)-1]!='\\') strcat(fn,_slash);
    sprintf(fn+strlen(fn),"%d.png",pictures_max++);
    if (GetFileStats(fn,&stat,&err)==-1) break;
  }
  while (stat.size>0);
  pictures_max--;
  CheckSmiles();
*/
//  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XSTATUSIMG_PROCESSED,&gipc2);
}

void ProcessNextXStatImg(void)
{
  char fn[128];
  const char _slash[]="\\";
  DYNPNGICONLIST *dp;
  unsigned int err;
  FSTATS stat;
  int i;

  strcpy(fn,XSTATUSES_PATH);
  if (fn[strlen(fn)-1]!='\\') strcat(fn,_slash);
  sprintf(fn+strlen(fn),"%d.png",total_xstatuses2);
  if (GetFileStats(fn,&stat,&err)!=-1)
  {
    if (stat.size>0)
    {
      dp=malloc(sizeof(DYNPNGICONLIST));
      zeromem(dp,sizeof(DYNPNGICONLIST));
      dp->icon=i=GetPicNByUnicodeSymbol(n_pic2);
      dp->img=CreateIMGHDRFromPngFile(fn,0);
      LockSched();
      if (XStatusesImgList)
      {
	dp->next=XStatusesImgList;
      }
      XStatusesImgList=dp;
      UnlockSched();
      total_xstatuses2++;
      pictures_loaded++;
      XStatusesIconArray=realloc(XStatusesIconArray,(total_xstatuses2*sizeof(int)));
      *(XStatusesIconArray+(total_xstatuses2-1))=i;
      n_pic2++;
    l1:
      gipc2.name_to=ipc_my_name;
      gipc2.name_from=ipc_my_name;
      gipc2.data=0;
      GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XSTATUSIMG_PROCESSED,&gipc2);
      return;
    }
  }else{
    if (total_xstatuses2<100){
      total_xstatuses=total_xstatuses2;
      total_xstatuses2=100;
      goto l1;
    }
  }
  total_clientid=total_xstatuses2-101;
  xstatuses_load=0;
}
