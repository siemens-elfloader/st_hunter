#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "configs.h"
#include "NatICQ.h"
#include "history.h"
#include "conf_loader.h"
#include "mainmenu.h"
#include "main.h"
#include "language.h"
#include "../inc/xtask_ipc.h"
#include "smiles.h"
#include "naticq_ipc.h"
#include "status_change.h"
#include "strings.h"
#include "manage_cl.h"
#include "cl_work.h"
#include "select_smile.h"
#include "revision.h"
#include "print.h"
#ifdef NEWSGOLD
#include "screen_save.h"
#include "player_tags.h"
#endif
#include "swilib2.h"
#include "rect_patcher.h"
#include "addon.h"
#include "auth.h"

extern int *XStatusesIconArray;

#ifndef NEWSGOLD
#define SEND_TIMER
#endif

#define IN_TICKS(sec) (sec * TMR_SECOND)

//#define LOG

#ifdef LOG
const char _rn[]="\r\n";
static void logwrite(char *text)
{
  int hFile;
  unsigned int io_error = 0;
  hFile = fopen("4:\\natlog.txt",A_ReadWrite + A_Create + A_Append,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
  {
    fwrite(hFile, text, strlen(text), &io_error);
    fwrite(hFile, _rn, 2, &io_error);
    fclose(hFile, &io_error);
  }
  else
    ShowMSG(1,(int)"Error!");
//  mfree(text);
}
#endif

//------------- ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ -------------//

int main_d;
int align=0;
//int space=1;
void *canvasdata;
int paused_height;
WSHDR *wsBuffer;//мультибуффер

char talk_sound[128];
int uins;
//char m_1[]="unofficial! private use only";
//char m_2[]="mod by (c)Evilfox 315384924";
char unread_msg_info[64];

int saver_on;

#ifdef NEWSGOLD
  extern char teast2[256];
#endif

//#ifdef ELKA
  char last_msg_buffer[4096];
//#endif

  
//-------------------------------------------------//

char prmsg[256];//вывод попап в контакт листе
int PrintTimer=0;//время попап
int curping=0; //тукущий пинг

volatile int sendq_l=0; //Длинна очереди для send
volatile void *sendq_p=NULL; //указатель очереди

extern const char *suc_config_name_mod;


int main_count;
int main2_count;

int DoRefreshCL=0;//Если нада обновить КЛ
void RefreshCL()
{
  DoRefreshCL=1; 
}

int ifJustNowRefresh=0;//Надо обновить поключившихся
void RefreshJustNow()
{
  ifJustNowRefresh=1;
}
//-------------------- AUTOPING -------------------//
  
GBSTMR ping_tmr;
static void PingToServer2(void)
{
  TPKT *p;
  p=malloc(sizeof(PKT)+sizeof(TTime));
  GetDateTime(NULL,(TTime *)p->data);
  p->pkt.uin=0;
  p->pkt.type=T_ECHO;
  p->pkt.data_len=sizeof(TTime);
  SUBPROC((void *)SendAnswer,0,p);
}

int Is_PING_On;

void Pinger2()
{
  if(Is_PING_On)
  {
    if(sendq_l<32)//если буфер для отправки меньше 32 байт то пингуем,чтобы не забивать очередь, тест...
      PingToServer2();
    GBS_StartTimerProc(&ping_tmr, 60*216, Pinger2);
  }
  else
    GBS_StartTimerProc(&ping_tmr, 216*10, Pinger2);
}

//-------------------------------------------------//



extern volatile int total_smiles;
extern volatile int total_xstatuses;
extern volatile int xstatuses_load;
extern volatile int pictures_max; //2026rev
extern volatile int pictures_loaded; //2026rev

#ifdef ELKA
  int Is_SLI_On;//мигаем светодиодом
#endif

//int Is_SMARTCPU_On;//Вывод сообщений на гэ
int Is_INC_On;//Popup message
int Is_ILU_On;//Режим подсветки
int Is_Double_Vibra_On;//Режим напоминания
int total_unread;
int total_unread2;

#ifdef ELKA
  const int SLI_TYPE=1;
#endif

char PINGO[256];

#ifdef ELKA
  extern const unsigned int SLI_time;
  extern const unsigned int SLI_time2;
#endif
//#define USE_MLMENU
#define USE_GUI //графический интерфейс

#define TMR_SECOND 216

//IPC
const char ipc_my_name[32]=IPC_NATICQ_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
const char ipc_test_name[]="test";
IPC_REQ gipc;


char elf_path[256];
int maincsm_id;
int maingui_id;
volatile int contactlist_menu_id;
void SMART_REDRAW(void)
{
#ifdef DEBUG
  int f;
  int t;
  LockSched();
  f=IsGuiOnTop(maingui_id);
  t=IsGuiOnTop(contactlist_menu_id);
  UnlockSched();
  if (f || t) REDRAW();
#else
  int f;
  LockSched();
  f=IsGuiOnTop(maingui_id);
  UnlockSched();
  if (f) REDRAW();
#endif
  
}

//По 10 секунд
#define ACTIVE_TIME 360

//Максимальное количество сообщений в логе
//#define MAXLOGMSG (20)

// Строковые описания статусов

const char S_OFFLINE[]="Offline";
const char S_INVISIBLE[]="Invisible";
const char S_AWAY[]="Away";
const char S_NA[]="N/A";
const char S_OCCUPIED[]="Occupied";
const char S_DND[]="DND";
const char S_ONLINE[]="Online";
const char S_FFC[]="FFC";

volatile int SENDMSGCOUNT;
#ifdef kluchnik1
  int IsActiveUp=1;
#else
  int IsActiveUp=0;
#endif
int Is_Vibra_Enabled;
int Is_Id3tag_Enabled;
unsigned int Is_Sounds_Enabled;
int Is_Show_Offline;
int Is_Show_Groups;

int S_ICONS[TOTAL_ICONS+1];

#define EOP -10
int CurrentStatus;
int CurrentXStatus;

WSHDR *ews;

static const char * const icons_names[TOTAL_ICONS]=
{
  "offline.png",
  "invisible.png",
  "away.png",
  "na.png",
  "occupied.png",
  "dnd.png",
  "depression.png",
  "evil.png",
  "home.png",
  "lunch.png",
  "work.png",
  "online.png",
  "ffc.png",
  "message.png",
  "unknown.png",
  "groupon.png",
  "groupoff.png",
  "null.png",
  "addcont.png",
  "head.png",
  "logo.png",
  "ping.png",
  "settings.png",
  "vis1.png",
  "vis2.png",
  "vis3.png",
  "vis4.png",
  "vis5.png",
  "ping1.png",
  "ping2.png",
  "ping3.png",
  "ping4.png",
  "bg.png"
};



  
//------------------------ SCREEN SAVER TIMER ---------------------------//
/*
#ifdef NEWSGOLD
GBSTMR scrsTmr;
int scrsTime;
void ScreenSaverTimer()
{
 if(scrsTime==screenSaverTime)
 {
   scrsTime=screenSaverTime;//на всякий случай
   CloseScreensaver();
   DrawScreenSaver();  
   GBS_StartTimerProc(&scrsTmr, 216*10, ScreenSaverTimer);
 }
 if(isScreenSaverEna)
       scrsTime++;
  GBS_StartTimerProc(&scrsTmr, 216*5, ScreenSaverTimer);
}
#endif
*/

//-----------------------------------------------------------------------//
  
  
int msg_away_timer=0;//Счетчик для автостатуса
char away_msg[264];//Само сообщение для ответа
char away_msg_time[64];//Время когда сработал таймер
char away_msg_time_dace[16];//время отсутвтвия для вывода на идле
//char *inp_away_msg2;//Текст сообщения,если будем писать сами(на долгую 6 или VOL_DOWN_BUTTON),а не брать из конфига
char tim_zone[16];
int timeh;
int timemin;
int away_m;//состояние автоответчика
int status_rem;//Для сохранения текущего статуса
int change_back_status;//вернуть предыдущий статус или нет
GBSTMR away_tmr;


int Show_XTX;//Показывать хстатус или время он лайн
void FreeXText2(CLIST *t)
{
  if (t->xtext2) { mfree(t->xtext2); t->xtext2=NULL; }
}

void away_timer()//Таймер автостатуса
{
  TTime tt;
  if(msg_away_timer==(AUTO_ENGADE*12))
  {
    GetDateTime(NULL,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "( Отсутствую примерно с %02d:%02d [%s] )", tt.hour, tt.min, tim_zone);
    away_m=1;
    
    if(CurrentStatus!=2 && CurrentStatus!=isAutoStatusOn)
    {
      status_rem=CurrentStatus;
      CurrentStatus=2;
      set_my_status();
      change_back_status=1;
    }
  }
  
  if( (IsOnNAStatus) && (msg_away_timer==((AUTO_ENGADE*12)+(NAStatusTime*12))) )//Включить переключение на недоступен,в минутах
  {
    GetDateTime(NULL,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "( Недоступен примерно с %02d:%02d [%s] )", tt.hour, tt.min, tim_zone);
    if(CurrentStatus!=3)
    {
      CurrentStatus=3;
      set_my_status();
    }
  }
  
  if(Auto_Status)
     msg_away_timer++;
  GBS_StartTimerProc(&away_tmr, 216*5, away_timer);
}


void init_time_zone()
{
  switch(time_zone)
  {
  case 0:
    sprintf(tim_zone, "GMT -12:00");
    break;
  case 1:
    sprintf(tim_zone, "GMT -11:00");
    break;
  case 2:
    sprintf(tim_zone, "GMT -10:00");
    break;
  case 3:
    sprintf(tim_zone, "GMT -09:00");
    break;
  case 4:
    sprintf(tim_zone, "GMT -08:00");
    break;
  case 5:
    sprintf(tim_zone, "GMT -07:00");
    break;
  case 6:
    sprintf(tim_zone, "GMT -06:00");
    break;
  case 7:
    sprintf(tim_zone, "GMT -05:00");
    break;
  case 8:
    sprintf(tim_zone, "GMT -04:00");
    break;
  case 9:
    sprintf(tim_zone, "GMT -03:30");
    break;
  case 10:
    sprintf(tim_zone, "GMT -03:00");
    break;
  case 11:
    sprintf(tim_zone, "GMT -02:00");
    break;
  case 12:
    sprintf(tim_zone, "GMT -01:00");
    break;
  case 13:
    sprintf(tim_zone, "GMT 00:00");
    break;
  case 14:
    sprintf(tim_zone, "GMT +01:00");
    break;
  case 15:
    sprintf(tim_zone, "GMT +02:00");
    break;
  case 16:
    sprintf(tim_zone, "GMT +03:00");
    break;
  case 17:
    sprintf(tim_zone, "GMT +03:30");
    break;
  case 18:
    sprintf(tim_zone, "GMT +04:00");
    break;
  case 19:
    sprintf(tim_zone, "GMT +04:30");
    break;
  case 20:
    sprintf(tim_zone, "GMT +05:00");
    break;
  case 21:
    sprintf(tim_zone, "GMT +05:30");
    break;
  case 22:
    sprintf(tim_zone, "GMT +05:45");
    break;
  case 23:
    sprintf(tim_zone, "GMT +06:00");
    break;
  case 24:
    sprintf(tim_zone, "GMT +06:30");
    break;
  case 25:
    sprintf(tim_zone, "GMT +07:00");
    break;
  case 26:
    sprintf(tim_zone, "GMT +08:00");
    break;
  case 27:
    sprintf(tim_zone, "GMT +09:00");
    break;
  case 28:
    sprintf(tim_zone, "GMT +09:30");
    break;
  case 29:
    sprintf(tim_zone, "GMT +10:00");
    break;
  case 30:
    sprintf(tim_zone, "GMT +11:00");
    break;
  case 31:
    sprintf(tim_zone, "GMT +12:00");
    break;
  case 32:
    sprintf(tim_zone, "GMT +13:00");
    break;
  }
}


//------------------------ AUTO CHANGE STATUS ------------------------------//

void SetIconBarHandler();
GBSTMR autoChangeStatus;
#pragma inline 
static unsigned int GetStatusByIndex(int pos)
{
  switch(pos)
  {
  case 1:
    return IS_ONLINE;
  case 2:
    return IS_AWAY;
  case 3:
    return IS_NA;
  case 4:
    return IS_DND;
  case 5:
    return IS_OCCUPIED;
  case 6:
    return IS_FFC;
  case 7:
    return IS_INVISIBLE;
  case 8:
    return IS_DEPRESSION;
  case 9:
    return IS_EVIL;
  case 10:
    return IS_HOME;
  case 11:
    return IS_LUNCH;
  case 12:
    return IS_WORK;
  }
  return IS_UNKNOWN;
}

#ifndef DEBUG
void AutoChangeStatus_timer()//Таймер автосмены статуса
{
  TTime tt;
  GetDateTime(NULL,&tt);
    
  if(status_1!=0 && tt.hour==tm_status_1.hour && tt.min==tm_status_1.min)
  {
      CurrentStatus=GetStatusByIndex(status_1);
      set_my_status();
  }
  
  if(status_2!=0 && tt.hour==tm_status_2.hour && tt.min==tm_status_2.min)
  {
      CurrentStatus=GetStatusByIndex(status_2);
      set_my_status();
  }
  
  if(status_3!=0 && tt.hour==tm_status_3.hour && tt.min==tm_status_3.min)
  {
      CurrentStatus=GetStatusByIndex(status_3);
      set_my_status();
  }
  
  if(status_4!=0 && tt.hour==tm_status_4.hour && tt.min==tm_status_4.min)
  {
      CurrentStatus=GetStatusByIndex(status_4);
      set_my_status();
  }
  
  if(status_5!=0 && tt.hour==tm_status_5.hour && tt.min==tm_status_5.min)
  {
      CurrentStatus=GetStatusByIndex(status_5);
      set_my_status();
  }

  GBS_StartTimerProc(&autoChangeStatus, 216*30, AutoChangeStatus_timer);
}
#endif
//=================================away_msg===============
//////////////////////////////////////////tozhe dlja auto ping///////////////

void DrawPing(void *canv)
{
  PrintField(Ping_X,Ping_Y, PINGO, align, fonto, spaco1);
}

//////////////////////////////////////////tozhe dlja auto ping///////////////

GBSTMR remind_tmr;
volatile int vibra_count;
#ifdef ELKA
  void SLI_Off(void);
#endif
void start_vibra2(void);

//////////////////////////reminder timer/////////////////////////

void Remind_Timer2()
{
  if (total_unread)
  {
    vibra_count=Vibra_cc;
    start_vibra2();
    GBS_StartTimerProc(&remind_tmr, 216*30, Remind_Timer2);
  }
}

void Reminder_timer()
{
  GBS_StartTimerProc(&remind_tmr, 216*30, Remind_Timer2);
}
//////////////////////////reminder timer/////////////////////////

//////////////////////////uptime//////////////////////////
//int Is_UPTIME_On;
int uptimeHour;
int uptimeMin;
int uptimeSec;

void UptimeMinHour()
{
  uptimeSec++;
  if(uptimeSec==60)
  {
    uptimeSec=0;
    uptimeMin++;
  }
  if(uptimeMin==60)
  {
    uptimeMin=0;
    uptimeHour++;
  }
}

GBSTMR uptime_tmr;
#ifdef NEWSGOLD
  GBSTMR tracktmr;
  int tracktimer;
#endif

void upTime()
{
  if(connect_state==3)
  {
   UptimeMinHour();
#ifdef NEWSGOLD
  if (tracktimer>=10)/*&&(_GetPlayStatus()!=1))*/
    {
      GBS_StartTimerProc(&tracktmr, 216*5, GetTags);
      tracktimer=0;
    }
    tracktimer++;
#endif
  GBS_StartTimerProc(&uptime_tmr, 216, upTime);
  }
  else
  {
    if(!Pause_Time)
    {
     uptimeHour=0;
     uptimeMin=0;
     uptimeSec=0;
    }
    GBS_StartTimerProc(&uptime_tmr, 216, upTime);
  }
}

char onlineTime[128];
void DrawOnlineTime(void *canv)
{  
  if(connect_state==3)
  {   
   sprintf(onlineTime,"Online: %02d:%02d:%02d", uptimeHour, uptimeMin, uptimeSec);
  }
  else
    sprintf(onlineTime,"Offline");
  PrintField(onlineTime_x,onlineTime_y, onlineTime, align, fonto, spaco1);
}


//----------------------  Вывод времени на идле  ---------------------//


void DrawAutoMSG(void *canv)
{  
  char awayTime[64];
  if(away_m && showAwayIdle)
  {
    sprintf(awayTime,"Away from [%s]",away_msg_time_dace);
    PrintField(auto_x,auto_y, awayTime, align, fonto, spaco1);
  }  
}


//----------------------------  Input away msg  ---------------------------//

//Вводим сообщение какое хотим,а не то что прописали в конфиге
int input_awaymsg_id;

const HEADER_DESC input_awaymsg_hdr={0,0,131,21,NULL,(int)"Ввод автосообщения",LGP_NULL};
//void UpdateCLheader(void);
int input_awaymsg_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws;
  if (msg->keys==0x0FFF || msg->keys==VOL_DOWN_BUTTON)
  {
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);	
    ws=ec.pWS;
    zeromem(awayMsg,256);
    int i=0;
    for(i=0;i<ws->wsbody[0];i++)
      awayMsg[i]=char16to8(ws->wsbody[i+1]);
      awayMsg[i]=0;

#ifdef kluchnik2
    char *s;
    ExtractEditControl(data,4,&ec);	
    ws=ec.pWS;
    s=malloc(ws->wsbody[0]);
    *s=0;
    for(i=0;i<ws->wsbody[0];i++)
      *(s+i)=char16to8(ws->wsbody[i+1]);
      *(s+i)=0;
    strcat(awayMsg,s);
    mfree(s);
#endif    
    msg_away_timer=(AUTO_ENGADE*12);
    away_m=1;
    away_timer();
//    UpdateCLheader();
    GeneralFunc_flag1(input_awaymsg_id,1);
//    GeneralFuncF1(1);
  }
  
  if(msg->keys==LEFT_SOFT || msg->keys==RED_BUTTON)
  {
     msg_away_timer=0;
     away_m=0;
     if(change_back_status)
      {
        CurrentStatus=status_rem;
        set_my_status();
        change_back_status=0;
      }
//     UpdateCLheader();
  }
return(0);
}

void input_awaymsg_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"Сохранить"};
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);
  }
  if(cmd==0xA)
  {
    DisableIDLETMR(); // Отключаем таймер выхода по таймауту
  }
  if (cmd==0x0C)
  {
    EDIT_SetCursorPos(gui,1);
  }
  if(cmd==0x03){}
}

void input_awaymsg_locret(void){}

SOFTKEY_DESC input_awaymsg_sk[]=
{
  {0x0018,0x0000,(int)"Лев"},
  {0x0001,0x0000,(int)"Прав"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

SOFTKEYSTAB input_awaymsg_skt=
{
  input_awaymsg_sk,0
};

const INPUTDIA_DESC input_awaymsg_desc=
{
  1,
  input_awaymsg_onkey,
  input_awaymsg_ghook,
  (void *)input_awaymsg_locret,
  0,
  &input_awaymsg_skt,
  {0,NULL,NULL,NULL},
  FONT_SMALL,
  100,
  101,
  0,
  0x00000000,
  0x40000000
};

void InputAwayMsg()
{
   WSHDR *wss=AllocWS(256);
   void *ma=malloc_adr();
   void *eq=AllocEQueue(ma, mfree_adr());
   EDITCONTROL ec;
   PrepareEditControl(&ec);
   wsprintf(wss,"%t","Сообщение:\n");
   ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,wss,32);
   AddEditControlToEditQend(eq,&ec,ma);  

   PrepareEditControl(&ec);
   CutWSTR(wss,0);
   ascii2ws(wss,awayMsg);
   ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,wss,255);
   AddEditControlToEditQend(eq,&ec,ma);
#ifdef kluchnik2
   PrepareEditControl(&ec);
   wsprintf(wss,"%t","Сообщение 2:\n");
   ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,wss,32);
   AddEditControlToEditQend(eq,&ec,ma);  

   ascii2ws(wss,"когда смогу отвечу:)");
   ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,wss,255);
   AddEditControlToEditQend(eq,&ec,ma);
#endif
   patch_header(&input_awaymsg_hdr);
   patch_input(&input_awaymsg_desc);      
 
   input_awaymsg_id=CreateInputTextDialog(&input_awaymsg_desc,&input_awaymsg_hdr,eq,1,0);//создаем диалоговое окно
}


//////////////////////////uptime//////////////////////////
#ifdef ELKA
GBSTMR sli_tmr;
void SLI_On()
{
  if(Is_SLI_On)
    SLI_SetState(1);   
  GBS_StartTimerProc(&sli_tmr, IN_TICKS(2) / 20, SLI_Off);
}

void SLI_Off()
{
  SLI_SetState(2);
  if(total_unread)
     GBS_StartTimerProc(&sli_tmr, IN_TICKS(2) / 20, SLI_On); //26*SLI_time2
  else
     SLI_SetState(0);
}

void SLI_check()
{
  if (total_unread)
    SLI_On();
}

void SLI_Timer()
{
  GBS_StartTimerProc(&sli_tmr, 262, SLI_check); //262*SLI_time
}
#endif

void setup_ICONS(void)
{
  int i=0;
  do
  {
    if (!S_ICONS[i]) S_ICONS[i]=(int)MakeGlobalString(ICON_PATH,'\\',icons_names[i]);
    i++;
  }
  while(i<TOTAL_ICONS);
  return;
}

IMGHDR *icons[TOTAL_ICONS];
IMGHDR *XStatusImg[35];

int get_file_size(char* fname)
{
  unsigned int err;
  FSTATS fs;
  if ((GetFileStats(fname,&fs,&err))==-1) return 0;
  return (fs.size);
}

void XStatusToIMGHDR()
{
  int i=0;
  char *s=malloc(256);
  do
  {
    sprintf(s,"%s\\%d.png",XSTATUSES_PATH,i);
    if(get_file_size(s)!=0)
      XStatusImg[i]=CreateIMGHDRFromPngFile(s,2);   	

    i++;
  }
  while(i<35);
}


void StatusToIMGHDR()
{
  int i=0;
  do
  {
#ifdef ELKA
    icons[i]=CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH2,'\\',icons_names[i]),2);   	
#else
    icons[i]=CreateIMGHDRFromPngFile( MakeGlobalString(ICON_PATH,'\\',icons_names[i]),2);   	
#endif
    i++;
  }
  while(i<TOTAL_ICONS);

 // SUBPROC((void*)XStatusToIMGHDR);
}



void FreeStatusToIMGHDR()
{ 
  int i=0;
  do
  {
    if(icons[i]) mfree(icons[i]->bitmap);
    i++;
  }
  while(i<TOTAL_ICONS);  
  
  i=0;
  do
  {
    if(XStatusImg[i]->bitmap) mfree(XStatusImg[i]->bitmap);
    i++;
  }
  while(i<35);  
}

void free_ICONS(void)
{
  int i=0;
  do
  {
    mfree((void*)S_ICONS[i]);
    S_ICONS[i]=0;
    i++;
  }
  while(i<TOTAL_ICONS);
  return;
}

const char percent_t[]="%t";
const char percent_s[]="%s";
const char percent_d[]="%d";
const char empty_str[]="";
//const char I_str[]="I";
const char x_status_change[]="X-Status change";

char logmsg[256];
char logmsg2[256];


GBSTMR tmr_illumination;

void IlluminationOff(){
  SetIllumination(0,1,0,ILL_OFF_FADE);
  SetIllumination(1,1,0,ILL_OFF_FADE);
}

void IlluminationOn(const int disp, const int key, const int tmr, const int fade)
{
  if(!tmr) return;
  GBS_DelTimer(&tmr_illumination);
  SetIllumination(0,1,disp,fade);
  SetIllumination(1,1,key,fade);
  GBS_StartTimerProc(&tmr_illumination,tmr*216,IlluminationOff);
}

volatile int silenthide;    //by BoBa 25.06.07
volatile int disautorecconect;	//by BoBa 10.07
///////////
//int Is_Vibra_Enabled;
unsigned int Is_Sounds_Enabled;
int Is_Show_Offline;
int Is_Show_Groups;
//int CurrentStatus;
//extern int CurrentXStatus; //ili extern? /windes
int CurrentPrivateStatus;

//===================================================================

const char def_setting[]="%sdef_settings_mod_%d";   
const char def_sett[]="%sdef_statusset_mod_%d";
DEF_SETTINGS2 def_set[35];
void ReadDefSettings(void)
{
  DEF_SETTINGS def_se;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if ((f=fopen(str,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
  {
    fread(f,&def_se,sizeof(DEF_SETTINGS),&err);//читаем последние настройки
    fclose(f,&err);
    Is_Vibra_Enabled=def_se.vibra_status;
    Is_Sounds_Enabled=def_se.sound_status;
#ifdef ELKA
    Is_SLI_On=def_se.sli_state;
#endif
    Is_INC_On=def_se.inc_state;
    Is_ILU_On=def_se.ilu_state;
    Is_PING_On=def_se.ping_state;
//    Is_UPTIME_On=def_se.uptime_state;
    Is_Show_Offline=def_se.off_contacts;
    Is_Show_Groups=def_se.show_groups;
    CurrentStatus=def_se.def_status+1;
    CurrentXStatus=def_se.def_xstatus;
    Is_Double_Vibra_On=def_se.remind_state;
    Show_XTX=def_se.isXTX;//Показывать хстатус или время он лайн
    IsActiveUp=def_se.ActiveUp;
    if(ind_set_xstatus)
    {      
      snprintf(str,127,def_sett,elf_path,UIN);
      if ((f=fopen(str,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
      {
        for(int i=0,y=0/*sizeof(DEF_SETTINGS2)*/; i<34 ; i++,y+=sizeof(DEF_SETTINGS2))//читаем настройки для всех статусов
        {
          lseek(f,y,S_SET,&err,&err);
          fread(f,&def_set[i],sizeof(DEF_SETTINGS2),&err);      
        }
      }
    }
  fclose(f,&err);
  }
  else
  {
    Is_Vibra_Enabled=1;
#ifdef NEWSGOLD
    Is_Id3tag_Enabled=0;
#endif
    Is_Sounds_Enabled=1;
    Is_Show_Offline=0;
    Is_Show_Groups=0;
    CurrentStatus=IS_ONLINE;
    CurrentXStatus=0;
#ifdef ELKA
    Is_SLI_On=1;
#endif
    //Is_SMARTCPU_On=0;
    Is_INC_On=1;
    Is_Double_Vibra_On=0;
    Is_ILU_On=1;
    Is_PING_On=0;
  //  Is_UPTIME_On=0;
    Show_XTX=1;
    IsActiveUp=0;
    
    if(ind_set_xstatus)
    {
      for(int i=0;i<34;i++)//первый раз настройки для всех одинаковые
      {
        def_set[i].vibra_status=Is_Vibra_Enabled;
      #ifdef NEWSGOLD
        def_set[i].Id3tag_status=Is_Id3tag_Enabled;
      #endif
        def_set[i].sound_status=Is_Sounds_Enabled;
        def_set[i].off_contacts=Is_Show_Offline;
        def_set[i].show_groups=Is_Show_Groups;
      #ifdef ELKA
        def_set[i].sli_state=Is_SLI_On;
      #endif
//        def_set[i].smartcpu_state=Is_SMARTCPU_On;
        def_set[i].inc_state=Is_INC_On;
        def_set[i].remind_state=Is_Double_Vibra_On;
        def_set[i].ilu_state=Is_ILU_On;
        def_set[i].ping_state=Is_PING_On;
  //      def_set[i].uptime_state=Is_UPTIME_On;
        def_set[i].isXTX=Show_XTX;
        def_set[i].ActiveUp=IsActiveUp;
      }
    }
  }
  fclose(f,&err);
}

void WriteDefSettings(void)
{
  DEF_SETTINGS def_se;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if ((f=fopen(str,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))!=-1)
  {
    def_se.vibra_status=Is_Vibra_Enabled;//сохраняем текущее состояние флагов
    def_se.sound_status=Is_Sounds_Enabled;
    def_se.off_contacts=Is_Show_Offline;
    def_se.show_groups=Is_Show_Groups;
    def_se.def_status=CurrentStatus-1;
    def_se.def_xstatus=CurrentXStatus;
#ifdef ELKA
    def_se.sli_state=Is_SLI_On;
#endif
    def_se.inc_state=Is_INC_On;
    def_se.remind_state=Is_Double_Vibra_On;
    def_se.ilu_state=Is_ILU_On;
//    def_se.uptime_state=Is_UPTIME_On;
    def_se.isXTX=Show_XTX;//Показывать хстатус или время он лайн
    def_se.ping_state=Is_PING_On;  
    def_se.ActiveUp=IsActiveUp;
    fwrite(f,&def_se,sizeof(DEF_SETTINGS),&err);    
  }  
  fclose(f,&err);

if(ind_set_xstatus)
{
  snprintf(str,127,def_sett,elf_path,UIN);
  unlink(str,&err);

  if ((f=fopen(str,A_WriteOnly+A_BIN+A_Create+A_Append,P_WRITE,&err))!=-1)
  {
    def_set[CurrentXStatus].vibra_status=Is_Vibra_Enabled;//для текущего хстатуса сохраняем флаги
#ifdef NEWSGOLD
    def_set[CurrentXStatus].Id3tag_status=Is_Id3tag_Enabled;
#endif
    def_set[CurrentXStatus].sound_status=Is_Sounds_Enabled;
    def_set[CurrentXStatus].off_contacts=Is_Show_Offline;
    def_set[CurrentXStatus].show_groups=Is_Show_Groups;
#ifdef ELKA
    def_set[CurrentXStatus].sli_state=Is_SLI_On;
#endif
//    def_set[CurrentXStatus].smartcpu_state=Is_SMARTCPU_On;
    def_set[CurrentXStatus].inc_state=Is_INC_On;
    def_set[CurrentXStatus].remind_state=Is_Double_Vibra_On;
    def_set[CurrentXStatus].ilu_state=Is_ILU_On;
    def_set[CurrentXStatus].ping_state=Is_PING_On;
  //  def_set[CurrentXStatus].uptime_state=Is_UPTIME_On;
    def_set[CurrentXStatus].isXTX=Show_XTX;
    def_set[CurrentXStatus].ActiveUp=IsActiveUp;

//    lseek(f,sizeof(DEF_SETTINGS),S_SET,&err,&err);

    for(int i=0;i<34;i++)//и дописываем флаги для всех статусов в файл
    {
      fwrite(f,&def_set[i],sizeof(DEF_SETTINGS2),&err);
    }
  }
}  
 fclose(f,&err);
// UpdateCLheader();//Обновим хедер чтоб каки не было))
}

//by KreN 27.09.2007
//===================================================================

extern S_SMILES *s_top;
extern DYNPNGICONLIST *SmilesImgList;
extern DYNPNGICONLIST *XStatusesImgList;

//=============================Проигрывание звука=======================

void Play(const char *fname)
{
  if ((!IsCalling())&&Is_Sounds_Enabled)
  {
    FSTATS fstats;
    unsigned int err;
    if (GetFileStats(fname,&fstats,&err)!=-1)
    {
      PLAYFILE_OPT _sfo1;
      WSHDR* sndPath=AllocWS(128);
      WSHDR* sndFName=AllocWS(128);
      char s[128];
      const char *p=strrchr(fname,'\\')+1;
      str_2ws(sndFName,p,128);
      strncpy(s,fname,p-fname);
      s[p-fname]='\0';
      str_2ws(sndPath,s,128);
      
      zeromem(&_sfo1,sizeof(PLAYFILE_OPT));
      _sfo1.repeat_num=1;
      _sfo1.time_between_play=0;
      _sfo1.play_first=0;
      _sfo1.volume=sndVolume;
#ifdef NEWSGOLD
      _sfo1.unk6=1;
      _sfo1.unk7=1;
      _sfo1.unk9=2;
      PlayFile(0x10, sndPath, sndFName, GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1);
#else
#ifdef X75
      _sfo1.unk4=0x80000000;
      _sfo1.unk5=1;
      PlayFile(0xC, sndPath, sndFName, 0,GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1);
#else
      _sfo1.unk5=1;
      PlayFile(0xC, sndPath, sndFName, GBS_GetCurCepid(), MSG_PLAYFILE_REPORT, &_sfo1);
#endif
#endif
      FreeWS(sndPath);
      FreeWS(sndFName);
    }
  }
}

GBSTMR tmr_vibra;

volatile int vibra_count; //2058 - tam tak /windes

void start_vibra(void)
{
  void stop_vibra(void);
  if((Is_Vibra_Enabled)&&(!IsCalling()))
  {
    SetVibration(vibraPower);
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>2,stop_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,stop_vibra);
  }
}

void stop_vibra(void)
{ 
  SetVibration(0);
  if (--vibra_count)
  {
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>5,start_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,start_vibra);
  }
}

void start_vibra2(void)
{  
  void stop_vibra2(void);
 
  SetVibration(vibraPower);
  if(VIBR_TYPE)
    GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>2,stop_vibra2);
   else
     GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,stop_vibra2);
}

void stop_vibra2(void)
{
  SetVibration(0);
  if (--vibra_count)
  {
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>5,start_vibra2);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,start_vibra2);
  }
}

