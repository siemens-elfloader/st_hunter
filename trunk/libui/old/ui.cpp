#include "ui.h"
#include "io.h"

////////////////////Config///////////////////////
int CFG_SCROLL_H=6;
 
int CFG_HEADER_X=10;
int CFG_HEADER_Y=0;

int CFG_SOFTKEY_X=8;

int CFG_PANEL_SIZE_UP=24;
int CFG_PANEL_SIZE_DOWN=24;
int CFG_PANEL_SIZE_HEAD=22;

/*
#define CFG_PANEL_SIZE_UP config->size_panel_up
#define CFG_PANEL_SIZE_DOWN config->size_panel_down
#define CFG_PANEL_SIZE_HEAD config->size_header
*/
 
int k_alpha=200;

int CFG_DISABLE_IBAR=1;
//////////////////

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


void DrawAIMG (AIMG *aimg, int x, int y){
	IMGHDR img;
	img.w=aimg->GetW();
	img.h=aimg->GetH();
	img.bpnum=aimg->GetBtype ();
	img.bitmap=(char*)*aimg->GetBitmap();
        
        DrwImg (&img, x, y,  0, 0);
}

void DrawAIMG2 (AIMG *aimg, int x, int y, MyRECT rc){
	IMGHDR img;
	img.w=aimg->GetW();
	img.h=aimg->GetH();
	img.bpnum=aimg->GetBtype ();
	img.bitmap=(char*)*aimg->GetBitmap();
        
        DrwImg2 (&img, x, y, rc.x, rc.y, rc.x2, rc.y2);
}
#endif

int GetNonDispH (){
#ifdef ELKA
  if (CFG_DISABLE_IBAR){
    DisableIconBar (1);
    return 0;
  }
#endif
  
  return YDISP;
}
///////////////////////////////////////

void
UI::DrawBG (){
  if (bg!=NULL){
    if (bg->GetW()!=0){
      int w=bg->GetW();
      int h=bg->GetH();
      for (int i=0; i<w; i++){
	  for (int j=0; j<h; j++){
                  buffer->SetColor16 (i, j, bg->GetColor16 (i, j));
	  }
      }
    //buffer->DrawLayer (&bg, 0, 0);
    }
    else buffer->DrawRect (0, 0, buffer->GetW(), buffer->GetH(), bgclr);
  }
  else buffer->DrawRect (0, 0, buffer->GetW(), buffer->GetH(), bgclr);
  
  DrawUpPanel (); 
  DrawDownPanel ();
}

void
UI::DrawBG2 (MyRECT rc){
  if (bg!=NULL){
    if (bg->GetW()!=0){
      int w=bg->GetW();
      int h=bg->GetH();
      for (int i=rc.x; i<rc.x2; i++){
	  for (int j=rc.y; j<rc.y2; j++){
                  buffer->SetColor16 (i, j, bg->GetColor16 (i, j));
	  }
      }
    //buffer->DrawLayer (&bg, 0, 0);
    }
    else buffer->DrawRect (rc.x, rc.y, rc.x2, rc.y2, bgclr);
  }
  else buffer->DrawRect (rc.x, rc.y, rc.x2, rc.y2, bgclr);
}

void
UI::DrawUpPanel (){
     //NonDispH=GetNonDispH (); 
     if (!NonDispH) buffer->DrawGradient2 (0, 0, buffer->GetW(), CFG_PANEL_SIZE_UP, panel_clr, panel_clr2, 0);
   }
void
UI::DrawDownPanel (){
     buffer->DrawGradient2 (0, buffer->GetH()-CFG_PANEL_SIZE_DOWN, buffer->GetW(), buffer->GetH(),  panel_clr, panel_clr2, 0);
   }
   
void
UI::DrawHeader (){
      //HEADER
  TFont *font=fonts [hfont];
  if (font){
        int FontH=font->GetFontH ();
        buffer->DrawGradient2 (0, CFG_PANEL_SIZE_UP+CFG_HEADER_Y, buffer->GetW(), CFG_PANEL_SIZE_UP+CFG_HEADER_Y+CFG_PANEL_SIZE_HEAD, header_clr, header_clr2, 0);
        buffer->DrawString (hname, font, CFG_HEADER_X, CFG_PANEL_SIZE_UP+CFG_HEADER_Y+(CFG_PANEL_SIZE_HEAD-FontH)/2, buffer->GetW(), CFG_PANEL_SIZE_UP+CFG_HEADER_Y+(CFG_PANEL_SIZE_HEAD-FontH)/2+FontH, TEXT_ALIGNLEFT, clr);
  }
}

