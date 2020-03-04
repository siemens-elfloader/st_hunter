#include "..\inc\swilib.h"
#include "string.h"
#include "strings.h"
#include "List.h"
#include "File.h"

#include <stdarg.h>

#define ECINPUT 3
#define ECOUTPUT 5
#define BUFSIZE 16383

//const char per_t[]="%t";
const int minus11=-11;
unsigned short maincsm_name_body[140];
int MAINCSM_ID;
unsigned int traf=0;
GUI *ggui=0;
char curlang[70];
char curtr[70];
char fname[100];
CList *list = NULL;

#define wslen(ws) ws->wsbody[0]

#pragma inline
void patch_rect(RECT*rc,int x,int y, int x2, int y2)
{
  rc->x=x;
  rc->y=y;
  rc->x2=x2;
  rc->y2=y2;
}

int sock;
int connect_state=0;
char buf[BUFSIZE+1];
int pbuf=0;

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

/*

settr()
{
  sprintf(curtr,"%s  ", tr[deftr]);
  savesettings();
}

setlang()
{
  sprintf(curlang,"%s",(*lg[deftr])[ deflang[deftr] ].s2);
  savesettings();
}
*/

void create_connect(void)
{
  //Устанавливаем соединение
  SOCK_ADDR sa;
  connect_state=0;
  pbuf=0;
  sock=socket(1,1,0);
  if (sock!=-1)
  {
    sa.family=1;
    sa.port=htons(80);
    //Имя сайта:  	inviz.su
    //IP адрес:	   	213.248.50.72
    sa.ip=htonl(IP_ADDR(213,248,50,72));

    if (connect(sock,&sa,sizeof(sa))!=-1)
    {
      connect_state=1;
    }
    else
      closesocket(sock);
  }
}

char UIN[10];
void send_req(void)
{
  char *str2 = (char*)malloc(400);
  char *req_buf = (char*)malloc(strlen(str2)+800);
  sprintf(req_buf,
            "GET /?uin=%s"
                " HTTP/1.1\r\n"
                  "Host: inviz.su\r\n"
                    "\n"
                      "0\n"
                        "\n",
                         UIN
                          );
      
  mfree(str2);
  traf+=send(sock,req_buf,strlen(req_buf),0);
  connect_state=2;
  mfree(req_buf);
}

void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
    connect_state=0;
  }
}


const char _rn[]="\r\n";
static void logwrite(char *name,char *text)
{  
  CFile file(name);
  if(file.open(A_ReadWrite + A_Create + A_Append)!=-1)
  {
    file.write(text,strlen(text));
    file.write(_rn,2);
  }
  else
    ShowMSG(1,(int)"Error write to log!");
  file.close();
}

static void logwrite(char *name,char *text,int len)
{  
  CFile file(name);
  if(file.open(A_ReadWrite + A_Create + A_Append)!=-1)
  {
    file.write(text,len);
    file.write(_rn,2);
  }
  else
    ShowMSG(1,(int)"Error write to log!");
  file.close();
}