void ChangeVibra(void)
{
  if (!(Is_Vibra_Enabled=!(Is_Vibra_Enabled)))
    ShowMSG(1,(int)LG_MSGVIBRADIS);
  else
    ShowMSG(1,(int)LG_MSGVIBRAENA);
}

void ChangeSound(void)
{
  if (!(Is_Sounds_Enabled=!(Is_Sounds_Enabled)))
    ShowMSG(1,(int)LG_MSGSNDDIS);
  else
    ShowMSG(1,(int)LG_MSGSNDENA);
}

//===================================================================
//Templates
char *templates_chars; //Собственно файл
char **templates_lines; //Массив указателей на строки

void FreeTemplates(void)
{
  if (templates_lines) mfree(templates_lines);
  if (templates_chars) mfree(templates_chars);
  templates_lines=NULL;
  templates_chars=NULL;
}

int LoadTemplates(unsigned int uin)
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  FreeTemplates();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\%d.txt",uin);
  if (GetFileStats(fn,&stat,&ul)==-1) goto L1;
  if ((fsize=stat.size)<=0) goto L1;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1)
  {
  L1:
    strcpy(fn+i,"\\0.txt");
    if (GetFileStats(fn,&stat,&ul)==-1) return 0;
    if ((fsize=stat.size)<=0) return 0;
    f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul);
  }
  if (f==-1) return 0;
  p=templates_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	templates_lines=realloc(templates_lines,(i+1)*sizeof(char *));
	templates_lines[i++]=pp;
      }
      pp=NULL;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==NULL) pp=p;
    }
    p++;
  }
  return i;
}



//---------------------- AUTOBOT MESSAGE -------------------------//


#ifdef kluchnik
char *bot_chars; //Собственно файл
char **bot_lines; //Массив указателей на строки

void FreeBotMessage(void)
{
  if (bot_lines) mfree(bot_lines);
  if (bot_chars) mfree(bot_chars);
  bot_lines=NULL;
  bot_chars=NULL;
}

int LoadBotMessage(unsigned int uin)
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  FreeBotMessage();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\bot_%d.txt",uin);
  if (GetFileStats(fn,&stat,&ul)==-1) goto L1;
  if ((fsize=stat.size)<=0) goto L1;
  if ((f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul))==-1)
  {
  L1:
    strcpy(fn+i,"\\bot.txt");
    if (GetFileStats(fn,&stat,&ul)==-1) return 0;
    if ((fsize=stat.size)<=0) return 0;
    f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul);
  }
  if (f==-1) return 0;
  p=bot_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	bot_lines=realloc(bot_lines,(i+1)*sizeof(char *));
	bot_lines[i++]=pp;
      }
      pp=NULL;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==NULL) pp=p;
    }
    p++;
  }
  return i;
}
#endif

//===================================================================

int RXstate=EOP; //-sizeof(RXpkt)..-1 - receive header, 0..RXpkt.data_len - receive data

TPKT RXbuf;
TPKT TXbuf;

int connect_state=0;

int sock=-1;

volatile unsigned long TOTALRECEIVED;
volatile unsigned long TOTALSENDED;
volatile unsigned long ALLTOTALRECEIVED;	//by BoBa 10.07
volatile unsigned long ALLTOTALSENDED;

//volatile int sendq_l=0; //Длинна очереди для send
//volatile void *sendq_p=NULL; //указатель очереди

volatile int is_gprs_online=1;

GBSTMR reconnect_tmr;

extern void kill_data(void *p,void (*func_p)(void *));

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

int total_unread; 

volatile CLIST *cltop;

volatile unsigned int GROUP_CACHE; //Текущая группа для добавления

//volatile int contactlist_menu_id;

GBSTMR tmr_active;

volatile int edchat_id;

//Применяется для добавления сообщений
//CLIST *edcontact;
//void *edgui_data;

//static int prev_clmenu_itemcount;

char clm_hdr_text[64];

static const char key_clm_hdr_text[] = LG_CLT9INP;

static const int menusoftkeys[] = {0,1,2};
static const SOFTKEY_DESC menu_sk[] =
{
  {0x0018, 0x0000, (int)LG_SELECT},
  {0x0001, 0x0000, (int)LG_CLOSE},
  {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

char clmenu_sk_r[16];
static const char def_clmenu_sk_r[] = LG_CLOSE;
static const char key_clmenu_sk_r[] = LG_CLEAR;

const SOFTKEYSTAB menu_skt =
{
  menu_sk, 0
};

int tenseconds_to_ping;

LOGQ *NewLOGQ(const char *s)
{
  LOGQ *p=malloc(sizeof(LOGQ)+1+strlen(s));
  zeromem(p,sizeof(LOGQ));
  strcpy(p->text,s);
  return p;
}

LOGQ *LastLOGQ(LOGQ **pp)
{
  LOGQ *q=*pp;
  if (q)
  {
    while(q->next) q=q->next;
  }
  return(q);
}

//Уничтожить лог
void FreeLOGQ(LOGQ **pp)
{
  LOGQ *p=*pp;
  *pp=NULL; //Сразу обрежем
  while(p)
  {
    LOGQ *np=p->next;
    mfree(p);
    p=np;
  }
}

//Удалить элемент из лога
void RemoveLOGQ(LOGQ **queue, LOGQ *p)
{
  LOGQ *q=(LOGQ *)queue;
  LOGQ *qp;
  while(qp=q->next)
  {
    if (qp==p)
    {
      q->next=p->next;
      break;
    }
    q=qp;
  }
  mfree(p); //Если элемент вообще не в логе
}

//Добавить элемент в лог
int AddLOGQ(LOGQ **queue, LOGQ *p)
{
  LOGQ *q=(LOGQ *)queue;
  LOGQ *qp;
  int i=0;
  while(qp=q->next) {q=qp;i++;}
  p->next=NULL;
  q->next=p;
  return(i+1); //Теперь всего в логе элементов
}

char *GetLastXTextLOGQ(CLIST *t)
{
  LOGQ *p = t->log;
  char *s = 0;
  if(!p) return 0;
  while(p->next)
  {
    if((p->type&0x0F)==3)
      s = p->text;
    p = p->next;
  }
  if((p->type&0x0F)==3)
    s = p->text;
  return s;
}

int GetIconIndex(CLIST *t)
{
  unsigned short s;
  if (t)
  {
    s=t->state;
    if (t->isgroup) return(IS_GROUP);
    if (t->isunread)
      return(IS_MSG);
    else
    {
      if (s==0xFFFF) return(IS_OFFLINE);
      if ((s&0xF000)==0x2000) return (IS_LUNCH);
      if ((s&0xF000)==0x3000) return (IS_EVIL);
      if ((s&0xF000)==0x4000) return (IS_DEPRESSION);
      if ((s&0xF000)==0x5000) return (IS_HOME);
      if ((s&0xF000)==0x6000) return (IS_WORK);
      if (s & 0x0020) return(IS_FFC);
      if (s & 0x0001) return(IS_AWAY);
      if (s & 0x0005) return(IS_NA);
      if (s & 0x0011) return(IS_OCCUPIED);
      if (s & 0x0013) return(IS_DND);
      if (s & 0x0002) return(IS_FFC);
      if (s & 0x0100) return(IS_INVISIBLE);
    }
  }
  else
  {
    return(IS_UNKNOWN);
  }
  return(IS_ONLINE);
}

LOGQ *FindContactLOGQByAck(TPKT *p)
{
  CLIST *t;
  LockSched();
  t=FindContactByUin(p->pkt.uin);
  UnlockSched();
  unsigned int id=*((unsigned short*)(p->data));
  LOGQ *q;
  if (!t) return NULL;
  LockSched();
  q=t->log;
  while(q)
  {
    if (q->ID==id) break;
    q=q->next;
  }
  UnlockSched();
  return q;
}




/*----------------------------- SAVE CONTACT LIST TO FILE --------------------------------*/

void CreateCLFile(void)
{
  CLIST *t;
  t=(CLIST *)(&cltop);
  char *s=malloc(128);
  volatile int hFile_cl;
  unsigned int io_error_cl = 0;
  char fn[256];
  strcpy(fn,TEMPLATES_PATH);
  strcat(fn,"\\local.cl"); 
  unlink(fn,&io_error_cl);
  hFile_cl = fopen(fn,A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE, &io_error_cl);
  if(hFile_cl!=-1)
  {
    while(t=(CLIST *)(t->next))
    {    
      if(!t->isgroup && t->uin!=UIN)
      {
        sprintf(s, ";%d;%s;;;\r\n", t->uin, t->name);      
        fwrite(hFile_cl, s, strlen(s), &io_error_cl);
      }
     }
    ShowMSG(2,(int)"Saved local.cl");
  }
  else ShowMSG(2,(int)"Can't save local.cl");
  fclose(hFile_cl, &io_error_cl);
  mfree(s);  
}

/*-----------------------------------------------------------------------------------------*/

char on_off_buf[128];
int on_off_timer=0;
char ContactT9Key[32];
int kopa_online;
int kopa_kontaktu;
char status[64];
//void UpdateCLheader(void){};


void ClearContactT9Key(void)
{
  zeromem(ContactT9Key,sizeof(ContactT9Key));
}

void AddContactT9Key(int chr)
{
  int l=strlen(ContactT9Key);
  if (l<(sizeof(ContactT9Key)-1))
  {
    ContactT9Key[l]=chr;
  }
}

void BackSpaceContactT9(void)
{
  int l=strlen(ContactT9Key);
  if (l)
  {
    l--;
    ContactT9Key[l]=0;
  }
}


void RecountMenu(CLIST *req, int needfocus);

//--------------------------------------------------------------------//


void LongPressActionOnChat(CLIST *t,int i)
{
   switch(i)
    {
    case 1:
        Is_Sounds_Enabled=!(Is_Sounds_Enabled);
        if(Is_Sounds_Enabled)
        {
         // ShowMSG(1,(int)"Sound 0n");
            strcpy(prmsg,"Sound 0n");
            PrintTimer=2;
        }
        else
        {
         // ShowMSG(1,(int)"Sound Off");
            strcpy(prmsg,"Sound Off");
            PrintTimer=2;
        }
        SUBPROC((void*)WriteDefSettings);
        break;
    case 2:
        Is_Vibra_Enabled=!(Is_Vibra_Enabled);
        SUBPROC((void*)WriteDefSettings);
        if(Is_Vibra_Enabled)
        {
//          ShowMSG(1,(int)"Vibra On");
          strcpy(prmsg,"Vibra On");
          PrintTimer=2;
        }
        else
        {
         // ShowMSG(1,(int)"Vibra Off");
            strcpy(prmsg,"Vibra Off");
            PrintTimer=2;
        }
        break;
    case 3:        
         if(CurrentPrivateStatus!=1)
         {
           SetPrivateStatus(1);//Инвиз
//           ShowMSG(1,(int)"Инвиз включен");
            strcpy(prmsg,"Инвиз включен");
            PrintTimer=2;
         }
         else
         {
           SetPrivateStatus(3);
//           ShowMSG(1,(int)"Инвиз выключен");
           strcpy(prmsg,"Инвиз выключен");
           PrintTimer=2;
         }
         break;
    case 4:
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        if(Is_Double_Vibra_On)
        {
         // ShowMSG(1,(int)"Vibra rem On");
            strcpy(prmsg,"Vibra rem On");
            PrintTimer=2;
        }
        else 
        {
         // ShowMSG(1,(int)"Vibra rem Off");
            strcpy(prmsg,"Vibra rem Off");
            PrintTimer=2;
        }
        break;
    case 5:
      {
        CLIST *cl;
        cl=(CLIST *)(&cltop);
        while(cl=(CLIST *)(cl->next))
        {
         if(cl->log)
          {
            FreeLOGQ(&cl->log);
            cl->msg_count=0;
            cl->isactive=0;
          }           
        }      
        if(strlen(ContactT9Key)) ClearContactT9Key();
        RecountMenu(NULL,1);
//        ShowMSG(1,(int)"All chat clear!");
        strcpy(prmsg,"All chat clear!");
        PrintTimer=3;
        break;
      }
    case 6:
      if(!away_m)
      {
        away_m=!away_m;
        GBS_StopTimer(&away_tmr);
        msg_away_timer=AUTO_ENGADE*12;
        InputAwayMsg();
        
      }
      else
      {
        msg_away_timer=0;
        away_m=!away_m;
        if(change_back_status)
        {
          CurrentStatus=status_rem;
          set_my_status();
          change_back_status=0;
        }
      }
//      UpdateCLheader();
      break;
    case 7:
      {
        Show_XTX=!(Show_XTX);
        if(Show_XTX)
        {
         // ShowMSG(1,(int)"Default show x-status");
            strcpy(prmsg,"Default show x-status");
            PrintTimer=2;
        }
        else
        {
         // ShowMSG(1,(int)"Default show time");
            strcpy(prmsg,"Default show time");
            PrintTimer=2;
        }
      }
      break;      
    case 8:
      {        
        WSHDR *ws;
        ws=AllocWS(150);
        str_2ws(ws,suc_config_name,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
      break;
    case 9:
      {        
        WSHDR *ws;
        ws=AllocWS(150);
        str_2ws(ws,suc_config_name_mod,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
      break;
    case 10://ping
      {
        janis=1;
        TPKT *p;
        p=malloc(sizeof(PKT)+sizeof(TTime));
        GetDateTime(NULL,(TTime *)p->data);
        p->pkt.uin=0;
        p->pkt.type=T_ECHO;
        p->pkt.data_len=sizeof(TTime);
        SUBPROC((void *)SendAnswer,0,p);
      }
      break;
    case 11://go to idle
      {
        ClearContactT9Key();
        RecountMenu(NULL,1);
        silenthide=1;
        gipc.name_to=ipc_xtask_name;
        gipc.name_from=ipc_my_name;
        gipc.data=0;
        GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);    
      }
      break;
    case 12://hide & lock
      {        
        ClearContactT9Key();
        RecountMenu(NULL,1);
        gipc.name_to=ipc_xtask_name;
        gipc.name_from=ipc_my_name;
        gipc.data=0;
        GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
        if (IsUnlocked())
        {
          KbdLock();
        }
//        return(-1);
      }
      break;
    case 13://users menu
     {
       int CreateContactMenu(CLIST *t);
       if(!t->isgroup)
         CreateContactMenu(t);    
     }
     break;
    }//switch()
   
   for(int j=0;j<1000;) j++; 
   
}

//{"Без действия","Звук","Вибра","Инвиз","Реж.напоминания","Очистить чаты","Автоответчик","Показ.хстат/время","Конфиг","Конфиг мода","Пинг","Заблокировать","На гэ","Меню юзера"}

void LongPressAction(CLIST *t,int key)
{ 
  switch(key)
  {
    case '0': LongPressActionOnChat(t,longAct_0); break;
    case '1': LongPressActionOnChat(t,longAct_1); break;
    case '2': LongPressActionOnChat(t,longAct_2); break;
    case '3': LongPressActionOnChat(t,longAct_3); break;
    case '4': LongPressActionOnChat(t,longAct_4); break;
    case '5': LongPressActionOnChat(t,longAct_5); break;
    case '6': LongPressActionOnChat(t,longAct_6); break;
    case '7': LongPressActionOnChat(t,longAct_7); break;
    case '8': LongPressActionOnChat(t,longAct_8); break;
    case '9': LongPressActionOnChat(t,longAct_9); break;  
    case '*': LongPressActionOnChat(t,longAct_10); break;  
    case '#': LongPressActionOnChat(t,longAct_11); break;  
    case RIGHT_SOFT: LongPressActionOnChat(t,longAct_RB); break;  
  }
}

char hostbuf2[128];


void GetOnTotalContact(int group_id,int *_onlinetotal)
{
  CLIST *t;
  t=(CLIST *)&cltop;
  int online=0,total=0;
  while((t=t->next))
  {
    if(t->group==group_id && !t->isgroup)
    {
      total++;
      if (t->state!=0xFFFF) online++;
    }
  }
  _onlinetotal[0]=online;
  _onlinetotal[1]=total;
}

//===============================================================================================

int DNR_ID=0;
int DNR_TRIES=3;

char hostname[128]; //2058 che toka 128? :)

int host_counter = 0;

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
  if(*str!=':') return NATICQ_PORT;
  str++;
  numbuf[5] = 0;
  for(;*str!=';' && *str!=' ' && *str!='\x0D' && *str!='\x0A' && *str && numcnt<5; numbuf[numcnt] = *str, str++, numcnt++);
  numbuf[numcnt] = 0; //2058
  return atoi(numbuf);

}
//
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
//---------------------------------------------------------------------------

void create_connect(void)
{
//  static int host_counter = 0;
  char hostbuf[128], buf[128];
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
    strcpy(logmsg,LG_GRWAITFORGPRS);
    SMART_REDRAW();
    return;
  }
  DNR_ID=0;
  *socklasterr()=0;
  if(host_counter > GetHostsCount(NATICQ_HOST)-1) host_counter = 0; //2058
  GetHost(host_counter, NATICQ_HOST, hostbuf);
  hostport = GetPort(host_counter, NATICQ_HOST);
  host_counter++;

  sprintf(hostname, "Connect to %s:%d", hostbuf, hostport); //2058 - stavim hostname, izbavlyaemsya ot buf
  //strcpy(hostbuf2, hostbuf); //vyvod host v get_online_time
  zeromem(hostbuf2,128);
  sprintf(hostbuf2, percent_s,hostbuf);
  strcpy(logmsg,buf);
  SMART_REDRAW();
    
  ip=str2ip(hostbuf);
  if (ip!=0xFFFFFFFF)  
  {
    sa.ip=ip;
    strcpy(logmsg,"\nConnect by IP!"); //strcat->strcpy
    SMART_REDRAW();
    goto L_CONNECT;
  }  
  strcat(logmsg,"\n");
  strcpy(logmsg,LG_GRSENDDNR); //strcat->strcpy
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
      snprintf(logmsg,255,LG_GRDNRERROR,err);
      SMART_REDRAW();
      GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
      return;
    }
  }
  if (p_res)
  {
    if (p_res[3])
    {
      strcpy(logmsg,buf);
      strcat(logmsg,"\n");
      strcpy(logmsg,LG_GRDNROK);
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
	  ShowMSG(1,(int)LG_MSGCANTCONN);
	  UnlockSched();
	  GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);

	}
      }
      else
      {
	LockSched();
	ShowMSG(1,(int)LG_MSGCANTCRSC);
	UnlockSched();
	//Не осилили создания сокета, закрываем GPRS-сессию
	GPRS_OnOff(0,1);
      }
    } 
  }
  else
  {
    DNR_TRIES--;
    LockSched();
    ShowMSG(1,(int)LG_MSGHOSTNFND);
    UnlockSched();
  }
}

#ifdef SEND_TIMER
GBSTMR send_tmr;
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

void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
  }
#ifdef SEND_TIMER
  GBS_DelTimer(&send_tmr);
#endif
  
  Play(sndLostConn);
}

#ifdef SEND_TIMER
static void resend(void)
{
  void SendAnswer(int dummy, TPKT *p);
  SUBPROC((void*)SendAnswer,0,0);
}
#endif

void SendAnswer(int dummy, TPKT *p)
{
  int i;
  int j;
  if (connect_state<2)
  {
    mfree(p);
    return;
  }
  if (p)
  {
    j=sizeof(PKT)+p->pkt.data_len; //Размер пакета
    TOTALSENDED+=j;
    ALLTOTALSENDED+=j;			//by BoBa 10.07    
    //Проверяем, не надо ли добавить в очередь
    if (sendq_p)
    {
      //Есть очередь, добавляем в нее
      sendq_p=realloc((void *)sendq_p,sendq_l+j);
      memcpy((char *)sendq_p+sendq_l,p,j);
      mfree(p);
      sendq_l+=j;
      return;
    }
    sendq_p=p;
    sendq_l=j;
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
    if (j<i)
    {
      //Передали меньше чем заказывали
#ifdef SEND_TIMER
      GBS_StartTimerProc(&send_tmr,216*5,resend);
#endif
      return; //Ждем сообщения ENIP_BUFFER_FREE1
    }
    tenseconds_to_ping=0; //Чего-то послали, можно начинать отсчет времени до пинга заново
  }
  mfree((void *)sendq_p);
  sendq_p=NULL;
}


void send_login(int dummy, TPKT *p)
{
  connect_state=2;
  char rev[16];
  //Кто будет менять в этом месте идентификатор клиента, буду банить на уровне сервера!!!
  //А Вова будет банить на форуме!
  snprintf(rev,9,"Sie_%04d",__SVN_REVISION__);
  
  TPKT *p2=malloc(sizeof(PKT)+8);
  p2->pkt.uin=UIN;
  p2->pkt.type=T_SETCLIENT_ID;
  p2->pkt.data_len=8;
  memcpy(p2->data,rev,8);
  SendAnswer(0,p2);
  SendAnswer(dummy,p);
  RXstate=-(int)sizeof(PKT);
}

void do_ping(void)
{
  TPKT *pingp=malloc(sizeof(PKT));
  pingp->pkt.uin=UIN;
  pingp->pkt.type=0;
  pingp->pkt.data_len=0;
  SendAnswer(0,pingp);
}

void SendMSGACK(int i)
{
  TPKT *ackp=malloc(sizeof(PKT));
  ackp->pkt.uin=i;
  ackp->pkt.type=T_MSGACK;
  ackp->pkt.data_len=0;
  SendAnswer(0,ackp);
}

void RequestXText(unsigned int uin)
{
  TPKT *p=malloc(sizeof(PKT));
  p->pkt.uin=uin;
  p->pkt.type=T_XTEXT_REQ;
  p->pkt.data_len=0;
  SUBPROC((void *)SendAnswer,0,p);
}

int ping_time;
char I_UIN[9];
char sds[]="%s%d.%s";
char mp3[]="mp3";
char wav[]="wav";
char amr[]="amr";
int ShowLoadScreen=1;
  
void get_answer(void)
{
  void *p;
  int i=RXstate;
  int j;
  int n;
  char rb[1024];
  char *rp=rb;
  if (connect_state<2) return;
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
    }
    if (i>=0)
    {
      //Принимаем тельце ;)
      n=RXbuf.pkt.data_len; //Всего в тельце
      if (n>16383)
      {
	//Слишком много
	strcpy(logmsg,LG_GRBADPACKET);
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
      if (RXbuf.pkt.data_len==i)
      {
	//Пакет полностью получен
	TOTALRECEIVED+=(i+8);
	ALLTOTALRECEIVED+=(i+8);			//by BoBa 10.07	
	//Пакет удачно принят, можно разбирать...
	RXbuf.data[i]=0; //Конец строки
        aa(&RXbuf);
	switch(RXbuf.pkt.type)
	{
	case T_LOGIN:
	  //Удачно залогинились
	  //Посылаем в MMI
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  Play(sndStartup);
	  //        GBS_StartTimerProc(&tmr_ping,120*TMR_SECOND,call_ping);
	  snprintf(logmsg,255,LG_GRLOGINMSG,RXbuf.data);
	  connect_state=3;
          host_counter--;
	  SMART_REDRAW();
	  break;
	case T_XTEXT_ACK:
	case T_GROUPID:
	case T_GROUPFOLLOW:
	case T_CLENTRY:
	  //Посылаем в MMI
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  //snprintf(logmsg,255,"CL: %s",RXbuf.data);
	  break;
	case T_STATUSCHANGE:
	  n=i+sizeof(PKT);
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  snprintf(logmsg,255,LG_GRSTATUSCHNG,RXbuf.pkt.uin,*((unsigned short *)(RXbuf.data)));
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  break;
	case T_ERROR:
	  snprintf(logmsg,255,LG_GRERROR,RXbuf.data);
	  SMART_REDRAW();
	  break;
	case T_RECVMSG:
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  {
	    char *s=p;
	    s+=sizeof(PKT);
	    int c;
	    while((c=*s))
	    {
	      if (c<3) *s=' ';
	      s++;
	    }
	  }          
          
//------------------------------------------------------------//    
         
          sprintf(I_UIN, "uin%d", RXbuf.pkt.uin);
          if(strstr(ignor_list,I_UIN)==0)
          {
            snprintf(logmsg,255,LG_GRRECVMSG,RXbuf.pkt.uin,RXbuf.data);
            SendMSGACK(TOTALRECEIVED);
            GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
            SMART_REDRAW();
            if(!extended_sounds)
            {
              CLIST *t;
              t=FindContactByUin(RXbuf.pkt.uin);
              if(t->isIndividSet)//Если индивидуальные натсройки(меню на долгое вправо)
              {
                if(t->issound) Play(sndMsg);//если звук включен то играем
                break;
              }
              else
                if(t->automsgcount < automsgcount && away_m)//если сигналы автоответчика
                {
                  Play(sndMsg);
                  break;
                }
              else
               if(t->isaccess)//если удаленный доступ ;)
                {
                  if(t->signals) Play(sndMsg);
                  break;
                }              
              else//в остальный случаях
                Play(sndMsg);
            }
            else
            {
              uins=RXbuf.pkt.uin;
              volatile int hFile_ex;
              unsigned int io_error_ex = 0;
              char ext[4];
              // __root const CFG_CBOX_ITEM cfgcbox18_3_2[2]={"Mp3","Wav","Amr"};
              switch(ext_type_sounds)
              {
                case 0: sprintf(ext,percent_s,mp3); break;
                case 1: sprintf(ext,percent_s,wav); break;
                case 2: sprintf(ext,percent_s,amr); break;              
                default : sprintf(ext,percent_s,mp3); break;
              }
              
              sprintf(talk_sound, sds, sndMsg2, uins, ext);
              hFile_ex = fopen(talk_sound,A_ReadOnly + A_BIN,P_READ, &io_error_ex);
              if(hFile_ex!=-1)
              {
                fclose(hFile_ex, &io_error_ex);
                Play(talk_sound);
              }
              else
                Play(sndMsg);
            }
          }
	  break;
	case T_SSLRESP:
	  LockSched();
	  ShowMSG(1,(int)RXbuf.data);
	  UnlockSched();
	  break;
	case T_SRV_ACK:
	  if (FindContactLOGQByAck(&RXbuf))
          {
            Play(sndMsgSent);          
            if(!ShowLoadScreen) SMART_REDRAW(); 
          }
	case T_CLIENT_ACK:
	  p=malloc(sizeof(PKT)+2);
	  memcpy(p,&RXbuf,sizeof(PKT)+2);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  break;
	case T_ECHORET:
	  {
	    TDate d;
	    TTime t;
	    TTime *pt=(TTime *)(RXbuf.data);
	    int s1;
	    int s2;
	    GetDateTime(&d,&t);
	    s1=t.hour*3600+t.min*60+t.sec;
	    s2=pt->hour*3600+pt->min*60+pt->sec;
	    s1-=s2;
	    if (s1<0) s1+=86400;
            curping=s1;
	    snprintf(logmsg,255,"Ping %d-%d seconds!",s1,s1+1);
            sprintf(PINGO, "Ping: %d sec",s1);
	    LockSched();
            if(janis==1)
            {
//              ShowMSG(1,(int)logmsg);
              PrintTimer=5;
              strcpy(prmsg,logmsg);
              Play(pingsnd);
              janis=0;
            }
            else
            {
              if(ping_time==0)
              {
                ping_time=ping_time_config;
                Play(pingsnd);
              }
              else
                ping_time--;
            }
	    UnlockSched();
	  }
	  break;
        case T_LASTPRIVACY:
          n=i+sizeof(PKT);
          p=malloc(n);
          memcpy(p,&RXbuf,n);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  break;
	}
        ad(&RXbuf);
	i=-(int)sizeof(PKT); //А может еще есть данные
      }
    }
  }
  RXstate=i;
  //  GBS_StartTimerProc(&tmr_dorecv,3000,dorecv);
  //  SMART_REDRAW();
}


//=====================add to main info====================

//Сюда я не лез...и че та желания нет...kluchnik ;)

char a_txt[45];
char s_txt[45];
char d_txt[45];
char f_txt[45];
char g_txt[45];
unsigned int main_msg_timer2;
void AddToMain(const char *nameu,char *si)
{
  char *de=malloc(64);
  int text_l;
  int iz;
  int name_l;
  name_l=0;
  text_l=0;
  name_l=strlen(nameu);
  iz=0;
#ifdef ELKA
  text_l=main_lenght-name_l;
#else
  text_l=main_lenght-name_l;
#endif
  
  if(strlen(si)>text_l)
  {
    while(text_l!=0)
    {
      de[iz]=si[iz];
      iz++;
      text_l--;
    }
    if(use_cutter)
    {
      de[iz]='.';
      iz++;
      de[iz]='.';
      iz++;
      de[iz]='.';
      iz++;
    }
    de[iz]='\0';
  }
  else
     strcpy(de, si);
  iz=0;
  while(strlen(de)!=iz)
   {
     if(de[iz]==0x0D) de[iz]=0x20;
     if(de[iz]==0x0A) de[iz]=0x20;
     iz++;
   }
  de[iz]='\0';
  if(main_count==0) sprintf(a_txt, "%s: %s", nameu,de);
  if(main_count==1) sprintf(s_txt, "%s: %s", nameu,de);
  if(main_count==2) sprintf(d_txt, "%s: %s", nameu,de);
  if(main_count==3) sprintf(f_txt, "%s: %s", nameu,de);
  if(main_count==4) sprintf(g_txt, "%s: %s", nameu,de);
  if(main_count>4)
  {
    strcpy(a_txt, s_txt);
    strcpy(s_txt, d_txt);
    strcpy(d_txt, f_txt);
    strcpy(f_txt, g_txt);
    sprintf(g_txt, "%s: %s", nameu,de);
  }
  mfree(de);
  main_d=1;
}

