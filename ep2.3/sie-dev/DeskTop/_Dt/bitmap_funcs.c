#include "../../inc/swilib.h"


#pragma inline
int infoc(RECT *p, short x, short y)
{
	if((x>=p->x)&&(x<p->x2)&&(y>=p->y)&&(y<p->y2)) return 1;
	else return 0;
}

#include "..\plglib.h"
#include "bitmap_funcs.h"
#include "Compressing_IMGHDR.c"

// альфа-наложение
// Приём.Blue=alpha* Источн.Blue+(1-alpha)* Приём.Blue
// Приём.Green=alpha* Источн.Green+(1-alpha)* Приём.Green
// Приём.Red=alpha* Источн.Red+(1-alpha)* Приём.Red
// Приём.alpha=alpha* Источн.alpha+(1-alpha)* Приём.alpha

/* alpha blend routine */
// unsigned int AlphaBlend(const unsigned int bg, const unsigned int src)
// {
   // unsigned int a = src >> 24;    /* alpha */

   // /* If source pixel is transparent, just return the background */
   // if (0 == a) 
      // return bg;

   // /* alpha blending the source and background colors */
   // unsigned int rb = (((src & 0x00ff00ff) * a) +  
      // ((bg & 0x00ff00ff) * (0xff - a))) & 0xff00ff00;
   // unsigned int    g  = (((src & 0x0000ff00) * a) + 
      // ((bg & 0x0000ff00) * (0xff - a))) & 0x00ff0000;

    // return (src & 0xff000000) | ((rb | g) >> 8);
// }



typedef struct {
  int x;
  int y;
}POINT;

typedef struct {
  char R;
  char G;
  char B;
  char A;
}color;

#define CTYPE1 10
#define CTYPE2 3

int sqr(int x)
{
  return x*x;
}

int abs(int x)
{
  return x < 0 ? x*(-1) : x;
}

int max(int x, int y)
{
  return x > y ? x : y;
}

int min(int x, int y)
{
  return x < y ? x : y;
}



color RGBA(char R, char G, char B, char A)
{
  color t={R,G,B,A};
  return t;
}


color getcolor(IMGHDR *img, int x, int y)
{
  color *bm=(color*)img->bitmap;
  if(x < img->w && x>=0 && y < img->h && y>=0) 
    return *(bm + x + y*img->w);
  else
    return (color){0,0,0,0};
}


void setcolor(IMGHDR *img, int x, int y, color clr)
{
  color *bm=(color*)img->bitmap;
  if(x < img->w && x>=0 && y < img->h && y>=0)
    *(bm + x + y*img->w)=clr;
}

void bmfree(IMGHDR *img)
{
  int x, y;
  for(y=0; y<img->h; y++)
    for(x=0; x<img->w; x++)
      setcolor(img, x, y, (color){0,0,0,0});
}

IMGHDR *createIMGHDR(int w, int h, int type)
{
  IMGHDR *img=malloc(sizeof(IMGHDR));
  img->w=w; 
  img->h=h; 
  img->bpnum=type;
  img->bitmap=malloc(h*w*sizeof(color));
  bmfree(img);
  return img;
}

void deleteIMGHDR(IMGHDR *img)
{
  mfree(img->bitmap);
  mfree(img); 
}

