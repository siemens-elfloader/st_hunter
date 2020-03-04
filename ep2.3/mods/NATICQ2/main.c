#include "../inc/swilib.h"
#include "../inc/cfg_items.h"
#include "../inc/pnglist.h"
#include "NatICQ.h"
#include "history.h"
#include "conf_loader.h"
#include "mainmenu.h"
#include "main.h"
#include "language.h"
#include "../inc/xtask_ipc.h"
#include "smiles.h"
#include "../inc/naticq_ipc.h"
#include "naticq_ipc.h"
#include "status_change.h"
#include "strings.h"
#include "manage_cl.h"
#include "cl_work.h"
#include "select_smile.h"
#include "revision.h"
#include "revisio0.h"
#include "lang.h"
#include "smiles_loader.h"
//Я сделал, что мог; пусть те, кто могут, сделают лучше
//#ifdef GUI_INTERFACE
  #include "externals.h"
//#endif
#include "auth.h"

#ifndef NEWSGOLD
#define SEND_TIMER
#endif

extern volatile int total_smiles;
extern volatile int total_xstatuses;
extern volatile int xstatuses_load;
extern volatile int pictures_max;
extern volatile int pictures_loaded;

extern const unsigned int IllByONCantact;
extern const int DEVELOP_IF;
    
#define USE_MLMENU
//#define GUI_INTERFACE //use my interface else use standart

#define TMR_SECOND 216

//IPC
const char ipc_my_name[32]=IPC_NATICQ_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
IPC_REQ gipc;

volatile CLIST *cltop;
char elf_path[256];
int maincsm_id;
int maingui_id;

void SMART_REDRAW(void)
{
  int f;
  LockSched();
  f=IsGuiOnTop(maingui_id);
  UnlockSched();
  if (f) REDRAW();
}

//По 10 секунд
#define ACTIVE_TIME 360

//Максимальное количество сообщений в логе
#define MAXLOGMSG (20)

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

int IsActiveUp = 0;
volatile int vibra_count;

int Is_Double_Vibra_On = 0;
int Is_Vibra_Enabled = 0;
unsigned int Is_Sounds_Enabled = 0;
int Is_Show_Offline = 0;
int Is_Show_Groups = 0;
int Is_POPUP_On = 1;
int Is_SLI_On = 1;
int Is_ILU_By_Online = 1;
int Is_Extd_Sound = 0;

int S_ICONS[TOTAL_ICONS+1];

#define EOP -10
int CurrentStatus;
int CurrentXStatus;

int total_unread;
int total_unread_all;//общее кол-во непрочитанных сообщений

char last_msg_buffer[4096];
extern const int IsBufferMenu;
WSHDR *wsBuffer;    //буффер cообщений

int develop_by_unlock = 0;// показать контакт лист после разблокировки
#ifdef GUI_INTERFACE
int ShowLoadScreen=1;

static GUIMENU *clistm;//contact list 

char on_off_buf[128]; //буфер для подключенных/отключенных контактов
int on_off_timer = 0; //таймер показа попап сообщения

/*
*  Функция: ClrOnOffBuffer
*  Описание: Очищаем буфер
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
int ClrOnOffBuffer()
{
  int retval = 0;
  if(on_off_timer > 0)
    {
      on_off_timer = 0;
      zeromem(on_off_buf,sizeof(on_off_buf));             
      retval = 1;
    }
  return retval;
}


int online_contact;//кол-во онлайн
int total_contact;//общее кол-во контактов в списке

int isonline;
void *canvasdata;

int DoRefreshCL=0;//Если нада обновить КЛ
void RefreshCL()
{
  DoRefreshCL=1; 
}

#endif

int CountMessage[2];//кол-во сообщений,отправленных и принятых
int blocked_spam = 0;//кол-во блокированных спамов

int LongPressTimer=0;//таймер для лонг пресс действия,чтоб была задержка при удержании кнопки

char prmsg[256];//вывод попап сообщения в контакт листе
int PrintTimer=0;//время попап,секунды


/*
*  Функция: PrintMessage
*  Описание: Выводить сообщение в контакт листе 
*  Параметры: 
*       char *s - сообщение
*       int sec - сколько секунд показывать сообщение до его закрытия
*  Возвращаемое значение: Ничего не возвращает
*/
void PrintMessage(char *s,int sec)
{
#ifdef GUI_INTERFACE
  strcpy(prmsg,s);
  PrintTimer = sec;
#else
  LockSched();
  ShowMSG(1,(int)s);
  UnlockSched();
#endif
}

/*
*  Функция: ClrPrintMess
*  Описание: Очистить буфер и такймер сообщения
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
int ClrPrintMess()
{
  int retval=0;
  if(PrintTimer > 0 && strlen(prmsg))
  {
    PrintTimer=0;
    LongPressTimer=0;
    zeromem(prmsg,256);
    retval=1;
  }
  return retval;
}

WSHDR *ews;

extern const unsigned int UIN;
extern const char PASS[];

//static const char * const icons_names[TOTAL_ICONS]=
const char *icons_names[TOTAL_ICONS]=
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
  "check.png",
  "uncheck.png",
  "bg.png"
};

extern const char ICON_PATH[];


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

extern const unsigned int IDLEICON_X;
extern const unsigned int IDLEICON_Y;

extern const unsigned int I_COLOR;
extern const unsigned int TO_COLOR;
extern const unsigned int X_COLOR;
extern const unsigned int O_I_COLOR;
extern const unsigned int O_TO_COLOR;
extern const unsigned int O_X_COLOR;

extern const unsigned int ED_FONT_SIZE;
extern const unsigned int ED_H_FONT_SIZE;
extern const unsigned int ED_X_FONT_SIZE;
extern const unsigned int O_ED_FONT_SIZE;
extern const unsigned int O_ED_H_FONT_SIZE;
extern const unsigned int O_ED_X_FONT_SIZE;

extern const unsigned int ACK_COLOR;
extern const unsigned int UNACK_COLOR;

extern const int ENA_AUTO_XTXT;
extern const int NOT_LOG_SAME_XTXT;
extern const int LOG_XTXT;

extern const int HISTORY_BUFFER;

const char percent_t[]="%t";
const char percent_d[]="%d";
const char empty_str[]="";
extern const char I_str[];//="I";
const char x_status_change[]="X-Status change";

char logmsg[256];

//Illumination by BoBa 19.04.2007
///////////
extern const unsigned int ILL_DISP_RECV;
extern const unsigned int ILL_KEYS_RECV;
extern const unsigned int ILL_DISP_SEND;
extern const unsigned int ILL_KEYS_SEND;
extern const unsigned int ILL_SEND_TMR;
extern const unsigned int ILL_SEND_FADE;
extern const unsigned int ILL_RECV_TMR;
extern const unsigned int ILL_RECV_FADE;
extern const unsigned int ILL_OFF_FADE;
int Is_ILU_Mode;

GBSTMR tmr_illumination;

void IlluminationOff(){
  SetIllumination(0,1,0,ILL_OFF_FADE);
  SetIllumination(1,1,0,ILL_OFF_FADE);
}

void IlluminationOn(const int disp, const int key, const int tmr, const int fade){
  if(!tmr) return;
  if(!Is_ILU_Mode)return;
  GBS_DelTimer(&tmr_illumination);
  SetIllumination(0,1,disp,fade);
  SetIllumination(1,1,key,fade);
  GBS_StartTimerProc(&tmr_illumination,tmr*216,IlluminationOff);
}

volatile int silenthide;    //by BoBa 25.06.07
volatile int disautorecconect;	//by BoBa 10.07
///////////
int Is_Vibra_Enabled;
unsigned int Is_Sounds_Enabled;
int Is_Show_Offline;
int Is_Show_Groups;
int CurrentStatus;
int CurrentXStatus;
int CurrentPrivateStatus;

//---------------------------------------------------

  
int msg_away_timer=0; //Счетчик для автостатуса
char away_msg[256]; //Само сообщение для ответа
char away_msg_time[64]; //Время когда сработал таймер
char away_msg_time_dace[16];  //время отсутвтвия для вывода на идле
int away_m; //состояние автоответчика
int status_rem; //Для сохранения текущего статуса
int change_back_status; //вернуть предыдущий статус или нет
GBSTMR away_tmr;
/*
char *timeZone()
{
  char *t=malloc(32);
  char *tim_zone=t;
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
  mfree(t);
  return (char*)tim_zone;
}
*/

/*
*  Функция: away_timer
*  Описание: Следит за временем бездействия, и формирует сообщение автоответчика, вызывается раз в 5 секунд
*  Параметры: Ни каких
*  Возвращаемое значение: ничего не возвращает
*/

void away_timer()//Таймер автостатуса
{
  TTime tt; 
  if(msg_away_timer==(AUTO_ENGADE*12))
  {
    GetDateTime(NULL,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "Отсутствую примерно с %02d:%02d",tt.hour, tt.min/*,((AUTO_ENGADE) + (msg_away_timer*5)/60) */);
    away_m=1;
    
    if(CurrentStatus!=2 && CurrentStatus!=isAutoStatusOn)
    {
      status_rem=CurrentStatus;
      CurrentStatus=2;
      set_my_status();
      change_back_status=1;
    }    
  }
  
  if( (IsOnNAStatus) && (msg_away_timer==(AUTO_ENGADE*12)+(NAStatusTime*12)) )//Переключение на недоступен,в минутах
  {
    GetDateTime(NULL,&tt);
    sprintf(away_msg_time_dace, "%02d:%02d", tt.hour, tt.min);
    sprintf(away_msg_time, "Недоступен примерно с %02d:%02d",tt.hour, tt.min/*,((AUTO_ENGADE) + (NAStatusTime) + (msg_away_timer*5)/60)*/);
    if(CurrentStatus != 3)
    {
      CurrentStatus = 3;
      set_my_status();
    }
  }
  
  if(Auto_Status && connect_state == 3)  msg_away_timer++;
  GBS_StartTimerProc(&away_tmr, 216*5, away_timer);
}



//----------------------- reminder timer ------------------------//


GBSTMR remind_tmr;
void Reminder_timer();
int double_on;

/*
*  Функция: Remind & Reminder_timer
*  Описание: При включенной опции "Напоминание" дергает виброй при не отвеченных сообщениях с интервалом в 30 сек.
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/

void Remind()
{
  double_on = 0;
  if(total_unread && Is_Double_Vibra_On)
  {
    vibra_count = 2;
    double_on = 1; 
    start_vibra();
    GBS_StartTimerProc(&remind_tmr, 216*30, Remind);
  }
}

void Reminder_timer()
{
  double_on = 0;
  GBS_StartTimerProc(&remind_tmr, 216*30, Remind);  
}


//---------------------- uptime -----------------------//

TTime uptime;

/*
*  Функция: UpTime
*  Описание: Время проведенное в онлайн
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void UpTime()
{
  if(connect_state==3)
  {
    uptime.sec++;
    if(uptime.sec==60)
    {
      uptime.sec=0;
      uptime.min++;
    }
    if(uptime.min==60)
    {
      uptime.min=0;
      uptime.hour++;
    }
  }
}


char ContactT9Key[32];

/*
*  Функция: LongPressActionOnChat
*  Описание: Запускает действие в контакт листе по длинному нажатию клавиши
*  Параметры: 
*   CLIST *t - Структура контакт листа
*   int i - номер действия
*  Возвращаемое значение: Возвращает 0
*/
int LongPressActionOnChat(CLIST *t,int i)
{
   switch(i)
    {
    case 1:
        Is_Sounds_Enabled=!(Is_Sounds_Enabled);
        PrintMessage(Is_Sounds_Enabled?"Sound 0n":"Sound Off",3);
        SUBPROC((void*)WriteDefSettings);
        break;
    case 2:
        Is_Vibra_Enabled=!(Is_Vibra_Enabled);
        SUBPROC((void*)WriteDefSettings);
        PrintMessage(Is_Vibra_Enabled?"Vibra On":"Vibra Off",3);
        break;
    case 3:        
         if(CurrentPrivateStatus!=/*1*/2)
         {
           SetPrivateStatus(2/*1*/);//Инвиз
           PrintMessage("Инвиз включен",2);
         }
         else
         {
           SetPrivateStatus(3);
           PrintMessage("Инвиз выключен",2);
         }
         break;
    case 4:
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        PrintMessage(Is_Double_Vibra_On?"Vibra rem On":"Vibra rem Off",3);
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
        PrintMessage("All chat clear!",2);
        break;
      }
    case 6:
      if(!away_m)
      {
        away_m=!away_m;
        GBS_StopTimer(&away_tmr);
        msg_away_timer=AUTO_ENGADE;
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
      break;
    case 7:
      {
        WSHDR *ws;
        ws=AllocWS(150);
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
      break;
    case 8://ping
      {
        TPKT *p;
        p=malloc(sizeof(PKT)+sizeof(TTime));
        GetDateTime(NULL,(TTime *)p->data);
        p->pkt.uin=0;
        p->pkt.type=T_ECHO;
        p->pkt.data_len=sizeof(TTime);
        SUBPROC((void *)SendAnswer,0,p);
      }
      break;
    case 9://go to idle
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
    case 10://hide & lock
      {        
        if(DEVELOP_IF == 3) develop_by_unlock = 1;//залочили клаву и поставили флаг 6.11.2009
        
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
      }
      break;
    case 11: //offline contacts
      {
        extern void ChangeShowOfflineMode(void);
        ChangeShowOfflineMode();
        PrintMessage(Is_Show_Offline?"Оффлайн контакты\nвключены":"Оффлайн контакты\nотключены",1);
      }
      break;
    case 12: //group mode
      {
        extern void ChangeShowGroupsMode(void);
        ChangeShowGroupsMode();
        PrintMessage(Is_Show_Groups?"Группы включены":"Группы отключены",1);
      }
      break;
    case 13: // action by *
      {
        int FindMessage(CLIST* );
        LongPressTimer=0;
        return FindMessage(t);        
      }
      //break;      
    default : LongPressTimer=0; break;
    }//switch()
   return 0;
}

//{"Без действия","Звук","Вибра","Инвиз","Реж.напоминания","Очистить чаты","Автоответчик","Конфиг","Пинг","Заблокировать","На гэ"}

/*
*  Функция: LongPressAction
*  Описание: Выборка нажатой клавиши в контакт листе, и вызов назначенной на эту клавишу функции. Функция указана в конфиге.
*  Параметры: 
*       CLIST *t - Структура контакт листа
*       int key - нажатая клавиша
*  Возвращаемое значение: Ничего не возвращает
*/
void LongPressAction(CLIST *t,int key)
{
  LongPressTimer = 2;
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
    default : LongPressTimer=0;break;
  }
}



//--------------------------------------------------------------------------------


/*
*  Функция: DrwImage
*  Описание: Отрисовка картинки
*  Параметры: 
*       IMGHDR *img - картинка для отрисовки
*       int x - начальная координата Х
*       int y - начальная координата У
*       int rettype - возвращаемое значение
*  Возвращаемое значение:
*       rettype = 0 - возратить высоту у
*       rettype = 1 - возвратить ширину Х
*/
int DrwImage(IMGHDR *img, int x, int y,int rettype)
{
  if(!img) return 0;
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
  SetPropTo_Obj5(&drwobj,&rc,0,img);
  SetColor(&drwobj,0,0);
  DrawObject(&drwobj);
  return ( rettype==RET_X ? ((img->w > 32) ? 32 : img->w )  : ((img->h > ScreenH()-1) ? 1 : img->h) );
}



//-------------  Попап месаже для подключившегося контакта -----------//


typedef struct
{
  char name[64];
  int timer;
  GBSTMR tmr;
//  int icon;
  ///int xicon;
  IMGHDR *icon;
  IMGHDR *xicon;
  int xiconf;
}POPUP;

#define POPUP_ON  0
#define POPUP_OFF 1
#define POPUP_MSG 2

#define POPUP_TMR 15
#define POPUP_N   20

POPUP popup[3];

int FIRST_START = 0;//флаг,проверяющий подключание,чтобы не показывать попапы,не мигать контактами не играть звуки при первом показе кл, а то подвисает сцука


/*
*  Функция: IsNoJavaRun
*  Описание: Проверяет активно ли чейчас ява-приложение либо простой процесс
*  Параметры: Никаких
*  Возвращаемое значение: 
*              0 - запущенный процесс ява
*              1 - запущенный процесс не ява
*/
int IsNoJavaRun()
{
  
  int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
  char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
  if((memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8))) return 1;
 return 0;
}


/*
*  Функция: show_on_contact
*  Описание: Показывает попап с именем подключенного контакта
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void show_on_contact()
{
  if(FIRST_START)//Если загружаем контакт лист первый раз, то не делаем попапы
  {
    popup[POPUP_ON].timer=0; 
    return;
  }

  if(popup[POPUP_ON].timer > 0)
  {
/*    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if((memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))*/
    if(IsNoJavaRun())
    {      
      WSHDR *ws=AllocWS(64);
      ascii2ws(ws,popup[POPUP_ON].name);
      popup[POPUP_ON].timer--;
      
      int x=0;
#ifdef GUI_INTERFACE     
      if(popup[POPUP_ON].icon) x = popup[POPUP_ON].icon->w;
      if(popup[POPUP_ON].xicon && popup[POPUP_ON].xiconf) x += popup[POPUP_ON].xicon->w;
#endif
      DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+x+5,GetFontYSIZE(PopupFont)+5,0,PopupOnlineCl,PopupBGCl);
#ifdef GUI_INTERFACE     
      if(popup[POPUP_ON].icon)
      {
        DrwImage(popup[POPUP_ON].icon,2,3,0);
        if(popup[POPUP_ON].xiconf) DrwImage(popup[POPUP_ON].xicon,4 + popup[POPUP_ON].icon->w,3,0);
      }
#endif
      DrawString(ws,x+3,2, 3+Get_WS_width(ws,PopupFont)+x ,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupOnlineCl,GetPaletteAdrByColorIndex(23));
      FreeWS(ws);
    }
   GBS_StartTimerProc(&popup[POPUP_ON].tmr, POPUP_TMR, show_on_contact);    
  }
}

/*
*  Функция: show_off_contact
*  Описание: Показываем попапы с именем отключенного контакта
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void show_off_contact()
{
  if(!isPopupOff) return;
  if(FIRST_START)
  {
    popup[POPUP_OFF].timer=0; 
    return;
  }
  //if(!Is_POPUP_On) popup[POPUP_OFF].timer=0; 
  if(popup[POPUP_OFF].timer>0/* && IsNoJava()*/)
  {
/*    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if((memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))*/
    if(IsNoJavaRun())
    {
      WSHDR *ws=AllocWS(64);
      ascii2ws(ws,popup[POPUP_OFF].name);
      popup[POPUP_OFF].timer--;  
#ifdef GUI_INTERFACE           
      DrawRectangle(ScreenW()-Get_WS_width(ws,PopupFont)-4-popup[POPUP_OFF].icon->w,0,ScreenW()-1,GetFontYSIZE(PopupFont)+4,0,PopupOffCl,PopupOffBGCl);
      DrwImage(popup[POPUP_OFF].icon,ScreenW()-Get_WS_width(ws,PopupFont)-2-popup[POPUP_OFF].icon->w,3,0);
#else
      DrawRectangle(ScreenW()-Get_WS_width(ws,PopupFont)-4,0,ScreenW()-1,GetFontYSIZE(PopupFont)+4,0,PopupOffCl,PopupOffBGCl);
#endif
      DrawString(ws,ScreenW()-Get_WS_width(ws,PopupFont)-3,2,ScreenW()-1,GetFontYSIZE(PopupFont)+2,PopupFont,TEXT_ALIGNRIGHT,PopupOffCl,GetPaletteAdrByColorIndex(23));
      FreeWS(ws);
    }
   GBS_StartTimerProc(&popup[POPUP_OFF].tmr, POPUP_TMR, show_off_contact);    
  }
}

