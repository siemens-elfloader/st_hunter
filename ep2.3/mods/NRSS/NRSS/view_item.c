#include "../inc/swilib.h"
#include "view_item.h"
#include "rect_patcher.h"
#include "strings.h"
#include "print.h"

static const HEADER_DESC view_item_hdr={0,0,NULL,NULL,NULL,(int)"View Item",LGP_NULL};

static void view_item_locret(void){}

static const SOFTKEY_DESC view_item_sk[] =
{
  {0x0018, 0x0000, (int)""},
  {0x0001, 0x0000, (int)"Close"},
  {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB view_item_skt =
{
  view_item_sk, 0
};

static int view_item_onkey(GUI *data, GUI_MSG *msg)
{
  return (0);
}

static void view_item_ghook(GUI *gui, int cmd)
{
  if (cmd==0xA/*TI_CMD_FOCUS*/)
  {
    DisableIDLETMR();
  }
  if (cmd==0xC/*TI_CMD_SUBFOCUS_CHANGE*/)
  {
    if (EDIT_GetFocus(gui)>EDIT_GetUnFocus(gui)) // Если листаем сверху вниз...
      EDIT_SetCursorPos(gui, 1); // ...То ставим курсор в начало
  }
}

static const INPUTDIA_DESC view_item_desc =
{
  1,
  view_item_onkey,
  view_item_ghook,
  (void *)view_item_locret,
  0,
  &view_item_skt,
  {0,NULL,NULL,NULL},
  FONT_SMALL,
  100,
  101,
  0,
  //  0x00000001 - Выровнять по правому краю
  //  0x00000002 - Выровнять по центру
  //  0x00000004 - Инверсия знакомест
  //  0x00000008 - UnderLine
  //  0x00000020 - Не переносить слова
  //  0x00000200 - bold
  0,
  //  0x00000002 - ReadOnly
  //  0x00000004 - Не двигается курсор
  //  0x40000000 - Поменять местами софт-кнопки
  0x40000000
};

int create_view_item2(RSS_ITEM *p)
{
  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  int t_len=0, d_len=0, h_len=64, max;
  WSHDR *ws;

  eq=AllocEQueue(ma,mfree_adr());
  if (p->title) t_len=strlen(p->title);
  if (p->description) d_len=strlen(p->description);
  if (t_len>h_len) max=t_len;
  if (d_len>max) max=d_len;

  ws=AllocWS(max);

  PrepareEditControl(&ec);
  ascii2ws(ws,"Title:");
  ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);

  PrepareEditControl(&ec);
  ascii2ws(ws,p->title);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,2);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);

  PrepareEditControl(&ec);
  ascii2ws(ws,"Description:");
  ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);

  PrepareEditControl(&ec);
  ascii2ws(ws,p->description);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,2);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);

  FreeWS(ws);
  patch_header(&view_item_hdr);
  patch_input(&view_item_desc);
  return CreateInputTextDialog(&view_item_desc,&view_item_hdr,eq,1,NULL);
}


RSS_ITEM *viewitem;
extern DrawHeader(char *s);
extern DrawSoftKeys(char *right,char *left);
int XX=0,YY=0;

static void OnRedraw(MAIN_GUI *data)
{
  LockSched();
  RSS_ITEM *p=viewitem;
  DrawRoundedFrame(0,YDISP,scr_w,scr_h,0,0,0,clrWhite,clrWhite);
  DrawHeader(p->title);
  
  DrawSoftKeys("Выбрать","Закрыть");
  UnlockSched();

  DrawList();
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  scr_w=ScreenW()-1;
  scr_h=ScreenH()-1;
  data->ws1=AllocWS(256);
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  
  FreeWS(data->ws1);
  data->gui.state=0;
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  DisableIDLETMR();
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  int k=msg->gbsmsg->submess;

  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(k)
    {  
      case UP_BUTTON:    MoveUp(0);   break;
      case DOWN_BUTTON:  MoveDown(0); break;
      case LEFT_BUTTON:  MoveUp(1);   break;
      case RIGHT_BUTTON: MoveDown(1); break;
      case '#' :
      {
        WSHDR *ws;
        ws=AllocWS(150);
        extern const char *successed_config_filename;
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);       
      }
      break;
      case RIGHT_SOFT:case RED_BUTTON: return (1);
    }
  } 


  
  if ((msg->gbsmsg->msg==KEY_DOWN) && (k==ENTER_BUTTON || k==LEFT_SOFT))
  {
    RSS_ITEM *p=GetItem(curpos);
    if (p)
    {
      create_view_item(p);
    }
    return(-1);
  }
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

static int method8(void){return(0);}
static int method9(void){return(0);}

static const void * const gui_methods[11]={
  (void *)OnRedraw,
  (void *)onCreate,
  (void *)onClose,
  (void *)onFocus,
  (void *)onUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data,
  (void *)method8,
  (void *)method9,
  0
};


int create_view_item(RSS_ITEM *p)
{
  viewitem=p;  
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
//  main_gui->gui.canvas=(void *)(&MM_Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  return CreateGUI(main_gui);   
}


