#include "include.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"

#include "../inc/xtask_ipc.h"
#include "sieicq_ipc.h"
#include "revision.h"
#include "strings.h"
#include "conf_loader.h"

#include "main.h"

#include "icq.h"
#include "items.h"

#include "gui.h"
#include "gui_begin.h"

#include "iconpack.h"

#ifndef NEWSGOLD
#define SEND_TIMER
#endif

const char percent_t[]="%t";
const char percent_d[]="%d";
const char percent_s[]="%s";

int VisibleGUI =0;

#define TMR_SECOND 216

#define EOP -99
int RXstate=EOP; //-sizeof(RXpkt)..-1 - receive header, 0..RXpkt.data_len - receive data

static GBSTMR OnRedrawTimer; 

//IPC
const char ipc_my_name[32]=IPC_SIEICQ_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
IPC_REQ gipc;

extern void SMART_REDRAW(void);

char elf_path[256];
int maincsm_id;
int maingui_id;


//extern const unsigned int UIN;
//extern const char PASS[];

// Visual parametres
unsigned int ScrW, ScrH;
char COLOUR[4]={0,0,0,100};
char COLOUR_FRING[4]={255,255,255,100};

char CURSOR_COLOUR[4]={100,100,100,100};
char CURSOR_COLOUR_FRING[4]={100,100,100,100};

int FONT=11;
char logmsg[256];
int Font_H;

int TYPE_DRAW;
//==================
// HOST
GBSTMR reconnect_tmr;
extern const unsigned int RECONNECT_TIME;
extern const char SieICQ_HOST[];
extern const unsigned int SieICQ_PORT;
char hostname[128];
int host_counter = 0;

void Close_MAIN_CSM()
{
  CloseCSM(maincsm_id);
}

//---------------------------------------------------------------------------
const char *GetHost(int cnt, const char *str, char *buf)
{
  const char *tmp = str, *begin, *end;
  if(cnt)
  {
    for(;cnt;cnt--)
    {
      for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
      if(!*str) str = tmp;
      for(;(*str==';' || *str==' ' || *str=='\x0D' || *str=='\x0A') && *str; str++);
      if(!*str) str = tmp;
    }
  }
  tmp = buf;
  begin = str;
  for(;*str!=';' && *str!=':' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
  end = str;
  for(;begin<end; *buf = *begin, begin++, buf++);
  *buf = 0;
  return tmp;
}
//---------------------------------------------------------------------------
int atoi(char *attr)
{
  int ret=0;
  int neg=1;
  for (int k=0; ; k++)
  {
    if ( attr[k]>0x2F && attr[k]<0x3A) {ret=ret*10+attr[k]-0x30;} else { if ( attr[k]=='-') {neg=-1;} else {return(ret*neg);}}
  }
}
//---------------------------------------------------------------------------
int GetPort(int cnt, const char *str)
{
  const char *tmp = str;
  char numbuf[6], numcnt = 0;
  if(cnt)
  {
    for(;cnt;cnt--)
    {
      for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
      if(!*str) str = tmp;
      for(;(*str==';' || *str==' ' || *str=='\x0D' || *str=='\x0A') && *str; str++);
      if(!*str) str = tmp;
    }
  }
  for(;*str!=';' && *str!=':' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
  if(*str!=':') return SieICQ_PORT;
  str++;
  numbuf[5] = 0;
  for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str && numcnt<5; numbuf[numcnt] = *str, str++, numcnt++);
  numbuf[numcnt] = 0;
  return atoi(numbuf);

}
//---------------------------------------------------------------------------
int GetHostsCount(const char *str)
{
  char cnt = 1;
  for(;cnt;cnt++)
  {
    for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str; str++);
    if(!*str) return cnt;
    for(;(*str==';' || *str==' ' || *str=='\x0D' || *str=='\x0A') && *str; str++);
    if(!*str) return cnt;
  }
  return 0;

}