//---------------------------- popup message ---------------------------//



/*
*  Функция: show_pp_sender
*  Описание: попап с именем контакта от которого пришло сообщение
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void show_pp_sender()
{
  if(FIRST_START)
  {
    popup[POPUP_MSG].timer=0; 
    return;
  }
 // if(!Is_POPUP_On) popup[POPUP_MSG].timer=0; 
  if(popup[POPUP_MSG].timer > 0/* && IsNoJava()*/)
  {
/*    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if((memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))*/
    if(IsNoJavaRun())
    {
      WSHDR *ws=AllocWS(256);
      ascii2ws(ws,popup[POPUP_MSG].name);
      popup[POPUP_MSG].timer--;
#ifdef GUI_INTERFACE
      popup[POPUP_MSG].icon = GetIconByItem(IS_MSG);
      DrawRectangle(0,0,Get_WS_width(ws,PopupFont)+6+popup[POPUP_MSG].icon->w,GetFontYSIZE(PopupFont)+4,0,PopupMsgCl,PopupBGCl);
      DrwImage(popup[POPUP_MSG].icon,2,2,0);
      DrawString(ws,popup[POPUP_MSG].icon->w+4,1,popup[POPUP_MSG].icon->w+Get_WS_width(ws,PopupFont)+4,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupMsgCl,GetPaletteAdrByColorIndex(23));
#else
      DrawString(ws,4,1,Get_WS_width(ws,PopupFont)+4,1+GetFontYSIZE(PopupFont),PopupFont,0,PopupMsgCl,GetPaletteAdrByColorIndex(23));     
#endif
      FreeWS(ws);
    }
    GBS_StartTimerProc(&popup[POPUP_MSG].tmr, POPUP_TMR, show_pp_sender);    
  }
}


//===================================================================
const char def_setting[]="%sdef_settings_%d";
int ExtSndModeIndivd;

void ReadDefSettings(void)
{
  DEF_SETTINGS def_set;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if(get_file_size(str) != sizeof(DEF_SETTINGS)) unlink(str,&err);
  if ((f=fopen(str,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
  {
    fread(f,&def_set,sizeof(DEF_SETTINGS),&err);
    fclose(f,&err);
    Is_Vibra_Enabled     = def_set.vibra_status;
    Is_Sounds_Enabled    = def_set.sound_status;
    Is_Show_Offline      = def_set.off_contacts;
    Is_Show_Groups       = def_set.show_groups;
    CurrentStatus        = def_set.def_status + 1;
    CurrentXStatus       = def_set.def_xstatus;
    IsActiveUp           = def_set.ActiveUp;
#ifdef ELKA
    Is_SLI_On            = def_set.sli_state;
#endif
    Is_POPUP_On          = def_set.popup_state;
    Is_Double_Vibra_On   = def_set.remind_state;
    Is_ILU_Mode          = def_set.ilu_mode;
    Is_ILU_By_Online     = def_set.ilu_online;
    Is_Extd_Sound        = def_set.extsnd_mode;
    ExtSndModeIndivd     = def_set.extsnd_flag;
    CurrentPrivateStatus = def_set.def_private_status;
  }
  else
  {
    Is_Vibra_Enabled     = 1;
    Is_Sounds_Enabled    = 0;
    Is_Show_Offline      = 0;
    Is_Show_Groups       = 0;
    CurrentStatus        = IS_ONLINE;
    CurrentXStatus       = 0;
    CurrentPrivateStatus = PL_VISLIST_CAN_SEE;
    
    IsActiveUp = 1;
        
#ifdef ELKA
    Is_SLI_On = 1;
#endif
    Is_POPUP_On        = 1;
    Is_Double_Vibra_On = 0;
    Is_ILU_Mode        = 1;
    Is_ILU_By_Online   = 1;
    Is_Extd_Sound      = 0;
    ExtSndModeIndivd   = 0;
    
  }
}

void WriteDefSettings(void)
{
  DEF_SETTINGS def_set;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if ((f=fopen(str,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))!=-1)
  {
    def_set.vibra_status=Is_Vibra_Enabled;
    def_set.sound_status=Is_Sounds_Enabled;
    def_set.off_contacts=Is_Show_Offline;
    def_set.show_groups=Is_Show_Groups;
    def_set.def_status=CurrentStatus-1;
    def_set.def_xstatus=CurrentXStatus;
    def_set.ActiveUp = IsActiveUp;
#ifdef ELKA
    def_set.sli_state=Is_SLI_On;
#endif
    def_set.popup_state=Is_POPUP_On;
    def_set.remind_state=Is_Double_Vibra_On;
    def_set.ilu_mode = Is_ILU_Mode;
    def_set.ilu_online = Is_ILU_By_Online;
    def_set.extsnd_mode = Is_Extd_Sound;
    def_set.extsnd_flag = ExtSndModeIndivd;
    def_set.def_private_status = CurrentPrivateStatus;
    fwrite(f,&def_set,sizeof(DEF_SETTINGS),&err);
    fclose(f,&err);
  }
}
//by KreN 27.09.2007
//===================================================================
extern S_SMILES *s_top;
extern DYNPNGICONLIST *SmilesImgList;
extern DYNPNGICONLIST *XStatusesImgList;

//=============================Проигрывание звука=======================
extern const char sndStartup[];
extern const char sndSrvMsg[];
extern const char sndGlobal[];
extern const char sndMsg[];
extern const char sndMsgSent[];
extern const unsigned int sndVolume;


int ExtSndMode_flag=0;

/* 
*  Функция: GetExtSnd
*  Описание: Ищет звукойвой файл, проверяет 2 разных формата, mp3, wav, amr 
*  Параметры: const char *extsng - путь + имя звукового файла, который нужно искать, имя - это UIN контакта
*  Возвращаемое значение: Полный путь до файла, если таковой существует, или NULL
*/
char *GetExtSnd(const char *extsnd)//получить звук с нужным расширением
{
  char *mp3,*wav,*amr;
  mp3 = MakeGlobalString(extsnd,'.',"mp3");
  wav = MakeGlobalString(extsnd,'.',"wav");
  amr = MakeGlobalString(extsnd,'.',"amr");
  if(get_file_size(mp3)) return mp3;
  else
    if(get_file_size(wav)) return wav;
  else
    if(get_file_size(amr)) return amr;
  return NULL;
}

void Play(const char *fname)
{
  if ((!IsCalling()) && ( Is_Sounds_Enabled || ExtSndMode_flag))
  {
    ExtSndMode_flag = 0;
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

void start_vibra(void)
{
  extern const int VIBR_TYPE;
  void stop_vibra(void);
  if((Is_Vibra_Enabled || (Is_Double_Vibra_On && double_on))&&(!IsCalling()))
  {
    extern const unsigned int vibraPower;
    SetVibration(vibraPower);
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>2,stop_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,stop_vibra);
  }
}

void stop_vibra(void)
{
  extern const int VIBR_TYPE;
  SetVibration(0);
  if (--vibra_count)
  {
    if(VIBR_TYPE)
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND/40,start_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,start_vibra);
  }
}
/*
void ChangeVibra(void)
{
  if (!(Is_Vibra_Enabled=!(Is_Vibra_Enabled)))
    ShowMSG(1,(int)lgpData[LGP_MsgVibraDis]);
  else
    ShowMSG(1,(int)lgpData[LGP_MsgVibraEna]);
}

void ChangeSound(void)
{
  if (!(Is_Sounds_Enabled=!(Is_Sounds_Enabled)))
    ShowMSG(1,(int)lgpData[LGP_MsgSndDis]);
  else
    ShowMSG(1,(int)lgpData[LGP_MsgSndEna]);
}
*/
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

extern const char TEMPLATES_PATH[];

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

volatile int sendq_l=0; //Длинна очереди для send
volatile void *sendq_p=NULL; //указатель очереди

volatile int is_gprs_online=1;

GBSTMR reconnect_tmr;

extern void kill_data(void *p,void (*func_p)(void *));

void ElfKiller(void)
{
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}


//===============================================================================================
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


volatile unsigned int GROUP_CACHE; //Текущая группа для добавления

volatile int contactlist_menu_id;

GBSTMR tmr_active;

volatile int edchat_id;

//Применяется для добавления сообщений
//CLIST *edcontact;
//void *edgui_data;


char clm_hdr_text[48];
static char def_clm_hdr_text[32] = "";//LgpData[LGP_ClTitle];
static char key_clm_hdr_text[32] = "";//LgpData[LGP_ClT9Inp];

char clmenu_sk_r[16];
static char def_clmenu_sk_r[32] = "";
static char key_clmenu_sk_r[32] = "";

static const int menusoftkeys[] = {0,1,2};

static SOFTKEY_DESC menu_sk[] =
{
  {0x0018, 0x0000, NULL/*(char*) lgpData[LGP_Select]*/},
  {0x0001, 0x0000, NULL/*(char*) lgpData[LGP_Back]*/},
  {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

void lgpUpdateClHdr()
{
  strcpy(def_clm_hdr_text, (char*) lgpData[LGP_ClTitle]);
  strcpy(key_clm_hdr_text, (char*) lgpData[LGP_ClT9Inp]);
  strcpy(def_clmenu_sk_r,  (char*) lgpData[LGP_Close]);
  strcpy(key_clmenu_sk_r,  (char*) lgpData[LGP_Clear]);
}

const SOFTKEYSTAB menu_skt =
{
  menu_sk, 0
};

//------------------------------------------------------

//GBSTMR tmr_ping;
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

//Получаем последний полученный X-статус
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


//------------------------------------------------------


//===============================================================================================
int DNR_ID=0;
int DNR_TRIES=3;

extern const char NATICQ_HOST[];
extern const unsigned int NATICQ_PORT;
extern const unsigned int RECONNECT_TIME;
char hostname[128];
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
//---------------------------------------------------------------------------

void create_connect(void)
{
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
    strcpy(logmsg,LG_GRWAITFORGPRS);
    SMART_REDRAW();
    return;
  }
  DNR_ID=0;
  *socklasterr()=0;

  if(host_counter > GetHostsCount(NATICQ_HOST)-1) host_counter = 0;
  GetHost(host_counter, NATICQ_HOST, hostbuf);
  hostport = GetPort(host_counter, NATICQ_HOST);
  host_counter++;

  sprintf(hostname, "%s:%d", hostbuf, hostport);

  SMART_REDRAW();

  ip=str2ip(hostbuf);
  if (ip!=0xFFFFFFFF)
  {
    sa.ip=ip;
    strcpy(logmsg,"Connect by IP!");
    SMART_REDRAW();
    goto L_CONNECT;
  }
  strcpy(logmsg,LG_GRSENDDNR);
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
      strcpy(logmsg,LG_GRDNROK);
      SMART_REDRAW();
      DNR_TRIES=0;
      sa.ip=p_res[3][0][0];
    L_CONNECT:
      sprintf(hostname, "%s:%d\n(IP: %d.%d.%d.%d)", hostbuf, hostport,
              sa.ip&0xFF, (sa.ip>>8)&0xFF, (sa.ip>>16)&0xFF, (sa.ip>>24)&0xFF);

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
	  ShowMSG(1,(int)lgpData[LGP_MsgCantConn]);
	  UnlockSched();
	  GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
	}
      }
      else
      {
	LockSched();
	ShowMSG(1,(int)lgpData[LGP_MsgCantCrSc]);
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
    ShowMSG(1,(int)lgpData[LGP_MsgHostNFnd]);
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
#ifdef GUI_INTERFACE
  SMART_REDRAW();
#endif
#ifdef SEND_TIMER
  GBS_DelTimer(&send_tmr);
#endif
}


/*
int Is_Pleer_Run()
{
  #ifdef ELKA
  #define HOBJ_LOC 0x54
  #else
  #define HOBJ_LOC 0x50
  #endif

  unsigned int playercsmid=0;
  void *playercsmadr=NULL;
  //el=A087B454 e=A087109C
  char CSMADR[9];
  int hobj=0;
  CSM_RAM *csmp;
  char *phone = Get_Phone_Info(PI_MODEL);
  
  zeromem(CSMADR,9);
  
  if(!strcmp_nocase(phone,"E71"))  strcpy(CSMADR,"A087109C");
  if(!strcmp_nocase(phone,"EL71")) strcpy(CSMADR,"A087B454");

  playercsmadr=(void *)strtoul(CSMADR,NULL,16);
  
  CSM_RAM *p=CSM_root()->csm_q->csm.first;
  while(p)
  {
    if (p->constr==playercsmadr) break;
    p=p->next;
  }
  if (p)
  {
    playercsmid=p->id;
    csmp=FindCSMbyID(playercsmid);
  }
  
  if(playercsmid)
  {
    hobj = ((int*)csmp)[HOBJ_LOC/4];    
    if (hobj) return 1;
  }  
  return 0;
}
*/
void end_socket(void)
{
  if (sock>=0)
  {
    shutdown(sock,2);
    closesocket(sock);
  }
  
//  if(!Is_Pleer_Run())
      Play(sndLostConn);
#ifdef SEND_TIMER
  GBS_DelTimer(&send_tmr);
#endif
}

#ifdef SEND_TIMER
static void resend(void)
{
  void SendAnswer(int dummy, TPKT *p);
  SUBPROC((void*)SendAnswer,0,0);
}
#endif

#define ac 35
#define af 36
/*
void _SendAnswer(int dummy, TPKT *p);
void SendAnswer(int dummy, TPKT *p){  
  if(p->pkt.type==T_SETCLIENT_ID)
  {
    char rev[9]="Sie_3565"; //??? ???? ?????? ?????? auth.r79
    memcpy(p->data,rev,8);
  } 
  if(p->pkt.type==T_REQLOGIN 
     ||p->pkt.type==T_SETCLIENT_ID 
       ||p->pkt.type==35)
  {      
    _SendAnswer(0,p);
  }
}

void Send_36(){
  TPKT *p=malloc(sizeof(PKT)+1);
  zeromem(p,sizeof(PKT)+1);  
  p->pkt.type=36;
  p->pkt.data_len=1;
  SUBPROC((void *)_SendAnswer,0,p);
}
*/

char *get_rev()
{
  char rev[9] = "";
  char txt[4];
  int f;
  unsigned int err;
  char fn[64];


   strcpy(fn,TEMPLATES_PATH);
   strcat(fn,"\\rev.txt");
   f = fopen(fn,A_ReadOnly + A_BIN,P_READ, &err );
     
   if( f != -1)
   {
     fread(f, txt, 4, &err);
     snprintf(rev,8,"Sie_%s",txt);
   }
   else
     snprintf(rev,8,"Sie_3689",txt);
   
   fclose(f,&err);
   rev[8] = 0;
   return rev;
}


void SendAnswer(int dummy, TPKT *p)
{
  if(p->pkt.type==T_SETCLIENT_ID)
  {
    char rev[9]="Sie_3680";//"Sie_3593";//"Sie_3565"; //??? ???? ?????? ?????? auth.r79
    memcpy(p->data,rev,8);
  } 
  
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
/*
void send_login(int dummy, TPKT *p)
{
  connect_state=2;
  char rev[16];
  //Кто будет менять в этом месте идентификатор клиента, буду банить на уровне сервера!!!
  //А Вова будет банить на форуме!
  snprintf(rev,9,"Sie_%04d",__SVN_REVISION__);
//  snprintf(rev,9,"Sie.%04d",ELF_REVISION);

  TPKT *p2=malloc(sizeof(PKT)+8);
  p2->pkt.uin=UIN;
  p2->pkt.type=T_SETCLIENT_ID;
  p2->pkt.data_len=8;
  memcpy(p2->data,rev,8);
  SendAnswer(0,p2);
  SendAnswer(dummy,p);
  RXstate=-(int)sizeof(PKT);
}*/

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

void get_answer(void)
{
  void *p;
  int i=RXstate;
  int j;
  int n;
  char rb[7168];
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
        // case 35:
        //  aa(&RXbuf);     
        //  break;
     /*   case 36:
          {
            typedef struct {
              char one[0x64];
              int answer;
              char two[8];
            }PACKET;          
             PACKET p2;
             memcpy((void*)(&p2),(void*)(&RXbuf.data), sizeof(PACKET));
            
             TPKT *p=malloc(sizeof(PKT)+1);
             zeromem(p,sizeof(PKT)+1);  
             p->pkt.uin = p2.answer;
             p->pkt.type = 36;
             p->pkt.data_len = 1;
             SUBPROC((void *)SendAnswer,0,p);
          }
          break;*/
	case T_LOGIN:
	  //Удачно залогинились
	  //Посылаем в MMI
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  Play(sndStartup);
	  //        GBS_StartTimerProc(&tmr_ping,120*TMR_SECOND,call_ping);
	  snprintf(logmsg,255,"T_LOGIN %s",RXbuf.data);
	  connect_state=3;
          host_counter--; //Если уж законнектились, будем сидеть на этом сервере
	  SMART_REDRAW();
	  break;
	case T_XTEXT_ACK:
	case T_GROUPID:
	case T_GROUPFOLLOW:
	case T_CLENTRY:
      //  case 36:
          //Посылаем в MMI
	  n=i+sizeof(PKT)+1;
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  //snprintf(logmsg,255,"CL: %s",RXbuf.data);
          if(RXbuf.pkt.type == T_XTEXT_ACK)
            snprintf(logmsg,255,"T_XTEXT_ACK %d %s",n,RXbuf.data);
          if(RXbuf.pkt.type == T_GROUPID)
            snprintf(logmsg,255,"T_GROUPID %d %s",n,RXbuf.data);
          
          if(RXbuf.pkt.type == T_GROUPFOLLOW)
            snprintf(logmsg,255,"T_GROUPFOLLOW %d %s",n,RXbuf.data);
          
          if(RXbuf.pkt.type == T_CLENTRY)
            snprintf(logmsg,255,"T_CLENTRY %d %s",n,RXbuf.data);
          
          if(RXbuf.pkt.type == 36)
            snprintf(logmsg,255,"PickOFF xD %d %s",n,RXbuf.data);
	  break;
	case T_STATUSCHANGE:
        case T_CLIENTID:
	  n=i+sizeof(PKT);
	  p=malloc(n);
	  memcpy(p,&RXbuf,n);
	  snprintf(logmsg,255,LG_GRSTATUSCHNG,RXbuf.pkt.uin,*((unsigned short *)(RXbuf.data)));
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  break;
	case T_ERROR:
          if(strstr(RXbuf.data, "Error code 1") || strstr(RXbuf.data, "110"))
          {
            snprintf(logmsg,255,LG_GRERROR,LG_GRDBLCONNECT);
            disautorecconect = 1;
          }
          else
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
	  snprintf(logmsg,255,LG_GRRECVMSG,RXbuf.pkt.uin,RXbuf.data);
	  SendMSGACK(TOTALRECEIVED);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
          
#ifdef GUI_INTERFACE
          void GotoFirstPosition();
          GotoFirstPosition();
#endif    
	  SMART_REDRAW();
          if(Is_Extd_Sound)
          {
            char extsnd[128];
            zeromem(extsnd,128);
            strcpy(extsnd,sndMsg);
            for(int i=strlen(extsnd);extsnd[i]!='\\';i--) extsnd[i]=0;
            sprintf((char*)(extsnd+strlen(extsnd)),"msg\\%u",RXbuf.pkt.uin); 
  
            char *p = GetExtSnd(extsnd);
            if(p)
              Play(p);
            else
              Play(sndMsg);
            
            if(p) mfree(p);
          }
          else
            Play(sndMsg);
	  break;
	case T_SSLRESP:
	  LockSched();
	  ShowMSG(1,(int)RXbuf.data);
	  UnlockSched();
	  break;
	case T_SRV_ACK:
	  if (FindContactLOGQByAck(&RXbuf))
          {
#ifdef GUI_INTERFACE
            RefreshCL();
            SMART_REDRAW();
#endif
            Play(sndMsgSent);          
          }
	case T_CLIENT_ACK:
	  p=malloc(sizeof(PKT)+2);
	  memcpy(p,&RXbuf,sizeof(PKT)+2);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
           snprintf(logmsg,255,"T_CLIENT_ACK %d %s",sizeof(PKT)+2,RXbuf.data);
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
	    snprintf(logmsg,255,"Ping %d-%d seconds!",s1,s1+1);
#ifdef GUI_INTERFACE
            if(IsGuiOnTop(maingui_id))
              PrintMessage(logmsg,5);
            else
            {
#endif
	      LockSched();            
	      ShowMSG(1,(int)logmsg);
	      UnlockSched();
#ifdef GUI_INTERFACE
            }
#endif
            Play(pingsnd);
	  }
	  break;
        case T_LASTPRIVACY:
          n=i+sizeof(PKT);
          p=malloc(n);
          memcpy(p,&RXbuf,n);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
          
    snprintf(logmsg,255,"T_LASTPRIVACY %d %s",n,RXbuf.data);
	  break;
	}
          /*
        if(RXbuf.pkt.type == 35 || RXbuf.pkt.type == 36)
        {
            unsigned int io_error = 0;
                int h;
             if(RXbuf.pkt.type == 35)
              h = fopen("4:\\222_35.txt",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE, &io_error);
            
            
            if(RXbuf.pkt.type == 36)
              h = fopen("4:\\222_36.txt",A_ReadWrite + A_Create + A_Append + A_BIN,P_READ+P_WRITE, &io_error);
            fclose(h,&io_error);
         }
        else*/
         ad(&RXbuf);
	i=-(int)sizeof(PKT); //А может еще есть данные
      }
    }
  }
  RXstate=i;
  //  GBS_StartTimerProc(&tmr_dorecv,3000,dorecv);
  //  SMART_REDRAW();
}



