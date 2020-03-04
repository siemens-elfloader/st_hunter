#include "inc/mc.h"
#include "inc/mui.h"
#include "inc/gui.h"
#include "inc/popup.h"
#include "inc/ScreenMake.h"

int fropon;
////////////////////////////////////////////////////////////////////////////////
void empty_ghook(GUI *gui, int cmd){};

void empty_locret(void) {}

SOFTKEY_DESC empty_menu_sk[]= { { 0, 0, 0 } };

SOFTKEYSTAB empty_menu_skt =
{
	empty_menu_sk, 0
};

void prep_hd(EDITCONTROL* ec)
{
	ec->ed_options.font = 2;
	ec->ed_options.pen[3] = 100;
	ec->ed_options.pen[2] = 0x80;
} 
////////////////////////////////////////////////////////////////////////////////

IFN_RET_PROC txt_inp_proc;
int testfn;

int txt_inp_onkey(GUI *gui, GUI_MSG *msg)
{
if (fropon==0)
{
 if (!wsbuf)
 {
 GeneralFuncF1(1);
 return(0);
 }

 EDITCONTROL ec;
 if (msg->keys==26)
 {
 ExtractEditControl(gui,2,&ec);
 wstrcpy(wsbuf,ec.pWS);
 if (!testfn || TestFileName(wsbuf))
 {
   if(txt_inp_proc)txt_inp_proc(wsbuf);
  GeneralFuncF1(1);
 }
 else
 {
  MsgBoxError(1, (int) muitxt(ind_err_badname));
  CorFileName(wsbuf);
  msg->keys = 0;
  EDIT_SetTextToFocused(gui,wsbuf);
 }
 }
}else
{
  if (msg->keys==0xFFF)
    return (0xFF);

}
 return(0); //Do standart keys
}

HEADER_DESC txt_inp_hdr={0,0,0,0,NULL,0,LGP_NULL};
INPUTDIA_DESC txt_inp_desc=
{
 1,
 txt_inp_onkey,
 empty_ghook,
 (void *)empty_locret,
 0,
 &empty_menu_skt,
 {0,0,0,0},
 FONT_SMALL,
 100,
 101,
 0,
 0,
 0x40000000
};

int TextInput(char *caption, char *prmpt, int TestFileName, WSHDR *wsname, IFN_RET_PROC retproc)
{
 DoScreen();
  
 txt_inp_hdr.lgp_id = (int)caption;
 txt_inp_proc = retproc;
 testfn = TestFileName;
 fropon=0;
 WSHDR *buff=AllocWS(32);

 void *ma=malloc_adr();
 void *eq=AllocEQueue(ma,mfree_adr());
 EDITCONTROL ec;
 PrepareEditControl(&ec);

 wsprintf(buff, _tc, prmpt);
 ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,buff,32);
 prep_hd(&ec);
 AddEditControlToEditQend(eq,&ec,ma);

 ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_NORMAL_STR,wsname,MAX_PATH);
 AddEditControlToEditQend(eq,&ec,ma);
 FreeWS(buff);
 return CreateInputTextDialog(&txt_inp_desc,&txt_inp_hdr,eq,1,0);
}


void FullPatch(WSHDR *wsname)
{      
 DoScreen();
  
 txt_inp_hdr.lgp_id = (int) muitxt(ind_fullpatch);
 txt_inp_proc = NULL;
 fropon=1;
 WSHDR *buff=AllocWS(32);

 void *ma=malloc_adr();
 void *eq=AllocEQueue(ma,mfree_adr());
 EDITCONTROL ec;
 PrepareEditControl(&ec);

 wsprintf(buff, _tc, muitxt(ind_fullpatch));
 ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,buff,32);
 prep_hd(&ec);
 AddEditControlToEditQend(eq,&ec,ma);
 
 ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_NORMAL_STR,wsname,MAX_PATH);
 AddEditControlToEditQend(eq,&ec,ma);
 CreateInputTextDialog(&txt_inp_desc,&txt_inp_hdr,eq,1,0);

 FreeWS(buff);
}



