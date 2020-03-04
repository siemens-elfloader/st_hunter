#include <alib/img.h>
#include <math.h>


#define matrix_transform(w, h, x, y, type) int mx, my;\
    switch(type){\
        case FLIP_H:\
        mx = w-1-x;\
        my = y;\
        break;\
\
        case FLIP_V:\
        mx = x;\
        my = h-1-y;\
        break;\
\
        case ROT180:\
        mx = w-1-x;\
        my = h-1-y;\
        break;\
\
        case ROT90:\
        mx = h-1-y;\
        my = x;\
        break;\
\
        case ROT270:\
        mx = y;\
        my = w-1-x;\
        break;\
\
        default:{\
            mx = x;\
            my = y;\
        }\
    }\

/*

32
buf [j*w*4 + 4*i+0]
buf [j*w*4 + 4*i+1]
buf [j*w*4 + 4*i+2]
buf [j*w*4 + 4*i+3]
*/
int
Image::
_transform_img (int type){
    int nw=w_;
    int nh=h_;

    if (type==FLIP_H || type==ROT180) nw=nw>>1;
    else if (type==FLIP_V) nh=nh>>1;

    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            for (int j=0; j<nh; j++){
                for (int i=0; i<nw; i++){
                    matrix_transform (w_, h_, i, j, type);
                    color32_t left=*(bm + i + j*w_);
                    color32_t right=*(bm + (mx) + (my)*w_);
                    *(bm + i + j*w_)=right;
                    *(bm + (mx) + (my)*w_)=left;
                }
            }
        }break;
        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            for (int j=0; j<nh; j++){
                for (int i=0; i<nw; i++){
                    matrix_transform (w_, h_, i, j, type);
                    color16_t left=*(bm + i + j*w_);
                    color16_t right=*(bm + (mx) + (my)*w_);
                    *(bm + i + j*w_)=right;
                    *(bm + (mx) + (my)*w_)=left;
                }
            }
        }break;

        default: return ERR_UNSUPPORTED_BITMAP_TYPE;
    }

	return 0;
}

void
Image::
FlipHoriz (){ _transform_img (FLIP_H);}


void
Image::
FlipVertic (){ _transform_img (FLIP_V);}

int
Image::
_transform_to_new_img (int type){
	Image *img=new Image (h_, w_, bpnum_);

    if (!img->bitmap_){ delete img; return ERR_ALLOC_BITMAP;}

    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            color32_t *d_bm=(color32_t*)img->bitmap_;
            for (int i=0; i<h_; i++){
                for (int j=0; j<w_; j++){
                    matrix_transform (w_, h_, j, i, type);
                    color32_t c=*(bm + j + i*w_);
                    *(d_bm + (mx) + (my)*h_)=c;
                }
            }
        }break;

        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            color16_t *d_bm=(color16_t*)img->bitmap_;
            for (int i=0; i<h_; i++){
                for (int j=0; j<w_; j++){
                    matrix_transform (w_, h_, j, i, type);
                    color16_t c=*(bm + j + i*w_);
                    *(d_bm + (mx) + (my)*h_)=c;
                }
            }
        }break;

        default: return ERR_UNSUPPORTED_BITMAP_TYPE;
    }

    w_=img->w_;
    h_=img->h_;
    delete bitmap_;
    bitmap_=img->bitmap_;
    img->NotReleaseBitmap ();

    delete img;

    return 0;
}

int
Image::
Rotate (int ang){
    switch (ang){
        case 90: return _transform_to_new_img (ROT90); break;
        case 180: return _transform_img (ROT180); break;
        case 270: return _transform_to_new_img (ROT270); break;
    }
	return 0;
}

color16_t interpolate_rgb16 (color32_t c1, color32_t c2, color32_t c3, color32_t c4, float dx, float dy){
    float dy_1=1-dy;
    float dx_1=1-dx;

    float ir1 = (GetR (c1)) * (dy_1) + (GetR (c3)) * dy;
    float ig1 = (GetG (c1)) * (dy_1) + (GetG (c3)) * dy;
    float ib1 = (GetB (c1)) * (dy_1) + (GetB (c3)) * dy;

    float ir2 = (GetR (c2)) * (dy_1) + (GetR (c4)) * dy;
    float ig2 = (GetG (c2)) * (dy_1) + (GetG (c4)) * dy;
    float ib2 = (GetB (c2)) * (dy_1) + (GetB (c4)) * dy;

    uint8_t r = (uint8_t)(ir1 * (dx_1) + ir2 * dx);
    uint8_t g = (uint8_t)(ig1 * (dx_1) + ig2 * dx);
    uint8_t b = (uint8_t)(ib1 * (dx_1) + ib2 * dx);

    return rgb_rgb16 (r, g, b);
}

