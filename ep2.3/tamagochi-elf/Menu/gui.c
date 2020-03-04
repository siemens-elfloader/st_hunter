#include "..\inc\swilib.h"
#include "..\inc\cfg_items.h"
#include "conf_loader.h"
#include "Items.h"
#include "main.h"
#include "math.h"
#include "revision.h"


#define TMR_SECOND 216
#define PI 3.1415926535

extern const char percent_t[];

extern void _WriteLog(char *buf);

//extern int ACTIVE_KEY;
extern char CSMText[32];
extern int closeMenu;
//==============================================================
extern int Animation;
extern unsigned int SpeedAnim;
//==============================================================
extern char MENU_PATH[64];
//extern char IMGMENU_PATH[64];
  //==============================================================
extern char TextRight[32];
extern char TextLeft[32];
extern char RunLeft[64];

// для чтения файла меню
typedef struct{
  char Text[32];
  char Description[128];
  char IconBig[128];
  char IconSmall[128];
  char Run[128];
} TMenuPoint;

extern void ascii2ws(WSHDR* ws, const char* s);

//--------------------------------------------------------------------//
//                           READ CONFIGS                             //
//--------------------------------------------------------------------//

//--------------------------------------------------------------------//
//------------------------  Menu     CONFIG  -------------------------//
//--------------------------------------------------------------------//

typedef struct 
{
 //menu
CFG_HDR cfghdr_m11;
CFG_HDR cfghdr1_4;
unsigned int Rows;
CFG_HDR cfghdr1_5;
unsigned int Columns;
CFG_HDR cfghdr1_3;
unsigned int Start;
CFG_HDR cfghdr1_10;
int cursorShow;
CFG_HDR cfghdr2_3;
char CURSOR_PATH[64];
CFG_HDR cfghdr1_6;
RECT menuRect;
CFG_HDR cfghdr1_7;
int styleMenu;
CFG_CBOX_ITEM cfgcbox0[3];
CFG_HDR cfghdr_f11;
  CFG_HDR cfghdrfl_h0;
  unsigned int listNameFont;
  CFG_HDR cfghdrfl_h1;
  unsigned int listNameStyle;
  CFG_HDR cfghdrfl_h2;
  char listNameColor[4];
  CFG_HDR cfghdrfl_8;
  int descListShow;
  CFG_CBOX_ITEM cfgcboxl0[3];
  CFG_HDR cfghdrfl_h3;
  unsigned int listDescFont;
  CFG_HDR cfghdrfl_h4;
  unsigned int listDescStyle;
  CFG_HDR cfghdrfl_h5;
  char listDescColor[4];
CFG_HDR cfghdr_f10;
CFG_HDR cfghdr_crc11;
CFG_HDR cfghdrcrc_5;
unsigned int Radius;
CFG_HDR cfghcrc_9;
int _position_cursor;
CFG_CBOX_ITEM cfgcboxcrc_0[4];
CFG_HDR cfghdrcrc_6;
int _AnimationCirc;
CFG_HDR cfghdrcrc_7;
unsigned int _SpeedAnimCirc;
CFG_HDR cfghdrcrc_8;
unsigned int _GradAnimCirc;
CFG_HDR cfghdr_crc10;
CFG_HDR cfghdr_pi11;
  CFG_HDR cfghdr1_8;
  int position_type;
  CFG_CBOX_ITEM cfgcboxpi0[2];
  CFG_HDR cfghdr1_9;
  unsigned int OffsetX;
  CFG_HDR cfghdr1pi_10;
  unsigned int OffsetY;
CFG_HDR cfghdr_pi10;
CFG_HDR cfghdr_m10;
  //header
CFG_HDR cfghdr_h11;
  CFG_HDR cfghdr0_h1;
  int headShow;
  CFG_HDR cfghdr0_h11;
  int headIconShow;
  CFG_HDR cfghdr0_h2;
  RECT headRect;
  CFG_HDR cfghdr0_h3;
  unsigned int headFont;
  CFG_HDR cfghdr0_h4;
  unsigned int headStyle;
  CFG_HDR cfghdr0_h5;
  char headColor[4];
CFG_HDR cfghdr_h10;
  //description
CFG_HDR cfghdr_k11;
  CFG_HDR cfghdr0_1 ;
  int descShow;
  CFG_HDR cfghdr0_2;
  RECT descRect;
  CFG_HDR cfghdr0_3;
  unsigned int descFont;
  CFG_HDR cfghdr0_4;
  unsigned int descStyle;
  CFG_HDR cfghdr0_5;
  char deskColor[4];
CFG_HDR cfghdr_k10;
    //description
CFG_HDR cfghdr_s11;
  CFG_HDR cfghdrs1_8;
  int scrollShow;
  CFG_CBOX_ITEM cfgcboxs0[3];
  CFG_HDR cfghdrs0_5;
  char scrollColor[4];
CFG_HDR cfghdr_s10;
}menuConfig;





