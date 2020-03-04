#include "../inc/swilib.h"
#include "../inc/pnglist.h"
#include "naticq_ipc.h"
#include "smiles.h"
#include "naticq.h"
#include "strings.h"
#include "readimg.h"

S_SMILES *s_top=0;

DYNPNGICONLIST *SmilesImgList;
DYNPNGICONLIST *XStatusesImgList;

int *XStatusesIconArray;

IMGHDR *XImg[35];
IMGHDR *SImg[TOTAL_ICONS+1];

volatile int total_smiles;
volatile int total_xstatuses;
volatile int xstatuses_load;
volatile int pictures_max;
volatile int pictures_loaded = 0;

extern const char SMILE_FILE[];
extern const char SMILE_PATH[];
extern const char XSTATUSES_PATH[];
extern const char ipc_my_name[32];
extern  int S_ICONS[];

static IPC_REQ gipc;
static char *p_buf;
static char *s_buf;
static S_SMILES *s_bot;
static int n_pic;


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
    if (sl->uni_smile == wchar) return (sl);
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

  n_pic=FIRST_UCS2_BITMAP;
  if (GetFileStats(SMILE_FILE,&stat,&err)==-1)
    return;

  if ((fsize=stat.size)<=0)
    return;

  if ((f=fopen(SMILE_FILE,A_ReadOnly+A_BIN,P_READ,&err))==-1)
    return;

  buf=s_buf=p_buf=malloc(fsize+1);
  buf[fread(f,buf,fsize,&err)]=0;
  fclose(f,&err);
  //f=fopen("4:\\smiles.cfg",A_ReadWrite+A_BIN+A_Create+A_Append,P_READ+P_WRITE,&err);
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
      
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SMILE_PROCESSED,&gipc);
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

  if ((f=fopen(SMILE_FILE,A_ReadOnly+A_BIN,P_READ,&err))==-1)
    return;

  buf=p_buf=malloc(fsize+1);
  buf[fread(f,buf,fsize,&err)]=0;
  fclose(f,&err);
  
  f = pictures_max;
  for(buf=p_buf;*buf; buf++)
    if(*buf == ':')
    {
      buf++;
      while(*buf && *buf != 0x0D) buf++;
      pictures_max++;
    }
/*  if(f == pictures_max)                 //А вдруг у нас кривые переводы строк?
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
      gipc.name_to=ipc_my_name;
      gipc.name_from=ipc_my_name;
      gipc.data=0;
      GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_SMILE_PROCESSED,&gipc);
      p_buf=buf;
      return;
    }
    p=strchr(buf,':');
    if (!p) break;
    zeromem(fn,128);
    strcpy(fn,SMILE_PATH);
    if (fn[strlen(fn)-1]!='\\') strcat(fn,"\\");
    c=p-buf;
    if (c>(127-strlen(fn))) break;
    strncpy(fn+strlen(fn),buf,c);
//    snprintf(logmsg,255,"Process file %s...",fn);
//    REDRAW();
    buf=p;
    dp=malloc(sizeof(DYNPNGICONLIST));
    zeromem(dp,sizeof(DYNPNGICONLIST));
    dp->icon=GetPicNByUnicodeSymbol(n_pic);
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
    si->uni_smile=n_pic;
    if (s_bot)
    {
      //Не первый
      s_bot->next=si;
      s_bot=si;
    }
    else
    {
      //Первый
      s_top=si;
      s_bot=si;
    }
    n_pic++;
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
  //fclose(f,&err);
  total_smiles=0;
  p_buf=NULL;
  mfree(s_buf);
  s_buf=NULL;
  REDRAW();
}

void FreeXStatusesImg(void)
{
#ifdef GUI_INTERFACE
  void FreeXIconList();
  FreeXIconList();
#else
  DYNPNGICONLIST *d;
  DYNPNGICONLIST *nd;
  LockSched();
  total_xstatuses=0;
  d=XStatusesImgList;
  XStatusesImgList=0;
  mfree(XStatusesIconArray);
  XStatusesIconArray=NULL;
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
#endif
}
int xstatuses_max=0;
void InitXStatusesImg(void)
{
  char fn[128];
  FSTATS stat;
  unsigned err;
  
  FreeXStatusesImg();
  total_xstatuses=0;
  *(XStatusesIconArray=malloc(sizeof(int)))=S_ICONS[IS_NULLICON];
  xstatuses_load=1;
  xstatuses_max=0;
  n_pic=FIRST_UCS2_BITMAP;
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
  
  pictures_max = 0;
  do
  {
    strcpy(fn,XSTATUSES_PATH);
    sprintf(fn+strlen(fn),"\\%d.png",pictures_max++);
    xstatuses_max++;
    if (GetFileStats(fn,&stat,&err)==-1) break;
  }  
  while (stat.size>0);
  pictures_max--;
  CheckSmiles();

#ifndef GUI_INTERFACE  
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XSTATUSIMG_PROCESSED,&gipc);
#endif
}


#ifdef GUI_INTERFACE  
/*
typedef struct
{
 void *next;
 IMGHDR *img;
}ICONS;

ICONS *xiconstop;


void AddXIcon(IMGHDR *img)
{
 ICONS *icon=malloc(sizeof(ICONS));
 icon->next=0;
 icon->img=img;  
 
 if(!xiconstop)
 {
   xiconstop = icon; 
 }
 else
 {
//    xiconstop->next = (ICONS*)icon;
   ICONS *ico=(ICONS *)xiconstop;
   while(ico->next)
       ico=ico->next;
   ico->next=icon;
 } 
}*/