color32_t interpolate_rgb32 (color32_t c1, color32_t c2, color32_t c3, color32_t c4, float dx, float dy){
    float dy_1=1-dy;
    float dx_1=1-dx;

    float ir1 = (GetR (c1)) * (dy_1) + (GetR (c3)) * dy;
    float ig1 = (GetG (c1)) * (dy_1) + (GetG (c3)) * dy;
    float ib1 = (GetB (c1)) * (dy_1) + (GetB (c3)) * dy;
    float ia1 = (GetA (c1)) * (dy_1) + (GetA (c3)) * dy;

    float ir2 = (GetR (c2)) * (dy_1) + (GetR (c4)) * dy;
    float ig2 = (GetG (c2)) * (dy_1) + (GetG (c4)) * dy;
    float ib2 = (GetB (c2)) * (dy_1) + (GetB (c4)) * dy;
    float ia2 = (GetA (c2)) * (dy_1) + (GetA (c4)) * dy;

    uint8_t r = (uint8_t)(ir1 * (dx_1) + ir2 * dx);
    uint8_t g = (uint8_t)(ig1 * (dx_1) + ig2 * dx);
    uint8_t b = (uint8_t)(ib1 * (dx_1) + ib2 * dx);
    uint8_t a = (uint8_t)(ia1 * (dx_1) + ia2 * dx);

    return rgb_rgb32 (r, g, b, a);
}

#define PI_VAL 3.14159265358979

//Поворот с применением билинейной интерполяции
int
Image::
FRotate (Image *rimg, float angle){

    //normolize angle
    while (angle >= 360) angle -= 360;

	while (angle < 0) angle += 360;

    float alpha = angle*PI_VAL/180;

    int xa = 0;
    int ya = 0;
    int xb = w_-1;
    int yb = h_-1;
    float sin_angle=sin(alpha);
    float cos_angle=cos(alpha);

    int xc = w_>>1;//(xb-xa)/2;
    int yc = h_>>1;//(xb-xa)/2;

    int x = -xc;//xa-xc;
    int y = -yc;//ya-yc;
    int x1 = round(x*cos_angle - y*sin_angle);
    int y1 = round(x*sin_angle + y*cos_angle);
    //x = xa-xc;
    y = yc;//yb-yc;
    int x2 = round(x*cos_angle - y*sin_angle);
    int y2 = round(x*sin_angle + y*cos_angle);
    x = xc;//xb-xc;
    y = -yc;//ya-yc;
    int x3 = round(x*cos_angle - y*sin_angle);
    int y3 = round(x*sin_angle + y*cos_angle);
    x = xc;//xb-xc;
    y = yc;//yb-yc;
    int x4 = round(x*cos_angle - y*sin_angle);
    int y4 = round(x*sin_angle + y*cos_angle);

    int dl = min(min(x1,x2),min(x3,x4));
    int dt = min(min(y1,y2),min(y3,y4));
    int dw = max(max(x1,x2),max(x3,x4)) - dl;
    int dh = max(max(y1,y2),max(y3,y4)) - dt;

    if (rimg->Create (dw, dh, bpnum_)) return ERR_ALLOC_BITMAP;

    float dx, dy;

    switch (bpnum_){
        case T_32COL:{
            color32_t *bm=(color32_t*)bitmap_;
            color32_t *rbm=(color32_t*)rimg->bitmap_;
            for (int j = 0;  j < dh; j++){
                for (int i = 0; i < dw; i++){
                    x = i + dl;
                    y = j + dt;
                    float sx = y*sin_angle + x*cos_angle + xc;
                    float sy = y*cos_angle - x*sin_angle + yc;

                    if ((sx<xa)||(sx>xb)||(sy<ya)||(sy>yb))  *(rbm + i + j*rimg->w_)=0;
                    else{
                        int sl = min(max((int)(sx),0), xb);
                        int sr = min(max(sl+1,0), xb);
                        int st = min(max((int)(sy),0), yb);
                        int sb = min(max(st+1,0), yb);

                        dx=sx-sl;
                        dy=sy-st;

                        color32_t c1=(*(bm + sl + st*w_));
                        color32_t c2=(*(bm + sr + st*w_));
                        color32_t c3=(*(bm + sl + sb*w_));
                        color32_t c4=(*(bm + sr + sb*w_));

                        *(rbm + i + j*rimg->w_)=interpolate_rgb32 (c1, c2, c3, c4, dx, dy);
                    }
                }
            }
        }break;

        case T_16COL:{
            color16_t *bm=(color16_t*)bitmap_;
            color16_t *rbm=(color16_t*)rimg->bitmap_;
            for (int j = 0;  j < dh; j++){
                for (int i = 0; i < dw; i++){
                    x = i + dl;
                    y = j + dt;
                    float sx = y*sin_angle + x*cos_angle + xc;
                    float sy = y*cos_angle - x*sin_angle + yc;

                    if ((sx<xa)||(sx>xb)||(sy<ya)||(sy>yb))  *(rbm + i + j*rimg->w_)=T16_PIXEL;
                    else{
                        int sl = min(max((int)(sx),0), xb);
                        int sr = min(max(sl+1,0), xb);
                        int st = min(max((int)(sy),0), yb);
                        int sb = min(max(st+1,0), yb);

                        dx=sx-sl;
                        dy=sy-st;

                        color32_t c1=rgb16_rgb32 (*(bm + sl + st*w_));
                        color32_t c2=rgb16_rgb32 (*(bm + sr + st*w_));
                        color32_t c3=rgb16_rgb32 (*(bm + sl + sb*w_));
                        color32_t c4=rgb16_rgb32 (*(bm + sr + sb*w_));

                        *(rbm + i + j*rimg->w_)=interpolate_rgb16 (c1, c2, c3, c4, dx, dy);
                    }
                }
            }
        }break;

        default: return ERR_UNSUPPORTED_BITMAP_TYPE;
    }

    return 0;
}