//======================add to main info================

int NEW_MSG;

void AddStringToLog(CLIST *t, int code, char *s, const char *name, unsigned int IDforACK,int flag)
{
  char hs[128], *lastX;
  TTime tt;
  TDate d;
  GetDateTime(&d,&tt);
  int i;
  
//  if(code == 3 && !LOG_XTXT) return; //Нl нуcно nоodrн?nь cennnrnуn
  if ( code==3 && NOT_LOG_SAME_XTXT )
  {
#ifdef kluchnik
    if(!t->isactive && HISTORY_BUFFER && (t->isLoadHistory && t->isIndividSet)) GetHistory(t, 64<<HISTORY_BUFFER);
    else if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
#else
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
#endif
    lastX = GetLastXTextLOGQ(t);
    if(lastX)
      if(strcmp(lastX, s) == 0) return;
  }
  
  snprintf(hs,127,"%02d:%02d:%02d %02d.%02d %s:\r\n",tt.hour,tt.min,tt.sec,d.day,d.month,name);
  if(code != 3 || ((LOG_XTXT) && (flag)))//Нужно сохранять иксстатус
   Add2History(t, hs, s, code); 

  LOGQ *p=NewLOGQ(s);
  snprintf(p->hdr,79,"%02d:%02d:%02d %02d.%02d %s:",tt.hour,tt.min,tt.sec,d.day,d.month,name);
  p->type=code;
  p->ID=IDforACK;  //0-32767
  i=AddLOGQ(&t->log,p);
  while(i>MAXLOGMSG)
  {
    if (t->log==t->last_log) t->last_log=t->last_log->next;
    RemoveLOGQ(&t->log,t->log);
    i--;
  }
  t->msg_count=i;
  if (!t->last_log) t->last_log=p;
  if (code==3)
  {
    if (edchat_id)
    {
      void *data=FindGUIbyId(edchat_id,NULL);
      if (data)
      {
	EDCHAT_STRUCT *ed_struct;
	ed_struct=EDIT_GetUserPointer(data);
	if (ed_struct) 
	{
	  if (ed_struct->ed_contact==t)
	  {
	    goto L_INC;
	  }
	}
      }
    }
    goto L_NOINC;
  }
L_INC:
  if (!t->isunread) total_unread++;  
  if(NEW_MSG==1)
  {
    t->unread_msg++;  
    total_unread2++;
    NEW_MSG=0;
  }
  t->isunread=1;
L_NOINC:
  ChangeContactPos(t);
}

void ParseAnswer(WSHDR *ws, const char *s);

int time_to_stop_t9;

void ParseXStatusText(WSHDR *ws, const char *s, int color)
{
  int c;
  int flag=0;
  CutWSTR(ws,0);
  if (strlen(s)==1) return;
  wsAppendChar(ws,0xE008);
  wsAppendChar(ws,color);
  wsAppendChar(ws,0xE013);
  while((c=*s++))
  {
    if (c==13)
    {
      if (!flag)
      {
        flag=1;
        wsAppendChar(ws,0xE012);
        c=' ';
      }
    }
    wsAppendChar(ws,char8to16(c));
  }
}

//Добавление итемов в чат при получении нового сообщения
void AddMsgToChat(void *data)
{
  LOGQ *p;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  EDCHAT_STRUCT *ed_struct;
  int j;
  int color, font, type;
  int zc;
  if (!data) return;
  ed_struct=EDIT_GetUserPointer(data);
  if (!ed_struct) return;
  if (!ed_struct->ed_contact->isunread) return;
    
  p=ed_struct->ed_contact->last_log;
  if (p)
  {
    while(p)
    {
      font = ED_H_FONT_SIZE;
      if ((zc=p->acked&3))
      {
	if (zc==1)
	  color=ACK_COLOR; //Зеленый
	else
	  color=I_COLOR;
      }
      else
      {
	if (p->ID==0xFFFFFFFF)
	  {
          type = p->type&0x0F;
          if(p->type&0x10)
          {
            color = (type==1)?O_I_COLOR:((type==3)?O_X_COLOR:O_TO_COLOR);
            font = (type==3)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
          }
          else
          {
            color = (type==1)?I_COLOR:((type==3)?X_COLOR:TO_COLOR);
            font = (type==3)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
          }
        }
        else
	  color=UNACK_COLOR; //?????
      }
      PrepareEditControl(&ec);
      if ((p->type&0x0F)!=3)
      {
        ascii2ws(ews,p->hdr);
        ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
      }
      else
        ConstructEditControl(&ec,ECT_HEADER,ECF_DELSTR,ews,0);      
      PrepareEditCOptions(&ec_options);
      SetPenColorToEditCOptions(&ec_options,color);
      SetFontToEditCOptions(&ec_options,font);
      CopyOptionsToEditControl(&ec,&ec_options);
      //AddEditControlToEditQend(eq,&ec,ma);
      EDIT_InsertEditControl(data,ed_struct->ed_answer-1,&ec);
      ed_struct->ed_answer++;
      if ((p->type&0x0F)!=3)
      {
        ParseAnswer(ews,p->text);
      }
      else
      {
        ParseXStatusText(ews, p->text, (p->type&0x10)?O_X_COLOR:X_COLOR);
      }
      PrepareEditControl(&ec);
      ConstructEditControl(&ec,
                           ECT_NORMAL_TEXT,
                           ews->wsbody[0] ? ECF_APPEND_EOL|ECF_DISABLE_T9 : ECF_DELSTR,
                           ews,ews->wsbody[0]);
      PrepareEditCOptions(&ec_options);
//#ifdef M75
      if ((p->type&0x0F)!=3)
      {
        SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_FONT_SIZE:ED_FONT_SIZE);
      }
      else
      {
        SetPenColorToEditCOptions(&ec_options,(p->type&0x10)?O_X_COLOR:X_COLOR); 
        SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_X_FONT_SIZE:ED_X_FONT_SIZE);
      }
//#else
//      SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_FONT_SIZE:ED_FONT_SIZE);
//#endif    
      CopyOptionsToEditControl(&ec,&ec_options);
      EDIT_InsertEditControl(data,ed_struct->ed_answer-1,&ec);
      ed_struct->ed_answer++;
      p=p->next;
    }
    j=((ed_struct->ed_contact->msg_count+1)*2); //Ожидаемый ed_answer
    while(j<ed_struct->ed_answer)
    {
      EDIT_RemoveEditControl(ed_struct->ed_chatgui,1);
      ed_struct->ed_answer--;
    }
  }
  ed_struct->ed_contact->last_log=NULL;
  if (IsGuiOnTop(edchat_id)) 
  {
    total_unread--;
  }
  else
    ed_struct->requested_decrement_total_unread++;
  ed_struct->ed_contact->isunread=0;
  total_unread2=total_unread2-ed_struct->ed_contact->unread_msg;
  ed_struct->ed_contact->unread_msg=0;
  ChangeContactPos(ed_struct->ed_contact);
  //  EDIT_SetFocus(data,ed_struct->ed_answer);
}

//Рисуем подтверждения непосредственно
void DrawAck(void *data)
{
  LOGQ *p;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  EDCHAT_STRUCT *ed_struct;
  int j;
  int color;
  int dorefresh=0;
  if (!data) return;
  ed_struct=EDIT_GetUserPointer(data);
  if (!ed_struct) return;
  p=ed_struct->ed_contact->log;
  j=ed_struct->ed_answer;
  j-=(ed_struct->ed_contact->msg_count*2)+1; //Номер собственно хедера верхнего итема
  while(p)
  {
    if (j>=1) //Пока возможно лишнее
    {
      if (!(p->acked&4))
      {
	ExtractEditControl(data,j,&ec);
	PrepareEditCOptions(&ec_options);
	if (p->acked)
	{
	  if (p->acked==1)
	    color=ACK_COLOR; //Зеленый
	  else
	    color=I_COLOR;
	}
	else
	{
	  if (p->ID==0xFFFFFFFF)
	    color=(p->type&0x10)?(((p->type&0x0F)==1)?O_I_COLOR:O_TO_COLOR):(((p->type&0x0F)==1)?I_COLOR:TO_COLOR);
	  else
	    color=UNACK_COLOR; //Серый
	}
	SetPenColorToEditCOptions(&ec_options,color/*p->type==1?I_COLOR:TO_COLOR*/);
	SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_H_FONT_SIZE:ED_H_FONT_SIZE);
	CopyOptionsToEditControl(&ec,&ec_options);
	StoreEditControl(data,j,&ec);
	p->acked|=4; //Обработали
	dorefresh=1;
      }
    }
    j+=2;
    p=p->next;
  } 
  if (IsGuiOnTop(edchat_id)&&dorefresh) RefreshGUI();
}

void ask_my_info(void)
{
  /*  TPKT *p;
  CLIST *t;
  p=malloc(sizeof(PKT));
  p->pkt.uin=UIN;
  p->pkt.type=T_REQINFOSHORT;
  p->pkt.data_len=0;
  //  AddStringToLog(t,0x01,"Request info...",I_str);
  SUBPROC((void *)SendAnswer,0,p);*/
}

void set_my_status(void)
{
  TPKT *p;
  p=malloc(sizeof(PKT)+1);
  p->pkt.uin=0;               // Никому; поле нужно проигнорировать на сервере
  p->pkt.type=T_MY_STATUS_CH; // Тип пакета: изменение статуса
  p->pkt.data_len=1;          // Длина пакета: 1 байт
  p->data[0]=CurrentStatus;
  SUBPROC((void *)SendAnswer,0,p);
}

extern int Send0;

void set_my_xstatus(void)
{
#ifdef NEWSGOLD
  if(Is_Id3tag_Enabled)
  {
    if(CurrentXStatus!=11 || !Is_ID3_ANY_STATUS)
    {
#endif
      TPKT *p;
      char *s1;
      char *s2;
      int l1;
      int l2;
      p=malloc(sizeof(PKT)+1);
      p->pkt.uin=0;               // Никому; поле нужно проигнорировать на сервере
      p->pkt.type=T_MY_XSTATUS_CH; // Тип пакета: изменение статуса
      p->pkt.data_len=1;          // Длина пакета: 1 байт
      p->data[0]=CurrentXStatus;
      SUBPROC((void *)SendAnswer,0,p);
      s1=GetXStatusStr(CurrentXStatus*3+1,&l1);
      s2=GetXStatusStr(CurrentXStatus*3+2,&l2);
      if ((!s1)||(!s2)) return;
      p=malloc(sizeof(PKT)+l1+l2+1);
      p->pkt.uin=0;
      p->pkt.type=T_XTEXT_SET;
      p->pkt.data_len=l1+l2+1;
      strncpy(p->data+0,s1,l1);
      p->data[l1]=0;
      strncpy(p->data+l1+1,s2,l2);
      SUBPROC((void *)SendAnswer,0,p);
#ifdef NEWSGOLD
    }
    else
    {
      tracktimer=0;
      GetTags();
    }
  }
  else
  {
    TPKT *p;
    char *s1;
    char *s2;
    int l1;
    int l2;
    p=malloc(sizeof(PKT)+1);
    p->pkt.uin=0;               // Никому; поле нужно проигнорировать на сервере
    p->pkt.type=T_MY_XSTATUS_CH; // Тип пакета: изменение статуса
    p->pkt.data_len=1;          // Длина пакета: 1 байт
    p->data[0]=CurrentXStatus;
    SUBPROC((void *)SendAnswer,0,p);
    s1=GetXStatusStr(CurrentXStatus*3+1,&l1);
    s2=GetXStatusStr(CurrentXStatus*3+2,&l2);
    if ((!s1)||(!s2)) return;
    p=malloc(sizeof(PKT)+l1+l2+1);
    p->pkt.uin=0;
    p->pkt.type=T_XTEXT_SET;
    p->pkt.data_len=l1+l2+1;
    strncpy(p->data+0,s1,l1);
    p->data[l1]=0;
    strncpy(p->data+l1+1,s2,l2);
    SUBPROC((void *)SendAnswer,0,p);
    tracktimer=0;
    GetTags();
  }
#endif
}

#ifdef NEWSGOLD

void set_my_xstatus2(void)
{
  TPKT *p;
//  char *s1;
//  char *s2;
  int l1;
  int l2;
  p=malloc(sizeof(PKT)+1);
  p->pkt.uin=0;               // Никому; поле нужно проигнорировать на сервере
  p->pkt.type=T_MY_XSTATUS_CH; // Тип пакета: изменение статуса
  p->pkt.data_len=1;          // Длина пакета: 1 байт
  p->data[0]=11;
  SUBPROC((void *)SendAnswer,0,p);
//  char *s1;
  char *sk2=malloc(256);
//  strcpy(s1, playing);
//  s1="Now playing:";
  strcpy(sk2,teast2);
  l1=strlen(playing);
  l2=strlen(sk2);
  if ((!playing)||(!sk2)) return;
  p=malloc(sizeof(PKT)+l1+l2+1);
  p->pkt.uin=0;
  p->pkt.type=T_XTEXT_SET;
  p->pkt.data_len=l1+l2+1;
  strncpy(p->data+0,playing,l1);
  p->data[l1]=0;
  strncpy(p->data+l1+1,sk2,l2);
  mfree(sk2);
  SUBPROC((void *)SendAnswer,0,p);
//  mfree(s1);
//  mfree(s2);
}
#endif
void to_develop(void)
{
  if (silenthide) return;
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)maincsm_id;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_SHOW_CSM,&gipc);  
}

void ReqAddMsgToChat(CLIST *t)
{
  if (edchat_id)
  {
    void *data=FindGUIbyId(edchat_id,NULL);
    {
      EDCHAT_STRUCT *ed_struct;
      ed_struct=EDIT_GetUserPointer(data);
      if (ed_struct) 
      {
        if (ed_struct->ed_contact==t)
        {
          if (EDIT_IsBusy(data))
          {
            t->req_add=1;
#ifdef EVILFOX
            time_to_stop_t9=2;
#else
            time_to_stop_t9=3;
#endif
          }
          else
          {
            AddMsgToChat(data);
            DirectRedrawGUI_ID(edchat_id);
	  }
	}
      }
    }
  }
}

GBSTMR develop;

void begin_to_develop()
{
  if(IsUnlocked())
    to_develop();
  else
    GBS_StartTimerProc(&develop, 100, begin_to_develop);
}



//----------------------------- LOAD COMMANDS  -----------------------------//
//Templates
char *cmd_chars; //Собственно файл
char **cmd_lines; //Массив указателей на строки

void cmdFree(void)
{
  if (cmd_lines) mfree(cmd_lines);
  if (cmd_chars) mfree(cmd_chars);
  cmd_lines=NULL;
  cmd_chars=NULL;
}

int LoadCmd()
{
  FSTATS stat;
  char fn[256];
  int f;
  unsigned int ul;
  int i;
  int fsize;
  char *p;
  char *pp;
  int c;
  cmdFree();
  strcpy(fn,TEMPLATES_PATH);
  i=strlen(fn);
  sprintf(fn+i,"\\cmd.txt");
  GetFileStats(fn,&stat,&ul);
  fsize=stat.size;
  f=fopen(fn,A_ReadOnly+A_BIN,P_READ,&ul);
  if (f==-1) return 0;
  p=cmd_chars=malloc(fsize+1);
  p[fread(f,p,fsize,&ul)]=0;
  fclose(f,&ul);
  i=0;
  pp=p;
  for(;;)
  {
    c=*p;
    if (c<32)
    {
      if (pp&&(pp!=p))
      {
	cmd_lines=realloc(cmd_lines,(i+1)*sizeof(char *));
	cmd_lines[i++]=pp;
      }
      pp=NULL;
      if (!c) break;
      *p=0;
    }
    else
    {
      if (pp==NULL) pp=p;
    }
    p++;
  }
  return i;
}

int NumOfItems=0;
HEADER_DESC CmdMenuHeader={NULL,NULL,NULL,NULL,NULL,NULL,LGP_NULL};

void CmdMenuItemHandler(void *data, int curitem, void *unk)
{
  void *item=AllocMenuItem(data);
  char *p=cmd_lines[curitem];
  WSHDR *ws=AllocMenuWS(data,strlen(p));
  ascii2ws(ws,p);
  SetMenuItemText(data, item, ws, curitem);
}

void CmdMenuGHook(void *data, int cmd)
{
  if (cmd==TI_CMD_FOCUS)
  {
    DisableIDLETMR();
  }
  if (cmd==TI_CMD_DESTROY)
  {
    cmdFree();
  }
}

static int CmdMenuKeyHook(void *data, GUI_MSG *msg)
{
  int Selected=GetCurMenuItem(data);
  EDCHAT_STRUCT *ed_struct=MenuGetUserPointer(data);
  if ((msg->keys==0x18)||(msg->keys==0x3D))
  {
    if (NumOfItems)
    {
      const char *p=cmd_lines[Selected];
      WSHDR *ws;
      ws=AllocWS(256);
     // wsprintf(ws,percent_t,p);
      ascii2ws(ws,p);
      EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ws);
      EDIT_SetCursorPos(ed_struct->ed_chatgui,wstrlen(ws)+1);      
      FreeWS(ws);
      return(1);
    }
  }
  return(0);
}

const int CmdSoftkeys[]={0,1,2};

const SOFTKEY_DESC cmd_sk[]=
{
  {0x0004,0x0000,(int)""},
  {0x0001,0x0000,(int)LG_CLOSE},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};
const SOFTKEYSTAB cmd_Back_skt=
{
 cmd_sk,0
};
static const MENU_DESC CmdMenuStruct=
{
  8,CmdMenuKeyHook,CmdMenuGHook,NULL,
  CmdSoftkeys,
  &cmd_Back_skt,
  0x10,
  CmdMenuItemHandler,
  NULL, //menuitems,
  NULL, //menuprocs,
  NULL
};

int DispCmdMenu(void *ed_gui)
{
  NumOfItems=LoadCmd();
  if(NumOfItems)
  {
    *((int *)(&CmdMenuHeader.lgp_id))=(int)"Select command";
    patch_header(&CmdMenuHeader);
    return CreateMenu(0,0,&CmdMenuStruct,&CmdMenuHeader,0,NumOfItems,ed_gui,0);
  }
  MsgBoxError(1, (int)"FILE NOT FOUND!");
  return(0);
}
/*
char *GetStatusByIconIndex(int icon)
{
  char *stat=malloc(32);  
  switch(icon)
   {
    case IS_OFFLINE :    sprintf(stat,"Offline");      break;
    case IS_INVISIBLE :  sprintf(stat,"Invisable");    break;
    case IS_AWAY :       sprintf(stat,"Away");         break;
    case IS_NA :         sprintf(stat,"N/A");          break;
    case IS_OCCUPIED:    sprintf(stat,"Occupied");     break;
    case IS_DND :        sprintf(stat,"DND");          break;
    case IS_DEPRESSION : sprintf(stat,"Depression");   break;
    case IS_EVIL :       sprintf(stat,"Evil");         break;
    case IS_HOME :       sprintf(stat,"Home");         break;
    case IS_LUNCH :      sprintf(stat,"Lunch");        break;
    case IS_WORK :       sprintf(stat,"Work");         break;
    case IS_ONLINE :     sprintf(stat,"Online");       break;
    case IS_FFC :        sprintf(stat,"FFC");          break;
    default :            sprintf(stat,"Error status"); break;
   }
   return stat;
}
*/

char *GetStatusByIconIndex(int icon)
{
  char *stat=malloc(32);  
  switch(icon)
   {
    case IS_OFFLINE :    sprintf(stat,"Отключен");     break;
    case IS_INVISIBLE :  sprintf(stat,"Невидимый");    break;
    case IS_AWAY :       sprintf(stat,"Отсутсвует");   break;
    case IS_NA :         sprintf(stat,"Не доступен");  break;
    case IS_OCCUPIED:    sprintf(stat,"Занят");        break;
    case IS_DND :        sprintf(stat,"Не беспокоить");break;
    case IS_DEPRESSION : sprintf(stat,"Депрессия");    break;
    case IS_EVIL :       sprintf(stat,"Злой");         break;
    case IS_HOME :       sprintf(stat,"Дома");         break;
    case IS_LUNCH :      sprintf(stat,"Жру");          break;
    case IS_WORK :       sprintf(stat,"Работа");       break;
    case IS_ONLINE :     sprintf(stat,"В сети");       break;
    case IS_FFC :        sprintf(stat,"Готов болтать");break;
    default :            sprintf(stat,"В сети");       break;
   }
   return stat;
}
//----------------------------- CHECK COMMANDS -----------------------------//


int comanda;

char requests1[]="Sending online time...";
char requests2[]="Sending msg info...";
char requests3[]="Sending battery info...";
char requests4[]="Sending mod info...";
char requests6[]="Sending offline info...";
char mod_info[264];
#ifdef NEWSGOLD
  #ifdef ELKA
    #ifdef ole64//kluchnik
      #define PLATFORM "Plяtform from ОлежкО"
    #else
      #define PLATFORM "Platform: ЁLKA"
    #endif
  #else
    #define PLATFORM "Platform: NEWЫGOLD"
  #endif
#else
    #define PLATFORM "Platform: ЫGOLD"
#endif
         
#define ENA_SIGNAL 1 //для команд, активировать сигналы,али нет
#define DIS_SIGNAL 0



void SendData(CLIST *t,char *s,int signal,int count)
{
  comanda=0;    
  t->signals=signal;
  TPKT *p;
  SENDMSGCOUNT+=count;
  p=malloc(sizeof(PKT)+strlen(s)+1);
  p->pkt.uin=t->uin;
  p->pkt.type=T_SENDMSG;
  p->pkt.data_len=strlen(s);
  strcpy(p->data,s);  
  SUBPROC((void *)SendAnswer,0,p);  
}

unsigned int *errornumber;

