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
#include "naticq_ipc.h"
#include "status_change.h"
#include "strings.h"
//#include "string_util_int.h"
#include "manage_cl.h"
#include "cl_work.h"
#include "select_smile.h"
#include "revision.h"
#include "print.h"
#include "screen_save.h"
#ifdef NEWSGOLD
#include "player_tags.h"
#endif
#include "swilib2.h"
#include "rect_patcher.h"

#ifndef NEWSGOLD
#define SEND_TIMER
#endif
extern const unsigned int Vibra_cc;
#ifdef ELKA
#pragma swi_number=54
__swi __arm void SLI_SetState(unsigned char state);
#endif

#define IN_TICKS(sec) (sec * TMR_SECOND)
extern int janis;
extern const unsigned int Ping_X;
extern const unsigned int Ping_Y;
extern const unsigned int unread_X;
extern const unsigned int unread_Y;
extern  const int show_xstatus;
extern const char fontpath[64];
extern const int ShowStatusIcon;

int main_d;

//For online/offline log
extern const int isLogOffline;
extern const int isLogOnline;

#ifdef ELKA
//If action vol up && vol down button on elka
extern const int isVolUpButton;
extern const int isVolDownButton;
#endif

//Autoaway message
extern const char awayHelloMsg[24];//hello msg
extern const char awayMsg[64];//message

extern const int IsShowLoopback;//Show Loopback contact in CL
//For png font
extern const unsigned int fonto;
extern const unsigned int spaco1;
extern const unsigned int spaco2;
//int spaco2=spaco1;
extern const char fontColor[];//���� ��� ���������� ������
extern const char bgrColor[];


extern int *XStatusesIconArray;
int align=0;
//int space=1;
void *canvasdata;
int paused_height;

extern const unsigned int AUTO_ENGADE;
char talk_sound[128];
int uins;
//char m_1[]="unofficial! private use only";
//char m_2[]="mod by (c)Evilfox 315384924";
char unread_msg_info[64];
extern const int Is_Unread_On;
int saver_on;

#ifdef NEWSGOLD
  extern char teast2[256];
#endif

#ifdef ELKA
  char last_msg_buffer[4048];
#endif


///////////////////////////////////////////////auto ping////////////////////
GBSTMR mytmr3;
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
    PingToServer2();
    GBS_StartTimerProc(&mytmr3, 12960, Pinger2);
  }
  else
    GBS_StartTimerProc(&mytmr3, 2160, Pinger2);
}

//////////////////////////////////////auto ping/////////////////////////////

extern volatile int total_smiles;
extern volatile int total_xstatuses;
extern volatile int xstatuses_load;
extern volatile int pictures_max; //2026rev
extern volatile int pictures_loaded; //2026rev

#ifdef ELKA
  int Is_SLI_On;//������ �����������
#endif

int Is_SMARTCPU_On;//����� ��������� �� ��
int Is_INC_On;//Popup message
int Is_ILU_On;//����� ���������
int Is_Double_Vibra_On;//����� �����������
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
#define USE_MLMENU

#define TMR_SECOND 216

//IPC
const char ipc_my_name[32]=IPC_NATICQ_NAME;
const char ipc_xtask_name[]=IPC_XTASK_NAME;
const char ipc_test_name[]="test";
IPC_REQ gipc;


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

//�� 10 ������
#define ACTIVE_TIME 360

//������������ ���������� ��������� � ����
#define MAXLOGMSG (20)

// ��������� �������� ��������

const char S_OFFLINE[]="Offline";
const char S_INVISIBLE[]="Invisible";
const char S_AWAY[]="Away";
const char S_NA[]="N/A";
const char S_OCCUPIED[]="Occupied";
const char S_DND[]="DND";
const char S_ONLINE[]="Online";
const char S_FFC[]="FFC";

volatile int SENDMSGCOUNT;
#ifdef kluchnik
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
#ifdef NEWSGOLD
extern int CurrentXStatus;
#else
int CurrentXStatus;
#endif

WSHDR *ews;

extern const unsigned int UIN;
extern const char PASS[];

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
  "vis5.png"  
};

extern const char ICON_PATH[];

////=================================away_msg==============

#ifdef NEWSGOLD
  #pragma swi_number=0x0C5
  __swi __arm int IsIdleUiOnTop();//�� �� ��� ���� �������
#else
  int IsIdleUiOnTop(void)
  { 
    void *icsm=FindCSMbyID(CSM_root()->idle_id); 
    if (IsGuiOnTop(((int*)icsm)[DISPLACE_OF_IDLEGUI_ID/4 ])) 
      return(1); 
    else
    return(0); 
  } 
#endif

int msg_away_timer=0;//������� ��� �����������
char away_msg[264];//���� ��������� ��� ������
char away_msg_time[64];//����� ����� �������� ������
char away_msg_time_dace[16];//����� ���������� ��� ������ �� ����
char *inp_away_msg2;//����� ���������,���� ����� ������ ����(�� ������ 6 ��� VOL_DOWN_BUTTON),� �� ����� �� �������
char tim_zone[16];
extern const int time_zone;
int timeh;
int timemin;
int away_m;
int status_rem;//��� ���������� �������� �������
int change_back_status;//������� ���������� ������ ��� ���
GBSTMR away_tmr;
extern const int Auto_Status;//������� ���������� ��� ���

int Show_XTX;//���������� ������� ��� ����� �� ����
void FreeXText2(CLIST *t)
{
  if (t->xtext2) {mfree(t->xtext2);t->xtext2=NULL;}
}

void away_timer()//������ �����������
{
  if(msg_away_timer==AUTO_ENGADE)
  {
    TTime tt;
    GetDateTime(NULL,&tt);
    sprintf(away_msg_time_dace, "%2d:%2d", tt.hour, tt.min);
    sprintf(away_msg_time, "( ���������� �������� � %02d:%02d [%s] )", tt.hour, tt.min, tim_zone);
    away_m=1;
    if(CurrentStatus!=2)
    {
      status_rem=CurrentStatus;
      CurrentStatus=2;
      set_my_status();
//      SUBPROC((void*)WriteDefSettings);
      change_back_status=1;
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

//=================================away_msg===============
//////////////////////////////////////////tozhe dlja auto ping///////////////

void DrawPing(void *canv)
{
#ifdef EVILFOX
  PrintField(date_X,date_Y, PINGO, align, fonto, space);
#else
  PrintField(Ping_X,Ping_Y, PINGO, align, fonto, spaco1);
#endif
}

//////////////////////////////////////////tozhe dlja auto ping///////////////

GBSTMR mytmr;
GBSTMR mytmr2;
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
    GBS_StartTimerProc(&mytmr2, 6480, Remind_Timer2);
  }
}

void Reminder_timer()
{
  GBS_StartTimerProc(&mytmr2, 6480, Remind_Timer2);
}
//////////////////////////reminder timer/////////////////////////

//////////////////////////uptime//////////////////////////
int Is_UPTIME_On;
int xstatus_old;
int minimum_xstatus;
int maximum;

//Online time on idle
extern const unsigned int onlineTime_x;
extern const unsigned int onlineTime_y;

void xstatuse()
{
  maximum++;
  if(maximum==60)
  {
    if(saver_on) reftes(); 
    maximum=0;
    minimum_xstatus++;
  }
  if(minimum_xstatus==60)
  {
    minimum_xstatus=0;
    xstatus_old++;
  }
}

GBSTMR mytmr4;
#ifdef NEWSGOLD
  GBSTMR tracktmr;
  int tracktimer;
#endif
extern const int Pause_Time;

void x_zstatus()
{
  if(connect_state==3)
  {
  xstatuse();
#ifdef NEWSGOLD
  if (tracktimer>=10)/*&&(_GetPlayStatus()!=1))*/
    {
      GBS_StartTimerProc(&tracktmr, 216*5, GetTags);
      tracktimer=0;
    }
    tracktimer++;
#endif
  GBS_StartTimerProc(&mytmr4, 216, x_zstatus);
  }
  else
  {
    if(!Pause_Time)
    {
    xstatus_old=0;
    minimum_xstatus=0;
    maximum=0;
    }
    GBS_StartTimerProc(&mytmr4, 216, x_zstatus);
  }
}

char onlineTime[128];
void DrawOnlineTime(void *canv)
{
  
  if(connect_state==3)
  {   
   sprintf(onlineTime,"Online: %02d:%02d:%02d", xstatus_old, minimum_xstatus, maximum);
  }
  else
    sprintf(onlineTime,"Offline");
  PrintField(onlineTime_x,onlineTime_y, onlineTime, align, fonto, spaco1);
}

//--------------------------  ����� ������� �� ����  -----------------//

extern const unsigned int auto_x;
extern const unsigned int auto_y;
extern const int showAwayIdle;

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

//������ ��������� ����� �����,� �� �� ��� ��������� � �������
int input_awaymsg_id;

const HEADER_DESC input_awaymsg_hdr={0,0,131,21,NULL,(int)"���� �������������",LGP_NULL};
void UpdateCLheader(void);
int input_awaymsg_onkey(GUI *data, GUI_MSG *msg)
{
  WSHDR *ws=AllocWS(64);
  if (msg->keys==0x0FFF || msg->keys==VOL_DOWN_BUTTON)//0xFFF
  {
    EDITCONTROL ec;
    ExtractEditControl(data,2,&ec);	
    ws=ec.pWS;
    inp_away_msg2=malloc(ec.maxlen);
    int i=0;
    for(i=0;i<ws->wsbody[0];i++)
      *(inp_away_msg2+i)=char16to8(ws->wsbody[i+1]);
      *(inp_away_msg2+i++)=0;

#ifdef kluchnik
    char *s;
    ExtractEditControl(data,4,&ec);	
    ws=ec.pWS;
    s=malloc(ec.maxlen);
    for(i=0;i<ws->wsbody[0];i++)
      *(s+i)=char16to8(ws->wsbody[i+1]);
      *(s+i++)=0;
    strcat(inp_away_msg2,s);
#endif    
      
    UpdateCLheader();
    GeneralFunc_flag1(input_awaymsg_id,1);
    FreeWS(ws);
//    GeneralFuncF1(1);
  }
  
  if(msg->keys==LEFT_SOFT || msg->keys==RED_BUTTON)
  {
     msg_away_timer=0;
     away_m=0;
     UpdateCLheader();
  }
return(0);
}

void input_awaymsg_ghook(GUI *gui, int cmd)
{ 
  static SOFTKEY_DESC sk={0x0FFF,0x0000,(int)"���������"};
  if (cmd==7)
  {
    SetSoftKey(gui,&sk,SET_SOFT_KEY_N);
  }
  if(cmd==0xA)
  {
    DisableIDLETMR(); // ��������� ������ ������ �� ��������
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
  {0x0018,0x0000,(int)"���"},
  {0x0001,0x0000,(int)"����"},
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
   wsprintf(wss,"%t","���������:\n");
   ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,wss,32);
   AddEditControlToEditQend(eq,&ec,ma);  

   PrepareEditControl(&ec);
   CutWSTR(wss,0);
   ascii2ws(wss,awayMsg);
   ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,wss,255);
   AddEditControlToEditQend(eq,&ec,ma);
#ifdef kluchnik
   PrepareEditControl(&ec);
   wsprintf(wss,"%t","��������� 2:\n");
   ConstructEditControl(&ec,ECT_HEADER,ECF_NORMAL_STR,wss,32);
   AddEditControlToEditQend(eq,&ec,ma);  

   ascii2ws(wss,"����� ����� ������:)");
   ConstructEditControl(&ec,ECT_NORMAL_TEXT,ECF_APPEND_EOL,wss,255);
   AddEditControlToEditQend(eq,&ec,ma);
#endif
   patch_header(&input_awaymsg_hdr);
   patch_input(&input_awaymsg_desc);      
 
   FreeWS(wss);
   input_awaymsg_id=CreateInputTextDialog(&input_awaymsg_desc,&input_awaymsg_hdr,eq,1,0);//������� ���������� ����
}


