#include "img.h"
#include "menu.h"
#include "list.h"

////////////////////Config///////////////////////
extern int CFG_SCROLL_H;
 
extern int CFG_HEADER_X;
extern int CFG_HEADER_Y;

extern int CFG_SOFTKEY_X;

extern int CFG_PANEL_SIZE_UP;
extern int CFG_PANEL_SIZE_DOWN;
extern int CFG_PANEL_SIZE_HEAD;


extern int CFG_DISABLE_IBAR;
//////////////////


void ALS_CreateUI ();
void ALS_DrawBG ();
int ALS_DrawHeader (char *hname);
void ALS_DrawSoft (char *lsoft, char *rsoft);
void ALS_CloseUI ();
void OnPaint ();
void ALS_WDrawString (WSHDR *ws,  int x, int y, int x2, int y2, int nfont, int TEXT_ALIGN, char *clr);
void ALS_DrawProgressBar (MyRECT rc, int percent);
void ALS_DrawLayer (int x, int y, int w, int h, int bpnum, unsigned char *bitmap);

struct ConfigInfo{
  AIMG *buffer;
  AIMG *bg;
  TFont *small;
  TFont *normal;
  TFont *large;
  
  color background;
  color panel;
  color panel2;

  color header;
  color header2;

  color scrollbg;
  color scrollbg2;

  color scroll;
  color scroll2;
  
  color cursor;
  color cursor2;
  
  int size_panel_up;
  int size_panel_down;
  int size_header;
  
  void (*IconBar)();
  //void (*WDrawString)();
  
};

struct ALSFunc{
  void (*ALS_CreateUI) ();
  void (*ALS_DrawBG) ();
  int (*ALS_DrawHeader) (char *hname);
  void (*ALS_DrawSoft) (char *lsoft, char *rsoft);
  void (*ALS_CloseUI) ();
  void (*OnPaint) ();
  void (*ALS_WDrawString) (WSHDR *ws,  int x, int y, int x2, int y2, int nfont, int TEXT_ALIGN, char *clr);
  void (*ALS_DrawProgressBar) (MyRECT rc, int percent);
  void (*ALS_DrawLayer) (int x, int y, int w, int h, int bpnum, unsigned char *bitmap);
};

void LoadTheme (ConfigInfo *cfg);

extern ConfigInfo *config;
extern ALSFunc alsfunc;

#ifdef WIN
	

	#define ScreenW() 240
	#define ScreenH() 320
	#define WindowW ScreenW()+8
	#define WindowH ScreenH()+54

	void OnRedraw (HDC hDC);
	void UI_OnKey (int mess, int key);
#else
      void UI_OnRedraw ();
      void UI_OnCreate ();
      void UI_OnClose ();
      void UI_OnKey (int mess, int key);
      void UI_OnFocus ();
      void UI_OnUnFocus ();
      
      void StartUI ();


#endif



//////////////////////////////UI////////////////////////
int GetNonDispH ();

class UI{

  char hname[64];
  char lname[64];
  char rname[64];
  
  int hfont;
  
  int sfont;
  
  color bgclr;
  
  color panel_clr;
  color panel_clr2;
  
  color header_clr;
  color header_clr2;
  
  color scroll_clr;
  color scroll_clr2;
  
  color sel_scroll_clr;
  color sel_scroll_clr2;

   
   color clr;
   
   bool focus;
 public: 
   int mfont;
   int NonDispH;
   
   UI (){ 
     focus=0;
     clr=RGBA (255, 255, 255,255);
     int w=ScreenW();
     int h=ScreenH();
     
     hfont=0;
     mfont=0;
     sfont=0;

     NonDispH=GetNonDispH ();
     
     bgclr=config->background;//RGBA (90, 100, 110,255);
     
     panel_clr=config->panel;//RGBA (100, 115, 130,180);
     
     panel_clr2=config->panel2;//RGBA (40, 50, 60,180);
     
     header_clr=config->header;//RGBA (120, 130, 140,180);
     header_clr2=config->header2;//RGBA (40, 50, 60,180);
     
     scroll_clr2=config->scrollbg2;//RGBA (80, 90, 100,180);
     scroll_clr=config->scrollbg;//RGBA (160, 170, 180,180);
     
     sel_scroll_clr2=config->scroll2;//RGBA (30, 40, 50,255);
     sel_scroll_clr=config->scroll;//RGBA (80, 90, 100,255);
   }
   