void CheckComand(char *txt, CLIST *t)
{
  if(CurrentPrivateStatus==2) return;
  comanda=1;
#ifdef kluchnik
  sprintf(mod_info,"mod by Evilfox,kluchnik,windes based on revision 2058, mod date: Jule 13 2008 at 14:03:59");
#else
  sprintf(mod_info,"mod by Evilfox,kluchnik,windes based on revision 2058, mod date: %s at %s ",__DATE__,__TIME__);
#endif
//  sprintf(mod_info,"mod by %s based on revision 2058, mod date: Apr 21 2008 ",devel);
  t->signals=1;
  
   if(!strcmp_nocase(txt, "__PING") || !strcmp_nocase(txt, "ping") || !strcmp_nocase(txt, "Ping")|| !strcmp_nocase(txt, "Пинг")|| !strcmp_nocase(txt, "Зштп"))
   {
     char *status=malloc(256);
     TTime the;
     GetDateTime(NULL,&the);
     sprintf(status, "Автосообщение: Запрос пинга в %02d:%02d:%02d",the.hour, the.min,the.sec);
     comanda=0;
     if(show_send_info)
       AddStringToLog(t,0x01,status,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
     else
       SENDMSGCOUNT++;
     SendData(t,status,ENA_SIGNAL,0);
     mfree(status);
     return;
   }
   
  if(!strcmp(txt, "__TEXTINFO") || !strcmp(txt, "__TI"))
   {
     char *s=malloc(16384);
     char pl[256];
     *s=0;
     snprintf(pl,256,"%s model: %s sw: %s\r\n",PLATFORM,Get_Phone_Info(PI_MODEL),Get_Phone_Info(PI_SW_NUMBER));
     strcat(s,pl);
     char online[256];
     sprintf(online,"Время в онлайн: %02d:%02d:%02d \r\n"
                    "Сервер: %s \r\n%s \r\n", uptimeHour, uptimeMin, uptimeSec, hostbuf2,PINGO);
     strcat(s,online);
     char *send_txt=malloc(64);
     sprintf(send_txt, "Отправлено сообщений: %d \r\n"
                       "Принято сообщений: %d \r\n", main2_count, main_count);
     strcat(s,send_txt);
    
     WSHDR *ws=AllocWS(64);
     GetProvider(ws,0);
     char prov[16];
     ws_2str(ws,prov,wstrlen(ws));
     sprintf(send_txt, "Оператор: %s \r\n", prov);
     FreeWS(ws);
     strcat(s,send_txt);
    
     int bat;
     bat=*RamCap();
     sprintf(send_txt, "Заряд батареи: %d процентов \r\n", bat);
     strcat(s,send_txt);
      
     char *status=malloc(256);
     TTime the;
     GetDateTime(NULL,&the);
     void Get_Online_total();
     Get_Online_total();
     unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;	//sendq_l	
     sprintf(status, "Traffic: Rx : %db Tx : %db Queue: %db \r\nTime: %02d:%02d\r\n"
                     "Online contact: %d [total: %d] \r\nUnread message: %d/%d ",
                                                          RX,TX,sendq_l,
                                                          the.hour, the.min,
                                                          kopa_kontaktu, kopa_online,
                                                          total_unread,total_unread2);
     strcat(s,status);
     comanda=0;
     if(show_send_info)
       AddStringToLog(t,0x01,"Sending textinfo...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
     else
       SENDMSGCOUNT++;
     SendData(t,s,ENA_SIGNAL,0);
     mfree(s);
     mfree(send_txt);
     mfree(status);
     return;
   }
  
  if(!strcmp(txt, "__TIME") || !strcmp(txt, "__T") || !strcmp(txt, "GET_TIME"))
   {
     char *status=malloc(256);
     TTime the;
     GetDateTime(NULL,&the);
     sprintf(status, "Time: %02d:%02d",the.hour, the.min);
     comanda=0;
     if(show_send_info)
       AddStringToLog(t,0x01,"Sending time...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
     else
       SENDMSGCOUNT++;
     SendData(t,status,ENA_SIGNAL,0);
     mfree(status);
     return;
   }
  
  if(!strcmp(txt, "GET_PLATFORM_INFO") || !strcmp(txt, "__PLATFORM") || !strcmp(txt,"__HW"))
  {
    comanda=0;
    TPKT *p;
    char pl[256];
    snprintf(pl,256,"%s model: %s sw: %s",PLATFORM,Get_Phone_Info(PI_MODEL),Get_Phone_Info(PI_SW_NUMBER));
    p=malloc(sizeof(PKT)+strlen(pl)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(pl);
    strcpy(p->data,pl);
    if(show_send_info)
      AddStringToLog(t,0x01,"Sending platform info...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
  
  if(!strcmp(txt, "GET_ONLINE_TIME") || !strcmp(txt, "__ONLINE"))
  {
    comanda=0;
    TPKT *p;
    char online[256];
    sprintf(online,"Время в онлайн: %02d:%02d:%02d \n"
                   "Сервер: %s", uptimeHour, uptimeMin, uptimeSec, hostbuf2);
    p=malloc(sizeof(PKT)+strlen(online)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(online);
    strcpy(p->data,online);
    if(show_send_info)
      AddStringToLog(t,0x01,requests1,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
  
  if(!strcmp(txt, "GET_MSG_INFO") || !strcmp(txt, "__MSG"))
  {
    comanda=0;
    TPKT *p;
    char *send_txt=malloc(64);
    sprintf(send_txt, "Отправлено сообщений: %d \n"
                      "Принято сообщений: %d"
                    , main2_count, main_count);
    p=malloc(sizeof(PKT)+strlen(send_txt)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
      AddStringToLog(t,0x01,requests2,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }

//==========================windes commands begin =====================

  if(!strcmp(txt, "GET_PROVIDER") || !strcmp(txt, "__PROVIDER"))
  {
    comanda=0;
    TPKT *p;
    
    WSHDR *ws=AllocWS(32);
    GetProvider(ws,0);
    char prov[16];
    ws_2str(ws,prov,wstrlen(ws));
    char *send_txt=malloc(32);
    sprintf(send_txt, "Оператор: %s", prov);
    FreeWS(ws);
    
    p=malloc(sizeof(PKT)+28+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
      AddStringToLog(t,0x01,"Sending provider name...",I_str,(++SENDMSGCOUNT)&0x7FFF,1); 
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }

  
//-----------------------------------------------------------------------
  
  
  if(!strcmp(txt, "GET_MOD_INFO") || !strcmp(txt, "__MOD"))
  {
    comanda=0;
    TPKT *p;
    char *send_txt=malloc(256);
//    sprintf(I_UIN, "uin%d", t->uin);
    strcpy(I_UIN, t->name);
    if(strstr(ignor_list,I_UIN)==0)
    {
      strcpy(send_txt, mod_info);
    }
    else
      strcpy(send_txt, "***Access denied***");

    strcpy(send_txt, mod_info);

    p=malloc(sizeof(PKT)+strlen(send_txt)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
#ifdef EVILFOX
      SENDMSGCOUNT++;
#else
    if(show_send_info)
       AddStringToLog(t,0x01,requests4,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
       SENDMSGCOUNT++;
#endif
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }
  
  if(!strcmp(txt, "GET_BAT_INFO") || !strcmp(txt, "__BAT"))
  {
    comanda=0;
    TPKT *p;
    int bat;
    bat=*RamCap();
    char *send_txt=malloc(64);
    sprintf(send_txt, "Заряд батареи: %d процентов", bat);
    p=malloc(sizeof(PKT)+28+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
#ifdef EVILFOX
    SENDMSGCOUNT++;
#else
    if(show_send_info)
       AddStringToLog(t,0x01,requests3,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
       SENDMSGCOUNT++;
#endif
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }
  
  if((away_m==1 || t->isautomsg) && (t->automsgcount < automsgcount))
  {
    t->automsgcount++;
    comanda=1;
    TPKT *p;
//    sprintf(hah,"online: %02d:%02d:%02d", uptimeHour, uptimeMin, uptimeSec);
    sprintf(away_msg, "Автоответчик: %s %s! %s %s", awayHelloMsg, t->name, awayMsg, away_msg_time);
    p=malloc(sizeof(PKT)+strlen(away_msg)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(away_msg);
    strcpy(p->data,away_msg);
    if(show_send_info)
    {
#ifdef kluchnik
      AddStringToLog(t,0x01,away_msg,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
#else
      AddStringToLog(t,0x01,requests6,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
#endif
    }
    else    
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
/*  else
    if((away_m==1 || t->isautomsg) && (t->automsgcount<=(automsgcount+3)) )
    {
      t->awaysignal=0;
      t->automsgcount++;
      sprintf(away_msg,"Бля,ну русским ж языком написали,НЕТУ МЕНЯ!!!че по сто сообщений слать?тем более если тебе делать нех...");
      if(show_send_info)
        AddStringToLog(t,0x01,away_msg,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
      else
        SENDMSGCOUNT++;
      SendData(t,away_msg,ENA_SIGNAL,0);
      return;    
    }*/
  else
    if(away_m==1 || t->isautomsg)
    {
      t->automsgcount=automsgcount;
      return;
    }
  
  
//---------------------------------------------------------------------------//
  
  
  if(!strcmp(txt, "GET_URL_INFO") || !strcmp(txt, "__URL"))// by kluchnik 26.03.08
  {
    comanda=0;
    TPKT *p;
    
    char *send_txt=malloc(256);
    sprintf(send_txt,"revision 2058, mod date: %s at %s ",
                     "http://kluchnik.jino-net.ru/naticqmod/",__DATE__,__TIME__);
    
    p=malloc(sizeof(PKT)+256);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
    if(show_send_info)
      AddStringToLog(t,0x01,"Sending url info...",I_str,(++SENDMSGCOUNT)&0x7FFF,1); 
    else
      SENDMSGCOUNT++;
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }
  
#ifdef kluchnik // не доступно для всех,потому что имхо,будет еще большая перегрузка серверов!!!а оно нам надо?
  if(t->isautobot)
  {
    TPKT *p;
    int i=LoadBotMessage(t->uin);
    if(i==0) return;//если нет никаких файлов то просто возвращаемся
    char *pp=bot_lines[t->bot_msg];
    t->bot_msg++;
    if(t->bot_msg==i)
      t->bot_msg=0;
    p=malloc(sizeof(PKT)+strlen(pp)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(pp);
    strcpy(p->data,pp);
    AddStringToLog(t,0x01,pp,"Autobot",(++SENDMSGCOUNT)&0x7FFF,1);
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
#endif
 
 CheckComand2(txt,t);
}

//-------------  Попап месаже для подключившегося контакта -----------//
extern DYNPNGICONLIST *GetXSByItem(int curitem);
char INC_Nick[32];
char show_nick[32];
int INC_show;

char onContact[64];
int INC_show2;
GBSTMR incs2;
extern void DrawScreenShot();
int pp_icon;
int pp_xicon;
void show_on_contact()
{
  if(INC_show2>0/* && !(IsGuiOnTop(maingui_id))*/)
  {
    WSHDR *ws=AllocWS(64);
    ascii2ws(ws,onContact);
    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if(!(memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))
    {
     DrawScreenShot();
     INC_show2-=2;
    }
    else
      INC_show2--;

    int x=0;
    if(icons[pp_icon])
      x=icons[pp_icon]->w;

    if(pp_xicon!=0 && XStatusImg[pp_xicon])
      x+=XStatusImg[pp_xicon]->w;
    
    DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+x+5,GetFontYSIZE(PopupFont)+5,0,PopupOnlineCl,PopupBGCl);
    
    if(icons[pp_icon]) 
    {
      DrwImage(icons[pp_icon],2,3);
      DrwImage(XStatusImg[pp_xicon],4+icons[pp_icon]->w,3);
    }
    
    DrawString(ws,x+3,2, 3+Get_WS_width(ws,PopupFont)+x ,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupOnlineCl,GetPaletteAdrByColorIndex(23));
    FreeWS(ws);
    GBS_StartTimerProc(&incs2, 15, show_on_contact);
  }
}

char OffContact[64];
int INC_show3;
GBSTMR incs3;


void show_off_contact()
{
  if(!isPopupOff) return;
  if(INC_show3>0)
  {
    WSHDR *ws=AllocWS(64);
    ascii2ws(ws,OffContact);
    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if(!(memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))
    {
     DrawScreenShot();
     INC_show3-=2;
    }
    else
      INC_show3--;
   
    DrawRectangle(ScreenW()-Get_WS_width(ws,PopupFont)-4-icons[IS_OFFLINE]->w,0,ScreenW()-1,GetFontYSIZE(PopupFont)+4,0,PopupOffCl,PopupOffBGCl);

    DrwImage(icons[IS_OFFLINE],ScreenW()-Get_WS_width(ws,PopupFont)-2-icons[IS_OFFLINE]->w,3);

    DrawString(ws,ScreenW()-Get_WS_width(ws,PopupFont)-3,2,ScreenW()-1,GetFontYSIZE(PopupFont)+2,PopupFont,TEXT_ALIGNRIGHT,PopupOffCl,GetPaletteAdrByColorIndex(23));
    FreeWS(ws);
    GBS_StartTimerProc(&incs3, 15, show_off_contact);
  }
}

//---------------------------- popup message ---------------------------//


void show_pp_sender()
{
  if(INC_show>0/* && !(IsGuiOnTop(maingui_id))*/)
  {
    WSHDR *ws=AllocWS(256);
    ascii2ws(ws,show_nick);
    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if(!(memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))
    {
     DrawScreenShot();
     INC_show-=2;
    }
    else
      INC_show--;
#ifdef NEWSGOLD
    DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+6+GetImgWidth(NumStatusToIconBar + IS_MSG),GetFontYSIZE(PopupFont)+4,0,PopupMsgCl,PopupBGCl);
    DrawImg(2,2,NumStatusToIconBar + IS_MSG);//GetImgWidth((int)S_ICONS[icn]
    DrawString(ws,GetImgWidth(NumStatusToIconBar + IS_MSG)+4,1,GetImgWidth(NumStatusToIconBar + IS_MSG)+Get_WS_width(ws,PopupFont)+4,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupMsgCl,GetPaletteAdrByColorIndex(23));
#else
    DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+2,GetFontYSIZE(PopupFont)+2,0,PopupBGCl,PopupBGCl);
    DrawString(ws,1,1,1+Get_WS_width(ws,PopupFont)+2,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupMsgCl,GetPaletteAdrByColorIndex(23));
#endif    
    FreeWS(ws);
    GBS_StartTimerProc(&develop, 10, show_pp_sender);
  }
}


//----------------------------------------------------------------------------------------//


void Get_Online_total()
{
  CLIST *t;
  kopa_online=0;
  kopa_kontaktu=0;
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (!t->isgroup)
    {
      kopa_online++;
      if (t->state!=65535) kopa_kontaktu++;
    }
  }
}


//-------------------------------------------------------------------------------------------//


void SetIndividStatus()//set to all contact to default vibra, sound and other by kluchnik 04.04.08
{
  CLIST *t;
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (!t->isgroup)
    {
      t->isvibra=1;
      t->issound=1;
      t->isShowXst=Show_XTX;
#ifdef kluchnik
      t->isLoadHistory=1;
#endif
      t->signals=1;
      t->isaccess=1;
      
      t->automsgcount=0;
    }
  }
}

//-------------------------------------------------------------------------------------------//
//int ShowLoadScreen=1;
int curpos;

ProcessPacket(TPKT *p)
{
  CLIST *t;
  LOGQ *q;
  char s[256];
  switch(p->pkt.type)
  {
  case T_LOGIN:
    set_my_status();
    set_my_xstatus();
    break;
  case T_CLENTRY:
    if (p->pkt.uin)
    {
      if ((t=FindContactByUin(p->pkt.uin)))
      {
	//        t->state=0xFFFF;
        if (!t->local) strncpy(t->name,p->data,63);
	t->group=GROUP_CACHE;
	ChangeContactPos(t);
	RecountMenu(t,1);
      }
      else
      {
        RecountMenu(AddContact(p->pkt.uin,p->data, GROUP_CACHE, 0),1);
      }
    }
    else
    {
      if(VIBR_ON_CONNECT)
      {
        vibra_count=1;
        start_vibra();
      }
      GROUP_CACHE=0;
      ask_my_info();
      if (ShowLoadScreen==0 && maingui_id/*contactlist_menu_id*/)
      {
	RecountMenu(NULL,1);
      }
      else
      {
        SetIndividStatus();
        if(ShowLoadScreen) ShowLoadScreen=0;        
        SMART_REDRAW();  
      }
    }
    break;
  case T_GROUPID:
    if (t=FindGroupByID(GROUP_CACHE=p->pkt.uin))
    {
      strncpy(t->name,p->data,63);
      ChangeContactPos(t);
      RecountMenu(t,1);
    }
    else
    {
      RecountMenu(AddGroup(p->pkt.uin,p->data),1);
    }
    break;
  case T_GROUPFOLLOW:
    GROUP_CACHE=p->pkt.uin;
    break;
  case T_STATUSCHANGE:
    t=FindContactByUin(p->pkt.uin);
    if (t)
    {
      int i=t->state;
//      CLIST *oldt=NULL;
      if (t->xstate!=p->data[2])  // Если картинка икс статуса сменилась
      {
        t->xstate=p->data[2];
        FreeXText(t);
        if (t->xstate && ENA_AUTO_XTXT)   // Если установлен икс статус и можно запрашивать по смене икс статуса :)
        {
          t->req_xtext=1;
          if (edchat_id)   // Если открыт чат
          {
            void *data=FindGUIbyId(edchat_id,NULL);
            if (data)
            {
              EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
              if (ed_struct)
              {
                if (ed_struct->ed_contact==t)   // Если наш чат то запрашиваем текст статуса
                {
                  t->req_xtext=0;
                  RequestXText(t->uin);
                }
              }
	    }
	  }
	}
        else  // Если икс статус не установлен то на всякий случай снимаем флаг получения текста
        {
          t->req_xtext=0;
        }
      }/*
      if (contactlist_menu_id)
      {
//	oldt=FindContactByN(GetCurMenuItem(FindGUIbyId(contactlist_menu_id,NULL)));
        oldt=FindContactByN(curpos);
      }*/
      if(t->state==65535) t->change=1;
      t->state=*((unsigned short *)(p->data));
      if((t->state!=65535)&&(t->change==1))
      {
        t->just_now=2;
        t->change=0;
        RefreshJustNow();
      }
      
      LogStatusChange(t);
      LogStatusChange2(t);//пишем лог templates\\logs.txt
      
      ChangeContactPos(t);
//      RecountMenu(oldt,1);
     
      if(isLogStatusChange)//isLogStatusChange //   by kluchnik 05.04.08
      {
       char *s=GetStatusByIconIndex(GetIconIndex(t));         
       FreeXText2(t);
       TTime time;
       GetDateTime(NULL,&time);
       char *txt=malloc(64);
       sprintf(txt," %s: %02d:%02d:%02d",s,time.hour,time.min,time.sec);      
       t->xtext2=malloc(64);
       snprintf(t->xtext2,64,txt);
       AddStringToLog(t,0x03,txt,"Status change",0xFFFFFFFF, 0);
       ReqAddMsgToChat(t);  
       RecountMenu(NULL, 0);
       mfree(txt);
      }
     
      if ((t->state!=0xFFFF)&&(i==0xFFFF))//Звук
      {
       if(Is_INC_On && (PopupShow==2 || PopupShow==3))
        {
          INC_show2=20;
          INC_show3=0;

          pp_icon=GetIconIndex(t);
          pp_xicon=t->xstate;

          sprintf(onContact,PopupOnlineString,t->name);
          sprintf(on_off_buf,"[%s: %s]\n",t->xtext2+1,t->name);
          on_off_timer=ON_OFF_TIME;
//          UpdateCLheader();
          show_on_contact();
          if(IllByONCantact)
              IlluminationOn(ILL_DISP_SEND,0,ILL_SEND_TMR,ILL_RECV_FADE);
  //          IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE);
        }
        else
          GBS_DelTimer(&incs2);//на всяк 
        if(!extended_sounds)
        {
          Play(sndGlobal);
        }
        else
        {
          FSTATS stat;
          int fsize;
          unsigned int ul = 0;
          uins=t->uin;
           char ext[4];
            // __root const CFG_CBOX_ITEM cfgcbox18_3_2[2]={"Mp3","Wav","Amr"};
            switch(ext_type_sounds)
            {
              case 0: sprintf(ext,percent_s,mp3); break;
              case 1: sprintf(ext,percent_s,wav); break;
              case 2: sprintf(ext,percent_s,amr); break;              
              default : sprintf(ext,percent_s,mp3); break;
            }
          sprintf(talk_sound, sds, sndGlobal2, uins,ext);
          GetFileStats(talk_sound,&stat,&ul);
          fsize=stat.size;
          if(fsize>0)
            Play(talk_sound);
          else
            Play(sndGlobal);       
          t->isvibra=1;
          t->issound=1;
          t->isShowXst=Show_XTX;
#ifdef kluchnik
          t->isLoadHistory=1;
#endif
          t->signals=1;
        }
      }
      
      if ((i!=0xFFFF)&&(t->state==0xFFFF))//Звук
      {
	Play(sndSrvMsg);   
        
        INC_show3=20;
        sprintf(OffContact,PopupOnlineString,t->name);
        show_off_contact();

        //if(TIME_OR_MSG==2 || TIME_OR_MSG==3 | TIME_OR_MSG==4)
          Get_Online_total();
       
        sprintf(on_off_buf,"[%s: %s]\n",t->xtext2+1,t->name);
        on_off_timer=ON_OFF_TIME;
//        UpdateCLheader();
      }
      
    if (edchat_id)
    {
      void *data=FindGUIbyId(edchat_id,NULL);
      {
	EDCHAT_STRUCT *ed_struct;
	ed_struct=EDIT_GetUserPointer(data);
	if (ed_struct) 
	{
	  if (ed_struct->ed_contact==t)
	  {
	    DirectRedrawGUI_ID(edchat_id);
	  }
	}
      }
    }
    RefreshCL();
    }
    break;
  case T_RECVMSG:
    NEW_MSG=1;
    t=FindContactByUin(p->pkt.uin);
    if (!t)//если не в списке
    {
      sprintf(s,percent_d,p->pkt.uin);
      t=AddContact(p->pkt.uin,s,GROUP_CACHE,0);
      CreateCLFile();//добавляем в local.cl....
    }
#ifdef kluchnik
    if(!t->isactive && HISTORY_BUFFER && (t->isLoadHistory && t->isIndividSet)) GetHistory(t, 64<<HISTORY_BUFFER);
    else
      if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
#else
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
#endif
    if (t->name[0]=='#')
    {
      //Если это конференция, патчим имя
      char *s=strchr(p->data,'>');
      //Если нашли символ > и после него пробел и ник короче 16 символов
      if (s)
      {
	if ((s[1]==' ')&&((s-p->data)<16))
	{
	  *s=0; //Режем строку
	  AddStringToLog(t,0x02,s+2,p->data,0xFFFFFFFF,1); //Добавляем имя из текста сообщения
	  goto L1;
	}
      }
    }
    
//=================== tas bus jautri ===================//

    comanda=1;
#ifdef DEBUG1
    comanda=1;
    
    if(SHOW_ACTIVE)
        t->isactive=ACTIVE_TIME;
    if(VIBR_TYPE)
        vibra_count=2;
    else
        vibra_count=1;

    start_vibra();

    #ifdef ELKA
        if(SLI_TYPE)  SLI_Timer();
    #endif
         if(Is_Double_Vibra_On)
         {
           Reminder_timer();
         }
         
         if (Is_ILU_On)
           IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
#else//DEBUG         
    CheckComand(p->data, t);
    if(t->signals && t->isaccess)
    {
#ifdef NEWSGOLD1
      scrsTime=0;
#endif
      if(SHOW_ACTIVE)
        t->isactive=ACTIVE_TIME;
      if(t->isvibra)
       {
        if(VIBR_TYPE)
           vibra_count=2;
        else
            vibra_count=1;
        if(t->automsgcount<automsgcount)
            start_vibra();
       }
    #ifdef ELKA
        if(SLI_TYPE)
           SLI_Timer();
    #endif
         if(Is_Double_Vibra_On)
         {
           Reminder_timer();
         }
         
         if (Is_ILU_On)
           IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
    }
    else
      if(t->isIndividSet)
      {
#ifdef NEWSGOLD1
        scrsTime=0;
#endif
        if(SHOW_ACTIVE)
          t->isactive=ACTIVE_TIME;
        if(t->isvibra)
         {
          if(VIBR_TYPE)
             vibra_count=2;
          else
              vibra_count=1;
          if(t->automsgcount < automsgcount && away_m)
             start_vibra();
          else
             start_vibra();
         }
      #ifdef ELKA
          if(SLI_TYPE)
             SLI_Timer();
      #endif
           if(Is_Double_Vibra_On)
           {
             Reminder_timer();
           }
           
           if (Is_ILU_On)
             IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
      }
    else
      {
#ifdef NEWSGOLD1
        scrsTime=0;
#endif
        if(SHOW_ACTIVE)
          t->isactive=ACTIVE_TIME;
        if(VIBR_TYPE)
           vibra_count=2;
        else
           vibra_count=1;
        if(t->automsgcount >= automsgcount) vibra_count=0;
           start_vibra();
     #ifdef ELKA
        if(SLI_TYPE)
            SLI_Timer();
     #endif
        if(Is_Double_Vibra_On)
          {
            Reminder_timer();
          }
           
        if (Is_ILU_On)
             IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
        }
    
#endif//DEBUG
    if(comanda==1)
    {
      AddStringToLog(t,0x02,p->data,t->name,0xFFFFFFFF,1);
      main_count++;
      if(Is_INC_On && (PopupShow==1 || PopupShow==3))
      {
        sprintf(show_nick,PopupMsgString,t->name);
        if(!IsIdleUiOnTop()&&(!saver_on))
        {
          INC_show=20;
          show_pp_sender();
        }
      }
      else
        GBS_DelTimer(&develop);
#ifndef DEBUG
      main_msg_timer2=main_msg_timer;//при получении сообщения восстанавливаем таймер
      if(Is_SMARTCPU_On)
        AddToMain(t->name,p->data);
#endif
    }
    L1:
    //Разворачиваем группу, в которой пришло сообщение
    {
      CLIST *g=FindGroupByID(t->group);
      if (g)
      {
	if (g->state)
	{
	  g->state=0;
	}
      }
    }

    ReqAddMsgToChat(t);
    RecountMenu(t,1);
   
    switch (DEVELOP_IF)
    {
    case 0:
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)) to_develop();
      break;
    case 1:
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)&&(IsUnlocked())) to_develop();
      break;
    case 2:
      break;
    case 3:
      GBS_DelTimer(&develop);
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)&&(!IsUnlocked())) begin_to_develop();
      break;
    }

    RefreshCL();
    break;
  case T_SRV_ACK:
  case T_CLIENT_ACK:
    q=FindContactLOGQByAck(p);
    if (q)
    {
      q->acked=p->pkt.type==T_SRV_ACK?1:2;
      t=FindContactByUin(p->pkt.uin);
      if (edchat_id)
      {
	void *data=FindGUIbyId(edchat_id,NULL);
	if (data)
	{
	  EDCHAT_STRUCT *ed_struct;
	  ed_struct=EDIT_GetUserPointer(data);
	  if (ed_struct) 
	  {
	    if (ed_struct->ed_contact==t)
	    {
	      if (EDIT_IsBusy(data))
	      {
		t->req_drawack=1;
                time_to_stop_t9=3;
	      }
	      else
              {
		DrawAck(data);
                if (p->pkt.type==T_SRV_ACK)
                {
                  SMART_REDRAW();
                  RefreshCL();
                  IlluminationOn(ILL_DISP_SEND,ILL_KEYS_SEND,ILL_SEND_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
                }
              }
	    }
	  }
	}
      }
    }
    break;
  case T_XTEXT_ACK:
    t=FindContactByUin(p->pkt.uin);
    if (t)
    {
      int i;
      int j;
//      char ss[256];
//      TTime tm;
      FreeXText(t);
      i=p->pkt.data_len;
      memcpy(t->xtext=malloc(i),p->data,i);
      t->xtext[i]=0;
//      GetDateTime(0,&tm);
  //    snprintf(ss,i+4,"%02d:%02d:%02d %s",tm.hour,tm.min,tm.sec,p->data);
    //  memcpy(t->xtext=malloc(strlen(ss)),ss,strlen(ss));      
      
      zeromem(s,256);      
      strcpy(s,t->name);
      strcat(s,":\n");
      i=strlen(s);
      j=p->data[0];
      if (j>(255-i)) j=255-i;
      strncpy(s+i,p->data+1,j);
      i+=j;
      if (i<255)
      {
	s[i]='\n';
	i++;
	j=p->pkt.data_len-p->data[0]-1;
	if (j>(255-i)) j=255-i;
	strncpy(s+i,p->data+p->data[0]+1,j);
      }

      zeromem(s,256);
      i=0;
      j=p->data[0];
      if (j>(255-i)) j=255-i;
      strncpy(s,p->data+1,j);
      i+=j;
      if (i<255)
      {
	s[i++]=13;
	j=p->pkt.data_len-p->data[0]-1;
	if (j>(255-i)) j=255-i;
	strncpy(s+i,p->data+p->data[0]+1,j);
      }

//      GetDateTime(0,&tm);
  //    sprintf(ss,"%02d:%02d:%02d %s",tm.hour,tm.min,tm.sec,s);
      AddStringToLog(t,0x03,s,x_status_change,0xFFFFFFFF,1);
      ReqAddMsgToChat(t);
      if (strlen(p->data))       // Если x-status не пустой
         RecountMenu(t, 1);
      else
         RecountMenu(NULL, 0);     
    }
    break;
  case T_LASTPRIVACY:
    CurrentPrivateStatus=p->data[0];
    break;
  }
  mfree(p);
}


IPC_REQ tmr_gipc;
void process_active_timer(void)
{
  if (connect_state>2)
  {
    if (++tenseconds_to_ping>12)
    {
      tenseconds_to_ping=0;
      SUBPROC((void *)do_ping);
    }
  }
  tmr_gipc.name_to=ipc_my_name;
  tmr_gipc.name_from=ipc_my_name;
  tmr_gipc.data=NULL;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_TENSECONDS,&tmr_gipc);  
  GBS_StartTimerProc(&tmr_active,TMR_SECOND*10,process_active_timer);
}

GBSTMR tm22;
void OneSecTmr()
{
  tmr_gipc.name_to=ipc_my_name;
  tmr_gipc.name_from=ipc_my_name;
  tmr_gipc.data=NULL;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_ONESECOND,&tmr_gipc);  
  GBS_StartTimerProc(&tm22,TMR_SECOND,OneSecTmr);
}
  

//===============================================================================================
/*
extern ADDUIN addonuin[];

int itemFromDisp;//кол-во на экране
int totalItems;//общее количество итемов
int curpos=0;//позичия курсора
int curpage=1;//текущая страница

const char clrSelect[4]={0xFF,0x00,0x00,0x60};
const char fntSelect[4]={0xFF,0xFF,0x00,0x60};
const char bordSelect[4]={0x00,0x00,0x00,0x60};

const char clrNotSelect[4]={0x00,0x00,0x00,0x60};
const char bordNotSelect[4]={0x00,0x00,0x00,0x60};
const char fntNotSelect[4]={0xFF,0xFF,0xFF,0x60};

char clrBlack[]={0x00,0x00,0x00,0x64};
char clrWhite[]={0xFF,0xFF,0xFF,0x64};
//char prozr[]={0x00,0x00,0x00,0x00};
const int font=FONT_SMALL;
const char _t[]="%t";

extern int ParseUINS();
int USE_ADD_UIN=0;
#define CLIST_Y1 (YDISP+GetFontYSIZE(font)+8)

static void DrawList()
{
  int font_y = GetFontYSIZE(font)+2;//16+2
  int scr_w=ScreenW();
  int scr_h=ScreenH()-1;
//  int y2=scr_w-(GetFontYSIZE(font)+2);
#ifdef ELKA
  itemFromDisp = sdiv(font_y,ScreenH()-CLIST_Y1-GetFontYSIZE(font)-2)-1;
#else
  itemFromDisp = sdiv(font_y,ScreenH()-CLIST_Y1-GetFontYSIZE(font)-2);
#endif
  
  LockSched();
  totalItems = ParseUINS(); 
  // Определяем количество страниц списка контактов
  int pageNum = sdiv(itemFromDisp, totalItems);  
  if( itemFromDisp*pageNum < totalItems) pageNum++; //На всяк,вдруг еще страница нужна
  WSHDR* out_ws = AllocWS(256);
  int i=0;
  int start_y;
  int x=1;
  while(i<=totalItems)
  {
     if((i>=(curpage-1)*itemFromDisp) && (i<=itemFromDisp*curpage))//если те итемы который нужно выводить на экран со страници curpage
        {          
          wsprintf(out_ws,"%d. %d",i+1,addonuin[i].uin);//имя закладки          
          start_y = (CLIST_Y1 + (i - (curpage-1)*itemFromDisp)*font_y);    //определяем начальное смещение по у      
          
          if(i==curpos)
          {
            DrawRoundedFrame(x-4,start_y,scr_w-1,start_y+font_y,5,5,0,bordSelect,clrSelect);
            DrawString(out_ws,x,start_y+2,scr_w-1,start_y+font_y,font,0,fntSelect,0);
          }
          else
          {
            DrawRoundedFrame(x,start_y+1,scr_w-1,start_y+font_y,0,0,0,bordNotSelect,clrNotSelect);//clrBlack
            DrawString(out_ws,x,start_y+2,scr_w-1,start_y+font_y,font,0,fntNotSelect,0);          
          }            
        }
      i++;
      if(i>(pageNum*itemFromDisp)) break;
  }
  UnlockSched();  
  FreeWS(out_ws);
}

// Управление курсором
void MoveUp()
{
  if(!itemFromDisp)return;
  if(curpos<1)
  {
    curpos=totalItems;    
    if(itemFromDisp!=totalItems)
    {
      curpage = sdiv(itemFromDisp,totalItems)+1;
    }
    else
      curpage = 1;
  }
  else
  {
    curpos--;
    if(curpos<(curpage-1)*itemFromDisp){ curpos++; curpage--; }
  }
 // REDRAW();
}


void MoveDown()
{
  if(!totalItems)return;
  curpos++;  
  if(curpos>totalItems) { curpos=0; curpage=1; }
  if(curpage*itemFromDisp<curpos){ curpos--; curpage++; } 
 // REDRAW();
}

void UpdateCSMname(void);
void Enter()
{
  UIN=addonuin[curpos].uin;
  snprintf(PASS,9,percent_s,addonuin[curpos].pass);
  USE_ADD_UIN=0;

  UpdateCSMname();
  ReadDefSettings();
  
  sprintf((char *)ipc_my_name+6,percent_d,UIN);

SUBPROC((void*)end_socket);
  GBS_DelTimer(&reconnect_tmr);
  DNR_TRIES=3;
  SUBPROC((void *)create_connect);
  
  REDRAW();
}
*/


/************************************************************************************************/

/*------------------------------------------------------------------------*/
/*-------------------- Весь процесс отрисовки списка ---------------------*/
/*------------------------------------------------------------------------*/
//--------------------  MANAGE CONTACT MENU --------------------------//


#ifdef kluchnik
  #define _MAX 10
#else
  #define _MAX 6
#endif

int uinContact;

void RqtXText(GUI *data)
{
  void RequestXText(unsigned int uin);
  RequestXText(uinContact);
  CLIST *t=FindContactByUin(uinContact);
  t->isIndividSet=0;
  REDRAW();
}

void ChangeVibraContact(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isvibra=!(t->isvibra);
//  RefreshGUI();
    REDRAW();
}

void ChangeSoundContact(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->issound=!(t->issound);
//  RefreshGUI();
    REDRAW();
}

void ChangeAutoMsg(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isautomsg=!(t->isautomsg);
//  RefreshGUI();
    REDRAW();
}

void ChangeShowXstatus(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isShowXst=!(t->isShowXst);
//  RefreshGUI();
    REDRAW();
}

#ifdef kluchnik

void ChangeAccess(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isaccess=!(t->isaccess);
///  RefreshGUI();
    REDRAW();
}


void ChangeAwaySignal(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->automsgcount=automsgcount;
//  RefreshGUI();
    REDRAW();
}

void ChangeAutobot(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isautobot=!(t->isautobot);
//  RefreshGUI();
    REDRAW();
}

void ChangeLoadHistory(GUI *data)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
  t->isLoadHistory=!(t->isLoadHistory);
//  RefreshGUI();
    REDRAW();
}
#endif  //kluchnik

const void *menuproc[_MAX]=
{ 
  (void *)RqtXText,
  (void *)ChangeVibraContact,
  (void *)ChangeSoundContact,
  (void *)ChangeAutoMsg,
  (void *)ChangeShowXstatus,
#ifdef kluchnik
  (void *)ChangeAccess,
  (void *)ChangeAwaySignal,
  (void *)ChangeLoadHistory,
  (void *)ChangeAutobot,
#endif
};

const char * const menutext[_MAX]=
{
  "Запрос хстатуса",  
  "Вибра",
  "Звук",
  "Автоответчик",
  "Показ.хстатус",
#ifdef kluchnik
  "Удаленное управление",
  "Синалы автоответчика",
  "Загруж.историю",
  "Aвтобот"
#endif
};

int hy;
const char clrSelect[4]={0xFF,0x00,0x00,0x20};
const char fntSelect[4]={0xFF,0xFF,0x00,0x60};
const char bordSelect[4]={0x00,0x00,0x00,0x60};

const char clrNotSelect[4]={0x00,0x00,0x00,0x00};
const char bordNotSelect[4]={0x00,0x00,0x00,0x00};
const char fntNotSelect[4]={0xFF,0xFF,0xFF,0x60};

char clrBlack[]={0x00,0x00,0x00,0x64};
char clrWhite[]={0xFF,0xFF,0xFF,0x64};
char clrRed[]={0xFF,0x00,0x00,0x64};
char transparent[]={0x00,0x00,0x00,0x00};
char blue[]="\x00\x00\xFF\x64";

//const int clfont=(FONT_SMALL+100);//ибо библа пнг 
int clfont;

//char clrRed2[]={0xFF,0x00,0x00,0x15};
int i_itemFromDisp;//кол-во на экране
int i_totalItems;//общее количество итемов
int i_curpos=1;//позичия курсора

GBSTMR i_tmr;

extern IMGHDR screen;
void DrawCheckBox(int x,int y,int flag,int type);
extern char *menutexts2[];

RECT i_rc;

int DrawIconInfo(int x, int y, int curitem)
{
  CLIST *t;
  t=FindContactByUin(uinContact);
     
  switch(curitem)
  {
    case 1: DrawCheckBox(x,y,1,1);/*DrwImage(icons[ICON_PING],x,y,0,0);*/ break;//    return icons[ICON_PING]->w;
    case 2: DrawCheckBox(x,y,t->isvibra?1:0,0);       break;
    case 3: DrawCheckBox(x,y,t->issound?1:0,0);       break;
    case 4: DrawCheckBox(x,y,t->isautomsg?1:0,0);     break;
    case 5: DrawCheckBox(x,y,t->isShowXst?1:0,0);     break;
#ifdef kluchnik
    case 6: DrawCheckBox(x,y,t->isaccess?1:0,0);      break;
    case 7: DrawCheckBox(x,y,t->automsgcount?1:0,0);  break;
    case 8: DrawCheckBox(x,y,t->isLoadHistory?1:0,0); break;
    case 9: DrawCheckBox(x,y,t->isautobot?0:1,0);     break;
#endif
  }
  return 12;
}

extern DYNPNGICONLIST *GetXSByItem(int curitem);
char *GetStatusByIconIndex(int icon);

char ixtext[256];
void I_DrawList()
{
  int font_y = FontH(clfont)+1;
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
 
  LockSched();  
  int i=1;
  int start_y=0;
  int start_x=i_rc.x+4;
 
  RECT rc;
  CLIST *t;
  t=FindContactByUin(uinContact);
  SetFontColor(clrWhite,transparent,0);           
  char *s=malloc(128);
  sprintf(s,"Меню контакта \"%s\"",t->name);
  PrintText(i_rc.x+3,i_rc.y+1,s,0,clfont,1,0,0,0,i_rc.x2-i_rc.x);//item name  

  DrawLine(i_rc.x,i_rc.y+font_y,i_rc.x2,i_rc.y+font_y,0,clrWhite);
  int back=i_rc.y;
  i_rc.y+=4;
  
  DrwImage(icons[GetIconIndex(t)],i_rc.x+2,i_rc.y+font_y);
  PrintText(i_rc.x+icons[GetIconIndex(t)+1]->w+6,i_rc.y+font_y,((t->xtext2)?(t->xtext2):("Оффлайн")),0,clfont,1,0,0,0,i_rc.x2-i_rc.x);//item name
 
//  if(t->xstate!=0)
  //{
//    DYNPNGICONLIST *xs=GetXSByItem(t->xstate);
    DrwImage(XStatusImg[t->xstate],i_rc.x+2,i_rc.y+(font_y*2));    

    sprintf(ixtext,(t->xtext)?(t->xtext+1):("Нет хстатуса"));        
    ixtext[strlen(t->xtext)-1]=0;
    PrintText(i_rc.x+XStatusImg[t->xstate]->w+4,i_rc.y+(font_y*2)-1,ixtext,0,clfont,1,0,1,0,i_rc.x2-i_rc.x-XStatusImg[t->xstate]->w-4);//item name
 // }
  /*
  DrwImage(icons[IS_UNKNOWN],i_rc.x+2,i_rc.y+(font_y*3)-1,0,0); 
  PrintText(i_rc.x+icons[IS_UNKNOWN]->w+1,i_rc.y+(font_y*3)-1,t->xtext2,0,clfont,1,0,0,0,i_rc.x2-i_rc.x-icons[IS_UNKNOWN]->w-3);
*/    
  while(i<_MAX)
  {
   start_y = (i_rc.y + (font_y*2) + (i/* - (_curpage-1)*_itemFromDisp*/)*font_y);    //определяем начальное смещение по у      
   StoreXYXYtoRECT(&rc,start_x,start_y,i_rc.x2,start_y+font_y);            
   if(i==i_curpos)//Если наш итем то выделяем
      DrawRoundedFrame(rc.x-2,rc.y-2,rc.x2-5,rc.y2-4,0,0,0,MMCursorClr,MMCursorBGClr);//cursor           
   
   PrintText(i_rc.x+DrawIconInfo(i_rc.x+2,start_y,i)+6,rc.y-3,(char*)menutext[i-1],0,clfont,1,0,0,0,rc.x2-rc.x-19);//item name
   i++;
  }
  UnlockSched();  
  i_rc.y=back;
  mfree(s);
}

// Управление курсором
void I_MoveUp()
{ 
  if(i_curpos > 0) i_curpos--;
  else
   i_curpos=_MAX-1;  
}


void I_MoveDown()
{
  if(i_curpos < _MAX - 1 ) i_curpos++; 
  else i_curpos=1;  
}  

void I_Redraw()
{
 DirectRedrawGUI();
}


void DrawHeader();

void I_OnRedraw(MAIN_GUI *data)
{
  DrawHeader();
  DrwImage(&screen,0,hy+2);  
//  DrawRoundedFrame(0,YDISP,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x30","\x00\x00\x00\x30");
  DrawRoundedFrame(i_rc.x,i_rc.y,i_rc.x2,i_rc.y2,0,0,0,/*"\x00\x00\x00\x45"*/MMBackgrBordClr,MMBackgrClr);
  I_DrawList();
  SetScrollEdgeDelay(0);
  if(i_curpos==0)
  {
    char xl[32] = {0};
    char xs[128] = {0};
    int c;
    int i=0;
    char *p=ixtext;
    c=*p;
    while(i<32)
    {      
      xl[i]=*p;
      i++;
      c=*p++;
      if(c==13) break;
    }
    xl[i]=0;
    *p++;
//    sprintf(xs,p);
  //  xs[strlen(xs)-1]=0;
    DrawRoundedFrame(i_rc.x-5,i_rc.y+FontH(clfont),i_rc.x2+5,i_rc.y+(FontH(clfont)*(_MAX+2)),0,0,0,MMBackgrBordClr,MMBackgrClr);
//    PrintText(i_rc.x,i_rc.y+(FontH(clfont)*2),ixtext,0,clfont,1,1,0,0,i_rc.x2-i_rc.x);//item name
    PrintText(i_rc.x,i_rc.y+(FontH(clfont)*2),xl,0,clfont,1,1,0,0,i_rc.x2-i_rc.x);//item name
    PrintText(i_rc.x,i_rc.y+(FontH(clfont)*4),p ,0,clfont,1,1,0,0,i_rc.x2-i_rc.x);//item name
  }
//  GBS_StartTimerProc(&i_tmr, 100, I_Redraw);  
}

void I_onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{    
  if(isSystemFont) clfont=100+pngFont;
  else clfont=2;
  DrawRoundedFrame(0,YDISP,ScreenW()-1,ScreenH()-1,0,0,0,"\x00\x00\x00\x30","\x00\x00\x00\x30");
  DoScreen();  
#ifdef ELKA
  i_rc.x=30;
  i_rc.x2=ScreenW()-25;
#else
  i_rc.x=5;
  i_rc.x2=ScreenW()-5;
#endif
  i_rc.y=hy+5;
  i_rc.y2=i_rc.y+(FontH(clfont)*(_MAX+3));
//  StoreXYXYToRECT(&i_rc,30,hy+5,ScreenW()-25,i_rc.y+(FontH(clfont)*(_MAX+4)));
  SetCanvasFlag(1);
  i_curpos=1;
  data->gui.state=1;
}

void I_onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
//  mfree(ixtext);
  mfree(screen.bitmap);
  GBS_DelTimer(&i_tmr);
  data->gui.state=0;
}

void I_onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
  data->gui.state=2;
  DisableIDLETMR();
}

void I_onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  if (data->gui.state!=2) return;
  data->gui.state=1;
}

int I_OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
  if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(msg->gbsmsg->submess)
    {
    case UP_BUTTON:    I_MoveUp();   break;
    case DOWN_BUTTON:  I_MoveDown(); break;
    case LEFT_BUTTON: case RIGHT_SOFT:case RED_BUTTON: return (1);
    }
  }
    if (msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS)
  {
    switch(msg->gbsmsg->submess)
    {
      case ENTER_BUTTON: case LEFT_SOFT: case RIGHT_BUTTON:
        {
            CLIST *t;
            t=FindContactByUin(uinContact);
            t->isIndividSet=1;
            ((void (*)(void))(menuproc[i_curpos-1]))();
            REDRAW();                
        }
        break;
    }
  }
        
  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int I_method8(void){return(0);}
int I_method9(void){return(0);}

const void * const I_gui_methods[11]={
  (void *)I_OnRedraw,
  (void *)I_onCreate,
  (void *)I_onClose,
  (void *)I_onFocus,
  (void *)I_onUnfocus,
  (void *)I_OnKey,
  0,
  (void *)kill_data,
  (void *)I_method8,
  (void *)I_method9,
  0
};

int CreateContactMenu(CLIST *t)
{
  uinContact=t->uin;
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
//  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)I_gui_methods;
  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  return CreateGUI(main_gui);   
}

/*********************************************************************/

int itemFromDisp;//кол-во на экране
int totalItems;//общее количество итемов
//int curpos=0;//позичия курсора
int curpage=1;//текущая страница
int collum=0;//столбцы))) 0 - первый, 1- второй
const char _t[]="%t";
const char _d[]="%d";
//int clfont;

#ifdef ELKA  
  #ifndef ICONBAR
    #undef YDISP
    #define   YDISP 0
  #endif
#endif

#define CLIST_Y1 (YDISP+FontH(clfont)+4)


void RecountMenu(CLIST *req, int needfocus)
{
  int i;
  int j;
  if (!maingui_id) return; //Нечего считать
  if (req==NULL)
  {
    j=0;
  }
  else
  {
    j=FindContactByContact(req);
  }
  i=CountContacts();
  if (j>=i) j=i-1;
  if (j<0) j=0;
  if(needfocus) curpos=j;
//  if (IsGuiOnTop(contactlist_menu_id)) 
    SMART_REDRAW();//DirectRedrawGUI_ID(contactlist_menu_id);//contactlist_menu_id
}


void DrawStr(int x,int y,char*s,char *color,int font,int align)
{
  SetFontColor(color,transparent,0);  
  PrintField(x,y,s,align,font,1);//capacity in header  
//void PrintText( int x, int y, char *str, int align, int font, int space, int lineSpace, int isScroll, short fixSize, int maxLen);
}

void DrawText(int x,int y,int x2,char*s,char *color,int font,int align,int isScroll)
{
  SetFontColor(color,transparent,0);  
//  SetScrollEdgeDelay(0);
  PrintText(x,y,s,align,font,1,350,isScroll,0,x2-x);
//int x, int y, char *str, int align, int font, int space, int lineSpace, int isScroll, short fixSize, int maxLen);
}


IMGHDR screen3={0,0,8,""};
int shot_soft=0;

void DoScreen3(int h)
{
  //int SCREENSIZE = ScreenH()*ScreenW();
  int HSIZE = (ScreenH()-h)*ScreenW();
  int ScrH=h+2;
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen3.w=ScrW;
  screen3.h=ScrH;
  screen3.bitmap=malloc(ScrW*ScrH*2);
  ms+=(HSIZE*2);
  memcpy(screen3.bitmap, ms, ScrW*ScrH*2);
}

void DrawSoftKeys(char *left,char *right)
{
  TTime t;  
  char *s=malloc(32);
  if(!PNG_SOFTKEY) clfont=100+HeadFont;
  else clfont=4;
  
  int FntH=FontH(clfont);
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  
  LockSched();
#ifdef ELKA
  if(shot_soft!=2)
  {
    DrawRoundedFrame(0,ScreenH()-1-FntH-2,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);
    for(int i=FontH(clfont);i>0;i-=2)
      DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i+2,0,0,0,SoftBackgrClr,SoftBackgrClr);

    DoScreen3(FntH);
    shot_soft++;
    SMART_REDRAW();
    return;
  }
  else  
    DrwImage(&screen3,0,scr_h-FntH);
#else
   DrawRoundedFrame(0,ScreenH()-1-FntH-2,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);
   for(int i=FontH(clfont);i>0;i-=2)
     DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i,0,0,0,SoftBackgrClr,SoftBackgrClr);  
#endif
  
  SetFontColor((char*)SoftFontClr,transparent,0);    
   
  if(sendq_l)
  {
    sprintf(s,"Q: %d b",sendq_l);
    PrintField(scr_w,scr_h-1-FntH,s,2,clfont,1);          
//    DrawStr(2,scr_h-1-FntH,s,SoftFontClr,clfont,2);
  }
  else
    PrintField(scr_w,scr_h-1-FntH,right,2,clfont,1);          
  
  DrawStr(2,scr_h-1-FntH,left,(char*)SoftFontClr,clfont,0);  
  
  GetDateTime(0,&t);
  sprintf(s,"%02d:%02d",t.hour,t.min);
  PrintField(scr_w/2,scr_h-1-FntH,s,1,clfont,1);
  UnlockSched();
  
  mfree(s);
}

void DrawSoftInfo(CLIST *t)
{
  TTime tm;  
  char *s=malloc(32);
  if(!PNG_SOFTKEY) clfont=100+HeadFont;
  else clfont=4;
  
  int FntH=FontH(clfont)*2;
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  
 
  LockSched();
//  DrawRoundedFrame(0,ScreenH()-5-FntH,ScreenW()-1,ScreenH()-1,0,0,0,clrRed,0);//Рамка
  //DrawRoundedFrame(1,ScreenH()-5-FntH,ScreenW()-2,ScreenH()-2,0,0,0,0,AddInfoClr);//Фон
    
  unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;	//sendq_l	
  if(sendq_l) 
    sprintf(s, "[T:%db/Q:%db]\n",RX+TX,sendq_l);         
  else
    sprintf(s, "[T:%db]\n",RX+TX);         
  PrintField(scr_w-5,scr_h-1-FntH,s,2,clfont,1);              
  
  int w=GetWidth(scr_w-5,scr_h-1-FntH);
  DrawRoundedFrame(0,scr_h-1-FntH-2,scr_w-1,scr_h-1,0,0,0,clrBlack,clrBlack);
  
  if(shot_soft!=2)
  {
    for(int i=FontH(clfont)*2+2;i>=0;i-=2)
       DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i,0,0,0,SoftBackgrClr,SoftBackgrClr);

    DoScreen3(FntH+4);
    shot_soft++;
    SMART_REDRAW();
    return;
  }
  else
    DrwImage(&screen3,0,scr_h-FntH-2);
  
  SetFontColor((char*)SoftFontClr,transparent,0);  
//  DrawStr(2,scr_h-1-FntH,s,SoftFontClr,clfont,0);
  PrintField(scr_w-3,scr_h-1-FntH,s,2,clfont,1);//left align
  
  GetDateTime(0,&tm);
  sprintf(s,"%02d:%02d",tm.hour,tm.min);
  PrintField(scr_w-3,scr_h-1-(FntH/2),s,2,clfont,1);//left align
//  PrintField(scr_w/2,scr_h-1-FntH,s,1,clfont,1);
  
  //void DrawText(int x,int y,int x2,char*s,char *color,int font,int align,int isScroll)
   DrwImage(icons[GetIconIndex(t)],2,scr_h-FntH);
   DrawText(6+icons[GetIconIndex(t)]->w,scr_h-FntH-2,scr_w-w,(t->xtext2?t->xtext2+1:"Ошибка"),(char*)FontInfoClr,clfont,0,0);
   
   DrwImage(XStatusImg[t->xstate],2,scr_h-(FntH/2));
   SetScrollEdgeDelay(1);
   DrawText(6+XStatusImg[t->xstate]->w,scr_h-(FntH/2)-1,scr_w-w,t->xtext?t->xtext:"Нет хстатуса",(char*)FontInfoClr,clfont,0,1);
//  DrawStr(2,scr_h-1-(FntH/2),t->xtext2,SoftFontClr,clfont,0);
  //DrawStr(2,scr_h-1-FntH,t->xtext,SoftFontClr,clfont,0);
  
  UnlockSched();
  
  mfree(s);
}

int shot_header=0;
IMGHDR screen4={0,0,8,""};
void DoScreen4(int h)
{
  //int SCREENSIZE = ScreenH()*ScreenW();
  int HSIZE = (h+8)*ScreenW();
  int ScrH=h+2;
  int ScrW=ScreenW();
  char *ms=RamScreenBuffer();
  screen4.w=ScrW;
  screen4.h=ScrH;
  screen4.bitmap=malloc(ScrW*ScrH*2);
  ms+=(HSIZE*2);
  memcpy(screen4.bitmap, ms, ScrW*ScrH*2);
}

void DrawHeader()
{
  SetCanvasFlag(1);
  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  char *s=malloc(128);
  char *ss=malloc(128);
  
  if(!PNG_HEADER)clfont=100+HeadFont;
  else clfont=4;
//  char clrRed[]={0xFF,0x00,0x00,0x15};
  if(icons[IS_ONLINE])
    hy=(icons[IS_ONLINE]->h > FontH(clfont)) ? (icons[IS_ONLINE]->h) : (FontH(clfont));
  else
    hy=FontH(clfont);    
  
  hy+=YDISP;
  LockSched();

  SetFontColor((char*)HeadFontClr,transparent,0);       

#ifdef ELKA
  if(shot_header!=2)
  {    
    DrawRoundedFrame(0,YDISP,scr_w,hy+2,0,0,0,clrBlack,clrBlack);    
    for(int i=hy-YDISP;i>0;i-=2)
      DrawRoundedFrame(0,YDISP,scr_w,hy-i+2,0,0,0,HeadBackgrClr,HeadBackgrClr); 
    DoScreen4(hy-YDISP);
    shot_header++;
    SMART_REDRAW();
    return;
  }
  else
    DrwImage(&screen4,0,YDISP);
#else
    DrawRoundedFrame(0,YDISP,scr_w,hy+2,0,0,0,clrBlack,clrBlack);    
    for(int i=hy-YDISP;i>0;i-=2)
      DrawRoundedFrame(0,YDISP,scr_w,hy-i+2,0,0,0,HeadBackgrClr,HeadBackgrClr); 
#endif
  
#ifdef DEBUG1
  return;
#else  
  int x=1;
  int icon;  
  if(total_unread) icon = IS_MSG;  
  else
  {
    switch(connect_state)
    {
     case 0:  icon=IS_OFFLINE;    break;
     case 3:  icon=CurrentStatus; break;
     default: icon=IS_UNKNOWN;    break;
    }
  }

  if(icons[icon] && HEAD_STATUS)
  {
    DrwImage(icons[icon],x,YDISP+1);
    x+=icons[icon]->w+2;//GetImgWidth(NumStatusToIconBar+icon)+2;// + смещение по х 
  }
  
  if(total_unread && isShowUnread)
  {
    sprintf(s,"%d/%d",total_unread,total_unread2);
    DrawStr(x,YDISP+1,s,clrWhite,clfont,0);
    x+=GetWidth(x,YDISP+1)+1;
  }
  
  if(CurrentXStatus!=0 && HEAD_XSTATUS)//если есть хстатус
  {
//    DYNPNGICONLIST *xs=GetXSByItem(CurrentXStatus);
  //  DrwImage(xs->img,x,YDISP+1);
//    x+=xs->img->w+1;  
    DrwImage(XStatusImg[CurrentXStatus],x,YDISP+1);
    x+=XStatusImg[CurrentXStatus]->w+1;  
  }   
  
  int priv=0;
  char A_s[]="[A:%s]";
 // char t[32];
  
  switch(CurrentPrivateStatus)
  {
    case 0: priv=ICON_ALL_CAN_SEE;          break;
    case 1: priv=ICON_NOBODY_CAN_SEE;break;
    case 2: priv=ICON_VISLIST_CAN_SEE;      break;
    case 3: priv=ICON_INVISLIST_CANNOT_SEE; break;
    case 4: priv=ICON_CONTACTLIST_CAN_SEE;  break;
  }

  if(icons[priv] && HEAD_PRIVATE)
  {
    DrwImage(icons[priv],x,YDISP+1);
    x+=icons[priv]->w+2;
  }


  if(!HEAD_PING)
  {
    int ping=0;
    if(curping < 3) ping=1;
    if(curping > 3 && curping < 10)  ping=2;
    if(curping > 10 && curping < 30) ping=3;
    if(curping > 30) ping=4;
    
    switch(ping)
    {
      case 1: ping=ICON_PING1; break;
      case 2: ping=ICON_PING2; break;
      case 3: ping=ICON_PING3; break;
      case 4: ping=ICON_PING4; break;  
      default: ping=ICON_PING1; break; 
    }
    
    if(icons[ping] && !HEAD_PING)
    {
      DrwImage(icons[ping],x,YDISP+1);
      x+=icons[ping]->w+1;
    }
  }

  int cap;   
  int x2=scr_w;  
#ifdef ELKA
  int y = YDISP-1;
#else
  int y = YDISP+1;
#endif
  //x2=scr_w-GetWidth(scr_w,YDISP+1);
  if(strlen(ContactT9Key))
  {
    sprintf(s,key_clm_hdr_text);
    strcat(s,ContactT9Key);
    strcpy(clmenu_sk_r,key_clmenu_sk_r);
    PrintField(scr_w,y,s,2,clfont,1);
  }
  else
    if(on_off_timer > 0 && strlen(on_off_buf)!=0)//если кто то подключился/отключился
    {
       SetScrollEdgeDelay(1);
       PrintText(scr_w,y,on_off_buf,2,clfont,1,350,1,0,scr_w-x-2);
    }
  else
  {
    x2=scr_w;
    cap=*RamCap();   
    sprintf(s,"[%d%%]",cap); 
    PrintField(scr_w,y,s,2,clfont,1);//capacity in header
    
    x2=scr_w-GetWidth(scr_w,y);
    strcpy(clmenu_sk_r,def_clmenu_sk_r);   
    Get_Online_total();

    strcpy(s,"");
    if(HEAD_PING==1)  sprintf(s,"[%d с.]",curping);  
    if(away_m && isShowA)
    {
      sprintf(ss,A_s,away_msg_time_dace);  
      strcat(s,ss);
    }
    if(Is_Vibra_Enabled && isShowV)        strcat(s,"[V]") ;
    if(Is_Sounds_Enabled && isShowS)       strcat(s,"[S]") ;
    if(Is_Double_Vibra_On && isShowR)      strcat(s,"[Vr]") ;

    if(isShowCont)
    {
      sprintf(ss,"[%d/%d]",kopa_kontaktu,kopa_online);
    //  PrintField(x2-2,YDISP+1,s,2,clfont,1);
      strcat(s,ss);
    }
    //PrintField(x2-2,YDISP+1,s,2,clfont,1);
    PrintText(x2,y,s,2,clfont,1,1,1,0, x2-x-2 );
  }
  UnlockSched();
  mfree(s);
  mfree(ss);
#endif
}

void DrawScrollBar(int cur,int total)
{
  int scr_h=ScreenH()-1;
  int scr_w=ScreenW()-1;
  int y=scr_h-CLIST_Y1-FontH(clfont)-2;
  int pos=y*cur/total;
  int y2=y/total;
  int i=0;
  
  DrawLine(scr_w-2,CLIST_Y1,scr_w-2,scr_h-FontH(clfont)-2,1,clrBlack);
  DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+y2,0,0,0,clrBlack,clrBlack);
  
  for(i=0;i<y2;i+=1)
    DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+i,0,0,0,ScrollBarClr,ScrollBarClr);  
}