int NEW_MSG;

void AddStringToLog(CLIST *t, int code, char *s, const char *name, unsigned int IDforACK,int SaveXST2History)
{
  char hs[128], *lastX;
  TTime tt;
  TDate d;
  GetDateTime(&d,&tt);
  int i;

  if ((code==3 || code==4) && NOT_LOG_SAME_XTXT)
  {
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    lastX = GetLastXTextLOGQ(t);
    if(lastX)
      if(strcmp(lastX, s) == 0) return;
  }

  snprintf(hs,127,"%02d:%02d:%02d %02d.%02d.%02d %s:\r\n",
           tt.hour,tt.min,tt.sec,d.day,d.month,d.year,name
            /*( (strcmp(name,I_str)==0) ? t->send : t->recv )*/);
  //if(code != 3 || LOG_XTXT) //Нужно сохранять иксстатус
  if((code != 3 && code!=4) || ((LOG_XTXT) && (SaveXST2History)))
    Add2History(t, hs, s, code); // Запись хистори
  LOGQ *p=NewLOGQ(s);
  snprintf(p->hdr,79,"[%d]%02d:%02d:%02d %02d.%02d %s:",
           ( (strcmp(name,I_str)==0) ? t->send : t->recv ),
           tt.hour,tt.min,tt.sec,d.day,d.month,name);
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
  if (code==3 || code==4)
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
    total_unread_all++;
    NEW_MSG=0;
  }
  
  t->isunread=1;
L_NOINC:
  ChangeContactPos(t);
}

void ParseAnswer(WSHDR *ws, const char *s);

int time_to_stop_t9;

void ParseXStatusText(WSHDR *ws, const char *s, int color,int xstate)
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
            color = (type==1)?O_I_COLOR:((type==3 || type==4)?O_X_COLOR:O_TO_COLOR);
            font = (type==3 || type==4)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
          }
          else
          {
            color = (type==1)?I_COLOR:((type==3 || type==4)?X_COLOR:TO_COLOR);
            font = (type==3 || type==4)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
          }
        }
        else
	  color=UNACK_COLOR; //Серый
      }
      PrepareEditControl(&ec);
      if ((p->type&0x0F)!=3 && (p->type&0x0F)!=4)
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
      if ((p->type&0x0F)!=3 && (p->type&0x0F)!=4)
      {
        ParseAnswer(ews,p->text);
      }
      else
      {//ed_contact->xstatus
        ParseXStatusText(ews, p->text, (p->type&0x10)?O_X_COLOR:X_COLOR, ed_struct->ed_contact->xstate);
      }
      PrepareEditControl(&ec);
      ConstructEditControl(&ec,
                           ECT_NORMAL_TEXT,
                           ews->wsbody[0] ? ECF_APPEND_EOL|ECF_DISABLE_T9 : ECF_DELSTR,
                           ews,ews->wsbody[0]);
      PrepareEditCOptions(&ec_options);
//#ifdef M75
      if ((p->type&0x0F)!=3 && (p->type&0x0F)!=4)
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
    total_unread--;
  else
    ed_struct->requested_decrement_total_unread++;
  
  ed_struct->ed_contact->isunread=0;

  total_unread_all -= ed_struct->ed_contact->unread_msg;
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

void set_my_xstatus(void)
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
}

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
            time_to_stop_t9=3;
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


/*static char *GetStatusByIconIndex(int icon)
{
  char stat[32];  
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
//    default :            sprintf(stat,empty_str);       break;
   }
   return stat;
}*/

/*
*  Функция: GetStatusByIconIndex
*  Описание: Получание текста статусов по иконке, нужно для сообщения в чате для информации о времени подключении\отключении контакта
*  Параметры: int icon - иконка контакта
*  Возвращаемое значение: Текст статуса
*/
const char *GetStatusByIconIndex(int icon)
{  
  switch(icon)
   {
    case IS_ONLINE :     return lgpData[LGP_StOnline];
    case IS_OFFLINE :    return lgpData[LGP_StOffline];
    case IS_INVISIBLE :  return lgpData[LGP_StInvis];
    case IS_AWAY :       return lgpData[LGP_StAway];
    case IS_NA :         return lgpData[LGP_StNa];
    case IS_OCCUPIED:    return lgpData[LGP_StOccup];
    case IS_DND :        return lgpData[LGP_StDnd];
    case IS_DEPRESSION : return lgpData[LGP_StDepression];
    case IS_EVIL :       return lgpData[LGP_StEvil];
    case IS_HOME :       return lgpData[LGP_StHome];
    case IS_LUNCH :      return lgpData[LGP_StLunch];
    case IS_WORK :       return lgpData[LGP_StWork];
    case IS_FFC :        return lgpData[LGP_StFfc];
//    default :            sprintf(stat,empty_str);       break;
   }
   return lgpData[LGP_StOffline];
}


int req_info_contact = 0; // флаг запроса инфы о контакте
extern const int Send_Auto_Status;


#ifdef CLV_LIST

int FindSubWord(char *buffer, char *subword)
{
  //ShowMSG(1,(int)buffer);
 // ShowMSG(1,(int)subword);
  int len,i;
  i = 0;
  len = strlen(subword);
  while(buffer[i] != 0)
  {
    if(buffer[i] == subword[0])
       if(!memcmp(&buffer[i],&subword[0],len)) return(1);
    i++;
  }
  return(0);
}

/*
*  Функция: GetCLV
*  Описание: Проверка контакта на видимость, псевдо-видимость для натаськи так сказать...
*            пишем (или берем файл из QIP'a) UIN пользователей которым моем отправлять текст автостатуса
*             когда находимся в состоянии "Видимый для списк видящих"
*  Параметры: CLIT *t
*  Возвращаемое значение: 
*           retval = 0 - Контакт отсутствует в списке
*           retval = 1 - Контакт есть, значит можно отправлять текст =)
*/
int GetCLV(CLIST *t)
{
  unsigned int io_error;
  char path[128];
  sprintf(path,"%s\\%u.clv",TEMPLATES_PATH,UIN);
  int hFile = fopen(path,A_ReadOnly,P_READ, &io_error);
  char *buf = NULL;
  char tmp[9];
  int retval = 0;
  const char _u[]="%u";
  if(hFile!=-1)
  {
    buf = malloc(10000);
    fread(hFile, buf, 9999, &io_error);
    
    sprintf(tmp, _u, t->uin);
    tmp[strlen(tmp)] = 0;
    int fuin = FindSubWord(buf,tmp);
    //int fname = FindSubWord(buf,t->name);
    if(fuin > 0)// || fname)
        retval = 1;
  }
  fclose(hFile, &io_error); 
  if(buf) mfree(buf);
  return retval;
}
#endif

#ifdef AUTOANSWER

/*
*  Функция: AutoAnswer
*  Описание: Автоответчик по бездействию, посылает пользовательское сообщение в чат
*  Параметры: CLIST *t - структура контакта
*  Возвращаемое значение: 1 - если автоответчик сработал
*                         0 - автоответик не сработал
*/

int AutoAnswer(CLIST *t)
{
  extern const char exept_list[];
  int retval = 0;
 
  //если запросили инфу о контакте то не отсылать текст автостатуса =)
  if(req_info_contact) { req_info_contact = 0; return 0; }

  if(Send_Auto_Status && away_m == 1 && ((t->automsgcount++) < automsgcount))
  {
      //если видим тока для списка видимых, то проверяем есть ли в файле templates//[uin].clv номер или ник для которого мона отослать автостатус
      if(CurrentPrivateStatus == 2/*PL_VISLIST_CAN_SEE*/ && !GetCLV(t)) return 0;
      if(CurrentPrivateStatus == 1) return 0;//если полный инвиз то ни че не отправляем никому

      retval = 1;
      TPKT *p;    
 
      int m = (msg_away_timer*5)/60;
      int sec = (msg_away_timer*5)%60;
      int h = m/60;
      
      //1 час 2 3 4 часа 5 > часов //(h == 1 ? "час" : ( (h > 1 && h < 5) ? "часа" : "часов" )) ;     
      char *hour_text = malloc(64);
      
      if(h>0) sprintf(hour_text,"%02d %s",h, (h == 1 ? "час" : ( (h > 1 && h < 5) ? "часа" : "часов" )) );
      else    strcpy(hour_text, empty_str);
      
      char *min_text = (m == 1 ? "минуту" :( (m > 1 && m < 5) ? "минуты" : "минут"));
      
      sprintf(away_msg, "Автоответчик: %s %s! %s ( %s [бездествую %s %02d %s %02d %s] )",
                         awayHelloMsg, t->name, awayMsg, away_msg_time, hour_text, m%60, min_text, sec, "секунд");
      
      p=malloc(sizeof(PKT)+strlen(away_msg)+1);
      p->pkt.uin=t->uin;
      p->pkt.type=T_SENDMSG;
      p->pkt.data_len=strlen(away_msg);
      strcpy(p->data,away_msg);
      AddStringToLog(t,0x01,t->automsgcount < 1 ? away_msg : "Send autoaway text...",I_str,(++SENDMSGCOUNT)&0x7FFF,1);
  
      SUBPROC((void *)SendAnswer,0,p);
      mfree(hour_text);
  }
  return retval;
}

#endif


void ProcessPacket(TPKT *p)
{
  extern const int VIBR_TYPE, VIBR_ON_CONNECT;
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
	RecountMenu(t, 0);
      }
      else
      {
        RecountMenu(AddContact(p->pkt.uin,p->data, GROUP_CACHE, 0),0);
      }
    }
    else
    {
      if(VIBR_ON_CONNECT)
      {
        vibra_count=1;
        start_vibra();
      }
      FIRST_START = 1;
      GROUP_CACHE=0;
      //ask_my_info();
#ifndef GUI_INTERFACE
      if (contactlist_menu_id)
      {
	RecountMenu(NULL, 1);
      }
      else
        create_contactlist_menu();
#else     
      if (ShowLoadScreen==0 && maingui_id)
      {
	//RecountMenu(NULL, 1);
      }
      else
      {
        if(ShowLoadScreen) ShowLoadScreen=0;        
        SMART_REDRAW();  
      }
#endif
    }
    break;
  case T_GROUPID:
    if (t=FindGroupByID(GROUP_CACHE=p->pkt.uin))
    {
      strncpy(t->name,p->data,63);
      ChangeContactPos(t);
      RecountMenu(t, 1);
    }
    else
    {
      RecountMenu(AddGroup(p->pkt.uin,p->data), 1);
    }
    break;
  case T_GROUPFOLLOW:
    GROUP_CACHE=p->pkt.uin;
    break;
  case T_CLIENTID:
    if (t=FindContactByUin(p->pkt.uin)){
      t->clientid=*(char *)p->data;
    }
    break;
  case T_STATUSCHANGE:
    t=FindContactByUin(p->pkt.uin);
    if (t)
    {
      int i=t->state;
      CLIST *oldt=NULL;
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
      }
#ifdef GUI_INTERFACE
      if(!ShowLoadScreen)
        oldt=FindContactByN(clistm->curpos);
#else
      if (contactlist_menu_id)
      {
	oldt=FindContactByN(GetCurMenuItem(FindGUIbyId(contactlist_menu_id,NULL)));
      }
#endif
      t->state=*((unsigned short *)(p->data));
      LogStatusChange(t);
      ChangeContactPos(t);
      RecountMenu(oldt, 1);

     // char *s = GetStatusByIconIndex(GetIconIndex(t));         
      //TTime time;
      GetDateTime(NULL,&t->time_struct);
      char *txt=malloc(64);
      sprintf(txt," %s: %02d:%02d:%02d",GetStatusByIconIndex(GetIconIndex(t)),t->time_struct.hour,t->time_struct.min,t->time_struct.sec);      
       
      if(isLogStatusChange && t->state != i)// by kluchnik 05.04.08
      {
//        if(t->state == IS_ONLINE || t->state == IS_OFFLINE)
              do_StatusChangeWrite(txt, t, 0);
        
        FreeStatusTime(t);
        t->sttime=malloc(64);
        zeromem(t->sttime,sizeof(t->sttime));
        snprintf(t->sttime,64,txt);
        AddStringToLog(t,0x04,txt,"Status change",0xFFFFFFFF, 0);
        ReqAddMsgToChat(t);  
        //RecountMenu(NULL, 0);
      }
#ifdef GUI_INTERFACE
       sprintf(on_off_buf,"[%s: %s]\n",txt+1,t->name);
#endif
       mfree(txt);

       if ((t->state!=0xFFFF)&&(i==0xFFFF))//Звук
       {
         {
            char extsnd_path[128];
            zeromem(extsnd_path,sizeof(extsnd_path));
            if(t->uin == ADDITION_UIN && ExtSndModeIndivd)//если звук для индивидуального контакта
            {
                 ExtSndMode_flag = 1;//то ставил флаг, чтоб играть независимо включен звук или нет...
                 goto p_extsnd;
            }
            if(/*ExtSndModeIndivd ||*/ (Is_Extd_Sound && Is_Sounds_Enabled))//расширенные звуки для каждого контакта
            {
            p_extsnd:

              strcpy(extsnd_path,sndMsg);
              for( int i=strlen(extsnd_path) ; extsnd_path[i] != '\\' ; i-- ) extsnd_path[i] = 0;
              sprintf((char*)(extsnd_path+strlen(extsnd_path)),"online\\%u",t->uin);
              

              char *p = GetExtSnd(extsnd_path);
                
              if(p) Play(p);
              else  Play(sndGlobal);
                
              if(p) mfree(p);                       
            }
            else
              Play(sndGlobal);
         }
          
        if(Is_POPUP_On && (PopupShow==2 || PopupShow==3))
        {
          popup[POPUP_ON].timer = POPUP_N;
///          popup[POPUP_OFF].timer=0;
   ///       popup[POPUP_MSG].timer=0;
#ifdef GUI_INTERFACE
          popup[POPUP_ON].icon = GetIconByItem(GetIconIndex(t));
          popup[POPUP_ON].xiconf = t->xstate;
          popup[POPUP_ON].xicon = GetXIconByItem(t->xstate);
#endif
          sprintf(popup[POPUP_ON].name,PopupOnlineString,t->name);
          show_on_contact();
        }

       if(Is_ILU_By_Online)
         IlluminationOn(IllByONCantact,0,ILL_SEND_TMR,ILL_RECV_FADE);
#ifdef GUI_INTERFACE
       t->isonline = 2;
       isonline = 1;//ON_OFF_TIME;
       on_off_timer = ON_OFF_TIME;
#ifndef DEBUG
      if(ShowLoadScreen == 0)
          RedrawOnline();
#endif
#endif
     }
      
      if ((i!=0xFFFF)&&(t->state==0xFFFF))//Звук
      {
#ifdef GUI_INTERFACE
        on_off_timer=ON_OFF_TIME;
#endif
        if(Is_POPUP_On && isPopupOff)
        {
#ifdef GUI_INTERFACE
          popup[POPUP_OFF].icon = GetIconByItem(IS_OFFLINE);
#endif
          popup[POPUP_OFF].timer = POPUP_N;
        //  popup[POPUP_ON].timer=0;
      //    popup[POPUP_MSG].timer=0;
          
          sprintf(popup[POPUP_OFF].name,PopupOnlineString,t->name);
          show_off_contact();        
        }  
        Play(sndSrvMsg);        
      }
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
#ifdef GUI_INTERFACE    
    Get_Online_Contact();    
     if(ShowLoadScreen == 0) RefreshCL();
#endif
    break;
  case T_RECVMSG:
    NEW_MSG=1;    
    t=FindContactByUin(p->pkt.uin);
      
    if (!t)
    {
#ifdef ANTISPAM //14.08.2009 by kluchnik
      //если запрос об авторизации от контакта вне списка, то на куй шлем...боримся со спамом
      //нет сообщений от контакта нет лога сообщений и пришел запрос об авторизации
      if(strcmp(p->data,"Auth REQ: ") == 0)
      {
        blocked_spam++;
        char log_path[128];
        char log1[128];
        zeromem(log1,128);
        TDate date;
        TTime time;
        GetDateTime(&date,&time);
        strcpy(log_path,TEMPLATES_PATH);
        strcat(log_path,"\\antispam.log");
        sprintf(log1,"%02d:%02d:%02d %02d.%02d.%04d Auth REQ noname contact blocked =) (uin %d %s)\n",
                                    time.hour,time.min,time.sec, date.day, date.month,date.year, t->uin,t->name);
        log(log_path,log1);
        PrintMessage("Block spam", 15);
        return;
      }
#endif
      
      sprintf(s,percent_d,p->pkt.uin);
      t=AddContact(p->pkt.uin,s,GROUP_CACHE,0);
    }
    
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    t->isactive=ACTIVE_TIME;
    
    CountMessage[0]++;//общее принятых
    t->recv++;//тока принятых для этого контакта
    
    //    ChangeContactPos(t);
    if(VIBR_TYPE)
      vibra_count=2;
    else
      vibra_count=1;
    start_vibra();
    if(Is_Double_Vibra_On) Reminder_timer();

    IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007

    if (t->name[0]=='#')
    {
      //Если это конференция, патчим имя
      char *s=strchr(p->data,'>');
      //Если нашли символ > и после него пробел и ник короче 16 символов
      if (s)
      {
	if ((s[1]==' ')&&((s-p->data)<40))
	{
	  *s=0; //Режем строку
	  AddStringToLog(t,0x02,s+2,p->data,0xFFFFFFFF,1); //Добавляем имя из текста сообщения
	  goto L1;
	}
      }
    }
//    AddStringToLog(t,0x02,p->data,t->name,0xFFFFFFFF,1);
#ifdef AUTOANSWER
    AutoAnswer(t);
