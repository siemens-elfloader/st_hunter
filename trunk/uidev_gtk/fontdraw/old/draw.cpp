#include <alib\img.h>

void
AIMG::
DrawLayer (AIMG *img, int x, int y){

    if (bpnum_==T_32COL){
        for (int j=0; j<img->h_; j++){
                for (int i=0; i<img->w_; i++){
                SetColor (x+i, y+j, img->GetColor (i, j));
            }
        }
    }
    else
    if (bpnum_==T_16COL){
        if (img->bpnum_==T_16COL){
            color16_t *bm=(color16_t*)bitmap_;
            color16_t *dbm=(color16_t*)img->bitmap_;
            for (int j=0; j<img->h_; j++){
                int line=(y+j)*w_, line_img=(j)*img->w_;
                for (int i=0; i<img->w_; i++){
                    if ((x+i)>=0 && (x+i)<w_ && (y+j)>=0 && (y+j)<h_) *(bm + (x+i) + line)=*(dbm + (i) + line_img);
                    //SetColor16 (x+i, y+j, img->GetColor16 (i, j));
                }
            }
        }
        else
        if (img->bpnum_==T_32COL){
            for (int j=0; j<img->h_; j++){
                for (int i=0; i<img->w_; i++){
                    SetColor (x+i, y+j, img->GetColor (i, j));
                }
            }
        }
    }

}

void
AIMG::
DrawLayerRECT (AIMG *img, int x, int y, MyRECT rc){
    int nx=max (x, rc.x);
    int nx2=min (x+img->w_, rc.x2);

    int ny=max (y, rc.y);
    int ny2=min (y+img->h_, rc.y2);

    if (bpnum_==T_16COL){
        color16_t *bm=(color16_t*)bitmap_;
        color16_t *dbm=(color16_t*)img->bitmap_;
        if (img->bpnum_==T_16COL){
            for (int j=ny; j<ny2; j++){
                for (int i=nx; i<nx2; i++){
                   //SetColor16 (i, j, img->GetColor16 (i-x, j-y));
                   *(bm + (i) + (j)*w_)=*(dbm + (i-x) + (j-y)*img->w_);

                }
            }
        }
        else
        if (bpnum_==T_32COL){
            for (int i=nx; i<nx2; i++){
                for (int j=ny; j<ny2; j++){
                    SetColor (i, j, img->GetColor (i-x, j-y));
                }
            }
        }
    }
    else
    if (bpnum_==T_32COL){
        for (int i=nx; i<nx2; i++){
            for (int j=ny; j<ny2; j++){
                SetColor (i, j, img->GetColor (i-x, j-y));
            }
        }
    }

}

template <class TYPE>
int sign (TYPE arg){
	if (arg>0) return 1;
	if (arg<0) return -1;

	return 0;
}

int Integer (double arg){return (int)(arg+0.5);}

//brezenhem
void
AIMG::
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

void
AIMG::
DrawFillRect (int x1, int y1, int x2, int y2, color32_t clr){
    for (int i=x1; i<=x2; i++)
        for (int j=y1; j<=y2; j++) SetColor (i, j, clr);
}

void
AIMG::
DrawRect (int x1, int y1, int x2, int y2, color32_t clr){
    DrawLine (x1, y1, x2, y1, clr);
    DrawLine (x1, y2, x2, y2, clr);

    DrawLine (x1, y1+1, x1, y2-1, clr);
    DrawLine (x2, y1+1, x2, y2-1, clr);
}

void
AIMG::
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
    uint8_t a=((GetA(clr1))+(GetA(clr2)))>>2;

	for (int i=0; i<hgt; i++){
		uint8_t r=r1+(i-hgt+1)*(r2)/(hgt-1)+(r2);
		uint8_t g=g1+(i-hgt+1)*(g2)/(hgt-1)+(g2);
		uint8_t b=b1+(i-hgt+1)*(b2)/(hgt-1)+(b2);

		if (rot) DrawFillRect (x+i, y, x+i+1, y2, rgb_rgb32 (r, g, b, a));
		else DrawFillRect (x, y+i, x2, y+i+1, rgb_rgb32 (r, g, b, a));
	}
}