int DrwImage(IMGHDR *img, int x, int y/*, char *pen, char *brush*/)
{
  if(!img) return 0;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0/*pen,brush*/);
  DrawObject(&drwobj);
  return 1;
}
/*
#ifdef ELKA
int LINE_MODE=0;
int VIEW_MODE=1;
#else
int LINE_MODE=1;
int VIEW_MODE=0;
#endif
*/

void MyRedraw()
{
  //DirectRedrawGUI_ID(contactlist_menu_id);
  SMART_REDRAW();
}


void DrawJustNow(RECT *rc,CLIST *cl)
{
  if(!cl->just_i)
    DrawText(rc->x+1,rc->y,rc->x2,cl->name,blue,clfont+1,0,0);      
  else
    DrawText(rc->x+1,rc->y,rc->x2,cl->name,clrRed,clfont+1,0,0);

  cl->just_i=!cl->just_i;
}


void log(char *name,int i,int j)
{
  int hFile;
  unsigned int io_error = 0;
  char text[255];
  sprintf(text,"%s - %d(%d)",name,i,j);
  hFile = fopen("4:\\nata.txt",A_ReadWrite + A_Create + A_Append,P_READ+P_WRITE, &io_error);
  if(hFile!=-1)
  {
    fwrite(hFile, text, strlen(text), &io_error);
    fwrite(hFile, "\r\n", 2, &io_error);
    fclose(hFile, &io_error);
  }
  else
    ShowMSG(1,(int)"Error!");
//  mfree(text);
}

/*
typedef struct
{
 int type; //0-доставлен, 1-отправлен, 2-не отправлено
 int count;//кол-во не доставленных 
}ACKED;
*/
#define SENDED  0
#define SEND    1
#define NOTSEND 2
ACKED *GetAck(CLIST *t)
{  
  if(!ShowProcessSend) return 0;
  if(!t) return 0;
  if(!t->isactive) return 0;
  if(t->state==0xFFFF) return 0;

  ACKED *ack=malloc(sizeof(ACKED));
  ack->type=0;
  ack->count=0;   
  LOGQ *lp;
  lp=t->log;

  while(lp)
  {
    if(lp->acked)//Если отправлен или доставлен
    {
      if (lp->acked==1)//отправлен,но не доставлен
      {
        ack->type = SEND;
        ack->count++;
      } 
      else
        ack->type = SENDED;//доставлен(прозрачный)
    }
    else
     if (lp->ID!=0xFFFFFFFF)//не доставлено
     {
       ack->type = NOTSEND; 
       ack->count++;
     }     
    lp=lp->next;
  }
  //if(ack->type==0) return 0;  
  //if(ack->count==0) return 0;
 // log(t->name,ack->type,ack->count);
  return ack;
}

int clstartItem=0;
int BlinkMSG=0;
void DrawList()
{  
  //Тут пиздец,и мне по хуй,я разбираюсь в этом и знаю что оно работает как нужно.
  
  if(isSystemFont) clfont=100+pngFont;
  else clfont=2;
  if(!cltop) return;

//  int font_y = FontH(clfont)+2;//16+2

  int font_y = 0;
  if(FontH(clfont) < icons[IS_ONLINE]->h) font_y = icons[IS_ONLINE]->h+2;//Если иконки выше чем фонт,то по высоте иконок рисуемся
  else font_y = FontH(clfont)+2;

  int scr_w=ScreenW()-1;
  int scr_h=ScreenH()-1;
  int y2;

  //__root const unsigned int SOFT_MODE = 0;
  // __root const CFG_CBOX_ITEM cfgcboxI_8[3]={"Ничего,""Софты","Инфу"};
  int SoftH=0;
  
  if(SOFT_MODE==1) SoftH=FontH(clfont);//если показывать софты
  else if(SOFT_MODE==2) SoftH=FontH(clfont)*2;//если показывать инфу
  
  if(VIEW_MODE)
    itemFromDisp = (sdiv(font_y,ScreenH()-CLIST_Y1-SoftH-2)*2)-1;//((font_y/(ScreenH()-CLIST_Y1-FontH(clfont)-2))*2)-1;
  else
    itemFromDisp = sdiv(font_y,ScreenH()-CLIST_Y1-SoftH-2);

  CLIST *cl;
  int i=0;
  int start_y;
  int x=0;  
  RECT rc;
  int icon;
  collum=0;
//  int clfont=108;
  char *name=malloc(128);
  int yy=0;
  int drawFont = clfont;
  
  ACKED *ack;
  DYNPNGICONLIST *xs;
  
  LockSched();
  totalItems = CountContacts(); //TotalBmk(); 

  if(LINE_MODE) itemFromDisp--; 

  while( i < ((VIEW_MODE!=0)?(totalItems):(itemFromDisp)) )
  {
     if(!VIEW_MODE) goto next;//если не два столбика то пропускаем условие
     if( (i>=(curpage-1)*itemFromDisp) && (i<=itemFromDisp*curpage) )//если те итемы который нужно выводить на экран со страници curpage
        {
        next:
          cl=FindContactByN( i + ((VIEW_MODE==0)?(clstartItem):(0)) );
          if(!cl) return;
          if(cl)//если такой есть то рисуем его
          {
            start_y=0;
            if(LINE_MODE)//если в две линии
            { 
              if(i + ((VIEW_MODE==0)?(clstartItem):(0)) > curpos ) start_y+=font_y+3;
              y2=2; 
            }
            else y2=1;
          
            if(VIEW_MODE)//если в две колонки
            {
              if(!collum)//если первая колонка
              {
                start_y += (CLIST_Y1 + (((i - (curpage-1)*itemFromDisp)*font_y)/2) );    //определяем начальное смещение по у      
                yy=start_y;
                StoreXYXYtoRECT(&rc, 1, start_y, (scr_w/2)-5, start_y+(font_y*y2));
              }
              else//если вторая
              {
                StoreXYXYtoRECT(&rc, (scr_w/2)-4, yy, scr_w-5, yy+(font_y*y2));
              }
              collum=!collum; 
            }
            else
            {
              start_y += (CLIST_Y1 + 2 + (i/* - (curpage-1)*itemFromDisp*/)*font_y);
              StoreXYXYtoRECT(&rc,2,start_y,scr_w-5,start_y+(font_y*y2));                          
            }
            
            icon=GetIconIndex(cl);
            if (icon!=IS_GROUP)//если не иконка группы
            {
              ack=GetAck(cl);//Обработка процесса отправки
             // log(cl->name,ack->type,ack->count);                   
              sprintf(name,percent_s,cl->name);
            }
            else
            {
              int onlinetotal[2];
              GetOnTotalContact(cl->group,onlinetotal);
              if(!VIEW_MODE)//если не в два столбца
              {
                char *s=malloc(32);
                sprintf(name,percent_s,cl->name);
                sprintf(s,"(%d/%d)",onlinetotal[0],onlinetotal[1]);
                SetFontColor((char*)GroupClr,transparent,0);  
                PrintField(scr_w-7,start_y,s,2,clfont,1);//capacity in header
                mfree(s);
              }
              else
               sprintf(name,"%s%c%c(%d/%d)",cl->name,0xE01D,0xE012,onlinetotal[0],onlinetotal[1]);
              if (cl->state) icon++; //Модификация иконки группы
            }
            
           // DrawImg(rc.x,rc.y,NumStatusToIconBar+icon);//status
            if(icons[icon])//вдруг иконки нет
            {
           /*   if(icon==IS_MSG)
              {
                if(!BlinkMSG)
                  DrwImage(icons[((cl->just_i)?(IS_OFFLINE):(icon))],rc.x,rc.y);//draw imghdr!
              }
              else*/
                  DrwImage(icons[((cl->just_i)?(IS_OFFLINE):(icon))],rc.x,rc.y+1);//draw imghdr!
              rc.x+=icons[icon]->w+2;// + смещение по х                          
            /*  if(icon==IS_MSG)
                BlinkMSG=!BlinkMSG; 
              else
                BlinkMSG=0;*/
            }
            
            if(cl->unread_msg!=0)
            {
              char p[3];
              sprintf(p,_d,cl->unread_msg);
              DrawText(rc.x,rc.y,rc.x2,p,(cl->isactive!=0)?(clrRed):(clrBlack),(isSystemFont)?clfont:((cl->isactive!=0)?(clfont+1):(clfont)),0,0);     
              rc.x+=GetWidth(rc.x,rc.y)+2;
            }
            
            if(ack && ack->count && (ShowProcessSend==1 || ShowProcessSend==3))
            {
              char p[3];
              if(cl->unread_msg)
                sprintf(p," %d",ack->count);
              else
                sprintf(p,_d,ack->count);
              DrawText(rc.x,rc.y,rc.x2,p,(ack->type==1)?((char*)SendClr):((ack->type==NOTSEND)?((char*)UnAckedClr):0),clfont+1,0,0);//(cl->ack->type==1)?((char*)UnAckedClr):((char*)SendClr)
              rc.x+=GetWidth(rc.x,rc.y)+2;
            }
            
            if(i + ((VIEW_MODE==0)?(clstartItem):(0)) == curpos )//Если наш итем то выделяем
            {
              DrawRoundedFrame(rc.x-1,rc.y-1,rc.x2-1,rc.y2-1,0,0,0,ClCursorBordClr,ClCursorClr);
              if(LINE_MODE && icon!=IS_GROUP)
              {
                char s[64];
                if ((cl->xtext && cl->xtext[0]!=0) && (cl->isShowXst) && (cl->state!=0xFFFF))//Если контакт онлайн и показывать хстату
                  {
                    int i;
                    zeromem(s,64);
                    i=cl->xtext[0];
                    if (i>63) i=63;
                    strncpy(s,cl->xtext+1,i);                    
                  }
                  else
                    if(cl->xtext2 && cl->xtext2[0]!=0 && isLogStatusChange)
                    {
                      int i;
                      zeromem(s,64);
                      i=cl->xtext2[0];
                      if (i>63) i=63;
                      strncpy(s,cl->xtext2+1,i);
                    }
                  else
                    sprintf(s,percent_d,cl->uin);
                SetFontColor((char*)CLDescricpClr,transparent,0);  
                PrintText(rc.x+x+1,rc.y+font_y,s,0,clfont,1,350,0,0,rc.x2-rc.x-3);//description under item name
              }
              
              if(totalItems>1)
                DrawScrollBar(i+clstartItem,totalItems);
              
              if(SOFT_MODE==2) DrawSoftInfo(cl);
            }
            
            if(cl->xstate!=0)//если есть хстатус
            {
              //xs=GetXSByItem(cl->xstate);
              if(XStatusImg[cl->xstate]->bitmap)
              {
                DrwImage(XStatusImg[cl->xstate],rc.x-x,rc.y+1);
                rc.x+=XStatusImg[cl->xstate]->w;
              }
            }
            /*
            if(cl->xstate!=0)//если есть хстатус
            {
              xs=GetXSByItem(cl->xstate);
              if(xs)
              {
                DrwImage(xs->img,rc.x-x,rc.y);
                rc.x+=xs->img->w;
              }
            }*/

            //Наш фонт,каким буим рисовать)) системный - жирный не жирный,или елси пнг фонт, то активный один,не активный другой
            drawFont = (isSystemFont) ? (clfont + (SelBoldActiveChat==1 ? ((cl->isactive && cl->state!=0xFFFF)?1:0) : 0)) : ((cl->isactive!=0)?(clfont+1):(clfont));
            if(ack && ack->type!=SENDED && ack->type <= 2  && (ShowProcessSend==2 || ShowProcessSend==3))//если есть не отправленные
            {
              if(!VIEW_MODE)//в один столбец, рисуем длинну по имени
              {
                PrintText(rc.x+3,rc.y,name,0,drawFont,1,350,0,0,rc.x2);
                DrawRoundedFrame(rc.x,rc.y,rc.x+GetWidth(rc.x+3,rc.y)+3,rc.y2-2,0,0,0,0, (ack->type==SEND)?(SendClr):((ack->type==NOTSEND)?(UnAckedClr):0));
              }
              else
                DrawRoundedFrame(rc.x,rc.y+1,rc.x2-1,rc.y2-2,0,0,0,0, (ack->type==1)?(SendClr):((ack->type==NOTSEND)?(UnAckedClr):0));
            }
            
            if(cl->just_now)
               DrawJustNow(&rc,cl);
            else
            {
              if(icon==IS_GROUP)//группы,жирным цветом
                DrawText(rc.x+3,rc.y,rc.x2,name,(char*)GroupClr,clfont+1,0,0);
              else//Рисуем наши имена,жирные,систкмные,выделенные......
                DrawText(rc.x+3,rc.y,rc.x2,name,(cl->isactive!=0)?((char*)ActiveClr):((char*)NotActiveClr),drawFont,0,0);
            }
         }//if(cl)
       }
      i++;
//      cl=cl->next;
      if(i > (itemFromDisp*((VIEW_MODE!=0)?(curpage):(1))) ) break;   
  }
  UnlockSched();  
  mfree(name);
//  mfree(ack);
}

