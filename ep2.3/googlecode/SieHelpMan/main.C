#include "..\inc\swilib.h"
#include "conf_loader.h"

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
}MAIN_GUI;

//config
extern const char main_bg_color[4];
extern const unsigned int main_font;
extern const char main_text_color[4];
extern const unsigned int sk_font;
extern const char sk_color[4];
extern const unsigned int sys_re_t;
extern const char test_string[256];
extern const char t_a_b_color[4];
extern const int default_vibra_sta;
extern const int default_light_sta;


extern void kill_data(void *p, void (*func_p)(void *));

int num=0;
int flag=0;
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
int status_flag=0;
WSHDR *ws_info;
GBSTMR mytmr;
#define TMR_SECOND 216
#define IPC_MY_IPC "SieHelpMan"
#define IPC_UPDATE_STAT 1
const char ipc_my_name[]=IPC_MY_IPC;
//unsigned int REFRESH=5;
int vibra_flag;
int light_flag; //0,all , 1,display , 2,kbd , 3,do nothing
int screenw;
int screenh;
#ifdef ELKA
int y_b=24;
#else
int y_b=0;
#endif


const IPC_REQ my_ipc={
  ipc_my_name,
  ipc_my_name,
  NULL
};

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
  ((HEADER_DESC*)head)->rc.x2=screenw-1;
  ((HEADER_DESC*)head)->rc.y2=HeaderH()+YDISP-1;
}

#pragma inline
int get_string_width(WSHDR *ws, int font)
{
  int width=0;
  unsigned short *body=ws->wsbody;
  int len=body[0];
  while(len)
  {
    width+=GetSymbolWidth(body[len],font);
    len--;
  }
  return (width);
}

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

int pow(int x,int y)
{
  int z=1;
  while(y)
  {
    z=z*x;
    y--;
  }
  return z;
}


void soft_key(void)
{
  WSHDR *wsl = AllocWS(16);
  WSHDR *wsr = AllocWS(16);
  wsprintf(wsl, "Menu");
  wsprintf(wsr, "Exit");
  //DrawRectangle(0,screenh-GetFontYSIZE(FONT_MEDIUM)-2,screenw,screenh,0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
  DrawString(wsl,2,screenh-GetFontYSIZE(sk_font)-2,screenw,screenh,sk_font,32,sk_color,GetPaletteAdrByColorIndex(23)); 
  DrawString(wsr,screenw-get_string_width(wsr,sk_font)-4,screenh-GetFontYSIZE(sk_font)-2,screenw,screenh,sk_font,32,sk_color,GetPaletteAdrByColorIndex(23)); 
  FreeWS(wsl);
  FreeWS(wsr);
}

