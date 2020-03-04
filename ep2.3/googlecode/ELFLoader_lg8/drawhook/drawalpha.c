//drawalpha.c
//(c)Dimadze

#include "config\defines.h"

//24 битный пиксель преобразуем в 16 битный
#define RGB16(R,G,B) ((B>>3)+((G>>2)<<5)+((R>>3)<<11))


typedef struct{
  short x;
  short y;
  short x2;
  short y2;
} RECT;


typedef struct
{
#ifdef ELKA
  unsigned short w;
  unsigned short h;
  long bpnum;  //For BW=1, 8bit=5, 16bit=8, 32bit=0x0A, 0x80 - packed
#else
  char w;
  char h;
  unsigned short bpnum; //For BW=1, 8bit=5, 16bit=8, 32bit=0x0A, 0x80 - packed
#endif
  char *bitmap;
}IMGHDR;


typedef struct{
  char   unk_5;    // 0x05
  char   unk_zero; // 0x00 (Не факт!)
  char   unk_0_0;  // 0x00
  char   und_0[9]; // Незаполненный массив char'ов 
  RECT   rc;  
  IMGHDR *img;
  short  unk_0_1;  // 0x0000
  char   und_1;    // Незаполненный байт
  char   unk_0_2;  // 0x00
}DRWOBJ_5;

// Производная от SetPropTo_Obj5()
// Незаполненный, значит ф-ия этот участок памяти и не трогала!

__thumb short mixcolor(short bg, char *font)
{
 char r = (((bg&0xF800)>>8)*(255-font[3])+font[0]*font[3])/255;
 char g = (((bg&0x7E0)>>3)*(255-font[3])+font[1]*font[3])/255;
 char b = (((bg&0x1F)<<3)*(255-font[3])+font[2]*font[3])/255;
 return RGB16(r,g,b);
}

__thumb void DrawObject_5_0x0A(DRWOBJ_5 *obj)
{
 char *scrbuf = (char *) RamScreenPhoneCache;
 for (int h=0;h<obj->img->h;h++) 
  {
   for (int w=0;w<obj->img->w*4;w+=4) 
    {
      if (obj->rc.x+w/4 < SCRW && obj->rc.x+w/4 >= 0 && obj->rc.y+h < SCRH && obj->rc.y+h >= 0)
       {
        char color[4];
       
        color[3]=obj->img->bitmap[w + obj->img->w*4*h + 3]; 
        
        if (color[3]>0x00)  
         { 
          color[0]=obj->img->bitmap[w + obj->img->w*4*h + 2];
          color[1]=obj->img->bitmap[w + obj->img->w*4*h + 1];
          color[2]=obj->img->bitmap[w + obj->img->w*4*h + 0];  
         
        
          if (color[3]==0xFF) *(unsigned short *)((char *)scrbuf + obj->rc.y*SCRW*PIXEL + 2*obj->rc.x + w/2 + h*SCRW*PIXEL) = RGB16(color[0],color[1],color[2]);
          else
           *(unsigned short *)((char *)scrbuf + obj->rc.y*SCRW*PIXEL + 2*obj->rc.x + w/2 + h*SCRW*PIXEL) = mixcolor(*(unsigned short *)((char *)scrbuf + obj->rc.y*SCRW*PIXEL + 2*obj->rc.x + w/2 + h*SCRW*PIXEL),color); 
         }
       }
    }
  }
}