///////////////////////////////////////////
// Menu VALUE
unsigned int Rows;
unsigned int Columns;
unsigned int Start;
int cursorShow;
char CURSOR_PATH[64];
RECT menuRect;
int styleMenu;
  unsigned int listNameFont;
  unsigned int listNameStyle;
  char listNameColor[4];
  int descListShow;
  unsigned int listDescFont;
  unsigned int listDescStyle;
  char listDescColor[4];
  unsigned int Radius;
  int position_cursor;
  int AnimationCirc;
  unsigned int SpeedAnimCirc;
  unsigned int GradAnimCirc;
  int position_type;
  unsigned int OffsetX;
  unsigned int OffsetY;
  //header
  int headShow;
  int headIconShow;
  RECT headRect;
  unsigned int headFont;
  unsigned int headStyle;
  char headColor[4];
  //description
  int descShow;
  RECT descRect;
  unsigned int descFont;
  unsigned int descStyle;
  char descColor[4];
    //SCROLL
  int scrollShow;
  char scrollColor[4];
///////////////////////////////////////////////
float curpos=0;

typedef struct
{
  int flag; //0, 8, 16 хз
  int (*onKey)(GUI *gui, GUI_MSG *msg);
  void (*global_hook_proc)(GUI *gui, int cmd);
  void *locret; //0
  const int *softkeys;
  const SOFTKEYSTAB *softkeystab;
  int unkflag; //1
  int lgp_null;
  const int *icons; //&x52A
  int pos; //0 (pos?)
  int font; //4
  int textcolor; //0x64
  int _101;
  int timeout; //0-forever
} MSG_BOX;


int(*MsgBox)(int _1, int, MSG_BOX*, int lgp_id);
//----------------MMenu----------------
int MSoftKeys[]={0,1,2};

