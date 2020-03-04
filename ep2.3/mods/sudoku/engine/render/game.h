#include "..\inc\swilib.h"



//file pathes
extern char dig[30][256];
extern char bgg[];

//Location vars
int x[9],y[9];
int gcell[9][9];
int rcell[9][9];

//Extern vars
extern int xc;
extern int yc;
extern char t[];
extern char sudokuname[32];
extern char level;

extern int winw;
extern int gmenu;
extern int gamew;
extern void clearSudoku();

int prev_press=0;

//Timer
int tmr=0;
int prev=0;
int podskazka=0;
int wintime;

char vremya[16];
char timegame[6];


void DrawGrid()
{
   static int one=0;
  
   int csx,csy;
   int w, h, vl1, vl2, gl1, gl2;
    w = 9*GetImgWidth((int)dig[0])+7;
    h = 9*GetImgHeight((int)dig[0])+7;
    
    if (bcfg_pole_x_min==0)  csx = ((SCRW/2)-(w/2)) + bcfg_pole_x; else  csx = ((SCRW/2)-(w/2)) - bcfg_pole_x;
    if (bcfg_pole_y_min==0)  csy = ((SCRH/2)-(h/2)) + bcfg_pole_y; else  csy = ((SCRH/2)-(h/2)) - bcfg_pole_y;
    
    
    vl1 = 3*GetImgHeight((int)dig[0])+2;
    vl2 = 6*GetImgHeight((int)dig[0])+4;
    
    gl1 = 3*GetImgWidth((int)dig[0])+2;
    gl2 = 6*GetImgWidth((int)dig[0])+4;
    
    if (one==0)
     {
      for (int i=0;i<3;i++){
       x[i]=(csx+2)+(i*GetImgWidth((int)dig[0]));
       y[i]=(csy+2)+(i*GetImgHeight((int)dig[0]));}
  
     for (int i=3;i<6;i++){
       x[i]=(csx+4)+(i*GetImgWidth((int)dig[0]));
       y[i]=(csy+4)+(i*GetImgHeight((int)dig[0]));}
  
     for (int i=6;i<9;i++){
       x[i]=(csx+6)+(i*GetImgWidth((int)dig[0]));
       y[i]=(csy+6)+(i*GetImgHeight((int)dig[0]));}
     one=1;
    }  
    
    DrawLine(csx+vl1,csy,csx+vl1,csy+h,0,bcfg_gamew_grid_col);
    DrawLine(csx+vl1+1,csy,csx+vl1+1,csy+h,0,bcfg_gamew_grid_col);
    DrawLine(csx+vl2,csy,csx+vl2,csy+h,0,bcfg_gamew_grid_col);
    DrawLine(csx+vl2+1,csy,csx+vl2+1,csy+h,0,bcfg_gamew_grid_col);
    
    DrawLine(csx,csy+gl1,csx+w,csy+gl1,0,bcfg_gamew_grid_col);
    DrawLine(csx,csy+gl1+1,csx+w,csy+gl1+1,0,bcfg_gamew_grid_col);
    DrawLine(csx,csy+gl2,csx+w,csy+gl2,0,bcfg_gamew_grid_col);
    DrawLine(csx,csy+gl2+1,csx+w,csy+gl2+1,0,bcfg_gamew_grid_col);
    
    DrawRoundedFrame(csx,csy,csx + w,csy + h,0,0,0,bcfg_gamew_kontur_col,alpha);
    DrawRoundedFrame(csx+1,csy+1,csx + w - 1,csy + h - 1,0,0,0,bcfg_gamew_kontur_col,alpha);
}


void DrawSoftHeaderGW(char dif, char skr[], int font)
{
   int w1,w2,hf,notleft = 0;
   
   char skl[16];
   
   if (dif==0x4C) sprintf(skl,"Лёгкий");  else notleft++;
   if (dif==0x4D) sprintf(skl,"Средний"); else notleft++;
   if (dif==0x48) sprintf(skl,"Тяжёлый"); else notleft++;
   
  
   WSHDR *skl_ws=AllocWS(32);
   WSHDR *skr_ws=AllocWS(32);
   
   wsprintf(skl_ws,t,skl);
   wsprintf(skr_ws,t,skr);
  
   w1 = Get_WS_width(skl_ws, font);
   w2 = Get_WS_width(skr_ws, font);
   hf = GetFontYSIZE(font);
   
   
     for (int i=0; i<(hf+2); i++)                                
     {                                                            
       int a=100 - (i*100)/(hf+2);
       char colh[4]={0,0,0,a};
       DrawRoundedFrame(0,i,SCRW-1,i,0,0,0,colh,alpha);
     }
 
   if (notleft!=3)
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
    
     if (notleft!=3)   DrawString(skl_ws, 2, SCRH - hf - 2, w1+5, SCRH - 2, font,0,red,alpha);
     if (skr>0)   DrawString(skr_ws, SCRW - w2 - 2, SCRH - hf - 2,SCRW, SCRH - 2, font,0,red,alpha);
    
    FreeWS(skl_ws);
    FreeWS(skr_ws);

}


