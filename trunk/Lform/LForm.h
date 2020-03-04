


#ifndef __LFORM_H__
#define __LFORM_H__

#include <swilib.h>
#include <vector>
#include <string>
#include <map>
#include "p2str_t.h"
#include <sigc++/sigc++.h>


class LForm
{
public:

    typedef sigc::signal<int, LForm *, int, void *> item_signal;

    /* structures */
    typedef struct
    {
        item_signal sig;

        /* call back */
        int (*proc)(LForm *, int, void *);

        /* upper text */
        p2str_t uitem;

        /* lower item */
        p2str_t litem;

        /* icon way */
        p2str_t icon_p;
        p2str_t lower_icon_p;

        /* user data */
        const void *ud;
    } Item;


    class SoftData
    {
    public:
        SoftData() :
            proc(0), ud(0)
        {}

        p2str_t text;
        int (*proc)(LForm *, void *);
        const void *ud;
    };


    class key_binder
    {
    public:

        key_binder() : proc(0), ud(0) {}


        key_binder(int (*p)(LForm *, int, void *), void *ud) :
            proc(0), ud(0)
        {
            proc = p;
            this->ud = ud;
        }


        key_binder(const item_signal::slot_type & slot, void *ud) :
            proc(0), ud(0)
        {
            proc = 0;
            this->ud = ud;
            sig.connect(slot);
        }


        key_binder( const key_binder & kb) :
            proc(0), ud(0)
        {
            sig = kb.sig;
            proc = kb.proc;
            ud = kb.ud;
        }

        key_binder( const key_binder * kb) :
            proc(0), ud(0)
        {
            sig = kb->sig;
            proc = kb->proc;
            ud = kb->ud;
        }

        item_signal sig;
        int (*proc)(LForm *, int, void *);
        const void *ud;
    };


public:
    LForm(int);
    ~LForm();

    inline int gui_id()
    {
        return _gui_id;
    }

    size_t size();
    void setItemCount(int count);
    int currentItem();
    int show();

    void setUserData(void *data, size_t group);
    void *userData(size_t group);


    void append(const p2str_t & lgp);
    void append(const p2str_t & ulgp, const p2str_t & llgp);

    void append(const p2str_t & icon, const p2str_t & lgp, int func(LForm *, int, void*), const void *ud = 0);
    void append(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                const p2str_t & llgp, int func(LForm *, int, void*), const void *ud = 0);

    void append(const p2str_t & icon, const p2str_t & lgp, const item_signal::slot_type & slot, const void *ud = 0);
    void append(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp, const p2str_t & llgp,
                const item_signal::slot_type & slot, const void *ud = 0);


    void insert(int item, const p2str_t & lgp);
    void insert(int item, const p2str_t & ulgp, const p2str_t & llgp);

    void insert(int item, const p2str_t & icon, const p2str_t & lgp, int func(LForm *, int, void*), const void *ud = 0);
    void insert(int item, const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                const p2str_t & llgp, int func(LForm *, int, void*), const void *ud = 0);

    void insert(int item, const p2str_t & icon, const p2str_t & lgp,
                const item_signal::slot_type & slot, const void *ud = 0);

    void insert(int item, const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                const p2str_t & llgp, const item_signal::slot_type & slot, const void *ud = 0);



    void remove(int item);



    void setVectorOfItems(const std::vector <Item *> & v, bool flush = true);
    inline std::vector <Item *> & vectorItems()
    {
        return items;
    }

    Item * item(int item);

    void setItemUpperText(int item, const p2str_t & text);
    void setItemLowerText(int item, const p2str_t & text);
    void setItemUpperIcon(int item, const p2str_t & icon);
    void setItemLowerIcon(int item, const p2str_t & icon);
    void setItemAction(int item, int func(LForm *, int, void*));
    void setItemAction(int item, const item_signal::slot_type & slot);
    void setItemUserData(int item, void *ud);

    const p2str_t & itemUpperText(int item);
    const p2str_t & itemLowerText(int item);
    const p2str_t & itemUpperIcon(int item);
    const p2str_t & itemLowerIcon(int item);
    void * setItemAction_f(int item);
    const item_signal * itemAction_s(int item);
    void * itemUserData(int item);


    int doCurrentItemFunc();
    int doItemFunc(int item);


    void setHeaderIcon(const p2str_t & icon);
    void setHeaderText(const p2str_t & text);
    void setHeader(const p2str_t & icon, const p2str_t & text);


    void setLeftSoft(const p2str_t & text, int func(LForm *, void *), const void *ud = 0);
    void setRightSoft(const p2str_t & text, int func(LForm *, void *), const void *ud = 0);
    //void setCenterSoft(const p2str_t & text, int func(LForm *, void *), void *ud = 0);

    void setKeyHook(int (*func)(LForm *, void *data, GUI_MSG *msg));
    void setItemHook(void (*func)(LForm *, void *data, int curitem, void*));

    void setOwnReturnAtKey(int ret, bool is_enable = true);
    void flush();
    void clear(bool is_flush = false);
    void close(bool delete_later = false);
    void redraw();


    void bindKey(int type, int key, int func(LForm *, int, void*), void *ud = 0);
    void bindKey(int type, int key, const item_signal::slot_type & slot, void *ud = 0);
    void unbindKey(int type, int key);


    static Item * newItem(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                          const p2str_t & llgp, int func(LForm *, int, void*), const void *ud = 0);

    static Item * newItem(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                          const p2str_t & llgp, const item_signal & it, const void *ud = 0);

    static Item * newItem(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                          const p2str_t & llgp, const item_signal::slot_type & slot, const void *ud = 0);

private:

    int constructor();
    void setup_header();
    void set_soft_key(short k1, short k2, const p2str_t & text, uint32_t keynum);
    void constructor_of_items(void *data, int curitem);
    void erase(Item *it);

private:
    HEADER_DESC menuhdr; // Заголовок
    SOFTKEY_DESC menu_sk[3]; // Подписывание кнопок
    SOFTKEYSTAB menu_skt;
    ML_MENU_DESC ml_menudest;
    MENU_DESC menudest;
    int menusoftkeys[3];
    GUI *dataGUI;
    int _gui_id;
    int menu_type, return_at_key;
    bool small_mode, own_return_at_key;

    struct
    {
        p2str_t text;
        p2str_t icon;
    } _header;

    SoftData left_soft, right_soft, center_soft;
    std::map <int, void *> _userData;
    std::vector <Item *> items;
    std::map <int, std::map <int, key_binder > > _keybind;
    int (*_key_hook)(LForm *, void *data, GUI_MSG *msg);
    void (*_items_hook)(LForm *, void *data, int curitem, void*);
    static p2str_t p2str_null;

public:
    //void (*PointOnMenuItems)(void *data, int curitem, void *user_pointer);
    static const int MultiLine = 0xAF;
    static const int Line = 0xBF;

    static void _menu_items_hook(void *data, int curitem, void*);
    static int _menu_on_key(void *data, GUI_MSG *msg);

    static const int LEFTSOFT = (int)0x0018;
    static const int RIGHTSOFT = (int)0x0001;
    static const int ENTERKEY = (int)0x003D;


private:

};





#endif
