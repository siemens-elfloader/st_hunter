#include <alib/menu.h>

int scrollSlide=0;

bool
BaseMenu::
DrawMenuList (UI *ui, Image *buffer, ft_font *font){
    bool need_redraw=0;
    int scroll_line_w=0;
    if (MAX_ITEM_>MAX_SHOW_ITEM_) scroll_line_w=CFG_SCROLL_W;

    int TEMP_ITEM_H=GetItemH (0, font)+OFFSET_ITEM_Y;

    if (!InitShow_){
        while ((MenuCoord.y+(MAX_SHOW_ITEM_+1)*(TEMP_ITEM_H)+OFFSET_ITEM_NAME_Y)<=MenuCoord.y2) MAX_SHOW_ITEM_++;

        if (MAX_SHOW_ITEM_>MAX_ITEM_) MAX_SHOW_ITEM_=MAX_ITEM_;
        InitShow_=1;
    }

    buffer->DrawGradient (MenuCoord.x, MenuCoord.y+(curShowPos_)*(TEMP_ITEM_H), MenuCoord.x2-scroll_line_w, MenuCoord.y+(1+curShowPos_)*(TEMP_ITEM_H),
                           get_color_from_set(CLR_CURSOR_BEG), get_color_from_set(CLR_CURSOR_END), 0);

    for (int k =0; k<MAX_SHOW_ITEM_; k++){
        int pos=k+slide_;
        int x=MenuCoord.x+OFFSET_ICON_X+OFFSET_ITEM_X;
        int y=MenuCoord.y+(k)*(TEMP_ITEM_H)+(OFFSET_ITEM_Y>>1);

        if (Draw (pos, buffer, font, x, y, MenuCoord.x2-scroll_line_w, MenuCoord.y2, k==curShowPos_, LongPress_)) need_redraw=1;
    }

    if (scroll_line_w){
        DrawIndex (buffer, font, curPosMenu_, MAX_ITEM_);
        DrawScroll (buffer, curPosMenu_, MAX_SHOW_ITEM_, MAX_ITEM_);
    }

    return need_redraw;
}

//////////////////////////////////////

#define  gran 2
void
BaseMenu::
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
BaseMenu::
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
BaseMenu::
MenuListKey (int mess, int key){
  if (mess==KEY_DOWN || mess==LONG_PRESS){

     switch (key){

     case DOWN_BUTTON:
       DownKeyList ();
       break;

     case UP_BUTTON:
       UpKeyList ();
       break;

       //case ENTER_BUTTON:
       //OnEnter ();
       //break;

     }

     //AddOnKey (mess, key);
  }
}

void
BaseMenu::
MenuOnKey (int mess, int key){

  scrollSlide=0;

  if (mess==LONG_PRESS) LongPress_=1;
  else LongPress_=0;

  if (MenuType_==1) ;// MenuIconsKey (mess, key);
  else MenuListKey (mess, key);

  CallAddOnKey (mess, key);

}


void
BaseIconMenu::
MenuOnKey (int mess, int key){

  scrollSlide=0;

  if (mess==LONG_PRESS) LongPress_=1;
  else LongPress_=0;

  MenuIconsKey (mess, key);

  CallAddOnKey (mess, key);

}