void get_answer(void)
{
  int i=pbuf;
  if (i==BUFSIZE)
    end_socket();
  else
  {
    traf+=(i=recv(sock,buf+i,BUFSIZE-i,0))/100;
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

void setstatus(char *status);

char *DelStr(char *string,char *substring,int need_n)
{
  char *buf = (char*)malloc(strlen(string)+strlen(substring));
 // char *s, *s2;

  char *ptr = string;
  int i = 0;
//  int len = strlen(ptr);
  int size = 0;
  int c;
  while(*ptr/* && size++ < len*/)
  {
    c = (*ptr++);
    if( c == substring[0] )
    {
      char *cmp = (char*)malloc(strlen(substring)-1);
      strncpy(cmp,ptr-1,strlen(substring)-1);
      if(! (memcmp(cmp,substring,strlen(substring)-1)) )
       {
         ptr+=strlen(substring);
         if(need_n) *(buf+(size++)) = '\n';
       }
      c = 0;
     }
     else
       *(buf+(size++)) = c;
    i++;
  }
  return buf;
}


char *ChangeStr(char *string,char *substring,char *newstring,int need_n)
{
 char *buf = (char*)malloc(strlen(string));
 char *ptr = string;
 int c;
 int size = 0;
 while(*ptr)
  {
    c = (*ptr++);
    if( c == substring[0] )
    {
       char *cmp = (char*)malloc(strlen(substring));
       strncpy(cmp,ptr-1,strlen(substring)-1);
       ///ShowMessage(cmp);
       if(!(memcmp(cmp,substring,strlen(substring)-1)) )
        {
           for(int i=0;i<strlen(newstring);i++)
           {
             *(buf+(size++)) = *(newstring+i);
             *ptr++;
           }

           if( strlen(newstring) < strlen(substring) )
            ptr += (strlen(substring) - strlen(newstring));
           else
            ptr -= (strlen(newstring) - strlen(substring));

          if(need_n) *(buf+(size++)) = '\n';
        }
      c = 0;
     }
     else
       *(buf+(size++)) = c;
  }
  return buf;
}

int Parsing()
{
  setstatus("Parsing\n");//!
//  logwrite("4:\\inv.txt",buf);
  char *p1 = "<p>" , *p2 = "</p>";
  char *s,*s1,*s2,*s3;
  int len;
  if((s1=strstr(buf,p1))>0)//первый <p>
  {
    s1+=strlen(p1);
    if((s2=strstr(s1,p1))>0)//второй <p> и дальше наша строка, пока не </p>
    {
      s3 = strstr(s2,p2);
      s2+=strlen(p1);
      
      len=0;
      len=s3-s2;
      s = (char*)malloc(len+1);
      strncpy(s,s2,len+1);
      s[len]='\0';
      TDate d;
      TTime t;
      GetDateTime(&d,&t);
      char *s2 = (char*)malloc(256);
      sprintf(s2,"original >> %02d.%02d %02d:%02d %s\n",d.day,d.month,t.hour,t.min,s);      
      CFile file("4:\\recv.txt");
      file.open(A_Create+A_ReadWrite+A_Append);
      file.write(s2,strlen(s2));
      logwrite("4:\\recv.txt",s2);
      WSHDR *ws=AllocWS(len);     
      char *ss = DelStr(s,"<br\\>",1);
     // ss = ChangeStr(ss,"OFFLINE","Не в сети",1);
      sprintf(s2,"delete <br> >> %02d.%02d %02d:%02d %s\n",d.day,d.month,t.hour,t.min,ss);
      file.write(s2,strlen(s2));      
      logwrite("4:\\recv.txt",s2);
      wsprintf(ws,"%t",ss);
      file.close();
      mfree(ss);
      mfree(s);
      mfree(s2);
      
      EDIT_SetTextToEditControl(ggui ,5 ,ws);
      FreeWS(ws);
      DirectRedrawGUI();
      setstatus("Already recived!\n");
      return 1;
    }
    else
    {
      WSHDR *ws=AllocWS(strlen(s1));
      wsprintf(ws,"%t",s1);
      EDIT_SetTextToEditControl(ggui ,5 ,ws);
      FreeWS(ws);
      setstatus("Parsing Error 2...try Again\n");
    }
    return 0;
  }
  setstatus("Parsing Error 1...try Again\n");
  return 0;
}


void OnRedraw(MAIN_GUI *data)
{       
  
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
  DirectRedrawGUI();
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
#ifndef NEWSGOLD
    case RED_BUTTON:       
      
      return(1);
#endif
    }
  }
  return(0);
}

extern "C"
{
  extern void kill_data(void *p, void (*func_p)(void *));
}

//extern void kill_data(void *p, void (*func_p)(void *));
void method7(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  kill_data(data, mfree_adr);
}

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
  (void *)method7,	
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,0,0};
void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui = (MAIN_GUI*)malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(RECT *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  csm->gui_id=CreateGUI(main_gui);   
  //setlang();
//  settr();
  //void LoadList();
  //LoadList();
//  list = (CList*)malloc(sizeof(CList));
 // list = new CList;
//  list = (CList*)malloc(sizeof(CList));
  list = new CList;
  void create_ed();
  SUBPROC((void *)create_ed);
}




void ElfKiller(void)
{  
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

void maincsm_onclose(CSM_RAM *csm)
{
  //void Save();
//  Save();
 // mfree(list);
  delete list;
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
        if (connect_state==1)
        {
          setstatus("Connected\n");
          //Если посылали запрос
          SUBPROC((void *)send_req);        
        }
        else
        {
          setstatus("Error\n");
          SUBPROC((void *)create_connect); 
        }
        break;
      case ENIP_SOCK_DATA_READ:
        if (connect_state==2)
        {
          char s[64];
          sprintf(s,"Reading %d bytes\n",traf);
          setstatus(s);
          //Если посылали send
          SUBPROC((void *)get_answer);
        }
        else
        {
          SUBPROC((void *)create_connect);
          setstatus("Error\n");
        }
        break;
      case ENIP_SOCK_REMOTE_CLOSED:
        //Закрыт со стороны сервера
        if (connect_state) SUBPROC((void *)end_socket);
        break;
      case ENIP_SOCK_CLOSED:
        //Закрыт вызовом closesocket
        if (connect_state) SUBPROC((void *)end_socket);
        setstatus("Receiving...\n");
        connect_state=3;
        Parsing();
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"Check_Inviz");
}

