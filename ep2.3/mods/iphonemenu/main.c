#include "swilib.h"
#include "cfg_items.h"
#include "conf_loader.h"

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

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

//TYPES&STRUCTURES

typedef struct
{
  char signature[16];
  unsigned short picnum;
  unsigned short unk1;
  char w; 
  char h; 
  char Compr_Bits;  
}PICHDR;


typedef struct _ICONINFO{
  char sFile[128];
  char eFile[128];
  char Caption[30];
  int atype;
}IconInfo, *pIconInfo;

typedef struct _ICONPR{
  unsigned int IconSeparator;
  unsigned int IconSize;
}IconPr;


typedef struct _ICONLINE{
  void* Prev;
  pIconInfo* Line;
  int Index;
  void* Next;
}IconLine;

//CONSTANTS
extern const unsigned int cfgCurrent;
extern const char cfgClr[4];
extern const char cfgHeaderColor[4];
extern const char cfgHeaderBrush[4];
extern const unsigned int cfgHeaderFontSize;
extern const unsigned int cfgHeaderX;
extern const unsigned int cfgHeaderY;
extern const char cfgFourColor[4];
extern const int cfgLineCount;
extern const int cfgTopY;
extern const int cfgBottomY;

const char *ELFName = "iPhoneMenu";

//VARIBLES
pIconInfo *IconList = NULL;
IconLine* MenuHead;
unsigned int err;
GBSTMR timer;
unsigned int Header;
int Current = 0;
char IconCount, Lines = 0;
IconPr Def, Four;
char mMode = 0;

//PROTOTYPES
void RunIcon(int type, char* action);
int ReadIcons(char* ConfigName);
void DrawScreen();
int rand(void);
char* GetFileDir(char* FileName);
void InitMenu();
void FreeMenu();
///////////////////////////////////////////////////////////////////////////////
void FreeMenu()
{
  IconLine *Cur, *tmp;
  Cur = MenuHead;
  while(Cur->Next != Cur)
  {
    IconLine* Result = NULL;
    Result = Cur->Prev;
    tmp = Cur->Prev;
    tmp->Next = Cur->Next;
    tmp = Cur->Next;
    tmp->Prev = Cur->Prev;
    mfree(Cur->Line);
    mfree(Cur);
    Cur = Result;
  }
  return;
}
///////////////////////////////////////////////////////////////////////////////
void InitMenu()
{
  int Idx = 0;
  if ((IconCount - 4) % cfgLineCount == 0) 
    Lines = (IconCount- 4) / cfgLineCount;
  else
    Lines = (IconCount - 4) / cfgLineCount + 1;

  IconLine *Head = NULL, *New = NULL, *tmp = NULL;
  
  while(Idx < Lines)
  {
    New = (IconLine*)malloc(sizeof(IconLine));
    New->Line = malloc(sizeof(pIconInfo) * cfgLineCount);
    New->Index = Idx;
    for(int i = 0; i < cfgLineCount; i++)
    {
      New->Line[i] = IconList[Idx * cfgLineCount + 4 + i];
    }
    if(Head == NULL)
    {
      Head = New;
      Head->Prev = Head->Next = New;
    }
    else
    {
      New->Prev = Head;
      New->Next = Head->Next;
      Head->Next = New;
      tmp = New->Next;
      tmp->Prev = New;
    }
    Head = New;
    Idx++;
  }
  MenuHead = Head->Next;
  return;
}