////////////////////////////
/// GPRS
volatile int sendq_l=0; //Длинна очереди для send
volatile void *sendq_p=NULL; //указатель очереди
TPKT RXbuf;
volatile int is_gprs_online=1;
int connect_state=0;
int sock=-1;
int DNR_ID=0;
int DNR_TRIES=3;
volatile unsigned long TOTALRECEIVED;
volatile unsigned long TOTALSENDED;
volatile unsigned long ALLTOTALRECEIVED;
volatile unsigned long ALLTOTALSENDED;




//===============================================================================================
/*
void SMART_REDRAW(void)
{
  int f;
  LockSched();
  f=IsGuiOnTop(maingui_id);
  UnlockSched();
  if (f) REDRAW();
}
*/
//---------------------------------------------------------------------------

void create_connect(void)
{
BeginStep(10);
NextStep("Connecting");
  
  char hostbuf[128];
  int hostport;
  int ***p_res=NULL;
  void do_reconnect(void);
  SOCK_ADDR sa;
  //Устанавливаем соединение
  connect_state = 0;
  int err;
  unsigned int ip;
  GBS_DelTimer(&reconnect_tmr);
  if (!IsGPRSEnabled())
  {
    is_gprs_online=0;
    strcpy(logmsg,"LG_GRWAITFORGPRS");
    SMART_REDRAW();
    return;
  }
  DNR_ID=0;
  *socklasterr()=0;

  if (auth_cookie) 
  {
    sprintf(hostbuf,HostBoss);
    hostport = PortBoss;
    NextStep("Connecting BOSS");
    sprintf(hostname, "BOSS: %s:%d", hostbuf, hostport);
    SMART_REDRAW();
  }
    else
  {
  if(host_counter > GetHostsCount(SieICQ_HOST)-1) host_counter = 0;
  GetHost(host_counter, SieICQ_HOST, hostbuf);
  hostport = GetPort(host_counter, SieICQ_HOST);
  host_counter++;

  sprintf(hostname, "%s:%d", hostbuf, hostport);
  }
  
  SMART_REDRAW();

  ip=str2ip(hostbuf);
  if (ip!=0xFFFFFFFF)
  {
    sa.ip=ip;
    strcpy(logmsg,"\nConnect by IP!");
    SMART_REDRAW();
    goto L_CONNECT;
  }
  strcpy(logmsg,"LG_GRSENDDNR");
  SMART_REDRAW();
  err=async_gethostbyname(hostbuf,&p_res,&DNR_ID); //03461351 3<70<19<81
  if (err)
  {
    if ((err==0xC9)||(err==0xD6))
    {
      if (DNR_ID)
      {
        host_counter--;
	return; //Ждем готовности DNR
      }
    }
    else
    {
      snprintf(logmsg,255,"LG_GRDNRERROR",err);
      SMART_REDRAW();
      GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
      return;
    }
  }
  if (p_res)
  {
    if (p_res[3])
    {
      strcpy(logmsg,"LG_GRDNROK");
      SMART_REDRAW();
      DNR_TRIES=0;
      sa.ip=p_res[3][0][0];
    L_CONNECT:
      sock=socket(1,1,0);
      if (sock!=-1)
      {
	sa.family=1;
	sa.port=htons(hostport);
	//    sa.ip=htonl(IP_ADDR(82,207,89,182));
	if (connect(sock,&sa,sizeof(sa))!=-1)
	{
	  connect_state=1;
	  TOTALRECEIVED=0;
	  TOTALSENDED=0;
	  SMART_REDRAW();
	}
	else
	{
	  closesocket(sock);
	  sock=-1;
	  LockSched();
	  //ShowMSG(1,(int)"LG_MSGCANTCONN");
	  UnlockSched();
	  GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
	}
      }
      else
      {
	LockSched();
	ShowMSG(1,(int)"LG_MSGCANTCRSC");
	UnlockSched();
	//Не осилили создания сокета, закрываем GPRS-сессию
	//GPRS_OnOff(0,1);
      }
    }
  }
  else
  {
    DNR_TRIES--;
    LockSched();
    ShowMSG(1,(int)"LG_MSGHOSTNFND");
    UnlockSched();
  }
    RXstate=-(int)sizeof(FLAP_HEAD);
}