int main(char *f, char *text)
{  
  char dummy[sizeof(MAIN_CSM)];
  UpdateCSMname();
  LockSched();
  //int len = strrchr(f,'\\')-f+1;
//  strncpy(fname, f, len);
  //strcat(fname, "translate.dat");
//  loadsettings();
  MAINCSM_ID=CreateCSM(&MAINCSM.maincsm,dummy,0);
  UnlockSched(); 
  return 0;
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

void edchat_ghook(GUI *data, int cmd)
{
  static SOFTKEY_DESC skk={0x0018, 0x0000,(int)""};
  if (cmd==7)
    SetSoftKey(data,&skk,SET_SOFT_KEY_N); 
}

int itemnum, wd_id;

void setstatus(char *status)
{
  if(ggui)
  {
    WSHDR *ws=AllocWS(strlen(status));
    utf8_2ws(ws,status,strlen(status));
    EDIT_SetTextToEditControl(ggui, 1,ws);
    FreeWS(ws);
    DirectRedrawGUI();
  }
}


int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

char *LoadBuf(char *fname)
{
  int f;
  unsigned int err;
  char *buf = (char*)malloc(get_file_size(fname));
  if ((f=fopen(fname,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))!=-1)
    fread(f,buf,strlen(buf),&err);
  fclose(f,&err);
  return buf; 
}


void SaveFile(char *fname,char *buf)
{
  int f;
  unsigned int err;
  if ((f=fopen(fname,A_WriteOnly+A_TXT+A_Create+A_Append,P_WRITE,&err))!=-1)
    fwrite(f,buf,strlen(buf),&err);
  fclose(f,&err);
}
/*
char **list;
int items;
*/

//CList list;

void Save()
{/*
  char *p;
  int flag = 0;
 // remove("clist2.txt");
  unsigned int err;
  //unlink("4:\\invizlist.txt",&err);
  for(int i = 0; i < items && list[i];i++)
  {
    p = list[i];
    for(int j = i+1 ; j<items; j++)
    {
      if(strcmp(p,list[j]) == 0){ flag = 1; break; }
    }

    if(!flag)
    {
     SaveFile("4:\\invizlist.txt",list[i]);
     SaveFile("4:\\invizlist.txt","\n");
    //}
    //flag = 0;
   }*/
}
/*
void FreeList()
{
  if (list) mfree(list);
  list=NULL;
}*/

/*
void addToList(char *s)
{
  if(s)
  {
    char *p;
    int flag = 0;
    
    for(int i = 0 ; i < items && !flag ; i++)
    {
      p = list[i];
      for(int j = i+1 ; j < items ; j++)
      {
        if(strcmp(p,list[j]) == 0){ flag = 1; break; }
      }
    }
    
    if(!flag)
    {
      list = (char**)realloc(list,(items+1)*sizeof(char *));
      list[items++] = s;
      logwrite("4:\\text.txt",s);
//         SaveFile("4:\\invizlist.txt",list[i]);
  //       SaveFile("4:\\invizlist.txt","\n");
    } 
  }
}
*/


int LoadList()
{
  list->free();
  CFile file("4:\\invizlist.txt");
  file.open(A_ReadOnly);
  
 
  char *ptrBuf = file.ReadBuf(file.Size());
//  char *ptrBuf = LoadBuf("4:\\invizlist.txt");
  int items = 0;
    
  char *s = (char*)malloc(256);
  int i=0;
  char c;
  LIST_STRUCT *l = new LIST_STRUCT();
  char *str = ptrBuf;
  const char *tmp = str;
//  while(str)
  int lines = 0;
  {
    char *p = strtok(ptrBuf,"\n\r");
    
    while(p)
    {
      p = strtok(NULL,"\n\r");
      lines++;
    }
  }
  
  for(int j=0;j<1;j++)
  {
      *s = 0;
      i = 0;
      for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++,i++)
         *(s+i) = *str;
      for(;(*str==';' || *str==' ' || *str=='\x0D' || *str=='\x0A') && *str; str++);
      
      *(s+i++) = 0;
     logwrite("4:\\tex.txt",s); 
     str++;
  }
/*  for(char *p = ptrBuf; *p ;)
  {     
    for(i=0 ; (*p!='\n' || *p!='r') && i<10 ; i++,p++)
    {
      c = *p;
      *(s+i) = c;
    }
    *(s+i++) = 0;
    logwrite("4:\\text.txt",s,10);
//    l->name = s;
 //   l->next = 0;
 //   list->add(l);  
     p+=2;
  }*/
  /*
  char *p=strtok(ptrBuf,"\n\r");
  
//  while(p)
  {
    logwrite("4:\\t.txt",p);  
  }*/
  
  mfree(l);
  mfree(s);
  file.close();
  /*
  for(int i = 0; i < list->getCount(); i++)
    file2.write(list->getByItem(i)->name,strlen(list->getByItem(i)->name));
  file2.close();*/
  return 1;
}


