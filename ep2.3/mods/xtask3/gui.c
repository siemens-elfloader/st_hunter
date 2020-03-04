#include "..\inc\swilib.h"
#include "conf_loader.h"
#include "rect_patcher.h"
#include "swaper.h"

extern void kill_data(void *p, void (*func_p)(void *));

char mmenu_hdr_txt[64];

int show_daemons;
//extern int mode;
extern CSM_RAM *under_idle;

extern const char BM1NAME[32];
extern const char BM1FILE[128];
extern const char BM2NAME[32];
extern const char BM2FILE[128];
extern const char BM3NAME[32];
extern const char BM3FILE[128];
extern const char BM4NAME[32];
extern const char BM4FILE[128];
extern const char BM5NAME[32];
extern const char BM5FILE[128];
extern const char BM6NAME[32];
extern const char BM6FILE[128];
extern const char BM7NAME[32];
extern const char BM7FILE[128];
extern const char BM8NAME[32];
extern const char BM8FILE[128];
extern const char BM9NAME[32];
extern const char BM9FILE[128];
extern const char BM0NAME[32];
extern const char BM0FILE[128];
extern const char BM_GREEN_NAME[32];
extern const char BM_GREEN_FILE[128];

const CSM_DESC maincsm;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
//  int show_csm;
//  int do_idle;
}MAIN_CSM;

int my_csm_id;

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}

int strncmp_nocase(const char *s1,const char *s2,unsigned int n)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))&&(--n)) if (!c) break;
  return(i);
}

typedef struct
{
  void *next;
  WSHDR *name;
  void *p;
  int is_daemon;
} NAMELIST;

NAMELIST * volatile nltop;

char csm_text[32768];

const char percent_t[]="%t";

void ClearNL(void)
{
  LockSched();
  NAMELIST *nl=nltop;
  nltop=0;
  UnlockSched();
  while(nl)
  {
    NAMELIST *p=nl;
    FreeWS(nl->name);
    nl=p->next;
    mfree(p);
  }
}

void AddNL(WSHDR *ws, int is_daemon)
{
  NAMELIST *nnl=malloc(sizeof(NAMELIST));
  nnl->name=ws;
  nnl->next=0;
  nnl->is_daemon=is_daemon;
  LockSched();
  if (!nltop)
  {
    nltop=nnl;
  }
  else
  {
    nnl->next=nltop;
    nltop=nnl;
  }
  UnlockSched();
}