//////////////////////////uptime//////////////////////////
#ifdef ELKA
void SLI_On()
{
  if(Is_SLI_On)
    SLI_SetState(1);   
  GBS_StartTimerProc(&mytmr, IN_TICKS(2) / 20, SLI_Off);
}

void SLI_Off()
{
  SLI_SetState(2);
  if(total_unread)
     GBS_StartTimerProc(&mytmr, IN_TICKS(2) / 20, SLI_On); //26*SLI_time2
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
  GBS_StartTimerProc(&mytmr, 262, SLI_check); //262*SLI_time
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

extern const unsigned int IDLEICON_XX;
extern const unsigned int IDLEICON_YX;

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
const char I_str[]="I";
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
int Is_Vibra_Enabled;
unsigned int Is_Sounds_Enabled;
int Is_Show_Offline;
int Is_Show_Groups;
int CurrentStatus;
extern int CurrentXStatus; //ili extern? /windes
int CurrentPrivateStatus;

//===================================================================
const char def_setting[]="%sdef_settings_%d";   
  
void ReadDefSettings(void)
{
  DEF_SETTINGS def_set;
  int f;
  unsigned int err;
  char str[128];
  snprintf(str,127,def_setting,elf_path,UIN);
  if ((f=fopen(str,A_ReadOnly+A_BIN,P_READ,&err))!=-1)
  {
    fread(f,&def_set,sizeof(DEF_SETTINGS),&err);
    fclose(f,&err);
    Is_Vibra_Enabled=def_set.vibra_status;
#ifdef NEWSGOLD
    Is_Id3tag_Enabled=def_set.Id3tag_status;
#endif
    Is_Sounds_Enabled=def_set.sound_status;
#ifdef ELKA
    Is_SLI_On=def_set.sli_state;
#endif
    Is_SMARTCPU_On=def_set.smartcpu_state;
    Is_INC_On=def_set.inc_state;
    Is_ILU_On=def_set.ilu_state;
    Is_PING_On=def_set.ping_state;
    Is_UPTIME_On=def_set.uptime_state;
    Is_Show_Offline=def_set.off_contacts;
    Is_Show_Groups=def_set.show_groups;
    CurrentStatus=def_set.def_status+1;
    CurrentXStatus=def_set.def_xstatus;
//    Is_SLI_On=def_set.sli_state;
    Is_Double_Vibra_On=def_set.remind_state;
//    Is_ILU_On=def_set.ilu_state;
  }
  else
  {
    Is_Vibra_Enabled=0;
#ifdef NEWSGOLD
    Is_Id3tag_Enabled=0;
#endif
    Is_Sounds_Enabled=0;
    Is_Show_Offline=0;
    Is_Show_Groups=0;
    CurrentStatus=IS_ONLINE;
    CurrentXStatus=0;
#ifdef ELKA
    Is_SLI_On=1;
#endif
    Is_SMARTCPU_On=1;
    Is_INC_On=1;
    Is_Double_Vibra_On=0;
    Is_ILU_On=1;
    Is_PING_On=0;
    Is_UPTIME_On=0;
  }
}

void UpdateCLheader(void);

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
#ifdef NEWSGOLD
    def_set.Id3tag_status=Is_Id3tag_Enabled;
#endif
    def_set.sound_status=Is_Sounds_Enabled;
    def_set.off_contacts=Is_Show_Offline;
    def_set.show_groups=Is_Show_Groups;
    def_set.def_status=CurrentStatus-1;
    def_set.def_xstatus=CurrentXStatus;
#ifdef ELKA
    def_set.sli_state=Is_SLI_On;
#endif
    def_set.smartcpu_state=Is_SMARTCPU_On;
    def_set.inc_state=Is_INC_On;
    def_set.remind_state=Is_Double_Vibra_On;
    def_set.ilu_state=Is_ILU_On;
    def_set.ping_state=Is_PING_On;
    def_set.uptime_state=Is_UPTIME_On;
    fwrite(f,&def_set,sizeof(DEF_SETTINGS),&err);
    fclose(f,&err);
  }
  
 UpdateCLheader();//������� ����� ���� ���� �� ����))
}

//by KreN 27.09.2007
//===================================================================

extern S_SMILES *s_top;
extern DYNPNGICONLIST *SmilesImgList;
extern DYNPNGICONLIST *XStatusesImgList;

//=============================������������ �����=======================

extern const char sndStartup[];
extern const char sndSrvMsg[];
extern const char sndGlobal[];
extern const char sndGlobal2[];
extern const char sndMsg[];
extern const char sndMsg2[];
extern const unsigned int extended_sounds;
extern const char sndMsgSent[];
extern const unsigned int sndVolume;


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
  extern const int VIBR_TYPE;
  void stop_vibra(void);
  if((Is_Vibra_Enabled)&&(!IsCalling()))
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
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>5,start_vibra);
    else
      GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,start_vibra);
  }
}

void start_vibra2(void)
{
  extern const int VIBR_TYPE;
  void stop_vibra2(void);
  extern const unsigned int vibraPower;
  SetVibration(vibraPower);
  if(VIBR_TYPE)
    GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>2,stop_vibra2);
   else
     GBS_StartTimerProc(&tmr_vibra,TMR_SECOND>>1,stop_vibra2);
}

void stop_vibra2(void)
{
  extern const int VIBR_TYPE;
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
char *templates_chars; //���������� ����
char **templates_lines; //������ ���������� �� ������

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



int RXstate=EOP; //-sizeof(RXpkt)..-1 - receive header, 0..RXpkt.data_len - receive data

TPKT RXbuf;
TPKT TXbuf;

int connect_state=0;

int sock=-1;

volatile unsigned long TOTALRECEIVED;
volatile unsigned long TOTALSENDED;
volatile unsigned long ALLTOTALRECEIVED;	//by BoBa 10.07
volatile unsigned long ALLTOTALSENDED;

volatile int sendq_l=0; //������ ������� ��� send
volatile void *sendq_p=NULL; //��������� �������

volatile int is_gprs_online=1;

GBSTMR reconnect_tmr;

extern void kill_data(void *p,void (*func_p)(void *));

void ElfKiller(void)
{
//  if(SLI_TYPE)
//  GBS_StopTimer(&mytmr);
//  if(Is_Double_Vibra_On)
//  GBS_StopTimer(&mytmr2);
//  GBS_StopTimer(&mytmr3);
//  GBS_StopTimer(&mytmr4);
//  DoChangeWall2();
  PrintLibFree();
  extern void *ELF_BEGIN;
  kill_data(&ELF_BEGIN,(void (*)(void *))mfree_adr());
}

int total_unread; //2058 - tam tak /windes

volatile CLIST *cltop;

volatile unsigned int GROUP_CACHE; //������� ������ ��� ����������

volatile int contactlist_menu_id;

GBSTMR tmr_active;

volatile int edchat_id;

//����������� ��� ���������� ���������
//CLIST *edcontact;
//void *edgui_data;

static int prev_clmenu_itemcount;

char clm_hdr_text[48];
static const char def_clm_hdr_text[] = LG_CLTITLE;
static const char key_clm_hdr_text[] = LG_CLT9INP;

static const HEADER_DESC contactlist_menuhdr = {0, 0, 0, 0, S_ICONS+ICON_HEAD, (int)clm_hdr_text, LGP_NULL};
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

static const SOFTKEY_DESC clmenu_sk[]=
{
  {0x0018, 0x0000, (int)LG_OPTIONS},
  {0x0001, 0x0000, (int)clmenu_sk_r},
  {0x003D, 0x0000, (int)LGP_DOIT_PIC}
};

const SOFTKEYSTAB menu_skt =
{
  menu_sk, 0
};

static const SOFTKEYSTAB clmenu_skt =
{
  clmenu_sk, 0
};

void contactlist_menu_ghook(void *data, int cmd);
int contactlist_menu_onkey(void *data, GUI_MSG *msg);
void contactlist_menu_iconhndl(void *data, int curitem, void *unk);

#ifdef USE_MLMENU
static const ML_MENU_DESC contactlist_menu=
{
  8,contactlist_menu_onkey,contactlist_menu_ghook,NULL,
  menusoftkeys,
  &clmenu_skt,
  0x11, //+0x400
  contactlist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0,   //n
  1 //���������� �����  
};
#else
static const MENU_DESC contactlist_menu=
{
  8,contactlist_menu_onkey,contactlist_menu_ghook,NULL,
  menusoftkeys,
  &clmenu_skt,
  0x11, //+0x400
  contactlist_menu_iconhndl,
  NULL,   //Items
  NULL,   //Procs
  0   //n
};
#endif

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

//���������� ���
void FreeLOGQ(LOGQ **pp)
{
  LOGQ *p=*pp;
  *pp=NULL; //����� �������
  while(p)
  {
    LOGQ *np=p->next;
    mfree(p);
    p=np;
  }
}

//������� ������� �� ����
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
  mfree(p); //���� ������� ������ �� � ����
}