   ~UI (){ 

   }
 
   void DrawBG ();
   void DrawBG2 (MyRECT rc);
   void DrawUpPanel ();
   void DrawDownPanel ();
   
   int DrawHeaderPanel ();
   
   void DrawHeader ();
   void DrawHeaderText (char *str, int align);
   void DrawScroll (int cur, int show, int max);
   void DrawSoft ();
   void DrawSoftName (char *left, char *right);
   void DrawIndex (int cur, int max);
   void DrawProgressBar (MyRECT rc, int percent);
   
   void DrawAbout (char *name, char *author, char *ver);
   
   void SetHeader (char *name){
     strcpy (hname, name);
   }
   void SetHeaderFont (int font){
     hfont=font-1;
   }
   
   void SetSofts (char *name1, char *name2){
     strcpy (lname, name1);
     strcpy (rname, name2);
   }
   void SetSoftsFont (int font){
     sfont=font-1;
   }
   
   void SetMainFont (int font){
     mfont=font-1;
   }
   
   void DrawIconbar ();
   
   //void OnRedraw ();
   //void OnKey (int mess, int key);
   void OnFocus (){ focus=1;}
   void UnFocus (){ focus=0;}
   bool IsFocus (){ return focus; }
  
   void SwitchFocus (){
    if (focus) focus=0;
    else focus=1;
  }
  
   void SetBGColor (color clr){ bgclr=clr;} 
 };


/////////////////////////Menu///////////////////////////      
      
//input
//char *names[]={"1", "2"};
//KeyHandler (int mes, int key)
struct Item{
  int type;
  char str[128];
#ifndef WIN
  WSHDR *ws;
#endif
  int IsAIMG;
  AIMG *aimg;
  int nimg;
  int IsColor;
  color clr;
  char action[128];
  int is_daemon;
  void *p;
};

class Menu{
private:

  char hname[64];
  bool InitShow_;
  int MenuType_;
  int curPosMenu_;
  int curShowPos_;
  int slide_;
  int MAX_SHOW_ITEM_;
  
  int MAX_ITEM_;
  
  int confNum_;
  
  bool editActive_;
  int countEdit_;
  
  bool IsSetHook_;
  int (*EnterFunc)(int, int, int);
  

  //CFG
  int SUBNAME_FONT_;
  int MENU_FONT_;
  
  int OFFSET_ITEM_X;
  int OFFSET_ITEM_Y;
  int OFFSET_ICON_X;
  int OFFSET_ICON_Y; 
  int OFFSET_ITEM_NAME_Y;
  int OFFSET_ITEM_SUBNAME_Y;
  
  color mclr;
  
  //icon
  int MAX_ITEM_HOR_;
  int MAX_ITEM_VERT_;
  int pic_w;
  int pic_h;
  int InitIconMenu;
  int curPosVert_;
  int curPosHor_;
  
    
public:
  MyRECT MenuCoord;
  bool LongPress;

  MyList<Item> List;
  
  int scrollSlide;
  Menu (){
    MAX_ITEM_=0;
    MenuType_=0;
    InitShow_=0;
    curPosMenu_=0;
    curShowPos_=0;
    slide_=0;
    MAX_SHOW_ITEM_=0;
    editActive_=0;
    countEdit_=0;


    scrollSlide=0;
    LongPress=0;
    
    IsSetHook_=0;
    //cursor_=NULL;
    mclr=RGBA (255, 255, 255, 255);

	//icon
	MAX_ITEM_HOR_=0;
	MAX_ITEM_VERT_=0;
	pic_w=0;
	pic_h=0;
	InitIconMenu=0;

	curPosVert_=0;
	curPosHor_=0;
    
  }
  
