#include <alib/img.h>

void
AIMG::
DrawLayer (AIMG *img, int x, int y){

    if (bpnum_==T_32COL){
        for (int i=0; i<img->GetW(); i++){
            for (int j=0; j<img->GetH(); j++){
                color dst=img->GetColor (i, j);
                SetColor (x+i, y+j, dst);
            }
        }
    }

    if (bpnum_==T_16COL){
        if (img->GetBtype ()==T_16COL){
            for (int i=0; i<img->GetW(); i++){
                for (int j=0; j<img->GetH(); j++){
                    SetColor16 (x+i, y+j, img->GetColor16 (i, j));
                }
            }
        }
        if (img->GetBtype ()==T_32COL){
            for (int i=0; i<img->GetW(); i++){
                for (int j=0; j<img->GetH(); j++){
                    SetColor (x+i, y+j, img->GetColor (i, j));
                }
            }
        }
    }

}

void
AIMG::
DrawLayerRECT (AIMG *img, int x, int y, MyRECT rc){
    if (bpnum_==T_16COL){
        int nx=max (x, rc.x);
        int nx2=min (x+img->GetW(), rc.x2);

        int ny=max (y, rc.y);
        int ny2=min (y+img->GetH(), rc.y2);

        for (int i=nx; i<nx2; i++){
            for (int j=ny; j<ny2; j++){
                if (bpnum_==T_16COL){
                    if (img->GetBtype ()==T_16COL) SetColor16 (i, j, img->GetColor16 (i-x, j-y));
                    if (img->GetBtype ()==T_32COL) SetColor (i, j, img->GetColor (i-x, j-y));
                }
                if (bpnum_==T_32COL){
                    SetColor (i, j, img->GetColor (i-x, j-y));
                }
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

void
AIMG::
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

void
AIMG::
DrawFillRect (int x1, int y1, int x2, int y2, color clr){
    for (int i=x1; i<=x2; i++)
        for (int j=y1; j<=y2; j++) SetColor (i, j, clr);
}

void
AIMG::
DrawRect (int x1, int y1, int x2, int y2, color clr){
    DrawLine (x1, y1, x2, y1, clr);
    DrawLine (x1, y2, x2, y2, clr);

    DrawLine (x1, y1+1, x1, y2-1, clr);
    DrawLine (x2, y1+1, x2, y2-1, clr);
}

void
AIMG::
DrawGradient (int x, int y, int x2, int y2, color clr1, color clr2, int rot){

    int hgt=0;
    if (rot) hgt=x2-x;
    else hgt=y2-y;

	for (int i=0; i<hgt; i++){
		int r=clr1.R+(i-hgt+1)*(clr2.R-clr1.R)/(hgt-1)+(clr2.R-clr1.R);
		int g=clr1.G+(i-hgt+1)*(clr2.G-clr1.G)/(hgt-1)+(clr2.G-clr1.G);
		int b=clr1.B+(i-hgt+1)*(clr2.B-clr1.B)/(hgt-1)+(clr2.B-clr1.B);

		if (rot) DrawFillRect (x+i, y, x+i+1, y2, RGBA (r, g, b, (clr1.A+clr2.A)/2));
		else DrawFillRect (x, y+i, x2, y+i+1, RGBA (r, g, b, (clr1.A+clr2.A)/2));
	}
}

void
AIMG::
DrawCircle (int x, int y, int r, color clr){
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
_DrawFillCircle (int x, int y, int r, color clr){
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
DrawFillCircle (int x, int y, int r, color clr){
    if (clr.A==0xFF) _DrawFillCircle (x, y, r, clr);
    else{
        int balpha=clr.A;
        clr.A=0xFF;
        AIMG img;
        img.Create (2*r+1, 2*r+1, T_32COL);
        img.Clean ();
        img._DrawFillCircle (r, r, r, clr);
        img.SetAlphaLevel (balpha);

        DrawLayer (&img, x-r, y-r);
    }

}

void
AIMG::
DrawEllipse(int x, int y, int a, int b, color clr){

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

