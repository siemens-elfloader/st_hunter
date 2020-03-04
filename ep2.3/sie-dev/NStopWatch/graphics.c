#include "../inc/swilib.h"
#include "graphics.h"
#include "write_read.h"

IMGHDR *IMG[IMG_TOTAL];

void Graphics_Init(int *load_status)
{
	extern const char CFG_PATH_SKIN[128];
	
	const char *names[]={"bg", "start_pressed", "start_unpressed", "reset_pressed", "reset_unpressed", "split_pressed",
		"split_unpressed", "stop_pressed", "stop_unpressed"};
	const int len=(strlen(CFG_PATH_SKIN))+30;
	char *fname=malloc(len);
	
	for(int i=0; i<IMG_TOTAL; i++, *load_status+=1)
	{
		sprintf(fname, "%sImg\\%s.png", CFG_PATH_SKIN, names[i]);
		IMG[i]=CreateIMGHDRFromPngFile(fname, 0);
	}
	mfree(fname);
	REDRAW();
}

void Graphics_Free()
{
	for(int i=0; i<IMG_TOTAL; i++)
	{
		if(IMG[i])
		{
			mfree(IMG[i]->bitmap);
			mfree(IMG[i]);
		}
	}
}


/*IMGHDR *CreateIMGHDRFromCacheFIle(const char *fname)
{
	IMGHDR *img=NULL;
	FSTATS fs;
	unsigned int err;
	
	if(GetFileStats(fname, &fs, &err)==-1) return NULL;
	
	int fp=fopen(fname, A_ReadOnly+A_BIN, P_READ, &err);
	char *buf=malloc(fs.size+1);
	fread(fp, buf, fs.size, &err);
	fclose(fp, &err);
	
	int bitsize, struct_size=sizeof(IMGHDR);
	img=malloc(struct_size);
	memcpy(img, buf, struct_size);
	bitsize=CalcBitmapSize(img->w, img->h, img->bpnum);
	img->bitmap=malloc(bitsize+1);
	memcpy(img->bitmap, buf+struct_size, bitsize);
	mfree(buf);
	
	return img;
}

int SaveCacheFile(const IMGHDR *img, const char *fname)
{
	unsigned int err;
	
	int fp=fopen(fname, A_WriteOnly+A_Create+A_BIN+A_Append, P_WRITE, &err);
	if(fp==-1) return fp;
	fwrite(fp, img, sizeof(IMGHDR), &err);
	fwrite(fp, img->bitmap, CalcBitmapSize(img->w, img->h, img->bpnum), &err);
	fclose(fp, &err);
			
	return 1;
}

char *SieRleCompress(const IMGHDR *img, unsigned int *out_size)
{
	char *bitmap=NULL;
	
	enum {psize=2};
	const unsigned int bitsize=img->w*img->h*psize;
	char p1[psize], p2[psize];
	unsigned int i=0, wlen=sizeof(short)*2;
	do
	{
		memcpy(p1, img->bitmap+i, psize);
		memcpy(p2, img->bitmap+i+psize, psize);
		
		if(strncmp(p1, p2, psize)==0)//текущие и посл. 2 байта равны
		{
			const int len=psize+1;
			unsigned int j=psize; //число повторений
			while(j<0x7F && i+j*2<bitsize)
			{
				zeromem(p2, psize);
				memcpy(p2, img->bitmap+i+j*2, psize);
				if(strncmp(p1, p2, psize)!=0) break;
				j++;
			}
			wlen+=len;
			bitmap=realloc(bitmap, wlen+1);
			bitmap[wlen-len]=j;
			memcpy(bitmap+wlen-len+1, p1, psize);
			i+=j*2;
		}
		else
		{
			unsigned int j=psize/2;
			while(j<0xFF-0x7F)
			{
				memcpy(p1, img->bitmap+i+j*psize, psize);
				memcpy(p2, img->bitmap+i+j*psize+psize, psize);
				if(strncmp(p1, p2, psize)==0) break;
				j++;
			}
			const int len=j*2+1;
			wlen+=len;
			bitmap=realloc(bitmap, wlen+1);
			bitmap[wlen-len]=0xFF-0x7F+0x01;
			memcpy(bitmap+wlen-len+1, img->bitmap+i, len-1);
			i+=j*2;
		}
	}while(i<bitsize);
	*out_size=wlen;
	return bitmap;
}*/