// Управление курсором
void MoveUp2(int i)
{
  if( curpos > itemFromDisp + 2 && totalItems!=itemFromDisp )
    {
      curpos-=i; 
      if(clstartItem > 0 && curpos > ((itemFromDisp/2)+1) && curpos < totalItems-((itemFromDisp/2)+1))
        clstartItem-=i; 
    }
  else
    if(curpos > 0)
    {
      curpos-=i;
      if(clstartItem > 0 && curpos > ((itemFromDisp/2)+1))
        clstartItem-=i;
      else
        if(curpos < ((itemFromDisp/2)+1))
          clstartItem=0; 
    }
  else
  {
    curpos=totalItems;
    if(totalItems==itemFromDisp)
      clstartItem=0;
    else
      clstartItem=totalItems-itemFromDisp;
  }
}


void MoveDown2(int i)
{
  if( curpos < itemFromDisp/2+1  && totalItems!=itemFromDisp ) curpos+=i;
  else
    if(curpos < totalItems - 1)
    {
      if(itemFromDisp != totalItems)
      {
        if(clstartItem < totalItems-itemFromDisp)
        clstartItem+=i;
      }
/*      else
      {
        if(clstartItem < itemFromDisp)
          clstartItem+=i;      
      }*/
      
      curpos+=i; 
    }
  else   
  {
    curpos=0;
    clstartItem=0;
  }
}

/*
// Управление курсором
void MoveUp(int i)
{
  if(!itemFromDisp)return;
  
  if(i) curpos-=2;
  else  curpos--;    
  
  if(i && curpos+2==2)
  {
    curpos=0;
    curpage=1;
  }
  else
    if(curpos<0)
    {
      curpos=totalItems;
    
      if(itemFromDisp!=totalItems)
      {
        curpage = sdiv(itemFromDisp,totalItems)+1;
      }
     else
      curpage = 1;
    }
  else
  {
   // if(i) curpos-=2;
   // else  curpos--;
    if(curpos<(curpage-1)*itemFromDisp){ curpos++; curpage--; }
  }
}

void MoveDown(int i)
{
  if(!totalItems)return;
  if(i) curpos+=2;
  else  curpos++;  

  if(curpos>totalItems) { curpos=0; curpage=1; }
  if(curpage*itemFromDisp<curpos){ curpos--; curpage++; } 
}*/


// Управление курсором
void MoveUp(int i)
{
  if(!itemFromDisp)return;
/*  if(i) curpos-=2;
  else  curpos--;  
  if(curpos<1) { curpos=totalItems-1; curpage=sdiv(itemFromDisp,totalItems)+1; }
  if(curpage*itemFromDisp>curpos){ curpos++; curpage--; } */
  
  if(i) curpos-=2;
  else  curpos--;    
  
  if(i && curpos+2==2)
  {
    curpos=0;
    curpage=1;
  }
  else
    if(curpos<0)
    {
      curpos=totalItems-1;
    
      if(itemFromDisp!=totalItems)
      {
        curpage = /*(itemFromDisp/totalItems)+1;*/sdiv(itemFromDisp,totalItems)+1;
      }
     else
      curpage = 1;
    }
  else
  {
   // if(i) curpos-=2;
   // else  curpos--;
    if(curpos<(curpage-1)*itemFromDisp){ curpos++; curpage--; }
  }
}

void MoveDown(int i)
{
  if(!totalItems)return;
  if(i) curpos+=2;
  else  curpos++;  
  if(curpos>=totalItems) { curpos=0; curpage=1; }
  if(curpage*itemFromDisp<curpos){ curpos--; curpage++; } 
}


void PrintMessage()
{
  DrawRoundedFrame(0,(ScreenH()/2)-(3*FontH(FONT_MEDIUM+100)),ScreenW()-1,(ScreenH()/2)+(FontH(FONT_MEDIUM+100)*2),0,0,0,clrWhite,"\x00\x00\x00\x40");
  SetFontColor(clrWhite,transparent,0);
  PrintText(ScreenW()/2+5,ScreenH()/2-FontH(FONT_MEDIUM+100)-5,prmsg,1,FONT_MEDIUM+100,1,1,0,0,ScreenW()-5);
}


void DrawContactList()
{
#ifdef DEBUG
  if(!IsGuiOnTop(contactlist_menu_id)) return;
#endif
  DrawRoundedFrame(0,hy,ScreenW()-1,ScreenH()-1,0,0,0,CLBackgrClr,CLBackgrClr);
 // DrawRoundedFrame(0,hy,ScreenW()-1,ScreenH()-1,0,0,0,clrWhite,clrWhite); 
  if(icons[ICON_BG]->bitmap) DrwImage(icons[ICON_BG],0,0);  
  DrawHeader();
  DrawList();
  if(SOFT_MODE==1) DrawSoftKeys(LG_MENU,clmenu_sk_r);
  if(strlen(prmsg)!=0 && PrintTimer!=0)  PrintMessage();
}

#ifdef DEBUG
static void OnRedraw(MAIN_GUI *data)
{
  DrawContactList();    
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
#ifdef ELKA
  #ifdef ICONBAR
    DisableIconBar(0);
  #else
     DisableIconBar(1);
  #endif
#endif
 
//  StatusToIMGHDR();
  data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
  data->gui.state=0;
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
#ifdef ELKA
  #ifdef ICONBAR
    DisableIconBar(0);
  #else
     DisableIconBar(1);
  #endif
#endif
  SetCanvasFlag(1);
  data->gui.state=2;
  DisableIDLETMR();
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
#ifdef ELKA
  DisableIconBar(0);
#endif
  if (data->gui.state!=2) return;
  data->gui.state=1;
}
#endif

//int ShowLoadScreen=1;
int lastKey = -1;
int lastIsLongPress = 0;

int ClOnKey(GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
/*  int isLongPress = 0;
  int ignore = 1;
*/
  CLIST *t;
//  t=FindContactByN(curpos);  
      
  if (msg->gbsmsg->msg==LONG_PRESS)
  {        
    //isLongPress = (msg->gbsmsg->msg == LONG_PRESS);
    // Игнорим KEY_UP если уже обработали долгое нажатие этой же кнопки
    // Обрабатываем только KEY_UP и LONG_PRESS
    //ignore = (lastKey == key && lastIsLongPress && msg->gbsmsg->msg == KEY_UP)
  //           || (msg->gbsmsg->msg != KEY_UP && msg->gbsmsg->msg != LONG_PRESS);
//    if (!ignore)
    LongPressAction(FindContactByN(curpos),msg->gbsmsg->submess);
       
//    lastKey = key;
  //  lastIsLongPress = isLongPress;
    
    switch(msg->gbsmsg->submess)
    {
      case UP_BUTTON:    if(VIEW_MODE) MoveUp(1);   else MoveUp2(1)/*MoveUp(0)*/;   break;
      case DOWN_BUTTON:  if(VIEW_MODE) MoveDown(1); else MoveDown2(1)/*MoveDown(0)*/; break;
      case RIGHT_BUTTON: if(VIEW_MODE) MoveDown(0); else MoveDown2(2)/*MoveDown(1)*/; break;
      case LEFT_BUTTON:  if(VIEW_MODE) MoveUp(0);   else MoveUp2(2)/*MoveUp(1)*/;   break;
    }
    
    int key=msg->gbsmsg->submess;
    if ((key>='0')&&(key<='9'))
    {
      ClearContactT9Key();
      RecountMenu(NULL,1);
    }
  }
  
  
  if (msg->gbsmsg->msg==KEY_DOWN)
  {  
    switch(msg->gbsmsg->submess)
    {
     case UP_BUTTON:    if(VIEW_MODE) MoveUp(1);   else /*MoveUp(0)*/MoveUp2(1);   break;
     case DOWN_BUTTON:  if(VIEW_MODE) MoveDown(1); else /*MoveDown(0)*/MoveDown2(1); break;
     case RIGHT_BUTTON: if(VIEW_MODE) MoveDown(0); else MoveDown2(2)/*MoveDown(1)*/; break;
     case LEFT_BUTTON:  if(VIEW_MODE) MoveUp(0);   else MoveUp2(2)/*MoveUp(1)*/;   break;
     case LEFT_SOFT: 
      {
        extern void CreateMainMenu();
     //   DoScreen();
        if(PrintTimer)
         {
           PrintTimer=0;
           zeromem(prmsg,256);
         }
       else
        CreateMainMenu();
//        ShowMainMenu();
      }break;
    case RIGHT_SOFT:
    case RED_BUTTON:
      if(PrintTimer)
        {
          PrintTimer=0;
          zeromem(prmsg,256);
        }
      else if (strlen(ContactT9Key))
      {
        BackSpaceContactT9();
        RecountMenu(NULL,1);
        return(-1);
      }

      else
      {
#ifdef DEBUG
        ShowLoadScreen=1;
        return 1;
#else
          ShowLoadScreen=1;//return 1;
#endif
      }
      break;
    case ENTER_BUTTON:
     {         
      t=FindContactByN(curpos);  
      if(PrintTimer)
       {
         PrintTimer=0;
         zeromem(prmsg,256);
       }
      else
      if(t)
      {
        if (t->isgroup)
        {
          t->state^=0xFFFF;
          RecountMenu(t,1);
	  return(-1);
        }
        if (strlen(ContactT9Key))
        {
    	  ClearContactT9Key();
	  RecountMenu(NULL,1);
       }
       if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
       CreateEditChat(t);
       }
      return(-1);
      }
      case '*':
        if(CountContacts()>0 && t)
         {
          t=FindContactByN(curpos);  
          while(t=(CLIST *)(t->next))
           {
             if (t->isunread) goto doit;
           }
          t=(CLIST *)(&cltop);
          while(t=(CLIST *)(t->next))
          {
            if (t->isunread) goto doit;
          }
          if (t && Is_Show_Groups)
          {
           if (!t->isgroup) t=FindGroupByID(t->group);
           if (t && !t->state)
           {
            t->state^=0xFFFF;
            goto does;
           }
          }
          
          curpage=1;
          curpos=0;
          clstartItem=0;
    /*    while(t=(CLIST *)(t->next))
         {
           if (t->isactive) goto doit;
         }
        t=(CLIST *)(&cltop);
        
        while(t=(CLIST *)(t->next))
        {
          if (t->isactive) goto doit;
        }
        return(-1);*/
       doit:
          {
            CLIST *g=FindGroupByID(t->group);
            if (g)
            {
              if (g->state)
              {
                g->state=0;
              }
            }
          }
        does:
          RecountMenu(t,1);
          return(-1);
         }
      break;
    case GREEN_BUTTON:
      {
//        IsActiveUp=!IsActiveUp;
  //      RecountMenu(NULL,1);
        t=FindContactByN(curpos);  
        if(!t->isgroup)
         CreateContactMenu(t); 
      }
      break;
  //  case RED_BUTTON: return (1);
    case VOL_UP_BUTTON:
#ifdef NEWSGOLD
       if(isVolUpButton)
         CreateScreenSelectGUI();
#endif
       break;
    case VOL_DOWN_BUTTON:
      {
        if(isVolDownButton)
        {
         if(!away_m)
          {
            away_m=!away_m;
            main_d=1;
            InputAwayMsg();
          }
          else
          {
            msg_away_timer=0;
            away_m=!away_m;
            if(change_back_status)
            {
              CurrentStatus=status_rem;
              set_my_status();
              change_back_status=0;
            }
          }
        }
      }
      break;      
    }
    
    if (key=='#' && (CountContacts()>0))//Если есть хоть один контакт то выполнять,иначе возможен пикофф
    {
      t=FindContactByN(curpos);  
      if(ActionResh)
      {
        while(t=(CLIST *)(t->next))
         {
           if (t->isactive) goto doit4;
         }
        t=(CLIST *)(&cltop);
        while(t=(CLIST *)(t->next))
        {
          if (t->isactive) goto doit4;
        }
        return(-1);
      doit4:
        {
        CLIST *g=FindGroupByID(t->group);
         if (g)
         {
          if (g->state)
          {
            g->state=0;
          }
         }
        }
       RecountMenu(t,1);
      }
      else
      {
       if(t->log && !(t->just_now))//очистить лог
        {
          FreeLOGQ(&t->log);
          t->msg_count=0;
          t->isactive=0;
        }
       
       if(t->just_now)//если контакт тока зашел,то обнуляем
       {
         CLIST *tt;
         tt=FindContactByUin(t->uin);
         tt->just_now=0;
         tt->just_i=0;
       }
       
       if(t->uin==UIN)//если нажали на Loopback, то обнуляем все just_now'ы
       {
        CLIST *tt;
        tt=(CLIST *)(&cltop);
        while(tt=(CLIST *)(tt->next))
        {
          if(tt->just_now){ tt->just_now=0; tt->just_i=0; }
        }
       }
       if(strlen(ContactT9Key)) ClearContactT9Key();
       RecountMenu(NULL,0);
      }
    }   
  }  
 
  if ((key>='0')&&(key<='9') && msg->gbsmsg->msg==KEY_DOWN)
    {
      AddContactT9Key(key);
      RecountMenu(NULL,1);
      return(-1);
    }     
  return 0;  
}

#ifdef DEBUG
static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
  DirectRedrawGUI(); 
//  int k=msg->gbsmsg->submess;
  return ClOnKey(msg);
//  return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int method28(void){return(0);}
int method29(void){return(0);}

const void * const gui_methods2[11]={
  (void *)OnRedraw,
  (void *)onCreate,
  (void *)onClose,
  (void *)onFocus,
  (void *)onUnfocus,
  (void *)OnKey,
  0,
  (void *)kill_data,
  (void *)method28,
  (void *)method29,
  0
};



static void CreateCL()
{
  const RECT Canvas={0,0,0,0};
  MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
  zeromem(main_gui,sizeof(MAIN_GUI));
  main_gui->gui.canvas=(void *)(&Canvas);
  main_gui->gui.methods=(void *)gui_methods2;
//  main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
  //patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
  contactlist_menu_id=CreateGUI(main_gui); 
  SMART_REDRAW();
}

#endif
#ifdef ICONBAR
#else
  #ifdef ELKA  
    #undef YDISP
    #define   YDISP 24
  #endif
#endif


/************************************************************************************************/


void DrawLoadScreen(MAIN_GUI *data)
{
  int scr_w=ScreenW();
  int scr_h=ScreenH();
  int pos_status;//2026rev
  int pm = pictures_max, pl = pictures_loaded;//2026rev
  RECT rc=PBRECT;
  DrawRectangle(0,YDISP,scr_w-1,scr_h-1,0, GetPaletteAdrByColorIndex(1),  GetPaletteAdrByColorIndex(1));

//  if(!USE_ADD_UIN)//Если не используем доп.уины
  //{
//    DrawImg(0,0,S_ICONS[ICON_LOGO]);
  DrwImage(icons[ICON_LOGO],0,0);
  unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;			//by BoBa 10.07
  if(IsShowInfo)
    {
      wsprintf(data->ws1,LG_GRSTATESTRING,connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);
      wsprintf(data->ws2,logmsg2);
      wstrcat(data->ws1,data->ws2);       
    }
    else
    {
      wsprintf(data->ws1,"State: %d, RXstate: %d",connect_state,RXstate);     
    }
  
   if(pm != pl)
      {
  /*       DrawRectangle(0,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD),scr_w-1,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-2,0,
                         GetPaletteAdrByColorIndex(0),
                         GetPaletteAdrByColorIndex(0));*/
        //      pos_status = ((scr_w-1) * pl) / pm;
         /*     DrawRectangle(1,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD)+1,pos_status,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-3,0,
                           GetPaletteAdrByColorIndex(14),
                         GetPaletteAdrByColorIndex(14));  */
    //    wstrcatprintf(data->ws1,"\nLoading images...");
        
        DrawRectangle(rc.x,rc.y,rc.x2,rc.y2,0,BGPBcolor,BGPBcolor);
        pos_status = (((rc.x2-rc.x)-1) * pl) / pm;
        DrawRectangle(rc.x+2,rc.y+2,rc.x+pos_status+2,rc.y2-2,0,PBcolor,PBcolor);
       
        if(total_smiles)
        {
          wstrcatprintf(data->ws1,"\nLoaded %d smiles",total_smiles);
        }
        if (xstatuses_load)
        {
          wstrcatprintf(data->ws1,"\nLoaded %d xstatus",total_xstatuses);
        }
      }
  
    DrawString(data->ws1,3,3+YDISP,scr_w-4,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),FONT_SMALL,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));   
/*}
  else
  {  
    wsprintf(data->ws1,"%t UIN!","Выберите");
    DrawLine(0,CLIST_Y1-3,ScreenW()-1,CLIST_Y1-3,0,clrWhite);
    DrawString(data->ws1,2,YDISP+2,scr_w-1,YDISP+GetFontYSIZE(FONT_SMALL),FONT_SMALL,0,fntSelect,0);
//    DrawList(); 
  }
  */
    
#ifdef kluchnik  
  int y=160;
  char A[]="[A] %s";
  char I[]="[I]";
  char V[]="[V]";
  char S[]="[S]";

  if(CurrentPrivateStatus==1) 
   {
     wsprintf(data->ws2,I);
     DrawString(data->ws2,2,y,ScreenW()-1,y+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
     y+=GetFontYSIZE(11)+5;
   }
  
  if(Is_Vibra_Enabled) 
   {
    wsprintf(data->ws2,V);
    DrawString(data->ws2,2,y,ScreenW()-1,y+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
    y+=GetFontYSIZE(11)+5;
  }
  
  if(Is_Sounds_Enabled) 
   {
    wsprintf(data->ws2,S);
    DrawString(data->ws2,2,y,ScreenW()-1,y+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
    y+=GetFontYSIZE(11)+5;
  }
  
  if(Is_Double_Vibra_On)
   {
    wsprintf(data->ws2,"[Vr]");
    DrawString(data->ws2,2,y,ScreenW()-1,y+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
   }
  
  if(away_m) 
  {
    wsprintf(data->ws2,A,away_msg_time_dace);
    DrawString(data->ws2,2,y,ScreenW()-1,y+GetFontYSIZE(11),11,0,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(23));
    y+=GetFontYSIZE(11)+5;
  }
  

  wsprintf(data->ws2,"Uin: %d",UIN);
  DrawString(data->ws2,1,scr_h-10-4*GetFontYSIZE(FONT_SMALL_BOLD)+1,ScreenW()-1,scr_h-10-GetFontYSIZE(FONT_MEDIUM_BOLD)-3
             ,FONT_SMALL,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  
  if(connect_state==3)
  {
    wsprintf(data->ws2,"Online: %02d:%02d:%02d   not action %d sec", uptimeHour, uptimeMin, uptimeSec,msg_away_timer*5);
  }
  else
    wsprintf(data->ws2,"Offline [%02d:%02d:%02d]",uptimeHour, uptimeMin, uptimeSec);
  DrawString(data->ws2,1,scr_h-10-3*GetFontYSIZE(FONT_SMALL_BOLD)+1,ScreenW()-1,scr_h-10-GetFontYSIZE(FONT_MEDIUM_BOLD)-3
             ,FONT_SMALL,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
#endif
  
  if(IsInvertSoftKey)
    {
     wsprintf(data->ws2,percent_t,LG_GRSKEYEXIT);
     DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),scr_w-4,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNRIGHT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  
     wsprintf(data->ws2,percent_t,cltop?LG_GRSKEYCLIST:empty_str);
     DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD), scr_w>>1,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
    }
    else
    {
     wsprintf(data->ws2,percent_t,cltop?LG_GRSKEYCLIST:empty_str);
     DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),scr_w-4,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNRIGHT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  
     wsprintf(data->ws2,percent_t,LG_GRSKEYEXIT);
     DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD), scr_w>>1,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
    }
  /*
  wsprintf(data->ws2,"%d %s",addonuin[0].uin,addonuin[0].pass);
  DrawString(data->ws2,3,scr_h-4-(GetFontYSIZE(FONT_MEDIUM_BOLD)*3), scr_w>>1,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  */
}

//int ShowLoadScreen=1;
void method0(MAIN_GUI *data)
{
//  if(!IsGuiOnTop(maingui_id)) return;
#ifndef DEBUG
  if(IsGuiOnTop(maingui_id))
  {
    if(ShowLoadScreen)    
    {
  #ifdef ELKA
      DisableIconBar(0);
  #endif
      DrawLoadScreen(data);
    }
    else
    {
  #ifdef ELKA
    #ifdef ICONBAR
      DisableIconBar(0);
    #else
       DisableIconBar(1);
    #endif
  #endif
      SetCanvasFlag(1);
      DrawContactList();    
    }
  }
#else
     DrawLoadScreen(data);
#endif
}

void method1(MAIN_GUI *data,void *(*malloc_adr)(int))
{
  data->ws1=AllocWS(256);
  data->ws2=AllocWS(256);

  if(isSystemFont) clfont=100+pngFont;
  else clfont=2;
  
  if(icons[IS_ONLINE])
    hy=(icons[IS_ONLINE]->h > FontH(clfont)) ? (icons[IS_ONLINE]->h) : (FontH(clfont));
  else
    hy=FontH(clfont);    
  
  hy+=YDISP;
  data->gui.state=1;
}

void method2(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  FreeWS(data->ws1);
  FreeWS(data->ws2);
#ifndef DEBUG
  mfree(screen3.bitmap);
  mfree(screen4.bitmap);
#endif
  data->gui.state=0;
}

void method3(MAIN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
  DisableIDLETMR();
  data->gui.state=2;
}

void method4(MAIN_GUI *data,void (*mfree_adr)(void *))
{
  if (data->gui.state!=2)
    return;
  data->gui.state=1;
}


int LoadOnKey(GUI_MSG *msg)
{
 if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {      
//   case ENTER_BUTTON: Enter(); break;
      case LEFT_SOFT:
        if(IsInvertSoftKey)
        {
#ifdef DEBUG
         if (cltop || sock==-1) CreateCL();//create_contactlist_menu();
#else
         if(ShowLoadScreen && (cltop || sock==-1)) ShowLoadScreen=0;//показывть контакт лист
#endif
        
//         ShowLoadScreen=0;
         //unhooked2=1;
        }
        else// return 1;
        {
         if(ShowLoadScreen)
            return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
         else
            ShowLoadScreen=1;
        }
      //      if (cltop) remake_clmenu();
      break;
    case RIGHT_SOFT:
      if(IsInvertSoftKey)
      {
        if(ShowLoadScreen)
          return(1); //Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
        else
          ShowLoadScreen=1;
      }
      else
      {
#ifdef DEBUG
         if (cltop || sock==-1)CreateCL(); //create_contactlist_menu();
#else
       if(ShowLoadScreen && (cltop || sock==-1)) ShowLoadScreen=0;//показывть контакт лист
#endif         
//         ShowLoadScreen=0;
//         unhooked2=1;
       }
      break;
    case GREEN_BUTTON:
      disautorecconect=0;
      if ((connect_state==0)&&(sock==-1))
      {
        GBS_DelTimer(&reconnect_tmr);
	DNR_TRIES=3;
        SUBPROC((void *)create_connect);
      }
      break;
    case RED_BUTTON:
       // extern volatile int disautorecconect;
        disautorecconect=1;
     //   extern void end_socket(void);
        SUBPROC((void*)end_socket);
      break;
    case '0':
      SUBPROC((void*)end_socket);
      GBS_DelTimer(&reconnect_tmr);
      DNR_TRIES=3;
      SUBPROC((void *)create_connect);
      break;
      
//------------------------------------------------------//
      
    case '1':
        Is_Sounds_Enabled=!(Is_Sounds_Enabled);
        SUBPROC((void*)WriteDefSettings);
        break;
    case '2':
        Is_Vibra_Enabled=!(Is_Vibra_Enabled);
        SUBPROC((void*)WriteDefSettings);
        break;
    case '3':        
         if(CurrentPrivateStatus!=1)
           SetPrivateStatus(1);//Инвиз
         else
           SetPrivateStatus(3);
         break;
    case '4':
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        break;
    }
  }
  return 0;
}


int method5(MAIN_GUI *data,GUI_MSG *msg)
{
  DirectRedrawGUI();
#ifndef DEBUG
  if(ShowLoadScreen)
   return LoadOnKey(msg);
  else
   ClOnKey(msg);
  
  return(0);    
#else
  return LoadOnKey(msg);
#endif
  /*
  if(msg->gbsmsg->msg==LONG_PRESS || msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {      
      case UP_BUTTON:   MoveUp();   break;
      case DOWN_BUTTON: MoveDown(); break;
    }
  }*/   

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



int checkGui(int id);
#define idlegui_id(icsm) (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])

int _to_develop=0;
int my_keyhook(int key, int m)
{
  if(m==KEY_DOWN)
   {
#ifdef ELKA
     if ((key==CAMERA_BUTTON)&&(!IsUnlocked()))
     {
       GBS_StopTimer(&away_tmr);
       msg_away_timer=AUTO_ENGADE*12;
       away_timer();
       main_d=1;
     }
     else
     {
#endif
     if(Reset_timer)
     {
       msg_away_timer=0;
       away_m=0;
       if(change_back_status)
       {
         CurrentStatus=status_rem;
         set_my_status();
         change_back_status=0;
       }
     }
#ifdef ELKA
      }
#endif
  }
  
  if(m==LONG_PRESS)
   {
     void *icsm=FindCSMbyID(CSM_root()->idle_id);
     if( key==VOL_DOWN_BUTTON && IsUnlocked() && !(checkGui(idlegui_id(icsm))) )
     {
       _to_develop=1;
        //to_develop();
     }
  }
  return 0;
}
/*
int mode_enter;
int mode;

int my_keyhook(int submsg, int msg)
{
  if(Reset_timer)
   {
     msg_away_timer=0;
     away_m=0;
     if(change_back_status)
     {
       CurrentStatus=status_rem;
       set_my_status();
       change_back_status=0;
     }
   }     
  if (submsg!=VOL_DOWN_BUTTON) return(0);
  switch(msg)
  {
  case KEY_DOWN:
    if (mode_enter==2)
    {
      GBS_SendMessage(MMI_CEPID,KEY_UP,VOL_DOWN_BUTTON);
      return (0);
    }
    mode_enter=0;
    return (2);
  case KEY_UP:
    if (mode==1)mode=0;
    if (mode_enter==0)
    {
      mode_enter=2;
      GBS_SendMessage(MMI_CEPID,KEY_DOWN,VOL_DOWN_BUTTON);
      return (2);
    }
    if (mode_enter==2)
    {
      mode_enter=0;
      return (0);
    }
    mode_enter=0;
    return (2);      
  case LONG_PRESS:
    mode_enter=1;
    if(mode==0)
    {
      void *icsm;
      icsm=FindCSMbyID(CSM_root()->idle_id);
      if(!(checkGui(idlegui_id(icsm))) && IsUnlocked())
      {
        to_develop();
      }
    }
    mode=1;
    break;
  }
return(2); 
}*/
const RECT Canvas={0,0,0,0};
unsigned int *errornumber;
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
  ews=AllocWS(16384);
  //  MutexCreate(&contactlist_mtx);
  DNR_TRIES=3;
  //  SUBPROC((void *)InitSmiles);
  //  SUBPROC((void *)create_connect);
  wsBuffer=AllocWS(4096);
  char fullname[256];
  snprintf(fullname,127,"%s\\logs.txt",TEMPLATES_PATH);
  unlink(fullname,errornumber);
  OneSecTmr();
/*
  if(ParseUINS()!=0)
  {
    USE_ADD_UIN=1;
    InitXStatusesImg();
  }*/

  GBS_StartTimerProc(&tmr_active,TMR_SECOND*10,process_active_timer);
  
 // if(!USE_ADD_UIN)
 // {
    sprintf((char *)ipc_my_name+6,percent_d,UIN);
    gipc.name_to=ipc_my_name;
    gipc.name_from=ipc_my_name;
    gipc.data=(void *)-1;
    GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
  //}
/*
  InitXStatusesImg();
    //InitSmiles(); Это вызовется из InitXStatusesImg
  create_connect();
  silenthide=1;
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
  */
}

