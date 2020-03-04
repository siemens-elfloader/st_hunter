#include "..\inc\swilib.h"
#include "system.h"

int ret=0;

int UNI_YDISP;

int isElka()
{
  if (isnewSGold())
  {
    if (ScreenW()==132 && ScreenH()==176)
    {
      UNI_YDISP=0;
      return 1;
    }
    if (ScreenW()==240 && ScreenH()==320)
    {
      UNI_YDISP=24;
      return 2;
    }
  }
  return 0;
}

void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=UNI_YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=UNI_YDISP+HeaderH()-1;
}

char *name[12]=
{
  (char*)name_1,
  (char*)name_2,
  (char*)name_3,
  (char*)name_4,
  (char*)name_5,
  (char*)name_6,
  (char*)name_7,
  (char*)name_8,
  (char*)name_9,
  (char*)name_10,
  (char*)name_11,
  (char*)name_12,
};

char *Open[12]=
{
  (char*)open_1,
  (char*)open_2,
  (char*)open_3,
  (char*)open_4,
  (char*)open_5,
  (char*)open_6,
  (char*)open_7,
  (char*)open_8,
  (char*)open_9,
  (char*)open_10,
  (char*)open_11,
  (char*)open_12
};

void ReadSettings()
{
  InitConfig();
  m=isElka();
  for(int i = 0; i < 12; i++)
  {
    Items[i].icon_inact=Menu_Items[i].icon_inact;
    Items[i].icon_big=Menu_Items[i].icon_big;
    
    if (*name[i]=='\0')
      Items[i].lgp_id=Menu_Items[i].lgp_id;
    else
      Items[i].lgp_id=(int)name[i];
    
    
    if (*Open[i]!='\0')
      Items[i].open=Open[i];
    else
      Items[i].run=Menu_Items[i].run;
    
    if (glob_ani!=0)
      Items[i].Anim=glob_ani;
  }  
  
  
  if (glob_ani==0)
  {
    Items[0].Anim=anim_1;
    Items[1].Anim=anim_2;
    Items[2].Anim=anim_3;
    Items[3].Anim=anim_4;
    Items[4].Anim=anim_5;
    Items[5].Anim=anim_6;
    Items[6].Anim=anim_7;
    Items[7].Anim=anim_8;
    Items[8].Anim=anim_9;
    Items[9].Anim=anim_10;
    Items[10].Anim=anim_11;
    Items[11].Anim=anim_12;
  }
  
  Items[0].pass=p1;
  Items[1].pass=p2;
  Items[2].pass=p3;
  Items[3].pass=p4;
  Items[4].pass=p5;
  Items[5].pass=p6;
  Items[6].pass=p7;
  Items[7].pass=p8;
  Items[8].pass=p9;
  Items[9].pass=p10;
  Items[10].pass=p11;
  Items[11].pass=p12;
  
  Items[0].CloseMenu=pc1;
  Items[1].CloseMenu=pc2;
  Items[2].CloseMenu=pc3;
  Items[3].CloseMenu=pc4;
  Items[4].CloseMenu=pc5;
  Items[5].CloseMenu=pc6;
  Items[6].CloseMenu=pc7;
  Items[7].CloseMenu=pc8;
  Items[8].CloseMenu=pc9;
  Items[9].CloseMenu=pc10;
  Items[10].CloseMenu=pc11;
  Items[11].CloseMenu=pc12;
  
  if (!anim_on)
    pic_n=select_nom;
  else
    pic_n=0;
}

