#include <alib\img.h>
#include <math.h>

void
AIMG::
FlipHoriz (){
    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        for (int j=0; j<h_; j++){
            int line=j*w_;
            for (int i=0; i<w_>>1; i++){
                color32_t left=*(bm + i + line);
                color32_t right=*(bm + (w_-i-1) + line);
                *(bm + i + line)=right;
                *(bm + (w_-i-1) + line)=left;
		    }
		}
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        for (int j=0; j<h_; j++){
            int line=j*w_;
            for (int i=0; i<w_>>1; i++){
                color16_t left=*(bm + i + line);
                color16_t right=*(bm + (w_-i-1) + line);
                *(bm + i + line)=right;
                *(bm + (w_-i-1) + line)=left;
			}
		}
	}
}

void
AIMG::
FlipVertic (){
    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        for (int j=0; j<h_>>1; j++){
            int line=(h_-j-1)*w_, line2=j*w_;
            for (int i=0; i<w_; i++){
                color32_t up=*(bm + i + line2);
                color32_t  down=*(bm + i + line);
                *(bm + i + line2)=down;
                *(bm + i + line)=up;
		    }
        }
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        for (int j=0; j<h_>>1; j++){
            int line=(h_-j-1)*w_, line2=j*w_;
            for (int i=0; i<w_; i++){
		        color16_t up=*(bm + i + line2);
                color16_t down=*(bm + i + line);
                *(bm + i + line2)=down;
                *(bm + i + line)=up;
		    }
		}
	}
}

