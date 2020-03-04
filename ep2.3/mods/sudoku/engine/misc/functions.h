

extern char *getSNbySDK();
extern char getLEVbySDK();

extern char t[];

extern int rcell[9][9];


#define TIMER_STOP -13

//colors
char alpha[4]={0,0,0,0};
char ablack[4]={0,0,0,30};
char hblack[4]={0,0,0,50};

char black[4]={0,0,0,100};       
char white[4]={255,255,255,100};

char blue[4]={0,0,255,100};
char dark_blue[4]={0,0,160,100};
char red[4]={255,0,0,100};
char dark_red[4]={160,0,0,100};
char green[4]={0,255,0,100};
char half_dark_green[4]={0,200,0,100};
char dark_green[4]={0,160,0,100};

char grey[4]={100,100,100,100};

char violet[4]={0xB4,0x75,0xFF,0x64};
char dark_violet[4]={0x93,0x0E,0xFF,0x64};



int char8to16(int c)
{
  if (c<168) return (c);
  c-=168;
  if (!c)  c=1;
  else if (c<24)
  {
    if (c==2) c=4;
    else if (c==10) c=6;
    else if (c==11) c=0x56;
    else if (c==16) c=0x51;
    else if (c==18) c=0x54;
    else if (c==23) c=0x57;
    else return (c);
  }
  else if (c>87) return (c);
    else c-=8;
  c+=0x400;
  return (c);
}

void ascii2ws(WSHDR *ws, const char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    wsAppendChar(ws,char8to16(c));
  }
}

DrwImg(IMGHDR *img, int x, int y, char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}

void DrawLastScreen()
{
  unsigned int len=SCRH*SCRW*2;
  static int ss=0;
  
  char color[4]={0,0,0,0};
  char *buffer=malloc(len);
  
  if (ss==0) {memcpy(buffer, RamScreenBuffer(), SCRH*SCRW*2);ss=1;}
  IMGHDR img = {SCRW,SCRH,0x8,(char *)buffer};
  
  DrwImg((IMGHDR *)&img, 0, 0, color, color);
  
  mfree(buffer);
}

void DrawSoftHeaderLogo(char skr[],char head[],char txt[])
{
  
  int font = bcfg_mmenu_sk_font;
  int hfont = bcfg_mmenu_head_font;
  
   int w2,wh,hf,hh;
 
   WSHDR *skr_ws=AllocWS(32);
   WSHDR *head_ws=AllocWS(32);
   WSHDR *txt_ws=AllocWS(256);
   
   wsprintf(skr_ws,t,skr);
   wsprintf(head_ws,t,head);
   ascii2ws(txt_ws,txt);
   
  
   w2 = Get_WS_width(skr_ws, font);
   wh = Get_WS_width(head_ws, hfont);
   hf = GetFontYSIZE(font);
   hh = GetFontYSIZE(hfont);
   
   DrawLastScreen();
   DrawRoundedFrame(0, 0,2,SCRH,0,0,0,white,white);
   DrawRoundedFrame(SCRW - 2, 0,SCRW,SCRH,0,0,0,white,white);
   DrawRoundedFrame(0, 0,SCRW,hh+2,0,0,0,white,white);
   DrawRoundedFrame(0, SCRH - ( hf + 4),SCRW,SCRH,0,0,0,white,white);
   
   if (head>0)                            //Header
    {
     for (int i=0; i<(hh+2); i++)                                
     {                                                            
       int a=100 - (i*100)/(hh+2);
       char colh[4]={0,0,0,a};
       DrawRoundedFrame(0,i,SCRW-1,i,0,0,0,colh,alpha);
     }
    } 
 
      
   if (skr>0)
    {
      for (int i=0; i<=(4*SCRW/10); i++)      //Right                      
      {                                                            
       int a=(i*100)/(4*SCRW/10);
       char col2[4]={0,0,0,a};
       DrawRoundedFrame(i + (6*SCRW)/10,SCRH-hf-2,i + (6*SCRW)/10, SCRH - 2,0,0,0,col2,alpha);
      }
    }
    
     if (skr>0)   DrawString(skr_ws, SCRW - w2 - 2, SCRH - hf - 2,SCRW, SCRH - 2, font,0,bcfg_mmenu_sk_col,alpha);
     if (head>0)  DrawString(head_ws, 0, 0, 2 + wh, 2 + hh, hfont,0,bcfg_mmenu_head_col,alpha);
     
     char halfwhite[4]={255,255,255,80};
     DrawRoundedFrame(2, hh + 2, SCRW - 3, SCRH - ( hf + 4),0,0,0,black,halfwhite);
     
     DrawString(txt_ws, 5, hh+2, SCRW - 5, SCRH - ( hf + 4), bcfg_help_font,0,bcfg_help_col,alpha);   
    
    FreeWS(txt_ws);
    FreeWS(skr_ws);
    FreeWS(head_ws);
}