//�������� ������� � ���
int AddLOGQ(LOGQ **queue, LOGQ *p)
{
  LOGQ *q=(LOGQ *)queue;
  LOGQ *qp;
  int i=0;
  while(qp=q->next) {q=qp;i++;}
  p->next=NULL;
  q->next=p;
  return(i+1); //������ ����� � ���� ���������
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

GBSTMR statusTmr;
extern const int TIME_OR_MSG;
char ContactT9Key[32];
int kopa_online;
int kopa_kontaktu;
char status[64];
void DrawStatusBar()
{
// PrintText( int x, int y, char *str, int align, int font, int space, int lineSpace, int isScroll, short fixSize, int maxLen);
//  if(TIME_OR_MSG==4)
  if(contactlist_menu_id)
  {
    DrawRectangle(40,YDISP,scr_w-1,YDISP+30,0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
    PrintText(33,YDISP+2,status,0,1,1,2,0,0,ScreenW()-20);
  }
  //PrintField( int x, int y, char *str, int align, int font, int space);
//  PrintField( 40, YDISP, status,0, t, int space);
  GBS_StartTimerProc(&statusTmr,120,DrawStatusBar);
}

void UpdateCLheader(void)
{
  if (strlen(ContactT9Key))
  {
    strcpy(clm_hdr_text,key_clm_hdr_text);
    strcat(clm_hdr_text,ContactT9Key);
    strcpy(clmenu_sk_r,key_clmenu_sk_r);
  }
  else
  {
    switch(TIME_OR_MSG)
    {
    case 0:
      {
        TTime the;
        GetDateTime(NULL,&the);
        sprintf(clm_hdr_text, "%s(%02d:%02d)",def_clm_hdr_text,the.hour,the.min);       
      }
      break;
    case 1:
      sprintf(clm_hdr_text, "%s(%d/%d)",def_clm_hdr_text,total_unread,total_unread2);
      break;
    case 2:
      sprintf(clm_hdr_text, "%s(%d/%d)",def_clm_hdr_text,kopa_kontaktu,kopa_online);
      break;
    case 3:
      {
        void Get_Online_total();
        Get_Online_total();
        sprintf(clm_hdr_text, "[%d/%d] [%d/%d] ",kopa_kontaktu,kopa_online,total_unread,total_unread2);
        if(away_m) strcat(clm_hdr_text,"[A]");
        if(CurrentPrivateStatus==1) strcat(clm_hdr_text,"[I]");
        if(Is_Vibra_Enabled) strcat(clm_hdr_text,"[V]");
        if(Is_Sounds_Enabled) strcat(clm_hdr_text,"[S]");
        if(Is_Double_Vibra_On) strcat(clm_hdr_text,"[R]");
       }
      break;      
    case 4:
      {
        void Get_Online_total();
        Get_Online_total();
        sprintf(status, "%s[%d/%d] [%d/%d] ",def_clm_hdr_text,kopa_kontaktu,kopa_online,total_unread,total_unread2);
        if(away_m) strcat(status,"[A]");
        if(CurrentPrivateStatus==1) strcat(status,"[I]");
        if(Is_Vibra_Enabled) strcat(status,"[V]");
        if(Is_Sounds_Enabled) strcat(status,"[S]");
        if(Is_Double_Vibra_On) strcat(status,"[R]");
        canvasdata=BuildCanvas();
        DrawStatusBar();
      }
      break;
    }
    strcpy(clmenu_sk_r,def_clmenu_sk_r);
  }
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

void create_contactlist_menu(void)
{
  int i;
  i=CountContacts();
  prev_clmenu_itemcount=i;
  UpdateCLheader();
  patch_header(&contactlist_menuhdr);
#ifdef USE_MLMENU
  contactlist_menu_id=CreateMultiLinesMenu(0,0,&contactlist_menu,&contactlist_menuhdr,0,i);
#else
  contactlist_menu_id=CreateMenu(0,0,&contactlist_menu,&contactlist_menuhdr,0,i,0,0);
#endif
}

void contactlist_menu_ghook(void *data, int cmd)
{
  PNGTOP_DESC *pltop=PNG_TOP();
  if (cmd==9)
  {
    pltop->dyn_pltop=NULL;
  }
  if (cmd==0x0A)
  {
    silenthide=0;
    pltop->dyn_pltop=XStatusesImgList;
    DisableIDLETMR();
  }
}

void RecountMenu(CLIST *req, int needfocus)
{
  int i;
  int j;
  void *data;
  UpdateCLheader();
  if (!contactlist_menu_id) return; //������ �������
  data=FindGUIbyId(contactlist_menu_id,NULL);
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
  if (i!=prev_clmenu_itemcount)
  {
    prev_clmenu_itemcount=i;
    Menu_SetItemCountDyn(data,i);
  }
  if(needfocus) SetCursorToMenuItem(data,j); //2058
  if (IsGuiOnTop(contactlist_menu_id)) RefreshGUI();
}

char hostbuf2[128];

int contactlist_menu_onkey(void *data, GUI_MSG *msg)
{
  CLIST *t;
  int i;
  i=GetCurMenuItem(data);
  t=FindContactByN(i);
  if (msg->keys==0x18)
  {
    ShowMainMenu();
    return(-1);
  }
  if (msg->keys==0x3D)
  {
    if (t)
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
  if (msg->keys==1)
  {
    if (strlen(ContactT9Key))
    {
      BackSpaceContactT9();
      RecountMenu(NULL,1);
      return(-1);
    }
  }
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    int key=msg->gbsmsg->submess;
    if ((key>='0')&&(key<='9'))
    {
      AddContactT9Key(key);
      RecountMenu(NULL,1);
      return(-1);
    }
#ifdef ELKA
    if (key==VOL_UP_BUTTON && isVolUpButton)
    {
      char *small_info=malloc(256);
      sprintf(small_info, "����� � ������:\n"
                          "%02d:%02d:%02d\n"
                          "������:\n%s"
                          ,xstatus_old, minimum_xstatus, maximum, hostbuf2);
      ShowMSG(1,(int)small_info);
      mfree(small_info);
    }
    
    if(msg->gbsmsg->submess==VOL_DOWN_BUTTON && isVolDownButton) 
    {
      if(!away_m)
      {
        away_m=!away_m;
        GBS_StopTimer(&away_tmr);
        msg_away_timer=AUTO_ENGADE;
        away_timer();
        main_d=1;
//        ShowMSG(1,(int)"������������ �������!");
        InputAwayMsg();
      }
      else
      {
        msg_away_timer=0;
        away_m=!away_m;
//        ShowMSG(1,(int)"������������ ��������!");
      }
      UpdateCLheader();
      RefreshGUI();
    }
#endif
    if (key==GREEN_BUTTON)
    {
#ifdef kluchnik
       Show_XTX=!(Show_XTX);
      if(Show_XTX)
        ShowMSG(1,(int)"Show x-status");
      else
        ShowMSG(1,(int)"Show time");
#else
      IsActiveUp=!IsActiveUp;
      RecountMenu(NULL,1);
#endif
      return(-1);
    }
    if (key==LEFT_BUTTON)
    {
      while(t=(CLIST *)(t->next))
       {
         if (t->isunread) goto doit;
       }
      t=(CLIST *)(&cltop);
      while(t=(CLIST *)(t->next))
      {
        if (t->isunread) goto doit;
      }
      if (t && Is_Show_Groups) {//???? ??????? ? ?????????? ???????
       if (!t->isgroup)
        t=FindGroupByID(t->group);
       if (t && !t->state) {//?????? ??????? ? ??? ????????
        t->state^=0xFFFF;
        goto does;
       }
      }
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
    if (key=='*')
    {
      while(t=(CLIST *)(t->next))
       {
         if (t->just_now) goto doit3;
       }
      t=(CLIST *)(&cltop);
      while(t=(CLIST *)(t->next))
      {
        if (t->just_now) goto doit3;
      }
      return(-1);
    doit3:
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
      return(-1);
    }
    extern const int ActionResh;
    if (key=='#')
    {
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
       if(t->log)
        {
          FreeLOGQ(&t->log);
          t->msg_count=0;
        }
        if(strlen(ContactT9Key))
          ClearContactT9Key();
          t->isactive=0;
          RecountMenu(NULL,0);
      }
      return(-1);
    }
    if (key==RIGHT_BUTTON)
    {
      //������� ������ ������ XStatus
      if (t)
      {
	if (!t->isgroup)
	{
	  FreeXText(t);
          t->req_xtext=0;          
          RequestXText(t->uin);
	  RefreshGUI();
	  return(-1);
	}
      }
    }
    
    if (key==0x19)
    {
      CreateScreenSelectGUI();
//      GeneralFuncF1(1);
      return(-1);
    }
  }
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
    if (msg->gbsmsg->submess=='#')
    {
    //  if(isLongPressResh==1)
    //  {
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
//      }
      
  //    CreateScreenSelectGUI();
      return(-1);
    }
    if (msg->gbsmsg->submess=='*')
    {
      ClearContactT9Key();
      RecountMenu(NULL,1);
      silenthide=1;
      gipc.name_to=ipc_xtask_name;
      gipc.name_from=ipc_my_name;
      gipc.data=0;
      GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
    }
    
//-------------------- LONG PRESS ACTION ON CL --------------------//
    
  switch(msg->gbsmsg->submess)
    {
    case '1':
        Is_Sounds_Enabled=!(Is_Sounds_Enabled);
        if(Is_Sounds_Enabled)
          ShowMSG(1,(int)"Sound 0n");
        else
          ShowMSG(1,(int)"Sound Off");
        SUBPROC((void*)WriteDefSettings);
        RefreshGUI();
        break;
    case '2':
        Is_Vibra_Enabled=!(Is_Vibra_Enabled);
        SUBPROC((void*)WriteDefSettings);
        RefreshGUI();
        if(Is_Vibra_Enabled)
          ShowMSG(1,(int)"Vibra On");
        else
          ShowMSG(1,(int)"Vibra Off");
        break;
    case '3':        
         if(CurrentPrivateStatus!=1)
         {
           SetPrivateStatus(1);//�����
           ShowMSG(1,(int)"����� �������");
         }
         else
         {
           SetPrivateStatus(3);//�� �����:)
           ShowMSG(1,(int)"����� ��������");
         }
         break;
    case '4':
        Is_Double_Vibra_On=!(Is_Double_Vibra_On);
        SUBPROC((void*)WriteDefSettings);
        RefreshGUI();
        if(Is_Double_Vibra_On)
          ShowMSG(1,(int)"Vibra rem On");
        else 
          ShowMSG(1,(int)"Vibra rem Off");
        break;
    case '5':
      {
        CLIST *cl;
        cl=(CLIST *)cltop;
        while(cl)
        {
         if(cl->log)
          {
            FreeLOGQ(&cl->log);
            cl->msg_count=0;
          }    
         cl=cl->next;
        }
      
        if(strlen(ContactT9Key))
          ClearContactT9Key();
          t->isactive=0;
          RecountMenu(NULL,1);
          return (-1);
        }
    case '6':
      if(!away_m)
      {
        away_m=!away_m;
        GBS_StopTimer(&away_tmr);
        msg_away_timer=AUTO_ENGADE;
        away_timer();
       // ShowMSG(1,(int)"������������ �������!");
        InputAwayMsg();
      }
      else
      {
        msg_away_timer=0;
        away_m=!away_m;
     //   ShowMSG(1,(int)"������������ ��������!");
      }
      UpdateCLheader();
      break;
    case '9':
      {
        extern const char *successed_config_filename;
        WSHDR *ws;
        ws=AllocWS(150);
        str_2ws(ws,successed_config_filename,128);
        ExecuteFile(ws,0,0);
        FreeWS(ws);
      }
    }
    
    int key=msg->gbsmsg->submess;
    if ((key>='0')&&(key<='9'))
    {
      ClearContactT9Key();
      UpdateCLheader();
      RecountMenu(NULL,1);    
    }      
  }
  return(0);
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


void contactlist_menu_iconhndl(void *data, int curitem, void *unk)
{
  CLIST *t;
#ifdef USE_MLMENU
  void *item=AllocMLMenuItem(data);
#else
  void *item=AllocMenuItem(data);
#endif
  int icon;
  
  WSHDR *ws2;
#ifdef USE_MLMENU
  WSHDR *ws4;
#endif
  WSHDR ws1loc, *ws1;
  unsigned short num[128];
  ws1=CreateLocalWS(&ws1loc,num,128);
#ifdef USE_MLMENU
  WSHDR ws3loc, *ws3;
  unsigned short num3[128];
  ws3=CreateLocalWS(&ws3loc,num3,128);
  char s[64];
#endif
  t=FindContactByN(curitem);
  if (t)
  {
    icon=GetIconIndex(t);
    if (icon!=IS_GROUP)
    {
      if(t->just_now)
      {
        char *test=malloc(64);
        sprintf(test, ">> %s <<", t->name);
        wsprintf(ws1,percent_t,test);
        mfree(test);
      }
      else
        wsprintf(ws1,percent_t,t->name);
      if (t->just_now)
      {
        wsInsertChar(ws1,0x0004,1);
        wsInsertChar(ws1,0xE008,1);
      }
      else
      if (t->isactive)
      {
        wsInsertChar(ws1,0x0002,1);
        wsInsertChar(ws1,0xE008,1);
      }
#ifdef USE_MLMENU
      if ((t->xtext && t->xtext[0]!=0) && (Show_XTX))
      {
	int i;
	zeromem(s,64);
	i=t->xtext[0];
	if (i>63) i=63;
	strncpy(s,t->xtext+1,i);
	wsprintf(ws3,"%c%t",0xE012,s);
      }
      else
        if(t->xtext2 && t->xtext2[0]!=0)
        {
          int i;
          zeromem(s,64);
          i=t->xtext2[0];
          if (i>63) i=63;
          strncpy(s,t->xtext2+1,i);
          wsprintf(ws3,"%c%t",0xE012,s);
        }
      else
	wsprintf(ws3,percent_d,t->uin);
      if ((t->xstate<total_xstatuses)&&(t->xstate))
      {
	wsInsertChar(ws3,FIRST_UCS2_BITMAP+t->xstate,1);
      }
#endif
    }
    else
    {
      int onlinetotal[2];
      GetOnTotalContact(t->group,onlinetotal);
#ifdef USE_MLMENU
      wsprintf(ws1,percent_t,t->name);
      wsprintf(ws3,"(%d/%d)",onlinetotal[0],onlinetotal[1]);
#else
      wsprintf(ws1,"%t%c%c(%d/%d)",t->name,0xE01D,0xE012,onlinetotal[0],onlinetotal[1]);
#endif
      if (t->state) icon++; //����������� ������ ������
    }
  }
  else
  {
    wsprintf(ws1, LG_CLERROR);
  }
  ws2=AllocMenuWS(data,ws1->wsbody[0]);
  wstrcpy(ws2,ws1);
  
  SetMenuItemIconArray(data, item, S_ICONS+icon);
#ifdef USE_MLMENU
  ws4=AllocMenuWS(data,ws3->wsbody[0]);
  wstrcpy(ws4,ws3);
  SetMLMenuItemText(data, item, ws2, ws4, curitem);
#else
  SetMenuItemText(data, item, ws2, curitem);
#endif
}

//===============================================================================================

int DNR_ID=0;
int DNR_TRIES=3;

extern const char NATICQ_HOST[];
extern const unsigned int NATICQ_PORT;
extern const unsigned int RECONNECT_TIME; //2058
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
  //������������� ����������
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
  strcpy(hostbuf2, hostbuf); //vyvod host v get_online_time
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
       return; //���� ���������� DNR
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
	//�� ������� �������� ������, ��������� GPRS-������
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
    j=sizeof(PKT)+p->pkt.data_len; //������ ������
    TOTALSENDED+=j;
    ALLTOTALSENDED+=j;			//by BoBa 10.07    
    //���������, �� ���� �� �������� � �������
    if (sendq_p)
    {
      //���� �������, ��������� � ���
      sendq_p=realloc((void *)sendq_p,sendq_l+j);
      memcpy((char *)sendq_p+sendq_l,p,j);
      mfree(p);
      sendq_l+=j;
      return;
    }
    sendq_p=p;
    sendq_l=j;
  }
  //���������� ��� ������������ � �������
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
	//�������� ��� ������
	strcpy(logmsg,"Send delayed...");
	return; //������, ���� ����� ��������� ENIP_BUFFER_FREE
      }
      else
      {
	//������
	LockSched();
	ShowMSG(1,(int)"Send error!");
	UnlockSched();
	end_socket();
	return;
      }
    }
    memcpy((void *)sendq_p,(char *)sendq_p+j,sendq_l-=j); //������� ����������
    if (j<i)
    {
      //�������� ������ ��� ����������
#ifdef SEND_TIMER
      GBS_StartTimerProc(&send_tmr,216*5,resend);
#endif
      return; //���� ��������� ENIP_BUFFER_FREE1
    }
    tenseconds_to_ping=0; //����-�� �������, ����� �������� ������ ������� �� ����� ������
  }
  mfree((void *)sendq_p);
  sendq_p=NULL;
}

