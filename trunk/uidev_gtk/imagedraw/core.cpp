#include <alib/img.h>

color16_t
Image::
GetColor16 (int x, int y){
    if (y<h_ && y>=0 && x>=0 && x<w_){
        color16_t *bm=(color16_t*)bitmap_;
        return *(bm + x + y*w_);
    }
    else return T16_PIXEL;
}

void
Image::
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
color32_t invert_color_rgb32 (color32_t clr){
    //use mask 0xFFFFFF00 //return 0xFFFFFFFF-clr;

    uint8_t r=(GetR(clr));
	uint8_t g=(GetG(clr));
	uint8_t b=(GetB(clr));
	uint8_t a=(GetA(clr));

	return rgb_rgb32 ((0xFF-r), (0xFF-g), (0xFF-b), a);
}

color32_t conv_to_gray_rgb32 (color32_t clr){
    uint8_t r=(GetR(clr));
	uint8_t g=(GetG(clr));
	uint8_t b=(GetB(clr));
	uint8_t a=(GetA(clr));

	uint8_t gray=0.21*r+0.72*g+0.07*b;

    return rgb_rgb32 (gray,  gray,  gray, a);
}

color16_t CalcColor16 (color16_t src, color32_t dst){
    uint8_t a_dst=GetA(dst);
	uint8_t a_dst2=0xFF-a_dst;

    uint8_t r = (((src&0xF800)>>8)*(a_dst2)+(GetR(dst))*a_dst)>> 8;
    uint8_t g = (((src&0x7E0)>>3)*(a_dst2)+(GetG(dst))*a_dst)>> 8;
    uint8_t b = (((src&0x1F)<<3)*(a_dst2)+(GetB(dst))*a_dst)>> 8;

    return rgb_rgb16(r,g,b);
}

color16_t invert_color_rgb16 (color16_t clr){
    uint8_t r=((clr&0xF800)>>8);
	uint8_t g=((clr&0x7E0)>>3);
	uint8_t b=((clr&0x1F)<<3);

    return rgb_rgb16 ((0xFF-r), (0xFF-g), (0xFF-b));
}

color16_t conv_to_gray_rgb16 (color16_t clr){
    uint8_t r=((clr&0xF800)>>8);
	uint8_t g=((clr&0x7E0)>>3);
	uint8_t b=((clr&0x1F)<<3);

	uint8_t gray=0.21*r+0.72*g+0.07*b;

    return rgb_rgb16 (gray,  gray,  gray);
}

color32_t
Image::
GetColor (int x, int y){
    switch (bpnum_){
        case T_32COL:{
            if (y<h_ && y>=0 && x>=0 && x<w_){
                color32_t *bm=(color32_t*)bitmap_;
                return *(bm + x + y*w_);
            }
            else return 0;
        }break;

        case T_16COL:{
            if (y<h_ && y>=0 && x>=0 && x<w_){
                color16_t *bm=(color16_t*)bitmap_;
                return rgb16_rgb32 (*(bm + x + y*w_));
            }
            else return 0;
        }break;

        default: return 0; //ERR_UNSUPPORTED_BITMAP_TYPE;
    }
}

color
Image::
GetColorS (int x, int y){
    color32_t clr=GetColor (x, y);
    color sclr;
    rgb32_color (clr, &sclr);
    return sclr;
}

void
Image::
SetColor (int x, int y, color32_t clr){
    uint8_t clrA=(GetA(clr));

    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            if (y<h_ && y>=0 && x>=0 && x<w_){
                if (clrA==0xFF) *(bm + x + y*w_)=clr;
                else{
                    color32_t src=GetColor (x, y);
                    uint8_t srcA=(GetA(src));
                    if (srcA==0) *(bm + x + y*w_)=clr;
                    else{
                        *(bm + x + y*w_)=CalcColor (src, clr);
                    }
                }
            }
        }break;

        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            if (y<h_ && y>=0 && x>=0 && x<w_){
                if (clrA>0){
                    if (clrA==0xFF) *(bm+ x + y*w_)=rgb32_rgb16(clr);
                    else{
                        *(bm+ x + y*w_)=CalcColor16 (*(bm+ x + y*w_), clr);
                    }
                }
            }
        }break;
    }
}

int GetPixelSize (int bpnum){
    switch (bpnum){
        case T_32COL: return sizeof(color32_t); break;
        case T_16COL: return sizeof(color16_t); break;
        default: return 0;
    }
}

color32_t
Image::
GetColorByIndex (int index){
    if (index < 0 || index >= w_*h_) return 0;

    switch (bpnum_){
        case T_32COL: return ((color32_t*)(bitmap_))[index]; break;
        case T_16COL: return rgb16_rgb32(((color16_t*)(bitmap_))[index]); break;
        default: return 0;
    }
}

bool
Image::
Create (int w, int h, int bpnum){
    w_=w;
	h_=h;
	bpnum_=bpnum;

	bitmap_=new unsigned char [h*w*GetPixelSize(bpnum)];

    if (!bitmap_) return ERR_ALLOC_BITMAP;

	return 0;
}

int
Image::
Fork (Image *dst){
    if (dst->Create (w_, h_, bpnum_)) return -1;

    memcpy(dst->bitmap_, bitmap_, w_*h_*GetPixelSize(bpnum_));
    return 0;
}

void
Image::Fill (color32_t clr){
    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            for (int j=0; j<h_; j++){
                int line=j*w_;
                for (int i=0; i<w_; i++)
                    *(bm + i + line)=clr;
            }
        }break;

        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            for (int j=0; j<h_; j++){
                int line=j*w_;
                for (int i=0; i<w_; i++)
                    *(bm + i + line)=rgb32_rgb16 (clr);
            }
        }break;
    }
}

void
Image::
Clean(){
    //int size=w_*h_*GetPixelSize(bpnum_);
    //memset (bitmap_, 0, size);

    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            for (int j=0; j<h_; j++)
                for (int i=0; i<w_; i++) *(bm + i + j*w_)=0;
        }break;

        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            for (int j=0; j<h_; j++)
                for (int i=0; i<w_; i++) *(bm + i + j*w_)=T16_PIXEL;
        }break;
    }
}
