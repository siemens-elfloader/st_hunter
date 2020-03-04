#include "main.h"
#define SKIN   //фон, варианты - COLOR,BG,SKIN 
#define STD    //сеть, варианты - ADV,STD
#define TMR_SECOND 216

char white[4]={0xFF,0xFF,0xFF,0x60};
char RED[4]={0xFF,0x00,0x00,0x60};
char prozr[4]={0x00,0x00,0x00,0x00};
char black[4]={0x00,0x00,0x00,0x60};

const char IMG1[126]="0:\\shell\\icons\\app.png";
const char IMG2[126]="0:\\shell\\icons\\BCFG.png";
const char IMG3[126]="0:\\shell\\icons\\calendar.png";
const char IMG4[126]="0:\\shell\\icons\\dev.png";
const char IMG5[126]="0:\\shell\\icons\\ELF.png";
const char IMG6[126]="0:\\shell\\icons\\explorer.png";
/*
const char MENU1_s[128]="2:\\japp\\FlexMenu\\1_s.png";//Маленькие иконки
const char MENU2_s[128]="2:\\japp\\FlexMenu\\2_s.png";
const char MENU3_s[128]="2:\\japp\\FlexMenu\\3_s.png";
const char MENU4_s[128]="2:\\japp\\FlexMenu\\4_s.png";
const char MENU5_s[128]="2:\\japp\\FlexMenu\\5_s.png";
const char MENU6_s[128]="2:\\japp\\FlexMenu\\6_s.png";

const char MENU1_b[128]="2:\\japp\\FlexMenu\\1_b.png";//Большие
const char MENU2_b[128]="2:\\japp\\FlexMenu\\2_b.png";
const char MENU3_b[128]="2:\\japp\\FlexMenu\\3_b.png";
const char MENU4_b[128]="2:\\japp\\FlexMenu\\4_b.png";
const char MENU5_b[128]="2:\\japp\\FlexMenu\\5_b.png";
const char MENU6_b[128]="2:\\japp\\FlexMenu\\6_b.png";
*/
/*const char MENU1_s[128]="0:\\shell\\menu\\1_s.png";//Маленькие иконки
const char MENU2_s[128]="0:\\shell\\menu\\2_s.png";
const char MENU3_s[128]="0:\\shell\\menu\\3_s.png";
const char MENU4_s[128]="0:\\shell\\menu\\4_s.png";
const char MENU5_s[128]="0:\\shell\\menu\\5_s.png";
const char MENU6_s[128]="0:\\shell\\menu\\6_s.png";

const char MENU1_b[128]="0:\\shell\\menu\\1_b.png";//Большие
const char MENU2_b[128]="0:\\shell\\menu\\2_b.png";
const char MENU3_b[128]="0:\\shell\\menu\\3_b.png";
const char MENU4_b[128]="0:\\shell\\menu\\4_b.png";
const char MENU5_b[128]="0:\\shell\\menu\\5_b.png";
const char MENU6_b[128]="0:\\shell\\menu\\6_b.png";*/
/*const char mnu[1].small="0:\\shell\\menu\\1_s.png";//Маленькие иконки
const char mnu[2].small="0:\\shell\\menu\\2_s.png";
const char mnu[3].small="0:\\shell\\menu\\3_s.png";
const char mnu[4].small="0:\\shell\\menu\\4_s.png";
const char mnu[5].small="0:\\shell\\menu\\5_s.png";
const char mnu[6].small"0:\\shell\\menu\\6_s.png";

const char mnu[1].big="0:\\shell\\menu\\1_b.png";//Большие
const char mnu[1].big="0:\\shell\\menu\\2_b.png";
const char mnu[1].big="0:\\shell\\menu\\3_b.png";
const char mnu[1].big="0:\\shell\\menu\\4_b.png";
const char mnu[1].big="0:\\shell\\menu\\5_b.png";
const char mnu[1].big="0:\\shell\\menu\\6_b.png";
*/
const char bg3img[128]="0:\\shell\\img\\BG.png";

GBSTMR timer,menu_tmr;
int panel_x=20,panel_y=120;//координаты панели
int Cross_x,Cross_y;//координаты крестика
int img_x=0,img_y=2;//координаты картинок для панели
int menu_x=40,menu_y=140;
unsigned int *err;
char per_s[]="%s";
char per_d[]="%d";
char per_t[]="%t";
char per_6s[]="%s\n%s\n%s\n%s\n%s\n%s";