void DrawLastScreenPart(int font,int xadj)
{
  int hf = GetFontYSIZE(font) + xadj;
  unsigned int len=SCRW*hf*2;
  
  char color[4]={0,0,0,0};
  char *buffer=malloc(len);
  
  memcpy(buffer, RamScreenBuffer(), hf*SCRW*2);
  const IMGHDR img = {SCRW,hf,0x8,(char *)buffer};
  
  DrwImg((IMGHDR *)&img, 0, 0, color, color);
  
  mfree(buffer);
}


void DrawSoftHeader(char skl[],char skr[],char head[])
{
   int font = bcfg_mmenu_sk_font;
   int hfont = bcfg_mmenu_head_font;
  
   int w1,w2,wh,hf,hh;
  
   WSHDR *skl_ws=AllocWS(32);
   WSHDR *skr_ws=AllocWS(32);
   WSHDR *head_ws=AllocWS(32);
   
   wsprintf(skl_ws,t,skl);
   wsprintf(skr_ws,t,skr);
   wsprintf(head_ws,t,head);
  
   w1 = Get_WS_width(skl_ws, font);
   w2 = Get_WS_width(skr_ws, font);
   wh = Get_WS_width(head_ws, hfont);
   hf = GetFontYSIZE(font);
   hh = GetFontYSIZE(hfont);
   
   if (head>0)                            //Header
    {
     for (int i=0; i<(hh+2); i++)                                
     {                                                            
       int a=100 - (i*100)/(hh+2);
       char colh[4]={0,0,0,a};
       DrawRoundedFrame(0,i,SCRW-1,i,0,0,0,colh,alpha);
     }
    } 
 
   if (skl>0)
    {
      for (int i=0; i<(4*SCRW/10); i++)      //Left                          
      {                                                            
       int a=100-(i*100)/(4*SCRW/10);
       char col1[4]={0,0,0,a};
       DrawRoundedFrame(i,SCRH-hf-2,i, SCRH - 2,0,0,0,col1,alpha);
      }
    }
      
   if (skr>0)
    {
      for (int i=0; i<=(4*SCRW/10); i++)      //Right                      
      {                                                            
       int a=(i*100)/(4*SCRW/10);
       char col2[4]={0,0,0,a};
       DrawRoundedFrame(i + (6*SCRW)/10,SCRH-hf-2,i + (6*SCRW)/10, SCRH - 2,0,0,0,col2,alpha);
      }
    }
    
     if (skl>0)   DrawString(skl_ws, 2, SCRH - hf - 2, w1+5, SCRH - 2, font,0,bcfg_mmenu_sk_col,alpha);
     if (skr>0)   DrawString(skr_ws, SCRW - w2 - 2, SCRH - hf - 2,SCRW, SCRH - 2, font,0,bcfg_mmenu_sk_col,alpha);
     if (head>0)  DrawString(head_ws, 0, 0, 2 + wh, 2 + hh, hfont,0,bcfg_mmenu_head_col,alpha);
    
    FreeWS(skl_ws);
    FreeWS(skr_ws);
    FreeWS(head_ws);
}

void DrawSudokuLevel(char lev1[],char sud1[], int font)
{
   int hf,notleft = 0;
   
   char sname[64];  
   char lev[16];
   
   
   if (getLEVbySDK(filepath)==0x4C) sprintf(lev,"Лёгкий");  else notleft++;
   if (getLEVbySDK(filepath)==0x4D) sprintf(lev,"Средний"); else notleft++;
   if (getLEVbySDK(filepath)==0x48) sprintf(lev,"Тяжёлый"); else notleft++;
   
  
   WSHDR *sud_ws=AllocWS(64);
   WSHDR *lev_ws=AllocWS(64);
   
   if (filepath[1]==':') wsprintf(sud_ws,"%t:\n%t",sud1,getSNbySDK(filepath,sname)); else wsprintf(sud_ws,"%t:\n%t",sud1,"Неизвестно");
   if (notleft<3) wsprintf(lev_ws,"%t:\n%t",lev1,lev); else wsprintf(lev_ws,"%t:\n%t",lev1,"Неизвестно");
  
   hf = GetFontYSIZE(font)+2;
   
   
   DrawString(lev_ws, 2, hf,SCRW-3, SCRH, font,TEXT_OUTLINE ,red,black);
   DrawString(sud_ws, 2, hf*3, SCRW-3, SCRH, font,TEXT_OUTLINE ,red,black);

    
   FreeWS(sud_ws);
   FreeWS(lev_ws);

}