int 
UI::DrawHeaderPanel (){
  buffer->DrawGradient2 (0, CFG_PANEL_SIZE_UP+CFG_HEADER_Y, buffer->GetW(), CFG_PANEL_SIZE_UP+CFG_HEADER_Y+CFG_PANEL_SIZE_HEAD, header_clr, header_clr2, 0);
  return CFG_PANEL_SIZE_UP+CFG_HEADER_Y+CFG_PANEL_SIZE_HEAD;
}

void
UI::DrawHeaderText (char *str, int align){
      //HEADER
  TFont *font=fonts [hfont];
  if (font){
        int FontH=font->GetFontH ();
        buffer->DrawString (str, font, CFG_HEADER_X, CFG_PANEL_SIZE_UP+CFG_HEADER_Y+(CFG_PANEL_SIZE_HEAD-FontH)/2, buffer->GetW(), CFG_PANEL_SIZE_UP+CFG_HEADER_Y+(CFG_PANEL_SIZE_HEAD-FontH)/2+FontH, align, clr);
  }
}

void
UI::DrawIndex (int cur, int max){
  TFont *font=fonts [hfont];
  if (font){
        int FontH=font->GetFontH ();
        char str[10];
        sprintf (str, "%d/%d", cur+1, max);
        buffer->DrawString (str, font, CFG_HEADER_X, CFG_PANEL_SIZE_UP+CFG_HEADER_Y+(CFG_PANEL_SIZE_HEAD-FontH)/2, buffer->GetW()-CFG_HEADER_X, CFG_PANEL_SIZE_UP+CFG_HEADER_Y+(CFG_PANEL_SIZE_HEAD-FontH)/2+FontH, TEXT_ALIGNRIGHT, clr);
  }
}

void
UI::DrawScroll (int cur, int show, int max){
       //SCROLL
        int w=buffer->GetW();
        int scroll_beg=(CFG_PANEL_SIZE_UP+CFG_HEADER_Y+CFG_PANEL_SIZE_HEAD);
        int scroll_end=buffer->GetH()-CFG_PANEL_SIZE_DOWN;
        int scroll_h=CFG_SCROLL_H;

        buffer->DrawGradient2 (w-scroll_h, scroll_beg, w-scroll_h/2, scroll_end,  scroll_clr2, scroll_clr, 1);
        buffer->DrawGradient2 (w-scroll_h/2, scroll_beg, w, scroll_end, scroll_clr, scroll_clr2, 1);
        
        int len=(scroll_end-scroll_beg)*show/max;
        int beg=(scroll_end-scroll_beg-len)*cur/(max-1);
        
        buffer->DrawGradient2 (w-scroll_h, scroll_beg+beg, w-scroll_h/2, scroll_beg+beg+len,  sel_scroll_clr2, sel_scroll_clr, 1);
        buffer->DrawGradient2 (w-scroll_h/2, scroll_beg+beg, w, scroll_beg+beg+len, sel_scroll_clr, sel_scroll_clr2, 1);
        
}

void
UI::DrawSoft (){
        //SOFT
  TFont *font=fonts [sfont];
  if (font){
        int FontH=font->GetFontH ();
        //buffer->DrawGradient2 (0, buffer->GetH()-CFG_PANEL_SIZE_DOWN, buffer->GetW(), buffer->GetH(),  RGBA (160, 170, 180,k_alpha), RGBA (80, 90, 100,k_alpha), 0);
        int y=buffer->GetH()-CFG_PANEL_SIZE_DOWN+(CFG_PANEL_SIZE_DOWN-FontH)/2;
        buffer->DrawString (lname, font, CFG_SOFTKEY_X, y, buffer->GetW()/2, y+FontH, TEXT_ALIGNLEFT, clr);
        buffer->DrawString (rname, font, buffer->GetW()/2, y, buffer->GetW()-CFG_SOFTKEY_X, y+FontH, TEXT_ALIGNRIGHT, clr);
  }
}