extern const char pingsnd[64];

void send_login(int dummy, TPKT *p)
{
  connect_state=2;
  char rev[16];
  //��� ����� ������ � ���� ����� ������������� �������, ���� ������ �� ������ �������!!!
  //� ���� ����� ������ �� ������!
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

extern int main2_count;
extern const unsigned int ping_time_config;
int ping_time;
//#ifdef EVILFOX
//#else
extern const char ignor_list[256];
//#endif
char I_UIN[9];

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
      //��������� ���������
      n=-i; //��������� ���������� ����
      if (j<n) n=j; //����������<���������?
      memcpy(RXbuf.data+i,rp,n); //��������
      i+=n;
      j-=n;
      rp+=n;
    }
    if (i>=0)
    {
      //��������� ������ ;)
      n=RXbuf.pkt.data_len; //����� � ������
      if (n>16383)
      {
	//������� �����
	strcpy(logmsg,LG_GRBADPACKET);
	end_socket();
	RXstate=EOP;
	return;
      }
      n-=i; //���������� ��������� ���� (����� ������ ������-������� �������)
      if (n>0)
      {
	if (j<n) n=j; //����������<���������?
	memcpy(RXbuf.data+i,rp,n);
	i+=n;
	j-=n;
	rp+=n;
      }
      if (RXbuf.pkt.data_len==i)
      {
	//����� ��������� �������
	TOTALRECEIVED+=(i+8);
	ALLTOTALRECEIVED+=(i+8);			//by BoBa 10.07	
	//����� ������ ������, ����� ���������...
	RXbuf.data[i]=0; //����� ������
	switch(RXbuf.pkt.type)
	{
	case T_LOGIN:
	  //������ ������������
	  //�������� � MMI
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
	  //�������� � MMI
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
#ifdef EVILFOX
#else
          sprintf(I_UIN, "uin%d", RXbuf.pkt.uin);
          if(strstr(ignor_list,I_UIN)==0)
          {
#endif
	  snprintf(logmsg,255,LG_GRRECVMSG,RXbuf.pkt.uin,RXbuf.data);
	  SendMSGACK(TOTALRECEIVED);
	  GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,0,p,sock);
	  SMART_REDRAW();
#ifdef EVILFOX
          if(!extended_sounds)
          {
    	    Play(sndMsg);
          }
          else
          {
            uins=RXbuf.pkt.uin;
            volatile int hFile_ex;
            unsigned int io_error_ex = 0;
            sprintf(talk_sound, "%s%d.wav", sndMsg2, uins);
            hFile_ex = fopen(ext_sound,A_ReadOnly + A_BIN,P_READ, &io_error_ex);
            if(hFile_ex!=-1)
            {
              fclose(hFile_ex, &io_error_ex);
              Play(talk_sound);
            }
            else
              Play(sndMsg);
          }
#else
          if(!extended_sounds)
          {
            Play(sndMsg);
          }
          else
          {
            uins=RXbuf.pkt.uin;
            volatile int hFile_ex;
            unsigned int io_error_ex = 0;
            sprintf(talk_sound, "%s%d.wav", sndMsg2, uins);
            hFile_ex = fopen(talk_sound,A_ReadOnly + A_BIN,P_READ, &io_error_ex);
            if(hFile_ex!=-1)
            {
              fclose(hFile_ex, &io_error_ex);
              Play(talk_sound);
            }
            else
              Play(sndMsg);
          }
#endif
#ifdef EVILFOX
#else
          }
#endif
	  break;
	case T_SSLRESP:
	  LockSched();
	  ShowMSG(1,(int)RXbuf.data);
	  UnlockSched();
	  break;
	case T_SRV_ACK:
	  if (FindContactLOGQByAck(&RXbuf)) Play(sndMsgSent);
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
	    snprintf(logmsg,255,"Ping %d-%d seconds!",s1,s1+1);
            sprintf(PINGO, "Ping: %dsec",s1);
	    LockSched();
            if(janis==1)
            {
	    ShowMSG(1,(int)logmsg);
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
	i=-(int)sizeof(PKT); //� ����� ��� ���� ������
      }
    }
  }
  RXstate=i;
  //  GBS_StartTimerProc(&tmr_dorecv,3000,dorecv);
  //  SMART_REDRAW();
}


extern int main_count;

//=====================add to main info====================

//���� � �� ���...kluchnik

char a_txt[45];
char s_txt[45];
char d_txt[45];
char f_txt[45];
char g_txt[45];
//char de[64];
//char du[38];
extern const unsigned int msg_d_x;
extern const unsigned int msg_d_y;
extern const unsigned int main_lenght;
extern const int use_cutter;


