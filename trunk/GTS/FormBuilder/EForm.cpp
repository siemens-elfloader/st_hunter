//@ (c) Z.Vova

/* это нужно в либу выносить, но вломно пока, да и в луне юзается версия поновей */
/* писалось йух знает когда, многое можно поумней сделать */


#include "LForm.h"
#include "EForm.h"


int EForm :: EFormOnKey(GUI *gui, GUI_MSG *msg)
{
   EForm * i = (EForm *)EDIT_GetUserPointer(gui);
   int ret=0;

   if(!i->StandartHook) goto nosth;
   if (msg->keys==0x0018)
   {
      if( i->leftsoft )
          ((void(*)())i->leftsoft)();
   }
   else
   if(msg->keys==0x0001)
   {
      if( i->rightsoft )
         ((void(*)())i->rightsoft)();
   }

nosth:

   if(i)   if(i->onKeyHook)
   {
      ret = i->onKeyHook(i,gui, msg);
   }

   return ret;
}


void EForm :: EFormGUIHook(GUI *gui, int cmd)
{
   EForm * i = (EForm *)EDIT_GetUserPointer(gui);

   if(i)
   {
      i->SetOnGUIHook(gui, cmd);
      if(i->GUIHook)  i->GUIHook(i,gui,cmd);
   }
}

EForm :: EForm()
{
   this->Alloc(0, 0);
}

EForm :: EForm(int onKey(EForm *, GUI *gui, GUI_MSG *msg), void global_hook_proc(EForm *, GUI *gui, int cmd))
{
   this->Alloc(onKey,global_hook_proc);
}

int EForm :: Alloc(int onKey(EForm *, GUI *gui, GUI_MSG *msg), void global_hook_proc(EForm *, GUI *gui, int cmd))
{
   StandartHook=true;
   ews = AllocWS(256);
   csm_ram = &csmr;
   headericon=0;
   EwsSize = 256;
   efsoftkeys[0] = 0;
   efsoftkeys[1] = 1;
   efsoftkeys[2] = 2;

   rightsoft=0, leftsoft=0;
   onKeyHook = onKey;
   GUIHook = global_hook_proc;

   SetHeader(0,YDISP,ScreenW()-1,HeaderH()+YDISP,0,(int)"Редактор");

   input_desc.rc.x=0;
   input_desc.rc.y=HeaderH()+1+YDISP;
   input_desc.rc.x2=ScreenW()-1;
   input_desc.rc.y2=ScreenH()-SoftkeyH()-1;

   SetupSoftKey(0x0018, 0x0000, (int)"Выбор", 0);
   SetupSoftKey(0x0001, 0x0000, (int)"Назад", 1);
   SetupSoftKey(0x003D, 0x0000, (int)LGP_DOIT_PIC, 2);


   ef_skt.n = 0;
   ef_skt.desc = ef_sk;


   input_desc.one = 1;
   input_desc.onKey = this->EFormOnKey;
   input_desc.global_hook_proc = this->EFormGUIHook;
   input_desc.locret = (void*)inp_locret;
   input_desc.zero1 = 0;
   input_desc.softkeystab = &ef_skt;
   //input_desc.rc = rc;
   input_desc.font = 4;
   input_desc._100 = 100;
   input_desc._101 = 101;
   input_desc.zero2 = 0;
   input_desc.zero3 = 0;
   input_desc._0x40000000 = 0x40000000;

   edata = 0;
   return 0;
}

EForm :: ~EForm()
{
   FreeWS(ews);
   delete headericon;
}


void EForm :: SetHeader(int recx, int recy, int recw, int rech, char *icon, int lgp)
{
   if((int)icon != -1 && icon!=0)
   {
      if(headericon) delete headericon;
      headericon = new char[strlen((char*)icon)+2];
      strcpy(headericon, (char*)icon);
      headericon[strlen((char*)icon)] =0;
      efhdr.icon = (int*)&headericon;
   }
   if((int)icon == 0)
   {
      if(headericon)  delete headericon;
      headericon=0;
      efhdr.icon=0;
   }

   efhdr.rc.x = recx;
   efhdr.rc.y = recy;
   efhdr.rc.x2 = recw;
   efhdr.rc.y2 = rech;
   efhdr.lgp_id = lgp;
   efhdr.lgp_null = LGP_NULL;
}


