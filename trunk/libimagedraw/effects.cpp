#include <alib/img.h>
#include <math.h>


int
Image::
Invert (){
    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            for (int j=0; j<h_; j++){
                for (int i=0; i<w_; i++){
                    color32_t c=*(bm + i + j*w_);
                    *(bm + i + j*w_)=invert_color_rgb32 (c);
                }
            }
        }break;
        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            for (int j=0; j<h_; j++){
                for (int i=0; i<w_; i++){
                    color16_t c=*(bm + i + j*w_);
                    *(bm + i + j*w_)=invert_color_rgb16 (c);
                }
            }
        }break;

        default: return ERR_UNSUPPORTED_BITMAP_TYPE;
    }

	return 0;
}

int
Image::
GrayScale (){
    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            for (int j=0; j<h_; j++){
                for (int i=0; i<w_; i++){
                    color32_t c=*(bm + i + j*w_);
                    *(bm + i + j*w_)=conv_to_gray_rgb32 (c);
                }
            }
        }break;
        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            for (int j=0; j<h_; j++){
                for (int i=0; i<w_; i++){
                    color16_t c=*(bm + i + j*w_);
                    *(bm + i + j*w_)=conv_to_gray_rgb16 (c);
                }
            }
        }break;

        default: return ERR_UNSUPPORTED_BITMAP_TYPE;
    }

	return 0;
}

void
Image::
SetAlphaLevel (uint8_t alpha){
    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        for (int i=0; i<w_; i++){
            for (int j=0; j<h_; j++){
                color32_t clr=GetColor (i, j);
                color sclr;
                rgb32_color (clr, &sclr);
                if (sclr.A){
                    sclr.A=alpha;
                    *(bm + i + j*w_)=rgb_rgb32 (sclr.R, sclr.G, sclr.B, sclr.A);
                }
            }
        }
    }
}
