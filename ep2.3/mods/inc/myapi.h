#ifndef  _MYAPI_H_
#define _MYAPI_H_

//defines
#define cchar const char

#define color(x) (x<24)?GetPaletteAdrByColorIndex(x):(char *)(&(x))
#define idlegui_id(icsm) (((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
#define MIN(a,b) (a<b)?a:b
#define MAX(a,b) (a>b)?a:b

//
const char _t[] = "%t";
const char _s[] = "%s";
const char _d[] = "%d";

//colors
const char transparent[] = {0,0,0,0};
const char white[] =  {255,255,255,100};
const char black[] =  {0,0,0,100};
const char red[] =    {255,0,0,100};
const char green[] =  {0,255,0,100};
const char blue[] =   {0,0,255,100};
const char yellow[] = {255,255,0,100};


//structures
 typedef struct{
  char Pen[4];
  char Brush[4];
  ubyte Size;
 }TFont;
 
 

//description
static void RunFile(char *s);
extern int GetWidth(WSHDR* wsh, char *s, int font);
inline int FontH(int font);
int get_file_size(char* fname);

 
//function
inline int FontH(int font)
{
 return GetFontYSIZE(font);
}


#pragma inline
int GetWidth(WSHDR* wsh, char *s, int font)
{
  WSHDR *ws;
  if(wsh->wsbody[0])
  {
    ws = wsh;
  }
  else  
    if(s && !ws->wsbody[0])
    {
	  WSHDR *ws=AllocWS(strlen(s));
	  wsprintf(ws,_t,s);
	}
	else return 0;
 
  unsigned short *body=ws->wsbody;
  int len=body[0],width;
  if(len>0) width=1; else width=0;
  while(len>0)
  {
    LockSched();
    
    width+=GetSymbolWidth(body[len],font);
    len--;
    
    UnlockSched();
  }
  
  if(s && !ws->wsbody[0]) FreeWS(ws);
  return (width+1);
}


#pragma inline//ÔËÐÐELF³ÌÐò
static void RunFile(char *s)
{
  if ((s)&&(strlen(s))){
      WSHDR *ws=AllocWS(128);
      str_2ws(ws,s,strlen(s));
      ExecuteFile(ws, 0,0);
      FreeWS(ws);
  }
}
/*
#pragma inline=forced
static int toupper(int c){ if ((c>='a')&&(c<='z')) c+='A'-'a';  return(c);}

#pragma inline
static int strcmp_nocase(const char *s1,const char *s2)
{
  int i,c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}
#pragma inline
static int cmpstr_nocase(const char *s, const char *d)
{
  int cs;
  int ds;
  do
  {
    cs=*s++;
    if (cs&0x40) cs&=0xDF;
    ds=*d++;
    if (ds&0x40) ds&=0xDF;
    cs-=ds;
    if (cs) break;
  }
  while(ds);
  return(cs);
}
*/


#pragma inline
static void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x =x;
  rc->y =y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
static void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}

#pragma inline
static void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

static char *LoadFile(const char *FileName)
{
  int f;
  char *buf=0;  
  uint err,fsize;  
  if ((f=fopen(FileName,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
  {
   fsize  = get_file_size(FilName);//lseek(f,0,S_END,&err,&err);
   if (fsize<=0)
   {
     fclose(f,&err);     
   }
   else
   {         
    lseek(f,0,S_SET,&err,&err);
    buf=malloc(fsize+1);
    fread(f,buf,fsize,&err);    
    fclose(f,&err); 
    buf[fsize]=0;   
   }
  }
  return(buf);
}

#endif
