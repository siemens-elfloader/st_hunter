#ifndef _SCRTOOL_H_
 #define _SCRTOOL_H_ 
 #include <sieapi.h> 
 #define DAEMON  
 #define SCRTOOL_NAME "ScrTool"
 #define UPDATE_STAT 1
 #define TMR_SECOND 216
 #define MAX_IDS 15
 #define MAX_APP 14
 #define MAX_BIR 8
 #define LEN 32
 #define NoName "\xE7\xA9\xBA\xE7\x99\xBD"
 #define ExcuteType "File", "Shortcut", "Address", "Path"
 #define ExcuteLen 4
 #define DateFmt "YYYY/MM/DD","YYYY-MM-DD","YYYY.MM.DD","YYYY MM DD","MM/DD","MM-DD","MM.DD","MM DD","年月日","月日"
 #define TimeFmt "13:00","PM 01:00","01:00","13:00:00","PM 01:00:00","03D00F","03D00F00M"
 
 #ifdef DAEMON
 typedef struct{
  CSM_RAM csm;
 }MAIN_CSM;
 #endif
 
 typedef struct{
  CSM_RAM csm;
  int task_id; 
  int menu_id;
 }MAIN_CSM_GUI;

 typedef struct{  
  RECT rc;
  WSHDR *ws;
  char Pen[4];
  char Brush[4];
  int Size;
  int Type;
  int show;
 }TSCR;
 
 typedef struct{
  WSHDR *ws;  
  char  *Pic;
  char  *File;
  int    Type;
 }TAPP;
 
 extern const int  MENU_ENA;
 extern const int  INFO_ENA;
 extern const int  SHOW_EXT;
 extern const uint EXIT_BTN;
#ifdef DAEMON
 extern const uint CALL_BTN;
#endif
//运行软件
 extern const uint OFFSET;
 extern const uint AUTO_CLOSE;
 extern const char APPTEXT_COLORS[4];
 extern const char APPTEXT_FCOLOR[4];
 extern const uint APPTEXT_FONT;
 extern const char cfgPanBorderCol[4];
 extern const char cfgPanBGCol[4];
 extern const char cfgBookBorderCol[4];
 extern const char cfgBookBGCol[4];
 //没有图标
 extern const char AINO[64];
 //运行内部地址
 extern const int AT04;
 extern const int AT05;
 extern const int AT06;
 extern const int AT07;
 extern const int AT08;
 extern const int AT09;
 extern const int AT10;
 extern const int AT11;
 extern const int AT12;
 extern const int AT13;
 //功能描述
 extern const char AN04[LEN];
 extern const char AN05[LEN];
 extern const char AN06[LEN];
 extern const char AN07[LEN];
 extern const char AN08[LEN];
 extern const char AN09[LEN];
 extern const char AN10[LEN];
 extern const char AN11[LEN];
 extern const char AN12[LEN];
 extern const char AN13[LEN];
 //功能图标 
 extern const char AIST[64];
 extern const char AITO[64];
 extern const char AIRT[64];
 extern const char AILK[64];
 extern const char AI04[64]; 
 extern const char AI05[64]; 
 extern const char AI06[64]; 
 extern const char AI07[64];
 extern const char AI08[64];
 extern const char AI09[64];
 extern const char AI10[64];
 extern const char AI11[64];
 extern const char AI12[64];
 extern const char AI13[64];
 //功能调用
 extern const char AF04[64];
 extern const char AF05[64];
 extern const char AF06[64];
 extern const char AF07[64]; 
 extern const char AF08[64];
 extern const char AF09[64];
 extern const char AF10[64];
 extern const char AF11[64];
 extern const char AF12[64];
 extern const char AF13[64];
//定义生日显示
 extern const int  NBIR_ENA;
 extern const uint NBIR_X;
 extern const uint NBIR_Y;
 extern const char NBIR_COLORS[4];
 extern const char NBIR_FCOLOR[4];
 extern const uint NBIR_FONT; 
 extern const int  OBIR_ENA;
 extern const uint OBIR_X;
 extern const uint OBIR_Y;
 extern const char OBIR_COLORS[4];
 extern const char OBIR_FCOLOR[4];
 extern const uint OBIR_FONT;
 extern const char BIR_FILE[64];
