#include <alib/img.h>

void
Image::
DrawLayer (Image *img, int x, int y){
    if (!img->bitmap_) return;

    switch (bpnum_){
        case T_32COL:{
            for (int j=0; j<img->h_; j++){
                for (int i=0; i<img->w_; i++){
                    if ((x+i)>=0 && (x+i)<w_ && (y+j)>=0 && (y+j)<h_) SetColor (x+i, y+j, img->GetColor (i, j));
                }
            }
        }break;

        case T_16COL:{
            switch (img->bpnum_){
                case T_16COL:{
                    color16_t *bm=(color16_t*)bitmap_;
                    color16_t *dbm=(color16_t*)img->bitmap_;
                    for (int j=0; j<img->h_; j++){
                        int line=(y+j)*w_, line_img=(j)*img->w_;
                        for (int i=0; i<img->w_; i++){
                            if ((x+i)>=0 && (x+i)<w_ && (y+j)>=0 && (y+j)<h_) *(bm + (x+i) + line)=*(dbm + (i) + line_img);
                        }
                    }
                }break;

                case T_32COL:{
                    for (int j=0; j<img->h_; j++){
                        for (int i=0; i<img->w_; i++){
                            if ((x+i)>=0 && (x+i)<w_ && (y+j)>=0 && (y+j)<h_) SetColor (x+i, y+j, img->GetColor (i, j));
                        }
                    }
                }break;
            }
        }break;
    }

}

void
Image::
DrawLayerRect (Image *img, int x, int y, MyRECT rc){
    if (!img->bitmap_) return;

    int nx=max (0, rc.x);
    int nx2=min (x+img->w_, rc.x2);

    int ny=max (0, rc.y);
    int ny2=min (y+img->h_, rc.y2);

    switch (bpnum_){
        case T_32COL:{
            for (int j=ny; j<ny2; j++){
                for (int i=nx; i<nx2; i++){
                    if ((x+i)>=0 && (x+i)<w_ && (y+j)>=0 && (y+j)<h_) SetColor (x+i, y+j, img->GetColor (i, j));
                }
            }
        }break;

        case T_16COL:{
            switch (img->bpnum_){
                case T_16COL:{
                    color16_t *bm=(color16_t*)bitmap_;
                    color16_t *dbm=(color16_t*)img->bitmap_;
                    for (int j=ny; j<ny2; j++){
                        int line=(y+j)*w_, line_img=(j)*img->w_;
                        for (int i=nx; i<nx2; i++){
                            if ((x+i)>=0 && (x+i)<w_ && (y+j)>=0 && (y+j)<h_) *(bm + (x+i) + line)=*(dbm + (i) + line_img);
                        }
                    }
                }break;

                case T_32COL:{
                    for (int j=ny; j<ny2; j++){
                        for (int i=nx; i<nx2; i++){
                            if ((x+i)>=0 && (x+i)<w_ && (y+j)>=0 && (y+j)<h_) SetColor (x+i, y+j, img->GetColor (i, j));
                        }
                    }
                }break;
            }
        }break;
    }

}


#define sign(a) ((a > 0)? 1: -1)
int Integer (double arg){return (int)(arg+0.5);}

//Рисование отрезка: Целочисленный алгоритм Брезенхема
void
Image::
DrawLine (int x1, int y1, int x2, int y2, color32_t clr){
    if (x1==x2){//vert
        for (int i=y1; i<=y2; i++) SetColor (x1, i, clr);
        return;
    }
    if (y1==y2){//hor
        for (int i=x1; i<=x2; i++) SetColor (i, y1, clr);
        return;
    }

    bool is_swap=0;
    int x=x1;
    int y=y1;

    int dx=(x2-x1);
    int dy=(y2-y1);

    int s1=sign (dx);
    int s2=sign (dy);

    dx=abs (dx);
    dy=abs (dy);

    if (dy>dx){
        int tmp=dx;
        dx=dy;
        dy=tmp;
        is_swap=1;
    }

    int err=2*dy-dx;

    for (int i=0; i<dx; i++){
        SetColor (x, y, clr);

        while (err>=0){
            if (is_swap==1) x=x+s1;
            else y=y+s2;

            err=err-2*dx;
        }

        if (is_swap==1) y=y+s2;
        else x=x+s1;

        err=err+2*dy;
    }
}