#endif
    {
      AddStringToLog(t,0x02,p->data,t->name,0xFFFFFFFF,1);
      if(Is_POPUP_On && (PopupShow==1 || PopupShow==3))
      {
        sprintf(popup[POPUP_MSG].name,PopupMsgString,t->name);
        popup[POPUP_MSG].timer = POPUP_N;
     //   popup[POPUP_ON].timer=0;
     //   popup[POPUP_OFF].timer=0;
        show_pp_sender();
      }
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
    RecountMenu(t, 1);
    extern const int DEVELOP_IF;
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
    }
#ifdef GUI_INTERFACE
    RefreshCL();
#endif
    break;
  case T_SRV_ACK:
  case T_CLIENT_ACK:
    q=FindContactLOGQByAck(p);
/*    if ((
	IsGuiOnTop(contactlist_menu_id)||
	  IsGuiOnTop(edchat_id)
	    )&&(q))
    {
      DrawRoundedFrame(ScreenW()-8,YDISP,ScreenW()-1,YDISP+7,0,0,0,
		       GetPaletteAdrByColorIndex(0),
		       GetPaletteAdrByColorIndex(p->pkt.type==T_SRV_ACK?3:4));
    }*/
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
                  IlluminationOn(ILL_DISP_SEND,ILL_KEYS_SEND,ILL_SEND_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
#ifdef GUI_INTERFACE
                RefreshCL();
                SMART_REDRAW();
#endif
              }
	    }
	  }
	}
      }
    }
#ifdef GUI_INTERFACE
    RefreshCL();
    SMART_REDRAW();
#endif
    break;
  case T_XTEXT_ACK:
    t=FindContactByUin(p->pkt.uin);
    if (t)
    {
      int i;
      int j;
      FreeXText(t);
      i=p->pkt.data_len;
      memcpy(t->xtext=malloc(i),p->data,i);
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
//      if (IsGuiOnTop(contactlist_menu_id)) RefreshGUI();
      // Только если редактор не наверху и x-status не пустой
#ifdef GUI_INTERFACE
      if(IsGuiOnTop(maingui_id) && strlen(p->data))
        PrintMessage(s,5);
      else
#endif
       if(!edchat_id && strlen(p->data))
       {
         ShowMSG(0,(int)s);
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

//===============================================================================================

//----------------------------- SLI ---------------------------//

#ifdef ELKA

#pragma swi_number=54
__swi __arm void SLI_SetState(unsigned char state);

GBSTMR sli_tmr;

/*
*  Функция: Sli_On & SLI_Off
*  Описание: Включение и отключения светодиода
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/

void SLI_On()
{
  void SLI_Off();
  SLI_SetState(1);   
  GBS_StartTimerProc(&sli_tmr,TMR_SECOND>>2, SLI_Off);
}

void SLI_Off()
{
  SLI_SetState(2);
  GBS_StartTimerProc(&sli_tmr, TMR_SECOND>>2, SLI_On);
}

/*
*  Функция: SLI_check
*  Описание: Если есть неотвеченные сообщения, то запускает функцию мигания SLI
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void SLI_check()
{
  if(!Is_SLI_On) return;
  GBS_DelTimer(&sli_tmr);
  if(total_unread) SLI_On();  
  else             SLI_SetState(0);
}
#endif



/*********************************************************************/


#ifdef GUI_INTERFACE
/*
* Здесть пошел код прорисовки графического контакт листа
*/
//static GUIMENU clistm;//clist menu

char clrBlack[]    = {0x00,0x00,0x00,0x64};
char clrWhite[]    = {0xFF,0xFF,0xFF,0x64};
char clrRed[]      = {0xFF,0x00,0x00,0x64};
char transparent[] = {0x00,0x00,0x00,0x00};
char blue[]        = "\x00\x00\xFF\x64";

const char _t[] = "%t";
const char _d[] = "%d";
const char _s[] = "%s";

unsigned int HeaderY;//Размер размера заголовка по У

int PNG_SOFTKEY=0;
int PNG_HEADER=0;  
/*
#ifdef ELKA
  #define ICONBAR //Если нужен иконбар
#endif
*/
#ifdef ELKA  
  #ifndef ICONBAR
    #undef  YDISP
    #define YDISP 0
  #endif
#endif

#define CLIST_Y1 (YDISP+FontH(ClFont)+4)


//------------------------------------------


void Get_Online_Contact()
{
  CLIST *t;
  online_contact=0;
  total_contact=0;
  t=(CLIST *)(&cltop);
  while(t=(CLIST *)(t->next))
  {
    if (!t->isgroup)
    {
      total_contact++;
      if (t->state!=0xFFFF) online_contact++;
    }
  }
}


//------------------------------------------

/*
*  Функция: DoScreen
*  Описание: делаем скрин экрана, и возвращаем его в IMGHDR
*  Параметры: 
*     ScrH - высота скриншота
*     ShiftY - смещение по Y
*  Возвращаемое значение: Возвращает скриншот выбранной области экрана в IMGHDR
*/
IMGHDR DoScreen(int ScrH,int ShiftY)
{
  IMGHDR screen = {0,0,8,""};
  int ScrW = ScreenW();
  char *ms = RamScreenBuffer();
  screen.w = ScrW;
  screen.h = ScrH;  
  screen.bitmap = malloc(ScrW*ScrH*2);
  ms += (ShiftY*2);
  memcpy(screen.bitmap, ms, ScrW*ScrH*2);
  return screen;
}


//------------------------------------------


void RecountMenu(CLIST *req, int needfocus)
{
  int i;
  int j;
  if(!needfocus) return;
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
  //if(needfocus)
    clistm->curpos=j;
  SMART_REDRAW();
}


//------------------------------------------
/*
//#pragma inline
int FontH(int font)
{
 return GetFontYSIZE(font);
}
*/
//#define FontH(font) GetFontYSIZE(font)
//------------------------------------------


/*
#pragma inline
int GetWidth(WSHDR *ws, int font)
{
 return Get_WS_width(ws,font);
}
*/


//#pragma inline
/*
*  Функция: GetWidth
*  Описание: Получение длинны текстовой строки в пикселях
*  Параметры: 
*       char *s - строка текста
*       inr font - шрифт, которым строка отрисовывается
*  Возвращаемое значение: Длинна в пикселях
*/
int GetWidth(char *s, int font)
{
  WSHDR *ws=AllocWS(strlen(s));
  wsprintf(ws,_t,s);
  unsigned short *body=ws->wsbody;
  int len=body[0],width;
  if(len>0) width=1; else width=0;
  while(len>0)
  {
    width+=GetSymbolWidth(body[len],font);
    len--; 
  }
  FreeWS(ws);
  return (width+1);
}


//------------------------------------------

/*
*  Функция: DrwStr
*  Описание: Альтернатива DrawString, вроде как быстрее отрисовывает...
*  Параметры: Как и в DrawString
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawWSStr(WSHDR *ws, int x, int y,int w,int h, int font,int flag,char *pen, char *brush)
{
  RECT rc;
  DRWOBJ drwobj;
  StoreXYWHtoRECT(&rc,x,y,w,h);
  SetPropTo_Obj1(&drwobj,&rc,0,ws,font,flag);
  SetColor(&drwobj,pen,brush);
  DrawObject(&drwobj);
}

/*
void TextOut(char *text, int x, int y, int font, int flag, char *pen, char *brush)
{
  WSHDR *txt = AllocWS(strlen(text));
  wsprintf(txt, "%s", text);
  int textlen = 0;
  for(int i = 0; i < strlen(text); i++)
    textlen += GetSymbolWidth(text[i], font);
  DrwStr(txt, x, y, textlen, GetFontYSIZE(font), font, flag, pen, brush);
}*/


/*
*  Функция: DrawStr
*  Описание: Тот же DrawString, только с возможностью обрезания выводимого текста находу, по размерам x2-x
*  Параметры: 
*       char *str - выводимая строка
*       int x,y,x2,y2 - координаты поля для вывода
*       int needcut - флаг указания, нужно обрезать строку по размеру поля x2-x или нет.
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawStr(char *str, int x, int y, int x2,int y2, int align, int font, char *color,int needcut)
{
  if(!str) return;
  if(!strlen(str)) return;
  WSHDR *ws = AllocWS(strlen(str));
  wsprintf(ws,_t,str);    

  if(needcut)
  {
    int len = GetWidth(str,font);
    if(len > x2-x)//Если имя длиннее области для рисования,отрезаем лишние символы
    {
      int pos = ws->wsbody[0] + 1;
      while((len > x2-x) && pos)
      {
        CutWSTR(ws,pos);
        len = Get_WS_width(ws,font);
        pos--;
      }  
    }
  }
 
//  void DrwStr(WSHDR *ws, int x, int y,int w,int h, int font,int flag,char *pen, char *brush)
  //DrwStr(ws,x,y,x2,y2,font,align,color,0);
  DrawString(ws, x, y, x2, y2, font, align, color, 0);
  FreeWS(ws);
}


//------------------------------------------


static volatile int scroll_disp;
static volatile int max_scroll_disp;
GBSTMR tmr_scroll;

/*
*  Функция: scroll_timer_proc
*  Описание: Таймер для плавной прорисовки бегущей строки
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
static void scroll_timer_proc(void)
{
  int i=max_scroll_disp;
  if (i)
  {
    if (scroll_disp>=i)
    {
      scroll_disp=0;
      GBS_StartTimerProc(&tmr_scroll,TMR_SECOND,scroll_timer_proc);
    }
    else
    {
      scroll_disp+=3;
      GBS_StartTimerProc(&tmr_scroll,scroll_disp<i ? TMR_SECOND : TMR_SECOND*2,scroll_timer_proc);
    }
//    SMART_REDRAW();
  }
}

static void DisableScroll(void)
{
  GBS_DelTimer(&tmr_scroll);
  max_scroll_disp=0;
  scroll_disp=0;
}


//------------------------------------------

/*
*  Функция: DrawScrollStr
*  Описание: Отрисовка бегужей строки
*  Параметры: 
*     char *s - текст для отрисовки
*     int x, y, x2, y2 - область рисования
*     int font - размер шрифта
*     char *color - цвет шрифта
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawScrollStr(char *s,int x,int y,int x2,int y2,int font,char *color)
{
  WSHDR *ws = AllocWS(strlen(s));
  wsprintf(ws,_t,s);
  DrawScrollString(ws, x, y, x2, y2, scroll_disp + 1, font, 0x80, color, 0);
  FreeWS(ws);
}


//------------------------------------------

/*
//config
#define LG_NOT_ACTION       "Без действия"
#define LG_SOUND            "Звук"
#define LG_VIBRA            "Вибра"
#define LG_INVIS            "Инвиз"
#define LG_VIBRA_REM        "Реж.напоминания"
#define LG_CLAER_ALL_CHAT   "Очистить чаты"
#define LG_AUTOMSG          "Автоответчик"
#define LG_CONFIG           "Конфиг"
#define LG_PING             "Пинг"
#define LG_HIHE_LOCK        "Заблокировать"
#define LG_IDLE             "На гэ"
*/

/*
*  Функция: GetLongPressText
*  Описание: Текст функции которая вызывается при длинном нажатии клавиши в контакт листе
*  Параметры: Никаких
*  Возвращаемое значение: Строка текста
*/
char *GetLongPressText()
{
  char *text[]={LG_NOT_ACTION,LG_SOUND,LG_VIBRA,LG_INVIS,LG_VIBRA_REM,LG_CLAER_ALL_CHAT,LG_AUTOMSG,LG_CONFIG,LG_PING,LG_HIHE_LOCK,LG_IDLE,LG_MNUSHOWOFF,LG_MNUSHOWGROUP};  
  char *ret = malloc(32);
  zeromem(ret,32);

  switch(ContactT9Key[0])
  {
    case '0': strncpy(ret,text[longAct_0],32); break;    
    case '1': strncpy(ret,text[longAct_1],32); break;    
    case '2': strncpy(ret,text[longAct_2],32); break;    
    case '3': strncpy(ret,text[longAct_3],32); break;    
    case '4': strncpy(ret,text[longAct_4],32); break;    
    case '5': strncpy(ret,text[longAct_5],32); break;    
    case '6': strncpy(ret,text[longAct_6],32); break;    
    case '7': strncpy(ret,text[longAct_7],32); break;    
    case '8': strncpy(ret,text[longAct_8],32); break;    
    case '9': strncpy(ret,text[longAct_9],32); break;    
    case '*': strncpy(ret,text[longAct_10],32);  break;
    case '#': strncpy(ret,text[longAct_11],32);  break;
    default : strncpy(ret,"error",32);  break;
  }
  
  return ret;
}

IMGHDR SoftShot={0,0,8,""};
int shot_soft=0;

/*
*  Функция: DrawSoftKeys
*  Описание: Рисуем бар с софт клавишами
*  Параметры: 
*       char *left - Текст левой клавиши
*       char *right - текст правой клавиши
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawSoftKeys(char *left,char *right)
{
  TTime t;  
  char *s=malloc(32);
  
  int FntH = FontH(HeadFont);
  int scr_w = ScreenW()-1;
  int scr_h = ScreenH()-1;

  //DrawRoundedFrame(0,ScreenH()-1-FntH-2,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);
  
#ifdef ELKA
  if(shot_soft!=2)
  {
#endif 
    if(SoftBackgrClr[3]!=0)
    {
      DrawRoundedFrame(0,ScreenH()-1-FntH-2,ScreenW()-1,ScreenH()-1,0,0,0,clrBlack,clrBlack);
    
     for(int i=FontH(ClFont);i>0;i-=2)
       DrawRoundedFrame(0,scr_h-FntH-2,scr_w,scr_h-FntH+i+2,0,0,0,SoftBackgrClr,SoftBackgrClr);
    }
#ifdef ELKA
    int ScrH = FntH+2;
    int HSIZE = (ScreenH()-FntH)*ScreenW();
    SoftShot = DoScreen(ScrH, HSIZE);
    shot_soft++;
    SMART_REDRAW();
    return;
  }
  else  
   if(SoftBackgrClr[3]!=0)    
      DrwImage(&SoftShot,0,scr_h-FntH,0);
#endif
  
 // return;
  
  if(sendq_l)//Если есть не отправленные данные в буфере, то выведем вместо текста на правой клавише.
  {
    sprintf(s,"Q: %d b",sendq_l);
    DrawStr(s, scr_w - GetWidth(s,ClFont), scr_h-1-FntH, scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
  }
  else
    DrawStr(right, scr_w-GetWidth(right,ClFont), scr_h-1-FntH, scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
  

  GetDateTime(0,&t);
  sprintf(s,"%02d:%02d",t.hour,t.min);
  DrawStr(s,0,scr_h-1-FntH,scr_w,scr_h,TEXT_ALIGNMIDDLE,HeadFont,(char*)SoftFontClr,0);
  
  if(!strlen(ContactT9Key))//если нет нажатых клавиш
      DrawStr(left,2,scr_h-1-FntH,scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
  else
    if(strlen(ContactT9Key)==1)//если есть нажатая кнопка,то выводим текст для лонг пресса на левом софте
      DrawStr(GetLongPressText(),2,scr_h-1-FntH,(scr_w/2)-(GetWidth(s,HeadFont)/2),scr_h,0,HeadFont,(char*)SoftFontClr,0);
  else//очистить т9 буффер
    DrawStr("Очистить Т9",2,scr_h-1-FntH,scr_w,scr_h,0,HeadFont,(char*)SoftFontClr,0);
    

  mfree(s);
}


//------------------------------------------


int shot_header=0;
IMGHDR HeaderShot={0,0,8,""};

extern IMGHDR *GetIconByItem(int curitem);
extern IMGHDR *GetXIconByItem(int curitem);

/*
*  Функция: GetPrivateIcon
*  Описание: Формирует номер картинки своего приватного статуса
*  Параметры: Никаких
*  Возвращаемое значение: Номер картинки
*/
int GetPrivateIcon()
{
  int private_icon = 0;  
  switch(CurrentPrivateStatus)
  {
    case 0: private_icon = ICON_ALL_CAN_SEE;          break;
    case 1: private_icon = ICON_NOBODY_CAN_SEE;       break;
    case 2: private_icon = ICON_VISLIST_CAN_SEE;      break;
    case 3: private_icon = ICON_INVISLIST_CANNOT_SEE; break;
    case 4: private_icon = ICON_CONTACTLIST_CAN_SEE;  break;
  } 
 return private_icon;
}

/*
*  Функция: DrawHeader
*  Описание: Отрисовывает бар заголовка
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawHeader()
{
  int scr_w = ScreenW()-1;
  int scr_h = ScreenH()-1;
  char *s = malloc(128);
  int drawFont = HeadFont;
  HeaderY = FontH(drawFont);

  HeaderY = (clistm->FontH < 16) ? 16 : (clistm->FontH);
  HeaderY += YDISP;       
  
//  DrawRoundedFrame(0,YDISP,scr_w,HeaderY+2,0,0,0,clrBlack,clrBlack);  
  

#ifdef ELKA
  //делаем скрин заголовка и потом его рисуем,на сг и нсг ненад..там и так быстро рисуется...
  if(shot_header!=2)//два раза,чтобы экран обновился
  {
#endif 
    if(HeadBackgrClr[3]!=0)//если непрозрачный цвет
    {
      DrawRoundedFrame(0,YDISP,scr_w,HeaderY+2,0,0,0,clrBlack,clrBlack);    
      for(int i = HeaderY-YDISP ; i > 0 ; i-=2)
        DrawRoundedFrame(0,YDISP,scr_w,HeaderY-i+2,0,0,0,HeadBackgrClr,HeadBackgrClr); 
    }
#ifdef ELKA   
    int HSIZE = (HeaderY-YDISP+8)*ScreenW();
    int ScrH  =  HeaderY-YDISP+2;
    HeaderShot = DoScreen(ScrH, HSIZE);
    shot_header++;
    SMART_REDRAW();
    return;
  }
  else
   if(HeadBackgrClr[3]!=0)//если цвет не полностью прозрачен
    DrwImage(&HeaderShot,0,YDISP,0);
#endif 
  
//  return;
  int x=1;
  if(HEAD_STATUS) //Показывать статус
  {
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
    x += DrwImage(GetIconByItem(icon),x,YDISP,RET_X) + 2;
  }
  
  if(total_unread && isShowUnread)// есть не прочитанные, выводим кол-во не прочитаннх сообщений и кол-во контактов от которых пришли сообщения
  {
    sprintf(s,"%d/%d",total_unread,total_unread_all);
    DrawStr(s,x,YDISP+1,x+GetWidth(s,drawFont),YDISP+1+HeaderY,0,drawFont,(char*)HeadFontClr,0);
    x += GetWidth(s,ClFont)+1;
  }
  
  if(CurrentXStatus!=0 && HEAD_XSTATUS)//если есть хстатус и показывать х-статус
  {
    if(GetXIconByItem(CurrentXStatus)) 
      x += DrwImage(GetXIconByItem(CurrentXStatus),x,YDISP+1,RET_X) + 2;
  }
 
  if(HEAD_PRIVATE)//выводить картинку своего приватного статуса
  {
    x += DrwImage(GetIconByItem(GetPrivateIcon()),x,YDISP,RET_X) + 2;
  }
  
#ifdef ELKA
  int y = YDISP-1;
#else
  int y = 1;
#endif
  if(strlen(ContactT9Key))//Есть введенный поиск по т9
  {
    sprintf(s,key_clm_hdr_text);
    strcat(s,ContactT9Key);
    strcpy(clmenu_sk_r,key_clmenu_sk_r);
    DrawStr(s,scr_w - GetWidth(s,drawFont),y, scr_w,y+FontH(drawFont),TEXT_ALIGNRIGHT,drawFont,(char*)HeadFontClr,1);
  }
  else //если кто то подключился/отключился, выводим кто подключился
    if(on_off_timer > 0 && strlen(on_off_buf)!=0)
    {
      int i=GetWidth(on_off_buf, drawFont);
      if((i<0) || (i < scr_w-x-2))//не нада скролла
       {
         int xx = scr_w-i-2;
	 DisableScroll();
         DrawStr(on_off_buf,xx,y,scr_w,y+FontH(drawFont),TEXT_ALIGNRIGHT,drawFont,(char*)HeadFontClr,0);
       }
      else//иначе скроллим
      {
	if(!max_scroll_disp)
           GBS_StartTimerProc(&tmr_scroll,TMR_SECOND,scroll_timer_proc);
        
	max_scroll_disp = (x - (scr_w-i)) - 4;
        DrawScrollStr(on_off_buf,x,y,scr_w,y+HeaderY,drawFont,(char*)HeadFontClr);
      }
      
      strcpy(clmenu_sk_r,def_clmenu_sk_r);   
    }
  else//Штатный вывод
  {
    strcpy(clmenu_sk_r,def_clmenu_sk_r);   
    strcpy(s,empty_str);
    strcpy(clistm->s,empty_str);
    /*
    if(away_m && isShowA)
    {
      sprintf(clistm->s,"[A:%s]",away_msg_time_dace);  
      strcat(s,clistm->s);
    }*/
    
    if(Is_Vibra_Enabled && isShowV)    strcat(s,"[V]") ;
    if(Is_Sounds_Enabled && isShowS)   strcat(s,"[S]") ;
    //if(Is_Double_Vibra_On && isShowVr) strcat(s,"[Vr]") ;

   // if(isShowCont)
    {
      sprintf(clistm->s,"[%d/%d]",online_contact,total_contact);
      strcat(s,clistm->s);
    }
    
  //  if(isShowBat)
    {
      sprintf(clistm->s,"[%d%%]",*RamCap()); 
      strcat(s,clistm->s);
    }
    
    DrawStr(s,(scr_w/2),y,scr_w,y+FontH(drawFont),TEXT_ALIGNRIGHT,drawFont,(char*)HeadFontClr,0);
/*
    int i = GetWidth(s, drawFont);
    if((i<0) || (i < scr_w-x-5))
     {
       clistm->i = scr_w - i - 2;
       DisableScroll();
       DrawStr(s,clistm->i,y,scr_w,y+FontH(drawFont),TEXT_ALIGNRIGHT,drawFont,(char*)HeadFontClr,0);
     }
    else
     {
      clistm->i = x+3;
      if (!max_scroll_disp)
       {
         GBS_StartTimerProc(&tmr_scroll,TMR_SECOND,scroll_timer_proc);
       }
      max_scroll_disp = clistm->i-(scr_w-i);
      DrawScrollStr(s,clistm->i,y,scr_w,y+HeaderY,drawFont,(char*)HeadFontClr);
     }*/
  }
  mfree(s);
}

