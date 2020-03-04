#include <alib/img.h>
#include <swilib.h>
#include "sysdraw.h"

///System draw
#ifdef WIN
#else

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

void DrawImage (Image *aimg, int x, int y){
	IMGHDR img;
	img.w=aimg->GetW();
	img.h=aimg->GetH();
	img.bpnum=aimg->GetBtype ();
	img.bitmap=(unsigned char*)*aimg->GetBitmap();

    DrwImg (&img, x, y,  0, 0);
}

void DrawImageRect (Image *aimg, int x, int y, MyRECT *rc){
	IMGHDR img;
	img.w=aimg->GetW();
	img.h=aimg->GetH();
	img.bpnum=aimg->GetBtype ();
	img.bitmap=(unsigned char*)*aimg->GetBitmap();

    DrwImg2 (&img, x, y, rc->x, rc->y, rc->x2, rc->y2);
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
char* LCD_GetLayerBasePtr(void)
__def( 0x80F6 , char*)


//char tmp_str[256];
void DrawImage_LCDLayer (Image *aimg, int x, int y){

    int *base_addr=(int*)LCD_GetLayerBasePtr ();
    int layer_addr=(int)(*base_addr);
    LCD_LAYER *layer=(LCD_LAYER*)layer_addr;

    //sprintf (tmp_str, "%08X", layer_addr);
    //Log (tmp_str);

	Image img (layer->w, layer->h, T_16COL, (unsigned char*)layer->buf);
	//depth=4 bpnum=8 T_16COL   depth==B4?
	//depth=5 bpnum=10 T_32COL
    img.DrawLayer (aimg, x, y);
	//Отрисовка
	//LCD_RedrawLayer (layer);
	//GBS_SendMessage(0x4200, 0, 0, layer);
	img.NotReleaseBitmap ();
}

#endif