void
UI::DrawSoftName (char *left, char *right){
        //SOFT
  TFont *font=fonts [sfont];
  if (font){
        int FontH=font->GetFontH ();
        //buffer->DrawGradient2 (0, buffer->GetH()-CFG_PANEL_SIZE_DOWN, buffer->GetW(), buffer->GetH(),  RGBA (160, 170, 180,k_alpha), RGBA (80, 90, 100,k_alpha), 0);
        int y=buffer->GetH()-CFG_PANEL_SIZE_DOWN+(CFG_PANEL_SIZE_DOWN-FontH)/2;
        buffer->DrawString (left, font, CFG_SOFTKEY_X, y, buffer->GetW()/2, y+FontH, TEXT_ALIGNLEFT, clr);
        buffer->DrawString (right, font, buffer->GetW()/2, y, buffer->GetW()-CFG_SOFTKEY_X, y+FontH, TEXT_ALIGNRIGHT, clr);
  }
}

void
UI::DrawAbout (char *name, char *author, char *ver){
    char str[128];
    TFont *font=fonts [mfont];
    
    if (font){
      int FontH=font->GetFontH ();
    
      sprintf (str, "%s\n(c) %s\nv. %s\n%s at %s", name, author, ver, __DATE__, __TIME__);
      buffer->DrawString (str, font, 0, buffer->GetW()/2, buffer->GetW(), buffer->GetH(), TEXT_ALIGNMIDDLE, clr); 
    }
  
}

void
UI::DrawProgressBar (MyRECT rc, int percent){
	color clr1=RGBA (225, 225, 225, 200);
	color clr2=RGBA (75, 75, 75, 200);

	color clr3=RGBA (125, 225, 125, 200);
	color clr4=RGBA (25, 75, 25, 200);
/*

	color clr1=RGBA (225, 225, 225, 200);
	color clr2=RGBA (75, 75, 75, 200);

	color clr3=RGBA (225, 100, 50, 225);
	color clr4=RGBA (125, 75, 25, 225);
	*/

	buffer->DrawGradient2 (rc.x,rc.y, rc.x+rc.x2, rc.y+rc.y2, clr1, clr2, 0);

	buffer->DrawGradient2 (rc.x,rc.y, rc.x+rc.x2*percent/100, rc.y+rc.y2, clr3, clr4, 0);
}

#define CFG_ICLOCK_X 235
#define CFG_ICLOCK_Y 2

/*
void
UI::DrawIconbar (){
  if (CFG_PANEL_SIZE_UP){
#ifndef WIN
  NonDispH=GetNonDispH (); 
  int battery = *RamCap();
  color clr=RGBA (255, 255, 255, 255);
  color bgclr=RGBA (100, 150, 200, 255);
  
  color rclr=RGBA (255, 0, 0, 255);
  color gclr=RGBA (0, 255, 0, 255);
  
  buffer->DrawGradient2 (0, 0, buffer->GetW()*battery/100, buffer->GetW()/100, rclr, gclr, 1);
  //buffer->DrawRect (0, 0, buffer->GetW(), buffer->GetW()/100, clr);
  
  TDate date;
  TTime time;
  GetDateTime(&date, &time); 
  
  char str[5];
  sprintf(str, "%02d:%02d", time.hour, time.min);
  TFont *font=fonts[A_NORMAL_FONT-1];
  
  if (font){
    int FontH=font->GetFontH ();
    buffer->DrawString (str, font, 0, CFG_ICLOCK_Y, CFG_ICLOCK_X, buffer->GetH(), TEXT_ALIGNRIGHT, clr); 
  }
#endif
  }
}
*/

void
UI::DrawIconbar (){
  //SendIPC (ALIB_CLIENT, "AIconBar", 0, 0);
  config->IconBar ();
}

////////////////////////////////////



#define  gran 2
void
Menu::
UpKeyList (){
    curPosMenu_--;
    if(curPosMenu_>=0){
      if ((curPosMenu_<MAX_SHOW_ITEM_-1-1)||(curPosMenu_>=MAX_ITEM_-gran)) curShowPos_--;
      if ((curPosMenu_>=MAX_SHOW_ITEM_-1-1)&&(curPosMenu_<MAX_ITEM_-gran)) slide_--;
    }
    else{
      curPosMenu_=curPosMenu_+gran;
      while(curPosMenu_<MAX_ITEM_){
        if((curPosMenu_<=MAX_SHOW_ITEM_-1-1)||(curPosMenu_>MAX_ITEM_-gran)) curShowPos_++;
        if((curPosMenu_>MAX_SHOW_ITEM_-1-1)&&(curPosMenu_<=MAX_ITEM_-gran)) slide_++;
        curPosMenu_++;
      }
      curPosMenu_--;
    } 
}

