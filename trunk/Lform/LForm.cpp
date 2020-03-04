
#include "LForm.h"

#define UNUSED(x) ((void)x);

static void delete_later_helper(LForm *del_me)
{
    if(!del_me) return;
    delete del_me;
}


template <class Map>
bool map_isexists(const Map& m, const typename Map::key_type& key)
{
    return m.find(key) != m.end();
}


p2str_t LForm::p2str_null = 0;

int LForm::_menu_on_key(void *data, GUI_MSG *msg)
{
    int key = msg->gbsmsg->submess;
    int type = msg->gbsmsg->msg;
    int ret = 0;

    UNUSED(key)
    UNUSED(type)

    LForm * _this = (LForm *)MenuGetUserPointer(data);
    if(!_this) return 0;


    if(map_isexists(_this->_keybind[type], key))
    {
        if(_this->_keybind[type][key].proc)
            return _this->_keybind[type][key].proc(_this, key, (void *)_this->_keybind[type][key].ud);
        else
            return _this->_keybind[type][key].sig.emit(_this, key, (void *)_this->_keybind[type][key].ud);
    }


    if(_this->_key_hook)
        return _this->_key_hook(_this, data, msg);

    if(msg->keys == LEFTSOFT || msg->keys == ENTERKEY)
    {
        ret = _this->doCurrentItemFunc();
        return ret;
    }

    return ret;
}


void LForm::_menu_items_hook(void *data, int curitem, void *un)
{
    LForm * _this = (LForm *)MenuGetUserPointer(data);
    if(!_this) {
        return;
    }

    _this->constructor_of_items(data, curitem);
    if(_this->_items_hook)
        _this->_items_hook(_this, data, curitem, un);
}



/** \brief LForm constructor
 *
 * \param type = Line || MultiLine
 * \return *this
 *
 */
LForm::LForm(int type)
{
    menu_type = type;
    constructor();
}



LForm::~LForm()
{
    close();
    clear();
    _userData.clear();
}



/** \brief Set default structures
 *
 * \return 0 || -1
 *
 */
int LForm::constructor()
{
    /* зарезервируем для хедера место */
    _header.text.reserve(128);

    /* реаллочить нельзя */
    _header.text.noRealloc(true);


    setup_header();
    small_mode = false;
    own_return_at_key = false;
    _key_hook = 0;
    _items_hook = 0;
    return_at_key = 0;
    menu_skt.n = 0;
    menu_skt.desc = menu_sk;

    /* soft signals */
    menusoftkeys[0] = 0;
    menusoftkeys[1] = 1;
    menusoftkeys[2] = 2;

    set_soft_key(LEFTSOFT, 0x0000, "Выбор", 0);
    set_soft_key(RIGHTSOFT, 0x0000, "Назад", 1);
    set_soft_key(ENTERKEY, 0x0000, (unsigned int)LGP_DOIT_PIC, 2);

    menudest.flag = 8;
    menudest.onkey = this->_menu_on_key;
    menudest.ghook = 0;
    menudest.proc3 = NULL;
    menudest.softkeys = menusoftkeys;
    menudest.softkeystab = &menu_skt;
    menudest.flags2 = 0x11;
    menudest.itemproc = this->_menu_items_hook;
    menudest.items = 0;
    menudest.procs = 0;
    menudest.n_items = 0;

    /* лайновы и мультилайновые структуры почти идентичны так что можно копировать*/
    memcpy(&ml_menudest, &menudest, sizeof(MENU_DESC));

    ml_menudest.n_lines = 1;
    dataGUI = 0;

    return 0;
}



/** \brief Set header
 *
 */
void LForm::setup_header()
{
    _header.text = "LForm";
    _header.icon = 0;
    menuhdr.icon = (int *)(_header.icon.datai());
    menuhdr.lgp_id = _header.text.datai();
    menuhdr.lgp_null = LGP_NULL;

    menuhdr.rc.x = 0;
    menuhdr.rc.y = YDISP;
    menuhdr.rc.x2 = ScreenW()-1;
    menuhdr.rc.y2 = HeaderH()-YDISP;
}



/** \brief Set soft key parameters
 *
 * \param k1 - Signal
 * \param k2 - Signal
 * \param text - Text
 * \param keynum: 0 - left, 1 - right, 2 - center
 * \return
 *
 */
