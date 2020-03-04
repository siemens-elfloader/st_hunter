#include <alib/img.h>
#include <math.h>

void
AIMG::
SetAlphaLevel (unsigned char alpha){
    if (bpnum_==T_32COL){
        color *bm=(color*)bitmap_;
        for (int i=0; i<w_; i++){
            for (int j=0; j<h_; j++){

                color clr=GetColor (i, j);
                if (clr.A){
                    clr.A=alpha;
                    *(bm + i + j*w_)=clr;
                }
            }
        }
    }
}

int
AIMG::
Blur (AIMG *dest, int r)
{
    if (r < 1) return -1;

    if (bpnum_!=T_32COL) return -2;

    if (!dest->Create (w_, h_, bpnum_)) return -3;

    int width = w_, height = h_;
    color *src = (color*)bitmap_;
    color *dst = (color*)dest->bitmap_;

    int e = 2; // r times e is the sampling interval
    int *gauss = (int*)malloc(r*e*sizeof(int));
    int sum=0;
    int x;
    for(x=0; x<r*e; x++)
    {
        int t = (x - r*e/2.0)/r;
        gauss[x] = exp(-0.5*t*t);
        sum += gauss[x];
    }
    int *weights = (int*)malloc(r*e*sizeof(int));
    for(x=0; x<r*e; x++)
    {
        weights[x] = (int)(gauss[x]*65536.0001/sum);
    }
    int range = r*e/2;

    int y;
    for(y=0; y<height; y++)
    {
        color *s = &src[y*width];
        color *d = &dst[y*width];
        for(x=0; x<range && x<width; x++)
        {
            d[x] = s[x];
        }
        for(; x<width-range; x++)
        {
            int r=0;
            int g=0;
            int b=0;
            int a=0;
            int*f = weights;
            int xx;
            for(xx=x-range; xx<x+range; xx++)
            {
                r += s[xx].R * f[0];
                g += s[xx].G * f[0];
                b += s[xx].B * f[0];
                a += s[xx].A * f[0];
                f++;
            }
            d[x].R = r >> 16;
            d[x].G = g >> 16;
            d[x].B = b >> 16;
            d[x].A = a >> 16;
        }
        for(; x<width; x++)
        {
            d[x] = s[x];
        }
    }

    for(x=0; x<width; x++)
    {
        color *d = &dst[x];
        //color_t*s = &src[x];
        int yy=0;
        //for(y=0;y<range&&y<height;y++) {
        //d[yy] = s[yy];
        //yy+=width;
        //}

        yy += (y = min(range, height)) *width;

        for(; y<height-range; y++)
        {
            int r=0;
            int g=0;
            int b=0;
            int a=0;
            int*f = weights;
            int cy,cyy=yy-range*width;
            for(cy=y-range; cy<y+range; cy++)
            {
                r += d[cyy].R * f[0];
                g += d[cyy].G * f[0];
                b += d[cyy].B * f[0];
                a += d[cyy].A * f[0];
                cyy += width;
                f++;
            }
            d[yy].R = r >> 16;
            d[yy].G = g >> 16;
            d[yy].B = b >> 16;
            d[yy].A = a >> 16;
            yy += width;
        }
        //for(;y<height;y++) {
        //d[yy] = s[yy];
        //    yy += width;
        //}
        if(y<height)
            yy += width*(height-y);
    }

    free(weights);
    free(gauss);

    return 0;
}


int
AIMG::
Wave (AIMG *dest, int l){
    int w = w_;
    int h = h_;
    int a=0;
    int x=0, y =0;
    int c = 0;
    int *siny = 0;

    if (!dest->Create (w_, h_, bpnum_)) return -1;

    dest->Clean ();
    siny = (int*)calloc(w, 4);
    for(int i=0; i<w; i++) siny[i]=(int)(sin(0.07*(1+i)+0.07*c)*l);

    for(int i=0; i<w*(h+l*2); i++){
        if(x==w){
            x=0;
            a++;
        }
        y=a+siny[x];

        if(y>=0 && y<(h+l*2)) dest->SetColor (x, y, GetColorByIndex (i));

        x++;
    }

    free(siny);

    return 0;
}


int
AIMG::
Smooth (AIMG *dst, int smooth_w, int smooth_h){
    if (smooth_w < 1 || smooth_h < 1) return -1;

    if (!dst->Create (w_, h_, bpnum_)) return -2;

    uint16_t iw = w_;
    uint16_t ih = h_;

    uint16_t around_w = smooth_w, around_h = smooth_h;
    uint32_t col = 0, a = 0, r = 0, g = 0, b = 0;
    color argb;
    for(int h = 0; h < ih; ++h)
    {
        for(int w = 0; w < iw; ++w)
        {
            col = 0, a = 0, r = 0, g = 0, b = 0;
            for(int r_h = h-around_h; r_h < h+around_h; ++r_h)
            {
                if(r_h >= 0 && r_h < ih){
                    for(int r_w = w-around_w; r_w < w+around_w; ++r_w)
                    {
                        if(r_w >= 0 && r_w < iw){
                            argb = GetColor (r_w, r_h);
                            a += (uint8_t)argb.A;
                            r += (uint8_t)argb.R;
                            g += (uint8_t)argb.G;
                            b += (uint8_t)argb.B;
                            ++col;
                        }
                    }
                }
            }
            a /= col;
            r /= col;
            g /= col;
            b /= col;

            dst->SetColor (w, h, RGBA(r, g, b, a));
        }
    }

    return 0;
}