SOFTKEY_DESC Msk[]=
{
  {0x0018,0x0000,(int)TextLeft},
  {0x0000,0x0000,(int)TextRight},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB Mskt=
{
  Msk,0
};

extern GBSTMR tmr;
GBSTMR tmrcrc;

int CellX;
int CellY;
int pic_n=0;
int pos=0;

// список вложенных меню
LIST *MenuTop;


#define TIME 262/5


extern int LoadItems(const char *);


int isFile(char *fname)
{
  return (strstr(fname,".")!=0);
}

int isDir(char *fname)
{
  return (strstr(fname,":")!=0);
}

int isSub(char *fname)
{
  return (strstr(fname,".cfg")!=0);
}

int strh2int(char* str)
{
  int n,c=0;
  if (*(str+strlen(str)-1)=='h') *(str+strlen(str)-1)='\0';
  while ((*str!='h')&&(*str))
  {
    if ((*str>='a')&&(*str <='f')) n=*str-('A'-('9'+1))-('a'-'A');
    else if ((*str>='A')&&(*str<='F')) n=*str-('A'-('9'+1));
    else if ((*str>='0')&&(*str<='9')) n=*str;
    c=c*16+(n-'0');
    str++;
  }
  return c;
}

int str2int(const char *str)
{
  int n,c=0;
  while(*str!='\0')
  {
    n=*str;
    c=c*10+(n-'0');
    str++;
  }
  return c;
}

void RunEntry(char *entry)
{
  typedef int (*func)(void);
  func ff;
  ff=(func)strh2int(entry);
  if (ff!=NULL) SUBPROC((void *)(*ff));
}

void RunShort(char *fname)
{
  typedef void (*voidfunc)(); 
  voidfunc pp=(voidfunc)GetFunctionPointer(fname); 
  if(pp!=0) 
    SUBPROC((void*)pp);
  else
    ShowMSG(1,(int)"NULL pointer function!");
}

char *nill="";
void RunFile(char *fname)
{
  WSHDR *ws=AllocWS(256);
  str_2ws(ws,fname,strlen(fname)+1);
  ExecuteFile(ws,0,nill);
  FreeWS(ws);
}

void RunDir(char *dirname)
{

NativeExplorerData *NatData=malloc(sizeof(NativeExplorerData));
zeromem(NatData,sizeof(NativeExplorerData));  
NatData->mode=0;
NatData->dir_enum=0x26;
NatData->is_exact_dir=1;

  NatData->file_name=AllocWS(256);
  NatData->path_to_file=AllocWS(256);
  str_2ws(NatData->path_to_file,dirname,strlen(dirname)+1);
  NatData->full_filename=AllocWS(256);
  //str_2ws(NatData->full_filename,dirname,strlen(dirname)+1);
    
StartNativeExplorer(NatData);
  
FreeWS(NatData->file_name);
FreeWS(NatData->path_to_file);
FreeWS(NatData->full_filename);
mfree(NatData);

}


void RunSub(char *sub_name)
{
      LockSched();
      LIST *sub=malloc(sizeof(LIST));
      sub->next=NULL;
      sub->pos=pos;
      sprintf(sub->item,sub_name); 
      if(!MenuTop)
          MenuTop=sub; 
       else
        {
         sub->next=MenuTop;
         MenuTop=sub;
        }
      LoadItems(sub_name);
      //pos=0;
      UnlockSched();
}


int Run(const char *s)
{
  char *file=malloc(strlen(s)+1);
  strcpy(file,s);
  int fsub=isSub(file);
  if (fsub) RunSub(file);
  else
  if (isFile(file)) RunFile(file);
  else
  if (isDir(file)) RunDir(file);
  else  
  if (strstr(file,"_")!=0)  
     RunShort(file); 
    else 
     RunEntry(file);

  mfree(file);  
  if ((closeMenu)&&(!fsub))
  return(1);
  else
  return(0);
}
// ----------------------------------------------
GBSTMR tmr;

void PickNumTmr()
{
  pic_n++;
  if(pic_n>=TotalAnim(pos))
    pic_n=0;
  DirectRedrawGUI();
  GBS_StartTimerProc(&tmr, SpeedAnim*TMR_SECOND/100, PickNumTmr);
}


void TmrReset()
{
  GBS_DelTimer(&tmr);
  pic_n=0;
  GBS_StartTimerProc(&tmr, SpeedAnim*TMR_SECOND/100, PickNumTmr);
}

void CircleTmr()
{
  
  int TI=TotalItems();
  float rad=2*PI/TI;
  
   // нет анимации
  if (!AnimationCirc) curpos=pos*rad;
  else
  {
    // анимация
    float grad=2*PI/360*GradAnimCirc;
    
    // скачок в начале круга и в конце
    if ((curpos==(TI-1)*rad)&&(pos==0))
      curpos=-rad;
    if ((curpos==0)&&(pos==TI-1))
      curpos=2*PI;
    // сдвиг  
    if (curpos>pos*rad)
      curpos-=grad;
    if (curpos<pos*rad)
      curpos+=grad;
    
    //точное позиционирование
    if (curpos-pos*rad>0)
    {
      if (curpos-pos*rad<grad) curpos=pos*rad;
    }
    else
      if (pos*rad-curpos<grad) curpos=pos*rad;
  }
  if((styleMenu==2))
  DirectRedrawGUI();
  GBS_StartTimerProc(&tmrcrc, SpeedAnimCirc*TMR_SECOND/100, CircleTmr);
}

void CircleTmrReset()
{
  GBS_DelTimer(&tmrcrc);
  //if (AnimationCirc)
  GBS_StartTimerProc(&tmrcrc, SpeedAnimCirc*TMR_SECOND/100, CircleTmr);
}

//extern unsigned long  strtoul (const char *nptr,char **endptr,int base);
int CalcPic(char* picture)
{
  int pic = strtoul(picture, 0, 10);
  if ( (pic <= 4) && (strlen(picture) > 1) )
  {
     pic = (int)picture;
  }
  return pic;
}



char transparent[]={0x00,0x00,0x00,0x00};
const int font=FONT_SMALL;

void (*OOnRedraw)(GUI *data);
void NOnRedraw(GUI *data)
{
  OOnRedraw(data); //Сначала старый OnRedraw

  int TI=TotalItems();
  int delta=0;
  int cursor=CalcPic((char*)CURSOR_PATH);
  int icon;
  int x=0;
  int y=0;
  int cx=0;
  int cy=0;
  
  if (TI==0)
  {
      WSHDR *ws=AllocWS(128);
      ascii2ws(ws,"Empty");
      DrawString(ws,menuRect.x,
                 menuRect.y+((menuRect.y2-headRect.y-GetFontYSIZE(headFont))/2),
                 menuRect.x2,menuRect.y2,headFont,2,headColor,0);
      FreeWS(ws);
    return;
  }
  
  switch (styleMenu)
  {
  // отрисовка сетки
  case 0: 
    {
     /* 
      if (pos+1>(Columns*(Rows-1)))
      {
       if (pos-(Columns*Rows-1)>0) 
          delta=(pos-(Columns*Rows-1))/Columns;
       if ((pos-(Columns*Rows-1))%Columns>0) ++delta;
       if (delta*Columns+Columns*Rows<TI)delta++;
      }
      */
    
      if (pos>(Columns*Rows-1))
        {
         delta=(pos-(Columns*Rows-1))/Columns;
         if ((pos-(Columns*Rows-1))%Columns!=0)
         ++delta;
        }
      
      int setka=Columns*Rows;
      if ((TI-delta*Columns)<(Columns*Rows))
        setka=TI-delta*Columns;
      

      int posi=0;
      for(int i=0;i<setka;i++)
      if ((i+delta*Columns)!=pos) 
      {

    
       ITEM *Item=GetItem(i+delta*Columns);
        if ((!Animation)||((i+delta*Columns)!=pos))
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    /* --==Формула==-- */
        if (position_type==0)
        {
        x=menuRect.x+(CellX-GetImgWidth(icon))/2+CellX*(i%Columns);
        y=menuRect.y+(CellY-GetImgHeight(icon))/2+CellY*(i/Columns);
        cx=menuRect.x+(CellX-GetImgWidth(cursor))/2+CellX*(i%Columns);
        cy=menuRect.y+(CellY-GetImgHeight(cursor))/2+CellY*(i/Columns);
        }
        else
        {
        x=menuRect.x+OffsetX+CellX*(i%Columns);
        y=menuRect.y+OffsetY+CellY*(i/Columns);
        cx=x;
        cy=y;
        }
        DrawImg(x,y,icon);
      }else posi=i;
      
      //otrisovka выделенного
  
       ITEM *Item=GetItem(posi+delta*Columns);
        if ((!Animation)||((posi+delta*Columns)!=pos))
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    /* --==Формула==-- */
        if (position_type==0)
        {
        x=menuRect.x+(CellX-GetImgWidth(icon))/2+CellX*(posi%Columns);
        y=menuRect.y+(CellY-GetImgHeight(icon))/2+CellY*(posi/Columns);
        cx=menuRect.x+(CellX-GetImgWidth(cursor))/2+CellX*(posi%Columns);
        cy=menuRect.y+(CellY-GetImgHeight(cursor))/2+CellY*(posi/Columns);
        }
        else
        {
        x=menuRect.x+OffsetX+CellX*(posi%Columns);
        y=menuRect.y+OffsetY+CellY*(posi/Columns);
        cx=x;
        cy=y;
        }
        if(((posi+delta*Columns)==pos)&&cursorShow)
        DrawImg(cx,cy,cursor);
        DrawImg(x,y,icon);
        
      
      // scrollbar
      int rowScroll=TI/Columns;
      if (TI>rowScroll*Columns) ++rowScroll;
      if ((scrollShow==1)||((rowScroll>Rows)&&(scrollShow==0)))
      {
        int posScroll=(pos)/Columns;
        //if (pos>posScroll*Columns) ++posScroll;
        int sh=(menuRect.y2-menuRect.y)/rowScroll;
        DrawLine(menuRect.x2-1,menuRect.y,menuRect.x2-1,menuRect.y2, LINE_DOTTED,scrollColor);
        DrawRectangle(menuRect.x2,menuRect.y+sh*posScroll,menuRect.x2-2,menuRect.y+sh*posScroll+sh,
                      0,0,scrollColor);
      }
   
      
      break;
    }
  // отрисовка списка
  case 1:
    {
      if (pos>=(Rows-1))
      {
       delta=(pos-(Rows-1));
      if (delta+Rows<TI)
       delta++;    
      }
      
      int posi=0;
      int list=Rows;
      if (Rows>TI) list=TI;
      for(int i=0;i<list;i++)
      if ((i+delta)!=pos) 
      {
       ITEM *Item=GetItem(i+delta);
        if ((!Animation)||((i+delta)!=pos))
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    /* --==Формула==-- */
        if (position_type==0)
        {
        x=menuRect.x+(CellY-GetImgHeight(icon))/2;
        y=menuRect.y+(CellY-GetImgHeight(icon))/2+CellY*i;
        cx=menuRect.x+(CellY-GetImgHeight(cursor))/2;
        cy=menuRect.y+(CellY-GetImgHeight(cursor))/2+CellY*i;
        }
        else
        {
        x=menuRect.x+OffsetX;
        y=menuRect.y+OffsetY+CellY*i;
        cx=x;
        cy=y;
        }
        
        DrawImg(x,y,icon);
        
        WSHDR *ws=AllocWS(128);
        if (descListShow==0)
        {
        ascii2ws(ws,Item->Text);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*i+((CellY/2-GetFontYSIZE(listNameFont))/2),
                   menuRect.x2,menuRect.y+CellY*i+CellY/2,listNameFont,listNameStyle,listNameColor,0);
        ascii2ws(ws,Item->Description);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*i+CellY/2+((CellY/2-GetFontYSIZE(listDescFont))/2),
                   menuRect.x2,menuRect.y+CellY*i+CellY,
                   listDescFont,listDescStyle,listDescColor,0);
        }
        else
        {
        ascii2ws(ws,Item->Text);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*i+((CellY-GetFontYSIZE(listNameFont))/2),
                   menuRect.x2,menuRect.y+CellY*i+CellY,listNameFont,listNameStyle,listNameColor,0);
        }
        FreeWS(ws);
        
      }
      else posi=i;
      
        ITEM *Item=GetItem(posi+delta);
        if ((!Animation)||((posi+delta)!=pos))
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    /* --==Формула==-- */
        if (position_type==0)
        {
        x=menuRect.x+(CellY-GetImgHeight(icon))/2;
        y=menuRect.y+(CellY-GetImgHeight(icon))/2+CellY*posi;
        cx=menuRect.x+(CellY-GetImgHeight(cursor))/2;
        cy=menuRect.y+(CellY-GetImgHeight(cursor))/2+CellY*posi;
        }
        else
        {
        x=menuRect.x+OffsetX;
        y=menuRect.y+OffsetY+CellY*posi;
        cx=x;
        cy=y;
        }
        
        if(((posi+delta)==pos)&&cursorShow)
        DrawImg(cx,cy,cursor);
        
        DrawImg(x,y,icon);
        
        WSHDR *ws=AllocWS(128);
        ascii2ws(ws,Item->Text);
        if (descListShow!=2)
        {
        ascii2ws(ws,Item->Text);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*posi+((CellY/2-GetFontYSIZE(listNameFont))/2),
                   menuRect.x2,menuRect.y+CellY*posi+CellY/2,listNameFont,listNameStyle,listNameColor,0);
        ascii2ws(ws,Item->Description);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*posi+CellY/2+((CellY/2-GetFontYSIZE(listDescFont))/2),
                   menuRect.x2,menuRect.y+CellY*posi+CellY,
                   listDescFont,listDescStyle,listDescColor,0);
        }
        else
        {
        ascii2ws(ws,Item->Text);
        DrawString(ws,2*x+GetImgWidth(icon),
                   menuRect.y+CellY*posi+((CellY-GetFontYSIZE(listNameFont))/2),
                   menuRect.x2,menuRect.y+CellY*posi+CellY,listNameFont,listNameStyle,listNameColor,0);
        }
        FreeWS(ws);

      
      // scrollbar
      if ((scrollShow==1)||((TI>Rows)&&(scrollShow==0)))
      {
        int sh=(menuRect.y2-menuRect.y)/TI;
        DrawLine(menuRect.x2-1,menuRect.y,menuRect.x2-1,menuRect.y2, LINE_DOTTED,scrollColor);
        DrawRectangle(menuRect.x2,menuRect.y+sh*pos,menuRect.x2-2,menuRect.y+sh*pos+sh,
                      0,0,scrollColor);
      }
   
      break;
    }
    // отрисовка круга  
    case 2:
    {
      float ugol_cur=0;
      switch (position_cursor){
        case 0:ugol_cur=-PI/2;break;
        case 1:ugol_cur=PI/2;break;
        case 2:ugol_cur=-PI;break;
        case 3:ugol_cur=0;break;
      }
        
      
      int posi=0;
      float rad=2*PI/TI;
      int centerX=(menuRect.x2-menuRect.x)/2;
      int centerY=(menuRect.y2-menuRect.y)/2;
      float ugol=-PI/2;
      cx=menuRect.x+centerX+Radius*cos(ugol_cur)-GetImgWidth(cursor)/2;
      cy=menuRect.y+centerY+Radius*sin(ugol_cur)-GetImgHeight(cursor)/2;

      for(int i=0;i<TI;i++)
      if (i!=pos) 
      {
       ITEM *Item=GetItem(i);
        if ((!Animation)||((i)!=pos))
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    /* --==Формула==-- */
        
        ugol=(i*rad)+ugol_cur-curpos;
        x=menuRect.x+centerX+Radius*cos(ugol)-GetImgWidth(icon)/2;
        y=menuRect.y+centerY+Radius*sin(ugol)-GetImgHeight(icon)/2;
        DrawImg(x,y,icon);
      }
      else
      posi=i;
      
        ITEM *Item=GetItem(posi);
        if (!Animation)
          icon=CalcPic(Item->IconBig);
         else
          if (TotalAnim(pos)==0) 
          icon=CalcPic(Item->IconBig);
          else
          icon=CalcPic(IconAnim(pos,pic_n));
    /* --==Формула==-- */
        
        ugol=(posi*rad)+ugol_cur-curpos;
        x=menuRect.x+centerX+Radius*cos(ugol)-GetImgWidth(icon)/2;
        y=menuRect.y+centerY+Radius*sin(ugol)-GetImgHeight(icon)/2;
        
       if(cursorShow)
        DrawImg(cx,cy,cursor);

        DrawImg(x,y,icon);
      
      // scrollbar
      if ((scrollShow==1)||((TI>Rows)&&(scrollShow==0)))
      {
        int sh=(menuRect.y2-menuRect.y)/TI;
        DrawLine(menuRect.x2-1,menuRect.y,menuRect.x2-1,menuRect.y2, LINE_DOTTED,scrollColor);
        DrawRectangle(menuRect.x2,menuRect.y+sh*pos,menuRect.x2-2,menuRect.y+sh*pos+sh,
                      0,0,scrollColor);
      }
   
      break;
    }
  }
 
  
  ITEM *Item=GetItem(pos);
  if (descShow)
    {
      WSHDR *ws=AllocWS(128);
      ascii2ws(ws,Item->Description);
      DrawString(ws,descRect.x,
                 descRect.y+((descRect.y2-descRect.y-GetFontYSIZE(descFont))/2),
                 descRect.x2,descRect.y2,descFont,descStyle,descColor,0);
      FreeWS(ws);
    }
  if (headShow)
    {
      int of=2;
      if (headIconShow)
      {
      int headIcon=CalcPic(Item->IconSmall);
      int hi=(headRect.y2-headRect.y-GetImgHeight(headIcon))/2;
      DrawImg(headRect.x+hi,headRect.y+hi,headIcon);
      of=of+GetImgWidth(headIcon)+2*hi+2;
      }
      
      WSHDR *ws=AllocWS(128);
      ascii2ws(ws,Item->Text);
      DrawString(ws,headRect.x+of,
                 headRect.y+((headRect.y2-headRect.y-GetFontYSIZE(headFont))/2),
                 headRect.x2,headRect.y2,headFont,headStyle,headColor,0);
      FreeWS(ws);
    }
  //_WriteLog("end draw");
}