void timeini(int *tmr)
{  
   static int temp = 0;
   TDate date; TTime time; 
   GetDateTime(&date,&time);
   
   if (temp!=time.sec) *tmr = *tmr + 1;  
   
   temp = time.sec;
}

char *timeBySeconds(char str[], int sec)
{
 int h,m,s;
 h = sec/3600;
 m = (sec - h*3600)/60;
 s = sec - h*3600 - m*60;
 sprintf(str,"%02d:%02d:%02d",h,m,s);
 return str;
}

void checker()
 {
   static int a,b;
   int k=0;
   for(int i=0;i<9;i++) for(int j=0;j<9;j++) if (gcell[i][j]<10) k=1;
   if (k!=1) for(int i=0;i<9;i++) for(int j=0;j<9;j++)
     {
       a = gcell[i][j];
       b = rcell[i][j];
       if (a>10 && a<20) b+=10;
       if (a>20 && a<30) b+=20;
       
       if (a!=b) k=2;
      
     }
   if (k!=2 && k!=1) k=3;

   if (k==1)  ShowMSG(1,(int)"Не заполнено");
   if (k==2)  ShowMSG(1,(int)"Ошибка!"); 
   if (k==3)  {winw=1;gamew=0;gmenu=0;clearSudoku();wintime=tmr;tmr=0;}
 }

void GameRender()
{
  
   int hf = GetFontYSIZE(bcfg_gamew_head_font);


    //Background
    DrawImg(0, 0, (int)bgg);
    
    //Сетка
    DrawGrid();
    
    //Матрица Цифр
    for (int i=0;i<9;i++) for (int k=0;k<9;k++) DrawImg(x[i], y[k], (int)dig[gcell[k][i]]);
    //Курсор
    DrawRoundedFrame(x[xc],y[yc],x[xc] + GetImgWidth((int)dig[0]) - 1,y[yc] + GetImgHeight((int)dig[0]) - 1,0,0,0,bcfg_gamew_cursor_col,ablack);
    
    //Фон заголовка и софт-кнопки
    DrawSoftHeaderGW(level,"Меню", bcfg_gamew_head_font);
      
    
     // - Время
    WSHDR *t=AllocWS(128);
    timeini(&tmr);
    sprintf(timegame,"%05d",tmr);
    wsprintf(t,timeBySeconds(vremya, tmr));
    int wf = Get_WS_width(t, bcfg_gamew_head_font);   
    DrawString(t,SCRW-wf-2, 2,SCRW, hf +2, bcfg_gamew_head_font,0,bcfg_gamew_timer_col,alpha);
    FreeWS(t);
    
     // - Названиe
    int dw=0;
    static int dx=0,dp=0;
    
    WSHDR *nl=AllocWS(128);
    wsprintf(nl,"%t",sudokuname);
    int wfn = Get_WS_width(nl, bcfg_gamew_head_font);
    if (wfn>((6*SCRW)/10)) dw = wfn - ((6*SCRW)/10);
    if (dw>=dx && dp==0) dx++; else dp = 1;
    if (dp==1 && dx>1) dx--; else dp = 0;
    if (wfn<=((6*SCRW)/10)) dx=1;
    DrawScrollString(nl,2,2,(6*SCRW)/10,hf+2,dx,bcfg_gamew_head_font,0,bcfg_gamew_head_col,alpha);
    FreeWS(nl);

    
    
    
    //Подсказка 
    if (prev_press==1){
     startTimer(&prev,20);
     DrawRoundedFrame(0,0,SCRW,SCRH,0,0,0,hblack,hblack);
     DrawGrid();
     for (int i=0;i<9;i++) for (int k=0;k<9;k++) DrawImg(x[i], y[k], (int)dig[20+rcell[k][i]]);
     if (isTimer(&prev,0,19) != 1) prev_press=0;
     podskazka=1;
    }  
    
}

char *getLev(char dif,char *str)
{
   if (dif==0x4C) sprintf(str,"Лёгкий");
   if (dif==0x4D) sprintf(str,"Средний");
   if (dif==0x48) sprintf(str,"Тяжёлый");
   if (dif!=0x48 && dif!=0x4C && dif!=0x4D) sprintf(str,"Неизвестно");
   return str;
   
}


void WinerWindowRender()
{
  char str[32];
  
  int h = GetFontYSIZE(bcfg_mmenu_head_font) + 2;
  
  DrawImg(0, 0, (int)bgg);
  
  DrawSoftHeader(0,"ОК","Выиграл!");
  WSHDR *win=AllocWS(128);
  if (podskazka==0) wsprintf(win,"%t\n%t: %s\n%t:\n%t\n%t:\n%t","Выиграл!","Время",timeBySeconds(vremya, wintime),"Сложность",getLev(level,str),"Название",sudokuname);
    else wsprintf(win,"%t\n%t: %s\n%t:\n%t\n%t:\n%t","Выиграл! (Нечестно)","Время",timeBySeconds(vremya, wintime),"Сложность",getLev(level,str),"Название",sudokuname);
                             
  DrawString(win, 2, h,SCRW-2, SCRH, bcfg_gamew_head_font,TEXT_OUTLINE,bcfg_mmenu_sk_col,black);
  FreeWS(win); 
  
}






