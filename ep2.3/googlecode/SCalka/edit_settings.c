#include "..\inc\swilib.h"
#include "rect_patcher.h"
#include "strings.h"
#include "edit_settings.h"

#ifdef NEWSGOLD
#define CBOX_CHECKED 0xE116
#define CBOX_UNCHECKED 0xE117
#else
#define CBOX_CHECKED 0xE10B
#define CBOX_UNCHECKED 0xE10C
#endif

CALC_SETTINGS calc_set;

const char setting_file[]="4:\\ZBin\\etc\\SCalka.cfg";

void ReadCalcSettings(void)
{
  char fname[256];
  int f;
  unsigned int err;
  strcpy(fname, setting_file);
  if ((f=fopen(fname, A_ReadOnly+A_BIN, P_READ, &err))==-1)
  {
    fname[0]='0';
    f=fopen(fname, A_ReadOnly+A_BIN, P_READ, &err);
  }
  if (f!=-1)
  {
    if (lseek(f, 0, S_END, &err, &err)==sizeof(CALC_SETTINGS))
    {
      lseek(f, 0, S_SET, &err, &err);
      fread(f, &calc_set, sizeof(CALC_SETTINGS), &err);      
    }
    fclose(f, &err);    
  }
  else
  {
    strcpy(calc_set.fmt, "%1.15lg");
    calc_set.drg=DEGREES;
    calc_set.x=calc_set.y=0;
  }
}

void WriteCalcSettings(void)
{
  char fname[256];
  int f;
  unsigned int err;
  strcpy(fname, setting_file);
  if ((f=fopen(fname, A_WriteOnly+A_Create+A_Truncate+A_BIN, P_WRITE, &err))==-1)
  {
    fname[0]='0';
    f=fopen(fname, A_WriteOnly+A_Create+A_Truncate+A_BIN, P_WRITE, &err);
  }
  if (f!=-1)
  {
    fwrite(f, &calc_set, sizeof(CALC_SETTINGS), &err);
    fclose(f, &err);    
  }    
}

static const char *drg_def[]={
  "角度",
  "弧度",
  "百分度"
};

static int view_settings_onkey(GUI *data, GUI_MSG *msg)
{
  EDITCONTROL ec;
  WSHDR *ws;
  int j, i, l;
  char *s;
  WSHDR *ws1, locws1;
  unsigned short ws_body1[128];
  ws1=CreateLocalWS(&locws1, ws_body1, 128);
  i=EDIT_GetFocus(data);
  l=msg->gbsmsg->submess;
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    if (i==6)
    {
      if (l==ENTER_BUTTON)
      {
        calc_set.auto_recalc=!calc_set.auto_recalc;
        CutWSTR(ws1, 0);
        wsAppendChar(ws1, calc_set.auto_recalc?CBOX_CHECKED:CBOX_UNCHECKED);
        EDIT_SetTextToFocused(data,ws1);
        return (-1);
      }
    }
  }
  if (msg->keys==0xFFF)
  {   
    EDIT_SetFocus(data, 2);
    j=EDIT_GetItemNumInFocusedComboBox(data);
    if (j) calc_set.drg=j-1;
    
    ExtractEditControl(data,4,&ec);
    ws=ec.pWS;
    s=calc_set.fmt;
    for (int i=0; i<ws->wsbody[0] && i<16; i++)
    {
      *s++=char16to8(ws->wsbody[i+1]);
    }
    *s=0;
    
    return (1);
  }
  return (0);
}

static void view_settings_ghook(GUI *data, int cmd)
{
  static SOFTKEY_DESC sk_save={0x0FFF,0x0000,(int)"保存"};
  WSHDR *ws, locws;
  unsigned short ws_body[128];

  EDITCONTROL ec;
  int i, j;
  ws=CreateLocalWS(&locws, ws_body, 128);
  i=EDIT_GetFocus(data);
  if (cmd==7)
  {
    SetSoftKey(data,&sk_save,SET_SOFT_KEY_N);  
  }
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
  if (cmd==0x0D)
  {
    if (i==2)
    {
      ExtractEditControl(data,i,&ec);
      if ((j=EDIT_GetItemNumInFocusedComboBox(data)))
      {
        gb2ws(ws,drg_def[j-1],10);                              
      }
      else
      {
        ExtractEditControl(data,i-1,&ec);
        wstrcpy(ws,ec.pWS);
      }
      EDIT_SetTextToFocused(data,ws);
    }
  }
}

static const SOFTKEY_DESC view_settings_sk[] =
{
  {0x0018, 0x0000, (int)""},
  {0x0001, 0x0000, (int)"关闭"},
  {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB view_settings_skt =
{
  view_settings_sk, 0
};

static void view_settings_locret(void){}

static HEADER_DESC view_settings_hdr={0,0,0,0,NULL,(int)"设定",LGP_NULL};

static INPUTDIA_DESC view_settings_desc=
{
  1,
  view_settings_onkey,
  view_settings_ghook,
  (void *)view_settings_locret,
  0,
  &view_settings_skt,
  {0,0,0,0},
  4,
  100,
  101,
  0,

//  0x00000001 - 蔓痤忭�螯 镱 镳噔铎?牮帼
//  0x00000002 - 蔓痤忭�螯 镱 鲥眚痼
//  0x00000004 - 软忮瘃? 珥嚓铎羼?//  0x00000008 - UnderLine
//  0x00000020 - 湾 镥疱眍耔螯 耠钼?//  0x00000200 - bold
  0,

//  0x00000002 - ReadOnly
//  0x00000004 - 湾 溻桡噱蝰� 牦瘃铕
//  0x40000000 - 项戾?螯 戾耱囔?耦趄-觏铒觇
  0x40000000
};


int CreateSettingsEdit(void)
{
  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  WSHDR *ws;
  
  eq=AllocEQueue(ma,mfree_adr());
  ws=AllocWS(256);
  
  PrepareEditControl(&ec);
  wsprintf(ws,"%t","角类型:");
  ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);   // 1
  
  PrepareEditControl(&ec);
  wsprintf(ws,"%t",drg_def[calc_set.drg]);
  ConstructComboBox(&ec,ECT_COMBO_BOX,ECF_APPEND_EOL,ws,64,0,3,1+calc_set.drg);
  AddEditControlToEditQend(eq,&ec,ma);    // 2
  
  PrepareEditControl(&ec);
  wsprintf(ws,"%t","输出格式:");
  ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);    // 3
  
  PrepareEditControl(&ec);
  wsprintf(ws,"%t",calc_set.fmt);
  ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,ws,15);
  AddEditControlToEditQend(eq,&ec,ma);   // 4
  
  PrepareEditControl(&ec);
  wsprintf(ws,"%t","实时计算:");
  ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);    // 5 
  
  PrepareEditControl(&ec);
  CutWSTR(ws, 0);
  wsAppendChar(ws, calc_set.auto_recalc?CBOX_CHECKED:CBOX_UNCHECKED);
  ConstructEditControl(&ec,ECT_LINK,ECF_APPEND_EOL,ws,ws->wsbody[0]);
  AddEditControlToEditQend(eq,&ec,ma);    // 6 
  
  FreeWS(ws);
  patch_header(&view_settings_hdr);
  patch_input(&view_settings_desc);
  return CreateInputTextDialog(&view_settings_desc,&view_settings_hdr,eq,1,NULL);
}

