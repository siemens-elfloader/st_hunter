#include "include.h"
#include "main.h"
#include "Menu.h"
#include "language.h"

#define CLIST_Y1 (YDISP + 2 + FontH(this->font))
#define SF_Y  scr_h - FontH(this->font)-2 //SoftKey Y
#define Y1 FontH(this->font)+2  

//GuiMenu *menu = NULL;
void *canvasdata;

char clrBlack[] = {0x00,0x00,0x00,0x64};
char clrWhite[] = {0xFF,0xFF,0xFF,0x64};
char clrRed[] = {0xFF,0x00,0x48,0x64};
char transparent[] = {0x00,0x00,0x00,0x00};

extern const char fntSelect[4];//={0xFF,0xFF,0x00,0x60};
extern const char bordSelect[4];//={0x00,0x00,0x00,0x60};

const char clrNotSelect[4]={0xFF,0x00,0x00,0x60};
const char bordNotSelect[4]={0x00,0x00,0x00,0x60};
const char fntNotSelect[4]={0xFF,0xFF,0xFF,0x60};


void GuiMenu::SMART_REDRAW(void)
{
  int f;
  LockSched();
  f=IsGuiOnTop(this->menus_id);
  UnlockSched();
  if (f) REDRAW();
}

GuiMenu::GuiMenu(GUI_MENU *create_menu)
{
  SetError(0);
  this->menu = create_menu;
  this->InitMenu();
}

GuiMenu::GuiMenu()
{

}


GuiMenu::~GuiMenu()
{

}


void GuiMenu::DrawSoftKeys(char *right,char *left)
{
  TTime t;
  GetDateTime(0,&t);
//  char *s = malloc(32);
//  LockSched();
  int FntH = FontH(font);
  DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h,0,0,0,clrBlack,clrBlack);
  if(this->soft_key.shot_n!=2)
  {
    for(int i=FontH(font);i>0;i-=2)
        DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i,0,0,0,CursorClr,CursorClr);
      
    int ScrH = FntH+2;
    int HSIZE = (scr_h-FntH)*ScreenW();
    this->soft_key.shot = DoScreen(ScrH, HSIZE);
    this->soft_key.shot_n++;
    SMART_REDRAW();
    return;
  }
  else
    DrwImage(&this->soft_key.shot,0,scr_h-FntH-2);
  
  PrintField(2,scr_h-1-FontH(font),right,0,font,space);
  PrintField(scr_w,scr_h-1-FontH(font),left,2,font,space);
  
  sprintf(s,"%02d:%02d",t.hour,t.min);
  PrintField(scr_w/2,scr_h-1-FontH(font),s,1,font,space);
  //UnlockSched();  
  mfree(s);
}

void GuiMenu::DrawHeader(BOOKMARKS *bmk,char *s,int x)
{
   int hy = FontH(font) + YDISP;
   DrawRoundedFrame(0,YDISP,scr_w,hy+2,0,0,0,clrBlack,clrBlack);
   if(this->head.shot_n!=2)
   {
     for(int i=hy - YDISP; i>0 ;i-=2)
        DrawRoundedFrame(0,YDISP,scr_w,hy-i+4,0,0,0,CursorClr,CursorClr);
     
    // DoScreen(hy-YDISP+2, (hy-YDISP+2+8)*ScreenW());
    int ScrH  =  hy - YDISP + 2;
    int HSIZE = (hy - YDISP + 2 + 6)*ScreenW();
    this->head.Shot = DoScreen(ScrH, HSIZE);
    this->head.shot_n++;
    SMART_REDRAW();
    return;
  }
  else
    DrwImage(&this->head.Shot,0,YDISP);    
  
   DrwImage(this->head.pic,2,YDISP);
   PrintField(scr_w,YDISP,s,2,font,space);//description in header
   int x2 = GetWidth(scr_w,YDISP);

      PrintText(x+2,YDISP,this->head.name,0,font,space,0,1,0,scr_w-x2-2);
}

void GuiMenu::DrawScrollBar(int cur,int total)
{
  int y=scr_h-CLIST_Y1-FontH(font)-2;
  int pos=y*cur/total;
  int y2=y/total;
  DrawLine(scr_w-2,CLIST_Y1,scr_w-2,scr_h-FontH(font),1,clrBlack);
  int i=0;
  DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+y2,0,0,0,clrBlack,clrBlack);
  for(i=0;i<y2;i+=1)
  {
    DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+i,0,0,0,CursorClr,CursorClr);
  }
}