//Масштабирование с применением метода ближайшего соседа
int
Image::
Resize (Image *simg, int num, int denom){

    if (num<1) num=1;
    if (denom<1) denom=1;

    if (num==1 && denom==1) return 1;//Fork (simg);
    else{
        int new_w = w_* num/denom;
        int new_h = h_ * num/denom;

        int nx, ny;

        if (simg->Create (new_w, new_h, bpnum_)) return ERR_ALLOC_BITMAP;

        switch (bpnum_){
            case T_32COL:{
                color32_t *bm=(color32_t*)bitmap_;
                color32_t *sbm=(color32_t*)simg->bitmap_;

                for (int y=0; y < new_h; y++){
                    ny = y * denom/num;
                    for (int x=0; x < new_w; x++){
                        nx = x * denom/num;
                        *(sbm + x + y*new_w)=(*(bm + nx + ny*w_));
                    }
                }
            }break;

            case T_16COL:{
                color16_t *bm=(color16_t*)bitmap_;
                color16_t *sbm=(color16_t*)simg->bitmap_;

                for (int y=0; y < new_h; y++){
                    ny = y * denom/num;
                    for (int x=0; x < new_w; x++){
                        nx = x * denom/num;
                        *(sbm + x + y*new_w)=(*(bm + nx + ny*w_));
                    }
                }
            }break;

            default: return ERR_UNSUPPORTED_BITMAP_TYPE;
        }
    }

    return 0;
}

