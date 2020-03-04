#ifndef __IMG_H__
#define __IMG_H__

#include <swilib.h>
#include <alib\freetype.h>
#include <wchar.h>

#define rgb_rgb16(R, G, B) (((B>>3)&0x1F) | ((G<<3)&0x7E0) | ((R<<8)&0xF800))
#define a16pixel 0xE000 //- transparent pixel

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

class AIMG{
	int w_;
	int h_;
	int bpnum_;
	unsigned char *bitmap_;
public:
	AIMG (){ w_=0; h_=0; bpnum_=0; bitmap_=0;}
	AIMG (int w, int h, int bpnum){ Create (w, h, bpnum);}
	AIMG (int w, int h, int bpnum, unsigned char *bitmap){ w_=w; h_=h; bpnum_=bpnum; bitmap_=bitmap;}
	AIMG (const AIMG &obj) {
		//a = obj.a;
		w_=obj.w_; h_=obj.h_; bpnum_=obj.bpnum_; bitmap_=obj.bitmap_;
	};

	~AIMG (){
		if (bitmap_)
			if (w_!=0 && h_!=0){ delete [] bitmap_; bitmap_=NULL;}
		//w_=0; h_=0; bpnum_=0;
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
	int Fork (AIMG *dst);
	void Fill (color32_t clr);
	void Clean ();

    //Draw
	void Draw (int x, int y);
	void DrawLayer (AIMG *img, int x, int y);
    void DrawLayerRECT (AIMG *img, int x, int y, MyRECT rc);
    void DrawGradient (int x, int y, int x2, int y2, color32_t clr1, color32_t clr2, int rot);
	void DrawLine (int x, int y, int x2, int y2, color32_t clr);
	void DrawRect (int x1, int y1, int x2, int y2, color32_t clr);
	void DrawFillRect (int x, int y, int x2, int y2, color32_t clr);
	void DrawCircle (int x, int y, int r, color32_t clr);
	void _DrawFillCircle (int x, int y, int r, color32_t clr);  //don't support alpha-channnel
    void DrawFillCircle (int x, int y, int r, color32_t clr);
    void DrawEllipse (int x, int y, int a, int b, color32_t clr);

    //Transform
	void FlipHoriz ();
	void FlipVertic ();
	int Rotate90 ();
	int Rotate270 ();
	int Rotate (int ang);
	int FRotate (AIMG *dst, float angle);
	int Resize (AIMG *simg, int num, int denom);
	//with interpolation
	int ResizeQuality (AIMG *dst, int px, int py);
	int FResizeQuality (AIMG *dst, float k);

    //Effects
	void SetAlphaLevel (uint8_t alpha); //only for T_32COL bitmap
	int Blur (AIMG *dst, int r);   //only for T_32COL bitmap
	int Wave (AIMG *dst, int l);
	int Smooth (AIMG *dst, int smooth_w, int smooth_h);

    //Formats
	int CreateFromJPEG (char *fname, int num=0, int denom=0);
    int SaveInJPEG (char *file, int quality);
	int CreateFromPNG (char *fname);
	int SaveInPNG (const char *path);

    //Font
	int FT_DrawLetter (ft_font *ftf, int num, int x, int y, MyRECT rc, color32_t clr); //UTF16
	int FT_DrawScrollString (wchar_t *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int TEXT_ALIGN, color32_t clr);//UTF16

    //With charmap
    int DrawScrollString (char *str, ft_font *font, int x1, int y1, int x2, int y2, int slide, int TEXT_ALIGN, color32_t clr, int charmap);
    int DrawString (char *str, ft_font *font, int x1, int y1, int x2, int y2, int TEXT_ALIGN, color32_t clr, int charmap);

};

//Fonts
#define UTF16 0
#define UTF8 1
#define ASCII 2

void DecodeString (wchar_t *wstr, char *str, int charmap);

int GetFontH (ft_font *ftf);

int FT_GetLetterW (ft_font *ftf, int num);
int FT_GetStringW (wchar_t *str, ft_font *font);//UTF16

int GetStringW (char *str, ft_font *font, int charmap);

#define DrawString_UTF8(...) DrawString(__VA_ARGS__, UTF8)
#define DrawString_UTF16(...) DrawString(__VA_ARGS__, UTF16)
#define DrawString_ASCII(...) DrawString(__VA_ARGS__, ASCII)

#define DrawScrollString_UTF8(...) DrawScrollString(__VA_ARGS__, UTF8)
#define DrawScrollString_UTF16(...) DrawScrollString(__VA_ARGS__, UTF16)
#define DrawScrollString_ASCII(...) DrawScrollString(__VA_ARGS__, ASCII)

#endif