int t_xstatus = 0;

void FreeXIconList()
{  
  int i=0;
  while(i < t_xstatus)
  {
    if(strlen(XImg[i]->bitmap))
    {
      mfree(XImg[i]->bitmap);
      mfree(XImg[i]);
    }
    i++;
  }
}


int CountXIcon()
{
  return t_xstatus;
}



int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}



IMGHDR *GetXIconByItem(int curitem)
{
  if(XImg[curitem]) return XImg[curitem];
  return 0;
}
#endif

void ProcessNextXStatImg(void)
{
 // char fn[128];
 // unsigned int err;
  //FSTATS stat;
#ifdef GUI_INTERFACE
//  IMGHDR *img;
#else
  DYNPNGICONLIST *dp;
  int i;
#endif  
  
  //strcpy(fn,XSTATUSES_PATH);
  //sprintf(fn+strlen(fn),"\\%d.png",total_xstatuses);
 /* sprintf(fn,"%s\\%d.png",XSTATUSES_PATH,total_xstatuses);
  if (GetFileStats(fn,&stat,&err)!=-1)
  {
    if (stat.size>0)
    {
#ifdef GUI_INTERFACE
    // XImg[t_xstatus++] = CreateIMGHDRFromPngFile(fn,0);
    // AddXIcon(img);
#else
      dp=malloc(sizeof(DYNPNGICONLIST));
      zeromem(dp,sizeof(DYNPNGICONLIST));
      dp->icon=i=GetPicNByUnicodeSymbol(n_pic);
      dp->img=CreateIMGHDRFromPngFile(fn,0);
      LockSched();
      if (XStatusesImgList)
      {
	dp->next=XStatusesImgList;
      }
      XStatusesImgList=dp;
      UnlockSched();
#endif
      total_xstatuses++;
      pictures_loaded++;
#ifndef GUI_INTERFACE
      XStatusesIconArray=realloc(XStatusesIconArray,(total_xstatuses*sizeof(int)));
      *(XStatusesIconArray+(total_xstatuses-1))=i;
#endif
      n_pic++;
      gipc.name_to=ipc_my_name;
      gipc.name_from=ipc_my_name;
      gipc.data=0;
      GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XSTATUSIMG_PROCESSED,&gipc);
      return;
    }
  }  */
  xstatuses_load=0;
  SUBPROC((void *)InitSmiles);
}


#ifdef GUI_INTERFACE


extern const char *icons_names[];
extern const char ICON_PATH[];

/*
ICONS *iconstop;


void AddIcon(IMGHDR *img)
{
   ICONS *icon=malloc(sizeof(ICONS));
   icon->next=0;
   icon->img=img;  
  
   if(!iconstop)
   {
     iconstop=icon; 
   }
   else
   {
//     iconstop->next=icon;
      ICONS *ico=(ICONS *)iconstop;
      while(ico->next) ico = ico->next;
      ico->next=icon;
   }
   
}
*/

