#include "../../inc/swilib.h"


//===============================================================================================

#pragma inline=forced
int toupper(int c)
{
  if ((c>='a')&&(c<='z')) c+='A'-'a';
  return(c);
}
#pragma inline
int strcmp_nocase(const char *s1,const char *s2)
{
  int i;
  int c;
  while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
  return(i);
}

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

#pragma inline
void patch_header(const HEADER_DESC* head)
{
  ((HEADER_DESC*)head)->rc.x=0;
  ((HEADER_DESC*)head)->rc.y=YDISP;
  ((HEADER_DESC*)head)->rc.x2=ScreenW()-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}
#pragma inline
void patch_input(const INPUTDIA_DESC* inp)
{
  ((INPUTDIA_DESC*)inp)->rc.x=0;
  ((INPUTDIA_DESC*)inp)->rc.y=HeaderH()+1+YDISP;
  ((INPUTDIA_DESC*)inp)->rc.x2=ScreenW()-1;
  ((INPUTDIA_DESC*)inp)->rc.y2=ScreenH()-SoftkeyH()-1;
}
//===============================================================================================




#include "../_Dt/files.c"



#define TMR_SECOND 216


int maingui_id;

void SMART_REDRAW(void)
{
  int f;
  LockSched();
  f=IsGuiOnTop(maingui_id);
  UnlockSched();
  if (f) REDRAW();
}


int scr_w, scr_h;
WSHDR *ews;
const char perc_t[]="%t",
		perc_d[]="%d",
		empty_str[]="",
		perc_s[]="%s",
		black[]={0,0,0,100},
		trans_black[]={0,0,0,50},
		red[]={0xff,0,0,100},
		grey[]={0x7f,0x7f,0x7f,100},
		white[]={0xff,0xff,0xff,100},
		white0[]={0xff,0xff,0xff,50};


//===================================================================

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  void *data;
  int type, cur;
}MAIN_GUI;



extern void kill_data(void *p,void (*func_p)(void *));

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}



//===============================================================================================


void on_redraw(MAIN_GUI *data)
{
  DrawRectangle(0,YDISP,scr_w-1,scr_h-1,0, black, grey);
	
	int len=strlen(header);
	str_2ws(data->ws1,header,len);
	DrawString(data->ws1,3,3+YDISP,scr_w-4,3+YDISP+GetFontYSIZE(FONT_SMALL),
	     FONT_SMALL,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
	
	
  // FLIST *fl;
  // int len;
  // fl=FindFLISTtByN(curitem);
  // if (fl)
  // {
    // len=strlen(fl->name);
    // ws=AllocMenuWS(data,len+4);
    // if (fl->is_folder==IS_BACK || fl->is_folder==IS_FOLDER)
    // {
      // str_2ws(ws,fl->name,len);
      // wsInsertChar(ws,0x0002,1);
      // wsInsertChar(ws,0xE008,1);
    // }
    // else
    // {
      // str_2ws(ws,fl->name,len);
    // }
  // }
  // else
  // {
    // ws=AllocMenuWS(data,10);
    // wsprintf(ws, "Ошибка");
  // }
  // SetMenuItemText(data, item, ws, curitem);
	
	
	
	
  // DrawImg(0,0,S_ICONS[ICON_LOGO]);
  // wsprintf(data->ws1,LG_GRSTATESTRING,connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);

  // if(pm != pl)
  // {
     // DrawRectangle(0,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD),scr_w-1,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-2,0,
                     // GetPaletteAdrByColorIndex(0),
                     // GetPaletteAdrByColorIndex(0));
    // pos_status = ((scr_w-1) * pl) / pm;
    // DrawRectangle(1,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD)+1,pos_status,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-3,0,
                     // GetPaletteAdrByColorIndex(14),
                     // GetPaletteAdrByColorIndex(14));  
    // wstrcatprintf(data->ws1,"\nLoading images...");
  // }
  // DrawString(data->ws1,3,3+YDISP,scr_w-4,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     // FONT_SMALL,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  // wsprintf(data->ws2,percent_t,cltop?LG_GRSKEYCLIST:empty_str);
  // DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     // scr_w-4,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNRIGHT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  // wsprintf(data->ws2,percent_t,LG_GRSKEYEXIT);
  // DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     // scr_w>>1,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
}

void on_create(MAIN_GUI *data,void *(*malloc_adr)(int))
{
  int scr_w=ScreenW();
  int scr_h=ScreenH();
  data->ws1=AllocWS(256);
  data->ws2=AllocWS(256);
  data->gui.state=1;
}

void on_close(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  FreeWS(data->ws2);
  data->gui.state=0;
  SUBPROC((void *)ElfKiller);
}

void on_focus(MAIN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
  DisableIDLETMR();
  data->gui.state=2;
}

void on_unfocus(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  if (data->gui.state!=2)
    return;
  data->gui.state=1;
}

int on_key(MAIN_GUI *data,GUI_MSG *msg)
{
  DirectRedrawGUI();
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
    case LEFT_SOFT:
      return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
    case RIGHT_SOFT:
      break;
    case GREEN_BUTTON:
      break;
    case '0':
      break;
    }
  }
  return(0);
}