char *find_name(CSM_RAM *csm)
{
  char s[40];
  char *p;
  static char u[40];

  CSM_DESC *desc=csm->constr;

  sprintf(s,"%08X ",desc);
  p=strstr(csm_text,s);
  if (p)
  {
    return(p+9);
  }
  else
  {
    sprintf(u,"Unknown %08X!",desc);
    return(u);
  }
}
unsigned int *ErrorNumber;
char *dday[7]={"Пн","Вт","Ср","Чт","Пт","Сб","Вс"};
int GetNumberOfDialogs(void)
{
  int count=0;
  int c;
  int i;
  CSM_RAM *icsm;
  WSHDR *ws;
  char ss[64];

  void *ircsm=FindCSMbyID(CSM_root()->idle_id);
  ClearNL();

  //Find new style daemons
  if (show_daemons)
  {
    icsm=((CSM_RAM *)(CSM_root()->csm_q->csm.first))->next; //пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ CSM пїЅпїЅпїЅпїЅпїЅпїЅпїЅ
    while(((unsigned int)(icsm->constr)>>27)==0x15)
    {
      WSHDR *tws=(WSHDR *)(((char *)icsm->constr)+sizeof(CSM_DESC));
      if((tws->ws_malloc==NAMECSM_MAGIC1)&&(tws->ws_mfree==NAMECSM_MAGIC2))
      {
        ws=AllocWS(64);
        wstrcpy(ws,tws);
        AddNL(ws,1);
        nltop->p=icsm;
        count++;
      }
      icsm=icsm->next;
    }
  }
  icsm=under_idle->next; //пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
  do
  {
    if (icsm==ircsm)
    {
   //   ws=AllocWS(40);
    //  wsprintf(ws,"IDLE Screen");
      ws = AllocWS(256);
      wsprintf(ws,"MainScreen | %d kb",GetFreeRamAvail()/1024);
      AddNL(ws,0);
      nltop->p=icsm;
      count++;
    }
    else
    {
      if (icsm->constr!=&maincsm)
      {
	WSHDR *tws=(WSHDR *)(((char *)icsm->constr)+sizeof(CSM_DESC));
	char *s;
	if((tws->ws_malloc==NAMECSM_MAGIC1)&&(tws->ws_mfree==NAMECSM_MAGIC2))
	{
	  ws=AllocWS(64);
	  wstrcpy(ws,tws);
	  AddNL(ws,0);
	  nltop->p=icsm;
	  count++;
	}
	else
	{
	  s=find_name(icsm);
#ifdef NEWSGOLD
	  if (!strncmp_nocase(s,"Java",4))
	  {
	    typedef struct
	    {
	      CSM_RAM csm;
	      int unk1;
	      int bearer;
	      int gui_id;
	      int unk2;
	      int param_R1;
	      int param_ZERO;
	    }JAVAINTERFACE_CSM;
	    int i=((JAVAINTERFACE_CSM *)icsm)->bearer;
	    int j=((JAVAINTERFACE_CSM *)icsm)->param_R1;
	    if (i==2) continue;
	    ws=AllocWS(128);
	    switch(i)
	    {
	    case 1:
	      wsprintf(ws,"Browser",j);
	      break;
	    case 2:
	      wsprintf(ws,"Jam %d",j);
	      break;
	    case 3:
	      switch(j)
	      {
	      case 2:
		wsprintf(ws,"Phone Java");
                break;
	      case 3:
		wsprintf(ws,"User Java");
                break;
	      default:
		wsprintf(ws,"Unknown Java (%d)",j);
		break;
	      }
	      break;
	    default:
	      wsprintf(ws,"Unknown %d(%d) bearer",i,j);
	      break;
	    }
	    goto L_ADD;
	  }
#else
          if (!strncmp_nocase(s,"Java",4))
	  {
	    typedef struct
	    {
	      CSM_RAM csm;
	      int bearer;
	      int gui_id;
	      int gui_id2;
	    }JAVAINTERFACE_CSM;
	    int i=((JAVAINTERFACE_CSM *)icsm)->bearer;
	    ws=AllocWS(64);
	    switch(i)
	    {
	    case 1:
	      wsprintf(ws,"Browser");
	      break;
	    case 0xF:
              wsprintf(ws,"User Java");
	      break;
            case 0x11:
#ifdef X75
              wsprintf(ws,"Java: %t",GetLastJavaApplication());
#else
              wsprintf(ws,"Java");
#endif
	      break; 
            case 18:
              wsprintf(ws,"E-mail client");
	      break; 
            default:
              wsprintf(ws,"Unknown %d bearer",i);
              break;
	    }
	    goto L_ADD;
	  }
#endif
	  if (strncmp(s,"!SKIP!",6))
	  {
	    ws=AllocWS(64);
	    i=0;
	    while((c=*s++)>=' ')
	    {
	      if (i<(sizeof(ss)-1)) ss[i++]=c;
	    }
	    ss[i]=0;
	    wsprintf(ws,percent_t,ss);
	  L_ADD:
	    AddNL(ws,0);
	    nltop->p=icsm;
	    count++;
	  }
	}
      }
    }
  }
  while((icsm=icsm->next));
  TDate date; 
  TTime time; 
  GetDateTime(&date,&time);  
  RAMNET *rn=RamNet();
 
//  sprintf(mmenu_hdr_txt,"XTask3.0 : %d dialogs",count);
  sprintf(mmenu_hdr_txt,"%ddb| %02d:%02d          %s,%d |%d%%",rn->power,time.hour,time.min,dday[GetWeek(&date)],date.day,*RamCap());
  return(count);
}