void FreeIconList()
{  
  int i=0;
  while(i < TOTAL_ICONS)
  {
    if(strlen(SImg[i]->bitmap))
    {
      mfree(SImg[i]->bitmap);
      mfree(SImg[i]);
    }
    
    i++;
  }
  
  /*LockSched();
  ICONS *icon=(ICONS *)iconstop;
  iconstop=0;
  UnlockSched();
  while(icon)
  {
    ICONS *icon_prev = icon;
    icon = icon->next;
    if(icon_prev) mfree(icon_prev);
  }*/
}
/*
int CountIcon()
{
  int i=0;
  ICONS *ico=(ICONS *)iconstop;
  while(ico) { i++; ico=ico->next; }
  return i;
}

*/
IMGHDR *GetIconByItem(int curitem)
{
//  if(curitem > CountIcon()) return 0;
  if(SImg[curitem]) return SImg[curitem];
  /*
  ICONS *icon;
  icon=(ICONS *)iconstop;
  int i=0;

  while(icon)
  {
    if(i==curitem) return (icon->img);
    i++;
    icon=icon->next;  
  }*/
  return 0;
}

enum status
{
  offline,
  invisible,
  away,
  na,
  occupied,
  dnd,
  depression,
  evil,
  home,
  lunch,
  work,
  online,
  ffc,
  msg,
  null,
  addcontact,
  setting,
  unknown,
  group_on,
  group_off,
  hrad,
  ping,
  check,
  uncheck,
  vis1,
  vis2,
  vis3,
  vis4,
  vis5
};


int drawimg2img(IMGHDR * onse, IMGHDR * timage2, int x, int y, int xRect, int yRect,int xxRect, int yyRect)
{
  if (!onse) return 1;
  if (yRect<0 || yRect>=onse->h) yRect = 0;
  if (xRect<0 || xRect>=onse->w) xRect = 0;  // ??????? ? ???????? ?? ?????? ?????????? ?? ???:)
  if (yyRect==0 || (yRect+yyRect >= onse->h)) yyRect = onse->h-yRect; //e??? ?????? ??????? ??????? ??? ??????? ?? ????????, ?? ????? ?? ????? ???????? 
  if (xxRect==0 || (xRect+xxRect >= onse->w)) xxRect = onse->w-xRect; //e??? ?????? ??????? ??????? ??? ??????? ?? ????????, ?? ????? ?? ????? ???????? 
  int *scr = (int*)((char*)timage2->bitmap);
  int *pic = (int*)((char*)onse->bitmap);
  int i,j;

  for (i = xRect; i < xRect+xxRect; i++)
  {
    for(j = yRect; j < yRect+yyRect; j++)
    {
      //i ? ????????? ??????? ??? ??????? (?? xRect ?? xRect+xxRect)                                                                         //j ? ????????? ??????? ??? ??????? (?? yRect ?? yRect+yyRect)
      if (((y+j-yRect)<timage2->h) && ((x+i-xRect)<timage2->w) && ((y+j-yRect)>=0) && ((x+i-xRect)>=0) 
          &&(i>=xRect)&&(j>=yRect)&&(i<xRect+xxRect)&&(j<yRect+yyRect))
       {   
         scr[(y+j-yRect)*timage2->w+i+x-xRect] = pic[(j)*onse->w+i];
       }
    }
  }
 return 0;
}


typedef struct
{
  int index;
  int w;
  int h;
}POS;


POS status[]=
{
  {IS_OFFLINE,    15,14},
  {IS_INVISIBLE,  14,14},
  {IS_AWAY,       15,14},
  {IS_NA,         15,14},
  {IS_OCCUPIED,   14,14},
  {IS_DND,        14,14},
  {IS_DEPRESSION, 15,14},
  {IS_EVIL,       15,14},
  {IS_HOME,       15,14},
  {IS_LUNCH,      16,14},
  {IS_WORK,       15,14},
  {IS_ONLINE,     14,14},
  {IS_FFC,        15,14},
  {IS_MSG,        14,14},
  {IS_UNKNOWN,    15,14},
  {IS_GROUP,      14,14},
  {IS_GROUPOFF,   14,14},
  {IS_NULLICON,   13,14},
  {ICON_ADDCONTACT,    14,14},
  {ICON_HEAD,       15,14},
  {ICON_LOGO,       0, 0},
  {ICON_PING,       15,14},
  {ICON_SETTINGS,   15,14},
  {ICON_ALL_CAN_SEE,          16,16},
  {ICON_VISLIST_CAN_SEE,      16,16},
  {ICON_INVISLIST_CANNOT_SEE, 16,16},
  {ICON_CONTACTLIST_CAN_SEE,  16,16},
  {ICON_NOBODY_CAN_SEE,       16,16},
  {ICON_CHECK,      14,14},
  {ICON_UNCHECK,    14,14}
//  TOTAL_ICONS
};

