
// swilib.c
#include <swilib.h>
#include <stdlib.h>
#include <stdio.h>
#include "Cbnlib.h"

//#define HIGHCOLOR // 16-������ �����
//#define GRAPH  // �������
//#define UNPACK // ���������� �������
//#define NO7Z   // ����� 7z
//#define NOZIP  // ����� zip
//#define NORAR  // ����� rar
//#define NOLOAD // in loadgraph data loaded allready

//#ifdef ELKA
//  #define width 240
//  #define height 304
//#else
//  #define width 132
//  #define height 176
//#endif


unsigned int err=0;
long loadfilesize=0;
int FRWBuf=32768;


int fread32(int fh, void *buf, int len, unsigned int *err){
  int clen, rlen, total=0;
  while(len){
  if (len>FRWBuf) clen=FRWBuf; else clen=len;
  total+=(rlen=_read(fh, buf, clen, err));
  if (rlen!=clen) break;
  buf=(char*)buf+rlen;
  len-=clen;
  }return(total);
}

int fwrite32(int fh, void *buf, int len, unsigned int *err){
  int clen, rlen, total=0;
  while(len){
  if (len>FRWBuf) clen=FRWBuf; else clen=len;
  total+=(rlen=_write(fh, buf, clen, err));
  if (rlen!=clen) break;
  buf=(char*)buf+rlen;
  len-=clen;
  }return(total);
}

//void *memset(void *mem, int val, int size){
//  char *s=(char*)mem;
//  while(--size>=0) *s++=(char)val;
//  return mem;
//}

//void *memsetw(void *mem, int val, int size){
//  short *s=(short*)mem;
//  while(--size>=0) *s++=val;
//  return mem;
//}





//void *memcpyrev(void *to, void *from, int size){
//  char *s=(char*)to+size, *ss=(char*)from+size;
//  while(--size>=0)
//    *--s=*--ss;
//  return to;
//}

int savefile(char *fname, char *buf, int size){
  int f,i;
  if((f=_open(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))==-1) return 0;
  i=fwrite32(f,buf,size,&err);
  _close(f,&err);
  return i;
}

char *loadfile(char *filename){ // ��������� ����, ������-��
  int f; unsigned int err;  char *buf=0;
  if((f=_open(filename,A_ReadOnly+A_BIN,P_READ,&err))!=-1){
    if((loadfilesize=_lseek(f,0,SEEK_END,&err,&err))){
      buf=(char*)malloc(loadfilesize);
      _lseek(f,0,0,&err,&err);
      if(fread32(f,buf,loadfilesize,&err)!=loadfilesize){ mfree(buf); buf=0;}
    } _close(f,&err);
#ifdef UNPACK
    extern char *un7z(char *buf);
    extern char *unrar(char *buf);
    extern char *unzip(char *buf);
    if(buf && loadfilesize>4){
#ifndef NORAR
      if(*(int *)buf==0x21726152) buf=unrar(buf); //unrar
      else
#endif
#ifndef NOZIP
      if(*(int*)buf==0x4034b50) buf=unzip(buf); //unzip
      else
#endif
#ifndef NO7Z
      if(*(short*)buf==0x7A37) buf=un7z(buf); //un7z
#endif
     ;
    }
#endif
  } return buf;
}

//void *memset(void *buf, int val, int size){
//  char *b=(char*)buf;
//  while(--size>=0) *b++=val;
//  return buf;
//}

char *toup(char *str){			// � �������� ��������
  char *val=str;
  while(*val!=0){
         if(*val>='a' && *val<='z')  *val-=' ';
    else if(*val>='�' && *val<='�')  *val-=' ';
    else if(*val>='�' && *val<='�')  *val-= 80;
    ++val;
  }
  return str;
}

char *todown(char *str){		// � ������� ��������
  char *val=str;
  while(*val!=0){
         if(*val>='A' && *val<='Z')  *val+=' ';
    else if(*val>='�' && *val<='�')  *val+=' ';
    else if(*val>='�' && *val<='�')  *val+= 80;
    ++val;
  }
  return str;
}

int  isalth (unsigned char sym){	// ���������� ������ ?
  return ((sym>='A' && sym<='Z') || (sym>=(unsigned char)'�' && sym<=(unsigned char)'�')
    || (sym>=(unsigned char)'�' && sym<=(unsigned char)'�'));}

int  isval (char sym){		// ������ �������� ������ ?
  return (sym>='0' && sym<='9');}

int strchrpos(const char *s, int c){	// ������� ������� � ������ (1-N, 0=���)
  char *sptr=strchr(s,c);
  return (sptr)?sptr-s+1:0;
}

