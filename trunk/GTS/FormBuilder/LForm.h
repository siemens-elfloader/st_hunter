


#ifndef __LFORM_H__
#define __LFORM_H__

#include <swilib.h>

class LForm
{
public:
   LForm(int);
   ~LForm();

private:

   void (*LItemsFunc)(LForm *, void *data, int curitem);
   int (*LOnKey)(LForm*,void *data, GUI_MSG *msg);

   bool StandartKey;
   int return_after_prees;
   char *headericon;
   int ICONS[512];
   int typeMenuShow;
   int listcount;
   HEADER_DESC menuhdr; // Заголовок
   SOFTKEY_DESC menu_sk[3]; // Подписывание кнопок
   SOFTKEYSTAB menu_skt;
   ML_MENU_DESC ml_menudest;
   MENU_DESC menudest;
   int menusoftkeys[3];
   GUI *dataGUI;
   char IsSmallMenu;
   int gui_id;

public:

   struct fListItems
   {
      int (*proc)();
      char *item;
      char *ditem;
      //bool isAlloc;
      char *IconPath;
   }ListItems[256];

   // Функции

   int RunItemProcById(int id);
   int GetListCount(){return listcount;}
   void SetSoftKey(short k1, short k2, int lgp, char keynum);
   void SetMitliStringText(int item, char *text);
   int Show();
   void PathHeadFormSmallMenu();
   void PathHeadFormNormalMenu();
   void RedrawGUI();
   int GetNamberOfItems() { return listcount;}
   int GetCurentMenuItem();
   void SetSmallMenu(bool s);
   int GetGUIId(){return this->gui_id;};
   void SetMenuItemNumber(int count);
   void AddItemElement(const char * icon, const char *lgp, int func());
   void AddItemElement(const char * icon, const char *uplgp, const char *downlgp, int func());
   void SetHeader(int recx, int rect, int recw, int rech, const char *icon, int lgp);
   void SetHeader(const char *icon, const char *lgp);
   void SetKeyListener(int (*onkey)(LForm *,void *, GUI_MSG *));
   void SetGuiHook(void (*ghook)(LForm * ,void *, int ));
   void ReSetItem(int item, char * icon, int lgp, int func());
   void ReSetItem(int item, char * icon, char *uplgp, char *dlgp, int func());
   void MenuItems(void *data, int curitem);
   void PathHeadString(int recx, int recy, int recw, int rech);
   void GuiHookSetup(void*);
   void CloseGui();
   void SetStandartKey(bool d){StandartKey=d;}
   void setReturnAfterPress(int is){return_after_prees= is;}
   int DoCurentItemProc();


   //void (*PointOnMenuItems)(void *data, int curitem, void *user_pointer);
   static const int MultiLine = 0xAF;
   static const int Line = 0xBF;

   static void OnMenuItemsHook(void *data, int curitem, void*);
   static int MenuOnKey(void *data, GUI_MSG *msg);

   static const int LEFTSOFT = (int)0x0018;
   static const int RIGHTSOFT = (int)0x0001;
   static const int ENTERKEY = (int)0x003D;
};





#endif
