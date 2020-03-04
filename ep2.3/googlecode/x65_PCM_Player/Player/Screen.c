
//#include "jpg.c"

unsigned short * screen;
IMGHDR img =
{
  16, 16, 0x8, 0
};
RECT rc;
DRWOBJ drwobj;
int color = 0;

#define RGB16(R,G,B) ((B>31?31:B)+((G>63?63:G)<<5)+((R>31?31:R)<<11));
#define ClearScreen() zeromem(&screen,sizeof(screen));

void ScreenInit()
{
  img.w = ScreenW();
  img.h = ScreenH();
  screen = malloc( 132 * 176 * 2 );
  img.bitmap = ( void * ) screen;
  StoreXYWHtoRECT( & rc, 0, 0, img.w, img.h );
  SetPropTo_Obj5( & drwobj, & rc, 0, ( IMGHDR * ) & img );
  SetColor( & drwobj, GetPaletteAdrByColorIndex( 0 ), GetPaletteAdrByColorIndex( 1 ) );
}

void ScreenFree()
{
  mfree( screen );
};

void DrawScreen()
{
  DrawObject( & drwobj );
}

void DrawBitmap( unsigned char x, unsigned char y, unsigned char w, unsigned char h, short * Bitmap )
{
  short yy;
  if ( ( !w ) || ( !h ) ) return;
  yy = 0;
  while ( yy < h )
  {
    memcpy( & screen[x + ScreenW() * ( yy + y )], & Bitmap[yy * w], w * 2 );
    yy++;
  }
}

