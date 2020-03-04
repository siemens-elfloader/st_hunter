#include "..\inc\swilib.h"

#pragma inline
void patch_header(const HEADER_DESC* headc)
{
  HEADER_DESC *head=(HEADER_DESC *)headc;
  head->rc.x=0;
  head->rc.y=YDISP;
  head->rc.x2=ScreenW()-1;
  head->rc.y2=HeaderH()+YDISP;
}

const char wintranslation[128]=
{
  0x5F,0x5F,0x27,0x5F,0x22,0x3A,0xC5,0xD8,0x5F,0x25,0x5F,0x3C,0x5F,0x5F,0x5F,0x5F,
  0x5F,0x27,0x27,0x22,0x22,0x07,0x2D,0x2D,0x5F,0x54,0x5F,0x3E,0x5F,0x5F,0x5F,0x5F,
  0xFF,0xF6,0xF7,0x5F,0xFD,0x83,0xB3,0x15,0xF0,0x63,0xF2,0x3C,0xBF,0x2D,0x52,0xF4,
  0xF8,0x2B,'I' ,'i' ,0xA3,0xE7,0x14,0xFA,0xF1,0xFC,0xF3,0x3E,0x5F,0x5F,0x5F,0xF5,
  0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
  0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
  0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
  0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF
};
const unsigned short dos2unicode[128]=
{
  0x0410,0x0411,0x0412,0x0413,0x0414,0x0415,0x0416,0x0417,
  0x0418,0x0419,0x041A,0x041B,0x041C,0x041D,0x041E,0x041F,
  0x0420,0x0421,0x0422,0x0423,0x0424,0x0425,0x0426,0x0427,
  0x0428,0x0429,0x042A,0x042B,0x042C,0x042D,0x042E,0x042F,
  0x0430,0x0431,0x0432,0x0433,0x0434,0x0435,0x0436,0x0437,
  0x0438,0x0439,0x043A,0x043B,0x043C,0x043D,0x043E,0x043F,
  0x002D,0x002D,0x002D,0x00A6,0x002B,0x00A6,0x00A6,0x00AC,
  0x00AC,0x00A6,0x00A6,0x00AC,0x002D,0x002D,0x002D,0x00AC,
  0x004C,0x002B,0x0054,0x002B,0x002D,0x002B,0x00A6,0x00A6,
  0x004C,0x0433,0x00A6,0x0054,0x00A6,0x003D,0x002B,0x00A6,
  0x00A6,0x0054,0x0054,0x004C,0x004C,0x002D,0x0433,0x002B,
  0x002B,0x002D,0x002D,0x002D,0x002D,0x00A6,0x00A6,0x002D,
  0x0440,0x0441,0x0442,0x0443,0x0444,0x0445,0x0446,0x0447,
  0x0448,0x0449,0x044A,0x044B,0x044C,0x044D,0x044E,0x044F,
  0x0401,0x0451,0x0404,0x0454,0x0407,0x0457,0x040E,0x045E,
  0x00B0,0x2022,0x00B7,0x0076,0x2116,0x00A4,0x00A6,0x00A0
};

#pragma inline
unsigned int char8to16(int c)
{
  if (c>=128)
  {
    //Win->Dos
    c=wintranslation[c-128];
    if (c<128) return(c);
    return(dos2unicode[c-128]);
  }
  return(c);
}



void ascii2ws(WSHDR *ws, /*const*/ char *s)
{
  char c;
  CutWSTR(ws,0);
  while((c=*s++))
  {
    wsAppendChar(ws,char8to16(c));
  }
}

unsigned short maincsm_name_body[140];
const int minus11=-11;
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;


//void do_ed();
void create_ed(void);


#define ONE_SECOND (212)

CSM_DESC icsmd;
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);
int sock;
int connect_state;
char buf[16384];
int pbuf;
GBSTMR mytmr;

char *item[20];
int x;


int receive;
int rec;
int ed;
int open;

typedef struct
{
  CSM_RAM csm;
  int gui_id;
}MAIN_CSM;

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

typedef struct
{
  char date[64];
  char nick[32];
  char comm[1024];
  char elf[32];  
  //int num;
  char numb[5];// в место num
}SVN_REV;