void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
    connect_state = 0;
  }
#ifdef SEND_TIMER
  GBS_DelTimer(&send_tmr);
#endif
}




#ifdef SEND_TIMER
static void resend(void)
{
  void Send(char * data, int size);
  SUBPROC((void*)Send,NULL,NULL);
}
#endif

void ClearSendQ(void)
{
  mfree((void *)sendq_p);
  sendq_p=NULL;
  sendq_l=NULL;
#ifdef SEND_TIMER
  GBS_DelTimer(&send_tmr);
#endif
}

void Send(char * data, int size)
{
  int i;
  int j;
  if (connect_state<2) return;
  if (data)
  {
    //Проверяем, не надо ли добавить в очередь
    if (sendq_p)
    {
      //Есть очередь, добавляем в нее
      sendq_p=realloc((void *)sendq_p,sendq_l+size);
      memcpy((char *)sendq_p+sendq_l,data,size);
      sendq_l+=size;
      return;
    }
    sendq_l=size;
    sendq_p=malloc(size + 1);
    memcpy((char*)sendq_p, data, size);
  }
  
  //Отправляем уже существующее в очереди
  while((i=sendq_l)!=0)
  {
    if (i>0x400) i=0x400;
    j=send(sock,(void *)sendq_p,i,0);
    snprintf(logmsg,255,"send res %d",j);
    SMART_REDRAW();
    if (j<0)
    {
      j=*socklasterr();
      if ((j==0xC9)||(j==0xD6))
      {
	//Передали что хотели
	strcpy(logmsg,"Send delayed...");
	return; //Видимо, надо ждать сообщения ENIP_BUFFER_FREE
      }
      else
      {
	//Ошибка
	LockSched();
	ShowMSG(1,(int)"Send error!");
	UnlockSched();
	end_socket();
	return;
      }
    }
    memcpy((void *)sendq_p,(char *)sendq_p+j,sendq_l-=j); //Удалили переданное
    
    TOTALSENDED+=j;
    ALLTOTALSENDED+=j;			

    if (j<i)
    {
      //Передали меньше чем заказывали
#ifdef SEND_TIMER
      GBS_StartTimerProc(&send_tmr,216*5,resend);
#endif
      return; //Ждем сообщения ENIP_BUFFER_FREE1
    }
  }
  mfree((void *)sendq_p);
  sendq_p=NULL;
}


void get_answer(void)
{ void *p;
  int i=RXstate;
  int j;
  int n;
  char rb[1024];
  char *rp=rb;
  if (connect_state<1) return;
  if (i==EOP) return;
  j=recv(sock,rb,sizeof(rb),0);
  while(j>0)
  {
    if (i<0)
    {
      //Принимаем заголовок
      n=-i; //Требуемое количество байт
      if (j<n) n=j; //полученное<требуемое?
      memcpy(RXbuf.data+i,rp,n); //Копируем
      i+=n;
      j-=n;
      rp+=n;
      RXbuf.flap.data_size = htons(RXbuf.flap.data_size);
    }
    if (i>=0)
    {
      //Принимаем тельце ;)
      n=RXbuf.flap.data_size; //Всего в тельце
      if (n>16383)
      {
	//Слишком много
	strcpy(logmsg,"LG_GRBADPACKET");
	end_socket();
	RXstate=EOP;
	return;
      }
      n-=i; //Количество требуемых байт (общая длинна тельца-текущая позиция)
      if (n>0)
      {
	if (j<n) n=j; //полученное<требуемое?
	memcpy(RXbuf.data+i,rp,n);
	i+=n;
	j-=n;
	rp+=n;
      }
      if (RXbuf.flap.data_size==i)
      {
        _WriteLogICQ((char*)&RXbuf,RXbuf.flap.data_size+sizeof(FLAP_HEAD),1); 
	
        //Пакет полностью получен
	TOTALRECEIVED+=(i+8);
	ALLTOTALRECEIVED+=(i+8);			//by BoBa 10.07
	//Пакет удачно принят, можно разбирать...
	RXbuf.data[i]=0; //Конец строки
        
           int pl=RXbuf.flap.data_size+sizeof(FLAP_HEAD)+1;
	   p=malloc(pl);
           memcpy(p,&RXbuf,pl);
           GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	i=-(int)sizeof(FLAP_HEAD); //А может еще есть данные
      }
    }
  }
  RXstate=i;
  //  GBS_StartTimerProc(&tmr_dorecv,3000,dorecv);
  //  SMART_REDRAW();
}

