#include <alib/img.h>

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


void DrawAIMG (Image *aimg, int x, int y){
    IMGHDR img;
    img.w=aimg->GetW();
    img.h=aimg->GetH();
    img.bpnum=aimg->GetBtype ();
    img.bitmap=(unsigned char*)*aimg->GetBitmap();

    DrwImg (&img, x, y,  0, 0);

    img.bitmap=0;
}

void DrawRectAIMG (Image *aimg, int x, int y, MyRECT rc){
	IMGHDR img;
	img.w=aimg->GetW();
	img.h=aimg->GetH();
	img.bpnum=aimg->GetBtype ();
	img.bitmap=(unsigned char*)*aimg->GetBitmap();

    DrwImg2 (&img, x, y, rc.x, rc.y, rc.x2, rc.y2);
}
#endif