void GuiMenu::PrintAbout()
{
//  char *s=malloc(128);
  sprintf(s,"AltMyMenu2 not daemon v0.1 C++ test\n(c)kluchnik\n%s at %s",__DATE__,__TIME__);
  DrawRoundedFrame(0,YDISP+FontH(font),scr_w,scr_h,0,0,0,clrWhite,clrWhite);  
//  DrwImage(bgpic,1,YDISP+FontH(font)+4);
  if(isSystemFont) SetFontColor(clrBlack,transparent,0);
  PrintText(ScreenW()/2,(ScreenH()/2)-(FontH(font)*1.5),s,1,font,space,1,0,0,ScreenW()-1);
  DrawSoftKeys(LG_SELECT,LG_CLOSE);
  //mfree(s);
}


extern CList *list;


  
int GuiMenu::InitMenu()
{
  list->FreeList();
  SetParams(this->menu->color,this->menu->setting);
  SetSoftKey(this->menu->softkey);
  SetHeader(this->menu->header);
  this->font = 100 + pngFont;
 return 1;  
}

void GuiMenu::SetSoftKey(SOFT_KEY *soft)
{
  this->soft_key = soft;
}

void GuiMenu::SetHeader(HEADER *h)
{
  this->head = h;
}


void GuiMenu::SetParams(COLOR *color,SET_MENU *setting)
  {
    this->set = setting;
    this->clr = color;
/*    this->set.itemFromDisp = setting.itemFromDisp;
    this->set.totalItems = setting.totalItems;
    this->set.curpage = setting.curpage;
    this->set.curpos = setting.curpos;
    this->set.column = setting.column;
    this->set.nlines = setting.nlines;
    
    
    this->clr.fntSelect = color.fntSelect;
    this->clr.bordSelect = color.bordSelect;
    
    this->clr.fntNotSelect = color.fntNotSelect;
    this->clr.clrNotSelect = color.clrNotSelect;
    this->clr.bordNotSelect = color.bordNotSelect;  
  
    this->clr.CursorBGClr = color.CursorBGClr;
    this->clr.CursorClr = color.CursorClr;*/
}  