NAMELIST *get_nlitem(int curitem)
{
  NAMELIST *nl;
  nl=nltop;
  int i=0;
  while(nl)
  {
    if (i==curitem) break;
    i++;
    nl=nl->next;
  }
  return(nl);
}/*
extern const char ICON1[128];//пути к иконкам
extern const char ICON2[128];
extern const char ICON3[128];
extern const char ICON4[128];
extern const char ICON5[128];
extern const char ICON6[128];
extern const char ICON7[128];
extern const char ICON8[128];
extern const char ICON9[128];
extern const char NO_ICON[128];

int S_ICONS[10]={
(int)ICON1,
(int)ICON2,
(int)ICON3,
(int)ICON4,
(int)ICON5,
(int)ICON6,
(int)ICON7,
(int)ICON8,
(int)ICON9,
(int)NO_ICON
};
extern const int name_cl;//main menu
extern const int name_bk;//bookmarks
extern const int demon_cl;
*/
void mm_menu_iconhndl(void *data, int curitem, void *unk)
{
  NAMELIST *nl;
  WSHDR *ws;
  void *item=AllocMenuItem(data);
  nl=get_nlitem(curitem);
  if (nl)
  {
    if (nl->name)
    {
      ws=AllocMenuWS(data,nl->name->wsbody[0]+128);
      wstrcpy(ws,nl->name);
//      wsprintf(ws,"%d.%w",curitem+1,nl->name);
      /*if (!nl->is_daemon)
      {
        wsInsertChar(ws,2,1);//Цвет надписей
        wsInsertChar(ws,0xE008,1);//
      }*/
      if (nl->is_daemon)
      {
        wsInsertChar(ws,2,1);
        wsInsertChar(ws,0xE008,1);
      }
    }
    else
    {
      ws=AllocMenuWS(data,16);
      wsprintf(ws,"no name???");
    }
  }
  else
  {
    ws=AllocMenuWS(data,16);
    wsprintf(ws,"error!!!");
  }
  SetMenuItemText(data,item,ws,curitem);
}

const char *bm_name(int bm)
{
  switch(bm)
  {
  case 0: return BM1NAME;
  case 1: return BM2NAME;
  case 2: return BM3NAME;
  case 3: return BM4NAME;
  case 4: return BM5NAME;
  case 5: return BM6NAME;
  case 6: return BM7NAME;
  case 7: return BM8NAME;
  case 8: return BM9NAME;
  case 9: return BM0NAME;
  case 10: return BM_GREEN_NAME;
  }
  return(0);
}

const char *bm_file(int bm)
{
  switch(bm)
  {
  case 0: return BM1FILE;
  case 1: return BM2FILE;
  case 2: return BM3FILE;
  case 3: return BM4FILE;
  case 4: return BM5FILE;
  case 5: return BM6FILE;
  case 6: return BM7FILE;
  case 7: return BM8FILE;
  case 8: return BM9FILE;
  case 9: return BM0FILE;
  case 10: return BM_GREEN_FILE;
  }
  return(0);
}

int RunBM(int bm)
{
  const char *s=bm_file(bm);
  if (s)
  {
    if (strlen(s))
    {
      WSHDR *ws;
      ws=AllocWS(150);
      str_2ws(ws,s,128);
      ExecuteFile(ws,0,0);
      FreeWS(ws);
      return(1);
    }
  }
  return(0);
}

int RunBCFG(WSHDR *ws)
{
  char *p=malloc(64);
  ws_2str(ws,p,63);
  char param[256];//=malloc(256);//"4\\zbin\\etc\\";
//  strcat(s,p);
  //strcat(s,".bcfg");
  sprintf(param,"4:\\zbin\\etc\\%s.bcfg",p);
//  ShowMSG(2,(int)param);
  if(param)
  {
    if (strlen(param))
    {
      WSHDR *ws;
      ws=AllocWS(150);
      str_2ws(ws,param,128);
      //wsprintf(ws,"%s",param);
      ExecuteFile(ws,0,"4\\zbin\\utilities\\cfgedit.elf");
      //str_2ws(ws,"4\\zbin\\utilities\\CfgEdit.elf",256);
      //ExecuteFile(ws,0,param);
      FreeWS(ws);
      return(1);
    }
  }
  return(0);
}

const HEADER_DESC bm_menuhdr={0,0,131,21,NULL,(int)"Bookmarks...",LGP_NULL};
const int menusoftkeys[]={0,1,2};
const SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Config"},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};