SVN_REV revis[20];
const char percent_t[]="%t";
const char per_s[]="%s";
//------------------------------------------------------------------------------
void *about()
{ 
  char s[128];
  snprintf(s,255,"Revisor v0.1.0\n(c)feanor\n(m)kluchnik\n%s %s",__DATE__,__TIME__);
  ShowMSG(2, (int)s);
  return 0;
}

void bm_menu_iconhnd(void *data, int curitem, void *unk);
void bm_menu_ghook(void *data, int cmd){}
int bm_menu_onkey2(void *data, GUI_MSG *msg);
HEADER_DESC bm_menuhdr2={0,0,131,21,NULL,(int)"Revision control",LGP_NULL};
const int menusoftkeys[]={0,1,2};

const SOFTKEY_DESC menu_sk2[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)"Close"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt2=
{
  menu_sk2,0
};

const ML_MENU_DESC bmmenu=
{
  8,bm_menu_onkey2,bm_menu_ghook,NULL,
  menusoftkeys,
  &menu_skt2,
  0x10,//0x11
  bm_menu_iconhnd,
  NULL,   //Items
  NULL,   //Procs
  0,   //n
  1  
};


void bm_menu_iconhnd(void *data, int curitem, void *unk)
{ 
  char *s=malloc(64),*ss=malloc(64);
  WSHDR *ws;
  WSHDR *ws1;
  void *item=AllocMLMenuItem(data);
  sprintf(s,"%s: %s %s",revis[curitem].elf,revis[curitem].numb,revis[curitem].nick);
  sprintf(ss,per_s,revis[curitem].date);
  ws1=AllocWS(128);
  if (s)
  {
    if (strlen(s))
    { 
      ws=AllocMenuWS(data,strlen(s)+128);
      wsprintf(ws,"%d.%t",curitem+1,s);
//      wsInsertChar(ws,2,1);//Цвет надписей
  //    wsInsertChar(ws,5,1);//
      
      wsprintf(ws1,percent_t,ss);//Цвет описаний
    //  wsInsertChar(ws1,2,1);//0x0002
     // wsInsertChar(ws1,4,0);
    }
    else
    { 
      ws=AllocMenuWS(data,10);
      wsprintf(ws,percent_t,"error");
      wsInsertChar(ws,2,1);//Цвет надписей
      wsInsertChar(ws,0xE008,1);//
      wsprintf(ws1,percent_t,"error");//Цвет описаний
      //wsInsertChar(ws1,2,1);//0x0002
      //wsInsertChar(ws1,0xE008,0);
    }
  }
  else
  {
    ws=AllocMenuWS(data,10);
    wsprintf(ws,"error");
    wsInsertChar(ws,2,1);//Цвет надписей

  }
 
//    SetMenuItemIconArray(data,item,S_ICONS + curitem);
    SetMLMenuItemText(data, item, ws, ws1, curitem);
 //  SetMenuItemText(data,item,ws,curitem);
}


void ShowReviz(void)
{ 
  patch_header(&bm_menuhdr2);
  CreateMultiLinesMenu(0,0,&bmmenu,&bm_menuhdr2,0,20);//number_book - кол-во закладок
}


//====================================
//         Обработка клавиш
//====================================

int bm_menu_onkey2(void *data, GUI_MSG *msg)//кейхук для закладок
{
  int i;
  i=GetCurMenuItem(data);
//  DirectRedrawGUI();
//  int k=msg->gbsmsg->submess;
  if(msg->keys==0x14)
   {
     about();
     return (-1);
   }
  if (msg->keys==0x3D)
    {
      x=i;
      create_ed();
    }
  return(0);
}

//------------------------------------------------------------------------------

int idd;
int icon[]={0x58,0};

SOFTKEY_DESC menu_sk[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB menu_skt=
{
  menu_sk,0
};


HEADER_DESC ed1_hdr={0,0,131,21,NULL,NULL,LGP_NULL};

void ed1_locret(void){}

int ed1_onkey(GUI *data, GUI_MSG *msg)
{
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {    
   /* case '4':*/// case  LEFT_BUTTON: x=(x+1)%20;  /*ed=0; open=1;*/ REDRAW(); break;
   /* case '6':*/ //case RIGHT_BUTTON: x=(x+19)%20; /*ed=0; open=1;*/; REDRAW(); break;
     
    //case '7': create_ed(); break;
    
   case 4: ed=0; REDRAW(); return (1);//break; //
   case RIGHT_BUTTON:
      x=(x+1)%20;
      GeneralFunc_flag1(idd,1);
      create_ed();
      break;
   case LEFT_BUTTON:
      x=(x+19)%20;
      GeneralFunc_flag1(idd,1);
      create_ed();
      break;
    }
  }
  return(0);
}