void LForm::set_soft_key(short k1, short k2, const p2str_t & text, uint32_t keynum)
{
    menu_sk[keynum].lgp_id = LGP_NULL;
    switch(keynum)
    {
        case 0:
            left_soft.text = text;
            menu_sk[keynum].lgp_id = left_soft.text.datai();
            break;

        case 1:
            right_soft.text = text;
            menu_sk[keynum].lgp_id = right_soft.text.datai();
            break;

        case 2:
            center_soft.text = text;
            menu_sk[keynum].lgp_id = center_soft.text.datai();
            break;

        default:
            menu_sk[keynum].lgp_id = text.datai();
            break;
    }

    menu_sk[keynum].key1 = k1;
    menu_sk[keynum].key2 = k2;
}




/** \brief Set left soft attribute
 *
 * \param text - soft text
 * \param func - call back for press
 * \param ud - user data
 *
 */

void LForm::setLeftSoft(const p2str_t & text, int func(LForm *, void *), const void *ud)
{
    left_soft.text = text;
    left_soft.proc = func;
    left_soft.ud   = ud;
}




/** \brief Set right soft attribute
 *
 * \param text - soft text
 * \param func - call back for press
 * \param ud - user data
 *
 */
void LForm::setRightSoft(const p2str_t & text, int func(LForm *, void *), const void *ud)
{
    right_soft.text = text;
    right_soft.proc = func;
    right_soft.ud   = ud;
}



/** \brief Set header icon
 *
 * \param icon - icon pack num or way to fs
 *
 */

void LForm::setHeaderIcon(const p2str_t & icon)
{
    _header.icon = icon;
    menuhdr.icon = (int *)(_header.icon.datai());
}



/** \brief Set header text
 *
 * \param text - lgp id or ptr to text
 *
 */

void LForm::setHeaderText(const p2str_t & text)
{
    _header.text = text;
    menuhdr.lgp_id = _header.text.datai();
}



/** \brief Set header icon && text
 *
 * \param icon - icon var
 * \param text - text var
 *
 */

void LForm::setHeader(const p2str_t & icon, const p2str_t & text)
{
    setHeaderIcon(icon);
    setHeaderText(text);
    menuhdr.icon = (int *)(_header.icon.datai());
    menuhdr.lgp_id = _header.text.datai();
}



/** \brief get items count
 *
 * \return items count
 *
 */
size_t LForm::size()
{
    return items.size();
}



/** \brief get item by num
 *
 * \param item - number item
 * \return item data
 *
 */
LForm::Item * LForm::item(int item)
{
    if( size() <= (size_t)item ) return 0;

    return items.at(item);
}



/** \brief construct menu items text and icons
 *
 * \param data - gui data
 * \param curitem - current item, it we must set
 *
 */
void LForm::constructor_of_items(void *data, int curitem)
{
    WSHDR *ws=0, *ws1=0;
    void *item = 0;

    /* setup data */
    dataGUI = (GUI*)data;


    Item *it = this->item(curitem);
    if(!it) {

        return;
    }

    switch( menu_type )
    {
        case Line:
            item = AllocMenuItem(data);
            break;

        case MultiLine:
            item = AllocMLMenuItem(data);
            if( it->litem.is_physical() )
            {
                ws1  = AllocMenuWS(data, it->litem.length()+1);
                wsprintf(ws1, "%t", it->litem.datac());
            } else {
                ws1  = AllocMenuWS(data, 24);
                wsprintf(ws1, "Error: %d", __LINE__);
            }
            break;
    }


    ws = AllocMenuWS(data, it->uitem.length());
    wsprintf(ws, "%t", it->uitem.datac());

    /* set icon */
    int icon = it->icon_p.datai();

    if(icon)
        SetMenuItemIconArray(data, item, &icon);


    switch( menu_type )
    {
        case Line:
            SetMenuItemText(data, item, ws, curitem);
            break;

        case MultiLine:
            SetMLMenuItemText(data, item, ws, ws1, curitem);
            break;
    }
}



/** \brief set items count
 *
 * \param count - item count
 *
 */
void LForm::setItemCount(int count)
{
    if(!dataGUI) return;

    int i = GetCurMenuItem(dataGUI), j = count;
    Menu_SetItemCountDyn(dataGUI, j);
    if (i>=j && j)
        SetCursorToMenuItem(dataGUI,j-1);
}