void lgp(void)
{
  WSHDR *ws = AllocWS(256);
  soft_key();
  wsprintf(ws, "LGP_ID: %d\n%t",num,num);
  num++;
  DrawString(ws,5,YDISP,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  wsprintf(ws, "LGP_ID: %d\n%t",num,num);
  num++;
  DrawString(ws,5,YDISP+(screenh-YDISP)/3,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  wsprintf(ws, "LGP_ID: %d\n%t",num,num);
  DrawString(ws,5,YDISP+2*(screenh-YDISP)/3,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws);
}


void keycode(int code)
{
  WSHDR *ws = AllocWS(128);
  wsprintf(ws, "Please long press\nLEFT SOFT KEY\nback to Menu.\n\nKeycode:\n\nDec: %d\n\nHex: %X", code, code);
  DrawRectangle(0,y_b,screenw,screenh,0,GetPaletteAdrByColorIndex(1),main_bg_color);
  DrawString(ws,5,y_b+screenh/12,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws);
}

void pic()
{
  WSHDR *ws = AllocWS(50);
  soft_key();
  if (num>=0)
  {
    int w=GetImgWidth(num);
    int h=GetImgHeight(num);
    wsprintf(ws, "Num:%d(D) %X(H) %dx%d",num,num,w,h);
    DrawString(ws,5,y_b,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
    DrawImg(screenw/2-w/2,(screenh+y_b+2*GetFontYSIZE(main_font)+2)/2-h/2,num);
  }
  else
  {
    wsprintf(ws, "Num:%d(D) %X(H)\nNO SUCH PICTURE!",num);
    DrawString(ws,5,y_b,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  }
  FreeWS(ws);
}

void font()
{
  int font_max;
  //char test_str[]="Test String 123";
  #ifdef NEWSGOLD
  #ifdef ELKA
  font_max=11;
  #else
  font_max=10;
  #endif
  #else
  font_max=10;
  #endif
  WSHDR *ws = AllocWS(64);
  WSHDR *ws1 = AllocWS(32);
  soft_key();
  wsprintf(ws1,"Font Size: %d",num);
  DrawString(ws1,5,screenh/3,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  if (num<0||num>font_max)
  {
    wsprintf(ws,"No such font");
    DrawString(ws,5,screenh/3+GetFontYSIZE(main_font)+screenh/12,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  }
  else
  {
    utf8_2ws(ws,test_string,strlen(test_string));
    DrawString(ws,5,screenh/3+GetFontYSIZE(main_font)+3,screenw,screenh,num,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  }
  FreeWS(ws1);
  FreeWS(ws);
}

void status(void)
{
  RAMNET *net=RamNet();
  int temp=GetAkku(1,3)-0xAAA+15;
  int volt=GetAkku(0,9);
  unsigned int err;
  int t_4=GetTotalFlexSpace(4,&err)/1024/1024;
  int f_4=GetFreeFlexSpace(4,&err)/1024/1024;
  #ifdef NEWSGOLD
  #ifdef ELKA
  char model[]="ELKA";
  #else
  char model[]="NewSGold";
  #endif
  #else
  char model[]="SGOLD";
  #endif
  DrawRectangle(0,y_b,screenw,screenh,0,GetPaletteAdrByColorIndex(1),main_bg_color);
  soft_key();
  GUI *igui=GetTopGUI();
  wsprintf(ws_info,"Phone: %s\nNet: %c%ddB T: %d.%d°C\nBts: %d-%d:%d\nC1: %d C2: %d\nV:%d.%02dV Cap: %02d%%\nCL: %d%% CC: %dMHz\nFreeRam: %uKb",model,(net->ch_number>=255)?'=':'-',net->power,temp/10,temp%10,net->ci,net->lac,net->ch_number,net->c1,net->c2,volt/1000,(volt%1000)/10,*RamCap(),GetCPULoad(),GetCPUClock(),GetFreeRamAvail()/1024);
  DrawString(ws_info,5,y_b,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  wsprintf(ws_info,"0: %dKb / %dKb\n1: %dKb / %dKb\n2: %dKb / %dKb\n4: %dMB / %dMB",GetFreeFlexSpace(0,&err)/1024,GetTotalFlexSpace(0,&err)/1024,GetFreeFlexSpace(1,&err)/1024,GetTotalFlexSpace(1,&err)/1024,GetFreeFlexSpace(2,&err)/1024,GetTotalFlexSpace(2,&err)/1024,f_4,t_4);
  DrawString(ws_info,5,y_b+GetFontYSIZE(main_font)*7,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
}


void TimerProc(void)
{
  if (!IsGuiOnTop(MAINGUI_ID)) status_flag=0;
  if(status_flag) status();
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}

void rgb24()
{
  WSHDR *ws1 = AllocWS(32);
  WSHDR *ws = AllocWS(32);
  soft_key();
  wsprintf(ws1,"RGB24 COLOR: %d",num);
  DrawString(ws1,5,y_b+screenh/12,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  if (num<0||num>23)
  {
    wsprintf(ws,"No such color");
    DrawString(ws,5,screenh/3,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  }
  else DrawRoundedFrame(5,screenh/3,screenw-5,screenh-GetFontYSIZE(FONT_MEDIUM)-screenh/12,0,0,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(num));
  FreeWS(ws);
  FreeWS(ws1);
}

void sound()
{
  WSHDR *ws = AllocWS(32);
  soft_key();
  wsprintf(ws, "Sound Num: %d", num);
  DrawString(ws,5,screenh/3,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  PlaySound(0,0,0,num,0);
  FreeWS(ws);
}

void text_attribute(void)
{
  WSHDR *wsh = AllocWS(128);
  int i;
  WSHDR *wst=AllocWS(8);
  wsprintf(wst,"256: ");
  int h_len=get_string_width(wst,main_font);
  FreeWS(wst);
  DrawRectangle(0,y_b,screenw,screenh,0,GetPaletteAdrByColorIndex(1),main_bg_color);
  soft_key();
  wsprintf(wsh, "Text attribute: \n    1:\n    2:\n    4:\n    8:\n  16:\n  32:\n  64:\n128:\n256:");
  DrawString(wsh,5,y_b,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23));
  WSHDR *ws = AllocWS(32);
  wsprintf(ws,"Test string");
  for(i=0;i<9;i++)
  {
    DrawString(ws,h_len+5,y_b+GetFontYSIZE(main_font)*(i+1),screenw,screenh,main_font,pow(2,i),main_text_color,t_a_b_color);
  }
  FreeWS(ws);
  FreeWS(wsh);
}

void vibra(void)
{
  WSHDR *ws = AllocWS(64);
  soft_key();
  if (num<0||num>100)
  {
    wsprintf(ws, "Vibra Power: %d%%\n\nWarning!\nOut of Power", num);
    SetVibration(0);
  }
  else
  {
    wsprintf(ws, "Vibra Power: %d%%", num);
    if (vibra_flag)
      SetVibration(num);
    else
      SetVibration(0);
  }
  DrawString(ws,5,screenh/3,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws);
}

  
void light(void)
{
  WSHDR *ws = AllocWS(64);
  soft_key();
  if (num<0||num>100)
  {
    wsprintf(ws, "Light: %d%%\n\nWarning!\nOut of Light", num);
  }
  else
  {
    wsprintf(ws, "Light: %d%%", num);
    switch(light_flag)
    {
      case 0:
        SetIllumination(0, 1, num, 0);
        SetIllumination(1, 1, num, 0);
        break;
      case 1:
        SetIllumination(0, 1, num, 0);
        break;
      case 2:
        SetIllumination(1, 1, num, 0);
        break;
      case 3:
        break;
    }
  }
  DrawString(ws,5,screenh/3,screenw,screenh,main_font,32,main_text_color,GetPaletteAdrByColorIndex(23)); 
  FreeWS(ws);
}

void onRedraw(MAIN_GUI *data)
{
  DrawRectangle(0,y_b,screenw,screenh,0,GetPaletteAdrByColorIndex(1),main_bg_color);
  if (flag!=4)
  {
    status_flag=0;
  }
  switch(flag)
  {
    case 0: lgp(); break;
    case 1: pic(); break;
    case 2: keycode(ENTER_BUTTON); break;
    case 3: font(); break;
    case 4: status(); status_flag=1; break;
    case 5: rgb24(); break;
    case 6: sound(); break;
    case 7: text_attribute(); break;
    case 8: vibra();break;
    case 9: light();break;
    default : flag=0; break;
  }
}


void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
  data->gui.state=1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

void create_num(int numx)
{
  num=num*10+numx;
  REDRAW();
}

void back_to_menu(void)
{
  status_flag=0;
  GeneralFuncF1(1); 
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  if (flag==2)
  {
    keycode(msg->gbsmsg->submess);
    if (msg->gbsmsg->submess==LEFT_SOFT)
    {
      if (msg->gbsmsg->msg==LONG_PRESS)
        back_to_menu();
    }
  }
  else
  {
    
    if (msg->gbsmsg->msg==KEY_UP)                        
    {                                                    
      switch(msg->gbsmsg->submess)                       
      {                                                  
        case '0':
          if(flag!=7||flag!=4) create_num(0); break;                  
        case '1':
          if(flag!=7||flag!=4) create_num(1); break;                  
        case '2': 
          if(flag!=7||flag!=4) create_num(2); break;                  
        case '3': 
          if(flag!=7||flag!=4) create_num(3); break;                  
        case '4': 
          if(flag!=7||flag!=4) create_num(4); break;                  
        case '5': 
          if(flag!=7||flag!=4) create_num(5); break;                  
        case '6': 
          if(flag!=7||flag!=4) create_num(6); break;                  
        case '7': 
          if(flag!=7||flag!=4) create_num(7); break;                  
        case '8': 
          if(flag!=7||flag!=4) create_num(8); break;                  
        case '9': 
          if(flag!=7||flag!=4) create_num(9); break;  
        case '*': 
          if(flag!=7||flag!=4) num=0; REDRAW(); break;  
        case '#': 
          if(flag!=7||flag!=4) num=num/10; REDRAW(); break;   
        case UP_BUTTON: 
          if(flag!=7||flag!=4) num=num+1; REDRAW(); break;      
        case DOWN_BUTTON: 
          if(flag!=7||flag!=4) num=num-1; REDRAW(); break;  
        case RIGHT_BUTTON: 
          if(flag!=7||flag!=4) num=num+1; REDRAW(); break;   
        case LEFT_BUTTON: 
          if(flag!=7||flag!=4) num=num-1; REDRAW(); break; 
        case ENTER_BUTTON:
          if(flag==8)
          {
            vibra_flag=!vibra_flag;
          }
          if(flag==9)
          {
            if(light_flag==3)
              light_flag=0;
            else
              light_flag++;
          }
          REDRAW();
          break; 
        case RIGHT_SOFT: 
          {
            if (vibra_flag) SetVibration(0);
            CloseCSM(MAINCSM_ID); 
            break;  
          }
        case LEFT_SOFT: 
          {
            if (vibra_flag) SetVibration(0);
            back_to_menu(); 
            break; 
          } 
      }  
    }  
    if (msg->gbsmsg->msg==LONG_PRESS)  
    {                                                    
      switch(msg->gbsmsg->submess)  
      {                                                  
        case UP_BUTTON: num=num+1; REDRAW(); break;      
        case DOWN_BUTTON: num=num-1; REDRAW(); break;    
        case RIGHT_BUTTON: num=num+1; REDRAW(); break;   
        case LEFT_BUTTON: num=num-1; REDRAW(); break;    
      }  
    }
  }
  return(0);
}

int method8(void){return(0);}
int method9(void){return(0);}

const void * const gui_methods[11]={
	(void *)onRedraw,	//Redraw
	(void *)onCreate,	//Create
	(void *)onClose,	//Close
	(void *)onFocus,	//Focus
	(void *)onUnfocus,	//Unfocus
	(void *)OnKey,		//OnKey
	0,
	(void *)kill_data,	//Destroy
	(void *)method8,
	(void *)method9,
	0
};

const RECT Canvas={0,0,0,0};

void startgui(void)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));	
  patch_rect((RECT*)&Canvas,0,0,screenw-1,screenh-1);
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();	
  MAINGUI_ID=CreateGUI(main_gui);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"SieHelpMan config updated!");
      InitConfig();
    }
  }
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }
  
  if (msg->msg==MSG_IPC)
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
      {
        switch (msg->submess)
        {
        case IPC_UPDATE_STAT:
#ifdef NEWSGOLD
          if (!getCpuUsedTime_if_ena())
          {
            StartCpuUsageCount();
          }
#endif
          GBS_StartTimerProc(&mytmr, sys_re_t*TMR_SECOND/10, TimerProc);
        }
      }
    }
    
  }
  return(1);
}


const int menusoftkeys[]={0,1,2};
SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Select"},
  {0x0001,0x0000,(int)"Exit"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};

#define ITEMS_N 12

MENUITEM_DESC menu_items[ITEMS_N]=
{
  {NULL,(int)"SystemInfo",     LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Picture",        LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"LGP_ID",         LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Sound",          LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Font",           LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"TextAttr",       LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"RGB24",          LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"VibraPower",     LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Light",          LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"KeyCode",        LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"Options",        LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
  {NULL,(int)"About",          LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2},
};

void lgp_gui(GUI *data)
{
  flag=0;
  startgui();
}

void pic_gui(GUI *data)
{
  flag=1;
  startgui();
}

void keycode_gui(GUI *data)
{
  flag=2;
  startgui();
}

void font_gui(GUI *data)
{
  flag=3;
  startgui();
}

void status_gui(GUI *data)
{
  flag=4;
  startgui();
}

void rgb24_gui(GUI *data)
{
  flag=5;
  startgui();
}

void sound_gui(GUI *data)
{
  flag=6;
  startgui();
}

void text_attr_gui(GUI *data)
{
  flag=7;
  startgui();
}

void vibra_gui(GUI *data)
{
  flag=8;
  startgui();
}

void light_gui(GUI *data)
{
  flag=9;
  startgui();
}

void options(GUI *data)
{
  extern const char *successed_config_filename;
  WSHDR *ws;
  ws=AllocWS(128);
  str_2ws(ws,successed_config_filename,128);
  ExecuteFile(ws,0,0);
  FreeWS(ws);
}

void about(GUI *data)
{
  ShowMSG(1, (int)"SieHelpMan\ncopyright 2007\nbinghelingxi");
}

const MENUPROCS_DESC menu_hndls[ITEMS_N]=
{
  status_gui,
  pic_gui,
  lgp_gui,
  sound_gui,
  font_gui,
  text_attr_gui,
  rgb24_gui,
  vibra_gui,
  light_gui,
  keycode_gui,
  options,
  about,
};


int menu_onkey(void *data, GUI_MSG *msg)
{
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
      case RIGHT_SOFT : CloseCSM(MAINCSM_ID); break;
      case LEFT_SOFT : GBS_SendMessage(MMI_CEPID,KEY_UP,ENTER_BUTTON); break;
    }
  }
  return (0);
}

void mm_menu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

HEADER_DESC mm_menuhdr={0,0,0,0,NULL,(int)"SieHelpMan",LGP_NULL};

const MENU_DESC mm_menu=
{
  8,menu_onkey,NULL,NULL,
  menusoftkeys,
  &menu_skt,
  0,
  NULL,
  menu_items,   //Items
  menu_hndls,   //Procs
  ITEMS_N   //n
};

void maincsm_oncreate(CSM_RAM *data)
{
  screenw=ScreenW();
  screenh=ScreenH();
  vibra_flag=default_vibra_sta;
  light_flag=default_light_sta;
  ws_info = AllocWS(256);
  MAIN_CSM *csm=(MAIN_CSM *)data;
  csm->csm.state=0;
  csm->csm.unk1=0;
  patch_header(&mm_menuhdr);
  csm->gui_id=CreateMenu(0,0,&mm_menu,&mm_menuhdr,0,ITEMS_N,0,0);
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_UPDATE_STAT,&my_ipc);
}


void Killer(void)
{
  extern void *ELF_BEGIN;
  GBS_DelTimer(&mytmr);
  FreeWS(ws_info);
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{
  SUBPROC((void *)Killer);
}

const int minus11=-11;
unsigned short maincsm_name_body[140];

const struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
}MAINCSM =
{
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
  },
  {
    maincsm_name_body,
    NAMECSM_MAGIC1,
    NAMECSM_MAGIC2,
    0x0,
    139
  }
};

void UpdateCSMname(void) 
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "SieHelpMan");
}

int main()
{
  char dummy[sizeof(MAIN_CSM)];
  InitConfig();
  UpdateCSMname();
  LockSched();
  MAINCSM_ID=CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched();
  return 0;
}