//------------------------------------------


/*
*  Функция: DrawScrollBar
*  Описание: Рисуем полосу прокрутки в контакт листе
*  Параметры: 
*       int cur - текущее положение курсора
*       int total - общее кол-во пунктов в меню, для вычисления позиции курсора
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawScrollBar(int cur,int total)
{
  int scr_h = ScreenH()-1;
  int scr_w = ScreenW()-1;
  int y = scr_h - CLIST_Y1 - clistm->FontH - 2;
  int pos = sdiv(total, y*cur);
  int y2 = sdiv(total, y);
  int i = 0;
  
  DrawLine(scr_w - 2, CLIST_Y1, scr_w - 2, scr_h - FontH(ClFont) - 2, 1, clrBlack);
  DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+y2,0,0,0,clrBlack,clrBlack);
  
  for(i=0;i<y2;i+=1)
    DrawRoundedFrame(scr_w-4,CLIST_Y1+pos+1,scr_w,CLIST_Y1+pos+i,0,0,0,ScrollBarClr,ScrollBarClr);  
}


//------------------------------------------


#ifndef DEBUG
//перерисовка подключившихся контактов
GBSTMR online_tmr;

/*
*  Функция: RedrawOnline
*  Описание: Таймер для отрисовки входящего контакта, меняем флаг t->online
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void RedrawOnline()
{
  int f = 0;
  CLIST *t=(CLIST*)cltop;
  while(t=t->next)
   {
     if(t->isonline)
     {
       f = 1;       
       t->online =! t->online;
     }
   }
  
  if(f)//еще есть подключившиеся,мигаем
  {
     isonline = 1;
     SMART_REDRAW();
     GBS_StartTimerProc(&online_tmr,TMR_SECOND,RedrawOnline);
  }
  else//иначе обнуляем все и удаляем таймер
  {
    FIRST_START = 0;//показывать попапы после того как все контакты промигают первый раз
    t=(CLIST*)cltop;
    while(t=t->next){ t->online=0; t->isonline=0; }
    GBS_DelTimer(&online_tmr);
    isonline = 0;
  }
}
#endif

/*
*  Функция: DrawOnlineContact
*  Описание: Сама отрисовка контакта в цвете
*  Параметры: 
*       RECT *rc - область прорисовки
*       CLIST *t
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawOnlineContact(RECT *rc,CLIST *cl)
{   
  DrawStr(cl->name,rc->x,rc->y,rc->x2,rc->y2,0,ClBoldFont,((!cl->online) ? (blue) : (clrRed)) ,1);
#ifdef DEBUG
//  DrawStr(cl->name,rc->x,rc->y,rc->x2,rc->y2,0,ClBoldFont,((!cl->online) ? (GetColor(cl)) : (clrRed))  ,1);
  cl->online=!cl->online;
#endif
}


//------------------------------------------


#define SENDED  0
#define SEND    1
#define NOTSEND 2

typedef struct
{
 unsigned int type; //0-доставлен, 1-отправлен, 2-не отправлено
 unsigned int count;//кол-во недоставленных 
}ACKED;

ACKED *myack;

/*
*  Функция: GetAck
*  Описание: Функция проверяет состояния отправки сообщений у контакта
*  Параметры: CLIST *t
*  Возвращаемое значение: возвращает структура ACKED
*/
static ACKED *GetAck(CLIST *t)
{
  if(!ShowProcessSend) return 0;
  if(!t) return 0;
  if(!t->isactive) return 0;
  if(t->state==0xFFFF) return 0;
  
  ACKED *ack = myack;//malloc(sizeof(ACKED));
  ack->type=0;
  ack->count=0;
  LOGQ *lp;
  lp=t->log;

  while(lp)
  {
    if(lp->acked)
    {
      if (lp->acked==1)//отправлен,но не доставлен
      {
        ack->type = SEND;
        ack->count++;
      } 
      else
      {
        ack->type = SENDED;//доставлен(прозрачный)

        if(!lp->next)//если последний отправлен, чтобы не было такого еси пред. не отправлено, то не висело значение
        {
          ack->type=0;
          ack->count=0;
        }
      }
    }
    else
     if (lp->ID!=0xFFFFFFFF)//не доставлено
     {
       ack->type = NOTSEND; 
       ack->count++;
     }
    lp=lp->next;
  }
  return ack;
}


//------------------------------------------


//Получаем цвет для своих статусов,если стоит в настройках
/*
*  Функция: GetColor
*  Описание: Получаем цвет для вывода имен контактов разными цветами, в зависимости от установленного у оного статуса
*  Параметры: CLIST *t
*  Возвращаемое значение: Цвет, настроенный в конфиге
*/
char* GetColor(CLIST *t)
{
  if(t->isgroup) return (char*)GroupClr;//если группа то сразу выходим
  if(isIndividualColor)
  {
    if(t->isactive) return (char*)ActiveClr;//если есть активные чаты то выделяем
    switch(GetIconIndex(t))
    {
      case IS_ONLINE: case IS_HOME: case IS_LUNCH: case IS_WORK: return (char*)CLColorsOnline;
      case IS_AWAY:       return (char*)CLColorsAway;
      case IS_NA:         return (char*)CLColorsNA;
      case IS_OCCUPIED:   return (char*)CLColorsOccupied;
      case IS_DND:        return (char*)CLColorsDND;
      case IS_DEPRESSION: return (char*)CLColorsDepression;
      case IS_FFC:        return (char*)CLColorsFFC;
      case IS_OFFLINE:    return (char*)CLColorsOff;
      default :           return t->isactive ? (char*)ActiveClr : (char*)NotActiveClr;
    }
  }
  //если не включена опция то по старинке
  return t->isactive ? (char*)ActiveClr : (char*)NotActiveClr;
}


//------------------------------------------

unsigned int scr_w;
unsigned int scr_h;

//#pragma optimize=z 9
/*
*  Функция: DrawList
*  Описание: Функция отрисовки ников
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawList()
{  
  unsigned int font_y = 0; //высота шрифта
  unsigned int SoftH = 0;  //высота софтов
  unsigned int start_y; //начальное смещение итема на экране
  unsigned int icon;   //иконка статуса
  
  unsigned int oldY = 0;
  unsigned int drawFont; //номер шрифта каким рисуем
  char *drawColor; //цвет каким будем рисовать

  if(!cltop) return;  
    
  SetVisibleScreen();

  font_y = clistm->FontH + 2; //FontH(ClFont)+2;

  SoftH = 0;  
  if(SOFT_MODE) SoftH = FontH(HeadFont);//если показывать софты,то высота равна высоте шрифта
  clistm->itemFromDisp = sdiv(font_y, ScreenH()-CLIST_Y1-SoftH-2 ); //вол-во контактов на экране
  if(VIEW_MODE) clistm->itemFromDisp = (clistm->itemFromDisp*2)-2; //еси в 2 столбца
  if(LINE_MODE) clistm->itemFromDisp--; //курсор в 2 линии,удаляем лишний котакт на экране

  CLIST *cl;
  RECT rc;
  ACKED *ack;
 
  char *name = malloc(32);
  
  drawFont = ClFont;  //наш шрифт
  clistm->column = 0; //столбик
  clistm->nlines = 0; //кол-во линий
  clistm->curitem = 0; //текущий итем
  clistm->totalItems = CountContacts(); //общее кол-во контактов
  //чтоб не было глюков с пропаданием контактов))
  if( clistm->itemFromDisp > clistm->totalItems ) clistm->itemFromDisp = clistm->totalItems;
    
  int start_item = ((VIEW_MODE==0) ? (clistm->startItem) : (0));
  for(; (clistm->curitem < ((VIEW_MODE!=0)?(clistm->totalItems):(clistm->itemFromDisp))) ; clistm->curitem++)
  {
     if(!VIEW_MODE) goto next;//если не два столбика то пропускаем условие
     if(clistm->curitem >= (clistm->curpage-1)*clistm->itemFromDisp)//если те итемы который нужно выводить на экран со страници clistm->curpage
        {
        next:
          cl = FindContactByN(clistm->curitem + start_item);
          if(!cl) return;
          //if(cl) //если такой есть то рисуем его
          {
            icon = GetIconIndex(cl);
            start_y = 0;
            if(LINE_MODE) //если в две линии
            {
              //если итем ниже курсора,то смещаем на размер фонта + 3
              if(clistm->curitem + start_item > clistm->curpos ) start_y += font_y + 3;
              clistm->nlines = 2;
            }
            else clistm->nlines = 1;
 
            if(VIEW_MODE) //если в две колонки
            {
              if(!clistm->column) //если первая колонка
              {
                //определяем начальное смещение по у
                start_y += ( CLIST_Y1 + ( sdiv(2,(clistm->curitem - (clistm->curpage-1)*clistm->itemFromDisp)*font_y) ) ); 
                oldY = start_y;//сохраняем чтобы вывести во втором столбике,без повторного расчета
                patch_rect(&rc, 1, start_y, (scr_w/2)-5, start_y+(font_y*clistm->nlines));
              }
              else//если вторая
              {
                patch_rect(&rc, (scr_w/2)-4, oldY, scr_w-5, oldY+(font_y*clistm->nlines));
              }
              clistm->column = !clistm->column;//меняем колонку
            }
            else
            {
              start_y += (CLIST_Y1 + 2 + (clistm->curitem*font_y));
              patch_rect(&rc,2,start_y,scr_w-5,start_y+(font_y*clistm->nlines));
            }

            if(cl->isgroup && Is_Show_Groups)//группа
            {
              int onlinetotal[2];
              GetOnTotalContact(cl->group,onlinetotal);
              if(!VIEW_MODE) //если не в два столбца
              {
                snprintf(name,32,_s,cl->name);
                snprintf(clistm->s,32,"(%d/%d)",onlinetotal[0],onlinetotal[1]);
                DrawStr(clistm->s,scr_w-7-GetWidth(clistm->s,ClFont),start_y,scr_w-7,start_y+font_y,TEXT_ALIGNRIGHT,ClFont,(char*)GroupClr,1);
              }
              else
               snprintf(name,32,"%s(%d/%d)",cl->name,onlinetotal[0],onlinetotal[1]);
              if (cl->state) icon++; //Модификация иконки группы
            }
            else
            {
              ack = GetAck(cl);//Обработка процесса отправки
              snprintf(name,32,_s,cl->name);
            }
            
            //рисуем статуc
            /*if( (clistm->img = GetIconByItem((cl->online && BlinkIconByOnline)?(IS_OFFLINE):(icon))) )
                rc.x += DrwImage(clistm->img,rc.x,rc.y+1,RET_X) + 2;            
            */            
            rc.x += DrwImage(GetIconByItem((cl->online && BlinkIconByOnline  && FIRST_START == 0)?(IS_OFFLINE):(icon)),rc.x,rc.y+1,RET_X) + 2;            
            
            if(cl->unread_msg!=0)//кол-во не прочитанных
            {
              snprintf(clistm->s,32,_d,cl->unread_msg);
              DrawStr(clistm->s,rc.x,rc.y,rc.x2,rc.y2,0,ClFont,clrRed,0);
              rc.x += GetWidth(clistm->s,ClFont)+2;
            }
            
            //Рисуем кол-во отправленных или не доставленных сообщений
            if(ack && ack->count!=0 && (ShowProcessSend==1 || ShowProcessSend==3))
            {
              if(!cl->isgroup && cl->state!=0xFFFF)
              {
                //тут пробел ставим если есть непрочитанные,чтобы не сливались непрочитанные и не отправленные
                snprintf(clistm->s,3, (cl->unread_msg ? "/%d" : _d),ack->count);
                drawColor = (ack->type==SEND) ? ((char*)SendClr) : ((ack->type==NOTSEND)?((char*)UnAckedClr):0);
                DrawStr(clistm->s,rc.x,rc.y, rc.x2,rc.y2,0,drawFont,drawColor,0);
                rc.x += GetWidth(clistm->s,drawFont) + 2;
              }
            }
         
           if( clistm->curitem + start_item == clistm->curpos )//Если наш итем то выделяем
            {
              DrawRoundedFrame(rc.x-1,rc.y-1,rc.x2-1,rc.y2-1,0,0,0,ClCursorBordClr,ClCursorClr);
              if(LINE_MODE && !cl->isgroup)
              {
                if ((cl->xtext && cl->xtext[0]!=0) && (cl->state!=0xFFFF))
                  {
                    int i;
                    zeromem(clistm->s,64);
                    i=cl->xtext[0];
                    if (i>63) i=63;
                    strncpy(clistm->s,cl->xtext+1,i);
                  }
                  else
                    snprintf(clistm->s,64,percent_d,cl->uin);
                //вторая строчка
                DrawStr(clistm->s,rc.x+1,rc.y+font_y,rc.x2-rc.x-3,rc.y2+font_y,0,ClFont,(char*)CLDescriptClr,1);
              }
              if(clistm->totalItems > 1)
                DrawScrollBar(clistm->curitem+clistm->startItem,clistm->totalItems);
            }

            //рисуем хстатус
            if((cl->xstate!=0 && cl->state!=0xFFFF)/* && GetXIconByItem(cl->xstate)*/)
            {
               unsigned int i;
               i = DrwImage(GetXIconByItem(cl->xstate),rc.x,rc.y,RET_X);            
               rc.x += i ;//( i>32 ? 32 : i );
             //  rc.x = DrwImage(GetXIconByItem(cl->xstate),rc.x,rc.y,RET_X);
            }

            //Наш фонт,каким буим рисовать
            if(cl->isgroup) drawFont = ClBoldFont; //группы
            else drawFont = ( (SelBoldActiveChat==1 && (cl->isactive && cl->state!=0xFFFF)) ? ClBoldFont : ClFont );
            
            if(ShowProcessSend==2 || ShowProcessSend==3)
            {
              if(ack && ack->type!=SENDED)//если есть не отправленные
              {
                drawColor = (ack->type==SEND) ? ((char*)SendClr) : ((ack->type==NOTSEND)?((char*)UnAckedClr):0);
                clistm->i = (VIEW_MODE) ? (rc.x2-1) : (rc.x+GetWidth(name, drawFont)+3);//если в одну колонку, расчитаем х2 для рисования линии по длинне имени
                DrawRoundedFrame(rc.x,rc.y,clistm->i,rc.y2-2,0,0,0,0,drawColor);
              }
            }
            
            rc.x+=3;            
            if(cl->isonline && cl->state!=0xFFFF && FIRST_START == 0)//если контакт тока подключился то мигаем
              DrawOnlineContact(&rc,cl);
            else
            {
         //     drawColor = (cl->isgroup) ? ((char*)GroupClr) : ((cl->isactive!=0)?((char*)ActiveClr):((char*)NotActiveClr));
              if( clistm->curitem + start_item == clistm->curpos )
                 drawColor = (char*)CLColorSelCont;
              else
                  drawColor = GetColor(cl);
              DrawStr(name, rc.x, rc.y, rc.x2, rc.y2, 0, drawFont, drawColor, 1);
            }
         }//if(cl)
       }//if( i>=(clistm->curpage-1)*clistm->itemFromDisp )

     //Если рисуем больше чем нужно
      if( (clistm->curitem > clistm->itemFromDisp*(clistm->curpage)) && (VIEW_MODE) ) break;
   //  if( clistm->i > clistm->itemFromDisp*((VIEW_MODE!=0)?(clistm->curpage):(1)) ) break;
    }//for(...)
  mfree(name);
}


/************** Управление курсором *****************/