void
Menu::
DownKeyList (){
    curPosMenu_++;
    if(curPosMenu_<MAX_ITEM_){
      if ((curPosMenu_<MAX_SHOW_ITEM_-1)||(curPosMenu_>MAX_ITEM_-gran)) curShowPos_++;
      if ((curPosMenu_>=MAX_SHOW_ITEM_-1)&&(curPosMenu_<=MAX_ITEM_-gran)) slide_++;
    }
    else{
        curShowPos_=0;
        curPosMenu_=0;
        slide_=0;
      }
}

void
Menu::
MenuListKey (int mess, int key){
  if (mess==KEY_DOWN || mess==LONG_PRESS){
     
     switch (key){
     
     case DOWN_BUTTON:
       DownKeyList ();
       break;
      
     case UP_BUTTON:
       UpKeyList ();
       break;
     /*  
       case ENTER_BUTTON:
       OnEnter ();
       break;
       */
     }
     
     //AddOnKey (mess, key);
  }
}


void
Menu::
UpKey(){
  if (curPosHor_>0) curPosHor_--; 
  else curPosHor_=MAX_ITEM_HOR_-1;
}

void
Menu::
DownKey(){
  if(curPosHor_<MAX_ITEM_HOR_-1) curPosHor_++;
  else curPosHor_=0;
}
#define  igran 2

void
Menu::
UpKeyIVert (){
    curPosVert_--;
    if(curPosVert_>=0){
      if ((curPosVert_<MAX_SHOW_ITEM_-1-1)||(curPosVert_>=MAX_ITEM_VERT_-igran)) curShowPos_--;
      if ((curPosVert_>=MAX_SHOW_ITEM_-1-1)&&(curPosVert_<MAX_ITEM_VERT_-igran)) slide_--;
    }
    else{
      curPosVert_=curPosVert_+igran;
      while(curPosVert_<MAX_ITEM_VERT_){
        if((curPosVert_<=MAX_SHOW_ITEM_-1-1)||(curPosVert_>MAX_ITEM_VERT_-igran)) curShowPos_++;
        if((curPosVert_>MAX_SHOW_ITEM_-1-1)&&(curPosVert_<=MAX_ITEM_VERT_-igran)) slide_++;
        curPosVert_++;
      }
      curPosVert_--;
    } 
}

void
Menu::
DownKeyIVert (){
    curPosVert_++;
    if(curPosVert_<MAX_ITEM_VERT_){
      if ((curPosVert_<MAX_SHOW_ITEM_-1)||(curPosVert_>MAX_ITEM_VERT_-igran)) curShowPos_++;
      if ((curPosVert_>=MAX_SHOW_ITEM_-1)&&(curPosVert_<=MAX_ITEM_VERT_-igran)) slide_++;
    }
    else{
        curShowPos_=0;
        curPosVert_=0;
        slide_=0;
      }
}

void
Menu::
MenuIconsKey (int mess, int key){
  if (mess==KEY_DOWN || mess==LONG_PRESS){
     
     switch (key){
     
     case DOWN_BUTTON:
       DownKeyIVert ();
       break;
      
     case UP_BUTTON:
       UpKeyIVert ();
       break;

	 case LEFT_BUTTON:
       UpKey ();
       break;
      
     case RIGHT_BUTTON:
       DownKey ();
       break;
     }
     
     //AddOnKey (mess, key);
  }
}



//char *test[15];

int scrollSlide=0;
int scrollStep=5;
int scrollCount=5;
int scroolWait=0;
int delay=10;

