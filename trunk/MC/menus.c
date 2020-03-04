#include "inc/lng.h"
#include "inc/mc.h"
#include "inc/zslib.h"

#pragma inline
void patch_header(HEADER_DESC* head)
{
 head->rc.x=0;
 head->rc.y=YDISP;
 head->rc.x2=ScreenW()-1;
 head->rc.y2=HeaderH()+YDISP-1;
}
#pragma inline
void patch_input(INPUTDIA_DESC* inp)
{
 inp->rc.x=0;
 inp->rc.y=HeaderH()+1+YDISP;
 inp->rc.x2=ScreenW()-1;
 inp->rc.y2=ScreenH()-SoftkeyH()-1;
}

void InitAllMD()
{
 extern HEADER_DESC txt_inp_hdr;
 extern INPUTDIA_DESC txt_inp_desc;

 patch_header(&txt_inp_hdr);
 patch_input(&txt_inp_desc);
 txt_inp_desc.rc.y+=15;
 txt_inp_desc.rc.x+=5;
 
 extern HEADER_DESC md5_inp_hdr;
 extern INPUTDIA_DESC md5_inp_desc;
 
 patch_header(&md5_inp_hdr);
 patch_input(&md5_inp_desc);
}