IMGHDR *resample(IMGHDR *img, int px, int py, int fast, int del)
{
  //if (px==100 && py==100) return img;
  if (px==img->w && py==img->h) return img;
  
  // long newx = (img->w*px)/100,
  // newy = (img->h*py)/100;
  long newx = px,
  newy = py;
  
  float xScale, yScale, fX, fY;
  xScale = (float)img->w  / (float)newx;
  yScale = (float)img->h / (float)newy;
  
  IMGHDR *img2=createIMGHDR(newx,newy, CTYPE1);
  if (fast) {
    for(long y=0; y<newy; y++){
      fY = y * yScale;
      for(long x=0; x<newx; x++){
        fX = x * xScale;
        setcolor(img2,  x,  y, getcolor(img, (long)fX,(long)fY));
      }
    }
  } else {
    long ifX, ifY, ifX1, ifY1, xmax, ymax;
    float ir1, ir2, ig1, ig2, ib1, ib2, ia1, ia2, dx, dy;
    char r,g,b,a;
    color rgb1, rgb2, rgb3, rgb4;
    xmax = img->w-1;
    ymax = img->h-1;
    for(long y=0; y<newy; y++){
      fY = y * yScale;
      ifY = (int)fY;
      ifY1 = min(ymax, ifY+1);
      dy = fY - ifY;
      for(long x=0; x<newx; x++){
        fX = x * xScale;
        ifX = (int)fX;
        ifX1 = min(xmax, ifX+1);
        dx = fX - ifX;
        rgb1= getcolor(img, ifX,ifY);
        rgb2= getcolor(img, ifX1,ifY);
        rgb3= getcolor(img, ifX,ifY1);
        rgb4= getcolor(img, ifX1,ifY1);
        
        ir1 = rgb1.R   * (1 - dy) + rgb3.R   * dy;
        ig1 = rgb1.G * (1 - dy) + rgb3.G * dy;
        ib1 = rgb1.B  * (1 - dy) + rgb3.B  * dy;
        ia1 = rgb1.A  * (1 - dy) + rgb3.A  * dy;
        ir2 = rgb2.R   * (1 - dy) + rgb4.R   * dy;
        ig2 = rgb2.G * (1 - dy) + rgb4.G * dy;
        ib2 = rgb2.B  * (1 - dy) + rgb4.B  * dy;
        ia2 = rgb2.A  * (1 - dy) + rgb4.A  * dy;
        
        r = (char)(ir1 * (1 - dx) + ir2 * dx);
        g = (char)(ig1 * (1 - dx) + ig2 * dx);
        b = (char)(ib1 * (1 - dx) + ib2 * dx);
        a = (char)(ia1 * (1 - dx) + ia2 * dx);
        
        setcolor(img2, x,y,RGBA(r,g,b,a));
      }
    }
  }
  
  if(del)
    deleteIMGHDR(img);
  return img2;
}








/* alpha blend routine */
char *AlphaBlend(char *bg, char *src, char a)
{
	char s[4], src_a=src[3]*a/100;
	s[3] = 0xff-(0xff-src_a)*(0xff-bg[3])/0xff;
	for(int i=0; i<3; i++)
		s[i] = src_a*src[i]/s[3] + bg[3]*bg[i]*(0xff-src_a)/s[3]/0xff;
	memcpy(bg, s, 4);
	return NULL;
}

// qq= sr-((sr-br)*ba)/0xff;
// outr= qq+((sr-qq)*sa)/0xff;


			// qq=pp->sl[k].map[ i*4 ]-((pp->sl[k].map[ i*4 ]-tobuf[ i*4 ])*tobuf[i*4+3])/0xff;
			// tobuf[ i*4 ]=qq+((pp->sl[k].map[ i*4 ]-qq)*pp->sl[k].map[i*4+3])/0xff;
			// qq=pp->sl[k].map[i*4+1]-((pp->sl[k].map[i*4+1]-tobuf[i*4+1])*tobuf[i*4+3])/0xff;
			// tobuf[i*4+1]=qq+((pp->sl[k].map[i*4+1]-qq)*pp->sl[k].map[i*4+3])/0xff;
			// qq=pp->sl[k].map[i*4+2]-((pp->sl[k].map[i*4+2]-tobuf[i*4+2])*tobuf[i*4+3])/0xff;
			// tobuf[i*4+2]=qq+((pp->sl[k].map[i*4+2]-qq)*pp->sl[k].map[i*4+3])/0xff;
			// tobuf[i*4+3]= tobuf[i*4+3]+((0xff-tobuf[i*4+3])*pp->sl[k].map[i*4+3])/0xff;

int GetBitmapMapPos(IMGHDR *img, int x, int y)
{
	if((!img)||(x>=img->w)||(y>=img->h)) return -1;
	switch(img->bpnum)
	{
	 case 5:
		return (img->w*y+x);
	 case 8:
		return (img->w*y+x)*2;
	 case 10:
		return (img->w*y+x)*4;
	 default: return -1;
	}
}