void GuiMenu::DrawList()
{
  int font_y = FontH(this->font)+4;//16+2
  int ii;

  if(VIEW_MODE)
    this->set.itemFromDisp = (sdiv(font_y,scr_h-CLIST_Y1-FontH(this->font)-2)*2)-1;
  else      
    this->set.itemFromDisp = sdiv(font_y,scr_h-CLIST_Y1-FontH(this->font)-2)-1;

  this->set.totalItems = list->Count();
  
  if(this->set.itemFromDisp > (CLIST_Y1 - (2-FontH(this->font)))) this->set.itemFromDisp--; 
  if(LINE_MODE) this->set.itemFromDisp--;
  
//  BOOKMARKS *bmk;
//  bmk = list->gettop();//(BOOKMARKS *)bmktop;
  int i=0;
 // int nlines=1;
//  int start_y = 0;
//  int start_x = 0;
  int x = iconSize;
//  char *s=malloc(64);
  RECT rc;
  int oldY=0;  
  this->set.column = 0;

  while(i<this->set.totalItems)
  {
    if((i>=(this->set.curpage-1)*this->set.itemFromDisp) && (i<=this->set.itemFromDisp*this->set.curpage))
     {
        bmk = list->Items(i);//GetBmkByItem(i);
        if(!bmk) break;      
        if(bmk)
        {
          sprintf(s,"%d/%d",this->set.curpos+1,this->set.totalItems);
          start_y=0;

          if(LINE_MODE)
          { 
            if(i>this->set.curpos) start_y += font_y + 3;
            set.nlines = 2; 
          }
          else set.nlines = 1;
          
          if(VIEW_MODE)
          {
            if(!this->set.column)
            {
              oldY = start_y += (CLIST_Y1 + 2 + (((i - (this->set.curpage-1)*this->set.itemFromDisp)*font_y)/2) ); 
              StoreXYXYtoRECT(&rc,2,start_y,(scr_w/2)+5,start_y+(font_y*set.nlines));
            }
            else 
            {
              start_x = (scr_w/2)+1;
              StoreXYXYtoRECT(&rc,start_x,oldY,scr_w-5,oldY+(font_y*set.nlines));
            }
            this->set.column=!this->set.column;
          }
          else
          {
            start_y += (CLIST_Y1 + 2 + (i - (this->set.curpage-1)*this->set.itemFromDisp)*font_y);       
            StoreXYXYtoRECT(&rc,2,start_y,scr_w,start_y+(font_y*set.nlines));            
          }                
          
          if(i == this->set.curpos)
          {
            if(isGradient)
            {
              DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2,0,0,0,clrBlack,clrBlack);//cursor        
              if(this->cursor_shot != 2)
              {
                for(ii=(font_y*oldY);ii>0;ii-=4)
                   DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2-ii,0,0,0,this->clr.CursorBGClr,this->clr.CursorClr);

                int ScrH  =  rc.y + 2;
                int HSIZE = (rc.y2 - rc.y + 2 + 6)*ScreenW();
                this->cursor = DoScreen(ScrH, HSIZE);                
                this->cursor_shot++;
                SMART_REDRAW();
               return;
              }
              else
                DrwImage(&this->cursor,rc.x,rc.y);
            }
            else DrawRoundedFrame(rc.x,rc.y,rc.x2-5,rc.y2,0,0,0,this->clr.CursorBGClr,this->clr.CursorClr);//cursor        

            DrawHeader(bmk,s,x);
            DrawScrollBar(i,this->set.totalItems);
            
            if(LINE_MODE)
               PrintText(rc.x+x,rc.y+font_y,bmk->descript,0,this->font,space,0,0,0,rc.x2);//description under item name
            
          }
          else
            if(isSystemFont)  SetFontColor((char*)clrBlack,(char*)transparent,isOutline); 
          PrintText(rc.x+x,rc.y+1,bmk->name,0,this->font,space,0,0,0,rc.x2-5);//item name
          
          if(bmk->icon_size)  DrwImage(bmk->icon,rc.x+3,rc.y);
//          else  DrwImage(this->noicon,rc.x+3,rc.y);          
         }
       }
      i++;
      if( i > (this->set.curpage*this->set.itemFromDisp)) break;
  }  
  mfree(s);
}



int GuiMenu::DrwImage(IMGHDR *img, int x, int y)
{
  if(!img->bitmap) return 0;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0);
  DrawObject(&drwobj);
  return 1;
}

IMGHDR GuiMenu::DoScreen(int ScrH,int HSIZE)
{
  IMGHDR screen={0,0,8,""};
  int ScrW=ScreenW();
  char *ms=(char*)RamScreenBuffer();
  screen.w=ScrW;
  screen.h=ScrH;
  char *bitmap=(char*)malloc(ScrW*ScrH*2);
  screen.bitmap=bitmap;
  ms+=(HSIZE*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
  return screen;
}


void GuiMenu::MoveUp(int i)
{
  if(!this->set.itemFromDisp)return;
  if(!list->gettop())return;
  if(this->set.curpos < 1)
  {
    this->set.curpos = this->set.totalItems-1;
    
    if(this->set.itemFromDisp != this->set.totalItems)
    {
      this->set.curpage = sdiv(this->set.itemFromDisp,this->set.totalItems)+1;
    }
    else
      this->set.curpage = 1;
  }
  else
  {
    if(i) this->set.curpos-=2;
    else this->set.curpos--;
    if( this->set.curpos < (this->set.curpage-1)*this->set.itemFromDisp){ this->set.curpos++; this->set.curpage--; }
  }
 // REDRAW();
}


void GuiMenu::MoveDown(int i)
{
  if(!this->set.totalItems)return;
  if(!list->gettop())return;
  if(i) this->set.curpos+=2;  
  else this->set.curpos++;
  if( this->set.curpos >= this->set.totalItems) { this->set.curpos = 0; this->set.curpage = 1; }
  if( this->set.curpage*this->set.itemFromDisp < this->set.curpos){ this->set.curpos--; this->set.curpage++; } 
}