GBSTMR tmr;
/*
void
Menu::OnTimer (){
  MenuOnRedraw ();
  GBS_StartTimerProc(&tmr, 216/delay, OnTimer);
}
*/
/*
void
Menu::
DrawMenuList (UI *ui){          
                 
  int fl=0;
    
  TFont *font=fonts [ui->mfont];
  
  if (font){
  int FontSize=font->GetFontH ();
  int TEMP_ITEM_H=FontSize+OFFSET_ITEM_Y;
    
  if (!InitShow_){
    while ((MenuCoord.y+(MAX_SHOW_ITEM_+1)*(TEMP_ITEM_H)+OFFSET_ITEM_NAME_Y)<=MenuCoord.y2) MAX_SHOW_ITEM_++;
    
    if (MAX_SHOW_ITEM_>MAX_ITEM_) MAX_SHOW_ITEM_=MAX_ITEM_;
    InitShow_=1;
  }
  
  //int height=(MAX_SHOW_ITEM_)*(TEMP_ITEM_H)+OFFSET_ITEM_NAME_Y;
  buffer->DrawGradient2 (MenuCoord.x, MenuCoord.y+(curShowPos_)*(TEMP_ITEM_H), MenuCoord.x2, MenuCoord.y+(1+curShowPos_)*(TEMP_ITEM_H),config->cursor, config->cursor2, 0);
 

  int pic_w=0;
  color clr;
  for (int k =0; k<MAX_SHOW_ITEM_; k++){
    int pos=k+slide_;
    int x=MenuCoord.x+OFFSET_ICON_X+OFFSET_ITEM_X+pic_w;
    int y=MenuCoord.y+(k)*(TEMP_ITEM_H)+OFFSET_ITEM_Y/2;
    int len=font->GetStringW (nameList[pos].str);
    
    if (nameList[pos].IsColor==1) clr=nameList[pos].clr;
    else clr=mclr;
      
    if (k==curShowPos_ && (len>MenuCoord.x2-x)){
      
      buffer->DrawScrollString (nameList[pos].str, font, x, y, MenuCoord.x2-CFG_SCROLL_H, MenuCoord.y2, scrollSlide, TEXT_ALIGNLEFT, clr);
      
      if ((scrollSlide==0 || (len-scrollSlide<=MenuCoord.x2-x) )&& scroolWait<scrollCount) scroolWait++;
      else{    
        scroolWait=0;
        if (len-scrollSlide>MenuCoord.x2-x) scrollSlide+=scrollStep;
        else scrollSlide=0;
      }
        
      fl=1;
    }
    else
      buffer->DrawString (nameList[pos].str, font, x, y,
                 MenuCoord.x2-CFG_SCROLL_H, MenuCoord.y2, TEXT_ALIGNLEFT, clr);
    
    if (nameList2[pos].str!="") buffer->DrawScrollString (nameList2[pos].str, font, x, y, MenuCoord.x2-CFG_SCROLL_H, MenuCoord.y2, scrollSlide, TEXT_ALIGNRIGHT, mclr);

    }
  
  if (MAX_ITEM_>MAX_SHOW_ITEM_){
    ui->DrawIndex (curPosMenu_, MAX_ITEM_);
    ui->DrawScroll (curPosMenu_, MAX_SHOW_ITEM_, MAX_ITEM_);
  }
  
  
    if (fl==1){
      if (ui->IsFocus()) GBS_StartTimerProc(&tmr, 216/delay, UI_OnRedraw);
    }
  }
    
}
*/