/*
void
AIMG::
DrawGradient (int x, int y, int x2, int y2, color32_t clr_1, color32_t clr_2, int rot){
    color clr1; color clr2;
    rgb32_color (clr_1, &clr1);
    rgb32_color (clr_2, &clr2);

    int hgt=0;
    if (rot) hgt=x2-x;
    else hgt=y2-y;

	for (int i=0; i<hgt; i++){
		uint8_t r=clr1.R+(i-hgt+1)*(clr2.R-clr1.R)/(hgt-1)+(clr2.R-clr1.R);
		uint8_t g=clr1.G+(i-hgt+1)*(clr2.G-clr1.G)/(hgt-1)+(clr2.G-clr1.G);
		uint8_t b=clr1.B+(i-hgt+1)*(clr2.B-clr1.B)/(hgt-1)+(clr2.B-clr1.B);

		if (rot) DrawFillRect (x+i, y, x+i+1, y2, rgb_rgb32 (r, g, b, (clr1.A+clr2.A)>>1));
		else DrawFillRect (x, y+i, x2, y+i+1, rgb_rgb32 (r, g, b, (clr1.A+clr2.A)>>1));
	}
}
*/
void
AIMG::
DrawCircle (int x, int y, int r, color32_t clr){
    int f = 1 - r;
    int ddfx = 1;
    int ddfy = -2 * r;
    int xp = 0;
    int yp = r;

    /* Draw outer points. */
    SetColor (x, y + r, clr);
    SetColor (x, y - r, clr);
    SetColor (x + r, y, clr);
    SetColor (x - r, y, clr);

    /* Calculate coordinates of points in one octant. */
    while(xp < yp)
    {
        /* ddfx == 2 * xp + 1;
           ddfy == -2 * yp;
           f == xp*xp + yp*yp - r*r + 2*xp - yp + 1; */
        if(f >= 0)
        {
            yp--;
            ddfy += 2;
            f += ddfy;
        }

        xp++;
        ddfx += 2;
        f += ddfx;

        /* Draw pixels in all octants. */
        SetColor (x + xp, y + yp, clr);
        SetColor (x + xp, y - yp, clr);
        SetColor (x - xp, y + yp, clr);
        SetColor (x - xp, y - yp, clr);
        SetColor (x + yp, y + xp, clr);
        SetColor (x + yp, y - xp, clr);
        SetColor (x - yp, y + xp, clr);
        SetColor (x - yp, y - xp, clr);
    }
}

void
AIMG::
_DrawFillCircle (int x, int y, int r, color32_t clr){
    int f = 1 - r;
    int ddfx = 1;
    int ddfy = -2 * r;
    int xp = 0;
    int yp = r;

    /* Draw outer points. */
    SetColor (x, y + r, clr);
    SetColor (x, y - r, clr);
    SetColor (x + r, y, clr);
    SetColor (x - r, y, clr);

    /* Calculate coordinates of points in one octant. */
    while (xp < yp)
    {
        /* ddfx == 2 * xp + 1;
           ddfy == -2 * yp;
           f == xp*xp + yp*yp - r*r + 2*xp - yp + 1; */
        if (f >= 0){
            yp--;
            ddfy += 2;
            f += ddfy;
        }

        xp++;
        ddfx += 2;
        f += ddfx;

        /* Fill circle with horizontal lines. */
        DrawLine (x - xp, y - yp, x + xp, y - yp, clr);
        DrawLine (x - xp, y + yp, x + xp, y + yp, clr);
        DrawLine (x - yp, y - xp, x + yp, y - xp, clr);
        DrawLine (x - yp, y + xp, x + yp, y + xp, clr);
    }

    /* Draw last horizontal line (central one). */
    DrawLine (x - r, y, x + r, y, clr);
}

void
AIMG::
DrawFillCircle (int x, int y, int r, color32_t clr){
    uint8_t balpha=GetA(clr);

    if (balpha==0xFF) _DrawFillCircle (x, y, r, clr);
    else{
        uint8_t rc=GetR(clr);
        uint8_t gc=GetG(clr);
        uint8_t bc=GetB(clr);

        AIMG img;
        img.Create (2*r+1, 2*r+1, T_32COL);
        img.Clean ();
        img._DrawFillCircle (r, r, r, rgb_rgb32 (rc, gc, bc, 0xFF));
        img.SetAlphaLevel (balpha);

        DrawLayer (&img, x-r, y-r);
    }

}

void
AIMG::
DrawEllipse(int x, int y, int a, int b, color32_t clr){

    int col,row;
    long a_square,b_square,two_a_square,two_b_square,four_a_square,four_b_square,d;

    b_square=b*b;
    a_square=a*a;
    row=b;
    col=0;
    two_a_square=a_square<<1;
    four_a_square=a_square<<2;
    four_b_square=b_square<<2;
    two_b_square=b_square<<1;
    d=two_a_square*((row-1)*(row))+a_square+two_b_square*(1-a_square);

    while(a_square*(row)>b_square*(col))
    {
        SetColor (col+x,row+y,clr);
        SetColor (col+x,y-row,clr);
        SetColor (x-col,row+y,clr);
        SetColor (x-col,y-row,clr);
        if (d>=0)
        {
            row--;
            d-=four_a_square*(row);
        }
        d+=two_b_square*(3+(col<<1));
        col++;
    }
    d=two_b_square*(col+1)*col+two_a_square*(row*(row-2)+1)+(1-two_a_square)*b_square;
    while ((row) + 1)
    {
        SetColor (col+x,row+y,clr);
        SetColor (col+x,y-row,clr);
        SetColor (x-col,row+y,clr);
        SetColor (x-col,y-row,clr);
        if (d<=0)
        {
            col++;
            d+=four_b_square*col;
        }
        row--;
        d+=two_a_square*(3-(row <<1));
    }
}