void do_reconnect(void)
{
  if (is_gprs_online)
  {
    DNR_TRIES=3;
    SUBPROC((void*)create_connect);
  }
}


//---------------------------------------------------------------------------

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

//===============================================================================================
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

//отрисовка
void method0(MAIN_GUI *data)
{
  //LockSched();
  OnRedraw();
  //UnlockSched(); 
}

void method1(MAIN_GUI *data,void *(*malloc_adr)(int))
{
  data->ws1=AllocWS(256);
  data->ws2=AllocWS(256);
  data->gui.state=1;
}

void method2(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  FreeWS(data->ws2);
  data->gui.state=0;
}

// focus
void method3(MAIN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
  DisableIDLETMR();
  data->gui.state=2;
}

//unfocus
void method4(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  if (data->gui.state!=2)
    return;
  data->gui.state=1;
}

void Create_Connect()
{
  if ((connect_state==0)&&(sock==-1))
  {
   
    GBS_DelTimer(&reconnect_tmr);
    DNR_TRIES=3;
    SUBPROC((void *)create_connect);
  }
}

// onkey
int method5(MAIN_GUI *data,GUI_MSG *msg)
{
  OnKey(msg->gbsmsg->msg, msg->gbsmsg->submess);
  DirectRedrawGUI();
  return(0);
}

int method8(void){return(0);}

int method9(void){return(0);}

const void * const gui_methods[11]={
  (void *)method0,  //Redraw
  (void *)method1,  //Create
  (void *)method2,  //Close
  (void *)method3,  //Focus
  (void *)method4,  //Unfocus
  (void *)method5,  //OnKey
  0,
  (void *)kill_data, //method7, //Destroy
  (void *)method8,
  (void *)method9,
  0
};

const RECT Canvas={0,0,0,0};

void maincsm_oncreate(CSM_RAM *data)
{
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  MAIN_CSM*csm=(MAIN_CSM*)data;
  zeromem(main_gui,sizeof(MAIN_GUI));
  patch_rect((RECT*)&Canvas,0,0,ScreenW()-1,ScreenH()-1);
  main_gui->gui.canvas=(void *)(&Canvas);
//  main_gui->gui.flag30=2;
  main_gui->gui.methods=(void *)gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  csm->csm.state=0;
  csm->csm.unk1=0;
  maingui_id=csm->gui_id=CreateGUI(main_gui);
  
  
//  ews=AllocWS(16384);
  DNR_TRIES=3;
//  GBS_StartTimerProc(&tmr_active,TMR_SECOND*10,process_active_timer);

  sprintf((char *)ipc_my_name+6,percent_d,UIN);
  
  CreateICQ();
  
//// для проверки запущенной копии
  
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
  
  
  char *path=malloc(256);
  sprintf(path,"%s\\%d.cl",TEMPLATES_PATH,UIN);
  LoadCL(path);
  mfree(path);
  // делаем видимыми группы, тк при загрузке у всех итемов visible=0
  GroupVisible(1);
  
  LoadIcon();
  
  DirectRedrawGUI();

}