void EForm :: SetHeader(char *icon, int lgp)
{
   if((int)icon != -1 && icon!=0)
   {
      if(headericon) delete headericon;
      headericon = new char[strlen((char*)icon)+2];
      strcpy(headericon, (char*)icon);
      headericon[strlen((char*)icon)] =0;
      efhdr.icon = (int*)&headericon;
   }
   if((int)icon == 0)
   {
      if(headericon)
      delete headericon;
         headericon=0;
      efhdr.icon=0;
   }

   if(lgp!=-1)
   efhdr.lgp_id = lgp;
   efhdr.lgp_null = LGP_NULL;
}


void EForm :: SetupSoftKey(short k1, short k2, int lgp, char keynum)
{
   ef_sk[(size_t)keynum].key1 = k1;
   ef_sk[(size_t)keynum].key2 = k2;
   ef_sk[(size_t)keynum].lgp_id = lgp;
}

void EForm :: SetLeftSoft(int lgp)
{
//#ifdef NEWSGOLD
   ef_sk[0].lgp_id = lgp;
//#else
//   ef_sk[1].lgp_id = lgp;
//#endif
}

void EForm :: SetRightSoft(int lgp)
{
//#ifdef NEWSGOLD
   ef_sk[1].lgp_id = lgp;
//#else
//   ef_sk[0].lgp_id = lgp;
//#endif
}


void EForm :: SetLeftSoft(int lgp, void *p)
{
   leftsoft = p;
   ef_sk[0].lgp_id = lgp;
}

void EForm :: SetRightSoft(int lgp, void *p)
{
   rightsoft =p;
   ef_sk[1].lgp_id = lgp;
}

int EForm :: Show()
{
  edata = 0;
  Clean();
  if(FormHook)  FormHook(this);

  gui_id = CreateInputTextDialog(&input_desc,&efhdr,eform.eq,1,this);
  return (gui_id);
}


void EForm :: AddFormElement(const char *element, int type, int size)
{
   EDITCONTROL ec;
   ApplySetingControl(&ec);

   if(type==7)
   {
      wsprintf(ews, "%t", element);
      ConstructComboBox(&ec, type, 0x40, ews, size,0,3,1);
      AddEditControlToEditQend(eform.eq, &ec, eform.ma);
      return;
   }

   wsprintf(ews, "%t", element);
   ConstructEditControl(&ec,type,0x40,ews, size);
   AddEditControlToEditQend(eform.eq, &ec, eform.ma);
}

void EForm :: AddFormElement(const char *element, int type, int size, int elementnum)
{
   EDITCONTROL ec;
   ApplySetingControl(&ec);

   if(type==7)
   {
      wsprintf(ews, "%t", element);
      ConstructComboBox(&ec, type, 0x40, ews, size,0,elementnum,1);
      AddEditControlToEditQend(eform.eq, &ec, eform.ma);
      return;
   }

   wsprintf(ews, "%t", element);
   ConstructEditControl(&ec,type,0x40,ews, size);
   AddEditControlToEditQend(eform.eq, &ec, eform.ma);
}

void EForm :: Clean()
{
   void *ma=malloc_adr();
   void *eq;

   //PrepareEditControl(&ec);
   eq=AllocEQueue(ma,mfree_adr());

   eform.ws = ews;
   eform.eq = eq;
   eform.ma = ma;
}


void EForm :: Extract(int element, WSHDR *buff, int extractsize)
{
   if(!edata) return;

   EDITCONTROL ec;
   ExtractEditControl(edata, element, &ec);
   wsprintf(buff, "%w\0",ec.pWS);
}


bool EForm :: IsItemEmpty(int item)
{
   if(!edata) return true;

   EDITCONTROL ec;
   ExtractEditControl(edata, item, &ec);
   char ent[2]="\0";
   ws_2str(ec.pWS, ent, 2);
   if(ent[0]=='\0')
      return true;
   return false;
}

bool one=1;
void EForm :: GetEditControlData(void *data)
{
   edata = (GUI*)data;
}