  void Release  (){
    MAX_SHOW_ITEM_=0;
    InitShow_=0;
    
    for (int i=0; i<MAX_ITEM_; i++){
      Item *item=List.GetItemVal(i);
      if (item->IsAIMG==1) delete item->aimg;
      
    }
    
    
  }
  
   ~Menu (){
     Release ();
   }
  
  int GetItemCount (){ return MAX_ITEM_;}
  void SetItemCount (int count){ MAX_ITEM_=count;}
  void IncItemCount (){ MAX_ITEM_++;}
  void Remove (int id){
	  List.Remove (id);
	  MAX_ITEM_--;

	  MAX_SHOW_ITEM_=0;
	  InitShow_=0;
  }
  

  void SetMenuType (int type){ MenuType_=type;}
  int GetMenuType (){return MenuType_;}
  
  char *GetHeader (){ return hname;}

  int GetPos (){ return curPosMenu_; }
  void SetPos (){ curPosMenu_=0; curShowPos_=0;}

  void SetAddKeyHook (int (*f)(int, int, int)){
    IsSetHook_=1;
    EnterFunc=f;
  }
  
  void SetItemName (Item *item, char *name){
	strcpy (item->str, name);
  }
  
  void SetItemColor (Item *item, color clr){
    item->IsColor=1;
    item->clr=clr;
  }
  
  void SetMenuRect (int x, int y, int x2, int y2){
    MenuCoord.x=x;
    MenuCoord.y=y;
    MenuCoord.x2=x2;
    MenuCoord.y2=y2;  
  }
  
  void SetCoordinates (int NOFFSET_ITEM_X, int NOFFSET_ITEM_Y,
                       int NOFFSET_ICON_X, int NOFFSET_ICON_Y, 
                       int NOFFSET_ITEM_NAME_Y,
                       int NOFFSET_ITEM_SUBNAME_Y){
                         
    OFFSET_ITEM_X=NOFFSET_ITEM_X;
    OFFSET_ITEM_Y=NOFFSET_ITEM_Y;
    OFFSET_ICON_X=NOFFSET_ICON_X;
    OFFSET_ICON_Y=NOFFSET_ICON_Y; 
    OFFSET_ITEM_NAME_Y=NOFFSET_ITEM_NAME_Y;
    OFFSET_ITEM_SUBNAME_Y=NOFFSET_ITEM_SUBNAME_Y;
              }

  void DrawMenu (UI *ui){
	  if (MenuType_==1) DrawMenuIcons (ui);
	  else DrawMenuList (ui);
  }
  
  void DrawMenuList (UI *ui);
  int GetMaxItemVert ();
  void DrawMenuIcons (UI *ui);
  
  void MenuListKey (int mess, int key); 
  void MenuIconsKey (int mess, int key);
  
  void DownKeyList ();
  void UpKeyList ();
  void DownKey ();
  void UpKey ();
  void DownKeyIVert ();
  void UpKeyIVert ();

  void AddOnKey (int mess, int key){
    if (IsSetHook_){
      if (MenuType_==0)  EnterFunc (mess, key, curPosMenu_);
      if (MenuType_==1){
        int id=curPosVert_*MAX_ITEM_HOR_+curPosHor_;
        if (id<MAX_ITEM_) EnterFunc (mess, key, id);
      }
    }
  }
  
  void MenuOnKey (int mess, int key);
  
  int LoadFromAMN (unsigned char *fname);
};





#define A_SMALL_FONT 1
#define A_NORMAL_FONT 2
#define A_LARGE_FONT 3

//#ifndef WIN
extern bool IsStarted;
extern AIMG *buffer;
extern AIMG *bg;
   
extern TFont *fonts[3];
//#endif

void DrawAIMG (AIMG *aimg, int x, int y);
void DrawAIMG2 (AIMG *aimg, int x, int y, MyRECT rc);
void CloseTimer ();