const void * NGuiMeths[11];

void MGHook(GUI *gui, int cmd)
{
  switch(cmd)
  {
  case 1://Создание: подмена onRedraw
    {
      memcpy(NGuiMeths,gui->methods,11*sizeof(void*));
      gui->methods=(void*)NGuiMeths;
      OOnRedraw=(void(*)(GUI*))NGuiMeths[0];//(gui->methods[0]);
      NGuiMeths[0]=(void*)NOnRedraw;
    }
    break;
  case 5://Получение фокуса (?)
    TmrReset();
    CircleTmrReset();
    break;
  case 6://Потеря фокуса
    GBS_DelTimer(&tmr);//Экономим ресурсы (типо)
    GBS_DelTimer(&tmrcrc);
    break;
  }
}


extern void ShowEditor();
extern char path_menu[128];

int MOnKey(GUI *gui, GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  
  if(msg->gbsmsg->msg==LONG_PRESS)
  {
     if (key=='*')
      {
        char s[256];
        sprintf(s,"Menu v1.5 rev.%d\n(c)Eraser\n%s at %s",ELF_REVISION,__DATE__,__TIME__);
        ShowMSG(2,(int)s);
        return(0);
      }
     if (key=='#')
      {
         // редактирование настроек
        WSHDR *ws;
        ws=AllocWS(150);
        char config_name[128];
        if (MenuTop)
        sprintf(config_name, "%s.bcfg", MenuTop->item);
        else
        sprintf(config_name, "%s.bcfg",MENU_PATH);
        
        str_2ws(ws,config_name,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
        return(0);
      }
     if (key=='0')
      {
        // редактирование пунктов
        char config_name[128];
        if (MenuTop)
        sprintf(config_name, percent_t, MenuTop->item);
        else
        sprintf(config_name, percent_t,MENU_PATH);
        sprintf(path_menu,percent_t,config_name);
        ShowEditor();
        return(0);
      }
  }
  
  if(msg->gbsmsg->msg==KEY_UP)
  {
    switch(key)
    {
    case ENTER_BUTTON:
      goto run;
    case LEFT_BUTTON:
      if (styleMenu==2)
      {
        switch (position_cursor){
        case 0:pos--;break;
        case 1:pos++;break;
        case 2:pos--;break;
        case 3:pos--;break;
       }
      }
      else
        pos--;
      break;
    case RIGHT_BUTTON:
      if (styleMenu==2)
      {
        switch (position_cursor){
        case 0:pos++;break;
        case 1:pos--;break;
        case 2:pos++;break;
        case 3:pos++;break;
       }
      }
      else
      pos++;
      break;
    case UP_BUTTON:
      switch (styleMenu)
      {
        case 0:pos-=Columns;break;
        case 1:pos--;break;
        case 2:
          switch (position_cursor){
            case 0:pos++;break;
            case 1:pos--;break;
            case 2:pos++;break;
            case 3:pos--;break;
           };break;
      }
      break;
    case DOWN_BUTTON:
      switch (styleMenu)
      {
        case 0:pos+=Columns;break;
        case 1:pos++;break;
        case 2:
          switch (position_cursor){
            case 0:pos--;break;
            case 1:pos++;break;
            case 2:pos--;break;
            case 3:pos++;break;
           };break;
      }
      break;
    case RIGHT_SOFT:
      if (!MenuTop)
      {
        GBS_DelTimer(&tmr);
        GBS_DelTimer(&tmrcrc);
        return 1;
      }
      else
      {
        LIST *List=(LIST *)MenuTop->next;
        int post=MenuTop->pos;
        mfree(MenuTop);
        MenuTop=List;
        LockSched();
        if (MenuTop)
        LoadItems(MenuTop->item);
        else
        LoadItems(MENU_PATH);
        pos=post;
        UnlockSched();
      }
      
      break;
      //return(0);
    case LEFT_SOFT:
      Run(RunLeft);
      return(closeMenu);
      
    case '*':
      pos=9;
      goto run;
    case '0':
      pos=10;
      goto run;
    case '#':
      pos=11;
      goto run;
   default:
      if((key>='1') && (key<='9'))
      {
        pos=key-1-'0';
        goto run;
      }
      else
        return(0);
    }
      
    if(pos<0)
      pos=TotalItems()-1;
    if(pos>TotalItems()-1)
      pos=0;

  //end:
    TmrReset();
    RefreshGUI();
    return(0);
  run:
    {
    int ti=TotalItems();
    if (ti==0) return 0;
    if (pos>=ti) 
    {
      if(pos>ti-1)
      pos=0;
      return(0);  
    }
    ITEM *Item=GetItem(pos);
    return Run(Item->Run);
    }
   
  }
  return(0);
}

MSG_BOX MMenu=
{
  0,
  MOnKey,
  MGHook,
  0,
  MSoftKeys,
  &Mskt,
  1,
  LGP_NULL,
  0,
  0,
  0,
  0x64,
  0x65,
  0
};


int LoadItems(const char *menu_path)
{
  FSTATS stat;
  char *fn;
  int f;
  unsigned int ul;
  int i;
  int fsize;
  int c;
  
  FreeItemsList();

  fn=(char *)menu_path;
  if (GetFileStats(fn,&stat,&ul)==-1) return 0;
  if ((fsize=stat.size)<=0) return 0;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1) return 0;

//  _WriteLog("Menu.cfg find");
  c=fsize/sizeof(TMenuPoint);
//  char msg[512];
//  sprintf(msg, "%02d %s\n", c,"item");
//  _WriteLog(msg);

  LockSched();
  TMenuPoint *itemsF;
  itemsF=malloc(c*sizeof(TMenuPoint));
  for (i=0;i<c;i++)
  {
   fread(f,&itemsF[i],sizeof(TMenuPoint),&ul);
   AddToItem(itemsF[i].Text,itemsF[i].Description,itemsF[i].IconSmall,itemsF[i].IconBig, itemsF[i].Run);
  }
  fclose(f,&ul);
  UnlockSched();
  
  mfree(itemsF);
  
  // загрузка настроек
 int menuConfigSize = sizeof(menuConfig);
 menuConfig* menuConf = (menuConfig*)malloc(menuConfigSize);
 extern const CFG_HDR cfghdr_m11;
 char config_name[128];
  sprintf(config_name, "%s.bcfg", fn);
 int newCfgFile = InitConfig(menuConf, menuConfigSize, config_name,(void*)&cfghdr_m11);
 if(newCfgFile!=-1)//Если конфиг есть
{

  Rows=menuConf->Rows;
  Columns=menuConf->Columns;
Start=menuConf->Start;
cursorShow=menuConf->cursorShow;
sprintf(CURSOR_PATH,percent_t,menuConf->CURSOR_PATH);

menuRect=menuConf->menuRect;
styleMenu=menuConf->styleMenu;
listNameFont=menuConf->listNameFont;
listNameStyle=menuConf->listNameStyle;
 listNameColor[0]=menuConf->listNameColor[0];
 listNameColor[1]=menuConf->listNameColor[1];
 listNameColor[2]=menuConf->listNameColor[2];
 listNameColor[3]=menuConf->listNameColor[3];
descListShow=menuConf->descListShow;
listDescFont=menuConf->listDescFont;
listDescStyle=menuConf->listDescStyle;
 listDescColor[0]=menuConf->listDescColor[0];
 listDescColor[1]=menuConf->listDescColor[1];
 listDescColor[2]=menuConf->listDescColor[2];
 listDescColor[3]=menuConf->listDescColor[3];
Radius=menuConf->Radius;
position_cursor=menuConf->_position_cursor;
AnimationCirc=menuConf->_AnimationCirc;
SpeedAnimCirc=menuConf->_SpeedAnimCirc;
GradAnimCirc=menuConf->_GradAnimCirc;
position_type=menuConf->position_type;
OffsetX=menuConf->OffsetX;
OffsetY=menuConf->OffsetY;
  //header
headShow=menuConf->headShow;
headIconShow=menuConf->headIconShow;
headRect=menuConf->headRect;
headFont=menuConf->headFont;
headStyle=menuConf->headStyle;
 headColor[0]=menuConf->headColor[0];
 headColor[1]=menuConf->headColor[1];
 headColor[2]=menuConf->headColor[2];
 headColor[3]=menuConf->headColor[3];

  //description
descShow=menuConf->descShow;
descRect=menuConf->descRect;
descFont=menuConf->descFont;
descStyle=menuConf->descStyle;
 descColor[0]=menuConf->deskColor[0];
 descColor[1]=menuConf->deskColor[1];
 descColor[2]=menuConf->deskColor[2];
 descColor[3]=menuConf->deskColor[3];

    //SCROLL
scrollShow=menuConf->scrollShow;
 scrollColor[0]=menuConf->scrollColor[0];
 scrollColor[1]=menuConf->scrollColor[1];
 scrollColor[2]=menuConf->scrollColor[2];
 scrollColor[3]=menuConf->scrollColor[3];

  
  mfree(menuConf);
  
  int w = menuRect.x2 - menuRect.x;
  int h = menuRect.y2 - menuRect.y;
  
  CellX=w/Columns;
  CellY=h/Rows;
  
  pos=Start;
  int TI=TotalItems();
  float rad=2*PI/TI;
  curpos=pos*rad;
}
else
  {
     LockSched();
     ShowMSG(1,(int)"Can't open config menu!");
     UnlockSched();
  }
  return c;
}


typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

int my_csm_id;

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_CSM *csm=(MAIN_CSM *)data;
  csm->csm.state=0;
  csm->csm.unk1=0;
  MenuTop=NULL;
  LoadItems(MENU_PATH);
  pos=Start;
  csm->gui_id=MsgBox(0,0,&MMenu,LGP_NULL);
  GBS_StartTimerProc(&tmr, SpeedAnim*TMR_SECOND/100, PickNumTmr);
  GBS_StartTimerProc(&tmrcrc, SpeedAnimCirc*TMR_SECOND/100, CircleTmr);
}

void maincsm_onclose(CSM_RAM *csm)
{
  GBS_DelTimer(&tmr);
  GBS_DelTimer(&tmrcrc);
  FreeItemsList();
  if (my_csm_id)
    CloseCSM(my_csm_id);
  my_csm_id=0;
}

extern const int MSG_RECONFIGURE_MENU;

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
        char config_name[128];
        if (MenuTop)
        sprintf(config_name, "%s.bcfg", MenuTop->item);
        else
        sprintf(config_name, "%s.bcfg",MENU_PATH);
        
     if (strcmp_nocase(config_name,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"Menu config updated!");
        LockSched();
        if (MenuTop)
        LoadItems(MenuTop->item);
        else
        LoadItems(MENU_PATH);
        UnlockSched();
    }
  }
    if (msg->msg==MSG_RECONFIGURE_MENU)
  {
      ShowMSG(1,(int)"Menu updated!");
        LockSched();
        if (MenuTop)
        LoadItems(MenuTop->item);
        else
        LoadItems(MENU_PATH);
        UnlockSched();
    
  }
  if (msg->msg==MSG_CSM_DESTROYED)
  {
    RefreshGUI();
  }
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    //if (csm->show_csm==-1) RefreshGUI();
  }
  if (msg->msg==MSG_INCOMMING_CALL)
  {
    csm->csm.state=-3;
  }
  return(1);
}

extern const int minus11;
unsigned short maincsm_name_menu[140];

const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MENUCSM =
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
    maincsm_name_menu,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};

static void UpdateCSMMenu(void)
{
  WSHDR *ws=AllocWS(256);
  ascii2ws(ws,CSMText);
  wsprintf((WSHDR *)(&MENUCSM.maincsm_name),"%w",ws);
  FreeWS(ws);
}

void ShowMenu(void)
{
  unsigned int adr;

  //Нахожу функцию MsgBox
  #pragma swi_number=0x8050
  __swi __arm unsigned int MsgBoxOkCancel_adr();
  adr=MsgBoxOkCancel_adr()+7; //Тут лежит инструкция "B MsgBox"
  MsgBox=(int(*)(int,int,MSG_BOX*,int))(adr+(short)((*(short*)adr|0xF800))*2+5);
  
  MAIN_CSM main_csm;
  if (my_csm_id)
    CloseCSM(my_csm_id);
  UpdateCSMMenu();
  my_csm_id=CreateCSM(&MENUCSM.maincsm,&main_csm,2);
}