void RunAction(char *s)
{
  if((s)&&strlen(s))
  {
   if ((s[1]==':')&&(s[2]=='\\'))
   {
     WSHDR  *ws=AllocWS(128); 
     str_2ws(ws,s,strlen(s)+1); 
     ExecuteFile(ws,0,NULL); 
     FreeWS(ws);
   }else
   {
    if ((s[0]!='a')&&(s[0]!='A')&&(s[1]!='0'))
    {unsigned int* addr = (unsigned int*)GetFunctionPointer(s);
    if (addr) {typedef void (*voidfunc)();  voidfunc pp=(voidfunc)*(addr+4);SUBPROC((void*)pp);}}

    if (((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))
    { int entry; sscanf(s,"%08X",&entry); SUBPROC((void*)entry);}
   }
  }
}

void pass_pos_cler()
{
  for (int M=0; M<=pass_pos; M++)
  { 
    pass[M]='\0';
  } 
  pass_pos=0;
  //flag='menu';
}

void Actions()
{          
  if (*Open[pos]!='\0')            //If a team
  {
    RunAction(Items[pos].open);
    ret=Items[pos].CloseMenu;                                   //return not to close the menu
  } else
  {
    Items[pos].run();
    ret = 0;                 //return to close the menu
  }
}

void EnterAction()
{
  
  if (anim_on)
    GBS_StopTimer(&tmr);
  
  if (prun==2)
    Actions();
  
  //If the pass is not checked when you open the menu
  if (!prun)
  {
    if (Items[pos].pass)
    {
      window_menu=1;
      if (!strcmp(pass,passw) && *pass!='\0') //if the general password coincided
      {
        Actions();
        window_menu=0;
        pass_pos_cler();
      }else
      if (*pass!='\0')
      {
        MsgBoxError(1,(int)"Incorrect password!");
      }
    }
    if (!Items[pos].pass)
    {
      window_menu=0;
      Actions();
    }
  }
  
  //If the pass is checked when you open the menu
  if (prun==1)
  {
    if (start==1)
    {
      if (!strcmp(pass,passw) &&  *pass!='\0') //if the general password coincided
      {
        window_menu=0;
        
        ret=-1;
        start=0;
        
      }else
      {
        MsgBoxError(1,(int)"Incorrect password");
        pass_pos_cler();
      }
    }else
      
      Actions();

  }
}

void Casher()
{
  if (Cash)
  {
    for(int i=0;i<12;i++)
    {
      DrawImg(ScreenW()+2,0, *Items[pos].icon_inact-11-m);
      DrawImg(ScreenW()+2,0, *Items[i].icon_inact);
      for(int cxz=0; cxz < Items[i].Anim; cxz++)
        DrawImg(ScreenW()+2,0, Items[i].icon_big[cxz]);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

char * BMNAME[9]=
{
  (char*)BM1NAME,
  (char*)BM2NAME,
  (char*)BM3NAME,
  (char*)BM4NAME,
  (char*)BM5NAME,
  (char*)BM6NAME,
  (char*)BM7NAME,
  (char*)BM8NAME,
  (char*)BM9NAME
}; 

const char * const LG_ADD_PLITEM[9]=
{
  BM1NAME,
  BM2NAME,
  BM3NAME,
  BM4NAME,
  BM5NAME,
  BM6NAME,
  BM7NAME,
  BM8NAME,
  BM9NAME
}; 

char *BMFILE[9]=
{
  (char*)BM1FILE,
  (char*)BM2FILE,
  (char*)BM3FILE,
  (char*)BM4FILE,
  (char*)BM5FILE,
  (char*)BM6FILE,
  (char*)BM7FILE,
  (char*)BM8FILE,
  (char*)BM9FILE
};
/*
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+UNI_YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}
*/
void patch_header_is_small(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=UNI_YDISP+HeaderH();
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()*2+UNI_YDISP-1-1;
}

HEADER_DESC pl_add_menuhdr={0,0,0,0,NULL,(int)"Options",LGP_NULL}; 

int add_pl_menusoftkeys[]={0,1,2}; 

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

void enter1(void){RunAction(BMFILE[0]); GeneralFuncF1(1);}
void enter2(void){RunAction(BMFILE[1]); GeneralFuncF1(1);}
void enter3(void){RunAction(BMFILE[2]); GeneralFuncF1(1);}
void enter4(void){RunAction(BMFILE[3]); GeneralFuncF1(1);}
void enter5(void){RunAction(BMFILE[4]); GeneralFuncF1(1);}
void enter6(void){RunAction(BMFILE[5]); GeneralFuncF1(1);}
void enter7(void){RunAction(BMFILE[6]); GeneralFuncF1(1);}
void enter8(void){RunAction(BMFILE[7]); GeneralFuncF1(1);}
void enter9(void){RunAction(BMFILE[8]); GeneralFuncF1(1);}

const void * add_pl_menu_HNDLS[9]= 
{ 
  (void *)enter1, 
  (void *)enter2, 
  (void *)enter3,
  (void *)enter4,
  (void *)enter5, 
  (void *)enter6, 
  (void *)enter7,
  (void *)enter8,
  (void *)enter9 
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
  8,
  add_pl_menu_keyhook,
  add_pl_menu_ghook,
  NULL, 
  add_pl_menusoftkeys, 
  &add_pl_menu_skt, 
  2, 
  add_pl_menu_iconhndl, 
  NULL, 
  NULL, 
  0 
}; 

int ShowOptMenu(void) 
{  
  int j=0;
  for(int i=0;i<9;i++)
  {
    if (*BMNAME[i]!='\0')  j++;
  }
  
  if (j)
  {
    patch_header_is_small(&pl_add_menuhdr); 
    return (CreateMenu(1,0,&add_pl_menu,&pl_add_menuhdr,0,j,0,0)); 
  }else
    MsgBoxError(1,(int)"No items to display!");
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