int method8(void){return(0);}

int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)on_redraw,
  (void *)on_create,
  (void *)on_close,
  (void *)on_focus,
  (void *)on_unfocus,
  (void *)on_key,
  0,
  (void *)kill_data, //method7, //Destroy
  (void *)method8,
  (void *)method9,
  0
};







////////////////////////////////////////////////
////////////////////////////////////////////////
/////////////////////////////////////////////////







//char header[128];
int list_menu_onkey(void *data, GUI_MSG *msg)
{
  // FVIEW *fview=MenuGetUserPointer(data);
  // FLIST *fl;
  // int i, n;
  // i=GetCurMenuItem(data);
  // fl=FindFLISTtByN(i);
  
  if (msg->keys==0x3D)
  {
    /* if (fl) 
    {
      if (fl->is_folder==IS_FOLDER || fl->is_folder==IS_BACK)
      {
        int len;
        if (strcmp(fl->fullname,"ROOT"))
        {
          strncpy(header,fl->fullname,sizeof(header)-1);
          len=strlen(fl->fullname);
          header[len>sizeof(header)-1?sizeof(header)-1:len]=0;
          n=FindFiles(fl->fullname,fview->type);
        }
        else
        {
          int CreateRootMenu();
          n=CreateRootMenu();
        }
		// S_ICONS=malloc(n*4);
		// for(j=0; j<n; j++) S_ICONS[j]=73;
        Menu_SetItemCountDyn(data,n);
        SetCursorToMenuItem(data, 0);
        RefreshGUI();
      }
      else
      {
        SavePath(fview->gui,fl);        
        return (1);
      }
    } */
    //    GeneralFunc_F1(1);
    return(-1);
  }
  if (msg->keys==0x18)
  {
    /* if (fl)
    {
      if (fl->is_folder!=IS_BACK)
      {
        SavePath(fview->gui,fl);
        return (1);
      }      
    }   */ 
    return (-1);
  }
  return (0);
}

//int S_ICONS[512];

void list_menu_ghook(void *data, int cmd)
{
	// int j;
	// IMGHDR *si;
  void *fview=MenuGetUserPointer(data);
	switch(cmd)
	{
		case TI_CMD_CREATE:
  maingui_id=CreateGUI(fview);
			break;
		case TI_CMD_DESTROY:
			// for(j=0;j<512;j++)
			 // if(S_ICONS[j]&&(S_ICONS[j]!=73))
			 // {
				// si=GetPITaddr(S_ICONS[j]);
				// mfree(si->bitmap);
				// mfree(si);
			 // }
			//Free_FLIST();
			//mfree(fview); 
			break;
		case TI_CMD_FOCUS:
			DisableIDLETMR();
			break;
		case TI_CMD_REDRAW:
			break;
	}
}

void list_menu_iconhndl(void *data, int curitem, void *user_pointer)
{
}

int softkeys[]={0,1,2};
SOFTKEY_DESC _sk[]=
{
  {0x0018,0x0000,(int)"Select"},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)"+"}
};

SOFTKEYSTAB _skt=
{
  fmenu_sk,0
};

HEADER_DESC list_HDR={0,0,0,0,NULL,(int)header,LGP_NULL};

MENU_DESC list_STRUCT=
{
  8,list_menu_onkey,list_menu_ghook,NULL,
  softkeys,
  &_skt,
  0x10,//+1,
  list_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};





////////////////////////////////////////////////
////////////////////////////////////////////////
/////////////////////////////////////////////////




















const RECT Canvas={0,0,0,0};

int main(char *exename, void *t)
{
  EDITCONTROL ec;
  char path[128];
  char *s, *rev=0;
  int n, c, len;
  int tt[2];
  memcpy(tt, t, 8);
  void *ed_gui=(void*)tt[0];
  int type=tt[1];
  
  //MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  //zeromem(main_gui,sizeof(MAIN_GUI));
  //main_gui->data=ed_gui;
  //main_gui->type=type;
  EDIT_ExtractFocusedControl(ed_gui,&ec);
  ws_2str(ec.pWS,path,127);
  s=path;
  while((c=*s++))
  {
    if (c=='\\' && *s!='\0') rev=s;  
  }
  if (!rev) n=CreateRootMenu();
  else
  {
    *rev=0;
    strncpy(header,path,sizeof(header)-1);
    len=strlen(path);
    header[len>sizeof(header)-1?sizeof(header)-1:len]=0;
    n=FindFiles(path,type);
  }


  //MAIN_CSM *csm=(MAIN_CSM*)data;
  StoreXYXYtoRECT((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
//  view_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  /* csm->csm. *///data->state=0;
  /* csm->csm.. *///data->unk1=0;
  /* csm->gui_id= *//* MAINGUI_ID= */CreateGUI(main_gui);

 /*  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
//  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //maingui_id=CreateGUI(main_gui); */
  ews=AllocWS(512);
  
  //patch_header(&list_HDR);
  //CreateMenu(0,0,&list_STRUCT,&list_HDR,0,0,&main_gui->gui,0);
  
  return 0;
}

