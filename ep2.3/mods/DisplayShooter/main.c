#include "\ARM\inc\swilib.h"
#include "conf_loader.h"

typedef struct{
  int (*onMessage)(CSM_RAM*,GBS_MSG*);
  void (*onCreate)(void*);
  void (*onClose)(CSM_RAM*);//int zero1;
  int zero2;
  int zero3;
  int zero4;
  void (*onCloseN)(CSM_RAM*);
  int datasize;
  int statesize;
  const int *states;
} MY_CSM_DESC;

MY_CSM_DESC icsmd;
int cs=1; //canshot

int (*oldIcsmOnMessage)(CSM_RAM*,GBS_MSG*);
void (*oldIcsmOnClose)(CSM_RAM*);

extern const char SAVE_PATH[];
extern const int SHOT_BTN;
extern const int LONG_P;
extern const int ENA_LOCK;
extern const int ENA_MSG;
extern const char SHOT_MSG[];

char bmphdr[]= {0x42,0x4D,0x42,0x58,0x02,0x00,0x00,0x00,
                0x00,0x00,0x42,0x00,0x00,0x00,0x28,0x00,
                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                0x00,0x00,0x01,0x00,0x10,0x00,0x03,0x00,
                0x00,0x00,0x00,0x58,0x02,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,
                0x00,0x00,0xE0,0x07,0x00,0x00,0x1F,0x00,
                0x00,0x00};

int mode;
//0 - норма
//1 - пора нажимать коротко
//2 - следующий 0

int hook(int submsg, int msg)
{
  int scrH=ScreenH();
  int scrW=ScreenW();

  if((IsUnlocked()|ENA_LOCK) && submsg==SHOT_BTN)
  {
    if(mode==1)
    {
      if(msg==KEY_UP)
      {
        GBS_SendMessage(MMI_CEPID,KEY_DOWN,SHOT_BTN);
        return(0);
      }
      if(msg==KEY_DOWN)
      {
        mode=0;
        GBS_SendMessage(MMI_CEPID,KEY_UP,SHOT_BTN);
        return(0);
      }
    }

    if(msg==KEY_UP)
    {
      cs=1;
      if(mode==2)
      {
        mode=0;
        return(2);
      }
    }
    else if(msg==(LONG_P?LONG_PRESS:KEY_DOWN) && cs)
    {
      int f;
      unsigned int ul;
      unsigned int len=scrW*scrH*2;
      char *scrbuf=RamScreenBuffer();
      char *buf=malloc(len);
      for(int i=0; i<scrH; i++)
        memcpy(buf+i*scrW*2, scrbuf+len-(i+1)*scrW*2, scrW*2);

      TTime t;
      TDate d;
      GetDateTime(&d,&t);
      char curname[20];
      sprintf(curname, "%02d%02d%02d_%02d%02d%02d.bmp",
              d.year, d.month, d.day, t.hour, t.min, t.sec/*t.param*/);
  
      char fname[145];
      strcpy(fname, SAVE_PATH);
      strcat(fname, curname+2);
  
      if ((f=fopen(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
      {
        if(fwrite(f,bmphdr,66,&ul)!=66)goto del;
        if(fwrite(f,buf,len,&ul)!=len)goto del;
        fclose(f,&ul);
        if(ENA_MSG)
          ShowMSG(1, (int)SHOT_MSG);
        goto end;
      }

    del:
      ShowMSG(1, (int)"Error!");
      fclose(f,&ul);
      unlink(fname,&ul);

    end:
      mfree(buf);

      if((msg==LONG_PRESS) && LONG_P)
        cs=0;
    }

    if(!IsUnlocked())
      return(2);

    if(LONG_P)
    {
      if(msg==LONG_PRESS)
      {
        mode=2;
        return(2);
      }
      if(msg==KEY_DOWN)
      {
        mode=1;
        return(2);
      }
    }
    else
      return(2);
  }
  return(0);
}

int newIcsmOnMessage(CSM_RAM* data,GBS_MSG* msg)
{
  int gbsMsgMsg;
  if(isnewSGold())
    gbsMsgMsg=((int*)msg)[1];
  else
    gbsMsgMsg=((short*)msg)[1]; 
  if(gbsMsgMsg == (isnewSGold()?0xDEAF:0x3F1)) 
    InitConfig();

  return oldIcsmOnMessage(data,msg);
}

void newIcsmOnClose(CSM_RAM *data)
{
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
  RemoveKeybMsgHook((void *)hook);
  seqkill(data,oldIcsmOnClose,&ELF_BEGIN,SEQKILLER_ADR());
}

int main(void)
{
  int scrW=ScreenW();
  int scrH=ScreenH();
  bmphdr[18]=(scrW>> 0)&0xFF;
  bmphdr[19]=(scrW>> 8)&0xFF;
  bmphdr[20]=(scrW>>16)&0xFF;
  bmphdr[21]=(scrW>>24)&0xFF;
  bmphdr[22]=(scrH>> 0)&0xFF;
  bmphdr[23]=(scrH>> 8)&0xFF;
  bmphdr[24]=(scrH>>16)&0xFF;
  bmphdr[25]=(scrH>>24)&0xFF;

  InitConfig();

  LockSched();
  CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
  memcpy(&icsmd,icsm->constr,isnewSGold()?40:24);
  oldIcsmOnMessage=icsmd.onMessage;
  if(isnewSGold())
  {
    oldIcsmOnClose=icsmd.onCloseN;
    icsmd.onCloseN=newIcsmOnClose;
  }
  else
  {
    oldIcsmOnClose=icsmd.onClose;
    icsmd.onClose=newIcsmOnClose;
  }
  icsmd.onMessage=newIcsmOnMessage;
  icsm->constr=&icsmd;
  AddKeybMsgHook((void *)hook);
  UnlockSched();

  return 0;
}