/*
*  Функция: GotoFirstPosition
*  Описание: Переход на первую позицию
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void GotoFirstPosition()
{
  clistm->startItem = 0;
  clistm->curpage = 1;
  clistm->curpos = 0;
}


/*
*  Функция: SetVisibleScreen
*  Описание: Бегаем за курсором, чтоб экран не уходил ни куда
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void SetVisibleScreen()
{
           int *curpos       = &clistm->curpos;
  unsigned int *itemFromDisp = &clistm->itemFromDisp;
  unsigned int *curpage      = &clistm->curpage;
  unsigned int *totalItems   = &clistm->totalItems;
  unsigned int *startItems   = &clistm->startItem; 
  
  if(VIEW_MODE)//в 2 столбца
  {
    if(*curpos > *itemFromDisp)//если курсор не на первой странице,то ищем нашу страницу
    {
      int i = 1;
      int j = *curpos;
      if(*itemFromDisp == *totalItems)//та мало ли =) можт кто-то отключился
        *curpage = 1;
      else
      {
        while(j > *itemFromDisp)//пока j больше кол-ва итемов на одной странице
        {
           j -= *itemFromDisp;
           i++;//номер страницы
           RefreshCL();
        }
        *curpage = i;
      }
    }
    else
    {
      *curpage = 1;
      RefreshCL();
    }
    
    if(*curpage == 1 && *itemFromDisp != *totalItems)
    {
      //если курсор убежал далеко за предел страницы, то попробуем вернуть его
      if(*curpos > *itemFromDisp)
      {
        while(*curpos > *itemFromDisp && *curpos > 0)
        {
          *curpos -= *itemFromDisp;
          RefreshCL();
        }
      }
    }
  }
  else // в один столбец
  {
    //если одна страница и курсор как то оказался на второй странице то ищем startItem,
    if( ((*totalItems - *itemFromDisp) <= 0) && (*curpos > *itemFromDisp))
    {
      unsigned int start = (sdiv(2,*itemFromDisp)+1) > 5 ? (sdiv(2,*itemFromDisp)+1) : (5);
      
      if(*curpos > start && *curpos < *totalItems - *itemFromDisp)
        *startItems = *curpos - start;
      else
        if(*curpos > *totalItems - *itemFromDisp)
           *startItems = *totalItems - *itemFromDisp;
    }
  }
  RefreshCL();
}



/*
*  Функция: MoveUpOneColumn
*  Описание: Перемещение курсора вверх, в режиме одного столбика
*  Параметры: int i - переход на указанное число позиций
*  Возвращаемое значение: Ничего не возвращает
*/
void MoveUpOneCollumn(int i)
{
  unsigned int *itemFromDisp = &clistm->itemFromDisp;
  unsigned int *totalItems   = &clistm->totalItems;
  unsigned int *startItem    = &clistm->startItem;
           int *curpos       = &clistm->curpos;
  
  //стартовый элемент, чтобы список прокручивался по мере передвижения
  int start = (sdiv(2,*itemFromDisp)+1) > 5 ? (sdiv(2,*itemFromDisp)+1) : (5);

  if( (*curpos > *itemFromDisp + 2) && (*totalItems!=*itemFromDisp) )//если дальше чем контактов на экране
    {
      *curpos -= i; 
      if(*startItem > 0 && *curpos > start && (*curpos < *totalItems - start))   *startItem -= i;//переменную смещения тоже вычитаем
    }
  else
    if(*curpos > 0)//первый экран с контактами
    {
      *curpos -= i;
      if(*startItem > 0 && *curpos > start && *totalItems != *itemFromDisp)  *startItem -= i;//переменная смещения отнимаем до стартового значения смещения
      else if(*curpos < start) *startItem = 0; //дошли до предела, обнуляемся
    }
/*  else
  {
    *curpos = *totalItems-1;//переход на последний контакт
    if((*totalItems - *itemFromDisp) <= 0) *startItem=0; //если одна страница
    else  *startItem = *totalItems - *itemFromDisp;
  }*/
  
  //может за грань дозволенного выскачили?  
  if(*curpos < 0)
  {
    //тогда идем на последний контакт
    *curpos = *totalItems-1;
    if((*totalItems - *itemFromDisp) <= 0) *startItem = 0; //если одна страница, то смещение не нужно
    else  *startItem = *totalItems - *itemFromDisp;
  }
}


/*
*  Функция: MoveDownOneColumn
*  Описание: Перемещение курсора вниз, в режиме одного столбика
*  Параметры: int i - переход на указанное число позиций
*  Возвращаемое значение: Ничего не возвращает
*/
void MoveDownOneCollumn(int i)
{
           int *curpos       = &clistm->curpos;
  unsigned int *itemFromDisp = &clistm->itemFromDisp;
  unsigned int *totalItems   = &clistm->totalItems;
  unsigned int *startItem    = &clistm->startItem;
  
  int start = (sdiv(2,*itemFromDisp)+1) > 5 ? (sdiv(2,*itemFromDisp)+1) : (5);//*itemFromDisp/2+1;
  //if(start==0) *startItem=0;
  
  if( *curpos < start && *totalItems!=*itemFromDisp )
  {
    *curpos += i;
  }
  else
    if( *curpos < *totalItems - 1 )
    {
      if( *totalItems - *itemFromDisp > 0 )
      {
        if(*startItem < (*totalItems - *itemFromDisp)) *startItem+=i;
      }
      *curpos+=i; 
    }
  else   
    {
      *curpos=0;
      *startItem=0;
    }

  //может за грань дозволенного выскачили?  
  if(*curpos >= *totalItems){ *curpos = 0; *startItem = 0; }
}

/*
*  Функция: MoveUpTwoColumn
*  Описание: Перемещение курсора вверх в режиме два столбца
*  Параметры:  int i - перемещение на указанное кол-во позиций
*  Возвращаемое значение: Ничего не возвращает
*/
void MoveUpTwoCollumn(int i)
{
  if(!clistm->itemFromDisp)return;  

  clistm->curpos -= i;
  
  if(clistm->curpos < 0)
    {
      clistm->curpos = clistm->totalItems - 1;//на последний
    /*
      if(clistm->itemFromDisp == clistm->totalItems)//если кол-во контактов на экране равно общему количеству контактов
        clistm->curpage = 1;//не меняем страницу
      else
        clistm->curpage = sdiv(clistm->itemFromDisp,clistm->totalItems)+1;//иначе на последнюю страницу
      */
    }
/*  else
  {
//    if(clistm->curpos < (clistm->curpage-1)*(clistm->itemFromDisp)){ clistm->curpos+=1; clistm->curpage--; }
  }*/
}

/*
*  Функция: MoveDownTwoColumn
*  Описание: Перемещение курсора вниз в режиме два столбца
*  Параметры:  int i - перемещение на указанное кол-во позиций
*  Возвращаемое значение: Ничего не возвращает
*/
void MoveDownTwoCollumn(int i)
{
  if(!clistm->totalItems) return;
  clistm->curpos += i;
  

  //Если курсор вышел за пределы, то на первую позицию
  if(clistm->curpos >= clistm->totalItems) { clistm->curpos = 0; clistm->curpage=1; }
  
  //смотрим убежал курсор на след. экран или нет
  if(clistm->curpos >= clistm->curpage*(clistm->itemFromDisp + i)) { clistm->curpos -= i; clistm->curpage++; }
  //if(clistm->curpos >= clistm->curpage*(clistm->itemFromDisp+(i?1:0))) { clistm->curpos-=(i?1:0); clistm->curpage++; }
}


//------------------------------------------

/*
*  Функция: PrintMessage
*  Описание: Вывод всплывающего сообщения в контакт листе, текст находится в буфере prmsg
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void PrintMess()
{
  int Font = FONT_MEDIUM_BOLD;
  int len = GetWidth(prmsg,Font);
  int nlines = (len > (ScreenW()-10)) ? sdiv((ScreenW()-10),len) : (1);
  
  for(int i=0 ; i<strlen(prmsg) ; i++)
    if(prmsg[i]=='\n') nlines++; 
  
  nlines--;
  
  int y=0;
  int y2=0;
 
  y  = (ScreenH()/2) - (FontH(Font)*(sdiv(2,nlines) + 2));
  y2 = (ScreenH()/2) + (FontH(Font)*(sdiv(2,nlines) + ((nlines>1) ? 2 : 1)) );
  
  DrawRoundedFrame(0,y,ScreenW()-5,y2,0,0,0,clrWhite,"\x00\x00\x00\x40");
  DrawStr(prmsg,5,y+FontH(Font),ScreenW()-5,y2,TEXT_ALIGNMIDDLE,Font,clrWhite,0);
}


//------------------------------------------

/*
*  Функция: DrawContaclList
*  Описание: Функция отрисовки всего окна контакт листа
*  Параметры: Никаких
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawContactList()
{
#ifdef ICONBAR
  DrawRoundedFrame(0,/*HeaderY*/YDISP,ScreenW()-1,ScreenH()-1,0,0,0,CLBackgrClr,CLBackgrClr);
#else
  DrawRoundedFrame(0,0,ScreenW()-1,ScreenH()-1,0,0,0,CLBackgrClr,CLBackgrClr);
#endif  
  DrawImg(0,YDISP,S_ICONS[ICON_BG]);
  DrawHeader();
  DrawList();
  if(strlen(prmsg)!=0 && PrintTimer > 0)  PrintMess();
  if(SOFT_MODE)  DrawSoftKeys(LG_MENU,clmenu_sk_r);
}


//------------------------------------------

/*
*  Функция: ZeroOnline
*  Описание: Обнуляем контакты, чтоб не мигали, если нет таковых, то отчищаем лог сообщений
*  Параметры: CLIST *t
*  Возвращаемое значение: Ничего не возвращает
*/
void ZeroOnline(CLIST *t)
 {
  if(t)
  {
    if(t->isonline)//если контакт тока зашел,то обнуляем
    {
      CLIST *tt;
      tt=FindContactByUin(t->uin);
      tt->isonline=0;
      tt->online=0;
    }
   else
    if(t->log && !(t->isonline))//очистить лог
     {
       FreeLOGQ(&t->log);
       t->msg_count=0;
       t->isactive=0;
     }     
   
   if(t->uin==UIN)//если нажали на Loopback, то обнуляем все isonline
   {
     isonline=0;
     CLIST *tt;
     tt=(CLIST *)(&cltop);
     while(tt=(CLIST *)(tt->next))
     {
       if(tt->isonline){ tt->isonline=0; tt->online=0; }
     }
   }
  }
  if(strlen(ContactT9Key)) ClearContactT9Key();
  RecountMenu(NULL,0);      
};



/*
*  Функция: FindMessage
*  Описание: Переход на не отвеченное сообщение, если таковых нет, то переходим на первую позицию
*  Параметры: CLIST *t
*  Возвращаемое значение: 
*         0 - не выполнили ничего
*         -1 - перерисовка
*/
int FindMessage(CLIST *t)
{
  if(CountContacts()>0 && t)
   {
    //t=FindContactByN(clistm->curpos);  
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
    
   GotoFirstPosition();           
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
  return 0;
};


/*
*  Функция: ClOnKey
*  Описание: Функция обработки клавиш
*  Параметры: GUI_MSG *msg
*  Возвращаемое значение: результат выполения функции
*/
int ClOnKey(GUI_MSG *msg)
{
  int key=msg->gbsmsg->submess;
  CLIST *t;
      
  if (msg->gbsmsg->msg==LONG_PRESS)
  {           
    int key=msg->gbsmsg->submess;
    if (((key>='0')&&(key<='9')) || key == '*' || key == '#' )
    {
      if(LongPressTimer==0)
      {
        LongPressAction(FindContactByN(clistm->curpos),msg->gbsmsg->submess);      
      }
      ClearContactT9Key();
      RecountMenu(NULL,1);
      return 0;
    }
    
    switch(msg->gbsmsg->submess)
    {
      case UP_BUTTON:    if(VIEW_MODE) MoveUpTwoCollumn(2);   else MoveUpOneCollumn(1);   break;
      case DOWN_BUTTON:  if(VIEW_MODE) MoveDownTwoCollumn(2); else MoveDownOneCollumn(1); break;
      case RIGHT_BUTTON: if(VIEW_MODE) MoveDownTwoCollumn(1); else MoveDownOneCollumn(2); break;
      case LEFT_BUTTON:  if(VIEW_MODE) MoveUpTwoCollumn(1);   else MoveUpOneCollumn(2);   break;
      case VOL_UP_BUTTON:
        {
         t=FindContactByN(clistm->curpos);    
         //Послать запрос текста XStatus
          if (t)
          {
            if (!t->isgroup)
            {
              FreeXText(t);
              t->req_xtext=0;
              RequestXText(t->uin);
            }
          }
        }
       break;
    }
  };
  
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
     case UP_BUTTON:    if(VIEW_MODE) MoveUpTwoCollumn(2);   else MoveUpOneCollumn(1);   break;
     case DOWN_BUTTON:  if(VIEW_MODE) MoveDownTwoCollumn(2); else MoveDownOneCollumn(1); break;
     case RIGHT_BUTTON: if(VIEW_MODE) MoveDownTwoCollumn(1); else MoveDownOneCollumn(2); break;
     case LEFT_BUTTON:  if(VIEW_MODE) MoveUpTwoCollumn(1);   else MoveUpOneCollumn(2);   break;
     case LEFT_SOFT: 
      {
        extern void CreateMainMenu();
        if(!ClrPrintMess())//если нет всплывающего окна,если есть то закрываем его
        {
          if(!strlen(ContactT9Key))
           CreateMainMenu();//открываем меню
          else            
            if(strlen(ContactT9Key)==1)
            { 
              LongPressAction(FindContactByN(clistm->curpos),ContactT9Key[0]);
              ClearContactT9Key();
              RecountMenu(NULL,1);
            }
          else
          {
            ClearContactT9Key();
            RecountMenu(NULL,1);
          }
        }
      }break;
    case RIGHT_SOFT:
    case RED_BUTTON:
      {
        if(msg->gbsmsg->submess==RED_BUTTON && (isonline || on_off_timer > 0))
        {
          //обнуляем все isonline
          if(isonline)
          {
            isonline=0;
            CLIST *tt;
            tt=(CLIST *)(&cltop);
            while(tt=(CLIST *)(tt->next))
            {
              if(tt->isonline){ tt->isonline=0; tt->online=0; }
            }
          }
          else
          {
            //убираем скролл в хедере
            ClrOnOffBuffer();
            DisableScroll();
          }
          break;
        }

        if(!ClrPrintMess())
         {
           if(strlen(ContactT9Key))
            {
              BackSpaceContactT9();
              RecountMenu(NULL,1);
              return(-1);
            }
           else
            {
             ShowLoadScreen=1;//return 1;
            }
         }
      }
      break;
    case ENTER_BUTTON:
     {         
      t=FindContactByN(clistm->curpos);  
      if(!ClrPrintMess())
      {
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
      }
      return(-1);
      }
      case '*':
        t=FindContactByN(clistm->curpos);  
        extern const int Action_by_Star;
        return LongPressActionOnChat(t,Action_by_Star);
//        FimdMessage(t);
      //break;
    case GREEN_BUTTON:
      {
          t=FindContactByN(clistm->curpos);    
         //Послать запрос текста XStatus
          if (t)
          {
            if (!t->isgroup)
            {
              char s[256];
              char xt[256];
              zeromem(xt, sizeof(xt));
              zeromem(s,sizeof(s));

              t = FindContactByN(clistm->curpos);    
              if(t->xtext == 0)
                strcpy(xt,"Нет текста хстатуса");
              else
              {
                int i = t->xtext[0];                
                int len = strlen(t->xtext);
                if( (len - i - 1) > 0)//если есть текст кроме заголовка
                {
                  strncpy(xt,t->xtext+1,i);
                  sprintf(xt+i, "\n%s\0",t->xtext+i+1);
                }
                else
                  sprintf(xt, "%s\0",t->xtext+1);
              }
              snprintf(s,255,"[   %s   ]\n%s\n%s\0", t->name,
                                                     t->sttime == 0 ? "" : t->sttime, 
                                                     t->xstate == 0 ? "Хстатус не установлен" : xt);
              s[strlen(s)] = 0;
              PrintMessage(s,10);
            }
          }
      }
      break;
      
    case VOL_UP_BUTTON:
      { 
        if(!isVolUpButton) break;
      }break;      
    }
    
    
    if (key=='#' && (CountContacts()>0) && ActionResh)//Если есть хоть один контакт то выполнять,иначе возможен пикофф
    {
      t=FindContactByN(clistm->curpos);     
      if(ActionResh == 1) ZeroOnline(t);
      if(ActionResh == 2) return FindMessage(t);
    }   
  }  
 
  if ((key>='0')&&(key<='9') && msg->gbsmsg->msg==KEY_DOWN)
    {
      GotoFirstPosition();
      AddContactT9Key(key);
      RecountMenu(NULL,1);
      return(-1);
    }
  return 0;  
}

#ifndef ICONBAR
  #ifdef ELKA  
    #undef YDISP
    #define   YDISP 24
  #endif
#endif

#endif //#ifdef GUI_INTERFACE


/************************************************************************************************/

