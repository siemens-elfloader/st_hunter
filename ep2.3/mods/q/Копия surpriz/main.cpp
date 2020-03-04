#include "..\inc\swilib.h"
#include "../inc/xtask_ipc.h"
//#include "../inc/cfg_items.h"
//#include "conf_loader.h"

//-------------------------------------//

#define IPC_TEXTINFO_NAME "surpriz" 
#define IPC_UPDATE_STAT 1 

const char my_name[]=IPC_TEXTINFO_NAME;
const IPC_REQ my_ipc={//эт структура ипц,в будущем отправим ее таску:)
  my_name,
  my_name,
  NULL
};

//-------------------------------------//
//в принципе это тоже нужно для демона
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);

extern void kill_data(void *p, void (*func_p)(void *));

#pragma segment="ELFBEGIN"
void ElfKiller(void)
{
  kill_data(__segment_begin("ELFBEGIN"),(void (*)(void *))mfree_adr());
}

//-------------------------------------//



#define TMR_SECOND 216
GBSTMR mytmr;
int my_csm_id;
unsigned int *ErrorNumber;

const char percent_t[]="%t";

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;   

// ----------------------------------------------------------------------------
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}
#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

const unsigned char rndtab[256] ={
  1,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
 74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
 95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
 52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   1,
175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
 25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
 94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
 80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
 24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
 28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
 71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
 17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
120, 163, 236, 249
};


unsigned int Random(int min,int max)
{
  static const int DMonth[]={0,31,59,90,120,151,181,212,243,273,304,334,365};
  unsigned int iday;
  TTime tt;
  TDate dd;
  GetDateTime(&dd,&tt);
  dd.year=dd.year%100;
  iday=DMonth[tt.millisec%12];
  iday = (tt.millisec+60*(tt.sec+60*(tt.hour+24* iday)));
  iday*=rndtab[iday%256];
  iday=iday%max;//получаем любое значение из диапазона до максимума
  
  if(iday<min)//если значение меньше минимума
  {
    while(iday<min)
    {
      iday=rndtab[iday%256]%(max-min);
    }    
  }
  return iday;
}

int get_string_width(WSHDR *ws, int font)
{
  int width=0;
  unsigned short *body=ws->wsbody;
  int len=body[0];
  while(len)
  {
    width+=GetSymbolWidth(body[len],font);
    len--;
  }
  return (width+1);//хз, без этого тупит
}

const char per_t[]="%t";

const char per_s[]="%s";
void *canvasdata;
char *s,*s1,*s2,*s3;
int X1=2,Y1=50;
int col=0;
GBSTMR tmr;
int str;
WSHDR *ws1;

int ys = 0;
int xs = 0;

int yy[10][2];
int xx[10][2];

void Canvas()
{


}

int DrawChar(char *s,int x,int y)
{
  y += (!ys ? 3 : 0);
  ys = !ys;  
  if(col<23) col++; else col=0;
  int wslen;
  wsprintf(ws1,per_t,s); 
  wslen = get_string_width(ws1, FONT_MEDIUM);
  int fonth = GetFontYSIZE(FONT_MEDIUM);
  int font = FONT_MEDIUM;
  DrawCanvas(canvasdata, xx[0][0]-10, y - (fonth*2), xx[0][0] + wslen, y + (fonth*(yy[0][1]?2:1)), 1);
  DrawString(ws1,x+2,y,x+wslen+4, y+fonth, font,0,GetPaletteAdrByColorIndex(col),GetPaletteAdrByColorIndex(23));
  return get_string_width(ws1,font);
}

int w;

char white[] = "\xFF\xFF\xFF\x64";

void drawpoint(int h, int w)
{
 //SetPixel(int x,int y,char *color);

  if(h%2) SetPixel(w,h,white);
  else SetPixel(w+5,h,white);
}
/*
void DrawWord(char *s)
{
  int len = strlen(s);
  int x = 0;
  int y = 0;
  //for(int i = 0 ; i < len && *s ; i++)
  {
    y = Random(0,5);
    X1 += DrawChar(s,X1,y);
  }
}*/

void upd1(int x)
{ 
 if((yy[0][0] < (ScreenH()-1 - GetFontYSIZE(FONT_MEDIUM))) && (!yy[0][1])) yy[0][0]+=5;
  else
  {
    yy[0][1] = 1;
    if(yy[0][0] > 0) yy[0][0]-=5;
    else
      yy[0][1] = 0;
  }
  
  if((xx[0][0] + x < ScreenW()-1) && (!xx[0][1])) xx[0][0]+=5;
  else
  {
    xx[0][1] = 1;
    if(xx[0][0] > 0) xx[0][0]-=5;
    else
      xx[0][1] = 0;  
  }
}