//显示文本
 extern const int  TEXT_ENA;
 extern const uint TEXT_X;
 extern const uint TEXT_Y;
 extern const char TEXT_COLORS[];
 extern const char TEXT_FCOLOR[];
 extern const uint TEXT_FONT;
 extern const char TEXT_FMT[LEN];
//显示温度
 extern const int  TEMP_ENA;
 extern const uint TEMP_X;
 extern const uint TEMP_Y;
 extern const char TEMP_COLORS[];
 extern const char TEMP_FCOLOR[];
 extern const uint TEMP_FONT;
 extern const char TEMP_FMT[16];
//显示电压
 extern const int  VOLT_ENA;
 extern const uint VOLT_X;
 extern const uint VOLT_Y;
 extern const char VOLT_COLORS[];
 extern const char VOLT_FCOLOR[];
 extern const uint VOLT_FONT;
 extern const char VOLT_FMT[16];
//显示内存
 extern const int  RAM_ENA;
 extern const uint RAM_X;
 extern const uint RAM_Y;
 extern const char RAM_COLORS[];
 extern const char RAM_FCOLOR[];
 extern const uint RAM_FONT;
//转换格式
 extern const char BYTES_FMT[16];
 extern const char BYTES_SG[8];
 extern const char KBYTES_SG[8];
//显示星期
 extern const int  WEEK_ENA;
 extern const uint WEEK_X;
 extern const uint WEEK_Y;
 extern const char WEEK_COLORS[];
 extern const char WEEK_FCOLOR[];
 extern const uint WEEK_FONT;
 extern const int  WEEK_FMT;
//显示日期
 extern const int  DATE_ENA;
 extern const uint DATE_X;
 extern const uint DATE_Y;
 extern const char DATE_COLORS[];
 extern const char DATE_FCOLOR[];
 extern const uint DATE_FONT;
 extern const int  DATE_FMT;
//显示时间
 extern const int  TIME_ENA;
 extern const uint TIME_X;
 extern const uint TIME_Y;
 extern const char TIME_COLORS[];
 extern const char TIME_FCOLOR[];
 extern const uint TIME_FONT;
 extern const int  TIME_FMT;
//显示农历年
 extern const int  CHSYear_ENA;
 extern const uint CHSYear_X;
 extern const uint CHSYear_Y;
 extern const char CHSYear_COLORS[];
 extern const char CHSYear_FCOLOR[];
 extern const uint CHSYear_FONT;
//显示农历月日
 extern const int  CHSDay_ENA;
 extern const uint CHSDay_X;
 extern const uint CHSDay_Y;
 extern const char CHSDay_COLORS[];
 extern const char CHSDay_FCOLOR[];
 extern const uint CHSDay_FONT;
//显示信号强度
 extern const int  NET_ENA;
 extern const uint NET_X;
 extern const uint NET_Y;
 extern const char NET_COLORS[];
 extern const char NET_FCOLOR[];
 extern const uint NET_FONT;
 extern const char NET_FMT[16];
//显示CPU使用率
 extern const int  CAP_ENA;
 extern const uint ACCU_X;
 extern const uint ACCU_Y;
 extern const char ACCU_COLORS[];
 extern const char ACCU_FCOLOR[];
 extern const uint ACCU_FONT;
 extern const char CAP_FMT[16];
//显示GPRS流量
 extern const int  GPRS_ENA;
 extern const uint GPRS_X;
 extern const uint GPRS_Y;
 extern const char GPRS_COLORS[];
 extern const char GPRS_FCOLOR[];
 extern const uint GPRS_FONT;
 //显示CPU使用率
 extern const int  CPU_ENA;
 extern const uint CPU_X;
 extern const uint CPU_Y;
 extern const char CPU_COLORS[];
 extern const char CPU_FCOLOR[];
 extern const uint CPU_FONT;
 extern const char CPU_FMT[16];
//刷新速度
 extern const uint REFRESH;

#endif