int DrawPuskk=0;//1-Запушен, 0-нет

int start(char name[128])//Запуск файлов
{
 WSHDR *ws;
 ws=AllocWS(256);
 str_2ws(ws,name,256);
 ExecuteFile(ws,0,0);
 FreeWS(ws);
 return(1);
}

char *shortcut(char cut[15])//запуск шоткартов
{
int (*p) (void);
 p=(int(*)(void))GetFunctionPointer(cut);
 if(p!=NULL)
  (*p)();
 else
   ShowMSG(1,(int)"NULL pointer function!");
return 0;
}
GBSTMR mytmr;
void TimerProc(void)
{  REDRAW();
   GBS_StartTimerProc(&mytmr,TMR_SECOND,TimerProc);
}

//=============================================================================
void DrawBG()
{
//#ifdef COLOR
  if(workdesctop==1)
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(1)); 
//#endif
   
//#ifdef BG  
//  if(workdesctop==2)
  //{
    void *canvasdata=BuildCanvas();
    DrawCanvas(canvasdata,0,0,ScreenW()-1,ScreenH()-1,1);
//  }
  //WSHDR *ws=AllocWS(64);
//  wsprintf(ws,"%d",Items[0].x);
  //DrawString(ws,2,160,128,172,7,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125)); 
     
//#endif  
  
//#ifdef SKIN 
  if(workdesctop==3)
  {
   // void *canvasdata=BuildCanvas();
    //DrawCanvas(canvasdata,0,0,132,176,1);
    DrawImg(0,0,(int)bg3img);
  }
//#endif*/
}



//--------------------------------------------------
void DrawNetAdv(int x, int y)
{
    for (int i=0; i!=6; i++)  
      if((RamNet()+i)->power!=0) 
        DrawRoundedFrame(x+i*5,y+14-((115-(RamNet()+i)->power)/4),x+3+i*5,y+14,0,0,0,
           GetPaletteAdrByColorIndex( (((RamNet()+i)->ch_number)<255)?2:4 ),
               GetPaletteAdrByColorIndex( (((RamNet()+i)->ch_number)<255)?2:4) );
}


void DrawNetStd(int x, int y)
{
  #define NET_PIC_NUM 557
    DrawImg(x,y,(115-(RamNet()->power))/6+NET_PIC_NUM);
}
    
void DrawNetDb(int x, int y)
{
   WSHDR *ws = AllocWS(256); 
   wsprintf(ws, "%c%ddb",(RamNet()->ch_number<255)?'-':'=',RamNet()->power);
   DrawString(ws,x,y,x+Get_WS_width(ws,FONT_SMALL),y+GetFontYSIZE(FONT_SMALL),FONT_SMALL,0,blue,GetPaletteAdrByColorIndex(23)); 
   FreeWS(ws);
}


void DrawNetExt(int x, int y)
{ 
  int gh=(((RamNet())->ch_number)<255)?2:4;
  
  /*DrawLine(x+2,y,x+2,y+5,0,GetPaletteAdrByColorIndex(gh));
  DrawLine(x,y,x+4,y,0,GetPaletteAdrByColorIndex(gh));
  
  SetPixel(x,y+1,GetPaletteAdrByColorIndex(gh));
  SetPixel(x+4,y+1,GetPaletteAdrByColorIndex(gh));
  
  SetPixel(x,y+2,GetPaletteAdrByColorIndex(gh));
  SetPixel(x+4,y+2,GetPaletteAdrByColorIndex(gh));
  
  SetPixel(x+1,y+3,GetPaletteAdrByColorIndex(gh));
  SetPixel(x+3,y+3,GetPaletteAdrByColorIndex(gh));*/
//   SetPixel(50,50,GetPaletteAdrByColorIndex(gh));
//  x=x+6;
//  for(int i=(RamNet()->power>111)?112:RamNet()->power; i<=111; i=i+2,x+=3,j--)
  int cap=*RamCap()/10;
  int j=2;
  for(int i=0;i<cap;i++,x+=j+4,j++)
    DrawRectangle(x,y,x+j+3,y+j,0,GetPaletteAdrByColorIndex(0),blue);
/*  int net=RamNet()->power/10;
  for(int i=0;i<net;i++,x+=j+4,j++)
  {
    DrawRectangle(x,y,x+j+3,y+j,0,GetPaletteAdrByColorIndex(0),blue);
   
    SetPixel(x+4,y,blue);
    SetPixel(x+3,y+1,blue);
    SetPixel(x+2,y+2,blue);
    SetPixel(x+1,y+3,blue);
    SetPixel(x,y+4,blue);
    SetPixel(x-1,y+5,blue);  
  }*/
}
    
