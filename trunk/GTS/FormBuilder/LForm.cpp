//@ (c) Z.Vova

#include <stdlib.h>
#include <swilib.h>
#include "LForm.h"


/* иаровские замашки */
/* :lol: смотря в сторону gcc */
const char prsz[] = "%s\0";


int ZeroFuntion(){ return -23;}
char *empty()
{
   char *r = (char*)malloc(1);
   r[0]=0;
   return r;
}


void LForm::OnMenuItemsHook(void *data, int curitem, void *user_item)
{
   LForm * i = (LForm *)MenuGetUserPointer(data);

    if(!i) return;

   i->MenuItems(data, curitem);
   if(i->LItemsFunc)  i->LItemsFunc(i,data, curitem);

}


int LForm::MenuOnKey(void *data, GUI_MSG *msg)
{
   int ret=0;
   LForm * i = (LForm *)MenuGetUserPointer(data);
   if(!i) goto user_break;

   if(i->LOnKey)  ret=i->LOnKey(i,data, msg);

user_break:

   if(i->StandartKey)
   {
      switch(msg->keys)
      {
      case 0x0018:
      case 0x003D:
         ret = i->DoCurentItemProc();

         return ret;
         break;
      }
   }
   return ret;
}

LForm :: LForm(int type)
{
   zeromem(ListItems, sizeof(fListItems)*256);

   typeMenuShow = type;
   headericon =0;


   StandartKey = true;
   return_after_prees = 1;
   LOnKey = 0,LItemsFunc=0;

   IsSmallMenu = 0;
   listcount = 0;
   menusoftkeys[0] = 0;
   menusoftkeys[1] = 1;
   menusoftkeys[2] = 2;

   SetHeader(0,YDISP,ScreenW()-1, HeaderH()+YDISP, 0,(int)"Меню");

   SetSoftKey(LEFTSOFT, 0x0000, (int)"Выбор", 0);
   SetSoftKey(RIGHTSOFT, 0x0000, (int)"Назад", 1);
   SetSoftKey(0x003D, 0x0000, (int)LGP_DOIT_PIC, 2);

   menu_skt.n = 0;
   menu_skt.desc = menu_sk;

   menudest.flag = 8;
   menudest.onkey = MenuOnKey;
   menudest.ghook = 0;
   menudest.proc3 = NULL;
   menudest.softkeys = menusoftkeys;
   menudest.softkeystab = &menu_skt;
   menudest.flags2 = 0x11;
   menudest.itemproc = this->OnMenuItemsHook;
   menudest.items = 0;
   menudest.procs = 0;
   menudest.n_items = 0;


   memcpy(&ml_menudest, &menudest, sizeof(MENU_DESC));

   ml_menudest.n_lines = 1;
}


LForm :: ~LForm()
{
   for(int i=0;i<256; i++)
   {
      if(ListItems[i].item)
      delete ((char*)ListItems[i].item);
      if(ListItems[i].ditem)
      delete ((char*)ListItems[i].ditem);

      if(ListItems[i].IconPath)
      delete ListItems[i].IconPath;
   }

   if(headericon)
      delete headericon;
}


void LForm :: SetSoftKey(short k1, short k2, int lgp, char keynum)
{
   menu_sk[(size_t)keynum].key1 = k1;
   menu_sk[(size_t)keynum].key2 = k2;
   menu_sk[(size_t)keynum].lgp_id = lgp;
}

#ifdef NEWSGOLD
int LForm :: Show()
{

   dataGUI=0;

   void *ma=malloc_adr(), *gui = 0;

   if(typeMenuShow == Line)
   {
      gui=GetMenuGUI(ma, mfree_adr());
      SetMenuToGUI(gui, &menudest);
   } else if(typeMenuShow == MultiLine)
   {
      gui=GetMultiLinesMenuGUI(ma, mfree_adr());
      SetMenuToGUI(gui, &ml_menudest);
   }

   SetHeaderToMenu(gui, &menuhdr, ma);

   SetMenuItemCount(gui, listcount);

   MenuSetUserPointer(gui, this);

   dataGUI = (GUI*)gui;

   if(typeMenuShow == MultiLine || typeMenuShow == Line)
   {
      if(!IsSmallMenu)   gui_id = CreateGUI(gui);
      else   gui_id = CreateGUI_30or2(gui);
   }

   return gui_id;
}
#else

int LForm :: Show()
{
    dataGUI=0;
     if(typeMenuShow == Line)
     {
         gui_id = CreateMenu(0, 0, &menudest, &menuhdr, 0, GetNamberOfItems(), this, 0);

     } else if(typeMenuShow == MultiLine)
     {
         MsgBoxError(1, (int)"Not suported!");
     }

     return gui_id;
}
#endif