int emptystr(char *str){		// ������ �� ������
  while(*str && (*str==' ' || *str==9)) ++str;
  return ((*str)?0:1);
}

char  *alltrim (char *str){		// ������� ������� �� ����� ������
  char *s;
  if(!*str) return str;
  while(*str==' ' || *str==9) ++str;
  s=str;
  while(*s) ++s;
  while(s>str && (*(s-1)==' ' || *(s-1)==9)) --s;
  *s=0;
  return str;
}

int   strcmpsize (const char *s1,const char *s2,int size){ // �������� ������ ����� size
  int i;
  for(i=0;i<size && s1[i]==s2[i];i++);
    return (i==size)?0:s1[i]-s2[i];
}

int getint(void *ptr){ // �������� int
  if((int)ptr & 3) return *(unsigned char*)ptr | *((unsigned char*)ptr+1)<<8\
    | *((unsigned char*)ptr+2)<<16 | *((unsigned char*)ptr+3)<<24;
  else return *(int*)ptr;
}

int get3int(void *ptr){ // �������� 3int
  return *(unsigned char*)ptr | *((unsigned char*)ptr+1)<<8 | *((unsigned char*)ptr+2)<<16;
}

short getshort(void *ptr){ // �������� short
  if((int)ptr & 1) return *(unsigned char*)ptr | *((unsigned char*)ptr+1)<<8;
  else return *(short*)ptr;
}

void putint(void *ptr, int data){ // �������� int
  if((int)ptr & 3){ *(char*)ptr=data; *((char*)ptr+1)=data>>8;
    *((char*)ptr+2)=data>>16; *((char*)ptr+3)=data>>24;
  }else *(int*)ptr=data;

}

void putshort(void *ptr, short data){ // �������� short
  if((int)ptr & 1){ *(char*)ptr=data; *((char*)ptr+1)=data>>8;
  }else *(short*)ptr=data;
}

#ifdef GRAPH
/*-----------------------------
����.	������	���
00	2	'BM'
02	4	������ ����� � ������;
06	2	0
08	2	0
10	4	�������� ������ ����������� � �����;
14	4	������ ����� ���������, 40;
18	4	������ ����������� � ��������;
22	4	������ ����������� � ��������;
26	2	����� ���������� �����������, 1;
28	2	��� �� ������: 1,4,8 ��� 24;
30	4	��� ������;
34	4	������ ������� ����������� � ������ ��� 0;
38	4	�������������� ����������, � �������� / �� ����;
42	4	������������ ����������, � �������� / �� ����;
46	4	���������� ������������ ������;
50	4	����� "������" ������;
54	4*N	����� ������ BGR�;

BMP	��������� 54 � + ������� 4*256;*/

inline unsigned char rgb888tobyte(int color){
  return ((color>>6)&3)|((color>>11)&0x1C)|((color>>16)&0xE0); }

inline short rgb888toshort(int color){
  return ((color>>3)&0x1f)|((color>>5)&0x7e0)|((color>>8)&0xf800); }

inline unsigned char rgb565tobyte(short color){
  return ((color>>3)&3)|((color>>6)&0x1C)|((color>>8)&0xE0); }

inline short rgb565toshort(short color){
  return color; }


int loadgraphsize=0;
#ifndef HIGHCOLOR
  const int graphpoints=1;
#else
  const int graphpoints=2;