//--------------------------------------------------
void DrawBattStd(int x, int y)
{ 
#define BATT_PIC_NUM 461//601//cx75/25
//  DrawImg(x,y,(BATT_PIC_NUM+*RamCap()/9));
  int cap=*RamCap()/10;
  int j=10;
  for(int i=0;i<cap;i++,x+=j+4,j--)
    DrawRectangle(x,y,x+j+3,y+j,0,GetPaletteAdrByColorIndex(0),blue);
  //  DrawLine(x+j,y+i,x+cap,y+i,1,blue);
//  DrawStrInt(cap,100,120);
/*    DrawLine(x,y,x+cap,y,1,blue);
    DrawLine(x,y,x+cap,y,1,blue);
    DrawLine(x,y,x+cap,y,1,blue);
    DrawLine(x,y,x+cap,y,1,blue);
    DrawLine(x,y,x+cap,y,1,blue);*/
//    SetPixel(x+2,y,blue);    
}

void DrawBattExt(int x, int y)
{ 
  DrawImg(x,y,(BATT_PIC_NUM+*RamCap()/9));
}


void DrawBattCap(int x, int y)
{ 
  WSHDR *ws = AllocWS(256); 
  wsprintf(ws, "%d%%",*RamCap());
  DrawString(ws,x,y,x+Get_WS_width(ws,FONT_SMALL),y+GetFontYSIZE(FONT_SMALL),FONT_SMALL,0,blue,GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws);
}


void DrawBattVolt(int x, int y)
{ 
      WSHDR *ws = AllocWS(256); 
      wsprintf(ws, "%d,%dV",(GetAkku(0,9))/1000,(GetAkku(0,9))%1000/10);
      DrawString(ws,x,y,x+Get_WS_width(ws,FONT_SMALL),y+GetFontYSIZE(FONT_SMALL),FONT_SMALL,0,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(23)); 
      FreeWS(ws);
}




//--------------------------------------------------
void DrawClock(int x, int y)
{
  #pragma swi_number=0x80CF
  __swi __arm int *RamAlarm();
  
  TDate date;
  TTime time;
  WSHDR *ws = AllocWS(256); 
   
  GetDateTime(&date,&time); 

  wsprintf(ws, "%c%02d:%02d:%02d",(*RamAlarm()==15)?57750:' ',time.hour,time.min,time.sec);
  DrawString(ws,x,y,x+Get_WS_width(ws,FONT_SMALL),y+GetFontYSIZE(FONT_SMALL),FONT_SMALL,0,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(23)); 
 
  FreeWS(ws);
}




//--------------------------------------------------
void DrawBar(x,y)
{
  ;
}

/*void DrawOperator(int x,int y)
{
  #pragma swi_number=0x81F4
__swi __arm int GetOperator();

  WSHDR *ws = AllocWS(256);

 unsigned char *serv=(unsigned char*)0xA868BE3D;
 // int adr=0x68BE3C;

  wsprintf(ws, "%s",serv);
  DrawString(ws,x,y,132,176,7,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23)); 
 
  FreeWS(ws);
}*/



//--------------------------------------------------
void DrawDialogs(int x, int y)
{
      int z,i;   
      for (i=0,z=0; i!=5000; i++)  {if (FindCSMbyID(i)!=NULL) z++;}
    
    WSHDR *ws = AllocWS(256);
    wsprintf(ws, "csm:%i",z);
      DrawString(ws,x,y,x+Get_WS_width(ws,FONT_SMALL)+4,y+GetFontYSIZE(FONT_SMALL),FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(23)); 
    FreeWS(ws);
}

//--------------------------------------------------
void DrawFreeRam(int x, int y)
{
 WSHDR *ws = AllocWS(64); 
 wsprintf(ws, "Ram: %d kb",GetFreeRamAvail()/1024);
 DrawString(ws,x,y,x+Get_WS_width(ws,FONT_SMALL)+4,y+GetFontYSIZE(FONT_SMALL),FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(23)); 
 FreeWS(ws);
}
//--------------------------------------------------