void bm_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A) DisableIDLETMR();
}
int bm_menu_onkey(void *data, GUI_MSG *msg);
void bm_menu_iconhndl(void *data, int curitem, void *unk);

const MENU_DESC bm_menu=
{
  8,bm_menu_onkey,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt,
  0x10,//0x11,
  bm_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

int bm_menu_onkey(void *data, GUI_MSG *msg)
{
  int i;
  i=GetCurMenuItem(data);
  if (msg->keys==0x18)
  {
    //    GeneralFunc_F1(1);
//    ShowMSG(1,(int)"Under construction!");
    extern const char *successed_config_filename;
    WSHDR *ws;
    ws=AllocWS(150);
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws,0,0);
    FreeWS(ws);
    GeneralFunc_flag1(((MAIN_CSM*)FindCSMbyID(my_csm_id))->gui_id,1);
    return(1);
  }
  if (msg->keys==0x3D)
  {
    if (RunBM(i))
    {
      GeneralFunc_flag1(((MAIN_CSM*)FindCSMbyID(my_csm_id))->gui_id,1);
      return(1);
    }
    return(-1);
  }
  return(0);
}


void bm_menu_iconhndl(void *data, int curitem, void *unk)
{
  const char *s;
  WSHDR *ws;
  int l;
  void *item=AllocMenuItem(data);
/*
  int len1=strlen(ICON1);
  int len2=strlen(ICON2);
  int len3=strlen(ICON3);
  int len4=strlen(ICON4);  
  int len5=strlen(ICON5);
  int len6=strlen(ICON6);
  int len7=strlen(ICON7);
  int len8=strlen(ICON8);
  int len9=strlen(ICON9);
  */
  s=bm_name(curitem);
  if (s)
  {
    if ((l=strlen(s)))
    {
      ws=AllocMenuWS(data,l+64);
      wsprintf(ws,percent_t,s);
//      wsInsertChar(ws,name_bk,1);//Цвет надписей закладок
  //    wsInsertChar(ws,0xE008,1);//
    }
    else
    {
      s=bm_file(curitem);
      if (s)
      {
        if (strlen(s))
        {
          char *p=strrchr(s,'\\');
          if (p)
          {
            unsigned int pos;
            ws=AllocWS((l=strlen(p+1)+64));
            str_2ws(ws,p+1,l);
            if ((pos=wstrrchr(ws,ws->wsbody[0],'.'))!=0xFFFF)
            {
              CutWSTR(ws,pos-1);
            }
            // wsInsertChar(ws,name_bk,1);//Цвет надписей закладок
             //wsInsertChar(ws,0xE008,1);//
          }
          else goto L_EMPTY;
        }
        else goto L_EMPTY;
      }
      else
      {
      L_EMPTY:
        ws=AllocMenuWS(data,10);
        wsprintf(ws,"Empty");   
        //wsInsertChar(ws,name_bk,1);//Цвет надписей
        //wsInsertChar(ws,0xE008,1);//
      }
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей
    wsInsertChar(ws,0xE008,1);//
  }/*
  switch(curitem)//Вызов иконок
  { 
  case 0:
    if(len1!=0)//если не путь не равен 0 то рисуем иконку
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);//если пути до иконки нет то рисуем иконку NO_ICON
    break;
  case 1:
    if(len2 != 0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;
  case 2:
    if(len3!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;
  case 3:
    if(len4!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;
  case 4:
    if(len5!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;
  case 5:
    if(len6!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;
  case 6:
    if(len7!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;    
  case 7:
    if(len8!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;
  case 8:
    if(len9!=0)
      SetMenuItemIconArray(data,item,S_ICONS + curitem);
    else SetMenuItemIconArray(data,item,S_ICONS + 9);
    break;
  
  }*/
   SetMenuItemText(data,item,ws,curitem);
 //  SetMenuItemIconArray(data,item,S_ICONS);
}

void ShowBMmenu(void)
{
  patch_header(&bm_menuhdr);
  CreateMenu(0,0,&bm_menu,&bm_menuhdr,0,11,0,0);
}

int mm_menu_onkey(void *data, GUI_MSG *msg)
{
  MAIN_CSM *csm=MenuGetUserPointer(data);
  NAMELIST *nl=get_nlitem(GetCurMenuItem(data));
  int i;
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    i=msg->gbsmsg->submess;
    {
      if (i=='#'/*i=='0'|| i==GREEN_BUTTON*/)
      {
        ShowBMmenu();
        return(-1);
      }
      if ((i>='1')&&(i<='9'))
      {
        if (RunBM(i-'1')) return 1;
	return(-1);
      }
      if(i=='0')
      {
        if (RunBM(9)) return 1;
	return(-1);
      }
      if(i==GREEN_BUTTON)
      {
        if (RunBM(9)) return 1;
	return(-1);
      }
    }
    switch(i)
    {
//#ifdef ELKA
//    case LEFT_BUTTON: 
    case RIGHT_BUTTON:
//    case '#':
      i=((CSM_RAM *)(nl->p))->id;
      if (i!=CSM_root()->idle_id) CloseCSM(i);
      return 0;
/*#else
    case '#':
      i=((CSM_RAM *)(nl->p))->id;
      if (i!=CSM_root()->idle_id) CloseCSM(i);
      return 0;
#endif*/
    case LEFT_BUTTON: return 1;
    case '*'://case RIGHT_BUTTON:
      show_daemons=!show_daemons;
      RefreshGUI();
      return 0;      
    case LEFT_SOFT:
      CSMtoTop(CSM_root()->idle_id,-1);
      return(1);
    case ENTER_BUTTON:
      if (!nl->is_daemon)
      {
	CSMtoTop(((CSM_RAM *)(nl->p))->id,-1);
      }
      else
      {
       RunBCFG(nl->name);      
      }
      return(1);
    case RIGHT_SOFT:
      return(1);
    }
  }
  return(0);
}