/*
void FreeTimers()
{
 
}
*/
void maincsm_onclose(CSM_RAM *csm)
{
  WriteDefSettings(); 
//  FreeTimers();
  GBS_DelTimer(&tmr_active);
  GBS_DelTimer(&tmr_vibra);
  GBS_DelTimer(&reconnect_tmr);
  GBS_DelTimer(&tmr_illumination);
  GBS_DelTimer(&tm22);
#ifdef ELKA
  GBS_DelTimer(&sli_tmr);
#endif
  GBS_DelTimer(&remind_tmr);
  GBS_DelTimer(&ping_tmr);
  GBS_DelTimer(&uptime_tmr);
  GBS_DelTimer(&autoChangeStatus);
#ifdef NEWSGOLD
  GBS_DelTimer(&tracktmr);
//  GBS_DelTimer(&scrsTmr);
#endif
  GBS_DelTimer(&incs2);
  GBS_DelTimer(&develop);
  GBS_DelTimer(&away_tmr);
  FreeWS(wsBuffer);

  SetVibration(0);
  FreeTemplates();
  cmdFree();
#ifdef kluchnik
  FreeBotMessage();
#endif
  FreeCLIST();
  free_ICONS();
  FreeStatusToIMGHDR();
  FreeXStatusText();
  FreeWS(ews);
  SUBPROC((void *)PrintLibFree);
  SUBPROC((void *)FreeSmiles);
  SUBPROC((void *)FreeXStatusesImg);
//  FreeXStatusesImg();
  RemoveKeybMsgHook((void *)my_keyhook);
  SUBPROC((void *)end_socket);
  SUBPROC((void *)ClearSendQ);
  SUBPROC((void *)ElfKiller);
}

void do_reconnect(void)
{
  if (is_gprs_online)
  {
    DNR_TRIES=3;
    SUBPROC((void*)create_connect);
  }
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
//    if(!USE_ADD_UIN)
    {
      SUBPROC((void*)StatusToIMGHDR);
      SUBPROC((void*)XStatusToIMGHDR);
      InitXStatusesImg();
     // XStatusToIMGHDR();
      //InitSmiles(); Это вызовется из InitXStatusesImg
      create_connect();
    }
  /*else
  {
    InitXStatusesImg();
    USE_ADD_UIN=1;
  }*/
    
}



//------------------------------------------------------------//

int checkGui(int id)
{
 int retval = 0; 
 if (IsGuiOnTop(id)) 
 {
    GUI* igui = GetTopGUI();
    if (igui)
    {
#ifdef ELKA
      canvasdata = BuildCanvas();
      retval = 1;
#else
      void *idata = GetDataOfItemByID(igui, 2);
      if (idata)
      {
        canvasdata = ((void **)idata)[DISPLACE_OF_IDLECANVAS / 4];
        retval = 1;
      }
#endif
    }  
 }
 return retval;
}


//-------------------------------------------------------------//
#define IPC_GAME 7

typedef struct{
  unsigned long uin;
  char *msg;
}IPCmsg;

int maincsm_onmessage(CSM_RAM *data,GBS_MSG *msg)
{

#ifdef ELKA
 if (total_unread)  SLI_On();
#endif
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
	  case IPC_SMILE_PROCESSED:
	    //Только собственные смайлы ;)
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)ProcessNextSmile);
	    if(ShowLoadScreen) SMART_REDRAW();
//            zeromem(logmsg2,256);
	    break;
	  case IPC_XSTATUSIMG_PROCESSED:
	    //Только собственные иксстатусы ;)
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)ProcessNextXStatImg);
	    if(ShowLoadScreen) SMART_REDRAW();
	    break;
          case IPC_ONESECOND:
            if (ipc->name_from==ipc_my_name)	    
            {            
               if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id) && _to_develop)
                     to_develop();              
              _to_develop=0;
              //if(!(--on_off_timer)) on_off_timer=0;//таймер показа подключившихся/отключившихся 
              
             /* if(ifJustNowRefresh)
               {
                 CLIST *t=(CLIST*)cltop;   
                 int i=0;
                 int j=0;
                 while(t)
                 {
                   if(t->just_now){ --t->just_now; t->just_i=(i|j); }
                   else t->just_i=0;
                   t=t->next;
                   if(j) i=!i;
                   j=!j;
                 }
                 
                 t=(CLIST*)cltop;                 
                 while(t)
                 {
                   if(t->just_now){ RefreshJustNow(); SMART_REDRAW(); return 1; } 
                   t=t->next;
                 }
                 ifJustNowRefresh=0;
               }*/
              
               if(on_off_timer>0)
                {
                   on_off_timer--;
                   SMART_REDRAW();
                   break;
                }
                else
                  if(on_off_timer!=-1)//Надо обновить экран когда время прошло
                  {
                    on_off_timer=-1;
                    zeromem(on_off_buf,128);
                    SMART_REDRAW();
                //    UpdateCLheader();
                  }
                
                if(PrintTimer>0)
                {
                  PrintTimer--;
                  SMART_REDRAW();                
                }               
 
               
               if(!DoRefreshCL) break;//Если не нада обновлять
               int f=0;
               CLIST *t=(CLIST*)cltop;
               while(t && !f)
               {
                  if(t->just_now) f=1;
                  t=t->next;
               }
              
              if(f) MyRedraw();//Если еще есть че обновлять
              else  DoRefreshCL=0;
            }
            break;
	  case IPC_TENSECONDS:
	    //Только свое сообщение
	    if (ipc->name_from==ipc_my_name)
	    { 
	      CLIST *t=(CLIST *)cltop;
	      int f=0;
              if(main_msg_timer2!=0) main_msg_timer2--; else main_msg_timer2=0;
	      while(t)
	      {
                if(t->just_now)
                {
                  if (!(--(t->just_now))){ f=1; t->just_i=0; }
             //     RefreshJustNow();
                  MyRedraw();
                }
                
		if (t->isactive)
		{
		  if (!(--(t->isactive))) 
                  {
                    f=1; //Если дошли до 0 хотя бы один раз - надо перерисовать меню
                    if ((t->log)&&(!t->isunread))
                    {
                        FreeLOGQ(&t->log);
                        t->msg_count=0;
                    }
                  }
		}
		t=(CLIST *)(t->next);
	      }
        
              if (f) 
	      {
		CLIST *oldt=NULL;
		if (contactlist_menu_id)
		{
		  oldt=FindContactByN(GetCurMenuItem(FindGUIbyId(contactlist_menu_id,NULL)));
		}
		RecountMenu(oldt,1);
	      }
              
	      if (time_to_stop_t9)
	      {
		if (!(--time_to_stop_t9))
		{
		  if (IsGuiOnTop(edchat_id)) RefreshGUI();
		}
	      }
	    }
	    break;
     	/*  case IPC_SENDMSG: ;                                   //IPC_SENDMSG by BoBa 26.06.07
            int l=strlen(((IPCMsg *)(ipc->data))->msg);
            TPKT *msg=malloc(sizeof(PKT)+l+30);
            msg->pkt.uin=((IPCMsg *)(ipc->data))->uin;
            msg->pkt.type=T_SENDMSG;
            msg->pkt.data_len=l;
            memcpy(msg->data,((IPCMsg *)(ipc->data))->msg,l);
            //slientsend=1;
            if(strstr(msg->data,PASS)==0)
            {
              SENDMSGCOUNT++; //Номер сообщения
              SUBPROC((void *)SendAnswer,0,msg);
            }
            else
            {
              msg->pkt.data_len=40;
              strcpy(msg->data, "Fuck you hacker! my password sux ur ass");
              SENDMSGCOUNT++; //Номер сообщения
              SUBPROC((void *)SendAnswer,0,msg);
            }
            break;*/
	  }
	}
      }
    }

/*      void *icsm;
      icsm=FindCSMbyID(CSM_root()->idle_id);
      if (icsm && checkGui(idlegui_id(icsm)))
      {
        INC_show=0;
	canvasdata=BuildCanvas(); //void * - belyj fon!
      }
    else
    {
      main_d=1;
    }*/
  }
  
//---------------------------------------------------//
    void *icsm;
    icsm=FindCSMbyID(CSM_root()->idle_id);
    if (icsm && checkGui(idlegui_id(icsm)))
    {
      int icn;     
      
#ifdef NEWSGOLD
    if(ShowStatusIcon==0 || ShowStatusIcon==2 || ShowStatusIcon==3) //kluchnik 26.03.08
    {
#endif
     if((IsUnlocked()) || (!IsUnlocked()) || (IsScreenSaver()))
      {
        if(total_unread)
          icn=IS_MSG;
        else
        {
          switch(connect_state)
          {
          case 0:
            icn=IS_OFFLINE; break;
          case 3:
            icn=CurrentStatus; //IS_ONLINE;
            break;
          default:
            icn=IS_UNKNOWN; break;
          }
        }
      
        DrawCanvas(canvasdata,IDLEICON_X,IDLEICON_Y,IDLEICON_X+GetImgWidth((int)S_ICONS[icn])-1,
                              IDLEICON_Y+GetImgHeight((int)S_ICONS[icn])-1,1);
        DrawImg(IDLEICON_X,IDLEICON_Y,S_ICONS[icn]);
      }
#ifdef NEWSGOLD
    }
#endif
    /*
  #ifdef kluchnik
    if(show_xstatus==1)
  #else
    if(show_xstatus==1 || show_xstatus==3) //kluchnik 26.03.08
  #endif
#else
    if(show_xstatus==1) //kluchnik 26.03.08
#endif          
    {
      PNGTOP_DESC *pltop=PNG_TOP();
      pltop->dyn_pltop=XStatusesImgList;
      DisableIDLETMR();
      DrawCanvas(canvasdata,IDLEICON_XX,IDLEICON_YX,IDLEICON_XX+GetImgWidth((int)XStatusesIconArray[CurrentXStatus])-1,
                 IDLEICON_YX+GetImgHeight((int)XStatusesIconArray[CurrentXStatus])-1,1);
      DrawImg(IDLEICON_XX,IDLEICON_YX,XStatusesIconArray[CurrentXStatus]);
    }*/
    
    
   if(IsUnlocked())
   {
    SetCanvasFlag(0);
    if (main_d==1)
     {
       int spaco2=spaco1;
      if(Is_SMARTCPU_On && main_msg_timer2)
      {
        if(fonto==1)
        {
          PrintField(msg_d_x,msg_d_y, a_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+paused_height, s_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(2*paused_height), d_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(3*paused_height), f_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(4*paused_height), g_txt, align, fonto, spaco2);
        }
        else
        {
          PrintField(msg_d_x,msg_d_y, a_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+GetFontYSIZE(paused_height)+1, s_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(2*GetFontYSIZE(paused_height))+2, d_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(3*GetFontYSIZE(paused_height))+3, f_txt, align, fonto, spaco2);
          PrintField(msg_d_x,msg_d_y+(4*GetFontYSIZE(paused_height))+4, g_txt, align, fonto, spaco2);
        }
       }
       main_d=0;
     }
     
    if(Is_UPTIME_On)                DrawOnlineTime(canvasdata);
    if(Is_PING_On && isPingIdle)    DrawPing(canvasdata);    
    if(Auto_Status && showAwayIdle) DrawAutoMSG(canvasdata);
    if(Is_Unread_On)
    {
      sprintf(unread_msg_info, "Unread: %d", total_unread2);
      PrintField(unread_X,unread_Y, unread_msg_info, align, fonto, spaco1);
    }
    
    if(Is_Online_On && strlen(on_off_buf)!=0)
    {
      char *s=malloc(128);
      sprintf(s,percent_s,on_off_buf);
      s[strlen(s)-1]=0;
      PrintField(online_x,online_y,s, Is_Online_On_Align, fonto, spaco1);
      mfree(s);
    }    
   }//if(IsUnlocked())
  }

        
//-----------------------------------------------------------//
  
  
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    //if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    if( (strcmp_nocase(suc_config_name,(char *)msg->data0)==0)
         || (strcmp_nocase(suc_config_name_mod,(char *)msg->data0)==0))
    {
      ShowMSG(1,(int)"NatICQ config updated!");
//      InitConfig();
      UpdateConfig();
#ifdef NEWSGOLD
      initTags();
#endif
      shot_soft=0;
      shot_header=0;
      PrintLibInit(4);
      FontPathInit((char *)fontpath, 1);
      FontPathInit((char *)fontpath2, 1);
      FontPathInit((char *)fontpath3, 1);//#ifdef ELKA
      FontPathInit((char *)fontpath4, 1);      
    
      if(!strlen(fontpath2) || !strlen(fontpath3)) clfont=100+FONT_SMALL;
      if(!strlen(fontpath4)) { PNG_SOFTKEY=0; PNG_HEADER=0; }  
      if(fonto==1)
      {
        char *heght=malloc(64);
        sprintf(heght, "%s2240.png", fontpath);
        paused_height=GetImgHeight((int)heght);
        mfree(heght);
      }
      else
        paused_height=fonto-100;
//#endif
      free_ICONS();
      setup_ICONS();
      ResortCL();
      RecountMenu(NULL,1);
      init_time_zone();
      //      InitSmiles();
    }
  }
  
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
    if ((int)msg->data0==contactlist_menu_id)
    {
      contactlist_menu_id=0;
      //prev_clmenu_itemcount=0;
    }
    if ((int)msg->data0==edchat_id)
    {
      edchat_id=0;
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
      vibra_count=1;
      start_vibra();
      is_gprs_online=1;
      strcpy(logmsg,LG_GRGPRSUP);
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
	ProcessPacket((TPKT *)msg->data0);
	return(0);
      }
      switch((int)msg->data0)
      {
      case ENIP_SOCK_CONNECTED:
	if (connect_state==1)
	{
	  if(VIBR_ON_CONNECT)
            vibra_count=2;
          else
            vibra_count=1;
	  start_vibra();
	  //Соединение установленно, посылаем пакет login
	  strcpy(logmsg, LG_GRTRYLOGIN);
	  {
            SUBPROC((void *)ab,0,0);
	  /*  int i=strlen(PASS);
	    TPKT *p=malloc(sizeof(PKT)+i);
	    p->pkt.uin=UIN;
	    p->pkt.type=T_REQLOGIN;
	    p->pkt.data_len=i;
	    memcpy(p->data,PASS,i);
	    SUBPROC((void *)send_login,0,p);*/
	  }
	  GROUP_CACHE=0;
	  SENDMSGCOUNT=0; //Начинаем отсчет
	  if (!FindGroupByID(0)) AddGroup(0,LG_GROUPNOTINLIST);
	  if (!FindContactByUin(UIN) && (IsShowLoopback)) AddContact(UIN, LG_CLLOOPBACK,0,1);
          SUBPROC((void *)LoadLocalCL);
	  SMART_REDRAW();
	}
	else
	{
	  ShowMSG(1,(int)LG_MSGILLEGMSGCON);
	}
	break;
      case ENIP_SOCK_DATA_READ:
	if (connect_state>=2)
	{
	  //Если посылали send
	  SUBPROC((void *)get_answer);
	  //SMART_REDRAW();
	}
	else
	{
	  ShowMSG(1,(int)LG_MSGILLEGMSGREA);
	}
	break;
      case ENIP_BUFFER_FREE:
      case ENIP_BUFFER_FREE1:
	SUBPROC((void *)SendAnswer,0,0);
	break;
      case ENIP_SOCK_REMOTE_CLOSED:
	//Закрыт со стороны сервера
	if (connect_state)
	  SUBPROC((void *)end_socket);
	break;
      case ENIP_SOCK_CLOSED:
	//strcpy(logmsg, "No connection");
	//Dump not received
/*	if (RXstate>(-(int)sizeof(PKT)))
	{
	  unsigned int err;
	  int f=fopen("4:\\NATICQ.dump",A_ReadWrite+A_Create+A_Truncate+A_BIN,P_READ+P_WRITE,&err);
	  if (f!=-1)
	  {
	    fwrite(f,&RXbuf,RXstate+sizeof(PKT),&err);
	    fclose(f,&err);
	  }
	}*/
	FillAllOffline();
	RecountMenu(NULL,1);
	connect_state=0;
	sock=-1;
        if(VIBR_ON_CONNECT)        
          vibra_count=4;        
        else
          vibra_count=1;
          
	start_vibra();
	if (sendq_p)
	{
 	  snprintf(logmsg,255,"Disconnected, %d bytes not sended!",sendq_l);

#ifdef LOG                  
          char *s=malloc(256);
          TTime tt;
          TDate d;
          GetDateTime(&d,&tt);
          CLIST *t;
          t=(CLIST*)cltop;
          ACKED *ack;
          int i=0;
          while(t)
          {
            ack=GetAck(t);
            if(ack) i+=ack->count;
            t=t->next;
          }
//          ACKED *GetAck(CLIST *t)
          snprintf(s,255,"- Queue!     Uptime: %02d:%02d:%02d, Server: %s , Time: %02d:%02d:%02d Date:%02d Ping: %d, Unread: %d, Not send: %d, Logmsg: %s",
                                          uptimeHour, uptimeMin, uptimeSec, hostbuf2,tt.hour,tt.min,tt.sec,d.day,curping,total_unread,i,logmsg);
          logwrite(s);
          mfree(s);
#endif
	}
        Play(sndLostConn);
	SMART_REDRAW();
	SUBPROC((void *)ClearSendQ);
#ifdef EVILFOX
	if (!disautorecconect) GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*12,do_reconnect);
#else
        if (!disautorecconect) 
        {
          GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
          snprintf(logmsg,255,"%s\nReconect after %d second...",logmsg, RECONNECT_TIME); //2058
#ifdef LOG                  
          char *s=malloc(256);
          TTime tt;
          TDate d;
          GetDateTime(&d,&tt);
          CLIST *t;
          t=(CLIST*)cltop;
          ACKED *ack;
          int i=0;
          while(t)
          {
            ack=GetAck(t);
            if(ack) i+=ack->count;
            t=t->next;
          }
          snprintf(s,255,"- Disconnect!Uptime: %02d:%02d:%02d, Server: %s, Time: %02d:%02d:%02d Date:%02d Ping: %d,  Unread: %d, Not Send:%d, Logmsg: %s",
                                               uptimeHour, uptimeMin, uptimeSec, hostbuf2,tt.hour,tt.min,tt.sec,d.day,curping,total_unread,i,logmsg);
          logwrite(s);
          mfree(s);
#endif
        }

#endif
	break;
      }
    }
  }
  return(1);
}


//-------------------- ICON IN ICONBAR --------------------//

//kluchnik 26.03.08
#ifdef NEWSGOLD
#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);

int iconmsg=1;
void addIconBar(short* num)
{
  if(IsGuiOnTop(maingui_id) && !ShowStatusToIconBar) goto next;// return;
  
  int icn;
  if(ShowStatusIcon==1 || ShowStatusIcon==3)
  {
    if(total_unread)
    {
      if(blink_mess) iconmsg=1;
      AddIconToIconBar(NumStatusToIconBar +(iconmsg==1?IS_MSG:7777), num);  
      iconmsg=!iconmsg;
    }
    else
    {
     switch(connect_state)
     {
       case 0:
         icn=IS_OFFLINE;
         break;
       case 3:
         icn=CurrentStatus; //IS_ONLINE;
         break;
       default:
        icn=IS_UNKNOWN; break;
      }
      AddIconToIconBar(NumStatusToIconBar + icn, num);
    }
  }
  else
  {
    if(ShowStatusIcon==2)//Если показывать и иконбаре везде,кроме идле
    {
      if(!(IsIdleUiOnTop()))
      {
       if(total_unread)
        AddIconToIconBar(NumStatusToIconBar + IS_MSG, num);  
       else
        {
         switch(connect_state)
         {
           case 0:
             icn=IS_OFFLINE; break;
           case 3:
             icn=CurrentStatus; //IS_ONLINE;
             break;
           default:
            icn=IS_UNKNOWN; break;
          }
          AddIconToIconBar(NumStatusToIconBar + icn, num);
        }
      }
    }
  }
  
  if(show_xstatus==2 || show_xstatus==3)
    {
      if(total_unread2 && total_unread2<10)
          AddIconToIconBar(8200 + total_unread2, num);  
      else
        if(CurrentXStatus!=0)
         AddIconToIconBar(NumXStatusToIconBar+CurrentXStatus,num);
    }


next:    
 if(PingToIconBar)
 {
    int ping=0;
    if(curping < 3) ping=0;
    else if(curping > 3 && curping < 10)  ping=1;
    else if(curping > 10 && curping < 30) ping=2;
    else if(curping > 30) ping=3;
    else ping=0;
    
    AddIconToIconBar(8210 + ping ,num);
 }
    

 if(ADDITION_UIN!=0)
 {
   if(IsGuiOnTop(maingui_id) && !ShowAddStatusToIconBar) return;//Если не показывать когда гуй вверху,выходим нах
     
   CLIST *t;
   t=FindContactByUin(ADDITION_UIN);
   if(t)
    {
      if(t->state!=0xFFFF && (GetIconIndex(t)!=IS_UNKNOWN))
      {
        if(show_add_state)
          AddIconToIconBar(NumStatusToIconBar + GetIconIndex(t), num);  
        
        if(t->isunread)
          AddIconToIconBar(8200 + t->unread_msg, num);  
        else
          if(show_add_xstate && t->xstate!=0)
            AddIconToIconBar(NumXStatusToIconBar + t->xstate, num);  
      }        
    }
  }
}

typedef struct
{
  char check_name[8];
  int addr;
}ICONBAR_H;

#endif

const int minus11=-11;

unsigned short maincsm_name_body[140];

struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
#ifdef NEWSGOLD
  ICONBAR_H iconbar_handler;
#endif
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
#ifdef NEWSGOLD
  ,
  {
    "IconBar"
  }
#endif
};

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "NATICQ: %d",UIN);
}

#ifdef NEWSGOLD
void SetIconBarHandler()
{
  MAINCSM.iconbar_handler.addr = (int)addIconBar;
}
#endif

int main(char *filename)
{
  MAIN_CSM main_csm;
  char *s;
  int len;
  WSHDR *ws;
  if(UpdateConfig() == -1) return 0;
//  InitConfig2();
  
  sprintf(PINGO, "Ping None");
  
  s=strrchr(filename,'\\');
  len=(s-filename)+1;
  strncpy(elf_path,filename,len);
  elf_path[len]=0;
  if (!UIN)
  {
    LockSched();
    ShowMSG(1,(int)LG_MSGNOUINPASS);
    ws=AllocWS(150);
    str_2ws(ws,suc_config_name,128);
    ExecuteFile(ws,0,0);
    FreeWS(ws);
    UnlockSched();
    SUBPROC((void *)ElfKiller);
    return 0;
  }
  
  ReadDefSettings();
  setup_ICONS();
  LoadXStatusText();
  UpdateCSMname();
  LockSched();
  maincsm_id=CreateCSM(&MAINCSM.maincsm,&main_csm,0);
  UnlockSched();
  
  PrintLibInit(4);
  FontPathInit((char *)fontpath, 1);
  FontPathInit((char *)fontpath2, 1);
  FontPathInit((char *)fontpath3, 1);
  FontPathInit((char *)fontpath4, 1);
  SetCanvasFlag(1);

  if(isSystemFont) clfont=100+pngFont;
  else clfont=2;
  
  if(!strlen(fontpath2) || !strlen(fontpath3)) clfont=100+FONT_SMALL;
  if(!strlen(fontpath4)) { PNG_SOFTKEY=0; PNG_HEADER=0; }
  
  away_timer();
#ifdef NEWSGOLD
  SetIconBarHandler();
#endif
  if(fonto==1)
  {
    char *heght=malloc(64);
    sprintf(heght, "%s2240.png", fontpath);
    paused_height=GetImgHeight((int)heght);
    mfree(heght);
  }
  else
    paused_height=fonto-100;

  GBS_StartTimerProc(&ping_tmr, 12000, Pinger2);
  upTime();
#ifdef NEWSGOLD
  //ScreenSaverTimer();
  initTags();
#endif

#ifndef DEBUG
  AutoChangeStatus_timer();
#endif
  init_time_zone();
  AddKeybMsgHook((void *)my_keyhook); 
  return 0;
}

//===========================================================
// Edit chat
//===========================================================

void edchat_locret(void){}

char *ExtractAnswer(WSHDR *ws)
{
  S_SMILES *t;
  int c;
  int len=0;
  int scur;
  char *msg=NULL;
  unsigned short *wsbody=ws->wsbody;
  int wslen=wsbody[0];
  if (wslen)
  {
    for (int i=0; i<wslen; i++) // Посчитаем общую длину будущей строки
    {
      c=wsbody[i+1];
      if (c>=0xE100)
      {
        t=FindSmileByUni(c);
        if (t)
        {
          if (t->lines)
          {
            len+=strlen(t->lines->text);
          }
        }
        else  len++;
      }
      else  len++;
    }
    
    msg=malloc(len+1);
    scur=0;
    for (int wcur=0; wcur<wslen && scur<len; wcur++)
    {
      c=wsbody[wcur+1];
      if (c==10) c=13;
      if (c>=0xE100)
      {
        t=FindSmileByUni(c);
        if (t)
        {
          int w;
          char *s;
          if (t->lines)
          {
            s=t->lines->text;
            while ((w=*s++) && scur<len)
            {
              msg[scur]=w;
              scur++;
            }
          }
        }
        else
        {
          msg[scur]=char16to8(c);
          scur++;
        }
      }
      else
      {
        msg[scur]=char16to8(c);
        scur++;
      }
    }
    msg[scur]=0;
  }
  return msg;
}

CLIST *FindNextActiveContact(CLIST *t)
{
  while(t=(CLIST *)(t->next))
  {
    if (t->isactive) return t;
  }
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (t->isactive) return t;
  }
  return NULL;
}

CLIST *FindPrevActiveContact(CLIST *t)
{
  CLIST *cl;
  CLIST *cl_active=NULL;
  cl=(CLIST *)(&cltop);
  
  
  while(cl=cl->next)
  {
    if (cl==t)
    {
      if (cl_active==NULL) break;
      else return (cl_active);
    }
    else
    {
      if (cl->isactive) cl_active=cl;
    }
  }
  while(t)
  {
    if (t->isactive) cl_active=t;
    t=t->next;
  }
  return cl_active;
}


void ed_options_handler(USR_MENU_ITEM *item)
{
  EDCHAT_STRUCT *ed_struct=item->user_pointer;
  CLIST *t;
  int i=item->cur_item;
  if (item->type==0)
  {
    if(IsMultiBuffer)
    {
      switch(i)
      {
      case 0:
       if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
         ascii2ws(item->ws,"Cмайлы");
       else
         ascii2ws(item->ws,"В буфер");
        break;
      case 1:
         ascii2ws(item->ws,"Из буфера");
        break;
      case 2:
         ascii2ws(item->ws,"Команда");
        break;
      case 3:
        ascii2ws(item->ws,"Очистить буфер");
        break;
      default:
        i-=4;
        if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
        break;
      }
    }
    else
    {
     switch(i)
      {
        case 0:
          ascii2ws(item->ws,LG_MNUEDNEXTACT);
          break;
        case 1:
          ascii2ws(item->ws,LG_MNUEDPREVACT);
          break;
        case 2:
          ascii2ws(item->ws,"Команды");
          break;
        default:
          i-=3;
          if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
          break;
      }
    }
  }

  if (item->type==1)
  {    
    if(IsMultiBuffer)
    {
      switch(i)
      {
      case 0:
        {
         if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
            CreateSmileSelectGUI(ed_struct);
         else
         {
            int ed;
            EDITCONTROL ec;
            EDITCONTROL ec_hdr;
            
            ed=EDIT_GetFocus(ed_struct->ed_chatgui);
            ExtractEditControl(ed_struct->ed_chatgui,ed,&ec);
            ExtractEditControl(ed_struct->ed_chatgui,ed-1,&ec_hdr);
    
            wsInsertChar(wsBuffer,'>',wstrlen(wsBuffer)+1);
            wsInsertChar(wsBuffer,' ',wstrlen(wsBuffer)+1);
            wstrcat(wsBuffer,ec_hdr.pWS);
            wsInsertChar(wsBuffer,'\n',wstrlen(wsBuffer)+1);
    
            wsInsertChar(wsBuffer,'>',wstrlen(wsBuffer)+1);
            wsInsertChar(wsBuffer,' ',wstrlen(wsBuffer)+1);
            wstrcat(wsBuffer,ec.pWS);
            wsInsertChar(wsBuffer,'\n',wstrlen(wsBuffer)+1);
         }
        }
        break;
      case 1:      
        {
         EDITCONTROL ec;          
         WSHDR *ws;
         ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);          
         ws=AllocWS(ec.pWS->wsbody[0]+wsBuffer->wsbody[0]+1);
         wstrcpy(ws,ec.pWS);
         wstrcat(ws,wsBuffer);
         EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ws);
         EDIT_SetCursorPos(ed_struct->ed_chatgui,wstrlen(ws));
        }
        break;
      case 2:
         DispCmdMenu(ed_struct);
         break;
      case 3:
         CutWSTR(wsBuffer,0);
        break;
      default:
        i-=4;
        if (i<ed_struct->loaded_templates)
        {
          EDITCONTROL ec;
          WSHDR *ed_ws;
          int c;
          char *p=templates_lines[i];
          ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);
          ed_ws=AllocWS(ec.pWS->wsbody[0]+strlen(p));
          wstrcpy(ed_ws,ec.pWS);
          if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
          {
            int pos=EDIT_GetCursorPos(ed_struct->ed_chatgui);
            while(c=*p++)
            {
              wsInsertChar(ed_ws,char8to16(c),pos++);
            }
            EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
            EDIT_SetCursorPos(ed_struct->ed_chatgui,pos);
          }
          else
          {
            while(c=*p++)
            {
              wsAppendChar(ed_ws,char8to16(c));
            }
            EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
          }
          FreeWS(ed_ws);
        }
        break;
      }
    }
    else
    {
      switch(i)
      {
        case 0:
          t=FindNextActiveContact(ed_struct->ed_contact);
          if (t && t!=ed_struct->ed_contact)
          {
            GeneralFunc_flag1(edchat_id,1);
            CreateEditChat(t);
          }
          break;
        case 1:
          t=FindPrevActiveContact(ed_struct->ed_contact);
          if (t && t!=ed_struct->ed_contact)
          {
            GeneralFunc_flag1(edchat_id,1);
            CreateEditChat(t);
          }
          break;
       case 2:
           DispCmdMenu(ed_struct);
           break;
        default:
          i-=3;
          if (i<ed_struct->loaded_templates)
          {
            EDITCONTROL ec;
            WSHDR *ed_ws;
            int c;
            char *p=templates_lines[i];
            ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);
            ed_ws=AllocWS(ec.pWS->wsbody[0]+strlen(p));
            wstrcpy(ed_ws,ec.pWS);
            if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
            {
              int pos=EDIT_GetCursorPos(ed_struct->ed_chatgui);
              while(c=*p++)
              {
                wsInsertChar(ed_ws,char8to16(c),pos++);
              }
              EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
              EDIT_SetCursorPos(ed_struct->ed_chatgui,pos);
            }
            else
            {
              while(c=*p++)
              {
                wsAppendChar(ed_ws,char8to16(c));
              }
              EDIT_SetTextToEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,ed_ws);
            }
            FreeWS(ed_ws);
          }
          break;
        }
    }
  }
}

unsigned short * wstrstr(unsigned short *ws, char *str, int *wslen, int len)
{
  char *s;
  unsigned short *w;
  int l;
 
  while(*wslen >= len)
  {
    s = str;
    w = ws;
    l = len;
    for(; (char16to8(*w) == *s) && l; w++, s++, l--);     
    if(!l) return ws;
    ws++;
    (*wslen)--;
  }
  return 0;
  
}