//==================================================
void DrawCross(int x, int y,char color[4])//Крестик:) type:0-cross, 1-img
{ /*DrawLine(x,y,x+8,y,0,color);
  DrawLine(x+4,y-4,x+4,y+4,0,color);*/
if(typecur)
  DrawImg(x,y,(int)"0:\\shell\\img\\cursor.png");
else 
{
  DrawLine(x,y,x-4,y,0,color);//left
  DrawLine(x,y,x+4,y,0,color);//right
  DrawLine(x,y,x,y+4,0,color);//down
  DrawLine(x,y,x,y-4,0,color);//up
}

}

void DrawLineCross(int x, int y,char color[4],int TypeLine)//Делать мне нех:))))
{
  switch(TypeLine)
  {
  case 1: SetPixel(x+5,y,color);SetPixel(x+6,y+1,color);SetPixel(x+7,y+2,color);SetPixel(x+8,y+3,color); break;
  case 2: SetPixel(x-5,y,color);SetPixel(x-6,y+1,color);SetPixel(x-7,y+2,color);SetPixel(x-8,y+3,color);break;
  case 3: SetPixel(x+5,y,color);SetPixel(x+6,y+1,color);SetPixel(x+7,y+2,color);SetPixel(x+8,y+3,color);break;
  case 4: SetPixel(x+5,y,color);SetPixel(x+6,y+1,color);SetPixel(x+7,y+2,color);SetPixel(x+8,y+3,color);break;
  }
}
  
//==================================================
//char *NAME[6]={"MC","CfgEdit","Calendar","Dev.Menu","NatIcq","Card Explorer"};
void DrawName(int crd,int i,int type)//type: 1-vertical, 0-horizontal
{SaveNamePanel();
 WSHDR *ws2=AllocWS(64);
  wsprintf(ws2,per_s,NAME[i-1]);
//  if(type)
  //     DrawString(ws2,30,crd,Get_WS_width(ws2,FONT_SMALL),GetFontYSIZE(FONT_SMALL),FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125)); 
  //else DrawString(ws2,crd,30,Get_WS_width(ws2,FONT_SMALL),GetFontYSIZE(FONT_SMALL),FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125)); 

 FreeWS(ws2);
}

void DrawContur_w(int y)//Контур вертикальный
{
  DrawRoundedFrame(2,y+1,17,y+17,2,2,0,RED,prozr);
}