int
Image::
ResizeWithTransform (Image *simg, int num, int denom, int transform){

    if (num<1) num=1;
    if (denom<1) denom=1;

    if (num==1 && denom==1) return 1;//Fork (simg);
    else{
        int new_w = w_* num/denom;
        int new_h = h_ * num/denom;

        int t_w, t_h;
        int nx, ny;

        if (transform==ROT90 || transform==ROT270){
            if (simg->Create (new_h, new_w, bpnum_)) return ERR_ALLOC_BITMAP;
            t_w=h_;
            t_h=w_;
            //because anti-transform
            if (transform==ROT90) transform=ROT270;
            else transform=ROT90;
        }
        else{
            if (simg->Create (new_w, new_h, bpnum_)) return ERR_ALLOC_BITMAP;
            t_w=w_;
            t_h=h_;
        }

        switch (bpnum_){
            case T_32COL:{
                color32_t *bm=(color32_t*)bitmap_;
                color32_t *sbm=(color32_t*)simg->bitmap_;

                if (transform==ROT90 || transform==ROT270){
                    for (int x=0; x < simg->w_; x++){
                        nx = x * denom/num;
                        for (int y=0; y < simg->h_; y++){
                            ny = y * denom/num;
                            matrix_transform(t_w, t_h, nx, ny, transform);
                            if (mx>=0 && my>=0 && my<h_ && mx<w_) *(sbm + x + y*simg->w_)=(*(bm + mx + my*w_));
                        }
                    }
                }else{
                    for (int y=0; y < simg->h_; y++){
                        ny = y * denom/num;
                        for (int x=0; x < simg->w_; x++){
                            nx = x * denom/num;
                            matrix_transform(t_w, t_h, nx, ny, transform);
                            if (mx>=0 && my>=0 && my<h_ && mx<w_) *(sbm + x + y*simg->w_)=(*(bm + mx + my*w_));
                        }
                    }
                }
            }break;

            case T_16COL:{
                color16_t *bm=(color16_t*)bitmap_;
                color16_t *sbm=(color16_t*)simg->bitmap_;

                if (transform==ROT90 || transform==ROT270){
                    for (int x=0; x < simg->w_; x++){
                        nx = x * denom/num;
                        for (int y=0; y < simg->h_; y++){
                            ny = y * denom/num;
                            matrix_transform(t_w, t_h, nx, ny, transform);
                            if (mx>=0 && my>=0 && my<h_ && mx<w_) *(sbm + x + y*simg->w_)=(*(bm + mx + my*w_));
                        }
                    }
                }else{
                    for (int y=0; y < simg->h_; y++){
                        ny = y * denom/num;
                        for (int x=0; x < simg->w_; x++){
                            nx = x * denom/num;
                            matrix_transform(t_w, t_h, nx, ny, transform);
                            if (mx>=0 && my>=0 && my<h_ && mx<w_) *(sbm + x + y*simg->w_)=(*(bm + mx + my*w_));
                        }
                    }
                }
            }break;

            default: return ERR_UNSUPPORTED_BITMAP_TYPE;
        }
    }

    return 0;
}


//Масштабирование с применением билинейной интерполяции
int
Image::
ResizeQuality (Image *simg, int num, int denom){

    if (num<1) num=1;
    if (denom<1) denom=1;

    if (num==1 && denom==1) return 1;//Fork (simg);
    else{
        int new_w = w_* num/denom;
        int new_h = h_ * num/denom;

        float fX, fY;

        if (simg->Create (new_w, new_h, bpnum_)) return ERR_ALLOC_BITMAP;

        int ifX, ifY, ifX1, ifY1;
        float dx, dy;

        switch (bpnum_){
            case T_32COL:{
                color32_t *bm=(color32_t*)bitmap_;
                color32_t *sbm=(color32_t*)simg->bitmap_;
                for (int y=0; y<new_h; y++){
                    fY = y * denom/num;
                    ifY = (int)fY;
                    ifY1 = min(h_-1, ifY+1);
                    dy = fY - ifY;
                    for (int x=0; x<new_w; x++){
                        fX = x * denom/num;
                        ifX = (int)fX;
                        ifX1 = min(w_-1, ifX+1);
                        dx = fX - ifX;

                        color32_t c1= (*(bm + ifX + ifY*w_));
                        color32_t c2= (*(bm + ifX1 + ifY*w_));
                        color32_t c3= (*(bm + ifX + ifY1*w_));
                        color32_t c4= (*(bm + ifX1 + ifY1*w_));

                        *(sbm + x + y*simg->w_)=interpolate_rgb32 (c1, c2, c3, c4, dx, dy);
                    }
                }
            }break;
            case T_16COL:{
                color16_t *bm=(color16_t*)bitmap_;
                color16_t *sbm=(color16_t*)simg->bitmap_;
                for (int y=0; y<new_h; y++){
                    fY = y * denom/num;
                    ifY = (int)fY;
                    ifY1 = min(h_-1, ifY+1);
                    dy = fY - ifY;
                    for (int x=0; x<new_w; x++){
                        fX = x * denom/num;
                        ifX = (int)fX;
                        ifX1 = min(w_-1, ifX+1);
                        dx = fX - ifX;

                        color32_t c1=rgb16_rgb32 (*(bm + ifX + ifY*w_));
                        color32_t c2=rgb16_rgb32 (*(bm + ifX1 + ifY*w_));
                        color32_t c3=rgb16_rgb32 (*(bm + ifX + ifY1*w_));
                        color32_t c4=rgb16_rgb32 (*(bm + ifX1 + ifY1*w_));

                        *(sbm + x + y*simg->w_)=interpolate_rgb16 (c1, c2, c3, c4, dx, dy);
                    }
                }
            }break;

            default: return ERR_UNSUPPORTED_BITMAP_TYPE;
        }
    }
    return 0;
}

