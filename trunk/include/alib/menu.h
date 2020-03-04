#ifndef __ALIB_MENU_H__
#define __ALIB_MENU_H__

#include <alib/img.h>
#include <alib/ui.h>
#include <alib/font.h>

extern int scrollSlide;

class BaseItem{
    public:
    virtual ~BaseItem(){}

    virtual bool Draw (Image *buffer, ft_font *font, int x, int y, int x2, int y2, bool active, bool press)=0;
    virtual int GetItemH (ft_font *font)=0;
};

class BaseMenu{
    public:
    bool InitShow_;
    int MenuType_;
    int curPosMenu_;
    int curShowPos_;
    int slide_;
    int MAX_SHOW_ITEM_;

    int MAX_ITEM_;

    bool IsSetHook_;
    int (*EnterFunc)(int, int, int);

    //CFG
    int OFFSET_ITEM_X;
    int OFFSET_ITEM_Y;
    int OFFSET_ICON_X;
    int OFFSET_ICON_Y;
    int OFFSET_ITEM_NAME_Y;
    int OFFSET_ITEM_SUBNAME_Y;


    MyRECT MenuCoord;
    bool LongPress_;
    bool ScroollActive_;

    BaseMenu (){
        MAX_ITEM_=0;
        MenuType_=0;
        InitShow_=0;
        curPosMenu_=0;
        curShowPos_=0;
        slide_=0;
        MAX_SHOW_ITEM_=0;

        LongPress_=0;
        ScroollActive_=0;

        IsSetHook_=0;
    }

    void ReInitShow (){
        InitShow_=0;
        MAX_SHOW_ITEM_=0;
    }

    virtual void Release ()=0;


    ~BaseMenu (){

    }

    virtual int GetItemCount ()=0;
    virtual void SetItemCount (int count)=0;
    virtual void IncItemCount ()=0;

    virtual bool Draw (int id, Image *buffer, ft_font *font, int x, int y, int x2, int y2, bool active, bool press)=0;
    virtual int GetItemH (int id, ft_font *font)=0;

    /*void Add (MenuItem *item){
        List.push_back (*item);
    }*/

    virtual void Remove (int id)=0;


    void SetMenuType (int type){ MenuType_=type;}
    int GetMenuType (){return MenuType_;}

    int GetPos (){ return curPosMenu_; }
    void SetPos (){ curPosMenu_=0; curShowPos_=0;}

    void SetAddKeyHook (int (*f)(int, int, int)){
        IsSetHook_=1;
        EnterFunc=f;
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

    bool DrawMenuList (UI *ui, Image *buffer, ft_font *font);

    void MenuListKey (int mess, int key);

    void DownKeyList ();
    void UpKeyList ();

    void CallAddOnKey (int mess, int key){
        if (IsSetHook_){
            EnterFunc (mess, key, curPosMenu_);
        }
    }

    void MenuOnKey (int mess, int key);

    int LoadFromAMN (char *fname);
};


class BaseIconMenu : public BaseMenu{
public:
    int MAX_ITEM_HOR_;
    int MAX_ITEM_VERT_;
    int pic_w;
    int pic_h;
    int InitIconMenu;
    int curPosVert_;
    int curPosHor_;

    BaseIconMenu (){
        //Menu ();
        MAX_ITEM_HOR_=0;
        MAX_ITEM_VERT_=0;
        pic_w=0;
        pic_h=0;
        InitIconMenu=0;

        curPosVert_=0;
        curPosHor_=0;
    }

    ~BaseIconMenu (){

    }

    int GetMaxItemVert ();
    void DrawMenuIcons (ft_font *font); //(UI *ui);

    void MenuIconsKey (int mess, int key){};

    void DownKey ();
    void UpKey ();
    void DownKeyIVert ();
    void UpKeyIVert ();

    void CallAddOnKey (int mess, int key){
        if (IsSetHook_){
            int id=curPosVert_*MAX_ITEM_HOR_+curPosHor_;
            if (id<MAX_ITEM_) EnterFunc (mess, key, id);
        }
    }

    void MenuOnKey (int mess, int key);

};

#endif
