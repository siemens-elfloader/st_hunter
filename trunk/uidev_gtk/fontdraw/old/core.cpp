#include <alib\img.h>

color16_t
AIMG::
GetColor16 (int x, int y){
    if (y<h_ && y>=0 && x>=0 && x<w_){
        color16_t *bm=(color16_t*)bitmap_;
        return *(bm + x + y*w_);
    }
    else return a16pixel;
}

void
AIMG::
SetColor16 (int x, int y, color16_t clr){
    color16_t *bm=(color16_t*)bitmap_;
    if (y<h_ && y>=0 && x>=0 && x<w_) *(bm + x + y*w_)=clr;
}

//Вычисление цвета при альфа-канале
color32_t CalcColor (color32_t src,  color32_t dst){
	uint8_t a_dst=GetA(dst);
	uint8_t a_dst2=0xFF-a_dst;

	uint8_t a=GetA(src);
	uint8_t r=((GetR(src))*a_dst2+(GetR(dst))*a_dst)>> 8;
	uint8_t g=((GetG(src))*a_dst2+(GetG(dst))*a_dst)>> 8;
	uint8_t b=((GetB(src))*a_dst2+(GetB(dst))*a_dst)>> 8;

	return rgb_rgb32 (r, g, b, a);
}

color16_t CalcColor16 (color16_t bg, color32_t dst){
    uint8_t a_dst=GetA(dst);
	uint8_t a_dst2=0xFF-a_dst;

    uint8_t r = (((bg&0xF800)>>8)*(a_dst2)+(GetR(dst))*a_dst)>> 8;
    uint8_t g = (((bg&0x7E0)>>3)*(a_dst2)+(GetG(dst))*a_dst)>> 8;
    uint8_t b = (((bg&0x1F)<<3)*(a_dst2)+(GetB(dst))*a_dst)>> 8;

    return rgb_rgb16(r,g,b);
}


color32_t
AIMG::
GetColor (int x, int y){
    if (bpnum_==T_32COL){
        if (y<h_ && y>=0 && x>=0 && x<w_){
            color32_t *bm=(color32_t*)bitmap_;
            return *(bm + x + y*w_);
        }
        return 0;
    }
    if (bpnum_==T_16COL){
        if (y<h_ && y>=0 && x>=0 && x<w_){
            color16_t *bm=(color16_t*)bitmap_;
            return rgb16_rgb32 (*(bm + x + y*w_));
        }
        //return rgb16_rgb32 (GetColor16 (x, y));
    }
    return 0;//rgb16_rgb32(a16pixel);
}

color
AIMG::
GetColorS (int x, int y){
    color32_t clr=GetColor (x, y);
    color sclr;
    rgb32_color (clr, &sclr);
    return sclr;
}

void
AIMG::
SetColor (int x, int y, color32_t clr){
    uint8_t clrA=(clr >> 24) & 0xFF;

    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        if (y<h_ && y>=0 && x>=0 && x<w_){
            if (y>=0 && y<h_){
                if (clrA==0xFF) *(bm + x + y*w_)=clr;
                else{
                    color32_t src=GetColor (x, y);
                    uint8_t srcA=(src >> 24) & 0xFF;
                    if (srcA==0) *(bm + x + y*w_)=clr;
                    else{
                        *(bm + x + y*w_)=CalcColor (src, clr);
                    }
                }
            }
        }
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        if (clrA>0){
            if (clrA==0xFF) *(bm+ x + y*w_)=rgb32_rgb16(clr);
            else{
                *(bm+ x + y*w_)=CalcColor16 (*(bm+ x + y*w_), clr);
            }
        }

    }
}

int GetPixelSize (int bpnum){
    if (bpnum==T_32COL) return sizeof(color32_t);
	if (bpnum==T_16COL) return sizeof(color16_t);
	return 0;
}

color32_t
AIMG::
GetColorByIndex (int index){

    if (index < 0 || index >= w_*h_) return 0;

    if (bpnum_ == T_16COL){
        return  rgb16_rgb32(((color16_t*)(bitmap_))[index]);
    }
    else{
        if(bpnum_ == T_32COL){
            return ((color32_t*)(bitmap_))[index];
        }
    }

    return 0;
}


bool
AIMG::
Create (int w, int h, int bpnum){
    w_=w;
	h_=h;
	bpnum_=bpnum;

	bitmap_=new unsigned char [h*w*GetPixelSize(bpnum)];

    if (!bitmap_) return 0;

	return 1;
}

int
AIMG::
Fork (AIMG *dst){
    if (!dst->Create (w_, h_, bpnum_)) return -1;

    memcpy(dst->bitmap_, bitmap_, w_*h_*GetPixelSize(bpnum_));
    return 0;
}

void
AIMG::Fill (color32_t clr){
    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        for (int j=0; j<h_; j++){
            int line=j*w_;
            for (int i=0; i<w_; i++)
                *(bm + i + line)=clr;
        }
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        for (int j=0; j<h_; j++){
            int line=j*w_;
            for (int i=0; i<w_; i++)
                *(bm + i + line)=rgb32_rgb16 (clr);
        }
    }
}

void
AIMG::
Clean(){
    //int size=w_*h_*GetPixelSize(bpnum_);
    //memset (bitmap_, 0, size);

    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        for (int j=0; j<h_; j++)
            for (int i=0; i<w_; i++)
                *(bm + i + j*w_)=0;
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        for (int j=0; j<h_; j++)
            for (int i=0; i<w_; i++)
                *(bm + i + j*w_)=a16pixel;
    }

}
