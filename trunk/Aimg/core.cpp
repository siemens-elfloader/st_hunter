#include <alib/img.h>

color RGBA(unsigned char R, unsigned char G, unsigned char B, unsigned char A){
	color t={R,G,B,A};
  return t;
}

color BGRA(unsigned char R, unsigned char G, unsigned char B, unsigned char A){
	color t={B,G,R,A};
  return t;
}

/////////////////////////RGB16
unsigned short
AIMG::
GetColor16 (int x, int y){
    if (y<h_ && y>=0 && x>=0 && x<w_){
        unsigned short *bm=(unsigned short*)bitmap_;
        return *(bm + x + y*w_);
    }
    else return a16pixel;
}

void
AIMG::
SetColor16 (int x, int y, unsigned short clr){
    unsigned short *bm=(unsigned short*)bitmap_;
    if (y<h_ && y>=0 && x>=0 && x<w_) *(bm + x + y*w_)=clr;
}

color RGB16_2_RGBA (unsigned short clr){
    color c;
    c.R=(clr&0xF800)>>8;
    c.G=(clr&0x7E0)>>3;
    c.B=(clr&0x1F)<<3;
    if (clr==a16pixel) c.A=0;
    else c.A=0xFF;

    return c;
}

//Вычисление цвета при альфа-канале
color CalcColor (color src,  color dst){
	color clr;

	clr.A=src.A;
	clr.R=(src.R*(0xFF-dst.A)+dst.R*dst.A)/0xFF;
	clr.G=(src.G*(0xFF-dst.A)+dst.G*dst.A)/0xFF;
	clr.B=(src.B*(0xFF-dst.A)+dst.B*dst.A)/0xFF;

	return clr;
}

short CalcColor16 (short bg, color clr){
    char r = (((bg&0xF800)>>8)*(0xFF-clr.A)+clr.R*clr.A)/0xFF;
    char g = (((bg&0x7E0)>>3)*(0xFF-clr.A)+clr.G*clr.A)/0xFF;
    char b = (((bg&0x1F)<<3)*(0xFF-clr.A)+clr.B*clr.A)/0xFF;

    return RGB16(r,g,b);
}


color
AIMG::
GetColor (int x, int y){
    if (bpnum_==T_32COL){
        if (y<h_ && y>=0 && x>=0 && x<w_){
            color *bm=(color*)bitmap_;
            return *(bm + x + y*w_);
        }
        return RGBA(0, 0, 0, 0);
    }
    if (bpnum_==T_16COL){
        unsigned short uclr=GetColor16 (x, y);
        return RGB16_2_RGBA (uclr);
    }

    return RGBA(0, 0, 0, 0);
}

void
AIMG::
SetColor (int x, int y, color clr){
    if (bpnum_==T_32COL){
        color *bm=(color*)bitmap_;
        if (y<h_ && y>=0 && x>=0 && x<w_){
            if (y>=0 && y<h_){
                if (clr.A==0xFF) *(bm + x + y*w_)=clr;
                else{
                    color src=GetColor (x, y);
                    if (src.A==0) *(bm + x + y*w_)=clr;
                    else{
                        color res=CalcColor (src, clr);
                        *(bm + x + y*w_)=res;
                    }
                }
            }
        }
    }

    if (bpnum_==T_16COL){
        unsigned short *bm=(unsigned short*)bitmap_;
        if (clr.A>0){
            if (clr.A==0xFF) *(bm+ x + y*w_)=RGB16(clr.R, clr.G, clr.B);
            else{
                *(bm+ x + y*w_)=CalcColor16 (*(bm+ x + y*w_), clr);
            }
        }

    }
}

int GetSizeByType (int bpnum){
    if (bpnum==T_32COL) return sizeof(color); //RGBA
	if (bpnum==T_16COL) return 2;	//RGB16

	return 0;
}

color
AIMG::
GetColorByIndex (int index){

    if (index < 0 || index >= w_*h_) return RGBA(0, 0, 0, 0);

    if (bpnum_ == T_16COL){
        return  RGB16_2_RGBA(((unsigned short*)(bitmap_))[index]);
    }
    else{
        if(bpnum_ == T_32COL){
            return ((color*)(bitmap_))[index];
        }
    }

    return RGBA(0, 0, 0, 0);
}


bool
AIMG::
Create (int w, int h, int bpnum){
    w_=w;
	h_=h;
	bpnum_=bpnum;

	bitmap_=new unsigned char [h*w*GetSizeByType (bpnum)];

    if (!bitmap_) return 0;

	return 1;
}

int
AIMG::
Fork (AIMG *dst){
    if (!dst->Create (w_, h_, bpnum_)) return -1;

    memcpy(dst->bitmap_, bitmap_, w_*h_*GetSizeByType(bpnum_));
    return 0;

}

void
AIMG::Fill (color clr){
    for (int i=0; i<w_; i++)
        for (int j=0; j<h_; j++) SetColor (i, j, clr);
}

void
AIMG::
Clean(){

    if (bpnum_==T_32COL){
        color *bm=(color*)bitmap_;
        for (int i=0; i<w_; i++)
            for (int j=0; j<h_; j++)
                *(bm + i + j*w_)=RGBA(0,0,0,0);
    }

    if (bpnum_==T_16COL){
        unsigned short *bm=(unsigned short*)bitmap_;
        for (int i=0; i<w_; i++)
            for (int j=0; j<h_; j++)
                *(bm + i + j*w_)=a16pixel;
    }
}