///////////////////////////////////////////////////////////////////////////////
char* GetFileDir(char* FileName)
{
  char *Result;
  for(int i = strlen(FileName) - 1; i > 0; i--)
  {
    if(FileName[i] == '\\')
    {
      Result = (char*)malloc(sizeof(char)*i);
      for(int j = 0; j <= i; j++)
          Result[j] = FileName[j];
      break;
    }
  }
  return Result;
}
////////////////////////////////////////////////////////////////////////////////
int ReadIcons(char* ConfigName)
{
  int bReaded = 0;
  char Count, Result = 0;
  int hConfig = fopen(ConfigName, A_ReadOnly + A_BIN, P_READ, &err);
    if(hConfig != NULL)
    {
      bReaded = fread(hConfig, &Count, sizeof(char), &err); 
      if(bReaded == sizeof(char))
      {
        IconList = (pIconInfo*)malloc(sizeof(pIconInfo)*Count);
        for(int i = 0; i < Count; i++)
        {
          pIconInfo Icon = (pIconInfo)malloc(sizeof(IconInfo));
          bReaded = fread(hConfig, Icon, sizeof(IconInfo), &err);
          IconList[i] = Icon;
        }
        Result = Count;
      }
      fclose(hConfig, &err);
    }
  if(Result != 0)
  {
    Def.IconSize = GetImgWidth((int)IconList[4]->sFile);
    Def.IconSeparator = (ScreenW() - Def.IconSize * cfgLineCount) / (cfgLineCount + 1);
    Four.IconSize = GetImgWidth((int)IconList[0]->sFile);
    Four.IconSeparator = (ScreenW() - Four.IconSize * 4) / 5;
  }
  return Result;
}

////////////////////////////////////////////////////////////////////////////////
unsigned long next=1;

int rand(void) {//типо рандом
 next = next * 1103515245 + 12345;
 return((unsigned int)(next/65536)%32768);
}

////////////////////////////////////////////////////////////////////////////////
int fexists(char* fname)//существует ли файл
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