void DrawRightSoft(char str[])
{
   int font = bcfg_mmenu_sk_font;
  
   int w,h;
  
   WSHDR *str_ws=AllocWS(32);  
   wsprintf(str_ws,t,str);

   w = Get_WS_width(str_ws, font);
   h = GetFontYSIZE(font);
   
   for (int i=0; i<=(6*SCRW/10); i++)                  
    {                                                            
      int a=(i*100)/(6*SCRW/10);
      char col[4]={0,0,0,a};
      DrawRoundedFrame(i + (4*SCRW)/10,SCRH-h-2,i + (4*SCRW)/10, SCRH - 2,0,0,0,col,alpha);
    }
   DrawString(str_ws, SCRW - w - 2, SCRH - h - 2, SCRW, SCRH - 2, font,0,bcfg_mmenu_sk_col,alpha);
   FreeWS(str_ws);
}

void DrawAboutItem(char s1[], char s2[], int n)
{
 WSHDR *ws1 = AllocWS(32);
 WSHDR *ws2 = AllocWS(32);
 
 n=n*2-1;
 
 wsprintf(ws1,"%t:",s1);
 wsprintf(ws2,t,s2);
 
 int h = GetFontYSIZE(FONT_SMALL_BOLD) + 3;
 
 DrawString(ws1, 2, h*n, SCRW, h*n+h, FONT_SMALL_BOLD,TEXT_OUTLINE ,dark_red,black);
 DrawString(ws2, 2, h*(n+1), SCRW, h*(n+1)+h, FONT_SMALL_BOLD,TEXT_OUTLINE ,dark_green,black);
}

void DrawScrollItem(char *str, int x, int y, int *xdisp, int *sw, int font, char *col) 
{  
    int dw=0;
    WSHDR *sws=AllocWS(128);
    wsprintf(sws,t,str);
    if (Get_WS_width(sws, font)>((9*SCRW)/10)) dw = Get_WS_width(sws, font) - ((9*SCRW)/10);
    if (dw >= *xdisp && *sw==0) *xdisp++; else *sw = 1;
    if (*sw==1 && *xdisp > 1) *xdisp--; else *sw = 0;
    if (Get_WS_width(sws, font)<=((9*SCRW)/10)) *xdisp = 1;
    DrawScrollString(sws,x,y,(9*SCRW)/10,GetFontYSIZE(font)+2,*xdisp,font,0,col,alpha);
    FreeWS(sws);
}



void startTimer(int *timer,int autostop) // without autostop use -1 is argument
{
  int t = *timer;
  if (t<autostop && t>-2) *timer = *timer + 1; else *timer = TIMER_STOP;
}

void resetTimer(int *timer,int start)
{
 *timer = start;
}

void stopTimer(int *timer)
{
  *timer = TIMER_STOP;
}

int isTimer(int *timer,int start,int end) // Не в секундах!
{
  int t = *timer;
  if(start <= t && end >= t) return 1; else return 0; 
}

int check_sudoku()
{
 int var=0;
 for (int i=1;i<10;i++) for (int k=0;k<9;k++) for (int j=0;j<9;j++) if (rcell[k][j]==i) var++;
 for (int i=1;i<10;i++) for (int k=0;k<9;k++) for (int j=0;j<9;j++) if (rcell[j][k]==i) var++;
 
 for (int i=1;i<10;i++) for (int k=0;k<3;k++) for (int j=0;j<3;j++) if (rcell[k][j]==i) var++;
 for (int i=1;i<10;i++) for (int k=0;k<3;k++) for (int j=3;j<6;j++) if (rcell[k][j]==i) var++;
 for (int i=1;i<10;i++) for (int k=0;k<3;k++) for (int j=6;j<9;j++) if (rcell[k][j]==i) var++;
 
 for (int i=1;i<10;i++) for (int k=3;k<6;k++) for (int j=0;j<3;j++) if (rcell[k][j]==i) var++;
 for (int i=1;i<10;i++) for (int k=3;k<6;k++) for (int j=3;j<6;j++) if (rcell[k][j]==i) var++;
 for (int i=1;i<10;i++) for (int k=3;k<6;k++) for (int j=6;j<9;j++) if (rcell[k][j]==i) var++;
 
 for (int i=1;i<10;i++) for (int k=6;k<9;k++) for (int j=0;j<3;j++) if (rcell[k][j]==i) var++;
 for (int i=1;i<10;i++) for (int k=6;k<9;k++) for (int j=3;j<6;j++) if (rcell[k][j]==i) var++;
 for (int i=1;i<10;i++) for (int k=6;k<9;k++) for (int j=6;j<9;j++) if (rcell[k][j]==i) var++;
 
 if (var==(9*9*3)) return 1; else return 0;

}