const HEADER_DESC mm_menuhdr={0,0,131,21,NULL,(int)mmenu_hdr_txt,LGP_NULL};
const SOFTKEY_DESC mm_menu_sk[]=
{
  {0x0018,0x0000,(int)"Idle"},
  {0x0001,0x0000,(int)"Back"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB mm_menu_skt=
{
  mm_menu_sk,0
};

void mm_menu_ghook(void *data, int cmd)
{
  int i,j;
  if (cmd==9)
  {
    ClearNL();
  }
  if (cmd==0x0A)
  {
    DisableIDLETMR();
    i=GetCurMenuItem(data);
    Menu_SetItemCountDyn(data,j=GetNumberOfDialogs());
    if (i>=j) SetCursorToMenuItem(data,j-1);
  }
}

const MENU_DESC mm_menu=
{
  8,mm_menu_onkey,mm_menu_ghook,NULL,
  menusoftkeys,
  &mm_menu_skt,
  0x10,//0x11,
  mm_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_CSM *csm=(MAIN_CSM *)data;
  csm->csm.state=0;
  csm->csm.unk1=0;
  int f;
  int sz=0;
  unsigned int ul;
  extern const char csmlist_fname[128];
  if ((f=fopen(csmlist_fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    sz=fread(f,csm_text,32767,&ul);
    fclose(f,&ul);
  }
  if (sz>=0) csm_text[sz]=0;
  patch_header(&mm_menuhdr);
  csm->gui_id=CreateMenu(0,0,&mm_menu,&mm_menuhdr,1,GetNumberOfDialogs(),csm,0);
}

void maincsm_onclose(CSM_RAM *csm)
{
  ClearNL();
  my_csm_id=0;
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_CSM_DESTROYED)
  {
    RefreshGUI();
  }
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    //if (csm->show_csm==-1) RefreshGUI();
  }
  if (msg->msg==MSG_INCOMMING_CALL)
  {
    csm->csm.state=-3;
  }
  return(1);
}

const int minus11=-11;

const CSM_DESC maincsm=
{
  maincsm_onmessage,
  maincsm_oncreate,
#ifdef NEWSGOLD
  0,
  0,
  0,
  0,
#endif
  maincsm_onclose,
  sizeof(MAIN_CSM),
  1,
  &minus11
};

void ShowMenu(void)
{
  MAIN_CSM main_csm;
  InitConfig();
  if (!my_csm_id)
  {
//    main_csm.show_csm=-1;
    my_csm_id=CreateCSM(&maincsm,&main_csm,2);
  }
}