/** \brief get item count
 *
 */
int LForm::currentItem()
{
    if(!dataGUI) return -1;
    return GetCurMenuItem(dataGUI);
}



/** \brief set user data
 *
 * \param data - user data
 *
 */
void LForm::setUserData(void *data, size_t group)
{
    _userData[group] = data;
}



/** \brief get user data
 *
 */
void *LForm::userData(size_t group)
{
    return _userData[group];
}



/** \brief Show menu
 *
 * \return gui id
 *
 */
int LForm::show()
{
#ifdef NEWSGOLD
    dataGUI = 0;

    void *ma = malloc_adr(), *gui = 0;

    switch(menu_type)
    {
        case Line:
            gui = GetMenuGUI(ma, mfree_adr());
            SetMenuToGUI(gui, &menudest);
            break;

        case MultiLine:
            gui = GetMultiLinesMenuGUI(ma, mfree_adr());
            SetMenuToGUI(gui, &ml_menudest);
            break;
    }

    SetHeaderToMenu(gui, &menuhdr, ma);

    SetMenuItemCount(gui, size());

    MenuSetUserPointer(gui, this);

    dataGUI = (GUI*)gui;

    if(!small_mode)
        _gui_id = CreateGUI(gui);
    else
        _gui_id = CreateGUI_30or2(gui);

    return _gui_id;
#else

    ShowMSG(1, (int)"SGOLD not support!");
    return -1;
#endif
}





/** ================ Append/remove/insert - edit list API =================== **/


/** \brief Create new item data
 *
 * \param ud - user data
 * \return item data
 *
 */

LForm::Item * LForm::newItem(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                             const p2str_t & llgp, int func(LForm *, int, void*), const void *ud)
{
    Item *it = new Item();

    it->icon_p = uicon;
    it->lower_icon_p = licon;
    it->uitem = ulgp;
    it->litem = llgp;
    it->proc = func;
    it->ud = ud;

    return it;
}


LForm::Item * LForm::newItem(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                             const p2str_t & llgp, const item_signal & sig_it, const void *ud)
{
    Item *it = new Item();

    it->icon_p = uicon;
    it->lower_icon_p = licon;
    it->uitem = ulgp;
    it->litem = llgp;
    it->proc = 0;
    it->sig = sig_it;
    it->ud = ud;
    return it;
}


LForm::Item * LForm::newItem(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                             const p2str_t & llgp, const item_signal::slot_type & slot, const void *ud)
{
    Item *it = new Item();
    item_signal sig;
    sig.connect(slot);

    it->icon_p = uicon;
    it->lower_icon_p = licon;
    it->uitem = ulgp;
    it->litem = llgp;
    it->proc = 0;
    it->sig = sig;
    it->ud = ud;
    return it;
}


/** \brief Delete item data
 *
 * \param it - item data
 *
 */

void LForm::erase(LForm::Item *it)
{
    if(!it) return;
    delete it;
}



/** \brief Push back item to end
 *
 * \param icon - icon
 * \param lgp - string || lgp id
 * \param func - call back for item select
 * \param ud - user data
 *
 */
void LForm::append(const p2str_t & icon, const p2str_t & lgp, int func(LForm *, int, void*), const void *ud)
{
    Item * it = newItem(icon, 0, lgp, "", func, ud);
    items.push_back(it);
}


void LForm::append(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                   const p2str_t & llgp, int func(LForm *, int, void*), const void *ud)
{
    Item * it = newItem(uicon, licon, ulgp, llgp, func, ud);
    items.push_back(it);
}


void LForm::append(const p2str_t & icon, const p2str_t & ulgp, const item_signal::slot_type & slot, const void *ud)
{
    Item * it = newItem(icon, 0, ulgp, "", slot, ud);
    items.push_back(it);
}


void LForm::append(const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp, const p2str_t & llgp,
                   const item_signal::slot_type & slot, const void *ud)
{
    Item * it = newItem(uicon, licon, ulgp, llgp, slot, ud);
    items.push_back(it);
}


void LForm::append(const p2str_t & lgp)
{
    Item * it = newItem(0, 0, lgp, "", 0, 0);
    items.push_back(it);
}


