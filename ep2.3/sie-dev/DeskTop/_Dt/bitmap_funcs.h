

inline char BGR_24_8(char *clr) {return((clr[2]&0xE0)|((clr[1]>>3)&0x1C)|((clr[0]>>6)&0x03));}
inline char RGB_24_8(char *clr) {return((clr[0]&0xE0)|((clr[1]>>3)&0x1C)|((clr[2]>>6)&0x03));}

inline short BGR_24_16(char *clr) {return(((clr[2]<<8)&0xF800)|((clr[1]<<3)&0x07E0)|((clr[0]>>3)&0x001F));}
inline short RGB_24_16(char *clr) {return(((clr[0]<<8)&0xF800)|((clr[1]<<3)&0x07E0)|((clr[2]>>3)&0x001F));}

inline short BGR_24_555(char *clr) {return(((clr[2]<<7)&0x7C00)|((clr[1]<<2)&0x03E0)|((clr[0]>>3)&0x001F));}
inline short RGB_24_555(char *clr) {return(((clr[0]<<7)&0x7C00)|((clr[1]<<2)&0x03E0)|((clr[2]>>3)&0x001F));}

inline int RGBA_BGRA(char *clr) {return((clr[3]<<24)+(clr[0]<<16)+(clr[1]<<8)+(clr[2]));}

inline int _16_RGB_32(char *clr)
{
	short c2=clr[0]+clr[1]*0x100;
	return((0xFF<<24)+((c2&0x001F)<<3)+((c2&0x07E0)<<5)+((c2&0xF800)<<8));
}

inline int _16_BGR_32(char *clr)
{
	short c2=clr[0]+clr[1]*0x100;
	return((0xFF<<24)+((c2&0x001F)<<19)+((c2&0x07E0)<<5)+((c2&0xF800)>>8));
}


IMGHDR *resample(IMGHDR *img, int px, int py, int fast, int del);

int ICON_REDRAW(void *dt, DT_ICON *ic);

int GetBitmapMapPos(IMGHDR *img, int x, int y);
int DrwImg2Img(IMGHDR *img, RECT p, IMGHDR *buf, char alfa/* , IMGHDR *fn */);
int FillRectByNum(IMGHDR *buf, RECT p, char n);

void PressImg(IMGHDR *imgsrc, char freemap); // freemap=1 if need mfree(imgsrc->bitmap)

IMGHDR* create_imghdr(const char *fname, int type);
