//DisplayShooter v2.0
//© DJ 2007

#include "uni.h"
#include "conf_loader.h"
#include "jpeglib.h"

int fds;

typedef struct{
  int (*onMessage)(CSM_RAM*,UNI_GBS_MSG*);
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
int cs=1; //canshot по клавишам
int cs2=1; //canshot по сохранению

int (*oldIcsmOnMessage)(CSM_RAM*,UNI_GBS_MSG*);
void (*oldIcsmOnClose)(CSM_RAM*);

extern const char SAVE_PATH[];
extern const int FORMAT;
extern const int JPG_QUAL;
extern const int JPG_PR;
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

int scrW, scrH; //Размер экрана
unsigned int len; //scrW*scrH*2

char *scrbuf; //Скринбуфер
unsigned char *mybuf; //Буфер программы

int mode;
//0 - норма
//1 - пора нажимать коротко
//2 - следующий 0

void getname(char *dest, const char *ext)
{
  TTime t;
  TDate d;
  GetDateTime(&d,&t);

  char curname[20];
  sprintf(curname, "%02d%02d%02d_%02d%02d%02d.",
          d.year, d.month, d.day, t.hour, t.min, t.sec);

  strcpy(dest, SAVE_PATH);
  strcat(dest, strcat(curname, ext)+2);
}

void save_bmp()
{
  int f;
  unsigned int ul;

  char fname[145];
  getname(fname, "bmp");

  if ((f=fopen(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
  {
    if(fwrite(f,bmphdr,66,&ul)!=66)goto del;
    if(fwrite(f,mybuf,len,&ul)!=len)goto del;
    fclose(f,&ul);
    goto end;
  }

del:
  ShowMSG(1, (int)"Error!");
  fclose(f,&ul);
  unlink(fname,&ul);

end:
  mfree(mybuf);

  cs2=1;
  if(ENA_MSG)ShowMSG(1, (int)SHOT_MSG);
}

void save_jpg()
{
  int f;
  unsigned int ul;

  char fname[145];
  getname(fname, "jpg");

  if ((f=fopen(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
  {
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, f);
    cinfo.image_width = scrW;
    cinfo.image_height = scrH;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, JPG_QUAL, TRUE);
//    cinfo.optimize_coding=TRUE;
//    if(JPG_PR)
      jpeg_simple_progression(&cinfo); //ЧЕ-ТО Я НАКУРАЛЕСИЛ
    jpeg_start_compress(&cinfo, TRUE);
    int row_stride = cinfo.image_width * 2; // JSAMPLEs per row in image_buffer
    JSAMPROW row_pointer[1];
    while (cinfo.next_scanline < cinfo.image_height)
    {
//      if(cinfo.global_state==0)
      row_pointer[0] = & mybuf[cinfo.next_scanline * row_stride];
      jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    fclose(f, &ul);
    jpeg_destroy((j_common_ptr)&cinfo);
  }

  mfree(mybuf);
  cs2=1;
  if(ENA_MSG)ShowMSG(1, (int)SHOT_MSG);
}

int hook(int submsg, int msg)
{
  if((IsUnlocked()|ENA_LOCK) && submsg==SHOT_BTN)
  {
    if(mode==1) //Первый if, притворяется, что нажимают клавишу
    {
      switch(msg)
      {
      case KEY_UP:
        GBS_SendMessage(MMI_CEPID,KEY_DOWN,SHOT_BTN);
        return(0);
      case KEY_DOWN:
        mode=0;
        GBS_SendMessage(MMI_CEPID,KEY_UP,SHOT_BTN);
        return(0);
      }
    }

    if(msg==KEY_UP)
    {
      cs=1;
      if(mode==2) //Если отпускаем клавишу после шота долгим, говорим телу, что клавиша не нажималась
      {
        mode=0;
        return(2);
      }
    }
    else if(msg==(LONG_P?LONG_PRESS:KEY_DOWN) && cs && cs2) //Шот
    {
      cs2=0;
      mybuf=malloc(len);
      if(FORMAT)
      {
        memcpy(mybuf, scrbuf, len);
        SUBPROC((void*)save_jpg);
      }
      else
      {
        for(int i=0; i<scrH; i++)
          memcpy(mybuf+i*scrW*2, scrbuf+len-(i+1)*scrW*2, scrW*2);
        SUBPROC((void*)save_bmp);
      }
      if((msg==LONG_PRESS) && LONG_P)
        cs=0;
    }

    if(!IsUnlocked()) //При залоченной клаве шот делаю, но телу ничего не говорю
      return(2);

    if(LONG_P)
    {
      switch(msg)
      {
      case KEY_DOWN: //Жду, будет ли долгое нажатие
        mode=1; //Если не будет, попаду на первый if
        return(2);
      case LONG_PRESS: //Дождались
        mode=2;
        return(2);
      }        
    }
    else
      return(2);
  }
  return(0);
}

int newIcsmOnMessage(CSM_RAM* data,UNI_GBS_MSG* msg)
{
  if((isnewSGold()?msg->newsgold.msg:msg->sgold.msg) == UNI_MSG_RECONFIGURE_REQ) 
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

void ElfKiller()
{
  extern void kill_data(void *p, void (*func_p)(void *));
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

int main(void)
{
  //Получаю размеры экрана
  scrW=ScreenW();
  scrH=ScreenH();
  
  len=scrW*scrH*2;
  scrbuf=RamScreenBuffer();

  //Пишу размеры экрана в bmp-хедер
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