int
AIMG::
Rotate90 (){
	AIMG *img=new AIMG (h_, w_, bpnum_);

    if (!bitmap_){ delete img; return -1;}

    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        color32_t *d_bm=(color32_t*)img->bitmap_;
        for (int i=0; i<h_; i++){
            for (int j=0; j<w_; j++){
                color32_t c=*(bm + j + i*w_);
                *(d_bm + (h_-1-i) + j*h_)=c;
            }
        }
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        color16_t *d_bm=(color16_t*)img->bitmap_;
        for (int i=0; i<h_; i++){
            for (int j=0; j<w_; j++){
                color16_t c=*(bm + j + i*w_);
                *(d_bm + (h_-1-i) + j*h_)=c;
            }
        }
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
AIMG::
Rotate270 (){
	AIMG *img=new AIMG (h_, w_, bpnum_);

    if (!bitmap_){ delete img; return -1;}

    if (bpnum_==T_32COL){
        color32_t *bm=(color32_t*)bitmap_;
        color32_t *d_bm=(color32_t*)img->bitmap_;
        for (int i=0; i<h_; i++){
            for (int j=0; j<w_; j++){
                color32_t c=*(bm + j + i*w_);
                *(d_bm + (i) + (w_-j-1)*h_)=c;
            }
        }
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        color16_t *d_bm=(color16_t*)img->bitmap_;
        for (int i=0; i<h_; i++){
            for (int j=0; j<w_; j++){
                color16_t c=*(bm + j + i*w_);
                *(d_bm + (i) + (w_-j-1)*h_)=c;
            }
        }
    }

    w_=img->w_;
    h_=img->h_;
    delete bitmap_;
    bitmap_=img->bitmap_;
    img->NotReleaseBitmap ();

    delete img;

    return 0;
}
/*
int
AIMG::
Rotate90 (){
    unsigned char *nbitmap=NULL;
    if (bpnum_==T_32COL){

    }
    else
    if (bpnum_==T_16COL){
        nbitmap=new unsigned char [w_*h_*2];
        color16_t *bm=(color16_t*)bitmap_;
        color16_t *nbm=(color16_t*)nbitmap;
        for (int i=0; i<h_; i++){
            for (int j=0; j<w_; j++){
                color16_t c=*(bm + j + i*w_);
                *(nbm + (h_-1-i) + j*h_)=c;
            }
        }
    }
    int swap=h_;
    h_=w_;
    w_=swap;

    delete bitmap_;
    bitmap_=nbitmap;

    return 0;
}
*/

int
AIMG::
Rotate (int ang){
    switch (ang){
        case 90: Rotate90 (); break;
        case 180:{
            if (bpnum_==T_32COL){
                color32_t *bm=(color32_t*)bitmap_;
                for (int j=0; j<h_; j++){
                    int line=j*w_, line2=(h_-j-1)*w_;
                    for (int i=0; i<w_>>1; i++){
                        color32_t left=*(bm + i + line);
                        color32_t right=*(bm + (w_-i-1) + line2);
                        *(bm + i + line)=right;
                        *(bm + (w_-i-1) + line2)=left;
                    }
                }
            }
            else
            if (bpnum_==T_16COL){
                color16_t *bm=(color16_t*)bitmap_;
                for (int j=0; j<h_; j++){
                    int line=j*w_, line2=(h_-j-1)*w_;
                    for (int i=0; i<w_>>1; i++){
                        color16_t left=*(bm + i + line);
                        color16_t right=*(bm + (w_-i-1) + line2);
                        *(bm + i + line)=right;
                        *(bm + (w_-i-1) + line2)=left;
                    }
                }
            }
        } break;
        case 270: Rotate270 (); break;
    }

	return 0;
}
/*
int
AIMG::
FRotate (AIMG *rimg, float angle){

    double ang = -angle*acos(0.)/90;
    int newWidth, newHeight;
    int nWidth = w_;
    int nHeight= h_;
    double cos_angle = cos(ang);
    double sin_angle = sin(ang);

    MyPOINT p1={0,0};
    MyPOINT p2={nWidth,0};
    MyPOINT p3={0,nHeight};
    MyPOINT p4={nWidth-1,nHeight};
    MyPOINT newP1,newP2,newP3,newP4, leftTop, rightTop, leftBottom, rightBottom;

    newP1.x = p1.x;
    newP1.y = p1.y;
    newP2.x = (long)(p2.x*cos_angle - p2.y*sin_angle);
    newP2.y = (long)(p2.x*sin_angle + p2.y*cos_angle);
    newP3.x = (long)(p3.x*cos_angle - p3.y*sin_angle);
    newP3.y = (long)(p3.x*sin_angle + p3.y*cos_angle);
    newP4.x = (long)(p4.x*cos_angle - p4.y*sin_angle);
    newP4.y = (long)(p4.x*sin_angle + p4.y*cos_angle);

    leftTop.x = min(min(newP1.x,newP2.x),min(newP3.x,newP4.x));
    leftTop.y = min(min(newP1.y,newP2.y),min(newP3.y,newP4.y));
    rightBottom.x = max(max(newP1.x,newP2.x),max(newP3.x,newP4.x));
    rightBottom.y = max(max(newP1.y,newP2.y),max(newP3.y,newP4.y));
    leftBottom.x = leftTop.x;
    leftBottom.y = rightBottom.y;
    rightTop.x = rightBottom.x;
    rightTop.y = leftTop.y;

    newWidth = rightTop.x - leftTop.x;
    newHeight= leftBottom.y - leftTop.y;

    if (!rimg->Create (newWidth, newHeight, bpnum_)) return -1;

    rimg->Clean ();

    int x,y,newX,newY,oldX,oldY;
    for (y = leftTop.y, newY = 0; y<=leftBottom.y; y++,newY++){
        for (x = leftTop.x, newX = 0; x<=rightTop.x; x++,newX++){
            oldX = (long)(x*cos_angle + y*sin_angle);
            oldY = (long)(y*cos_angle - x*sin_angle);
            rimg->SetColor (newX,newY, GetColor(oldX,oldY));
        }
    }

    return 0;
}*/

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

int
AIMG::
FRotate (AIMG *rimg, float angle){

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

    if (!rimg->Create (dw, dh, bpnum_)) return -1;

    float dx, dy;

    if (bpnum_==T_32COL){
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
    }
    else
    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        color16_t *rbm=(color16_t*)rimg->bitmap_;
        for (int j = 0;  j < dh; j++){
            for (int i = 0; i < dw; i++){
                x = i + dl;
                y = j + dt;
                float sx = y*sin_angle + x*cos_angle + xc;
                float sy = y*cos_angle - x*sin_angle + yc;

                if ((sx<xa)||(sx>xb)||(sy<ya)||(sy>yb))  *(rbm + i + j*rimg->w_)=a16pixel;
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
    }

    return 0;
}

int
AIMG::
Resize (AIMG *simg, int num, int denom){

    if (num<1) num=1;
    if (denom<1) denom=1;

    if (num==1 && denom==1) return 1;//Fork (simg);
    else{
        int new_w = w_* num/denom;
        int new_h = h_ * num/denom;

        int nx, ny;

        if (!simg->Create (new_w, new_h, bpnum_)) return -1;

        if (bpnum_==T_32COL){
            color32_t *bm=(color32_t*)bitmap_;
            color32_t *sbm=(color32_t*)simg->bitmap_;

            for (int y=0; y < new_h; y++){
                ny = min(h_-1, y * denom/num);
                for (int x=0; x < new_w; x++){
                    nx = min(w_-1, x * denom/num);

                    *(sbm + x + y*simg->w_)=(*(bm + nx + ny*w_));
                }
            }
        }else
        if (bpnum_==T_16COL){
            color16_t *bm=(color16_t*)bitmap_;
            color16_t *sbm=(color16_t*)simg->bitmap_;

            for (int y=0; y < new_h; y++){
                ny = min(h_-1, y * denom/num);
                for (int x=0; x < new_w; x++){
                    nx = min(w_-1, x * denom/num);

                    *(sbm + x + y*simg->w_)=(*(bm + nx + ny*w_));
                }
            }
        }
    }

    return 0;
}

int
AIMG::
ResizeQuality (AIMG *simg, int px, int py){

    if (w_==px && h_==py)  return 1;  //original
    else{
        int newx = px;
        int newy = py;

        float fX, fY;
        float xScale = (float)w_  / (float)newx;
        float yScale = (float)h_ / (float)newy;

        if (!simg->Create (newx, newy, bpnum_)) return -1;

        int ifX, ifY, ifX1, ifY1;
        float ir1, ir2, ig1, ig2, ib1, ib2, ia1, ia2, dx, dx_1, dy, dy_1;
        uint8_t r,g,b,a;

        if (bpnum_==T_32COL){
            color32_t *bm=(color32_t*)bitmap_;
            color32_t *sbm=(color32_t*)simg->bitmap_;
            for (int y=0; y<newy; y++){
                fY = y * yScale;
                ifY = (int)fY;
                ifY1 = min(h_-1, ifY+1);
                dy = fY - ifY;
                dy_1=1-dy;
                for (int x=0; x<newx; x++){
                    fX = x * xScale;
                    ifX = (int)fX;
                    ifX1 = min(w_-1, ifX+1);
                    dx = fX - ifX;
                    dx_1=1-dx;

                    color32_t c1= (*(bm + ifX + ifY*w_));
                    color32_t c2= (*(bm + ifX1 + ifY*w_));
                    color32_t c3= (*(bm + ifX + ifY1*w_));
                    color32_t c4= (*(bm + ifX1 + ifY1*w_));

                    ir1 = (GetR (c1)) * (dy_1) + (GetR (c3)) * dy;
                    ig1 = (GetG (c1)) * (dy_1) + (GetG (c3)) * dy;
                    ib1 = (GetB (c1)) * (dy_1) + (GetB (c3)) * dy;
                    ia1 = (GetA (c1)) * (dy_1) + (GetA (c3)) * dy;

                    ir2 = (GetR (c2)) * (dy_1) + (GetR (c4)) * dy;
                    ig2 = (GetG (c2)) * (dy_1) + (GetG (c4)) * dy;
                    ib2 = (GetB (c2)) * (dy_1) + (GetB (c4)) * dy;
                    ia2 = (GetA (c2)) * (dy_1) + (GetA (c4)) * dy;

                    r = (uint8_t)(ir1 * (dx_1) + ir2 * dx);
                    g = (uint8_t)(ig1 * (dx_1) + ig2 * dx);
                    b = (uint8_t)(ib1 * (dx_1) + ib2 * dx);
                    a = (uint8_t)(ia1 * (dx_1) + ia2 * dx);

                    *(sbm + x + y*simg->w_)=rgb_rgb32(r,g,b,a);
                }
            }
        }else
        if (bpnum_==T_16COL){
            color16_t *bm=(color16_t*)bitmap_;
            color16_t *sbm=(color16_t*)simg->bitmap_;
            for (int y=0; y<newy; y++){
                fY = y * yScale;
                ifY = (int)fY;
                ifY1 = min(h_-1, ifY+1);
                dy = fY - ifY;
                dy_1=1-dy;
                for (int x=0; x<newx; x++){
                    fX = x * xScale;
                    ifX = (int)fX;
                    ifX1 = min(w_-1, ifX+1);
                    dx = fX - ifX;
                    dx_1=1-dx;

                    color32_t c1=rgb16_rgb32 (*(bm + ifX + ifY*w_));
                    color32_t c2=rgb16_rgb32 (*(bm + ifX1 + ifY*w_));
                    color32_t c3=rgb16_rgb32 (*(bm + ifX + ifY1*w_));
                    color32_t c4=rgb16_rgb32 (*(bm + ifX1 + ifY1*w_));

                    ir1 = (GetR (c1)) * (dy_1) + (GetR (c3)) * dy;
                    ig1 = (GetG (c1)) * (dy_1) + (GetG (c3)) * dy;
                    ib1 = (GetB (c1)) * (dy_1) + (GetB (c3)) * dy;

                    ir2 = (GetR (c2)) * (dy_1) + (GetR (c4)) * dy;
                    ig2 = (GetG (c2)) * (dy_1) + (GetG (c4)) * dy;
                    ib2 = (GetB (c2)) * (dy_1) + (GetB (c4)) * dy;

                    r = (uint8_t)(ir1 * (dx_1) + ir2 * dx);
                    g = (uint8_t)(ig1 * (dx_1) + ig2 * dx);
                    b = (uint8_t)(ib1 * (dx_1) + ib2 * dx);

                    *(sbm + x + y*simg->w_)=rgb_rgb16(r,g,b);
                }
            }
        }

    }
    return 0;
}

int
AIMG::
FResizeQuality (AIMG *dst, float k){
	return Resize (dst, w_*k, h_*k);
}