/*
//Рисование отрезка: Алгоритм ЦДА
void
Image::
DrawLine (int x1, int y1, int x2, int y2, color clr){
    if (x1==x2){//vert
        for (int i=y1; i<=y2; i++) SetColor (x1, i, clr);
        return;
    }
    if (y1==y2){//hor
        for (int i=x1; i<=x2; i++) SetColor (i, y1, clr);
        return;
    }

	int len=0;
	int lenx=abs(x2-x1);
	int leny=abs(y2-y1);

	if (lenx>=leny) len=lenx;
	else len=leny;

	double deltax=(x2-x1)*1./len;
	double deltay=(y2-y1)*1./len;

	double x=x1+0.5*sign (deltax);
	double y=y1+0.5*sign (deltay);

	for (int i=0; i<len; i++){
		SetColor (Integer(x), Integer(y), clr);
		x+=deltax;
		y+=deltay;
	}
}
*/

void
Image::
DrawFillRect (int x1, int y1, int x2, int y2, color32_t clr){
    for (int i=x1; i<=x2; i++)
        for (int j=y1; j<=y2; j++) SetColor (i, j, clr);
}

void
Image::
DrawRect (int x1, int y1, int x2, int y2, color32_t clr){
    DrawLine (x1, y1, x2, y1, clr);
    DrawLine (x1, y2, x2, y2, clr);

    DrawLine (x1, y1+1, x1, y2-1, clr);
    DrawLine (x2, y1+1, x2, y2-1, clr);
}

void
Image::
DrawGradient (int x, int y, int x2, int y2, color32_t clr1, color32_t clr2, int rot){

    int hgt=0;
    if (rot) hgt=x2-x;
    else hgt=y2-y;

    uint8_t r1=(GetR(clr1));
    uint8_t g1=(GetG(clr1));
    uint8_t b1=(GetB(clr1));

    int r2=(GetR(clr2))-r1;
    int g2=(GetG(clr2))-g1;
    int b2=(GetB(clr2))-b1;
    uint16_t a=((GetA(clr1))+(GetA(clr2)))>>2;

	for (int i=0; i<hgt; i++){
		uint8_t r=r1+(i-hgt+1)*(r2)/(hgt-1)+(r2);
		uint8_t g=g1+(i-hgt+1)*(g2)/(hgt-1)+(g2);
		uint8_t b=b1+(i-hgt+1)*(b2)/(hgt-1)+(b2);

		if (rot) DrawFillRect (x+i, y, x+i+1, y2, rgb_rgb32 (r, g, b, a));
		else DrawFillRect (x, y+i, x2, y+i+1, rgb_rgb32 (r, g, b, a));
	}
}

//Рисование окружности: Алгоритм Брезенхема
void
Image::
_DrawCircle (int xc, int yc, int r, color32_t brush, color32_t pen){
	int b=0;
	int x=0;
	int y=r;

	xc=xc+r;
	yc=yc+r;

	int delta=2*(1-r);
	int limit=0;

    if (brush){
        DrawLine (xc-r, yc, xc+r, yc, brush);
    }
    if (pen){
        SetColor (xc-r, yc, pen);
        SetColor (xc+r, yc, pen);
    }

    while (y>limit){
        if (brush){
            DrawLine (xc-x, yc-y, xc+x, yc-y, brush);
            DrawLine (xc-x, yc+y, xc+x, yc+y, brush);
        }

        if (pen){
            SetColor (xc-x, yc-y, pen);
            SetColor (xc+x, yc-y, pen);

            SetColor (xc-x, yc+y, pen);
            SetColor (xc+x, yc+y, pen);
        }

        if (delta<0){
            b=2*delta+2*y-1;
            if (b<=0){
                x++;
                delta+=2*x+1;
            }
            else{
                x++;
                y--;
                delta+=2*x-2*y+2;
            }
        }else{

            if (delta>0){
                b=2*delta+2*x-1;
                if (b<=0){
                    x++;
                    y--;
                    delta+=2*x-2*y+2;
                }
                else{
                    y--;
                    delta+=-2*y+1;
                }
            }
            else{
                x++;
                y--;
                delta+=2*x-2*y+2;
            }
        }
    }

}

void
Image::
DrawFillCircle (int x, int y, int r, color32_t clr){
    uint8_t balpha=GetA(clr);

    if (balpha==0xFF) _DrawCircle (x, y, r, clr, 0);
    else{
        uint8_t rc=GetR(clr);
        uint8_t gc=GetG(clr);
        uint8_t bc=GetB(clr);

        Image img;
        img.Create (2*r+1, 2*r+1, T_32COL);
        img.Clean ();
        img._DrawCircle (0, 0, r, rgb_rgb32 (rc, gc, bc, 0xFF), 0);
        img.SetAlphaLevel (balpha);

        DrawLayer (&img, x, y);
    }

}

void
Image::
DrawCircle (int x, int y, int r, color32_t clr){
    _DrawCircle (x, y, r, 0, clr);
}

