#ifndef __IMG_H__
#define __IMG_H__
#include <swilib.h>
#include <alib/freetype.h>
#include <alib/def_error.h>
#include <alib/config.h>


#define rgb_rgb16(R, G, B) (((B>>3)&0x1F) | ((G<<3)&0x7E0) | ((R<<8)&0xF800))
#define T16_PIXEL 0xE000 //- transparent pixel

#define T_16COL 8
#define T_32COL 10

#define color16_t uint16_t
#define color32_t uint32_t

typedef struct{
  uint8_t R;
  uint8_t G;
  uint8_t B;
  uint8_t A;
}color;

struct MyPOINT{
  int x;
  int y;
};

struct MyRECT{
	int x;
	int y;
	int x2;
	int y2;
};

#define GetA(c) (c >> 24) & 0xff
#define GetR(c) (c >> 16) & 0xff
#define GetG(c) (c >> 8) & 0xff
#define GetB(c) c & 0xff

#define min(a,b) ((a <= b)?(a):(b))
#define max(a,b) ((a > b)?(a):(b))

static inline color32_t rgb_rgb32 (uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return (a << 24) | (r << 16) | (g << 8) | b;
}

static inline void rgb32_color (color32_t c, color *clr){
    clr->A = (c >> 24) & 0xff;
    clr->R = (c >> 16) & 0xff;
    clr->G = (c >> 8) & 0xff;
    clr->B = c & 0xff;
}

static inline color16_t rgb32_rgb16(const color32_t rgb32) {
    return (rgb32 >> 8 & 0xF800) | (rgb32 >> 5 & 0x07E0) | (rgb32 >> 3 & 0x001F);
}

static inline color32_t rgb16_rgb32(color16_t c) {
    return 0xff000000
        | ((((c) << 3) & 0xf8) | (((c) >> 2) & 0x7))
        | ((((c) << 5) & 0xfc00) | (((c) >> 1) & 0x300))
        | ((((c) << 8) & 0xf80000) | (((c) << 3) & 0x70000));
}

color32_t invert_color_rgb32 (color32_t clr);
color16_t invert_color_rgb16 (color16_t clr);

color32_t conv_to_gray_rgb32 (color32_t clr);
color16_t conv_to_gray_rgb16 (color16_t clr);

class Image{
	int w_;
	int h_;
	int bpnum_;
	unsigned char *bitmap_;
public:
	Image (){ w_=0; h_=0; bpnum_=0; bitmap_=0;}
	Image (int w, int h, int bpnum){ Create (w, h, bpnum);}
	Image (int w, int h, int bpnum, unsigned char *bitmap){ w_=w; h_=h; bpnum_=bpnum; bitmap_=bitmap;}
	Image (const Image &obj) {
		w_=obj.w_; h_=obj.h_; bpnum_=obj.bpnum_; bitmap_=obj.bitmap_;
	};

	~Image (){
		if (bitmap_)
			if (w_!=0 && h_!=0){ delete [] bitmap_; bitmap_=NULL;}
	}

    //Core
	int GetW (){ return w_;}
	int GetH (){ return h_;}
	int GetBtype (){ return bpnum_;}
	unsigned char **GetBitmap (){ return &bitmap_;}
	void NotReleaseBitmap (){ w_=h_=0;}
	void FreeBitmap (){ if (bitmap_){ delete [] bitmap_; bitmap_=NULL;}}

    color16_t GetColor16 (int x, int y);
	color32_t GetColor (int x, int y);
	color32_t GetColorByIndex (int index);
	color GetColorS (int x, int y);

	void SetColor (int x, int y, color32_t clr);
    void SetColor16 (int x, int y, color16_t clr);

	bool Create (int w, int h, int bpnum);
	int Fork (Image *dst);
	void Fill (color32_t clr);
	void Clean ();

    //Draw
	void Draw (int x, int y);
	void DrawLayer (Image *img, int x, int y);
    void DrawLayerRect (Image *img, int x, int y, MyRECT rc);
    void DrawGradient (int x, int y, int x2, int y2, color32_t clr1, color32_t clr2, int rot);
	void DrawLine (int x, int y, int x2, int y2, color32_t clr);
	void DrawRect (int x1, int y1, int x2, int y2, color32_t clr);
	void DrawFillRect (int x, int y, int x2, int y2, color32_t clr);
	void _DrawCircle (int xc, int yc, int r, color32_t brush, color32_t pen);  //don't support alpha-channnel
	void DrawCircle (int x, int y, int r, color32_t clr);
    void DrawFillCircle (int x, int y, int r, color32_t clr);

    //Transform
	void FlipHoriz ();
	void FlipVertic ();
	int Rotate (int ang);
	int FRotate (Image *dst, float angle);
	int Resize (Image *simg, int num, int denom);
	int ResizeWithTransform (Image *simg, int num, int denom, int transform=0);
	int ResizeQuality (Image *simg, int num, int denom);
	int _transform_img (int type);
	int _transform_to_new_img (int type);

    //Effects
    int Invert ();
    int GrayScale ();
	void SetAlphaLevel (uint8_t alpha); //only for T_32COL bitmap

    //Formats
	int CreateFromJPEG (const char *fname, int num=0, int denom=0, int max_resolution=8192);
    int SaveInJPEG (const char *file, int quality);
	int CreateFromPNG (const char *fname);
	int SaveInPNG (const char *path);

};

#define NO_TRANSFORM    0
#define FLIP_H          1
#define FLIP_V          2
#define ROT90           3
#define ROT180          4
#define ROT270          5


#endif