void LForm :: AddItemElement(const char * icon, const char * lgp, int func())
{
   if(ListItems[listcount].item)
      mfree((char*) ListItems[listcount].item);
   if(ListItems[listcount].ditem)
      mfree((char*) ListItems[listcount].ditem);
   ListItems[listcount].item = 0;
   ListItems[listcount].ditem = 0;

   char *lgpc =new char[strlen((char*)lgp)+2];
   sprintf(lgpc, prsz, lgp);
   ListItems[listcount].item = (char*)lgpc;

   ListItems[listcount].ditem = (char*)empty();

   if(ListItems[listcount].IconPath) delete ListItems[listcount].IconPath;

   ListItems[listcount].IconPath = new char[strlen((char*)icon)+1];
   strcpy(ListItems[listcount].IconPath, (char*)icon);
   if(!func)
   ListItems[listcount++].proc = ZeroFuntion;
   else
   ListItems[listcount++].proc = func;
}

void LForm :: AddItemElement(const char * icon, const char * uplgp, const char * downlgp, int func())
{
   if(ListItems[listcount].item)
      mfree((char*) ListItems[listcount].item);
   if(ListItems[listcount].ditem)
      mfree((char*) ListItems[listcount].ditem);
   ListItems[listcount].item = 0;
   ListItems[listcount].ditem = 0;
   char *lgpc = 0;
   if(!uplgp)
   {
      ListItems[listcount].item = (char*)empty();
   }
   else
   {
        lgpc =new char[strlen((char*)uplgp)+2];
        sprintf(lgpc, prsz, uplgp);
        ListItems[listcount].item = (char*)lgpc;
        lgpc = 0;
   }
   if(!downlgp)
   {
      ListItems[listcount].ditem = (char*)malloc(1);
   }
   else
   {
        lgpc = new char[strlen((char*)downlgp)+2];
        sprintf(lgpc, prsz, downlgp);
        ListItems[listcount].ditem = (char*)lgpc;
   }

   if(ListItems[listcount].IconPath) delete ListItems[listcount].IconPath;

   ListItems[listcount].IconPath = new char[strlen((char*)icon)+1];

   strcpy(ListItems[listcount].IconPath, (char*)icon);
   if(!func)
   ListItems[listcount++].proc = ZeroFuntion;
   else
   ListItems[listcount++].proc = func;
}


void LForm :: SetHeader(int recx, int recy, int recw, int rech, const char *icon, int lgp)
{
   if((int)icon != -1 && icon!=0)
   {
      if(headericon) delete headericon;
      headericon = new char[strlen((char*)icon)+2];
      strcpy(headericon, (char*)icon);
      menuhdr.icon = (int*)&headericon;
   }
   if((int)icon == 0)
   {
      if(headericon)
      delete headericon;
         headericon=0;
      menuhdr.icon=0;
   }

   menuhdr.rc.x = recx;
   menuhdr.rc.y = recy;
   menuhdr.rc.x2 = recw;
   menuhdr.rc.y2 = rech;
   menuhdr.lgp_id = lgp;
   menuhdr.lgp_null = LGP_NULL;
}

void LForm :: PathHeadString(int recx, int recy, int recw, int rech)
{
   if(recx !=-1)
      menuhdr.rc.x = recx;
   if(recy !=-1)
      menuhdr.rc.y = recy;
   if(recw !=-1)
      menuhdr.rc.x2 = recw;
   if(rech !=-1)
      menuhdr.rc.y2 = rech;
}

void LForm :: SetHeader(const char *_icon, const char *lgp)
{
   if((int)_icon != -1 && _icon!=0)
   {
      if(headericon) delete headericon;
      headericon = new char[strlen((char*)_icon)+2];
      sprintf(headericon, "%s",(char*)_icon);
      menuhdr.icon = (int*)&headericon;
   }
   if((int)_icon == 0)
   {
      if(headericon)
      delete headericon;
         headericon=0;
      menuhdr.icon=0;
   }

   if((int)lgp!=-1)
   menuhdr.lgp_id = (int)lgp;
   menuhdr.lgp_null = LGP_NULL;
}


void LForm :: SetKeyListener(int (*onkey)(LForm *,void *, GUI_MSG *))
{
   LOnKey = onkey;
}


void LForm :: SetGuiHook(void (*ghook)(LForm *, void *, int ))
{
   LItemsFunc = ghook;
}

void LForm :: ReSetItem(int item, char * icon, int lgp, int func())
{
   item--;
   if(item<1 && item >= listcount) return;

   if((int)icon != -1)
   {
      if(ListItems[listcount].IconPath) delete ListItems[listcount].IconPath;
      ListItems[listcount].IconPath = new char[strlen((char*)icon)+1];
      strcpy(ListItems[item].IconPath, (char*)icon);
   }
   if(lgp != -1)
      ListItems[item].item = (char*)lgp;
   if((int)func != -1)
   {
      if(!func)
         ListItems[item].proc = ZeroFuntion;
      else
         ListItems[item].proc = func;
   }

   if(!dataGUI) return;

   if(IsGuiOnTop(gui_id))
      RedrawGUI();
}

