#include "..\inc\swilib.h"
#include "system.h"

const char * const LG_ADD_PLITEM[1]={"Read!!!"}; 
extern int ShowID3(); 

#define YDISP_SMALL_MENU (20) 

#pragma inline 
void patch_header_is_small(HEADER_DESC* head) 
{ 
  head->rc.x=0; 
  head->rc.y=YDISP_SMALL_MENU; 
  head->rc.x2=ScreenW()-1; 
  head->rc.y2=HeaderH()+YDISP_SMALL_MENU; 
} 

HEADER_DESC pl_add_menuhdr={0,0,0,0,NULL,(int)"Options",LGP_NULL}; 
int add_pl_menusoftkeys[]={0,1,2}; 
extern int PIC_m[]; 

SOFTKEY_DESC add_pl_menu_sk[]= 
{ 
  {0x0018,0x0000,(int)"Select"}, 
  {0x0001,0x0000,(int)"Back"}, 
  {0x003D,0x0000,(int)LGP_DOIT_PIC} 
}; 

SOFTKEYSTAB add_pl_menu_skt= 
{ 
  add_pl_menu_sk,0 
}; 

const void * add_pl_menu_HNDLS[1]= 
{ 
  NULL
}; 

void add_pl_menu_ghook(void *data, int cmd) 
{ 
  if (cmd==0x0A) 
  { 
    DisableIDLETMR(); 
  } 
} 

int add_pl_menu_keyhook(void *data, GUI_MSG *msg) 
{ 
  if ((msg->keys==0x18)||(msg->keys==0x3D)) 
  { 
    ((void (*)(void))(add_pl_menu_HNDLS[GetCurMenuItem(data)]))(); 
  } 
  else if(msg->keys==0x01) 
    GeneralFuncF1(1); 
  return(0); 
} 

void add_pl_menu_iconhndl(void *data, int curitem, void *unk) 
{ 
  void *item=AllocMenuItem(data); 
  WSHDR *ws=AllocMenuWS(data,strlen(LG_ADD_PLITEM[curitem])); 
  wsprintf(ws,per_t,LG_ADD_PLITEM[curitem]); 

  SetMenuItemText(data,item,ws,curitem); 
} 

MENU_DESC add_pl_menu= 
{ 
  8,add_pl_menu_keyhook,add_pl_menu_ghook,NULL, 
  add_pl_menusoftkeys, 
  &add_pl_menu_skt, 
  0x11, 
  add_pl_menu_iconhndl, 
  NULL, 
  NULL, 
  0 
}; 

int ShowMenu(void) 
{ 
  patch_header_is_small(&pl_add_menuhdr); 
  return (CreateMenu(1,0,&add_pl_menu,&pl_add_menuhdr,0,3,0,0)); 
}