#include "Compressing_IMGHDR.c"

void LoadImages()
{
 /* char s[32];
  char s2[32];

  int i=0;
  int i2=0;
  IMGHDR *icon;
  */
 // while( i < TOTAL_ICONS )
 //   if(get_file_size("4:\\status.png"/*MakeGlobalString(ICON_PATH,'\\',"status.png")*/))
    {
      IMGHDR *img = CreateIMGHDRFromPngFile(MakeGlobalString(ICON_PATH,'\\',"status.png"),0);
//      int size = 0;
      int start = 0;
      int i2 = 0;
//      while(i<TOTAL_ICONS)
      for(int i = 0;i<TOTAL_ICONS;i++)
      {
        if(i <= IS_FFC)
        {
          SImg[i] = CreateImgr(status[i].w,status[i].h);
          drawimg2img(img,SImg[i],0,0,start,0,start + status[i].w,status[i].h);
          start += status[i].w;
        }
        else
        {
          if(!i2)
          {
            mfree(img);
            img = CreateIMGHDRFromPngFile(MakeGlobalString(ICON_PATH,'\\',"image.png"),0);
            i2 = 1;
            start = 0;
          }
          if(i == ICON_LOGO) i++;          
          SImg[i] = CreateImgr(status[i].w, status[i].h);
          drawimg2img(img,SImg[i],0,0,start,0,start + status[i].w,status[i].h);
          start += status[i].w + (i >= ICON_ALL_CAN_SEE && i <= ICON_NOBODY_CAN_SEE?1:0);
          if(start > img->w) break;
        }
       }
      
      SImg[ICON_LOGO] = CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH,'\\',icons_names[ICON_LOGO]),0);
      SImg[ICON_BG] = CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH,'\\',icons_names[ICON_BG]),0);
      PressImg(SImg[ICON_LOGO],0);
      PressImg(SImg[ICON_BG],0);
      mfree(img);
      img = CreateIMGHDRFromPngFile(MakeGlobalString(ICON_PATH,'\\',"xst.png"),0);
      start = 0;
      for(int i=0;i<35;i++)
       {
          XImg[i] = CreateImgr(16,16);   
          drawimg2img(img,XImg[i],0,0,start,0,start+16,16);
          start+=17;
          t_xstatus++;
          total_xstatuses++;
          pictures_loaded++;
          if(start > img->w) break;
       }
    /*strcpy(s2,icons_names[i]);
    s2[strlen(s2)-4]=0;//отрезаем '.png'
    snprintf(s,32,"%s_small.png",s2);

    if( get_file_size(MakeGlobalString(ICON_PATH,'\\',s)) )
      SImg[i] = CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH,'\\',s), 0 );          
   else
      SImg[i] = CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH,'\\',icons_names[i]), 0 );  
   i++;*/
  }
}



void ProcessNextImg(void)
{
//  char s[32];
  //char s2[32];
 // int i=pictures_loaded;
  /*
  strcpy(s2,icons_names[i]);
  s2[strlen(s2)-4]=0;//отрезаем '.png'
  snprintf(s,32,"%s_small.png",s2);

  IMGHDR *icon;
  
  if( get_file_size(MakeGlobalString(ICON_PATH,'\\',s)) )
    icon = CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH,'\\',s), 0 );   	
  else
    icon = CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH,'\\',icons_names[i]), 0 );

  AddIcon(icon);
  */
/*  if(pictures_loaded < TOTAL_ICONS)
  {
    pictures_loaded++;
    gipc.name_to=ipc_my_name;
    gipc.name_from=ipc_my_name;
    gipc.data=0;
    GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_IMG_PROCESSED,&gipc);
    return;
  }*/
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XSTATUSIMG_PROCESSED,&gipc);
 //  SUBPROC((void *)InitSmiles);
}
#endif