void DrawContur_h(int x)//Горизонтальный
{
  DrawRoundedFrame(x+1,2,x+17,17,2,2,0,RED,prozr);
}
//==================================================
void DrawPanel_w()//Вертикальная панель
{
  //panel_x=20,panel_y=120;
  void *canvasdata=BuildCanvas();
  int panel_y=6*20;
  img_y=2;
  DrawCanvas(canvasdata,1,1,19,panel_y,1);
  DrawImg(2,img_y,(int)IMG1);
  img_y+=GetImgHeight((int)IMG1)+2;
  DrawImg(2,img_y,(int)IMG2);
    img_y+=GetImgHeight((int)IMG2)+2;
  DrawImg(2,img_y,(int)IMG3);
    img_y+=GetImgHeight((int)IMG3)+2;
  DrawImg(2,img_y,(int)IMG4);
    img_y+=GetImgHeight((int)IMG4)+2;
  DrawImg(2,img_y,(int)IMG5);
    img_y+=GetImgHeight((int)IMG5)+2;
  DrawImg(2,img_y,(int)IMG6);
    img_y+=GetImgHeight((int)IMG6)+2;
  DrawRoundedFrame(0,0,GetImgWidth((int)IMG1)+2,img_y,2,2,0,white,prozr);
/*  DrawImg(2,img_y,(int)name[0].pic);
  DrawImg(2,img_y+20,(int)name[1].pic);
  DrawImg(2,img_y+40,(int)name[2].pic);
  DrawImg(2,img_y+60,(int)name[3].pic);
//  DrawImg(30,140,(int)IMG3);
//  DrawImg(60,140,(int)IMG4);
  DrawImg(2,img_y+80,(int)name[4].pic);
  DrawImg(2,img_y+100,(int)name[5].pic);*/
  DrawCross(Cross_x,Cross_y,RED);
  GBS_StartTimerProc(&timer,1,DrawPanel_w);
  GBS_StartTimerProc(&timer,1,DrawContur_w);
}
void DrawPanel_h()//Горизонтальная
{ 
  void *canvasdata=BuildCanvas();
  DrawCanvas(canvasdata,40,0,panel_y,panel_x,1);
  DrawRoundedFrame( 40,0,panel_y,panel_x,2,2,0,white,prozr);
  DrawImg(img_y+40,2,(int)IMG1);
  DrawImg(img_y+60,2,(int)IMG2);
  DrawImg(img_y+80,2,(int)IMG3);
  DrawImg(img_y+100,2,(int)IMG4);
  //DrawImg(2,img_y+80,(int)IMG5);
  //DrawImg(2,img_y+100,(int)IMG6);
  DrawCross(Cross_x,Cross_y,RED);
  GBS_StartTimerProc(&timer,1,DrawPanel_h);
  GBS_StartTimerProc(&timer,1,DrawContur_h);

}
void DrawNameContr()//Рисуем контур и имена
  {
   if(Cross_x>=0 && Cross_x<=17 && Cross_y>=0 && Cross_y <=20 && name[0].NumItems>=1)
   { DrawName(2,1,1);
     DrawContur_w(0);
   }
   else if(Cross_x>=0 && Cross_x<=17 && Cross_y>=20 && Cross_y <=40 &&name[0].NumItems>=2)
   { DrawName(19,2,1);
     DrawContur_w(20);
    }
   else if(Cross_x>=0 && Cross_x<=17 && Cross_y>=40 && Cross_y <=60 && name[0].NumItems>=3)
   { DrawName(35,3,1);
     DrawContur_w(40);
   }
   else if(Cross_x>=0 && Cross_x<=17 && Cross_y>=60 && Cross_y <=80 &&name[0].NumItems>=4)
   {   DrawName(52,4,1);
       DrawContur_w(60);
   }
   else if(Cross_x>=0 && Cross_x<=17 && Cross_y>=80 && Cross_y <=100 && name[0].NumItems>=5)
   {   DrawName(69,5,1);
       DrawContur_w(80);
   }
   else if(Cross_x>=0 && Cross_x<=17 && Cross_y>=100 && Cross_y <=120 && name[0].NumItems>=6)
   { DrawName(87,6,1);
     DrawContur_w(100);
   }
   /*//Горизонтальная
   else if(Cross_x>=40 && Cross_x<=60 && Cross_y>=0 && Cross_y <=17)
   { DrawName(40,1,0);
     DrawContur_h(40);
   }
   else if(Cross_x>=60 && Cross_x<=80 && Cross_y>=0 && Cross_y <=17)
   {   DrawName(60,2,0);
       DrawContur_h(60);
   }
   else if(Cross_x>=80 && Cross_x<=100 && Cross_y>=0 && Cross_y <=17)
   {   DrawName(80,3,0);
       DrawContur_h(80);
   }
   else if(Cross_x>=100 && Cross_x<=120 && Cross_y>=0 && Cross_y <=17)
   { DrawName(100,4,0);
     DrawContur_h(100);
   }*/
}

void StartFilePanel_w()//Стартуем  файл/шорты для панели
{    
       if(Cross_x>=0 && Cross_x<=20 && Cross_y>=0 && Cross_y <=20 && name[0].NumItems>=1)
       { if(strlen(name[0].file)==15) shortcut(name[0].file);
         else start(name[0].file);
         GBS_DelTimer(&timer);
         
       }
       else if(Cross_x>=0 && Cross_x<=20 && Cross_y>=20 && Cross_y <=40 && name[0].NumItems>=2)
       { if(strlen(name[1].file)==15) shortcut(name[1].file);
         else start(name[1].file);
         GBS_DelTimer(&timer);
         
       }
       else if(Cross_x>=0 && Cross_x<=20 && Cross_y>=40 && Cross_y <=60 && name[0].NumItems>=3)
       {  if(strlen(name[2].file)==15) shortcut(name[2].file);
         else start(name[2].file);
         GBS_DelTimer(&timer);
     
       }
       else if(Cross_x>=0 && Cross_x<=20 && Cross_y>=60 && Cross_y <=80 && name[0].NumItems>=4)
       { if(strlen(name[3].file)==15) shortcut(name[3].file);
         else start(name[3].file);
         GBS_DelTimer(&timer);
       }
       else if(Cross_x>=0 && Cross_x<=20 && Cross_y>=80 && Cross_y <=100 && name[0].NumItems>=5)
       { if(strlen(name[4].file)==15) shortcut(name[4].file);
         else start(name[4].file);
         GBS_DelTimer(&timer);
        }
       else if(Cross_x>=0 && Cross_x<=20 && Cross_y>=100 && Cross_y <=120 && name[0].NumItems>=6)
        { if(strlen(name[5].file)==15) start(name[5].file);
          else shortcut(name[5].file);
          GBS_DelTimer(&timer);
        }

}