void maincsm_onclose(CSM_RAM *csm)
{
  
  extern GBSTMR tmr_active;
  GBS_DelTimer(&tmr_active);

  //  GBS_DelTimer(&tmr_dorecv);
//  GBS_DelTimer(&tmr_ping);
//  FreeSmiles();

//  Disconnect();
  char *path=malloc(256);
  sprintf(path,"%s\\%d.cl",TEMPLATES_PATH,UIN);
  SaveCL(path);
  mfree(path);
  
  FreeIcon();

  FreeItemsList();
  SUBPROC((void *)end_socket);
  SUBPROC((void *)ClearSendQ);
  SUBPROC((void *)ElfKiller);
  
  GBS_DelTimer(&OnRedrawTimer);
}



void CheckDoubleRun(void)
{
  int csm_id;
  if ((csm_id=(int)(gipc.data))!=-1)
  {
    gipc.name_to=ipc_xtask_name;
    gipc.name_from=ipc_my_name;
    gipc.data=(void *)csm_id;
    GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_SHOW_CSM,&gipc);
    LockSched();
    CloseCSM(maincsm_id);
    //ShowMSG(1,(int)LG_ALREADY_STARTED);
    UnlockSched();
  }
  else
  {
    //InitXStatusesImg();
    //InitSmiles(); Это вызовется из InitXStatusesImg
   
    Create_Connect();
  }
}

void OpenSettings(void)
{
  WSHDR * ws = AllocWS(150);
  str_2ws(ws, successed_config_filename, 128);
  ExecuteFile(ws, 0, 0);
  FreeWS(ws);
}

int maincsm_onmessage(CSM_RAM *data,GBS_MSG *msg)
{
  extern const int VIBR_ON_CONNECT;

  //  char ss[100];
  MAIN_CSM *csm=(MAIN_CSM*)data;
  {
    //IPC
    if (msg->msg==MSG_IPC)
    {
      IPC_REQ *ipc;
      if ((ipc=(IPC_REQ*)msg->data0))
      {
	if (strcmp_nocase(ipc->name_to,ipc_my_name)==0)
	{
	  switch (msg->submess)
	  {
	  case IPC_CHECK_DOUBLERUN:
	    //Если приняли свое собственное сообщение, значит запускаем чекер
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)CheckDoubleRun);
            else ipc->data=(void *)maincsm_id;
	    break;
     	  case IPC_SENDMSG: ;                                   //IPC_SENDMSG by BoBa 26.06.07
/*
          int l=strlen(((IPCMsg *)(ipc->data))->msg);
            TPKT *msg=malloc(sizeof(PKT)+l);
            msg->pkt.uin=((IPCMsg *)(ipc->data))->uin;
            msg->pkt.type=T_SENDMSG;
            msg->pkt.data_len=l;
            memcpy(msg->data,((IPCMsg *)(ipc->data))->msg,l);
            //slientsend=1;
	    SENDMSGCOUNT++; //Номер сообщения
            SUBPROC((void *)SendAnswer,0,msg);
          */
            break;
	  }
	}
      }
    }
    //Нарисуем иконочку моего статуса