void draw1()
{    
  int x = xx[0][0];
  int y = yy[0][0];
  x += DrawChar("Т",x,y);
  x += DrawChar("ы",x,y);
  x += DrawChar(" ",x,y);
  x += DrawChar("т",x,y);
  x += DrawChar("е",x,y);
  x += DrawChar("п",x,y);
  x += DrawChar("е",x,y);
  x += DrawChar("р",x,y);
  x += DrawChar("ь",x,y);
  /*
  char s[2];
  sprintf(s,"%d\\%d",xx[0][0],x);  
  x += DrawChar(s,x,y);*/
  upd1(x);
}


void idle()
{
  void *icsm;
  w=ScreenW();
  icsm=FindCSMbyID(CSM_root()->idle_id);  
  if (icsm)
  {
    if (IsGuiOnTop(idlegui_id(icsm))) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {
        canvasdata = BuildCanvas();
        draw1();
        /*for(int i=0; i < ScreenH()-1;i+=2)
        {
          drawpoint(i,Random(1,ScreenW()-1));        
        }*/
/*        str=DrawChar ("Д",2);
        str+=DrawChar("ю",str);        
        str+=DrawChar("х",str);
        str+=DrawChar("а",str);
        Y1=50;
        X1=35;
        str=DrawChar ("В",2)+2;
        str+=DrawChar("л",str);        
        str+=DrawChar("а",str);
        str+=DrawChar("д",str);
        str+=DrawChar("е",str);
        str+=DrawChar("г",str);
        str=DrawChar ("К",2)+2;
        str+=DrawChar("о",str);
        str+=DrawChar("т",str);
        str+=DrawChar("я",str);
        str+=DrawChar("р",str);
        str+=DrawChar("а",str);

        Y1 = Y1+GetFontYSIZE(FONT_MEDIUM)+5;
        X1 = 25;
        
        str=DrawChar("Н",2)+2;
        str+=DrawChar("О",str);
        str+=DrawChar("В",str);
        str+=DrawChar("Ы",str);
        str+=DrawChar("М",str);
        
        str+=DrawChar("Г",str);
        str+=DrawChar("О",str);
        str+=DrawChar("Д",str);
        str+=DrawChar("О",str);
        str+=DrawChar("М!!!",str);
        str=DrawChar("с",2);
        
        str+=10;
        
        str+=DrawChar("Д",str);
        str+=DrawChar("н",str);
        str+=DrawChar("ю",str);
        str+=DrawChar("х",str);
        str+=DrawChar("о",str);
        str+=DrawChar("й",str);
        str+=DrawChar("!",str);

  
        wsprintf(ws1,per_t,"Ты постарел(");
        DrawString(ws1,2,Y1+GetFontYSIZE(FONT_MEDIUM)*2,ScreenW()-1,Y1+GetFontYSIZE(FONT_MEDIUM)*2+5+GetFontYSIZE(FONT_SMALL),FONT_SMALL,2,GetPaletteAdrByColorIndex(col),GetPaletteAdrByColorIndex(23));*/
      }
    }
  }
}

void check()
{
 idle();
 GBS_StartTimerProc(&tmr, 10*TMR_SECOND/10,check);
}
int maincsm_onmessage(CSM_RAM* data,GBS_MSG* msg)
{

 return(1);
}

static void maincsm_oncreate(CSM_RAM *data)
{ 
 ws1=AllocWS(256);
 yy[0][0] = 10;
 xx[0][0] = 2;
 xx[0][1] = 0;
}

void maincsm_onclose(CSM_RAM *data)
{ 
  GBS_DelTimer(&tmr);
  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
  extern void *ELF_BEGIN;
}

static unsigned short maincsm_name_body[140];
const int minus11=-11;

static const struct
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
  sizeof(CSM_RAM),
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


static void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"surpriz");
}
// ----------------------------------------------------------------------------


CSM_DESC icsmd;
int main()
{
//-------------------------------------//
  CSMROOT *csmr;//А это самое интересное:) тут замуты с цсм для того чтобы он определялся как демон,А не просто процесс
  CSM_RAM *save_cmpc;//и голову ломать не стоит,прост копируешь отсюда и усе...
  CSM_RAM main_csm;
  UpdateCSMname();
  LockSched();
  csmr=CSM_root();
  save_cmpc=csmr->csm_q->current_msg_processing_csm;
  csmr->csm_q->current_msg_processing_csm=csmr->csm_q->csm.first;
  CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  csmr->csm_q->current_msg_processing_csm=save_cmpc;
  UnlockSched();
//-------------------------------------//
  ShowMSG(1,(int)"Во Бля!!!");  
  check();

  const char ipc_xtask_name[]=IPC_XTASK_NAME;
  
  IPC_REQ gipc;
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=my_name;
  gipc.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);

  return (0);
}