int DrwImg2Img(IMGHDR *img, RECT p, IMGHDR *buf, char alfa)
{
	if((!img)||(!buf)) return 0;
	int w, h;
	int t=GetBitmapMapPos(buf, p.x, p.y);
	if(p.x2>=buf->w) w=buf->w-p.x;
	else w=p.x2-p.x;
	if(p.y2>=buf->h) h=buf->h-p.y;
	else h=p.y2-p.y;
	if(buf->bpnum==0xA)
	{
		if(t>=0)
		for(int i=0; i<h; i++)
		{
			//zeromem(buf->bitmap+t+i*buf->w*4, w*4);
		  for(int j=0; j<w; j++)
			if(img->bpnum==0xA) AlphaBlend(buf->bitmap+t+i*buf->w*4+j*4, img->bitmap+i*img->w*4+j*4, alfa);
		}
			//memcpy(buf->bitmap+t+i*buf->w*4+j*4, AlphaBlend((int)buf->bitmap+t+i*buf->w*4+j*4, (int)img->bitmap+i*img->w*4+j*4), 4);
		//{
			// memcpy(buf->bitmap+t+i*buf->w*4, img->bitmap+i*img->w*4, w*4);
		  // for(int j=0; j<w; j++)
			// buf->bitmap[t+i*buf->w*4+j*4+3]=img->bitmap[i*img->w*4+j*4+3]*alfa/100;
		//}
	}
	else if(buf->bpnum==8)
	{
		if(t>=0)
		for(int i=0; i<h; i++)
		{
			//memcpy(buf->bitmap+t+i*buf->w*2, fn->bitmap+t+i*buf->w*2, w*2);
		  for(int j=0; j<w; j++)
			if(img->bpnum==0xA)
			{
				int col=_16_RGB_32(buf->bitmap+t+i*buf->w*2+j*2);
				AlphaBlend((char*)&col, img->bitmap+i*img->w*4+j*4, alfa);
				short cl=BGR_24_16((char*)&col);
				memcpy(buf->bitmap+t+i*buf->w*2+j*2, (char*)&cl, 2);
			}
			else if(img->bpnum==8)
			{
				int col=_16_RGB_32(buf->bitmap+t+i*buf->w*2+j*2);
				int col2=_16_RGB_32(img->bitmap+i*img->w*2+j*2);
				AlphaBlend((char*)&col, (char*)&col2, alfa);
			}
		}
			//memcpy(buf->bitmap+t+i*buf->w*4+j*4, AlphaBlend((int)buf->bitmap+t+i*buf->w*4+j*4, (int)img->bitmap+i*img->w*4+j*4), 4);
		//{
			// memcpy(buf->bitmap+t+i*buf->w*4, img->bitmap+i*img->w*4, w*4);
		  // for(int j=0; j<w; j++)
			// buf->bitmap[t+i*buf->w*4+j*4+3]=img->bitmap[i*img->w*4+j*4+3]*alfa/100;
		//}
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

// typedef struct{
	// short x,y, x2,y2, w,h;
// }IMPR;

int IsImposingRect(RECT *p1, RECT *p2)
{
	if((((p1->x >= p2->x) && (p1->x <= p2->x2)) ||
		((p1->x2 >= p2->x) && (p1->x2 <= p2->x2))) &&
		(((p1->y >= p2->y) && (p1->y <= p2->y2)) ||
		((p1->y2 >= p2->y) && (p1->y2 <= p2->y2))))
	{
		// IMPR *pf=malloc(sizeof(IMPR));
		// if(p1->x >= p2->x) pf.x=p1->x;
		// else pf.x=p2->x;
		// if(p1->x2 <= p2->x2) pf.x2=p1->x2;
		// else pf.x2=p2->x2;
		// if(p1->y >= p2->y) pf.y=p1->y;
		// else pf.y=p2->y;
		// if(p1->y2 <= p2->y2) pf.y2=p1->y2;
		// else pf.y2=p2->y2;
		// w=pf.x2-pf.x;
		// h=pf.y2-pf.y;
		return 1;
	}
	else return 0;
}

// int filrects(DT *dt)
// {
//    zeromem(dt->ni.bitmap, dt->ni.w*dt->ni.h);
//    DT_ICON *TI=dt->first;
// 	do
// 	{
// 	  FillRectByNum(&dt->ni, TI->p, TI->n);
// 	}while(TI=TI->next);
// 	return 1;
// }

int ICON_REDRAW(void *dtt, DT_ICON *ic)
{
	DT *dt=dtt;
	if(!ic->img)
	{
	  dt->ef->fill_icons(2);
	  return 0;
	}
	//IMGHDR *img=ic->img;
	IMGHDR *buf=&dt->i;
	
	if((ic->img->w!=ic->p.x2-ic->p.x)||(ic->img->h!=ic->p.y2-ic->p.y))
			ic->img=resample(ic->img, ic->p.x2-ic->p.x, ic->p.y2-ic->p.y, 0, 1);
	
	//int alfa=ic->alfa;
	int w, h;//, i, j, i2, j2;
	int t=GetBitmapMapPos(buf, ic->p.x, ic->p.y);
	if(ic->p.x2>=buf->w) w=buf->w-ic->p.x;
	else w=ic->p.x2-ic->p.x;
	if(ic->p.y2>=buf->h) h=buf->h-ic->p.y;
	else h=ic->p.y2-ic->p.y;
	// if(buf->bpnum==0xA)
	// {
		// if(t>=0)
		// for(int i=0; i<h; i++)
		// {
			// zeromem(buf->bitmap+t+i*buf->w*4, w*4);
		  // for(int j=0; j<w; j++)
			// if(img->bpnum==0xA) AlphaBlend(buf->bitmap+t+i*buf->w*4+j*4, img->bitmap+i*img->w*4+j*4, alfa);
		// }
	// }
	// else 
	//IMPR *im;
	if(buf->bpnum==8)
	{
		
		DT_ICON *TAI=dt->first;
		if(t>=0)
		{
		 for(int i=0; i<h; i++)
		 {
			memcpy(buf->bitmap+t+i*buf->w*2, dt->fon->bitmap+t+i*buf->w*2, w*2);
		 }
		 do
		 {
		 //if(TAI!=ic)
		 //{
		  if(TAI->img && !TAI->dyn)
		  //if( IsImposingRect(&TAI->p, &ic->p) )
		  {
			//t=GetBitmapMapPos(buf, im->x, im->y);
			//w=im->w;
			//h=im->h;
			for(int i=0; i<h; i++)
			{
				//memcpy(buf->bitmap+t+i*buf->w*2, dt->fon->bitmap+t+i*buf->w*2, w*2);
			  for(int j=0; j<w; j++)
				if(TAI->img->bpnum==0xA)
				{
				 if(infoc(&TAI->p, ic->p.x+j, ic->p.y+i))
				 {
					int col=_16_RGB_32(buf->bitmap+t+i*buf->w*2+j*2);
					AlphaBlend((char*)&col, TAI->img->bitmap+(i-TAI->p.y+ic->p.y)*TAI->img->w*4+(j-TAI->p.x+ic->p.x)*4, TAI->alfa);
					short cl=BGR_24_16((char*)&col);
					memcpy(buf->bitmap+t+i*buf->w*2+j*2, (char*)&cl, 2);
				 }
				}
				else if(TAI->img->bpnum==8)
				{
				 if(infoc(&TAI->p, ic->p.x+j, ic->p.y+i))
				 {
					int col=_16_RGB_32(buf->bitmap+t+i*buf->w*2+j*2);
					int col2=_16_RGB_32(TAI->img->bitmap+(i-TAI->p.y+ic->p.y)*TAI->img->w*2+(j-TAI->p.x+ic->p.x)*2);
					AlphaBlend((char*)&col, (char*)&col2, TAI->alfa);
					short cl=BGR_24_16((char*)&col);
					memcpy(buf->bitmap+t+i*buf->w*2+j*2, (char*)&cl, 2);
				 }
				}
			}
		  }
		 //}
		 }while(TAI=TAI->next);
		}
	}
	dt->ef->fill_icons(2);
	return 0;
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////



int FillRectByNum(IMGHDR *buf, RECT p, char n)
{
	if(!buf) return 0;
	int w, h;
	int t=GetBitmapMapPos(buf, p.x, p.y);
	if(p.x2>=buf->w) w=buf->w-p.x;
	else w=p.x2-p.x;
	if(p.y2>=buf->h) h=buf->h-p.y;
	else h=p.y2-p.y;
	// if(buf->bpnum==5)
	{
		if(t>=0)
		 for(int i=0; i<h; i++)
		  for(int j=0; j<w; j++)
			buf->bitmap[t+i*buf->w+j]=n;
		   //buf->bitmap[GetBitmapMapPos(buf, p.x+j, p.y+w)]=n;
	}
	return 1;
}










////////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////



// #include "..\inc\swilib.h"
#include "..\..\inc\pnglist.h"

unsigned int DEFAULT_COLOR=3;
unsigned int ALPHA_THRESHOLD=128;
// extern unsigned int CACHE_PNG;
// extern unsigned int DEFAULT_DISK_N;

// const char DEFAULT_FOLDER[]=":\\ZBin\\img\\";

#define number 8

const char Pointer[1]={0xFF};
const IMGHDR empty_img = {0,0,0x1,(char *)Pointer};

void* xmalloc(int x,int n)
{
  return malloc(n);
}

void xmfree(int x,void* ptr)
{
  mfree(ptr);
}

void read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length)
{
  unsigned int err;
  int f;
  f=(int)png_get_io_ptr(png_ptr);
  fread(f, data, length, &err);
}

IMGHDR* create_imghdr(const char *fname, int type)
{
  int f;
  char buf[number];
  unsigned int err;
  struct PP
  {
    char *row;
    char *img;
    IMGHDR * img_h;
  } pp;
  IMGHDR * img_hc;
  png_structp png_ptr=NULL;
  png_infop info_ptr=NULL;
  png_uint_32 rowbytes;
  
  if ((f=fopen(fname, A_ReadOnly+A_BIN, P_READ, &err))==-1) return 0;
  pp.row=NULL;
  pp.img=NULL;
  pp.img_h=NULL;
  
  if (fread(f, &buf, number, &err)!=number) goto L_CLOSE_FILE;
  if  (!png_check_sig((png_bytep)buf,number)) goto  L_CLOSE_FILE;
  
  png_ptr = png_create_read_struct_2("1.2.5", (png_voidp)0, 0, 0, (png_voidp)0,(png_malloc_ptr)xmalloc,(png_free_ptr)xmfree);
  if (!png_ptr) goto L_CLOSE_FILE;
  
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    goto L_CLOSE_FILE;
  }
  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    goto L_CLOSE_FILE;
  }
  
  png_set_read_fn(png_ptr, (void *)f, read_data_fn);
  
  png_set_sig_bytes(png_ptr, number);
  
  png_read_info(png_ptr, info_ptr);
  
  png_uint_32 width, height;
  int bit_depth, color_type;
  
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
  
  if (type==0)
  {
    if (color_type == PNG_COLOR_TYPE_GRAY) 
      type=PNG_1;
    else type=DEFAULT_COLOR+1;
  }
  
  if (bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
    
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr); 
  
  if (bit_depth == 16) png_set_strip_16(png_ptr);
  
  if (bit_depth < 8) png_set_packing(png_ptr);
  
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);
  
  if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_GRAY)
    png_set_gray_to_rgb(png_ptr);
  
  png_set_filler(png_ptr,0xFF,PNG_FILLER_AFTER);
  png_read_update_info(png_ptr, info_ptr);
  
  rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  
  pp.row=malloc(rowbytes);
  pp.img_h=img_hc=malloc(sizeof(IMGHDR));
  
  if (type==PNG_1)
  {
    int rowc_w=(width+7)>>3;
    int size=height*rowc_w;
    unsigned char *iimg=(unsigned char *)(pp.img=malloc(size));
    zeromem(iimg,size);
    for (unsigned int y = 0; y<height; y++)
    {
      png_read_row(png_ptr, (png_bytep)pp.row, NULL);
      for (unsigned int x = 0; x<width; x++)
      {
        if (!pp.row[x*4+0] && !pp.row[x*4+1] && !pp.row[x*4+2])
          iimg[x>>3]|=(0x80>>(x&7));
      }
      iimg+=rowc_w;
    }
    pp.img_h->bpnum=1;
  }
  else
  {
    switch (type)
    {
    case PNG_8:
      {
        unsigned char *iimg=(unsigned char *)(pp.img=malloc(width*height));
        for (unsigned int y = 0; y<height; y++)
        {
          png_read_row(png_ptr, (png_bytep)pp.row, NULL);
          for (unsigned int x = 0; x<width; x++)
          {
            if (pp.row[x*4+3]<ALPHA_THRESHOLD)
              *iimg++=0xC0;
            else
            {
              unsigned char c=(pp.row[x*4+0] & 0xE0);
              c|=((pp.row[x*4+1]>>3)&0x1C);
              c|=((pp.row[x*4+2]>>6)&0x3);
              *iimg++=c;
            }
          }
        }
        pp.img_h->bpnum=5;
        break;
      }
    case PNG_16:
      {
        unsigned short *iimg=(unsigned short *)(pp.img=malloc(width*height*2));
        for (unsigned int y = 0; y<height; y++)
        {
          png_read_row(png_ptr, (png_bytep)pp.row, NULL);
          for (unsigned int x = 0; x<width; x++)
          {
            if (pp.row[x*4+3]<ALPHA_THRESHOLD)
              *iimg++=0xE000;
            else
            {
              unsigned int c=((pp.row[x*4+0]<<8)&0xF800);
              c|=((pp.row[x*4+1]<<3)&0x7E0);
              c|=((pp.row[x*4+2]>>3)&0x1F);
              *iimg++=c;
            }
          }
        }
        pp.img_h->bpnum=8;
        break;
      }
// #if NEWSGOLD || X75
    case 3:
      {
        unsigned char *iimg=(unsigned char *)(pp.img=malloc((width*height)<<2));
        for (unsigned int y = 0; y<height; y++)
        {
          png_read_row(png_ptr, (png_bytep)pp.row, NULL);
          for (unsigned int x = 0; x<width; x++)
          {
	    unsigned int c;
            *iimg++=pp.row[x*4+2];
            *iimg++=pp.row[x*4+1];
            *iimg++=pp.row[x*4+0];
	    c=pp.row[x*4+3];
//	    if (c>=128) c++;
//	    c*=100;
//	    c>>=8;
            *iimg++=c;//(pp.row[x*4+3]*100)/0xFF;
          }
        }
        pp.img_h->bpnum=0xA;
        break;
      }
// #endif
    }
  }
  pp.img_h->w=width;
  pp.img_h->h=height;
  //pp->img_h->zero=0;
  pp.img_h->bitmap=pp.img;
  
  png_read_end(png_ptr, info_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
  if (!pp.img)
  {
  L_CLOSE_FILE:
    mfree(pp.row);
    mfree(pp.img);
    mfree(pp.img_h);
    fclose(f, &err);
    return NULL;
  }
  mfree(pp.row);
  fclose(f, &err);
  return (img_hc);
}