void LForm :: ReSetItem(int item, char * icon, char *uplgp, char *dlgp, int func())
{
   item--;
   if(item <0 || item >= listcount) return;

   if((int)icon != -1)
   {
      if(ListItems[listcount].IconPath) delete ListItems[listcount].IconPath;
      ListItems[listcount].IconPath = new char[strlen((char*)icon)+1];
      strcpy(ListItems[item].IconPath, (char*)icon);
   }
   if((int)uplgp != -1)
   {
      if(ListItems[item].item)   delete ListItems[item].item;
      ListItems[item].item = new char[strlen(uplgp)+2];

      sprintf(ListItems[item].item, prsz, uplgp);
   }
   if((int)dlgp != -1)
   {
      if(ListItems[item].ditem)
         delete (char*)ListItems[item].ditem;

      ListItems[item].ditem = (char*)malloc(strlen(dlgp)+2);
      sprintf(ListItems[item].ditem, prsz, dlgp);
   }

   if((int)func != -1)
   {
      if(!func)
         ListItems[item].proc = ZeroFuntion;
      else
         ListItems[item].proc = func;
   }

   if(!dataGUI) return;

   if(IsGuiOnTop(gui_id))
      RedrawGUI();
}


void LForm :: RedrawGUI()
{
   if(!dataGUI) return;
   Menu_SetItemCountDyn(dataGUI, GetNamberOfItems());  // эта функция вызывает itemproc
   DirectRedrawGUI_ID(gui_id);
}

void LForm :: CloseGui()
{
   GeneralFunc_flag1(this->GetGUIId(), 1);
   dataGUI=0;
}


void LForm :: MenuItems(void *data, int curitem)
{
   dataGUI = (GUI*)data;

   WSHDR *ws=0, *ws1=0;
   void *item = 0;

   if(typeMenuShow == Line)
      item=AllocMenuItem(data);
   else if(typeMenuShow == MultiLine)
   {
      item = AllocMLMenuItem(data);
      ws1=AllocMenuWS(data,strlen((char*)ListItems[curitem].ditem)+2);
      wsprintf(ws1, "%t", ListItems[curitem].ditem);
   }
   ws=AllocMenuWS(data,40);

   wsprintf(ws, "%t", ListItems[curitem].item);


   ICONS[curitem] = (int)ListItems[curitem].IconPath;

   if(ICONS[curitem]) SetMenuItemIconArray(data, item, ICONS+curitem);

   if(typeMenuShow == Line)
      SetMenuItemText(data, item, ws, curitem);
   else if(typeMenuShow == MultiLine)
      SetMLMenuItemText(data, item, ws, ws1,curitem);
}


void LForm :: PathHeadFormSmallMenu()
{
  menuhdr.rc.x=0;
  menuhdr.rc.y=YDISP+HeaderH()+3;
  menuhdr.rc.x2=ScreenW()-1;
  menuhdr.rc.y2=HeaderH()+HeaderH()+YDISP-1-1;
}


void LForm :: PathHeadFormNormalMenu()
{
   menuhdr.rc.x=0;
   menuhdr.rc.y=YDISP;
   menuhdr.rc.x2=ScreenW()-1;
   menuhdr.rc.y2=HeaderH()+YDISP;
}

void LForm :: SetMenuItemNumber(int count)
{
   if(!dataGUI) return;
   listcount = count;
   int i=GetCurMenuItem(dataGUI), j=GetNamberOfItems();
   if (i>=j) SetCursorToMenuItem(dataGUI,j-1);
   Menu_SetItemCountDyn(dataGUI, j);
}

int LForm :: GetCurentMenuItem()
{
   if(!dataGUI) return -1;

   return GetCurMenuItem(dataGUI);
}

void LForm :: SetSmallMenu(bool s)
{
   if(s)
   {
      PathHeadFormSmallMenu();  // патчим под небольшую менюшку
      IsSmallMenu = true;
   } else
   {
      PathHeadFormNormalMenu();
      IsSmallMenu = false;
   }

}


void LForm :: GuiHookSetup(void *gui)
{
   dataGUI = (GUI*)gui;
}

int LForm :: RunItemProcById(int id)
{
   if(!ListItems[id].proc || id >= listcount) return -23;
   return (int)ListItems[id].proc();
}


void LForm :: SetMitliStringText(int item, char *text)
{
   item--;
   if(item <0 || item >= listcount) return;

   if(text)
   {
      if(ListItems[item].ditem)
         delete (char*)ListItems[item].ditem;

      ListItems[item].ditem = (char*)malloc(strlen(text)+2);
      sprintf(ListItems[item].ditem, prsz, text);
   } else
   {
      if(ListItems[item].ditem)
         delete (char*)ListItems[item].ditem;

      ListItems[item].ditem = empty();
   }
}


int LForm :: DoCurentItemProc()
{
    return RunItemProcById(GetCurentMenuItem());
}