void ed1_ghook(GUI *data, int cmd){};

INPUTDIA_DESC ed1_desc=
{
  1,
  ed1_onkey,
  ed1_ghook,
  (void *)ed1_locret,
  0,
  &menu_skt,
  {0,22,131,153},
  4,
  100,
  101,
  0,

//  0x00000001 - Выровнять по правому краю
//  0x00000002 - Выровнять по центру
//  0x00000004 - Инверсия знакомест
//  0x00000008 - UnderLine
//  0x00000020 - Не переносить слова
//  0x00000200 //- bold
  0x00000000,
//  0x00000002 //- ReadOnly
//  0x00000004 - Не двигается курсор
//  0x40000000 // Поменять местами софт-кнопки
  0x00000002
};


void create_ed(void)
{
  WSHDR *ews;
  ews=AllocWS(1024);//FreeWS(ews);
  
  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;

  PrepareEditControl(&ec);
  eq=AllocEQueue(ma,mfree_adr());
  char *s=malloc(64);
  sprintf(s,"%s: %s",revis[x].elf,revis[x].nick);
  ed1_hdr.lgp_id = (int)s; 

  
  wsprintf(ews,"< %i >",x+1);
  ConstructEditControl(&ec,1,0x40,ews,256);
  AddEditControlToEditQend(eq,&ec,ma);
  
  wsprintf(ews,"%t %s","Ревизия:",revis[x].numb);
  ConstructEditControl(&ec,1,0x40,ews,256);
  AddEditControlToEditQend(eq,&ec,ma);
  
  wsprintf(ews,"%t%s","Aвтор:",revis[x].nick);
  ConstructEditControl(&ec,1,0x40,ews,256);
  AddEditControlToEditQend(eq,&ec,ma);

  
  wsprintf(ews,"%t%t","Проект:",revis[x].elf);
  ConstructEditControl(&ec,1,0x40,ews,256);
  AddEditControlToEditQend(eq,&ec,ma);
  
  
  wsprintf(ews,"%t:%s","Дата",revis[x].date);
  ConstructEditControl(&ec,1,0x40,ews,256);
  AddEditControlToEditQend(eq,&ec,ma);
  
  
  //wsprintf(ews,percent_t,revis[x].comm);
  
  ascii2ws(ews,revis[x].comm);
  ConstructEditControl(&ec,3,0x40,ews,1024);
  AddEditControlToEditQend(eq,&ec,ma);

  FreeWS(ews);
  idd=CreateInputTextDialog(&ed1_desc,&ed1_hdr,eq,1,0); 
}

//=====================================================================

//=====================================================================
void create_connect(void)
{
  SOCK_ADDR sa;
  //Устанавливаем соединение
  connect_state=0;
  pbuf=0;
  sock=socket(1,1,0);
  if (sock!=-1)
  {
    sa.family=1;
    sa.port=htons(80);
    sa.ip=htonl(IP_ADDR(82,146,43,164));
    if (connect(sock,&sa,sizeof(sa))!=-1)
    {
      connect_state=1;
      receive=1;
      REDRAW();
    }
    else
      closesocket(sock);
  }
}

void do_start_connection(void)
{
  SUBPROC((void *)create_connect);
}

char req_buf[256];

void send_req(void)
{
  sprintf(req_buf,"GET /elf/rss.php"
          " HTTP/1.0\r\nHost: perk11.info\r\n\r\n");
  send(sock,req_buf,strlen(req_buf),0);
  connect_state=2;
}



void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
  }
}



void get_answer(void)
{
  int i=pbuf;
  if (i==16383)
    end_socket();
  else
  {
    i=recv(sock,buf+i,16383-i,0);
    if (i>=0)
    {
      pbuf+=i;
    }
    else
    {
      end_socket();
    }
  }
}