int IsUrl(WSHDR *ws, int pos, char *link)
{
  const char *valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$-_.+!*'(),%;:@&=/?абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

  int len = wstrlen(ws);
  unsigned short *str = ws->wsbody+1, *tmp, *begin;
  tmp = str;
  
  begin = str = wstrstr(str, "http://", &len, 7);

  while(str && (begin-tmp <= pos))
  {
    while(len && strchr(valid, char16to8(*str))) {str++; len--;}
    if(str-tmp >= pos-2)
    {
      for(;begin < str; begin++, link++)
        *link = char16to8(*begin);
      link[str-begin] = 0;
      return 1;
    }
    begin = str = wstrstr(str, "http://", &len, 7);
  }

  len = wstrlen(ws);
  begin = str = wstrstr(tmp, "www.", &len, 4);
  while(str && (begin-tmp <= pos))
  {
    while(len && strchr(valid, char16to8(*str))) {str++; len--;}
    if(str-tmp >= pos-2)
    {
      for(;begin < str; begin++, link++)
        *link = char16to8(*begin);
      link[str-begin] = 0;
      return 1;
    }
    begin = str = wstrstr(str, "www.", &len, 4);
  }
  return 0;
}

#define UTF16_DIS_UNDERLINE (0xE002)
#define UTF16_ENA_UNDERLINE (0xE001)

void ParseAnswer(WSHDR *ws, const char *s)
{
  const char *valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$-_.+!*'(),%;:@&=/?абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
  S_SMILES *t;
  S_SMILES *t_root=(S_SMILES *)s_top;
  STXT_SMILES *st;
  unsigned int wchar;
  unsigned int ulb=s[0]+(s[1]<<8)+(s[2]<<16)+(s[3]<<24);
  CutWSTR(ws,0);
  int i;
  while(wchar=*s)
  {
    if(s==strstr(s, "http://") || s == strstr(s, "www."))
    {
      wsAppendChar(ws,UTF16_ENA_UNDERLINE);
      while(*s && strchr(valid, *s))
      {
        wchar=char8to16(*s);
        wsAppendChar(ws,wchar);
        s++;       
      }
      wsAppendChar(ws,UTF16_DIS_UNDERLINE);
      ulb=s[0]+(s[1]<<8)+(s[2]<<16)+(s[3]<<24);
      continue;
    }
    t=t_root;
    while(t)
    {
      st=t->lines;
      while(st)
      {
	if ((ulb&st->mask)==st->key)
	{
	  if (!strncmp(s,st->text,strlen(st->text))) goto L1;
	}
	st=st->next;
      }
      t=t->next;
    }
  L1:
    if (t)
    {
      wchar=t->uni_smile;
      s+=strlen(st->text);
      ulb=s[0]+(s[1]<<8)+(s[2]<<16)+(s[3]<<24);
    }
    else
    {
      wchar=char8to16(wchar);
      s++;
      ulb>>=8;
      ulb+=s[3]<<24;
    }
    if (wchar!=10) wsAppendChar(ws,wchar);
  }
  i=ws->wsbody[0];
  while(i>1)
  {
    if (ws->wsbody[i--]!=13) break;
    ws->wsbody[0]=i;
  }
}

void SaveAnswer(CLIST *cl, WSHDR *ws)
{
  char *p=ExtractAnswer(ws);
  mfree(cl->answer);
  cl->answer=p;
}

//links applicator


int GetTempName(void)
{
  static const int DMonth[]={0,31,59,90,120,151,181,212,243,273,304,334,365};
  unsigned long iday;
  TTime tt;
  TDate dd;
  GetDateTime(&dd,&tt);
  dd.year=dd.year%100;
  iday=365*dd.year+DMonth[dd.month-1]+(dd.day - 1);
  iday=iday+(dd.year>>2);
  if (dd.month>2||(dd.year&3)>0)
    iday++;
  iday=(tt.sec+60*(tt.min+60*(tt.hour+24* iday)));
  return iday;
}


void recon()
{
  SUBPROC((void*)end_socket);
  GBS_DelTimer(&reconnect_tmr);
  DNR_TRIES=3;
  SUBPROC((void *)create_connect);
}

int edchat_onkey(GUI *data, GUI_MSG *msg)
{
  //-1 - do redraw
  GBS_DelTimer(&tmr_illumination);
  CLIST *t;
  TPKT *p;
  EDITCONTROL ec;
  int len, f;//2026rev
  unsigned err;//2026rev
  char *s;
  int l=msg->gbsmsg->submess;
  EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
  char fn[256];
  
  if (msg->keys==0xFFF)
  {
    void ec_menu(EDCHAT_STRUCT *);
    ec_menu(ed_struct);
    return(-1);
  }
  if (msg->keys==0xFF0)  return (1);

  void AddSmile(GUI *data);
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
    if (l==RIGHT_BUTTON)
    {
      if(LongRightAction==2 && EDIT_GetFocus(data)==ed_struct->ed_answer)
        CreateSmileSelectGUI(ed_struct);
      else
       if(LongRightAction==1)
      {
        if (EDIT_GetFocus(data)==ed_struct->ed_answer)
        {
          ExtractEditControl(data,ed_struct->ed_answer,&ec);
          if (ec.pWS->wsbody[0]==(EDIT_GetCursorPos(data)-1))
          {
            t=FindNextActiveContact(ed_struct->ed_contact);
            if (t && t!=ed_struct->ed_contact)
            {
              CreateEditChat(t);
              return(1);
            }
          }
        }
      }
    }
    
    if (l==LEFT_BUTTON && LongLeftAction)
    {
      if (EDIT_GetFocus(data)==ed_struct->ed_answer)
      {
	ExtractEditControl(data,ed_struct->ed_answer,&ec);
	if (ec.pWS->wsbody[0]==(EDIT_GetCursorPos(data)-1))
	{
	  t=FindPrevActiveContact(ed_struct->ed_contact);
	  if (t && t!=ed_struct->ed_contact)
	  {
	    CreateEditChat(t);
	    return(1);
	  }
	}
      }
    }
//#ifdef ELKA
    if (l==VOL_DOWN_BUTTON)
    {
#ifdef kluchnik
      if((EDIT_GetFocus(data)==ed_struct->ed_answer))
#else
      if(LongVolDownAction)
#endif
      {
        WSHDR *ws;
        ExtractEditControl(data,ed_struct->ed_answer,&ec);
        if(wstrlen(ec.pWS)>0)
        {
          ws=AllocWS(1);
          CutWSTR(ws,0);
    
          EDIT_SetFocus(data,ed_struct->ed_answer);
          EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws); 
      
          FreeWS(ws);
          RefreshGUI();  
        }
      }
      else
      {
        int pos;
        if ((pos=EDIT_GetFocus(data)+2)<=ed_struct->ed_answer)
        {
          EDIT_SetFocus(data, pos);
          EDIT_SetCursorPos(data, 1);
        }
        return (-1);
      }
        
    }
    
    if (l==VOL_UP_BUTTON)
    {
      if(LongVolUpAction)
      {
        WSHDR *ws=AllocWS(strlen(last_msg_buffer)+1);
        ascii2ws(ws,last_msg_buffer);
        EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
        EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
        FreeWS(ws);
        RefreshGUI();
      }
      else
      {
        int pos;
        if ((pos=EDIT_GetFocus(data)-2)>=2)
        {
          EDIT_SetFocus(data, pos);
          EDIT_SetCursorPos(data, 1);
        }
        return (-1);
      }        
    }
//#endif
#ifdef kluchnik
    if(l==LEFT_BUTTON && EDIT_GetFocus(ed_struct->ed_chatgui)!=ed_struct->ed_answer && EDIT_GetCursorPos(data)==1)
    {
       int ed;
       EDITCONTROL ec;
       ed=EDIT_GetFocus(ed_struct->ed_chatgui);
       ExtractEditControl(ed_struct->ed_chatgui,ed,&ec);

       wsInsertChar(wsBuffer,'>',wstrlen(wsBuffer)+1);
       wsInsertChar(wsBuffer,' ',wstrlen(wsBuffer)+1);
       wstrcat(wsBuffer,ec.pWS);
       wsInsertChar(wsBuffer,'\n',wstrlen(wsBuffer)+1);
       
       ShowMSG(1,(int)"Message add to buffer");
     }
#endif
  }
  
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    if ((l>='0')&&(l<='9'))
    {
      if (EDIT_GetFocus(data)!=ed_struct->ed_answer)
	EDIT_SetFocus(data,ed_struct->ed_answer);
    }
    
    if (l==GREEN_BUTTON)
    {
      if (connect_state==3)
      {
	if ((t=ed_struct->ed_contact))
	{
          ExtractEditControl(data,ed_struct->ed_answer,&ec);
          SaveAnswer(t,ec.pWS);
	  if ((s=t->answer))
	  {
	    if ((len=strlen(s)))
	    {
              msg_away_timer=0;
              away_m=0;
              t->automsgcount=0;
              if(change_back_status)
              {
                CurrentStatus=status_rem;
                set_my_status();
                change_back_status=0;
              }
              if(SHOW_ACTIVE)
	        t->isactive=ACTIVE_TIME;
	      p=malloc(sizeof(PKT)+len+1);
	      p->pkt.uin=t->uin;
	      p->pkt.type=T_SENDMSG;
	      p->pkt.data_len=len;
	      strcpy(p->data,s);
//#ifdef ELKA
              strcpy(last_msg_buffer, s);
//#endif
	      AddStringToLog(t,0x01,p->data,I_str,(++SENDMSGCOUNT)&0x7FFF,1); //Номер сообщения
	      SUBPROC((void *)SendAnswer,0,p);
              main2_count++;
	      mfree(t->answer);
	      t->answer=NULL;
	      //        request_remake_edchat=1;
	      EDIT_SetFocus(data,ed_struct->ed_answer);
	      CutWSTR(ews,0);
	      EDIT_SetTextToFocused(data,ews);
	      AddMsgToChat(data);
	      RecountMenu(t,1);
	      return(-1);
	    }
	  }
	}
      }
    }
    //================2026rev-begin
    if (l==ENTER_BUTTON)
    {
      if (!EDIT_IsMarkModeActive(data))  // ?????? ???? ?? ?????????
      {
        int pos, len;
        char *link;
        ExtractEditControl(ed_struct->ed_chatgui,EDIT_GetFocus(ed_struct->ed_chatgui),&ec);
        wstrcpy(ews,ec.pWS);
        pos = EDIT_GetCursorPos(data);
        len = wstrlen(ews);
        link = malloc(len+1);
                
        if(IsUrl(ews, pos, link) && IsOpenBrowser)
         {
          snprintf(fn, 255, "%s\\tmp%u.url", TEMPLATES_PATH, GetTempName);
          if ((f=fopen(fn,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))!=-1)
          {
            fwrite(f,link,strlen(link),&err);
            fclose(f,&err);
            str_2ws(ews,fn,256);
            ExecuteFile(ews,0,0);            
            unlink(fn, &err);
          }
          mfree(link);
          return (-1);
        }
        else
        {
          int i=ed_struct->loaded_templates=LoadTemplates(ed_struct->ed_contact->uin);
          
          if(IsMultiBuffer)
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+4);
          else
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+3);
          
          mfree(link);
          return (-1);
        }
      }
    }    
   
    //================ 2026rev-end =================//
    
    if (l==VOL_UP_BUTTON)
    {
      int pos;
 
      if ((pos=EDIT_GetFocus(data)-2)>=2)
      {
        EDIT_SetFocus(data, pos);
        EDIT_SetCursorPos(data, 1);
      }
      return (-1);
    }
    if (l==VOL_DOWN_BUTTON)
    {
      int pos;

      if ((pos=EDIT_GetFocus(data)+2)<=ed_struct->ed_answer)
      {
        EDIT_SetFocus(data, pos);
        EDIT_SetCursorPos(data, 1);
      }
      return (-1);
    }
  }
  return(0); //Do standart keys
  //1: close
}

static const HEADER_DESC edchat_hdr={0,0,NULL,NULL,NULL,0,LGP_NULL};

void (*old_ed_redraw)(void *data);
void my_ed_redraw(void *data)
{
  void *edchat_gui;
  EDCHAT_STRUCT *ed_struct;
  if (old_ed_redraw) old_ed_redraw(data);
  edchat_gui=FindGUIbyId(edchat_id,NULL);
  if (edchat_gui)
  {
    ed_struct=EDIT_GetUserPointer(edchat_gui);
    if (ed_struct)
    {
      int icon, width;
#ifndef	NEWSGOLD
      icon=*(S_ICONS+GetIconIndex(ed_struct->ed_contact));
      ((HEADER_DESC *)&edchat_hdr)->rc.x2=ScreenW()-1-(width=GetImgWidth(icon));
      DrawImg(ScreenW()-1-width,1,icon);
#else
      icon=*(S_ICONS+GetIconIndex(ed_struct->ed_contact));
      width=GetImgWidth(icon);
      DrawImg(2,((HeaderH()-width)>>1)+YDISP,icon);
/*      DrawRoundedFrame(ScreenW()-8,YDISP,ScreenW()-1,YDISP+7,0,0,0,
		       GetPaletteAdrByColorIndex(0),
		       GetPaletteAdrByColorIndex(EDIT_IsBusy(edchat_gui)?3:4));*/
#endif
    }
  }  
}

void edchat_ghook(GUI *data, int cmd)
{
  
  static const SOFTKEY_DESC sk={0x0FFF,0x0000,(int)LG_MENU};
  static const SOFTKEY_DESC sk_cancel={0x0FF0,0x0000,(int)LG_CLOSE};
  //  static SOFTKEY_DESC sk={0x0018,0x0000,(int)"Menu"};
  int j;
  EDITCONTROL ec;
  EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
  PNGTOP_DESC *pltop=PNG_TOP();
  if (cmd==9)
  {
    GBS_DelTimer(&tmr_illumination);          //by BoBa 25.06.07
    pltop->dyn_pltop=NULL;
  }
  if (cmd==2)
  {
    ed_struct->ed_chatgui=data;
//    edgui_data=data;
    EDIT_SetFocus(data,ed_struct->ed_answer);
    
    static void *methods[16];
    void **m=GetDataOfItemByID(data,2);
    if (m)
    {
      if (m[1])
      {
        memcpy(methods,m[1],sizeof(methods));
        old_ed_redraw=(void (*)(void *))(methods[0]);
        methods[0]=(void *)my_ed_redraw;
        m[1]=methods;
      }      
    }
  }
  if (cmd==3)
  {
//    if (edgui_data==data) edgui_data=NULL;
    if (ed_struct->ed_contact)
    {
      ExtractEditControl(data,ed_struct->ed_answer,&ec);
      SaveAnswer(ed_struct->ed_contact,ec.pWS);
    }
    RecountMenu(ed_struct->ed_contact,1);
    mfree(ed_struct);
  }
  if (cmd==0x0A)
  {
    pltop->dyn_pltop=SmilesImgList;
    DisableIDLETMR();
    total_unread-=ed_struct->requested_decrement_total_unread;
    ed_struct->requested_decrement_total_unread=0;
#ifdef ELKA
    if (!total_unread)
    {
      SLI_Off();
    }
#endif
/*    if (request_close_edchat)
    {
      request_close_edchat=0;
      GeneralFunc_flag1(edchat_id,1);
      return;
    }*/
  }
  if (cmd==7)
  {
    SetSoftKey(data,&sk,SET_SOFT_KEY_N);
    ExtractEditControl(data,ed_struct->ed_answer,&ec);
    if (ec.pWS->wsbody[0]==0)
      SetSoftKey(data,&sk_cancel,SET_SOFT_KEY_N==0?1:0);
    if (!EDIT_IsBusy(data))
    {
      time_to_stop_t9=0;
      if (ed_struct->ed_contact->req_add)
      {
	ed_struct->ed_contact->req_add=0;
	AddMsgToChat(data);
	RecountMenu(ed_struct->ed_contact,1);
      }
      if (ed_struct->ed_contact->req_drawack)
      {
	ed_struct->ed_contact->req_drawack=0;
	DrawAck(data);
      }
    }
  }
  if (cmd==0x0C)
  {
    j=EDIT_GetFocus(data);
    if ((EDIT_GetUnFocus(data)<j)&&(j!=ed_struct->ed_answer))
      EDIT_SetCursorPos(data,1);
  }
}



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

void CreateEditChat(CLIST *t)
{

  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  int color, font, type;
  int zc;
  
  LOGQ *lp;
  int edchat_toitem;
//  edcontact=t;
  
  *((int *)(&edchat_hdr.lgp_id))=(int)t->name;
//  *((int **)(&edchat_hdr.icon))=(int *)S_ICONS+GetIconIndex(t);
  *((int **)(&edchat_hdr.icon))=(int *)S_ICONS+IS_NULLICON;
  
  eq=AllocEQueue(ma,mfree_adr());
  
  /*  hist = malloc(256);
  if(!GetHistory(hist, 256, t->uin))
  {
    ascii2ws(ews,hist);
    PrepareEditControl(&ec);
    ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL|ECF_DISABLE_T9,ews,ews->wsbody[0]);
  }
  mfree(hist);

    AddEditControlToEditQend(eq,&ec,ma);  */
  lp=t->log;
  
  while(lp)
  {
    font = ED_H_FONT_SIZE;
    if ((zc=lp->acked&3))//Если отправлен или доставлен
    {
      if (zc==1)
	color=ACK_COLOR; //Зеленый, отправлен,но не доставлен
      else
	color=I_COLOR;//доставлен
    }
    else//иначе входящее сообщение
    {
      if (lp->ID==0xFFFFFFFF)
      {
        type = lp->type&0x0F;
        if(lp->type&0x10)//история
        {
          color = (type==1)?O_I_COLOR:((type==3)?O_X_COLOR:O_TO_COLOR);
          font = (type==3)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
        }
        else//текущий чат
        {
          color = (type==1)?I_COLOR:((type==3)?X_COLOR:TO_COLOR);
          font = (type==3)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
        }
      }
      else//если не входящее
        color=UNACK_COLOR; //то знач не доставлено
    }
	
    PrepareEditControl(&ec);
    
	if ((lp->type&0x0F)!=3)
    {
      ascii2ws(ews,lp->hdr);
      ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
    }
    else
    {
      ConstructEditControl(&ec,ECT_HEADER,ECF_DELSTR,ews,0);
    }
	
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,color);
    SetFontToEditCOptions(&ec_options,font);
    CopyOptionsToEditControl(&ec,&ec_options);
    AddEditControlToEditQend(eq,&ec,ma);
	
    if ((lp->type&0x0F)!=3)
    {
      ParseAnswer(ews,lp->text);
    }
    else
    {
      ParseXStatusText(ews,lp->text,(lp->type&0x10)?O_X_COLOR:X_COLOR);
    }
	
    PrepareEditControl(&ec);
    ConstructEditControl(&ec,
                         ECT_NORMAL_TEXT,
                         ews->wsbody[0] ? ECF_APPEND_EOL|ECF_DISABLE_T9 : ECF_DELSTR,
                         ews,ews->wsbody[0]);    
    PrepareEditCOptions(&ec_options);
//#ifdef M75
    if ((lp->type&0x0F)!=3)
    {
      SetFontToEditCOptions(&ec_options,(lp->type&0x10)?O_ED_FONT_SIZE:ED_FONT_SIZE);
    }
    else
    {
      SetPenColorToEditCOptions(&ec_options,(lp->type&0x10)?O_X_COLOR:X_COLOR); 
      SetFontToEditCOptions(&ec_options,(lp->type&0x10)?O_ED_X_FONT_SIZE:ED_X_FONT_SIZE);
    }
//#else
//    SetFontToEditCOptions(&ec_options,(lp->type&0x10)?O_ED_FONT_SIZE:ED_FONT_SIZE);
//#endif     
    CopyOptionsToEditControl(&ec,&ec_options);
    AddEditControlToEditQend(eq,&ec,ma);
    lp=lp->next;
  }
  if (t->isunread) total_unread--;
  total_unread2=total_unread2-t->unread_msg;
  t->unread_msg=0;
  t->isunread=0;
  ChangeContactPos(t);
  wsprintf(ews, "-------");
  PrepareEditControl(&ec);
  ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,ED_FONT_SIZE);
  CopyOptionsToEditControl(&ec,&ec_options);
  AddEditControlToEditQend(eq,&ec,ma);
  
  if (t->answer) ParseAnswer(ews,t->answer);
  else  CutWSTR(ews,0);
  PrepareEditControl(&ec);
//  ConstructEditControl(&ec,3,(FIRST_LETTER)?ECF_DEFAULT_BIG_LETTER:0,ews,1024);
  ConstructEditControl(&ec,3,(FIRST_LETTER)?ECF_DEFAULT_BIG_LETTER:0,ews,4096);

  PrepareEditCOptions(&ec_options);
  SetFontToEditCOptions(&ec_options,ED_FONT_SIZE);
  CopyOptionsToEditControl(&ec,&ec_options);
  edchat_toitem=AddEditControlToEditQend(eq,&ec,ma);
  
  if (t->req_xtext)
  {
    FreeXText(t);
    t->req_xtext=0;
    RequestXText(t->uin);
  }
  
  EDCHAT_STRUCT *ed_struct=malloc(sizeof(EDCHAT_STRUCT));
  ed_struct->ed_contact=t;
  ed_struct->ed_answer=edchat_toitem;
  ed_struct->requested_decrement_total_unread=0;
  t->req_add=0;
  t->last_log=NULL;
  
  //  int scr_w=ScreenW();
  //  int scr_h=ScreenH();
  //  int head_h=HeaderH();
  
  patch_header(&edchat_hdr);
  patch_input(&edchat_desc);
  //  edchat_desc.font=ED_FONT_SIZE;
  edchat_id=CreateInputTextDialog(&edchat_desc,&edchat_hdr,eq,1,ed_struct);
}

//-----------------------------------------------------------------------------

#define EC_MNU_MAX 9

void Quote(GUI *data)
{
  int q_n;
  EDITCONTROL ec, ec_hdr;
  EDITCONTROL ec_ed;
  WSHDR *ed_ws;
  WSHDR *ws;
  
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  q_n=EDIT_GetFocus(ed_struct->ed_chatgui);
  ExtractEditControl(ed_struct->ed_chatgui,q_n,&ec);
  ExtractEditControl(ed_struct->ed_chatgui,q_n-1,&ec_hdr);
  ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec_ed);

  ed_ws=AllocWS((ec_ed.maxlen<<1) + 1);
  if(wstrlen(ec_hdr.pWS))
  {
    wstrcpy(ed_ws,ec_hdr.pWS);
    wsAppendChar(ed_ws,'\r');
  }
  else
    CutWSTR(ed_ws,0);
  if (EDIT_IsMarkModeActive(ed_struct->ed_chatgui))
  {
    EDIT_GetMarkedText(ed_struct->ed_chatgui,ed_ws);
  }
  else
  {
    wstrcat(ed_ws,ec.pWS);
  }
  int ed_pos=0;
  do
  {
    ed_pos++;
    wsInsertChar(ed_ws,'>',ed_pos++);
    wsInsertChar(ed_ws,' ',ed_pos);
  }
  while((ed_pos=wstrchr(ed_ws,ed_pos,'\r'))!=0xFFFF);
  wsAppendChar(ed_ws,'\r');
  wsAppendChar(ed_ws,'\r');
  ws=AllocWS(ec_ed.pWS->wsbody[0]+ed_ws->wsbody[0]);
  wstrcpy(ws,ec_ed.pWS);
  wstrcat(ws,ed_ws);
  FreeWS(ed_ws);
  CutWSTR(ws,ec_ed.maxlen);
  EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
  EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
  FreeWS(ws);
  GeneralFuncF1(1);
}

void Quote2(GUI *data)//цитирование без фокуса на ответ
{
  int q_n;
  EDITCONTROL ec, ec_hdr;
  EDITCONTROL ec_ed;
  WSHDR *ed_ws;
  WSHDR *ws;
  
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  q_n=EDIT_GetFocus(ed_struct->ed_chatgui);
  ExtractEditControl(ed_struct->ed_chatgui,q_n,&ec);
  ExtractEditControl(ed_struct->ed_chatgui,q_n-1,&ec_hdr);
  ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec_ed);

  ed_ws=AllocWS((ec_ed.maxlen<<1) + 1);
  if(wstrlen(ec_hdr.pWS))
  {
    wstrcpy(ed_ws,ec_hdr.pWS);
    wsAppendChar(ed_ws,'\r');
  }
  else
    CutWSTR(ed_ws,0);
  if (EDIT_IsMarkModeActive(ed_struct->ed_chatgui))
  {
    EDIT_GetMarkedText(ed_struct->ed_chatgui,ed_ws);
  }
  else
  {
    wstrcat(ed_ws,ec.pWS);
  }
  int ed_pos=0;
  do
  {
    ed_pos++;
    wsInsertChar(ed_ws,'>',ed_pos++);
    wsInsertChar(ed_ws,' ',ed_pos);
  }
  while((ed_pos=wstrchr(ed_ws,ed_pos,'\r'))!=0xFFFF);
  wsAppendChar(ed_ws,'\r');
  wsAppendChar(ed_ws,'\r');
  ws=AllocWS(ec_ed.pWS->wsbody[0]+ed_ws->wsbody[0]);
  wstrcpy(ws,ec_ed.pWS);
  wstrcat(ws,ed_ws);
  FreeWS(ed_ws);
  CutWSTR(ws,ec_ed.maxlen);
  int ii=EDIT_GetFocus(ed_struct->ed_chatgui);
  EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
  EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
  EDIT_SetFocus(ed_struct->ed_chatgui,ii);
  FreeWS(ws);
  GeneralFuncF1(1);
}


void GetShortInfo(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  TPKT *p;
  CLIST *t;
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT));
    p->pkt.uin=t->uin;
    p->pkt.type=T_REQINFOSHORT;
    p->pkt.data_len=0;
    AddStringToLog(t, 0x01, "Request info...", I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t,1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void AskNickAndAddContact(EDCHAT_STRUCT *);
void AddCurContact(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  if ((ed_struct->ed_contact)&&(connect_state==3)) AskNickAndAddContact(ed_struct);
  GeneralFuncF1(1);
}

void SendAuthReq(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  TPKT *p;
  CLIST *t;
  int l;
  const char s[]=LG_AUTHREQ;
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen(s))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_AUTHREQ;
    p->pkt.data_len=l;
    strcpy(p->data,s);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t,1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void SendAuthGrant(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  TPKT *p;
  CLIST *t;
  int l;
  const char s[]=LG_AUTHGRANT;
  if((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen(s))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_AUTHGRANT;
    p->pkt.data_len=l;
    strcpy(p->data,s);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t,1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void OpenLogfile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);
  
  CLIST *t;
  WSHDR *ws=AllocWS(256);
  if ((t=ed_struct->ed_contact))
  {    
    if(HISTORY_SAVE_TYPE)
    {
      if(HISTORY_TYPE)
        wsprintf(ws,"%s\\%u\\%s(%d).txt", HIST_PATH, UIN, t->name, t->uin);
      else
        wsprintf(ws,"%s\\%s(%d).txt", HIST_PATH, t->name, t->uin);
    }     
    else
    {
      if(HISTORY_TYPE)
        wsprintf(ws,"%s\\%u\\%u.txt", HIST_PATH, UIN, t->uin);
      else
        wsprintf(ws,"%s\\%u.txt", HIST_PATH, t->uin);
    }
    ExecuteFile(ws,NULL,NULL);
  }
  FreeWS(ws);
  GeneralFuncF1(1);
}

void ClearLog(GUI *data/*,void *dummy*/)
{
  EDITCONTROL ec;
  EDCHAT_STRUCT *ed_struct;
  WSHDR *ws;
  CLIST *t;
  
  ed_struct=MenuGetUserPointer(data);
  
  ExtractEditControl(ed_struct->ed_chatgui,ed_struct->ed_answer,&ec);
  if(wstrlen(ec.pWS)>0)
  {
    ws=AllocWS(1);
    CutWSTR(ws,0);
  
    EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
    EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws); 
    
    FreeWS(ws);
    GeneralFuncF1(1);    
  }
  else
  {
  
    if ((t=ed_struct->ed_contact))
    {
      if (t->log)
      {
        FreeLOGQ(&t->log);
        t->msg_count=0;
        if (ed_struct->ed_answer>=2&&ed_struct->ed_chatgui)
        {
          while(ed_struct->ed_answer!=2)
          {
            EDIT_RemoveEditControl(ed_struct->ed_chatgui,1);
            ed_struct->ed_answer--;
          }
        }
      }
      t->isactive=0;		//by BoBa  18.06.07
      RecountMenu(t,1);
      GeneralFuncF1(1);
    }
  }
}

void ecmenu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

static const MENUITEM_DESC ecmenu_ITEMS[EC_MNU_MAX]=
{
  {NULL,(int)LG_MNUQUOTE,    LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUQUOTE2,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUADDSML,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUSHINFO,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUADDREN,   LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUSAUTHREQ, LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUSAUTHGRT, LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUOPENLOG,  LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,(int)LG_MNUCLEARCHT, LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2}
};

extern void AddSmile(GUI *data);
static const MENUPROCS_DESC ecmenu_HNDLS[EC_MNU_MAX]=
{
  Quote,
  Quote2,
  AddSmile,
  GetShortInfo,
  AddCurContact,
  SendAuthReq,
  SendAuthGrant,
  OpenLogfile,
  ClearLog,
};

char ecm_contactname[64];

static const HEADER_DESC ecmenu_HDR={0,0,NULL,NULL,NULL,(int)ecm_contactname,LGP_NULL};

static const MENU_DESC ecmenu_STRUCT=
{
  8,NULL,ecmenu_ghook,NULL,
  menusoftkeys,
  &menu_skt,
  0,
  NULL,
  ecmenu_ITEMS,
  ecmenu_HNDLS,
  EC_MNU_MAX
};

void ec_menu(EDCHAT_STRUCT *ed_struct)
{
  CLIST *t;
  int to_remove[EC_MNU_MAX+1];
  int remove=0;
  if ((t=ed_struct->ed_contact))
  {
    if (t->name)
    {
      strncpy(ecm_contactname,t->name,63);
    }
    else
    {
      sprintf(ecm_contactname,"%u",t->uin);
    }
    if (EDIT_GetFocus(ed_struct->ed_chatgui)==ed_struct->ed_answer)
    {
      to_remove[++remove]=0;
      to_remove[++remove]=1;
    }
    else
    {
//      to_remove[++remove]=1;
      to_remove[++remove]=2;
    }
    
    if (ed_struct->ed_answer<=2) 
        to_remove[++remove]=7;

    if (!ed_struct->ed_contact || connect_state!=3)
    {
      to_remove[++remove]=2;
      to_remove[++remove]=3;
      to_remove[++remove]=4;
      to_remove[++remove]=5;
      to_remove[++remove]=6;
    }
    
    patch_header(&ecmenu_HDR);
    to_remove[0]=remove;
    CreateMenu(0,0,&ecmenu_STRUCT,&ecmenu_HDR,0,EC_MNU_MAX,ed_struct,to_remove);
  }
}


void AskNickAndAddContact(EDCHAT_STRUCT *ed_struct)
{
  CreateAddContactGrpDialog(ed_struct->ed_contact);
}

void AddSmile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct=MenuGetUserPointer(data);
  CreateSmileSelectGUI(ed_struct);
  GeneralFuncF1(1);
}
