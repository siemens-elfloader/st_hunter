#include "swilib.h"
#include "cfg_items.h"
#include "conf_loader.h"

typedef struct
{
  char w;
  char h;
  unsigned short bpnum;
  char *bitmap;
}IMGHDR_;

const int minus11=-11;
unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
char colors[4][4]={{0xFF,0,0,0x64},{0,0xFF,0,0x64},{0,0,0xFF,0x64},{0xC6,0xAA,0xAF,0x32}};
const char Pointer[5]={0x27,0x27,0xFF,0x27,0x27};
const IMGHDR_ imgPointer = {5,5,0x1,(char *)Pointer};
GBSTMR mytmr;

WSHDR *ws;

typedef struct{
  int gsize;
  int count;
} CHKDS_PARAM;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

typedef struct
{
  char signature[16];
  unsigned short picnum;
  unsigned short unk1;
  char w; 
  char h; 
  char Compr_Bits;  
}PICHDR;

//CONSTANTS
const char *ELFName = "iPhoneSaver";

const char* ClockImgs[] = {"0:\\Zbin\\img\\iPhoneSaver\\0.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\1.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\2.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\3.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\4.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\5.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\6.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\7.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\8.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\9.png",
                           "0:\\Zbin\\img\\iPhoneSaver\\d.png"};

extern const unsigned int cfgRefresh;
extern const char cfgBookBorderCol[4];
extern const char cfgBookBGCol[4];

//VARIBLES
unsigned int err;
TDate Date;
TTime Time;
GBSTMR timer;

/*char *AkkuX = (char*)(0xA1715ABCL);
char *AkkuY = (char*)(0xA1715ABAL);
char *NetX = (char*)(0xA17156B8L);
char *NetY = (char*)(0xA17156B6L);
RAMNET *RN;*/

//PROTOTYPES
//char GetAkkuCapacity()
void DrawScreen();
int rand(void);

////////////////////////////////////////////////////////////////////////////////
unsigned long next=1;

int rand(void) {
 next = next * 1103515245 + 12345;
 return((unsigned int)(next/65536)%32768);
}

////////////////////////////////////////////////////////////////////////////////
int fexists(char* fname)
{
  FSTATS fs;
  return (GetFileStats(fname,&fs,&err)!=-1);
}
////////////////////////////////////////////////////////////////////////////////

void DrwStr(WSHDR *ws, int x, int y,int w,int h, int font,int flag,char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,w,h);
  SetPropTo_Obj1(&drwobj,&rc,0,ws,font,flag);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}

////////////////////////////////////////////////////////////////////////////////

DrwImg(IMGHDR *img, int x, int y)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(0));
  DrawObject(&drwobj);
}
///////////////////////////////////////////////////////////////////////////////

void DrawGPF(char *fname, int x, int y)
{
  volatile int hFile;
  PICHDR Pic_Header;
  unsigned int io_error = 0;
  hFile = fopen(fname, A_ReadOnly + A_BIN, P_READ, &io_error);
  if(!io_error)
  {
    fread(hFile, &Pic_Header, sizeof(Pic_Header), &io_error);
    unsigned int buf_size = lseek(hFile, 0, S_END, &io_error, &io_error) - sizeof(PICHDR);
    lseek(hFile, sizeof(PICHDR), S_SET, &io_error, &io_error); 
    char *pic_buffer = malloc((buf_size+3)&(~3));
    fread(hFile, pic_buffer, buf_size, &io_error);    
    fclose(hFile, &io_error);
    IMGHDR img;
    img.w = Pic_Header.w;
    img.h = Pic_Header.h;
    img.bpnum = Pic_Header.Compr_Bits;
    //img.zero = 0;
    img.bitmap = pic_buffer;
    DrwImg(&img, x, y);
    mfree(pic_buffer);
  }
}
////////////////////////////////////////////////////////////////////////////////