int EForm :: GetFocusItem()
{
   if(!edata) return -1;
   return EDIT_GetFocus(edata);
}

void EForm :: SetFocusTo(int element)
{
   if(!edata) return;
   EDIT_SetFocus(edata, element);
}


void EForm :: SetCursorTo(int pos)
{
   if(!edata) return;
   EDIT_SetCursorPos(edata, pos);
}


int EForm :: GetCursorPos()
{
   if(!edata) return -1;
   return EDIT_GetCursorPos(edata);
}

void EForm :: SetTextToFocusedItem(WSHDR *ws)
{
   if(!edata) return;
   EDIT_SetTextToFocused(edata, ws);
}


int EForm :: GetFocusedItemNum()
{
   if(!edata) return 0;
   return EDIT_GetItemNumInFocusedComboBox(edata);
}

void EForm :: AppendTextToFocisedItem(WSHDR *ws)
{
   Extract(GetFocusItem(), ews, EwsSize);
   wstrcat(ews, ws);
   SetTextToFocusedItem(ews);
}

void EForm :: ReallocEws(int newsize)
{
   WSHDR *tmpws = AllocWS(EwsSize+1);
   wstrcpy(tmpws, ews);
   FreeWS(ews);
   ews = AllocWS(newsize);
   wstrcpy(ews, tmpws);
   FreeWS(tmpws);
   EwsSize = newsize;
}


void EForm :: Clear(int element)
{
   if(!edata) return;

   wsprintf(ews, "\0\0");
   EDIT_SetTextToEditControl(edata, element, ews);
}


void EForm :: SetTextToItem(int item, const char *text)
{
   if(!edata) return;
   wsprintf(ews, "%t\0", text);
   EDIT_SetTextToEditControl(edata, item, ews);
   RedrawGUI();
}

void EForm :: SetOnGUIHook(GUI *gui, int cmd)
{
   GetEditControlData((void*)gui);

   if(cmd == 7)
   {
      SetSoftKey(gui, ef_sk, SET_SOFT_KEY_N);
   }
}


void EForm :: RemoveEditControl(int item)
{
   if(!edata) return;
   EDIT_RemoveEditControl(edata, item);
}


void EForm :: InsertEditControl(char *name,int item,int type, int size)
{
   if(!edata) return;

   EDITCONTROL ec;
   wsprintf(ews, "%t", name);
   ConstructEditControl(&ec,type,0x40,ews,size);
   EDIT_InsertEditControl(edata, item, &ec);
}


void EForm :: SetUtf8TextToItem(int item, const char *text)
{
   if(!edata) return;

   utf8_2ws(ews, text, strlen(text));
   EDIT_SetTextToEditControl(edata, item, ews);
   RedrawGUI();
}


void EForm :: RedrawGUI()
{
   REDRAW();
}



bool EForm :: IsMarkedText()
{
   if(!edata) return 0;

   return EDIT_IsMarkModeActive(edata);
}


void EForm :: GetMarkedText(WSHDR *ws)
{
   if(!edata) return;

   EDIT_GetMarkedText(edata, ws);
}


void EForm :: SetFont(int font)
{
   SetFontToEditCOptions(&edit_opt, font);
}


void EForm :: SetColor(int pen, int brush)
{
   if(pen!=-1)   SetPenColorToEditCOptions(&edit_opt, pen);
   if(brush!=-1)   SetBrushColorToEditCOptions(&edit_opt, brush);
}


void EForm :: SetAlign(int align)
{
   SetAlignToEditCOptions(&edit_opt, align);
}


void EForm :: ApplySetingControl(EDITCONTROL *ec)
{
   CopyOptionsToEditControl(ec, &edit_opt);
}


void EForm :: close()
{
   GeneralFunc_flag1(this->GetGuiId(), 1);
   edata=0;
}


int EForm :: GetGuiId()
{
   return this->gui_id;
}



void EForm :: SetEditControlAtrib(int a)
{
   input_desc.zero3 = a;
}


void EForm :: SetShowEditControlAtrib(int s)
{
   input_desc._0x40000000 = s;
}


void EForm :: SetKeyListener(int key(EForm *,GUI *, GUI_MSG *))
{
   onKeyHook = key;
}


