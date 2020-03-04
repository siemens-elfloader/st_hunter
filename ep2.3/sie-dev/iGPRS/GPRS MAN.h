/*
  Типы данных, общие для всех модулей проекта
*/

#ifdef NEWSGOLD
#define DEFAULT_DISK "4"
#else
#define DEFAULT_DISK "0"
#endif

#define WS_MAXLEN 20

#define TMR_SECOND (1300/6)

#define wslen(ws) ws->wsbody[0]

typedef struct
{
  GUI gui;
  WSHDR *ws1;
  WSHDR *ws2;
  int i1;
}MAIN_GUI;

extern const char perc_t[];
extern const char perc_s[];
extern const int minus11;
extern int HookEnable;

extern int CLOSE;

extern int session_num;  // Количество подключений к сети

extern const int ENABLE_MAN;
extern const int ENABLE_NSD;
extern const int START_MES;
extern const int START_SOUND;
extern const int NUMBER_SOUNG;
extern const int NOTIFY_UPDATED;
extern const int NOTIFY_ON_MESSAGE;

extern const int NOTIFY_EX;
extern const TTime TimeFrom;
extern const TTime TimeTo;

extern const char MESSAGE_ON_TEXT[];
extern const int NOTIFY_ON_VIBRA;
extern const int Vibra_ON_Power;
extern const int NOTIFY_ON_SOUND;
extern const int NUMBER_ON_SOUND;
extern const int NOTIFY_OFF_MESSAGE;
extern const char MESSAGE_OFF_TEXT[];
extern const int NOTIFY_OFF_VIBRA;
extern const int Vibra_OFF_Power;
extern const int NOTIFY_OFF_SOUND;
extern const int NUMBER_OFF_SOUND;

extern const int NOTIFY_SEND;
extern const int NOTIFY_SEND_NORM_DISP;
extern const int NOTIFY_SEND_NORM_KEY;
extern const int NOTIFY_SEND_NORM_ILL;
extern const int NOTIFY_SEND_NORM_FADE;
extern const int NOTIFY_SEND_NORM_TIMEOUT;
extern const int NOTIFY_SEND_LOCK_DISP;
extern const int NOTIFY_SEND_LOCK_KEY;
extern const int NOTIFY_SEND_LOCK_ILL;
extern const int NOTIFY_SEND_LOCK_FADE;
extern const int NOTIFY_SEND_LOCK_TIMEOUT;

extern const int NOTIFY_AUTO;
extern const int NOTIFY_INT;

/*extern const int PING_ENABLE;
extern const int PING_TIME;
extern const int NOTIFY_PING;
extern const char PING_MESSAGE[];
extern const char HOST_NAME[];
extern const int HOST_PORT;
extern const int GPRS_RESTART;
extern const int GPRS_AWAY;
extern const int GPRS_LONG;
extern const int WRITE_IP;
extern const char WRITE_FILEPATH[];*/

extern const int reg_enable;
extern const int reg_file;
extern const int reg_create_enable;
extern const char reg_create_file_1[];
extern const char reg_create_file_2[];
extern const char reg_create_file_3[];
extern const char reg_create_file_4[];
extern const char reg_create_file_5[];
extern const char reg_create_file_6[];
extern const char reg_create_file_7[];
extern const char reg_create_file_8[];
extern const char reg_create_file_9[];
extern const char reg_create_file_10[];
extern const int regulation_close_enable;
extern const char regulation_close_file_1[];
extern const char regulation_close_file_2[];
extern const char regulation_close_file_3[];
extern const char regulation_close_file_4[];
extern const char regulation_close_file_5[];
extern const char regulation_close_file_6[];
extern const char regulation_close_file_7[];
extern const char regulation_close_file_8[];
extern const char regulation_close_file_9[];
extern const char regulation_close_file_10[];
extern const int interception_enable;
extern const int interception_buffer;
extern const char interception_FILEPATH[];
extern const int session_time_enable;
extern const int session_time_show;
extern const char session_time_message[];

extern const int IDLE_SES_ENABLE;
extern const int IDLE_SES_ACTIVE;
extern const int IDLE_SES_LOCK;
extern const char IDLE_SES_MES[];
extern const unsigned int IDLE_SES_X;
extern const unsigned int IDLE_SES_Y;
extern const char IDLE_SES_COLORS[];
extern const unsigned int IDLE_SES_FONT;
extern const int IDLE_SES_FRINGING_ENABLE;
extern const char IDLE_SES_FRINGING_COLORS[];

//extern const int trafic_count_enable;
extern const int trafic_size;
extern const int trafic_gold;
extern const int trafic_silver;
//extern const int trafic_start;
extern const int trafic_blur;    
extern const int trafic_all_blur;

extern const char text_valute[];
extern const char text_mb[];
extern const char text_kb[];

extern const int LIMIT_TRAF_ACTIVE;
extern const unsigned int LIMIT_TRAF_KB;
extern const unsigned int LIMIT_TRAF_SEC;
extern const int LIMIT_TRAF_OFF;
extern const int LIMIT_TRAF_PLAY;
extern const unsigned int LIMIT_TRAF_NUM;
extern const int LIMIT_TRAF_VIBRA;
extern const unsigned int LIMIT_TRAF_POWER;
extern const int LIMIT_TRAF_MES;
extern const char LIMIT_TRAF_TEXT[];