void DrawScreen()
{
  GBS_StartTimerProc(&timer, cfgRefresh  * 216, DrawScreen);
  unsigned int ImgWidth = GetImgWidth((int)ClockImgs[0]);
  unsigned int ImgHeight = GetImgHeight((int)ClockImgs[0]);
  unsigned int Separator = GetImgWidth((int)ClockImgs[10]);
  unsigned int TimeLen = ImgWidth * 4 + Separator;
  int offsetX = rand()%ScreenW();
  offsetX = (offsetX + TimeLen) > ScreenW()?(offsetX << 1 - TimeLen):offsetX;
  int offsetY = rand()%(ScreenH());
  
  int MinY = 0;// = (*AkkuY > *NetY)?*AkkuY + 20:*NetX + 20;
  
  offsetY = (offsetY < MinY)? MinY :offsetY;
  offsetY = (offsetY > ScreenH() - ImgHeight)?ScreenH() - ImgHeight: offsetY;

  
  WSHDR *datestr = AllocWS(20);
  GetDateTime(&Date, &Time);
  //DrawGPF(szBackground, 0, 0);
  void *canvasdata=BuildCanvas();
  DrawCanvas(canvasdata, 0, 0, ScreenW() - 1, ScreenH() - 1, 1);
  DrawRoundedFrame(0, offsetY - 2, ScreenW() -1, offsetY + ImgHeight + 2, 0, 0, 0, cfgBookBorderCol, cfgBookBGCol);
  DrawImg(offsetX, offsetY, (int)ClockImgs[Time.hour / 10]);
  DrawImg(offsetX + ImgWidth, offsetY, (int)ClockImgs[Time.hour % 10]);
  DrawImg(offsetX + ImgWidth * 2, offsetY, (int)ClockImgs[10]);
  DrawImg(offsetX + ImgWidth * 2 + Separator, offsetY, (int)ClockImgs[Time.min / 10]);
  DrawImg(offsetX + ImgWidth * 3 + Separator, offsetY, (int)ClockImgs[Time.min % 10]);        
  wsprintf(datestr, "%02d.%02d.%04d", Date.day, Date.month, Date.year);
  /*DrwStr(datestr, 0, ScreenH() - 32 - GetFontYSIZE(1) , ScreenW(), ScreenH() - GetFontYSIZE(1), 1, 
            0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
  
  RN = RamNet();
  wsprintf(datestr, "net is %d", RN->power);
  DrwStr(datestr, 0, 0, ScreenW(), GetFontYSIZE(1), 1, 0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
  wsprintf(datestr, "akku is %d", *RamCap());
  DrwStr(datestr, 0, GetFontYSIZE(1), ScreenW(), GetFontYSIZE(1)*2, 1, 0, GetPaletteAdrByColorIndex(0), GetPaletteAdrByColorIndex(23));
  
  int Idx = 0, RC = *RamCap();
  if(RC == 0)
  {
    Idx = 589;
  }
  else
    Idx = 589 + RC / 10;
  if(RC == 100)
    Idx = 600;
  DrawImg(*AkkuX, *AkkuY, Idx);
  
  //110 - 45
  
  if(RN->power == 110)
  {
    Idx = 835;
  }
  else
    Idx = 839 - (RN->power - 60) / (110 - 60) * 10;
  if(RN->power < 60)
    Idx = 840;
  
  DrawImg(*NetX, *NetY, Idx);
  */
  FreeWS(datestr);
}

///////////////////////////////////////////////////////////////////////////////

void OnRedraw()
{
  DrawScreen();
}

void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  ws = AllocWS(128);
  data->gui.state=1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  GBS_DelTimer(&timer);
  FreeWS(ws);
  data->gui.state=0;
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  DisableIDLETMR();
  data->gui.state=2;
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int onkey(unsigned char keycode, int pressed)
{
  if(pressed == KEY_DOWN)
  {
    switch(keycode){
    case ENTER_BUTTON:
      { 
        break;
      }
    case RED_BUTTON:
      {
        return(1); //Закрываем лавочку
      }
    }
  }
  DirectRedrawGUI();
  return(0);
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  //return onkey(msg->gbsmsg->submess, msg->gbsmsg->msg);
  return(1);
}

extern void kill_data(void *p, void (*func_p)(void *));
#ifdef NEWSGOLD
void onDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  kill_data(data,mfree_adr);
}
#endif

int method8(void){return(0);}
int method9(void){return(0);}
const void * const gui_methods[11]={
  (void *)OnRedraw,
  (void *)onCreate,
  (void *)onClose,
  (void *)onFocus,
  (void *)onUnfocus,
  (void *)OnKey,
  0,
  #ifdef NEWSGOLD
  (void *)onDestroy,
  #else
  (void *)kill_data,
  #endif
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,129,129};

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  MAINGUI_ID=csm->gui_id;
}

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  return(1);
}

const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
  {
  maincsm_onmessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  maincsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};

void UpdateCSMname(void)
{
  WSHDR *ws=AllocWS(256);
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),ELFName);
  FreeWS(ws);
}

int main(void)
{
  InitConfig();
  LockSched();
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched();
  return 0;
}
