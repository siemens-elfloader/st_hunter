#include <alib/img.h>
#include <math.h>

void
AIMG::
FlipHoriz (){
	for (int j=0; j<h_; j++){
		for (int i=0; i<w_/2; i++){
		    if (bpnum_==T_32COL){
                color left=GetColor (i, j);
                color right=GetColor (w_-i-1, j);
                SetColor (i, j, right);
                SetColor (w_-i-1, j, left);
		    }

			if (bpnum_==T_16COL){
                unsigned short left=GetColor16 (i, j);
                unsigned short right=GetColor16 (w_-i-1, j);
                SetColor16 (i, j, right);
                SetColor16 (w_-i-1, j, left);
			}
		}
	}
}

void
AIMG::
FlipVertic (){
	for (int i=0; i<w_; i++){
		for (int j=0; j<h_/2; j++){
		    if (bpnum_==T_32COL){
                color up=GetColor (i, j);
                color down=GetColor (i, h_-j-1);
                SetColor (i, j, down);
                SetColor (i, h_-j-1, up);
		    }

		    if (bpnum_==T_16COL){
		        unsigned short up=GetColor16 (i, j);
                unsigned short  down=GetColor16 (i, h_-j-1);
                SetColor16 (i, j, down);
                SetColor16 (i, h_-j-1, up);
		    }
		}
	}
}

int
AIMG::
Rotate90 (){
	AIMG *img=new AIMG (h_, w_, bpnum_);

    if (!bitmap_){ delete img; return -1;}

    if (bpnum_==T_32COL) img->Clean ();

    for (int i=0; i<h_; i++){
        for (int j=0; j<w_; j++){
            if (bpnum_==T_32COL) img->SetColor (h_-1-i, j, GetColor (j, i));
            if (bpnum_==T_16COL) img->SetColor16 (h_-1-i, j, GetColor16 (j, i));
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
Rotate (int ang){
    switch (ang){
        case 90: Rotate90 (); break;
        case 180: FlipVertic (); FlipHoriz (); break;
        case 270: Rotate90 (); Rotate90 (); Rotate90 (); break;
    }

	return 0;
}

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
}

int
AIMG::
Resize (AIMG *simg, int px, int py){

    if (w_==px && h_==py)  return 1;  //original
    else{
        long newx = px,
        newy = py;

        float xScale, yScale, fX, fY;
        xScale = (float)w_  / (float)newx;
        yScale = (float)h_ / (float)newy;

        if (!simg->Create (newx, newy, bpnum_)) return -1;

        if (bpnum_==T_32COL) simg->Clean ();

        long ifX, ifY, ifX1, ifY1, xmax, ymax;
        float ir1, ir2, ig1, ig2, ib1, ib2, ia1, ia2, dx, dy;
        char r,g,b,a;
        color rgb1, rgb2, rgb3, rgb4;
        xmax = w_-1;
        ymax = h_-1;
        for (long y=0; y<newy; y++){
            fY = y * yScale;
            ifY = (int)fY;
            ifY1 = min(ymax, ifY+1);
            dy = fY - ifY;
            for (long x=0; x<newx; x++){
                fX = x * xScale;
                ifX = (int)fX;
                ifX1 = min(xmax, ifX+1);
                dx = fX - ifX;
                rgb1= GetColor(ifX,ifY);
                rgb2= GetColor(ifX1,ifY);
                rgb3= GetColor(ifX,ifY1);
                rgb4= GetColor(ifX1,ifY1);

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

                simg->SetColor (x,y,RGBA(r,g,b,a));
            }
        }

    }

    return 0;
}

int
AIMG::
FResize (AIMG *dst, float k){
	return Resize (dst, w_*k, h_*k);
}