void StartFilePanel_h()//Стартуем для горизонтальной панели 
{      if(Cross_x>=0 && Cross_x<=20 && Cross_y>=0 && Cross_y <=20)
       { start("0:\\zbin\\mc\\mc.elf");GBS_DelTimer(&timer);
         //DrawName(2,1);
       }
       else if(Cross_x>=20 && Cross_x<=40 && Cross_y>=0 && Cross_y <=20)
       { start("0:\\zbin\\utilities\\CfgEdit.elf");GBS_DelTimer(&timer);
         //DrawName(20,2);
       }
       else if(Cross_x>=40 && Cross_x<=60 && Cross_y>=0 && Cross_y <=20)
       {  shortcut("ORGZ_CALENDERMV");GBS_DelTimer(&timer);
         //DrawName(40,3);
       }
       else if(Cross_x>=60 && Cross_x<=80 && Cross_y>=0 && Cross_y <=20)
       { shortcut("ELSE_DEVELP_MNU");GBS_DelTimer(&timer);
          //DrawName(60,4);
       }
       else if(Cross_x>=80 && Cross_x<=100 && Cross_y>=0 && Cross_y <=20)
       {  start("0:\\zbin\\naticq\\naticq.elf");
        //  DrawName(80,5);
          GBS_DelTimer(&timer);
        }
     else if(Cross_x>=100 && Cross_x<=120 && Cross_y>=0 && Cross_y <=20)
     {  shortcut("FLSH_FLEXPLORER");
      // DrawName(100,6);
         GBS_DelTimer(&timer);
     }

}
//====================================================
int mmm=0;
void DrawMenu(int i)//Менюшка
{
  menu_x=80,menu_y=120; 
//  int k;
// void *canvasdata=BuildCanvas();
 //DrawCanvas(canvasdata,1,1,100,130,1);
/*  switch(i)
  {  
    case 1:  DrawImg(menu_x,menu_y,(int)mnu[i-1].big);break;
    case 2:  DrawImg(menu_x,menu_y,(int)mnu[i-1].big);break;
    case 3:  DrawImg(menu_x,menu_y,(int)mnu[i-1].big);break;
    case 4:  DrawImg(menu_x,menu_y,(int)mnu[i-1].big);break;
    case 5:  DrawImg(menu_x,menu_y,(int)mnu[i-1].big);break;
    case 6:  DrawImg(menu_x,menu_y,(int)mnu[i-1].big);break;
  }*/
// DrawImg(menu_x,menu_y,882);//Большие
 if(mmm<5)
 {
  DrawImg(menu_x,menu_y,931+mmm);//Большие
  mmm++;
 }
 else mmm=0;

//  if(i==1)k=12;
  //else k=i-1;
  DrawImg(menu_x-GetImgWidth(881),menu_y-15,881);//Маленькие

  if(i==12)i=0;
  DrawImg(menu_x+80,menu_y-15,883);//Маленькие

GBS_StartTimerProc(&menu_tmr,120,DrawMenu); 
}

void DrawNameMenu(int i)
{
//  SaveNameMENU();
//  char *NAME2[12]={"Контакты","Спис.Вызовов","Internet","Camera","Sms","Органайзер","Sms","Sms","Sms","Sms","Sms","Sms"};
  WSHDR *ws2=AllocWS(64);
  wsprintf(ws2,per_t,"Cообщения");
  DrawString(ws2,0,220,ScreenW()-1,ScreenH()-1,FONT_MEDIUM_BOLD,2,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws2);
}