void
Menu::
DrawMenuList (UI *ui){          
                 
  int fl=0;
    
  TFont *font=fonts [ui->mfont];
  
  if (font){
  int FontSize=font->GetFontH ();
  int TEMP_ITEM_H=FontSize+OFFSET_ITEM_Y;
    
  if (!InitShow_){
    while ((MenuCoord.y+(MAX_SHOW_ITEM_+1)*(TEMP_ITEM_H)+OFFSET_ITEM_NAME_Y)<=MenuCoord.y2) MAX_SHOW_ITEM_++;
    
    if (MAX_SHOW_ITEM_>MAX_ITEM_) MAX_SHOW_ITEM_=MAX_ITEM_;
    InitShow_=1;
  }
  
  //int height=(MAX_SHOW_ITEM_)*(TEMP_ITEM_H)+OFFSET_ITEM_NAME_Y;
  buffer->DrawGradient2 (MenuCoord.x, MenuCoord.y+(curShowPos_)*(TEMP_ITEM_H), MenuCoord.x2, MenuCoord.y+(1+curShowPos_)*(TEMP_ITEM_H),config->cursor, config->cursor2, 0);
 

  int pic_w=0;
  color clr;
  List.external=(List.GetItem (slide_));
  for (int k =0; k<MAX_SHOW_ITEM_; k++){
    //int pos=k+slide_;

	//Item *item=List.GetItemVal (pos);
	Item *item=&(List.external->key_);

    int x=MenuCoord.x+OFFSET_ICON_X+OFFSET_ITEM_X+pic_w;
    int y=MenuCoord.y+(k)*(TEMP_ITEM_H)+OFFSET_ITEM_Y/2;
    int len=font->GetStringW (item->str);
    
    if (item->IsColor==1) clr=item->clr;
    else clr=mclr;
      
    if (k==curShowPos_ && (len>MenuCoord.x2-x)){
      
      buffer->DrawScrollString (item->str, font, x, y, MenuCoord.x2-CFG_SCROLL_H, MenuCoord.y2, scrollSlide, TEXT_ALIGNLEFT, clr);
      
      if ((scrollSlide==0 || (len-scrollSlide<=MenuCoord.x2-x) )&& scroolWait<scrollCount) scroolWait++;
      else{    
        scroolWait=0;
        if (len-scrollSlide>MenuCoord.x2-x) scrollSlide+=scrollStep;
        else scrollSlide=0;
      }
        
      fl=1;
    }
    else
      buffer->DrawString (item->str, font, x, y, MenuCoord.x2-CFG_SCROLL_H, MenuCoord.y2, TEXT_ALIGNLEFT, clr);
    
    List.external=List.external->next_;
    }
  
  if (MAX_ITEM_>MAX_SHOW_ITEM_){
    ui->DrawIndex (curPosMenu_, MAX_ITEM_);
    ui->DrawScroll (curPosMenu_, MAX_SHOW_ITEM_, MAX_ITEM_);
  }
  

#ifndef WIN 
    if (fl==1){
      if (ui->IsFocus()) GBS_StartTimerProc(&tmr, 216/delay, UI_OnRedraw);
    }
#endif
  }
    
}


int
Menu::
GetMaxItemVert (){
  int hor=-1;
  int vert=0;
  for (int k=0; k<MAX_ITEM_; k++){
    hor=k%MAX_ITEM_HOR_;
    if (!hor){ vert++;}
  }
  
  return vert;
}

int MENU_CORRECTION_X=0;
void
Menu::
DrawMenuIcons (UI *ui){
  
  int smch=0;
  int FontSize=0;

  int sm_x=0;
  int sm_y=0;

  TFont *font=fonts [ui->mfont];
  
  if (font){
    FontSize=font->GetFontH ();
  }
  
  int width;
  Item *item;
   
  if (!InitIconMenu){
    InitIconMenu=1;
    item=List.GetItemVal(0);
    
    if (item->IsAIMG!=1){
#ifndef WIN
      pic_w=GetImgWidth(item->nimg);
      pic_h=GetImgHeight(item->nimg);
#else
      pic_w=48;
      pic_h=48;
#endif
    }
    else{
      pic_w=item->aimg->GetW();
      pic_h=item->aimg->GetH();
    }
    
    //pic_w=64;
    //pic_h=64;
  
	
    width=pic_w;//2*MenuCoord.x+;
    while (width<=MenuCoord.x2){
		MAX_ITEM_HOR_++;
		width=width+OFFSET_ITEM_X+pic_w; 
		//if (width<=MenuCoord.x2) 
    }

    MAX_ITEM_VERT_=GetMaxItemVert ();
    
  }
  
  if (!InitShow_){
    while (MenuCoord.y+smch+(MAX_SHOW_ITEM_)*(FontSize+pic_h+OFFSET_ITEM_Y+sm_y)+FontSize+pic_h+sm_y<=MenuCoord.y2) MAX_SHOW_ITEM_++;
    
    if (MAX_SHOW_ITEM_>MAX_ITEM_) MAX_SHOW_ITEM_=MAX_ITEM_;
    InitShow_=1;
    
    int h=0;
    if (MAX_ITEM_VERT_>MAX_SHOW_ITEM_) h=CFG_SCROLL_H;
    width=width-OFFSET_ITEM_X-pic_w;
    MENU_CORRECTION_X=((MenuCoord.x2-MenuCoord.x)-width-h)/2;
  }
  
    

  int x=MenuCoord.x+MENU_CORRECTION_X+(curPosHor_)*(pic_w+OFFSET_ITEM_X+sm_x);
  int y=MenuCoord.y+(curShowPos_)*(FontSize+pic_h+OFFSET_ITEM_Y+sm_y);
  buffer->DrawGradient2 (x, y, x+pic_w, min (y+pic_h+sm_y, MenuCoord.y2), config->cursor, config->cursor2, 0);

  
  for (int v =0; v<MAX_SHOW_ITEM_+1; v++){
    int vert=v+slide_;
	if (vert<MAX_ITEM_VERT_){
		for (int hor=0; hor<MAX_ITEM_HOR_ && vert*MAX_ITEM_HOR_+hor<MAX_ITEM_; hor++){

			Item *item=List.GetItemVal(vert*MAX_ITEM_HOR_+hor);
                        if (item->IsAIMG==1){
                          MyRECT rc;
                          rc.x=MenuCoord.x+MENU_CORRECTION_X+hor*(pic_w+OFFSET_ITEM_X);
                          rc.y=MenuCoord.y+(v)*(FontSize+pic_h+OFFSET_ITEM_Y);
                          rc.x2=rc.x+item->aimg->GetW();
                          rc.y2=MenuCoord.y2;
                          buffer->DrawLayerRECT (item->aimg, rc.x, rc.y, rc);
                        }
                        else DrawImg (MenuCoord.x+MENU_CORRECTION_X+hor*(pic_w+OFFSET_ITEM_X), MenuCoord.y+(v)*(FontSize+pic_h+OFFSET_ITEM_Y), item->nimg);
			buffer->DrawString (item->str, font, MenuCoord.x+MENU_CORRECTION_X+hor*(pic_w+OFFSET_ITEM_X)-10, MenuCoord.y+smch+(v)*(FontSize+pic_h+OFFSET_ITEM_Y)+pic_h, MenuCoord.x+MENU_CORRECTION_X+hor*(pic_w+OFFSET_ITEM_X)+pic_w+10, MenuCoord.y2, TEXT_ALIGNMIDDLE, mclr);
		}
	}
  }

  if (MAX_ITEM_VERT_>MAX_SHOW_ITEM_){
    ui->DrawIndex (curPosVert_*MAX_ITEM_HOR_+curPosHor_, MAX_ITEM_);
    ui->DrawScroll (curPosVert_*MAX_ITEM_HOR_+curPosHor_, MAX_SHOW_ITEM_*MAX_ITEM_HOR_, MAX_ITEM_);
  }
}