extern const int IDLE_TRAF_SES_ENABLE;
extern const int IDLE_TRAF_SES_ACTIVE;
extern const int IDLE_TRAF_SES_LOCK;
extern const char IDLE_TRAF_SES_MES[];
extern const unsigned int IDLE_TRAF_SES_X;
extern const unsigned int IDLE_TRAF_SES_Y;
extern const char IDLE_TRAF_SES_COLORS[];
extern const unsigned int IDLE_TRAF_SES_FONT;
extern const int IDLE_TRAF_SES_FRINGING_ENABLE;
extern const char IDLE_TRAF_SES_FRINGING_COLORS[];
extern const int IDLE_ALL_TRAF_ENABLE;
extern const int IDLE_ALL_TRAF_ACTIVE;
extern const int IDLE_ALL_TRAF_LOCK;
extern const char IDLE_ALL_TRAF_MES[];
extern const unsigned int IDLE_ALL_TRAF_X;
extern const unsigned int IDLE_ALL_TRAF_Y;
extern const char IDLE_ALL_TRAF_COLORS[];
extern const unsigned int IDLE_ALL_TRAF_FONT;
extern const int IDLE_ALL_TRAF_FRINGING_ENABLE;
extern const char IDLE_ALL_TRAF_FRINGING_COLORS[];
extern const int IDLE_TRAF_ENABLE;
extern const int IDLE_TRAF_ACTIVE;
extern const int IDLE_TRAF_LOCK;
extern const char IDLE_TRAF_MES[];
extern const unsigned int IDLE_TRAF_X;
extern const unsigned int IDLE_TRAF_Y;
extern const char IDLE_TRAF_COLORS[];
extern const unsigned int IDLE_TRAF_FONT;
extern const int IDLE_TRAF_FRINGING_ENABLE;
extern const char IDLE_TRAF_FRINGING_COLORS[];
extern const int IDLE_TRAF_ALL_ENABLE;
extern const int IDLE_TRAF_ALL_ACTIVE;
extern const int IDLE_TRAF_ALL_LOCK;
extern const char IDLE_TRAF_ALL_MES[];
extern const unsigned int IDLE_TRAF_ALL_X;
extern const unsigned int IDLE_TRAF_ALL_Y;
extern const char IDLE_TRAF_ALL_COLORS[];
extern const unsigned int IDLE_TRAF_ALL_FONT;
extern const int IDLE_TRAF_ALL_FRINGING_ENABLE;
extern const char IDLE_TRAF_ALL_FRINGING_COLORS[];

extern const int STAT_ENABLE;
extern const int STAT_START_TIME;
extern const int START_END_TIME;
extern const int STAT_TRAF;
extern const int STAT_BLUR_TRAF;
extern const int STAT_MONEY;
extern const int STAT_TIME;
extern const char STAT_FILEPATH[];


extern const int IDLE_TIME_ENABLE;
extern const int IDLE_TIME_ACTIVE;
extern const int IDLE_TIME_LOCK;
extern const char IDLE_TIME_MES[];
extern const unsigned int IDLE_TIME_X;
extern const unsigned int IDLE_TIME_Y;
extern const char IDLE_TIME_COLORS[];
extern const unsigned int IDLE_TIME_FONT;
extern const int IDLE_TIME_FRINGING_ENABLE;
extern const char IDLE_TIME_FRINGING_COLORS[];

extern const int SEE_STAT_ENABLE;
extern const int SEE_STAT_ACTIVE;
extern const int SEE_STAT_LOCK;
extern const int SEE_MODE_KBD;
extern const unsigned int SEE_STAT_BTN;
extern const unsigned int SEE_STAT_BTN2;
extern const char SEE_FOLDER[];
extern const char SES_COLORS[];
extern const unsigned int SES_FONT;
extern const int SES_FRINGING_ENABLE;
extern const char SES_FRINGING_COLORS[];
extern const char SES_GPRS_ON[];
extern const char SES_GPRS_OFF[];
extern const char SES_IT[];
extern const char SES_SR[];
extern const char SES_MAX[];
extern const char SES_NUM[];
extern const char SES_ALL[];
extern const char SES_SES[];
/*extern const char SES_FORM_ST[];
extern const char SES_FORM_KB[];
extern const char SES_FORM_MB[];*/

extern const int ses_ex_key_en;
extern const unsigned int ses_ex_key_off;
extern const unsigned int ses_ex_key_on;
extern const unsigned int ses_ex_key_res;

extern const int png_on_idle;
extern const int png_full;
extern const char PNG_FOLDER[];
extern const int PNG_LEN;

extern const int icon_on_idle;
extern const int png_on_gui;
extern const unsigned int icon_on_idle_X;
extern const unsigned int icon_on_idle_Y;

extern const int UPDATE_TIME;
extern const int ShowMSG_Param;
extern const int canvas_on_idle;
extern const int gprs_start;
extern const int gprs_end;
extern const int cpu_low;
//extern const unsigned int UPD_TIME;

//extern const int low_on_idle;

////////////////////////////////////////////////////////////////////////////////

extern void kill_data(void *p, void (*func_p)(void *));
extern void setup_ICONS(void);
extern void free_ICONS(void);
extern int my_keyhook(int submsg, int msg);
extern void ShowStat(int update);//char *ses_traf, char *all_traf, char *ses_money, char *all_money , char *time, char *maxtime, char *srtime, int num, int update);
//extern char * win2utf8(const char *text);

// для работы с файлами
extern void WriteStr(char * msg, char * fn);
// для работы с сокетом
/*extern void do_reconnect(void);
extern void end_socket(void);
extern void create_connect(void); 
// для вешанья на таймер
extern void TimerPing(void);
extern void timer_proc(void);
extern void TimerCheckPing(void);*/
// обычные функции
extern void StartVibra(void);
extern void StopVibra(void);
/*extern void SEND_PING(void);
extern void READ_PING(void);*/
//#endif