void StartFileMenu(int i)
{
  switch(i)
  {
  case 1:shortcut(mnu[i-1].file);IRDA=!IRDA;break;
  case 2:shortcut(mnu[i-1].file);break;
  case 3:shortcut(mnu[i-1].file);break;
  case 4:shortcut(mnu[i-1].file);break;
  case 5:shortcut(mnu[i-1].file);break;
  case 6:shortcut(mnu[i-1].file);break;
  }    
}
//================================================================
//     Размер папки аля Мастер памяти,
//     где то есть косяк:( не правильно считает...:(
//================================================================
int dir_size=0;
static DIR_ENTRY de;
int FindSizeInSubDir(char *dir)
{
  unsigned int err;
  char path[256];
  strcpy(path,dir); 
  char *ptr=path+strlen(path)+1;
  strcat(path,"\\*.*");
  if (FindFirstFile(&de,path,&err)){ 
    do{ 
        strcpy(ptr,de.file_name);  
        dir_size += de.file_size;
       }while(FindNextFile(&de,&err));
  }FindClose(&de,&err);
return dir_size;
}

int SizeDir(char *dir)
{ unsigned int err;
  WSHDR *ws=AllocWS(64);
  char path[256];
  strcpy(path,dir); 
  char *ptr=path+strlen(path)+1;
  strcat(path,"\\*.*");
  if (FindFirstFile(&de,path,&err))
  { do{
        strcpy(ptr,de.file_name);  
        if(de.file_attr && FA_DIRECTORY) dir_size+=FindSizeInSubDir(path);
        else dir_size += de.file_size;
      }while(FindNextFile(&de,&err));
  }FindClose(&de,&err);
return dir_size;
}
//=============================================================================
void DrawFreeMamory(char *path,int y)
{    int x_p=15,y_p=15;
     WSHDR *ws=AllocWS(256);
     void *canvasdata=BuildCanvas();
     DrawCanvas(canvasdata,0,0,132,176,1);
     int total_0 = GetTotalFlexSpace(0, err)/1024;
     int free_0 = GetFreeFlexSpace(0, err)/1024;
     int per_0 = (free_0*100)/total_0;
     int app=SizeDir(path);
    
     wsprintf(ws, "Состояние памяти:\nДоступно: %d kb\nСвободно: %d kb",total_0,free_0);
  DrawString(ws,2,2,131,175,7,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125)); 

  wsprintf(ws, "%d",free_0);
  DrawString(ws,10+per_0,45+y_p,131,175,6,32,white,prozr); 

  DrawRoundedFrame(2,35+y_p,102+x_p,42+y_p,0,0,0,white,prozr);//Полоса
  DrawRoundedFrame(3,36+y_p,3+per_0+x_p,41+y_p,0,0,0,RED,RED); 
  wsprintf(ws,"Applications: %d kb",app/1024);
  DrawString(ws,2,y,131,175,7,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125)); 
 // FreeWS(ws);
}
//==============================================================================
int pusk_y=190;
void DrawPusk(int crd,int i)//Pusk:)
{// int k,j;  
  char *NAME[6]={"MC","NatIcq","Native Menu","SieJC","Opera 206","NRSS"};
  void *canvasdata=BuildCanvas();
  DrawCanvas(canvasdata,0,pusk_y,75,pusk_y+100,1);
  DrawRoundedFrame(0,pusk_y,75,pusk_y+100,3,5,0,white,prozr);
  WSHDR *ws=AllocWS(256);

  wsprintf(ws,per_6s,NAME[0],NAME[1],NAME[2],NAME[3],NAME[4],NAME[5]);
  DrawString(ws,2,pusk_y+2,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,white,prozr); 
      
// if(Cross_y>=30 && Cross_y<=175 && Cross_x>=0 && Cross_x<=30) DrawPuskk=1;
 if(Cross_x>=0 && Cross_x<=60 && Cross_y>=35+pusk_y && Cross_y <=50+pusk_y)
   {
     wsprintf(ws,per_s,NAME[0]);
     DrawString(ws,2,35+pusk_y,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125));
     DrawPuskk=1;
   }
 else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=50+pusk_y && Cross_y <=62+pusk_y)
   { 
     wsprintf(ws,per_s,NAME[1]);
     DrawString(ws,2,47+pusk_y,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125));
     DrawPuskk=1;
     
   }
  else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=62+pusk_y && Cross_y <=75+pusk_y)
   { 
     wsprintf(ws,per_s,NAME[2]);
     DrawString(ws,2,59+pusk_y,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125));
     DrawPuskk=1;
   }
 else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=(GetFontYSIZE(FONT_SMALL)*4)+pusk_y && Cross_y <=87+pusk_y)
   { 
     wsprintf(ws,per_s,NAME[3]);
     DrawString(ws,2,61+pusk_y,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125));
     DrawPuskk=1;
   }
 else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=(GetFontYSIZE(FONT_SMALL)*5)+pusk_y && Cross_y <=99+pusk_y)
   { 
     wsprintf(ws,per_s,NAME[4]);
     DrawString(ws,2,83+pusk_y-GetFontYSIZE(FONT_SMALL),ScreenW()-1,ScreenH()-1,FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125));
     DrawPuskk=1;
   }
  else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=99+pusk_y && Cross_y <=111+pusk_y)
   { 
     wsprintf(ws,per_s,NAME[5]);
     DrawString(ws,2,95+pusk_y,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(125));
     DrawPuskk=1;
   }
 else DrawPuskk=0;