void LForm::append(const p2str_t & ulgp, const p2str_t & llgp)
{
    Item * it = newItem(0, 0, ulgp, llgp, 0, 0);
    items.push_back(it);
}


/** \brief Insert item it own position
 *
 * \param
 * \param
 * \return
 *
 */

void LForm::insert(int item, const p2str_t & icon, const p2str_t & lgp, int func(LForm *, int, void*), const void *ud)
{
    if(size() < (size_t)item) return;

    Item * it = newItem(icon, 0, lgp, "", func, ud);
    items.insert(items.begin() + item, it);
}

/** Overload
*/
void LForm::insert(int item, const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                   const p2str_t & llgp, int func(LForm *, int, void*), const void *ud)
{
    if(size() < (size_t)item) return;

    Item * it = newItem(uicon, licon, ulgp, llgp, func, ud);
    items.insert(items.begin() + item, it);
}


void LForm::insert(int item, const p2str_t & icon, const p2str_t & lgp, const item_signal::slot_type & slot, const void *ud)
{
    if(size() < (size_t)item) return;

    Item * it = newItem(icon, 0, lgp, "", slot, ud);
    items.insert(items.begin() + item, it);
}


void LForm::insert(int item, const p2str_t & uicon, const p2str_t & licon, const p2str_t & ulgp,
                   const p2str_t & llgp, const item_signal::slot_type & slot, const void *ud)
{
    if(size() < (size_t)item) return;

    Item * it = newItem(uicon, licon, ulgp, llgp, slot, ud);
    items.insert(items.begin() + item, it);
}


void LForm::insert(int item, const p2str_t & lgp)
{
    if(size() < (size_t)item) return;

    Item * it = newItem(0, 0, lgp, "", 0, 0);
    items.insert(items.begin() + item, it);
}


void LForm::insert(int item, const p2str_t & ulgp, const p2str_t & llgp)
{
    if(size() < (size_t)item) return;

    Item * it = newItem(0, 0, ulgp, llgp, 0, 0);
    items.insert(items.begin() + item, it);
}


/** \brief Remove item by position
 *
 * \param item - item to remove
 *
 */

void LForm::remove(int item)
{
    if(size() <= (size_t)item) return;

    Item *it = this->item(item);

    if(it)
        delete it;
    items.erase(items.begin() + item);
}




/** \brief Run call back or send signal of current item
 *
 */

int LForm::doCurrentItemFunc()
{
    return doItemFunc(currentItem());
}



/** \brief Run call back or send signal of item
 *
 * \param item - number of item
 *
 */

int LForm::doItemFunc(int item)
{
    Item *it = this->item(item);
    if(!it) return 0;

    if(it->proc)
        return it->proc(this, item, (void *)it->ud);

    return it->sig.emit(this, item, (void *)it->ud);
}



/** \brief While finish edit call to this function, it make changes is actualy
 *
 *
 */

void LForm::flush()
{
    setItemCount(size());
}



void LForm::setKeyHook(int (*func)(LForm *, void *data, GUI_MSG *msg))
{
    _key_hook = func;
}


void LForm::setItemHook(void (*func)(LForm *, void *data, int curitem, void*))
{
    _items_hook = func;
}


/** \brief Setreturn value for return at key function
 *
 * \param ret - return value
 * \param is_enable - enable or disable own return(true || false). Default == true
 *
 */

void LForm::setOwnReturnAtKey(int ret, bool is_enable)
{
    own_return_at_key = is_enable;
    return_at_key = ret;
}



/** \brief Close gui
 *
 * \param delete_later - delete class later, default = false
 *
 */
void LForm::close(bool delete_later)
{
    if(!dataGUI) return;

    GeneralFunc_flag1(this->gui_id(), 1);
    //GBS_SendMessage(MMI_CEPID, KEY_DOWN, 0xFDEAD);
    dataGUI=0;

    if(delete_later)
        SUBPROC(delete_later_helper, this);
}



/** \brief Clear all items
 *
 * \param is_flush - apply changes
 *
 */

void LForm::clear(bool is_flush)
{
    size_t sz = items.size();
    for(size_t i = 0; i<sz; ++i) {

        Item *it = items[i];
        if(it)
            delete items[i];
    }
    items.clear();

//#warning think what to do
    if(is_flush)
        flush();
}