void 
Menu::
MenuOnKey (int mess, int key){

  scrollSlide=0;
   
  if (mess==LONG_PRESS) LongPress=1;
  else LongPress=0;
  
  if (MenuType_==1) MenuIconsKey (mess, key);
  else MenuListKey (mess, key);
  
  AddOnKey (mess, key);
  
}
void CloseTimer (){
  GBS_DelTimer (&tmr);
}
///////////////////////////////////


/*
i
"Меню"||5|
"Контакты"|4:\MyELFs\AInterFace\AMenu\Menu\876.png|A06E7BA5|
*/

int
Menu::
LoadFromAMN (unsigned char *fname){
  char tmp[64];
  AFILE file;
  file.Open ((char*)fname);

  if (file.GetCount ()>0){
	  
	  unsigned char t=file.GetCurCh ();
	  if (t=='i') SetMenuType (1);
	  if (t=='s') SetMenuType (0);

	  if (file.GoToSymb ('"')==-1) return -1;
	  if (file.CopyToSymb ((unsigned char*)tmp, '"')==-1) return -1;
          strcpy (hname, tmp);

	  if (file.GoToSymb ('|')==-1) return -1;
	  if (file.CopyToSymb ((unsigned char*)tmp, '|')==-1) return -1;

	  if (file.CopyToSymb ((unsigned char*)tmp, '|')==-1) return -1;
	  
	  Item item;
	  while (file.GetCur ()<file.GetCount ()){
		if (file.GoToSymb ('"')==-1) return -1;
		if (file.CopyToSymb ((unsigned char*)item.str, '"')==-1) return -1;

		if (file.GoToSymb ('|')==-1) return -1;
		if (file.CopyToSymb ((unsigned char*)tmp, '|')==-1) return -1;
                
                if (tmp[1]==':'){
                  item.aimg=new AIMG ();
                  item.aimg->CreateFromPNG (tmp);
                  item.IsAIMG=1;
                }
                else {
                  item.nimg=str2int (tmp);
                  item.IsAIMG=0;
                }

		if (file.CopyToSymb ((unsigned char*)item.action, '|')==-1) return -1;

		List.Add (item);
		IncItemCount ();
                
	}


	file.Close ();
  }
  
  return 0;
}
