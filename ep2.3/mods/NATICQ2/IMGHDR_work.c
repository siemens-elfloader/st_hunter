#include "../inc/swilib.h"

// compressing bitmap of IMGHDR struct

void PressImg(IMGHDR *imgsrc, char freemap) // freemap=1 if need mfree(imgsrc->bitmap)
{
  // проверка
	if(!imgsrc) return;
	if(!imgsrc->bitmap) return;
	char c=0;
	switch(imgsrc->bpnum) // формат изображения:
	{
	 case 5: c=1; break; // 8-битный
	 case 8: c=2; break; // 16-битный
	 case 0xA: c=4; break; // 32-битный
	 default: return; // уже сжатый или другой, прерывание функции
	}
	imgsrc->bpnum+=0x80;

  // само сжатие
	int cps=0, tsz;
	unsigned int tc[2];
	unsigned int tmpBufsz=0;
	char *tmpBuf=malloc(imgsrc->h*imgsrc->w*(c+1)); // временный буфер 
	zeromem(tmpBuf, imgsrc->h*imgsrc->w*(c+1));
	while(cps<imgsrc->h*imgsrc->w)
	{
		tsz=0;
		tc[0]=0;
		tc[1]=0;
		memcpy(&tc[0], imgsrc->bitmap+cps*c, c);
		memcpy(&tc[1], imgsrc->bitmap+cps*c+c, c);
		if((tc[0]==tc[1]))
		{
			do
			{
				tsz++;
				tc[0]=0;
				tc[1]=0;
				memcpy(&tc[0], imgsrc->bitmap+(tsz+cps)*c, c);
				memcpy(&tc[1], imgsrc->bitmap+(tsz+cps)*c+c, c);
			}while((tc[0]==tc[1]) && (tsz<127) && ((tsz+cps)<imgsrc->h*imgsrc->w));
			tmpBuf[tmpBufsz]=tsz;
			memcpy(tmpBuf+tmpBufsz+1, imgsrc->bitmap+cps*c, c);
			tmpBufsz+=c+1;
		}
		else
		{
			do
			{
				tsz++;
				tc[0]=0;
				tc[1]=0;
				memcpy(&tc[0], imgsrc->bitmap+(tsz+cps)*c, c);
				memcpy(&tc[1], imgsrc->bitmap+(tsz+cps)*c+c, c);
			}while((tc[0]!=tc[1]) && (tsz<127) && ((tsz+cps)<imgsrc->h*imgsrc->w));
			tmpBuf[tmpBufsz]=0x100-tsz;
			memcpy(tmpBuf+tmpBufsz+1, imgsrc->bitmap+cps*c, tsz*c);
			tmpBufsz+=tsz*c+1;
		}
		cps+=tsz;
	}
	char *buf2=malloc(tmpBufsz);
	memcpy(buf2,tmpBuf,tmpBufsz);
	if(freemap) mfree(imgsrc->bitmap);
	imgsrc->bitmap=buf2;
	mfree(tmpBuf);
}


int drawimg2img(IMGHDR * onse, IMGHDR * timage2, int x, int y, int xRect, int yRect,int xxRect, int yyRect)
{
 
  if (!onse) return 1;
  if (yRect<0 || yRect>=onse->h) yRect = 0;
  if (xRect<0 || xRect>=onse->w) xRect = 0;
  if (yyRect==0 || (yRect+yyRect >= onse->h)) yyRect = onse->h-yRect; 
  if (xxRect==0 || (xRect+xxRect >= onse->w)) xxRect = onse->w-xRect; 
  int *scr = (int*)((char*)timage2->bitmap);
  int *pic = (int*)((char*)onse->bitmap);
  int i,j;

  for (i = xRect; i < xRect+xxRect; i++)
  {
    for(j = yRect; j < yRect+yyRect; j++)
    {
      if (((y+j-yRect)<timage2->h) && ((x+i-xRect)<timage2->w) && ((y+j-yRect)>=0) && ((x+i-xRect)>=0) 
          &&(i>=xRect)&&(j>=yRect)&&(i<xRect+xxRect)&&(j<yRect+yyRect))
       {   
         scr[(y+j-yRect)*timage2->w+i+x-xRect] = pic[(j)*onse->w+i];
       }
    }
  }
 return 0;
}