#define idlegui_id (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
    CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
    if (IsGuiOnTop(idlegui_id)/*&&IsUnlocked()*/) //Если IdleGui на самом верху
    {
      GUI *igui=GetTopGUI();
      if (igui) //И он существует
      {
	void *canvasdata=BuildCanvas();
	//int icn;
	
	//Тут трохи поменял
	// by Rainmaker: Рисуем канву только для иконки и выводим в своих координатах
	//DrawCanvas(canvasdata,IDLEICON_X,IDLEICON_Y,IDLEICON_X+GetImgWidth((int)S_ICONS[icn])-1,
	//	   IDLEICON_Y+GetImgHeight((int)S_ICONS[icn])-1,1);
	//DrawImg(IDLEICON_X,IDLEICON_Y,S_ICONS[icn]);
      }
    }
  }
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"SieICQ config updated!");
      InitConfig();
      InitData();
    }
  }
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
  }
  if (msg->msg==MSG_HELPER_TRANSLATOR)
  {
    switch((int)msg->data0)
    {
    case LMAN_DISCONNECT_IND:
      is_gprs_online=0;
      return(1);
    case LMAN_CONNECT_CNF:
      is_gprs_online=1;
      strcpy(logmsg,"LG_GRGPRSUP");
      GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
      return(1);
    case ENIP_DNR_HOST_BY_NAME:
      if ((int)msg->data1==DNR_ID)
      {
	if (DNR_TRIES) SUBPROC((void *)create_connect);
      }
      return(1);
    }
    if ((int)msg->data1==sock)
    {
      //Если наш сокет
      if ((((unsigned int)msg->data0)>>28)==0xA)
      {
	//Принят пакет
        TPKT *buf =(TPKT *)msg->data0; 
	switch(buf->flap.channel) {
                case 0x01:
                      if (auth_cookie) 
                      {
                        NextStep("Sending cookie");
                        connect_state=3;
                        send_cookie();
                      } 
                      else 
                      {
                        NextStep("Send login/pass");
                        connect_state=2;
                        send_login();
                      }
                      
                      break;
                      
                case 0x02:
                       parse_snac(buf->data,buf->flap.data_size);
                      break;
                      
                case 0x04:
                      if (!auth_cookie)
                      {
                       parse_auth(buf->data,buf->flap.data_size);
                       end_socket();
                       create_connect();
                      }
                      break;
                      
                default:
                  ;
              };
        mfree (buf);
	return(0);
      }
      
      switch((int)msg->data0)
      {
      case ENIP_SOCK_CONNECTED:
	if ((connect_state==1)||(connect_state==3))
	{
	  //Соединение установленно
	  strcpy(logmsg, "Connected");
          SMART_REDRAW();
	}
	else
	{
	  ShowMSG(1,(int)"LG_MSGILLEGMSGCON");
	}
	break;
      case ENIP_SOCK_DATA_READ:
	SUBPROC((void *)get_answer);
	SMART_REDRAW();
	break;
      case ENIP_BUFFER_FREE:
      case ENIP_BUFFER_FREE1:
        {
	SUBPROC((void *)Send,NULL,NULL);
        }
	break;
      case ENIP_SOCK_REMOTE_CLOSED:
	//Закрыт со стороны сервера
	if (connect_state)
	  SUBPROC((void *)end_socket);
        strcpy(logmsg, "REMOTE_CLOSED");
        SUBPROC((void *)ClearSendQ);
	break;
      case ENIP_SOCK_CLOSED:
	connect_state=0;
	sock=-1;
	if (sendq_p)
	{
	  snprintf(logmsg,255,"Disconnected, %d bytes not sended!",sendq_l);
	}
        
	SMART_REDRAW();
        SUBPROC((void *)ClearSendQ);
        GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
        snprintf(logmsg,255,"%s\nReconect after %d second...",logmsg, RECONNECT_TIME);
	break;
        
      }
    }
  }
  return(1);
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
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "SieICQ: %d",UIN);
}

void StartTimerSendGuiRedraw()
{
  SMART_REDRAW();
  //DirectRedrawGUI();
  GBS_StartTimerProc(&OnRedrawTimer,216/4,StartTimerSendGuiRedraw);
}


int main(char *filename)
{
  MAIN_CSM main_csm;
  char *s;
  int len;
  extern const char *successed_config_filename;
  WSHDR *ws;

  //Visual parametres
  ScrW = ScreenW();
  ScrH = ScreenH();
  Font_H = GetFontYSIZE(FONT);
  //StartTimerSendGuiRedraw();
  TYPE_DRAW = Draw_Begin;
  InitData();
    
  InitConfig();
  s=strrchr(filename,'\\');
  len=(s-filename)+1;
  strncpy(elf_path,filename,len);
  elf_path[len]=0;
  
  UIN=_UIN;
  strcpy(PASS,_PASS);

  if (!UIN)
  {
    LockSched();
    ShowMSG(1,(int)"UIN PASS ??");
    ws=AllocWS(150);
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws,0,0);
    UnlockSched();
    SUBPROC((void *)ElfKiller);
    return 0;
  }
  
  
  UpdateCSMname();
  LockSched();
  maincsm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
  
  return 0;
}