//extern int xstatuses_max;
/*
*  Функция: DrawLoadScreen
*  Описание: Экран загрузки натаськи
*  Параметры: MAIN_GUI *data
*  Возвращаемое значение: Ничего не возвращает
*/
void DrawLoadScreen(MAIN_GUI *data)
{
  int scr_w = ScreenW();
  int scr_h = ScreenH();
//  int pos_status;
//  int pm = pictures_max , pl = pictures_loaded;
/*
#ifdef GUI_INTERFACE
  pm = pm + TOTAL_ICONS;
#endif
*/  
/*  DrawRoundedFrame(0,YDISP,scr_w-1,scr_h-1,0,0,0,
		   GetPaletteAdrByColorIndex(0),
		   GetPaletteAdrByColorIndex(20));*/

  DrawRectangle(0,YDISP,scr_w-1,scr_h-1,0,
		   GetPaletteAdrByColorIndex(1),
		   GetPaletteAdrByColorIndex(1));
  DrawImg(0,0,S_ICONS[ICON_LOGO]);
//  DrwImage(GetIconByItem(ICON_LOGO),0,0);
  unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;			//by BoBa 10.07
  wsprintf(data->ws1,LG_GRSTATESTRING,connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);

/*  if(pm != pl)
  {
    DrawRectangle(0,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD),scr_w-1,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-2,0,
                     GetPaletteAdrByColorIndex(0),
                     GetPaletteAdrByColorIndex(0));
    pos_status = ((scr_w-1) * pl) / pm;
    DrawRectangle(1,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD)+1,pos_status,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-3,0,
                     GetPaletteAdrByColorIndex(14),
                     GetPaletteAdrByColorIndex(14));  
//   wstrcatprintf(data->ws1,"\nLoading images...");
  }*/
   /*
  if(pictures_loaded < TOTAL_ICONS)
    {
      wstrcatprintf(data->ws1,"\nLoading %d/%d images...",pictures_loaded,TOTAL_ICONS);
    }
  else        */
 /* if(xstatuses_load)
    {
      wstrcatprintf(data->ws1,"\nLoaded %d/%d xstatus",total_xstatuses, xstatuses_max-1);
    }
  else*/
    /*if(total_smiles)
    {
      wstrcatprintf(data->ws1,"\nLoaded %d/%d smiles",total_smiles,pm-xstatuses_max);//-TOTAL_ICONS
    }*/
  DrawString(data->ws1,3,3+YDISP,scr_w-4,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     FONT_SMALL,TEXT_OUTLINE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
 
  
#ifdef ELKA
  int y=160;
  char A[]="[A: %s]";
  char I[]="[I]\n";
  char V[]="[V]\n";
  char S[]="[S]\n";

  wsprintf(data->ws2,empty_str);
  if(CurrentPrivateStatus==1)  wstrcatprintf(data->ws2,I);
  if(Is_Vibra_Enabled)         wstrcatprintf(data->ws2,V); 
  if(Is_Sounds_Enabled)        wstrcatprintf(data->ws2,S);
  if(Is_Double_Vibra_On)       wstrcatprintf(data->ws2,"[Vr]\n");  
  if(away_m)                   wsprintf(data->ws2,A,away_msg_time_dace);
 
  DrawString(data->ws2,2,y,ScreenW()-1,y+GetFontYSIZE(FONT_SMALL_ITALIC_BOLD)*5,FONT_SMALL_ITALIC_BOLD,TEXT_OUTLINE,GetPaletteAdrByColorIndex(2),GetPaletteAdrByColorIndex(1));
#endif  

  wsprintf(data->ws2,"Uin: %d\nRecv: %d Send: %d Blocked: %d",UIN, CountMessage[0],CountMessage[1], blocked_spam);
  DrawString(data->ws2,1,scr_h-10-5*GetFontYSIZE(FONT_SMALL_BOLD)+1,ScreenW()-1,scr_h-10-GetFontYSIZE(FONT_MEDIUM_BOLD)-3
             ,FONT_SMALL,TEXT_OUTLINE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));

  
  if(connect_state==3)
  {
    if(Auto_Status)
    {
      int m = (msg_away_timer*5)/60;
      int s = (msg_away_timer*5)%60;
      int h = m/60;  
      wsprintf(data->ws2,"Online: %02d:%02d:%02d   not action %02d:%02d:%02d", uptime.hour, uptime.min, uptime.sec,h,m%60,s);
    }
    else
      wsprintf(data->ws2,"Online: %02d:%02d:%02d",uptime.hour, uptime.min, uptime.sec);
  }
  else
    wsprintf(data->ws2,"Offline [%02d:%02d:%02d]",uptime.hour, uptime.min, uptime.sec);

  DrawString(data->ws2,1,scr_h-10- 3*GetFontYSIZE(FONT_SMALL_BOLD)+1,ScreenW()-1,scr_h - 10 - GetFontYSIZE(FONT_MEDIUM_BOLD)-3
             ,FONT_SMALL,TEXT_OUTLINE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
 
  
  wsprintf(data->ws2,percent_t,cltop ? lgpData[LGP_GrsKeyClist] : empty_str);
  DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     scr_w-4,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNRIGHT+TEXT_OUTLINE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
  wsprintf(data->ws2,percent_t, lgpData[LGP_GrsKeyExit] );
  DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     scr_w>>1,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT+TEXT_OUTLINE,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
}


void method0(MAIN_GUI *data)
{
#ifdef GUI_INTERFACE
  if(edchat_id) return;//если открыт чат
  if(IsGuiOnTop(maingui_id))
  {
    if(ShowLoadScreen)    
    {
 /* #ifdef ELKA
      DisableIconBar(0);
  #endif*/
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
#ifdef GUI_INTERFACE
//  extern void LoadImages();
//  LoadImages();
  scr_w = ScreenW()-1;
  scr_h = ScreenH()-1;
  clistm = malloc(sizeof(GUIMENU));
  
  clistm->font = ClFont;
  clistm->FontH = FontH(ClFont);
  
  menu_sk[0].lgp_id=(int)lgpData[LGP_Select];
  menu_sk[1].lgp_id=(int)lgpData[LGP_Close];  
 
  //char s[128];
  //sprintf(s,"%s\\bg.png",ICON_PATH);
  //BGpic = get_file_size(s);    
  myack = malloc(sizeof(ACKED));
  clistm->totalItems = 0;//CountContacts(); //общее кол-во контактов
  clistm->startItem = 0;
  clistm->curpos = 0;
  //InitCLSet();  
#endif
  data->gui.state=1;
}

void method2(MAIN_GUI *data,void (*mfree_adr)(void *))
{

  FreeWS(data->ws1);
  FreeWS(data->ws2);
#ifdef GUI_INTERFACE
  if(clistm) mfree(clistm);
  clistm = NULL;
  if(myack) mfree(myack);
  myack = NULL;
  #ifdef ELKA
    if(strlen(SoftShot.bitmap))   mfree(SoftShot.bitmap);
    if(strlen(HeaderShot.bitmap)) mfree(HeaderShot.bitmap);
  #endif
#endif
  data->gui.state=0;
}

void method3(MAIN_GUI *data,void *(*malloc_adr)(int),void (*mfree_adr)(void *))
{
 // InitCLSet();
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
  DirectRedrawGUI();
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
    case LEFT_SOFT:
#ifdef GUI_INTERFACE
      if(ShowLoadScreen) return(1);//Происходит вызов GeneralFunc для тек. GUI -> закрытие GUI
      else  ShowLoadScreen=1;
#else
      return 1;
#endif
    case RIGHT_SOFT:
      //      if (cltop) remake_clmenu();
#ifdef GUI_INTERFACE
       if(ShowLoadScreen && (cltop || sock==-1)) ShowLoadScreen=0;//показывть контакт лист
#else
       if (cltop) create_contactlist_menu();
#endif
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
    case '0':
      disautorecconect=0;
      SUBPROC((void*)end_socket);
      GBS_DelTimer(&reconnect_tmr);
      DNR_TRIES=3;
      SUBPROC((void *)create_connect);
      break;
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
         {
           SetPrivateStatus(1);//Инвиз
         }
         else
         {
           SetPrivateStatus(3);
         }
         break;
    case '4':
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        break;
    case RED_BUTTON:
    //   extern volatile int disautorecconect;
       disautorecconect=1;
       extern void end_socket(void);
       SUBPROC((void*)end_socket);
       ShowMSG(1,(int)"Disconnect!");
       break;
    }
  }
  return(0);
}


int method5(MAIN_GUI *data,GUI_MSG *msg)
{
  DirectRedrawGUI();
#ifdef GUI_INTERFACE
  if(ShowLoadScreen)
   return LoadOnKey(msg);
  else
   ClOnKey(msg);  
    return(0);    
#else
   return LoadOnKey(msg);
#endif
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

GBSTMR OneSecTmr;
void OneSecondTimer()
{
  tmr_gipc.name_to=ipc_my_name;
  tmr_gipc.name_from=ipc_my_name;
  tmr_gipc.data=NULL;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_ONESECOND,&tmr_gipc);  
  GBS_StartTimerProc(&OneSecTmr,TMR_SECOND,OneSecondTimer);
}


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
  
  ews=AllocWS(16384);
  wsBuffer=AllocWS(4096);
  //  MutexCreate(&contactlist_mtx);
//#ifndef START_OFFLINE
  DNR_TRIES = 3;
//#endif
  //  SUBPROC((void *)InitSmiles);
  //  SUBPROC((void *)create_connect);
  CountMessage[0] = 0;
  CountMessage[1] = 0;
 
  FreeAllList();

#ifdef GUI_INTERFACE
  GBS_StartTimerProc(&OneSecTmr,TMR_SECOND*10,OneSecondTimer);
#endif
  GBS_StartTimerProc(&tmr_active,TMR_SECOND*10,process_active_timer);
  sprintf((char *)ipc_my_name+6,percent_d,UIN);
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
}

void maincsm_onclose(CSM_RAM *csm)
{
  WriteDefSettings();
  do_StatusChangeWrite(NULL, NULL, 1);  
  GBS_DelTimer(&online_tmr);  
  //  GBS_DelTimer(&tmr_dorecv);
  
  GBS_DelTimer(&OneSecTmr);
  GBS_DelTimer(&tmr_active);
  GBS_DelTimer(&tmr_vibra);
  GBS_DelTimer(&reconnect_tmr);
  GBS_DelTimer(&tmr_illumination);

  
#ifdef ELKA
  GBS_DelTimer(&sli_tmr);
#endif
  GBS_DelTimer(&remind_tmr);
  GBS_DelTimer(&popup[POPUP_ON].tmr);
  GBS_DelTimer(&popup[POPUP_OFF].tmr);
  GBS_DelTimer(&popup[POPUP_MSG].tmr);
  GBS_DelTimer(&away_tmr);
  
  FreeAllList();   
  FreeWS(wsBuffer);

  SetVibration(0);
  FreeTemplates();
  FreeCLIST();
  free_ICONS();
  //  FreeSmiles();
  FreeWS(ews);
  FreeXStatusText();
  lgpFreeLangPack();
  //  MutexDestroy(&contactlist_mtx);
/*  SUBPROC((void *)FreeSmiles);// вызывается в FreeAllList()
  SUBPROC((void *)FreeXStatusesImg);*/
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
  {   
    LoadingImages();//инициализируем все картинки 
#ifndef START_OFFLINE
    create_connect();
#else
    LoadLocalCL();
#endif
  }
}

int maincsm_onmessage(CSM_RAM *data,GBS_MSG *msg)
{
  extern const int VIBR_ON_CONNECT;
  
#ifdef UP_BY_UNLOCK
  if(DEVELOP_IF == 3) //popup naticq by unlock 6.11.2009
  {
    if(!IsUnlocked()) develop_by_unlock = 1;
    if(IsUnlocked() && develop_by_unlock == 1)
    {
      develop_by_unlock = 0;
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)) to_develop();   
    }  
  }
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
	    if (ipc->name_from==ipc_my_name)
            {
                SUBPROC((void *)CheckDoubleRun);
            }
            else ipc->data=(void *)maincsm_id;
	    break;
	 /* case IPC_SMILE_PROCESSED:
	    //Только собственные смайлы ;)
	    if (ipc->name_from==ipc_my_name)
            {
              SUBPROC((void *)ProcessNextSmile);
            }
        #ifdef GUI_INTERFACE
	    if(ShowLoadScreen)
        #endif
              SMART_REDRAW();
	    break;
	  case IPC_XSTATUSIMG_PROCESSED:
	    //Только собственные иксстатусы ;)
	    if (ipc->name_from==ipc_my_name)
            {
               SUBPROC((void *)ProcessNextXStatImg);
            }
        #ifdef GUI_INTERFACE
	    if(ShowLoadScreen)
        #endif
              SMART_REDRAW();
	    break;*/
#ifdef GUI_INTERFACE
        /* case IPC_IMG_PROCESSED:
	    //Только собственные статусы ;)
	    if (ipc->name_from==ipc_my_name)
            {
              SUBPROC((void *)ProcessNextImg);
            }
	    if(ShowLoadScreen) SMART_REDRAW();
	    break;*/
          case IPC_ONESECOND:
            if (ipc->name_from==ipc_my_name)
            {
            #ifdef ELKA
               SLI_check();
            #endif
               UpTime();
               if(ShowLoadScreen && !edchat_id) DoRefreshCL = 1; //SMART_REDRAW();
             //  if(ShowLoadScreen || !IsGuiOnTop(maingui_id)) break;

               if((--on_off_timer) > 0)
                {
                   if(!edchat_id) DoRefreshCL = 1;//SMART_REDRAW();
                   //break;
                }
                else
                  if(on_off_timer != -1)//Надо обновить экран когда время прошло
                  {
                    on_off_timer = -1;
                    isonline = 0;
                    zeromem(on_off_buf,128);
                    DisableScroll();
                    if(!edchat_id) DoRefreshCL = 1;// SMART_REDRAW();
                  }
                
                if(LongPressTimer>0) LongPressTimer--;
               
                if((PrintTimer--) > 0 && !edchat_id) DoRefreshCL = 1;
                //   SMART_REDRAW();                                    
               
               if(!DoRefreshCL) break;//Если не нада обновлять

               SMART_REDRAW();
               DoRefreshCL=0;
            }
            break;
#endif
	  case IPC_TENSECONDS:
	    //Только свое сообщение
	    if (ipc->name_from==ipc_my_name)
	    {
	      CLIST *t=(CLIST *)cltop;
	      int f=0;
	      while(t)
	      {
         #ifdef GUI_INTERFACE
               if(t->isonline)
                {
                  if (!(--(t->isonline))){ f=1; t->online=0; }
                  SMART_REDRAW();
                }
         #endif

                if (t->isactive)
		{
		  if (!(--(t->isactive))) //Если дошли до 0 хотя бы один раз - надо перерисовать меню
                   {
                     f=1;
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
    #ifdef GUI_INTERFACE
                if(!ShowLoadScreen)
     		  oldt=FindContactByN(clistm->curpos);
    #else
		if (contactlist_menu_id)
		{
		  oldt=FindContactByN(GetCurMenuItem(FindGUIbyId(contactlist_menu_id,NULL)));
		}
    #endif
		RecountMenu(oldt, 1);
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
     	  case IPC_SENDMSG:                                   //IPC_SENDMSG by BoBa 26.06.07
	    {
	      int l=strlen(((IPCMsg *)(ipc->data))->msg);
	      TPKT *msg=malloc(sizeof(PKT)+l);
	      msg->pkt.uin=((IPCMsg *)(ipc->data))->uin;
	      msg->pkt.type=T_SENDMSG;
	      msg->pkt.data_len=l;
	      memcpy(msg->data,((IPCMsg *)(ipc->data))->msg,l);
	      //slientsend=1;
	      SENDMSGCOUNT++; //Номер сообщения
	      SUBPROC((void *)SendAnswer,0,msg);
	    }
            break;
	  case IPC_FREEMSG:
	    ;
	    IPCMsg_RECVMSG *fmp=((IPCMsg_RECVMSG *)(ipc->data));
	    if (ipc->name_to!=ipc_my_name) break;
	    if (!fmp->drop_msg)
	    {
	      //Добавляем сообщение в чат
	      int l=strlen(fmp->msg);
	      TPKT *msg=malloc(sizeof(PKT)+l+1);
	      msg->pkt.uin=fmp->uin;
	      msg->pkt.type=T_RECVMSG;
	      msg->pkt.data_len=l;
	      memcpy(msg->data,fmp->msg,l+1);
               ae(msg);
	      //ProcessPacket(msg);
	    }
	    mfree(fmp->msg); //Освобождаем сам текст сообщения
	    mfree(fmp->ipc); //Освобождаем родительский IPC_REQ
	    mfree(fmp); //Освобождаем собственно IPCMsg_RECVMSG
	    mfree(ipc); //Освободили текущий IPC_REQ
	    return 0; //вышли нах
	  }
	}
      }
    }
    //Нарисуем иконочку моего статуса
#define idlegui_id (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
    CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
    if (IsGuiOnTop(idlegui_id)/*&&IsUnlocked()*/) //Если IdleGui на самом верху
    {
      extern const int ICON_IDLE_ENABLE;
      if(ICON_IDLE_ENABLE)
      {
        GUI *igui=GetTopGUI();
        if (igui) //И он существует
        {
          void *canvasdata=BuildCanvas();
          int icn;
          if (total_unread)
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
          //Тут трохи поменял
          // by Rainmaker: Рисуем канву только для иконки и выводим в своих координатах
          DrawCanvas(canvasdata,IDLEICON_X,IDLEICON_Y,IDLEICON_X+GetImgWidth((int)S_ICONS[icn])-1,
                     IDLEICON_Y+GetImgHeight((int)S_ICONS[icn])-1,1);
          DrawImg(IDLEICON_X,IDLEICON_Y,S_ICONS[icn]);
        }
      }
    }
  }
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"NatICQ config updated!");
      InitConfig();
      
      void UpdateCSMname(void);
      UpdateCSMname();//обновим имя еси сменили уин и сделали рекконект...

      sprintf((char *)ipc_my_name+6,percent_d,UIN);
#ifdef GUI_INTERFACE
      clistm->font = ClFont;
      clistm->FontH = FontH(ClFont);
#endif
      free_ICONS();
      setup_ICONS();
      ResortCL();
      RecountMenu(NULL, 1);
      //      InitSmiles();
      
#ifdef GUI_INTERFACE
      shot_header=0;
      shot_soft=0;
#endif
    }
  }
  
  if (msg->msg==MSG_GUI_DESTROYED)
  {
    if ((int)msg->data0==csm->gui_id)
    {
      csm->csm.state=-3;
    }
#ifndef GUI_INTERFACE
    if ((int)msg->data0==contactlist_menu_id)
    {
      contactlist_menu_id=0;
      prev_clmenu_itemcount=0;
    }
#endif
    if ((int)msg->data0==edchat_id)
    {
      edchat_id=0;
      SMART_REDRAW();
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
      //strcpy(logmsg,LG_GRGPRSUP);
      snprintf(logmsg, 255, LG_GRGPRSUP, RECONNECT_TIME);
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
	TPKT *p=(TPKT *)msg->data0;
	//Принят пакет
	if (p->pkt.type==T_RECVMSG)
	{
	  //Просунем через IPC

	  int l=p->pkt.data_len+1; //С завершающим \0
	  IPC_REQ *ripc=malloc(sizeof(IPC_REQ));
	  IPC_REQ *fipc=malloc(sizeof(IPC_REQ));
	  IPCMsg_RECVMSG *msg=malloc(sizeof(IPCMsg_RECVMSG));
	  memcpy(msg->msg=malloc(l),p->data,l);
	  msg->uin=p->pkt.uin;
	  msg->drop_msg=0;
	  msg->ipc=ripc;
	  ripc->data=msg;
	  fipc->data=msg;
	  ripc->name_from=ipc_my_name;
	  ripc->name_to=ipc_my_name;
	  fipc->name_from=ipc_my_name;
	  fipc->name_to=ipc_my_name;
	  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_RECVMSG,ripc);
	  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_FREEMSG,fipc);
	  mfree(p);
	}
	else
	{
	  //Непосредственная обработка
          // ae(p);
	  ProcessPacket(p);
	}
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
	   /* int i=strlen(PASS);
	    TPKT *p=malloc(sizeof(PKT)+i);
	    p->pkt.uin=UIN;
	    p->pkt.type=T_REQLOGIN;
	    p->pkt.data_len=i;
	    memcpy(p->data,PASS,i);
	    SUBPROC((void *)send_login,0,p);*/
	  }
	  GROUP_CACHE=0;
	  SENDMSGCOUNT=0; //Начинаем отсчет
	  if (!FindGroupByID(0)) AddGroup(0,lgpData[LGP_GroupNotInList]);
          extern const int IS_SHOW_LOOPBACK;
	  if (!FindContactByUin(UIN) && IS_SHOW_LOOPBACK) AddContact(UIN, lgpData[LGP_ClLoopback], 0,1);
	  SUBPROC((void *)LoadLocalCL);
	  SMART_REDRAW();
	}
	else
	{
	  ShowMSG(1,(int)lgpData[LGP_MsgIllegmsgcon]);
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
	  ShowMSG(1,(int)lgpData[LGP_MsgIllegmsgrea]);
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
	RecountMenu(NULL, 1);
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
	}
	SMART_REDRAW();
	SUBPROC((void *)ClearSendQ);
	if (!disautorecconect)
        {
          GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
          snprintf(logmsg,255,LG_GRRECONNECT,logmsg, RECONNECT_TIME);
        }
	break;
      }
    }
  }
  return(1);
}


const int minus11=-11;

unsigned short maincsm_name_body[140];


//-------------------- ICON IN ICONBAR --------------------//


//kluchnik 26.03.08
#ifdef NEWSGOLD
#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);

int iconmsgblink=1;
extern const unsigned int ShowStatusToIconBar;
extern const unsigned int ADDITION_UIN;