DrawCross(Cross_x,Cross_y,prozr);
GBS_StartTimerProc(&timer,262,DrawPusk);
FreeWS(ws);
}

void StartFilePusk()//Запуск файлов меню пуск :)
{ if(Cross_x>=0 && Cross_x<=60 && Cross_y>=35+pusk_y && Cross_y <=50+pusk_y)
   { start("0:\\zbin\\mc\\mc.elf");//GBS_DelTimer(&timer);
    }
 else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=50+pusk_y && Cross_y <=62+pusk_y)
   { start("0:\\zbin\\utilities\\CfgEdit.elf");//GBS_DelTimer(&timer);
    }
  else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=62+pusk_y && Cross_y <=75+pusk_y)
   { shortcut("ORGZ_CALENDERMV");//GBS_DelTimer(&timer);
    }
 else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=75+pusk_y && Cross_y <=87+pusk_y)
   { shortcut("ELSE_DEVELP_MNU");//GBS_DelTimer(&timer);
    }
 else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=87+pusk_y && Cross_y <=99+pusk_y)
   { start("0:\\zbin\\naticq\\naticq.elf");//GBS_DelTimer(&timer);
    }
  else  if(Cross_x>=0 && Cross_x<=60 && Cross_y>=99+pusk_y && Cross_y <=111+pusk_y)
   { shortcut("FLSH_FLEXPLORER");//GBS_DelTimer(&timer);
    }
}
//==============================================================================
void DrawStrInt(int s,int x, int y)//Для отладки...
{
  WSHDR *ws = AllocWS(5); 
  wsprintf(ws, per_d,s);
  DrawString(ws,x,y,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,GetPaletteAdrByColorIndex(124),GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws);
}
void DrawStrChar(char *s,int x, int y)
{  
  int len=strlen(s);
  WSHDR *ws = AllocWS(len); 
  wsprintf(ws, per_s,s);
  DrawString(ws,x,y,ScreenW()-1,ScreenH()-1,FONT_SMALL,32,blue,GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws);
}

//==============================================================================
/*void DrawInfo()
{
   DrawRoundedFrame(2,35,102,42+y_p,0,0,0,white,prozr);//Полоса
   DrawNetAdv (crd[NetAdv].x, crd[NetAdv].y);
   DrawNetDb ( crd[NetDB].x, crd[NetDB].y);
   DrawDialogs (crd[Dialogs].x, crd[Dialogs].y);
   DrawFreeRam(100,5); 


}*/

void DrawWorkDesc()
{
   if(Cross_x>=121 && Cross_x<=128 && Cross_y>=cr_y && Cross_y<=170)
    {
      DrawStrInt(1,120,130);//Номер раб.стола
      DrawStrInt(2,120,144);//Номер раб.стола
      DrawStrInt(3,120,158);//Номер раб.стола
      cr_y=130;
    }
   else
   {
      DrawStrInt(workdesctop,120,158);//Номер раб.стола
      cr_y=157;
   }

 if(!startmenu && !kdblock)
  {/*
    if(Cross_x>=ScreenW()-1)
    {
      if(workdesctop<3) workdesctop++;
      else workdesctop=1;
      Cross_x=1;
    }
    else if(Cross_y>ScreenH()-1)Cross_y=ScreenH()-1;
    else if(Cross_x<=1)
    {
      if(workdesctop>1) workdesctop--; 
      else workdesctop=3;
      Cross_x=131;
    }*/
    DrawCross(Cross_x,Cross_y,white);
  }
}

void DrawSysBar()
{

}