void ed_options_handler(USR_MENU_ITEM *item)
{/*
  const char _d[]="%d";
  int i=item->cur_item;
  if (item->type==0)
  {
    ascii2ws(item->ws,"list[i]");
   // wsprintf(item->ws,"%s",list[i]);
  }
  
  if (item->type==1)
  {
    if (i < items)
    {
      WSHDR *ed_ws;
      char *p=list[i];
      ed_ws=AllocWS(strlen(p));
      wsprintf(ed_ws,_d,p);
      EDIT_SetTextToEditControl(ggui,3,ed_ws);
      FreeWS(ed_ws);
     }
   }    */
}


int edchat_onkey(GUI *data, GUI_MSG *msg)
{
  ggui=data;
  switch (msg->gbsmsg->submess)
  {
#ifndef NEWSGOLD
  case RED_BUTTON:
    CloseCSM(MAINCSM_ID);
    break;
#endif  
  case ENTER_BUTTON:
    {
      int i = 0;
        LoadList();
      EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,0,i);
    }
    return (-1);
    //break;
  case GREEN_BUTTON:
    {
      EDITCONTROL ec;
      ExtractEditControl(data,ECINPUT,&ec);
      ws_2str(ec.pWS,UIN,10); 

      WSHDR *ws = AllocWS(32);
      wsprintf(ws,"Waiting.....");
      EDIT_SetTextToEditControl(ggui ,5 ,ws);
      FreeWS(ws);
//      setstatus("\n");
      if(!(wstrlen(ec.pWS)))
      {
        setstatus("Error..Input uin!\n");
      }
      else
      {        
        //addToList(UIN);
        Save();
        SUBPROC((void *)create_connect);
        setstatus("Start connection...\n");
      }
    }
    break;
  }/*
  if (msg->keys==0x18)
  {
    void MMenu();
    MMenu();
  }*/
  if (msg->keys==0x1)
  {
    CloseCSM(MAINCSM_ID);
  }
  return(0); 
}

void edchat_locret(void){};

HEADER_DESC edchat_hdr={0,0,0,0,NULL,(int)"Check inviz",LGP_NULL};

static const SOFTKEY_DESC menu_sk[] =
{
  {0x0018, 0x0000, (int)""},
  {0x0001, 0x0000, (int)"Close"},
  {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt =
{
  menu_sk, 0
};

static const INPUTDIA_DESC edchat_desc =
{
  1,
  edchat_onkey,
  edchat_ghook,
  (void *)edchat_locret,
  0,
  &menu_skt,
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


void create_ed(void)
{
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  void *ma=malloc_adr();
  void *eq;
  WSHDR *ws;
  PrepareEditControl(&ec);
  eq=AllocEQueue(ma,mfree_adr());  
  ws=AllocWS(50);

  ConstructEditControl(&ec,1,0,0,50);
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,0);
    CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma); 
  
  utf8_2ws(ws, "UIN:",20);
  ConstructEditControl(&ec,1,0x40,ws,wslen(ws));
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,2);
    CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma); 
  
  ConstructEditControl(&ec,ECT_NORMAL_NUM,0x40,0,1024);
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,0);

    CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);  
  
  utf8_2ws(ws, "Resultat:",20);
  ConstructEditControl(&ec,1,0x40,ws,wslen(ws));
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,2);
    CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);   
  
  ConstructEditControl(&ec,3,0x40,0,1024);
    PrepareEditCOptions(&ec_options);
    SetFontToEditCOptions(&ec_options,1);//Шрифт
    SetPenColorToEditCOptions(&ec_options,0);
    CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma); 
  
  FreeWS(ws);
  patch_header(&edchat_hdr);
  patch_input(&edchat_desc);    
  wd_id=CreateInputTextDialog(&edchat_desc,&edchat_hdr,eq,1,0);
}

