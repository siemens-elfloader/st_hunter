



#ifndef __EFORM_H__
#define __EFORM_H__


#include "LForm.h"

class EForm
{
public:

   EForm(int (*onKey)(EForm *, GUI *, GUI_MSG *), void (*global_hook_proc)(EForm *, GUI *, int));
   EForm();
   ~EForm();

private:

   int Alloc(int (*onKey)(EForm *,GUI *, GUI_MSG *), void (*global_hook_proc)(EForm *, GUI *, int));
   int (*onKeyHook)(EForm *, GUI *, GUI_MSG *);
   void (*GUIHook)(EForm *, GUI *, int);
   char *headericon;
   CSM_RAM csmr, *csm_ram;
   WSHDR *ews;
   int typeMenuShow;
   int listcount;
   SOFTKEY_DESC ef_sk[3]; // Подписывание кнопок
   SOFTKEYSTAB ef_skt;
   HEADER_DESC efhdr; // Заголовок
   INPUTDIA_DESC input_desc;
   int efsoftkeys[3];
   void (*FormHook)(EForm *);
   int gui_id;
   int EwsSize;
   GUI *edata;
   EDITC_OPTIONS edit_opt;
   void *rightsoft, *leftsoft;
   bool StandartHook;

   struct sEForm {
      WSHDR *ws;
      void *eq;
      void *ma;
   }eform;

public:


   void RemoveEditControl(int item);
   void InsertEditControl(char *name,int item,int type, int size);
   void SetOnGUIHook(GUI *, int);
   void SetTextToItem(int item, const char *text);
   void SetUtf8TextToItem(int item, const char *text);

   void SetHeader(int recx, int rect, int recw, int rech, char *icon, int lgp);
   void SetHeader(char *icon, int lgp);
   void SetupSoftKey(short k1, short k2, int lgp, char keynum);
   void SetLeftSoft(int lgp);
   void SetRightSoft(int lgp);
   void SetLeftSoft(int lgp, void *);
   void SetRightSoft(int lgp, void *);
   void SetHookFormBuilder(void func(EForm *)){FormHook = func;}
   void AddFormElement(const char *element, int type, int);
   void AddFormElement(const char *element, int type, int size, int elementnum);
   void Extract(int element, WSHDR *buff, int);
   bool IsItemEmpty(int item);
   void GetEditControlData(void *data);
   WSHDR * GetWsHdr() { return ews;};
   void Clean();
   void SetFocusTo(int element);
   void SetCursorTo(int pos);
   int GetCursorPos();
   int GetFocusItem();
   void SetTextToFocusedItem(WSHDR *ws);
   int GetFocusedItemNum();
   void AppendTextToFocisedItem(WSHDR *ws);
   void ReallocEws(int newsize);
   void Clear(int element);
   int Show();
   void RedrawGUI();
   bool IsMarkedText();
   void GetMarkedText(WSHDR *ws);
   void close();
   int GetGuiId();
   void SetEditControlAtrib(int a);
   void SetShowEditControlAtrib(int s);
   void SetStandartKeyState(bool b){StandartHook=b;}
   void SetKeyListener(int key(EForm *,GUI *, GUI_MSG *));
   int GetGUIId(){return gui_id;}

   // опции

   void SetFont(int font);
   void SetColor(int pen, int brush);
   void SetAlign(int align);
   void ApplySetingControl(EDITCONTROL *ec);

   static void inp_locret(void){}

   static const int ReadOnly = 0;
   static const int NormalText = 1;
   static const int EnumEnter = 2;
   static const int TextEnter = 3;
   static const int ReadOnlyCursor = 4;
   static const int FixEnumEnter = 5;
   static const int UnsignedEnumEnter = 6;
   static const int ComboBox = 7;
   static const int SelectType = 8;
   static const int ChoiceSelectType = 9;

   static void EFormGUIHook(GUI *gui, int cmd);
   static int EFormOnKey(GUI *gui, GUI_MSG *msg);
};



#endif