IMGHDR* img_24_16(IMGHDR *img, int del)
{
	IMGHDR *img2=malloc(sizeof(IMGHDR));
	img2->w=img->w; 
	img2->h=img->h; 
	img2->bpnum=8;
	img2->bitmap=malloc(img->h*img->w*2);
	zeromem(img2->bitmap, img->h*img->w*2);
	
	
// 	p->w->bpnum=8;
	short cl;
// 	unsigned short *iimg=(unsigned short *)p->w->bitmap;
	for (unsigned int y = 0; y<img->h; y++)
	{
		//png_read_row(png_ptr, (png_bytep)pp.row, NULL);
		for (unsigned int x = 0; x<img->w; x++)
		{
			if (img->bitmap[(x+y*img->w)*4+3]<ALPHA_THRESHOLD)
				cl=0xE000;
			else
			{
				cl=BGR_24_16(img->bitmap+(x+y*img->w)*4);
// 				unsigned int c=((p->w->bitmap[x*4+0]<<8)&0xF800);
// 				c|=((p->w->bitmap[x*4+1]<<3)&0x7E0);
// 				c|=((p->w->bitmap[x*4+2]>>3)&0x1F);
// 				*iimg++=c;
			}
			memcpy(img2->bitmap+(x+y*img->w)*2, (char*)&cl, 2);
		}
	}
	if(del)
		deleteIMGHDR(img);
	return img2;
}