/*char * strrrchr(char *str, char chr)
{
  for(int i=0; i!=strlen(str); i++)
  {
    if (str[i]==chr) {str[i]=0; return(*str);}
  }
}*/



int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (int k=0; ; k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A) {ret=ret*10+attr[k]-0x30;} else { if ( attr[k]=='-') {neg=-1;} else {return(ret*neg);}}
  }
  
}


void Parsing()
{
  
  item[0]=strstr(buf,"<item>");
  for(int i=0;i<19;i++)
    item[i+1]=strstr(item[i]+5,"<item>");
  /*
  item[1]=strstr(item[0]+5,"<item>");
  item[2]=strstr(item[1]+5,"<item>");
  item[3]=strstr(item[2]+5,"<item>");
  item[4]=strstr(item[3]+5,"<item>");
  item[5]=strstr(item[4]+5,"<item>");
  item[6]=strstr(item[5]+5,"<item>");
  item[7]=strstr(item[6]+5,"<item>");
  item[8]=strstr(item[7]+5,"<item>");
  item[9]=strstr(item[8]+5,"<item>");  
  item[10]=strstr(item[9]+5,"<item>");
  item[11]=strstr(item[10]+5,"<item>");
  item[12]=strstr(item[11]+5,"<item>");
  item[13]=strstr(item[12]+5,"<item>");
  item[14]=strstr(item[13]+5,"<item>");
  item[15]=strstr(item[14]+5,"<item>");
  item[16]=strstr(item[15]+5,"<item>");
  item[17]=strstr(item[16]+5,"<item>");
  item[18]=strstr(item[17]+5,"<item>");
  item[19]=strstr(item[18]+5,"<item>");*/
  
  int ii=0;
  for(int i=0; i<=19; i++)
  {
    snprintf(revis[i].nick,32,"%s",strstr(item[i],"CDATA")+6);
      for (int j=1; ;j++) {if ((revis[i].nick)[j]==' ') {(revis[i].nick)[j]=0;ii=j; break;}}
     
    snprintf(revis[i].date,64,"%s",strstr(item[i],"Date")+5);
      for (int j=1; ;j++) {if ((revis[i].date)[j]=='+') {(revis[i].date)[j]=0; break;}}
    
    snprintf(revis[i].comm,1024,"%s",strstr(item[i],"Changed paths:"));
      for (int j=1; ;j++) {if ((revis[i].comm)[j]==']' && (revis[i].comm)[j+1]==']') {(revis[i].comm)[j]=0; break;}}
      
    snprintf(revis[i].elf,32,"%s",strstr(item[i],"/ARM/")+5);
      for (int j=1; ;j++) {if ((revis[i].elf)[j]=='/' || (revis[i].elf)[j]==' ') {(revis[i].elf)[j]=0; break;}}
      
    snprintf(revis[i].numb,5,"%s",strstr(item[i],"CDATA")+ii+31);  
      for (int j=1; ;j++) {if ((revis[i].numb)[j]=='.') {(revis[i].numb)[j]=0; break;}}
     }
  rec=1;ed=1;pbuf=(ScreenW()-1)*100;
}




void Save()
{ 
  unsigned int ul;
  
  int f=fopen("0:\\Misc\\Data Inbox\\svnn.log",A_ReadWrite+A_Create,P_READ+P_WRITE,&ul);
  if (f!=-1)
  {
    fwrite(f,buf,strlen(buf),&ul);    
    fclose(f,&ul);   
  }
}