#endif
// ������ ����� 2� � 2� �  ����� ����� (��������� ��� ����, ��� ������������)
char *loadgraph(char *filename){ // ��������� ������� (������������� � ������������) #define HIGHCOLOR ������
  unsigned char *bmp=0, *buf=0, *pbuf, *pbmp, *ptr;
  int *pal;
  int sizex, sizey, bytepixel, pitch;
  int i,j;
  do{
    if(!(bmp=(unsigned char*)
#ifndef NOLOAD
    loadfile(filename)
#else
    filename
#endif
    )) break;
    if(getshort(bmp)!='MB') break;
    sizex=getint(bmp+18);
    sizey=getint(bmp+22);
    bytepixel=getshort(bmp+28)>>3;
    if(!sizex || !sizey || bytepixel<1 || bytepixel>4) break;
    pitch=sizex*bytepixel;
    if(pitch&3) pitch+=4-(pitch&3);
    if(!(buf=(unsigned char*)malloc(loadgraphsize=sizex*sizey*graphpoints+4))) break;
    *(short*)buf=sizex; *(short*)(buf+2)=sizey;
    pbuf=buf+4;
    ptr=bmp+getint(bmp+10)+sizey*pitch;
#ifndef HIGHCOLOR //1 byte
    switch(bytepixel){
    case 1: //8 -> 8
      memcpy(pal=(int*)(bmp+52),bmp+54,*(short*)(bmp+50)?(*(short*)(bmp+50)<<2):1024);
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *pbuf++=rgb888tobyte(pal[*pbmp++]);
    }  } break;
    case 2: //16 bit
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *pbuf++=rgb565tobyte(*(short*)pbmp);
        pbmp+=2;
    }  } break;
    case 3: //24 bit
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *pbuf++=rgb888tobyte(get3int(pbmp));
        pbmp+=3;
    }  } break;
    case 4: //32 bit
    if((i=getint(bmp+10))&3){ //��������� ������ �� ������� 4
      j=i-(i&3);
      memcpy(bmp+j, bmp+i, sizey*pitch);
      ptr=bmp+j+sizey*pitch;
    }
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *pbuf++=rgb888tobyte(*(int*)pbmp);
        pbmp+=4;
    }  } break;
 #else    // 2 byte
    switch(bytepixel){
    case 1: //8 -> 16
    memcpy(pal=(int*)(bmp+52),bmp+54,*(short*)(bmp+50)?(*(short*)(bmp+50)<<2):1024);
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *(short*)pbuf=rgb888toshort(pal[*pbmp++]);
        pbuf+=2;
    }  } break;
    case 2: //16 bit
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *(short*)pbuf=rgb565toshort(*(short*)pbmp);
        pbmp+=2;
        pbuf+=2;
    }  } break;
    case 3: //24 bit
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *(short*)pbuf=rgb888toshort(get3int(pbmp));
        pbmp+=3;
        pbuf+=2;
    }  } break;
    case 4: //32 bit
    if((i=getint(bmp+10))&3){ //��������� ������ �� ������� 4
      j=i-(i&3);
      memcpy(bmp+j, bmp+i, sizey*pitch);
      ptr=bmp+j+sizey*pitch;
    }
    for(j=0;j<sizey;j++){
      pbmp=(ptr-=pitch);
      for(i=0;i<sizex;i++){
        *(short*)pbuf=rgb888toshort(*(int*)pbmp);
        pbmp+=4;
        pbuf+=2;
    }  } break;
#endif
    }
    mfree(bmp);
    return (char*)buf;
  }while(0);
  if(bmp) mfree(bmp);
  if(buf) mfree(buf);
  return 0;
}
#endif

// ������� �� ����� � ��� �,� �������� �������� sizex,sizey � ���. � bmp �0,�0 �
// ���������� ������ (����<>0)  �����: 0x40 = ������
void bitblt(void *screen, void *bmp, int x, int y, int sizex, int sizey, int x0, int y0,  int colormask, int flags){
  int i,j, sizexx, bmpx, bmpy;
  // �������� �� ������
  int width=ScreenW(),height=ScreenH();

  if(!screen || !bmp || x>=width || y>=height) return;
  bmpx=*(short*)bmp;
  bmpy=*(short*)((char*)bmp+2);
  if(!sizex) sizex=bmpx;
  if(!sizey) sizey=bmpy;

  if(x<0){ sizex+=x; x0-=x; x=0;}
  if(y<0){ sizey+=y0; y0-=y; y=0;}
  if(x0<0 || x0>=bmpx || y0<0 || y0>=bmpy || sizex<=0 || sizey<=0) return;

  if(x+sizex>=width) sizex=width-x;
  if(x0+sizex>=bmpx) sizex=bmpx-x0;
  if(y+sizey>=height) sizey=height-y;
  if(y0+sizey>=bmpy) sizey=bmpy-y0;

#ifndef HIGHCOLOR
  char *scr=(char*)screen, *pic=(char*)bmp+4;
  sizexx=sizex;
#else
  short *scr=(short*)screen, *pic=(short*)((char*)bmp+4);
  sizexx=sizex<<1;
#endif
  scr+=width*y+x;
  pic+=bmpx*y0 + x0;
  if(colormask || (flags&0x40)){
    for(j=0; j<sizey; j++, scr+=width-sizex, pic+=bmpx-sizex)
      for(i=0; i<sizex; i++, scr++, pic++)
        if(*pic!=
#ifndef HIGHCOLOR
  (char)
#else
 (short)
#endif
   colormask) *scr=*pic;
  }else{
    for(j=0; j<sizey; j++, scr+=width, pic+=bmpx)
      memcpy(scr,pic,sizexx);
  }
}