void addIconBar(short* num)
{
  if(!ShowStatusToIconBar) return;
  unsigned int NumStatusToIconBar = 8000;
  unsigned int NumXStatusToIconBar = 8100;  
  
  int icn;
  {
    if(total_unread)
    {
      //еси нет хстатуса то тада кол-во сообщений поочередно мигаем с статусосм

      /*if(CurrentStatus == 0 && (total_unread_all && total_unread_all<10))
      {
          icn = (iconmsgblink==1) ? (NumStatusToIconBar + IS_MSG) : (8200 + total_unread_all);
      }
      else
      {
          icn = NumStatusToIconBar + (iconmsgblink==1 ? (IS_MSG) : (7777));
  //      AddIconToIconBar(NumStatusToIconBar +(iconmsgblink==1?IS_MSG:7777), num);  
      }*/
//       AddIconToIconBar(icn, num);        

      AddIconToIconBar(NumStatusToIconBar +(iconmsgblink==1?IS_MSG:7777), num);  
//       iconmsgblink=!iconmsgblink;
      
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
        icn=IS_UNKNOWN;
        break;
      }
      AddIconToIconBar(NumStatusToIconBar + icn, num);
    }
  }
 
  if(CurrentXStatus!=0)//еси есть хстатус то делаем так
  {
    if(total_unread_all && total_unread_all<10)
      AddIconToIconBar(8200 + total_unread_all, num);  
    else
      AddIconToIconBar(NumXStatusToIconBar+CurrentXStatus,num);
   }
  
  if(ADDITION_UIN!=0)
   {
    CLIST *t;
    t=FindContactByUin(ADDITION_UIN);
    if(t)
    {
      if(t->state!=0xFFFF && (GetIconIndex(t)!=IS_UNKNOWN))
      {
        AddIconToIconBar(NumStatusToIconBar + GetIconIndex(t), num); 
        if(t->xstate!=0)
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

//const int minus11=-11;

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
  extern const char *successed_config_filename;
  WSHDR *ws;
  
  InitConfig();
  
  s=strrchr(filename,'\\');
  len=(s-filename)+1;
  strncpy(elf_path,filename,len);
  elf_path[len]=0;
  
  lgpLoadLangpack();
  
  if (!UIN)
  {
    LockSched();
    ShowMSG(1,(int)lgpData[LGP_MsgNoUinPass]);
    ws=AllocWS(150);
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws,0,0);
    UnlockSched();
    lgpFreeLangPack();
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
#ifdef NEWSGOLD  
  SetIconBarHandler();
#endif
  away_timer();
  Reminder_timer();
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
    if(IsBufferMenu)
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
        ascii2ws(item->ws,"Очистить буфер");
        break;
      default:
        i-=3;
        if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
        break;
      }
    }
    else
    {
      switch(i)
      {
      case 0:
        ascii2ws(item->ws,lgpData[LGP_MnuEdNextAct]);
        break;
      case 1:
        ascii2ws(item->ws,lgpData[LGP_MnuEdPrevAct]);
        break;
      default:
        i-=2;
        if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
        break;
      }
    }
  }
  if (item->type==1)
  {
    if(IsBufferMenu)
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
         CutWSTR(wsBuffer,0);
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
      default:
        i-=2;
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


int edchat_onkey(GUI *data, GUI_MSG *msg)
{
  //-1 - do redraw
  GBS_DelTimer(&tmr_illumination);
  CLIST *t;
  TPKT *p;
  EDITCONTROL ec;
  int len, f;
  unsigned err;
  char *s;
  int l=msg->gbsmsg->submess;
  EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
//  WSHDR *ews;
  char fn[256];

  if (msg->keys==0xFFF)
  {
    void ec_menu(EDCHAT_STRUCT *);
    ec_menu(ed_struct);
    return(-1);
  }
  if (msg->keys==0xFF0)  return (1);
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
    
    if (l==VOL_DOWN_BUTTON)
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
    
    if (l==VOL_UP_BUTTON)
    {
      WSHDR *ws=AllocWS(strlen(last_msg_buffer)+1);
      ascii2ws(ws,last_msg_buffer);
      EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
      EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
      FreeWS(ws);
      RefreshGUI();
    }
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
              CountMessage[1]++;
              t->send++;
              msg_away_timer=0;
              away_m=0;
              t->automsgcount=0;
              if(change_back_status)
              {
                CurrentStatus=status_rem;
                set_my_status();
                change_back_status=0;
              }
	      t->isactive=ACTIVE_TIME;
	      p=malloc(sizeof(PKT)+len+1);
	      p->pkt.uin=t->uin;
	      p->pkt.type=T_SENDMSG;
	      p->pkt.data_len=len;
	      strcpy(p->data,s);
              strcpy(last_msg_buffer, s);
	      AddStringToLog(t,0x01,p->data,I_str,(++SENDMSGCOUNT)&0x7FFF,1); //Номер сообщения
	      SUBPROC((void *)SendAnswer,0,p);
	      mfree(t->answer);
	      t->answer=NULL;
	      //        request_remake_edchat=1;
	      EDIT_SetFocus(data,ed_struct->ed_answer);
	      CutWSTR(ews,0);
	      EDIT_SetTextToFocused(data,ews);
	      AddMsgToChat(data);
	      RecountMenu(t, 1);
	      return(-1);
	    }
	  }
	}
      }
    }
    if (l==ENTER_BUTTON)
    {

      if (!EDIT_IsMarkModeActive(data))  // Только если не выделение
      {
        int pos, len;
        char *link;
        ExtractEditControl(ed_struct->ed_chatgui,EDIT_GetFocus(ed_struct->ed_chatgui),&ec);
        wstrcpy(ews,ec.pWS);
        pos = EDIT_GetCursorPos(data);
        len = wstrlen(ews);
        
        link = malloc(len+1);
                
        if(IsUrl(ews, pos, link))
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
          
//          LockSched();
//          ShowMSG(1,(int)link);
//          UnlockSched();
          mfree(link);
          return (-1);
        }
        else
        {
          int i=ed_struct->loaded_templates=LoadTemplates(ed_struct->ed_contact->uin);
//          EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+2);
          if(IsBufferMenu)
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+3);
          else
            EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+2);
          
          mfree(link);
          return (-1);
        }          
          
      }

    }
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
#ifndef	NEWSGOLD
      int icon, width;
      icon=*(S_ICONS+GetIconIndex(ed_struct->ed_contact));
      ((HEADER_DESC *)&edchat_hdr)->rc.x2=ScreenW()-1-(width=GetImgWidth(icon));
      DrawImg(ScreenW()-1-width,1,icon);
#else

  #ifdef NEW_HEADER_TO_CHAT
      CLIST *t = ed_struct->ed_contact;
      IMGHDR *status = GetIconByItem(GetIconIndex(t));
      IMGHDR *xstatus = GetXIconByItem(t->xstate);
      int pos_x=1;
      char *s=malloc(128);
      zeromem(s,sizeof(s));
      DrawRoundedFrame(0,YDISP,ScreenW()-40,YDISP+status->h*2+3,0,0,1,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(25));
      
      pos_x = DrwImage(status,pos_x,YDISP+1,RET_X) + 2;
      if(t->xstate!=0) pos_x += DrwImage(xstatus,pos_x,YDISP+1,RET_X) + 2;
      
      //void DrawStr(char *str, int x, int y, int x2,int y2, int align, int font, char *color,int needcut)
      char  *ptr;
      ptr = strchr(t->sttime,':');
            
//      sprintf(s,"%s, %s", t->name, (strlen(t->sttime) && ptr) ? (ptr+1) : (empty_str));
      sprintf(s,_s, t->name);
      if(strlen(t->sttime) && ptr)
      {
         strcat(s,", ");
         strcat(s,ptr+1); 
      }
      DrawStr(s,pos_x,YDISP+1,ScreenW()-42,YDISP+status->h,0,FONT_SMALL,"\xFF\xFF\xFF\x64",1);
      //char *GetLastXTextLOGQ(CLIST *t)
      char *xtext = malloc(256);
      zeromem(xtext,sizeof(xtext));
      if(strlen(t->xtext) && t->xtext)
      {
         strncpy(xtext,t->xtext+1, t->xtext[0]);
         xtext[strlen(xtext)] = '\0';
      }
      else
        strcpy(xtext, "Нет хстатуса");
      DrawStr(xtext ,2,YDISP+status->h+2,ScreenW()-42,YDISP+status->h*2,0,FONT_SMALL,"\xFF\xFF\xFF\x64",1);
//      DrawLine(0,YDISP+status->h*2+3,ScreenW(),YDISP+status->h*2+3,0,"\xFF\x00\x00\x64");
      mfree(s);
  #else
      int icon, width;
//      extern const char XSTATUSES_PATH[];
      icon=*(S_ICONS+GetIconIndex(ed_struct->ed_contact));
      width=GetImgWidth(icon);
      DrawImg(2,((HeaderH()-width)>>1)+YDISP,icon);
/*      CLIST *t = ed_struct->ed_contact;
      char s[256];
      zeromem(s,sizeof(s));
      sprintf(s,"%s\\%d.png",XSTATUSES_PATH,t->xstate);
      DrawImg(4+width,YDISP,(int)s);*/
  
  /*      DrawRoundedFrame(ScreenW()-8,YDISP,ScreenW()-1,YDISP+7,0,0,0,
                         GetPaletteAdrByColorIndex(0),
                         GetPaletteAdrByColorIndex(EDIT_IsBusy(edchat_gui)?3:4));*/
  #endif
      
#endif
    }
  }
}

void edchat_ghook(GUI *data, int cmd)
{
  static SOFTKEY_DESC sk={0x0FFF,0x0000,NULL};
  sk.lgp_id=(int)lgpData[LGP_Menu];
  static SOFTKEY_DESC sk_cancel={0x0FF0,0x0000,NULL};
  sk_cancel.lgp_id=(int)lgpData[LGP_Close];
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
    RecountMenu(ed_struct->ed_contact, 1);
    mfree(ed_struct);
  }
  if (cmd==0x0A)
  {
    pltop->dyn_pltop=SmilesImgList;
    DisableIDLETMR();
    total_unread-=ed_struct->requested_decrement_total_unread;
    ed_struct->requested_decrement_total_unread=0;
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
	RecountMenu(ed_struct->ed_contact, 1);
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
  extern const int FIRST_LETTER;
  void *ma=malloc_adr();
  void *eq;
  EDITCONTROL ec;
  EDITC_OPTIONS ec_options;
  int color, font, type;
  int zc;

  LOGQ *lp;
  int edchat_toitem;

#ifdef NEW_HEADER_TO_CHAT  
  *((int *)(&edchat_hdr.lgp_id))=(int)"";
#else
  *((int *)(&edchat_hdr.lgp_id))=(int)t->name;//(int)s;
#endif
//  *((int **)(&edchat_hdr.icon))=(int *)S_ICONS+GetIconIndex(t);
  *((int **)(&edchat_hdr.icon))=(int *)S_ICONS+IS_NULLICON;

  eq=AllocEQueue(ma,mfree_adr());


  lp=t->log;

  while(lp)
  {
    font = ED_H_FONT_SIZE;
    if ((zc=lp->acked&3))
    {
      if (zc==1)
	color=ACK_COLOR; //Зеленый
      else
	color=I_COLOR;
    }
    else
    {
      if (lp->ID==0xFFFFFFFF)
      {
        type = lp->type&0x0F;
        if(lp->type&0x10)
        {
          color = (type==1)?O_I_COLOR:((type==3 || type==4)?O_X_COLOR:O_TO_COLOR);
          font = (type==3 || type==4)?O_ED_X_FONT_SIZE:O_ED_H_FONT_SIZE;
        }
        else
        {
          color = (type==1)?I_COLOR:((type==3 || type==4)?X_COLOR:TO_COLOR);
          font = (type==3 || type==4)?ED_X_FONT_SIZE:ED_H_FONT_SIZE;
        }
      }
      else
	color=UNACK_COLOR; //Серый
    }
    PrepareEditControl(&ec);
    if ((lp->type&0x0F)!=3 && (lp->type&0x0F)!=4)
    {
      ascii2ws(ews,lp->hdr);
      ConstructEditControl(&ec,ECT_HEADER,ECF_APPEND_EOL,ews,ews->wsbody[0]);
    }
    else
      ConstructEditControl(&ec,ECT_HEADER,ECF_DELSTR,ews,0);
    PrepareEditCOptions(&ec_options);
    SetPenColorToEditCOptions(&ec_options,color);
    SetFontToEditCOptions(&ec_options,font);
    CopyOptionsToEditControl(&ec,&ec_options);
    AddEditControlToEditQend(eq,&ec,ma);
    if ((lp->type&0x0F)!=3 && (lp->type&0x0F)!=4)
    {
      ParseAnswer(ews,lp->text);
    }
    else
    {
      ParseXStatusText(ews,lp->text,(lp->type&0x10)?O_X_COLOR:X_COLOR,t->xstate);
    }
    PrepareEditControl(&ec);
    ConstructEditControl(&ec,
                         ECT_NORMAL_TEXT,
                         ews->wsbody[0] ? ECF_APPEND_EOL|ECF_DISABLE_T9 : ECF_DELSTR,
                         ews,ews->wsbody[0]);
    PrepareEditCOptions(&ec_options);
//#ifdef M75
    if ((lp->type&0x0F)!=3 && (lp->type&0x0F)!=4)
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
  t->isunread=0;

  total_unread_all -= t->unread_msg;
  t->unread_msg=0;

  
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
  ConstructEditControl(&ec,3,(FIRST_LETTER)?ECF_DEFAULT_BIG_LETTER:0,ews,7168);

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

    int scr_w=ScreenW();
    int scr_h=ScreenH();
  //  int head_h=HeaderH();

  patch_header(&edchat_hdr);
  patch_input(&edchat_desc);
  //  edchat_desc.font=ED_FONT_SIZE;
  
  
  edchat_id=CreateInputTextDialog(&edchat_desc,&edchat_hdr,eq,1,ed_struct);
}

//-----------------------------------------------------------------------------

#define EC_MNU_MAX 10

int Focus_Quote = 0;//Флаг, определяет после цитирования перейти на поле ввода, или остаться на текущей позиции

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
  
  int ii=EDIT_GetFocus(ed_struct->ed_chatgui);
  
  EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
  EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
  
  if(!Focus_Quote) EDIT_SetFocus(ed_struct->ed_chatgui,ii);
  FreeWS(ws);
  GeneralFuncF1(1);
}

void QuoteFocus(GUI *data)
{
  Focus_Quote = 1;
  Quote(data);
}

void QuoteNotFocus(GUI *data)
{
  Focus_Quote = 0;
  Quote(data);
}

void GetShortInfo(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);

  TPKT *p;
  CLIST *t;
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    req_info_contact = 1;
    p=malloc(sizeof(PKT));
    p->pkt.uin=t->uin;
    p->pkt.type=T_REQINFOSHORT;
    p->pkt.data_len=0;
    AddStringToLog(t, 0x01, "Request info...", I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t, 1);
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
  //const char s[]=LG_AUTHREQ;
  //char s[]="";
  //strcpy(s,(char*)lgpData[LGP_AuthReq]);
  
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen((char*)lgpData[LGP_AuthReq]))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_AUTHREQ;
    p->pkt.data_len=l;
    strcpy(p->data,(char*)lgpData[LGP_AuthReq]);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t, 1);
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
  //const char s[]=LG_AUTHGRANT;
  //char s[]="";
  //strcpy(s,(char*)lgpData[LGP_AuthGrant]);
  
  if ((t=ed_struct->ed_contact)&&(connect_state==3))
  {
    p=malloc(sizeof(PKT)+(l=strlen((char*)lgpData[LGP_AuthGrant]))+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_AUTHGRANT;
    p->pkt.data_len=l;
    strcpy(p->data,(char*)lgpData[LGP_AuthGrant]);
    AddStringToLog(t,0x01,p->data,I_str,0xFFFFFFFF,1);
    AddMsgToChat(ed_struct->ed_chatgui);
    RecountMenu(t, 1);
    SUBPROC((void *)SendAnswer,0,p);
  }
  GeneralFuncF1(1);
}

void OpenLogfile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct;
  ed_struct=MenuGetUserPointer(data);

  extern const char HIST_PATH[64];
  extern const int HISTORY_TYPE;
  CLIST *t;
  WSHDR *ws=AllocWS(256);
  if ((t=ed_struct->ed_contact))
  {
    if (HISTORY_TYPE)
      wsprintf(ws,"%s\\%u\\%u.txt",HIST_PATH,UIN,t->uin);
    else
      wsprintf(ws,"%s\\%u.txt",HIST_PATH,t->uin);
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
      RecountMenu(t, 1);
      GeneralFuncF1(1);
    }
  }
}

/*
*  Функция: LoadMoreHistory
*  Описание: Загружает в чат истории на 8кб, бывает нужно))
*  Параметры: GUI *data
*  Возвращаемое значение: Ничего не возвращает
*/
void LoadMoreHistory(GUI *data)
{
   ClearLog(data);
  EDCHAT_STRUCT *ed_struct;
  CLIST *t;

  ed_struct = MenuGetUserPointer(data);
  t=ed_struct->ed_contact;    
  GetHistory(t, 8*1024);
  GeneralFunc_flag1(edchat_id,1);
  CreateEditChat(t);
}

void ecmenu_ghook(void *data, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}

static MENUITEM_DESC ecmenu_ITEMS[EC_MNU_MAX]=
{
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2},
  {NULL,NULL,LGP_NULL,0,NULL,MENU_FLAG3,MENU_FLAG2}
};

extern void AddSmile(GUI *data);
static const MENUPROCS_DESC ecmenu_HNDLS[EC_MNU_MAX]=
{
  QuoteFocus,//Quote,
  QuoteNotFocus,//QuoteNotFocus,
  AddSmile,
  GetShortInfo,
  AddCurContact,
  SendAuthReq,
  SendAuthGrant,
  OpenLogfile,
  LoadMoreHistory,
  ClearLog,
};
/*
static const void *menuprocs[EC_MNU_MAX]=
{
  (void *)QuoteFocus,
  (void *)QuoteNotFocus,
  (void *)AddSmile,
  (void *)GetShortInfo,
  (void *)AddCurContact,
  (void *)ChangeSoundMode,
#ifdef ELKA
  (void *)ChangeSLIMode,
#endif
  (void *)ChangeDoubleVibra,
  (void *)ChangePOPUPMode,
  (void *)ChangeShowOfflineMode,
  (void *)ChangeShowGroupsMode,
  (void *)ChangeActiveUp,
  (void *)EditConfig,
  (void *)Disconnect, 
  (void *)PingToServer,
  (void *)AboutDlg
};*/

//05.10.2009
int ec_menu_onkey(void *data, GUI_MSG *msg)//кейхук для закладок
{
/*  int i;
  i=GetCurMenuItem(data);
  int k=msg->gbsmsg->submess;
  int m=msg->gbsmsg->msg;
  if(m==KEY_DOWN && (k>='0' && k<='9'))
  {
//    if(k>='0' && k<='9')       
      ((void (*)(void))(ecmenu_HNDLS[k-'1']))();    
    return 1;  
  }*/  
  return 0;
}


char ecm_contactname[64];

static const HEADER_DESC ecmenu_HDR={0,0,NULL,NULL,NULL,(int)ecm_contactname,LGP_NULL};

static const MENU_DESC ecmenu_STRUCT=
{
  8,/*ec_menu_onkey*/NULL,ecmenu_ghook,NULL,
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
    if(strlen(t->name))
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
    
    //Инициализация ленгпака
    ecmenu_ITEMS[0].lgp_id_small=(int)lgpData[LGP_MnuQuote]; 
    ecmenu_ITEMS[1].lgp_id_small=(int)lgpData[LGP_MnuQuoteNotFocus]; 
    ecmenu_ITEMS[2].lgp_id_small=(int)lgpData[LGP_MnuAddSml];
    ecmenu_ITEMS[3].lgp_id_small=(int)lgpData[LGP_MnuShInfo]; 
    ecmenu_ITEMS[4].lgp_id_small=(int)lgpData[LGP_MnuAddRen];
    ecmenu_ITEMS[5].lgp_id_small=(int)lgpData[LGP_MnuSAuthReq]; 
    ecmenu_ITEMS[6].lgp_id_small=(int)lgpData[LGP_MnuSAuthGrt]; 
    ecmenu_ITEMS[7].lgp_id_small=(int)lgpData[LGP_MnuOpenLog]; 
    ecmenu_ITEMS[8].lgp_id_small=(int)"Больше истории"; 
    ecmenu_ITEMS[9].lgp_id_small=(int)lgpData[LGP_MnuClearCht]; 
    
    CreateMenu(0,0,&ecmenu_STRUCT,&ecmenu_HDR,0,EC_MNU_MAX,ed_struct,to_remove);
  }
}


void AskNickAndAddContact(EDCHAT_STRUCT *ed_struct)
{
  CreateAddContactGrpDialog(ed_struct->ed_contact);
}

static void AddSmile(GUI *data)
{
  EDCHAT_STRUCT *ed_struct=MenuGetUserPointer(data);
  CreateSmileSelectGUI(ed_struct);
  GeneralFuncF1(1);
}