/** \brief Redraw ui
 *
 */

void LForm::redraw()
{
    //REDRAW();
    RefreshGUI();
}



/** \brief Set vector array of items, flushed at other time. At delete class it free all members of vector
 *
 * \param v - items vector
 * \param fl - flush. Default == false
 *
 */

void LForm::setVectorOfItems(const std::vector <Item *> & v, bool fl)
{
    items = v;
    if(fl)
        flush();
}



/** \brief Set upper item text
 *
 * \param item - item number
 * \param text - text for item
 *
 */

void LForm::setItemUpperText(int item, const p2str_t & text)
{
    Item *it = this->item(item);
    if(!it) return;

    it->uitem = text;
}



/** \brief Set lower item text
 *
 * \param item - item number
 * \param text - text for item
 *
 */

void LForm::setItemLowerText(int item, const p2str_t & text)
{
    Item *it = this->item(item);
    if(!it) return;

    it->litem = text;
}



/** \brief Set upper item icon
 *
 * \param item - item number
 * \param icon - icon for item
 *
 */

void LForm::setItemUpperIcon(int item, const p2str_t & icon)
{
    Item *it = this->item(item);
    if(!it) return;

    it->icon_p = icon;
}



/** \brief Set lower item icon
 *
 * \param item - item number
 * \param icon - icon for item
 *
 */

void LForm::setItemLowerIcon(int item, const p2str_t & icon)
{
    Item *it = this->item(item);
    if(!it) return;

    it->icon_p = icon;
}



/** \brief Set item action
 *
 * \param item - item number
 * \param func - action
 *
 */

void LForm::setItemAction(int item, int func(LForm *, int, void*))
{
    Item *it = this->item(item);
    if(!it) return;

    it->proc = func;
}



/** \brief Set item action
 *
 * \param item - item number
 * \param slot - slot for connect action
 *
 */

void LForm::setItemAction(int item, const item_signal::slot_type & slot)
{
    Item *it = this->item(item);
    if(!it) return;

    item_signal sig;
    sig.connect(slot);
    it->sig = sig;
}



/** \brief Set item user data
 *
 * \param item - item number
 * \param ud - user data
 *
 */

void LForm::setItemUserData(int item, void *ud)
{
    Item *it = this->item(item);
    if(!it) return;

    it->ud = ud;
}




/** \brief Bind action for key pressed
 *
 * \param type - type press
 * \param key - key press
 * \param func - action
 * \param ud - user data
 *
 */

void LForm::bindKey(int type, int key, int func(LForm *, int, void*), void *ud)
{
    //_keybind[type][key] = func;
    _keybind[type][key] = key_binder(func, ud);
}



/** \brief Bind action for key pressed
 *
 * \param type - type press
 * \param key - key press
 * \param slot - slot action fol signal
 * \param ud - user data
 *
 */

void LForm::bindKey(int type, int key, const item_signal::slot_type & slot, void *ud)
{
    _keybind[type][key] = key_binder(slot, ud);
}



/** \brief Unbind action from key
 *
 * \param type - type press
 * \param key - key press
 *
 */

void LForm::unbindKey(int type, int key)
{
    _keybind[type].erase(key);
}



const p2str_t & LForm::itemUpperText(int item)
{
    Item *it = this->item(item);
    if(!it) return p2str_null;

    return it->uitem;
}


const p2str_t & LForm::itemLowerText(int item)
{
    Item *it = this->item(item);
    if(!it) return p2str_null;

    return it->litem;
}


const p2str_t & LForm::itemUpperIcon(int item)
{
    Item *it = this->item(item);
    if(!it) return p2str_null;

    return it->icon_p;
}


const p2str_t & LForm::itemLowerIcon(int item)
{
    Item *it = this->item(item);
    if(!it) return p2str_null;

    return it->lower_icon_p;
}


void * LForm::setItemAction_f(int item)
{
    Item *it = this->item(item);
    if(!it) return 0;

    return (void *)it->proc;
}


const LForm::item_signal *LForm::itemAction_s(int item)
{
    Item *it = this->item(item);
    if(!it) return 0;

    return &it->sig;
}


void * LForm::itemUserData(int item)
{
    Item *it = this->item(item);
    if(!it) return 0;

    return (void *)it->ud;
}