int dott=0;
char *dot[6]={".","..","...","....",".....","......"};
void OnRedraw(MAIN_GUI *data)
{ 
  WSHDR *ws = AllocWS(256);
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(1));

 /* char pic[128];
  sprintf(pic,"0:\\Zbin\\netutils\\svn\\proj\\%s.png",revis[x].elf);
  //DrawImg(32,32,(int)pic);
  
  volatile int hFile = -1;
  unsigned int errcode;
  hFile = fopen(pic,A_ReadOnly,P_READ,&errcode);
  if(hFile!=-1) DrawImg (32,32,(int)pic);
  fclose(hFile, &errcode);
  */
  wsprintf(ws, "<  Revision: %s  >",revis[x].numb);
  DrawString(ws,0,20,ScreenW()-1,100,FONT_SMALL,34,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(4)); 
  
  wsprintf(ws, "%t %t %t",revis[x].date,revis[x].nick,revis[x].elf);
  DrawString(ws,0,50,ScreenW()-1,/*ScreenH()-1*/120,7,34,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(4)); 
  
  if(pbuf/100 && sock!=-1)
  {
    if(dott<4) dott++;
    else dott=4;
    wsprintf(ws, "Please wait%s %d b",dot[dott],pbuf);
    DrawString(ws,2,ScreenH()-42,ScreenW()-1,ScreenH()-1,FONT_SMALL,34,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(4)); 
  }
  DrawRoundedFrame(0,ScreenH()-17,pbuf/100,ScreenH()-1,0,0,0,GetPaletteAdrByColorIndex(4),GetPaletteAdrByColorIndex(4));
  /*
  str_2ws(ws,revis[x].comm,strlen(revis[x].comm));//(ws,revis[x].comm,strlen(revis[x].comm));
 // wsprintf(ws, "%s ",revis[x].comm);
  DrawString(ws,5,25,ScreenW()-1,ScreenH()-1,7,32,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(22)); */
  
/*  if (rec==1 && ed==1)
  {
    create_ed();
   // ShowReviz();
    open=0;
  }
  */
  FreeWS(ws);
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
  DisableIDLETMR();
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {    
    case '4': case LEFT_BUTTON: x=(x+1)%20;   /*if(x>0) x--; else x=19;*/REDRAW(); break;
    case '6': case RIGHT_BUTTON: x=(x+19)%20; /*if(x<20) x++; else x=0; */REDRAW(); break;
    case '3' :ShowReviz();break;
    
    case GREEN_BUTTON: do_start_connection();  REDRAW(); break;
    
    case 1: case '5': case ENTER_BUTTON:create_ed(); break;   
    case RIGHT_SOFT:  case RED_BUTTON: return (1);
    default: REDRAW(); break;
    }    
  }
  return(0);
}/*
#ifdef NEWSGOLD
void kill_data(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  mfree_adr(data);
}
#else
extern void kill_data(void *p, void (*func_p)(void *));
#endif*/
/*
void onDestroy(MAIN_GUI *data, void (*mfree_adr)(void *))
{
extern void kill_data(void *p, void (*func_p)(void *));
kill_data(data,mfree_adr);
}
*/
 extern void kill_data(void *p, void (*func_p)(void *));
int method8(void){return(0);}
int method9(void){return(0);}
const void * const gui_methods[11]={
  (void *)OnRedraw,	
  (void *)onCreate,	
  (void *)onClose,	
  (void *)onFocus,	
  (void *)onUnfocus,
  (void *)OnKey,	
  0,
/*#ifdef NEWSGOLD
  (void *)onDestroy,
#else*/
  (void *)kill_data,	
//#endif
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,129,129};
void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);
  MAINGUI_ID=csm->gui_id;
  do_start_connection();

}



void ElfKiller(void)
{
 
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{
//  GBS_DelTimer(&mytmr);
  SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
  MAIN_CSM *csm=(MAIN_CSM*)data;
  if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
  {
    csm->csm.state=-3;
  }

  if (msg->msg==MSG_HELPER_TRANSLATOR)
  {
    if ((int)msg->data1==sock)
    {
      //Если наш сокет
      switch((int)msg->data0)
      {
      case ENIP_SOCK_CONNECTED:

          //Если посылали запрос
          SUBPROC((void *)send_req);
          REDRAW();
        
        break;
      case ENIP_SOCK_DATA_READ:

          //Если посылали send
          SUBPROC((void *)get_answer);
          REDRAW();


        break;
      case ENIP_SOCK_REMOTE_CLOSED:
        //Закрыт со стороны сервера
        if (connect_state) SUBPROC((void *)end_socket);
        break;
      case ENIP_SOCK_CLOSED:
        //Закрыт вызовом closesocket
        ///Save();
        Parsing();
     //   ShowReviz();
        ShowMSG(1,(int)"!!");
        connect_state=0;
        sock=-1;
        break;
      }
    }    
  }
  return(1);
}

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
  WSHDR *ws=AllocWS(256);
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Revision");
  FreeWS(ws);
}


int main(char *exename, char *fname)
{
  char dummy[sizeof(MAIN_CSM)];
  MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,0);
  UpdateCSMname();
  return 0;
}