void TextOut(char *text, int x, int y, int font, int flag, char *pen, char *brush)
{
  WSHDR *txt = AllocWS(strlen(text));
  wsprintf(txt, "%s", text);
  int textlen = 0;
  for(int i = 0; i < strlen(text); i++)
    textlen += GetSymbolWidth(text[i], font);
  DrwStr(txt, x, y, textlen, GetFontYSIZE(font), font, flag, pen, brush);
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

void RunIcon(int type, char* action)
{
   switch(type){
  case 0://run elf
    {
      WSHDR *elfname=AllocWS(256);
      wsprintf(elfname,action);
      ExecuteFile(elfname,NULL,NULL);
      FreeWS(elfname);
      break;
    }
  case 1:
    {
      unsigned int* addr = (unsigned int*)GetFunctionPointer(action);
            if (addr)
            {
              SUBPROC((void*)addr);//run shurtcat
            }
      break;
    }
  case 2:
    {
      typedef void (*voidfunc)(); //run addres
      unsigned int addr=strtoul(action, 0, 16 );
      voidfunc pp=(voidfunc)addr;
      SUBPROC((void*)pp);
      break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void DrawScreen()
{
  void *CanvasData = BuildCanvas();
  DrawCanvas(CanvasData, 0, 0, ScreenW() - 1, ScreenH() - 1, 1);
  IconLine *Cur = MenuHead;
  
  int CurX = 0, CurY = 0, Idx = 0;
  int SelX, SelY;
  
  TextOut(IconList[Current + 4]->Caption, cfgHeaderX, cfgHeaderY, cfgHeaderFontSize,
          0, (char*)&cfgHeaderColor, (char*)&cfgHeaderBrush);
  
  do{
  int Line = Current/cfgLineCount;
  SelY = cfgTopY + Def.IconSeparator + (Def.IconSize + Def.IconSeparator) * (Line - Cur->Index);
  if(SelY > cfgBottomY)
    Cur = Cur->Next;
  }while(SelY > cfgBottomY);
  
  SelX = Def.IconSeparator + (Def.IconSize + Def.IconSeparator) * (Current%cfgLineCount);
  
  while((Idx < Lines)&&(CurY < cfgBottomY))
  {
    for(int i = 0; i < cfgLineCount; i++)
    {
      CurY = cfgTopY + Def.IconSeparator + (Def.IconSize + Def.IconSeparator) * Idx;
      CurX = Def.IconSeparator + (Def.IconSize + Def.IconSeparator) * i;
      DrawImg(CurX, CurY, (int)Cur->Line[i]->sFile);
    }
    Idx++;
    Cur = Cur->Next;
  }
  
  if(mMode == 0)
    DrawRoundedFrame(SelX, SelY, SelX + Def.IconSize, SelY + Def.IconSize,
      0, 0, 0, cfgClr, cfgClr);
  
  
  DrawCanvas(CanvasData, 0, ScreenH() - Four.IconSize - Four.IconSeparator,
             ScreenW() - 1, ScreenH() - 1, 1);

  DrawRoundedFrame(0, ScreenH() - Four.IconSeparator * 2 - Four.IconSize,
                   ScreenW(), ScreenH(), 0, 0, 0, cfgFourColor, cfgFourColor);
                   
  for(int i = 0; i < 4; i++)
    DrawImg(Four.IconSeparator + (Four.IconSeparator + Four.IconSize) * i,
            ScreenH() - Four.IconSeparator - Four.IconSize,
              (int)IconList[i]->sFile);
  
  if(mMode == 1)
    if(Current != 0)
      DrawRoundedFrame(Four.IconSeparator + (Four.IconSeparator + Four.IconSize) * (Current + 4),
                     ScreenH() - Four.IconSeparator - Four.IconSize,
                     Four.IconSeparator + (Four.IconSeparator + Four.IconSize) * (Current + 4) + Four.IconSize,
                     ScreenH() - Four.IconSeparator - Four.IconSize + Four.IconSize,
                     0, 0, 0, cfgClr, cfgClr);

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
  
  if(pressed == KEY_UP)
  {
    if(Current  < 0)
    {
      RunIcon(IconList[Current + 4]->atype, IconList[Current + 4]->eFile);
      Current = 0;
    }
  }
  
  if((pressed == KEY_DOWN)||(pressed == LONG_PRESS))
  {
    switch(keycode){
    case '4':
      break;
    case '6':
      break;
    case '2':
      break;
    case '8':
      break;
    case '5':
      RunIcon(IconList[Current + 4]->atype, IconList[Current + 4]->eFile);
      break;
    case ENTER_BUTTON:
      RunIcon(IconList[Current + 4]->atype, IconList[Current + 4]->eFile);
      break;
    case LEFT_SOFT:
      if(mMode == 1)
      {
        Current = cfgCurrent;
        mMode = 0;
      }
      else
      {
        Current = -1;
        mMode = 1;
      }
      break;
    case LEFT_BUTTON:
      if(mMode == 0)
      {
        if(Current > 0)
          Current--;
        else
          Current = IconCount - 5;
      }
      else
        Current = -4;
      break;
    case RIGHT_BUTTON:
      if(mMode == 0)
      {
        if(Current < IconCount - 5)
          Current++;
        else
          Current = 0;
      }
      else
        Current = -1;
      break;
    case UP_BUTTON:
      if(mMode == 0)
      {
        if((Current - cfgLineCount) >= 0)
          Current -= cfgLineCount;
        else
          Current = IconCount - 4 + (Current - cfgLineCount);
      }
      else
        Current = -3;
      break;
    case DOWN_BUTTON:
      if(mMode == 0)
      {
        if(Current + cfgLineCount < IconCount - 4)
          Current += cfgLineCount;
        else
          Current = Current + cfgLineCount - (IconCount - 4);
      }
      else
        Current = -2;
      break;
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
  return onkey(msg->gbsmsg->submess, msg->gbsmsg->msg);
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
  InitMenu();
}

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{
  for(int i = 0; i < IconCount; i++)
    mfree(IconList[i]);
  mfree(IconList);
  FreeMenu();
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

int main(char *exename, char *fname)
{
  InitConfig();
  IconCount = ReadIcons(DEFAULT_DISK ":\\Zbin\\iPhoneMenu\\iPhoneMenu.config");
  Current = cfgCurrent;
  Header = GetFontYSIZE(cfgHeaderFontSize);
  LockSched();
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched();
  return 0;
}
