#ifndef __MY_MENU_H__
#define __MY_MENU_H__

#include <alib/menu.h>

#define MAX_LEN_NAME 128

class RadioItem: public BaseItem{
    int freq_;
    char name_[MAX_LEN_NAME];
public:
    RadioItem (){ }
    RadioItem (char *name, int freq){ strcpy (name_, name); freq_=freq;}
    RadioItem (const RadioItem &obj){
        freq_=obj.freq_;
		strcpy (name_, obj.name_);
	};

    virtual ~RadioItem (){}

    char *GetName (){ return name_;}
    void SetName (char *name){ strcpy (name_, name);}
    int GetFreq (){ return freq_;}
    void SetFreq (int freq){ freq_=freq;}

    bool Draw (Image *buffer, ft_font *font, int x, int y, int x2, int y2, bool active, bool press);

    int GetItemH (ft_font *font){ return GetFontH (font);}
};

class RadioMenu : public BaseMenu{
public:
    std::vector <RadioItem> List;

    RadioMenu (){
        //Menu ();
    }

    ~RadioMenu (){
        Release ();
    }

    int GetItemCount (){ return MAX_ITEM_;}
    void SetItemCount (int count){ MAX_ITEM_=count;}
    void IncItemCount (){ MAX_ITEM_++;}

    bool Draw (int id, Image *buffer, ft_font *font, int x, int y, int x2, int y2, bool active, bool press){
        return (List.at (id)).Draw (buffer, font, x, y, x2, y2, active, press);
    }

    int GetItemH (int id, ft_font *font){ return (List.at (id)).GetItemH (font);}

    void Remove (int id){
        //List.erase (id);
        MAX_ITEM_--;

        MAX_SHOW_ITEM_=0;
        InitShow_=0;
    }

    void Release  (){
        MAX_SHOW_ITEM_=0;
        InitShow_=0;

        List.clear ();

        /*
        for (int i=0; i<MAX_ITEM_; i++){
            Item *item=List.GetItemVal(i);
            if (item->IsAIMG==1) delete item->aimg;
        }
        */
    }

};


#endif