void AddToMain(const char *nameu,char *si)
{
  char *de=malloc(64);
  int text_l;
  int iz;
  int name_l;
  name_l=0;
  text_l=0;
  name_l=strlen(nameu);
//  char *du=malloc(64);a_txt
  iz=0;
#ifdef ELKA
  text_l=main_lenght-name_l;
#else
  text_l=main_lenght-name_l;
#endif
  
  if(strlen(si)>text_l)
  {
    /*
    char *du=malloc(text_l);
    strncpy(du,si,text_l);
    sprintf(de,"%s%s",du,"...");
    mfree(du);
  }
    */
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
//    if(d[i]==0x2A) d[i]=0x20;
//    if(d[i]==0x27) d[i]=0x20;
//    if(d[i]==0x22) d[i]=0x20;
//    if(d[i]==0x26) d[i]=0x20;
//    if(d[i]==0x3D) d[i]=0x20;
//    if(d[i]==0x3C) d[i]=0x20;
//    if(d[i]==0x3E) d[i]=0x20;
//    if(d[i]==0x7B) d[i]=0x20;
//    if(d[i]==0x7D) d[i]=0x20;
//    if(d[i]==0x5B) d[i]=0x20;
//    if(d[i]==0x5D) d[i]=0x20;
//    if(d[i]==0x23) d[i]=0x20;
//    if(d[i]==0x2F) d[i]=0x20;
//    if(d[i]==0x5C) d[i]=0x20;
//    if(d[i]==0x7E) d[i]=0x20;
//    if(d[i]==0x5E) d[i]=0x20;
//   if(d[i]==0xAC) d[i]=0x20;
//    if(d[i]==0xA3) d[i]=0x20;
//    if(d[i]==0x24) d[i]=0x20;
//    if(d[i]==0xA5) d[i]=0x20;
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
//int LOG_TIME_CONN=0;//not log online/offline time

void AddStringToLog(CLIST *t, int code, char *s, const char *name, unsigned int IDforACK,int flag)
{
  char hs[128], *lastX;
  TTime tt;
  TDate d;
  GetDateTime(&d,&tt);
  int i;
  
//  if(code == 3 && !LOG_XTXT) return; //�l ��c�� n�odr�?n� cennnrn�n
  if ( code==3 && NOT_LOG_SAME_XTXT )
  {
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    lastX = GetLastXTextLOGQ(t);
    if(lastX)
      if(strcmp(lastX, s) == 0) return;
  }
  
  snprintf(hs,127,"%02d:%02d %02d-%02d %s:\r\n",tt.hour,tt.min,d.day,d.month,name);
  if(code != 3 || ((LOG_XTXT) && (flag)))//����� ��������� ���������
   Add2History(t, hs, s, code); 

//  ShowMSG(1,(int)t);
//  ShowMSG(1,(int)s);  // ������ �������
  LOGQ *p=NewLOGQ(s);
  snprintf(p->hdr,79,"%02d:%02d %02d-%02d %s:",tt.hour,tt.min,d.day,d.month,name);
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
  if(saver_on) reftes(); 
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
//���������� ������ � ��� ��� ��������� ������ ���������
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
	  color=ACK_COLOR; //�������
	else
	  color=I_COLOR;
//#endif
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
    j=((ed_struct->ed_contact->msg_count+1)*2); //��������� ed_answer
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

//������ ������������� ���������������
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
  j-=(ed_struct->ed_contact->msg_count*2)+1; //����� ���������� ������ �������� �����
  while(p)
  {
    if (j>=1) //���� �������� ������
    {
      if (!(p->acked&4))
      {
	ExtractEditControl(data,j,&ec);
	PrepareEditCOptions(&ec_options);
	if (p->acked)
	{
	  if (p->acked==1)
	    color=ACK_COLOR; //�������
	  else
	    color=I_COLOR;
	}
	else
	{
	  if (p->ID==0xFFFFFFFF)
	    color=(p->type&0x10)?(((p->type&0x0F)==1)?O_I_COLOR:O_TO_COLOR):(((p->type&0x0F)==1)?I_COLOR:TO_COLOR);
	  else
	    color=UNACK_COLOR; //�����
	}
	SetPenColorToEditCOptions(&ec_options,color/*p->type==1?I_COLOR:TO_COLOR*/);
	SetFontToEditCOptions(&ec_options,(p->type&0x10)?O_ED_H_FONT_SIZE:ED_H_FONT_SIZE);
	CopyOptionsToEditControl(&ec,&ec_options);
	StoreEditControl(data,j,&ec);
	p->acked|=4; //����������
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
  p->pkt.uin=0;               // ������; ���� ����� ��������������� �� �������
  p->pkt.type=T_MY_STATUS_CH; // ��� ������: ��������� �������
  p->pkt.data_len=1;          // ����� ������: 1 ����
  p->data[0]=CurrentStatus;
  SUBPROC((void *)SendAnswer,0,p);
}

extern int Send0;

void set_my_xstatus(void)
{
#ifdef NEWSGOLD
  if(Is_Id3tag_Enabled)
  {
  if(CurrentXStatus!=11)
  {
#endif
  TPKT *p;
  char *s1;
  char *s2;
  int l1;
  int l2;
  p=malloc(sizeof(PKT)+1);
  p->pkt.uin=0;               // ������; ���� ����� ��������������� �� �������
  p->pkt.type=T_MY_XSTATUS_CH; // ��� ������: ��������� �������
  p->pkt.data_len=1;          // ����� ������: 1 ����
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
  p->pkt.uin=0;               // ������; ���� ����� ��������������� �� �������
  p->pkt.type=T_MY_XSTATUS_CH; // ��� ������: ��������� �������
  p->pkt.data_len=1;          // ����� ������: 1 ����
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
extern const char playing[13];

void set_my_xstatus2(void)
{
  TPKT *p;
//  char *s1;
//  char *s2;
  int l1;
  int l2;
  p=malloc(sizeof(PKT)+1);
  p->pkt.uin=0;               // ������; ���� ����� ��������������� �� �������
  p->pkt.type=T_MY_XSTATUS_CH; // ��� ������: ��������� �������
  p->pkt.data_len=1;          // ����� ������: 1 ����
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
////=================================comand lists
int comanda;

char requests1[]="Sending online time...";
char requests2[]="Sending msg info...";
char requests3[]="Sending battery info...";
char requests4[]="Sending mod info...";
char requests6[]="Sending offline info...";

extern const int show_send_info;

#ifdef kluchnik
extern const unsigned int AUTOBOT_UIN;
int autobot;
#endif

char mod_info[264];

const char ipc_game_name[]="Combats";
IPC_REQ game_gipc;
#define IPC_GAME_DATA 24

#ifdef kluchnik
void AutobotInit()
{
  if(autobot==1) 
   {
    autobot=0;
    ShowMSG(1,(int)"Autobot Off");
   }
  else 
   {
    autobot=1;
    ShowMSG(1,(int)"Autobot On");
   }
}
#endif


void CheckComand(char *txt, CLIST *t, char *ddd)
{
  comanda=1;
  sprintf(mod_info,"mod by Evilfox,kluchnik,windes based on revision 2058, mod date: %s at %s ",__DATE__,__TIME__);
  
  if(strstr(txt,"GAME:")!=0)
  {
    game_gipc.name_to=ipc_game_name;
    game_gipc.name_from=ipc_my_name;
    game_gipc.data=txt;
    GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_GAME_DATA,&game_gipc);
    comanda=0;
    return;
  }
  
  if(!strcmp(txt, "GET_ONLINE_TIME"))
  {
    comanda=0;
    TPKT *p;
    char online[256];
    sprintf(online,"����� � ������: %02d:%02d:%02d \n"
            "������: %s", xstatus_old, minimum_xstatus, maximum, hostbuf2);
    p=malloc(sizeof(PKT)+strlen(online)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(online);
    strcpy(p->data,online);
#ifdef EVILFOX
    SENDMSGCOUNT++;
#else
    if(show_send_info)
      AddStringToLog(t,0x01,requests1,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
#endif
    SUBPROC((void *)SendAnswer,0,p);
    return;
  }
  
  if(!strcmp(txt, "GET_MSG_INFO"))
  {
    comanda=0;
    TPKT *p;
    char *send_txt=malloc(64);
    sprintf(send_txt, "���������� ���������: %d \n"
                      "������� ���������: %d"
                    , main2_count, main_count);
    p=malloc(sizeof(PKT)+strlen(send_txt)+1);
    p->pkt.uin=t->uin;
    p->pkt.type=T_SENDMSG;
    p->pkt.data_len=strlen(send_txt);
    strcpy(p->data,send_txt);
#ifdef EVILFOX
    SENDMSGCOUNT++;
#else
    if(show_send_info)
      AddStringToLog(t,0x01,requests2,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
    else
      SENDMSGCOUNT++;
#endif
    SUBPROC((void *)SendAnswer,0,p);
    mfree(send_txt);
    return;
  }

//==========================windes commands begin =====================

  if(!strcmp(txt, "GET_PROVIDER"))
  {
    comanda=0;
    TPKT *p;
    
    WSHDR *ws=AllocWS(32);
    GetProvider(ws,0);
    char prov[16];
    ws_2str(ws,prov,wstrlen(ws));
    char *send_txt=malloc(32);
    sprintf(send_txt, "��������: %s", prov);
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
  
  if(!strcmp(txt, "GET_MOD_INFO"))
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
  
  if(!strcmp(txt, "GET_BAT_INFO"))
  {
    comanda=0;
    TPKT *p;
    int bat;
    bat=*RamCap();
    char *send_txt=malloc(64);
    sprintf(send_txt, "����� �������: %d ���������", bat);
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
  
  if(away_m==1)
  {
    comanda=1;
    TPKT *p;
//    sprintf(hah,"online: %02d:%02d:%02d", xstatus_old, minimum_xstatus, maximum);
    
    if(inp_away_msg2)//���� ����� ����� �� ������� ���,����� �� �������
      sprintf(away_msg, "������������: %s %s! %s %s", awayHelloMsg, ddd, inp_away_msg2, away_msg_time);
    else
      sprintf(away_msg, "������������: %s %s! %s %s", awayHelloMsg, ddd, awayMsg, away_msg_time);
      p=malloc(sizeof(PKT)+strlen(away_msg)+1);
      p->pkt.uin=t->uin;
      p->pkt.type=T_SENDMSG;
      p->pkt.data_len=strlen(away_msg);
      strcpy(p->data,away_msg);
#ifdef EVILFOX
      SENDMSGCOUNT++;
#else
      if(show_send_info)
      AddStringToLog(t,0x01,requests6,I_str,(++SENDMSGCOUNT)&0x7FFF,1);
      else
        SENDMSGCOUNT++;
#endif
      SUBPROC((void *)SendAnswer,0,p);
  }
#ifdef klcuhnik
  if((t->uin==AUTOBOT_UIN)&&(autobot))
  {
    TPKT *p;
    if(t->bot_msg==0) sprintf(away_msg, "������");
    if(t->bot_msg==1) sprintf(away_msg, "�������?!");
    if(t->bot_msg==2) sprintf(away_msg, "�� ��� �� ��������");
    if(t->bot_msg==3) sprintf(away_msg, "�������");
    if(t->bot_msg==4) sprintf(away_msg, "�� ��� �������?");
    if(t->bot_msg==5) sprintf(away_msg, "����� ����");
    if(t->bot_msg==6) sprintf(away_msg, "�� ������");
    if(t->bot_msg==7) sprintf(away_msg, "�������");
    if(t->bot_msg==8) sprintf(away_msg, "������ ���");
    t->bot_msg++;
    if(t->bot_msg==9)
      t->bot_msg=1;
 //     strcpy(away_msg, c);
      p=malloc(sizeof(PKT)+strlen(away_msg)+1);
      p->pkt.uin=t->uin;
      p->pkt.type=T_SENDMSG;
      p->pkt.data_len=strlen(away_msg);
      strcpy(p->data,away_msg);
      AddStringToLog(t,0x01,away_msg,I_str,(++SENDMSGCOUNT)&0x7FFF);
      SUBPROC((void *)SendAnswer,0,p);

  extern const int autobot_kel;
  if((t->uin==334492880)&&(autobot_kel))
  {
    TPKT *p;
    //if(t->bot_msg==0) sprintf(away_msg, "������");
    if(t->bot_msg==0) sprintf(away_msg, "�������?!");
    if(t->bot_msg==1) sprintf(away_msg, "�� ��� �� ��������");
    if(t->bot_msg==2) sprintf(away_msg, "�������");
    if(t->bot_msg==3) sprintf(away_msg, "���");
    if(t->bot_msg==4) sprintf(away_msg, "�� ��� ��� ����!");
    if(t->bot_msg==5) sprintf(away_msg, "��� �� ���!!");
    if(t->bot_msg==6) sprintf(away_msg, "����");
    if(t->bot_msg==7) sprintf(away_msg, "������");
    if(t->bot_msg==8) sprintf(away_msg, "�������� c���!!!*FUCK_YOU*");
    t->bot_msg++;
    if(t->bot_msg==9)
      t->bot_msg=1;
 //     strcpy(away_msg, c);
      p=malloc(sizeof(PKT)+strlen(away_msg)+1);
      p->pkt.uin=t->uin;
      p->pkt.type=T_SENDMSG;
      p->pkt.data_len=strlen(away_msg);
      strcpy(p->data,away_msg);
      AddStringToLog(t,0x01,away_msg,I_str,(++SENDMSGCOUNT)&0x7FFF);
      SUBPROC((void *)SendAnswer,0,p);
   }
  }
#endif
  //----------------------//
  
  if(!strcmp(txt, "GET_URL_INFO"))// kluchnik 26.03.08
  {
    comanda=0;
    TPKT *p;
    
    char *send_txt=malloc(786);
    sprintf(send_txt,"revision 2058, mod date: %s at %s ",
                     "http://windes.narod.ru/files/naticq_mod2058s.zip (sgold)    (40kb)\n"
                     "http://windes.narod.ru/files/naticq_mod2058n.zip (NewSGold) (40kb)\n"
                     "http://windes.narod.ru/files/naticq_mod2058e.zip (elka)     (40kb)\n"
                     "files: http://windes.narod.ru/files/naticqmod_allfiles_w.zip (432kb)\n"
                     "Status on iconbar http://windes.narod.ru/files/img16x16.zip (74kb)\n"
                     "��� �� ��� ����� ��� http://kluchnik.jino-net.ru/naticqmod/",__DATE__,__TIME__);
    
    p=malloc(sizeof(PKT)+786);
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
}
//===========================komand list end

char INC_Nick[32];
char show_nick[32];
int INC_show;
#ifdef ELKA
int font =9;
#else
int font= 4;
#endif
const char color[4]={255,255,255,100};

void show_pp_sender()
{
  if(INC_show>0)
  {
    WSHDR *ws=AllocWS(256);
    ascii2ws(ws,show_nick);
    int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
    char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
    if(!(memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8)))
    {
     extern void DrawScreenShot();
     DrawScreenShot();
    }
    DrawRectangle(0,0,Get_WS_width(ws,font)+2,GetFontYSIZE(font)+2,0,GetPaletteAdrByColorIndex(1),GetPaletteAdrByColorIndex(1));
    DrawString(ws,1,1,1+Get_WS_width(ws,font)+2,1+GetFontYSIZE(font),font,0,( char*) color,GetPaletteAdrByColorIndex(23));
    
    FreeWS(ws);
    INC_show--;
    GBS_StartTimerProc(&develop, 10, show_pp_sender);
  }
}

extern const int SHOW_ACTIVE;

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

ProcessPacket(TPKT *p)
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
      if (contactlist_menu_id)
      {
	RecountMenu(NULL,1);
      }
      else
      {
        create_contactlist_menu();
      //  if(!dontLogOnConnect && isLogOnline)  dontLogOnConnect=1;
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
      CLIST *oldt=NULL;
      if (t->xstate!=p->data[2])  // ���� �������� ��� ������� ���������
      {
        t->xstate=p->data[2];
        FreeXText(t);
        if (t->xstate && ENA_AUTO_XTXT)   // ���� ���������� ��� ������ � ����� ����������� �� ����� ��� ������� :)
        {
          t->req_xtext=1;
          if (edchat_id)   // ���� ������ ���
          {
            void *data=FindGUIbyId(edchat_id,NULL);
            if (data)
            {
              EDCHAT_STRUCT *ed_struct=EDIT_GetUserPointer(data);
              if (ed_struct)
              {
                if (ed_struct->ed_contact==t)   // ���� ��� ��� �� ����������� ����� �������
                {
                  t->req_xtext=0;
                  RequestXText(t->uin);
                }
              }
	    }
	  }
	}
        else  // ���� ��� ������ �� ���������� �� �� ������ ������ ������� ���� ��������� ������
        {
          t->req_xtext=0;
        }
      }
      if (contactlist_menu_id)
      {
	oldt=FindContactByN(GetCurMenuItem(FindGUIbyId(contactlist_menu_id,NULL)));
      }
      if(t->state==65535) t->change=1;
      t->state=*((unsigned short *)(p->data));
      if((t->state!=65535)&&(t->change==1))
      {
        t->just_now=2;
        t->change=0;
      }
      LogStatusChange(t);
      ChangeContactPos(t);
      RecountMenu(oldt,1);
      if ((t->state!=0xFFFF)&&(i==0xFFFF))//����
      {
#ifdef EVILFOX
        if(!extended_sounds)
        {
      	  Play(sndGlobal);
          
        }
        else
        {
          uins=t->uin;
          sprintf(talk_sound, "%s%d.wav", sndGlobal2, uins);
          Play(talk_sound);
          
         }
#else
        /*
#ifdef kluchnik
        void SetIconBarHandler();
        if(t->uin==438532198)
        {
          solnce=1;
          SetIconBarHandler();
        }
#endif*/
        if(!extended_sounds)
        {
    	  Play(sndGlobal);
          if(isLogOnline) //by kluchnik 26.03.08
          {
            FreeXText2(t);
            TTime time;
            TDate date;
            GetDateTime(&date,&time);
            char *txt=malloc(64);
            sprintf(txt," Online: %02d:%02d:%02d %02d-%02d",time.hour,time.min,time.sec,date.month,date.day);
        
            t->xtext2=malloc(64);
            snprintf(t->xtext2,64,txt);
            AddStringToLog(t,0x03,txt,x_status_change,0xFFFFFFFF, 0);
            ReqAddMsgToChat(t);  
           // RecountMenu(t, 1);
            RecountMenu(NULL, 0);
            mfree(txt);
          }
        }
        else
        {
          FSTATS stat;
          int fsize;
          unsigned int ul = 0;
          uins=t->uin;
          sprintf(talk_sound, "%s%d.wav", sndGlobal2, uins);
          GetFileStats(talk_sound,&stat,&ul);
          fsize=stat.size;
          if(fsize>0)
            Play(talk_sound);
          else
            Play(sndGlobal);
          if(isLogOnline )  //by kluchnik 26.03.08
          {
            FreeXText2(t);
            TTime time;
            TDate date;
            GetDateTime(&date,&time);
            char *txt=malloc(64);
            sprintf(txt," Online: %02d:%02d:%02d %02d-%02d",time.hour,time.min,time.sec,date.month,date.day);
        
            t->xtext2=malloc(64);
            snprintf(t->xtext2,64,txt);
            AddStringToLog(t,0x03,txt,x_status_change,0xFFFFFFFF, 0);
            ReqAddMsgToChat(t);  
           // RecountMenu(t, 1);
            RecountMenu(NULL, 0);
            mfree(txt);
          }
        }
#endif
      }
      
      if ((i!=0xFFFF)&&(t->state==0xFFFF))//����
      {
	Play(sndSrvMsg);
        
//-------------------- Log offline ----------------------//
        
        if(t->isactive && isLogOffline) // by kluchnik 26.03.08
        {
            FreeXText2(t); 
            TTime time;
            TDate date;
            GetDateTime(&date,&time);
            char *txt=malloc(64);
            sprintf(txt," Online: %02d:%02d:%02d %02d-%02d",time.hour,time.min,time.sec,date.month,date.day);
        
            t->xtext2=malloc(64);
            snprintf(t->xtext2,64,txt);
            AddStringToLog(t,0x03,txt,x_status_change,0xFFFFFFFF, 0);
            ReqAddMsgToChat(t);  
           // RecountMenu(t, 1);
            RecountMenu(NULL, 0);
            mfree(txt);
        }
      }
      
//------------------------------------------------------------//

    if(TIME_OR_MSG==2)
       Get_Online_total();
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
    break;
  case T_RECVMSG:
    NEW_MSG=1;
    t=FindContactByUin(p->pkt.uin);
    if (!t)
    {
      sprintf(s,percent_d,p->pkt.uin);
      t=AddContact(p->pkt.uin,s,GROUP_CACHE,0);
    }
    if(!t->isactive && HISTORY_BUFFER) GetHistory(t, 64<<HISTORY_BUFFER);
    if(SHOW_ACTIVE)
    t->isactive=ACTIVE_TIME;
    //    ChangeContactPos(t);
    if(VIBR_TYPE)
      vibra_count=2;
    else
      vibra_count=1;
    start_vibra();
#ifdef ELKA
    if(SLI_TYPE)
      SLI_Timer();
#endif
        if(Is_Double_Vibra_On)
          Reminder_timer();
      if (Is_ILU_On)
    IlluminationOn(ILL_DISP_RECV,ILL_KEYS_RECV,ILL_RECV_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
    if (t->name[0]=='#')
    {
      //���� ��� �����������, ������ ���
      char *s=strchr(p->data,'>');
      //���� ����� ������ > � ����� ���� ������ � ��� ������ 16 ��������
      if (s)
      {
	if ((s[1]==' ')&&((s-p->data)<16))
	{
	  *s=0; //����� ������
	  AddStringToLog(t,0x02,s+2,p->data,0xFFFFFFFF,1); //��������� ��� �� ������ ���������
	  goto L1;
	}
      }
    }
    
//=================== tas bus jautri ===================//

    comanda=1;
//    if(UIN!=t->uin)
    CheckComand(p->data, t, t->name);
    if(comanda==1)
    {
      AddStringToLog(t,0x02,p->data,t->name,0xFFFFFFFF,1);
      main_count++;
      if(Is_INC_On)
      {     
        strcpy(show_nick, t->name);
        if(!IsIdleUiOnTop()&&(!saver_on)) 
        {
          //int csm_oncreate_addr = ((int*)(((CSM_RAM*)(CSM_root()->csm_q->csm.last))->constr))[1] - 1;
        //  char java_oncreate_pattern[8] = {0xB0, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x05, 0x61};
        //  if (memcmp((void*)csm_oncreate_addr, java_oncreate_pattern, 8))
        //  {
            INC_show=15;
            show_pp_sender();
          //}
        }
      }
      if(Is_SMARTCPU_On)
      AddToMain(t->name,p->data);
    }
//    if((Is_SMARTCPU_On)&&(comanda==1))
//    AddToMain(t->name,p->data);
    L1:
    //������������� ������, � ������� ������ ���������
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
//    if(comanda==1)
//    {
    ReqAddMsgToChat(t);
    RecountMenu(t,1);
    extern const int DEVELOP_IF;
    switch (DEVELOP_IF)
    {
    case 0:
#ifdef EVILFOX
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)) GBS_StartTimerProc(&develop, 216*2, to_develop);
#else
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)) to_develop();
#endif
      break;
    case 1:
#ifdef EVILFOX
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)&&(IsUnlocked())) GBS_StartTimerProc(&develop, 216*2, to_develop);
#else
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)&&(IsUnlocked())) to_develop();
#endif
      break;
    case 2:
      break;
    case 3:
      GBS_DelTimer(&develop);
      if ((((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=maincsm_id)&&(!IsUnlocked())) begin_to_develop();
      break;
    }
//    }
    break;
  case T_SRV_ACK:
  case T_CLIENT_ACK:
    q=FindContactLOGQByAck(p);
/*    if (q&&(p->pkt.type==T_SRV_ACK))
    {
            if (Is_ILU_On)
              IlluminationOn(ILL_DISP_SEND,ILL_KEYS_SEND,ILL_SEND_TMR,ILL_RECV_FADE); 
    }//Illumination by BoBa 19.04.2007 */
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
#ifdef EVILFOX
		time_to_stop_t9=2;
#else
                time_to_stop_t9=3;
#endif
	      }
	      else {
		DrawAck(data);
                if (p->pkt.type==T_SRV_ACK)
                 IlluminationOn(ILL_DISP_SEND,ILL_KEYS_SEND,ILL_SEND_TMR,ILL_RECV_FADE); //Illumination by BoBa 19.04.2007
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
       //if (!edchat_id &&          // ������ ���� �������� �� �������
         // strlen(p->data))       // � x-status �� ������
         // ShowMSG(0,(int)s);

      /*
#ifdef EVILFOX
#else
      if (!edchat_id) ShowMSG(0,(int)s);   // ������ ���� �������� �� �������
#endif  
      */
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
      if (strlen(p->data))       // ���� x-status �� ������
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

void method0(MAIN_GUI *data)
{
  int scr_w=ScreenW();
  int scr_h=ScreenH();
  int pos_status;//2026rev
  int pm = pictures_max, pl = pictures_loaded;//2026rev
/*  DrawRoundedFrame(0,YDISP,scr_w-1,scr_h-1,0,0,0,
		   GetPaletteAdrByColorIndex(0),
		   GetPaletteAdrByColorIndex(20));*/

  DrawRectangle(0,YDISP,scr_w-1,scr_h-1,0,
		   GetPaletteAdrByColorIndex(1),
		   GetPaletteAdrByColorIndex(1));

  DrawImg(0,0,S_ICONS[ICON_LOGO]);
  unsigned long RX=ALLTOTALRECEIVED; unsigned long TX=ALLTOTALSENDED;			//by BoBa 10.07
  wsprintf(data->ws1,LG_GRSTATESTRING,connect_state,RXstate,RX,TX,sendq_l,hostname,logmsg);
  if(pm != pl)
  {
     DrawRectangle(0,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD),scr_w-1,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-2,0,
                     GetPaletteAdrByColorIndex(0),
                     GetPaletteAdrByColorIndex(0));
    pos_status = ((scr_w-1) * pl) / pm;
    DrawRectangle(1,scr_h-4-2*GetFontYSIZE(FONT_SMALL_BOLD)+1,pos_status,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD)-3,0,
                     GetPaletteAdrByColorIndex(14),
                     GetPaletteAdrByColorIndex(14));  
//    wstrcatprintf(data->ws1,"\nLoading images...");
   
    if(total_smiles)
    {
      wstrcatprintf(data->ws1,"\nLoaded %d smiles",total_smiles);
    }
    if (xstatuses_load)
    {
      wstrcatprintf(data->ws1,"\nLoaded %d xstatus",total_xstatuses);
    }
  }
#ifdef kluchnik
  
   DrawString(data->ws1,3,3+YDISP,scr_w-4,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     FONT_SMALL,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  wsprintf(data->ws2,percent_t,LG_GRSKEYEXIT);
  DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     scr_w-4,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNRIGHT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  wsprintf(data->ws2,percent_t,cltop?LG_GRSKEYCLIST:empty_str);
  DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     scr_w>>1,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
#else
  
  DrawString(data->ws1,3,3+YDISP,scr_w-4,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     FONT_SMALL,0,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  wsprintf(data->ws2,percent_t,cltop?LG_GRSKEYCLIST:empty_str);
  DrawString(data->ws2,(scr_w >> 1),scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     scr_w-4,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNRIGHT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
  wsprintf(data->ws2,percent_t,LG_GRSKEYEXIT);
  DrawString(data->ws2,3,scr_h-4-GetFontYSIZE(FONT_MEDIUM_BOLD),
	     scr_w>>1,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
//  DrawImg(15,200,(int)"4:\\zbin\\naticq\\img\\smiles_logo.png");
  wsprintf(data->ws2,percent_t,"Mod Evilfox(c), kluchnik, windes");
  DrawString(data->ws2,3,scr_h-GetFontYSIZE(FONT_MEDIUM_BOLD)-GetFontYSIZE(FONT_MEDIUM_BOLD)-40,
	     scr_w-4,scr_h-4,FONT_MEDIUM_BOLD,TEXT_ALIGNLEFT,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(23));
#endif
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

int method5(MAIN_GUI *data,GUI_MSG *msg)
{
  DirectRedrawGUI();
  if (msg->gbsmsg->msg==KEY_DOWN)
  {
    switch(msg->gbsmsg->submess)
    {
#ifdef kluchnik
      case LEFT_SOFT:
      if (cltop) create_contactlist_menu();
      //      if (cltop) remake_clmenu();
      break;
    case RIGHT_SOFT:
      return(1); //���������� ����� GeneralFunc ��� ���. GUI -> �������� GUI
#else
     case LEFT_SOFT:
      return(1); //���������� ����� GeneralFunc ��� ���. GUI -> �������� GUI
     case RIGHT_SOFT:
      //      if (cltop) remake_clmenu();
     if (cltop) create_contactlist_menu();
     break;
#endif
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
      SUBPROC((void*)end_socket);
      GBS_DelTimer(&reconnect_tmr);
      DNR_TRIES=3;
      SUBPROC((void *)create_connect);
      break;
    }
  }
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

extern const int Reset_timer;
int my_keyhook(int key, int m)
{
  if (m==KEY_DOWN)
       {
         if(change_back_status==1)
         {
           CurrentStatus=status_rem;
           set_my_status();
//           SUBPROC((void*)WriteDefSettings);
           change_back_status=0;
         }
#ifdef EVILFOX
         if ((key==VOL_UP_BUTTON)&&(IsIdleUiOnTop()))
         {
           if(autobot==1) 
           {
             autobot=0;
             ShowMSG(1,(int)"Autobot Off");
           }
           else 
           {
             autobot=1;
             ShowMSG(1,(int)"Autobot On");
           }
         }
#endif
#ifdef ELKA
#ifdef EVILFOX
         if ((key==VOL_DOWN_BUTTON)&&(IsIdleUiOnTop()))
#else
         if ((key==CAMERA_BUTTON)&&(!IsUnlocked()))
#endif
         {
           GBS_StopTimer(&away_tmr);
           msg_away_timer=AUTO_ENGADE;
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
           }
#ifdef ELKA
         }
#endif
       }
  return 0;
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
  //  MutexCreate(&contactlist_mtx);
  DNR_TRIES=3;
  //  SUBPROC((void *)InitSmiles);
  //  SUBPROC((void *)create_connect);
  GBS_StartTimerProc(&tmr_active,TMR_SECOND*10,process_active_timer);
  sprintf((char *)ipc_my_name+6,percent_d,UIN);
  gipc.name_to=ipc_my_name;
  gipc.name_from=ipc_my_name;
  gipc.data=(void *)-1;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_CHECK_DOUBLERUN,&gipc);
/*
  InitXStatusesImg();
    //InitSmiles(); ��� ��������� �� InitXStatusesImg
  create_connect();
  silenthide=1;
  gipc.name_to=ipc_xtask_name;
  gipc.name_from=ipc_my_name;
  gipc.data=0;
  GBS_SendMessage(MMI_CEPID,MSG_IPC,IPC_XTASK_IDLE,&gipc);
  */
}

void maincsm_onclose(CSM_RAM *csm)
{
  WriteDefSettings(); 
  //  SaveConfigData(successed_config_filename);
  
/*
  #pragma segment="CONFIG_C"
  unsigned int ul;
  int f;
  extern const CFG_HDR cfghdr0; //first var in CONFIG
  void *cfg=(void*)&cfghdr0;
  unsigned int len=(int)__segment_end("CONFIG_C")-(int)__segment_begin("CONFIG_C");

  if ((f=fopen("4:\\ZBin\\etc\\NATICQ.bcfg",A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))==-1){
   f=fopen("0:\\ZBin\\etc\\NATICQ.bcfg",A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul);
  }
  fwrite(f,cfg,len,&ul);
  fclose(f,&ul);
*/

  //  GBS_DelTimer(&tmr_dorecv);
  RemoveKeybMsgHook((void *)my_keyhook);
  GBS_DelTimer(&tmr_active);
  GBS_DelTimer(&tmr_vibra);
  GBS_DelTimer(&reconnect_tmr);
  GBS_DelTimer(&tmr_illumination);
  GBS_DelTimer(&mytmr);
  GBS_DelTimer(&mytmr2);
  GBS_DelTimer(&mytmr3);
  GBS_DelTimer(&mytmr4);
  GBS_DelTimer(&statusTmr);
#ifdef NEWSGOLD
  GBS_DelTimer(&tracktmr);
#endif
  GBS_DelTimer(&develop);
  GBS_DelTimer(&away_tmr);
  SetVibration(0);
  FreeTemplates();
  FreeCLIST();
  free_ICONS();
//  FreeSmiles();
  FreeWS(ews);
  FreeXStatusText();
  //  MutexDestroy(&contactlist_mtx);
  SUBPROC((void *)FreeSmiles);
  SUBPROC((void *)FreeXStatusesImg);
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
    InitXStatusesImg();
    //InitSmiles(); ��� ��������� �� InitXStatusesImg
    create_connect();
  }
}

#define IPC_GAME 7

typedef struct{
  unsigned long uin;
  char *msg;
}IPCmsg;

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
	    //���� ������� ���� ����������� ���������, ������ ��������� �����
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)CheckDoubleRun);
            else ipc->data=(void *)maincsm_id;
	    break;
	  case IPC_SMILE_PROCESSED:
	    //������ ����������� ������ ;)
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)ProcessNextSmile);
	    SMART_REDRAW();
	    break;
	  case IPC_XSTATUSIMG_PROCESSED:
	    //������ ����������� ���������� ;)
	    if (ipc->name_from==ipc_my_name) SUBPROC((void *)ProcessNextXStatImg);
	    SMART_REDRAW();
	    break;
          case IPC_GAME:
            {

            }
            break;
	  case IPC_TENSECONDS:
	    //������ ���� ���������
	    if (ipc->name_from==ipc_my_name)
	    {
	      CLIST *t=(CLIST *)cltop;
	      int f=0;
	      while(t)
	      {
                if(t->just_now)
                {
                if (!(--(t->just_now))) f=1;
                }
		if (t->isactive)
		{
		  if (!(--(t->isactive))) 
                  {
                    f=1; //���� ����� �� 0 ���� �� ���� ��� - ���� ������������ ����
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
     	  case IPC_SENDMSG: ;                                   //IPC_SENDMSG by BoBa 26.06.07
            int l=strlen(((IPCMsg *)(ipc->data))->msg);
            TPKT *msg=malloc(sizeof(PKT)+l+30);
            msg->pkt.uin=((IPCMsg *)(ipc->data))->uin;
            msg->pkt.type=T_SENDMSG;
            msg->pkt.data_len=l;
            memcpy(msg->data,((IPCMsg *)(ipc->data))->msg,l);
            //slientsend=1;
            if(strstr(msg->data,PASS)==0)
            {
	    SENDMSGCOUNT++; //����� ���������
            SUBPROC((void *)SendAnswer,0,msg);
            }
            else
            {
              msg->pkt.data_len=40;
              strcpy(msg->data, "Fuck you hacker! my password sux ur ass");
              SENDMSGCOUNT++; //����� ���������
              SUBPROC((void *)SendAnswer,0,msg);
            }
            break;
	  }
	}
      }
    }
    //�������� �������� ����� �������
#define idlegui_id (((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4])
    CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
    if (IsGuiOnTop(idlegui_id)/*&&IsUnlocked()*/) //���� IdleGui �� ����� �����
    {
      GUI *igui=GetTopGUI();
      if (igui) //� �� ����������
      {
//#ifdef ELKA
        INC_show=0;
//#endif
	canvasdata=BuildCanvas(); //void * - belyj fon!
	int icn;
       // if(ShowStatusIcon==0 || ShowStatusIcon==3)
       // {
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
        //}
	//��� ����� �������
	// by Rainmaker: ������ ����� ������ ��� ������ � ������� � ����� �����������
// __root const CFG_CBOX_ITEM cfgcbox9_1[5]={"On main screen","Iconbar","Iconbar except idle","All","Nothing"};
        if(ShowStatusIcon==0 || ShowStatusIcon==2 || ShowStatusIcon==3) //kluchnik 26.03.08
        {
          DrawCanvas(canvasdata,IDLEICON_X,IDLEICON_Y,IDLEICON_X+GetImgWidth((int)S_ICONS[icn])-1,
                     IDLEICON_Y+GetImgHeight((int)S_ICONS[icn])-1,1);
          DrawImg(IDLEICON_X,IDLEICON_Y,S_ICONS[icn]);
        }
//    __root const CFG_CBOX_ITEM cfgcbox_8z[4]={"Nothing","On main screen","Iconbar","All"};
        if(show_xstatus==1 || show_xstatus==3) //kluchnik 26.03.08
        {
          PNGTOP_DESC *pltop=PNG_TOP();
          pltop->dyn_pltop=XStatusesImgList;
          DisableIDLETMR();
          DrawCanvas(canvasdata,IDLEICON_XX,IDLEICON_YX,IDLEICON_XX+GetImgWidth((int)XStatusesIconArray[CurrentXStatus])-1,
                     IDLEICON_YX+GetImgHeight((int)XStatusesIconArray[CurrentXStatus])-1,1);
          DrawImg(IDLEICON_XX,IDLEICON_YX,XStatusesIconArray[CurrentXStatus]);
        }
#ifdef EVILFOX
        DrawImg(IDLEICON_X-1,IDLEICON_Y-8,(int)"4:\\zbin\\naticq\\img\\naticq_ms.png");
#endif
        /*
#ifdef EVILFOX
#else
        if(strlen(fontpath)>5)
        {
#endif
        */
      
#ifdef ELKA
                if (total_unread)
          SLI_On();
#endif
      }
    }
    else
    {
      main_d=1;
    }
  }
  
//---------------------------------------------------//
  
  if(msg->msg == MSG_IPC)  //kluchnik 26.03.08
  {
    IPC_REQ *ipc;
    if ((ipc=(IPC_REQ*)msg->data0))
    {
      if (strcmp_nocase(ipc->name_to,"IdleUpd")==0)
      {
        switch (msg->submess)
        {
        case 0:
          {
            if (main_d==1)
                {
                ClearScreen();
                if(Is_SMARTCPU_On)
                {
      #ifdef EVILFOX
                PrintField(msg_d_x,msg_d_y, a_txt, align, fonto, 0);
                PrintField(msg_d_x,msg_d_y+paused_height, s_txt, align, fonto, 0);
                PrintField(msg_d_x,msg_d_y+(2*paused_height), d_txt, align, fonto, 0);
                PrintField(msg_d_x,msg_d_y+(3*paused_height), f_txt, align, fonto, 0);
                PrintField(msg_d_x,msg_d_y+(4*paused_height), g_txt, align, fonto, 0);
      #else
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
      #endif
                }
                main_d=0;
                }
              if(Is_UPTIME_On)
                DrawOnlineTime(canvasdata);
              if(Is_PING_On)
                DrawPing(canvasdata);
              
              if(Auto_Status && showAwayIdle)
                DrawAutoMSG(canvasdata);
              if(Is_Unread_On)
              {
              sprintf(unread_msg_info, "Unread: %d", total_unread2);
      #ifdef EVILFOX
              PrintField(unread_X,unread_Y, unread_msg_info, align, fonto, space);
      #else
              PrintField(unread_X,unread_Y, unread_msg_info, align, fonto, spaco1);
      #endif
              }
          }
        }
      }
    }
  }
        
//-----------------------------------------------------------//
  
  if (msg->msg==MSG_RECONFIGURE_REQ)
  {
    extern const char *successed_config_filename;
    if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
    {
      ShowMSG(1,(int)"NatICQ config updated!");
      InitConfig();
#ifdef NEWSGOLD
      initTags();
#endif
      PrintLibInit(1);
      FontPathInit((char *)fontpath, 1);
      SetFontColor((char*)fontColor,(char*)bgrColor,1);
//#ifdef ELKA
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
      prev_clmenu_itemcount=0;
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
      //���� ��� �����
      if ((((unsigned int)msg->data0)>>28)==0xA)
      {
	//������ �����
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
	  //���������� ������������, �������� ����� login
	  strcpy(logmsg, LG_GRTRYLOGIN);
	  {
	    int i=strlen(PASS);
	    TPKT *p=malloc(sizeof(PKT)+i);
	    p->pkt.uin=UIN;
	    p->pkt.type=T_REQLOGIN;
	    p->pkt.data_len=i;
	    memcpy(p->data,PASS,i);
	    SUBPROC((void *)send_login,0,p);
	  }
	  GROUP_CACHE=0;
	  SENDMSGCOUNT=0; //�������� ������
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
	  //���� �������� send
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
	//������ �� ������� �������
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
	}
	SMART_REDRAW();
	SUBPROC((void *)ClearSendQ);
#ifdef EVILFOX
	if (!disautorecconect) GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*12,do_reconnect);
#else
        if (!disautorecconect) 
        {
          GBS_StartTimerProc(&reconnect_tmr,TMR_SECOND*RECONNECT_TIME,do_reconnect);
          snprintf(logmsg,255,"%s\nReconect after %d second...",logmsg, RECONNECT_TIME); //2058
        }

#endif
	break;
      }
    }
  }
  return(1);
}


//-------------------- ICON IN ICONBAR ------------//

//kluchnik 26.03.08

#pragma swi_number=0x27 
__swi __arm void AddIconToIconBar(int pic, short *num);

extern const unsigned int NumStatusToIconBar;
extern const unsigned int NumXStatusToIconBar;
//extern void SetIconBarHandler();
extern const int ShowStatusIcon;

void addIconBar(short* num)
{
  int icn;
  if(ShowStatusIcon==1 || ShowStatusIcon==3)
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
  else
  {
    if(ShowStatusIcon==2)//���� ���������� � �������� �����,����� ����
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
    if(CurrentXStatus!=0)
     AddIconToIconBar(NumXStatusToIconBar+CurrentXStatus,num);

#ifdef kluchnik
 // if(solnce)
  //{
    CLIST *t;
    t=FindContactByUin(438532198);
    if(t->state!=0xFFFF && (GetIconIndex(t)!=IS_UNKNOWN))
      AddIconToIconBar(NumStatusToIconBar + GetIconIndex(t), num);  
//  }
#endif
}

typedef struct
{
  char check_name[8];
  int addr;
}ICONBAR_H;


const int minus11=-11;

unsigned short maincsm_name_body[140];

struct
{
  CSM_DESC maincsm;
  WSHDR maincsm_name;
  ICONBAR_H iconbar_handler;
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
  },
  {
    "IconBar"
  }
};

void UpdateCSMname(void)
{
  wsprintf((WSHDR *)(&MAINCSM.maincsm_name), "NATICQ: %d",UIN);
}

void SetIconBarHandler()
{
  MAINCSM.iconbar_handler.addr = (int)addIconBar;
}

int main(char *filename)
{
  MAIN_CSM main_csm;
  char *s;
  int len;
  extern const char *successed_config_filename;
  WSHDR *ws;
  sprintf(PINGO, "Ping None");
  InitConfig();
  s=strrchr(filename,'\\');
  len=(s-filename)+1;
  strncpy(elf_path,filename,len);
  elf_path[len]=0;
  if (!UIN)
  {
    LockSched();
    ShowMSG(1,(int)LG_MSGNOUINPASS);
    ws=AllocWS(150);
    str_2ws(ws,successed_config_filename,128);
    ExecuteFile(ws,0,0);
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
  
  PrintLibInit(1);
  FontPathInit((char *)fontpath, 1);
  SetFontColor((char*)fontColor,(char*)bgrColor,1);
  away_timer();
  SetIconBarHandler();
  if(fonto==1)
  {
    char *heght=malloc(64);
    sprintf(heght, "%s2240.png", fontpath);
    paused_height=GetImgHeight((int)heght);
    mfree(heght);
  }
  else
    paused_height=fonto-100;
  GBS_StartTimerProc(&mytmr3, 12000, Pinger2);
  x_zstatus();
#ifdef NEWSGOLD
  initTags();
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
    for (int i=0; i<wslen; i++) // ��������� ����� ����� ������� ������
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
    switch(i)
    {
    case 0:
      ascii2ws(item->ws,LG_MNUEDNEXTACT);
      break;
    case 1:
      ascii2ws(item->ws,LG_MNUEDPREVACT);
      break;
    default:
      i-=2;
      if (i<ed_struct->loaded_templates) ascii2ws(item->ws,templates_lines[i]);
      break;
    }
  }
  if (item->type==1)
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
  const char *valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$-_.+!*'(),%;:@&=/?�������������������������������������Ũ��������������������������";

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
  const char *valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$-_.+!*'(),%;:@&=/?�������������������������������������Ũ��������������������������";
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
#ifdef EVILFOX
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
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
//      WSHDR *ws=AllocWS(strlen(ed_struct->ed_contact->last_msg_buffer)+1);
//      ascii2ws(ws,ed_struct->ed_contact->last_msg_buffer);
      WSHDR *ws=AllocWS(strlen(last_msg_buffer)+1);
      ascii2ws(ws,last_msg_buffer);
      EDIT_SetFocus(ed_struct->ed_chatgui,ed_struct->ed_answer);
      EDIT_SetTextToFocused(ed_struct->ed_chatgui,ws);
      FreeWS(ws);
      RefreshGUI();
    }
  }
#else

  void AddSmile(GUI *data);
  extern const int LongRightAction;
  extern const int LongLeftAction;
  if (msg->gbsmsg->msg==LONG_PRESS)
  {
    if (l==RIGHT_BUTTON)
    {
      if(LongRightAction==2)
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
#ifdef ELKA
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
#endif
      }
#endif
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
              if(SHOW_ACTIVE)
	      t->isactive=ACTIVE_TIME;
	      p=malloc(sizeof(PKT)+len+1);
	      p->pkt.uin=t->uin;
	      p->pkt.type=T_SENDMSG;
	      p->pkt.data_len=len;
	      strcpy(p->data,s);
              /*
#ifdef EVILFOX
              strcpy(t->last_msg_buffer, s);
#else
              */
#ifdef ELKA
              strcpy(last_msg_buffer, s);
#endif
	      AddStringToLog(t,0x01,p->data,I_str,(++SENDMSGCOUNT)&0x7FFF,1); //����� ���������
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
                
        if(IsUrl(ews, pos, link))
         {
           //templates_lines=realloc(templates_lines,(i+1)*sizeof(char *));
//	   templates_lines[i++]=pp;
          snprintf(fn, 255, "%s\\tmp%u.url", TEMPLATES_PATH, GetTempName);
          if ((f=fopen(fn,A_WriteOnly+A_BIN+A_Create+A_Truncate,P_WRITE,&err))!=-1)
          {
            fwrite(f,link,strlen(link),&err);
            fclose(f,&err);
            str_2ws(ews,fn,256);
            extern const int OpenBrowser;
            if(OpenBrowser)
              ExecuteFile(ews,0,0);            
            else
            {
              extern const char BalleteMiniPath[]; //kluchnik 26.03.08
              ExecuteFile(ews,0,(void*)BalleteMiniPath);            
            }
                          
            unlink(fn, &err);
          }
//          int i=ed_struct->loaded_templates=LoadTemplates(ed_struct->ed_contact->uin);
  //        templates_lines=realloc(templates_lines,(i+1)*sizeof(char *));
    //      templates_lines[i++]="Open Url";
      //    EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+2);          
//          LockSched();
//          ShowMSG(1,(int)link);
//          UnlockSched();
          mfree(link);
          return (-1);
        }
        else
        {
          int i=ed_struct->loaded_templates=LoadTemplates(ed_struct->ed_contact->uin);
          EDIT_OpenOptionMenuWithUserItems(data,ed_options_handler,ed_struct,i+2);
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
  //  0x00000001 - ��������� �� ������� ����
  //  0x00000002 - ��������� �� ������
  //  0x00000004 - �������� ���������
  //  0x00000008 - UnderLine
  //  0x00000020 - �� ���������� �����
  //  0x00000200 - bold
  0,
  //  0x00000002 - ReadOnly
  //  0x00000004 - �� ��������� ������
  //  0x40000000 - �������� ������� ����-������
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
    if ((zc=lp->acked&3))
    {
      if (zc==1)
	color=ACK_COLOR; //�������
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
  ConstructEditControl(&ec,3,(FIRST_LETTER)?ECF_DEFAULT_BIG_LETTER:0,ews,1024);

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
#define EC_MNU_MAX 8

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
  
  extern const char HIST_PATH[64];
#ifdef EVILFOX
#else
  extern const int HISTORY_TYPE;
#endif
  CLIST *t;
  WSHDR *ws=AllocWS(256);
  if ((t=ed_struct->ed_contact))
  {
#ifdef EVILFOX
      wsprintf(ws,"%s\\%u\\%s(%d).txt",HIST_PATH,UIN,t->name, t->uin);
#else
#ifdef UIN_WRITE
    if (HISTORY_TYPE)
      wsprintf(ws,"%s\\%u\\%s(%d).txt",HIST_PATH,UIN,t->name, t->uin);
    else
      wsprintf(ws,"%s\\%s(%d).txt",HIST_PATH,t->name, t->uin);
#else
    if (HISTORY_TYPE)
      wsprintf(ws,"%s\\%u\\%u.txt",HIST_PATH,UIN, t->uin);
    else
      wsprintf(ws,"%s\\%u.txt",HIST_PATH, t->uin);
#endif
#endif
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
    }
    else
    {
      to_remove[++remove]=1;
    }
    
    if (ed_struct->ed_answer<=2) to_remove[++remove]=7;
    if (!ed_struct->ed_contact || connect_state!=3)
    {
      to_remove[++remove]=2;
      to_remove[++remove]=3;
      to_remove[++remove]=4;
      to_remove[++remove]=5;
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
