#include <alib/img.h>

///System draw
#ifdef WIN
#else

//#define LCD_DRAW_METHOD

void DrwImg(IMGHDR *img, int x, int y,  char *pen, char *brush){
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}

void DrwImg2(IMGHDR * onse, int x, int y, int xRect, int yRect,int xxRect, int yyRect)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,xxRect,yyRect);
  SetPropTo_Obj5(&drwobj,&rc,0,onse);
  SetProp2ImageOrCanvas(&drwobj, &rc, 0, onse, xRect, yRect);
  //SetColor(&drwobj,NULL,NULL);
  DrawObject(&drwobj);
}

#ifndef LCD_DRAW_METHOD
void DrawAIMG (AIMG *aimg, int x, int y){
	IMGHDR img;
	img.w=aimg->GetW();
	img.h=aimg->GetH();
	img.bpnum=aimg->GetBtype ();
	img.bitmap=(unsigned char*)*aimg->GetBitmap();

    DrwImg (&img, x, y,  0, 0);
}
#endif

void DrawAIMG2 (AIMG *aimg, int x, int y, MyRECT rc){
	IMGHDR img;
	img.w=aimg->GetW();
	img.h=aimg->GetH();
	img.bpnum=aimg->GetBtype ();
	img.bitmap=(unsigned char*)*aimg->GetBitmap();

    DrwImg2 (&img, x, y, rc.x, rc.y, rc.x2, rc.y2);
}


////////////////////////////////////
typedef struct{
  int w;
  int h;
  RECT unk_rc;
  RECT rc;
  char *buf;
  char depth;
}LCD_LAYER;

__inl
void *Ram_LCD_Layer_Base_Ptr (void)
__def( 0xF6, void *)

#ifdef LCD_DRAW_METHOD
void DrawAIMG (AIMG *aimg, int x, int y){
    LCD_LAYER *layer=(LCD_LAYER*)Ram_LCD_Layer_Base_Ptr ();

    char clrWhite[]= {0xFF,0xFF,0xFF,0x64};
    WSHDR *ws=AllocWS (128);
    wsprintf (ws, "%d", layer->w);
    DrawString(ws,  0,  0, ScreenW(), ScreenH(), FONT_SMALL_BOLD, TEXT_ALIGNLEFT, clrWhite,0);
    FreeWS (ws);


    /*
	AIMG img (layer->w, layer->h, layer->depth*2, (unsigned char*)layer->buf);
	//depth=4 bpnum=8 T_16COL
	//depth=5 bpnum=10 T_32COL

	img.DrawLayer (aimg, 0, 0);

	//Отрисовка
	//LCDRedrawLAYER(layer);
	//GBS_SendMessage(0x4200, 0, 0, layer);

	img.NotReleaseBitmap ();
	*/
}
#endif

#endif


